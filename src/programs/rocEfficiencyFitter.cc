// Descriptions:
// Creates fitted functions from the output Ntuples of efficiencyTiming.cc

// Data structure
#include "../../interface/DataStructures_v5.h"

// Utility
#include "../../interface/TestsCommon.h"
#include "../../interface/TTreeTools.h"
#include "../../interface/HelperFunctionsCommon.h"
#include "../../interface/CanvasExtras.h"
#include "../../interface/TimerColored.h"
#include "../../interface/ProgressBar.h"
// #include "../../interface/common_functions_jkarancs.h"
#include "../../interface/EfficiencyPlotsModule.h"
#include "../../interface/WilsonScoreInterval.h"

// Root
#include <TROOT.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TGaxis.h>
#include <TPaletteAxis.h>
#include <TLatex.h>
#include <TMath.h>
#include <TF1.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TFrame.h>
#include <TPaveStats.h>
#include <TGraphAsymmErrors.h>
// #include <TRandom3.h>

// C++ libraries
#include <experimental/filesystem>
#include <future>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <exception>
#include <functional>
#include <algorithm>
#include <numeric>
#include <initializer_list>

#include "../../interface/json.hpp"
using JSON = nlohmann::json;

constexpr auto                    CONFIG_FILE_PATH                = "./config_main.json"; 
// const     std::vector<std::string> PLOT_NAME_SUFFICES       = { "AllDisks", "Layer1", "Layer2", "Layer3", "Layer4" };
// FIXME: Monkey patch, change this, when running on properly created histograms
const     std::vector<int>         NUM_BINS_ON_DISKS_LAYERS = { 16188, 2072, 4440, 6808, 9768 };
const     std::vector<std::string> PLOT_NAME_SUFFICES       = { "all disks", "layer 1", "layer 2", "layer 3", "layer 4" };
const     int   DELAY_PLOTS_NUM_BINS                             = 40; // WBC setting 164 is 0.
const     float DELAY_PLOTS_LOWER_EDGE                           = -10;
const     float DELAY_PLOTS_UPPER_EDGE                           =  30;

enum class FitStatus
{
    NotCalculated = 0,
    Approximated,
    MadePosDef,
    Accurate
};

enum class FitterErrorCodes
{
   HistogramMissing = -1000,
   HistogramHasNoEntries,
   HistogramHasLessThanThreeEntries,
   FitterAlgorithmTimedOut
};

std::vector<std::vector<TH1D*>> getROCefficiencyHistograms(TFile* inputFile);

int main(int argc, char** argv) try
{
   std::cout << process_prompt << argv[0] << " started..." << std::endl;
   std::time_t processStarted = std::time(nullptr);
   std::cout << timer_prompt << "Time: " << std::asctime(std::localtime(&processStarted)) << std::flush;
   std::cout << process_prompt << "Loading config file... ";
   JSON config = JSON::parse(fileToString(CONFIG_FILE_PATH));
   std::cout << "Done." << std::endl;
   TimerColored timer(timer_prompt);
   std::chrono::milliseconds milliseconds_100(100);
   // [[maybe_unused]] TFile* histogramsNtuple = generateOutputNtuple(config);
   gROOT -> SetBatch(kFALSE);
   gStyle -> SetOptFit(1);
   TFile* inputFile = TFile::Open("CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/histograms_fill5824_small_1_batch.root", "READ");

   // ---------------------------------------------------------------------------------------------
   // Fitting all the ROC efficiencies

   // The ROC-s need to be fitted from two sides (if this is possible)
   // with the proper sigmoid (single sided ramp) function.
   // To find the intervals for these two functions two different approach is 
   // used: calculating the zero point of the extrapolated discrete derivative
   // or fitting the curves with a gaussian function and determining its mean 
   // value.
   // 

   auto fitHistogram = [] (TH1* histogramToFit, std::chrono::duration<double> executionTimeLimit)
   {
      // Performing quick checks
      if(histogramToFit == nullptr)                   return FitterErrorCodes::HistogramMissing;
      if(efficiencyVsDelayOnROC -> GetEntries() == 0) return FitterErrorCodes::HistogramHasNoEntries;
      if(efficiencyVsDelayOnROC -> GetEntries()  < 3) return FitterErrorCodes::HistogramHasLessThanThreeEntries;
      // Method 1: finding the zero-derivative point by interpolation
      // Get histogram non-zero bins
      std::vector<int> nonZeroBins;
      std::vector<float> nonZeroBinValues;
      int numBins = efficiencyVsDelayOnROC -> GetSize();
      for(int binIndex = 0; binIndex < numBins; ++binIndex)
      {
         float value = efficiencyVsDelayOnROC -> GetAt(binIndex);
         if(value != 0.0f)
         {
            nonZeroBins.push_back(binIndex);
            nonZeroBinValues.push_back(value);
         }
      }
      if(nonZeroBinValues.size() == 0 || nonZeroBinValues.size() == 1)
      {
         std::cout << "ROC " << rocIndex << " on \"" << PLOT_NAME_SUFFICES[layerIndex] << 
            "\" seems to contain entries, but none, or only one of the bins are non-zero.\n" <<
            "Check underflow and overflow bins! Skipping ROC..." << std::endl;
         ++skippedROCsOnLayer;
         updateSkippedROCsMsg();
         continue;
      }

      // Get derivative values
      std::vector<float> derivatives(nonZeroBinValues.size());
      std::adjacent_difference(nonZeroBinValues.begin(), nonZeroBinValues.end(), derivatives.begin());
      derivatives.erase(derivatives.begin());

      // Get zero derivative points by interpolation
      std::vector<float> derivativeZeroPositions;
      const auto interpolateKeyByValues = [] (const float& goal, const float& lowerBound, const float& upperBound, const float& lowerKey, const float& upperKey)
      {
         float lowerBoundDifference = std::abs(goal - lowerBound);
         float upperBoundDifference = std::abs(goal - upperBound);
         return (lowerKey * lowerBoundDifference + upperKey * upperBoundDifference) / (lowerBoundDifference + upperBoundDifference);
      };
      for(size_t index: range(derivatives.size() - 1))
      {
         if(std::signbit(derivatives[index]) != std::signbit(derivatives[index + 1]))
         {
            // The derivative value at index is calculated for the interval 
            // between nonZeroBinValues[index] and nonZeroBinValues[index + 1]
            float lowerKey = 0.5 * (efficiencyVsDelayOnROC -> GetBinCenter(nonZeroBinValues[index]) + efficiencyVsDelayOnROC -> GetBinCenter(nonZeroBinValues[index + 1]));
            float upperKey = 0.5 * (efficiencyVsDelayOnROC -> GetBinCenter(nonZeroBinValues[index + 1]) + efficiencyVsDelayOnROC -> GetBinCenter(nonZeroBinValues[index + 2]));
            derivativeZeroPositions.push_back(interpolateKeyByValues(0, derivatives[index], derivatives[index + 1], lowerKey, upperKey));
         }
      }

      float intervalBoundary = std::accumulate(derivativeZeroPositions.begin(), derivativeZeroPositions.end(), 0);
   };

   // Reading in the saved ROC efficiency histograms from the input file
   std::cout << "Reading in the saved ROC efficiency histograms from the input file." << std::flush;
   std::future<std::vector<std::vector<TH1D*>>> fileReaderProcess = std::async(getROCefficiencyHistograms, inputFile);
   while(fileReaderProcess.wait_for(milliseconds_100) == std::future_status::timeout)
   {
      std::cout << '.' << std::flush;
   }
   std::vector<std::vector<TH1D*>> efficiencyVsDelayHistogramsToFit = fileReaderProcess.get();
   std::cout << "Done." << std::endl;

   for(size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      const std::vector<TH1D*> efficiencyVsDelayOnROCLayerHistograms = efficiencyVsDelayHistogramsToFit[layerIndex];
      const int numROCsOnLayer = efficiencyVsDelayOnROCLayerHistograms.size();
      int skippedROCsOnLayer   = 0;
      std::string skippedROCsMsg("");
      auto updateSkippedROCsMsg = [&] 
      {
         std::cout << std::string(skippedROCsMsg.size(), '\b') << std::string(skippedROCsMsg.size(), ' ') << std::string(skippedROCsMsg.size(), '\b');
         skippedROCsMsg = "";
         skippedROCsMsg += "Skipped ROCs: ";
         skippedROCsMsg += std::to_string(skippedROCsOnLayer);
         skippedROCsMsg += "/";
         skippedROCsMsg += std::to_string(numROCsOnLayer);
         std::cout << skippedROCsMsg << std::flush;
      };
      for(const size_t rocIndex: range(numROCsOnLayer))
      {
         
         TH1D* efficiencyVsDelayOnROC = efficiencyVsDelayOnROCLayerHistograms[rocIndex];

         auto getFit = [&] (TH1D* histogram, const float& lowerBound, const float& upperBound) -> TF1*
         {
            if(*(derivatives.begin()) < *(derivatives.end()))
            {
               while(1)
               {
                  TF1* parametrizedSigmoid = new TF1("parametrizedSigmoid", "[0] / (1 + exp(([1] - x) / [2]))", 0, intervalBoundary);
                  parametrizedSigmoid -> SetParLimits(1, 0, intervalBoundary);
                  parametrizedSigmoid -> SetParameter(0, 9.95782e-01);
                  parametrizedSigmoid -> SetParameter(1, 4.05233e+03);
                  parametrizedSigmoid -> SetParameter(2, 1.22612e+01);
                  efficiencyVsDelayOnROC -> Fit("parametrizedSigmoid", "M");
               }
            }
            else
            {
               while(1)
               {
                  TF1* parametrizedSigmoid = new TF1("parametrizedSigmoid", ("[0] / (1 + exp(([1] - (" + std::to_string(intervalBoundary) + " - x)) / [2]))").c_str(), 0, intervalBoundary);
                  parametrizedSigmoid -> SetParLimits(1, 0, intervalBoundary);
                  parametrizedSigmoid -> SetParameter(0, 9.95782e-01);
                  parametrizedSigmoid -> SetParameter(1, 4.05233e+03);
                  parametrizedSigmoid -> SetParameter(2, 1.22612e+01);
                  efficiencyVsDelayOnROC -> Fit("parametrizedSigmoid", "M");
               }
            }
         };

         // Calculate the actual fit
         if(derivativeZeroPositions.size() != 0)
         {
            getFit(efficiencyVsDelayOnROC, 0, intervalBoundary);
         }
         else
         {

         }
         // canvas -> SaveAs(std::string("ROC_fits/" + std::to_string(layerIndex) + "_" + std::to_string(rocIndex) + ".eps").c_str());

         // Method 2: fitting a gaussian curve to the distribution of the measurement points
      }
      std::cout << std::endl;
   }

   std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
   return 0;
}
catch(const std::exception& e)
{
   std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
   return -1;
}

// Read in ROC efficiencies from a file

std::vector<std::vector<TH1D*>> getROCefficiencyHistograms(TFile* inputFile) try
{
   std::vector<std::vector<TH1D*>> rocEfficiencyHistograms;
   if(!(inputFile -> IsOpen())) throw std::runtime_error("input file bad or missing.");
   inputFile -> cd("ROCEfficiencies");
   for(size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      inputFile  -> cd("ROCEfficiencies");
      std::string layerName = layerIndex == 0 ? "FPix" : "Layer_" + std::to_string(layerIndex); 
      gDirectory -> cd(layerName.c_str());
      size_t numBins = NUM_BINS_ON_DISKS_LAYERS[layerIndex];
      std::vector<TH1D*> rocEfficienciesOnLayer;
      rocEfficienciesOnLayer.reserve(numBins);
      // Trying to get histograms by names
      for(size_t binIndex = 0; binIndex < numBins; ++binIndex)
      {
         std::string histogramName  = "delayVsEfficiencyOnROCs" + PLOT_NAME_SUFFICES[layerIndex] + "ROC" + std::to_string(binIndex + 1);
         TH1D* histogramPointer = dynamic_cast<TH1D*>(gDirectory -> Get(histogramName.c_str()));
         // // This should be unnecesarry now... 1
         // if(histogramPointer == nullptr)
         // {
         //    std::cout << "Missing plot: " << std::endl;
         //    std::cout << "binIndex: " << binIndex << std::endl;
         //    std::cout << "histogramName: " << histogramName << std::endl;
         //    std::cin.get();
         //    std::cout << "List of possible keys in the current directory: " << std::endl;
         //    for(const auto& key: *(gDirectory -> GetListOfKeys()))
         //    {
         //       std::cout << "Name: "   << key -> GetName();
         //       std::cout << " Title: " << key -> GetTitle() << std::endl;
         //    }
         //    std::cout << "histogramName: " << histogramName << std::endl;
         //    std::cin.get();
         // }
         rocEfficienciesOnLayer.emplace_back(histogramPointer);
      }
      rocEfficiencyHistograms.emplace_back(std::move(rocEfficienciesOnLayer));
   }
   return rocEfficiencyHistograms;
}
catch(const std::exception& e)
{
   std::cout << error_prompt << e.what() << std::endl;
   return {};
}
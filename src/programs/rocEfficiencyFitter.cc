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
#include <TRandom3.h>
#include <TFitResult.h>

// C++ libraries
#include <experimental/filesystem>
#include <future>
#include <iostream>
#include <regex>
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

TRandom3 rndGen;

constexpr auto                    CONFIG_FILE_PATH                = "./config_main.json"; 
// FIXME: Monkey patch, change this, when running on properly created histograms
// const     std::vector<std::string> PLOT_NAME_SUFFICES       = { "all disks", "layer 1", "layer 2", "layer 3", "layer 4" };
const     std::vector<int>         NUM_BINS_ON_DISKS_LAYERS = { 16188, 2072, 4440, 6808, 9768 };
const     std::vector<std::string> PLOT_NAME_SUFFICES       = { "AllDisks", "Layer1", "Layer2", "Layer3", "Layer4" };
const     int   DELAY_PLOTS_NUM_BINS                             = 40; // WBC setting 164 is 0.
const     float DELAY_PLOTS_LOWER_EDGE                           = -10;
const     float DELAY_PLOTS_UPPER_EDGE                           =  30;

std::vector<std::pair<double, double>> PAR_LIMITS_ASCENDING
{
   { 0.8, 1.2  },
   { -10, 30   },
   { 0.01, 100 }
};

std::vector<std::pair<double, double>> PAR_LIMITS_DESCENDING
{
   { 0.8, 1.2  },
   { -30, 10   },
   { 0.01, 100 }
};

struct ROCPlacement
{
   int side = NOVAL_I;
   int half = NOVAL_I;
   int sec  = NOVAL_I;
   int disk = NOVAL_I;
   int ring = NOVAL_I;
};

std::vector<std::vector<TH1F*>> getROCefficiencyHistograms(TFile* t_inputFile);
std::vector<std::map<int, ROCPlacement>> getLayersROCIndexToPlacementMaps(std::string t_filename);

class HistogramFitter
{
   using clock_t = std::chrono::steady_clock;
   public:
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
         HistogramHasNotEnoughEntriesInRange,
         FitterAlgorithmTimedOut,
         UnhandledError
      };
      // static TF1* ascendingSigmoidTemplate;
      // static TF1* descendingSigmoidTemplate;
   private:
      TH1F* histogramToFit;
      std::chrono::duration<double> secondsToTimeout;
      TCanvas* canvas;
      // The actual fitting process
   public: 
      HistogramFitter(TH1F* histogramArg, std::chrono::duration<double> secondsToTimeoutArg, TCanvas* canvasArg) : histogramToFit(histogramArg), secondsToTimeout(secondsToTimeoutArg), canvas(canvasArg) {};
      int operator()();
      TCanvas* getCanvas() { return canvas; };
      TH1F* getHistogram() { return histogramToFit; };
};

// TF1* HistogramFitter::ascendingSigmoidTemplate  = new TF1("ascendingSigmoidTemplate",  "[0] / ((1 + exp([1] - x) / [2]))");
// TF1* HistogramFitter::descendingSigmoidTemplate = new TF1("descendingSigmoidTemplate", "[0] / ((1 + exp([1] + x) / [2]))");

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
   TApplication* app = new TApplication("The application", &argc, argv);
   // [[maybe_unused]] TFile* histogramsNtuple = generateOutputNtuple(config);
   // gROOT -> SetBatch(kFALSE);
   gROOT -> SetBatch(kTRUE);
   gStyle -> SetOptFit(1);
   [[maybe_unused]] TFile* inputFile = TFile::Open("/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/histograms_fill5824_4.root", "READ");
   [[maybe_unused]] TFile* outputFile = TFile::Open("/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/ROC_fits_fill5824_updated.root", "RECREATE");

   // inputFile -> cd("Efficiency_vs_delay");
   // TH1F* bpix = dynamic_cast<TH1F*>(gDirectory -> Get("delayVsEfficiencyBPix"));

   // ---------------------------------------------------------------------------------------------
   // Reading in the saved ROC efficiency histograms from the input file

   std::cout << "Reading in the saved ROC efficiency histograms from the input file." << std::flush;
   std::future<std::vector<std::vector<TH1F*>>> fileReaderProcess = std::async(getROCefficiencyHistograms, inputFile);
   while(fileReaderProcess.wait_for(milliseconds_100) == std::future_status::timeout)
   {
      std::cout << '.' << std::flush;
   }
   std::vector<std::vector<TH1F*>> efficiencyVsDelayHistogramsToFit = fileReaderProcess.get();
   std::cout << "Done." << std::endl;
   
   inputFile -> Close();

   // ---------------------------------------------------------------------------------------------
   // Fitting all the ROC efficiencies

   // The ROC-s need to be fitted from two sides (if this is possible)
   // with the proper sigmoid (single sided ramp) function.
   // To find the intervals for these two functions two different approach is 
   // used: calculating the zero point of the extrapolated discrete derivative
   // or fitting the curves with a gaussian function and determining its mean 
   // value.
   // 

   // ---------------------------------------------------------------------------------------------
   // Generating lightweight fitter objects for each of the histograms

   std::cout << "Generating fitter objects... " << std::flush;
   std::vector<std::vector<HistogramFitter>> fitterObjectList(efficiencyVsDelayHistogramsToFit.size());

   for(std::size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      const std::vector<TH1F*> efficiencyVsDelayOnROCLayerHistograms = efficiencyVsDelayHistogramsToFit[layerIndex];
      for(std::size_t rocIndex = 0, maxROCIndex = efficiencyVsDelayOnROCLayerHistograms.size(); rocIndex < maxROCIndex; ++rocIndex)
      {
         TH1F* efficiencyVsDelayOnROC = efficiencyVsDelayOnROCLayerHistograms[rocIndex];
         // Skipping bins with not enough bins
         int nonZeroBinValues = std::count_if(efficiencyVsDelayOnROC -> GetArray(), efficiencyVsDelayOnROC -> GetArray() + efficiencyVsDelayOnROC -> GetSize(), [] (const auto& e) { return e != 0; });
         if(nonZeroBinValues < 3) continue;
         fitterObjectList[layerIndex].emplace_back(efficiencyVsDelayOnROC, std::chrono::duration<double>(3.50), new TCanvas());
         std::string canvasName = "layer" + std::to_string(layerIndex) + "_ROC" + std::to_string(rocIndex); 
         fitterObjectList[layerIndex].back().getCanvas() -> SetName(canvasName.c_str());
      }
   }
   std::cout << "Done." << std::endl;

   // ---------------------------------------------------------------------------------------------
   // Getting the results of the fits

   // // Async helpers
   // enum class ThreadState
   // {
   //     Running,
   //     Waiting
   // };
   // std::vector<std::pair<std::future<TCanvas*>, std::unique_ptr<ThreadState>>> futureStore;
   // auto isStoreElementWaiting = [] (const std::pair<std::future<TCanvas*>, std::unique_ptr<ThreadState>>& futureStoreElement) { return *(futureStoreElement.second) == ThreadState::Waiting; };
   // const int maxNumParallelThreads = 10;
   
   // Setting up directory naming conventions
   const std::vector<std::string> outputMainDirectoryNames { "FPix", "Layer_1", "Layer_2", "Layer_3", "Layer_4" };
   const std::vector<std::string> sideNames                { "Negative_side", "Positive_side" };
   const std::vector<std::string> halfshellNames           { "Negative_halfshell", "Positive_halfshell" };
   const std::vector<std::string> sectorNames              { "Sector_1", "Sector_2", "Sector_3", "Sector_4", "Sector_5", "Sector_6", "Sector_7", "Sector_8", };
   const std::vector<std::string> diskNames                { "Disk_1", "Disk_2", "Disk_3"};
   const std::vector<std::string> ringNames                { "Inner_ring", "Outer_ring" };
   // Creating directories
   for(std::size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      const char* layerDirName = outputMainDirectoryNames[layerIndex].c_str();
      outputFile -> mkdir(layerDirName);
      if(layerIndex == 0) // Forward region
      {
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            outputFile -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t diskIndex = 0, diskMaxIndex = diskNames.size(); diskIndex < diskMaxIndex; ++diskIndex)
            {
               const char* diskDirName = diskNames[diskIndex].c_str();
               outputFile -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(diskDirName);
               gDirectory -> cd(diskDirName);
               for(std::size_t ringIndex = 0, ringMaxIndex = ringNames.size(); ringIndex < ringMaxIndex; ++ringIndex)
               {
                  const char* ringDirName = ringNames[ringIndex].c_str();
                  outputFile -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(diskDirName);
                  gDirectory -> mkdir(ringDirName);
                  gDirectory -> cd(ringDirName);
               }
            }
         }
      }
      else // Barrel region
      {
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            outputFile -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t halfshellIndex = 0, maxHalfshellIndex = halfshellNames.size(); halfshellIndex < maxHalfshellIndex; ++halfshellIndex)
            {
               const char* halfshellDirName = halfshellNames[halfshellIndex].c_str();
               outputFile -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(halfshellDirName);
               gDirectory -> cd(halfshellDirName);
               for(std::size_t sectorIndex = 0, sectorMaxIndex = sectorNames.size(); sectorIndex < sectorMaxIndex; ++sectorIndex)
               {
                  const char* sectorDirName = sectorNames[sectorIndex].c_str();
                  outputFile -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(halfshellDirName);
                  gDirectory -> mkdir(sectorDirName);
                  gDirectory -> cd(sectorDirName);
               }
            }
         }
      }
   }
   // Getting directory maps
   // std::vector<std::map<int, ROCPlacement>> layersROCIndexToPlacementMaps = getLayersROCIndexToPlacementMaps("ROCPlacement.txt");
   const std::vector<std::map<int, ROCPlacement>> layersROCIndexToPlacementMaps = getLayersROCIndexToPlacementMaps("ROCPlacement_test.txt");

   std::cout << "Fitting histograms... " << std::flush; 
   for(std::size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      int fitROCsOnLayer = 0;
      int skippedROCsOnLayer = 0;
      std::string skippedROCsMsg("");
      auto updateSkippedROCsMsg = [&] 
      {
         std::cout << std::string(skippedROCsMsg.size(), '\b') << std::string(skippedROCsMsg.size(), ' ') << std::string(skippedROCsMsg.size(), '\b');
         skippedROCsMsg = "";
         skippedROCsMsg += "Layer: ";
         skippedROCsMsg += std::to_string(layerIndex);
         skippedROCsMsg += " Skipped ROCs: ";
         skippedROCsMsg += std::to_string(skippedROCsOnLayer);
         skippedROCsMsg += " / ";
         skippedROCsMsg += std::to_string(fitROCsOnLayer);
         skippedROCsMsg += " / ";
         skippedROCsMsg += std::to_string(fitterObjectList[layerIndex].size());
         skippedROCsMsg += " ";
         std::cout << skippedROCsMsg << std::flush;
      };     
      // int threadsTerminated           = 0;
      // std::size_t nextROCIndex = 0;
      // while(threadsTerminated < fitterObjectList[layerIndex].size())
      // {
      //    while(futureStore.size() < maxNumParallelThreads && nextROCIndex < fitterObjectList[layerIndex].size())
      //    {
      //       auto state = std::make_unique<ThreadState>(ThreadState::Running);
      //       auto threadPair = std::make_pair(std::async([statePtr = state.get(), index = nextROCIndex, &fitterObjectList, &layerIndex] ()
      //       {
      //          fitterObjectList[layerIndex][index]();
      //          *statePtr = ThreadState::Waiting;
      //          return fitterObjectList[layerIndex][index].getCanvas();
      //       }), std::move(state));
      //       std::cin.get();
      //       nextROCIndex++;
      //       futureStore.emplace_back(std::move(threadPair));
      //    }
      //    auto find_res = std::find_if(futureStore.begin(), futureStore.end(), isStoreElementWaiting);
      //    if(find_res == futureStore.end())
      //    {
      //        continue;
      //    }
      //    outputFile -> cd();
      //    find_res -> first.get() -> Write();
      //    std::cout << "." << std::flush;
      //    futureStore.erase(find_res);
      //    threadsTerminated++;
      // }
      for(auto& fitterObject: fitterObjectList[layerIndex])
      {
         using FitterErrorCodes = HistogramFitter::FitterErrorCodes;
         FitterErrorCodes fitResult = FitterErrorCodes(fitterObject());
         switch(fitResult)
         {
            case FitterErrorCodes::HistogramMissing:
               skippedROCsOnLayer++;
               // std::cout << "HistogramMissing" << std::endl;
               break;
            case FitterErrorCodes::HistogramHasNoEntries:
               skippedROCsOnLayer++;
               // std::cout << "HistogramHasNoEntries" << std::endl;
               break;
            case FitterErrorCodes::HistogramHasLessThanThreeEntries:
               skippedROCsOnLayer++;
               // std::cout << "HistogramHasLessThanThreeEntries" << std::endl;
               break;
            case FitterErrorCodes::HistogramHasNotEnoughEntriesInRange:
               skippedROCsOnLayer++;
               // std::cout << "HistogramHasNotEnoughEntriesInRange" << std::endl;
               break;
            case FitterErrorCodes::FitterAlgorithmTimedOut:
               skippedROCsOnLayer++;
               // std::cout << "FitterAlgorithmTimedOut" << std::endl;
               break;
            case FitterErrorCodes::UnhandledError:
               skippedROCsOnLayer++;
               break;
            default:
               fitROCsOnLayer++;
               outputFile -> cd(outputMainDirectoryNames[layerIndex].c_str());
               std::cmatch match;
               std::regex_match(fitterObject.getHistogram() -> GetName(), match, std::regex(R"(.*ROC ?(\d+))"));
               int rocIndex = std::stoi(match[1].str());
               const auto& findRes = layersROCIndexToPlacementMaps[layerIndex].find(rocIndex);
               if(findRes != layersROCIndexToPlacementMaps[layerIndex].end())
               {
                  auto rocPlacement = findRes -> second;
                  if(layerIndex == 0) // Forward region
                  {
                     int side = rocPlacement.side;
                     int disk = rocPlacement.disk;
                     int ring = rocPlacement.ring;
                     gDirectory -> cd(sideNames[side - 1].c_str());
                     gDirectory -> cd(diskNames[std::abs(disk) - 1].c_str());
                     gDirectory -> cd(ringNames[ring - 1].c_str());
                  }
                  else // Barrel region
                  {
                     int side = rocPlacement.side;
                     int half = rocPlacement.half;
                     int sec  = rocPlacement.sec;
                     gDirectory -> cd(sideNames[side - 1].c_str());
                     gDirectory -> cd(halfshellNames[half].c_str());
                     gDirectory -> cd(sectorNames[sec - 1].c_str());
                  }
               }
               fitterObject.getCanvas() -> Write();
               break;
         }
         updateSkippedROCsMsg();
      }
      std::cout << std::endl;
   }
   std::cout << "Done." << std::endl;
   outputFile -> Close();
   std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
   app -> Run();
   return 0;
}
catch(const std::exception& e)
{
   std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
   return -1;
}

// ------------------------------------------------------------------------------------------------
// Read in ROC efficiencies from a file

std::vector<std::vector<TH1F*>> getROCefficiencyHistograms(TFile* t_inputFile) try
{
   std::vector<std::vector<TH1F*>> rocEfficiencyHistograms;
   if(!(t_inputFile -> IsOpen())) throw std::runtime_error("input file bad or missing.");
   t_inputFile -> cd("ROCEfficiencies");
   for(std::size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      t_inputFile  -> cd("ROCEfficiencies");
      std::string layerName = layerIndex == 0 ? "FPix" : "Layer_" + std::to_string(layerIndex); 
      gDirectory -> cd(layerName.c_str());
      std::size_t numBins = NUM_BINS_ON_DISKS_LAYERS[layerIndex];
      std::vector<TH1F*> rocEfficienciesOnLayer;
      rocEfficienciesOnLayer.reserve(numBins);
      // Trying to get histograms by names
      for(std::size_t binIndex = 0; binIndex < numBins; ++binIndex)
      {
         std::string histogramName  = "delayVsEfficiencyOnROCs" + PLOT_NAME_SUFFICES[layerIndex] + "ROC" + std::to_string(binIndex + 1);
         TH1F* histogramPointer = dynamic_cast<TH1F*>(gDirectory -> Get(histogramName.c_str()));
         if(histogramPointer == nullptr)
         {
            std::cout << "Missing plot: binIndex: " << binIndex << "histogramName: " << histogramName << std::endl;
         }
         histogramPointer -> SetDirectory(0);
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

// ------------------------------------------------------------------------------------------------
// Read in maps containing information about the placement of the ROCs 

// The returned vector should contain 5 maps, each of them mapping ROCindex values to ROCPlacement objects
// The expected format is lines containing:
// {{ layer, rocIndex }, { side, half, sec, disk, ring }},

std::vector<std::map<int, ROCPlacement>> getLayersROCIndexToPlacementMaps(std::string t_filename)
{
   std::vector<std::map<int, ROCPlacement>> layersROCIndexToPlacementMaps(5);
   std::ifstream inputStream;
   std::regex regex(R"(\{\{ (-?\d+)\, (-?\d+) \}\, \{ (-?\d+)\, (-?\d+)\, (-?\d+)\, (-?\d+)\, (-?\d+) \}\}\,)");
   inputStream.open(t_filename);
   std::string line;
   while(std::getline(inputStream, line))
   {
      std::cmatch match;
      std::regex_match(line.c_str(), match, regex);
      int layer    = std::stoi(match[1].str());
      int rocIndex = std::stoi(match[2].str());
      ROCPlacement rocPlacement { std::stoi(match[3].str()), std::stoi(match[4].str()), std::stoi(match[5].str()), std::stoi(match[6].str()), std::stoi(match[7].str()) };
      layersROCIndexToPlacementMaps[layer].insert(std::make_pair(rocIndex, rocPlacement));
   }
   return layersROCIndexToPlacementMaps;
}

// ------------------------------------------------------------------------------------------------
// Histogram fitter

int HistogramFitter::operator()() try
{
   // Performing quick checks
   if(histogramToFit == nullptr)           return static_cast<int>(FitterErrorCodes::HistogramMissing);
   if(histogramToFit -> GetEntries() == 0) return static_cast<int>(FitterErrorCodes::HistogramHasNoEntries);
   if(histogramToFit -> GetEntries()  < 3) return static_cast<int>(FitterErrorCodes::HistogramHasLessThanThreeEntries);
   // Method 1: finding the zero-derivative point by interpolation
   // Get histogram non-zero bins
   std::vector<int> nonZeroBins;
   std::vector<float> nonZeroBinValues;
   int numBins = histogramToFit -> GetSize();
   for(int binIndex = 1; binIndex < numBins - 1; ++binIndex)
   {
      float value = histogramToFit -> GetAt(binIndex);
      if(value != 0.0f)
      {
         nonZeroBins.push_back(binIndex);
         nonZeroBinValues.push_back(value);
      }
   }
   if(nonZeroBinValues.size() == 0 || nonZeroBinValues.size() == 1)
   {
      std::cout << "Less than two non-zero bins found. Check underflow and overflow bins!" << std::endl;
      return static_cast<int>(FitterErrorCodes::HistogramHasNotEnoughEntriesInRange);
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
   for(std::size_t index: range(derivatives.size() - 1))
   {
      if(std::signbit(derivatives[index]) != std::signbit(derivatives[index + 1]))
      {
         // The derivative value at index is calculated for the interval 
         // between nonZeroBinValues[index] and nonZeroBinValues[index + 1]
         float lowerKey = 0.5 * (histogramToFit -> GetBinCenter(nonZeroBins[index]) + histogramToFit -> GetBinCenter(nonZeroBins[index + 1]));
         float upperKey = 0.5 * (histogramToFit -> GetBinCenter(nonZeroBins[index + 1]) + histogramToFit -> GetBinCenter(nonZeroBins[index + 2]));
         derivativeZeroPositions.push_back(interpolateKeyByValues(0, derivatives[index], derivatives[index + 1], lowerKey, upperKey));
      }
   }

   float intervalBoundary = std::accumulate(derivativeZeroPositions.begin(), derivativeZeroPositions.end(), 0.0f) / derivativeZeroPositions.size();
   if(derivativeZeroPositions.size() == 0) intervalBoundary = std::signbit(derivatives[0]) ? -10.0f : 30.0f;
   // Basic properties
   // TF1* ascendingSigmoid  = static_cast<TF1*>(ascendingSigmoidTemplate  -> Clone("ascendingSigmoid"));
   // TF1* descendingSigmoid = s6tatic_cast<TF1*>(descendingSigmoidTemplate -> Clone("descendingSigmoid"));
   static auto generateAscendingSigmoidName  = [i = 0] () mutable { return "ascendingSigmoid_"  + std::to_string(i++); };
   static auto generateDescendingSigmoidName = [i = 0] () mutable { return "descendingSigmoid_" + std::to_string(i++); };
   TF1* ascendingSigmoid  = new TF1(generateAscendingSigmoidName().c_str(),  "[0] / ((1 + exp([1] - x) / [2]))", -10.0f,           intervalBoundary);
   TF1* descendingSigmoid = new TF1(generateDescendingSigmoidName().c_str(), "[0] / ((1 + exp([1] + x) / [2]))", intervalBoundary, 30.0f           );
   ascendingSigmoid  -> SetParameter(0, 0.5 * (PAR_LIMITS_ASCENDING[0].first + PAR_LIMITS_ASCENDING[0].second));
   descendingSigmoid -> SetParameter(0, 0.5 * (PAR_LIMITS_DESCENDING[0].first + PAR_LIMITS_DESCENDING[0].second));
   ascendingSigmoid  -> SetParameter(1, 0.5 * (PAR_LIMITS_ASCENDING[1].first + PAR_LIMITS_ASCENDING[1].second));
   descendingSigmoid -> SetParameter(1, 0.5 * (PAR_LIMITS_DESCENDING[1].first + PAR_LIMITS_DESCENDING[1].second));
   ascendingSigmoid  -> SetParameter(2, 0.5 * (PAR_LIMITS_ASCENDING[2].first + PAR_LIMITS_ASCENDING[2].second));
   descendingSigmoid -> SetParameter(2, 0.5 * (PAR_LIMITS_DESCENDING[2].first + PAR_LIMITS_DESCENDING[2].second));
   ascendingSigmoid  -> SetParLimits(0, PAR_LIMITS_ASCENDING[0].first, PAR_LIMITS_ASCENDING[0].second);
   descendingSigmoid -> SetParLimits(0, PAR_LIMITS_DESCENDING[0].first, PAR_LIMITS_DESCENDING[0].second);
   ascendingSigmoid  -> SetParLimits(1, PAR_LIMITS_ASCENDING[1].first, PAR_LIMITS_ASCENDING[1].second);
   descendingSigmoid -> SetParLimits(1, PAR_LIMITS_DESCENDING[1].first, PAR_LIMITS_DESCENDING[1].second);
   ascendingSigmoid  -> SetParLimits(2, PAR_LIMITS_ASCENDING[2].first, PAR_LIMITS_ASCENDING[2].second);
   descendingSigmoid -> SetParLimits(2, PAR_LIMITS_DESCENDING[2].first, PAR_LIMITS_DESCENDING[2].second);
   // ascendingSigmoid -> SetRange(-10.0f, intervalBoundary);
   // descendingSigmoid -> SetRange(intervalBoundary, 30.0f);
   ascendingSigmoid -> SetLineColor(kRed);
   descendingSigmoid -> SetLineColor(kBlue);
   auto adjustFit = [] (TH1F* t_histogram, TF1* t_function, const float t_lowerBound, const float t_upperBound, const std::vector<std::pair<double, double>>& t_parLimits, clock_t::time_point& t_startTime, std::chrono::duration<double>& t_secondsToTimeout)
   {
      int valuesInRange = 0;
      for(std::size_t binIndex = 1, maxBinIndex = t_histogram -> GetNbinsX(); binIndex <= maxBinIndex; ++binIndex)
      {
         float binCenter = t_histogram -> GetBinCenter(binIndex);
         if(t_lowerBound <= binCenter && binCenter < t_upperBound && t_histogram -> GetBinContent(binIndex) != 0) ++valuesInRange;
      }
      if(valuesInRange == 0) return static_cast<int>(FitterErrorCodes::HistogramHasNoEntries);
      while(1)
      {
         if(clock_t::now() - t_startTime > t_secondsToTimeout)
         {
            return static_cast<int>(FitterErrorCodes::FitterAlgorithmTimedOut);
         }
         t_function -> SetParameter(0, t_parLimits[0].first + rndGen.Rndm() * (t_parLimits[0].second - t_parLimits[0].first));
         t_function -> SetParameter(1, t_parLimits[1].first + rndGen.Rndm() * (t_parLimits[1].second - t_parLimits[1].first));
         t_function -> SetParameter(2, t_parLimits[2].first + rndGen.Rndm() * (t_parLimits[2].second - t_parLimits[2].first));
         TFitResultPtr fitResults = t_histogram -> Fit(t_function -> GetName(), "QNRMS", "", t_lowerBound, t_upperBound);
         if(!fitResults -> IsValid()) continue;
         FitStatus status = FitStatus(fitResults -> CovMatrixStatus());
         // if(status == FitStatus::MadePosDef) std::cout << "Cov. matrix is pos. def. but inaccurate." << std::endl;
         if(status == FitStatus::Accurate)
         {
            break;
         }
      }
      return 0;
   };
   clock_t::time_point startTime = clock_t::now();
   if(-10.0f < intervalBoundary)
   {
      int executionStatus = adjustFit(histogramToFit, ascendingSigmoid, -10.0f, intervalBoundary, PAR_LIMITS_ASCENDING, startTime, secondsToTimeout);
      if(executionStatus != 0)
      {
         return executionStatus;
      }
   }
   if(intervalBoundary < 30.0f)
   {
      int executionStatus = adjustFit(histogramToFit, descendingSigmoid, intervalBoundary, 30.0f, PAR_LIMITS_DESCENDING, startTime, secondsToTimeout);
      if(executionStatus != 0)
      {
         return executionStatus;
      }
   }
   canvas -> cd();
   histogramToFit -> Draw();
   ascendingSigmoid -> Draw("SAME");
   descendingSigmoid -> Draw("SAME");
   return 0;
}
catch(const std::exception& e)
{
   std::cout << e.what() << std::endl;
   return static_cast<int>(FitterErrorCodes::UnhandledError);
};

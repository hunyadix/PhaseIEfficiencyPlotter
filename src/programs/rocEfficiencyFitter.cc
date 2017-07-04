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
// #include "../../interface/WilsonScoreInterval.h"
#include "../../interface/StylePresets.h"

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

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

TRandom3 rndGen;

constexpr auto CONFIG_FILE_PATH       = "./config_main.json"; 
constexpr auto INPUT_FILE_PATH        = "/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/histograms_fill5824_4.root";
constexpr auto OUTPUT_FILE_PATH       = "/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/ROC_fits_fill5824_updated_detailed_2.root";
constexpr auto ROC_PLACEMENT_MAP_PATH = "/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/ROCPlacement_extra.txt";
// constexpr auto


// FIXME: Monkey patch, change this, when running on properly created histograms
// const     std::vector<std::string> PLOT_NAME_SUFFICES       = { "all disks", "layer 1", "layer 2", "layer 3", "layer 4" };
const     std::vector<int>         NUM_BINS_ON_DISKS_LAYERS = { 16188, 2072, 4440, 6808, 9768 };
const     std::vector<std::string> PLOT_NAME_SUFFICES       = { "AllDisks", "Layer1", "Layer2", "Layer3", "Layer4" };
const     int   DELAY_PLOTS_NUM_BINS                        = 40; // WBC setting 164 is 0.
const     float DELAY_PLOTS_LOWER_EDGE                      = -10;
const     float DELAY_PLOTS_UPPER_EDGE                      =  30;

// Directory naming conventions
const std::vector<std::string> outputMainDirectoryNames { "FPix", "Layer_1", "Layer_2", "Layer_3", "Layer_4" };
const std::vector<std::string> sideNames                { "Negative_side", "Positive_side" };
const std::vector<std::string> halfshellNames           { "Negative_halfshell", "Positive_halfshell" };
const std::vector<std::string> sectorNames              { "Sector_1", "Sector_2", "Sector_3", "Sector_4", "Sector_5", "Sector_6", "Sector_7", "Sector_8", };
const std::vector<std::string> diskNames                { "Disk_1", "Disk_2", "Disk_3"};
const std::vector<std::string> ringNames                { "Inner_ring", "Outer_ring" };
const std::vector<std::string> bladeNames               { []
{
   std::vector<std::string> bladeNames;
   for(int bladeIndex = -17, maxBladeIndex = 18; bladeIndex < maxBladeIndex; ++bladeIndex)
   {
      if(bladeIndex == 0) continue;
      bladeNames.emplace_back("Blade_" + std::to_string(bladeIndex));
   }
   return bladeNames;
}() };
const std::vector<std::string> panelNames               { "Panel_1", "Panel_2" };

// ------------------------------------------------------------------------------------------------
// ROC efficiencies compared to the detector part means

// ROC efficiency differences compared to the mean of  the containing
//    - module       -o                                                                           |
//    - sector         \                                                                          |
//    - side            o---- Barrel                                                              |
//    - halfshell      /                                                                          |
//    - quarter      -o                                                                           |
//    - blade-panel     -o                                                                        |
//    - blade             \                                                                       |
//    - disk               o---- Forward                                                          |
//    - disk-ring         /                                                                       |
//    - side            -o                                                                        |
//

std::vector<std::string> generateModuleComparisonDirNames(int moduleMin, int moduleMax, int ladderMin, int ladderMax)
{
   std::vector<std::string> layerModuleComparisonDirNames;
   for(int moduleIndex = moduleMin; moduleIndex <= moduleMax; ++moduleIndex)
   {
      if(moduleIndex == 0) continue;
      for(int ladderIndex = ladderMin; ladderIndex < ladderMax; ++ladderIndex)
      {
         if(ladderIndex == 0) continue;
         layerModuleComparisonDirNames.push_back("Module_[" + std::to_string(moduleIndex) + "]_ladder_[" + std::to_string(ladderIndex) + "]");
      }
   }
   return layerModuleComparisonDirNames;
}

const std::vector<int> LAYER_HALF_WIDTH_IN_MODULES { 0, 6, 14, 22, 32 };
const std::vector<std::vector<std::string>> rocModuleComparisonDirNamesOnLayers
{
   generateModuleComparisonDirNames(0, 0, 0, 0),   // Fwd
   generateModuleComparisonDirNames(-4, 4, -LAYER_HALF_WIDTH_IN_MODULES[1], LAYER_HALF_WIDTH_IN_MODULES[1]), // Layer 1
   generateModuleComparisonDirNames(-4, 4, -LAYER_HALF_WIDTH_IN_MODULES[2], LAYER_HALF_WIDTH_IN_MODULES[2]), // Layer 2
   generateModuleComparisonDirNames(-4, 4, -LAYER_HALF_WIDTH_IN_MODULES[3], LAYER_HALF_WIDTH_IN_MODULES[3]), // Layer 3
   generateModuleComparisonDirNames(-4, 4, -LAYER_HALF_WIDTH_IN_MODULES[4], LAYER_HALF_WIDTH_IN_MODULES[4])  // Layer 4
};

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
   int layer  = NOVAL_I; // Adding layer info explicitly
   int module = NOVAL_I;
   int ladder = NOVAL_I;
   int sec    = NOVAL_I;
   int side   = NOVAL_I;
   int disk   = NOVAL_I;
   int blade  = NOVAL_I;
   int panel  = NOVAL_I;
   int ring   = NOVAL_I;
};

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
      TF1* m_ascendingSigmoidFit = nullptr;
      TF1* m_descendingSigmoidFit = nullptr;
   public: 
      HistogramFitter(TH1F* histogramArg, std::chrono::duration<double> secondsToTimeoutArg, TCanvas* canvasArg) : histogramToFit(histogramArg), secondsToTimeout(secondsToTimeoutArg), canvas(canvasArg) {};
      int operator()();
      TCanvas* getCanvas() const { return canvas; };
      TH1F* getHistogram() const { return histogramToFit; };
      [[noreturn, gnu::cold]] void e_accessMemberFitIsNull() const { std::cout << "Requested access fitted function before it was generated." << std::endl; throw std::runtime_error("fitter"); };
      TF1*  getAscendingSigmoidFit()  const { if(m_ascendingSigmoidFit  == nullptr) e_accessMemberFitIsNull(); return m_ascendingSigmoidFit;  };
      TF1*  getDescendingSigmoidFit() const { if(m_descendingSigmoidFit == nullptr) e_accessMemberFitIsNull(); return m_descendingSigmoidFit; };
};

std::vector<std::vector<TH1F*>> getROCEfficiencyHistograms(TFile* t_inputFile);
std::vector<std::vector<HistogramFitter>> generateROCefficiencyFitterObjects(const std::vector<std::vector<TH1F*>>& t_efficiencyVsDelayHistogramsToFit);
std::vector<std::map<TH1F*, std::pair<TF1*, TF1*>>> handleROCEfficiencyHistograms(const std::vector<std::vector<TH1F*>>& t_efficiencyVsDelayHistogramsToFit, const std::vector<std::map<int, ROCPlacement>>& t_layersROCIndexToPlacementMaps, TFile* t_outputFile);
std::vector<std::pair<TH1F*, TH1F*>> getRampBoundaryPointDistributions(const std::vector<std::map<TH1F*, std::pair<TF1*, TF1*>>>& t_fitResults);
std::vector<TH1F*> getReadOutGroupEfficiencyHistograms(TFile* t_inputFile);
std::vector<std::map<int, ROCPlacement>> getLayersROCIndexToPlacementMaps(std::string t_filename);
double findSigmoidNTimesMaxHeightPosition(const TF1* t_sigmoid, const double t_relativeHeight, const double t_accuracy = 0.001);
void createOutputDirectories(TFile* t_outputFile);
TH1F* getROCefficiencyDifferencesPlot(TH1F* t_ROC_histogram, std::vector<TH1F*> t_other_ROC_histograms, const std::string& t_histogramName, const std::string& t_histogramTitle);
int getROCIndexByName(const std::string& t_name);

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
   [[maybe_unused]] TFile* inputFile = TFile::Open(INPUT_FILE_PATH, "READ");
   [[maybe_unused]] TFile* outputFile = TFile::Open(OUTPUT_FILE_PATH, "RECREATE");

   // inputFile -> cd("Efficiency_vs_delay");
   // TH1F* bpix = dynamic_cast<TH1F*>(gDirectory -> Get("delayVsEfficiencyBPix"));

   // ---------------------------------------------------------------------------------------------
   // Reading in the saved ROC efficiency histograms from the input file

   std::cout << "Reading in the saved ROC efficiency histograms from the input file." << std::flush;
   std::future<std::vector<std::vector<TH1F*>>> fileReaderProcess = std::async(getROCEfficiencyHistograms, inputFile);
   while(fileReaderProcess.wait_for(milliseconds_100) == std::future_status::timeout)
   {
      std::cout << '.' << std::flush;
   }
   std::vector<std::vector<TH1F*>> efficiencyVsDelayHistogramsToFit = fileReaderProcess.get();
   std::cout << "Done." << std::endl;
   
   inputFile -> Close();

   // ---------------------------------------------------------------------------------------------
   // Prepare to produce the output

   // Create output directory system
   std::cout << "Generating directory system for the output file... " << std::flush;
   createOutputDirectories(outputFile);
   std::cout << "Done." << std::endl;

   // Get directory maps
   std::cout << "Loading directory maps... " << std::flush;
   std::vector<std::map<int, ROCPlacement>> layersROCIndexToPlacementMaps = getLayersROCIndexToPlacementMaps(ROC_PLACEMENT_MAP_PATH);
   std::cout << "Done. " << std::endl;

   // Fit the ROC histograms
   std::vector<std::map<TH1F*, std::pair<TF1*, TF1*>>> fitResults = handleROCEfficiencyHistograms(efficiencyVsDelayHistogramsToFit, layersROCIndexToPlacementMaps, outputFile);

   // Get the distribution of the ramp-relative point
   std::vector<std::pair<TH1F*, TH1F*>> rampDistributions = getRampBoundaryPointDistributions(fitResults);
   outputFile -> mkdir("RampEndPointDistributions");
   outputFile -> cd("RampEndPointDistributions");
   for(const auto& rampDistributionPair: rampDistributions)
   {
      if(rampDistributionPair.first == nullptr) throw "";
      if(rampDistributionPair.second == nullptr) throw "";
      rampDistributionPair.first  -> Write();
      rampDistributionPair.second -> Write();
   }
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

std::vector<std::vector<TH1F*>> getROCEfficiencyHistograms(TFile* t_inputFile) try
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

// ---------------------------------------------------------------------------------------------
// Generating fitter objects

// These objects are used to fit the ROC efficiencies

std::vector<std::vector<HistogramFitter>> generateROCefficiencyFitterObjects(const std::vector<std::vector<TH1F*>>& t_efficiencyVsDelayHistogramsToFit)
{
   std::vector<std::vector<HistogramFitter>> fitterObjectList(t_efficiencyVsDelayHistogramsToFit.size());
   for(std::size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      const std::vector<TH1F*> efficiencyVsDelayOnROCLayerHistograms = t_efficiencyVsDelayHistogramsToFit[layerIndex];
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
   return fitterObjectList;
}

// ---------------------------------------------------------------------------------------------
// Fitting all the ROC efficiencies

// The ROC-s need to be fitted from two sides (if this is possible)
// with the proper sigmoid (single sided ramp) function.
// To find the intervals for these two functions two different approach is 
// used: calculating the zero point of the extrapolated discrete derivative
// or fitting the curves with a gaussian function and determining its mean 
// value.

std::vector<std::map<TH1F*, std::pair<TF1*, TF1*>>> handleROCEfficiencyHistograms(const std::vector<std::vector<TH1F*>>& t_efficiencyVsDelayHistogramsToFit, const std::vector<std::map<int, ROCPlacement>>& t_layersROCIndexToPlacementMaps, TFile* t_outputFile)
{
   std::vector<std::map<TH1F*, std::pair<TF1*, TF1*>>> result(5);

   // ---------------------------------------------------------------------------------------------
   // Generating lightweight fitter objects for each of the histograms

   std::cout << "Generating fitter objects... " << std::flush;
   std::vector<std::vector<HistogramFitter>> fitterObjectList(generateROCefficiencyFitterObjects(t_efficiencyVsDelayHistogramsToFit));
   std::cout << "Done." << std::endl;

   // ---------------------------------------------------------------------------------------------
   // Get the results of the fits

   std::cout << "Fitting histograms... " << std::endl; 
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
      for(auto& fitterObject: fitterObjectList[layerIndex])
      {
         using FitterErrorCodes = HistogramFitter::FitterErrorCodes;
         FitterErrorCodes fitResult = FitterErrorCodes(fitterObject());
         if(fitResult == FitterErrorCodes(0)) // Succesful fits
         {
            fitROCsOnLayer++;
            TH1F* rocEfficiencyHistogram = fitterObject.getHistogram();
            int rocIndex = getROCIndexByName(rocEfficiencyHistogram -> GetName());
            const auto& findRes = t_layersROCIndexToPlacementMaps[layerIndex].find(rocIndex);
            if(findRes != t_layersROCIndexToPlacementMaps[layerIndex].end())
            {
               auto rocPlacement = findRes -> second;
               const char* layerDirName = outputMainDirectoryNames[layerIndex].c_str();

               // Filtering helper, creates predicates about the placements of the ROCs
               std::mutex regexLock;
               auto transformToROCPredicate = [&t_layersROCIndexToPlacementMaps, layerIndex, &regexLock] (const std::function<bool(const ROCPlacement&)>& pred)
               {
                  return [&t_layersROCIndexToPlacementMaps, layerIndex, &pred, &regexLock] (auto e)
                  {
                     const char* histogramName = e -> GetName();
                     int otherROCIndex = [&regexLock, &histogramName]
                     {
                        std::lock_guard<std::mutex> regexLockGuard(regexLock);
                        return getROCIndexByName(histogramName);
                     }();
                     const auto findRes = t_layersROCIndexToPlacementMaps[layerIndex].find(otherROCIndex);
                     if(findRes != t_layersROCIndexToPlacementMaps[layerIndex].end()) // This skips non-mapped ROCs without any kind of message, FIXME
                     {
                        return pred(findRes -> second); 
                     }
                     return false;
                  };
               };

               // Comparison helper
               std::mutex writeLock;
               auto writeComparisonPlots = [&, transformToROCPredicate, layerIndex] 
                  (const std::string& histogramName, const std::string& histogramTitle, std::initializer_list<const char*> directoryPath, const std::function<bool(const ROCPlacement&)>& pred)
               {
                  assert(directoryPath.size());
                  std::vector<TH1F*> passedROCFiltering = filter(t_efficiencyVsDelayHistogramsToFit[layerIndex], transformToROCPredicate(pred));
                  TH1F* rocEfficiencyDifferencesModuleAvg = getROCefficiencyDifferencesPlot(rocEfficiencyHistogram, passedROCFiltering, histogramName.c_str(), histogramTitle.c_str());
                  std::lock_guard<std::mutex> writeLockGuard(writeLock);
                  t_outputFile -> cd(*directoryPath.begin());
                  std::for_each(directoryPath.begin() + 1, directoryPath.end(), [] (const char* e)
                  {
                     gDirectory -> cd(e); 
                  });
                  rocEfficiencyDifferencesModuleAvg -> Write();
               };

               if(layerIndex == 0) // Forward region
               {
                  const int side  = rocPlacement.side;
                  const int disk  = rocPlacement.disk;
                  const int ring  = rocPlacement.ring;
                  const int blade = rocPlacement.blade;
                  const int panel = rocPlacement.panel;
                  // Directory names
                  const char* sideDirName  = sideNames[side - 1].c_str();
                  const char* diskDirName  = diskNames[std::abs(disk) - 1].c_str();
                  const char* ringDirName  = ringNames[ring - 1].c_str();
                  const char* bladeDirName = bladeNames[blade < 0 ? blade + 17 : blade + 16].c_str();
                  const char* panelDirName = panelNames[panel - 1].c_str();
                  // Saving the default fit plot
                  t_outputFile -> cd(outputMainDirectoryNames[layerIndex].c_str());
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(diskDirName);
                  gDirectory -> cd(ringDirName);
                  gDirectory -> cd(bladeDirName);
                  gDirectory -> cd(panelDirName);
                  fitterObject.getCanvas() -> Write();
                  // Save comparison plots

                  // ROC efficiency differences compared to the mean of  the containing
                  //    - blade-panel     -o                                                                  |
                  //    - blade             \                                                                 |
                  //    - disk               o---- Forward                                                    |
                  //    - disk-ring         /                                                                 |
                  //    - side            -o                                                                  |

                  // Side
                  auto sameSide = [side] (const ROCPlacement& rp) { return rp.side == side; };
                  std::future<void> sideComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_side_comparison", "ROC: " + std::to_string(rocIndex) + " - side comparison",
                  {
                     "ROC_efficiency_side_level_comparison", layerDirName, sideDirName
                  }, sameSide); });
                  // Blade-panel
                  auto sameBladePanel = [disk, ring, blade, panel] (const ROCPlacement& rp) { return rp.disk == disk && rp.ring == ring && rp.blade == blade && rp.panel == panel; };
                  std::future<void> bladePanelComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_blade-panel_comparison", "ROC: " + std::to_string(rocIndex) + " - blade-panel comparison",
                  {
                     "ROC_efficiency_panel_level_comparison", layerDirName, sideDirName, diskDirName, ringDirName, bladeDirName, panelDirName
                  }, sameBladePanel); });
                  // Blade
                  auto sameBlade = [disk, ring, blade] (const ROCPlacement& rp) { return rp.disk == disk && rp.ring == ring && rp.blade == blade; };
                  std::future<void> bladeComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_blade_comparison", "ROC: " + std::to_string(rocIndex) + " - blade comparison",
                  {
                     "ROC_efficiency_blade_level_comparison", layerDirName, sideDirName, diskDirName, ringDirName, bladeDirName
                  }, sameBlade); });
                  // Disk
                  auto sameDisk = [disk] (const ROCPlacement& rp) { return rp.disk == disk; };
                  std::future<void> diskComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_disk_comparison", "ROC: " + std::to_string(rocIndex) + " - disk comparison",
                  {
                     "ROC_efficiency_disk_level_comparison", layerDirName, sideDirName, diskDirName
                  }, sameDisk); });
                  // Disk-ring
                  auto sameDiskRing = [disk, ring] (const ROCPlacement& rp) { return rp.disk == disk && rp.ring == ring; };
                  // The last thread should be run synchronously; the name std::async is misleading
                  std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_disk-ring_comparison", "ROC: " + std::to_string(rocIndex) + " - disk-ring comparison",
                  {
                     "ROC_efficiency_disk-ring_level_comparison", layerDirName, sideDirName, diskDirName, ringDirName
                  }, sameDiskRing); });
                  sideComparisonThread.get();
                  bladePanelComparisonThread.get();
                  bladeComparisonThread.get();
                  diskComparisonThread.get();
               }
               else // Barrel region
               {
                  const int side                      = rocPlacement.side;
                  const int module                    = rocPlacement.module;
                  const int ladder                    = rocPlacement.ladder;
                  const int half                      = module < 0 ? 0 : 1;
                  const int sec                       = rocPlacement.sec;
                  const int layerHalfWidth            = LAYER_HALF_WIDTH_IN_MODULES[layerIndex];
                  const std::size_t moduleLadderIndex = (module + (module < 0 ? 4 : 3)) * layerHalfWidth + ladder + (ladder < 0 ? layerHalfWidth : layerHalfWidth - 1);
                  // assert(moduleLadderIndex < rocModuleComparisonDirNamesOnLayers[layerIndex].size());
                  // Directory names
                  const char* sideDirName         = sideNames[side - 1].c_str();
                  const char* halfshellDirName    = halfshellNames[half].c_str();
                  const char* sectorDirName       = sectorNames[sec - 1].c_str();
                  const char* moduleLadderDirname = rocModuleComparisonDirNamesOnLayers[layerIndex][moduleLadderIndex].c_str();
                  // Saving the default fit plot
                  t_outputFile -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(halfshellDirName);
                  gDirectory -> cd(sectorDirName);
                  fitterObject.getCanvas() -> Write();
                  // Save comparison plots

                  // ROC efficiency differences compared to the mean of  the containing
                  //    - module       -o                                                                     |
                  //    - sector         \                                                                    |
                  //    - side            o---- Barrel                                                        |
                  //    - halfshell      /                                                                    |
                  //    - quarter      -o                                                                     |

                  // Module
                  auto sameModule = [module, ladder] (const ROCPlacement& rp) { return rp.module == module && rp.ladder == ladder; };
                  std::future<void> moduleComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_module_comparison", "ROC: " + std::to_string(rocIndex) + " - module comparison",
                  {
                     "ROC_efficiency_module_level_comparison", layerDirName, moduleLadderDirname
                  }, sameModule); });
                  // Sector
                  auto sameSector = [side, module, sec] (const ROCPlacement& rp) { return rp.side == side && std::signbit(rp.module) == std::signbit(module) && rp.sec == sec; };
                  std::future<void> sectorComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_sector_comparison", "ROC: " + std::to_string(rocIndex) + " - sector comparison",
                  {
                     "ROC_efficiency_sector_level_comparison", layerDirName, sideDirName, halfshellDirName, sectorDirName
                  }, sameSector); });
                  // Side
                  auto sameSide = [side] (const ROCPlacement& rp) { return rp.side == side; };
                  std::future<void> sideComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_side_comparison", "ROC: " + std::to_string(rocIndex) + " - side comparison",
                  {
                     "ROC_efficiency_side_level_comparison", layerDirName, sideDirName
                  }, sameSide); });
                  // Halfshell
                  auto sameHalfshell = [module] (const ROCPlacement& rp) { return std::signbit(rp.module) == std::signbit(module); };
                  std::future<void> halfshellComparisonThread = std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_halfshell_comparison", "ROC: " + std::to_string(rocIndex) + " - halfshell comparison",
                  {
                     "ROC_efficiency_halfshell_level_comparison", layerDirName, halfshellDirName
                  }, sameHalfshell); });
                  // Quarter
                  auto sameQuarter = [side, module] (const ROCPlacement& rp) { return rp.side == side && std::signbit(rp.module) == std::signbit(module); };
                  // The last thread should be run synchronously; the name std::async is misleading
                  std::async(std::launch::async, [&] { writeComparisonPlots("ROC:" + std::to_string(rocIndex) + "_quarter_comparison", "ROC: " + std::to_string(rocIndex) + " - quarter comparison",
                  {
                     "ROC_efficiency_quarter_level_comparison", layerDirName, sideDirName, halfshellDirName
                  }, sameQuarter); });
                  moduleComparisonThread.get();
                  sectorComparisonThread.get();
                  sideComparisonThread.get();
                  halfshellComparisonThread.get();
               }
            }
            else // The placement of the ROC is unindentified
            {
               // Save the fitted ROC plots nevertheless
               t_outputFile -> cd(outputMainDirectoryNames[layerIndex].c_str());
               fitterObject.getCanvas() -> Write();
               result[layerIndex].emplace(fitterObject.getHistogram(), std::make_pair(fitterObject.getAscendingSigmoidFit(), fitterObject.getDescendingSigmoidFit()));
            }
         }
         else
         {
            skippedROCsOnLayer++;
         }
         updateSkippedROCsMsg();
         static int breaker = 0;
         if(breaker++ == 20)
         {
            breaker = 0;
            break;
         }
      }
      std::cout << std::endl;
   }
   std::cout << "Done." << std::endl;
   return result;
}

// ---------------------------------------------------------------------------------------------
// Get the distribution of the locations where the ramps end

// End point: the efficiency drops by 1 percent compared to the maximum

std::vector<std::pair<TH1F*, TH1F*>> getRampBoundaryPointDistributions(const std::vector<std::map<TH1F*, std::pair<TF1*, TF1*>>>& t_fitResults)
{
   std::vector<std::pair<TH1F*, TH1F*>> result(5);
   for(std::size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      TH1F* ascendingRampEndPointsOnLayer =  new TH1F(("ascRampEndPointDistributionOnLayer"  + std::to_string(layerIndex)).c_str(), ("Asc. ramp end point distribution on layer "  + std::to_string(layerIndex)).c_str(), 160, -10.0f, 30.0f);
      TH1F* descendingRampEndPointsOnLayer = new TH1F(("descRampEndPointDistributionOnLayer" + std::to_string(layerIndex)).c_str(), ("Desc. ramp end point distribution on layer " + std::to_string(layerIndex)).c_str(), 160, -10.0f, 30.0f);
      for(const auto& histogramFitsPair: t_fitResults[layerIndex])
      {
         ascendingRampEndPointsOnLayer  -> Fill(findSigmoidNTimesMaxHeightPosition(histogramFitsPair.second.first,  0.99, 0.001));
         descendingRampEndPointsOnLayer -> Fill(findSigmoidNTimesMaxHeightPosition(histogramFitsPair.second.second, 0.99, 0.001));
         std::cout << "findSigmoidNTimesMaxHeightPosition(histogramFitsPair.second.first,  0.99, 0.001): " << findSigmoidNTimesMaxHeightPosition(histogramFitsPair.second.first,  0.99, 0.001) << std::endl;
         std::cout << "findSigmoidNTimesMaxHeightPosition(histogramFitsPair.second.second, 0.99, 0.001): " << findSigmoidNTimesMaxHeightPosition(histogramFitsPair.second.second, 0.99, 0.001) << std::endl;
      }
      result[layerIndex] = std::make_pair(std::move(ascendingRampEndPointsOnLayer), std::move(descendingRampEndPointsOnLayer));
   }
   std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
   std::cout << "result.size(): " << result.size() << std::endl;
   return result;
}

// Read in read-out-group efficiencies from a file
std::vector<TH1F*> getReadOutGroupEfficiencyHistograms(TFile* t_inputFile) try
{
   std::vector<TH1F*> readOutGroupEfficiencyHistograms;
   if(!(t_inputFile -> IsOpen())) throw std::runtime_error("input file bad or missing.");
   t_inputFile -> cd("Efficiency_vs_delay");
   for(unsigned int side = 1; side <= 2; ++side)
   {
      static const std::vector<std::string> sideStrings = {"Negative", "Positive"};
      std::string sideAsString                          = sideStrings[side - 1];
      for(unsigned int halfShell = 1; halfShell <= 2; ++halfShell)
      {
         std::string halfShellAsString = sideStrings[halfShell - 1];
         for(unsigned int layerPair = 1; layerPair <= 2; ++layerPair)
         {
            static const std::vector<std::string> layerPairStringsShort = {"1and2", "3and4"};
            std::string layerPairAsShortString                          = layerPairStringsShort[layerPair - 1];
            for(unsigned int sec = 1; sec <= 8; ++sec)
            {
               std::string secAsString = std::to_string(sec);
               std::string histogramName = "delayVsEfficiency" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString;
               TH1F* histogram = dynamic_cast<TH1F*>(gDirectory -> Get(histogramName.c_str()));
               if(histogram == nullptr)
               {
                  std::cout << "Missing plot: histogramName: " << histogramName << std::endl;
               }
               histogram -> SetDirectory(0);
               readOutGroupEfficiencyHistograms.emplace_back(histogram);
            }
         }
      }
   }
   return readOutGroupEfficiencyHistograms;
}
catch(const std::exception& e)
{
   std::cout << e.what() << std::endl;
   return {};
}

// ------------------------------------------------------------------------------------------------
// Creating directories

// This nesting seems like an overkill. It works though.
// Here is a happy smile to help you overcome your frustration about it: (*.*)
// Make sure to use code folding to hide this mess. Because that is exactly what should be done with messy things. :D

// This is how the directory structure should look like:
// ├── FPix/
// │   ├── Positive_Side
// │   │   ├─ Disk_1
// │   │   │   ├─ Ring_1
// │   │   │   │   └─ Other_nested_levels
// │   │   │   └─ Ring_2
// │   │   │       └─ Other_nested_levels
// │   │   ├─ Disk_2
// │   │   │   ├─ Ring_1
// │   │   │   │   └─ Other_nested_levels
// │   │   │   └─ Ring_2
// │   │   │      └─ Other_nested_levels
// │   │   └─ ...
// │   │       └─ Same_as_the_other_disks
// │   │
// │   └── Negative_side
// │        └─ Same_as_the_positive_side
// │
// ├── ROC_efficiency_disk_level_comparison
// │   └─ Nested levels until disks
// │        └─ ...
// │
// ├── ROC_efficiency_ring_level_comparison
// │   └─ Nested levels until rings
// │        └─ ...
// │
// ├── Other comparisons for FPix
// │
// ├── Bpix
// │    └─ ...
// │
// └── BPix comparison plots
//      └─ ...

void createOutputDirectories(TFile* t_outputFile)
{
   t_outputFile -> mkdir("ROC_efficiency_module_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_sector_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_side_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_halfshell_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_quarter_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_blade_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_panel_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_disk-ring_level_comparison");
   t_outputFile -> mkdir("ROC_efficiency_disk_level_comparison");

   // ---------------------------------------------------------------------------------------------
   // The fited ROC efficiency plots
   for(std::size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
   {
      const char* layerDirName = outputMainDirectoryNames[layerIndex].c_str();
      t_outputFile -> mkdir(layerDirName);
      if(layerIndex == 0) // Forward region
      {
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t diskIndex = 0, diskMaxIndex = diskNames.size(); diskIndex < diskMaxIndex; ++diskIndex)
            {
               const char* diskDirName = diskNames[diskIndex].c_str();
               t_outputFile -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(diskDirName);
               gDirectory -> cd(diskDirName);
               for(std::size_t ringIndex = 0, ringMaxIndex = ringNames.size(); ringIndex < ringMaxIndex; ++ringIndex)
               {
                  const char* ringDirName = ringNames[ringIndex].c_str();
                  t_outputFile -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(diskDirName);
                  gDirectory -> mkdir(ringDirName);
                  gDirectory -> cd(ringDirName);
                  for(std::size_t bladeIndex = 0, bladeMaxIndex = bladeNames.size(); bladeIndex < bladeMaxIndex; ++bladeIndex)
                  {
                     const char* bladeDirName = bladeNames[bladeIndex].c_str();
                     t_outputFile -> cd(layerDirName);
                     gDirectory -> cd(sideDirName);
                     gDirectory -> cd(diskDirName);
                     gDirectory -> cd(ringDirName);
                     gDirectory -> mkdir(bladeDirName);
                     gDirectory -> cd(bladeDirName);
                     for(std::size_t panelIndex = 0, panelMaxIndex = panelNames.size(); panelIndex < panelMaxIndex; ++panelIndex)
                     {
                        const char* panelDirName = panelNames[panelIndex].c_str();
                        t_outputFile -> cd(layerDirName);
                        gDirectory -> cd(sideDirName);
                        gDirectory -> cd(diskDirName);
                        gDirectory -> cd(ringDirName);
                        gDirectory -> cd(bladeDirName);
                        gDirectory -> mkdir(panelDirName);
                     }
                  }
               }
            }
         }
      }
      else // Barrel region
      {
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t halfshellIndex = 0, maxHalfshellIndex = halfshellNames.size(); halfshellIndex < maxHalfshellIndex; ++halfshellIndex)
            {
               const char* halfshellDirName = halfshellNames[halfshellIndex].c_str();
               t_outputFile -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(halfshellDirName);
               gDirectory -> cd(halfshellDirName);
               for(std::size_t sectorIndex = 0, sectorMaxIndex = sectorNames.size(); sectorIndex < sectorMaxIndex; ++sectorIndex)
               {
                  const char* sectorDirName = sectorNames[sectorIndex].c_str();
                  t_outputFile -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(halfshellDirName);
                  gDirectory -> mkdir(sectorDirName);
               }
            }
         }
      }

      // ---------------------------------------------------------------------------------------------
      // ROC efficiencies compared to the detector part means

      t_outputFile -> cd("ROC_efficiency_module_level_comparison");
      gDirectory   -> mkdir(layerDirName);
      if(layerIndex == 0) // Forward region
      {
         // ROC efficiency differences compared to the mean of  the containing
         //    - blade-panel     -o                                                                  |
         //    - blade             \                                                                 |
         //    - disk               o---- Forward                                                    |
         //    - disk-ring         /                                                                 |
         //    - side            -o                                                                  |
      
         // Side
         t_outputFile -> cd("ROC_efficiency_side_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_side_level_comparison");
            gDirectory   -> cd(layerDirName);
            gDirectory   -> mkdir(sideDirName);
         }
         // Blade-panel
         t_outputFile -> cd("ROC_efficiency_panel_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_panel_level_comparison");
            gDirectory -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t diskIndex = 0, diskMaxIndex = diskNames.size(); diskIndex < diskMaxIndex; ++diskIndex)
            {
               const char* diskDirName = diskNames[diskIndex].c_str();
               t_outputFile -> cd("ROC_efficiency_panel_level_comparison");
               gDirectory -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(diskDirName);
               gDirectory -> cd(diskDirName);
               for(std::size_t ringIndex = 0, ringMaxIndex = ringNames.size(); ringIndex < ringMaxIndex; ++ringIndex)
               {
                  const char* ringDirName = ringNames[ringIndex].c_str();
                  t_outputFile -> cd("ROC_efficiency_panel_level_comparison");
                  gDirectory -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(diskDirName);
                  gDirectory -> mkdir(ringDirName);
                  gDirectory -> cd(ringDirName);
                  for(std::size_t bladeIndex = 0, bladeMaxIndex = bladeNames.size(); bladeIndex < bladeMaxIndex; ++bladeIndex)
                  {
                     const char* bladeDirName = bladeNames[bladeIndex].c_str();
                     t_outputFile -> cd("ROC_efficiency_panel_level_comparison");
                     gDirectory -> cd(layerDirName);
                     gDirectory -> cd(sideDirName);
                     gDirectory -> cd(diskDirName);
                     gDirectory -> cd(ringDirName);
                     gDirectory -> mkdir(bladeDirName);
                     gDirectory -> cd(bladeDirName);
                     for(std::size_t panelIndex = 0, panelMaxIndex = panelNames.size(); panelIndex < panelMaxIndex; ++panelIndex)
                     {
                        const char* panelDirName = panelNames[panelIndex].c_str();
                        t_outputFile -> cd("ROC_efficiency_panel_level_comparison");
                        gDirectory -> cd(layerDirName);
                        gDirectory -> cd(sideDirName);
                        gDirectory -> cd(diskDirName);
                        gDirectory -> cd(ringDirName);
                        gDirectory -> cd(bladeDirName);
                        gDirectory -> mkdir(panelDirName);
                     }
                  }
               }
            }
         }
         // Blade
         t_outputFile -> cd("ROC_efficiency_blade_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_blade_level_comparison");
            gDirectory -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t diskIndex = 0, diskMaxIndex = diskNames.size(); diskIndex < diskMaxIndex; ++diskIndex)
            {
               const char* diskDirName = diskNames[diskIndex].c_str();
               t_outputFile -> cd("ROC_efficiency_blade_level_comparison");
               gDirectory -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(diskDirName);
               gDirectory -> cd(diskDirName);
               for(std::size_t ringIndex = 0, ringMaxIndex = ringNames.size(); ringIndex < ringMaxIndex; ++ringIndex)
               {
                  const char* ringDirName = ringNames[ringIndex].c_str();
                  t_outputFile -> cd("ROC_efficiency_blade_level_comparison");
                  gDirectory -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(diskDirName);
                  gDirectory -> mkdir(ringDirName);
                  gDirectory -> cd(ringDirName);
                  for(std::size_t bladeIndex = 0, bladeMaxIndex = bladeNames.size(); bladeIndex < bladeMaxIndex; ++bladeIndex)
                  {
                     const char* bladeDirName = bladeNames[bladeIndex].c_str();
                     t_outputFile -> cd("ROC_efficiency_blade_level_comparison");
                     gDirectory -> cd(layerDirName);
                     gDirectory -> cd(sideDirName);
                     gDirectory -> cd(diskDirName);
                     gDirectory -> cd(ringDirName);
                     gDirectory -> mkdir(bladeDirName);
                  }
               }
            }
         }
         // Disk
         t_outputFile -> cd("ROC_efficiency_disk_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_disk_level_comparison");
            gDirectory -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t diskIndex = 0, diskMaxIndex = diskNames.size(); diskIndex < diskMaxIndex; ++diskIndex)
            {
               const char* diskDirName = diskNames[diskIndex].c_str();
               t_outputFile -> cd("ROC_efficiency_disk_level_comparison");
               gDirectory -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(diskDirName);
            }
         }
         // Disk-ring
         t_outputFile -> cd("ROC_efficiency_disk-ring_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_disk-ring_level_comparison");
            gDirectory -> cd(layerDirName);
            gDirectory -> mkdir(sideDirName);
            gDirectory -> cd(sideDirName);
            for(std::size_t diskIndex = 0, diskMaxIndex = diskNames.size(); diskIndex < diskMaxIndex; ++diskIndex)
            {
               const char* diskDirName = diskNames[diskIndex].c_str();
               t_outputFile -> cd("ROC_efficiency_disk-ring_level_comparison");
               gDirectory -> cd(layerDirName);
               gDirectory -> cd(sideDirName);
               gDirectory -> mkdir(diskDirName);
               gDirectory -> cd(diskDirName);
               for(std::size_t ringIndex = 0, ringMaxIndex = ringNames.size(); ringIndex < ringMaxIndex; ++ringIndex)
               {
                  const char* ringDirName = ringNames[ringIndex].c_str();
                  t_outputFile -> cd("ROC_efficiency_disk-ring_level_comparison");
                  gDirectory -> cd(layerDirName);
                  gDirectory -> cd(sideDirName);
                  gDirectory -> cd(diskDirName);
                  gDirectory -> mkdir(ringDirName);
               }
            }
         }
      }
      else // Barrel region
      {
         // ROC efficiency differences compared to the mean of  the containing
         //    - module       -o                                                                     |
         //    - sector         \                                                                    |
         //    - side            o---- Barrel                                                        |
         //    - halfshell      /                                                                    |
         //    - quarter      -o                                                                     |

         // Module
         for(std::size_t moduleLadderIndex = 0, maxModuleLadderIndex = rocModuleComparisonDirNamesOnLayers[layerIndex].size(); moduleLadderIndex < maxModuleLadderIndex; ++moduleLadderIndex)
         {
            t_outputFile -> cd("ROC_efficiency_module_level_comparison");
            gDirectory   -> cd(layerDirName);
            gDirectory   -> mkdir(rocModuleComparisonDirNamesOnLayers[layerIndex][moduleLadderIndex].c_str());
         }
         // Sector
         t_outputFile -> cd("ROC_efficiency_sector_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_sector_level_comparison");
            gDirectory   -> cd(layerDirName);
            gDirectory   -> mkdir(sideDirName);
            gDirectory   -> cd(sideDirName);
            for(std::size_t halfshellIndex = 0, maxHalfshellIndex = halfshellNames.size(); halfshellIndex < maxHalfshellIndex; ++halfshellIndex)
            {
               const char* halfshellDirName = halfshellNames[halfshellIndex].c_str();
               t_outputFile -> cd("ROC_efficiency_sector_level_comparison");
               gDirectory   -> cd(layerDirName);
               gDirectory   -> cd(sideDirName);
               gDirectory   -> mkdir(halfshellDirName);
               gDirectory   -> cd(halfshellDirName);
               for(std::size_t sectorIndex = 0, sectorMaxIndex = sectorNames.size(); sectorIndex < sectorMaxIndex; ++sectorIndex)
               {
                  const char* sectorDirName = sectorNames[sectorIndex].c_str();
                  t_outputFile -> cd("ROC_efficiency_sector_level_comparison");
                  gDirectory   -> cd(layerDirName);
                  gDirectory   -> cd(sideDirName);
                  gDirectory   -> cd(halfshellDirName);
                  gDirectory   -> mkdir(sectorDirName);
               }
            }
         }
         // Side
         t_outputFile -> cd("ROC_efficiency_side_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_side_level_comparison");
            gDirectory   -> cd(layerDirName);
            gDirectory   -> mkdir(sideDirName);
         }
         // Halfshell
         t_outputFile -> cd("ROC_efficiency_halfshell_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t halfshellIndex = 0, maxHalfshellIndex = halfshellNames.size(); halfshellIndex < maxHalfshellIndex; ++halfshellIndex)
         {
            const char* halfshellDirName = halfshellNames[halfshellIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_halfshell_level_comparison");
            gDirectory   -> cd(layerDirName);
            gDirectory   -> mkdir(halfshellDirName);
         }
         // Quarter
         t_outputFile -> cd("ROC_efficiency_quarter_level_comparison");
         gDirectory   -> mkdir(layerDirName);
         for(std::size_t sideIndex = 0, maxSideIndex = sideNames.size(); sideIndex < maxSideIndex; ++sideIndex)
         {
            const char* sideDirName = sideNames[sideIndex].c_str();
            t_outputFile -> cd("ROC_efficiency_quarter_level_comparison");
            gDirectory   -> cd(layerDirName);
            gDirectory   -> mkdir(sideDirName);
            for(std::size_t halfshellIndex = 0, maxHalfshellIndex = halfshellNames.size(); halfshellIndex < maxHalfshellIndex; ++halfshellIndex)
            {
               const char* halfshellDirName = halfshellNames[halfshellIndex].c_str();
               t_outputFile -> cd("ROC_efficiency_quarter_level_comparison");
               gDirectory   -> cd(layerDirName);
               gDirectory   -> cd(sideDirName);
               gDirectory   -> mkdir(halfshellDirName);
            }
         }
      }
   }
}

[[noreturn]] void e_ROCRegexMatch()
{
   throw std::runtime_error("Matching ROC name regex failed.");
}

int getROCIndexByName(const std::string& t_name)
{
   std::smatch match;
   if(std::regex_match(t_name, match, std::regex(R"(.*ROC ?(\d+))")))
   {
      // Correcting for my own mistake with the naming conventions
      // the ROC with 0 as rocIndex has the name "... ROC 1"
      return std::stoi(match[1].str()) - 1;
   }
   e_ROCRegexMatch();
}

TH1F* getROCefficiencyDifferencesPlot(TH1F* t_ROC_histogram, std::vector<TH1F*> t_other_ROC_histograms, const std::string& t_histogramName, const std::string& t_histogramTitle)
{
   int numROCBins = t_ROC_histogram -> GetNbinsX(); // Not necessarily filled bins
   assert(std::all_of(t_other_ROC_histograms.begin(), t_other_ROC_histograms.end(), 
      [numROCBins] (const TH1F* other) { return other -> GetNbinsX() == numROCBins; }));
   TH1F* differencesHistogram = new TH1F(t_histogramName.c_str(), t_histogramTitle.c_str(), numROCBins, t_ROC_histogram -> GetXaxis() -> GetXmin(), t_ROC_histogram -> GetXaxis() -> GetXmax());
   for(std::size_t binIndex = 0, maxBinIndex = t_ROC_histogram -> GetSize(); binIndex < maxBinIndex; ++binIndex)
   {
      float rocVal = t_ROC_histogram -> GetAt(binIndex);
      if(rocVal == 0.0f) continue;
      int nonZeroReferenceValues = std::count_if(t_other_ROC_histograms.begin(), t_other_ROC_histograms.end(), 
         [binIndex] (TH1F* l_reference) { return l_reference -> GetAt(binIndex) == 0; });
      float referenceVal = std::accumulate(t_other_ROC_histograms.begin(), t_other_ROC_histograms.end(), 0.0f, 
         [binIndex] (float sum, auto e) { return sum + e -> GetAt(binIndex); }) / nonZeroReferenceValues;
      differencesHistogram -> SetAt(rocVal - referenceVal, binIndex);
   }
   // Stylize histograms
   StylePresets::markersOnly(differencesHistogram, 34, 4);
   return differencesHistogram;
}

// ------------------------------------------------------------------------------------------------
// Read in maps containing information about the placement of the ROCs 

// The returned vector should contain 5 maps, each of them mapping ROCindex values to ROCPlacement objects
// The expected format is lines containing:
// {{layer, rocIndex}, {layer, module, ladder, sec, side, disk, blade, panel, ring}},
// Regex tempate to match the coordinates:
// R"(\{\{(-?\d+)\, (-?\d+)\}\, \{(-?\d+)\, (-?\d+)\, (-?\d+)\, (-?\d+)\, (-?\d+), (-?\d+), (-?\d+), (-?\d+), (-?\d+)\}\}\,)"
//         layer    rocIndex       layer     module    ladder      sec      side     disk    blade    panel    ring

std::vector<std::map<int, ROCPlacement>> getLayersROCIndexToPlacementMaps(std::string t_filename)
{
   std::cout << "Getting layer and ROC index -> ROC placement maps... " << std::flush;
   std::vector<std::map<int, ROCPlacement>> layersROCIndexToPlacementMaps(5);
   std::ifstream inputStream;
   // Regex matches:        layer    rocIndex       layer     module    ladder      sec      side     disk    blade    panel    ring
   std::regex regex(R"(\{\{(-?\d+)\, (-?\d+)\}\, \{(-?\d+)\, (-?\d+)\, (-?\d+)\, (-?\d+)\, (-?\d+), (-?\d+), (-?\d+), (-?\d+), (-?\d+)\}\}\,)");
   inputStream.open(t_filename);
   std::string line;
   while(std::getline(inputStream, line))
   {
      std::cmatch match;
      std::regex_match(line.c_str(), match, regex);
      int layer    = std::stoi(match[1].str());
      int rocIndex = std::stoi(match[2].str());
      ROCPlacement rocPlacement 
      {
         std::stoi(match[3].str()), std::stoi(match [4].str()), std::stoi(match [5].str()), 
         std::stoi(match[6].str()), std::stoi(match [7].str()), std::stoi(match [8].str()), 
         std::stoi(match[9].str()), std::stoi(match[10].str()), std::stoi(match[11].str())
      };
      layersROCIndexToPlacementMaps[layer].insert(std::make_pair(rocIndex, rocPlacement));
   }
   std::cout << "Done." << std::endl;
   return layersROCIndexToPlacementMaps;
}

// ------------------------------------------------------------------------------------------------
// Find N times maximum location of a monotone function | 0 < n < 1.00

// Monotoniticy of the function is assumed

[[noreturn, gnu::cold]] void e_sigmoidIsNullptr()
{
   throw std::runtime_error("error at " __FILE__ ":" S__LINE__);
}

[[noreturn, gnu::cold]] void e_relativeHeightRange()
{
   throw std::runtime_error("error at " __FILE__ ":" S__LINE__);
}

double findSigmoidNTimesMaxHeightPosition(const TF1* t_sigmoid, const double t_relativeHeight, const double t_accuracy)
{
   if(t_relativeHeight < 1.0) e_relativeHeightRange();
   if(t_sigmoid == nullptr)   e_sigmoidIsNullptr();
   double maximumValue = t_sigmoid -> GetMaximum();
   double goalValue    = t_relativeHeight * maximumValue;
   double keyMin = 0;
   double keyMax = 0;
   t_sigmoid -> GetRange(keyMin, keyMax);
   double keyApproximation;
   while(1)
   {
      keyApproximation = 0.5 * (keyMax - keyMin);
      double approximatedValue   = t_sigmoid -> Eval(keyApproximation);
      double goalValueDifference = approximatedValue - goalValue;
      if(std::abs(goalValueDifference) < t_accuracy)
      {
         return keyApproximation;
      }
      if(std::signbit(goalValueDifference)) keyMax = keyApproximation;
      else                                  keyMin = keyApproximation;
   }
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
   m_ascendingSigmoidFit  = new TF1(generateAscendingSigmoidName().c_str(),  "[0] / ((1 + exp([1] - x) / [2]))", -10.0f,           intervalBoundary);
   m_descendingSigmoidFit = new TF1(generateDescendingSigmoidName().c_str(), "[0] / ((1 + exp([1] + x) / [2]))", intervalBoundary, 30.0f           );
   m_ascendingSigmoidFit  -> SetParameter(0, 0.5 * (PAR_LIMITS_ASCENDING[0].first + PAR_LIMITS_ASCENDING[0].second));
   m_descendingSigmoidFit -> SetParameter(0, 0.5 * (PAR_LIMITS_DESCENDING[0].first + PAR_LIMITS_DESCENDING[0].second));
   m_ascendingSigmoidFit  -> SetParameter(1, 0.5 * (PAR_LIMITS_ASCENDING[1].first + PAR_LIMITS_ASCENDING[1].second));
   m_descendingSigmoidFit -> SetParameter(1, 0.5 * (PAR_LIMITS_DESCENDING[1].first + PAR_LIMITS_DESCENDING[1].second));
   m_ascendingSigmoidFit  -> SetParameter(2, 0.5 * (PAR_LIMITS_ASCENDING[2].first + PAR_LIMITS_ASCENDING[2].second));
   m_descendingSigmoidFit -> SetParameter(2, 0.5 * (PAR_LIMITS_DESCENDING[2].first + PAR_LIMITS_DESCENDING[2].second));
   m_ascendingSigmoidFit  -> SetParLimits(0, PAR_LIMITS_ASCENDING[0].first, PAR_LIMITS_ASCENDING[0].second);
   m_descendingSigmoidFit -> SetParLimits(0, PAR_LIMITS_DESCENDING[0].first, PAR_LIMITS_DESCENDING[0].second);
   m_ascendingSigmoidFit  -> SetParLimits(1, PAR_LIMITS_ASCENDING[1].first, PAR_LIMITS_ASCENDING[1].second);
   m_descendingSigmoidFit -> SetParLimits(1, PAR_LIMITS_DESCENDING[1].first, PAR_LIMITS_DESCENDING[1].second);
   m_ascendingSigmoidFit  -> SetParLimits(2, PAR_LIMITS_ASCENDING[2].first, PAR_LIMITS_ASCENDING[2].second);
   m_descendingSigmoidFit -> SetParLimits(2, PAR_LIMITS_DESCENDING[2].first, PAR_LIMITS_DESCENDING[2].second);
   // ascendingSigmoid -> SetRange(-10.0f, intervalBoundary);
   // descendingSigmoid -> SetRange(intervalBoundary, 30.0f);
   m_ascendingSigmoidFit -> SetLineColor(kRed);
   m_descendingSigmoidFit -> SetLineColor(kBlue);
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
      int executionStatus = adjustFit(histogramToFit, m_ascendingSigmoidFit, -10.0f, intervalBoundary, PAR_LIMITS_ASCENDING, startTime, secondsToTimeout);
      if(executionStatus != 0)
      {
         return executionStatus;
      }
   }
   if(intervalBoundary < 30.0f)
   {
      int executionStatus = adjustFit(histogramToFit, m_descendingSigmoidFit, intervalBoundary, 30.0f, PAR_LIMITS_DESCENDING, startTime, secondsToTimeout);
      if(executionStatus != 0)
      {
         return executionStatus;
      }
   }
   canvas -> cd();
   histogramToFit -> Draw();
   m_ascendingSigmoidFit -> Draw("SAME");
   m_descendingSigmoidFit -> Draw("SAME");
   return 0;
}
catch(const std::exception& e)
{
   std::cout << e.what() << std::endl;
   return static_cast<int>(FitterErrorCodes::UnhandledError);
};

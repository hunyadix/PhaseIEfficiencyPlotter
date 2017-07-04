// Descriptions:
// Reads in an Ntuple created by running the PhaseIPixelNtuplizer and creates
// the main plots required for efficiency measurements.
// List of the plots generated:
//  - 
//  -
//  - (nothing else yet...)

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
#include <initializer_list>

#include "../../interface/json.hpp"
using JSON = nlohmann::json;

// #define RANDOM_DELAYS

constexpr float HALF_PI = 0.5 * 3.141592653589793238462;

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);
constexpr auto                    CONFIG_FILE_PATH                = "./config_main.json"; 
const     std::string             EFFICIENCY_PLOT_IDENTIFIER      = "Efficiency";
const     std::string             EFFICIENCY_NUMERATOR_IDENTIFIER = "Numhits";

const     int   DELAY_PLOTS_NUM_BINS                             = 80; // WBC setting 164 is 0.
const     float DELAY_PLOTS_LOWER_EDGE                           = -10;
const     float DELAY_PLOTS_UPPER_EDGE                           = 30;

const bool CLUST_LOOP_REQUESTED = false;
const bool TRAJ_LOOP_REQUESTED  = true;
constexpr EfficiencyPlotsModule::Scenario SCENARIO = EfficiencyPlotsModule::Collisions;
// constexpr EfficiencyPlotsModule::Scenario SCENARIO = EfficiencyPlotsModule::Cosmics;

void                     testSaveFolders(const JSON& config);
TFile*                   generateOutputNtuple(const JSON& config);
std::vector<std::string> getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey);
void                     readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);
TGraphAsymmErrors*       getGraphForEfficiencyWithAsymmetricErrors(const TH1D& efficiencyHistogram, const TH1D& numHitsHistogram);
float                    getDelayNs(int runNumber, int lumisectionNumber);

int main([[maybe_unused]] int argc, char** argv) try
{
   std::cout << process_prompt << argv[0] << " started..." << std::endl;
   std::time_t processStarted = std::time(nullptr);
   std::cout << timer_prompt << "Time: " << std::asctime(std::localtime(&processStarted)) << std::flush;
   std::cout << process_prompt << "Loading config file... ";
   JSON config = JSON::parse(fileToString(CONFIG_FILE_PATH));
   std::cout << "Done." << std::endl;
   testSaveFolders(config);
   TimerColored timer(timer_prompt);
   [[maybe_unused]] TFile* histogramsNtuple = generateOutputNtuple(config);
   // std::map<std::string, EfficiencyPlotsModule::Scenario> scenarioStringToObjectMap {{"collisions", EfficiencyPlotsModule::Collisions}, {"cosmics", EfficiencyPlotsModule::Cosmics}};
   // EfficiencyPlotsModule::Scenario scenario = scenarioStringToObjectMap.at(config.at("scenario"));
   gROOT -> SetBatch(kFALSE);
   EventData       clusterEventField;
   Cluster         clusterField;
   EventData       trajEventField;
   TrajMeasurement trajField;
   // Run number and wbc setting
   bool filterForRunNumberPresent = config.at("filter_for_runs");
   int  runNumberLowerBound = NOVAL_I;
   int  runNumberUpperBound = NOVAL_I;
   // bool singleRun = false;
   std::string WBCSetting  = "WBC";
   if(filterForRunNumberPresent)
   {
      runNumberLowerBound  = config.at("run_filter_lower_bound").get<int>();
      runNumberUpperBound  = config.at("run_filter_upper_bound").get<int>();
      WBCSetting          += config.at("WBC_setting");
      // if(runNumberLowerBound + 1 == runNumberUpperBound) singleRun = true;
      if(runNumberUpperBound <= runNumberLowerBound)
      {
         std::cout << error_prompt << "Inconsistent run filter setup." << std::endl;
         exit(-1);
      }
   }
   // Histogram definitions
   std::cout << process_prompt << "Loading histogram definitions... ";
   std::cout << "Done." << std::endl;
   // Modules
   std::map<float, EfficiencyPlotsModule> delayToPlotterModuleMap;
   //////////////////
   // Cluster loop //
   //////////////////
   if(CLUST_LOOP_REQUESTED) try
   {
#ifdef RANDOM_DELAYS
      constexpr std::array<int, 5> delayTest {{-5, 7, 12, 13, 16}};
#endif
      TChain* clustTreeChain = new TChain("clustTree", "List of the clusters.");
      readInFilesAndAddToChain(config, "input_files_list", "input_files", clustTreeChain);
      clustTreeChain -> SetBranchAddress("event",  &clusterEventField);
      // clustTreeChain -> SetBranchAddress("mod",    &(clusterField.mod));
      clustTreeChain -> SetBranchAddress("mod_on", &(clusterField.mod_on));
      clustTreeChain -> SetBranchAddress("clust",  &clusterField);
      // check if data is present
      Long64_t clustTreeNumEntries  = clustTreeChain  -> GetEntries();
      std::cout << debug_prompt << "total entries in the clustTree chain: " << clustTreeNumEntries << std::endl;
      if(clustTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: clustTree.");
      ProgressBar progressBar;
      const int    progressBarUpdateInterval = 10000;
      const double progressBarUpdateBy       = progressBarUpdateInterval / static_cast<double>(clustTreeNumEntries) * 100;
      auto  updateAndPrintProgress =  [&] (const int& entryIndex)
      {
         if(entryIndex % progressBarUpdateInterval == 0)
         {
            progressBar.update(progressBarUpdateBy);
            progressBar.print();
            std::cout << " -- Estimated time left: " << std::setw(6) << std::fixed << std::setprecision(1) << timer.getSecondsElapsed() * (1 / progressBar.getProgressionRate() - 1) << " second(s).";
         }
      };
      timer.restart("Measuring the time required for looping on the clusters...");
      for(Long64_t entryIndex = 0; entryIndex < clustTreeNumEntries; ++entryIndex)
      {
         clustTreeChain -> GetEntry(entryIndex);
         if(filterForRunNumberPresent) if(clusterEventField.run <  runNumberLowerBound || runNumberUpperBound <= clusterEventField.run)
         {
            updateAndPrintProgress(entryIndex);
            continue;
         }
         [[maybe_unused]] static const int&  runNumber   = clusterEventField.run;
         [[maybe_unused]] static const int&  lumisection = clusterEventField.ls;
#ifdef RANDOM_DELAYS
         const float delayInNs   = delayTest[rand() % 5] * 25;
#else
         const        float delayInNs   = getDelayNs(runNumber, lumisection);
#endif
         auto plotterModuleIt = delayToPlotterModuleMap.find(delayInNs);
         // If key does not exist yet: add key
         if(plotterModuleIt == delayToPlotterModuleMap.end())
         {
            plotterModuleIt = delayToPlotterModuleMap.emplace(std::make_pair(delayInNs, EfficiencyPlotsModule(clusterEventField, clusterField, clusterEventField, trajField, delayInNs))).first;
         }
         plotterModuleIt -> second.fillClusterHistograms();
         updateAndPrintProgress(entryIndex);
      }
      std::cout << std::endl;
      timer.printSeconds("Loop done. Took about: ", " second(s).");
   }
   catch(const std::exception& e)
   {
      std::cout << error_prompt << "In the clusters loop: " << e.what() << " exception occured." << std::endl;
      exit(-1);
   }
   /////////////////////////////////
   // Trajectory measurement loop //
   /////////////////////////////////
   if(TRAJ_LOOP_REQUESTED) try
   {
#ifdef RANDOM_DELAYS
      constexpr std::array<int, 5> delayTest {{-5, 7, 12, 13, 16}};
#endif
      TChain* trajTreeChain  = new TChain("trajTree", "List of the trajectory measurements.");
      readInFilesAndAddToChain(config, "input_files_list", "input_files", trajTreeChain);
      // Trajectory measurement tree
      trajTreeChain -> SetBranchStatus ("mod",       0);
      trajTreeChain -> SetBranchAddress("event",     &trajEventField);
      trajTreeChain -> SetBranchAddress("mod_on",    &(trajField.mod_on));
      trajTreeChain -> SetBranchAddress("clust",     &(trajField.clu));
      trajTreeChain -> SetBranchAddress("track",     &(trajField.trk));
      trajTreeChain -> SetBranchAddress("clust_pix", &(trajField.clu.pix));
      trajTreeChain -> SetBranchAddress("traj",      &trajField);
      // check if data is present
      Long64_t trajTreeNumEntries  = trajTreeChain  -> GetEntries();
      std::cout << debug_prompt << "total entries in the trajTree chain: " << trajTreeNumEntries << std::endl;
      if(trajTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: trajTree.");
      ProgressBar progressBar;
      const int    progressBarUpdateInterval = 10000;
      const double progressBarUpdateBy       = progressBarUpdateInterval / static_cast<double>(trajTreeNumEntries) * 100;
      auto  updateAndPrintProgress =  [&] (const int& entryIndex)
      {
         if(entryIndex % progressBarUpdateInterval == 0)
         {
            progressBar.update(progressBarUpdateBy);
            progressBar.print();
            std::cout << " -- Estimated time left: " << std::setw(6) << std::fixed << std::setprecision(1) << timer.getSecondsElapsed() * (1 / progressBar.getProgressionRate() - 1) << " second(s).";
         }
      };
      timer.restart("Measuring the time required for looping on the trajectory measurements...");
      for(Long64_t entryIndex = 0; entryIndex < trajTreeNumEntries; ++entryIndex)
      {
         trajTreeChain -> GetEntry(entryIndex);
         int panelBefore = trajField.mod_on.panel;
         int diskBefore  = trajField.mod_on.disk;
         if(filterForRunNumberPresent) if(trajEventField.run < runNumberLowerBound || runNumberUpperBound <= trajEventField.run)
         {
            updateAndPrintProgress(entryIndex);
            continue;
         }
         [[maybe_unused]] const int&  runNumber   = trajEventField.run;
         [[maybe_unused]] const int&  lumisection = trajEventField.ls;
#ifdef RANDOM_DELAYS
         const float delayInNs   = delayTest[rand() % 5] * 25;
#else
         const float delayInNs   = getDelayNs(runNumber, lumisection);
#endif
         auto plotterModuleIt = delayToPlotterModuleMap.find(delayInNs);
         // If key does not exist yet: add key
         if(plotterModuleIt == delayToPlotterModuleMap.end())
         {
            std::cout << std::endl << process_prompt << "adding plot collection for a new delay scenario. Delay: " << delayInNs << std::endl;
            plotterModuleIt = delayToPlotterModuleMap.emplace(std::make_pair(delayInNs, EfficiencyPlotsModule(clusterEventField, clusterField, trajEventField, trajField, delayInNs))).first;
         }
         // std::cout << trajTreeChain -> GetCurrentFile() -> GetName() << std::endl;
         if(!(trajField.mod_on.panel == NOVAL_I || trajField.mod_on.panel == 1 || trajField.mod_on.panel == 2))
         {
            printTrajFieldInfo(trajField);
            std::cout << "disk before : " << diskBefore  << std::endl;
            std::cout << "panel before: " << panelBefore << std::endl;
            continue;
         }
         // std::cin.get();
         plotterModuleIt -> second.fillTrajMeasHistograms<SCENARIO>();
         updateAndPrintProgress(entryIndex);
      }
      std::cout << std::endl;
      timer.printSeconds("Loop done. Took about: ", " second(s).");
      for(auto& delayPlotterModulePair: delayToPlotterModuleMap)
      {
         delayPlotterModulePair.second.printCounters();
      }
   }
   catch(const std::exception& e)
   {
      std::cout << error_prompt << "In the traj meas loop: " << e.what() << " exception occured." << std::endl;
      exit(-1);
   }
   // Form a vector out of all the different modules
   std::vector<EfficiencyPlotsModule*> modules;
   modules.reserve(delayToPlotterModuleMap.size());
   for(auto& delayPlotterModulePair: delayToPlotterModuleMap)
   {
      modules.push_back(&(delayPlotterModulePair.second));
   }
   // Scale Efficiency plots
   {
      for(size_t moduleIndex: range(modules.size()))
      {
         modules[moduleIndex] -> downscaleEfficiencyPlots();
         modules[moduleIndex] -> addExtraEfficiencyPlots();
         std::cout << "Average efficiency for delay " << std::setprecision(1) << modules[moduleIndex] -> getDelayValueNs() << ": " << std::setprecision(6) << modules[moduleIndex] -> getAvarageEfficiency() << std::endl;
      }
   }
   // Print bad ROC list
   {
      EfficiencyPlotsModule::printCombinedBadROCList(delayToPlotterModuleMap, config.at("save_bad_ROC_list_output"));
   }
   try
   {
      // ------------------------------------------------------------------------------------------
      // Save plots unique to each of the delay scenarios

      std::cout << process_prompt << "Saving plots unique for each of the plotter modules... " << std::flush;
      std::string msg("");
      for(size_t moduleIndex: range(modules.size()))
      {
         msg = std::to_string(moduleIndex) + " out of " + std::to_string(modules.size()) + ".";
         std::cout << msg;
         modules[moduleIndex] -> savePlots(config, "");
         std::cout << std::string(msg.size(), '\b');
      }
      std::cout << std::string(msg.size(), ' ') << std::string(msg.size(), '\b') << "Done." << std::endl;

      // ------------------------------------------------------------------------------------------
      // Save efficiency vs delay plots on detector part level

      // Plots created here:
      //    - Delay vs cluster X size
      //    - Delay vs cluster Y size
      //    - Delay vs cluster total size
      //    - Delay vs cluster charge4
      // 
      // Detector part classification types:
      //    - Barrel or forward pixel
      //    - On barrel: layer and side (sign of Z coordinate)
      //    - On forward? disk and ring (inner or outer)
      //    - On barrel: side (sign of Z), half shell (sign of X), sector, [layer 1-2] or [layer 3-4]
      // 

      std::future<void> delayVsClusterPropertySaveProcess = std::async(EfficiencyPlotsModule::createEfficiencyVsDelayDefaultPlots, modules, config, DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE); 
      // std::future<void> delayVsClusterPropertySaveProcess = std::async(EfficiencyPlotsModule::createEfficiencyVsDelayDefaultPlots, modules, config, 0, 0.0f, 0.0f);
      std::cout << process_prompt << "Creating delay vs cluster property plots." << std::flush;
      std::chrono::milliseconds milliseconds_100(100);
      while(delayVsClusterPropertySaveProcess.wait_for(milliseconds_100) == std::future_status::timeout)
      {
         std::cout << '.' << std::flush;
      }
      delayVsClusterPropertySaveProcess.get(); // should do nothing, throws exception if the thread fails
      std::cout << "Done." << std::endl;      

      // ------------------------------------------------------------------------------------------
      // Save efficiency vs delay plots for each of the ROCs

      // Static function used to create efficiency vs delay plots for each of the ROC-s.
      // Takes a list of pointers to module objects, returns a vector of dynamically allocated 
      // TH1D histograms. The first vector index is the layer (0 for the forward region) the ROC is 
      // located on, the second is the bin number of the ROC on the histograms.
      // 
      // - Requires the "efficiencyROCPlots" to be filled before. (throws an std::runtime_error otherwise)
      // - Discards histograms where delayInNs_ is NOVAL_F (aka. -9999.0)

      std::cout << process_prompt << "Saving ROC plots... " << std::flush;
      std::vector<std::vector<TH1*>> delayVsEfficiencyOnROCs = EfficiencyPlotsModule::createEfficiencyVsDelayROCPlots(modules, DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
      histogramsNtuple -> cd();
      histogramsNtuple -> mkdir("ROCEfficiencies");
      for(size_t layerIndex = 0; layerIndex < 5; ++layerIndex)
      {
         std::string layerName = layerIndex == 0 ? "FPix" : "Layer_" + std::to_string(layerIndex);
         histogramsNtuple -> cd("ROCEfficiencies");
         gDirectory -> mkdir(layerName.c_str());
         gDirectory -> cd   (layerName.c_str());
         for(size_t binIndex = 0, binIndexMax = delayVsEfficiencyOnROCs[layerIndex].size(); binIndex < binIndexMax; ++binIndex)
         {
            delayVsEfficiencyOnROCs[layerIndex][binIndex] -> Write();
         }
      }
      std::cout << "Done." << std::endl;

      // ------------------------------------------------------------------------------------------
      // Save efficiency vs delay plots for each of the modules

       
   }
   catch(const std::exception& e)
   {
      std::cout << error_prompt << e.what() << std::endl;
   }
   std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
   return 0;
}
catch(const std::exception& e)
{
   std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
   return -1;
}

void testSaveFolders(const JSON& config)
{
   auto checkDirectory = [] (const std::string& directoryName)
   {
      try 
      {
         std::experimental::filesystem::path        filePath   = directoryName; 
         std::experimental::filesystem::file_status fileStatus = std::experimental::filesystem::status(filePath);
         if(!std::experimental::filesystem::is_directory(fileStatus))
         {
            std::cerr << "Error: Missing directory: " << directoryName << std::endl;
            std::cout << "Do you want to automatically create this missing directory: " << directoryName << "? (y/n): ";
            char answer = std::cin.get();
            if(answer == 'y')
            {
               std::experimental::filesystem::create_directory(filePath);
               std::cout << "Directory created succesfully." << std::endl;
            }
            else
            {
               exit(-1);
            }
         }
      }
      catch(std::experimental::filesystem::filesystem_error &e)
      {
         std::cerr << e.what() << std::endl;
      }
   }; 
   // Directories to check
   try { checkDirectory(config.at("plots_save_directory" )); } catch(...) { std::cout << error_prompt << "Configuration might require the definition of plots_save_directory..."  << std::endl; }
   try { checkDirectory(config.at("ntuple_save_directory")); } catch(...) { std::cout << error_prompt << "Configuration might require the definition of ntuple_save_directory..." << std::endl; }
}

TFile* generateOutputNtuple(const JSON& config)
{
   if(config.find("save_histograms_to_ntuple") == config.end())
   {
      std::cout << error_prompt << "In the configurations: \"save_histograms_to_ntuple\" is undefined." << std::endl;
      exit(-1);
   }
   if(config["save_histograms_to_ntuple"] == false) return nullptr;
   std::string ntupleSavePath;
   try
   {
      std::string saveDirname  = config.at("ntuple_save_directory");
      std::string saveFilename = config.at("ntuple_name");
      ntupleSavePath           = saveDirname + "/" + saveFilename;
      std::cout << process_prompt << "Will be saving the histograms as: " << ntupleSavePath << std::endl;
   }
   catch(...) { ntupleSavePath = "./Ntuple.root"; }
   std::time_t timeCreated = std::time(nullptr);
   std::string fileTitle = std::string("Efficiency filter calibrations output histograms, created at: ") + std::asctime(std::localtime(&timeCreated));
   return new TFile(ntupleSavePath.c_str(), "RECREATE", fileTitle.c_str());
}

std::vector<std::string> getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey)
{
   std::string inputFilesListPath = config[configKey];
   JSON inputListJSON = JSON::parse(fileToString(inputFilesListPath));
   return inputListJSON[innerKey];  
}

void readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain)
{
   std::vector<std::string> inputFiles = getFilesFromConfig(config, configKey, innerKey);
   std::for_each(inputFiles.begin(), inputFiles.end(), [&] (const std::string& fileName) { chain  -> Add(fileName.c_str()); });
}

template <typename T>
T checkGetElement(const JSON& definition, const std::string& propertyName)
{
   auto propertyIt = definition.find(propertyName);
   if(propertyIt == definition.end())
   {
      if(definition.count("name") == 0) 
      {
         std::cout << error_prompt << "Element of unknown name has one or more missing properties..." << std::endl;
         exit(-1);
      }
      std::string name = definition["name"];
      std::cout << error_prompt << "Cant find property: " << propertyName << " for " << name << "." << std::endl;
      exit(-1);
   }
   return propertyIt.value();
}

float getDelayNs(int runNumber, int lumisectionNumber) try
{
   if(runNumber == NOVAL_F || runNumber == 1) return NOVAL_F;
   static const std::map<int, std::map<int, float>> delayList =
   {
      // (164 - WBC) * 25 + Globaldelay

      // Fill 5824
      // { 296665, {{ -1, 0 * 25 + 18 }}},
      // { 296666, {{ -1, 0 * 25 + 14 }}},
      // { 296668, {{ -1, 0 * 25 + 11 }}},
      // { 296669, {{ -1, 0 * 25 + 9  }}},
      // { 296674, {{ -1, 0 * 25 + 7.5}}},
      // { 296664, {{ -1, 0 * 25 + 6  }}},
      // { 296675, {{ -1, 0 * 25 + 4.5}}},
      // { 296676, {{ -1, 0 * 25 + 1.5}}},
      // { 296678, {{ -1, 0 * 25 + 0  }}},
      // { 296679, {{ -1, -1 * 25 + 22 }}},
      // { 296680, {{ -1, -1 * 25 + 18 }}}

      // Fill 5856, scan 23
      { 297281, {{ -1, 0 * 25 +     6}}},
      { 297282, {{ -1, 0 * 25 +    -4}}},
      { 297283, {{ -1, 0 * 25 +    -1}}},
      { 297284, {{ -1, 0 * 25 +     2}}},
      { 297285, {{ -1, 0 * 25 +     4}}},
      { 297286, {{ -1, 0 * 25 +     8}}},
      { 297287, {{ -1, 0 * 25 +    10}}},
      { 297288, {{ -1, 0 * 25 +    13}}},
      { 297289, {{ -1, 0 * 25 +    16}}},
      { 297290, {{ -1, 0 * 25 +   5.5}}},
      { 297291, {{ -1, 0 * 25 +   6.5}}}

      // Fill 5838

      // { 297003, {{ -1, 44 * 25 + normal settings}}
      // { 297004, {{ -1, 64 * 25 + 18ns}}
      // { 297006, {{ -1, 64 * 25 + 21ns}}
      // { 297007, {{ -1, 62 * 25 + 0ns}}
      // { 297009, {{ -1, 62 * 25 + 1.5ns}}
      // { 297010, {{ -1, 62 * 25 + 3ns}}
      // { 297011, {{ -1, 62 * 25 + 4.5ns}}
      // { 297020, {{ -1, 62 * 25 + 6ns}}
      // { 297012, {{ -1, 62 * 25 + 7.5ns}}
      // { 297015, {{ -1, 62 * 25 + 9ns}}
      // { 297016, {{ -1, 62 * 25 + 10.5ns}}
      // { 297017, {{ -1, 62 * 25 + 12ns}}
      // { 297018, {{ -1, 62 * 25 + 15ns}}
      // { 297019, {{ -1, 62 * 25 + 18ns}}

      // { 294928, {{-1, 162 * 25  - 6}}},
      // { 294929, {{-1, 163 * 25  - 6}}},
      // { 294931, {{-1, 164 * 25  - 6}}},
      // { 294932, {{-1, 162 * 25  - 18}}},
      // { 294933, {{-1, 163 * 25  - 18}}},
      // { 294934, {{-1, 164 * 25  - 18}}},
      // { 294935, {{-1, 165 * 25  - 18}}},
      // { 294936, {{-1, 164 * 25  - 0}}},
      // { 294937, {{-1, 162 * 25  - 0}}},
      // { 294939, {{-1, 164 * 25  - 12}}},
      // { 294940, {{-1, 163 * 25  - 12}}},
      // { 294947, {{-1, 163 * 25  - 0}}},
      // { 294949, {{-1, 162 * 25  - 3}}},
      // { 294950, {{-1, 163 * 25  - 21}}},
      // { 294951, {{-1, 163 * 25  - 15}}},
      // { 294952, {{-1, 163 * 25  - 9}}},
      // { 294953, {{-1, 163 * 25  - 3}}},
      // { 294954, {{-1, 164 * 25  - 21}}},
      // { 294955, {{-1, 164 * 25  - 15}}},
      // { 294956, {{-1, 164 * 25  - 3}}},
      // { 294957, {{-1, 163 * 25  - 0}}},
      // { 294960, {{-1, 163 * 25  - 6}}}

      // FPix
      // { 291872, {{ -1, 162.0f * 25 - 12 }}},
      // { 292283, {{ -1, 164.0f * 25 - 18 }}},
      // { 292364, {{ -1, 164.0f * 25 - 6 }}},
      // { 292365, {{ -1, 164.0f * 25 - 6 }}},
      // { 292366, {{ -1, 164.0f * 25 - 6 }}},
      // { 292367, {{ -1, 164.0f * 25 - 6 }}},
      // { 292592, {{ -1, 162.0f * 25 - 6 }}},
      // { 292593, {{ -1, 163.0f * 25 - 6 }}},
      // { 292594, {{ -1, 163.0f * 25 - 6 }}},
      // // //BPix
      // { 292079, {{ -1, 163.0f * 25 - 18 }}},
      // { 292080, {{ -1, 163.0f * 25 - 18 }}},
      // { 292283, {{ -1, 165.0f * 25 - 18 }}},
      // { 292364, {{ -1, 164.0f * 25 - 6 }}},
      // { 292365, {{ -1, 164.0f * 25 - 6 }}},
      // { 292366, {{ -1, 164.0f * 25 - 6 }}},
      // { 292367, {{ -1, 164.0f * 25 - 6 }}},
      // { 292592, {{ -1, 162.0f * 25 - 6 }}},
      // { 292593, {{ -1, 163.0f * 25 - 6 }}},
      // { 292594, {{ -1, 163.0f * 25 - 6 }}}
   };
   const std::map<int, float>& runLsToDelayMap = delayList.at(runNumber);
   const auto& universalLumisectionIt = runLsToDelayMap.find(-1);
   if(universalLumisectionIt != runLsToDelayMap.end()) return universalLumisectionIt -> second;
   return runLsToDelayMap.at(lumisectionNumber);
}
catch(const std::out_of_range& e)
{
   return NOVAL_F;
}

std::pair<int, int> delayInNsToRelativeWBCandPhaseDelay(float delayInNs)
{
   int valueAsInt = (delayInNs + 0.5f);
   return std::make_pair(valueAsInt / 25, valueAsInt % 25);
}

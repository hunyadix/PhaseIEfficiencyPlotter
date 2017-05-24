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
#include <boost/filesystem.hpp>
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

#define RANDOM_DELAYS

constexpr float HALF_PI = 0.5 * 3.141592653589793238462;

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);
constexpr auto                    CONFIG_FILE_PATH                = "./config_main.json"; 
const     std::string             EFFICIENCY_PLOT_IDENTIFIER      = "Efficiency";
const     std::string             EFFICIENCY_NUMERATOR_IDENTIFIER = "Numhits";

const     int   DELAY_PLOTS_NUM_BINS                             = 20 * 25;
const     float DELAY_PLOTS_LOWER_EDGE                           = 153;
const     float DELAY_PLOTS_UPPER_EDGE                           = 173;

const bool CLUST_LOOP_REQUESTED = false;
const bool TRAJ_LOOP_REQUESTED  = true;
constexpr EfficiencyPlotsModule::Scenario SCENARIO = EfficiencyPlotsModule::Collisions;
// constexpr EfficiencyPlotsModule::Scenario SCENARIO = EfficiencyPlotsModule::Cosmics;

void                                        testSaveFolders(const JSON& config);
TFile*                                      generateOutputNtuple(const JSON& config);
std::vector<std::string>                    getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey);
void                                        readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);
TGraphAsymmErrors*                          getGraphForEfficiencyWithAsymmetricErrors(const TH1D& efficiencyHistogram, const TH1D& numHitsHistogram);
float                                       getDelayNs(int runNumber, int lumisectionNumber);

int main(int argc, char** argv) try
{
   std::cout << process_prompt << argv[0] << " started..." << std::endl;
   std::time_t processStarted = std::time(nullptr);
   std::cout << timer_prompt << "Time: " << std::asctime(std::localtime(&processStarted)) << std::flush;
   std::cout << process_prompt << "Loading config file... ";
   JSON config = JSON::parse(fileToString(CONFIG_FILE_PATH));
   std::cout << "Done." << std::endl;
   testSaveFolders(config);
   TimerColored timer(timer_prompt);
   TFile* histogramsNtuple = generateOutputNtuple(config);
   (void)histogramsNtuple; // To disable the warning
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
      constexpr std::array<int, 5> delayTest {{157, 160, 161, 163, 165}};
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
         static const int&  runNumber   = clusterEventField.run;
         static const int&  lumisection = clusterEventField.ls;
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
   ////////////////////////////////
   // Trajector measurement loop //
   ////////////////////////////////
   if(TRAJ_LOOP_REQUESTED) try
   {
#ifdef RANDOM_DELAYS
      constexpr std::array<int, 5> delayTest {{157, 160, 161, 163, 165}};
#endif
      TChain* trajTreeChain  = new TChain("trajTree", "List of the trajectory measurements.");
      readInFilesAndAddToChain(config, "input_files_list", "input_files", trajTreeChain);
      // Trajectory measurement tree
      trajTreeChain -> SetBranchAddress("event",     &trajEventField);
      // trajTreeChain -> SetBranchAddress("mod",       &(trajField.mod));
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
         if(filterForRunNumberPresent) if(trajEventField.run < runNumberLowerBound || runNumberUpperBound <= trajEventField.run)
         {
            updateAndPrintProgress(entryIndex);
            continue;
         }
         static const int&  runNumber   = trajEventField.run;
         static const int&  lumisection = trajEventField.ls;
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
   ////////////////////////////
   // Scale Efficiency plots //
   ////////////////////////////
   {
      for(auto& delayPlotterModulePair: delayToPlotterModuleMap)
      {
         delayPlotterModulePair.second.downscaleEfficiencyPlots();
         delayPlotterModulePair.second.addExtraEfficiencyPlots();
         std::cout << "Average efficiency for delay " << std::setprecision(1) << delayPlotterModulePair.first << ": " << std::setprecision(6) << delayPlotterModulePair.second.getAvarageEfficiency() << std::endl;
      }
   }
   ////////////////
   // Save plots //
   ////////////////
   try
   {
      for(auto& delayPlotterModulePair: delayToPlotterModuleMap)
      {
         // std::stringstream directoryNameStream;
         // directoryNameStream << "Delay_" << std::fixed << std::setprecision(2) << delayPlotterModulePair.first;
         // std::string directoryName = directoryNameStream.str();
         delayPlotterModulePair.second.savePlots(config, "");
      }
   }
   catch(const std::exception& e)
   {
      std::cout << error_prompt << "While saving histograms: " << e.what() << " exception occured." << std::endl;
      exit(-1);
   }
   ///////////////////////////////
   // Efficiency vs delay plots //
   ///////////////////////////////
   {
      std::vector<std::string> labelsWBC;
      for(int wbcSetting = DELAY_PLOTS_LOWER_EDGE; wbcSetting < DELAY_PLOTS_UPPER_EDGE; wbcSetting++)
      {
         for(int nanosec = 0; nanosec < 25; nanosec += 5)
         {
            labelsWBC.push_back("WBC" + std::to_string(wbcSetting) + ":++" + std::to_string(nanosec) + "ns");
         }
      }
      std::vector<TH1D*> delayVsClusterSizeXBpixFpix                  ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterSizeXBpixFpixHits              ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterSizeXLayersNegativePositive    ( 8, nullptr);
      std::vector<TH1D*> delayVsClusterSizeXLayersNegativePositiveHits( 8, nullptr);
      std::vector<TH1D*> delayVsClusterSizeXDisksInnerOuter           (12, nullptr);
      std::vector<TH1D*> delayVsClusterSizeXDisksInnerOuterHits       (12, nullptr);
      std::vector<TH1D*> delayVsClusterSizeXBNPZHSSIOLP               (64, nullptr);
      std::vector<TH1D*> delayVsClusterSizeXBNPZHSSIOLPHits           (64, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYBpixFpix                  ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYBpixFpixHits              ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYLayersNegativePositive    ( 8, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYLayersNegativePositiveHits( 8, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYDisksInnerOuter           (12, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYDisksInnerOuterHits       (12, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYBNPZHSSIOLP               (64, nullptr);
      std::vector<TH1D*> delayVsClusterSizeYBNPZHSSIOLPHits           (64, nullptr);
      std::vector<TH1D*> delayVsEfficiencyBpixFpix                    ( 2, nullptr);
      std::vector<TH1D*> delayVsEfficiencyBpixFpixHits                ( 2, nullptr);
      std::vector<TH1D*> delayVsEfficiencyLayersNegativePositive      ( 8, nullptr);
      std::vector<TH1D*> delayVsEfficiencyLayersNegativePositiveHits  ( 8, nullptr);
      std::vector<TH1D*> delayVsEfficiencyDisksInnerOuter             (12, nullptr);
      std::vector<TH1D*> delayVsEfficiencyDisksInnerOuterHits         (12, nullptr);
      std::vector<TH1D*> delayVsEfficiencyBNPZHSSIOLP                 (64, nullptr);
      std::vector<TH1D*> delayVsEfficiencyBNPZHSSIOLPHits             (64, nullptr);
      for(unsigned int det = 0; det <= 1; ++det)
      {
         static const std::vector<std::string> detStrings  = {"BPix", "FPix"};
         std::string detAsString = detStrings[det];
         // Cluster Size X
         delayVsClusterSizeXBpixFpix[det] = new TH1D(
            ("delayVsClusterSizeX" + detAsString).c_str(),
            ("Delay vs cluster X size on " + detAsString + ";delay;cluster X size (pix)").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         delayVsClusterSizeXBpixFpixHits[det] = new TH1D(
            ("delayVsClusterSizeXHits" + detAsString).c_str(),
            ("Number of clustres for delay scenarios on " + detAsString + ";delay;clusters").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         // Cluster Size Y
         delayVsClusterSizeYBpixFpix[det] = new TH1D(
            ("delayVsClusterSizeY" + detAsString).c_str(),
            ("Delay vs cluster Y size on " + detAsString + ";delay;cluster Y size (pix)").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         delayVsClusterSizeYBpixFpixHits[det] = new TH1D(
            ("delayVsClusterSizeYHits" + detAsString).c_str(),
            ("Number of clustres for delay scenarios on " + detAsString + ";delay;clusters").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         // Efficiency
         delayVsEfficiencyBpixFpix[det] = new TH1D(
            ("delayVsEfficiency" + detAsString).c_str(),
            ("Delay vs efficiency on " + detAsString + ";delay;efficiency").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         delayVsEfficiencyBpixFpixHits[det] = new TH1D(
            ("delayVsEfficiencyHits" + detAsString).c_str(),
            ("Number of eff. hits for delay scenarios on " + detAsString + ";delay;hits").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         delayVsEfficiencyBpixFpix[det] -> GetXaxis() -> SetNdivisions(500 + DELAY_PLOTS_NUM_BINS, kFALSE);
         for(unsigned int numBin = 0; numBin < labelsWBC.size(); ++numBin)
         {
            delayVsEfficiencyBpixFpix[det] -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, labelsWBC[numBin].c_str());
         }
      }
      for(unsigned int layer = 1; layer <= 4; ++layer)
      {
         for(unsigned int side = 1; side <= 2; ++side)
         {
            static const std::vector<std::string> sideStrings  = {"Negative", "Positive"};
            std::string layerAsString = std::to_string(layer);
            std::string sideAsString  = sideStrings[side - 1];
            // Cluster Size X
            delayVsClusterSizeXLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizeXLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs cluster X size on layer " + layerAsString + ", " + sideAsString + " side;delay;cluster X size (pix)").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsClusterSizeXLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizeXLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of clusters for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;clusters").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            // Cluster Size Y
            delayVsClusterSizeYLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizeYLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs cluster Y size on layer " + layerAsString + ", " + sideAsString + " side;delay;cluster Y size (pix)").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsClusterSizeYLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizeYLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of clusters for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;clusters").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            // Efficiency
            delayVsEfficiencyLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsEfficiencyLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs efficiency on layer " + layerAsString + ", " + sideAsString + " side;delay;efficiency").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsEfficiencyLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of eff. hits for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;hits").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyLayersNegativePositive[(layer - 1) * 2 + side - 1] -> GetXaxis() -> SetNdivisions(500 + DELAY_PLOTS_NUM_BINS, kFALSE);
            for(unsigned int numBin = 0; numBin < labelsWBC.size(); ++numBin)
            {
               delayVsEfficiencyLayersNegativePositive[(layer - 1) * 2 + side - 1] -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, labelsWBC[numBin].c_str());
               // std::cout << "Label in vector: " << labelsWBC[numBin] << std::endl;
               // std::cout << "Label on histogram: " << delayVsEfficiencyLayersNegativePositive[(layer - 1) * 2 + side - 1] -> GetXaxis() -> GetBinLabel(numBin + 1) << std::endl;
            }
         }
      }
      for(int disk = -3; disk <=3; ++disk)
      {
         if(disk == 0) continue;
         for(unsigned int ring = 1; ring <= 2; ++ring)
         {
            static const std::vector<std::string> sideStrings  = {"Negative", "Positive"};
            static const std::vector<std::string> ringStrings  = {"Inner", "Outer"};
            std::string diskAsString = std::to_string(std::abs(disk));
            std::string sideAsString = sideStrings[1 <= disk];
            std::string ringAsString = ringStrings[ring - 1];
            int histogramIndex = (0 <= disk) * 6 + (std::abs(disk) - 1) * 2 + ring - 1;
            // Cluster Size X
            delayVsClusterSizeXDisksInnerOuter[histogramIndex] = new TH1D(
               ("delayVsClusterSizeXDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs cluster X size on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;cluster X size (pix)" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsClusterSizeXDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsClusterSizeXDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of clusters for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;clusters" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsClusterSizeXDisksInnerOuter[histogramIndex] -> GetXaxis() -> SetNdivisions(510, kFALSE);
            // Cluster Size Y
            delayVsClusterSizeYDisksInnerOuter[histogramIndex] = new TH1D(
               ("delayVsClusterSizeYDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs cluster Y size on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;cluster Y size (pix)" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsClusterSizeYDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsClusterSizeYDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of clusters for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;clusters" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsClusterSizeYDisksInnerOuter[histogramIndex] -> GetXaxis() -> SetNdivisions(510, kFALSE);
            // Efficiency
            delayVsEfficiencyDisksInnerOuter[histogramIndex] = new TH1D(
               ("delayVsEfficiencyDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs efficiency on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;efficiency" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsEfficiencyDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of eff. hits for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;hits" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyDisksInnerOuter[histogramIndex] -> GetXaxis() -> SetNdivisions(500 + DELAY_PLOTS_NUM_BINS, kFALSE);
            for(unsigned int numBin = 0; numBin < labelsWBC.size(); ++numBin)
            {
               delayVsEfficiencyDisksInnerOuter[histogramIndex] -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, labelsWBC[numBin].c_str());
            }
         }
      }
      for(unsigned int side = 1; side <= 2; ++side)
      {
         for(unsigned int halfShell = 1; halfShell <= 2; ++halfShell)
         {
            for(unsigned int layerPair = 1; layerPair <= 2; ++layerPair)
            {
               for(unsigned int sec = 1; sec <= 8; ++sec)
               {
                  static const std::vector<std::string> sideStrings  = {"Negative", "Positive"};
                  static const std::vector<std::string> layerPairStrings  = {"1 and 2", "3 and 4"};
                  static const std::vector<std::string> layerPairStringsShort  = {"1and2", "3and4"};
                  std::string sideAsString = sideStrings[side - 1];
                  std::string secAsString  = std::to_string(sec);
                  std::string halfShellAsString  = sideStrings[halfShell - 1];
                  std::string layerPairAsString = layerPairStrings[layerPair - 1];
                  std::string layerPairAsShortString = layerPairStringsShort[layerPair - 1];
                  int plotIndex = (side - 1) * 32 + (halfShell - 1) * 16 + (layerPair - 1) * 8 + (sec - 1);
                  // Cluster Size X
                  delayVsClusterSizeXBNPZHSSIOLP[plotIndex] = new TH1D(
                     ("delayVsClusterSizeX" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs cluster X size on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;cluster X size (pix)").c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  delayVsClusterSizeXBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsClusterSizeXHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of clusters for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;clusters").c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  // Cluster Size Y
                  delayVsClusterSizeYBNPZHSSIOLP[plotIndex] = new TH1D(
                     ("delayVsClusterSizeY" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs cluster Y size on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;cluster Y size (pix)").c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  delayVsClusterSizeYBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsClusterSizeYHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of clusters for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;clusters").c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  // Efficiency
                  delayVsEfficiencyBNPZHSSIOLP[plotIndex] = new TH1D(
                     ("delayVsEfficiency" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs efficiency on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;efficiency").c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  delayVsEfficiencyBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsEfficiencyHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of eff. hits for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;hits").c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  delayVsEfficiencyBNPZHSSIOLP[plotIndex] -> GetXaxis() -> SetNdivisions(500 + DELAY_PLOTS_NUM_BINS, kFALSE);
                  for(unsigned int numBin = 0; numBin < labelsWBC.size(); ++numBin)
                  {
                     delayVsEfficiencyBNPZHSSIOLP[plotIndex] -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, labelsWBC[numBin].c_str());
                  }
               }
            }
         }
      }
      for(auto& delayPlotterModulePair: delayToPlotterModuleMap)
      {
         const float& delay = delayPlotterModulePair.first;
         // Cluster Size X
         const std::array<std::pair<int, int>, 2>*  clusterSizeXBpixFpix               = delayPlotterModulePair.second.getClusterSizeXBpixFpix();
         const std::array<std::pair<int, int>, 8>*  clusterSizeXLayersNegativePositive = delayPlotterModulePair.second.getClusterSizeXLayersNegativePositive();
         const std::array<std::pair<int, int>, 12>* clusterSizeXDisksInnerOuter        = delayPlotterModulePair.second.getClusterSizeXDisksInnerOuter();
         const std::array<std::pair<int, int>, 64>* clusterSizeXBNPZHSSIOLP            = delayPlotterModulePair.second.getClusterSizeXBNPZHSSIOLP();
         // Cluster Size Y
         const std::array<std::pair<int, int>, 2>*  clusterSizeYBpixFpix               = delayPlotterModulePair.second.getClusterSizeYBpixFpix();
         const std::array<std::pair<int, int>, 8>*  clusterSizeYLayersNegativePositive = delayPlotterModulePair.second.getClusterSizeYLayersNegativePositive();
         const std::array<std::pair<int, int>, 12>* clusterSizeYDisksInnerOuter        = delayPlotterModulePair.second.getClusterSizeYDisksInnerOuter();
         const std::array<std::pair<int, int>, 64>* clusterSizeYBNPZHSSIOLP            = delayPlotterModulePair.second.getClusterSizeYBNPZHSSIOLP();
         // Efficiency
         const std::array<std::pair<int, int>, 2>*  efficiencyBpixFpix                 = delayPlotterModulePair.second.getEfficiencyBpixFpix();
         const std::array<std::pair<int, int>, 8>*  efficiencyLayersNegativePositive   = delayPlotterModulePair.second.getEfficiencyLayersNegativePositive();
         const std::array<std::pair<int, int>, 12>* efficiencyDisksInnerOuter          = delayPlotterModulePair.second.getEfficiencyDisksInnerOuter();
         const std::array<std::pair<int, int>, 64>* efficiencyBNPZHSSIOLP              = delayPlotterModulePair.second.getEfficiencyBNPZHSSIOLP();
         for(unsigned int plotIndex = 0; plotIndex < 2; ++plotIndex)
         {
            // Cluster Size X
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeXBpixFpix[plotIndex]     -> Fill(delay / 25, (*clusterSizeXBpixFpix)[plotIndex].second / static_cast<double>((*clusterSizeXBpixFpix)[plotIndex].first));
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeXBpixFpixHits[plotIndex] -> Fill(delay / 25, (*clusterSizeXBpixFpix)[plotIndex].first);
            // Cluster Size Y
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeYBpixFpix[plotIndex]     -> Fill(delay / 25, (*clusterSizeYBpixFpix)[plotIndex].second / static_cast<double>((*clusterSizeYBpixFpix)[plotIndex].first));
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeYBpixFpixHits[plotIndex] -> Fill(delay / 25, (*clusterSizeYBpixFpix)[plotIndex].first);
            // Efficiency
            if((*efficiencyBpixFpix)[plotIndex]  .first != 0) delayVsEfficiencyBpixFpix[plotIndex]       -> Fill(delay / 25, (*efficiencyBpixFpix)[plotIndex].second / static_cast<double>((*efficiencyBpixFpix)[plotIndex].first));
            if((*efficiencyBpixFpix)[plotIndex]  .first != 0) delayVsEfficiencyBpixFpixHits[plotIndex]   -> Fill(delay / 25, (*efficiencyBpixFpix)[plotIndex].first);
         }
         for(unsigned int plotIndex = 0; plotIndex < 8; ++plotIndex)
         {
            // Cluster Size X
            if((*clusterSizeXLayersNegativePositive)[plotIndex].first != 0) delayVsClusterSizeXLayersNegativePositive[plotIndex]     -> Fill(delay / 25, (*clusterSizeXLayersNegativePositive)[plotIndex].second / static_cast<double>((*clusterSizeXLayersNegativePositive)[plotIndex].first));
            if((*clusterSizeXLayersNegativePositive)[plotIndex].first != 0) delayVsClusterSizeXLayersNegativePositiveHits[plotIndex] -> Fill(delay / 25, (*clusterSizeXLayersNegativePositive)[plotIndex].first);
            // Cluster Size Y
            if((*clusterSizeYLayersNegativePositive)[plotIndex].first != 0) delayVsClusterSizeYLayersNegativePositive[plotIndex]     -> Fill(delay / 25, (*clusterSizeYLayersNegativePositive)[plotIndex].second / static_cast<double>((*clusterSizeYLayersNegativePositive)[plotIndex].first));
            if((*clusterSizeYLayersNegativePositive)[plotIndex].first != 0) delayVsClusterSizeYLayersNegativePositiveHits[plotIndex] -> Fill(delay / 25, (*clusterSizeYLayersNegativePositive)[plotIndex].first);
            // Efficiency
            if((*efficiencyLayersNegativePositive)[plotIndex].first != 0) delayVsEfficiencyLayersNegativePositive[plotIndex]     -> Fill(delay / 25, (*efficiencyLayersNegativePositive)[plotIndex].second / static_cast<double>((*efficiencyLayersNegativePositive)[plotIndex].first));
            if((*efficiencyLayersNegativePositive)[plotIndex].first != 0) delayVsEfficiencyLayersNegativePositiveHits[plotIndex] -> Fill(delay / 25, (*efficiencyLayersNegativePositive)[plotIndex].first);
         }
         for(unsigned int plotIndex = 0; plotIndex < 12; ++plotIndex)
         {
            // Cluster Size X
            if((*clusterSizeXDisksInnerOuter)[plotIndex].first != 0) delayVsClusterSizeXDisksInnerOuter[plotIndex]      -> Fill(delay / 25, (*clusterSizeXDisksInnerOuter)[plotIndex].second / static_cast<double>((*clusterSizeXDisksInnerOuter)[plotIndex].first));
            if((*clusterSizeXDisksInnerOuter)[plotIndex].first != 0) delayVsClusterSizeXDisksInnerOuterHits[plotIndex]  -> Fill(delay / 25, (*clusterSizeXDisksInnerOuter)[plotIndex].first);
            // Cluster Size Y
            if((*clusterSizeYDisksInnerOuter)[plotIndex].first != 0) delayVsClusterSizeYDisksInnerOuter[plotIndex]      -> Fill(delay / 25, (*clusterSizeYDisksInnerOuter)[plotIndex].second / static_cast<double>((*clusterSizeYDisksInnerOuter)[plotIndex].first));
            if((*clusterSizeYDisksInnerOuter)[plotIndex].first != 0) delayVsClusterSizeYDisksInnerOuterHits[plotIndex]  -> Fill(delay / 25, (*clusterSizeYDisksInnerOuter)[plotIndex].first);
            // Efficiency
            if((*efficiencyDisksInnerOuter)[plotIndex].first != 0) delayVsEfficiencyDisksInnerOuter[plotIndex]      -> Fill(delay / 25, (*efficiencyDisksInnerOuter)[plotIndex].second / static_cast<double>((*efficiencyDisksInnerOuter)[plotIndex].first));
            if((*efficiencyDisksInnerOuter)[plotIndex].first != 0) delayVsEfficiencyDisksInnerOuterHits[plotIndex]  -> Fill(delay / 25, (*efficiencyDisksInnerOuter)[plotIndex].first);
         }
         for(unsigned int plotIndex = 0; plotIndex < 64; ++plotIndex)
         {
            // Cluster Size X
            if((*clusterSizeXBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterSizeXBNPZHSSIOLP[plotIndex]     -> Fill(delay / 25, (*clusterSizeXBNPZHSSIOLP)[plotIndex].second / static_cast<double>((*clusterSizeXBNPZHSSIOLP)[plotIndex].first));
            if((*clusterSizeXBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterSizeXBNPZHSSIOLPHits[plotIndex] -> Fill(delay / 25, (*clusterSizeXBNPZHSSIOLP)[plotIndex].first);
            // Cluster Size Y
            if((*clusterSizeYBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterSizeYBNPZHSSIOLP[plotIndex]     -> Fill(delay / 25, (*clusterSizeYBNPZHSSIOLP)[plotIndex].second / static_cast<double>((*clusterSizeYBNPZHSSIOLP)[plotIndex].first));
            if((*clusterSizeYBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterSizeYBNPZHSSIOLPHits[plotIndex] -> Fill(delay / 25, (*clusterSizeYBNPZHSSIOLP)[plotIndex].first);
            // Efficiency
            if((*efficiencyBNPZHSSIOLP)[plotIndex].first != 0) delayVsEfficiencyBNPZHSSIOLP[plotIndex]     -> Fill(delay / 25, (*efficiencyBNPZHSSIOLP)[plotIndex].second / static_cast<double>((*efficiencyBNPZHSSIOLP)[plotIndex].first));
            if((*efficiencyBNPZHSSIOLP)[plotIndex].first != 0) delayVsEfficiencyBNPZHSSIOLPHits[plotIndex] -> Fill(delay / 25, (*efficiencyBNPZHSSIOLP)[plotIndex].first);
         }
      }
      gStyle -> SetPalette(kVisibleSpectrum);
      gStyle -> SetNumberContours(999);
      gStyle -> SetOptStat(1112211); // Integral overflow Underflow RMS (value 2: +error) Mean (value 2: +error) Entries Title
      gErrorIgnoreLevel = kError;
      gDirectory -> cd("/");
      gDirectory -> mkdir("Cluster_size_X_vs_delay");
      gDirectory -> mkdir("Cluster_size_Y_vs_delay");
      gDirectory -> mkdir("Efficiency_vs_delay");
      gDirectory -> mkdir("Efficiency_vs_delay/Graphs");
      // Cluster Size X
      gDirectory -> cd("Cluster_size_X_vs_delay");
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeXBpixFpix,               "" , "Cluster_size_X_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeXLayersNegativePositive, "" , "Cluster_size_X_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeXDisksInnerOuter,        "" , "Cluster_size_X_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeXBNPZHSSIOLP,            "" , "Cluster_size_X_vs_delay", config);
      // Cluster Size Y
      gDirectory -> cd("../Cluster_size_Y_vs_delay");
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeYBpixFpix,               "" , "Cluster_size_Y_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeYLayersNegativePositive, "" , "Cluster_size_Y_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeYDisksInnerOuter,        "" , "Cluster_size_Y_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizeYBNPZHSSIOLP,            "" , "Cluster_size_Y_vs_delay", config);
      // Efficiency
      gDirectory -> cd("../Efficiency_vs_delay");
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyBpixFpix,               "" , "Efficiency_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyLayersNegativePositive, "" , "Efficiency_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyDisksInnerOuter,        "" , "Efficiency_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyBNPZHSSIOLP,            "" , "Efficiency_vs_delay", config);
      gDirectory -> cd("/");
      gDirectory -> cd("Efficiency_vs_delay");
      gDirectory -> cd("Graphs");
      if(config["save_histograms_to_ntuple"] == true)
      {
         gDirectory -> mkdir("BpixFpix");
         gDirectory -> mkdir("LayersNegativePositive");
         gDirectory -> mkdir("DisksInnerOuter");
         gDirectory -> mkdir("LayerDetailed");
         gDirectory -> cd("BpixFpix");
         for(unsigned int plotIndex = 0; plotIndex < 2; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyBpixFpix[plotIndex], delayVsEfficiencyBpixFpixHits[plotIndex], (2 + plotIndex * 2));
         gDirectory -> cd("../LayersNegativePositive");
         for(unsigned int plotIndex = 0; plotIndex < 8; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyLayersNegativePositive[plotIndex], delayVsEfficiencyLayersNegativePositiveHits[plotIndex], (1 + plotIndex));
         gDirectory -> cd("../DisksInnerOuter");
         for(unsigned int plotIndex = 0; plotIndex < 12; ++plotIndex) EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyDisksInnerOuter[plotIndex], delayVsEfficiencyDisksInnerOuterHits[plotIndex], (1 + plotIndex) % 8 == 0 ? 9 : (1 + plotIndex) % 8);
         gDirectory -> cd("../LayerDetailed");
         for(unsigned int plotIndex = 0; plotIndex < 64; ++plotIndex) 
         {
            EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyBNPZHSSIOLP[plotIndex], delayVsEfficiencyBNPZHSSIOLPHits[plotIndex], 1 + plotIndex % 8);
            // std::cout << delayVsEfficiencyBNPZHSSIOLP[plotIndex] -> GetName() << std::endl;
         }
      }
      // std::cout << delayVsEfficiencyBpixFpix[0] -> GetXaxis() -> GetNdivisions() << std::endl;
      // delayVsEfficiencyBpixFpix[0] -> SaveAs("test.C");
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
         boost::filesystem::path        filePath   = directoryName; 
         boost::filesystem::file_status fileStatus = boost::filesystem::status(filePath);
         if(!boost::filesystem::is_directory(fileStatus))
         {
            std::cerr << "Error: Missing directory: " << directoryName << std::endl;
            std::cout << "Do you want to automatically create this missing directory: " << directoryName << "? (y/n): ";
            char answer = std::cin.get();
            if(answer == 'y')
            {
               boost::filesystem::create_directory(filePath);
               std::cout << "Directory created succesfully." << std::endl;
            }
            else
            {
               exit(-1);
            }
         }
      }
      catch(boost::filesystem::filesystem_error &e)
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
      // FPix
      { 291872, {{ -1, 162.0f * 25 - 12 }}},
      { 292283, {{ -1, 164.0f * 25 - 18 }}},
      { 292364, {{ -1, 164.0f * 25 - 6 }}},
      { 292365, {{ -1, 164.0f * 25 - 6 }}},
      { 292366, {{ -1, 164.0f * 25 - 6 }}},
      { 292367, {{ -1, 164.0f * 25 - 6 }}},
      { 292592, {{ -1, 162.0f * 25 - 6 }}},
      { 292593, {{ -1, 163.0f * 25 - 6 }}},
      { 292594, {{ -1, 163.0f * 25 - 6 }}},
      // //BPix
      { 292079, {{ -1, 163.0f * 25 - 18 }}},
      { 292080, {{ -1, 163.0f * 25 - 18 }}},
      { 292283, {{ -1, 165.0f * 25 - 18 }}},
      { 292364, {{ -1, 164.0f * 25 - 6 }}},
      { 292365, {{ -1, 164.0f * 25 - 6 }}},
      { 292366, {{ -1, 164.0f * 25 - 6 }}},
      { 292367, {{ -1, 164.0f * 25 - 6 }}},
      { 292592, {{ -1, 162.0f * 25 - 6 }}},
      { 292593, {{ -1, 163.0f * 25 - 6 }}},
      { 292594, {{ -1, 163.0f * 25 - 6 }}}
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

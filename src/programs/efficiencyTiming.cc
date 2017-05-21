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
         const        float delayInNs   = getDelayNs(runNumber, lumisection);
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
      // constexpr std::array<int, 5> delayTest {{157, 160, 161, 163, 165}};
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
         const float delayInNs   = getDelayNs(runNumber, lumisection);
         // const float delayInNs   = delayTest[rand() % 5] * 25;
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
      std::vector<TH1D*> delayVsEfficiencyBpixFpix                  ( 2, nullptr);
      std::vector<TH1D*> delayVsEfficiencyBpixFpixHits              ( 2, nullptr);
      std::vector<TH1D*> delayVsEfficiencyLayersNegativePositive    ( 8, nullptr);
      std::vector<TH1D*> delayVsEfficiencyLayersNegativePositiveHits( 8, nullptr);
      std::vector<TH1D*> delayVsEfficiencyDisksInnerOuter           (12, nullptr);
      std::vector<TH1D*> delayVsEfficiencyDisksInnerOuterHits       (12, nullptr);
      std::vector<TH1D*> delayVsEfficiencyBNPZHSSIOLP               (64, nullptr);
      std::vector<TH1D*> delayVsEfficiencyBNPZHSSIOLPHits           (64, nullptr);
      for(unsigned int det = 0; det <= 1; ++det)
      {
         static const std::vector<std::string> detStrings  = {"BPix", "FPix"};
         std::string detAsString = detStrings[det];
         delayVsEfficiencyBpixFpix[det] = new TH1D(
            ("delayVsEfficiency" + detAsString).c_str(),
            ("Delay vs efficiency on " + detAsString + ";delay;efficiency").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         delayVsEfficiencyBpixFpixHits[det] = new TH1D(
            ("delayVsefficiencyHits" + detAsString).c_str(),
            ("Number of eff. hits for delay scenarios on " + detAsString + ";delay;efficiency").c_str(), 
            DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
         delayVsEfficiencyBpixFpix[det] -> GetXaxis() -> SetNdivisions(510, kFALSE);
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
            delayVsEfficiencyLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsEfficiencyLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs efficiency on layer " + layerAsString + ", " + sideAsString + " side;delay;efficiency").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsEfficiencyLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of eff. hits for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;efficiency").c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyLayersNegativePositive[(layer - 1) * 2 + side - 1] -> GetXaxis() -> SetNdivisions(510, kFALSE);
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
            delayVsEfficiencyDisksInnerOuter[(0 <= disk) * 6 + (std::abs(disk) - 1) * 2 + ring - 1] = new TH1D(
               ("delayVsEfficiencyDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs efficiency on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;efficiency" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyDisksInnerOuterHits[(0 <= disk) * 6 + (std::abs(disk) - 1) * 2 + ring - 1] = new TH1D(
               ("delayVsEfficiencyDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of eff. hits for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;efficiency" ).c_str(),
               DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
            delayVsEfficiencyDisksInnerOuter[(0 <= disk) * 6 + (std::abs(disk) - 1) * 2 + ring - 1] -> GetXaxis() -> SetNdivisions(510, kFALSE);
            for(unsigned int numBin = 0; numBin < labelsWBC.size(); ++numBin)
            {
               delayVsEfficiencyDisksInnerOuter[(0 <= disk) * 6 + (std::abs(disk) - 1) * 2 + ring - 1] -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, labelsWBC[numBin].c_str());
            }
         }
      }
      for(unsigned int side = 1; side <= 2; ++side)
      {
         for(unsigned int halfShell = 1; halfShell <= 2; ++halfShell)
         {
            for(unsigned int sec = 1; sec <= 8; ++sec)
            {
               for(unsigned int layerPair = 1; layerPair <= 2; ++layerPair)
               {
                  static const std::vector<std::string> sideStrings  = {"Negative", "Positive"};
                  static const std::vector<std::string> layerPairStrings  = {"1 and 2", "3 and 4"};
                  static const std::vector<std::string> layerPairStringsShort  = {"1and2", "3and4"};
                  std::string sideAsString = sideStrings[side - 1];
                  std::string secAsString  = std::to_string(sec);
                  std::string halfShellAsString  = sideStrings[halfShell - 1];
                  std::string layerPairAsString = layerPairStrings[layerPair - 1];
                  std::string layerPairAsShortString = layerPairStringsShort[layerPair - 1];
                  delayVsEfficiencyBNPZHSSIOLP[(side - 1) * 32 + (halfShell - 1) * 16 + (sec - 1) * 2 + (layerPair - 1)] = new TH1D(
                     ("delayVsEfficiency" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs efficiency on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString).c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  delayVsEfficiencyBNPZHSSIOLPHits[(side - 1) * 32 + (halfShell - 1) * 16 + (sec - 1) * 2 + (layerPair - 1)] = new TH1D(
                     ("delayVsEfficiencyHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of eff. hits for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString).c_str(),
                     DELAY_PLOTS_NUM_BINS, DELAY_PLOTS_LOWER_EDGE, DELAY_PLOTS_UPPER_EDGE);
                  delayVsEfficiencyBNPZHSSIOLP[(side - 1) * 32 + (halfShell - 1) * 16 + (sec - 1) * 2 + (layerPair - 1)] -> GetXaxis() -> SetNdivisions(510, kFALSE);
                  for(unsigned int numBin = 0; numBin < labelsWBC.size(); ++numBin)
                  {
                     delayVsEfficiencyBNPZHSSIOLP[(side - 1) * 32 + (halfShell - 1) * 16 + (sec - 1) * 2 + (layerPair - 1)] -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, labelsWBC[numBin].c_str());
                  }
               }
            }
         }
      }
      for(auto& delayPlotterModulePair: delayToPlotterModuleMap)
      {
         const float& delay = delayPlotterModulePair.first;
         const std::array<std::pair<int, int>, 2>*  efficiencyBpixFpix               = delayPlotterModulePair.second.getEfficiencyBpixFpix();
         const std::array<std::pair<int, int>, 8>*  efficiencyLayersNegativePositive = delayPlotterModulePair.second.getEfficiencyLayersNegativePositive();
         const std::array<std::pair<int, int>, 12>* efficiencyDisksInnerOuter        = delayPlotterModulePair.second.getEfficiencyDisksInnerOuter();
         const std::array<std::pair<int, int>, 64>* efficiencyBNPZHSSIOLP            = delayPlotterModulePair.second.getEfficiencyBNPZHSSIOLP();
         for(unsigned int plotIndex = 0; plotIndex < 2; ++plotIndex)
         {
            if((*efficiencyBpixFpix)[plotIndex].first != 0) delayVsEfficiencyBpixFpix[plotIndex]     -> Fill(delay / 25, (*efficiencyBpixFpix)[plotIndex].second / static_cast<double>((*efficiencyBpixFpix)[plotIndex].first));
            if((*efficiencyBpixFpix)[plotIndex].first != 0) delayVsEfficiencyBpixFpixHits[plotIndex] -> Fill(delay / 25, (*efficiencyBpixFpix)[plotIndex].first);
         }
         for(unsigned int plotIndex = 0; plotIndex < 8; ++plotIndex)
         {
            if((*efficiencyLayersNegativePositive)[plotIndex].first != 0) delayVsEfficiencyLayersNegativePositive[plotIndex]     -> Fill(delay / 25, (*efficiencyLayersNegativePositive)[plotIndex].second / static_cast<double>((*efficiencyLayersNegativePositive)[plotIndex].first));
            if((*efficiencyLayersNegativePositive)[plotIndex].first != 0) delayVsEfficiencyLayersNegativePositiveHits[plotIndex] -> Fill(delay / 25, (*efficiencyLayersNegativePositive)[plotIndex].first);
         }
         for(unsigned int plotIndex = 0; plotIndex < 12; ++plotIndex)
         {
            if((*efficiencyDisksInnerOuter)[plotIndex].first != 0) delayVsEfficiencyDisksInnerOuter[plotIndex]      -> Fill(delay / 25, (*efficiencyDisksInnerOuter)[plotIndex].second / static_cast<double>((*efficiencyDisksInnerOuter)[plotIndex].first));
            if((*efficiencyDisksInnerOuter)[plotIndex].first != 0) delayVsEfficiencyDisksInnerOuterHits[plotIndex]  -> Fill(delay / 25, (*efficiencyDisksInnerOuter)[plotIndex].first);
         }
         for(unsigned int plotIndex = 0; plotIndex < 64; ++plotIndex)
         {
            if((*efficiencyBNPZHSSIOLP)[plotIndex].first != 0) delayVsEfficiencyBNPZHSSIOLP[plotIndex]     -> Fill(delay / 25, (*efficiencyBNPZHSSIOLP)[plotIndex].second / static_cast<double>((*efficiencyBNPZHSSIOLP)[plotIndex].first));
            if((*efficiencyBNPZHSSIOLP)[plotIndex].first != 0) delayVsEfficiencyBNPZHSSIOLPHits[plotIndex] -> Fill(delay / 25, (*efficiencyBNPZHSSIOLP)[plotIndex].first);
         }
      }
      gStyle -> SetPalette(kVisibleSpectrum);
      gStyle -> SetNumberContours(999);
      gStyle -> SetOptStat(1112211); // Integral overflow Underflow RMS (value 2: +error) Mean (value 2: +error) Entries Title
      gErrorIgnoreLevel = kError;
      gDirectory -> cd("/");
      gDirectory -> mkdir("Efficiency_vs_delay");
      gDirectory -> cd("Efficiency_vs_delay");
      gDirectory -> mkdir("Graphs");
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
         for(unsigned int plotIndex = 0; plotIndex < 2; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyBpixFpix[plotIndex], delayVsEfficiencyBpixFpixHits[plotIndex]);
         gDirectory -> cd("../LayersNegativePositive");
         for(unsigned int plotIndex = 0; plotIndex < 8; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyLayersNegativePositive[plotIndex], delayVsEfficiencyLayersNegativePositiveHits[plotIndex]);
         gDirectory -> cd("../DisksInnerOuter");
         for(unsigned int plotIndex = 0; plotIndex < 12; ++plotIndex) EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyDisksInnerOuter[plotIndex], delayVsEfficiencyDisksInnerOuterHits[plotIndex]);
         gDirectory -> cd("../LayerDetailed");
         for(unsigned int plotIndex = 0; plotIndex < 64; ++plotIndex) EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyBNPZHSSIOLP[plotIndex], delayVsEfficiencyBNPZHSSIOLPHits[plotIndex]);
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
      // { 291877, {{ -1, 1.0 }, { 2, 1.0 }} },
      { 291877, {{ -1, 164.0f * 25 }}},
      { 291879, {{ -1, 164.0f * 25 + 18 }}},
      { 291880, {{ -1, 164.0f * 25 + 18 }}},
      { 291881, {{ -1, 164.0f * 25 + 18 }}}
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
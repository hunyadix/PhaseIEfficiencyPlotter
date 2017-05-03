// Descriptions:
// Reads in an Ntuple created by running the PhaseIPixelNtuplizer and creates
// the main plots required for efficiency measurements.
// List of the plots generated:
//  - 
//  -
//  - (nothing else yet...)

// Data structure
#include "../../interface/DataStructures_v4.h"

// Utility
#include "../../interface/TestsCommon.h"
#include "../../interface/TTreeTools.h"
#include "../../interface/HelperFunctionsCommon.h"
#include "../../interface/CanvasExtras.h"
#include "../../interface/TimerColored.h"
#include "../../interface/ProgressBar.h"
// #include "../../interface/common_functions_jkarancs.h"
#include "../../interface/TrajMeasEfficiencyPlotsModule.h"
#include "../../interface/ClusterOccupancyModule.h"
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

constexpr std::pair<float, float> EFFICIENCY_ZOOM_RANGE_1D = {0.95,  1.005};
constexpr std::pair<float, float> EFFICIENCY_ZOOM_RANGE_2D = {0.945, 1.000};

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);
constexpr auto                    CONFIG_FILE_PATH                = "./config_main.json"; 
const     std::string             EFFICIENCY_PLOT_IDENTIFIER      = "Efficiency";
const     std::string             EFFICIENCY_NUMERATOR_IDENTIFIER = "Numhits";

const bool CLUST_LOOP_REQUESTED = true;
const bool TRAJ_LOOP_REQUESTED  = true;

void                                        testSaveFolders(const JSON& config);
TFile*                                      generateOutputNtuple(const JSON& config);
std::vector<std::string>                    getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey);
void                                        readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);
std::map<std::string, std::shared_ptr<TH1>> processHistogramDefinitions(const JSON& config, const std::string& configKey, const std::string& innerKey);
TH1*                                        getEfficiencyNumeratorHisto(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& efficiencyHistoName);
TGraphAsymmErrors*                          getGraphForEfficiencyWithAsymmetricErrors(const TH1D& efficiencyHistogram, const TH1D& numHitsHistogram);
void                                        addLegend(TH1* histogram);
void                                        addLegend(TH1* histogram, TGraphAsymmErrors* graph);

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
   gROOT -> SetBatch(kFALSE);
   EventData       eventField;
   Cluster         clusterField;
   TrajMeasurement trajField;
   // trajTreeChain -> Draw("d_cl >> hsqrt(1000, 0.0, 500.0)");
   // std::cout << "Running the app." << std::endl;
   // theApp -> Run();
   // Run number and wbc setting
   bool filterForRunNumberPresent = config.at("filter_for_runs");
   int  runNumberLowerBound = NOVAL_I;
   int  runNumberUpperBound = NOVAL_I;
   bool singleRun = false;
   std::string WBCSetting  = "WBC";
   if(filterForRunNumberPresent)
   {
      runNumberLowerBound  = config.at("run_filter_lower_bound").get<int>();
      runNumberUpperBound  = config.at("run_filter_upper_bound").get<int>();
      WBCSetting          += config.at("WBC_setting");
      if(runNumberLowerBound + 1 == runNumberUpperBound) singleRun = true;
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
   // ClusterOccupancyModule     clusterOccupancyModule (histograms, clusterField);
   TrajMeasEfficiencyPlotsModule trajMeasefficiencyPlotsModule(eventField, trajField);
   //////////////////
   // Cluster loop //
   //////////////////
   // if(CLUST_LOOP_REQUESTED) try
   // {
   //    TChain* clustTreeChain = new TChain("clustTree", "List of the clusters.");
   //    readInFilesAndAddToChain(config, "input_files_list", "input_files", clustTreeChain);
   //    clustTreeChain -> SetBranchAddress("event",  &eventField);
   //    // clustTreeChain -> SetBranchAddress("mod",    &(clusterField.mod));
   //    clustTreeChain -> SetBranchAddress("mod_on", &(clusterField.mod_on));
   //    clustTreeChain -> SetBranchAddress("clust",  &clusterField);
   //    // check if data is present
   //    Long64_t clustTreeNumEntries  = clustTreeChain  -> GetEntries();
   //    std::cout << debug_prompt << "total entries in the clustTree chain: " << clustTreeNumEntries << std::endl;
   //    if(clustTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: clustTree.");
   //    ProgressBar progressBar;
   //    const int    progressBarUpdateInterval = 10000;
   //    const double progressBarUpdateBy       = progressBarUpdateInterval / static_cast<double>(clustTreeNumEntries) * 100;
   //    auto  updateAndPrintProgress =  [&] (const int& entryIndex)
   //    {
   //       if(entryIndex % progressBarUpdateInterval == 0)
   //       {
   //          progressBar.update(progressBarUpdateBy);
   //          progressBar.print();
   //          std::cout << " -- Estimated time left: " << std::setw(6) << std::fixed << std::setprecision(1) << timer.getSecondsElapsed() * (1 / progressBar.getProgressionRate() - 1) << " second(s).";
   //       }
   //    };
   //    timer.restart("Measuring the time required for looping on the clusters...");
   //    for(Long64_t entryIndex = 0; entryIndex < clustTreeNumEntries; ++entryIndex)
   //    {
   //       clustTreeChain -> GetEntry(entryIndex);
   //       if(filterForRunNumberPresent) if(eventField.run <  runNumberLowerBound || runNumberUpperBound <= eventField.run)
   //       {
   //          updateAndPrintProgress(entryIndex);
   //          continue;
   //       }
   //       clusterOccupancyModule.fillHistograms();
   //       updateAndPrintProgress(entryIndex);
   //    }
   //    std::cout << std::endl;
   //    timer.printSeconds("Loop done. Took about: ", " second(s).");
   // }
   // catch(const std::exception& e)
   // {
   //    std::cout << error_prompt << "In the clusters loop: " << e.what() << " exception occured." << std::endl;
   //    exit(-1);
   // }
   ////////////////////////////////
   // Trajector measurement loop //
   ////////////////////////////////
   if(TRAJ_LOOP_REQUESTED) try
   {
      TChain* trajTreeChain  = new TChain("trajTree", "List of the trajectory measurements.");
      readInFilesAndAddToChain(config, "input_files_list", "input_files", trajTreeChain);
      // Trajectory measurement tree
      trajTreeChain -> SetBranchAddress("event",  &eventField);
      // trajTreeChain -> SetBranchAddress("mod",    &(trajField.mod));
      trajTreeChain -> SetBranchAddress("mod_on",    &(trajField.mod_on));
      trajTreeChain -> SetBranchAddress("clust",     &(trajField.clu));
      trajTreeChain -> SetBranchAddress("track",     &(trajField.trk));
      trajTreeChain -> SetBranchAddress("clust_pix", &(trajField.clu.pix));

      trajTreeChain -> SetBranchAddress("traj",   &trajField);
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
         if(filterForRunNumberPresent) if(eventField.run <  runNumberLowerBound || runNumberUpperBound <= eventField.run)
         {
            updateAndPrintProgress(entryIndex);
            continue;
         }
         // printTrajFieldInfoTrajOnly(trajField);
         trajMeasefficiencyPlotsModule.fillHistograms();
         updateAndPrintProgress(entryIndex);
      }
      std::cout << std::endl;
      timer.printSeconds("Loop done. Took about: ", " second(s).");
      trajMeasefficiencyPlotsModule.printCounters();
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
      // trajMeasefficiencyPlotsModule.postLoopScaleAveragesSpecialEfficiencies();
      // for(const auto& histogramPair: histograms)
      // {
      //    std::string efficiencyHistoName = histogramPair.first;
      //    if(efficiencyHistoName.find(EFFICIENCY_PLOT_IDENTIFIER) == std::string::npos) continue;
      //    auto numhitsHisto               = getEfficiencyNumeratorHisto(histograms, efficiencyHistoName);
      //    auto efficiencyHisto2DPtrConversionResult = dynamic_cast<TH2D*>(histogramPair.second.get());
      //    if(efficiencyHisto2DPtrConversionResult)
      //    {
      //       // std::cout << "Downscaling histogram: " << efficiencyHistoName << std::endl;
      //       downscale2DHistogram(efficiencyHisto2DPtrConversionResult, dynamic_cast<TH2D*>(numhitsHisto));
      //       efficiencyHisto2DPtrConversionResult -> GetZaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE_2D.first, EFFICIENCY_ZOOM_RANGE_2D.second);
      //       continue;
      //    }
      //    downscale1DHistogram(dynamic_cast<TH1D*>(histogramPair.second.get()), dynamic_cast<TH1D*>(numhitsHisto));
      //    dynamic_cast<TH1D*>(histogramPair.second.get()) -> GetYaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE_1D.first, EFFICIENCY_ZOOM_RANGE_1D.second);
      // }
   }
   ////////////////
   // Save plots //
   ////////////////
   try
   {
      trajMeasefficiencyPlotsModule.savePlots(config);
   }
   catch(const std::exception& e)
   {
      std::cout << error_prompt << "While saving histograms: " << e.what() << " exception occured." << std::endl;
      exit(-1);
   }
   // if(histogramsNtuple)
   // {
   //    std::cout << process_prompt << "Saving histograms in: " << config["ntuple_save_directory"] << "/" << config["ntuple_name"] << "... ";
   //    histogramsNtuple -> Write();
   //    histogramsNtuple -> Close();
   //    std::cout << "Done." << std::endl;
   // }
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

std::map<std::string, std::shared_ptr<TH1>> processHistogramDefinitions(const JSON& config, const std::string& configKey, const std::string& innerKey)
{
   auto noDeletionPolicyTH1D = [] (TH1D*) -> void {};
   auto noDeletionPolicyTH2D = [] (TH2D*) -> void {};
   std::map<std::string, std::shared_ptr<TH1>> histograms;
   std::string histogramDefinitionListPath = config[configKey];
   JSON inputListJSON = JSON::parse(fileToString(histogramDefinitionListPath));
   std::vector<JSON> histogramDefinitionList = inputListJSON[innerKey];
   for(const auto& definition: histogramDefinitionList)
   {
      std::string name  = checkGetElement<std::string>(definition, "name");
      std::string title = checkGetElement<std::string>(definition, "title");
      std::string type  = checkGetElement<std::string>(definition, "type");
      int nbinsx        = checkGetElement<int>(definition, "nbinsx");
      float xMin        = checkGetElement<float>(definition, "xMin");
      float xMax        = checkGetElement<float>(definition, "xMax");
      if(type == "TH1D") histograms.insert({name, std::shared_ptr<TH1D>(new TH1D(name.c_str(),  title.c_str(),  nbinsx, xMin, xMax), noDeletionPolicyTH1D)});
      if(type == "TH2D")
      {
         int nbinsy        = checkGetElement<int>(definition, "nbinsy");
         float yMin        = checkGetElement<float>(definition, "yMin");
         float yMax        = checkGetElement<float>(definition, "yMax");
         histograms.insert({name, std::shared_ptr<TH2D>(new TH2D(name.c_str(),  title.c_str(),  nbinsx, xMin, xMax, nbinsy, yMin, yMax), noDeletionPolicyTH2D)});
      }
   }
   return histograms;
}

TH1* getEfficiencyNumeratorHisto(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& efficiencyHistoName)
{
   if(histograms.find(efficiencyHistoName) == histograms.end())
   {
      std::cout << error_prompt << "Looking up efficiency histogram called " << efficiencyHistoName << " failed." << std::endl;
      return nullptr;
   }
   size_t effPos = efficiencyHistoName.find(EFFICIENCY_PLOT_IDENTIFIER);
   if(effPos != std::string::npos)
   {
      std::string numeratorHistoName = std::string(efficiencyHistoName).replace(effPos, EFFICIENCY_PLOT_IDENTIFIER.size(), EFFICIENCY_NUMERATOR_IDENTIFIER);
      auto numHitsHisto              = histograms.find(numeratorHistoName);
      if(numHitsHisto == histograms.end())
      {
         std::cout << error_prompt << "Efficiency histo: " << efficiencyHistoName << " exist, but there is no histogram called: " << numeratorHistoName << std::endl;
         return nullptr;
      }
      return numHitsHisto -> second.get();
   }
   return nullptr;
}

TGraphAsymmErrors* getGraphForEfficiencyWithAsymmetricErrors(const TH1D& efficiencyHistogram, const TH1D& numHitsHistogram)
{
   const TAxis* xAxis = efficiencyHistogram.GetXaxis();
   const TAxis* yAxis = efficiencyHistogram.GetYaxis();
   const int numBins = xAxis -> GetNbins();
   std::vector<Double_t> valuesX;
   std::vector<Double_t> valuesY;
   std::vector<Double_t> errorsXLow (numBins, 0.5 * xAxis -> GetBinWidth(xAxis -> GetFirst()));
   std::vector<Double_t> errorsXHigh(numBins, 0.5 * xAxis -> GetBinWidth(xAxis -> GetFirst()));
   std::vector<Double_t> errorsYLow;
   std::vector<Double_t> errorsYHigh;
   for(int bin = 0; bin < numBins; ++bin)
   {
      valuesX.push_back(xAxis -> GetBinCenter(bin + 1));
      if(std::string(efficiencyHistogram.GetName()) == std::string("layersDisksEfficiency"))
      {
         std::cout << efficiencyHistogram.GetBinContent(bin + 1) << std::endl;
      }
      valuesY.push_back(efficiencyHistogram.GetBinContent(bin + 1));
      double lowerBound, upperBound;
      std::tie(lowerBound, upperBound) = WilsonScoreIntervalErrorCalculator(numHitsHistogram.GetBinContent(bin + 1), valuesY[bin], 1.0).getError();
      errorsYLow .emplace_back(std::move(valuesY[bin] - lowerBound  ));
      errorsYHigh.emplace_back(std::move(upperBound   - valuesY[bin]));
   }
   TGraphAsymmErrors* graph = new TGraphAsymmErrors(numBins, valuesX.data(), valuesY.data(), errorsXLow.data(), errorsXHigh.data(), errorsYLow.data(), errorsYHigh.data());
   graph -> SetTitle(efficiencyHistogram.GetTitle());
   graph -> GetXaxis() -> SetTitle (xAxis -> GetTitle());
   graph -> GetYaxis() -> SetTitle (yAxis -> GetTitle());
   graph -> GetXaxis() -> SetRangeUser (xAxis -> GetXmin(), xAxis -> GetXmax());
   // graph -> GetYaxis() -> SetRangeUser (yAxis -> GetXmin(), yAxis -> GetXmax());
   graph -> GetXaxis() -> SetNdivisions(xAxis -> GetNdivisions());
   graph -> GetYaxis() -> SetNdivisions(yAxis -> GetNdivisions());
   graph -> GetXaxis() -> SetLabelOffset(xAxis -> GetLabelOffset());
   graph -> GetYaxis() -> SetLabelOffset(yAxis -> GetLabelOffset());
   graph -> SetMarkerColor(4);
   graph -> SetMarkerStyle(24);
   graph -> SetLineWidth(1);
   graph -> SetLineStyle(1);
   return graph;
   // const_cast<TH1D*>(&efficiencyHistogram) -> Draw("HIST");
}

void addLegend(TH1* histogram)
{
   histogram -> SetTitleSize(0);
   TH2D* histo2D = dynamic_cast<TH2D*>(histogram);
   TLegend* legend = new TLegend(0.45, 0.6, 0.8625, 0.9551);
   if(histo2D != nullptr)
   {
      legend -> AddEntry(histogram -> GetName(), histogram -> GetTitle(), "");
      legend -> AddEntry("", ("Entries: " + std::to_string(histogram -> GetEntries())).c_str(), "");
      legend -> AddEntry("", ("X Mean: "    + std::to_string(histogram -> GetMean(1)  )).c_str(), "");
      legend -> AddEntry("", ("X Std Dev: " + std::to_string(histogram -> GetStdDev(1))).c_str(), "");
      legend -> AddEntry("", ("Y Mean: "    + std::to_string(histogram -> GetMean(2)  )).c_str(), "");
      legend -> AddEntry("", ("Y Std Dev: " + std::to_string(histogram -> GetStdDev(2))).c_str(), "");
   }
   else
   {
      legend -> AddEntry(histogram -> GetName(), histogram -> GetTitle(), "cf");
      legend -> AddEntry("", ("Entries: " + std::to_string(histogram -> GetEntries())).c_str(), "");
      legend -> AddEntry("", ("Mean: "    + std::to_string(histogram -> GetMean(1)  )).c_str(), "");
      legend -> AddEntry("", ("Std Dev: " + std::to_string(histogram -> GetStdDev(1))).c_str(), "");
   }
   legend -> SetBorderSize(1);
   legend -> SetFillColor(10);
   legend -> SetFillStyle(3001);
   // legend -> SetFillStyle(1);
   // legend -> SetFillColorAlpha(10, 0.5);
   legend -> SetTextFont(42);
   legend -> SetTextSize(0.02);
   legend -> Draw();
}

void addLegend(TH1* histogram, TGraphAsymmErrors* graph)
{
   histogram -> SetTitleSize(0);
   TLegend* legend = new TLegend(0.45, 0.6, 0.8625, 0.9551);
   legend -> AddEntry(graph -> GetName(), graph -> GetTitle(), "pe");
   legend -> AddEntry("", ("Entries: " + std::to_string(histogram -> GetEntries())).c_str(), ""); 
   legend -> AddEntry("", ("Mean: "         + std::to_string(histogram -> GetMean(1)  )).c_str(), "");
   legend -> AddEntry("", ("Std Dev: "      + std::to_string(histogram -> GetStdDev(1))).c_str(), "");
   legend -> SetBorderSize(1);
   // legend -> SetFillStyle(1);
   // legend -> SetFillColorAlpha(10, 0.5);
   legend -> SetFillColor(10);
   legend -> SetFillStyle(3001);
   legend -> SetTextFont(42);
   legend -> SetTextSize(0.02);
   legend -> Draw();
}
// Data structure
#include "../interface/DataStructures_v5.h"

// Utility
#include "../interface/TTreeTools.h"
#include "../interface/CommonActors.h"
#include "../interface/TimerColored.h"
#include "../interface/HelperFunctionsCommon.h"
// #include "../interface/CanvasExtras.h"
#include "../interface/ModuleClusterPlot.h"
#include "../interface/TestsCommon.h"
#include "../interface/HelperFunctionsCommon.h"

// Root
#include <TROOT.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TText.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TEventList.h>
#include <TMath.h>
// #include <TRandom3.h>

// C++ libraries
#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include <functional>
#include <chrono>
#include <random>
#include <future>
#include <list>

#include "../interface/json.hpp"
using JSON = nlohmann::json;

class ClusterChainLoader
{
	public:
		TChain*   m_clustChain;
		TChain*   m_eventChain;
		Cluster   m_clusterField;
		EventData m_clusterEventField;
		EventData m_eventChainField;
		ClusterChainLoader(const JSON& t_config, const std::string& t_configKey, const std::string& t_innerKey);
		std::vector<std::string> getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey);
		void loadFilesFromConfigToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);
		void initializeChains(const JSON& t_config, const std::string& t_configKey, const std::string& t_innerKey);
};

// Errors
[[noreturn, gnu::cold]] void e_no_clusterChain_entries();
[[noreturn, gnu::cold]] void e_no_eventChain_entries();
// Checks
void checkClusterChainEntries(TChain* t_clustChain);
void checkEventChainEntries(TChain* t_eventChain);
// Main logic
void mainStartedPrompt(char** argv);
void mainTerminatedPrompt(char** argv);

class SplitClusterStatistics
{
	public:
		// Filled on a per event basis
		enum PerEventCountable
		{
			NUM_CLUSTERS_IN_EVENT,            NUM_CLUSTERS_LOW_ETA,            NUM_CLUSTERS_HIGH_ETA,
			NUM_PAIR_CANDIDATES_IN_EVENT,     NUM_PAIR_CANDIDATES_LOW_ETA,     NUM_PAIR_CANDIDATES_HIGH_ETA,
			EVEN_COL_CANDIDATES_IN_EVENT,     EVEN_COL_CANDIDATES_LOW_ETA,     EVEN_COL_CANDIDATES_HIGH_ETA,
			AVG_CLUSTER_SIZE_X_IN_EVENT,      AVG_CLUSTER_SIZE_X_LOW_ETA,      AVG_CLUSTER_SIZE_X_HIGH_ETA,
			AVG_CLUSTER_SIZE_Y_IN_EVENT,      AVG_CLUSTER_SIZE_Y_LOW_ETA,      AVG_CLUSTER_SIZE_Y_HIGH_ETA,
			AVG_CLUSTER_SIZE_PIXELS_IN_EVENT, AVG_CLUSTER_SIZE_PIXELS_LOW_ETA, AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA,
			NUM_PAIR_CANDIDATES_IN_BPIX,      NUM_PAIR_CANDIDATES_IN_FPIX,
			EVEN_COL_CANDIDATES_IN_BPIX,      EVEN_COL_CANDIDATES_IN_FPIX,
			NUM_CLUSTERS_LAYER_1,             NUM_CLUSTERS_LAYER_2,             NUM_CLUSTERS_LAYER_3,            NUM_CLUSTERS_LAYER_4,
			NUM_PAIR_CANDIDATES_LAYER_1,      NUM_PAIR_CANDIDATES_LAYER_2,      NUM_PAIR_CANDIDATES_LAYER_3,     NUM_PAIR_CANDIDATES_LAYER_4,
			EVEN_COL_CANDIDATES_LAYER_1,      EVEN_COL_CANDIDATES_LAYER_2,      EVEN_COL_CANDIDATES_LAYER_3,     EVEN_COL_CANDIDATES_LAYER_4,
			AVG_CLUSTER_SIZE_X_LAYER_1,       AVG_CLUSTER_SIZE_X_LAYER_2,       AVG_CLUSTER_SIZE_X_LAYER_3,      AVG_CLUSTER_SIZE_X_LAYER_4,
			AVG_CLUSTER_SIZE_Y_LAYER_1,       AVG_CLUSTER_SIZE_Y_LAYER_2,       AVG_CLUSTER_SIZE_Y_LAYER_3,      AVG_CLUSTER_SIZE_Y_LAYER_4,
			AVG_CLUSTER_SIZE_PIXELS_LAYER_1,  AVG_CLUSTER_SIZE_PIXELS_LAYER_2,  AVG_CLUSTER_SIZE_PIXELS_LAYER_3, AVG_CLUSTER_SIZE_PIXELS_LAYER_4,
			NUMBER_OF_EVENT_COUNTABLES
		};
		// Filled with each entry
		enum Distributions
		{
			CLUSTER_SIZE_X_TOTAL,   CLUSTER_SIZE_X_LOW_ETA,   CLUSTER_SIZE_X_HIGH_ETA,
			CLUSTER_SIZE_Y_TOTAL,   CLUSTER_SIZE_Y_LOW_ETA,   CLUSTER_SIZE_Y_HIGH_ETA,
			CLUSTER_SIZE_PIX_TOTAL, CLUSTER_SIZE_PIX_LOW_ETA, CLUSTER_SIZE_PIX_HIGH_ETA,
			NUMBER_OF_DISTRIBUTION_COUNTABLES
		};
	private:
		// Constants
		const int LOW_ETA_SELECTED_LAYER   = 1;
		const int LOW_ETA_SELECTED_MODULE  = 1;
		const int LOW_ETA_SELECTED_LADDER  = 2;
		const int HIGH_ETA_SELECTED_LAYER  = 1;
		const int HIGH_ETA_SELECTED_MODULE = 4;
		const int HIGH_ETA_SELECTED_LADDER = 6;
		// States
		TChain*            m_clustChain;
		TChain*            m_eventChain;
		Cluster&           m_clusterField;
		EventData&         m_clusterEventField;
		EventData&         m_eventChainField;
		Long64_t           m_clusterChainEntries;
		Long64_t           m_eventChainEntries;
		std::vector<TH1F>  m_perEventDistributions;
		std::vector<TH1F>  m_distributions;
		// std::map<int, std::vector<Cluster>> m_eventClustersMap;
	public:
		SplitClusterStatistics(TChain* t_clustChain, TChain* t_eventChain, Cluster& t_clusterField, EventData& t_clusterEventField, EventData& t_eventChainField);
		~SplitClusterStatistics();
	private:
		[[noreturn, gnu::cold]] void e_event_without_key();
		SplitClusterStatistics(const SplitClusterStatistics&) = delete;
		SplitClusterStatistics& operator=(SplitClusterStatistics&) = delete;
		// [[gnu::hot]] std::map<int, std::vector<Cluster>> getClusterCollectionSortedByEvtnum(TChain* t_clustChain, const Cluster& t_clusterField, const EventData& t_clusterEventField);
		std::vector<int> getNUniqueEventNumbers(std::size_t t_nEventNumbersToGet);
		void addModuleClusterPlots(std::list<ModuleClusterPlot>& t_moduleClusterPlots, const ModuleClusterPlot::Type& t_type, const int& t_layer, const int& t_module, const int& t_ladder, const std::vector<int>& t_nUniqueEventNumbers);
		TEventList getEventSortedEventList(const std::string& eventListName);
		static bool areModulesSame(const ModuleData& t_lhs, const ModuleData& t_rhs);
		static bool areClustersOnSameModule(const Cluster* t_lhs, const Cluster* t_rhs);
		bool clusterIsOnLowEtaSelectedModule(const Cluster* t_clusterField);
		bool clusterIsOnHighEtaSelectedModule(const Cluster* t_clusterField);
		void savePlotAsEps(TH1& t_histogram, const std::string& t_path = "./");
		static std::tuple<bool, bool> isDistanceYBetweenClustersTwoOrDcol(const Cluster* t_lhs, const Cluster* t_rhs);
		void generateHistogramCollections();
		void fillDistributionHistogramCollections(const std::vector<Cluster*>& t_eventModuleClusters, const std::vector<Cluster*>& t_lowEtaModuleClusters, const std::vector<Cluster*>& t_highEtaModuleClusters);
		void fillPerEventHistogramCollections(const std::vector<Cluster*>& t_eventModuleClusters, std::vector<Cluster*>& t_lowEtaModuleClusters, std::vector<Cluster*>& t_highEtaModuleClusters);
		void incrementPerEventCounters();
	public:
		void createModuleClusterPlots(const JSON& t_config, TFile* t_outputFile);
		void createStatistics(const JSON& t_config, TFile* t_outputFile);
};

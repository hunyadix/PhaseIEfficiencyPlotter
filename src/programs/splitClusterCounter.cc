#include "../../interface/splitClusterCounter.h"

constexpr auto CONFIG_FILE_PATH = "./config_splitClusterStudy.json";
constexpr auto OUTPUT_FILE_PATH = "/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/split_cluster_test.root";

int main([[maybe_unused]] int argc, char** argv) try
{
	mainStartedPrompt(argv);
	std::cout << process_prompt << "Loading config file... ";
	JSON config = JSON::parse(fileToString(CONFIG_FILE_PATH));
	std::cout << "Done." << std::endl;

   TFile* outputFile = TFile::Open(OUTPUT_FILE_PATH, "RECREATE");

	// Read in the files into a chain
	ClusterChainLoader clusterChainLoader(config, "input_files_list", "input_files");
	TChain*    clustChain        = clusterChainLoader.m_clustChain;
	TChain*    eventChain        = clusterChainLoader.m_eventChain;
	Cluster&   clusterField      = clusterChainLoader.m_clusterField;
	EventData& clusterEventField = clusterChainLoader.m_clusterEventField;
	EventData& eventChainField   = clusterChainLoader.m_eventChainField;
	checkClusterChainEntries(clustChain);
	checkEventChainEntries(eventChain);

	SplitClusterStatistics analysis(clustChain, eventChain, clusterField, clusterEventField, eventChainField);

	if(config.at("plot_module_clusters").at("create_plots"))
	{
		analysis.createModuleClusterPlots(config, outputFile);
	}
	else
	{
		std::cout << process_prompt << "Creation of module cluster plots disabled." << std::endl;
	}

	analysis.createStatistics(config, outputFile);
	outputFile -> Close();
	// theApp -> Run();
	mainTerminatedPrompt(argv);
	return 0;
}
catch(const std::exception& e)
{
	std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
	return -1;
}

[[noreturn, gnu::cold]] void e_no_clusterChain_entries()
{
	throw std::runtime_error("No entries found in tree: clusterTree.");
}

[[noreturn, gnu::cold]] void e_no_eventChain_entries()
{
	throw std::runtime_error("No entries found in tree: eventTree.");
}

void checkClusterChainEntries(TChain* t_clustChain)
{
	Long64_t clusterChainEntries = t_clustChain -> GetEntries();
	if(clusterChainEntries == 0) e_no_clusterChain_entries();
	std::cout << debug_prompt << "Total entries in the cluster chain: " << clusterChainEntries << std::endl;
}

void checkEventChainEntries(TChain* t_eventChain)
{
	Long64_t eventChainEntries = t_eventChain -> GetEntries();
	if(eventChainEntries == 0) e_no_eventChain_entries();
	std::cout << debug_prompt << "Total entries in the event chain: " << eventChainEntries << std::endl;
}

void mainStartedPrompt(char** t_argv)
{
	std::cout << process_prompt << t_argv[0] << " started..." << std::endl;
	std::time_t processStarted = std::time(nullptr);
	std::cout << timer_prompt << "Time: " << std::asctime(std::localtime(&processStarted)) << std::flush;
	std::cout << process_prompt << "Loading config file... ";
}

void mainTerminatedPrompt(char** t_argv)
{
	std::cout << process_prompt << t_argv[0] << " terminated succesfully." << std::endl;
}

ClusterChainLoader::ClusterChainLoader(const JSON& t_config, const std::string& t_configKey, const std::string& t_innerKey):
	m_clustChain(new TChain("clustTree", "List of the clusters.")),
	m_eventChain(new TChain("eventTree", "List of the events."))
{
	initializeChains(t_config, t_configKey, t_innerKey);
}

std::vector<std::string> ClusterChainLoader::getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey)
{
   std::string inputFilesListPath = config[configKey];
   JSON inputListJSON = JSON::parse(fileToString(inputFilesListPath));
   return inputListJSON[innerKey];
}

void ClusterChainLoader::loadFilesFromConfigToChain(const JSON& t_config, const std::string& t_configKey, const std::string& t_innerKey, TChain* t_chain)
{
   std::vector<std::string> inputFiles = getFilesFromConfig(t_config, t_configKey, t_innerKey);
   std::for_each(inputFiles.begin(), inputFiles.end(), [&] (const std::string& fileName) { t_chain  -> Add(fileName.c_str()); });
}

void ClusterChainLoader::initializeChains(const JSON& t_config, const std::string& t_configKey, const std::string& t_innerKey)
{
	loadFilesFromConfigToChain(t_config, t_configKey, t_innerKey, m_clustChain);
	m_clustChain -> SetBranchAddress("event",     &m_clusterEventField);
	// m_clustChain -> SetBranchAddress("mod",       &(m_clusterField.mod));
	m_clustChain -> SetBranchAddress("clust",     &m_clusterField);
	m_clustChain -> SetBranchAddress("mod_on",    &(m_clusterField.mod_on));
	m_clustChain -> SetBranchAddress("clust_adc", &m_clusterField.adc);
	m_clustChain -> SetBranchAddress("clust_pix", &m_clusterField.pix);
	loadFilesFromConfigToChain(t_config, t_configKey, t_innerKey, m_eventChain);
	m_eventChain  -> SetBranchAddress("event", &m_eventChainField);
}

SplitClusterStatistics::SplitClusterStatistics(TChain* t_clustChain, TChain* t_eventChain, Cluster& t_clusterField, EventData& t_clusterEventField, EventData& t_eventChainField):
	m_clustChain(t_clustChain),
	m_eventChain(t_eventChain),
	m_clusterField(t_clusterField),
	m_clusterEventField(t_clusterEventField),
	m_eventChainField(t_eventChainField),
	m_clusterChainEntries(m_clustChain -> GetEntries()),
	m_eventChainEntries(m_eventChain -> GetEntries())
	// m_eventClustersMap(getClusterCollectionSortedByEvtnum(m_clustChain, m_clusterField, m_clusterEventField))
{
	gStyle -> SetOptStat(2222222);
	gStyle -> SetPalette(1);
	generateHistogramCollections();
}

SplitClusterStatistics::~SplitClusterStatistics() {}

[[noreturn, gnu::cold]] void SplitClusterStatistics::e_event_without_key()
{
	throw std::runtime_error("Entry missing in clusters of selected runs.");
}

// [[gnu::hot]] std::map<int, std::vector<Cluster>> SplitClusterStatistics::getClusterCollectionSortedByEvtnum(TChain* t_clustChain, const Cluster& t_clusterField, const EventData& t_clusterEventField, Long64_t& t_minEntryNum, Long64_t& t_maxEntryNum)
// {
// 	std::cout << "Mapping events to cluster collections..." << std::endl;
// 	std::map<int, std::vector<Cluster>> eventClustersMap;
// 	for(Long64_t entryIndex = t_minEntryNum; entryIndex < clusterChainEntries; ++entryIndex)
// 	{
// 		int eventNum = t_clusterEventField.evt;
// 		t_clustChain -> GetEntry(entryIndex);
// 		auto eventClustersIt = eventClustersMap.find(eventNum);
// 		// If key does not exist yet: add key
// 		if(eventClustersIt == eventClustersMap.end())
// 		{
// 			eventClustersIt = eventClustersMap.emplace(eventNum, std::vector<Cluster>()).first;
// 		}
// 		eventClustersIt -> second.push_back(std::move(heapReference));
// 	}
// 	std::cout << "Done mapping events to cluster collections." << std::endl;
// 	return eventClustersMap;
// }

std::vector<int> SplitClusterStatistics::getNUniqueEventNumbers(std::size_t t_nEventNumbersToGet)
{
	std::vector<int> result(t_nEventNumbersToGet);
	// If good randomness is required, change this to two std::random_device calls,
	// and shift one of them with " << 32"
	std::chrono::time_point<std::chrono::steady_clock> seed;
	std::mt19937_64 longIntGenerator(std::chrono::duration_cast<std::chrono::milliseconds>(seed.time_since_epoch()).count());
	for(std::size_t eventNumberIndex = 0; eventNumberIndex != t_nEventNumbersToGet; ++eventNumberIndex)
	{
		Long64_t entryToPlot = longIntGenerator() % m_eventChainEntries;
		m_eventChain -> GetEntry(entryToPlot);
		if(m_eventChainField.evt == NOVAL_I)
		{
			continue;
			--eventNumberIndex;
		}
		result[eventNumberIndex] = m_eventChainField.evt;
	}
	return result;
}

void SplitClusterStatistics::addModuleClusterPlots(std::list<ModuleClusterPlot>& t_moduleClusterPlots, const ModuleClusterPlot::Type& t_type, const int& t_layer, const int& t_module, const int& t_ladder, const std::vector<int>& t_nUniqueEventNumbers)
{
	for(const int& i: t_nUniqueEventNumbers)
	{
		t_moduleClusterPlots.emplace_back(t_type, t_layer, t_module, t_ladder, i, i);
	}
}

void SplitClusterStatistics::createModuleClusterPlots(const JSON& t_config, TFile* t_outputFile)
{
	std::cout << process_prompt << "Creating module cluster plots... " << std::endl;
	const unsigned int numPlotsToCreate   = t_config.at("plot_module_clusters").at("n_plots_to_create");
	std::cout << "Number of events to create plots from: " << numPlotsToCreate << std::endl;
	if(numPlotsToCreate == 0) return;

	std::cout << "Producing unique event numbers..." << std::endl;
	std::vector<int> nUniqueEventNumbers = getNUniqueEventNumbers(numPlotsToCreate);
	std::sort(nUniqueEventNumbers.begin(), nUniqueEventNumbers.end());
	std::cout << "The list of selected events: " << std::endl;
	std::copy(nUniqueEventNumbers.begin(), nUniqueEventNumbers.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	std::cout << "Loading histogram types..." << std::endl;
	// Histogram definitions
	const auto& digisType                          = ModuleClusterPlot::Type::digis;
	const auto& pairsType                          = ModuleClusterPlot::Type::pairs;
	// const auto& pairsWithMarkersType               = ModuleClusterPlot::Type::pairsWithMarkers;
	const auto& pairsWithNeighboursType            = ModuleClusterPlot::Type::pairsWithNeighbours;
	// const auto& pairsWithAngleLabelsType           = ModuleClusterPlot::Type::pairsWithAngleLabels;
	// const auto& pairsWithAngleColorCodesType       = ModuleClusterPlot::Type::pairsWithAngleColorCodes;
	// const auto& pairsWithIndividualAngleLabelsType = ModuleClusterPlot::Type::pairsWithIndividualAngleLabels;
	// const auto& pairsWithIndividualAngleColorsType = ModuleClusterPlot::Type::pairsWithIndividualAngleColors;

	std::cout << "Defining histograms..." << std::endl;
	std::list<ModuleClusterPlot> moduleClusterPlots;
	// Low eta
	addModuleClusterPlots(moduleClusterPlots, digisType,                          LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	addModuleClusterPlots(moduleClusterPlots, pairsType,                          LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithMarkersType,               LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	addModuleClusterPlots(moduleClusterPlots, pairsWithNeighboursType,            LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithAngleLabelsType,           LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithAngleColorCodesType,       LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithIndividualAngleLabelsType, LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithIndividualAngleColorsType, LOW_ETA_SELECTED_LAYER, LOW_ETA_SELECTED_MODULE, LOW_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// High eta
	addModuleClusterPlots(moduleClusterPlots, digisType,                          HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	addModuleClusterPlots(moduleClusterPlots, pairsType,                          HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithMarkersType,               HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	addModuleClusterPlots(moduleClusterPlots, pairsWithNeighboursType,            HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithAngleLabelsType,           HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithAngleColorCodesType,       HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithIndividualAngleLabelsType, HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);
	// addModuleClusterPlots(moduleClusterPlots, pairsWithIndividualAngleColorsType, HIGH_ETA_SELECTED_LAYER, HIGH_ETA_SELECTED_MODULE, HIGH_ETA_SELECTED_LADDER, nUniqueEventNumbers);

	t_outputFile -> mkdir("Module_cluster_plots");
	t_outputFile -> cd("Module_cluster_plots");

	std::cout << "Searching for the clusters of the selected runs..." << std::endl;
	std::map<int, std::vector<Cluster>> clustersOfSelectedMergedEvents;
	for(int eventNum: nUniqueEventNumbers)
	{
		clustersOfSelectedMergedEvents.emplace(eventNum, std::vector<Cluster>());
	}

	bool clusterSelectionEnded = false;
	Long64_t entryIndex = 0;
	std::future<void> progressIndicator = std::async(std::launch::async, [&] () -> void
	{
		std::chrono::milliseconds updateRate(1200);
		std::vector<int> numClustersPerEvent;
		while(1)
		{
			std::cout << "Entry: " << entryIndex << " / " << m_clusterChainEntries <<
				" Total size: " << std::accumulate(clustersOfSelectedMergedEvents.begin(), clustersOfSelectedMergedEvents.end(), 0, [] (int sum, const auto &e) { return sum + e.second.size(); }) <<
				"\nNumber of clusters for each of the elements: " << std::endl;
			numClustersPerEvent = (fmap(clustersOfSelectedMergedEvents, [] (auto& e) -> int { return e.second.size(); } ));
			std::copy(numClustersPerEvent.begin(), numClustersPerEvent.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
			if(clusterSelectionEnded) return;
			std::this_thread::sleep_for(updateRate);
		}
	});

	assert(std::is_sorted(nUniqueEventNumbers.begin(), nUniqueEventNumbers.end()));
	for(; entryIndex < m_clusterChainEntries; ++entryIndex) try
	{
		m_clustChain -> GetEntry(entryIndex);
		if(m_clusterField.i == NOVAL_I) continue;
		auto findRes = std::lower_bound(nUniqueEventNumbers.begin(), nUniqueEventNumbers.end(), m_clusterEventField.evt);
		if(findRes == nUniqueEventNumbers.end()) continue;
		// if(*findRes != m_clusterEventField.evt) continue;
		if(10 < std::abs(*findRes - m_clusterEventField.evt)) continue;
		clustersOfSelectedMergedEvents.at(*findRes).push_back(m_clusterField);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		e_event_without_key();
	}

	clusterSelectionEnded = true;
	progressIndicator.get();

	std::cout << "Looping for module cluster plot creation..." << std::endl;
	std::cout << "Expected avg. number of clusters per event: " << m_clusterChainEntries / static_cast<double>(m_eventChainEntries) << std::endl;
	for(const int eventNum: nUniqueEventNumbers)
	{
		const std::vector<Cluster>& clustersForEventCollection = clustersOfSelectedMergedEvents.at(eventNum);
		std::cout << "clustersForEventCollection.size(): " << clustersForEventCollection.size() << std::endl;
		std::cout << "Avg. num clusters per module: " << clustersForEventCollection.size() / 1184.0 << std::endl;
		std::cout << "Avg. num clusters per module * number of merged evt.: " << clustersForEventCollection.size() * 11.0 / 1184.0 << std::endl;
		int numberOfClustersOnLowEtaModule = std::count_if(clustersForEventCollection.begin(), clustersForEventCollection.end(), [this] (const auto& e)
		{
			return 
				e.mod_on.layer  == LOW_ETA_SELECTED_LAYER  &&
				e.mod_on.module == LOW_ETA_SELECTED_MODULE &&
				e.mod_on.ladder == LOW_ETA_SELECTED_LADDER;
		});
		int numberOfClustersOnHighEtaModule = std::count_if(clustersForEventCollection.begin(), clustersForEventCollection.end(), [this] (const auto& e)
		{
			return 
				e.mod_on.layer  == HIGH_ETA_SELECTED_LAYER  &&
				e.mod_on.module == HIGH_ETA_SELECTED_MODULE &&
				e.mod_on.ladder == HIGH_ETA_SELECTED_LADDER;
		});
		std::cout << debug_prompt << "Creating plots for event " << eventNum << "." << std::endl;
		std::cout << debug_prompt << "Number of clusters for this event: " << clustersForEventCollection.size() << "." << std::endl;
		std::cout << debug_prompt << "Clusters on the selected module for low  eta module: " << numberOfClustersOnLowEtaModule << std::endl;
		std::cout << debug_prompt << "Clusters on the selected module for high eta module: " << numberOfClustersOnHighEtaModule << std::endl;
		ModuleClusterPlot::fillDigisMarkers(clustersForEventCollection, eventNum);
		ModuleClusterPlot::fillAllPairs(clustersForEventCollection, eventNum);
		ModuleClusterPlot::saveAllFinished(eventNum);
	}
	std::cout << process_prompt << "Creating module cluster plots... " << std::endl;
}

TEventList SplitClusterStatistics::getEventSortedEventList(const std::string& eventListName)
{
	m_clustChain -> SetEstimate(m_clusterChainEntries);
	m_clustChain -> Draw("event.evt", "", "goff");
	std::unique_ptr<Long64_t[]> entryList = std::make_unique<Long64_t[]>(m_clusterChainEntries);
	TMath::Sort(m_clusterChainEntries, m_clustChain -> GetV1(), entryList.get());
	TEventList eventList(eventListName.c_str(), eventListName.c_str());
	Long64_t processed = 0;
	auto start = std::chrono::steady_clock::now();
	while(1)
	{
		bool done = false;
		auto now = std::chrono::steady_clock::now();
		for(int i = 0; i < 10000; ++i)
		{
			Long64_t e = *(entryList.get() + processed);
			eventList.Enter(e);
			++processed;
			if(processed == m_clusterChainEntries)
			{
				done = true;
				break;
			}
		}
		if(done) break;
		std::chrono::duration<double> elapsed = now - start;
		std::cout << "processed: " << processed << ", " << static_cast<double>(processed) / m_clusterChainEntries * 100.0 << " % " <<
			elapsed.count() / (static_cast<double>(processed) / m_clusterChainEntries) - elapsed.count() << " s" << std::endl;
	}
	std::for_each(entryList.get(), entryList.get() + m_clusterChainEntries, [&eventList] (const Long64_t& e)
	{
		eventList.Enter(e);
	});
	std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
    return eventList;
}

void SplitClusterStatistics::generateHistogramCollections()
{
	// Per event distributions
	m_perEventDistributions.clear();
	m_perEventDistributions.reserve(NUMBER_OF_EVENT_COUNTABLES);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_IN_EVENT", "NUM_CLUSTERS_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LOW_ETA", "NUM_CLUSTERS_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_HIGH_ETA", "NUM_CLUSTERS_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_IN_EVENT", "NUM_PAIR_CANDIDATES_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LOW_ETA", "NUM_PAIR_CANDIDATES_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_HIGH_ETA", "NUM_PAIR_CANDIDATES_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_IN_EVENT", "EVEN_COL_CANDIDATES_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LOW_ETA", "EVEN_COL_CANDIDATES_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_HIGH_ETA", "EVEN_COL_CANDIDATES_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_IN_EVENT", "AVG_CLUSTER_SIZE_X_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LOW_ETA", "AVG_CLUSTER_SIZE_X_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_HIGH_ETA", "AVG_CLUSTER_SIZE_X_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_IN_EVENT", "AVG_CLUSTER_SIZE_Y_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LOW_ETA", "AVG_CLUSTER_SIZE_Y_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_HIGH_ETA", "AVG_CLUSTER_SIZE_Y_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_IN_EVENT", "AVG_CLUSTER_SIZE_PIXELS_IN_EVENT", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LOW_ETA", "AVG_CLUSTER_SIZE_PIXELS_LOW_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA", "AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_IN_BPIX", "NUM_PAIR_CANDIDATES_IN_BPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_IN_FPIX", "NUM_PAIR_CANDIDATES_IN_FPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_IN_BPIX", "EVEN_COL_CANDIDATES_IN_BPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_IN_FPIX", "EVEN_COL_CANDIDATES_IN_FPIX", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_1", "NUM_CLUSTERS_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_2", "NUM_CLUSTERS_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_3", "NUM_CLUSTERS_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_CLUSTERS_LAYER_4", "NUM_CLUSTERS_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_1", "NUM_PAIR_CANDIDATES_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_2", "NUM_PAIR_CANDIDATES_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_3", "NUM_PAIR_CANDIDATES_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("NUM_PAIR_CANDIDATES_LAYER_4", "NUM_PAIR_CANDIDATES_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_1", "EVEN_COL_CANDIDATES_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_2", "EVEN_COL_CANDIDATES_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_3", "EVEN_COL_CANDIDATES_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("EVEN_COL_CANDIDATES_LAYER_4", "EVEN_COL_CANDIDATES_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_1", "AVG_CLUSTER_SIZE_X_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_2", "AVG_CLUSTER_SIZE_X_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_3", "AVG_CLUSTER_SIZE_X_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_X_LAYER_4", "AVG_CLUSTER_SIZE_X_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_1", "AVG_CLUSTER_SIZE_Y_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_2", "AVG_CLUSTER_SIZE_Y_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_3", "AVG_CLUSTER_SIZE_Y_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_Y_LAYER_4", "AVG_CLUSTER_SIZE_Y_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_1", "AVG_CLUSTER_SIZE_PIXELS_LAYER_1", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_2", "AVG_CLUSTER_SIZE_PIXELS_LAYER_2", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_3", "AVG_CLUSTER_SIZE_PIXELS_LAYER_3", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.emplace_back("AVG_CLUSTER_SIZE_PIXELS_LAYER_4", "AVG_CLUSTER_SIZE_PIXELS_LAYER_4", 1000, 0.0f, 10000000.0f);
	m_perEventDistributions.shrink_to_fit();
	
	// General distributions
	m_distributions.clear();
	m_distributions.emplace_back("CLUSTER_SIZE_X_TOTAL", "CLUSTER_SIZE_X_TOTAL", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_X_LOW_ETA", "CLUSTER_SIZE_X_LOW_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_X_HIGH_ETA", "CLUSTER_SIZE_X_HIGH_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_Y_TOTAL", "CLUSTER_SIZE_Y_TOTAL", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_Y_LOW_ETA", "CLUSTER_SIZE_Y_LOW_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_Y_HIGH_ETA", "CLUSTER_SIZE_Y_HIGH_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_PIX_TOTAL", "CLUSTER_SIZE_PIX_TOTAL", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_PIX_LOW_ETA", "CLUSTER_SIZE_PIX_LOW_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.emplace_back("CLUSTER_SIZE_PIX_HIGH_ETA", "CLUSTER_SIZE_PIX_HIGH_ETA", 1000, 0.0f, 100000000.0f);
	m_distributions.shrink_to_fit();
}

bool SplitClusterStatistics::areModulesSame(const ModuleData& t_lhs, const ModuleData& t_rhs)
{
	return 
		t_lhs.layer  == t_rhs.layer &&
		t_lhs.module == t_rhs.module &&
		t_lhs.ladder == t_rhs.ladder &&
		t_lhs.disk   == t_rhs.disk &&
		t_lhs.ring   == t_rhs.ring &&
		t_lhs.blade  == t_rhs.blade &&
		t_lhs.panel  == t_rhs.panel;
}

bool SplitClusterStatistics::areClustersOnSameModule(const Cluster* t_lhs, const Cluster* t_rhs)
{
	return areModulesSame(t_lhs -> mod_on, t_rhs -> mod_on);
}

bool SplitClusterStatistics::clusterIsOnLowEtaSelectedModule(const Cluster* t_clusterField)
{
	return
		t_clusterField -> mod_on.layer  == LOW_ETA_SELECTED_LAYER   &&
		t_clusterField -> mod_on.module == LOW_ETA_SELECTED_MODULE  &&
		t_clusterField -> mod_on.ladder == LOW_ETA_SELECTED_LADDER;
}

bool SplitClusterStatistics::clusterIsOnHighEtaSelectedModule(const Cluster* t_clusterField)
{
	return
		t_clusterField -> mod_on.layer  == HIGH_ETA_SELECTED_LAYER   &&
		t_clusterField -> mod_on.module == HIGH_ETA_SELECTED_MODULE  &&
		t_clusterField -> mod_on.ladder == HIGH_ETA_SELECTED_LADDER;
}

void calcPairEndsAndOrderLeftRight_(Cluster const* lhs, Cluster const* rhs, int& lhsColMin, int& lhsColMax, int& rhsColMin, int& rhsColMax)
{
	static auto colLessThan = [] (const auto& lhs, const auto& rhs) { return lhs[1] < rhs[1]; };
	const float(* lhsColMinIt)[2];
	const float(* lhsColMaxIt)[2];
	const float(* rhsColMinIt)[2];
	const float(* rhsColMaxIt)[2];
	std::tie(lhsColMinIt, lhsColMaxIt) = std::minmax_element(lhs -> pix, lhs -> pix + lhs -> size, colLessThan);
	std::tie(rhsColMinIt, rhsColMaxIt) = std::minmax_element(rhs -> pix, rhs -> pix + rhs -> size, colLessThan);
	lhsColMin = static_cast<int>((*lhsColMinIt)[1] + 0.5f);
	lhsColMax = static_cast<int>((*lhsColMaxIt)[1] + 0.5f);
	rhsColMin = static_cast<int>((*rhsColMinIt)[1] + 0.5f);
	rhsColMax = static_cast<int>((*rhsColMaxIt)[1] + 0.5f);
	if(rhsColMin < lhsColMin)
	{
		// std::cout << "Before swap: " << lhs << " " << rhs << std::endl;
		std::cout << "SWAP!!!" << std::endl;
		std::swap(lhs, rhs);
		std::swap(lhsColMin, rhsColMin);
		std::swap(lhsColMax, rhsColMax);
	}
}

void SplitClusterStatistics::savePlotAsEps(TH1& t_histogram, const std::string& t_path)
{
	TCanvas canvas(t_histogram.GetName(), t_histogram.GetName());
	CanvasExtras::redesignCanvas(&canvas, &t_histogram);
	canvas.cd();
	t_histogram.Draw();
	std::string filename = canvas.GetTitle();
	std::transform(filename.begin(), filename.end(), filename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
	filename = t_path + "/" + filename + ".eps";
	std::cout << process_prompt << "Saving canvas: " << filename << std::endl;
	canvas.SaveAs(filename.c_str());
}

std::tuple<bool, bool> SplitClusterStatistics::isDistanceYBetweenClustersTwoOrDcol(const Cluster* t_lhs, const Cluster* t_rhs)
{
	if(!areClustersOnSameModule(t_lhs, t_rhs))   return std::make_tuple(false, false);
	if(5.0f < std::abs(t_lhs -> x - t_rhs -> x)) return std::make_tuple(false, false);
	int lhsColMin, lhsColMax, rhsColMin, rhsColMax;
	calcPairEndsAndOrderLeftRight_(t_lhs, t_rhs, lhsColMin, lhsColMax, rhsColMin, rhsColMax);
	if(lhsColMax + 3 != rhsColMin)
	{
		if(lhsColMax % 2 != 1) std::make_tuple(true, true);
		else                   std::make_tuple(true, false);
	}
	return std::make_tuple(false, false);
}

void SplitClusterStatistics::fillDistributionHistogramCollections(const std::vector<Cluster*>& t_eventModuleClusters, const std::vector<Cluster*>& t_lowEtaModuleClusters, const std::vector<Cluster*>& t_highEtaModuleClusters)
{
	std::for_each(t_eventModuleClusters.begin(), t_eventModuleClusters.end(), [this] (const auto& e)
	{
		m_distributions[CLUSTER_SIZE_X_TOTAL]   .Fill(e -> sizeX);
		m_distributions[CLUSTER_SIZE_Y_TOTAL]   .Fill(e -> sizeY);
		m_distributions[CLUSTER_SIZE_PIX_TOTAL] .Fill(e -> size);
	});
	std::for_each(t_lowEtaModuleClusters.begin(), t_lowEtaModuleClusters.end(), [this] (const auto& e)
	{
		m_distributions[CLUSTER_SIZE_X_LOW_ETA]  .Fill(e -> sizeX);
		m_distributions[CLUSTER_SIZE_Y_LOW_ETA]  .Fill(e -> sizeY);
		m_distributions[CLUSTER_SIZE_PIX_LOW_ETA].Fill(e -> size);
	});
	std::for_each(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end(), [this] (const auto& e)
	{
		m_distributions[CLUSTER_SIZE_X_HIGH_ETA]  .Fill(e -> sizeX);
		m_distributions[CLUSTER_SIZE_Y_HIGH_ETA]  .Fill(e -> sizeY);
		m_distributions[CLUSTER_SIZE_PIX_HIGH_ETA].Fill(e -> size);
	});
}

void SplitClusterStatistics::fillPerEventHistogramCollections(const std::vector<Cluster*>& t_eventModuleClusters, std::vector<Cluster*>& t_lowEtaModuleClusters, std::vector<Cluster*>& t_highEtaModuleClusters)
{
	// Sacrifice speed for safety
	assert(std::is_sorted(t_lowEtaModuleClusters .begin(), t_lowEtaModuleClusters .end()));
	assert(std::is_sorted(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end()));
	// Pair candidates
	std::vector<Cluster*> clustersWithPairCandidates;
	std::vector<Cluster*> clustersWithDcolMissingPairCandidates;
	for(auto it_1 = t_eventModuleClusters.begin(), last = t_eventModuleClusters.end(); it_1 != last; ++it_1)
	{
		for(auto it_2 = it_1 + 1; it_2 != last; ++it_2)
		{
			std::tuple<bool, bool> candidateTestResults(isDistanceYBetweenClustersTwoOrDcol(*it_1, *it_2));
			if(std::get<0>(candidateTestResults))
			{
				clustersWithPairCandidates.emplace_back(*it_1);
				clustersWithPairCandidates.emplace_back(*it_2);
			}
			if(std::get<1>(candidateTestResults))
			{
				clustersWithDcolMissingPairCandidates.emplace_back(*it_1);
				clustersWithDcolMissingPairCandidates.emplace_back(*it_2);
			}
		}
	}
	{
		std::set<Cluster*> uniqueElements(clustersWithPairCandidates.begin(), clustersWithPairCandidates.end());
		clustersWithPairCandidates.assign(uniqueElements.begin(), uniqueElements.end());
	}
	{
		std::set<Cluster*> uniqueElements(clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end());
		clustersWithDcolMissingPairCandidates.assign(uniqueElements.begin(), uniqueElements.end());
	}
	// Sacrifice speed for safety
	assert(std::is_sorted(clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end()));
	assert(std::is_sorted(clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end()));
	m_perEventDistributions[NUM_CLUSTERS_IN_EVENT]            .Fill(t_eventModuleClusters.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_IN_EVENT]     .Fill(clustersWithPairCandidates.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_IN_EVENT]     .Fill(clustersWithDcolMissingPairCandidates.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_IN_EVENT]      .Fill(std::accumulate(t_eventModuleClusters.begin(), t_eventModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeX; }) / t_eventModuleClusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_IN_EVENT]      .Fill(std::accumulate(t_eventModuleClusters.begin(), t_eventModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeY; }) / t_eventModuleClusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_IN_EVENT] .Fill(std::accumulate(t_eventModuleClusters.begin(), t_eventModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> size;  }) / t_eventModuleClusters.size());
	// Low eta module
	std::sort(t_lowEtaModuleClusters.begin(), t_lowEtaModuleClusters.end());
	std::vector<Cluster*> lowEtaModuleClustersWithPairCandidates;
	std::vector<Cluster*> lowEtaModuleClustersWithDcolMissingPairCandidates;
	std::set_intersection(t_lowEtaModuleClusters.begin(), t_lowEtaModuleClusters.end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(lowEtaModuleClustersWithPairCandidates));
	std::set_intersection(t_lowEtaModuleClusters.begin(), t_lowEtaModuleClusters.end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(lowEtaModuleClustersWithDcolMissingPairCandidates));
	// Fill the actual containers
	m_perEventDistributions[NUM_CLUSTERS_LOW_ETA]             .Fill(t_lowEtaModuleClusters.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LOW_ETA]      .Fill(lowEtaModuleClustersWithPairCandidates.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_LOW_ETA]      .Fill(lowEtaModuleClustersWithDcolMissingPairCandidates.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LOW_ETA]       .Fill(std::accumulate(t_lowEtaModuleClusters.begin(), t_lowEtaModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeX; }) / t_lowEtaModuleClusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LOW_ETA]       .Fill(std::accumulate(t_lowEtaModuleClusters.begin(), t_lowEtaModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeY; }) / t_lowEtaModuleClusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LOW_ETA]  .Fill(std::accumulate(t_lowEtaModuleClusters.begin(), t_lowEtaModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> size; }) / t_lowEtaModuleClusters.size());
	// High eta module
	std::sort(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end());
	std::vector<Cluster*> highEtaModuleClustersWithPairCandidates;
	std::vector<Cluster*> highEtaModuleClustersWithDcolMissingPairCandidates;
	std::set_intersection(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(highEtaModuleClustersWithPairCandidates));
	std::set_intersection(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(highEtaModuleClustersWithDcolMissingPairCandidates));
	// Fill the actual containers
	m_perEventDistributions[NUM_CLUSTERS_HIGH_ETA]            .Fill(t_highEtaModuleClusters.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_HIGH_ETA]     .Fill(highEtaModuleClustersWithPairCandidates.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_HIGH_ETA]     .Fill(highEtaModuleClustersWithDcolMissingPairCandidates.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_HIGH_ETA]      .Fill(std::accumulate(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeX; }) / t_highEtaModuleClusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_HIGH_ETA]      .Fill(std::accumulate(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeY; }) / t_highEtaModuleClusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA] .Fill(std::accumulate(t_highEtaModuleClusters.begin(), t_highEtaModuleClusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> size; }) / t_highEtaModuleClusters.size());
	const std::pair<std::vector<Cluster*>, std::vector<Cluster*>> bpixFpixClusters = dualFilter(t_eventModuleClusters, [] (const Cluster* e) { return e -> mod_on.layer == 1; });
	// Bpix, fpix
	const std::vector<Cluster*> bpixClusters = bpixFpixClusters.first;
	const std::vector<Cluster*> fpixClusters = bpixFpixClusters.second;
	// Layers
	const std::vector<Cluster*> layer1Clusters = filter(bpixClusters, [] (const Cluster* e) { return e -> mod_on.layer == 1; });
	const std::vector<Cluster*> layer2Clusters = filter(bpixClusters, [] (const Cluster* e) { return e -> mod_on.layer == 2; });
	const std::vector<Cluster*> layer3Clusters = filter(bpixClusters, [] (const Cluster* e) { return e -> mod_on.layer == 3; });
	const std::vector<Cluster*> layer4Clusters = filter(bpixClusters, [] (const Cluster* e) { return e -> mod_on.layer == 4; });
	// Sacrifice speed for safety
	assert(std::is_sorted(bpixClusters  .begin(), bpixClusters  .end()));
	assert(std::is_sorted(fpixClusters  .begin(), fpixClusters  .end()));
	assert(std::is_sorted(layer1Clusters.begin(), layer1Clusters.end()));
	assert(std::is_sorted(layer2Clusters.begin(), layer2Clusters.end()));
	assert(std::is_sorted(layer3Clusters.begin(), layer3Clusters.end()));
	assert(std::is_sorted(layer4Clusters.begin(), layer4Clusters.end()));
	std::vector<Cluster*> clustersWithPairCandidatesBPix;
	std::vector<Cluster*> clustersWithDcolMissingPairCandidatesBPix;
	std::vector<Cluster*> clustersWithPairCandidatesFPix;
	std::vector<Cluster*> clustersWithDcolMissingPairCandidatesFPix;
	std::vector<Cluster*> clustersWithPairCandidatesLayer1;
	std::vector<Cluster*> clustersWithDcolMissingPairCandidatesLayer1;
	std::vector<Cluster*> clustersWithPairCandidatesLayer2;
	std::vector<Cluster*> clustersWithDcolMissingPairCandidatesLayer2;
	std::vector<Cluster*> clustersWithPairCandidatesLayer3;
	std::vector<Cluster*> clustersWithDcolMissingPairCandidatesLayer3;
	std::vector<Cluster*> clustersWithPairCandidatesLayer4;
	std::vector<Cluster*> clustersWithDcolMissingPairCandidatesLayer4;
	std::set_intersection(clustersWithPairCandidatesBPix             .begin(), clustersWithPairCandidatesBPix             .end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(clustersWithPairCandidatesBPix));
	std::set_intersection(clustersWithDcolMissingPairCandidatesBPix  .begin(), clustersWithDcolMissingPairCandidatesBPix  .end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(clustersWithDcolMissingPairCandidatesBPix));
	std::set_intersection(clustersWithPairCandidatesFPix             .begin(), clustersWithPairCandidatesFPix             .end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(clustersWithPairCandidatesFPix));
	std::set_intersection(clustersWithDcolMissingPairCandidatesFPix  .begin(), clustersWithDcolMissingPairCandidatesFPix  .end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(clustersWithDcolMissingPairCandidatesFPix));
	std::set_intersection(clustersWithPairCandidatesLayer1           .begin(), clustersWithPairCandidatesLayer1           .end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(clustersWithPairCandidatesLayer1));
	std::set_intersection(clustersWithDcolMissingPairCandidatesLayer1.begin(), clustersWithDcolMissingPairCandidatesLayer1.end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(clustersWithDcolMissingPairCandidatesLayer1));
	std::set_intersection(clustersWithPairCandidatesLayer2           .begin(), clustersWithPairCandidatesLayer2           .end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(clustersWithPairCandidatesLayer2));
	std::set_intersection(clustersWithDcolMissingPairCandidatesLayer2.begin(), clustersWithDcolMissingPairCandidatesLayer2.end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(clustersWithDcolMissingPairCandidatesLayer2));
	std::set_intersection(clustersWithPairCandidatesLayer3           .begin(), clustersWithPairCandidatesLayer3           .end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(clustersWithPairCandidatesLayer3));
	std::set_intersection(clustersWithDcolMissingPairCandidatesLayer3.begin(), clustersWithDcolMissingPairCandidatesLayer3.end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(clustersWithDcolMissingPairCandidatesLayer3));
	std::set_intersection(clustersWithPairCandidatesLayer4           .begin(), clustersWithPairCandidatesLayer4           .end(), clustersWithPairCandidates           .begin(), clustersWithPairCandidates           .end(), std::back_inserter(clustersWithPairCandidatesLayer4));
	std::set_intersection(clustersWithDcolMissingPairCandidatesLayer4.begin(), clustersWithDcolMissingPairCandidatesLayer4.end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), std::back_inserter(clustersWithDcolMissingPairCandidatesLayer4));
	
	std::cout << std::set_intersection(clustersWithDcolMissingPairCandidatesLayer4.begin(), clustersWithDcolMissingPairCandidatesLayer4.end(), clustersWithDcolMissingPairCandidates.begin(), clustersWithDcolMissingPairCandidates.end(), counter_iterator()) << " vs " << clustersWithDcolMissingPairCandidatesLayer4.size() << std::endl;
	// Fill the actual containers
	m_perEventDistributions[NUM_PAIR_CANDIDATES_IN_BPIX].Fill(bpixClusters.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_IN_FPIX].Fill(fpixClusters.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_IN_BPIX].Fill(clustersWithPairCandidatesBPix.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_IN_FPIX].Fill(clustersWithPairCandidatesFPix.size());
	m_perEventDistributions[NUM_CLUSTERS_LAYER_1].Fill(layer1Clusters.size());
	m_perEventDistributions[NUM_CLUSTERS_LAYER_2].Fill(layer2Clusters.size());
	m_perEventDistributions[NUM_CLUSTERS_LAYER_3].Fill(layer3Clusters.size());
	m_perEventDistributions[NUM_CLUSTERS_LAYER_4].Fill(layer4Clusters.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_1].Fill(clustersWithPairCandidatesLayer1.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_2].Fill(clustersWithPairCandidatesLayer2.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_3].Fill(clustersWithPairCandidatesLayer3.size());
	m_perEventDistributions[NUM_PAIR_CANDIDATES_LAYER_4].Fill(clustersWithPairCandidatesLayer4.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_1].Fill(clustersWithDcolMissingPairCandidatesLayer1.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_2].Fill(clustersWithDcolMissingPairCandidatesLayer2.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_3].Fill(clustersWithDcolMissingPairCandidatesLayer3.size());
	m_perEventDistributions[EVEN_COL_CANDIDATES_LAYER_4].Fill(clustersWithDcolMissingPairCandidatesLayer4.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_1].Fill(std::accumulate(layer1Clusters.begin(), layer1Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeX; }) / layer1Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_2].Fill(std::accumulate(layer2Clusters.begin(), layer2Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeX; }) / layer2Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_3].Fill(std::accumulate(layer3Clusters.begin(), layer3Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeX; }) / layer3Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_X_LAYER_4].Fill(std::accumulate(layer4Clusters.begin(), layer4Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeX; }) / layer4Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_1].Fill(std::accumulate(layer1Clusters.begin(), layer1Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeY; }) / layer1Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_2].Fill(std::accumulate(layer2Clusters.begin(), layer2Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeY; }) / layer2Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_3].Fill(std::accumulate(layer3Clusters.begin(), layer3Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeY; }) / layer3Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_Y_LAYER_4].Fill(std::accumulate(layer4Clusters.begin(), layer4Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> sizeY; }) / layer4Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_1].Fill(std::accumulate(layer1Clusters.begin(), layer1Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> size; }) / layer1Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_2].Fill(std::accumulate(layer2Clusters.begin(), layer2Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> size; }) / layer2Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_3].Fill(std::accumulate(layer3Clusters.begin(), layer3Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> size; }) / layer3Clusters.size());
	m_perEventDistributions[AVG_CLUSTER_SIZE_PIXELS_LAYER_4].Fill(std::accumulate(layer4Clusters.begin(), layer4Clusters.end(), 0.0f, [] (float total, const Cluster* e) { return total + e -> size; }) / layer4Clusters.size());
}

void SplitClusterStatistics::incrementPerEventCounters()
{
	// m_perEventCounters[NUM_CLUSTERS_IN_EVENT]++;

	// m_perEventCounters[NUM_PAIR_CANDIDATES_IN_EVENT]++;
	// m_perEventCounters[EVEN_COL_CANDIDATES_IN_EVENT]++;
	// m_perEventCounters[AVG_CLUSTER_SIZE_X_IN_EVENT]++;
	// m_perEventCounters[AVG_CLUSTER_SIZE_Y_IN_EVENT]++;
	// m_perEventCounters[AVG_CLUSTER_SIZE_PIXELS_IN_EVENT]++;
	// if(clusterIsOnLowEtaSelectedModule())
	// {
	// 	m_perEventCounters[NUM_CLUSTERS_LOW_ETA]++;
	// 	m_perEventCounters[NUM_PAIR_CANDIDATES_LOW_ETA]++;
	// 	m_perEventCounters[EVEN_COL_CANDIDATES_LOW_ETA]++;
	// 	m_perEventCounters[AVG_CLUSTER_SIZE_X_LOW_ETA]++;
	// 	m_perEventCounters[AVG_CLUSTER_SIZE_Y_LOW_ETA]++;
	// 	m_perEventCounters[AVG_CLUSTER_SIZE_PIXELS_LOW_ETA]++;
	// }
	// else
	// {
	// 	if(clusterIsOnHighEtaSelectedModule())
	// 	{
	// 		m_perEventCounters[NUM_CLUSTERS_HIGH_ETA]++;
	// 		m_perEventCounters[NUM_PAIR_CANDIDATES_HIGH_ETA]++;
	// 		m_perEventCounters[EVEN_COL_CANDIDATES_HIGH_ETA]++;
	// 		m_perEventCounters[AVG_CLUSTER_SIZE_X_HIGH_ETA]++;
	// 		m_perEventCounters[AVG_CLUSTER_SIZE_Y_HIGH_ETA]++;
	// 		m_perEventCounters[AVG_CLUSTER_SIZE_PIXELS_HIGH_ETA]++;
	// 	}
	// }
}

void SplitClusterStatistics::createStatistics([[maybe_unused]] const JSON& t_config, [[maybe_unused]] TFile* t_outputFile)
{
	std::cout << process_prompt << "Creating distributions and other statistics..." << std::endl;
	TEventList eventListSortedByEventNum { getEventSortedEventList("eventListSortedByEventNum") };
	// Count here:
	// Number of clusters {in event/ onmodule}
	// Number of cluster pair candidates {in event/ on module}
	// Number of clusters pair candidates starting on an even numbered dcol {in event/ on module}
	// Cluster size distribution {in event/ on module}, {x, y, pixels}

	// Calculate:
	// Ratio of clusters starting on even/odd dcol coordinate {in event/ onmodule}
	// Ratio of cluster sizes on high eta and low eta modules
	// Generate_histograms
	assert(m_perEventDistributions.size() == NUMBER_OF_EVENT_COUNTABLES);
	assert(m_distributions.size() == NUMBER_OF_DISTRIBUTION_COUNTABLES);

	// Get the list of clusters in the event
	m_clustChain -> GetEntry(eventListSortedByEventNum.GetEntry(0));
	int currentEvent = m_clusterEventField.evt;
	Long64_t entryIndex = 0;
	std::vector<Cluster*> clustersInEvent;
	while(1)
	{
		m_clustChain -> GetEntry(eventListSortedByEventNum.GetEntry(entryIndex));
		if(m_clusterField.mod_on.det != 0 && m_clusterField.mod_on.det != 1) continue;
		int eventNum = m_clusterEventField.evt;
		if(eventNum != currentEvent)
		{
			std::cout << "eventNum: " << eventNum << std::endl;
			if(clustersInEvent.size()) continue;
			std::sort(clustersInEvent.begin(), clustersInEvent.end());
			std::cout << "clustersInEvent.size(): " << clustersInEvent.size() << std::endl;
			std::cout << "clustersInEvent.front() -> mod_on.module: " << clustersInEvent.front() -> mod_on.module << std::endl;
			std::vector<Cluster*> lowEtaModuleClusters  = filter(clustersInEvent, [this] (Cluster* e) { return clusterIsOnLowEtaSelectedModule (e); });
			std::vector<Cluster*> highEtaModuleClusters = filter(clustersInEvent, [this] (Cluster* e) { return clusterIsOnHighEtaSelectedModule(e); });
			fillPerEventHistogramCollections    (clustersInEvent, lowEtaModuleClusters, highEtaModuleClusters);
			fillDistributionHistogramCollections(clustersInEvent, lowEtaModuleClusters, highEtaModuleClusters);
			std::for_each(clustersInEvent.begin(), clustersInEvent.end(), [] (const Cluster* e) { delete e; });
			clustersInEvent.clear();
			currentEvent = eventNum;
		}
		clustersInEvent.emplace_back(new Cluster(m_clusterField));
		++entryIndex;
		if(entryIndex == m_clusterChainEntries) break;
	}
	std::for_each(m_perEventDistributions.begin(), m_perEventDistributions.end(), [&] (auto& histogram)
	{
		savePlotAsEps(histogram, "results/Distributions/");
	});

	// for(Long64_t entryIndex = 0; entryIndex < m_clusterChainEntries; ++entryIndex)
	// {
	// 	m_clustChain -> GetEntry(eventListSortedByEventNum.GetEntry(entryIndex));
	// 	int eventNum = m_clusterEventField.evt;
	// 	if(eventNum != lastEvt)
	// 	{
	// 		fillPerEventHistogramCollections();
	// 		std::fill(m_perEventCounters.begin(), m_perEventCounters.end(), 0);
	// 	}
	// 	fillDistributionHistogramCollections();
	// 	lastEvt = std::move(eventNum);
	// }
	std::cout << process_prompt << "Done creating distributions and other statistics..." << std::endl;
}


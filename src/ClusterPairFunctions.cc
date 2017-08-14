#include "../interface/ClusterPairFunctions.h"

namespace ClusterPairFunctions
{
	bool areModulesSame(const ModuleData& t_lhs, const ModuleData& t_rhs)
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

	void printClusterFieldInfo(const Cluster& clusterField)
	{
		std::cout << "--- Begin cluster field info ---" << std::endl;
		std::cout << "det: "        << clusterField.mod_on.det    << "." << std::endl;
		std::cout << "layer: "      << clusterField.mod_on.layer  << "." << std::endl;
		std::cout << "module: "     << clusterField.mod_on.module << "." << std::endl;
		std::cout << "ladder: "     << clusterField.mod_on.ladder << "." << std::endl;
		std::cout << "x: "          << clusterField.x             << "." << std::endl;
		std::cout << "y: "          << clusterField.y             << "." << std::endl;
		std::cout << "clustSizeX: " << clusterField.sizeX         << "." << std::endl;
		std::cout << "clustSizeY: " << clusterField.sizeY         << "." << std::endl;
		std::cout << "clustIndex: " << clusterField.i             << "." << std::endl;
		std::cout << "clustSize: "  << clusterField.size          << "." << std::endl;
		std::cout << "charge: "     << clusterField.charge        << "." << std::endl;
		std::cout << "--- End cluster field info ---"   << std::endl;
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

	int calcPairXWidth(const PairType& pair)
	{
		static auto rowLessThan = [] (const auto coordPair_lhs, const auto coordPair_rhs) { return coordPair_lhs[0] < coordPair_rhs[0]; };
		const float(* lhsRowMinIt)[2];
		const float(* lhsRowMaxIt)[2];
		const float(* rhsRowMinIt)[2];
		const float(* rhsRowMaxIt)[2];
		std::tie(lhsRowMinIt, lhsRowMaxIt) = std::minmax_element(pair.first.pix,  pair.first.pix  + pair.first.size,  rowLessThan);
		std::tie(rhsRowMinIt, rhsRowMaxIt) = std::minmax_element(pair.second.pix, pair.second.pix + pair.second.size, rowLessThan);
		float min1 = (*lhsRowMinIt)[0];
		float min2 = (*rhsRowMinIt)[0];
		float max1 = (*lhsRowMaxIt)[0];
		float max2 = (*rhsRowMaxIt)[0];
		// if(max1 > min2 && max2 > min1)
		if(!areClustersPair(pair.first, pair.second))
		{
			std::cout << "BAD PAIRS!!!" << std::endl;
			std::cout << "(" << min1 << ", " << max1 << "), (" << min2 << ", " << max2 << ")" << std::endl;
		}
		int xWidth = static_cast<int>(std::max(max1, max2) - std::min(min1, min2) + 1.5f);
		return xWidth;
	}

	int isClusterTagged([[maybe_unused]]const Cluster& cluster)
	{
		// for(const auto& pixelTag: cluster.pixelsMarker)
		// {
		// 	if(pixelTag != 0) return 1;
		// }
		return 0;
	}

	int isClusterTaggedInColumn([[maybe_unused]] const Cluster& clusterField, [[maybe_unused]] int col)
	{
		// for(int i: range(clusterField.pixelsCol.size()))
		// {
		// 	if(clusterField.pixelsCol[i] == col && clusterField.pixelsMarker[i] != 0) return 1;		
		// }
		return 0;
	}

	int areClustersPair(const Cluster& first, const Cluster& second)
	{
		if(!(areModulesSame(first.mod_on, second.mod_on))) return 0;
		if(5 < std::abs(first.x - second.x)) return 0;
		const Cluster* lhs = &first;
		const Cluster* rhs = &second;
		int lhsColMin, lhsColMax, rhsColMin, rhsColMax;
		calcPairEndsAndOrderLeftRight_(lhs, rhs, lhsColMin, lhsColMax, rhsColMin, rhsColMax);
		if(lhsColMax % 2 != 1 || lhsColMax + 3 != rhsColMin) return 0;
		// if(lhsColMax > rhsColMin && rhsColMax > lhsColMin)
		// {
		// 	std::cout << "+++BAD PAIRS!!!" << std::endl;
		// 	std::cout << "(" << lhsColMin << ", " << lhsColMax << "), (" << rhsColMin << ", " << rhsColMax << ")" << std::endl;
		// }
		return 1;
	}

	int areClustersEndTaggedPair(const Cluster& first, const Cluster& second)
	{
		if(!areModulesSame(first.mod_on, second.mod_on)) return 0;
		if(5 < std::abs(first.x - second.x)) return 0;
		const Cluster* lhs = &first;
		const Cluster* rhs = &second;
		int lhsColMin, lhsColMax, rhsColMin, rhsColMax;
		calcPairEndsAndOrderLeftRight_(lhs, rhs, lhsColMin, lhsColMax, rhsColMin, rhsColMax);
		if(lhsColMax % 2 != 1 || lhsColMax + 3 != rhsColMin) return 0;
		if(!isClusterTaggedInColumn(*lhs, lhsColMax)) return 0;
		if(!isClusterTaggedInColumn(*rhs, rhsColMin)) return 0;
		return 1;
	}

	PairCollectionType getClusterPairCollection(const std::vector<Cluster>& clusterCollection)
	{
		PairCollectionType clusterPairCollection;
		for(auto firstClusterIt = clusterCollection.begin(); firstClusterIt != clusterCollection.end(); ++firstClusterIt)
		{
			const ModuleData& mod1 = firstClusterIt -> mod_on;
			if(mod1.det != 0) continue;
			for(auto secondClusterIt = firstClusterIt + 1; secondClusterIt != clusterCollection.end(); ++secondClusterIt)
			{
				const ModuleData& mod2 = secondClusterIt -> mod_on;
				if(!areModulesSame(mod1, mod2)) continue;
				if(!areClustersPair(*firstClusterIt, *secondClusterIt)) continue;
				clusterPairCollection.emplace_back(std::make_pair(*firstClusterIt, *secondClusterIt));
			}
		}
		return clusterPairCollection;
	}

	// PairCollectionType getClusterPairCollection(const CollectionType& clusterCollection)
	// {
	// 	PairCollectionType clusterPairCollection;
	// 	for(auto firstClusterIt = clusterCollection.begin(); firstClusterIt != clusterCollection.end(); ++firstClusterIt)
	// 	{
	// 		const ModuleData& mod1 = (*firstClusterIt) -> mod_on;
	// 		if(mod1.det != 0) continue;
	// 		for(auto secondClusterIt = firstClusterIt + 1; secondClusterIt != clusterCollection.end(); ++secondClusterIt)
	// 		{
	// 			const ModuleData& mod2 = (*secondClusterIt) -> mod_on;
	// 			if(!areModulesSame(mod1, mod2)) continue;
	// 			if(!areClustersPair(**firstClusterIt, **secondClusterIt)) continue;
	// 			clusterPairCollection.emplace_back(std::make_pair(*firstClusterIt, *secondClusterIt));
	// 		}
	// 	}
	// 	return clusterPairCollection;
	// }

	// The complexity of the calculations can be decreased by looping once and looking for every weighted avarage to calculate
	float getAdcWeightedRowCoordinateInCol(const Cluster& cluster, const int& col)
	{
		int rowSum      = 0;
		int adcSum      = 0;
		for(int pixelIndex: range(cluster.size))
		{
			if(cluster.pix[pixelIndex][1] == col)
			{
				rowSum += cluster.pix[pixelIndex][0] * cluster.adc[pixelIndex];
				adcSum += cluster.adc[pixelIndex];
			}
		}
		return static_cast<float>(rowSum) / adcSum;
	}

	float getClusterIndAngle(const Cluster& cluster)
	{
		static auto colLessThan = [] (const auto& coordPair_lhs, const auto& coordPair_rhs) { return coordPair_lhs[1] < coordPair_rhs[1]; };
		const float(* colMinIt)[2];
		const float(* colMaxIt)[2];
		std::tie(colMinIt, colMaxIt) = std::minmax_element(cluster.pix, cluster.pix + cluster.size, colLessThan);
		int colMin = static_cast<int>((*colMinIt)[1] + 0.5f);
		int colMax = static_cast<int>((*colMaxIt)[1] + 0.5f);
		float startRow = getAdcWeightedRowCoordinateInCol(cluster, colMin);
		float endRow   = getAdcWeightedRowCoordinateInCol(cluster, colMax);
		return std::atan2(colMax - colMin, endRow - startRow);
	}

	// If this is slow, the speed can be increased to almost 2x by only looping once on the collection and only calculating
	// the min and max values required. Finding both the min and max together should not be that slow though.
	float getClusterPairAngle(const PairType& pair)
	{
		static auto colLessThan = [] (const auto& coordPair_lhs, const auto& coordPair_rhs) { return coordPair_lhs[1] < coordPair_rhs[1]; };
		const float(* lhsColMinIt)[2];
		const float(* lhsColMaxIt)[2];
		const float(* rhsColMinIt)[2];
		const float(* rhsColMaxIt)[2];
		std::tie(lhsColMinIt, lhsColMaxIt) = std::minmax_element(pair.first.pix , pair.first.pix  + pair.first.size,  colLessThan);
		std::tie(rhsColMinIt, rhsColMaxIt) = std::minmax_element(pair.second.pix, pair.second.pix + pair.second.size, colLessThan);
		int lhsColMin = static_cast<int>((*lhsColMinIt)[1] + 0.5f);
		int lhsColMax = static_cast<int>((*lhsColMaxIt)[1] + 0.5f);
		int rhsColMin = static_cast<int>((*rhsColMinIt)[1] + 0.5f);
		int rhsColMax = static_cast<int>((*rhsColMaxIt)[1] + 0.5f);
		int lhsFirst = (lhsColMin < rhsColMin);
		const int& startCol = lhsFirst ? lhsColMin : rhsColMin;
		const int& endCol   = lhsFirst ? rhsColMax : lhsColMax;
		float startRow = getAdcWeightedRowCoordinateInCol(pair.first,  startCol);
		float endRow   = getAdcWeightedRowCoordinateInCol(pair.second, endCol  );
		return std::atan2(endCol - startCol, endRow - startRow);
	}

	std::vector<float> getClusterPairAngles(const std::vector<Cluster>& clusterCollection)
	{
		std::vector<float> clusterPairAngles;
		PairCollectionType clusterPairCollection(getClusterPairCollection(clusterCollection));
		for(const PairType& clusterPair: clusterPairCollection)
		{
			clusterPairAngles.emplace_back(getClusterPairAngle(clusterPair));
		}
		return clusterPairAngles;
	}
	TH1D getClusterPairAngles(const std::vector<Cluster>& clusterCollection, const std::string& histoName, const std::string& histoTitle) 
	{
		std::vector<float> pairAngles = getClusterPairAngles(clusterCollection);
		TH1D pairAnglesPlot(histoName.c_str(), histoTitle.c_str(), 100, 0.0, 3.15);
		for(const float& angle: pairAngles)
		{
			pairAnglesPlot.Fill(angle);
		}
		return pairAnglesPlot;
	}

	CollectionType getClustersWithSize(const std::vector<Cluster>& clusterCollection, const int& length) 
	{
		return
			filter(clusterCollection, [&length] (const Cluster& cluster) 
			{
				return cluster.size == length;
			});
	}

	CollectionType getClustersWithXLength(const std::vector<Cluster>& clusterCollection, const int& length) 
	{
		return
			filter(clusterCollection, [&length] (const Cluster& cluster) 
			{
				return cluster.sizeX == length;
			});
	}

	CollectionType getClustersWithYLength(const std::vector<Cluster>& clusterCollection, const int& length) 
	{
		return 
			filter(clusterCollection, [&length] (const Cluster& cluster) 
			{
				return cluster.sizeY == length;
			});
	}

	// Healthy: not tagged, unhealthy: tagged
	CollectionType getHealthyClustersWithSize(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithSize(clusterCollection, length), [] (const Cluster& cluster) 
		{
			return !isClusterTagged(cluster);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	CollectionType getHealthyClustersWithXLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithXLength(clusterCollection, length), [] (const Cluster& cluster) 
		{
			return !isClusterTagged(cluster);
		});
	}
	
	// Healthy: not tagged, unhealthy: tagged
	CollectionType getHealthyClustersWithYLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithYLength(clusterCollection, length), [] (const Cluster& cluster) 
		{
			return !isClusterTagged(cluster);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	CollectionType getUnhealthyClustersWithSize(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithSize(clusterCollection, length), [] (const Cluster& cluster) 
		{
			return isClusterTagged(cluster);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	CollectionType getUnhealthyClustersWithXLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithXLength(clusterCollection, length), [] (const Cluster& cluster) 
		{
			return isClusterTagged(cluster);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	CollectionType getUnhealthyClustersWithYLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithYLength(clusterCollection, length), [] (const Cluster& cluster) 
		{
			return isClusterTagged(cluster);
		});
	}

	PairCollectionType getRealPairsWithXLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter  = [&mergedLength] (const PairType& pair) { return calcPairXWidth(pair) == mergedLength; };
		const auto isEndTaggedFilter   = [] (const PairType& pair) { return areClustersEndTaggedPair(pair.first, pair.second); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isEndTaggedFilter));
	}

	PairCollectionType getRealPairsWithYLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter = [&mergedLength] (const PairType& pair) { return pair.first.sizeY + pair.second.sizeY + 2 == mergedLength; };
		const auto isEndTaggedFilter   = [] (const PairType& pair) { return areClustersEndTaggedPair(pair.first, pair.second); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isEndTaggedFilter));
	}

	PairCollectionType getFakePairsWithXLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter  = [&mergedLength] (const PairType& pair) { return calcPairXWidth(pair) == mergedLength; };
		const auto isNotEndTaggedFilter = [] (const PairType& pair) { return !areClustersEndTaggedPair(pair.first, pair.second); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isNotEndTaggedFilter));
	}

	PairCollectionType getFakePairsWithYLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter  = [&mergedLength] (const PairType& pair) { return pair.first.sizeY + pair.second.sizeY + 2 == mergedLength; };
		const auto isNotEndTaggedFilter = [] (const PairType& pair) { return !areClustersEndTaggedPair(pair.first, pair.second); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isNotEndTaggedFilter));
	}

	// Merging length: Y length of the cluster pairs after merging. It is basically the lengths of the 
	// clusters added together plus two (for the dcol loss)
	PairCollectionType getClusterPairsWithMergingLength(const std::vector<Cluster>& clusterCollection, const int& mergedLength) 
	{
		const auto mergingLengthFilter = [&mergedLength] (const PairType& pair) { return pair.first.sizeY + pair.second.sizeY + 2 == mergedLength; };
		return PairCollectionType(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter));
	}
} // ClusterPairFunctions

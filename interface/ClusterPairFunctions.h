#pragma once

#include "../interface/DataStructures_v5.h"
#include "../interface/HelperFunctionsCommon.h"

#include <TH2D.h>

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>

namespace ClusterPairFunctions
{
	using PairType                = std::pair<Cluster, Cluster>;
	using PairCollectionType      = std::vector<PairType>;
	using CollectionType          = std::vector<Cluster>;

	void                    printClusterFieldInfo(const Cluster& clusterField);
	int                     calcPairXWidth(const PairType& pair);
	int                     isClusterTagged(const Cluster& cluster);
	int                     isClusterTaggedInColumn(const Cluster& clusterField, int col);
	int                     areClustersPair(const Cluster& lhs, const Cluster& rhs);
	int                     areClustersEndTaggedPair(const Cluster& first, const Cluster& second);
	// PairCollectionType      getClusterPairCollection(const CollectionType& clusterCollection);
	// PairCollectionType      getClusterPairCollection(      CollectionType&  clusterCollection) = delete; // Only bind this to constant collections
	// PairCollectionType      getClusterPairCollection(      CollectionType&& clusterCollection) = delete; // Only bind this to constant collections
	PairCollectionType      getClusterPairCollection(const std::vector<Cluster>&  clusterCollection);
	PairCollectionType      getClusterPairCollection(const std::vector<Cluster>&  clusterCollection);
	PairCollectionType      getClusterPairCollection(      std::vector<Cluster>&  clusterCollection) = delete; // Only bind this to constant collections
	PairCollectionType      getClusterPairCollection(      std::vector<Cluster>&& clusterCollection) = delete; // Only bind this to constant collections
	float                   getAdcWeightedRowCoordinateInCol(const Cluster& cluster, const int& col);
	float                   getClusterIndAngle(const Cluster& cluster);
	float                   getClusterPairAngle(const PairType& pair);
	std::vector<float>      getClusterPairAngles(const std::vector<Cluster>&  clusterCollection);
	std::vector<float>      getClusterPairAngles(      std::vector<Cluster>&  clusterCollection) = delete; // Only bind this to constant collections
	std::vector<float>      getClusterPairAngles(      std::vector<Cluster>&& clusterCollection) = delete; // Only bind this to constant collections
	CollectionType          getClustersWithSize(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getClustersWithSize(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getClustersWithSize(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getClustersWithXLength(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getClustersWithXLength(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getClustersWithXLength(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getClustersWithYLength(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getClustersWithYLength(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getClustersWithYLength(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getHealthyClustersWithSize(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getHealthyClustersWithSize(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getHealthyClustersWithSize(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getHealthyClustersWithXLength(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getHealthyClustersWithXLength(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getHealthyClustersWithXLength(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getHealthyClustersWithYLength(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getHealthyClustersWithYLength(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getHealthyClustersWithYLength(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getUnhealthyClustersWithSize(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getUnhealthyClustersWithSize(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getUnhealthyClustersWithSize(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getUnhealthyClustersWithXLength(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getUnhealthyClustersWithXLength(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getUnhealthyClustersWithXLength(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getUnhealthyClustersWithYLength(const std::vector<Cluster>&  clusterCollection, const int& length);
	CollectionType          getUnhealthyClustersWithYLength(      std::vector<Cluster>&  clusterCollection, const int& length) = delete; // Only bind this to constant collections
	CollectionType          getUnhealthyClustersWithYLength(      std::vector<Cluster>&& clusterCollection, const int& length) = delete; // Only bind this to constant collections
	PairCollectionType      getRealPairsWithXLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength);
	PairCollectionType      getRealPairsWithXLength(      std::vector<Cluster>&  clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getRealPairsWithXLength(      std::vector<Cluster>&& clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getRealPairsWithYLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength);
	PairCollectionType      getRealPairsWithYLength(      std::vector<Cluster>&  clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getRealPairsWithYLength(      std::vector<Cluster>&& clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getFakePairsWithXLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength);
	PairCollectionType      getFakePairsWithXLength(      std::vector<Cluster>&  clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getFakePairsWithXLength(      std::vector<Cluster>&& clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getFakePairsWithYLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength);
	PairCollectionType      getFakePairsWithYLength(      std::vector<Cluster>&  clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getFakePairsWithYLength(      std::vector<Cluster>&& clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	TH1D                    getClusterPairAngles(const std::vector<Cluster>&  clusterCollection, const std::string& histoName, const std::string& histoTitle);
	TH1D                    getClusterPairAngles(      std::vector<Cluster>&  clusterCollection, const std::string& histoName, const std::string& histoTitle) = delete; // Only bind this to constant collections
	TH1D                    getClusterPairAngles(      std::vector<Cluster>&& clusterCollection, const std::string& histoName, const std::string& histoTitle) = delete; // Only bind this to constant collections
	PairCollectionType      getClusterPairsWithMergingLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength); 
	PairCollectionType      getClusterPairsWithMergingLength(      std::vector<Cluster>&  clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
	PairCollectionType      getClusterPairsWithMergingLength(      std::vector<Cluster>&& clusterCollection, const int& mergedLength) = delete; // Only bind this to constant collections
} // ClusterPairFunctions

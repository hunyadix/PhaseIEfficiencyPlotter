#pragma once

// Data structure
#include "../interface/DataStructures_v4.h"
#include "../interface/TimerColored.h"
#include "../interface/CommonActors.h"

#include <TROOT.h>
#include <TApplication.h>
#include <TError.h>
#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH2D.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "../interface/json.hpp"
using JSON = nlohmann::json;

class TrajMeasEfficiencyPlotsModule
{
   using LayersDiskPlotsCollection = std::array<TH1*, 46>;
   using EfficiencyPlotPair        = std::array<TH1*, 2>;
   private:
      static constexpr int                  ZEROBIAS_TRIGGER_BIT               = 0;
      static constexpr int                  ZEROBIAS_BITMASK                   = 1 << ZEROBIAS_TRIGGER_BIT;
      static constexpr int                  VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL  = 10;
      static constexpr int                  TRACK_QUALITY_HIGH_PURITY_BIT      = 2;
      static constexpr int                  TRACK_QUALITY_HIGH_PURITY_MASK     = 1 << TRACK_QUALITY_HIGH_PURITY_BIT;
      static constexpr float                TRACK_PT_CUT_N_MINUS_1_VAL         = 1.0f;
      static constexpr int                  TRACK_NSTRIP_CUT_N_MINUS_1_VAL     = 10;
      static constexpr std::array<float, 4> TRACK_D0_CUT_BARREL_N_MINUS_1_VAL  = {0.01f, 0.02f, 0.02f, 0.02f};
      static constexpr float                TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL = 0.05f;
      static constexpr float                TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL  = 0.01f;
      static constexpr float                TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL = 0.5f;
      static constexpr float                MEAS_HITSEP_CUT_N_MINUS_1_VAL      = 0.01f;
      static constexpr float                HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL   = 0.10f;
      static constexpr float                BARREL_MODULE_EDGE_X_CUT           = 0.6f;
      static constexpr float                BARREL_MODULE_EDGE_Y_CUT           = 3.0f;

      const EventData&       eventField_;
      const TrajMeasurement& trajField_;
      // Shortcuts
      const int&       det_;
      const int&       layer_;
      const int&       flipped_;
      const int&       module_;
      const int&       ladder_;
      const int&       side_;
      const int&       disk_;
      const int&       blade_;
      const int&       panel_;
      const int&       ring_;
      const int&       missing_;
      const float&     ladderCoord_;
      const float&     moduleCoord_;
      const float&     bladePanelCoord_;
      const float&     diskRingCoord_;
      const float&     lx_;
      const float&     ly_;
      const float&     lz_;
      const float&     glx_;
      const float&     gly_;
      const float&     glz_;
      const float&     norm_charge_;
      const TrackData& trk_;
      float            d_cl_;
      float            d_tr_;
      float            phi_;
      int              clust_near_;
      int              fillEfficiencyCondition_;
      int              absDisk_;
      int              panelOrientation_;
      int              layersDisks_;
      // Cut values
      int nvtxCut     = 0;
      int zerobiasCut = 0;
      int federrCut   = 0;
      int hpCut       = 0;
      int ptCut       = 0;
      int nstripCut   = 0;
      int d0Cut       = 0;
      int dzCut       = 0;
      int pixhitCut   = 0;
      int lxFidCut    = 0;
      int lyFidCut    = 0;
      int valmisCut   = 0;
      int hitsepCut   = 0;
      int effCutAll   = 0;
      int noVtxCut       = 0;
      // int noHpCut        = 0;
      int noPtCut        = 0;
      int noNStripCut    = 0;
      int noD0Cut        = 0;
      int noDZCut        = 0;
      int noFidicualsCut = 0;
      int noHitsepCut    = 0;
      // Counters
      int entry_counter_       = 0;
      int nvtxCut_counter_     = 0;
      int zerobiasCut_counter_ = 0;
      int federrCut_counter_   = 0;
      int hpCut_counter_       = 0;
      int ptCut_counter_       = 0;
      int nstripCut_counter_   = 0;
      int d0Cut_counter_       = 0;
      int dzCut_counter_       = 0;
      int pixhitCut_counter_   = 0;
      int lxFidCut_counter_    = 0;
      int lyFidCut_counter_    = 0;
      int valmisCut_counter_   = 0;
      int hitsepCut_counter_   = 0;
      int effCutAll_counter_   = 0;
      // Histogram helpers
      enum LayersDiskPlotIndecies
      {
         LayersAndDisks = 0,
         AllLayers,
         AllDisks,
         DisksPositiveZ,
         DisksNegativeZ,
         Layer1,
         Layer2,
         Layer3,
         Layer4,
         Layer1Inner,
         Layer2Inner,
         Layer3Inner,
         Layer4Inner,
         Layer1Outer,
         Layer2Outer,
         Layer3Outer,
         Layer4Outer,
         Disk1,
         Disk2,
         Disk3,
         Disk4,
         Disk5,
         Disk6,
         LayersAndDisksEfficiency,
         AllLayersEfficiency,
         AllDisksEfficiency,
         DisksPositiveZEfficiency,
         DisksNegativeZEfficiency,
         Layer1Efficiency,
         Layer2Efficiency,
         Layer3Efficiency,
         Layer4Efficiency,
         Layer1InnerEfficiency,
         Layer2InnerEfficiency,
         Layer3InnerEfficiency,
         Layer4InnerEfficiency,
         Layer1OuterEfficiency,
         Layer2OuterEfficiency,
         Layer3OuterEfficiency,
         Layer4OuterEfficiency,
         Disk1Efficiency,
         Disk2Efficiency,
         Disk3Efficiency,
         Disk4Efficiency,
         Disk5Efficiency,
         Disk6Efficiency
      };
      const std::array<std::string, 23> plotNameSuffices = 
      {{
         "LayersAndDisks",
         "AllLayers",
         "AllDisks",
         "DisksPositiveZ",
         "DisksNegativeZ",
         "Layer1",
         "Layer2",
         "Layer3",
         "Layer4",
         "Layer1Inner",
         "Layer2Inner",
         "Layer3Inner",
         "Layer4Inner",
         "Layer1Outer",
         "Layer2Outer",
         "Layer3Outer",
         "Layer4Outer",
         "Disk1",
         "Disk2",
         "Disk3",
         "Disk4",
         "Disk5",
         "Disk6",
      }};
      const std::array<std::string, 23> plotTitleSuffices = 
      {{
         "all layers and disks",
         "all layers",
         "all disks",
         "disks, positivez",
         "disks, negativez",
         "layer 1",
         "layer 2",
         "layer 3",
         "layer 4",
         "layer 1 inner modules",
         "layer 2 inner modules",
         "layer 3 inner modules",
         "layer 4 inner modules",
         "layer 1 outer modules",
         "layer 2 outer modules",
         "layer 3 outer modules",
         "layer 4 outer modules",
         "disk 1",
         "disk 2",
         "disk 3",
         "disk 4",
         "disk 5",
         "disk 6"
      }};
      const std::array<std::string, 8> orientationTextDefinitions =
      {{
         "positive z, inner ring, panel 1",
         "positive z, inner ring, panel 2",
         "positive z, outer ring, panel 1",
         "positive z, outer ring, panel 2",
         "negative z, inner ring, panel 1",
         "negative z, inner ring, panel 2",
         "negative z, outer ring, panel 1",
         "negative z, outer ring, panel 2"
      }};
      // Histograms
      EfficiencyPlotPair                       layersDisksEfficiencyPlots;
      LayersDiskPlotsCollection                rechitOccupancyROCPlots;
      LayersDiskPlotsCollection                efficiencyROCPlots;
      LayersDiskPlotsCollection                rechitOccupancyPhiVsZPlots;
      LayersDiskPlotsCollection                efficiencyPhiVsZPlots;
      EfficiencyPlotPair                       vtxNtrkEfficiencyPreCutsPlots;
      EfficiencyPlotPair                       vtxNtrkEfficiencyWithCutsPlots;
      EfficiencyPlotPair                       ptEfficiencyPreCutsPlots;
      EfficiencyPlotPair                       ptEfficiencyWithCutsPlots;
      EfficiencyPlotPair                       striphitsEfficiencyPreCutsPlots;
      EfficiencyPlotPair                       striphitsEfficiencyWithCutsPlots;
      LayersDiskPlotsCollection                lxEfficiencyPreCutsPlots;
      LayersDiskPlotsCollection                lxEfficiencyWithCutsPlots;
      LayersDiskPlotsCollection                lyEfficiencyPreCutsPlots;
      LayersDiskPlotsCollection                lyEfficiencyWithCutsPlots;
      LayersDiskPlotsCollection                lyVsLxEfficiencyPreCutsPlots;
      LayersDiskPlotsCollection                lyVsLxEfficiencyWithCutsPlots;
      LayersDiskPlotsCollection                clustDistPreCutsPlots;
      EfficiencyPlotPair                       clustDistWithCutsPlots;
      LayersDiskPlotsCollection                hitDistPreCuts;
      EfficiencyPlotPair                       hitDistWithCutsPlots;
      LayersDiskPlotsCollection                d0PreCutsPlots;
      LayersDiskPlotsCollection                d0WithCutsPlots;
      LayersDiskPlotsCollection                dzPreCutsPlots;
      LayersDiskPlotsCollection                dzWithCutsPlots;
      std::array<LayersDiskPlotsCollection, 8> forwardLocalPositionsByOrientationEfficiencyPlots;
   public:
      TrajMeasEfficiencyPlotsModule(const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg);
      ~TrajMeasEfficiencyPlotsModule() = default;
      void defineHistograms();
      void fillHistograms();
      void fillFullLayersDiskPlotsCollectionsAtDetectorPart(const LayersDiskPlotIndecies& index, const LayersDiskPlotIndecies& efficiencyIndex);
      void savePlots(const JSON& config);
      void saveHistogramsInCollectionIfNotEmpty(const LayersDiskPlotsCollection& collection, const std::string& subdirectoryName, const JSON& config);
      void saveHistogramsInCollectionIfNotEmpty(const EfficiencyPlotPair& collection, const std::string& subdirectoryName, const JSON& config);
      void saveHistogramInSubdirectory(TH1* histogram, const std::string& subdirectoryName, const JSON& config);
      bool histogramExistAndNotEmpty(TH1* histogram);
      void draw1DPlot(TH1D* histogram, TCanvas* canvas);
      void draw2DPlot(TH2D* histogram, TCanvas* canvas);
      void dressIfROCPlot(TH2D* histogram);
      int  plotIndexToLayerToDress(LayersDiskPlotIndecies plotIndex);
      void printCounters();
      void printCutValues();
   private:
      void calculateCuts();
      void incrementCounters();
      void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const int& cuts = 1);
      void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts);
      void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const int& cuts = 1);
      void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts);
};
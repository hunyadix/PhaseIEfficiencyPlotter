#include "../interface/EfficiencyPlotsModule.h"

#include "../interface/common_functions_jkarancs.h"


constexpr int                  EfficiencyPlotsModule::ZEROBIAS_TRIGGER_BIT;
constexpr int                  EfficiencyPlotsModule::ZEROBIAS_BITMASK;
constexpr int                  EfficiencyPlotsModule::VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL;
constexpr int                  EfficiencyPlotsModule::TRACK_QUALITY_HIGH_PURITY_BIT;
constexpr int                  EfficiencyPlotsModule::TRACK_QUALITY_HIGH_PURITY_MASK;
constexpr float                EfficiencyPlotsModule::TRACK_PT_CUT_N_MINUS_1_VAL;
constexpr int                  EfficiencyPlotsModule::TRACK_NSTRIP_CUT_N_MINUS_1_VAL;
constexpr std::array<float, 4> EfficiencyPlotsModule::TRACK_D0_CUT_BARREL_N_MINUS_1_VAL;
constexpr float                EfficiencyPlotsModule::TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL;
constexpr float                EfficiencyPlotsModule::TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL;
constexpr float                EfficiencyPlotsModule::TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL;
constexpr float                EfficiencyPlotsModule::MEAS_HITSEP_CUT_N_MINUS_1_VAL;
constexpr float                EfficiencyPlotsModule::HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL;
constexpr float                EfficiencyPlotsModule::BARREL_MODULE_EDGE_X_CUT;
constexpr float                EfficiencyPlotsModule::BARREL_MODULE_EDGE_Y_CUT;

EfficiencyPlotsModule::EfficiencyPlotsModule(const EventData& clusterEventFieldArg, const Cluster& clusterFieldArg, const EventData& trajEventFieldArg, const TrajMeasurement& trajFieldArg): 
   clusterEventField_     (clusterEventFieldArg),
   clusterField_          (clusterFieldArg),
   trajEventField_        (trajEventFieldArg),
   trajField_             (trajFieldArg)
{
   defineHistograms();
}

void EfficiencyPlotsModule::defineHistograms()
{
   // Plot pairs
   layersDisksEfficiencyPlots[0]       = new TH1D("layersDisksEfficiencyPlots_Hits",       "num. hits. used to calculate efficiency, layers 1-4, disks 1-3 [with cuts]",                    7,   0.5,   7.5);
   layersDisksEfficiencyPlots[1]       = new TH1D("layersDisksEfficiencyPlots_Eff.",       "efficiency, layers 1-4, disks 1-3 [with cuts];;efficiency",                                     7,   0.5,   7.5);
   vtxNtrkEfficiencyPreCutsPlots[0]    = new TH1D("vtxNtrkEfficiencyPreCutsPlots_Hits",    "parent vertex n. tracks [no cuts];track number",                                                200, 0.0, 200.0);
   // vtxNtrkEfficiencyPreCutsPlots[1]    = new TH1D("vtxNtrkEfficiencyPreCutsPlots_Eff.",    "eff. vs parent vertex n. tracks [no cuts];track number;efficiency",                             200, 0.0, 200.0);
   vtxNtrkEfficiencyWithCutsPlots[0]   = new TH1D("vtxNtrkEfficiencyWithCutsPlots_Hits",   "parent vertex n. tracks [with other effcuts];parent vtx. tracks",                               200, 0.0, 200.0);
   vtxNtrkEfficiencyWithCutsPlots[1]   = new TH1D("vtxNtrkEfficiencyWithCutsPlots_Eff.",   "eff. vs parent vertex n. tracks [with other effcuts];parent vtx. tracks",                       200, 0.0, 200.0);
   ptEfficiencyPreCutsPlots[0]         = new TH1D("ptEfficiencyPreCutsPlots_Hits",         "pt [no cuts];pt (GeV/c)",                                                                       60,  0.0, 10.0);
   // ptEfficiencyPreCutsPlots[1]         = new TH1D("ptEfficiencyPreCutsPlots_Eff.",         "eff. vs pt [no cuts];pt (GeV/c);efficiency",                                                    30,  0.0, 10.0);
   ptEfficiencyWithCutsPlots[0]        = new TH1D("ptEfficiencyWithCutsPlots_Hits",        "pt [with other effcuts];pt (GeV/c)",                                                            60,  0.0, 10.0);
   ptEfficiencyWithCutsPlots[1]        = new TH1D("ptEfficiencyWithCutsPlots_Eff.",        "eff. vs pt [with other effcuts];pt (GeV/c);efficiency",                                         60,  0.0, 10.0);
   striphitsEfficiencyPreCutsPlots[0]  = new TH1D("striphitsEfficiencyPreCutsPlots_Hits",  "strip hits [no cuts];strip hits",                                                               30,  0.0, 30.0);
   // striphitsEfficiencyPreCutsPlots[1]  = new TH1D("striphitsEfficiencyPreCutsPlots_Eff.",  "eff. vs n. strip hits [no cuts];strip hits;efficiency",                                         30,  0.0, 30.0);
   striphitsEfficiencyWithCutsPlots[0] = new TH1D("striphitsEfficiencyWithCutsPlots_Hits", "strip hits [with other effcuts];strip hits",                                                    30,  0.0, 30.0);
   striphitsEfficiencyWithCutsPlots[1] = new TH1D("striphitsEfficiencyWithCutsPlots_Eff.", "eff. vs n. strip hits [with other effcuts];strip hits;efficiency",                              30,  0.0, 30.0);
   clustDistWithCutsPlots[0]           = new TH1D("clustDistWithCutsPlots_Hits",           "closest cluster distance to traj. meas. [with other effcuts];distance (cm)",                    200,  0.0, 0.025);
   clustDistWithCutsPlots[1]           = new TH1D("clustDistWithCutsPlots_Eff.",           "eff. vs closest cluster distance to traj. meas. [with other effcuts];distance (cm);efficiency", 200,  0.0, 0.025);
   hitDistWithCutsPlots[0]             = new TH1D("hitDistWithCutsPlots_Hits",             "closest hit distance to traj. meas. [with other effcuts];distance (cm)",                        200, 0.0, 0.12);
   hitDistWithCutsPlots[1]             = new TH1D("hitDistWithCutsPlots_Eff.",             "eff. vs closest hit distance to traj. meas. [with other effcuts];distance (cm);efficiency",     200, 0.0, 0.12 );
   // Plot collections
   for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
   {
      clusterPhiVsZPlots[plotIndex]                 = new TH2D(("clusterPhiVsZPlots_"            + plotNameSuffices[plotIndex]         ).c_str(), ("cluster occupancy, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                       150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      clusterGlyVsGlxPlots[plotIndex]               = new TH2D(("clusterGlyVsGlxPlots_"          + plotNameSuffices[plotIndex]         ).c_str(), ("cluster occupancy, Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),                   300, -16.2,  16.2,  300, -16.2,     16.2);
      clusterZPlots[plotIndex]                      = new TH1D(("clusterZPlots_"                 + plotNameSuffices[plotIndex]         ).c_str(), ("cluster Z distribution - " + plotTitleSuffices[plotIndex] + ";global z (cm)").c_str(),                                    150, -52.15, 52.15);
      rechitOccupancyPhiVsZPlots[plotIndex]         = new TH2D(("rechitOccupancyPhiVsZPlots_"    + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("rechit occupancy, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                       150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      efficiencyPhiVsZPlots[plotIndex]              = new TH2D(("efficiencyPhiVsZPlots_"         + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. hits used to calc. eff., phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                           150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      efficiencyPhiVsZPlots[plotIndex + 23]         = new TH2D(("efficiencyPhiVsZPlots_"         + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("efficiency, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                             150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      rechitOccupancyGlyVsGlxPlots[plotIndex]       = new TH2D(("rechitOccupancyGlyVsGlxPlots_"  + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("rechit occupancy, Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),                   300, -16.2,  16.2,  300, -16.2,     16.2);
      efficiencyGlyVsGlxPlots[plotIndex]            = new TH2D(("efficiencyGlyVsGlxPlots_"       + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. hits used to calc. eff., Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),       300, -16.2,  16.2,  300, -16.2,     16.2);
      efficiencyGlyVsGlxPlots[plotIndex + 23]       = new TH2D(("efficiencyGlyVsGlxPlots_"       + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("efficiency, Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),                         300, -16.2,  16.2,  300, -16.2,     16.2);
      lxEfficiencyPreCutsPlots[plotIndex]           = new TH1D(("lxEfficiencyPreCutsPlots"       + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("eff. vs lx - " + plotTitleSuffices[plotIndex] + " [no cuts];lx (cm)").c_str(),                                           40,   -0.9,   0.9);
      lxEfficiencyWithCutsPlots[plotIndex]          = new TH1D(("lxEfficiencyWithCutsPlots"      + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("lx - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm)").c_str(),                                        40,   -0.9,   0.9);
      lxEfficiencyWithCutsPlots[plotIndex + 23]     = new TH1D(("lxEfficiencyWithCutsPlots_"     + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs lx - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);efficiency").c_str(),                     40,   -0.9,   0.9);
      lyEfficiencyPreCutsPlots[plotIndex]           = new TH1D(("lyEfficiencyPreCutsPlots"       + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("eff. vs ly - " + plotTitleSuffices[plotIndex] + " [no cuts];ly (cm)").c_str(),                                           104,  -3.5,   3.5);
      lyEfficiencyWithCutsPlots[plotIndex]          = new TH1D(("lyEfficiencyWithCutsPlots"      + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("ly - " + plotTitleSuffices[plotIndex] + " [with other effcuts];ly (cm)").c_str(),                                        104,  -3.5,   3.5);
      lyEfficiencyWithCutsPlots[plotIndex + 23]     = new TH1D(("lyEfficiencyWithCutsPlots_"     + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs ly - " + plotTitleSuffices[plotIndex] + " [with other effcuts];ly (cm);efficiency").c_str(),                     104,  -3.5,   3.5);
      lyVsLxEfficiencyPreCutsPlots[plotIndex]       = new TH2D(("lyVsLxEfficiencyPreCutsPlots"   + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [no cuts];lx (cm);ly (cm)").c_str(),            40,   -0.9,   0.9,  104,  -3.5,      3.5);
      lyVsLxEfficiencyWithCutsPlots[plotIndex]      = new TH2D(("lyVsLxEfficiencyWithCutsPlots"  + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(), 40,   -0.9,   0.9,  104,  -3.5,      3.5);
      lyVsLxEfficiencyWithCutsPlots[plotIndex + 23] = new TH2D(("lyVsLxEfficiencyWithCutsPlots_" + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(), 40,   -0.9,   0.9,  104,  -3.5,      3.5);
      clustDistPreCutsPlots[plotIndex]              = new TH1D(("clustDistPreCutsPlots_"         + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("closest cluster distance to traj. meas. - " + plotTitleSuffices[plotIndex] + " [no cuts];distance (cm)").c_str(),        200,   0.0,   0.025);
      hitDistPreCuts[plotIndex]                     = new TH1D(("hitDistPreCuts_"                + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("closest hit distance to traj. meas. - " + plotTitleSuffices[plotIndex] + " [no cuts];distance (cm)").c_str(),            200,   0.0,   0.12);
      d0PreCutsPlots[plotIndex]                     = new TH1D(("d0PreCutsPlots_"                + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("d0 - " + plotTitleSuffices[plotIndex] + " hits [no cuts];d0 (cm)").c_str(),                                              200,   0.0,   0.15);
      d0WithCutsPlots[plotIndex]                    = new TH1D(("d0WithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("d0 - " + plotTitleSuffices[plotIndex] + "[with other effcuts];d0 (cm)").c_str(),                                         200,   0.0,   0.15);
      d0WithCutsPlots[plotIndex + 23]               = new TH1D(("d0WithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs d0 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];d0 (cm);efficiency").c_str(),                     200,   0.0,   0.15);
      dzPreCutsPlots[plotIndex]                     = new TH1D(("dzPreCutsPlots_"                + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("dz - " + plotTitleSuffices[plotIndex] + " [no cuts];dz (cm)").c_str(),                                                   200,   0.0,   0.5);
      dzWithCutsPlots[plotIndex]                    = new TH1D(("dzWithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("dz - " + plotTitleSuffices[plotIndex] + " [with other effcuts];dz (cm)").c_str(),                                        200,   0.0,   0.5);
      dzWithCutsPlots[plotIndex + 23]               = new TH1D(("dzWithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs dz - " + plotTitleSuffices[plotIndex] + " [with other effcuts];dz (cm);efficiency").c_str(),                     200,   0.0,   0.5);
      // Different binning based on detector part
   }
   // Fwd.
   clusterOccupancyROCPlots[AllDisks]       = new TH2D(("clusterOccupancyROCPlots_"       + plotNameSuffices[AllDisks]          ).c_str(), ("cluster occupancy - " + plotTitleSuffices[AllDisks] + ";disk, ring;blade, panel").c_str(),                             112, -3.5, 3.5, 140, -17.5, 17.5);
   rechitOccupancyROCPlots[AllDisks]        = new TH2D(("rechitOccupancyROCPlots_"        + plotNameSuffices[AllDisks] + "_Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[AllDisks] + ";disk, ring;blade, panel").c_str(),                              112, -3.5, 3.5, 140, -17.5, 17.5);
   efficiencyROCPlots[AllDisks]             = new TH2D(("efficiencyROCPlots_"             + plotNameSuffices[AllDisks] + "_Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[AllDisks] + " [with effcuts];disk, ring;blade, panel").c_str(), 112, -3.5, 3.5, 140, -17.5, 17.5);
   efficiencyROCPlots[AllDisksEfficiency]   = new TH2D(("efficiencyROCPlots_"             + plotNameSuffices[AllDisks] + "_Eff.").c_str(), ("ROC efficiency - " + plotTitleSuffices[AllDisks] + " [with effcuts];disk, ring;blade, panel;efficiency").c_str(),      112, -3.5, 3.5, 140, -17.5, 17.5);
   rocEfficiencyDistributionPlots[AllDisks] = new TH1D(("rocEfficiencyDistributionPlots_" + plotNameSuffices[AllDisks] + "_Hits").c_str(), ("ROC efficiency distribution - " + plotTitleSuffices[AllDisks] + " [with effcuts];").c_str(),                           401, 0.0, 1.025);

   // L1
   clusterOccupancyROCPlots[Layer1]       = new TH2D(("clusterOccupancyROCPlots_"         + plotNameSuffices[Layer1]          ).c_str(), ("cluster occupancy - " + plotTitleSuffices[Layer1] + ";module;ladders").c_str(),                                          72, -4.5, 4.5,  26,  -6.5,  6.5);
   rechitOccupancyROCPlots[Layer1]        = new TH2D(("rechitOccupancyROCPlots_"          + plotNameSuffices[Layer1] + "_Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[Layer1] + ";module;ladders").c_str(),                                           72, -4.5, 4.5,  26,  -6.5,  6.5);
   efficiencyROCPlots[Layer1]             = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer1] + "_Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[Layer1] + " [with effcuts];module;ladders").c_str(),              72, -4.5, 4.5,  26,  -6.5,  6.5);
   efficiencyROCPlots[Layer1Efficiency]   = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer1] + "_Eff.").c_str(), ("ROC efficiency - " + plotTitleSuffices[Layer1] + " [with effcuts];module;ladders;efficiency").c_str(),                   72, -4.5, 4.5,  26,  -6.5,  6.5);
   rocEfficiencyDistributionPlots[Layer1] = new TH1D(("rocEfficiencyDistributionPlots_"   + plotNameSuffices[Layer1] + "_Hits").c_str(), ("ROC efficiency distribution - " + plotTitleSuffices[Layer1] + " [with effcuts];").c_str(),                               401, 0.0, 1.025);
   // L2
   clusterOccupancyROCPlots[Layer2]       = new TH2D(("clusterOccupancyROCPlots_"         + plotNameSuffices[Layer2]          ).c_str(), ("cluster occupancy - " + plotTitleSuffices[Layer2] + ";module;ladders").c_str(),                                          72, -4.5, 4.5,  58, -14.5, 14.5);
   rechitOccupancyROCPlots[Layer2]        = new TH2D(("rechitOccupancyROCPlots_"          + plotNameSuffices[Layer2] + "_Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[Layer2] + ";module;ladders").c_str(),                                           72, -4.5, 4.5,  58, -14.5, 14.5);
   efficiencyROCPlots[Layer2]             = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer2] + "_Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[Layer2] + " [with effcuts];module;ladders").c_str(),              72, -4.5, 4.5,  58, -14.5, 14.5);
   efficiencyROCPlots[Layer2Efficiency]   = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer2] + "_Eff.").c_str(), ("ROC efficiency - " + plotTitleSuffices[Layer2] + " [with effcuts];module;ladders;efficiency").c_str(),                   72, -4.5, 4.5,  58, -14.5, 14.5);
   rocEfficiencyDistributionPlots[Layer2] = new TH1D(("rocEfficiencyDistributionPlots_"   + plotNameSuffices[Layer2] + "_Hits").c_str(), ("ROC efficiency distribution - " + plotTitleSuffices[Layer2] + " [with effcuts];").c_str(),                               401, 0.0, 1.025);
   // L3
   clusterOccupancyROCPlots[Layer3]       = new TH2D(("clusterOccupancyROCPlots_"         + plotNameSuffices[Layer3]          ).c_str(), ("cluster occupancy - " + plotTitleSuffices[Layer3] + ";module;ladders").c_str(),                                          72, -4.5, 4.5,  90, -22.5, 22.5);
   rechitOccupancyROCPlots[Layer3]        = new TH2D(("rechitOccupancyROCPlots_"          + plotNameSuffices[Layer3] + "_Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[Layer3] + ";module;ladders").c_str(),                                           72, -4.5, 4.5,  90, -22.5, 22.5);
   efficiencyROCPlots[Layer3]             = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer3] + "_Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[Layer3] + " [with effcuts];module;ladders").c_str(),              72, -4.5, 4.5,  90, -22.5, 22.5);
   efficiencyROCPlots[Layer3Efficiency]   = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer3] + "_Eff.").c_str(), ("ROC efficiency - " + plotTitleSuffices[Layer3] + " [with effcuts];module;ladders;efficiency").c_str(),                   72, -4.5, 4.5,  90, -22.5, 22.5);
   rocEfficiencyDistributionPlots[Layer3] = new TH1D(("rocEfficiencyDistributionPlots_"   + plotNameSuffices[Layer3] + "_Hits").c_str(), ("ROC efficiency distribution - " + plotTitleSuffices[Layer3] + " [with effcuts];").c_str(),                               401, 0.0, 1.025);
   // L4
   clusterOccupancyROCPlots[Layer4]       = new TH2D(("clusterOccupancyROCPlots_"         + plotNameSuffices[Layer4]          ).c_str(), ("cluster occupancy - " + plotTitleSuffices[Layer4] + ";module;ladders").c_str(),                                          72, -4.5, 4.5, 130, -32.5, 32.5);
   rechitOccupancyROCPlots[Layer4]        = new TH2D(("rechitOccupancyROCPlots_"          + plotNameSuffices[Layer4] + "_Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[Layer4] + ";module;ladders").c_str(),                                           72, -4.5, 4.5, 130, -32.5, 32.5);
   efficiencyROCPlots[Layer4]             = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer4] + "_Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[Layer4] + " [with effcuts];module;ladders").c_str(),              72, -4.5, 4.5, 130, -32.5, 32.5);
   efficiencyROCPlots[Layer4Efficiency]   = new TH2D(("efficiencyROCPlots_"               + plotNameSuffices[Layer4] + "_Eff.").c_str(), ("ROC efficiency - " + plotTitleSuffices[Layer4] + " [with effcuts];module;ladders;efficiency").c_str(),                   72, -4.5, 4.5, 130, -32.5, 32.5);
   rocEfficiencyDistributionPlots[Layer4] = new TH1D(("rocEfficiencyDistributionPlots_"   + plotNameSuffices[Layer4] + "_Hits").c_str(), ("ROC efficiency distribution - " + plotTitleSuffices[Layer4] + " [with effcuts];").c_str(),                               401, 0.0, 1.025);
   for(unsigned int orientationIndex      = 0; orientationIndex < 8;                      ++orientationIndex)
   {
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex] = std::array<TH1*, 46> {nullptr};
      std::string orientationIndexString = std::to_string(orientationIndex + 1);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][AllDisks]           = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_AllDisks_"           + orientationIndexString).c_str(), ("num. of hits vs hit position - all disks, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)")           .c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][AllDisksEfficiency] = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_AllDisksEfficiency_" + orientationIndexString).c_str(), ("eff. vs hit position - all disks, "          + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
      std::array<LayersDiskPlotIndecies, 6> disks = {Disk1, Disk2, Disk3, Disk4, Disk5, Disk6};
      for(unsigned int diskIndex = 0; diskIndex < disks.size(); ++diskIndex)
      {
         forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][disks[diskIndex]]              = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk" + std::to_string(diskIndex + 1) + "_"              + orientationIndexString).c_str(), ("num. of hits vs hit position - disk " + std::to_string(diskIndex + 1) + ", "    + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)")           .c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
         forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][disks[diskIndex] + 23]         = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk" + std::to_string(diskIndex + 1) + "Efficiency_"    + orientationIndexString).c_str(), ("eff. vs hit position - disk " + std::to_string(diskIndex + 1) + ", "            + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
      }
   }
   std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void EfficiencyPlotsModule::fillClusterHistograms()
{
   static const int&       det             = clusterField_.mod_on.det;
   static const int&       layer           = clusterField_.mod_on.layer;
   static const int&       disk            = clusterField_.mod_on.disk;
   static const float&     ladderCoord     = clusterField_.mod_on.ladder_coord;
   static const float&     moduleCoord     = clusterField_.mod_on.module_coord;
   static const float&     bladePanelCoord = clusterField_.mod_on.blade_panel_coord;
   static const float&     diskRingCoord   = clusterField_.mod_on.disk_ring_coord;
   static const float&     glx             = clusterField_.glx;
   static const float&     gly             = clusterField_.gly;
   static const float&     glz             = clusterField_.glz;
   const float phi  = atan2(gly, glx);
   auto fillFullLayersDiskPlotsCollectionsAtDetectorPart = [&] (const LayersDiskPlotIndecies& index)
   {
      clusterZPlots[index]        -> Fill(glz);
      clusterPhiVsZPlots[index]   -> Fill(glz, phi);
      clusterGlyVsGlxPlots[index] -> Fill(glx, gly);
   };
   fillFullLayersDiskPlotsCollectionsAtDetectorPart(LayersAndDisks);
   if(det == 0)
   {
      fillFullLayersDiskPlotsCollectionsAtDetectorPart(AllLayers);
      if(layer == 1)
      {
         clusterOccupancyROCPlots[Layer1] -> Fill(moduleCoord, ladderCoord);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer1);
      }
      if(layer == 2)
      {
         clusterOccupancyROCPlots[Layer2] -> Fill(moduleCoord, ladderCoord);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer2);
      }
      if(layer == 3)
      {
         clusterOccupancyROCPlots[Layer3] -> Fill(moduleCoord, ladderCoord);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer3);
      }
      if(layer == 4)
      {
         clusterOccupancyROCPlots[Layer4] -> Fill(moduleCoord, ladderCoord);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer4);
      }
   }
   if(det == 1)
   {
      clusterOccupancyROCPlots[AllDisks] -> Fill(diskRingCoord, bladePanelCoord);
      fillFullLayersDiskPlotsCollectionsAtDetectorPart(AllDisks);
      if(0 <= glz) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksNegativeZ);
      if(glz < 0 ) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksPositiveZ);
      if(disk == -1) fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk1);
      if(disk == -2) fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk2);
      if(disk == -3) fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk3);
      if(disk == 1)  fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk4);
      if(disk == 2)  fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk5);
      if(disk == 3)  fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk6);
   }
}

void EfficiencyPlotsModule::fillTrajMeasHistograms()
{
   // Shortcuts
   static const int&       det             = trajField_.mod_on.det;
   static const int&       layer           = trajField_.mod_on.layer;
   static const int&       side            = trajField_.mod_on.side;
   static const int&       disk            = trajField_.mod_on.disk;
   static const int&       panel           = trajField_.mod_on.panel;
   static const int&       ring            = trajField_.mod_on.ring;
   static const int&       missing         = trajField_.missing;
   static const float&     ladderCoord     = trajField_.mod_on.ladder_coord;
   static const float&     moduleCoord     = trajField_.mod_on.module_coord;
   static const float&     bladePanelCoord = trajField_.mod_on.blade_panel_coord;
   static const float&     diskRingCoord   = trajField_.mod_on.disk_ring_coord;
   static const float&     lx              = trajField_.lx;
   static const float&     ly              = trajField_.ly;
   static const float&     glx             = trajField_.glx;
   static const float&     gly             = trajField_.gly;
   static const float&     glz             = trajField_.glz;
   // static const float&     norm_charge_     = trajField_.norm_charge;
   static const float&     d_cl            = trajField_.d_cl;
   static const float&     d_tr            = trajField_.d_tr;
   static const TrackData& trk             = trajField_.trk;
   // FIXME: this recalculation should be obsolete now
   const float phi  = atan2(gly, glx);
   const int   clust_near              = (0 < d_cl) && (d_cl < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL);
   const int   fillEfficiencyCondition = !missing || clust_near;
   const int   absDisk          = std::abs(disk);
   const int   panelOrientation = (side - 1) * 4 + std::abs(ring % 2) * 2 + panel - 1; // +Z, -Z, ring 1, ring 2, panel 1, panel 2
   const int   layersDisks = det == 0 ? layer : 4 + absDisk;
   auto fillFullLayersDiskPlotsCollectionsAtDetectorPart = [&] (const LayersDiskPlotIndecies& index, const LayersDiskPlotIndecies& efficiencyIndex)
   {
      rechitOccupancyPhiVsZPlots[index]   -> Fill(glz, phi);
      rechitOccupancyGlyVsGlxPlots[index] -> Fill(glx, gly);
      lxEfficiencyPreCutsPlots[index]     -> Fill(lx);
      lyEfficiencyPreCutsPlots[index]     -> Fill(ly);
      lyVsLxEfficiencyPreCutsPlots[index] -> Fill(lx, ly);
      clustDistPreCutsPlots[index]        -> Fill(d_cl);
      hitDistPreCuts[index]               -> Fill(d_tr);
      d0PreCutsPlots[index]               -> Fill(trk.d0);
      dzPreCutsPlots[index]               -> Fill(trk.dz);
      fillPairs(efficiencyPhiVsZPlots[index],         efficiencyPhiVsZPlots[efficiencyIndex],         glz,    phi, fillEfficiencyCondition, effCutAll     );
      fillPairs(efficiencyGlyVsGlxPlots[index],       efficiencyGlyVsGlxPlots[efficiencyIndex],       glx,    glx, fillEfficiencyCondition, effCutAll     );
      fillPairs(lxEfficiencyWithCutsPlots[index],     lxEfficiencyWithCutsPlots[efficiencyIndex],     lx,          fillEfficiencyCondition, noFidicualsCut);
      fillPairs(lyEfficiencyWithCutsPlots[index],     lyEfficiencyWithCutsPlots[efficiencyIndex],     ly,          fillEfficiencyCondition, noFidicualsCut);
      fillPairs(lyVsLxEfficiencyWithCutsPlots[index], lyVsLxEfficiencyWithCutsPlots[efficiencyIndex], lx,     ly,  fillEfficiencyCondition, noFidicualsCut);
      fillPairs(d0WithCutsPlots[index],               d0WithCutsPlots[efficiencyIndex],               trk.d0,      fillEfficiencyCondition, noD0Cut       );
      fillPairs(dzWithCutsPlots[index],               dzWithCutsPlots[efficiencyIndex],               trk.dz,      fillEfficiencyCondition, noDZCut       );
   };
   calculateCuts();
   vtxNtrkEfficiencyPreCutsPlots[0]   -> Fill(trk.fromVtxNtrk);
   ptEfficiencyPreCutsPlots[0]        -> Fill(trk.pt);
   striphitsEfficiencyPreCutsPlots[0] -> Fill(trk.strip);
   fillPairs(layersDisksEfficiencyPlots[0],       layersDisksEfficiencyPlots[1],       layersDisks,     fillEfficiencyCondition,  effCutAll  );
   fillPairs(vtxNtrkEfficiencyWithCutsPlots[0],   vtxNtrkEfficiencyWithCutsPlots[1],   trk.fromVtxNtrk, fillEfficiencyCondition, noVtxCut   );
   fillPairs(ptEfficiencyWithCutsPlots[0],        ptEfficiencyWithCutsPlots[1],        trk.pt,          fillEfficiencyCondition, noPtCut    );
   fillPairs(striphitsEfficiencyWithCutsPlots[0], striphitsEfficiencyWithCutsPlots[1], trk.strip,       fillEfficiencyCondition, noNStripCut);
   fillPairs(clustDistWithCutsPlots[0],           clustDistWithCutsPlots[1],           d_cl,            !missing,                 effCutAll  );
   fillPairs(hitDistWithCutsPlots[0],             hitDistWithCutsPlots[1],             d_tr,            fillEfficiencyCondition,  noHitsepCut);
   fillFullLayersDiskPlotsCollectionsAtDetectorPart(LayersAndDisks, LayersAndDisksEfficiency);
   if(det == 0)
   {
      fillFullLayersDiskPlotsCollectionsAtDetectorPart(AllLayers, AllLayersEfficiency);
      if(layer == 1)
      {
         rechitOccupancyROCPlots[Layer1] -> Fill(moduleCoord, ladderCoord);
         fillPairs(efficiencyROCPlots[Layer1], efficiencyROCPlots[Layer1Efficiency], moduleCoord, ladderCoord, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer1, Layer1Efficiency);
      }
      if(layer == 2)
      {
         rechitOccupancyROCPlots[Layer2] -> Fill(moduleCoord, ladderCoord);
         fillPairs(efficiencyROCPlots[Layer2], efficiencyROCPlots[Layer2Efficiency], moduleCoord, ladderCoord, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer2, Layer2Efficiency);
      }
      if(layer == 3)
      {
         rechitOccupancyROCPlots[Layer3] -> Fill(moduleCoord, ladderCoord);
         fillPairs(efficiencyROCPlots[Layer3], efficiencyROCPlots[Layer3Efficiency], moduleCoord, ladderCoord, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer3, Layer3Efficiency);
      }
      if(layer == 4)
      {
         rechitOccupancyROCPlots[Layer4] -> Fill(moduleCoord, ladderCoord);
         fillPairs(efficiencyROCPlots[Layer4], efficiencyROCPlots[Layer4Efficiency], moduleCoord, ladderCoord, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer4, Layer4Efficiency);
      }
   }
   if(det == 1)
   {
      rechitOccupancyROCPlots[AllDisks] -> Fill(diskRingCoord, bladePanelCoord);
      fillPairs(efficiencyROCPlots[AllDisks], efficiencyROCPlots[AllDisksEfficiency], diskRingCoord, bladePanelCoord, fillEfficiencyCondition, effCutAll);
      fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation - 1][AllDisks], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation - 1][AllDisksEfficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
      fillFullLayersDiskPlotsCollectionsAtDetectorPart(AllDisks, AllDisksEfficiency);
      if(0 <= glz) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksNegativeZ, DisksNegativeZEfficiency);
      if(glz < 0 ) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksPositiveZ, DisksPositiveZEfficiency);
      if(disk == -1)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk1], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk1Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk1, Disk1Efficiency);
      }
      if(disk == -2)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk2], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk2Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk2, Disk2Efficiency);
      }
      if(disk == -3)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk3], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk3Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk3, Disk3Efficiency);
      }
      if(disk == 1)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk4], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk4Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk4, Disk4Efficiency);
      }
      if(disk == 2)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk5], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk5Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk5, Disk5Efficiency);
      }
      if(disk == 3)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk6], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk6Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk6, Disk6Efficiency);
      }
   }
   incrementCounters();
}

void EfficiencyPlotsModule::downscaleEfficiencyPlots()
{
   std::vector<boost::variant<EfficiencyPlotPair, LayersDiskPlotsCollection>> efficiencyCollections;
   // Pairs
   efficiencyCollections.insert(efficiencyCollections.end(), 
   {
      layersDisksEfficiencyPlots,       vtxNtrkEfficiencyPreCutsPlots, vtxNtrkEfficiencyWithCutsPlots,
      ptEfficiencyPreCutsPlots,         ptEfficiencyWithCutsPlots,     striphitsEfficiencyPreCutsPlots,
      striphitsEfficiencyWithCutsPlots, clustDistWithCutsPlots,        hitDistWithCutsPlots
   });
   // Collections
   efficiencyCollections.insert(efficiencyCollections.end(), 
   {
      efficiencyROCPlots,        efficiencyPhiVsZPlots,     efficiencyGlyVsGlxPlots,
      lxEfficiencyWithCutsPlots, lyEfficiencyWithCutsPlots, lyVsLxEfficiencyWithCutsPlots,
      hitDistPreCuts,            d0WithCutsPlots,           dzPreCutsPlots,
      dzWithCutsPlots
   });
   // Multiple collections
   for(unsigned int orientationIndex = 0; orientationIndex < 8; ++orientationIndex)
   {
      efficiencyCollections.insert(efficiencyCollections.end(), forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex]);
   }
   for(auto& collection: efficiencyCollections)
   {
      if(EfficiencyPlotPair*        pair                   = boost::get<EfficiencyPlotPair>       (&collection)) downscaleCollectionIfNotEmpty(*pair);
      if(LayersDiskPlotsCollection* detectorPartCollection = boost::get<LayersDiskPlotsCollection>(&collection)) downscaleCollectionIfNotEmpty(*detectorPartCollection);
   };
}

void EfficiencyPlotsModule::addExtraEfficiencyPlots()
{
   // ROC efficiency distribution
   for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
   {
      const TH2D* detectorPartROCHits         = dynamic_cast<TH2D*>(efficiencyROCPlots[plotIndex]);
      const TH2D* detectorPartROCEfficiencies = dynamic_cast<TH2D*>(efficiencyROCPlots[plotIndex + 23]); 
      if(detectorPartROCHits == nullptr || detectorPartROCEfficiencies == nullptr) continue;
      unsigned int numRocs = detectorPartROCEfficiencies -> GetSize();
      for(unsigned int rocBin = 0; rocBin < numRocs; ++rocBin)
      {
         if((*detectorPartROCHits)[rocBin] == 0) continue;
         rocEfficiencyDistributionPlots[plotIndex] -> Fill((*detectorPartROCEfficiencies)[rocBin]);
      }
      std::cout << "rocEfficiencyDistributionPlots.size(): " << rocEfficiencyDistributionPlots[plotIndex] -> GetEntries() << std::endl;
   }
}

void EfficiencyPlotsModule::savePlots(const JSON& config, std::string saveDirectoryName)
{
   std::array<std::string, 7> layersDiskLabels
   {{
            "Lay 1",
            "Lay 2",
            "Lay 3",
            "Lay 4",
            "Disk 1",
            "Disk 2",
            "Disk 3"
   }};
   // Custom labels for layersDisksEfficiencyPlots
   for(auto& histogram: layersDisksEfficiencyPlots)
   {
      for(unsigned int numBin = 0; numBin < layersDiskLabels.size(); ++numBin)
      {
         histogram -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, layersDiskLabels[numBin].c_str());
      }
   }
   gStyle -> SetPalette(kVisibleSpectrum);
   gStyle -> SetNumberContours(999);
   gStyle -> SetOptStat(1112211); // Integral overflow Underflow RMS (value 2: +error) Mean (value 2: +error) Entries Title
   gErrorIgnoreLevel = kError;
   if(saveDirectoryName.back() == '/') saveDirectoryName.pop_back();
   if(saveDirectoryName != "") gDirectory -> mkdir(saveDirectoryName.c_str());
   saveHistogramsInCollectionIfNotEmpty(clusterOccupancyROCPlots,         saveDirectoryName.c_str(), "clusterOccupancyROCPlots",         config);
   saveHistogramsInCollectionIfNotEmpty(clusterPhiVsZPlots,               saveDirectoryName.c_str(), "clusterPhiVsZPlots",               config);
   saveHistogramsInCollectionIfNotEmpty(clusterGlyVsGlxPlots,             saveDirectoryName.c_str(), "clusterGlyVsGlxPlots",             config);
   saveHistogramsInCollectionIfNotEmpty(clusterZPlots,                    saveDirectoryName.c_str(), "clusterZPlots",                    config);
   saveHistogramsInCollectionIfNotEmpty(layersDisksEfficiencyPlots,       saveDirectoryName.c_str(), "layersDisksEfficiencyPlots",       config);
   saveHistogramsInCollectionIfNotEmpty(rechitOccupancyROCPlots,          saveDirectoryName.c_str(), "rechitOccupancyROCPlots",          config);
   saveHistogramsInCollectionIfNotEmpty(efficiencyROCPlots,               saveDirectoryName.c_str(), "efficiencyROCPlots",               config);
   saveHistogramsInCollectionIfNotEmpty(rechitOccupancyPhiVsZPlots,       saveDirectoryName.c_str(), "rechitOccupancyPhiVsZPlots",       config);
   saveHistogramsInCollectionIfNotEmpty(efficiencyPhiVsZPlots,            saveDirectoryName.c_str(), "efficiencyPhiVsZPlots",            config);
   saveHistogramsInCollectionIfNotEmpty(rechitOccupancyGlyVsGlxPlots,     saveDirectoryName.c_str(), "rechitOccupancyGlyVsGlxPlots",     config);
   saveHistogramsInCollectionIfNotEmpty(efficiencyGlyVsGlxPlots,          saveDirectoryName.c_str(), "efficiencyGlyVsGlxPlots",          config);
   saveHistogramsInCollectionIfNotEmpty(vtxNtrkEfficiencyPreCutsPlots,    saveDirectoryName.c_str(), "vtxNtrkEfficiencyPreCutsPlots",    config);
   saveHistogramsInCollectionIfNotEmpty(vtxNtrkEfficiencyWithCutsPlots,   saveDirectoryName.c_str(), "vtxNtrkEfficiencyWithCutsPlots",   config);
   saveHistogramsInCollectionIfNotEmpty(ptEfficiencyPreCutsPlots,         saveDirectoryName.c_str(), "ptEfficiencyPreCutsPlots",         config);
   saveHistogramsInCollectionIfNotEmpty(ptEfficiencyWithCutsPlots,        saveDirectoryName.c_str(), "ptEfficiencyWithCutsPlots",        config);
   saveHistogramsInCollectionIfNotEmpty(striphitsEfficiencyPreCutsPlots,  saveDirectoryName.c_str(), "striphitsEfficiencyPreCutsPlots",  config);
   saveHistogramsInCollectionIfNotEmpty(striphitsEfficiencyWithCutsPlots, saveDirectoryName.c_str(), "striphitsEfficiencyWithCutsPlots", config);
   saveHistogramsInCollectionIfNotEmpty(lxEfficiencyPreCutsPlots,         saveDirectoryName.c_str(), "lxEfficiencyPreCutsPlots",         config);
   saveHistogramsInCollectionIfNotEmpty(lxEfficiencyWithCutsPlots,        saveDirectoryName.c_str(), "lxEfficiencyWithCutsPlots",        config);
   saveHistogramsInCollectionIfNotEmpty(lyEfficiencyPreCutsPlots,         saveDirectoryName.c_str(), "lyEfficiencyPreCutsPlots",         config);
   saveHistogramsInCollectionIfNotEmpty(lyEfficiencyWithCutsPlots,        saveDirectoryName.c_str(), "lyEfficiencyWithCutsPlots",        config);
   saveHistogramsInCollectionIfNotEmpty(lyVsLxEfficiencyPreCutsPlots,     saveDirectoryName.c_str(), "lyVsLxEfficiencyPreCutsPlots",     config);
   saveHistogramsInCollectionIfNotEmpty(lyVsLxEfficiencyWithCutsPlots,    saveDirectoryName.c_str(), "lyVsLxEfficiencyWithCutsPlots",    config);
   saveHistogramsInCollectionIfNotEmpty(clustDistPreCutsPlots,            saveDirectoryName.c_str(), "clustDistPreCutsPlots",            config);
   saveHistogramsInCollectionIfNotEmpty(clustDistWithCutsPlots,           saveDirectoryName.c_str(), "clustDistWithCutsPlots",           config);
   saveHistogramsInCollectionIfNotEmpty(hitDistPreCuts,                   saveDirectoryName.c_str(), "hitDistPreCuts",                   config);
   saveHistogramsInCollectionIfNotEmpty(hitDistWithCutsPlots,             saveDirectoryName.c_str(), "hitDistWithCutsPlots",             config);
   saveHistogramsInCollectionIfNotEmpty(d0PreCutsPlots,                   saveDirectoryName.c_str(), "d0PreCutsPlots",                   config);
   saveHistogramsInCollectionIfNotEmpty(d0WithCutsPlots,                  saveDirectoryName.c_str(), "d0WithCutsPlots",                  config);
   saveHistogramsInCollectionIfNotEmpty(dzPreCutsPlots,                   saveDirectoryName.c_str(), "dzPreCutsPlots",                   config);
   saveHistogramsInCollectionIfNotEmpty(dzWithCutsPlots,                  saveDirectoryName.c_str(), "dzWithCutsPlots",                  config);
   saveHistogramsInCollectionIfNotEmpty(rocEfficiencyDistributionPlots,   saveDirectoryName.c_str(), "rocEfficiencyDistributionPlots",   config);
   for(unsigned int orientationIndex = 0; orientationIndex < 8; ++orientationIndex)
   {
      saveHistogramsInCollectionIfNotEmpty(forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex], saveDirectoryName.c_str(), "forwardLocalPositionsByOrientationEfficiencyPlots", config);
   }
}

float EfficiencyPlotsModule::getAvarageEfficiency()
{
   std::vector<std::pair<float, float>> efficienciesAndWeights =
   {
      { layersDisksEfficiencyPlots[1] -> GetBinContent(1), layersDisksEfficiencyPlots[0] -> GetBinContent(1) },
      { layersDisksEfficiencyPlots[1] -> GetBinContent(2), layersDisksEfficiencyPlots[0] -> GetBinContent(2) },
      { layersDisksEfficiencyPlots[1] -> GetBinContent(3), layersDisksEfficiencyPlots[0] -> GetBinContent(3) },
      { layersDisksEfficiencyPlots[1] -> GetBinContent(4), layersDisksEfficiencyPlots[0] -> GetBinContent(4) },
      { layersDisksEfficiencyPlots[1] -> GetBinContent(5), layersDisksEfficiencyPlots[0] -> GetBinContent(5) },
      { layersDisksEfficiencyPlots[1] -> GetBinContent(6), layersDisksEfficiencyPlots[0] -> GetBinContent(6) },
      { layersDisksEfficiencyPlots[1] -> GetBinContent(7), layersDisksEfficiencyPlots[0] -> GetBinContent(7) }
   };
   float effNumerator = 0;
   float effDenominator = 0;
   for(const auto& efficiencyWeightPair: efficienciesAndWeights)
   {
      effNumerator   += efficiencyWeightPair.first * efficiencyWeightPair.second;
      effDenominator += efficiencyWeightPair.second;
   }
   return effNumerator / effDenominator;
}

void EfficiencyPlotsModule::printCounters()
{
   std::cout << " --- Start cut counter values --- " << std::endl;
   std::cout << "nvtxCut:     " << std::setw(12) << nvtxCut_counter_     << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << nvtxCut_counter_     * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "zerobiasCut: " << std::setw(12) << zerobiasCut_counter_ << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << zerobiasCut_counter_ * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "federrCut:   " << std::setw(12) << federrCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << federrCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "hpCut:       " << std::setw(12) << hpCut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << hpCut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "ptCut:       " << std::setw(12) << ptCut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << ptCut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "nstripCut:   " << std::setw(12) << nstripCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << nstripCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "d0Cut:       " << std::setw(12) << d0Cut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << d0Cut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "dzCut:       " << std::setw(12) << dzCut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << dzCut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "pixhitCut:   " << std::setw(12) << pixhitCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << pixhitCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "lxFidCut:    " << std::setw(12) << lxFidCut_counter_    << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << lxFidCut_counter_    * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "lyFidCut:    " << std::setw(12) << lyFidCut_counter_    << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << lyFidCut_counter_    * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "valmisCut:   " << std::setw(12) << valmisCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << valmisCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "hitsepCut:   " << std::setw(12) << hitsepCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << hitsepCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << "total:       " << std::setw(12) << effCutAll_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << effCutAll_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
   std::cout << " --- End cut counter values --- " << std::endl;
}

void EfficiencyPlotsModule::printCutValues()
{
   std::cout << " --- Start cut values --- " << std::endl;
   std::cout << "nvtxCut:     " << nvtxCut     << std::endl;
   std::cout << "zerobiasCut: " << zerobiasCut << std::endl;
   std::cout << "federrCut:   " << federrCut   << std::endl;
   std::cout << "hpCut:       " << hpCut       << std::endl;
   std::cout << "ptCut:       " << ptCut       << std::endl;
   std::cout << "nstripCut:   " << nstripCut   << std::endl;
   std::cout << "d0Cut:       " << d0Cut       << std::endl;
   std::cout << "dzCut:       " << dzCut       << std::endl;
   std::cout << "pixhitCut:   " << pixhitCut   << std::endl;
   std::cout << "lxFidCut:    " << lxFidCut    << std::endl;
   std::cout << "lyFidCut:    " << lyFidCut    << std::endl;
   std::cout << "valmisCut:   " << valmisCut   << std::endl;
   std::cout << "hitsepCut:   " << hitsepCut   << std::endl;
   std::cout << " --- End cut values --- " << std::endl;
}

void EfficiencyPlotsModule::downscaleCollectionIfNotEmpty(EfficiencyPlotPair& plotPair)
{
   if(!histogramExistAndNotEmpty(plotPair[0]) || !histogramExistAndNotEmpty(plotPair[1])) return;
   std::array<TH2D*, 2> pairOf2DPointers = { dynamic_cast<TH2D*>(plotPair[0]), dynamic_cast<TH2D*>(plotPair[1]) };
   if(pairOf2DPointers[0] != nullptr && pairOf2DPointers[1] != nullptr)
   {
      downscale2DHistogram(pairOf2DPointers[1], pairOf2DPointers[0]);
   }
   else
   {
      downscale1DHistogram(dynamic_cast<TH1D*>(plotPair[1]), dynamic_cast<TH1D*>(plotPair[0]));
   }
}
void EfficiencyPlotsModule::downscaleCollectionIfNotEmpty(LayersDiskPlotsCollection& plotCollection)
{
   for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
   {
      EfficiencyPlotPair pairToDownscale = { plotCollection[plotIndex], plotCollection[plotIndex + 23] };
      downscaleCollectionIfNotEmpty(pairToDownscale);
   }
}

template <typename T>
void EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(const T& collection, const std::string& parentDirectoryName, const std::string& subdirectoryName, const JSON& config)
{
   if(std::any_of(collection.begin(), collection.end(), [&] (const auto& histogram) { return this -> histogramExistAndNotEmpty(histogram); }))
   {
      std::cout << "Saving plots of collection: " << subdirectoryName << std::endl;
      for(const auto& histogram: collection) saveHistogramInSubdirectory(histogram, parentDirectoryName, subdirectoryName, config);
   }
}

void EfficiencyPlotsModule::saveHistogramInSubdirectory(TH1* histogram, std::string parentDirectoryName, const std::string& subdirectoryName, const JSON& config)
{
   const char* originalDirectory(gDirectory -> GetPath());
   gDirectory -> cd("/");
   if(parentDirectoryName.back() == '/') parentDirectoryName.pop_back();
   const std::string directoryToSavePlots = (parentDirectoryName + "/" + subdirectoryName).c_str();
   gDirectory -> cd(parentDirectoryName.c_str());
   // if(gDirectory -> GetDirectory(subdirectoryName.c_str(), kFALSE) == nullptr) gDirectory -> mkdir(subdirectoryName.c_str());
   if(gDirectory -> GetDirectory(subdirectoryName.c_str(), kFALSE) == nullptr)
   {
      gDirectory -> mkdir(subdirectoryName.c_str());
   }
   // gDirectory -> cd(subdirectoryName.c_str());
   gDirectory -> cd(subdirectoryName.c_str());
   if(!histogramExistAndNotEmpty(histogram)) return;
   std::string histogramName      = histogram -> GetName();
   std::string originalHistoTitle = histogram -> GetTitle();
   std::string xAxisTitle         = histogram -> GetXaxis() -> GetTitle();
   std::string yAxisTitle         = histogram -> GetYaxis() -> GetTitle();
   // if(filterForRunNumberPresent)
   // {
   //    if(singleRun) histogram -> SetTitle(("RUN: " + std::to_string(runNumberLowerBound) + "_" + WBCSetting + ": " + originalHistoTitle).c_str());
   //    else          histogram -> SetTitle(("RUNS: [ " + std::to_string(runNumberLowerBound) + "-" + std::to_string(runNumberUpperBound - 1) + "]_" + WBCSetting + ": " + originalHistoTitle).c_str());
   // }
   TCanvas* canvas;
   int histoSizeX = 800;
   int histoSizeY = 800;
   canvas = custom_can_(histogram, histogramName + "_canvas", 0, 0, histoSizeX, histoSizeY, 80, 140);
   canvas -> cd();
   TH2D* histo2D = dynamic_cast<TH2D*>(histogram);
   TH1D* histo1D = dynamic_cast<TH1D*>(histogram);
   if(histo2D == nullptr && histo1D == nullptr)
   {
      std::cout << error_prompt << " error while typecasting for drawing." << std::endl;
      return;
   }
   if(histo2D != nullptr)
   {
      draw2DPlot(histo2D, canvas);
      dressIfROCPlot(histo2D);
   }
   else if(histo1D != nullptr)
   {
      draw1DPlot(histo1D, canvas);
   }
   canvas -> Update();
   gPad -> Update();
   if(config["save_histograms_to_ntuple"] == true)
   {
      histogram -> Write();
   }
   // static int print = 0;
   // if(print++ == 0) canvas -> SaveAs("CanvasTest.C");
   {
      std::string epsFilename = (std::string(canvas -> GetTitle()) + ".eps");
      std::transform(epsFilename.begin(), epsFilename.end(), epsFilename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
      std::string savePath = config["plots_save_directory"].get<std::string>() + "/" + epsFilename;
      canvas -> SaveAs(savePath.c_str());
      if(config["save_histograms_to_ntuple"] == true)
      {
         canvas -> Write();
      }
   }
   gDirectory -> cd("/");
   gDirectory -> cd(originalDirectory);
}

bool EfficiencyPlotsModule::histogramExistAndNotEmpty(TH1* histogram)
{
   if(!histogram) return false;
   if(histogram -> GetEntries() == 0) return false;
   return true;
}

void EfficiencyPlotsModule::draw1DPlot(TH1D* histogram, TCanvas* canvas)
{
   histogram -> SetFillColor(38);
   histogram -> Draw("HIST");
   // if(histogramName.find(EFFICIENCY_PLOT_IDENTIFIER) != std::string::npos)
   // {
   //    TGraphAsymmErrors* graph = getGraphForEfficiencyWithAsymmetricErrors(*dynamic_cast<TH1D*>(histogram), *dynamic_cast<TH1D*>(getEfficiencyNumeratorHisto(histograms, histogramName)));
   //    graph -> SetName((histogramName + "AsGraph").c_str());
   //    if(plotOptions.customTicks)
   //    {
   //       graph -> GetXaxis() -> SetNdivisions(plotOptions.xAxisDivisions);
   //       graph -> GetYaxis() -> SetNdivisions(plotOptions.yAxisDivisions);
   //    }
   //    if(plotOptions.customXAxisLabels)
   //    {
   //       for(unsigned int numBin = 0; numBin < plotOptions.xAxisLabels.size(); ++numBin)
   //       {
   //          graph -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, plotOptions.xAxisLabels[numBin].c_str());
   //       }
   //       if(std::string(graph -> GetName()).find("Efficiency") != std::string::npos)
   //       {
   //          graph -> GetYaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE_1D.first, EFFICIENCY_ZOOM_RANGE_1D.second);
   //       }
   //    }
   //    graph -> Draw("AP");
   //    // addLegend(histogram, graph);
   //    if(config["save_histograms_to_ntuple"] == true)
   //    {
   //       graph -> Write();
   //    }
   // }
}

void EfficiencyPlotsModule::draw2DPlot(TH2D* histogram, TCanvas* canvas)
{
   histogram -> Draw("COLZ");
}

void EfficiencyPlotsModule::dressIfROCPlot(TH2D* histogram)
{
   constexpr int PHASE_SCENARIO = 1;
   auto resultsForRechitOccupancyROCPlots = std::find(rechitOccupancyROCPlots.begin(), rechitOccupancyROCPlots.end(), histogram);
   if(resultsForRechitOccupancyROCPlots != rechitOccupancyROCPlots.end())
   {
      dress_occup_plot(histogram, plotIndexToLayerToDress(static_cast<LayersDiskPlotIndecies>(std::distance(rechitOccupancyROCPlots.begin(), resultsForRechitOccupancyROCPlots))), PHASE_SCENARIO);
      return;
   }
   auto resultsForEfficiencyROCPlots = std::find(efficiencyROCPlots.begin(), efficiencyROCPlots.end(), histogram);
   if(resultsForEfficiencyROCPlots != efficiencyROCPlots.end())
   {
      dress_occup_plot(histogram, plotIndexToLayerToDress(static_cast<LayersDiskPlotIndecies>(std::distance(efficiencyROCPlots.begin(), resultsForEfficiencyROCPlots))), PHASE_SCENARIO);
   }
   auto resultsForClusterOccupancyROCPlots = std::find(clusterOccupancyROCPlots.begin(), clusterOccupancyROCPlots.end(), histogram);
   if(resultsForClusterOccupancyROCPlots != clusterOccupancyROCPlots.end())
   {
      dress_occup_plot(histogram, plotIndexToLayerToDress(static_cast<LayersDiskPlotIndecies>(std::distance(clusterOccupancyROCPlots.begin(), resultsForClusterOccupancyROCPlots))), PHASE_SCENARIO);
   }
}

int EfficiencyPlotsModule::plotIndexToLayerToDress(LayersDiskPlotIndecies plotIndex)
{
   if(plotIndex == AllDisks)           return 0;
   if(plotIndex == AllDisksEfficiency) return 0;
   if(plotIndex == Layer1)             return 1;
   if(plotIndex == Layer1Efficiency)   return 1;
   if(plotIndex == Layer2)             return 2;
   if(plotIndex == Layer2Efficiency)   return 2;
   if(plotIndex == Layer3)             return 3;
   if(plotIndex == Layer3Efficiency)   return 3;
   if(plotIndex == Layer4)             return 4;
   if(plotIndex == Layer4Efficiency)   return 4;
   return -1;
}

void EfficiencyPlotsModule::calculateCuts()
{
   static const TrackData& trk             = trajField_.trk;
   static const int&       det             = trajField_.mod_on.det;
   static const int&       layer           = trajField_.mod_on.layer;  
   static const int&       disk            = trajField_.mod_on.disk;
   static const float&     lx              = trajField_.lx;
   static const float&     d_tr            = trajField_.d_tr;
   nvtxCut = VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL < trk.fromVtxNtrk;
   // Zerobias cut
   zerobiasCut = trajEventField_.trig & ZEROBIAS_BITMASK >> ZEROBIAS_TRIGGER_BIT;
   // Federr cut
   federrCut = trajEventField_.federrs_size == 0;
   // Hp cut
   hpCut = (trk.quality & TRACK_QUALITY_HIGH_PURITY_MASK) >> TRACK_QUALITY_HIGH_PURITY_BIT;
   // Pt cut
   ptCut = TRACK_PT_CUT_N_MINUS_1_VAL < trk.pt;
   // Nstrip cut
   nstripCut = TRACK_NSTRIP_CUT_N_MINUS_1_VAL < trk.strip;
   // D0 cut
   if(det == 0) d0Cut = std::abs(trk.d0) < TRACK_D0_CUT_BARREL_N_MINUS_1_VAL[layer - 1];
   if(det == 1) d0Cut = std::abs(trk.d0) < TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL;
   // Dz cut
   if(det == 0) dzCut = std::abs(trk.dz) < TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL;
   if(det == 1) dzCut = std::abs(trk.dz) < TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL;
   // Pixhit cut
   if(det == 0)
   {
      if(layer == 1) pixhitCut =
         (trk.validbpix[1] > 0 && trk.validbpix[2] > 0 && trk.validbpix[3] > 0) ||
         (trk.validbpix[1] > 0 && trk.validbpix[2] > 0 && trk.validfpix[0] > 0) ||
         (trk.validbpix[1] > 0 && trk.validfpix[0] > 0 && trk.validfpix[1] > 0) ||
         (trk.validfpix[0] > 0 && trk.validfpix[2] > 0 && trk.validfpix[2] > 0);
      if(layer == 2) pixhitCut =
         (trk.validbpix[0] > 0 && trk.validbpix[2] > 0 && trk.validbpix[3] > 0) ||
         (trk.validbpix[0] > 0 && trk.validbpix[2] > 0 && trk.validfpix[0] > 0) ||
         (trk.validbpix[0] > 0 && trk.validfpix[0] > 0 && trk.validfpix[1] > 0);
      if(layer == 3) pixhitCut =
         (trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validbpix[3] > 0) ||
         (trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validfpix[0] > 0);
      if(layer == 4) pixhitCut =
         (trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validbpix[2] > 0);
   }
   if(det == 1)
   {
      if(std::abs(disk) == 1) pixhitCut =
         (trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validbpix[2] > 0) ||
         (trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validfpix[1] > 0) ||
         (trk.validbpix[0] > 0 && trk.validfpix[1] > 0 && trk.validfpix[2] > 0);
      if(std::abs(disk) == 2) pixhitCut =
         (trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validfpix[0] > 0) ||
         (trk.validbpix[0] > 0 && trk.validfpix[0] > 0 && trk.validfpix[2] > 0);
      if(std::abs(disk) == 3) pixhitCut =
         (trk.validbpix[0] > 0 && trk.validfpix[0] > 0 && trk.validfpix[1] > 0);
   }
   // Fidicual cuts
   lxFidCut = 1;
   lyFidCut = 1;
   if(det == 0)
   {
      lxFidCut = std::abs(lx) < BARREL_MODULE_EDGE_X_CUT;
      lyFidCut = std::abs(lx) < BARREL_MODULE_EDGE_Y_CUT;
   }
   // Valmis cut
   valmisCut = trajField_.validhit || trajField_.missing;
   // Hitsep cut
   hitsepCut = MEAS_HITSEP_CUT_N_MINUS_1_VAL < d_tr;
   effCutAll      = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
   noVtxCut       =            zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
   // noHpCut        = nvtxCut && zerobiasCut && federrCut          && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
   noPtCut        = nvtxCut && zerobiasCut && federrCut && hpCut          && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
   noNStripCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut              && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
   noD0Cut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut          && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
   noDZCut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut          && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
   noFidicualsCut = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut                         && valmisCut && hitsepCut;
   noHitsepCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut             ;
}

void EfficiencyPlotsModule::incrementCounters()
{
   nvtxCut_counter_     += nvtxCut;
   zerobiasCut_counter_ += zerobiasCut;
   federrCut_counter_   += federrCut;
   hpCut_counter_       += hpCut;
   ptCut_counter_       += ptCut;
   nstripCut_counter_   += nstripCut;
   d0Cut_counter_       += d0Cut;
   dzCut_counter_       += dzCut;
   pixhitCut_counter_   += pixhitCut;
   lxFidCut_counter_    += lxFidCut;
   lyFidCut_counter_    += lyFidCut;
   valmisCut_counter_   += valmisCut;
   hitsepCut_counter_   += hitsepCut;
   effCutAll_counter_   += effCutAll;
   // missing_counter_     += missing;
   // clust_near_counter_  += clust_near;
   entry_counter_++;
}

void EfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const int& cuts)
{
   if(!cuts) return;
   numHitsHisto -> Fill(xFill);
   if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill);
}
void EfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
   for(const auto& cut: cuts) if(!cut) return;
   numHitsHisto -> Fill(xFill);
   if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill);
}
void EfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const int& cuts)
{
   if(!cuts) return;
   numHitsHisto -> Fill(xFill, yFill);
   if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill, yFill);
}
void EfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
   for(const auto& cut: cuts) if(!cut) return;
   numHitsHisto -> Fill(xFill, yFill);
   if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill, yFill);
}

void EfficiencyPlotsModule::filterBadRocs()
{
   // int max_module_bin_number = layer_total_hits_p -> GetXaxis() -> GetNbins();
   // int max_ladder_bin_number = layer_total_hits_p -> GetYaxis() -> GetNbins();
   // std::string layer_name = "layer_"; 
   // layer_name += std::to_string(layer_number_p);
   // // std::cerr << debug_prompt << "layer name: " << layer_name << std::endl;
   // // std::cerr << debug_prompt << "layer number: " << layer_number_p << std::endl;
   // std::string distribution_name("ROC_efficiency_distribution_simple_");
   // distribution_name += layer_name;
   // TH1D* roc_efficiency_distribution = new TH1D(distribution_name.c_str(), "ROC efficiency distribution, non-weighted;efficiency;num_rocs", 1000, 0.0, 1.0);
   // distribution_name += "_second";
   // TH1D* roc_efficiency_distribution_weighted = new TH1D(distribution_name.c_str(), "ROC efficiency distribution, weighted;efficiency;num_rocs", 1000, 0.0, 1.0);
   // for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
   // {
   //    for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
   //    {
   //       double current_ROC_efficiency = layer_efficiency_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
   //       double current_number_of_hits = layer_total_hits_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
   //       if(!(check_coordinate_pair(layer_efficiency_p, layer_number_p, module_ROC_bin, ladder_ROC_bin)))
   //       {
   //          continue;
   //       }
   //       if(current_ROC_efficiency == -1.0 || current_ROC_efficiency > 1.0 )
   //       {
   //          std::cerr << error_prompt << "Data processing error, found a pixel with -1.0 or bigger than 1.0 efficiency." << std::endl;
   //          continue;
   //       }
   //       if(current_ROC_efficiency != 0)
   //       {
   //          roc_efficiency_distribution -> Fill(current_ROC_efficiency - 0.0005);
   //          roc_efficiency_distribution_weighted -> Fill(current_ROC_efficiency - 0.0005, current_number_of_hits);
   //       }
   //    }
   // }

   // bad_ROC_eff_limit_p        = roc_efficiency_distribution          -> GetMean() - 0.02;
   // double bad_ROC_eff_limit_weighted = roc_efficiency_distribution_weighted -> GetMean() - 0.02;

   // for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
   // {
   //    for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
   //    {
   //       double current_ROC_efficiency = layer_efficiency_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
   //       if(!(check_coordinate_pair(layer_efficiency_p, layer_number_p, module_ROC_bin, ladder_ROC_bin)))
   //       {
   //          bad_ROCs_map_p        -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
   //          bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
   //          continue;
   //       }
   //       if(current_ROC_efficiency == -1.0 || current_ROC_efficiency > 1.0 )
   //       {
   //          bad_ROCs_map_p        -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
   //          bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
   //          std::cerr << error_prompt << "Data processing error, found a pixel with -1.0 or bigger than 1.0 efficiency." << std::endl;
   //          continue;
   //       }
   //       if(current_ROC_efficiency < bad_ROC_eff_limit_p)
   //       {
   //          bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
   //       }
   //       else
   //       {
   //          bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
   //       }
   //       if(current_ROC_efficiency < bad_ROC_eff_limit_weighted)
   //       {
   //          bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
   //       }
   //       else
   //       {
   //          bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
   //       }
   //    }
   // }
}
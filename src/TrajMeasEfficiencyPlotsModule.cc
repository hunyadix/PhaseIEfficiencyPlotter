#include "../interface/TrajMeasEfficiencyPlotsModule.h"
#include "../interface/common_functions_jkarancs.h"

constexpr int                  TrajMeasEfficiencyPlotsModule::ZEROBIAS_TRIGGER_BIT;
constexpr int                  TrajMeasEfficiencyPlotsModule::ZEROBIAS_BITMASK;
constexpr int                  TrajMeasEfficiencyPlotsModule::VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL;
constexpr int                  TrajMeasEfficiencyPlotsModule::TRACK_QUALITY_HIGH_PURITY_BIT;
constexpr int                  TrajMeasEfficiencyPlotsModule::TRACK_QUALITY_HIGH_PURITY_MASK;
constexpr float                TrajMeasEfficiencyPlotsModule::TRACK_PT_CUT_N_MINUS_1_VAL;
constexpr int                  TrajMeasEfficiencyPlotsModule::TRACK_NSTRIP_CUT_N_MINUS_1_VAL;
constexpr std::array<float, 4> TrajMeasEfficiencyPlotsModule::TRACK_D0_CUT_BARREL_N_MINUS_1_VAL;
constexpr float                TrajMeasEfficiencyPlotsModule::TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL;
constexpr float                TrajMeasEfficiencyPlotsModule::TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL;
constexpr float                TrajMeasEfficiencyPlotsModule::TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL;
constexpr float                TrajMeasEfficiencyPlotsModule::MEAS_HITSEP_CUT_N_MINUS_1_VAL;
constexpr float                TrajMeasEfficiencyPlotsModule::HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL;
constexpr float                TrajMeasEfficiencyPlotsModule::BARREL_MODULE_EDGE_X_CUT;
constexpr float                TrajMeasEfficiencyPlotsModule::BARREL_MODULE_EDGE_Y_CUT;

TrajMeasEfficiencyPlotsModule::TrajMeasEfficiencyPlotsModule(const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg): 
   eventField_      (eventFieldArg),
   trajField_       (trajFieldArg),
   det_             (trajField_.mod_on.det),
   layer_           (trajField_.mod_on.layer),
   flipped_         (trajField_.mod_on.flipped),
   module_          (trajField_.mod_on.module),
   ladder_          (trajField_.mod_on.ladder),
   side_            (trajField_.mod_on.side),
   disk_            (trajField_.mod_on.disk),
   blade_           (trajField_.mod_on.blade),
   panel_           (trajField_.mod_on.panel),
   ring_            (trajField_.mod_on.ring),
   missing_         (trajField_.missing),
   ladderCoord_     (trajField_.mod_on.ladder_coord),
   moduleCoord_     (trajField_.mod_on.module_coord),
   bladePanelCoord_ (trajField_.mod_on.blade_panel_coord),
   diskRingCoord_   (trajField_.mod_on.disk_ring_coord),
   lx_              (trajField_.lx),
   ly_              (trajField_.ly),
   lz_              (trajField_.lz),
   glx_             (trajField_.glx),
   gly_             (trajField_.gly),
   glz_             (trajField_.glz),
   norm_charge_     (trajField_.norm_charge),
   trk_             (trajField_.trk)
{
   defineHistograms();
}

void TrajMeasEfficiencyPlotsModule::defineHistograms()
{
   // Plot pairs
   layersDisksEfficiencyPlots[0]       = new TH1D("layersDisksEfficiencyPlots_0",       "num. hits. used to calculate efficiency, layers 1-4, disks 1-3 [with cuts]",                    7,   0.5,   7.5);
   layersDisksEfficiencyPlots[1]       = new TH1D("layersDisksEfficiencyPlots_1",       "efficiency, layers 1-4, disks 1-3 [with cuts];;efficiency",                                     7,   0.5,   7.5);
   vtxNtrkEfficiencyPreCutsPlots[0]    = new TH1D("vtxNtrkEfficiencyPreCutsPlots_0",    "parent vertex n. tracks [no cuts];track number",                                                200, 0.0, 200.0);
   // vtxNtrkEfficiencyPreCutsPlots[1]    = new TH1D("vtxNtrkEfficiencyPreCutsPlots_1",    "eff. vs parent vertex n. tracks [no cuts];track number;efficiency",                             200, 0.0, 200.0);
   vtxNtrkEfficiencyWithCutsPlots[0]   = new TH1D("vtxNtrkEfficiencyWithCutsPlots_0",   "parent vertex n. tracks [with other effcuts];parent vtx. tracks",                               200, 0.0, 200.0);
   vtxNtrkEfficiencyWithCutsPlots[1]   = new TH1D("vtxNtrkEfficiencyWithCutsPlots_1",   "eff. vs parent vertex n. tracks [with other effcuts];parent vtx. tracks",                       200, 0.0, 200.0);
   ptEfficiencyPreCutsPlots[0]         = new TH1D("ptEfficiencyPreCutsPlots_0",         "pt [no cuts];pt (GeV/c)",                                                                       30,  0.0, 10.0);
   // ptEfficiencyPreCutsPlots[1]         = new TH1D("ptEfficiencyPreCutsPlots_1",         "eff. vs pt [no cuts];pt (GeV/c);efficiency",                                                    30,  0.0, 10.0);
   ptEfficiencyWithCutsPlots[0]        = new TH1D("ptEfficiencyWithCutsPlots_0",        "pt [with other effcuts];pt (GeV/c)",                                                            30,  0.0, 10.0);
   ptEfficiencyWithCutsPlots[1]        = new TH1D("ptEfficiencyWithCutsPlots_1",        "eff. vs pt [with other effcuts];pt (GeV/c);efficiency",                                         30,  0.0, 10.0);
   striphitsEfficiencyPreCutsPlots[0]  = new TH1D("striphitsEfficiencyPreCutsPlots_0",  "strip hits [no cuts];strip hits",                                                               30,  0.0, 30.0);
   // striphitsEfficiencyPreCutsPlots[1]  = new TH1D("striphitsEfficiencyPreCutsPlots_1",  "eff. vs n. strip hits [no cuts];strip hits;efficiency",                                         30,  0.0, 30.0);
   striphitsEfficiencyWithCutsPlots[0] = new TH1D("striphitsEfficiencyWithCutsPlots_0", "strip hits [with other effcuts];strip hits",                                                    30,  0.0, 30.0);
   striphitsEfficiencyWithCutsPlots[1] = new TH1D("striphitsEfficiencyWithCutsPlots_1", "eff. vs n. strip hits [with other effcuts];strip hits;efficiency",                              30,  0.0, 30.0);
   clustDistWithCutsPlots[0]           = new TH1D("clustDistWithCutsPlots_0",           "closest cluster distance to traj. meas. [with other effcuts];distance (cm)",                    90,  0.0, 0.025);
   clustDistWithCutsPlots[1]           = new TH1D("clustDistWithCutsPlots_1",           "eff. vs closest cluster distance to traj. meas. [with other effcuts];distance (cm);efficiency", 90,  0.0, 0.025);
   hitDistWithCutsPlots[0]             = new TH1D("hitDistWithCutsPlots_0",             "closest hit distance to traj. meas. [with other effcuts];distance (cm)",                        100, 0.0, 0.12);
   hitDistWithCutsPlots[1]             = new TH1D("hitDistWithCutsPlots_1",             "eff. vs closest hit distance to traj. meas. [with other effcuts];distance (cm);efficiency",     100, 0.0, 0.12 );
   // Plot collections
   for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
   {
      rechitOccupancyPhiVsZPlots[plotIndex]         = new TH2D(("rechitOccupancyPhiVsZPlots_"    + plotNameSuffices[plotIndex] + "Hits").c_str(), ("rechit occupancy, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                       150, -52.15, 52.15, 300, -3.14159, 3.14159);
      efficiencyPhiVsZPlots[plotIndex]              = new TH2D(("efficiencyPhiVsZPlots_"         + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. hits used to calc. eff., phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                           150, -52.15, 52.15, 300, -3.14159, 3.14159);
      efficiencyPhiVsZPlots[plotIndex + 23]         = new TH2D(("efficiencyPhiVsZPlots_"         + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("efficiency, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                             150, -52.15, 52.15, 300, -3.14159, 3.14159);
      lxEfficiencyPreCutsPlots[plotIndex]           = new TH1D(("lxEfficiencyPreCutsPlots"       + plotNameSuffices[plotIndex] + "Hits").c_str(), ("eff. vs lx - " + plotTitleSuffices[plotIndex] + " [no cuts];lx (cm)").c_str(),                                           40,  -0.9, 0.9);
      lxEfficiencyWithCutsPlots[plotIndex]          = new TH1D(("lxEfficiencyWithCutsPlots"      + plotNameSuffices[plotIndex] + "Hits").c_str(), ("lx - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm)").c_str(),                                        40,  -0.9, 0.9);
      lxEfficiencyWithCutsPlots[plotIndex + 23]     = new TH1D(("lxEfficiencyWithCutsPlots_"     + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("eff. vs lx - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);efficiency").c_str(),                     40,  -0.9, 0.9);
      lyEfficiencyPreCutsPlots[plotIndex]           = new TH1D(("lyEfficiencyPreCutsPlots"       + plotNameSuffices[plotIndex] + "Hits").c_str(), ("eff. vs ly - " + plotTitleSuffices[plotIndex] + " [no cuts];ly (cm)").c_str(),                                           104, -3.5, 3.5);
      lyEfficiencyWithCutsPlots[plotIndex]          = new TH1D(("lyEfficiencyWithCutsPlots"      + plotNameSuffices[plotIndex] + "Hits").c_str(), ("ly - " + plotTitleSuffices[plotIndex] + " [with other effcuts];ly (cm)").c_str(),                                        104, -3.5, 3.5);
      lyEfficiencyWithCutsPlots[plotIndex + 23]     = new TH1D(("lyEfficiencyWithCutsPlots_"     + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("eff. vs ly - " + plotTitleSuffices[plotIndex] + " [with other effcuts];ly (cm);efficiency").c_str(),                     104, -3.5, 3.5);
      lyVsLxEfficiencyPreCutsPlots[plotIndex]       = new TH2D(("lyVsLxEfficiencyPreCutsPlots"   + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [no cuts];lx (cm);ly (cm)").c_str(),            40,  -0.9, 0.9, 104, -3.5, 3.5);
      lyVsLxEfficiencyWithCutsPlots[plotIndex]      = new TH2D(("lyVsLxEfficiencyWithCutsPlots"  + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(), 40,  -0.9, 0.9, 104, -3.5, 3.5);
      lyVsLxEfficiencyWithCutsPlots[plotIndex + 23] = new TH2D(("lyVsLxEfficiencyWithCutsPlots_" + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(), 40,  -0.9, 0.9, 104, -3.5, 3.5);
      clustDistPreCutsPlots[plotIndex]              = new TH1D(("clustDistPreCutsPlots_"         + plotNameSuffices[plotIndex] + "Hits").c_str(), ("closest cluster distance to traj. meas. - " + plotTitleSuffices[plotIndex] + " [no cuts];distance (cm)").c_str(),        90,   0.0, 0.025);
      hitDistPreCuts[plotIndex]                     = new TH1D(("hitDistPreCuts_"                + plotNameSuffices[plotIndex] + "Hits").c_str(), ("closest hit distance to traj. meas. - " + plotTitleSuffices[plotIndex] + " [no cuts];distance (cm)").c_str(),            100,  0.0, 0.12);
      d0PreCutsPlots[plotIndex]                     = new TH1D(("d0PreCutsPlots_"                + plotNameSuffices[plotIndex] + "Hits").c_str(), ("d0 - " + plotTitleSuffices[plotIndex] + " hits [no cuts];d0 (cm)").c_str(),                                              120,  0.0, 0.15);
      d0WithCutsPlots[plotIndex]                    = new TH1D(("d0WithCutsPlots_"               + plotNameSuffices[plotIndex] + "Hits").c_str(), ("d0 - " + plotTitleSuffices[plotIndex] + "[with other effcuts];d0 (cm)").c_str(),                                         120,  0.0, 0.15);
      d0WithCutsPlots[plotIndex + 23]               = new TH1D(("d0WithCutsPlots_"               + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("eff. vs d0 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];d0 (cm);efficiency").c_str(),                     120,  0.0, 0.15);
      dzPreCutsPlots[plotIndex]                     = new TH1D(("dzPreCutsPlots_"                + plotNameSuffices[plotIndex] + "Hits").c_str(), ("dz - " + plotTitleSuffices[plotIndex] + " [no cuts];dz (cm)").c_str(),                                                   100,  0.0, 0.5);
      dzWithCutsPlots[plotIndex]                    = new TH1D(("dzWithCutsPlots_"               + plotNameSuffices[plotIndex] + "Hits").c_str(), ("dz - " + plotTitleSuffices[plotIndex] + " [with other effcuts];dz (cm)").c_str(),                                        100,  0.0, 0.5);
      dzWithCutsPlots[plotIndex + 23]               = new TH1D(("dzWithCutsPlots_"               + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("eff. vs dz - " + plotTitleSuffices[plotIndex] + " [with other effcuts];dz (cm);efficiency").c_str(),                     100,  0.0, 0.5);

      if(plotIndex == AllDisks)
      {
         rechitOccupancyROCPlots[plotIndex] = new TH2D(("rechitOccupancyROCPlots_"       + plotNameSuffices[plotIndex] + "Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[plotIndex] + ";disk, ring;blade, panel").c_str(),                              112, -3.5, 3.5, 140, -17.5, 17.5);
         efficiencyROCPlots[plotIndex]      = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[plotIndex] + " [with effcuts];disk, ring;blade, panel").c_str(), 112, -3.5, 3.5, 140, -17.5, 17.5);
         efficiencyROCPlots[plotIndex + 23] = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("roc efficiency - " + plotTitleSuffices[plotIndex] + " [with effcuts];disk, ring;blade, panel;efficiency").c_str(),      112, -3.5, 3.5, 140, -17.5, 17.5);
      }
      if(plotIndex == Layer1)
      {
         rechitOccupancyROCPlots[plotIndex] = new TH2D(("rechitOccupancyROCPlots_"       + plotNameSuffices[plotIndex] + "Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[plotIndex] + ";module;ladders").c_str(),                                       72, -4.5, 4.5,  26,  -6.5,  6.5);
         efficiencyROCPlots[plotIndex]      = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders").c_str(),          72, -4.5, 4.5,  26,  -6.5,  6.5);
         efficiencyROCPlots[plotIndex + 23] = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("roc efficiency - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders;efficiency").c_str(),               72, -4.5, 4.5,  26,  -6.5,  6.5);
      }
      if(plotIndex == Layer2)
      {
         rechitOccupancyROCPlots[plotIndex] = new TH2D(("rechitOccupancyROCPlots_"       + plotNameSuffices[plotIndex] + "Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[plotIndex] + ";module;ladders").c_str(),                                       72, -4.5, 4.5,  58, -14.5, 14.5);
         efficiencyROCPlots[plotIndex]      = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders").c_str(),          72, -4.5, 4.5,  58, -14.5, 14.5);
         efficiencyROCPlots[plotIndex + 23] = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("roc efficiency - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders;efficiency").c_str(),               72, -4.5, 4.5,  58, -14.5, 14.5);
      }
      if(plotIndex == Layer3)
      {
         rechitOccupancyROCPlots[plotIndex] = new TH2D(("rechitOccupancyROCPlots_"       + plotNameSuffices[plotIndex] + "Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[plotIndex] + ";module;ladders").c_str(),                                       72, -4.5, 4.5,  90, -22.5, 22.5);
         efficiencyROCPlots[plotIndex]      = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders").c_str(),          72, -4.5, 4.5,  90, -22.5, 22.5);
         efficiencyROCPlots[plotIndex + 23] = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("roc efficiency - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders;efficiency").c_str(),               72, -4.5, 4.5,  90, -22.5, 22.5);
      }
      if(plotIndex == Layer4)
      {
         rechitOccupancyROCPlots[plotIndex] = new TH2D(("rechitOccupancyROCPlots_"       + plotNameSuffices[plotIndex] + "Hits").c_str(), ("rechit occupancy - " + plotTitleSuffices[plotIndex] + ";module;ladders").c_str(),                                       72, -4.5, 4.5, 130, -32.5, 32.5);
         efficiencyROCPlots[plotIndex]      = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Hits").c_str(), ("num. tracks used to calc. eff. - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders").c_str(),          72, -4.5, 4.5, 130, -32.5, 32.5);
         efficiencyROCPlots[plotIndex + 23] = new TH2D(("efficiencyROCPlots_"            + plotNameSuffices[plotIndex] + "Eff.").c_str(), ("roc efficiency - " + plotTitleSuffices[plotIndex] + " [with effcuts];module;ladders;efficiency").c_str(),               72, -4.5, 4.5, 130, -32.5, 32.5);
      }
   }
   for(unsigned int orientationIndex = 0; orientationIndex < 8; ++orientationIndex)
   {
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex] = std::array<TH1*, 46> {nullptr};
      std::string orientationIndexString = std::to_string(orientationIndex + 1);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][AllDisks]           = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_AllDisks_" + orientationIndexString).c_str(), ("num. of hits vs hit position - all disks, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(),  40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][AllDisksEfficiency] = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_AllDisksEfficiency_" + orientationIndexString).c_str(), ("eff. vs hit position - all disks" + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][Disk1]              = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk1_" + orientationIndexString).c_str(), ("num. of hits vs hit position - disk 1, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(),     40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][Disk1Efficiency]    = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk1Efficiency_" + orientationIndexString).c_str(), ("eff. vs hit position - disk 1, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(),  40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][Disk2]              = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk2_" + orientationIndexString).c_str(), ("num. of hits vs hit position - disk 2, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(),     40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][Disk2Efficiency]    = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk2Efficiency_" + orientationIndexString).c_str(), ("eff. vs hit position - disk 2, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(),  40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][Disk3]              = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk3_" + orientationIndexString).c_str(), ("num. of hits vs hit position - disk 3, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(),     40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][Disk3Efficiency]    = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk3Efficiency_" + orientationIndexString).c_str(), ("eff. vs hit position - disk 3, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(),  40, -0.9, 0.9, 104, -3.5, 3.5);
   }
   std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void TrajMeasEfficiencyPlotsModule::fillHistograms()
{
   // FIXME: this recalculation should be obsolete now
   d_cl_ = sqrt(trajField_.dx_cl * trajField_.dx_cl + trajField_.dy_cl * trajField_.dy_cl);
   d_tr_ = sqrt(trajField_.dx_tr * trajField_.dx_tr + trajField_.dy_tr * trajField_.dy_tr);
   phi_  = atan2(gly_, glx_);
   clust_near_              = (0 < d_cl_) && (d_cl_ < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL);
   fillEfficiencyCondition_ = !missing_ || clust_near_;
   absDisk_          = std::abs(disk_);
   panelOrientation_ = (side_ - 1) * 4 + std::abs(ring_ % 2) * 2 + panel_; // +Z, -Z, ring 1, ring 2, panel 1, panel 2
   layersDisks_ = det_ == 0 ? layer_ : 4 + absDisk_;
   calculateCuts();
   vtxNtrkEfficiencyPreCutsPlots[0]   -> Fill(trk_.fromVtxNtrk);
   ptEfficiencyPreCutsPlots[0]        -> Fill(trk_.pt);
   striphitsEfficiencyPreCutsPlots[0] -> Fill(trk_.strip);
   fillPairs(layersDisksEfficiencyPlots[0],       layersDisksEfficiencyPlots[1],       layersDisks_,     fillEfficiencyCondition_, effCutAll   );
   fillPairs(vtxNtrkEfficiencyWithCutsPlots[0],   vtxNtrkEfficiencyWithCutsPlots[1],   trk_.fromVtxNtrk, fillEfficiencyCondition_, noVtxCut    );
   fillPairs(ptEfficiencyWithCutsPlots[0],        ptEfficiencyWithCutsPlots[1],        trk_.pt,          fillEfficiencyCondition_, noPtCut     );
   fillPairs(striphitsEfficiencyWithCutsPlots[0], striphitsEfficiencyWithCutsPlots[1], trk_.strip,       fillEfficiencyCondition_, noNStripCut );
   fillPairs(clustDistWithCutsPlots[0],           clustDistWithCutsPlots[1],           d_cl_,            !missing_,               effCutAll   );
   fillPairs(hitDistWithCutsPlots[0],             hitDistWithCutsPlots[1],             d_tr_,            fillEfficiencyCondition_, noHitsepCut );
   fillFullLayersDiskPlotsCollectionsAtDetectorPart(LayersAndDisks, LayersAndDisksEfficiency);
   if(det_ == 0)
   {
      fillFullLayersDiskPlotsCollectionsAtDetectorPart(AllLayers, AllLayersEfficiency);
      if(layer_ == 1)
      {
         rechitOccupancyROCPlots[Layer1] -> Fill(moduleCoord_, ladderCoord_);
         fillPairs(efficiencyROCPlots[Layer1], efficiencyROCPlots[Layer1Efficiency], moduleCoord_, ladderCoord_, fillEfficiencyCondition_, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer1, Layer1Efficiency);
      }
      if(layer_ == 2)
      {
         rechitOccupancyROCPlots[Layer2] -> Fill(moduleCoord_, ladderCoord_);
         fillPairs(efficiencyROCPlots[Layer2], efficiencyROCPlots[Layer2Efficiency], moduleCoord_, ladderCoord_, fillEfficiencyCondition_, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer2, Layer2Efficiency);
      }
      if(layer_ == 3)
      {
         rechitOccupancyROCPlots[Layer3] -> Fill(moduleCoord_, ladderCoord_);
         fillPairs(efficiencyROCPlots[Layer3], efficiencyROCPlots[Layer3Efficiency], moduleCoord_, ladderCoord_, fillEfficiencyCondition_, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer3, Layer3Efficiency);
      }
      if(layer_ == 4)
      {
         rechitOccupancyROCPlots[Layer4] -> Fill(moduleCoord_, ladderCoord_);
         fillPairs(efficiencyROCPlots[Layer4], efficiencyROCPlots[Layer4Efficiency], moduleCoord_, ladderCoord_, fillEfficiencyCondition_, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Layer4, Layer4Efficiency);
      }
   }
   if(det_ == 1)
   {
      rechitOccupancyROCPlots[AllDisks] -> Fill(diskRingCoord_, bladePanelCoord_);
      fillPairs(efficiencyROCPlots[AllDisks], efficiencyROCPlots[AllDisks], diskRingCoord_, bladePanelCoord_, fillEfficiencyCondition_, effCutAll);
      fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_ - 1][AllDisks], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_ - 1][AllDisksEfficiency], lx_, ly_, fillEfficiencyCondition_, noFidicualsCut);
      fillFullLayersDiskPlotsCollectionsAtDetectorPart(AllDisks, AllDisksEfficiency);
      // if(disk_ == 1)
      // {
      //    fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk1], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk1Efficiency], lx_, ly_, fillEfficiencyCondition_, noFidicualsCut);
      // }
      // if(disk_ == 2)
      // {
      //    fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk2], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk2Efficiency], lx_, ly_, fillEfficiencyCondition_, noFidicualsCut);
      // }
      // if(disk_ == 3)
      // {
      //    fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk3], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk3Efficiency], lx_, ly_, fillEfficiencyCondition_, noFidicualsCut);
      // }
      // if(disk_ == 4)
      // {
      //    fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk4], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk4Efficiency], lx_, ly_, fillEfficiencyCondition_, noFidicualsCut);
      // }
      // if(disk_ == 5)
      // {
      //    fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk5], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk5Efficiency], lx_, ly_, fillEfficiencyCondition_, noFidicualsCut);
      // }
      // if(disk_ == 6)
      // {
      //    fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk6], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation_][Disk6Efficiency], lx_, ly_, fillEfficiencyCondition_, noFidicualsCut);
      // }
   }
   incrementCounters();
}

void TrajMeasEfficiencyPlotsModule::fillFullLayersDiskPlotsCollectionsAtDetectorPart(const LayersDiskPlotIndecies& index, const LayersDiskPlotIndecies& efficiencyIndex)
{
   rechitOccupancyPhiVsZPlots[index]   -> Fill(glz_, phi_);
   lxEfficiencyPreCutsPlots[index]     -> Fill(lx_);
   lyEfficiencyPreCutsPlots[index]     -> Fill(ly_);
   lyVsLxEfficiencyPreCutsPlots[index] -> Fill(lx_, ly_);
   clustDistPreCutsPlots[index]        -> Fill(d_cl_);
   hitDistPreCuts[index]               -> Fill(d_tr_);
   d0PreCutsPlots[index]               -> Fill(trk_.d0);
   dzPreCutsPlots[index]               -> Fill(trk_.dz);
   fillPairs(efficiencyPhiVsZPlots[index],         efficiencyPhiVsZPlots[efficiencyIndex],         glz_,    phi_, fillEfficiencyCondition_, effCutAll);
   fillPairs(lxEfficiencyWithCutsPlots[index],     lxEfficiencyWithCutsPlots[efficiencyIndex],     lx_,           fillEfficiencyCondition_, noFidicualsCut);
   fillPairs(lyEfficiencyWithCutsPlots[index],     lyEfficiencyWithCutsPlots[efficiencyIndex],     ly_,           fillEfficiencyCondition_, noFidicualsCut);
   fillPairs(lyVsLxEfficiencyWithCutsPlots[index], lyVsLxEfficiencyWithCutsPlots[efficiencyIndex], lx_,     ly_,  fillEfficiencyCondition_, noFidicualsCut);
   fillPairs(d0WithCutsPlots[index],               d0WithCutsPlots[efficiencyIndex],               trk_.d0,       fillEfficiencyCondition_, noD0Cut);
   fillPairs(dzWithCutsPlots[index],               dzWithCutsPlots[efficiencyIndex],               trk_.dz,       fillEfficiencyCondition_, noDZCut);
}

void TrajMeasEfficiencyPlotsModule::savePlots(const JSON& config)
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
   gStyle -> SetPalette(1);
   gStyle -> SetNumberContours(999);
   gStyle -> SetOptStat(1112211); // Integral overflow Underflow RMS (value 2: +error) Mean (value 2: +error) Entries Title
   gErrorIgnoreLevel = kError;
   saveHistogramsInCollectionIfNotEmpty(layersDisksEfficiencyPlots,       "layersDisksEfficiencyPlots",       config);
   saveHistogramsInCollectionIfNotEmpty(rechitOccupancyROCPlots,          "rechitOccupancyROCPlots",          config);
   saveHistogramsInCollectionIfNotEmpty(efficiencyROCPlots,               "efficiencyROCPlots",               config);
   saveHistogramsInCollectionIfNotEmpty(rechitOccupancyPhiVsZPlots,       "rechitOccupancyPhiVsZPlots",       config);
   saveHistogramsInCollectionIfNotEmpty(efficiencyPhiVsZPlots,            "efficiencyPhiVsZPlots",            config);
   saveHistogramsInCollectionIfNotEmpty(vtxNtrkEfficiencyPreCutsPlots,    "vtxNtrkEfficiencyPreCutsPlots",    config);
   saveHistogramsInCollectionIfNotEmpty(vtxNtrkEfficiencyWithCutsPlots,   "vtxNtrkEfficiencyWithCutsPlots",   config);
   saveHistogramsInCollectionIfNotEmpty(ptEfficiencyPreCutsPlots,         "ptEfficiencyPreCutsPlots",         config);
   saveHistogramsInCollectionIfNotEmpty(ptEfficiencyWithCutsPlots,        "ptEfficiencyWithCutsPlots",        config);
   saveHistogramsInCollectionIfNotEmpty(striphitsEfficiencyPreCutsPlots,  "striphitsEfficiencyPreCutsPlots",  config);
   saveHistogramsInCollectionIfNotEmpty(striphitsEfficiencyWithCutsPlots, "striphitsEfficiencyWithCutsPlots", config);
   saveHistogramsInCollectionIfNotEmpty(lxEfficiencyPreCutsPlots,         "lxEfficiencyPreCutsPlots",         config);
   saveHistogramsInCollectionIfNotEmpty(lxEfficiencyWithCutsPlots,        "lxEfficiencyWithCutsPlots",        config);
   saveHistogramsInCollectionIfNotEmpty(lyEfficiencyPreCutsPlots,         "lyEfficiencyPreCutsPlots",         config);
   saveHistogramsInCollectionIfNotEmpty(lyEfficiencyWithCutsPlots,        "lyEfficiencyWithCutsPlots",        config);
   saveHistogramsInCollectionIfNotEmpty(lyVsLxEfficiencyPreCutsPlots,     "lyVsLxEfficiencyPreCutsPlots",     config);
   saveHistogramsInCollectionIfNotEmpty(lyVsLxEfficiencyWithCutsPlots,    "lyVsLxEfficiencyWithCutsPlots",    config);
   saveHistogramsInCollectionIfNotEmpty(clustDistPreCutsPlots,            "clustDistPreCutsPlots",            config);
   saveHistogramsInCollectionIfNotEmpty(clustDistWithCutsPlots,           "clustDistWithCutsPlots",           config);
   saveHistogramsInCollectionIfNotEmpty(hitDistPreCuts,                   "hitDistPreCuts",                   config);
   saveHistogramsInCollectionIfNotEmpty(hitDistWithCutsPlots,             "hitDistWithCutsPlots",             config);
   saveHistogramsInCollectionIfNotEmpty(d0PreCutsPlots,                   "d0PreCutsPlots",                   config);
   saveHistogramsInCollectionIfNotEmpty(d0WithCutsPlots,                  "d0WithCutsPlots",                  config);
   saveHistogramsInCollectionIfNotEmpty(dzPreCutsPlots,                   "dzPreCutsPlots",                   config);
   saveHistogramsInCollectionIfNotEmpty(dzWithCutsPlots,                  "dzWithCutsPlots",                  config);
   for(unsigned int orientationIndex = 0; orientationIndex < 8; ++orientationIndex)
   {
      saveHistogramsInCollectionIfNotEmpty(forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex], "forwardLocalPositionsByOrientationEfficiencyPlots", config);
   }
}

void TrajMeasEfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(const LayersDiskPlotsCollection& collection, const std::string& subdirectoryName, const JSON& config)
{
   if(std::any_of(collection.begin(), collection.end(), [&] (const auto& histogram) { return this -> histogramExistAndNotEmpty(histogram); }))
   {
      std::cout << "Saving plots of collection: " << subdirectoryName << std::endl;
      for(const auto& histogram: collection) saveHistogramInSubdirectory(histogram, subdirectoryName.c_str(), config);
   }
}
void TrajMeasEfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(const EfficiencyPlotPair& collection, const std::string& subdirectoryName, const JSON& config)
{
   if(std::any_of(collection.begin(), collection.end(), [&] (const auto& histogram) { return this -> histogramExistAndNotEmpty(histogram); }))
   {
      std::cout << "Saving plots of collection: " << subdirectoryName << std::endl;
      for(const auto& histogram: collection) saveHistogramInSubdirectory(histogram, subdirectoryName.c_str(), config);
   }
}

void TrajMeasEfficiencyPlotsModule::saveHistogramInSubdirectory(TH1* histogram, const std::string& subdirectoryName, const JSON& config)
{
   gDirectory -> cd("/");
   if(gDirectory -> GetDirectory(subdirectoryName.c_str(), kFALSE) == nullptr) gDirectory -> mkdir(subdirectoryName.c_str());
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
}

bool TrajMeasEfficiencyPlotsModule::histogramExistAndNotEmpty(TH1* histogram)
{
   if(!histogram) return false;
   if(histogram -> GetEntries() == 0) return false;
   return true;
}

void TrajMeasEfficiencyPlotsModule::draw1DPlot(TH1D* histogram, TCanvas* canvas)
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

void TrajMeasEfficiencyPlotsModule::draw2DPlot(TH2D* histogram, TCanvas* canvas)
{
   histogram -> Draw("COLZ");
}

void TrajMeasEfficiencyPlotsModule::dressIfROCPlot(TH2D* histogram)
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
}

int TrajMeasEfficiencyPlotsModule::plotIndexToLayerToDress(LayersDiskPlotIndecies plotIndex)
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

void TrajMeasEfficiencyPlotsModule::printCounters()
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

void TrajMeasEfficiencyPlotsModule::printCutValues()
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

void TrajMeasEfficiencyPlotsModule::calculateCuts()
{
   const float d_tr_ = sqrt(trajField_.dx_tr * trajField_.dx_tr + trajField_.dy_tr * trajField_.dy_tr);
   nvtxCut = VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL < trk_.fromVtxNtrk;
   // Zerobias cut
   zerobiasCut = eventField_.trig & ZEROBIAS_BITMASK >> ZEROBIAS_TRIGGER_BIT;
   // Federr cut
   federrCut = eventField_.federrs_size == 0;
   // Hp cut
   hpCut = (trk_.quality & TRACK_QUALITY_HIGH_PURITY_MASK) >> TRACK_QUALITY_HIGH_PURITY_BIT;
   // Pt cut
   ptCut = TRACK_PT_CUT_N_MINUS_1_VAL < trk_.pt;
   // Nstrip cut
   nstripCut = TRACK_NSTRIP_CUT_N_MINUS_1_VAL < trk_.strip;
   // D0 cut
   if(det_ == 0) d0Cut = std::abs(trk_.d0) < TRACK_D0_CUT_BARREL_N_MINUS_1_VAL[layer_ - 1];
   if(det_ == 1) d0Cut = std::abs(trk_.d0) < TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL;
   // Dz cut
   if(det_ == 0) dzCut = std::abs(trk_.dz) < TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL;
   if(det_ == 1) dzCut = std::abs(trk_.dz) < TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL;
   // Pixhit cut
   if(det_ == 0)
   {
      if(layer_ == 1) pixhitCut =
         (trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0 && trk_.validbpix[3] > 0) ||
         (trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0 && trk_.validfpix[0] > 0) ||
         (trk_.validbpix[1] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[1] > 0) ||
         (trk_.validfpix[0] > 0 && trk_.validfpix[2] > 0 && trk_.validfpix[2] > 0);
      if(layer_ == 2) pixhitCut =
         (trk_.validbpix[0] > 0 && trk_.validbpix[2] > 0 && trk_.validbpix[3] > 0) ||
         (trk_.validbpix[0] > 0 && trk_.validbpix[2] > 0 && trk_.validfpix[0] > 0) ||
         (trk_.validbpix[0] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[1] > 0);
      if(layer_ == 3) pixhitCut =
         (trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validbpix[3] > 0) ||
         (trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validfpix[0] > 0);
      if(layer_ == 4) pixhitCut =
         (trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0);
   }
   if(det_ == 1)
   {
      if(std::abs(disk_) == 1) pixhitCut =
         (trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0) ||
         (trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validfpix[1] > 0) ||
         (trk_.validbpix[0] > 0 && trk_.validfpix[1] > 0 && trk_.validfpix[2] > 0);
      if(std::abs(disk_) == 2) pixhitCut =
         (trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validfpix[0] > 0) ||
         (trk_.validbpix[0] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[2] > 0);
      if(std::abs(disk_) == 3) pixhitCut =
         (trk_.validbpix[0] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[1] > 0);
   }
   // Fidicual cuts
   lxFidCut = 1;
   lyFidCut = 1;
   if(det_ == 0)
   {
      lxFidCut = std::abs(lx_) < BARREL_MODULE_EDGE_X_CUT;
      lyFidCut = std::abs(lx_) < BARREL_MODULE_EDGE_Y_CUT;
   }
   // Valmis cut
   valmisCut = trajField_.validhit || trajField_.missing;
   // Hitsep cut
   hitsepCut = MEAS_HITSEP_CUT_N_MINUS_1_VAL < d_tr_;
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

void TrajMeasEfficiencyPlotsModule::incrementCounters()
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
   // clust_near_counter_  += clust_near_;
   entry_counter_++;
}

void TrajMeasEfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition_, const int& cuts)
{
   if(!cuts) return;
   numHitsHisto -> Fill(xFill);
   if(fillEfficiencyCondition_) efficiencyHisto -> Fill(xFill);
}
void TrajMeasEfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition_, const std::initializer_list<int>& cuts)
{
   for(const auto& cut: cuts) if(!cut) return;
   numHitsHisto -> Fill(xFill);
   if(fillEfficiencyCondition_) efficiencyHisto -> Fill(xFill);
}
void TrajMeasEfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition_, const int& cuts)
{
   if(!cuts) return;
   numHitsHisto -> Fill(xFill, yFill);
   if(fillEfficiencyCondition_) efficiencyHisto -> Fill(xFill, yFill);
}
void TrajMeasEfficiencyPlotsModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition_, const std::initializer_list<int>& cuts)
{
   for(const auto& cut: cuts) if(!cut) return;
   numHitsHisto -> Fill(xFill, yFill);
   if(fillEfficiencyCondition_) efficiencyHisto -> Fill(xFill, yFill);
}

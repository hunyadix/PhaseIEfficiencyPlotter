#include "../interface/EfficiencyPlotsModule.h"

#include "../interface/common_functions_jkarancs.h"

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

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
EfficiencyPlotsModule::BadROClist EfficiencyPlotsModule::badROClist = 
{
   { 0, 2354 },
   { 0, 2355 },
   { 0, 2356 },
   { 0, 2357 },
   { 0, 2358 },
   { 0, 2534 },
   { 0, 8324 },
   { 0, 8325 },
   { 0, 8326 },
   { 0, 8327 },
   { 0, 8328 },
   { 0, 8329 },
   { 0, 8633 },
   { 0, 8634 },
   { 0, 8635 },
   { 0, 8636 },
   { 0, 8637 },
   { 0, 9300 },
   { 0, 9301 },
   { 0, 9302 },
   { 0, 9303 },
   { 0, 9304 },
   { 0, 9305 },
   { 0, 10963 },
   { 0, 10964 },
   { 0, 10965 },
   { 0, 10966 },
   { 0, 10967 },
   { 0, 11410 },
   { 0, 11411 },
   { 0, 11412 },
   { 0, 12418 },
   { 0, 12419 },
   { 0, 12420 },
   { 0, 12421 },
   { 0, 12422 },
   { 0, 12423 },
   { 0, 14790 },
   { 0, 14846 },
   { 0, 14847 },
   { 0, 14848 },
   { 0, 15802 },
   { 0, 15818 },
   { 0, 15834 },
   { 1, 104 },
   { 1, 106 },
   { 1, 176 },
   { 1, 180 },
   { 1, 194 },
   { 1, 195 },
   { 1, 227 },
   { 1, 251 },
   { 1, 278 },
   { 1, 337 },
   { 1, 338 },
   { 1, 339 },
   { 1, 340 },
   { 1, 400 },
   { 1, 442 },
   { 1, 461 },
   { 1, 462 },
   { 1, 463 },
   { 1, 464 },
   { 1, 653 },
   { 1, 654 },
   { 1, 655 },
   { 1, 656 },
   { 1, 676 },
   { 1, 727 },
   { 1, 728 },
   { 1, 729 },
   { 1, 730 },
   { 1, 782 },
   { 1, 858 },
   { 1, 867 },
   { 1, 876 },
   { 1, 1111 },
   { 1, 1112 },
   { 1, 1113 },
   { 1, 1114 },
   { 1, 1152 },
   { 1, 1154 },
   { 1, 1155 },
   { 1, 1156 },
   { 1, 1157 },
   { 1, 1158 },
   { 1, 1180 },
   { 1, 1253 },
   { 1, 1254 },
   { 1, 1255 },
   { 1, 1256 },
   { 1, 1283 },
   { 1, 1284 },
   { 1, 1285 },
   { 1, 1286 },
   { 1, 1299 },
   { 1, 1300 },
   { 1, 1301 },
   { 1, 1302 },
   { 1, 1373 },
   { 1, 1374 },
   { 1, 1375 },
   { 1, 1376 },
   { 1, 1378 },
   { 1, 1408 },
   { 1, 1482 },
   { 1, 1524 },
   { 1, 1558 },
   { 1, 1595 },
   { 1, 1596 },
   { 1, 1597 },
   { 1, 1615 },
   { 1, 1616 },
   { 1, 1617 },
   { 1, 1618 },
   { 1, 1625 },
   { 1, 1632 },
   { 1, 1669 },
   { 1, 1671 },
   { 1, 1676 },
   { 1, 1729 },
   { 1, 1732 },
   { 1, 1743 },
   { 1, 1744 },
   { 1, 1746 },
   { 1, 1747 },
   { 1, 1797 },
   { 1, 1798 },
   { 1, 1799 },
   { 1, 1800 },
   { 1, 1804 },
   { 1, 1806 },
   { 1, 1808 },
   { 1, 1817 },
   { 1, 1818 },
   { 1, 1819 },
   { 1, 1820 },
   { 1, 1824 },
   { 1, 1826 },
   { 1, 1891 },
   { 1, 1894 },
   { 1, 1955 },
   { 2, 166 },
   { 2, 167 },
   { 2, 168 },
   { 2, 169 },
   { 2, 170 },
   { 2, 171 },
   { 2, 172 },
   { 2, 831 },
   { 2, 832 },
   { 2, 833 },
   { 2, 834 },
   { 2, 835 },
   { 2, 836 },
   { 2, 837 },
   { 2, 838 },
   { 2, 1431 },
   { 2, 1432 },
   { 2, 1433 },
   { 2, 1434 },
   { 2, 1435 },
   { 2, 1436 },
   { 2, 1437 },
   { 2, 1438 },
   { 2, 1505 },
   { 2, 1506 },
   { 2, 1507 },
   { 2, 1508 },
   { 2, 1509 },
   { 2, 1510 },
   { 2, 1511 },
   { 2, 1512 },
   { 2, 1875 },
   { 2, 1876 },
   { 2, 1877 },
   { 2, 1878 },
   { 2, 1879 },
   { 2, 1880 },
   { 2, 1881 },
   { 2, 1882 },
   { 2, 1949 },
   { 2, 1950 },
   { 2, 1951 },
   { 2, 1952 },
   { 2, 1953 },
   { 2, 1954 },
   { 2, 1955 },
   { 2, 1956 },
   { 2, 2039 },
   { 2, 2040 },
   { 2, 2041 },
   { 2, 2042 },
   { 2, 2043 },
   { 2, 2044 },
   { 2, 2045 },
   { 2, 2046 },
   { 2, 2113 },
   { 2, 2114 },
   { 2, 2115 },
   { 2, 2116 },
   { 2, 2117 },
   { 2, 2118 },
   { 2, 2119 },
   { 2, 2120 },
   { 2, 2673 },
   { 2, 2674 },
   { 2, 2675 },
   { 2, 2676 },
   { 2, 2677 },
   { 2, 2678 },
   { 2, 2679 },
   { 2, 2680 },
   { 2, 2837 },
   { 2, 2838 },
   { 2, 2839 },
   { 2, 2840 },
   { 2, 2841 },
   { 2, 2842 },
   { 2, 2843 },
   { 2, 2844 },
   { 2, 3009 },
   { 2, 3010 },
   { 2, 3011 },
   { 2, 3012 },
   { 2, 3013 },
   { 2, 3014 },
   { 2, 3015 },
   { 2, 3016 },
   { 2, 3461 },
   { 2, 3462 },
   { 2, 3463 },
   { 2, 3464 },
   { 2, 3465 },
   { 2, 3466 },
   { 2, 3467 },
   { 2, 3468 },
   { 2, 3469 },
   { 2, 3470 },
   { 2, 3471 },
   { 2, 3472 },
   { 2, 3473 },
   { 2, 3474 },
   { 2, 3475 },
   { 2, 3476 },
   { 2, 3519 },
   { 2, 3520 },
   { 2, 3521 },
   { 2, 3522 },
   { 2, 3523 },
   { 2, 3524 },
   { 2, 3525 },
   { 2, 3526 },
   { 2, 3527 },
   { 2, 3528 },
   { 2, 3529 },
   { 2, 3530 },
   { 2, 3531 },
   { 2, 3532 },
   { 2, 3533 },
   { 2, 3534 },
   { 2, 4259 },
   { 2, 4260 },
   { 2, 4261 },
   { 2, 4262 },
   { 2, 4263 },
   { 2, 4264 },
   { 2, 4265 },
   { 2, 4266 },
   { 2, 4267 },
   { 2, 4268 },
   { 2, 4269 },
   { 2, 4270 },
   { 2, 4271 },
   { 2, 4272 },
   { 2, 4273 },
   { 2, 4274 },
   { 3, 595 },
   { 3, 596 },
   { 3, 597 },
   { 3, 598 },
   { 3, 599 },
   { 3, 600 },
   { 3, 4711 },
   { 3, 4712 },
   { 3, 4713 },
   { 3, 4714 },
   { 3, 4715 },
   { 3, 4716 },
   { 3, 4717 },
   { 3, 4718 },
   { 3, 4785 },
   { 3, 4786 },
   { 3, 4787 },
   { 3, 4788 },
   { 3, 4789 },
   { 3, 4790 },
   { 3, 4791 },
   { 3, 4792 },
   { 3, 6011 },
   { 3, 6012 },
   { 3, 6013 },
   { 3, 6014 },
   { 3, 6015 },
   { 3, 6016 },
   { 3, 6017 },
   { 3, 6018 },
   { 3, 6085 },
   { 3, 6086 },
   { 3, 6087 },
   { 3, 6088 },
   { 3, 6089 },
   { 3, 6090 },
   { 3, 6091 },
   { 3, 6092 },
   { 3, 6207 },
   { 3, 6208 },
   { 3, 6209 },
   { 3, 6210 },
   { 3, 6211 },
   { 3, 6212 },
   { 3, 6213 },
   { 3, 6214 },
   { 4, 123 },
   { 4, 124 },
   { 4, 125 },
   { 4, 126 },
   { 4, 127 },
   { 4, 128 },
   { 4, 129 },
   { 4, 130 },
   { 4, 197 },
   { 4, 198 },
   { 4, 199 },
   { 4, 200 },
   { 4, 201 },
   { 4, 202 },
   { 4, 203 },
   { 4, 204 },
   { 4, 2133 },
   { 4, 2269 },
   { 4, 2270 },
   { 4, 2271 },
   { 4, 2272 },
   { 4, 2273 },
   { 4, 2274 },
   { 4, 2275 },
   { 4, 2276 },
   { 4, 2681 },
   { 4, 2682 },
   { 4, 2683 },
   { 4, 2684 },
   { 4, 2685 },
   { 4, 2686 },
   { 4, 2687 },
   { 4, 2688 },
   { 4, 3080 },
   { 4, 3081 },
   { 4, 3082 },
   { 4, 3085 },
   { 4, 3089 },
   { 4, 3093 },
   { 4, 3094 },
   { 4, 3099 },
   { 4, 3105 },
   { 4, 3151 },
   { 4, 3162 },
   { 4, 3164 },
   { 4, 3168 },
   { 4, 3172 },
   { 4, 3233 },
   { 4, 3235 },
   { 4, 3236 },
   { 4, 3238 },
   { 4, 3239 },
   { 4, 3240 },
   { 4, 3241 },
   { 4, 3248 },
   { 4, 3310 },
   { 4, 3313 },
   { 4, 3314 },
   { 4, 3316 },
   { 4, 3317 },
   { 4, 3318 },
   { 4, 3319 },
   { 4, 3320 },
   { 4, 3321 },
   { 4, 3323 },
   { 4, 3325 },
   { 4, 3326 },
   { 4, 3328 },
   { 4, 3371 },
   { 4, 3372 },
   { 4, 3373 },
   { 4, 3375 },
   { 4, 3380 },
   { 4, 3383 },
   { 4, 3387 },
   { 4, 3390 },
   { 4, 3391 },
   { 4, 3392 },
   { 4, 3396 },
   { 4, 3400 },
   { 4, 3402 },
   { 4, 3446 },
   { 4, 3449 },
   { 4, 3450 },
   { 4, 3453 },
   { 4, 3457 },
   { 4, 3458 },
   { 4, 3459 },
   { 4, 3463 },
   { 4, 3465 },
   { 4, 3466 },
   { 4, 3472 },
   { 4, 3474 },
   { 4, 3475 },
   { 4, 3519 },
   { 4, 3521 },
   { 4, 3522 },
   { 4, 3525 },
   { 4, 3526 },
   { 4, 3527 },
   { 4, 3528 },
   { 4, 3529 },
   { 4, 3530 },
   { 4, 3535 },
   { 4, 3536 },
   { 4, 3537 },
   { 4, 3538 },
   { 4, 3540 },
   { 4, 3542 },
   { 4, 3545 },
   { 4, 3593 },
   { 4, 3595 },
   { 4, 3596 },
   { 4, 3597 },
   { 4, 3605 },
   { 4, 3609 },
   { 4, 3611 },
   { 4, 3612 },
   { 4, 3613 },
   { 4, 3615 },
   { 4, 3616 },
   { 4, 3619 },
   { 4, 5197 },
   { 4, 5198 },
   { 4, 5199 },
   { 4, 5200 },
   { 4, 5201 },
   { 4, 5202 },
   { 4, 5203 },
   { 4, 5204 },
   { 4, 5385 },
   { 4, 5386 },
   { 4, 5387 },
   { 4, 5388 },
   { 4, 5389 },
   { 4, 5390 },
   { 4, 5391 },
   { 4, 5392 },
   { 4, 5404 },
   { 4, 5405 },
   { 4, 5406 },
   { 4, 5407 },
   { 4, 5408 },
   { 4, 5409 },
   { 4, 5410 },
   { 4, 5700 },
   { 4, 5701 },
   { 4, 5702 },
   { 4, 5703 },
   { 4, 5704 },
   { 4, 5705 },
   { 4, 5706 },
   { 4, 6307 },
   { 4, 6308 },
   { 4, 6309 },
   { 4, 6310 },
   { 4, 6311 },
   { 4, 6312 },
   { 4, 6313 },
   { 4, 6314 },
   { 4, 6381 },
   { 4, 6382 },
   { 4, 6383 },
   { 4, 6384 },
   { 4, 6385 },
   { 4, 6386 },
   { 4, 6387 },
   { 4, 6388 },
   { 4, 6421 },
   { 4, 6422 },
   { 4, 6423 },
   { 4, 6424 },
   { 4, 6425 },
   { 4, 6426 },
   { 4, 6427 },
   { 4, 6428 },
   { 4, 7005 },
   { 4, 7006 },
   { 4, 7007 },
   { 4, 7008 },
   { 4, 7009 },
   { 4, 7010 },
   { 4, 7011 },
   { 4, 7012 },
   { 4, 7631 },
   { 4, 7632 },
   { 4, 7633 },
   { 4, 7634 },
   { 4, 7706 },
   { 4, 7707 },
   { 4, 7870 },
   { 4, 7872 },
   { 4, 8387 },
   { 4, 8388 },
   { 4, 8389 },
   { 4, 8390 },
   { 4, 8391 },
   { 4, 8392 },
   { 4, 8393 },
   { 4, 8394 },
   { 4, 9599 },
   { 4, 9602 }
};


template void EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty<std::vector<TH1D*>>(const std::vector<TH1D*>& collection, const std::string& parentDirectoryName, const std::string& subdirectoryName, const JSON& config);
template void EfficiencyPlotsModule::fillTrajMeasHistograms<EfficiencyPlotsModule::Collisions>();
template void EfficiencyPlotsModule::fillTrajMeasHistograms<EfficiencyPlotsModule::Cosmics>();

EfficiencyPlotsModule::EfficiencyPlotsModule(const EventData& clusterEventFieldArg, const Cluster& clusterFieldArg, const EventData& trajEventFieldArg, const TrajMeasurement& trajFieldArg, const float& delayInNsArg): 
   clusterEventField_     (clusterEventFieldArg),
   clusterField_          (clusterFieldArg),
   trajEventField_        (trajEventFieldArg),
   trajField_             (trajFieldArg),
   delayInNs_             (delayInNsArg)
{
   defineHistograms();
   // printCheckHistogramPointers();
}

void EfficiencyPlotsModule::setBadRocList(BadROClist&& badROCs)
{
   EfficiencyPlotsModule::badROClist = badROCs;
}

void EfficiencyPlotsModule::defineHistograms()
{
   std::cout << process_prompt << "Loading histogram definitions. " << std::endl;
   std::stringstream directoryNameStream;
   directoryNameStream << "Delay_" << std::fixed << std::setprecision(2) << delayInNs_;
   std::string directoryName = directoryNameStream.str();
   gDirectory -> cd("/");
   gDirectory -> mkdir(directoryName.c_str());
   gDirectory -> cd(directoryName.c_str());
   // Plot pairs
   layersDisksEfficiencyPlots[0]       = new TH1D("layersDisksEfficiencyPlots_Hits",       "num. hits. used to calculate efficiency, layers 1-4, disks 1-3 [with cuts]",                    7,   0.5,   7.5);
   layersDisksEfficiencyPlots[1]       = new TH1D("layersDisksEfficiencyPlots_Eff.",       "efficiency, layers 1-4, disks 1-3 [with cuts];;efficiency",                                     7,   0.5,   7.5);
   vtxNtrkEfficiencyPreCutsPlots[0]    = new TH1D("vtxNtrkEfficiencyPreCutsPlots_Hits",    "parent vertex n. tracks [no cuts];track number",                                                200, 0.0, 200.0);
   // vtxNtrkEfficiencyPreCutsPlots[1]    = new TH1D("vtxNtrkEfficiencyPreCutsPlots_Eff.",    "eff. vs parent vertex n. tracks [no cuts];track number;efficiency",                             200, 0.0, 200.0);
   vtxNtrkEfficiencyWithCutsPlots[0]   = new TH1D("vtxNtrkEfficiencyWithCutsPlots_Hits",   "parent vertex n. tracks [with other effcuts];parent vtx. tracks",                               200, 0.0, 200.0);
   vtxNtrkEfficiencyWithCutsPlots[1]   = new TH1D("vtxNtrkEfficiencyWithCutsPlots_Eff.",   "eff. vs parent vertex n. tracks [with other effcuts];parent vtx. tracks",                       200, 0.0, 200.0);
   ptEfficiencyPreCutsPlots[0]         = new TH1D("ptEfficiencyPreCutsPlots_Hits",         "pt [no cuts];pt (GeV/c)",                                                                       100, 0.0, 10.0);
   // ptEfficiencyPreCutsPlots[1]         = new TH1D("ptEfficiencyPreCutsPlots_Eff.",         "eff. vs pt [no cuts];pt (GeV/c);efficiency",                                                    100, 0.0, 10.0);
   ptEfficiencyWithCutsPlots[0]        = new TH1D("ptEfficiencyWithCutsPlots_Hits",        "pt [with other effcuts];pt (GeV/c)",                                                            100, 0.0, 10.0);
   ptEfficiencyWithCutsPlots[1]        = new TH1D("ptEfficiencyWithCutsPlots_Eff.",        "eff. vs pt [with other effcuts];pt (GeV/c);efficiency",                                         100, 0.0, 10.0);
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
      clusterPhiVsZPlots[plotIndex]                 = new TH2D(("clusterPhiVsZPlots_"            + plotNameSuffices[plotIndex]          ).c_str(), ("cluster occupancy, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                      150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      clusterGlyVsGlxPlots[plotIndex]               = new TH2D(("clusterGlyVsGlxPlots_"          + plotNameSuffices[plotIndex]          ).c_str(), ("cluster occupancy, Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),                  300, -16.2,  16.2,  300, -16.2,     16.2);
      clusterZPlots[plotIndex]                      = new TH1D(("clusterZPlots_"                 + plotNameSuffices[plotIndex]          ).c_str(), ("cluster Z distribution - " + plotTitleSuffices[plotIndex] + ";global z (cm)").c_str(),                                   150, -52.15, 52.15);
      rechitOccupancyPhiVsZPlots[plotIndex]         = new TH2D(("rechitOccupancyPhiVsZPlots_"    + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("rechit occupancy, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                       150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      efficiencyPhiVsZPlots[plotIndex]              = new TH2D(("efficiencyPhiVsZPlots_"         + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. hits used to calc. eff., phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                           150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      efficiencyPhiVsZPlots[plotIndex + 23]         = new TH2D(("efficiencyPhiVsZPlots_"         + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("efficiency, phi vs Z - " + plotTitleSuffices[plotIndex] + ";z;phi").c_str(),                                             150, -52.15, 52.15, 300,  -3.14159,  3.14159);
      rechitOccupancyGlyVsGlxPlots[plotIndex]       = new TH2D(("rechitOccupancyGlyVsGlxPlots_"  + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("rechit occupancy, Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),                   300, -16.2,  16.2,  300, -16.2,     16.2);
      efficiencyGlyVsGlxPlots[plotIndex]            = new TH2D(("efficiencyGlyVsGlxPlots_"       + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. hits used to calc. eff., Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),       300, -16.2,  16.2,  300, -16.2,     16.2);
      efficiencyGlyVsGlxPlots[plotIndex + 23]       = new TH2D(("efficiencyGlyVsGlxPlots_"       + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("efficiency, Y vs X - " + plotTitleSuffices[plotIndex] + ";global x (cm);global y (cm)").c_str(),                         300, -16.2,  16.2,  300, -16.2,     16.2);
      lxEfficiencyPreCutsPlots[plotIndex]           = new TH1D(("lxEfficiencyPreCutsPlots"       + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("lx - " + plotTitleSuffices[plotIndex] + " [no cuts];lx (cm)").c_str(),                                                   40,   -0.9,   0.9);
      lxEfficiencyWithCutsPlots[plotIndex]          = new TH1D(("lxEfficiencyWithCutsPlots"      + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("lx - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm)").c_str(),                                        40,   -0.9,   0.9);
      lxEfficiencyWithCutsPlots[plotIndex + 23]     = new TH1D(("lxEfficiencyWithCutsPlots_"     + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs lx - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);efficiency").c_str(),                     40,   -0.9,   0.9);
      lyEfficiencyPreCutsPlots[plotIndex]           = new TH1D(("lyEfficiencyPreCutsPlots"       + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("ly - " + plotTitleSuffices[plotIndex] + " [no cuts];ly (cm)").c_str(),                                                   104,  -3.5,   3.5);
      lyEfficiencyWithCutsPlots[plotIndex]          = new TH1D(("lyEfficiencyWithCutsPlots"      + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("ly - " + plotTitleSuffices[plotIndex] + " [with other effcuts];ly (cm)").c_str(),                                        104,  -3.5,   3.5);
      lyEfficiencyWithCutsPlots[plotIndex + 23]     = new TH1D(("lyEfficiencyWithCutsPlots_"     + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs ly - " + plotTitleSuffices[plotIndex] + " [with other effcuts];ly (cm);efficiency").c_str(),                     104,  -3.5,   3.5);
      lyVsLxEfficiencyPreCutsPlots[plotIndex]       = new TH2D(("lyVsLxEfficiencyPreCutsPlots"   + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [no cuts];lx (cm);ly (cm)").c_str(),            40,   -0.9,   0.9,  104,  -3.5,      3.5);
      lyVsLxEfficiencyWithCutsPlots[plotIndex]      = new TH2D(("lyVsLxEfficiencyWithCutsPlots"  + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(), 40,   -0.9,   0.9,  104,  -3.5,      3.5);
      lyVsLxEfficiencyWithCutsPlots[plotIndex + 23] = new TH2D(("lyVsLxEfficiencyWithCutsPlots_" + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("num. of hits vs hit position lay1 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(), 40,   -0.9,   0.9,  104,  -3.5,      3.5);
      clustDistPreCutsPlots[plotIndex]              = new TH1D(("clustDistPreCutsPlots_"         + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("closest cluster distance to traj. meas. - " + plotTitleSuffices[plotIndex] + " [no cuts];distance (cm)").c_str(),        200,   0.0,   0.025);
      hitDistPreCuts[plotIndex]                     = new TH1D(("hitDistPreCuts_"                + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("closest hit distance to traj. meas. - " + plotTitleSuffices[plotIndex] + " [no cuts];distance (cm)").c_str(),            200,   0.0,   0.12);
      d0PreCutsPlots[plotIndex]                     = new TH1D(("d0PreCutsPlots_"                + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("d0 - " + plotTitleSuffices[plotIndex] + " hits [no cuts];d0 (cm)").c_str(),                                              200,   0.0,   0.06);
      d0WithCutsPlots[plotIndex]                    = new TH1D(("d0WithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("d0 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];d0 (cm)").c_str(),                                        200,   0.0,   0.06);
      d0WithCutsPlots[plotIndex + 23]               = new TH1D(("d0WithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs d0 - " + plotTitleSuffices[plotIndex] + " [with other effcuts];d0 (cm);efficiency").c_str(),                     200,   0.0,   0.06);
      dzPreCutsPlots[plotIndex]                     = new TH1D(("dzPreCutsPlots_"                + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("dz - " + plotTitleSuffices[plotIndex] + " [no cuts];dz (cm)").c_str(),                                                   200,   0.0,   0.10);
      dzWithCutsPlots[plotIndex]                    = new TH1D(("dzWithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Hits").c_str(), ("dz - " + plotTitleSuffices[plotIndex] + " [with other effcuts];dz (cm)").c_str(),                                        200,   0.0,   0.10);
      dzWithCutsPlots[plotIndex + 23]               = new TH1D(("dzWithCutsPlots_"               + plotNameSuffices[plotIndex] + "_Eff.").c_str(), ("eff. vs dz - " + plotTitleSuffices[plotIndex] + " [with other effcuts];dz (cm);efficiency").c_str(),                     200,   0.0,   0.10);
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
      std::string orientationIndexString = std::to_string(orientationIndex + 1);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][AllDisks]              = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_AllDisks_"           + orientationIndexString).c_str(), ("num. of hits vs hit position - all disks, " + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)").c_str(),              40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][AllDisksEfficiency]    = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_AllDisksEfficiency_" + orientationIndexString).c_str(), ("eff. vs hit position - all disks, "          + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(),  40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsWithFidicualCutsEfficiencyPlots[orientationIndex][AllDisks]           = new TH2D(("forwardLocalPositionsWithFidicualCutsEfficiencyPlots_AllDisks_"           + orientationIndexString).c_str(), ("num. of hits vs hit position - all disks, " + orientationTextDefinitions[orientationIndex] + " [with all effcuts];lx (cm);ly (cm)").c_str(),             40, -0.9, 0.9, 104, -3.5, 3.5);
      forwardLocalPositionsWithFidicualCutsEfficiencyPlots[orientationIndex][AllDisksEfficiency] = new TH2D(("forwardLocalPositionsWithFidicualCutsEfficiencyPlots_AllDisksEfficiency_" + orientationIndexString).c_str(), ("eff. vs hit position - all disks, "          + orientationTextDefinitions[orientationIndex] + " [with all effcuts];lx (cm);ly (cm);efficiency").c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
      std::array<LayersDiskPlotIndecies, 6> disks = {Disk1, Disk2, Disk3, Disk4, Disk5, Disk6};
      for(unsigned int diskIndex = 0; diskIndex < disks.size(); ++diskIndex)
      {
         forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][disks[diskIndex]]         = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk" + std::to_string(diskIndex + 1) + "_"              + orientationIndexString).c_str(), ("num. of hits vs hit position - disk " + std::to_string(diskIndex + 1) + ", "    + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm)")           .c_str(),               40, -0.9, 0.9, 104, -3.5, 3.5);
         forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex][disks[diskIndex] + 23]    = new TH2D(("forwardLocalPositionsByOrientationEfficiencyPlots_Disk" + std::to_string(diskIndex + 1) + "Efficiency_"    + orientationIndexString).c_str(), ("eff. vs hit position - disk " + std::to_string(diskIndex + 1) + ", "            + orientationTextDefinitions[orientationIndex] + " [with other effcuts];lx (cm);ly (cm);efficiency").c_str(),               40, -0.9, 0.9, 104, -3.5, 3.5);
         forwardLocalPositionsWithFidicualCutsEfficiencyPlots[orientationIndex][disks[diskIndex]]      = new TH2D(("forwardLocalPositionsByOrientationWithFidicualCutsEfficiencyPlots_Disk" + std::to_string(diskIndex + 1) + "_"              + orientationIndexString).c_str(), ("num. of hits vs hit position - disk " + std::to_string(diskIndex + 1) + ", "    + orientationTextDefinitions[orientationIndex] + " [with all effcuts];lx (cm);ly (cm)")           .c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
         forwardLocalPositionsWithFidicualCutsEfficiencyPlots[orientationIndex][disks[diskIndex] + 23] = new TH2D(("forwardLocalPositionsByOrientationWithFidicualCutsEfficiencyPlots_Disk" + std::to_string(diskIndex + 1) + "Efficiency_"    + orientationIndexString).c_str(), ("eff. vs hit position - disk " + std::to_string(diskIndex + 1) + ", "            + orientationTextDefinitions[orientationIndex] + " [with all effcuts];lx (cm);ly (cm);efficiency").c_str(), 40, -0.9, 0.9, 104, -3.5, 3.5);
      }
   }
   std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void EfficiencyPlotsModule::fillClusterHistograms() try
{
   // const int&       sizeX           = clusterField_.sizeX;
   // const int&       sizeY           = clusterField_.sizeY;
   // const int&       sizePixels      = clusterField_.size;
   // const int&       charge          = clusterField_.charge;
   const int&       det             = clusterField_.mod_on.det;
   // const int&       module          = clusterField_.mod_on.module;
   const int&       layer           = clusterField_.mod_on.layer;
   // const int&       sec             = clusterField_.mod_on.sec;
   // const int&       ladder          = clusterField_.mod_on.ladder;
   const int&       disk            = clusterField_.mod_on.disk;
   // const int&       ring            = clusterField_.mod_on.ring;
   const float&     ladderCoord     = clusterField_.mod_on.ladder_coord;
   const float&     moduleCoord     = clusterField_.mod_on.module_coord;
   const float&     bladePanelCoord = clusterField_.mod_on.blade_panel_coord;
   const float&     diskRingCoord   = clusterField_.mod_on.disk_ring_coord;
   const float&     glx             = clusterField_.glx;
   const float&     gly             = clusterField_.gly;
   const float&     glz             = clusterField_.glz;
   const float phi  = atan2(gly, glx);
   if(det != 0 && det != 1)
   {
      std::cout << "Invalid detector part ID in dataset: " << det << ". Execution is continued with the next data point." << std::endl;
      return;
   }
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
      if(0 <= disk) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksNegativeZ);
      if(disk < 0 ) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksPositiveZ);
      if(disk == -1) fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk1);
      if(disk == -2) fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk2);
      if(disk == -3) fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk3);
      if(disk == 1)  fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk4);
      if(disk == 2)  fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk5);
      if(disk == 3)  fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk6);
   }
   // const int layersNegativePositive = [&] { if(det == 0) return (layer - 1) * 2 + (0 <= module); return NOVAL_I; }();
   // const int disksInnerOuter        = [&] { if(det == 1) return (0 <= disk) * 4 + (std::abs(disk) - 1) * 2 + ring - 1;; return NOVAL_I; }();
   // const int eBNPZHSSIOLP           = [&] { if(det == 0) return (0 <= module) * 32 + (0 <= ladder) * 16 + (sec - 1) * 2 + (2 < layer); return NOVAL_I; }();
   // // Denominator
   // clusterSizeXBpixFpix[det].first++;
   // clusterSizeYBpixFpix[det].first++;
   // clusterSizePixelsBpixFpix[det].first++;
   // clusterChargeBpixFpix[det].first++;
   // // Numerator
   // clusterSizeXBpixFpix[det].second += sizeX;
   // clusterSizeYBpixFpix[det].second += sizeY;
   // clusterSizePixelsBpixFpix[det].second += sizePixels;
   // clusterChargeBpixFpix[det].second += charge;

   // if(det == 0)
   // {
   //    // Denominator
   //    clusterSizeXLayersNegativePositive[layersNegativePositive]      .first++;
   //    clusterSizeYLayersNegativePositive[layersNegativePositive]      .first++;
   //    clusterSizePixelsLayersNegativePositive[layersNegativePositive] .first++;
   //    clusterChargeLayersNegativePositive[layersNegativePositive]     .first++;
   //    clusterSizeXBNPZHSSIOLP[eBNPZHSSIOLP]                           .first++; // Barrel negative and positive Z, half shell, sector, inner and outer layer pairs
   //    clusterSizeYBNPZHSSIOLP[eBNPZHSSIOLP]                           .first++;
   //    clusterSizePixelsBNPZHSSIOLP[eBNPZHSSIOLP]                      .first++;
   //    clusterChargeBNPZHSSIOLP[eBNPZHSSIOLP]                          .first++;
   //    // Numerator
   //    clusterSizeXLayersNegativePositive[layersNegativePositive]      .second += sizeX;
   //    clusterSizeYLayersNegativePositive[layersNegativePositive]      .second += sizeY;
   //    clusterSizePixelsLayersNegativePositive[layersNegativePositive] .second += sizePixels;
   //    clusterChargeLayersNegativePositive[layersNegativePositive]     .second += charge;
   //    clusterSizeXBNPZHSSIOLP[eBNPZHSSIOLP]                           .second += sizeX;
   //    clusterSizeYBNPZHSSIOLP[eBNPZHSSIOLP]                           .second += sizeY;
   //    clusterSizePixelsBNPZHSSIOLP[eBNPZHSSIOLP]                      .second += sizePixels;
   //    clusterChargeBNPZHSSIOLP[eBNPZHSSIOLP]                          .second += charge;
   // }
   // if(det == 1)
   // {
   //    // Denominator
   //    clusterSizeXDisksInnerOuter[disksInnerOuter]      .first++;
   //    clusterSizeYDisksInnerOuter[disksInnerOuter]      .first++;
   //    clusterSizePixelsDisksInnerOuter[disksInnerOuter] .first++;
   //    clusterChargeDisksInnerOuter[disksInnerOuter]     .first++;
   //    // Numerator
   //    clusterSizeXDisksInnerOuter[disksInnerOuter]      .second += sizeX;
   //    clusterSizeYDisksInnerOuter[disksInnerOuter]      .second += sizeY;
   //    clusterSizePixelsDisksInnerOuter[disksInnerOuter] .second += sizePixels;
   //    clusterChargeDisksInnerOuter[disksInnerOuter]     .second += charge;
   // }
}
catch(const std::exception& e)
{
   std::cout << e.what() << std::endl;
}

template <>
void EfficiencyPlotsModule::calculateCuts<EfficiencyPlotsModule::Scenario::Collisions>()
{
   const TrackData& trk             = trajField_.trk;
   const int&       det             = trajField_.mod_on.det;
   const int&       layer           = trajField_.mod_on.layer;  
   const int&       disk            = trajField_.mod_on.disk;
   const float&     ladderCoord     = trajField_.mod_on.ladder_coord;
   const float&     moduleCoord     = trajField_.mod_on.module_coord;
   const float&     bladePanelCoord = trajField_.mod_on.blade_panel_coord;
   const float&     diskRingCoord   = trajField_.mod_on.disk_ring_coord;
   const float&     lx              = trajField_.lx;
   const float&     ly              = trajField_.ly;
   const float&     d_tr            = trajField_.d_tr;
   const std::vector<TH2D*>  binMaps = 
   { 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[AllDisks]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer1]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer2]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer3]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer4]) 
   };
   const int layerMapKey = det ? 0 : layer;
   const float* binXKey  = det ? &diskRingCoord   : &moduleCoord;
   const float* binYKey  = det ? &bladePanelCoord : &ladderCoord;
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
   if(det == 0)
   {
      lxFidCut = std::abs(lx) < BARREL_MODULE_EDGE_X_CUT;
      lyFidCut = std::abs(ly) < BARREL_MODULE_EDGE_Y_CUT;
   }
   else if(det == 1)
   {
      lxFidCut = testForForwardFidicualCuts();
      lyFidCut = lxFidCut;
   }
   // Valmis cut
   valmisCut = trajField_.validhit || trajField_.missing;
   // Hitsep cut
   hitsepCut = MEAS_HITSEP_CUT_N_MINUS_1_VAL < d_tr;
   std::tuple<int, int> rocLocation                           = std::make_tuple(layerMapKey, binMaps[layerMapKey] -> FindFixBin(*binXKey, *binYKey));
   badROCCut = std::find(badROClist.begin(), badROClist.end(), rocLocation) == badROClist.end();
   effCutAll      = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noVtxCut       =            zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   // noHpCut        = nvtxCut && zerobiasCut && federrCut          && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noPtCut        = nvtxCut && zerobiasCut && federrCut && hpCut          && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noNStripCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut              && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noD0Cut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut          && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noDZCut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut          && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noFidicualsCut = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut                         && valmisCut && hitsepCut && badROCCut;
   noHitsepCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut              && badROCCut;
}
template <>
void EfficiencyPlotsModule::calculateCuts<EfficiencyPlotsModule::Scenario::Cosmics>()
{
   const TrackData& trk             = trajField_.trk;
   const int&       det             = trajField_.mod_on.det;
   const int&       layer           = trajField_.mod_on.layer;
   const float&     ladderCoord     = trajField_.mod_on.ladder_coord;
   const float&     moduleCoord     = trajField_.mod_on.module_coord;
   const float&     bladePanelCoord = trajField_.mod_on.blade_panel_coord;
   const float&     diskRingCoord   = trajField_.mod_on.disk_ring_coord;
   const float&     lx              = trajField_.lx;
   const float&     ly              = trajField_.ly;
   const float&     d_tr            = trajField_.d_tr;
   static const std::vector<TH2D*>  binMaps = 
   { 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[AllDisks]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer1]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer2]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer3]), 
      dynamic_cast<TH2D*>(rechitOccupancyROCPlots[Layer4]) 
   };
   const int layerMapKey = det ? 0 : layer;
   const float* binXKey  = det ? &diskRingCoord   : &moduleCoord;
   const float* binYKey  = det ? &bladePanelCoord : &ladderCoord;
   nvtxCut = 1;
   // Zerobias cut
   zerobiasCut = trajEventField_.trig & ZEROBIAS_BITMASK >> ZEROBIAS_TRIGGER_BIT;
   // zerobiasCut = true;
   // Federr cut
   federrCut = trajEventField_.federrs_size == 0;
   // Hp cut
   hpCut = 1;
   // Pt cut
   ptCut = TRACK_PT_CUT_N_MINUS_1_VAL < trk.pt;
   // Nstrip cut
   // nstripCut = TRACK_NSTRIP_CUT_N_MINUS_1_VAL < trk.strip;
   nstripCut = 1;
   // D0 cut
   d0Cut = 1;
   dzCut = 1;
   // Pixhit cut
   pixhitCut = 1;
   // Fidicual cuts
   if(det == 0)
   {
      lxFidCut = std::abs(lx) < BARREL_MODULE_EDGE_X_CUT;
      lyFidCut = std::abs(ly) < BARREL_MODULE_EDGE_Y_CUT;
   }
   else if(det == 1)
   {
      // lxFidCut = testForForwardFidicualCuts();
      lxFidCut = 1;
      lyFidCut = lxFidCut;
   }
   // Valmis cut
   valmisCut = trajField_.validhit || trajField_.missing;
   // Hitsep cut
   hitsepCut                             = MEAS_HITSEP_CUT_N_MINUS_1_VAL < d_tr;
   std::tuple<int, int> rocLocation = std::make_tuple(layerMapKey, binMaps[layerMapKey] -> FindFixBin(*binXKey, *binYKey));
   badROCCut                             = std::find(badROClist.begin(), badROClist.end(), rocLocation) == badROClist.end();
   effCutAll      = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noVtxCut       =            zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   // noHpCut        = nvtxCut && zerobiasCut && federrCut          && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noPtCut        = nvtxCut && zerobiasCut && federrCut && hpCut          && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noNStripCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut              && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noD0Cut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut          && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noDZCut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut          && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut && badROCCut;
   noFidicualsCut = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut                         && valmisCut && hitsepCut && badROCCut;
   noHitsepCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut              && badROCCut;
}

template <EfficiencyPlotsModule::Scenario scenario>
void EfficiencyPlotsModule::fillTrajMeasHistograms() try
{
   // Shortcuts
   const int&       det             = trajField_.mod_on.det;
   const int&       module          = trajField_.mod_on.module;
   const int&       layer           = trajField_.mod_on.layer;
   const int&       sec             = trajField_.mod_on.sec;
   const int&       ladder          = trajField_.mod_on.ladder;
   const int&       side            = trajField_.mod_on.side;
   const int&       disk            = trajField_.mod_on.disk;
   const int&       panel           = trajField_.mod_on.panel;
   const int&       ring            = trajField_.mod_on.ring;
   const int&       missing         = trajField_.missing;
   const float&     ladderCoord     = trajField_.mod_on.ladder_coord;
   const float&     moduleCoord     = trajField_.mod_on.module_coord;
   const float&     bladePanelCoord = trajField_.mod_on.blade_panel_coord;
   const float&     diskRingCoord   = trajField_.mod_on.disk_ring_coord;
   const float&     lx              = trajField_.lx;
   const float&     ly              = trajField_.ly;
   const float&     glx             = trajField_.glx;
   const float&     gly             = trajField_.gly;
   const float&     glz             = trajField_.glz;
   // const float&     norm_charge_     = trajField_.norm_charge;
   const float&     d_cl            = trajField_.d_cl;
   const float&     d_tr            = trajField_.d_tr;
   const TrackData& trk             = trajField_.trk;
   const int&       clusterX        = trajField_.clu.sizeX;
   const int&       clusterY        = trajField_.clu.sizeY;
   const int&       clusterSize     = trajField_.clu.size;
   const int&       clusterCharge   = trajField_.clu.charge;
   // FIXME: this recalculation should be obsolete now
   const float phi  = atan2(gly, glx);
   const int   clust_near              = (0 < d_cl) && (d_cl < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL);
   const int   fillEfficiencyCondition = !missing || clust_near;
   const int   absDisk          = std::abs(disk);
   const int   panelOrientation = [&] { if(det == 1) return ((side - 1) << 2) + ((ring - 1) << 1) + panel - 1; return NOVAL_I; }();;
   // if(det)
   // {
   //    std::cout << "ring: " << ring << std::endl;
   //    std::cout << "ring % 2: " << ring % 2 << std::endl;
   //    std::cout << "std::abs(ring % 2): " << std::abs(ring % 2) << std::endl;
   //    std::cin.get();
   // }
   if(det != 0 && det != 1) 
   {
      std::cout << "Invalid detector part ID in dataset: " << det << ". Execution is continued with the next data point." << std::endl;
   }
   if(det == 1  && !(0 <= panelOrientation && panelOrientation < 8))
   {
      std::cout << error_prompt << "Error reading some of the files..." << std::endl;
      std::cout << debug_prompt << "panelOrientation: " << std::endl;
      std::cout << debug_prompt << "side: " << side << std::endl;
      std::cout << debug_prompt << "ring: " << ring << std::endl;
      std::cout << debug_prompt << "panel: " << panel << std::endl;
      return;
   }
   const int   layersDisks = det == 0 ? layer : 4 + absDisk;
   calculateCuts<scenario>();
   // std::cout << "evt:    " << trajEventField_.evt << std::endl;
   // std::cout << "run:    " << trajEventField_.run << std::endl;
   // std::cout << "det:    " << det                 << std::endl;
   // std::cout << "side:   " << side                << std::endl;
   // if(det == 0)
   // {
   //    std::cout << "module: " << module << std::endl;
   //    std::cout << "layer:  " << layer  << std::endl;
   //    std::cout << "sec:    " << sec    << std::endl;
   //    std::cout << "ladder: " << ladder << std::endl;
   // }
   // if(det == 1)
   // {
   //    std::cout << "disk:             " << disk             << std::endl;
   //    std::cout << "side:             " << side             << std::endl;
   //    std::cout << "panel:            " << panel            << std::endl;
   // }
   // std::cin.get();
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
      fillPairs(efficiencyGlyVsGlxPlots[index],       efficiencyGlyVsGlxPlots[efficiencyIndex],       glx,    gly, fillEfficiencyCondition, effCutAll     );
      fillPairs(lxEfficiencyWithCutsPlots[index],     lxEfficiencyWithCutsPlots[efficiencyIndex],     lx,          fillEfficiencyCondition, noFidicualsCut);
      fillPairs(lyEfficiencyWithCutsPlots[index],     lyEfficiencyWithCutsPlots[efficiencyIndex],     ly,          fillEfficiencyCondition, noFidicualsCut);
      fillPairs(lyVsLxEfficiencyWithCutsPlots[index], lyVsLxEfficiencyWithCutsPlots[efficiencyIndex], lx,     ly,  fillEfficiencyCondition, noFidicualsCut);
      fillPairs(d0WithCutsPlots[index],               d0WithCutsPlots[efficiencyIndex],               trk.d0,      fillEfficiencyCondition, noD0Cut       );
      fillPairs(dzWithCutsPlots[index],               dzWithCutsPlots[efficiencyIndex],               trk.dz,      fillEfficiencyCondition, noDZCut       );
   };
   vtxNtrkEfficiencyPreCutsPlots[0]   -> Fill(trk.fromVtxNtrk);
   ptEfficiencyPreCutsPlots[0]        -> Fill(trk.pt);
   striphitsEfficiencyPreCutsPlots[0] -> Fill(trk.strip);
   fillPairs(layersDisksEfficiencyPlots[0],       layersDisksEfficiencyPlots[1],       layersDisks,     fillEfficiencyCondition, effCutAll  );
   fillPairs(vtxNtrkEfficiencyWithCutsPlots[0],   vtxNtrkEfficiencyWithCutsPlots[1],   trk.fromVtxNtrk, fillEfficiencyCondition, noVtxCut   );
   fillPairs(ptEfficiencyWithCutsPlots[0],        ptEfficiencyWithCutsPlots[1],        trk.pt,          fillEfficiencyCondition, noPtCut    );
   fillPairs(striphitsEfficiencyWithCutsPlots[0], striphitsEfficiencyWithCutsPlots[1], trk.strip,       fillEfficiencyCondition, noNStripCut);
   fillPairs(clustDistWithCutsPlots[0],           clustDistWithCutsPlots[1],           d_cl,            !missing,                effCutAll  );
   fillPairs(hitDistWithCutsPlots[0],             hitDistWithCutsPlots[1],             d_tr,            fillEfficiencyCondition, noHitsepCut);
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
      fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][AllDisks], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][AllDisksEfficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
      fillPairs(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][AllDisks], forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][AllDisksEfficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
      fillFullLayersDiskPlotsCollectionsAtDetectorPart(AllDisks, AllDisksEfficiency);
      if(0 <= disk) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksNegativeZ, DisksNegativeZEfficiency);
      if(disk < 0 ) fillFullLayersDiskPlotsCollectionsAtDetectorPart(DisksPositiveZ, DisksPositiveZEfficiency);
      if(disk == -1)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk1], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk1Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillPairs(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk1], forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk1Efficiency], lx, ly, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk1, Disk1Efficiency);
      }
      if(disk == -2)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk2], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk2Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillPairs(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk2], forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk2Efficiency], lx, ly, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk2, Disk2Efficiency);
      }
      if(disk == -3)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk3], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk3Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillPairs(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk3], forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk3Efficiency], lx, ly, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk3, Disk3Efficiency);
      }
      if(disk == 1)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk4], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk4Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillPairs(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk4], forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk4Efficiency], lx, ly, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk4, Disk4Efficiency);
      }
      if(disk == 2)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk5], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk5Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillPairs(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk5], forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk5Efficiency], lx, ly, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk5, Disk5Efficiency);
      }
      if(disk == 3)
      {
         fillPairs(forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk6], forwardLocalPositionsByOrientationEfficiencyPlots[panelOrientation][Disk6Efficiency], lx, ly, fillEfficiencyCondition, noFidicualsCut);
         fillPairs(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk6], forwardLocalPositionsWithFidicualCutsEfficiencyPlots[panelOrientation][Disk6Efficiency], lx, ly, fillEfficiencyCondition, effCutAll);
         fillFullLayersDiskPlotsCollectionsAtDetectorPart(Disk6, Disk6Efficiency);
      }
   }
   incrementCounters();
   // For efficiency calculations
   if(effCutAll)
   {
      // Layers, negative Z, positive Z
      const int layersNegativePositive = [&] { if(det == 0) return (layer - 1) * 2 + (0 <= module); return NOVAL_I; }();
      // Disk, inner ring, outer ring
      const int disksInnerOuter        = [&] { if(det == 1) return (0 <= disk) * 4 + (std::abs(disk) - 1) * 2 + ring - 1;; return NOVAL_I; }();
      // Barrel negative and positive Z, half shell, sector, inner and outer layer pairs
      const int eBNPZHSSIOLP           = [&] { if(det == 0) return (0 <= module) * 32 + (0 <= ladder) * 16 + (sec - 1) * 2 + (2 < layer); return NOVAL_I; }();
      // Cluster X size
      clusterSizeXBpixFpix[det].first++;
      clusterSizeXBpixFpix[det].second += clusterX;
      // Cluster Y size
      clusterSizeYBpixFpix[det].first++;
      clusterSizeYBpixFpix[det].second += clusterY;
      // Total size
      clusterSizePixelsBpixFpix[det].first++;
      clusterSizePixelsBpixFpix[det].second += clusterSize;
      // Charge
      clusterChargeBpixFpix[det].first++;
      clusterChargeBpixFpix[det].second += clusterCharge;
      // Efficiency
      efficiencyBpixFpix[det].first++;
      efficiencyBpixFpix[det].second += fillEfficiencyCondition;
      if(det == 0)
      {
         if(8 <= layersNegativePositive || layersNegativePositive < 0) throw std::runtime_error("error at " __FILE__ ":" S__LINE__);
         if(64 <= eBNPZHSSIOLP || eBNPZHSSIOLP < 0) throw std::runtime_error("error at " __FILE__ ":" S__LINE__);
         // Cluster X size
         clusterSizeXLayersNegativePositive[layersNegativePositive] .first++;
         clusterSizeXBNPZHSSIOLP[eBNPZHSSIOLP]                      .first++;
         clusterSizeXLayersNegativePositive[layersNegativePositive] .second += clusterX;
         clusterSizeXBNPZHSSIOLP[eBNPZHSSIOLP]                      .second += clusterX;
         // Cluster Y size
         clusterSizeYLayersNegativePositive[layersNegativePositive] .first++;
         clusterSizeYBNPZHSSIOLP[eBNPZHSSIOLP]                      .first++;
         clusterSizeYLayersNegativePositive[layersNegativePositive] .second += clusterY;
         clusterSizeYBNPZHSSIOLP[eBNPZHSSIOLP]                      .second += clusterY;
         // Total size
         clusterSizePixelsLayersNegativePositive[layersNegativePositive] .first++;
         clusterSizePixelsBNPZHSSIOLP[eBNPZHSSIOLP]                      .first++;
         clusterSizePixelsLayersNegativePositive[layersNegativePositive] .second += clusterSize;
         clusterSizePixelsBNPZHSSIOLP[eBNPZHSSIOLP]                      .second += clusterSize;
         // Charge
         clusterChargeLayersNegativePositive[layersNegativePositive] .first++;
         clusterChargeBNPZHSSIOLP[eBNPZHSSIOLP]                      .first++;
         clusterChargeLayersNegativePositive[layersNegativePositive] .second += clusterCharge;
         clusterChargeBNPZHSSIOLP[eBNPZHSSIOLP]                      .second += clusterCharge;
         // Efficiency
         efficiencyLayersNegativePositive[layersNegativePositive] .first++;
         efficiencyBNPZHSSIOLP[eBNPZHSSIOLP]                      .first++;
         efficiencyLayersNegativePositive[layersNegativePositive] .second += fillEfficiencyCondition;
         efficiencyBNPZHSSIOLP[eBNPZHSSIOLP]                      .second += fillEfficiencyCondition;
      }
      if(det == 1)
      {
         if(12 <= disksInnerOuter || disksInnerOuter < 0) throw std::runtime_error("error at " __FILE__ ":" S__LINE__);
         // Cluster X size
         clusterSizeXDisksInnerOuter[disksInnerOuter] .first++;
         clusterSizeXDisksInnerOuter[disksInnerOuter] .second += clusterX;
         // Cluster Y size
         clusterSizeYDisksInnerOuter[disksInnerOuter] .first++;
         clusterSizeYDisksInnerOuter[disksInnerOuter] .second += clusterY;
         // Total size
         clusterSizePixelsDisksInnerOuter[disksInnerOuter] .first++;
         clusterSizePixelsDisksInnerOuter[disksInnerOuter] .second += clusterSize;
         // Charge
         clusterChargeDisksInnerOuter[disksInnerOuter] .first++;
         clusterChargeDisksInnerOuter[disksInnerOuter] .second += clusterCharge;
         // Efficiency
         efficiencyDisksInnerOuter[disksInnerOuter].first++;
         efficiencyDisksInnerOuter[disksInnerOuter].second += fillEfficiencyCondition;
      }
   }
}
catch(std::exception& e)
{
   std::cout << e.what() << std::endl;
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
      efficiencyCollections.insert(efficiencyCollections.end(), forwardLocalPositionsWithFidicualCutsEfficiencyPlots[orientationIndex]);
   }
   for(auto& collection: efficiencyCollections)
   {
      if(EfficiencyPlotPair*        pair                   = boost::get<EfficiencyPlotPair>       (&collection)) downscaleCollectionIfNotEmpty(*pair);
      if(LayersDiskPlotsCollection* detectorPartCollection = boost::get<LayersDiskPlotsCollection>(&collection)) downscaleCollectionIfNotEmpty(*detectorPartCollection);
   };
}

void EfficiencyPlotsModule::printCombinedBadROCList(std::map<float, EfficiencyPlotsModule>& delayToPlotterModuleMap)
{
   double sum = 0;
   double weightSum = 0;
   for(const auto& delayScenarioPair: delayToPlotterModuleMap)
   {
      const EfficiencyPlotsModule& efficiencyPlotsModule = delayScenarioPair.second;
      for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
      {
         const TH2D* detectorPartROCHits         = dynamic_cast<TH2D*>(efficiencyPlotsModule.efficiencyROCPlots[plotIndex]);
         const TH2D* detectorPartROCEfficiencies = dynamic_cast<TH2D*>(efficiencyPlotsModule.efficiencyROCPlots[plotIndex + 23]); 
         if(detectorPartROCHits == nullptr || detectorPartROCEfficiencies == nullptr) continue;
         unsigned int numRocs = detectorPartROCEfficiencies -> GetSize();
         for(unsigned int rocBin = 0; rocBin < numRocs; ++rocBin)
         {
            if((*detectorPartROCHits)[rocBin] == 0) continue;
            sum       += (*detectorPartROCEfficiencies)[rocBin] * (*detectorPartROCHits)[rocBin];
            weightSum += (*detectorPartROCHits)[rocBin];
         }
      }
   }
   // ROC efficiency distribution
   double mean = sum / weightSum;
   double errorSquaredSum = 0;
   std::cout << sum << " " << weightSum << " " << sum / weightSum << std::endl;
   for(const auto& delayScenarioPair: delayToPlotterModuleMap)
   {
      const EfficiencyPlotsModule& efficiencyPlotsModule = delayScenarioPair.second;
      for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
      {
         const TH2D* detectorPartROCHits         = dynamic_cast<TH2D*>(efficiencyPlotsModule.efficiencyROCPlots[plotIndex]);
         const TH2D* detectorPartROCEfficiencies = dynamic_cast<TH2D*>(efficiencyPlotsModule.efficiencyROCPlots[plotIndex + 23]); 
         if(detectorPartROCHits == nullptr || detectorPartROCEfficiencies == nullptr) continue;
         unsigned int numRocs = detectorPartROCEfficiencies -> GetSize();
         for(unsigned int rocBin = 0; rocBin < numRocs; ++rocBin)
         {
            errorSquaredSum += std::pow(mean - (*detectorPartROCEfficiencies)[rocBin], 2) * (*detectorPartROCHits)[rocBin];
         }
      }
   }
   double statError = std::sqrt(errorSquaredSum / (weightSum - 1));
   std::cout << "mean: " << mean << ", statError: " << statError << std::endl;
   std::cout << "combined badROC list: " << std::endl;
   for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
   {
      TH2D* framePlot = dynamic_cast<TH2D*>(delayToPlotterModuleMap.begin() -> second.efficiencyROCPlots[plotIndex]);
      if(!framePlot) continue;
      int layer = NOVAL_I;
      if(plotIndex == AllDisks) layer = 0;
      if(plotIndex == Layer1)   layer = 1;
      if(plotIndex == Layer2)   layer = 2;
      if(plotIndex == Layer3)   layer = 3;
      if(plotIndex == Layer4)   layer = 4;
      unsigned int numRocs = framePlot -> GetSize();
      for(unsigned int rocBin = 0; rocBin < numRocs; ++rocBin)
      {
         std::vector<double> rocEfficiecnyAvg;
         for(const auto& delayScenarioPair: delayToPlotterModuleMap)
         {
            const EfficiencyPlotsModule& efficiencyPlotsModule = delayScenarioPair.second;
            const TH2D* detectorPartROCHits         = dynamic_cast<TH2D*>(efficiencyPlotsModule.efficiencyROCPlots[plotIndex]);
            const TH2D* detectorPartROCEfficiencies = dynamic_cast<TH2D*>(efficiencyPlotsModule.efficiencyROCPlots[plotIndex + 23]); 
            if(detectorPartROCHits == nullptr || detectorPartROCEfficiencies == nullptr) continue;
            if((*detectorPartROCHits)[rocBin] == 0) continue;
            std::vector<double> rocEfficiencyMeasurements(static_cast<int>((*detectorPartROCHits)[rocBin] + 0.5), (*detectorPartROCEfficiencies)[rocBin]);
            std::move(rocEfficiencyMeasurements.begin(), rocEfficiencyMeasurements.end(), std::back_inserter(rocEfficiecnyAvg));
         }
         if(rocEfficiecnyAvg.size() == 0) continue;
         // std::copy(rocEfficiecnyAvg.begin(), rocEfficiecnyAvg.end(), std::ostream_iterator<double>(std::cout, ", "));
         float efficiency = static_cast<double>(std::accumulate(rocEfficiecnyAvg.begin(), rocEfficiecnyAvg.end(), 0.0)) / rocEfficiecnyAvg.size();
         // std::cout << efficiency << std::endl;   
         // std::cout << "efficiency: " << efficiency << std::endl;
         if((efficiency < mean - 0.1 && efficiency < mean - statError * 2) || efficiency < 0.15)
         {
             std::cout << "{ " << layer << ", " << rocBin << " }" << std::endl;
         }

      }
   }
}

void EfficiencyPlotsModule::addExtraEfficiencyPlots()
{
   // double sum = 0;
   // double weightSum = 0;
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
         // sum       += (*detectorPartROCEfficiencies)[rocBin] * (*detectorPartROCHits)[rocBin];
         // weightSum += (*detectorPartROCHits)[rocBin];
      }
   }
   // double mean = sum / weightSum;
   // double errorSquaredSum = 0;
   // std::cout << sum << " " << weightSum << " " << sum / weightSum << std::endl;
   // for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
   // {
   //    const TH2D* detectorPartROCHits         = dynamic_cast<TH2D*>(efficiencyROCPlots[plotIndex]);
   //    const TH2D* detectorPartROCEfficiencies = dynamic_cast<TH2D*>(efficiencyROCPlots[plotIndex + 23]); 
   //    if(detectorPartROCHits == nullptr || detectorPartROCEfficiencies == nullptr) continue;
   //    unsigned int numRocs = detectorPartROCEfficiencies -> GetSize();
   //    for(unsigned int rocBin = 0; rocBin < numRocs; ++rocBin)
   //    {
   //       errorSquaredSum += std::pow(mean - (*detectorPartROCEfficiencies)[rocBin], 2) * (*detectorPartROCHits)[rocBin];
   //    }
   // }
   // double statError = std::sqrt(errorSquaredSum / (weightSum - 1));
   // std::cout << "mean: " << mean << ", statError: " << statError << std::endl;
   // std::cout << "badROC list: " << std::endl;
   // for(LayersDiskPlotIndecies plotIndex = static_cast<LayersDiskPlotIndecies>(0); plotIndex < 23; plotIndex = static_cast<LayersDiskPlotIndecies>(plotIndex + 1))
   // {
   //    const TH2D* detectorPartROCHits         = dynamic_cast<TH2D*>(efficiencyROCPlots[plotIndex]);
   //    const TH2D* detectorPartROCEfficiencies = dynamic_cast<TH2D*>(efficiencyROCPlots[plotIndex + 23]); 
   //    if(detectorPartROCHits == nullptr || detectorPartROCEfficiencies == nullptr) continue;
   //    int layer = NOVAL_I;
   //    if(plotIndex == AllDisks) layer = 0;
   //    if(plotIndex == Layer1)   layer = 1;
   //    if(plotIndex == Layer2)   layer = 2;
   //    if(plotIndex == Layer3)   layer = 3;
   //    if(plotIndex == Layer4)   layer = 4;
   //    unsigned int numRocs = detectorPartROCEfficiencies -> GetSize();
   //    for(unsigned int rocBin = 0; rocBin < numRocs; ++rocBin)
   //    {
   //       if((*detectorPartROCHits)[rocBin] == 0) continue;
   //       float efficiency = (*detectorPartROCEfficiencies)[rocBin];
   //       // std::cout << efficiency << std::endl;   
   //       if((efficiency < mean - 0.1 && efficiency < mean - statError * 2) || efficiency < 0.15)
   //       {
   //           std::cout << "{ " << (layer == 0) << ", " << layer << ", " << rocBin << " }" << std::endl;
   //       }

   //    }
   // }
}

void EfficiencyPlotsModule::savePlots(const JSON& config, std::string mainDirectoryName)
{
   // printCheckHistogramPointers();
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
   gDirectory -> cd("/");
   if(mainDirectoryName.back() == '/') mainDirectoryName.pop_back();
   std::stringstream delayAsStringStream;
   delayAsStringStream << "Delay_" << std::fixed << std::setprecision(2) << delayInNs_;
   std::string saveDirectoryName = mainDirectoryName + "/" + delayAsStringStream.str();
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
      saveHistogramsInCollectionIfNotEmpty(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[orientationIndex], saveDirectoryName.c_str(), "forwardLocalPositionsWithFidicualCutsEfficiencyPlots", config);
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

void EfficiencyPlotsModule::printCheckHistogramPointers()
{
   auto printCollectionInfo = [&] (auto& collection)
   {
      for(const auto& histogram: collection)
      {
         if(histogram)
         {
            std::cout << "Address: " << histogram << ", entries: " << histogram -> GetEntries() << ", title: " << histogram -> GetTitle() << std::endl;
         }
      }
   };
   std::cout << "layersDisksEfficiencyPlots: " <<       std::endl; printCollectionInfo(layersDisksEfficiencyPlots);
   std::cout << "rechitOccupancyROCPlots: " <<          std::endl; printCollectionInfo(rechitOccupancyROCPlots);
   std::cout << "efficiencyROCPlots: " <<               std::endl; printCollectionInfo(efficiencyROCPlots);
   std::cout << "rechitOccupancyPhiVsZPlots: " <<       std::endl; printCollectionInfo(rechitOccupancyPhiVsZPlots);
   std::cout << "efficiencyPhiVsZPlots: " <<            std::endl; printCollectionInfo(efficiencyPhiVsZPlots);
   std::cout << "rechitOccupancyGlyVsGlxPlots: " <<     std::endl; printCollectionInfo(rechitOccupancyGlyVsGlxPlots);
   std::cout << "efficiencyGlyVsGlxPlots: " <<          std::endl; printCollectionInfo(efficiencyGlyVsGlxPlots);
   std::cout << "vtxNtrkEfficiencyPreCutsPlots: " <<    std::endl; printCollectionInfo(vtxNtrkEfficiencyPreCutsPlots);
   std::cout << "vtxNtrkEfficiencyWithCutsPlots: " <<   std::endl; printCollectionInfo(vtxNtrkEfficiencyWithCutsPlots);
   std::cout << "ptEfficiencyPreCutsPlots: " <<         std::endl; printCollectionInfo(ptEfficiencyPreCutsPlots);
   std::cout << "ptEfficiencyWithCutsPlots: " <<        std::endl; printCollectionInfo(ptEfficiencyWithCutsPlots);
   std::cout << "striphitsEfficiencyPreCutsPlots: " <<  std::endl; printCollectionInfo(striphitsEfficiencyPreCutsPlots);
   std::cout << "striphitsEfficiencyWithCutsPlots: " << std::endl; printCollectionInfo(striphitsEfficiencyWithCutsPlots);
   std::cout << "lxEfficiencyPreCutsPlots: " <<         std::endl; printCollectionInfo(lxEfficiencyPreCutsPlots);
   std::cout << "lxEfficiencyWithCutsPlots: " <<        std::endl; printCollectionInfo(lxEfficiencyWithCutsPlots);
   std::cout << "lyEfficiencyPreCutsPlots: " <<         std::endl; printCollectionInfo(lyEfficiencyPreCutsPlots);
   std::cout << "lyEfficiencyWithCutsPlots: " <<        std::endl; printCollectionInfo(lyEfficiencyWithCutsPlots);
   std::cout << "lyVsLxEfficiencyPreCutsPlots: " <<     std::endl; printCollectionInfo(lyVsLxEfficiencyPreCutsPlots);
   std::cout << "lyVsLxEfficiencyWithCutsPlots: " <<    std::endl; printCollectionInfo(lyVsLxEfficiencyWithCutsPlots);
   std::cout << "clustDistPreCutsPlots: " <<            std::endl; printCollectionInfo(clustDistPreCutsPlots);
   std::cout << "clustDistWithCutsPlots: " <<           std::endl; printCollectionInfo(clustDistWithCutsPlots);
   std::cout << "hitDistPreCuts: " <<                   std::endl; printCollectionInfo(hitDistPreCuts);
   std::cout << "hitDistWithCutsPlots: " <<             std::endl; printCollectionInfo(hitDistWithCutsPlots);
   std::cout << "d0PreCutsPlots: " <<                   std::endl; printCollectionInfo(d0PreCutsPlots);
   std::cout << "d0WithCutsPlots: " <<                  std::endl; printCollectionInfo(d0WithCutsPlots);
   std::cout << "dzPreCutsPlots: " <<                   std::endl; printCollectionInfo(dzPreCutsPlots);
   std::cout << "dzWithCutsPlots: " <<                  std::endl; printCollectionInfo(dzWithCutsPlots);
   std::cout << "rocEfficiencyDistributionPlots: " <<   std::endl; printCollectionInfo(rocEfficiencyDistributionPlots);
   std::cout << "forwardLocalPositionsByOrientationEfficiencyPlots"    << std::endl; for(unsigned int orientationIndex = 0; orientationIndex < 8; ++orientationIndex) printCollectionInfo(forwardLocalPositionsByOrientationEfficiencyPlots[orientationIndex]);
   std::cout << "forwardLocalPositionsWithFidicualCutsEfficiencyPlots" << std::endl; for(unsigned int orientationIndex = 0; orientationIndex < 8; ++orientationIndex) printCollectionInfo(forwardLocalPositionsWithFidicualCutsEfficiencyPlots[orientationIndex]);
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
   std::cout << "badROCCut:   " << std::setw(12) << badROCCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << badROCCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
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
   std::cout << "badROCCut:   " << badROCCut   << std::endl;
   std::cout << " --- End cut values --- " << std::endl;
}

float EfficiencyPlotsModule::getDelayValueNs()
{
   return delayInNs_;
}

std::array<std::pair<Long64_t, Long64_t>, 2>*  EfficiencyPlotsModule::getClusterSizeXBpixFpix()                    { return &clusterSizeXBpixFpix;                    }
std::array<std::pair<Long64_t, Long64_t>, 8>*  EfficiencyPlotsModule::getClusterSizeXLayersNegativePositive()      { return &clusterSizeXLayersNegativePositive;      }
std::array<std::pair<Long64_t, Long64_t>, 12>* EfficiencyPlotsModule::getClusterSizeXDisksInnerOuter()             { return &clusterSizeXDisksInnerOuter;             }
std::array<std::pair<Long64_t, Long64_t>, 64>* EfficiencyPlotsModule::getClusterSizeXBNPZHSSIOLP()                 { return &clusterSizeXBNPZHSSIOLP;                 }
std::array<std::pair<Long64_t, Long64_t>, 2>*  EfficiencyPlotsModule::getClusterSizeYBpixFpix()                    { return &clusterSizeYBpixFpix;                    }
std::array<std::pair<Long64_t, Long64_t>, 8>*  EfficiencyPlotsModule::getClusterSizeYLayersNegativePositive()      { return &clusterSizeYLayersNegativePositive;      }
std::array<std::pair<Long64_t, Long64_t>, 12>* EfficiencyPlotsModule::getClusterSizeYDisksInnerOuter()             { return &clusterSizeYDisksInnerOuter;             }
std::array<std::pair<Long64_t, Long64_t>, 2>*  EfficiencyPlotsModule::getClusterSizePixelsBpixFpix()               { return &clusterSizePixelsBpixFpix;               }
std::array<std::pair<Long64_t, Long64_t>, 8>*  EfficiencyPlotsModule::getClusterSizePixelsLayersNegativePositive() { return &clusterSizePixelsLayersNegativePositive; }
std::array<std::pair<Long64_t, Long64_t>, 12>* EfficiencyPlotsModule::getClusterSizePixelsDisksInnerOuter()        { return &clusterSizePixelsDisksInnerOuter;        }
std::array<std::pair<Long64_t, Long64_t>, 64>* EfficiencyPlotsModule::getClusterSizePixelsBNPZHSSIOLP()            { return &clusterSizePixelsBNPZHSSIOLP;            }
std::array<std::pair<Long64_t, Long64_t>, 2>*  EfficiencyPlotsModule::getClusterChargeBpixFpix()                   { return &clusterChargeBpixFpix;                   }
std::array<std::pair<Long64_t, Long64_t>, 8>*  EfficiencyPlotsModule::getClusterChargeLayersNegativePositive()     { return &clusterChargeLayersNegativePositive;     }
std::array<std::pair<Long64_t, Long64_t>, 12>* EfficiencyPlotsModule::getClusterChargeDisksInnerOuter()            { return &clusterChargeDisksInnerOuter;            }
std::array<std::pair<Long64_t, Long64_t>, 64>* EfficiencyPlotsModule::getClusterChargeBNPZHSSIOLP()                { return &clusterChargeBNPZHSSIOLP;                }
std::array<std::pair<Long64_t, Long64_t>, 64>* EfficiencyPlotsModule::getClusterSizeYBNPZHSSIOLP()                 { return &clusterSizeYBNPZHSSIOLP;                 }
std::array<std::pair<Long64_t, Long64_t>, 2>*  EfficiencyPlotsModule::getEfficiencyBpixFpix()                      { return &efficiencyBpixFpix;                      }
std::array<std::pair<Long64_t, Long64_t>, 8>*  EfficiencyPlotsModule::getEfficiencyLayersNegativePositive()        { return &efficiencyLayersNegativePositive;        }
std::array<std::pair<Long64_t, Long64_t>, 12>* EfficiencyPlotsModule::getEfficiencyDisksInnerOuter()               { return &efficiencyDisksInnerOuter;               }
std::array<std::pair<Long64_t, Long64_t>, 64>* EfficiencyPlotsModule::getEfficiencyBNPZHSSIOLP()                   { return &efficiencyBNPZHSSIOLP;                   }

TGraphAsymmErrors* EfficiencyPlotsModule::getEfficiencyGraphAsymmErrors(const TH1D& efficiencyHistogram, const TH1D& numHitsHistogram, const int& markerColor, const int& markerStyle)
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
   int numZeroBins = 0;
   for(int bin = 0; bin < numBins; ++bin)
   {
      if(numHitsHistogram.GetBinContent(bin + 1) == 0)
      {
         numZeroBins++;
         continue;
      }
      valuesX.push_back(xAxis -> GetBinCenter(bin + 1));
      valuesY.push_back(efficiencyHistogram.GetBinContent(bin + 1));
      double lowerBound, upperBound;
      std::tie(lowerBound, upperBound) = WilsonScoreIntervalErrorCalculator(numHitsHistogram.GetBinContent(bin + 1), valuesY[bin], 1.0).getError();
      errorsYLow .emplace_back(std::move(valuesY[bin] - lowerBound  ));
      errorsYHigh.emplace_back(std::move(upperBound   - valuesY[bin]));
   }
   TGraphAsymmErrors* graph = new TGraphAsymmErrors(numBins - numZeroBins, valuesX.data(), valuesY.data(), errorsXLow.data(), errorsXHigh.data(), errorsYLow.data(), errorsYHigh.data());
   graph -> SetTitle(efficiencyHistogram.GetTitle());
   graph -> GetXaxis() -> SetTitle (xAxis -> GetTitle());
   graph -> GetYaxis() -> SetTitle (yAxis -> GetTitle());
   graph -> GetXaxis() -> SetTitleSize(xAxis -> GetTitleSize());
   graph -> GetYaxis() -> SetTitleSize(yAxis -> GetTitleSize());
   graph -> GetXaxis() -> SetTitleOffset(xAxis -> GetTitleOffset());
   graph -> GetYaxis() -> SetTitleOffset(yAxis -> GetTitleOffset());
   graph -> GetXaxis() -> SetTitleFont(xAxis -> GetTitleFont());
   graph -> GetYaxis() -> SetTitleFont(yAxis -> GetTitleFont());
   graph -> GetXaxis() -> SetRangeUser (xAxis -> GetXmin(), xAxis -> GetXmax());
   // graph -> GetYaxis() -> SetRangeUser (yAxis -> GetXmin(), yAxis -> GetXmax());
   graph -> GetXaxis() -> SetNdivisions(xAxis -> GetNdivisions());
   graph -> GetYaxis() -> SetNdivisions(yAxis -> GetNdivisions());
   graph -> GetXaxis() -> SetLabelOffset(xAxis -> GetLabelOffset());
   graph -> GetYaxis() -> SetLabelOffset(yAxis -> GetLabelOffset());
   graph -> SetMarkerColor(markerColor);
   graph -> SetFillColor(markerColor);
   graph -> SetMarkerStyle(markerStyle);
   graph -> SetLineWidth(1);
   graph -> SetLineStyle(1);
   graph -> SetMarkerSize(0.8);
   return graph;
   // const_cast<TH1D*>(&efficiencyHistogram) -> Draw("HIST");
}

void EfficiencyPlotsModule::createEfficiencyVsDelayDefaultPlots(const std::vector<EfficiencyPlotsModule*>& modulePtrs, const JSON& config, const int& delayPlotsNumBins, const float& delayPlotsLowerEdge, const float& delayPlotsUpperEdge)
{
      std::vector<std::string> labelsWBC;
      for(int wbcSetting = delayPlotsLowerEdge; wbcSetting < delayPlotsUpperEdge; wbcSetting++)
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

      std::vector<TH1D*> delayVsClusterSizePixelsBpixFpix                   ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterSizePixelsBpixFpixHits               ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterSizePixelsLayersNegativePositive     ( 8, nullptr);
      std::vector<TH1D*> delayVsClusterSizePixelsLayersNegativePositiveHits ( 8, nullptr);
      std::vector<TH1D*> delayVsClusterSizePixelsDisksInnerOuter            (12, nullptr);
      std::vector<TH1D*> delayVsClusterSizePixelsDisksInnerOuterHits        (12, nullptr);
      std::vector<TH1D*> delayVsClusterSizePixelsBNPZHSSIOLP                (64, nullptr);
      std::vector<TH1D*> delayVsClusterSizePixelsBNPZHSSIOLPHits            (64, nullptr);
      std::vector<TH1D*> delayVsClusterChargeBpixFpix                       ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterChargeBpixFpixHits                   ( 2, nullptr);
      std::vector<TH1D*> delayVsClusterChargeLayersNegativePositive         ( 8, nullptr);
      std::vector<TH1D*> delayVsClusterChargeLayersNegativePositiveHits     ( 8, nullptr);
      std::vector<TH1D*> delayVsClusterChargeDisksInnerOuter                (12, nullptr);
      std::vector<TH1D*> delayVsClusterChargeDisksInnerOuterHits            (12, nullptr);
      std::vector<TH1D*> delayVsClusterChargeBNPZHSSIOLP                    (64, nullptr);
      std::vector<TH1D*> delayVsClusterChargeBNPZHSSIOLPHits                (64, nullptr);

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
         std::string detAsString = detStrings[det];         // Cluster Size X
         delayVsClusterSizeXBpixFpix[det] = new TH1D(
            ("delayVsClusterSizeX" + detAsString).c_str(),
            ("Delay vs cluster X size on " + detAsString + ";delay;cluster X size (pix)").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         delayVsClusterSizeXBpixFpixHits[det] = new TH1D(
            ("delayVsClusterSizeXHits" + detAsString).c_str(),
            ("Number of clusters for delay scenarios on " + detAsString + ";delay;clusters").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         // Cluster Size Y
         delayVsClusterSizeYBpixFpix[det] = new TH1D(
            ("delayVsClusterSizeY" + detAsString).c_str(),
            ("Delay vs cluster Y size on " + detAsString + ";delay;cluster Y size (pix)").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         delayVsClusterSizeYBpixFpixHits[det] = new TH1D(
            ("delayVsClusterSizeYHits" + detAsString).c_str(),
            ("Number of clusters for delay scenarios on " + detAsString + ";delay;clusters").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         // Cluster Total Size
         delayVsClusterSizePixelsBpixFpix[det] = new TH1D(
            ("delayVsClusterSizePixels" + detAsString).c_str(),
            ("Delay vs cluster size on " + detAsString + ";delay;cluster size (pix)").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         delayVsClusterSizePixelsBpixFpixHits[det] = new TH1D(
            ("delayVsClusterSizePixelsHits" + detAsString).c_str(),
            ("Number of clusters for delay scenarios on " + detAsString + ";delay;clusters").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         // Cluster Charge 
         delayVsClusterChargeBpixFpix[det] = new TH1D(
            ("delayVsClusterCharge" + detAsString).c_str(),
            ("Delay vs charge on " + detAsString + ";delay;charge (ke)").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         delayVsClusterChargeBpixFpixHits[det] = new TH1D(
            ("delayVsClusterChargeHits" + detAsString).c_str(),
            ("Number of clusters for delay scenarios on " + detAsString + ";delay;clusters").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         // Efficiency
         delayVsEfficiencyBpixFpix[det] = new TH1D(
            ("delayVsEfficiency" + detAsString).c_str(),
            ("Delay vs efficiency on " + detAsString + ";delay;efficiency").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         delayVsEfficiencyBpixFpixHits[det] = new TH1D(
            ("delayVsEfficiencyHits" + detAsString).c_str(),
            ("Number of eff. hits for delay scenarios on " + detAsString + ";delay;hits").c_str(), 
            delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
         delayVsEfficiencyBpixFpix[det] -> GetXaxis() -> SetNdivisions(500 + delayPlotsNumBins, kFALSE);
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
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterSizeXLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizeXLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of clusters for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;clusters").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Cluster Size Y
            delayVsClusterSizeYLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizeYLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs cluster Y size on layer " + layerAsString + ", " + sideAsString + " side;delay;cluster Y size (pix)").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterSizeYLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizeYLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of clusters for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;clusters").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Cluster Total Size
            delayVsClusterSizePixelsLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizePixelsLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs cluster size on layer " + layerAsString + ", " + sideAsString + " side;delay;cluster size (pix)").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterSizePixelsLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterSizePixelsLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of clusters for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;clusters").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Charge
            delayVsClusterChargeLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterChargeLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs cluster charge on layer " + layerAsString + ", " + sideAsString + " side;delay;cluster charge (ke)").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterChargeLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsClusterChargeLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of clusters for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;clusters").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Efficiency
            delayVsEfficiencyLayersNegativePositive[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsEfficiencyLayer" + layerAsString + sideAsString).c_str(),
               ("Delay vs efficiency on layer " + layerAsString + ", " + sideAsString + " side;delay;efficiency").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsEfficiencyLayersNegativePositiveHits[(layer - 1) * 2 + side - 1] = new TH1D(
               ("delayVsEfficiencyLayerHits" + layerAsString + sideAsString).c_str(),
               ("Number of eff. hits for delay scenarios on layer " + layerAsString + ", " + sideAsString + " side;delay;hits").c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
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
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterSizeXDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsClusterSizeXDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of clusters for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;clusters" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Cluster Size Y
            delayVsClusterSizeYDisksInnerOuter[histogramIndex] = new TH1D(
               ("delayVsClusterSizeYDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs cluster Y size on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;cluster Y size (pix)" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterSizeYDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsClusterSizeYDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of clusters for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;clusters" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Cluster Total Size
            delayVsClusterSizePixelsDisksInnerOuter[histogramIndex] = new TH1D(
               ("delayVsClusterSizePixelsDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs cluster size on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;cluster size (pix)" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterSizePixelsDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsClusterSizePixelsDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of clusters for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;clusters" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Charge
            delayVsClusterChargeDisksInnerOuter[histogramIndex] = new TH1D(
               ("delayVsClusterChargeDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs cluster charge on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;cluster charge (ke)" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsClusterChargeDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsClusterChargeDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of clusters for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;clusters" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            // Efficiency
            delayVsEfficiencyDisksInnerOuter[histogramIndex] = new TH1D(
               ("delayVsEfficiencyDisk" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Delay vs efficiency on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;efficiency" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
            delayVsEfficiencyDisksInnerOuterHits[histogramIndex] = new TH1D(
               ("delayVsEfficiencyDiskHits" + diskAsString + sideAsString + "Side" + ringAsString + "Ring").c_str(),
               ("Number of eff. hits for delay scenarios on disk " + diskAsString + ", " + sideAsString + " Z, " + ringAsString + " Ring;delay;hits" ).c_str(),
               delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
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
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  delayVsClusterSizeXBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsClusterSizeXHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of clusters for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;clusters").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  // Cluster Size Y
                  delayVsClusterSizeYBNPZHSSIOLP[plotIndex] = new TH1D(
                     ("delayVsClusterSizeY" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs cluster Y size on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;cluster Y size (pix)").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  delayVsClusterSizeYBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsClusterSizeYHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of clusters for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;clusters").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  // Cluster Total Size
                  delayVsClusterSizePixelsBNPZHSSIOLP[plotIndex] = new TH1D(
                     ("delayVsClusterSizePixels" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs cluster size on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;cluster size (pix)").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  delayVsClusterSizePixelsBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsClusterSizePixelsHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of clusters for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;clusters").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  // Charge
                  delayVsClusterChargeBNPZHSSIOLP[plotIndex] = new TH1D(
                     ("delayVsClusterCharge" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs cluster charge on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;cluster charge (ke)").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  delayVsClusterChargeBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsClusterChargeHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of clusters for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;clusters").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  // Efficiency
                  delayVsEfficiencyBNPZHSSIOLP[plotIndex] = new TH1D(
                     ("delayVsEfficiency" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Delay vs efficiency on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;efficiency").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
                  delayVsEfficiencyBNPZHSSIOLPHits[plotIndex] = new TH1D(
                     ("delayVsEfficiencyHits" + sideAsString + "Z" + halfShellAsString + "XShell" + "Sector" + secAsString + "Layers" + layerPairAsShortString).c_str(),
                     ("Number of eff. hits for delay scenarios on " + sideAsString + " Z, " + halfShellAsString + "X half-shell, sector " + secAsString + ", layers " + layerPairAsString + "delay;hits").c_str(),
                     delayPlotsNumBins, delayPlotsLowerEdge, delayPlotsUpperEdge);
               }
            }
         }
      }
      for(size_t moduleIndex: range(modulePtrs.size()))
      {
         EfficiencyPlotsModule& module = *(modulePtrs[moduleIndex]);
         const float&                 delay  = module.delayInNs_;
         // Cluster Size X
         const std::array<std::pair<Long64_t, Long64_t>, 2>*  clusterSizeXBpixFpix                    = module.getClusterSizeXBpixFpix();
         const std::array<std::pair<Long64_t, Long64_t>, 8>*  clusterSizeXLayersNegativePositive      = module.getClusterSizeXLayersNegativePositive();
         const std::array<std::pair<Long64_t, Long64_t>, 12>* clusterSizeXDisksInnerOuter             = module.getClusterSizeXDisksInnerOuter();
         const std::array<std::pair<Long64_t, Long64_t>, 64>* clusterSizeXBNPZHSSIOLP                 = module.getClusterSizeXBNPZHSSIOLP();
         // Cluster Size Y
         const std::array<std::pair<Long64_t, Long64_t>, 2>*  clusterSizeYBpixFpix                    = module.getClusterSizeYBpixFpix();
         const std::array<std::pair<Long64_t, Long64_t>, 8>*  clusterSizeYLayersNegativePositive      = module.getClusterSizeYLayersNegativePositive();
         const std::array<std::pair<Long64_t, Long64_t>, 12>* clusterSizeYDisksInnerOuter             = module.getClusterSizeYDisksInnerOuter();
         const std::array<std::pair<Long64_t, Long64_t>, 64>* clusterSizeYBNPZHSSIOLP                 = module.getClusterSizeYBNPZHSSIOLP();
         // Cluster Total Size
         const std::array<std::pair<Long64_t, Long64_t>, 2>*  clusterSizePixelsBpixFpix               = module.getClusterSizePixelsBpixFpix();
         const std::array<std::pair<Long64_t, Long64_t>, 8>*  clusterSizePixelsLayersNegativePositive = module.getClusterSizePixelsLayersNegativePositive();
         const std::array<std::pair<Long64_t, Long64_t>, 12>* clusterSizePixelsDisksInnerOuter        = module.getClusterSizePixelsDisksInnerOuter();
         const std::array<std::pair<Long64_t, Long64_t>, 64>* clusterSizePixelsBNPZHSSIOLP            = module.getClusterSizePixelsBNPZHSSIOLP();
         // Charge
         const std::array<std::pair<Long64_t, Long64_t>, 2>*  clusterChargeBpixFpix                   = module.getClusterChargeBpixFpix();
         const std::array<std::pair<Long64_t, Long64_t>, 8>*  clusterChargeLayersNegativePositive     = module.getClusterChargeLayersNegativePositive();
         const std::array<std::pair<Long64_t, Long64_t>, 12>* clusterChargeDisksInnerOuter            = module.getClusterChargeDisksInnerOuter();
         const std::array<std::pair<Long64_t, Long64_t>, 64>* clusterChargeBNPZHSSIOLP                = module.getClusterChargeBNPZHSSIOLP();
         // Efficiency
         const std::array<std::pair<Long64_t, Long64_t>, 2>*  efficiencyBpixFpix                      = module.getEfficiencyBpixFpix();
         const std::array<std::pair<Long64_t, Long64_t>, 8>*  efficiencyLayersNegativePositive        = module.getEfficiencyLayersNegativePositive();
         const std::array<std::pair<Long64_t, Long64_t>, 12>* efficiencyDisksInnerOuter               = module.getEfficiencyDisksInnerOuter();
         const std::array<std::pair<Long64_t, Long64_t>, 64>* efficiencyBNPZHSSIOLP                   = module.getEfficiencyBNPZHSSIOLP();
         for(unsigned int plotIndex = 0; plotIndex < 2; ++plotIndex)
         {
            // Cluster Size X
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeXBpixFpix[plotIndex]     -> Fill(delay / 25, (*clusterSizeXBpixFpix)[plotIndex].second / static_cast<double>((*clusterSizeXBpixFpix)[plotIndex].first));
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeXBpixFpixHits[plotIndex] -> Fill(delay / 25, (*clusterSizeXBpixFpix)[plotIndex].first);
            // Cluster Size Y
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeYBpixFpix[plotIndex]     -> Fill(delay / 25, (*clusterSizeYBpixFpix)[plotIndex].second / static_cast<double>((*clusterSizeYBpixFpix)[plotIndex].first));
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizeYBpixFpixHits[plotIndex] -> Fill(delay / 25, (*clusterSizeYBpixFpix)[plotIndex].first);
            // Cluster Total Size
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizePixelsBpixFpix[plotIndex]     -> Fill(delay / 25, (*clusterSizePixelsBpixFpix)[plotIndex].second / static_cast<double>((*clusterSizePixelsBpixFpix)[plotIndex].first));
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterSizePixelsBpixFpixHits[plotIndex] -> Fill(delay / 25, (*clusterSizePixelsBpixFpix)[plotIndex].first);
            // Cluster Charge
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterChargeBpixFpix[plotIndex]     -> Fill(delay / 25, (*clusterChargeBpixFpix)[plotIndex].second / static_cast<double>((*clusterChargeBpixFpix)[plotIndex].first));
            if((*clusterSizeXBpixFpix)[plotIndex].first != 0) delayVsClusterChargeBpixFpixHits[plotIndex] -> Fill(delay / 25, (*clusterChargeBpixFpix)[plotIndex].first);
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
            // Cluster Total Size
            if((*clusterSizePixelsLayersNegativePositive)[plotIndex].first != 0) delayVsClusterSizePixelsLayersNegativePositive[plotIndex]     -> Fill(delay / 25, (*clusterSizePixelsLayersNegativePositive)[plotIndex].second / static_cast<double>((*clusterSizePixelsLayersNegativePositive)[plotIndex].first));
            if((*clusterSizePixelsLayersNegativePositive)[plotIndex].first != 0) delayVsClusterSizePixelsLayersNegativePositiveHits[plotIndex] -> Fill(delay / 25, (*clusterSizePixelsLayersNegativePositive)[plotIndex].first);
            // Cluster Charge
            if((*clusterChargeLayersNegativePositive)[plotIndex].first != 0) delayVsClusterChargeLayersNegativePositive[plotIndex]     -> Fill(delay / 25, (*clusterChargeLayersNegativePositive)[plotIndex].second / static_cast<double>((*clusterChargeLayersNegativePositive)[plotIndex].first));
            if((*clusterChargeLayersNegativePositive)[plotIndex].first != 0) delayVsClusterChargeLayersNegativePositiveHits[plotIndex] -> Fill(delay / 25, (*clusterChargeLayersNegativePositive)[plotIndex].first);
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
            // Cluster Total Size
            if((*clusterSizePixelsDisksInnerOuter)[plotIndex].first != 0) delayVsClusterSizePixelsDisksInnerOuter[plotIndex]      -> Fill(delay / 25, (*clusterSizePixelsDisksInnerOuter)[plotIndex].second / static_cast<double>((*clusterSizePixelsDisksInnerOuter)[plotIndex].first));
            if((*clusterSizePixelsDisksInnerOuter)[plotIndex].first != 0) delayVsClusterSizePixelsDisksInnerOuterHits[plotIndex]  -> Fill(delay / 25, (*clusterSizePixelsDisksInnerOuter)[plotIndex].first);
            // Cluster Charge
            if((*clusterChargeDisksInnerOuter)[plotIndex].first != 0) delayVsClusterChargeDisksInnerOuter[plotIndex]      -> Fill(delay / 25, (*clusterChargeDisksInnerOuter)[plotIndex].second / static_cast<double>((*clusterChargeDisksInnerOuter)[plotIndex].first));
            if((*clusterChargeDisksInnerOuter)[plotIndex].first != 0) delayVsClusterChargeDisksInnerOuterHits[plotIndex]  -> Fill(delay / 25, (*clusterChargeDisksInnerOuter)[plotIndex].first);
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
            // Cluster Total Size
            if((*clusterSizePixelsBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterSizePixelsBNPZHSSIOLP[plotIndex]     -> Fill(delay / 25, (*clusterSizePixelsBNPZHSSIOLP)[plotIndex].second / static_cast<double>((*clusterSizePixelsBNPZHSSIOLP)[plotIndex].first));
            if((*clusterSizePixelsBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterSizePixelsBNPZHSSIOLPHits[plotIndex] -> Fill(delay / 25, (*clusterSizePixelsBNPZHSSIOLP)[plotIndex].first);
            // Cluster Charge
            if((*clusterChargeBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterChargeBNPZHSSIOLP[plotIndex]     -> Fill(delay / 25, (*clusterChargeBNPZHSSIOLP)[plotIndex].second / static_cast<double>((*clusterChargeBNPZHSSIOLP)[plotIndex].first));
            if((*clusterChargeBNPZHSSIOLP)[plotIndex].first != 0) delayVsClusterChargeBNPZHSSIOLPHits[plotIndex] -> Fill(delay / 25, (*clusterChargeBNPZHSSIOLP)[plotIndex].first);
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
      gDirectory -> mkdir("Cluster_size_X_vs_delay/Graphs");
      gDirectory -> mkdir("Cluster_size_Y_vs_delay");
      gDirectory -> mkdir("Cluster_size_Y_vs_delay/Graphs");
      gDirectory -> mkdir("Cluster_total_size_vs_delay");
      gDirectory -> mkdir("Cluster_total_size_vs_delay/Graphs");
      gDirectory -> mkdir("Cluster_charge_vs_delay");
      gDirectory -> mkdir("Cluster_charge_vs_delay/Graphs");
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
      // Cluster Total Size
      gDirectory -> cd("../Cluster_total_size_vs_delay");
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizePixelsBpixFpix,               "" , "Cluster_total_size_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizePixelsLayersNegativePositive, "" , "Cluster_total_size_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizePixelsDisksInnerOuter,        "" , "Cluster_total_size_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterSizePixelsBNPZHSSIOLP,            "" , "Cluster_total_size_vs_delay", config);
      // Cluster Charge
      gDirectory -> cd("../Cluster_charge_vs_delay");
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterChargeBpixFpix,               "" , "Cluster_charge_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterChargeLayersNegativePositive, "" , "Cluster_charge_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterChargeDisksInnerOuter,        "" , "Cluster_charge_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsClusterChargeBNPZHSSIOLP,            "" , "Cluster_charge_vs_delay", config);
      // Efficiency
      gDirectory -> cd("../Efficiency_vs_delay");
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyBpixFpix,               "" , "Efficiency_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyLayersNegativePositive, "" , "Efficiency_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyDisksInnerOuter,        "" , "Efficiency_vs_delay", config);
      EfficiencyPlotsModule::saveHistogramsInCollectionIfNotEmpty(delayVsEfficiencyBNPZHSSIOLP,            "" , "Efficiency_vs_delay", config);
      if(config["save_histograms_to_ntuple"] == true)
      {
         gDirectory -> cd("/");
         gDirectory -> cd("Efficiency_vs_delay");
         gDirectory -> cd("Graphs");
         gDirectory -> mkdir("BpixFpix");
         gDirectory -> mkdir("LayersNegativePositive");
         gDirectory -> mkdir("DisksInnerOuter");
         gDirectory -> mkdir("LayerDetailed");
         gDirectory -> cd("BpixFpix");
         std::vector<int> markerSet = {20, 21, 22, 23, 29, 33, 34, 3};
         for(unsigned int plotIndex = 0; plotIndex < 2; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyBpixFpix[plotIndex], delayVsEfficiencyBpixFpixHits[plotIndex], 4, markerSet[plotIndex]);
         gDirectory -> cd("../LayersNegativePositive");
         for(unsigned int plotIndex = 0; plotIndex < 8; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyLayersNegativePositive[plotIndex], delayVsEfficiencyLayersNegativePositiveHits[plotIndex], 4, markerSet[plotIndex]);
         gDirectory -> cd("../DisksInnerOuter");
         for(unsigned int plotIndex = 0; plotIndex < 12; ++plotIndex) EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyDisksInnerOuter[plotIndex], delayVsEfficiencyDisksInnerOuterHits[plotIndex], 4, markerSet[plotIndex % 8]);
         gDirectory -> cd("../LayerDetailed");
         for(unsigned int plotIndex = 0; plotIndex < 64; ++plotIndex) EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsEfficiencyBNPZHSSIOLP[plotIndex], delayVsEfficiencyBNPZHSSIOLPHits[plotIndex], 4, markerSet[plotIndex % 8]);
         gDirectory -> cd("/");
         gDirectory -> cd("Cluster_total_size_vs_delay");
         gDirectory -> cd("Graphs");
         gDirectory -> mkdir("BpixFpix");
         gDirectory -> mkdir("LayersNegativePositive");
         gDirectory -> mkdir("DisksInnerOuter");
         gDirectory -> mkdir("LayerDetailed");
         gDirectory -> cd("BpixFpix");
         for(unsigned int plotIndex = 0; plotIndex < 2; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsClusterSizePixelsBpixFpix[plotIndex], delayVsClusterSizePixelsBpixFpixHits[plotIndex], 2, markerSet[plotIndex]);
         gDirectory -> cd("../LayersNegativePositive");
         for(unsigned int plotIndex = 0; plotIndex < 8; ++plotIndex)  EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsClusterSizePixelsLayersNegativePositive[plotIndex], delayVsClusterSizePixelsLayersNegativePositiveHits[plotIndex], 2, markerSet[plotIndex]);
         gDirectory -> cd("../DisksInnerOuter");
         for(unsigned int plotIndex = 0; plotIndex < 12; ++plotIndex) EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsClusterSizePixelsDisksInnerOuter[plotIndex], delayVsClusterSizePixelsDisksInnerOuterHits[plotIndex], 2, markerSet[plotIndex % 8]);
         gDirectory -> cd("../LayerDetailed");
         for(unsigned int plotIndex = 0; plotIndex < 64; ++plotIndex) EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(delayVsClusterSizePixelsBNPZHSSIOLP[plotIndex], delayVsClusterSizePixelsBNPZHSSIOLPHits[plotIndex], 2, markerSet[plotIndex % 8]);
      }
      // std::cout << delayVsEfficiencyBpixFpix[0] -> GetXaxis() -> GetNdivisions() << std::endl;
      // delayVsEfficiencyBpixFpix[0] -> SaveAs("test.C");
}


// ------------------------------------------------------------------------------------------------

// Static function used to create efficiency vs delay plots for each of the ROC-s.
// Takes a list of pointers to module objects, returns a vector of dynamically allocated 
// TH1D histograms. The first vector index is the layer (0 for the forward region) the ROC is 
// located on, the second is the bin number of the ROC on the histograms.
// 
// - Requires the "efficiencyROCPlots" to be filled before. (throws an std::runtime_error otherwise)
// - Discards histograms where delayInNs_ is NOVAL_F (aka. -9999.0)

std::vector<std::vector<TH1*>> EfficiencyPlotsModule::createEfficiencyVsDelayROCPlots(const std::vector<EfficiencyPlotsModule*>& modulePtrs)
{
   auto discardNOVAL_F = [] (const EfficiencyPlotsModule* module) { return module -> delayInNs_ != NOVAL_F; };
   auto moduleEfficiencyROCPlotsFilled = [] (const EfficiencyPlotsModule* module)
   {
      if(module == nullptr) throw std::runtime_error("Trying to deference a nullptr in the module list. This is not supported.");
      return std::any_of(module -> efficiencyROCPlots.begin(), module -> efficiencyROCPlots.end(), [](TH1 *histogram) {
         if(!histogram)
         {
            return false;
         }
         return 0 < histogram->GetEntries();
      });
   };
   // Filtering out unknown delays
   std::vector<EfficiencyPlotsModule*> modulesWithKnownDelay;
   modulesWithKnownDelay.reserve(modulePtrs.size());
   std::copy_if(modulePtrs.begin(), modulePtrs.end(), std::back_inserter(modulesWithKnownDelay), discardNOVAL_F);
   modulesWithKnownDelay.shrink_to_fit();
   // Testing if there are efficiencyROCPlots filled
   bool areEfficiencyROCPlotsFilled = std::any_of(modulesWithKnownDelay.begin(), modulesWithKnownDelay.end(), moduleEfficiencyROCPlotsFilled);
   if(!areEfficiencyROCPlotsFilled) throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ":efficiency ROC plots are not filled.");
   // Calculation of delay limts
   std::vector<float> delayValues;
   delayValues.reserve(modulesWithKnownDelay.size());
   for(const auto& module: modulesWithKnownDelay) { delayValues.push_back(module -> delayInNs_); }
   float minDelay, maxDelay;
   std::tie(minDelay, maxDelay) = deref_minmax_element(delayValues.begin(), delayValues.end());
   // To avoid floating-point errors in the difference calculation
   // truncating the actual value + 0.5f
   int delayBins = maxDelay - minDelay + 1.0f + 0.5f;
   // Create histograms for each of the ROC-s
   std::vector<std::vector<TH1*>> rocEfficiencyHits;
   std::vector<LayersDiskPlotIndecies> plotIndexList = { AllDisksEfficiency, Layer1Efficiency, Layer2Efficiency, Layer3Efficiency, Layer4Efficiency };
   for(size_t plotIndex: plotIndexList)
   {
      // Using the histograms ot the first module as templates
      // GetNbinsX() does not count the under- and overflow bins
      size_t numBins =  dynamic_cast<TH2D*>(modulesWithKnownDelay.front() -> efficiencyROCPlots[plotIndex]) -> GetSize();
      // Create a vector with X TH1D elements, where X is the number of
      // bins the passed efficiencyHistogram has, excluding the under and overflow bins 
      std::vector<TH1*> rocEfficiency;
      rocEfficiency.reserve(numBins);
      for(size_t index = 0; index < numBins; ++index)
      {
         std::string histogramName  = "delayVsEfficiencyOnROCs" + modulesWithKnownDelay.front() -> plotNameSuffices[plotIndex] + "ROC" + std::to_string(index + 1);
         std::string histogramTitle = "Efficiency on " + modulesWithKnownDelay.front() -> plotTitleSuffices[plotIndex] + " ROC " + std::to_string(index + 1);
         rocEfficiency.emplace_back(new TH1D(histogramName.c_str(), histogramTitle.c_str(), delayBins, minDelay, maxDelay));
      }
      rocEfficiency.shrink_to_fit();
      rocEfficiencyHits.emplace_back(rocEfficiency);
   }
   for(auto module: modulesWithKnownDelay)
   {
      std::vector<TH1*> efficiencyHistograms;
      for(const auto& plotIndex: plotIndexList) efficiencyHistograms.push_back(module -> efficiencyROCPlots[plotIndex]);
      for(size_t index: range(efficiencyHistograms.size()))
      {
         Long64_t numBins = dynamic_cast<TH2D*>(efficiencyHistograms[index]) -> GetSize();
         // Add the content of the bins of the given efficiencyROCPlot to 
         // the corresponding delay bin of the ROC's histogram
         for(Long64_t binIndex = 0; binIndex < numBins; ++binIndex)
         {
            // std::cout << "Filling at: " << module -> delayInNs_ << std::endl;
            // std::cout << "Filling with a weight of: " << dynamic_cast<TH2D*>(efficiencyHistograms[index]) -> GetAt(binIndex) << std::endl;
            rocEfficiencyHits[index][binIndex] -> Fill(module -> delayInNs_, dynamic_cast<TH2D*>(efficiencyHistograms[index]) -> GetAt(binIndex));
         }
      }
   }
   return rocEfficiencyHits;
}


void EfficiencyPlotsModule::downscaleCollectionIfNotEmpty(EfficiencyPlotPair& plotPair)
{
   if(!histogramExistsAndNotEmpty(plotPair[0]) || !histogramExistsAndNotEmpty(plotPair[1])) return;
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
   if(std::any_of(collection.begin(), collection.end(), [&] (const auto& histogram) { return EfficiencyPlotsModule::histogramExistsAndNotEmpty(histogram); }))
   {
      // std::cout << "Saving plots of collection: " << subdirectoryName << std::endl;
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
   if(!histogramExistsAndNotEmpty(histogram)) return;
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
      draw2DPlot(histo2D);
      // dressIfROCPlot(histo2D);
   }
   else if(histo1D != nullptr)
   {
      draw1DPlot(histo1D);
   }
   canvas -> Update();
   gPad -> Update();
   if(config["save_histograms_to_ntuple"] == true)
   {
      histogram -> Write();
      canvas -> Write();
   }
   if(config["save_plots"] == true) saveCanvasAsEps(canvas, config["plots_save_directory"].get<std::string>());
   gDirectory -> cd("/");
   gDirectory -> cd(originalDirectory);
}

bool EfficiencyPlotsModule::histogramExistsAndNotEmpty(TH1* histogram)
{
   if(!histogram) return false;
   if(histogram -> GetEntries() == 0) return false;
   return true;
}

void EfficiencyPlotsModule::draw1DPlot(TH1D* histogram)
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
   // }
}

void EfficiencyPlotsModule::writeEfficiencyPlotAsGraph(TH1D* efficiencyHistogram, TH1D* numHitsHistogram, const int& markerColor, const int& markerStyle)
{
   TGraphAsymmErrors* graph = getEfficiencyGraphAsymmErrors(*efficiencyHistogram, *numHitsHistogram, markerColor);
   graph -> SetName((efficiencyHistogram -> GetName() + std::string("AsGraph")).c_str());
   graph -> Draw("ap");
   graph -> Write();
}

void EfficiencyPlotsModule::draw2DPlot(TH2D* histogram)
{
   histogram -> Draw("COLZ");
}

void EfficiencyPlotsModule::saveCanvasAsEps(TCanvas* canvas, const std::string& parentDirectoryName)
{
   std::string epsFilename = (std::string(canvas -> GetTitle()) + ".eps");
   std::transform(epsFilename.begin(), epsFilename.end(), epsFilename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
   {
      std::string savePath = parentDirectoryName + "/" + epsFilename;
      canvas -> SaveAs(savePath.c_str());
   }
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

bool EfficiencyPlotsModule::testForForwardFidicualCuts()
{
   using PolygonDefinition = const std::pair<std::vector<float>, std::vector<float>>;
   const int&   side  = trajField_.mod_on.side;
   const int&   disk  = trajField_.mod_on.disk;
   const int&   panel = trajField_.mod_on.panel;
   const int&   ring  = trajField_.mod_on.ring;
   const float& lx    = trajField_.lx;
   const float& ly    = trajField_.ly;
   const int           panelOrientation = ((side - 1) << 2) + ((ring - 1) << 1) + panel - 1; // -Z, +Z, ring 1, ring 2, panel 1, panel 2
   // Test these by plotting with TCutG

   static PolygonDefinition filterForDisk1NegativeZRing1Panel1 = {{-0.65,   0.65,  0.19       }, {2.4, 2.60, -2.00       }};
   static PolygonDefinition filterForDisk1NegativeZRing1Panel2 = {{-0.65,   0.60,  0.2        }, {2.5, 2.40, -2.00       }}; 
   static PolygonDefinition filterForDisk1NegativeZRing2Panel1 = {{-0.35,   0.65,  0.65,  0.30}, {1.4, 1.40, -2.40, -2.40}};
   static PolygonDefinition filterForDisk1NegativeZRing2Panel2 = {{-0.35,   0.65,  0.65,  0.35}, {1.3, 1.30, -2.50, -2.50}}; 
   static PolygonDefinition filterForDisk1PositiveZRing1Panel1 = {{-0.60,   0.64, -0.18       }, {2.7, 2.50, -1.80       }};
   static PolygonDefinition filterForDisk1PositiveZRing1Panel2 = {{-0.65,   0.65, -0.20       }, {2.2, 2.45, -1.80       }};
   static PolygonDefinition filterForDisk1PositiveZRing2Panel1 = {{-0.70,   0.33, -0.34, -0.70}, {1.5, 1.50, -2.40, -2.40}};
   static PolygonDefinition filterForDisk1PositiveZRing2Panel2 = {{-0.70,   0.25, -0.8,  -0.70}, {1.2, 1.20, -2.50, -2.50}};
   static PolygonDefinition filterForDisk2NegativeZRing1Panel1 = {{-0.66,   0.66,  0.3,   0.00}, {2.4, 2.60, -0.90, -0.90}};
   static PolygonDefinition filterForDisk2NegativeZRing1Panel2 = {{-0.70,   0.63,  0.25,  0.00}, {2.2, 2.10, -1.40, -1.40}};
   static PolygonDefinition filterForDisk2NegativeZRing2Panel1 = {{-0.37,   0.67,  0.68,  0.36}, {1.5, 1.50, -2.40, -2.40}};
   static PolygonDefinition filterForDisk2NegativeZRing2Panel2 = {{-0.38,   0.68,  0.68,  0.35}, {1.6, 1.55, -2.40, -2.40}};
   static PolygonDefinition filterForDisk2PositiveZRing1Panel1 = {{-0.61,   0.64,  0.0,  -0.20}, {2.4, 2.20, -1.20, -1.10}};
   static PolygonDefinition filterForDisk2PositiveZRing1Panel2 = {{-0.65,   0.65,  0.0,  -0.25}, {2.1, 2.20, -1.20, -1.20}};
   static PolygonDefinition filterForDisk2PositiveZRing2Panel1 = {{-0.70,   0.40, -0.34, -0.70}, {1.8, 1.80, -2.10, -2.10}};
   static PolygonDefinition filterForDisk2PositiveZRing2Panel2 = {{-0.65,   0.43, -0.38, -0.65}, {1.6, 1.80, -2.30, -2.30}};
   static PolygonDefinition filterForDisk3NegativeZRing1Panel1 = {{-0.60,   0.65,  0.34, -0.30}, {2.3, 2.40,  0.60,  0.60}};
   static PolygonDefinition filterForDisk3NegativeZRing1Panel2 = {{-0.68,   0.60,  0.38, -0.34}, {2.2, 2.10,  0.74,  0.85}};
   static PolygonDefinition filterForDisk3NegativeZRing2Panel1 = {{-0.43,   0.65,  0.65,  0.33}, {1.9, 1.90, -2.00, -2.00}};
   static PolygonDefinition filterForDisk3NegativeZRing2Panel2 = {{-0.43,   0.65,  0.65,  0.33}, {1.9, 1.90, -2.00, -2.00}};
   static PolygonDefinition filterForDisk3PositiveZRing1Panel1 = {{-0.63,   0.67,  0.24, -0.33}, {2.4, 2.10,  0.87,  0.87}};
   static PolygonDefinition filterForDisk3PositiveZRing1Panel2 = {{-0.61,   0.61,  0.30, -0.45}, {2.1, 2.20,  0.60,  0.60}};
   static PolygonDefinition filterForDisk3PositiveZRing2Panel1 = {{-0.70,   0.50, -0.34, -0.70}, {2.2, 2.20, -2.30, -2.30}};
   static PolygonDefinition filterForDisk3PositiveZRing2Panel2 = {{-0.70,   0.50, -0.34, -0.70}, {2.0, 2.00, -2.30, -2.30}};

   if(disk == -1)
   {
      // Disk 1, negative Z, ring 1, panel 1
      if(panelOrientation == 0)
      {
         // std::cout << "panelOrientation: " << panelOrientation << std::endl;
         // std::cout << "disk:             " << disk             << std::endl;
         // std::cout << "side:             " << side             << std::endl;
         // std::cout << "panel:            " << panel            << std::endl;
         // std::cout << "lx:               " << lx               << std::endl;
         // std::cout << "ly:               " << ly               << std::endl;
         // std::cout << "passed:           " << isPointInPolygon(lx, ly, filterForDisk1NegativeZRing1Panel1) << std::endl;
         // std::cin.get();
         return isPointInPolygon(lx, ly, filterForDisk1NegativeZRing1Panel1);
      }
      // Disk 1, negative Z, ring 1, panel 2
      if(panelOrientation == 1) return isPointInPolygon(lx, ly, filterForDisk1NegativeZRing1Panel2);
      // Disk 1, negative Z, ring 2, panel 1
      if(panelOrientation == 2) return isPointInPolygon(lx, ly, filterForDisk1NegativeZRing2Panel1);
      // Disk 1, negative Z, ring 2, panel 2
      if(panelOrientation == 3) return isPointInPolygon(lx, ly, filterForDisk1NegativeZRing2Panel2);
   }
   if(disk == -2)
   {
      // Disk 2, negative Z, ring 1, panel 1 
      if(panelOrientation == 0) return isPointInPolygon(lx, ly, filterForDisk2NegativeZRing1Panel1);
      // Disk 2, negative Z, ring 1, panel 2 
      if(panelOrientation == 1) return isPointInPolygon(lx, ly, filterForDisk2NegativeZRing1Panel2);
      // Disk 2, negative Z, ring 2, panel 1 
      if(panelOrientation == 2) return isPointInPolygon(lx, ly, filterForDisk2NegativeZRing2Panel1);
      // Disk 2, negative Z, ring 2, panel 2 
      if(panelOrientation == 3) return isPointInPolygon(lx, ly, filterForDisk2NegativeZRing2Panel2);
   }
   if(disk == -3)
   {
      // Disk 3, negative Z, ring 1, panel 1 
      if(panelOrientation == 0) return isPointInPolygon(lx, ly, filterForDisk3NegativeZRing1Panel1);
      // Disk 3, negative Z, ring 1, panel 2 
      if(panelOrientation == 1) return isPointInPolygon(lx, ly, filterForDisk3NegativeZRing1Panel2);
      // Disk 3, negative Z, ring 2, panel 1 
      if(panelOrientation == 2) return isPointInPolygon(lx, ly, filterForDisk3NegativeZRing2Panel1);
      // Disk 3, negative Z, ring 2, panel 2 
      if(panelOrientation == 3) return isPointInPolygon(lx, ly, filterForDisk3NegativeZRing2Panel2);
   }
   if(disk ==  1)
   {
      // Disk 1, positive Z, ring 1, panel 1 
      if(panelOrientation == 4) return isPointInPolygon(lx, ly, filterForDisk1PositiveZRing1Panel1);
      // Disk 1, positive Z, ring 1, panel 2 
      if(panelOrientation == 5) return isPointInPolygon(lx, ly, filterForDisk1PositiveZRing1Panel2);
      // Disk 1, positive Z, ring 2, panel 1 
      if(panelOrientation == 6) return isPointInPolygon(lx, ly, filterForDisk1PositiveZRing2Panel1);
      // Disk 1, positive Z, ring 2, panel 2 
      if(panelOrientation == 7) return isPointInPolygon(lx, ly, filterForDisk1PositiveZRing2Panel2);
   }
   if(disk ==  2)
   {
      // Disk 2, positive Z, ring 1, panel 1 
      if(panelOrientation == 4) return isPointInPolygon(lx, ly, filterForDisk2PositiveZRing1Panel1);
      // Disk 2, positive Z, ring 1, panel 2 
      if(panelOrientation == 5) return isPointInPolygon(lx, ly, filterForDisk2PositiveZRing1Panel2);
      // Disk 2, positive Z, ring 2, panel 1 
      if(panelOrientation == 6) return isPointInPolygon(lx, ly, filterForDisk2PositiveZRing2Panel1);
      // Disk 2, positive Z, ring 2, panel 2 
      if(panelOrientation == 7) return isPointInPolygon(lx, ly, filterForDisk2PositiveZRing2Panel2);
   }
   if(disk ==  3)
   {
      // Disk 3, positive Z, ring 1, panel 1 
      if(panelOrientation == 4) return isPointInPolygon(lx, ly, filterForDisk3PositiveZRing1Panel1);
      // Disk 3, positive Z, ring 1, panel 2 
      if(panelOrientation == 5) return isPointInPolygon(lx, ly, filterForDisk3PositiveZRing1Panel2);
      // Disk 3, positive Z, ring 2, panel 1 
      if(panelOrientation == 6) return isPointInPolygon(lx, ly, filterForDisk3PositiveZRing2Panel1);
      // Disk 3, positive Z, ring 2, panel 2 
      if(panelOrientation == 7) return isPointInPolygon(lx, ly, filterForDisk3PositiveZRing2Panel2);
   }
   std::cout << error_prompt << __PRETTY_FUNCTION__ << ": end of control statement reached. This is an unexpected scenario." << std::endl;
   std::cout << debug_prompt << " disk: " << disk << ", panelOrientation: " << panelOrientation << std::endl;
   return false;
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
   badROCCut_counter_   += badROCCut;
   effCutAll_counter_   += effCutAll;
   // missing_counter_     += missing;
   // clust_near_counter_  += clust_near;
   entry_counter_++;
}

template <typename T>
void EfficiencyPlotsModule::setCollectionElementsToNullptr(T& collection)
{
   for(auto& histogram: collection)
   {
      histogram = nullptr;
   }
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

bool EfficiencyPlotsModule::isPointInPolygon(const float& pointX, const float& pointY, const std::pair<std::vector<float>, std::vector<float>>& poligonVertices)
{
   bool answer = false;
   int numVertices = poligonVertices.first.size();
   for(int i = 0, j = numVertices - 1; i < numVertices; j = i++)
   {
      if(((poligonVertices.second[i] > pointY) != (poligonVertices.second[j] > pointY)) && (pointX < (poligonVertices.first[j] - poligonVertices.first[i]) * (pointY - poligonVertices.second[i]) / (poligonVertices.second[j] - poligonVertices.second[i]) + poligonVertices.first[i]))
      {
        answer = !answer;
      }
   }
   return answer;
}
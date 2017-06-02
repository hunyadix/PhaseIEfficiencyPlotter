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
EfficiencyPlotsModule::BadROClist EfficiencyPlotsModule::badROClist = {
   { 0, 696   }, { 0, 3852  }, { 0, 4011  }, { 0, 6459  }, { 0, 7561  }, { 0, 8324  }, { 0, 8325  }, { 0, 8326  },
   { 0, 8327  }, { 0, 8328  }, { 0, 12076 }, { 0, 12077 }, { 0, 12078 }, { 0, 12079 }, { 0, 12080 }, { 0, 12081 },
   { 0, 12192 }, { 0, 12193 }, { 0, 12194 }, { 0, 12195 }, { 0, 15802 }, { 0, 15818 }, { 0, 15834 }, { 1, 76    },
   { 1, 83    }, { 1, 85    }, { 1, 91    }, { 1, 92    }, { 1, 95    }, { 1, 97    }, { 1, 98    }, { 1, 99    },
   { 1, 100   }, { 1, 101   }, { 1, 102   }, { 1, 104   }, { 1, 105   }, { 1, 106   }, { 1, 115   }, { 1, 116   },
   { 1, 117   }, { 1, 118   }, { 1, 119   }, { 1, 121   }, { 1, 122   }, { 1, 123   }, { 1, 127   }, { 1, 149   },
   { 1, 150   }, { 1, 159   }, { 1, 163   }, { 1, 166   }, { 1, 168   }, { 1, 169   }, { 1, 171   }, { 1, 172   },
   { 1, 173   }, { 1, 174   }, { 1, 175   }, { 1, 176   }, { 1, 177   }, { 1, 178   }, { 1, 179   }, { 1, 180   },
   { 1, 190   }, { 1, 191   }, { 1, 192   }, { 1, 194   }, { 1, 195   }, { 1, 196   }, { 1, 202   }, { 1, 205   },
   { 1, 228   }, { 1, 232   }, { 1, 234   }, { 1, 238   }, { 1, 239   }, { 1, 240   }, { 1, 241   }, { 1, 242   },
   { 1, 243   }, { 1, 244   }, { 1, 245   }, { 1, 246   }, { 1, 248   }, { 1, 251   }, { 1, 252   }, { 1, 253   },
   { 1, 254   }, { 1, 264   }, { 1, 265   }, { 1, 266   }, { 1, 268   }, { 1, 269   }, { 1, 272   }, { 1, 278   },
   { 1, 280   }, { 1, 303   }, { 1, 304   }, { 1, 307   }, { 1, 312   }, { 1, 316   }, { 1, 318   }, { 1, 319   },
   { 1, 321   }, { 1, 322   }, { 1, 323   }, { 1, 324   }, { 1, 325   }, { 1, 326   }, { 1, 327   }, { 1, 328   },
   { 1, 337   }, { 1, 338   }, { 1, 341   }, { 1, 353   }, { 1, 377   }, { 1, 378   }, { 1, 390   }, { 1, 391   },
   { 1, 392   }, { 1, 394   }, { 1, 395   }, { 1, 396   }, { 1, 397   }, { 1, 398   }, { 1, 399   }, { 1, 400   },
   { 1, 401   }, { 1, 402   }, { 1, 413   }, { 1, 415   }, { 1, 435   }, { 1, 436   }, { 1, 439   }, { 1, 440   },
   { 1, 447   }, { 1, 449   }, { 1, 451   }, { 1, 461   }, { 1, 462   }, { 1, 463   }, { 1, 464   }, { 1, 465   },
   { 1, 466   }, { 1, 467   }, { 1, 468   }, { 1, 469   }, { 1, 470   }, { 1, 471   }, { 1, 472   }, { 1, 473   },
   { 1, 474   }, { 1, 475   }, { 1, 476   }, { 1, 486   }, { 1, 487   }, { 1, 515   }, { 1, 531   }, { 1, 535   },
   { 1, 537   }, { 1, 538   }, { 1, 539   }, { 1, 540   }, { 1, 541   }, { 1, 543   }, { 1, 544   }, { 1, 546   },
   { 1, 547   }, { 1, 548   }, { 1, 550   }, { 1, 587   }, { 1, 607   }, { 1, 608   }, { 1, 616   }, { 1, 617   },
   { 1, 620   }, { 1, 622   }, { 1, 623   }, { 1, 624   }, { 1, 633   }, { 1, 634   }, { 1, 635   }, { 1, 637   },
   { 1, 707   }, { 1, 708   }, { 1, 709   }, { 1, 710   }, { 1, 711   }, { 1, 712   }, { 1, 716   }, { 1, 727   },
   { 1, 728   }, { 1, 729   }, { 1, 730   }, { 1, 736   }, { 1, 781   }, { 1, 782   }, { 1, 785   }, { 1, 786   },
   { 1, 787   }, { 1, 788   }, { 1, 810   }, { 1, 820   }, { 1, 821   }, { 1, 822   }, { 1, 823   }, { 1, 824   },
   { 1, 825   }, { 1, 826   }, { 1, 827   }, { 1, 828   }, { 1, 829   }, { 1, 830   }, { 1, 831   }, { 1, 832   },
   { 1, 833   }, { 1, 834   }, { 1, 835   }, { 1, 836   }, { 1, 837   }, { 1, 838   }, { 1, 858   }, { 1, 863   },
   { 1, 894   }, { 1, 895   }, { 1, 896   }, { 1, 897   }, { 1, 898   }, { 1, 899   }, { 1, 900   }, { 1, 901   },
   { 1, 902   }, { 1, 903   }, { 1, 904   }, { 1, 905   }, { 1, 906   }, { 1, 907   }, { 1, 908   }, { 1, 909   },
   { 1, 910   }, { 1, 911   }, { 1, 912   }, { 1, 957   }, { 1, 1135  }, { 1, 1136  }, { 1, 1139  }, { 1, 1140  },
   { 1, 1142  }, { 1, 1159  }, { 1, 1160  }, { 1, 1161  }, { 1, 1162  }, { 1, 1164  }, { 1, 1166  }, { 1, 1168  },
   { 1, 1173  }, { 1, 1175  }, { 1, 1177  }, { 1, 1179  }, { 1, 1180  }, { 1, 1182  }, { 1, 1185  }, { 1, 1211  },
   { 1, 1212  }, { 1, 1214  }, { 1, 1216  }, { 1, 1233  }, { 1, 1234  }, { 1, 1235  }, { 1, 1236  }, { 1, 1237  },
   { 1, 1238  }, { 1, 1239  }, { 1, 1245  }, { 1, 1253  }, { 1, 1254  }, { 1, 1255  }, { 1, 1256  }, { 1, 1263  },
   { 1, 1264  }, { 1, 1266  }, { 1, 1270  }, { 1, 1274  }, { 1, 1276  }, { 1, 1279  }, { 1, 1280  }, { 1, 1284  },
   { 1, 1286  }, { 1, 1287  }, { 1, 1288  }, { 1, 1289  }, { 1, 1290  }, { 1, 1299  }, { 1, 1300  }, { 1, 1301  },
   { 1, 1302  }, { 1, 1303  }, { 1, 1305  }, { 1, 1306  }, { 1, 1308  }, { 1, 1322  }, { 1, 1326  }, { 1, 1337  },
   { 1, 1349  }, { 1, 1350  }, { 1, 1351  }, { 1, 1352  }, { 1, 1353  }, { 1, 1355  }, { 1, 1356  }, { 1, 1361  },
   { 1, 1363  }, { 1, 1377  }, { 1, 1378  }, { 1, 1382  }, { 1, 1385  }, { 1, 1386  }, { 1, 1388  }, { 1, 1390  },
   { 1, 1398  }, { 1, 1404  }, { 1, 1434  }, { 1, 1435  }, { 1, 1436  }, { 1, 1437  }, { 1, 1438  }, { 1, 1447  },
   { 1, 1448  }, { 1, 1449  }, { 1, 1450  }, { 1, 1451  }, { 1, 1452  }, { 1, 1453  }, { 1, 1454  }, { 1, 1456  },
   { 1, 1483  }, { 1, 1504  }, { 1, 1505  }, { 1, 1508  }, { 1, 1509  }, { 1, 1511  }, { 1, 1512  }, { 1, 1521  },
   { 1, 1522  }, { 1, 1523  }, { 1, 1524  }, { 1, 1525  }, { 1, 1526  }, { 1, 1527  }, { 1, 1539  }, { 1, 1566  },
   { 1, 1567  }, { 1, 1577  }, { 1, 1721  }, { 1, 1725  }, { 1, 1727  }, { 1, 1728  }, { 1, 1729  }, { 1, 1730  },
   { 1, 1731  }, { 1, 1732  }, { 1, 1733  }, { 1, 1734  }, { 1, 1743  }, { 1, 1744  }, { 1, 1745  }, { 1, 1746  },
   { 1, 1747  }, { 1, 1748  }, { 1, 1749  }, { 1, 1750  }, { 1, 1751  }, { 1, 1753  }, { 1, 1755  }, { 1, 1795  },
   { 1, 1801  }, { 1, 1802  }, { 1, 1803  }, { 1, 1804  }, { 1, 1805  }, { 1, 1806  }, { 1, 1807  }, { 1, 1808  },
   { 1, 1817  }, { 1, 1818  }, { 1, 1819  }, { 1, 1820  }, { 1, 1821  }, { 1, 1822  }, { 1, 1823  }, { 1, 1824  },
   { 1, 1826  }, { 1, 1832  }, { 1, 1836  }, { 1, 1840  }, { 1, 1872  }, { 1, 1875  }, { 1, 1877  }, { 1, 1878  },
   { 1, 1881  }, { 1, 1882  }, { 1, 1891  }, { 1, 1894  }, { 1, 1897  }, { 1, 1898  }, { 1, 1900  }, { 1, 1902  },
   { 1, 1909  }, { 1, 1913  }, { 1, 1951  }, { 1, 1954  }, { 1, 1955  }, { 1, 1956  }, { 1, 1965  }, { 1, 1968  },
   { 1, 1969  }, { 1, 1975  }, { 1, 1982  }, { 1, 1983  }, { 1, 1985  }, { 1, 1987  }, { 2, 1431  }, { 2, 1432  },
   { 2, 1433  }, { 2, 1434  }, { 2, 1435  }, { 2, 1436  }, { 2, 1437  }, { 2, 1438  }, { 2, 1505  }, { 2, 1506  },
   { 2, 1507  }, { 2, 1508  }, { 2, 1509  }, { 2, 1510  }, { 2, 1511  }, { 2, 1512  }, { 2, 1855  }, { 2, 1856  },
   { 2, 1857  }, { 2, 1858  }, { 2, 1859  }, { 2, 1860  }, { 2, 1861  }, { 2, 1862  }, { 2, 1863  }, { 2, 1864  },
   { 2, 1865  }, { 2, 1866  }, { 2, 1867  }, { 2, 1868  }, { 2, 1869  }, { 2, 1870  }, { 2, 1871  }, { 2, 1872  },
   { 2, 1873  }, { 2, 1874  }, { 2, 1875  }, { 2, 1876  }, { 2, 1877  }, { 2, 1878  }, { 2, 1879  }, { 2, 1880  },
   { 2, 1881  }, { 2, 1882  }, { 2, 1928  }, { 2, 1929  }, { 2, 1930  }, { 2, 1931  }, { 2, 1932  }, { 2, 1933  },
   { 2, 1934  }, { 2, 1935  }, { 2, 1936  }, { 2, 1937  }, { 2, 1938  }, { 2, 1939  }, { 2, 1940  }, { 2, 1941  },
   { 2, 1942  }, { 2, 1943  }, { 2, 1944  }, { 2, 1945  }, { 2, 1946  }, { 2, 1947  }, { 2, 1948  }, { 2, 1949  },
   { 2, 1950  }, { 2, 1951  }, { 2, 1952  }, { 2, 1953  }, { 2, 1954  }, { 2, 1955  }, { 2, 1956  }, { 2, 2005  },
   { 2, 2006  }, { 2, 2007  }, { 2, 2008  }, { 2, 2009  }, { 2, 2010  }, { 2, 2011  }, { 2, 2012  }, { 2, 2013  },
   { 2, 2014  }, { 2, 2015  }, { 2, 2016  }, { 2, 2017  }, { 2, 2019  }, { 2, 2020  }, { 2, 2021  }, { 2, 2022  },
   { 2, 2023  }, { 2, 2024  }, { 2, 2025  }, { 2, 2026  }, { 2, 2027  }, { 2, 2028  }, { 2, 2029  }, { 2, 2030  },
   { 2, 2039  }, { 2, 2040  }, { 2, 2041  }, { 2, 2042  }, { 2, 2043  }, { 2, 2044  }, { 2, 2045  }, { 2, 2046  },
   { 2, 2078  }, { 2, 2079  }, { 2, 2080  }, { 2, 2081  }, { 2, 2082  }, { 2, 2083  }, { 2, 2084  }, { 2, 2085  },
   { 2, 2086  }, { 2, 2087  }, { 2, 2088  }, { 2, 2089  }, { 2, 2090  }, { 2, 2091  }, { 2, 2092  }, { 2, 2093  },
   { 2, 2094  }, { 2, 2095  }, { 2, 2096  }, { 2, 2097  }, { 2, 2098  }, { 2, 2099  }, { 2, 2100  }, { 2, 2101  },
   { 2, 2102  }, { 2, 2103  }, { 2, 2104  }, { 2, 2113  }, { 2, 2114  }, { 2, 2115  }, { 2, 2116  }, { 2, 2117  },
   { 2, 2118  }, { 2, 2119  }, { 2, 2120  }, { 3, 123   }, { 3, 140   }, { 3, 142   }, { 3, 144   }, { 3, 145   },
   { 3, 146   }, { 3, 196   }, { 3, 214   }, { 3, 216   }, { 3, 218   }, { 3, 220   }, { 3, 289   }, { 3, 290   },
   { 3, 292   }, { 3, 340   }, { 3, 361   }, { 3, 363   }, { 3, 5173  }, { 3, 5174  }, { 3, 5175  }, { 3, 5176  },
   { 3, 5177  }, { 3, 5221  }, { 3, 5246  }, { 3, 5251  }, { 3, 5319  }, { 3, 5321  }, { 3, 5381  }, { 3, 5393  },
   { 3, 5396  }, { 3, 5399  }, { 3, 5400  }, { 3, 5443  }, { 3, 5444  }, { 3, 5445  }, { 3, 5446  }, { 3, 5447  },
   { 3, 5448  }, { 3, 5449  }, { 3, 5450  }, { 3, 5451  }, { 3, 5452  }, { 3, 5454  }, { 3, 5455  }, { 3, 5457  },
   { 3, 5459  }, { 3, 5460  }, { 3, 5462  }, { 3, 5463  }, { 3, 5468  }, { 3, 5469  }, { 3, 5471  }, { 3, 5522  },
   { 3, 5524  }, { 3, 5542  }, { 3, 5543  }, { 3, 5545  }, { 3, 6011  }, { 3, 6012  }, { 3, 6013  }, { 3, 6014  },
   { 3, 6015  }, { 3, 6016  }, { 3, 6017  }, { 3, 6018  }, { 3, 6085  }, { 3, 6086  }, { 3, 6087  }, { 3, 6088  },
   { 3, 6089  }, { 3, 6090  }, { 3, 6091  }, { 3, 6092  }, { 4, 118   }, { 4, 123   }, { 4, 124   }, { 4, 125   },
   { 4, 126   }, { 4, 127   }, { 4, 128   }, { 4, 129   }, { 4, 130   }, { 4, 189   }, { 4, 190   }, { 4, 194   },
   { 4, 197   }, { 4, 198   }, { 4, 199   }, { 4, 200   }, { 4, 201   }, { 4, 202   }, { 4, 203   }, { 4, 204   },
   { 4, 270   }, { 4, 339   }, { 4, 342   }, { 4, 358   }, { 4, 412   }, { 4, 414   }, { 4, 415   }, { 4, 416   },
   { 4, 418   }, { 4, 419   }, { 4, 420   }, { 4, 423   }, { 4, 427   }, { 4, 431   }, { 4, 433   }, { 4, 486   },
   { 4, 490   }, { 4, 493   }, { 4, 501   }, { 4, 559   }, { 4, 560   }, { 4, 561   }, { 4, 562   }, { 4, 563   },
   { 4, 564   }, { 4, 565   }, { 4, 566   }, { 4, 567   }, { 4, 568   }, { 4, 569   }, { 4, 570   }, { 4, 571   },
   { 4, 574   }, { 4, 577   }, { 4, 578   }, { 4, 579   }, { 4, 582   }, { 4, 583   }, { 4, 584   }, { 4, 633   },
   { 4, 634   }, { 4, 636   }, { 4, 637   }, { 4, 639   }, { 4, 640   }, { 4, 641   }, { 4, 642   }, { 4, 643   },
   { 4, 644   }, { 4, 645   }, { 4, 646   }, { 4, 653   }, { 4, 655   }, { 4, 4629  }, { 4, 7367  }, { 4, 7368  },
   { 4, 7369  }, { 4, 7372  }, { 4, 7373  }, { 4, 7441  }, { 4, 7442  }, { 4, 7443  }, { 4, 7444  }, { 4, 7448  },
   { 4, 7451  }, { 4, 7453  }, { 4, 7454  }, { 4, 7457  }, { 4, 7461  }, { 4, 7515  }, { 4, 7516  }, { 4, 7519  },
   { 4, 7520  }, { 4, 7521  }, { 4, 7527  }, { 4, 7529  }, { 4, 7540  }, { 4, 7589  }, { 4, 7591  }, { 4, 7592  },
   { 4, 7594  }, { 4, 7596  }, { 4, 7606  }, { 4, 7610  }, { 4, 7663  }, { 4, 7664  }, { 4, 7665  }, { 4, 7666  },
   { 4, 7667  }, { 4, 7668  }, { 4, 7670  }, { 4, 7737  }, { 4, 7739  }, { 4, 7743  }, { 4, 7746  }, { 4, 7757  },
   { 4, 7811  }, { 4, 7812  }, { 4, 7813  }, { 4, 7816  }, { 4, 7823  }, { 4, 7826  }, { 4, 7885  }, { 4, 7886  },
   { 4, 7888  }, { 4, 7889  }, { 4, 7907  }};


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

// void EfficiencyPlotsModule::downscaleClusterSizes() {}

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
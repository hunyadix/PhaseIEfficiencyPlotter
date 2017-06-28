#include "interface/DataStructures_v5.h"

void rocNumberGeometryMapGenerator()
{
   std::cout << "rocNumberGeometryMapGenerator started..." << std::endl;
   TChain* chain = new TChain("trajTree", "Trajectory informations");
   std::vector<std::string> inputFiles = 
   {
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_10.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_100.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_101.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_102.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_103.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_104.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_105.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_106.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_107.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_108.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_109.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_11.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_110.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_111.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_112.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_113.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_114.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_115.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_116.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_117.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_118.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_119.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_12.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_120.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_121.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_122.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_123.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_124.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_125.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_126.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_127.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_128.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_129.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_13.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_130.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_131.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_132.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_133.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_134.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_135.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_136.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_137.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_138.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_139.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_14.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_140.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_141.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_142.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_143.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_144.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_145.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_146.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_147.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_148.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_149.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_15.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_150.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_151.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_152.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_153.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_154.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_155.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_156.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_157.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_158.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_159.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_16.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_160.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_161.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_162.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_163.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_164.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_165.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_166.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_167.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_168.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_169.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_17.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_170.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_171.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_172.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_173.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_174.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_175.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_176.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_177.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_178.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_179.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_18.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_180.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_181.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_182.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_183.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_184.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_185.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_186.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_187.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_188.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_189.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_19.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_190.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_191.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_192.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_193.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_194.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_195.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_196.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_197.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_198.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_199.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_2.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_20.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_200.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_201.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_202.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_203.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_204.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_21.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_22.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_23.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_24.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_25.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_26.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_27.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_28.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_29.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_3.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_30.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_31.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_32.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_33.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_34.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_35.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_36.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_37.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_38.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_39.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_4.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_40.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_41.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_42.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_43.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_44.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_45.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_46.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_47.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_48.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_49.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_5.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_50.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_51.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_52.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_53.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_54.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_55.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_56.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_57.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_58.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_59.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_6.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_60.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_61.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_62.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_63.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_64.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_65.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_66.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_67.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_68.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_69.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_7.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_70.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_71.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_72.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_73.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_74.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_75.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_76.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_77.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_78.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_79.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_8.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_80.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_81.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_82.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_83.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_84.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_85.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_86.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_87.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_88.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_89.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_9.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_90.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_91.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_92.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_93.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_94.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_95.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_96.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_97.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_98.root",
      "/data/gridout/ahunyadi/PhaseIPixelNtuplizer/Scans/v0405_default_921_92X_dataRun2_Express_v2_TimingScan2017_Scan20/170613_173906/0000/Ntuple_99.root"
   };
   std::for_each(inputFiles.begin(), inputFiles.end(), [&] (const std::string& fileName) { chain  -> Add(fileName.c_str()); });
   ModuleData mod_on;
   chain -> SetBranchAddress("mod_on", &mod_on);
   // TBranch* branch_mod_on = chain -> GetBranch("mod_on");

   // ---------------------------------------------------------------------------------------------
   // Generate ROC location identifier objects

   // - Required minimum data for roc indecies:
   //      - layer (should it be implicit?)
   //      - module (half shell should be deducable using this)
   //      - ladder
   //      - sector
   //      - side
   //      - disk
   //      - ring
   //      - blade
   //      - panel

   struct ROCPlacement
   {
      int layer  = NOVAL_I; // Adding layer info explicitly
      int module = NOVAL_I;
      int ladder = NOVAL_I;
      int sec    = NOVAL_I;
      int side   = NOVAL_I;
      int disk   = NOVAL_I;
      int blade  = NOVAL_I;
      int panel  = NOVAL_I;
      int ring   = NOVAL_I;
   };
   const std::vector<TH2D*>  binMaps = 
   { 
      new TH2D("binmap_fwd",    "binmap_fwd",    112, -3.5, 3.5, 140, -17.5, 17.5),
      new TH2D("binmap_layer1", "binmap_layer1",  72, -4.5, 4.5,  26,  -6.5,  6.5),
      new TH2D("binmap_layer2", "binmap_layer2",  72, -4.5, 4.5,  58, -14.5, 14.5),
      new TH2D("binmap_layer3", "binmap_layer3",  72, -4.5, 4.5,  90, -22.5, 22.5),
      new TH2D("binmap_layer4", "binmap_layer4",  72, -4.5, 4.5, 130, -32.5, 32.5)
   };
   std::vector<std::map<int, ROCPlacement>> foundROCs(5);
   std::cout << "entry: " << std::setw(12) << 0 << " out of " << std::setw(12) << chain -> GetEntries() << std::string(32, '\b');
   auto updateMsg = [] (Long64_t numEntry)
   {
      std::cout << std::setw(12) << numEntry << std::string(12, '\b');
   };
   for(Long64_t numEntry = 0, maxNumEntries = chain -> GetEntries(); numEntry < maxNumEntries; ++numEntry)
   {
      chain -> GetEntry(numEntry);
      const int   layer    = mod_on.det == 1 ? 0 : mod_on.layer;
      if(layer < 0 || 4 < layer)
      {
         std::cout << "Error at: " << std::endl;
         std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
         continue;
      }
      const float binXKey  = mod_on.det ? mod_on.disk_ring_coord   : mod_on.module_coord;
      const float binYKey  = mod_on.det ? mod_on.blade_panel_coord : mod_on.ladder_coord;
      int rocIndex = binMaps[layer] -> FindFixBin(binXKey, binYKey);
      ROCPlacement rocInfo 
      {
         mod_on.layer, mod_on.module, mod_on.ladder,
         mod_on.sec,   mod_on.side,   mod_on.disk,
         mod_on.blade, mod_on.panel,  mod_on.ring
      };
      foundROCs[layer].insert(std::make_pair(rocIndex, std::move(rocInfo)));
      updateMsg(numEntry);
   }
   std::cout << std::endl;
   std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
   std::ofstream rocPlacementFile("ROCPlacement_extra.txt");
   for(size_t layer = 0; layer < 5; ++layer)
   {
      for(size_t rocIndex = 0, maxROCIndex = binMaps[layer] -> GetSize(); rocIndex < maxROCIndex; ++rocIndex)
      {
         if(foundROCs[layer].find(rocIndex) != foundROCs[layer].end())
         {
            rocPlacementFile << 
               "{{" 
                  << layer << ", " << rocIndex << "}, " << 
               "{" 
                  << foundROCs[layer][rocIndex].layer  << ", "
                  << foundROCs[layer][rocIndex].module << ", "
                  << foundROCs[layer][rocIndex].ladder << ", "
                  << foundROCs[layer][rocIndex].sec    << ", "
                  << foundROCs[layer][rocIndex].side   << ", "
                  << foundROCs[layer][rocIndex].disk   << ", "
                  << foundROCs[layer][rocIndex].blade  << ", "
                  << foundROCs[layer][rocIndex].panel  << ", "
                  << foundROCs[layer][rocIndex].ring   <<
               "}}," << std::endl;
         }
      }
   }
   std::cout << "rocNumberGeometryMapGenerator terminated." << std::endl;   
}
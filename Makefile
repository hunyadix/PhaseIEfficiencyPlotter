include Makefile.arch
SrcSuf = cc

CXX       = ${HOME}//usr/bin/g++
CXXFLAGS  = -std=c++14 -O3 -Wall -fPIC -pthread -m64 -I/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_9_1_0_pre3/external/slc6_amd64_gcc530/bin/../../../../../../../slc6_amd64_gcc530/lcg/root/6.08.06-mlhled2/include -fdiagnostics-color=always
LIBS      =  -L/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_9_1_0_pre3/external/slc6_amd64_gcc530/bin/../../../../../../../slc6_amd64_gcc530/lcg/root/6.08.06-mlhled2/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic 
LIBS     += -lboost_system -lstdc++fs

# CXXFLAGS += -std=c++1z -fdiagnostics-color=always

# Utility

CONSOLECOLORS_S = ./src/ConsoleColors.$(SrcSuf)
CONSOLECOLORS_O = ./obj/ConsoleColors.$(ObjSuf)
OBJS     += $(CONSOLECOLORS_O)

CONSOLEACTOR_S = ./src/ConsoleActor.$(SrcSuf)
CONSOLEACTOR_O = ./obj/ConsoleActor.$(ObjSuf)
OBJS     += $(CONSOLEACTOR_O)

COMMONACTORS_S = ./src/CommonActors.$(SrcSuf)
COMMONACTORS_O = ./obj/CommonActors.$(ObjSuf)
OBJS     += $(COMMONACTORS_O)

TIMER_S = ./src/Timer.$(SrcSuf)
TIMER_O = ./obj/Timer.$(ObjSuf)
OBJS     += $(TIMER_O)

TIMERCOL_S = ./src/TimerColored.$(SrcSuf)
TIMERCOL_O = ./obj/TimerColored.$(ObjSuf)
OBJS     += $(TIMERCOL_O)

TTREETOOLS_S = ./src/TTreeTools.$(SrcSuf)
TTREETOOLS_O = ./obj/TTreeTools.$(ObjSuf)
OBJS     += $(TTREETOOLS_O)

CANVASEXTRAS_S = ./src/CanvasExtras.$(SrcSuf)
CANVASEXTRAS_O = ./obj/CanvasExtras.$(ObjSuf)
OBJS     += $(CANVASEXTRAS_O)

EFFICIENCY_PLOTS_MODULE_S = ./src/EfficiencyPlotsModule.$(SrcSuf)
EFFICIENCY_PLOTS_MODULE_O = ./obj/EfficiencyPlotsModule.$(ObjSuf)
OBJS     += $(EFFICIENCY_PLOTS_MODULE_O)

# PROGRAMS

EFFICIENCY_MAIN_S = ./src/programs/efficiencyMain.$(SrcSuf)
EFFICIENCY_MAIN_O = ./obj/efficiencyMain.$(ObjSuf)
EFFICIENCY_MAIN_A = ./bin/efficiencyMain$(ExeSuf)
OBJS     += $(EFFICIENCY_MAIN_O)
PROGRAMS += $(EFFICIENCY_MAIN_A)

EFFICIENCY_TIMING_S = ./src/programs/efficiencyTiming.$(SrcSuf)
EFFICIENCY_TIMING_O = ./obj/efficiencyTiming.$(ObjSuf)
EFFICIENCY_TIMING_A = ./bin/efficiencyTiming$(ExeSuf)
OBJS     += $(EFFICIENCY_TIMING_O)
PROGRAMS += $(EFFICIENCY_TIMING_A)

ROC_EFFICIENCY_FITTER_S = ./src/programs/rocEfficiencyFitter.$(SrcSuf)
ROC_EFFICIENCY_FITTER_O = ./obj/rocEfficiencyFitter.$(ObjSuf)
ROC_EFFICIENCY_FITTER_A = ./bin/rocEfficiencyFitter$(ExeSuf)
OBJS     += $(ROC_EFFICIENCY_FITTER_O)
PROGRAMS += $(ROC_EFFICIENCY_FITTER_A)

all: $(PROGRAMS)

# Executables

$(EFFICIENCY_MAIN_A): $(EFFICIENCY_MAIN_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(EFFICIENCY_PLOTS_MODULE_O)
	@printf "Compiling done, linking \""$@"\"...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(EFFICIENCY_TIMING_A): $(EFFICIENCY_TIMING_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(EFFICIENCY_PLOTS_MODULE_O)
	@printf "Compiling done, linking \""$@"\"...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(ROC_EFFICIENCY_FITTER_A): $(ROC_EFFICIENCY_FITTER_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(EFFICIENCY_PLOTS_MODULE_O)
	@printf "Compiling done, linking \""$@"\"...\n"
	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

# $(EFFICIENCY_MULTIHITMEAS_A): $(EFFICIENCY_MULTIHITMEAS_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O)
# 	@printf "Compiling done, linking efficiency_multiHitMeas...\n"
# 	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
# 	$(MT_EXE)
# 	@echo "Succesful make..."
# 	@echo "...$@ is ready to use."

# $(MERGE_SIMHIT_PLOTS_A): $(MERGE_SIMHIT_PLOTS_O) $(CONSOLECOLORS_O) $(CONSOLEACTOR_O) $(COMMONACTORS_O) $(TIMER_O) $(TIMERCOL_O) $(TTREETOOLS_O) $(CANVASEXTRAS_O) $(EFFICIENCY_PLOTS_MODULE_O)
# 	@printf "Compiling done, linking mergeSimhitPlots...\n"
# 	@$(LD) $(LDFLAGS) -Wall -Wshadow $^ $(LIBS) $(OutPutOpt)$@
# 	$(MT_EXE)
# 	@echo "Succesful make..."
# 	@echo "...$@ is ready to use."

# Utility obj files

$(CONSOLECOLORS_O): $(CONSOLECOLORS_S)
	@printf "Compiling utility: \"ConsoleColors\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(CONSOLEACTOR_O): $(CONSOLEACTOR_S)
	@printf "Compiling utility: \"ConsoleActor\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(COMMONACTORS_O): $(COMMONACTORS_S)
	@printf "Compiling utility: \"CommonActors\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(TIMER_O): $(TIMER_S)
	@printf "Compiling utility: \"TimerColored\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(TIMERCOL_O): $(TIMERCOL_S)
	@printf "Compiling utility: \"TimerColored\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(TTREETOOLS_O): $(TTREETOOLS_S)
	@printf "Compiling utility: \"TTreeTools\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(CANVASEXTRAS_O): $(CANVASEXTRAS_S)
	@printf "Compiling utility: \"CanvasExtras\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(EFFICIENCY_PLOTS_MODULE_O): $(EFFICIENCY_PLOTS_MODULE_S)
	@printf "Compiling utility: \"EfficiencyPlotsModule\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

###

$(EFFICIENCY_MAIN_O): $(EFFICIENCY_MAIN_S)  
	@printf "Compiling program: \""$<"\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(EFFICIENCY_TIMING_O): $(EFFICIENCY_TIMING_S)  
	@printf "Compiling program: \""$<"\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

$(ROC_EFFICIENCY_FITTER_O): $(ROC_EFFICIENCY_FITTER_S)  
	@printf "Compiling program: \""$<"\"...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Done.\n"

# $(EFFICIENCY_MULTIHITMEAS_O): $(EFFICIENCY_MULTIHITMEAS_S)  
# 	@printf "Compiling program: \"efficiency_multiHitMeas\"...\n"
# 	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
# 	@printf "Done.\n"

# $(MERGE_SIMHIT_PLOTS_O): $(MERGE_SIMHIT_PLOTS_S)
# 	@printf "Compiling program: \"mergeSimhitPlots\"...\n"
# 	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
# 	@printf "Done.\n"

print_compiler_info:
	@printf "Compiler: $(CXX)\n"
	@printf "Compiler flags: $(CXXFLAGS)\n"
	@printf "Linked libs: $(LIBS)\n"
	@printf "Linker: $(LD)\n"
	@printf "Linker flags: $(LDFLAGS)\n"

clean:
	@rm -f $(OBJS) $(PROGRAMS) core

distclean: clean
	@rm -f $(PROGRAMS) *Dict.* *.def *.exp \
	*.root *.ps *.so *.lib *.dll *.d *.log .def so_locations
	@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)

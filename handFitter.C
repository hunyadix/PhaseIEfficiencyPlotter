#include "interface/DataStructures_v5.h"
TRandom3 rndGen;

enum class FitStatus
{
    NotCalculated = 0,
    Approximated,
    MadePosDef,
    Accurate
};

void handFitter()
{
    TFile* inputFile = TFile::Open("/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/histograms_fill5824_small_1_batch.root", "READ");
    inputFile -> cd("Efficiency_vs_delay");
    // for(auto key: *(gDirectory -> GetListOfKeys())) std::cout << key -> GetName() << std::endl;
    TH1D* efficiencyVsDelayOnBPix = dynamic_cast<TH1D*>(gDirectory -> Get("delayVsEfficiencyBPix"));
    TH1D* efficiencyVsDelayOnFPix = dynamic_cast<TH1D*>(gDirectory -> Get("delayVsEfficiencyFPix"));
    std::chrono::milliseconds milliseconds_100(100);
    auto fitLooper = [&] (std::chrono::duration<double> secondsToTimeout, TH1D* histogramToFit)
    {
        using clock_t = std::chrono::steady_clock;
        TF1* parametrizedSigmoid = new TF1("parametrizedSigmoid", "[0] / ((1 + exp([1] + x) / [2]))", -10, 30);
        clock_t::time_point startTime = clock_t::now();
        while(1)
        {
            parametrizedSigmoid -> SetParameter(0, rndGen.Rndm() * 2 + 0.01);
            parametrizedSigmoid -> SetParameter(1, rndGen.Rndm() * 40 - 10);
            parametrizedSigmoid -> SetParameter(2, rndGen.Rndm() * 10 + 0.01);
            TFitResultPtr fitResults = histogramToFit -> Fit("parametrizedSigmoid", "NMRS");
            FitStatus status = FitStatus(fitResults -> CovMatrixStatus());
            if(status == FitStatus::MadePosDef)
            {
                std::cout << "Cov. matrix is pos. def. but inaccurate." << std::endl;
            }
            if(status == FitStatus::Accurate)
            {
                std::cout << "Cov. matrix is accurate." << std::endl;
                histogramToFit -> Draw();
                parametrizedSigmoid -> Draw("SAME");
                return true;
            }
            if(clock_t::now() - startTime > secondsToTimeout)
            {
                return false;
            }
        }
    };
    std::vector<std::function<bool()>> fitProcessList 
    {
        [&] { return fitLooper(std::chrono::duration<double>(2.0), efficiencyVsDelayOnBPix); },
        [&] { return fitLooper(std::chrono::duration<double>(2.0), efficiencyVsDelayOnFPix); }
    };
    for(const auto& fitter: fitProcessList)
    {
        static int canvasCounter = 0;
        TCanvas* canvas = new TCanvas(std::to_string(canvasCounter).c_str(), "", 800, 600);
        canvasCounter++;
        canvas -> cd();
        if(!fitter())
        {
            std::cout << "Fitting did not succeed in the given time interval." << std::endl;
        }
    }
}

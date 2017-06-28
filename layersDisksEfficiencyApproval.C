#include "interface/DataStructures_v5.h"
#include "interface/common_functions_jkarancs.h"
#include "interface/WilsonScoreInterval.h"
TRandom3 rndGen;

TGraphAsymmErrors* getEfficiencyGraphAsymmErrors(const TH1& efficiencyHistogram, const TH1& numHitsHistogram, const int& markerColor, const int& markerStyle)
{
   const TAxis* xAxis = efficiencyHistogram.GetXaxis();
   const TAxis* yAxis = efficiencyHistogram.GetYaxis();
   const int numBins = xAxis -> GetNbins();
   std::vector<Double_t> valuesX;
   std::vector<Double_t> valuesY;
   // std::vector<Double_t> errorsXLow (numBins, 0.5 * xAxis -> GetBinWidth(xAxis -> GetFirst()));
   // std::vector<Double_t> errorsXHigh(numBins, 0.5 * xAxis -> GetBinWidth(xAxis -> GetFirst()));
   std::vector<Double_t> errorsXLow (numBins, 0.0);
   std::vector<Double_t> errorsXHigh(numBins, 0.0);
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
   graph -> SetMarkerSize(1.0);
   return graph;
   // const_cast<TH1D*>(&efficiencyHistogram) -> Draw("HIST");
}

void layersDisksEfficiencyApproval()
{
    TFile* inputFile = TFile::Open("/data/hunyadi/CMSSW/PhaseIEfficiencyPlotter_2017_04_02/CMSSW_9_1_0_pre3/src/DPGAnalysis/PhaseIEfficiencyPlotter/histograms_fill5824.root", "READ");
    inputFile  -> cd("Delay_6.00");
    gDirectory -> cd("layersDisksEfficiencyPlots");
    TH1D* layersDiskEfficiency = dynamic_cast<TH1D*>(gDirectory -> Get("layersDisksEfficiencyPlots_Eff."));
    TH1D* layersDiskHits       = dynamic_cast<TH1D*>(gDirectory -> Get("layersDisksEfficiencyPlots_Hits"));
    TGraphAsymmErrors* graph = getEfficiencyGraphAsymmErrors(*layersDiskEfficiency, *layersDiskHits, 4, 34);
    gStyle -> SetOptStat(0);
    std::array<std::string, 7> layersDiskLabels
    {{
        "Layer 1",
        "Layer 2",
        "Layer 3",
        "Layer 4",
        "Disk 1",
        "Disk 2",
        "Disk 3"
    }};
    layersDiskEfficiency -> SetTitle("Efficiency - run 296664");
    graph -> SetTitle("Efficiency - run 296664");
    graph -> GetYaxis() -> SetRangeUser(0.52, 1.02);
    graph -> GetYaxis() -> SetNdivisions(-510);
    graph -> GetYaxis() -> SetTitleOffset(1.7);
    for(unsigned int numBin = 0; numBin < layersDiskLabels.size(); ++numBin)
    {
        layersDiskEfficiency -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, layersDiskLabels[numBin].c_str());
        graph -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, layersDiskLabels[numBin].c_str());
    }
    TCanvas* canvas = custom_can_(layersDiskEfficiency, "Efficiency on layers and disks - run 296664");
    canvas -> SetTopMargin(0.12);
    layersDiskEfficiency -> SetMarkerStyle(34);
    graph -> Draw("AP0");
    add_labels(layersDiskEfficiency);
    std::string waitingForApprovalText = "#scale[0.7]{#color[2]{#font[52]{[Waiting for approval]}}}";
    TLatex* waitingForApprovalLabel = new TLatex(0.55, 1.0745, waitingForApprovalText.c_str()); 
    waitingForApprovalLabel -> SetLineWidth(2); 
    waitingForApprovalLabel -> Draw();
}

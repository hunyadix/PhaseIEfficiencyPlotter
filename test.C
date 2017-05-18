{
//========= Macro generated from object: delayVsEfficiencyBPix/Delay vs efficiency on BPix
//========= by ROOT version6.08/07
   
   TH1D *delayVsEfficiencyBPix__1 = new TH1D("delayVsEfficiencyBPix__1","Delay vs efficiency on BPix",250,158,168);
   delayVsEfficiencyBPix__1->SetBinContent(0,0.960162);
   delayVsEfficiencyBPix__1->SetBinError(0,0.960162);
   delayVsEfficiencyBPix__1->SetEntries(1);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.575,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *AText = ptstats->AddText("delayVsEfficiencyBPix");
   AText->SetTextSize(0.0473143);
   AText = ptstats->AddText("Entries = 1      ");
   AText = ptstats->AddText("Mean  =      0 #pm      0");
   AText = ptstats->AddText("Std Dev   =      0 #pm      0");
   AText = ptstats->AddText("Underflow = 0.9602");
   AText = ptstats->AddText("Overflow  =      0");
   AText = ptstats->AddText("Integral =      0");
   ptstats->SetOptStat(1112211);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   delayVsEfficiencyBPix__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(delayVsEfficiencyBPix__1);
   delayVsEfficiencyBPix__1->SetFillColor(38);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   delayVsEfficiencyBPix__1->SetLineColor(ci);
   delayVsEfficiencyBPix__1->GetXaxis()->SetTitle("delay");
   delayVsEfficiencyBPix__1->GetXaxis()->SetNdivisions(-510);
   delayVsEfficiencyBPix__1->GetXaxis()->SetLabelFont(42);
   delayVsEfficiencyBPix__1->GetXaxis()->SetLabelSize(0.0222222);
   delayVsEfficiencyBPix__1->GetXaxis()->SetTitleSize(0.0355556);
   delayVsEfficiencyBPix__1->GetXaxis()->SetTitleOffset(0.78125);
   delayVsEfficiencyBPix__1->GetXaxis()->SetTitleFont(42);
   delayVsEfficiencyBPix__1->GetYaxis()->SetTitle("efficiency");
   delayVsEfficiencyBPix__1->GetYaxis()->SetDecimals();
   delayVsEfficiencyBPix__1->GetYaxis()->SetLabelFont(42);
   delayVsEfficiencyBPix__1->GetYaxis()->SetLabelSize(0.0222222);
   delayVsEfficiencyBPix__1->GetYaxis()->SetTitleSize(0.0355556);
   delayVsEfficiencyBPix__1->GetYaxis()->SetTitleOffset(1.03401);
   delayVsEfficiencyBPix__1->GetYaxis()->SetTitleFont(42);
   delayVsEfficiencyBPix__1->GetZaxis()->SetDecimals();
   delayVsEfficiencyBPix__1->GetZaxis()->SetLabelFont(42);
   delayVsEfficiencyBPix__1->GetZaxis()->SetLabelSize(0.0222222);
   delayVsEfficiencyBPix__1->GetZaxis()->SetTitleSize(0.0355556);
   delayVsEfficiencyBPix__1->GetZaxis()->SetTitleOffset(2.06801);
   delayVsEfficiencyBPix__1->GetZaxis()->SetTitleFont(42);
   delayVsEfficiencyBPix__1->Draw("");
}

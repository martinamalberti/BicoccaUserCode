{
 gStyle->SetOptStat(0);
 gStyle->SetOptTitle(0);
 TFile file1 ("validazione_TTbar_3_3_3.root","READ");
 TH1F EoPOld = (TH1F) file1.Get("EoP");
 TH1F EoPZOld= (TH1F) file1.Get("EoPZ");
 TH1F etaDistOld = (TH1F) file1.Get("etaDist");
 TH1F energyOld = (TH1F) file1.Get("energy");
 TH1F ESCoPOld = (TH1F) file1.Get("ESCoP");
 TH1F HitsOeOld = (TH1F) file1.Get("HitsOe");

 TFile file2 ("validazione_TTbar_3_3_0.root","READ");
 TH1F EoPNew = (TH1F) file2.Get("EoP");
 TH1F EoPZNew= (TH1F) file2.Get("EoPZ");
 TH1F etaDistNew = (TH1F) file2.Get("etaDist");
 TH1F energyNew = (TH1F) file2.Get("energy");
 TH1F ESCoPNew = (TH1F) file2.Get("ESCoP");
 TH1F HitsOeNew = (TH1F) file2.Get("HitsOe");

 EoPOld.SetTitle("CMSSW_3_3_3");
 EoPZOld.SetTitle("CMSSW_3_3_3");
 etaDistOld.SetTitle("CMSSW_3_3_3");
 energyOld.SetTitle("CMSSW_3_3_3");
 ESCoPOld.SetTitle("CMSSW_3_3_3");
 HitsOeOld.SetTitle("CMSSW_3_3_3");

 EoPNew.SetTitle("CMSSW_3_3_0");
 EoPZNew.SetTitle("CMSSW_3_3_0");
 etaDistNew.SetTitle("CMSSW_3_3_0");
 energyNew.SetTitle("CMSSW_3_3_0");
 ESCoPNew.SetTitle("CMSSW_3_3_0");
 HitsOeNew.SetTitle("CMSSW_3_3_0");

 EoPOld.SetLineColor(kRed);
 EoPZOld.SetLineColor(kRed);
 etaDistOld.SetLineColor(kRed);
 energyOld.SetLineColor(kRed);
 ESCoPOld.SetLineColor(kRed);
 HitsOeOld.SetLineColor(kRed);

 EoPNew.SetLineColor(kBlue);
 EoPZNew.SetLineColor(kBlue);
 etaDistNew.SetLineColor(kBlue);
 energyNew.SetLineColor(kBlue);
 ESCoPNew.SetLineColor(kBlue);
 HitsOeNew.SetLineColor(kBlue);
 
 TCanvas cc1("cc1","cc1");
 EoPNew.GetXaxis().SetTitle("E/p");
 EoPNew.DrawNormalized();
 EoPOld.DrawNormalized("same");
 cc1.SetLogy();
 cc1.BuildLegend();
 
 TCanvas cc2("cc2","cc2");
 EoPZNew.GetXaxis().SetTitle("E/p");
 EoPZNew.DrawNormalized();
 EoPZOld.DrawNormalized("same");
 cc2.SetLogy();
 cc2.BuildLegend();

 TCanvas cc3("cc3","cc3");
 etaDistNew.GetXaxis().SetTitle("#eta");
 etaDistNew.DrawNormalized();
 etaDistOld.DrawNormalized("same");
 cc3.SetLogy();
 cc3.BuildLegend();

 TCanvas cc4("cc4","cc4");
 energyNew.GetXaxis()->SetTitle("Energy [GeV]");
 energyNew.DrawNormalized();
 energyOld.DrawNormalized("same");
 cc4.SetLogy();
 cc4.BuildLegend();

 TCanvas cc5("cc5","cc5");
 ESCoPNew.GetXaxis()->SetTitle("E_{SC}/p");
 ESCoPNew.DrawNormalized();
 ESCoPOld.DrawNormalized("same");
 cc5.SetLogy();
 cc5.BuildLegend();

 TCanvas cc6("cc6","cc6");
 HitsOeNew.GetXaxis()->SetTitle("#frac{#sum Hits}{energy}");
 HitsOeNew.DrawNormalized();
 HitsOeOld.DrawNormalized("same");
 cc6.SetLogy();
 cc6.BuildLegend();

 gApplication->Terminate(0);

}

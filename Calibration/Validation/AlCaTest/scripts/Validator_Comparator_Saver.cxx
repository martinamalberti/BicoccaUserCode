{
///==== Validator ====
  
  TH1F etaDist ("etaDist","etaDist",200,-3,3);
  TH1F energy ("energy","energy",300,0,800);
  TH1F ESCoP ("ESCoP","ESCoP",200,0,20);
  TH1F HitsOe ("HitsOe","HitsOe",200,0,1.02);
  m_tree->Draw("eta>>etaDist");
  m_tree->Draw("energy>>energy");
  m_tree->Draw("ESCoP>>ESCoP");
  m_tree->Draw("recHits/energy>>HitsOe");
  TFile fileNew ("_NEWFILE_","RECREATE");
  etaDist.Write();
  energy.Write();
  ESCoP.Write();
  HitsOe.Write();

///==== Comparator ====

 gStyle->SetOptStat(0);
 gStyle->SetOptTitle(0);
 TFile fileOld ("_OLDFILE_","READ");
 TH1F etaDistOld = (TH1F) fileOld.Get("etaDist");
 TH1F energyOld = (TH1F) fileOld.Get("energy");
 TH1F ESCoPOld = (TH1F) fileOld.Get("ESCoP");
 TH1F HitsOeOld = (TH1F) fileOld.Get("HitsOe");


 etaDistOld.SetTitle("_OLDRELEASE_");
 energyOld.SetTitle("_OLDRELEASE_");
 ESCoPOld.SetTitle("_OLDRELEASE_");
 HitsOeOld.SetTitle("_OLDRELEASE_");

 etaDist.SetTitle("_NEWRELEASE_");
 energy.SetTitle("_NEWRELEASE_");
 ESCoP.SetTitle("_NEWRELEASE_");
 HitsOe.SetTitle("_NEWRELEASE_");

 etaDistOld.SetLineColor(kRed);
 energyOld.SetLineColor(kRed);
 ESCoPOld.SetLineColor(kRed);
 HitsOeOld.SetLineColor(kRed);

 etaDist.SetLineColor(kBlue);
 energy.SetLineColor(kBlue);
 ESCoP.SetLineColor(kBlue);
 HitsOe.SetLineColor(kBlue);

 TLegend* leg = new TLegend(0.1,0.7,0.48,0.9);
 leg->AddEntry(&etaDist,"","l");
 leg->AddEntry(&etaDistOld,"","l");

 TLegend* legMiddle = new TLegend(0.3333333,0.190678,0.7140805,0.3898305);
 legMiddle->AddEntry(&etaDist,"","l");
 legMiddle->AddEntry(&etaDistOld,"","l");

 TCanvas ccetaDist("ccetaDist","ccetaDist");
 etaDist.GetXaxis().SetTitle("#eta");
 etaDist.DrawNormalized();
 etaDistOld.DrawNormalized("same");
 ccetaDist.SetLogy();
 legMiddle->Draw();
//  ccetaDist.BuildLegend();
ccetaDist.SetGrid();
 ccetaDist.SaveAs("~/public/html/AlCaRecoValidation/RelVal__NEWRELEASE_/SAMPLE_etaDist.gif");

 TCanvas ccenergy("ccenergy","ccenergy");
 energy.GetXaxis()->SetTitle("Energy [GeV]");
 energy.DrawNormalized();
 energyOld.DrawNormalized("same");
 ccenergy.SetLogy();
 ccenergy.BuildLegend();
ccenergy.SetGrid();
 ccenergy.SaveAs("~/public/html/AlCaRecoValidation/RelVal__NEWRELEASE_/SAMPLE_energy_dist.gif");

 TCanvas ccESCoP("ccESCoP","ccESCoP");
 ESCoP.GetXaxis()->SetTitle("E_{SC}/p");
 ESCoP.DrawNormalized();
 ESCoPOld.DrawNormalized("same");
 ccESCoP.SetLogy();
 ccESCoP.BuildLegend();
ccESCoP.SetGrid();
 ccESCoP.SaveAs("~/public/html/AlCaRecoValidation/RelVal__NEWRELEASE_/SAMPLE_ESCoP_dist.gif");

 TCanvas ccHitsOe("ccHitsOe","ccHitsOe");
 HitsOe.GetXaxis()->SetTitle("#frac{#sum Hits}{energy}");
 HitsOe.DrawNormalized();
 HitsOeOld.DrawNormalized("same");
 ccHitsOe.SetLogy();
//  ccHitsOe.BuildLegend();
 leg->Draw();
 ccHitsOe.SetGrid(); 
 ccHitsOe.SaveAs("~/public/html/AlCaRecoValidation/RelVal__NEWRELEASE_/SAMPLE_HitsOe.gif");

 gApplication->Terminate(0);

}

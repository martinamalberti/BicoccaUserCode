{
  // Input file
  TFile inFile("CRAFT_dEdXdistr.root", "read");

  TGraphAsymmErrors* BetheBloch_graph = (TGraphAsymmErrors*)inFile.Get("BetheBloch_graph_curved");
  TGraphErrors* BetheBloch_residuals = (TGraph*)inFile.Get("BetheBloch_graph_residuals_curved");


  TGraph* BetheBloch_th = (TGraph*)inFile.Get("BetheBloch_th");
  TGraph* BetheBloch_th_ion = (TGraph*)inFile.Get("BetheBloch_th_ion");
  TGraph* BetheBloch_th_irr = (TGraph*)inFile.Get("BetheBloch_th_irr");


  TCanvas* c1 = new TCanvas ("c1", "BetheBloch");
  c1->SetLogx();
  c1->SetGridx();
  c1->SetGridy();

  BetheBloch_graph->Draw("AP");
  BetheBloch_graph->SetMarkerStyle(20);
  BetheBloch_graph->SetMarkerSize(0.7);
  BetheBloch_graph->SetMarkerColor(kMagenta);
  BetheBloch_graph->GetYaxis()->SetRangeUser(1., 8.);


  BetheBloch_th->Draw("PC, same");
  BetheBloch_th_ion->Draw("PC, same");
  BetheBloch_th_irr->Draw("PC, same");
  


  TCanvas* c2 = new TCanvas ("c2", "BetheBloch Residuals");
  c2->SetLogx();
  c2->SetGridx();
  c2->SetGridy();
  
  BetheBloch_residuals->Draw("AP");
  BetheBloch_residuals->SetMarkerStyle(20);
  BetheBloch_residuals->SetMarkerSize(0.7);
  BetheBloch_residuals->SetMarkerColor(kMagenta);
  BetheBloch_residuals->GetYaxis()->SetRangeUser(-0.1, 0.1);

}

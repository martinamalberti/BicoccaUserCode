{
  // Input file
  TFile inFile("CRAFT_dEdXdistr.root", "read");

  TGraphAsymmErrors* BetheBloch_graph = (TGraphAsymmErrors*)inFile.Get("BetheBloch_graph");
  TGraph* BetheBloch_th = (TGraph*)inFile.Get("BetheBloch_th");
  TGraph* BetheBloch_th_ion = (TGraph*)inFile.Get("BetheBloch_th_ion");
  TGraph* BetheBloch_th_irr = (TGraph*)inFile.Get("BetheBloch_th_irr");


  TCanvas* canvas = new TCanvas;
  //  gPad->SetLogx(0);
  BetheBloch_graph->Draw("AP");
  BetheBloch_th->Draw("PC, same");
  BetheBloch_th_ion->Draw("PC, same");
  BetheBloch_th_irr->Draw("PC, same");
}

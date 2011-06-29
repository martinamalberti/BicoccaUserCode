
{
    
  // Set style options
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110); 
  gStyle->SetOptFit(0); 
  gStyle->SetFitFormat("6.3g"); 
  gStyle->SetPalette(1); 
 
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.05);
  gStyle->SetTitleFont(42,"xyz");
  gStyle->SetTitleSize(0.05);
  gStyle->SetLabelFont(42,"xyz");
  gStyle->SetLabelSize(0.05);
  gROOT->ForceStyle();


  // Drawings
  TGraphAsymmErrors *gphi[170];
  TH1F *href[170];
  TH1F *h_EoP[100];

  TF1 *fref[170];
  TF1 *f_EoP[100];

  TFile *f = TFile::Open("plots/TTscale/May10ReReco_plus_PromptReco_2011Laser_Bon_V20110616_weightedAverage/checkScale.root");

  // --- example reference
  TCanvas *creference[5000]; 
  int ncref = 0;
  for(int i = 0; i < 6; ++i){
    char canvasName[50];
    if (i%6==0) {
      sprintf(canvasName, "template-%0d", i/6); 
      creference[i/6] = new TCanvas(canvasName, canvasName);
      creference[i/6] -> Divide(3,2);
    }
    
    creference[i/6] -> cd (i%6+1);
    char objname[100];
    sprintf(objname,"reference/href_region%02d",i);
    href[i] = (TH1F*)f->Get(objname);
    sprintf(objname,"reference/fref_%d",i);
    fref[i] = (TF1*)f->Get(objname);

    href[i] -> SetFillColor(kCyan+2);
    href[i] -> GetXaxis() -> SetTitle("E/p");
    href[i] -> GetXaxis() -> SetRangeUser(0.7,1.7); 
    href[i] -> Draw("histos");
    fref[i] -> Draw("same");
    gPad->Update();
  }
  


  // -- example fits
  TCanvas *cfit[5000]; 
  for(int i = 0; i < 10; ++i){
    char canvasName[50];
    if (i%6==0) {
      sprintf(canvasName, "fit-%0d", i/6); 
      cfit[i/6] = new TCanvas(canvasName, canvasName);
      cfit[i/6] -> Divide(3,2);
    }
    
    cfit[i/6] -> cd (i%6+1);
    
    char objname[100];
    sprintf(objname,"data/EoP_region%02d",i);
    h_EoP[i] = (TH1F*)f->Get(objname);
    sprintf(objname,"data/f_EoP_%d",i);
    f_EoP[i] = (TF1*)f->Get(objname);

    h_EoP[i] -> SetMarkerStyle(20);
    h_EoP[i] -> SetMarkerSize(0.8);
    h_EoP[i] -> SetMarkerColor(kGreen+2);
    h_EoP[i] -> SetLineColor(kGreen+2);

    h_EoP[i] -> GetXaxis() -> SetTitle("E/p");
    h_EoP[i] -> GetXaxis() -> SetRangeUser(0.3,1.7); 
    h_EoP[i] -> Draw("e1");
    f_EoP[i] -> Draw("sames+");
    char ll[100];
    sprintf(ll,"scale = %.3f +/- %.3f",f_EoP[i] -> GetParameter(7),f_EoP[i] -> GetParError(7));
    TLatex *latex = new TLatex(0.2,0.8,ll);
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.05);
    latex->Draw("same");
  }


  f->cd();


  // --- 
  TCanvas *c = new TCanvas("c","c");
  c -> cd();
  c->SetGridx();
  c->SetGridy();    
  gregion->SetMarkerStyle(20);
  gregion->SetMarkerSize(0.8);
  gregion->SetMarkerColor(kGreen+2);
  gregion -> Draw("ap");
  gregion -> GetHistogram()->GetXaxis() ->SetTitle("region index");
  gregion -> GetHistogram()->GetYaxis() ->SetTitle("scale");
  gregion -> GetHistogram()->GetYaxis() ->SetRangeUser(0.9,1.1);


  TLegend * leg = new TLegend(0.69, 0.69, 0.89, 0.89);
  leg->SetFillColor(0);
    
  TCanvas *cc = new TCanvas("cc","cc");
  cc -> cd();
  cc->SetGridx();
  cc->SetGridy();
  int mycolor ;
  for ( int i = 0; i < 34; i++){
    char gname[100];
    sprintf(gname,"gphi_etaRegion%d",i);
    gphi[i] = (TGraphAsymmErrors*)f->Get(gname);
    if (gphi[i]->IsZombie()) continue;

    gphi[i]->SetMarkerStyle(20);
    gphi[i]->SetMarkerSize(0.8);
    if (i==9) {
      mycolor = 1; 
      gphi[i]->SetMarkerStyle(24);
    }
    else { mycolor = i+1; }
    gphi[i]->SetMarkerColor(mycolor);
    gphi[i]->SetLineColor(mycolor);
   
    if (i==0) {
      gphi[i] -> Draw("ap");
      leg->AddEntry(gphi[i], gphi[i]->GetTitle(), "PL");
    }
    if (i!=0 && abs(i%5)==0)  {
      gphi[i] -> Draw("psame");
      leg->AddEntry(gphi[i], gphi[i]->GetTitle(), "PL");
    }
    gphi[i] -> GetHistogram()->GetXaxis() ->SetTitle("#phi");
    gphi[i] -> GetHistogram()->GetYaxis() ->SetTitle("scale");
    gphi[i] -> GetHistogram()->GetXaxis() ->SetRangeUser(0.,361);
    gphi[i] -> GetHistogram()->GetYaxis() ->SetRangeUser(0.95,1.05);
  }
  leg->Draw("same");
 
  
  TCanvas *cOcc = new TCanvas("cOcc","cOcc");
  cOcc->SetLeftMargin(0.1); 
  cOcc->SetRightMargin(0.15); 
  cOcc -> cd();
  h_occupancy->Draw("COLZ");
  h_occupancy->GetXaxis() ->SetTitle("#phi");
  h_occupancy->GetYaxis() ->SetTitle("#eta");

  TCanvas *cmap = new TCanvas("cmap","cmap");
  cmap -> cd();
  cmap->SetLeftMargin(0.1); 
  cmap->SetRightMargin(0.15); 
  h_scale_map->Draw("COLZ");
  h_scale_map->GetXaxis() ->SetTitle("#phi");
  h_scale_map->GetYaxis() ->SetTitle("#eta");
  h_scale_map->GetZaxis() ->SetRangeUser(0.95,1.05);
  
  TCanvas *cScaleProfile = new TCanvas("cScaleProfile","cScaleProfile");
  cScaleProfile -> cd();
  cScaleProfile ->SetGridx();
  cScaleProfile ->SetGridy();
  h_scale_profile_vs_ieta->SetMarkerStyle(20);
  h_scale_profile_vs_ieta->SetMarkerSize(0.9);
  h_scale_profile_vs_ieta->Draw("ep");
  h_scale_profile_vs_ieta->GetXaxis() ->SetTitle("ieta");
  h_scale_profile_vs_ieta->GetYaxis() ->SetTitle("average relative scale");
  h_scale_profile_vs_ieta->GetYaxis() ->SetRangeUser(0.98,1.02);

  TCanvas *cmapErr = new TCanvas("cmapErr","cmapErr");
  cmapErr->SetLeftMargin(0.1); 
  cmapErr->SetRightMargin(0.15); 
  cmapErr -> cd();
  h_error_map->Draw("COLZ");
  h_error_map->GetXaxis() ->SetTitle("#phi");
  h_error_map->GetYaxis() ->SetTitle("#eta");
  //h_error_map->GetZaxis() ->SetRangeUser(0.,2.5*h_EoP_error->GetRMS());
  h_error_map->GetZaxis() ->SetRangeUser(0.,0.02);

  TCanvas *cErrorProfile = new TCanvas("cErrorProfile","cErrorProfile");
  cErrorProfile -> cd();
  cErrorProfile ->SetGridx();
  cErrorProfile ->SetGridy();
  h_error_profile_vs_ieta->SetMarkerStyle(20);
  h_error_profile_vs_ieta->SetMarkerSize(0.9);
  h_error_profile_vs_ieta->Draw("ep");
  h_error_profile_vs_ieta->GetXaxis() ->SetTitle("ieta");
  h_error_profile_vs_ieta->GetYaxis() ->SetTitle("< precision >");
  h_error_profile_vs_ieta->GetYaxis() ->SetRangeUser(0.,0.03);
  
  TCanvas* cspread_centralregion = new TCanvas("cspread_centralregion", "cspread_centralregion",500,500);
  cspread_centralregion->cd();
  h_EoP_spread_centralregion -> SetFillStyle(3001);
  h_EoP_spread_centralregion -> SetLineWidth(3001);
  h_EoP_spread_centralregion -> SetFillColor(kGreen+2);
  h_EoP_spread_centralregion -> SetLineColor(kGreen+2);
  h_EoP_spread_centralregion -> GetXaxis() -> SetTitle("Relative E/p scale");
  h_EoP_spread_centralregion -> Draw("hs");
  //h_EoP_spread_centralregion   -> Fit("gaus");
  gPad->Update();

  TPaveStats *s_EoP_spread_centralregion = (TPaveStats*)(h_EoP_spread_centralregion->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread_centralregion -> SetX1NDC(0.55); //new x start position
  s_EoP_spread_centralregion -> SetX2NDC(0.89); //new x end position
  s_EoP_spread_centralregion -> SetY1NDC(0.750); //new x start position
  s_EoP_spread_centralregion -> SetY2NDC(0.85); //new x end position
  s_EoP_spread_centralregion -> SetOptStat(1110);
  s_EoP_spread_centralregion -> SetTextColor(kGreen+2);
  s_EoP_spread_centralregion -> SetTextSize(0.03);
  s_EoP_spread_centralregion -> Draw("sames");

  TCanvas* cspread_outerregion = new TCanvas("cspread_outerregion", "cspread_outerregion",500,500);
  cspread_outerregion->cd();
  h_EoP_spread_outerregion -> SetFillStyle(3001);
  h_EoP_spread_outerregion -> SetLineWidth(3001);
  h_EoP_spread_outerregion -> SetFillColor(kGreen+2);
  h_EoP_spread_outerregion -> SetLineColor(kGreen+2);
  h_EoP_spread_outerregion -> GetXaxis() -> SetTitle("Relative E/p scale");
  h_EoP_spread_outerregion -> Draw("hs");
  //h_EoP_spread_outerregion -> Fit("gaus");
  gPad->Update();

  TPaveStats *s_EoP_spread_outerregion = (TPaveStats*)(h_EoP_spread_outerregion->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread_outerregion -> SetX1NDC(0.55); //new x start position
  s_EoP_spread_outerregion -> SetX2NDC(0.89); //new x end position
  s_EoP_spread_outerregion -> SetY1NDC(0.750); //new x start position
  s_EoP_spread_outerregion -> SetY2NDC(0.85); //new x end position
  s_EoP_spread_outerregion -> SetOptStat(1110);
  s_EoP_spread_outerregion -> SetTextColor(kGreen+2);
  s_EoP_spread_outerregion -> SetTextSize(0.03);
  s_EoP_spread_outerregion -> Draw("sames");

  TCanvas* cspread = new TCanvas("cspread", "cspread",500,500);
  cspread->cd();
  h_EoP_spread -> SetFillStyle(3001);
  h_EoP_spread -> SetLineWidth(3001);
  h_EoP_spread -> SetFillColor(kGreen+2);
  h_EoP_spread -> SetLineColor(kGreen+2);
  h_EoP_spread -> GetXaxis() -> SetTitle("Relative E/p scale");
  h_EoP_spread -> Draw("hs");
  gPad->Update();

  TPaveStats *s_EoP_spread = (TPaveStats*)(h_EoP_spread->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread -> SetX1NDC(0.55); //new x start position
  s_EoP_spread -> SetX2NDC(0.89); //new x end position
  s_EoP_spread -> SetY1NDC(0.750); //new x start position
  s_EoP_spread -> SetY2NDC(0.85); //new x end position
  s_EoP_spread -> SetOptStat(1110);
  s_EoP_spread -> SetTextColor(kGreen+2);
  s_EoP_spread -> SetTextSize(0.03);
  s_EoP_spread -> Draw("sames");

  TCanvas* cErr = new TCanvas("cErr", "cErr",500,500);
  cErr->cd();
  h_EoP_error -> SetFillStyle(3001);
  h_EoP_error -> SetLineWidth(3001);
  h_EoP_error -> SetFillColor(kGreen+2);
  h_EoP_error -> SetLineColor(kGreen+2);
  h_EoP_error -> GetXaxis() -> SetTitle("error");
  //  h_EoP_error -> GetXaxis() -> SetRangeUser(0.,10*h_EoP_error->GetRMS());
  h_EoP_error -> GetXaxis() -> SetRangeUser(0.,0.03);
  h_EoP_error -> Draw("hs");
  gPad->Update();

  TPaveStats *s_scale_error = (TPaveStats*)(h_EoP_error->GetListOfFunctions()->FindObject("stats"));
  s_scale_error->SetX1NDC(0.55); //new x start position
  s_scale_error->SetX2NDC(0.89); //new x end position
  s_scale_error->SetY1NDC(0.750); //new x start position
  s_scale_error->SetY2NDC(0.875); //new x end position
  s_scale_error ->SetOptStat(1110);
  s_scale_error ->SetTextColor(kGreen+2);
  s_scale_error ->SetTextSize(0.03);
  s_scale_error -> Draw("sames");



  // ----------- print plots 

  std::string dirname("./plots/TTscale/May10ReReco_plus_PromptReco_2011Laser_Bon_V20110616_weightedAverage/");

  // print plots
  std::string plotname;

  creference[0] -> cd();
  plotname = "exampleReferences.gif";
  creference[0] -> Print((dirname+plotname).c_str());

  cfit[0] ->cd(); 
  plotname = "exampleFits.gif";
  cfit[0] -> Print((dirname+plotname).c_str());
  
  c->cd();
  plotname = "RelativeScale_vs_regionIndex.gif";
  c -> Print((dirname+plotname).c_str());
 
  cc->cd();
  plotname="RelativeScale_vs_phi.gif";
  cc->Print((dirname+plotname).c_str());

  cspread->cd();
  plotname="RelativeScale_spread.gif";
  cspread->Print((dirname+plotname).c_str()); 

  cspread_centralregion->cd();
  plotname="RelativeScale_spread_centralBarrel.gif";
  cspread_centralregion->Print((dirname+plotname).c_str()); 

  cspread_outerregion->cd();
  plotname="RelativeScale_spread_outerBarrel.gif";
  cspread_outerregion->Print((dirname+plotname).c_str()); 

  cErr->cd();
  plotname="RelativeScale_error.gif";
  cErr->Print((dirname+plotname).c_str()); 

  gStyle->SetOptStat(0);
  cmap->cd();
  plotname="RelativeScale_map.gif";
  cmap->Print((dirname+plotname).c_str());

  cmapErr->cd();
  plotname="RelativeScale_errorsmap.gif";
  cmapErr->Print((dirname+plotname).c_str());
  
  cOcc->cd();
  plotname="occupancy_map.gif";
  cOcc->Print((dirname+plotname).c_str()); 

  
  cScaleProfile->cd();
  plotname="RelativeScale_profile_vs_ieta.gif";
  cScaleProfile->Print((dirname+plotname).c_str()); 

  cErrorProfile->cd();
  plotname="RelativeScale_error_profile_vs_ieta.gif";
  cErrorProfile->Print((dirname+plotname).c_str()); 
  
}

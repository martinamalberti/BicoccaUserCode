makeAlphaMap(Char_t* infile1 =0)
{
  // Open file
  if (!infile1)
  {
    std::cout << "CHK-STB Error: Input file not specified !" << std::endl;
    return;
  }  
  
  TFile *f = TFile::Open(infile1);  // data
  
  
  
  // Get trees
  std::cout << std::endl;
  
  TTree* ntu_DA = (TTree*)(f->Get("ntu"));
  std::cout << "     DATA: " << ntu_DA->GetEntries() << " entries in " << infile1 << std::endl; 
  
  
  
  // Set branch addresses  
  int isEB;
  float seedLaserCorrection, seedLaserAlpha;
  int seedIeta,seedIphi;
  int seedIx,seedIy,seedZside;

  ntu_DA->SetBranchAddress("ele1_isEB",           &isEB);  
  ntu_DA->SetBranchAddress("ele1_seedLaserCorr",  &seedLaserCorrection);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele1_seedIeta",       &seedIeta);
  ntu_DA->SetBranchAddress("ele1_seedIphi",       &seedIphi);
  ntu_DA->SetBranchAddress("ele1_seedIx",         &seedIx);
  ntu_DA->SetBranchAddress("ele1_seedIy",         &seedIy);
  ntu_DA->SetBranchAddress("ele1_seedZside",      &seedZside);  
  
  
  
  // Define histograms
  TH1F* h_seedLaserAlphaDistr_EB  = new TH1F("h_seedLaserAlphaDistr_EB", "", 200,0.9,1.6);
  TH1F* h_seedLaserAlphaDistr_EEp = new TH1F("h_seedLaserAlphaDistr_EEp","", 200,0.9,1.6);
  TH1F* h_seedLaserAlphaDistr_EEm = new TH1F("h_seedLaserAlphaDistr_EEm","", 200,0.9,1.6);
  
  TProfile2D* h_seedLaserAlphaMap_EB  = new TProfile2D("h_seedLaserAlphaMap_EB","",  171, -85., 86., 361,   0.,361., 0.9,1.6);
  TProfile2D* h_seedLaserAlphaMap_EEp = new TProfile2D("h_seedLaserAlphaMap_EEp","", 101,   0.,101., 100,   0.,101., 0.9,1.6);
  TProfile2D* h_seedLaserAlphaMap_EEm = new TProfile2D("h_seedLaserAlphaMap_EEm","", 101,   0.,101., 100,   0.,101., 0.9,1.6);
  
  
  
  // Loop over entries
  int nEntries = ntu_DA -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%10000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_DA -> GetEntry(ientry); 
    
    
    if( isEB == 1 )
    {
      h_seedLaserAlphaDistr_EB -> Fill(seedLaserAlpha);
      h_seedLaserAlphaMap_EB -> Fill(seedIeta,seedIphi,seedLaserAlpha);
    }
    
    else
    {
      if( seedZside > 0)
      {
       h_seedLaserAlphaDistr_EEp -> Fill(seedLaserAlpha);
       h_seedLaserAlphaMap_EEp -> Fill(seedIx,seedIy,seedLaserAlpha);
      }
      else
      {
       h_seedLaserAlphaDistr_EEm -> Fill(seedLaserAlpha);
       h_seedLaserAlphaMap_EEm -> Fill(seedIx,seedIy,seedLaserAlpha);
      }
    }
    
  }
  
  
  
  TCanvas* c_seedLaserAlpha_EB = new TCanvas("c_seedLaserAlpha_EB","seedLaserAlpha_EB",100,100,900,500);
  c_seedLaserAlpha_EB -> Divide(2,1);
  
  c_seedLaserAlpha_EB -> cd(1);
  h_seedLaserAlphaMap_EB -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserAlphaMap_EB -> GetYaxis() -> SetTitle("i#phi");
  h_seedLaserAlphaMap_EB -> GetZaxis() -> SetTitle("#alpha");
  h_seedLaserAlphaMap_EB -> GetZaxis() -> SetRangeUser(0.9,1.6);
  h_seedLaserAlphaMap_EB -> Draw("COLZ");
  
  c_seedLaserAlpha_EB -> cd(2);
  c_seedLaserAlpha_EB -> SetLogy();
  h_seedLaserAlphaDistr_EB -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserAlphaDistr_EB -> Draw();
  
  
  
  TCanvas* c_seedLaserAlpha_EEp = new TCanvas("c_seedLaserAlpha_EEp","seedLaserAlpha_EEp",100,100,900,500);
  c_seedLaserAlpha_EEp -> Divide(2,1);
  
  c_seedLaserAlpha_EEp -> cd(1);
  h_seedLaserAlphaMap_EEp -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserAlphaMap_EEp -> GetYaxis() -> SetTitle("i#phi");
  h_seedLaserAlphaMap_EEp -> GetZaxis() -> SetTitle("#alpha");
  h_seedLaserAlphaMap_EEp -> GetZaxis() -> SetRangeUser(0.9,1.6);
  h_seedLaserAlphaMap_EEp -> Draw("COLZ");
  
  c_seedLaserAlpha_EEp -> cd(2);
  c_seedLaserAlpha_EEp -> SetLogy();
  h_seedLaserAlphaDistr_EEp -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserAlphaDistr_EEp -> Draw();
  
  
  
  TCanvas* c_seedLaserAlpha_EEm = new TCanvas("c_seedLaserAlpha_EEm","seedLaserAlpha_EEm",100,100,900,500);
  c_seedLaserAlpha_EEm -> Divide(2,1);
  
  c_seedLaserAlpha_EEm -> cd(1);
  h_seedLaserAlphaMap_EEm -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserAlphaMap_EEm -> GetYaxis() -> SetTitle("i#phi");
  h_seedLaserAlphaMap_EEm -> GetZaxis() -> SetTitle("#alpha");
  h_seedLaserAlphaMap_EEm -> GetZaxis() -> SetRangeUser(0.9,1.6);
  h_seedLaserAlphaMap_EEm -> Draw("COLZ");
  
  c_seedLaserAlpha_EEm -> cd(2);
  c_seedLaserAlpha_EEm -> SetLogy();
  h_seedLaserAlphaDistr_EEm -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserAlphaDistr_EEm -> Draw();    
}
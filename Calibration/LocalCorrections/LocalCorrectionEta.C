#include "histoFunc.h"

//derive f(local eta) correction in MC, with 4 different f for each module.
// foldi crystal in eta into one single module

bool IsEtaGap(float eta){
  float feta = fabs(eta);
  if( fabs(feta - 0 )<2) return true;
  if( fabs(feta - 25)<2) return true;
  if( fabs(feta - 45)<2) return true;
  if( fabs(feta - 65)<2) return true;
  if( fabs(feta - 85)<2) return true;
  return false;
}

int templIndex(float eta){
    float feta = fabs(eta);
    if (feta <= 25)            {return 0;}
    if (feta> 25 && feta <= 45){return 1;}
    if (feta> 45 && feta <= 65){return 2;}
    if (feta> 65 && feta <= 85){return 3;}

    return -1;
}

void LocalCorrectionEta()
{
  gROOT->SetStyle("Plain");
  
  float xtalWidth = 0.01745329;

  //---- variables for selections
  float etaMax = 1.44;
  
  float r9max = 999. ;
  float r9min = 0.94 ;
  
  bool useOddCry  = false;
  bool useEvenCry = false;
  
  //---- output file to save graphs
  char outfilename[100];
  sprintf(outfilename,"GraphsLocalEtaDK_highR9.root");

 
  TChain *ntu_MC = new TChain("ntu");
  TChain *ntu_Data = new TChain("ntu");
  
  // MC summer 2011
  ntu_MC->Add("/data2/ghezzi/Calib/PhotonMCDat/WZAnalysis_WToENu_ter.root");
  
  //data
  ntu_Data->Add("/data2/ghezzi/Calib/PhotonDataDat/WZAnalysis_PromptV4_ter.root");
  ntu_Data->Add("/data2/ghezzi/Calib/PhotonDataDat/WZAnalysis_PromptV5_ter.root");
  ntu_Data->Add("/data2/ghezzi/Calib/PhotonDataDat/WZAnalysis_May10ReReco_ter.root");
  
  std::cout << "     MC  : " << ntu_MC->GetEntries() << " entries in  MC  sample" << std::endl;
  std::cout << "     Data  : " << ntu_Data->GetEntries() << " entries in  Data  sample" << std::endl;

  // observables
  float EoP, scEta, scPhi;
  float scE3x3, scE5x5, scEne;  
  float charge, scLocalEta, scLocalPhi,crackCorr,scLocalCorr,scLocalCorr_DK, phR9, phE; 
  float etaC_DK, phiC_DK;


  // Set branch addresses for MC  
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchAddress("ele1_scPhi", &scPhi);
  ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_MC->SetBranchAddress("ele1_e3x3", &scE3x3);
  //ntu_MC->SetBranchAddress("ele1_e5x5", &scE5x5);
  ntu_MC->SetBranchAddress("ele1_scE", &scEne);
  ntu_MC->SetBranchAddress("ele1_charge", &charge);
  ntu_MC->SetBranchAddress("ele1_scLocalPhi",&scLocalPhi); 
  ntu_MC->SetBranchAddress("ele1_scLocalEta",&scLocalEta); 
  ntu_MC->SetBranchAddress("ele1_scCrackCorr",&crackCorr); 
  ntu_MC->SetBranchAddress("ele1_scLocalContCorr",&scLocalCorr); 
  ntu_MC->SetBranchAddress("ele1_scLocalContCorr_DK",&scLocalCorr_DK); 
  ntu_MC->SetBranchAddress("ele1_etaC_DK",&etaC_DK); 
  ntu_MC->SetBranchAddress("ele1_phiC_DK",&phiC_DK); 
  ntu_MC->SetBranchAddress("ele1_ph_R9",&phR9); 
  ntu_MC->SetBranchAddress("ele1_ph_E",&phE); 

 // Set branch addresses for Data
  ntu_Data->SetBranchAddress("ele1_scEta", &scEta);
  ntu_Data->SetBranchAddress("ele1_scPhi", &scPhi);
  ntu_Data->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_Data->SetBranchAddress("ele1_e3x3", &scE3x3);
  //ntu_Data->SetBranchAddress("ele1_e5x5", &scE5x5);
  ntu_Data->SetBranchAddress("ele1_scE", &scEne);
  ntu_Data->SetBranchAddress("ele1_charge", &charge);
  ntu_Data->SetBranchAddress("ele1_scLocalPhi",&scLocalPhi); 
  ntu_Data->SetBranchAddress("ele1_scLocalEta",&scLocalEta); 
  ntu_Data->SetBranchAddress("ele1_scCrackCorr",&crackCorr); 
  ntu_Data->SetBranchAddress("ele1_scLocalContCorr",&scLocalCorr); 
  ntu_Data->SetBranchAddress("ele1_scLocalContCorr_DK",&scLocalCorr_DK);
  ntu_Data->SetBranchAddress("ele1_etaC_DK",&etaC_DK); 
  ntu_Data->SetBranchAddress("ele1_phiC_DK",&phiC_DK);  
  ntu_Data->SetBranchAddress("ele1_ph_R9",&phR9); 
  ntu_Data->SetBranchAddress("ele1_ph_E",&phE); 

  const unsigned int nBins = 20;
  const int Ntempl = 4;
  std::cout << "nBins = " << nBins << std::endl;
  
  // histogram definition
  TH1F* h_EoP_MC[nBins][Ntempl] ;   
  TH1F* h_EoC_MC[nBins][Ntempl] ;

  TH1F* h_EoP_Data[nBins][Ntempl];
  TH1F* h_EoC_Data[nBins][Ntempl] ;

  for(int mod=0; mod<Ntempl; mod++){
    for(unsigned int i = 0; i < nBins; ++i)
      {
	char histoName[80];

	sprintf(histoName, "EoP_MC_%d_mod%d", i,mod+1);
	h_EoP_MC[i][mod] = new TH1F(histoName, histoName, 1200, 0., 3.);
	h_EoP_MC[i][mod] -> SetFillColor(4);
	h_EoP_MC[i][mod] -> SetFillStyle(3004);
	sprintf(histoName, "EoC_MC_%d_mod%d", i,mod+1);
	h_EoC_MC[i][mod] = new TH1F(histoName, histoName, 1200, 0., 3.);
	h_EoC_MC[i][mod] -> SetFillColor(3);
	h_EoC_MC[i][mod] -> SetFillStyle(3004);

	sprintf(histoName, "EoP_Data_%d_mod%d", i,mod+1);
	h_EoP_Data[i][mod] = new TH1F(histoName, histoName, 1200, 0., 3.);
	h_EoP_Data[i][mod] -> SetFillColor(4);
	h_EoP_Data[i][mod] -> SetFillStyle(3004);
	sprintf(histoName, "EoC_Data_%d_mod%d", i,mod+1);
	h_EoC_Data[i][mod] = new TH1F(histoName, histoName, 1200, 0., 3.);
	h_EoC_Data[i][mod] -> SetFillColor(3);
	h_EoC_Data[i][mod] -> SetFillStyle(3004);
      }
  }

  
 
  TH1F* h_template[Ntempl];
  for(unsigned int i = 0; i < Ntempl; ++i)
    {
      char histoName[100];
      sprintf(histoName, "template_%d", i);
      h_template[i] = new TH1F(histoName, "", 1200, 0., 3.);   
    }


  ////////////////////////////////////////       MC        ///////////////////////////////////////////////
  std::cout << "Loop on MC events ... " << endl; 
  // loop on MC, make refernce and fit dist
  for(int entry = 0; entry < ntu_MC->GetEntries(); ++entry) {
    if( entry%200000 == 0 ) std::cout << "reading saved entry " << entry << std::endl;
    //    if (entry>1000) break;

    ntu_MC->GetEntry(entry);

    // redefine E/p using Photn Energy (needed to work with DK corrections)
    EoP = EoP/scEne*phE;

    // eta or R9 cuts
    if( fabs(scEta) > etaMax ) continue;
    if ( phR9 < r9min || phR9 > r9max ) continue; 

    // cut phi cracks
    float phi = (scPhi+3.1415926536)/xtalWidth;;
    float modphi = (int)phi%20;
    if (fabs(modphi-10)<2.) continue;

    // use only even/odd crystals (for data)
    if ( useOddCry  && int(modphi)%2 == 0) continue; 
    if ( useEvenCry && int(modphi)%2 != 0) continue;	 
    
    //remove eta gaps
    float fetaCry = fabs (scEta) / xtalWidth;
    if( IsEtaGap(fetaCry) ) continue;
    
    int mod = templIndex(fetaCry);
    
    // fill template for each mod
    h_template[mod]-> Fill(EoP*1.001*scLocalCorr_DK);

    // fill MC histos in eta bins
    //float locEta = scLocalEta+0.5; 
    float locEta = etaC_DK/xtalWidth + 0.5;
    if (locEta<0.) locEta+=1.;
    if (locEta>=1.) locEta-=1.;

    int bin = nBins * locEta; 
    if (bin>nBins-1 || bin < 0 ) {
      cout << "Error in bins: " << bin << " " << scLocalEta << endl;
      continue;
    }

    
    float correction = scLocalCorr_DK;
    crackCorr = 1;
    (h_EoP_MC[bin][mod]) -> Fill(EoP/crackCorr);
    (h_EoC_MC[bin][mod]) -> Fill(EoP/crackCorr*correction);

  }
  cout << "stat: "<< h_EoP_MC[0][10]->GetEntries()<< " " <<  h_EoC_MC[0][10]->GetEntries() << endl; 

  

  ////////////////////////////////////////    Data    ///////////////////////////////////////////////
  std::cout << "Loop on Data events ..." << endl; 
  // loop on data
  for(int entry = 0; entry < ntu_Data->GetEntries(); ++entry) {
    if( entry%200000 == 0 ) std::cout << "reading saved entry " << entry << std::endl;
    //    if (entry>1000) break;

    ntu_Data->GetEntry(entry);

    // redefine E/p using Photn Energy (needed to work with DK corrections)
    EoP = EoP/scEne*phE;

    // eta or R9 cuts
    if( fabs(scEta) > etaMax) continue;
    if ( phR9 < r9min || phR9 > r9max ) continue; 

    // cut phi cracks
    float phi = (scPhi+3.1415926536)/xtalWidth;
    float modphi = (int)phi%20;
    if (fabs(modphi-10)<2.) continue;
    
    // use only even/odd crystals (for data)
    if ( useOddCry  && int(modphi)%2 == 0) continue; 
    if ( useEvenCry && int(modphi)%2 != 0) continue; 
    
    // cut eta gaps
    float fetaCry = fabs (scEta) / xtalWidth;
    if( IsEtaGap(fetaCry) ) continue;

    int mod = templIndex(fetaCry);
    
    // fill MC histos in eta bins
    //float locEta = scLocalEta+0.5; 
    float locEta = etaC_DK/xtalWidth + 0.5;
    if (locEta<0.) locEta+=1.;
    if (locEta>=1.) locEta-=1.;
    
    int bin = nBins * locEta; 
    if (bin>nBins-1 || bin < 0 ) {
      cout << "Error in bins: " << bin << " " << scLocalEta << endl;
      continue;
    }

    
    float correction = scLocalCorr_DK;
    crackCorr = 1;
    
    (h_EoP_Data[bin][mod]) -> Fill(EoP/crackCorr);
    (h_EoC_Data[bin][mod]) -> Fill(EoP/crackCorr*correction);
    
  }
  
  //cout << "stat: "<< h_EoP_Data[10][0]->GetEntries()<< " " <<  h_EoC_Data[10][0]->GetEntries() << endl; 

  /////////////// Fit the histograms and fill the graphs ////////////////////////
  int rebin = 8;

  TGraphErrors* g_EoP_MC[Ntempl];
  TGraphErrors* g_EoC_MC[Ntempl];
  TGraphErrors* g_ratio_MC[Ntempl];
  
  TGraphErrors* g_EoP_Data[Ntempl];
  TGraphErrors* g_EoC_Data[Ntempl];
  TGraphErrors* g_ratio_Data[Ntempl];

  for (int mod=0; mod<4; mod++){
    char histoName[100];
    
    sprintf(histoName, "gEoP_MC_mod%d", mod+1);
    g_EoP_MC[mod]   = new TGraphErrors(); g_EoP_MC[mod]->SetName(histoName);
    sprintf(histoName, "gEoC_MC_mod%d", mod+1);
    g_EoC_MC[mod]   = new TGraphErrors(); g_EoC_MC[mod]->SetName(histoName);
    sprintf(histoName, "gRatio_MC_mod%d", mod+1);
    g_ratio_MC[mod]   = new TGraphErrors(); g_ratio_MC[mod]->SetName(histoName);
    
    sprintf(histoName, "gEoP_Data_mod%d", mod+1);
    g_EoP_Data[mod]   = new TGraphErrors(); g_EoP_Data[mod]->SetName(histoName);
    sprintf(histoName, "gEoC_Data_mod%d", mod+1);
    g_EoC_Data[mod]   = new TGraphErrors(); g_EoC_Data[mod]->SetName(histoName);
    sprintf(histoName, "gRatio_Data_mod%d", mod+1);
    g_ratio_Data[mod]   = new TGraphErrors(); g_ratio_Data[mod]->SetName(histoName);
    
    h_template[mod] -> Rebin(rebin);

  }
  
  TCanvas *cc = new TCanvas("cc");
  cc->Divide(2,2); 

  /////////////  MC ////////////////////
    
  for(int mod=0;mod<4;mod++){
    histoFunc *templateHistoFunc = new histoFunc(h_template[mod]);
    for(unsigned int i = 0; i < nBins; ++i)
      {
	std::cout << "***** Fitting :  mod: " <<mod <<"  bin:  "<< i << endl; 

	h_EoP_MC[i][mod] -> Rebin(rebin);    
	h_EoC_MC[i][mod] -> Rebin(rebin);    
	h_EoP_Data[i][mod] -> Rebin(rebin);    
	h_EoC_Data[i][mod] -> Rebin(rebin);    


	TF1 * templateFunc = new TF1("templateFunc", templateHistoFunc, 0.7, 1.3, 3, "histoFunc");
	templateFunc -> SetNpx(10000);

	////////////////     MC ///////////////////////

	// uncorrected MC   
	double xNorm = h_EoP_MC[i][mod]->GetEntries()/h_template[mod]->GetEntries() *
	  h_EoP_MC[i][mod]->GetBinWidth(1)/h_template[mod]->GetBinWidth(1); 

	templateFunc -> FixParameter(0, xNorm);
	templateFunc -> SetParameter(1, 1.05 );
	templateFunc -> FixParameter(2, 0.);
    

	h_EoP_MC[i][mod] -> Fit("templateFunc", "MRQLN+");
	g_EoP_MC[mod] -> SetPoint(i, (i+0.5)*1/(float)nBins , 1./templateFunc->GetParameter(1));
	g_EoP_MC[mod] -> SetPointError(i, 0., templateFunc->GetParError(1));
	//    if ( templateFunc->GetParError(1) < 0.003) g_EoP -> SetPointError(i, 0., 0.003);
	//    cout  << " ***** " <<  1./templateFunc->GetParameter(1) << " " << templateFunc->GetParError(1) << endl; 

	//ratio preparation
	float ratioMC  = templateFunc->GetParameter(1);
	float eratioMC = templateFunc->GetParError(1); 

	//corrected MC  
	xNorm = h_EoC_MC[i][mod]->GetEntries()/h_template[mod]->GetEntries() *
	  h_EoC_MC[i][mod]->GetBinWidth(1)/h_template[mod]->GetBinWidth(1); 

	templateFunc -> FixParameter(0, xNorm);
	templateFunc -> SetParameter(1, 1.05 );
	templateFunc -> FixParameter(2, 0.);
    

	h_EoC_MC[i][mod] -> Fit("templateFunc", "MRQLN+");
	g_EoC_MC[mod] -> SetPoint(i, (i+0.5)*1/(float)nBins , 1./templateFunc->GetParameter(1));
	g_EoC_MC[mod] -> SetPointError(i, 0., templateFunc->GetParError(1));
    
	//ratio finalization
	ratioMC /= templateFunc->GetParameter(1);
	eratioMC = ratioMC*sqrt(eratioMC*eratioMC + templateFunc->GetParError(1)*templateFunc->GetParError(1)); 
	g_ratio_MC[mod] -> SetPoint(i, (i+0.5)*1/(float)nBins , ratioMC);
	g_ratio_MC[mod] -> SetPointError(i, 0., eratioMC);
    

	/////////////////////////// data //////////////////////////////////////


	// uncorrected Data   
	double xNorm = h_EoP_Data[i][mod]->GetEntries()/h_template[mod]->GetEntries() *
	  h_EoP_Data[i][mod]->GetBinWidth(1)/h_template[mod]->GetBinWidth(1); 

	templateFunc -> FixParameter(0, xNorm);
	templateFunc -> SetParameter(1, 1.05 );
	templateFunc -> FixParameter(2, 0.);
    

	h_EoP_Data[i][mod] -> Fit("templateFunc", "MRQLN+");
	g_EoP_Data[mod] -> SetPoint(i, (i+0.5)*1/(float)nBins , 1./templateFunc->GetParameter(1));
	g_EoP_Data[mod] -> SetPointError(i, 0., templateFunc->GetParError(1));
	//    if ( templateFunc->GetParError(1) < 0.003) g_EoP -> SetPointError(i, 0., 0.003);
	//    cout  << " ***** " <<  1./templateFunc->GetParameter(1) << " " << templateFunc->GetParError(1) << endl; 

	//ratio preparation
	float ratioDA = templateFunc->GetParameter(1);
	float eratioDA = templateFunc->GetParError(1); 

	//corrected Data  
	xNorm = h_EoC_Data[i][mod]->GetEntries()/h_template[mod]->GetEntries() *
	  h_EoC_Data[i][mod]->GetBinWidth(1)/h_template[mod]->GetBinWidth(1); 

	templateFunc -> FixParameter(0, xNorm);
	templateFunc -> SetParameter(1, 1.05 );
	templateFunc -> FixParameter(2, 0.);
    

	h_EoC_Data[i][mod] -> Fit("templateFunc", "MRQLN+");
	g_EoC_Data[mod] -> SetPoint(i, (i+0.5)*1/(float)nBins , 1./templateFunc->GetParameter(1));
	g_EoC_Data[mod] -> SetPointError(i, 0., templateFunc->GetParError(1));

	//ratio finalization
	ratioDA /= templateFunc->GetParameter(1);
	eratioDA = ratioDA*sqrt(eratioDA*eratioDA+ templateFunc->GetParError(1)*templateFunc->GetParError(1)); 
	g_ratio_Data[mod] -> SetPoint(i, (i+0.5)*1/(float)nBins , ratioDA);
	g_ratio_Data[mod] -> SetPointError(i, 0., eratioDA);
    
      }

  }

  TCanvas* c_g_fit[Ntempl];
  TLegend* tl[Ntempl];

  for(int mod=0;mod<4;mod++) {
    char padName[100];
    sprintf(padName, "g_fit_mod%d", mod+1);
    c_g_fit[mod] = new TCanvas(padName,padName,100,100,700,500);
    
    c_g_fit[mod]->cd(1);
    gPad->SetGrid();
    TH1F *hPad = (TH1F*)gPad->DrawFrame(-0.1,0.93,1.1,1.03);
    hPad->GetXaxis()->SetTitle("#eta_{SC} (deg)");
    hPad->GetYaxis()->SetTitle("Relative E/p scale");
 
    g_EoP_MC[mod] -> SetMarkerStyle(20);
    g_EoP_MC[mod] -> SetMarkerSize(.7);
    g_EoP_MC[mod] -> SetMarkerColor(kRed); 
    g_EoP_MC[mod] -> Draw("PL");
    g_EoC_MC[mod] -> SetMarkerStyle(20);
    g_EoC_MC[mod] -> SetMarkerSize(.7);
    g_EoC_MC[mod] -> SetMarkerColor(kRed+2); 
    g_EoC_MC[mod] -> Draw("PL");
  
    g_EoP_Data[mod] -> SetMarkerStyle(20);
    g_EoP_Data[mod] -> SetMarkerSize(.7);
    g_EoP_Data[mod] -> SetMarkerColor(kGreen); 
    g_EoP_Data[mod] -> Draw("PL");
    g_EoC_Data[mod] -> SetMarkerStyle(20);
    g_EoC_Data[mod] -> SetMarkerSize(.7);
    g_EoC_Data[mod] -> SetMarkerColor(kGreen+2); 
    g_EoC_Data[mod] -> Draw("PL");
  

    g_ratio_MC[mod]->SetLineColor(kBlue+2);
    g_ratio_MC[mod]->SetMarkerColor(kBlue+2);
    g_ratio_MC[mod]->SetMarkerStyle(20);
    g_ratio_MC[mod]->SetMarkerSize(0.5);
    g_ratio_MC[mod]->Draw("PL");
    
    g_ratio_Data[mod]->SetLineColor(kBlue);
    g_ratio_Data[mod]->SetMarkerColor(kBlue);
    g_ratio_Data[mod]->SetMarkerStyle(20);
    g_ratio_Data[mod]->SetMarkerSize(0.5);
    g_ratio_Data[mod]->Draw("PL");

    tl[mod] = new TLegend(0.80,0.85,1.01,1.01);
    tl[mod] -> SetFillColor(0);
    tl[mod] -> AddEntry(g_EoP_MC[mod],"MC uncorrected","PL");
    tl[mod] -> AddEntry(g_EoC_MC[mod],"MC corrected","PL");
    tl[mod] -> AddEntry(g_EoP_Data[mod],"Data uncorrected","PL");
    tl[mod] -> AddEntry(g_EoC_Data[mod],"Data corrected","PL");
    tl[mod] -> Draw();
    
  }

  TFile fout(outfilename,"recreate");
  for(int mod=0;mod<4;mod++) {
    g_EoP_MC[mod]->Write();
    g_EoC_MC[mod]->Write();
    g_EoP_Data[mod]->Write();
    g_EoC_Data[mod]->Write();
    g_ratio_MC[mod]->Write();
    g_ratio_Data[mod]->Write();
    
  }
  fout.Close();
}

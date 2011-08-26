typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > PtEtaPhiELorentzVector;

using namespace RooFit;

double crystalBall(double* x, double* par);

int modId(float eta){
  int ieta  = fabs(eta)/0.01745329;
  int im = 0;
  if (ieta>25) im = 1;
  if (ieta>45) im = 2;
  if (ieta>65) im = 3;
  return (im);
}

bool IsEtaGap(float eta){
  int ieta  = fabs(eta)/0.01745329;
  float feta = fabs(ieta);
  if( fabs(feta - 0 )<2) return true;
  if( fabs(feta - 25)<2) return true;
  if( fabs(feta - 45)<2) return true;
  if( fabs(feta - 65)<2) return true;
  if( fabs(feta - 85)<2) return true;
  return false;
}


void makeZPeak()
{

  gROOT->SetStyle("Plain");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110);
  gStyle->SetOptFit(1);
  
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.03);
  gStyle->SetTitleFont(42,"xyz");
  gStyle->SetTitleSize(0.03);
  gStyle->SetLabelFont(42,"xyz");
  gStyle->SetLabelSize(0.03);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.03);
  gROOT->ForceStyle();

  float xtalWidth = 0.01745329;
  float zmass = 91.187;

  //---- variables for selections
  float etaMax = 1.44;
  
  float r9min = 0. ;
  float r9max = 999 ;
  
  //---- output file to save graphs
  char outfilename[100];
  sprintf(outfilename,"mcZpeak_DKcorr_all.root");

  //----

  // Get trees
  TChain *ntu_DA = new TChain("ntu");
  //ntu_DA->Add("/data2/ghezzi/Calib/PhotonDataDat/WZAnalysis_DoubleElectron_Run2011A-ZElectron-05Jul2011.root");
  //ntu_DA->Add("/data2/ghezzi/Calib/PhotonDataDat/WZAnalysis_DoubleElectron_Run2011A-ZElectron-05Aug2011.root");
 

 //  ntu_DA->Add("/data2/ghezzi/Calib/PhotonDataDat/WZAnalysis_ZElectron_05Jul2011ReReco_ter.root");
//   ntu_DA->Add("/data2/ghezzi/Calib/PhotonDataDat/WZAnalysis_ZElectron_05Aug2011_ter.root");

  ntu_DA->Add("/data2/ghezzi/Calib/PhotonMCDat/WZAnalysis_DYToEE_ter.root");
 
   
  std::cout << "     Data  : " << ntu_DA->GetEntries() << " entries in  Data  sample" << std::endl;
  std::cout << std::endl;
  
  // Set branch addresses  
  int runId,isZ;
  int PV_n;
  int isEB,isEB2;
  float mZ, mZc;
  
  float p, p2;
  float scE,scE2;
  float scET,scET2;
  float scEta,scEta2;
  float scPhi,scPhi2;
  float eta,eta2;
  float phi,phi2;
  float scLocalEta, scLocalEta2;
  float scLocalPhi, scLocalPhi2;

  float scLocalContCorr, scLocalContCorr2;
  float scLocalCorr_DK, scLocalCorr_DK2;
  float etaC_DK, phiC_DK;
  float etaC_DK2, phiC_DK2;

  float phR9,phR9_2;
  float phE,phE2;
  
  float charge, charge2;

  ntu_DA->SetBranchAddress("isZ",              &isZ);
  ntu_DA->SetBranchAddress("ele1_isEB",        &isEB);
  ntu_DA->SetBranchAddress("ele2_isEB",        &isEB2);
  ntu_DA->SetBranchAddress("ele1_scE",         &scE);
  ntu_DA->SetBranchAddress("ele2_scE",         &scE2);
  ntu_DA->SetBranchAddress("ele1_scEta",       &scEta);
  ntu_DA->SetBranchAddress("ele2_scEta",       &scEta2);
  ntu_DA->SetBranchAddress("ele1_scPhi",       &scPhi);
  ntu_DA->SetBranchAddress("ele2_scPhi",       &scPhi2);
  ntu_DA->SetBranchAddress("ele1_eta",         &eta);
  ntu_DA->SetBranchAddress("ele2_eta",         &eta2);
  ntu_DA->SetBranchAddress("ele1_phi",         &phi);
  ntu_DA->SetBranchAddress("ele2_phi",         &phi2);
  ntu_DA->SetBranchAddress("ele1_scLocalPhi",  &scLocalPhi); 
  ntu_DA->SetBranchAddress("ele2_scLocalPhi",  &scLocalPhi2); 
  ntu_DA->SetBranchAddress("ele1_scLocalEta",  &scLocalEta); 
  ntu_DA->SetBranchAddress("ele2_scLocalEta",  &scLocalEta2); 
  ntu_DA->SetBranchAddress("ele1_scLocalContCorr",  &scLocalContCorr); 
  ntu_DA->SetBranchAddress("ele2_scLocalContCorr",  &scLocalContCorr2); 
  ntu_DA->SetBranchAddress("ele1_scLocalContCorr_DK",&scLocalCorr_DK); 
  ntu_DA->SetBranchAddress("ele2_scLocalContCorr_DK",&scLocalCorr_DK2); 
  ntu_DA->SetBranchAddress("ele1_etaC_DK",&etaC_DK); 
  ntu_DA->SetBranchAddress("ele1_phiC_DK",&phiC_DK); 
  ntu_DA->SetBranchAddress("ele2_etaC_DK",&etaC_DK2); 
  ntu_DA->SetBranchAddress("ele2_phiC_DK",&phiC_DK2); 
  ntu_DA->SetBranchAddress("ele1_ph_E",&phE); 
  ntu_DA->SetBranchAddress("ele2_ph_E",&phE2);
  ntu_DA->SetBranchAddress("ele1_ph_R9",&phR9); 
  ntu_DA->SetBranchAddress("ele2_ph_R9",&phR9_2); 
  ntu_DA->SetBranchAddress("ele1_charge",&charge); 
  ntu_DA->SetBranchAddress("ele2_charge",&charge2);
  ntu_DA->SetBranchAddress("ele1_p",&p); 
  ntu_DA->SetBranchAddress("ele2_p",&p2);
  

  // Define histograms
  TH1F* h_mZ_EBEB = new TH1F("h_mZ_EBEB", "",200,65.,115.);
  TH1F* h_mZ_EEEE = new TH1F("h_mZ_EEEE", "",200,65.,115.);
  TH1F* h_mZ_EBEE = new TH1F("h_mZ_EBEE", "",200,65.,115.);

  TH1F* h_mZc_EBEB = new TH1F("h_mZc_EBEB", "",200,65.,115.);
  TH1F* h_mZc_EEEE = new TH1F("h_mZc_EEEE", "",200,65.,115.);  
  TH1F* h_mZc_EBEE = new TH1F("h_mZc_EBEE", "",200,65.,115.);
  
  TProfile *mZ_vs_localEta_EBEB = new TProfile("mZ_vs_localEta_EBEB", "",20,0,1,0.,2.);
  mZ_vs_localEta_EBEB->SetMarkerStyle(20);
  mZ_vs_localEta_EBEB->SetMarkerColor(kRed);
  mZ_vs_localEta_EBEB->SetLineColor(kRed);
  mZ_vs_localEta_EBEB-> GetXaxis()->SetTitle("#eta_{SC} (deg)");
  mZ_vs_localEta_EBEB-> GetYaxis()->SetTitle("< M_{ee} > / M_{Z}");

  TProfile *mZc_vs_localEta_EBEB = new TProfile("mZc_vs_localEta_EBEB", "",20,0,1,0.,2.);
  mZc_vs_localEta_EBEB->SetMarkerStyle(20);
  mZc_vs_localEta_EBEB->SetMarkerColor(kGreen+2);
  mZc_vs_localEta_EBEB->SetLineColor(kGreen+2);
  mZc_vs_localEta_EBEB-> GetXaxis()->SetTitle("#eta_{SC} (deg)");
  mZc_vs_localEta_EBEB-> GetYaxis()->SetTitle("< M_{ee} > / M_{Z}");

  TProfile *mZ_vs_localPhi_EBEB = new TProfile("mZ_vs_localPhi_EBEB", "",20,0,1,0.,2.);
  mZ_vs_localPhi_EBEB->SetMarkerStyle(20);
  mZ_vs_localPhi_EBEB->SetMarkerColor(kRed);
  mZ_vs_localPhi_EBEB->SetLineColor(kRed);
  mZ_vs_localPhi_EBEB-> GetXaxis()->SetTitle("#phi_{SC} (deg)");
  mZ_vs_localPhi_EBEB-> GetYaxis()->SetTitle("< M_{ee} > / M_{Z}");

  TProfile *mZc_vs_localPhi_EBEB = new TProfile("mZc_vs_localPhi_EBEB", "",20,0,1,0.,2.);
  mZc_vs_localPhi_EBEB->SetMarkerStyle(20);
  mZc_vs_localPhi_EBEB->SetMarkerColor(kGreen+2);
  mZc_vs_localPhi_EBEB->SetLineColor(kGreen+2);
  mZc_vs_localPhi_EBEB-> GetXaxis()->SetTitle("#phi_{SC} (deg)");
  mZc_vs_localPhi_EBEB-> GetYaxis()->SetTitle("< M_{ee}> /M_{Z}");

  TProfile *mZ_vs_Eta_EBEB = new TProfile("mZ_vs_Eta_EBEB", "",50,-1.5,1.5,0.,2.);
  mZ_vs_Eta_EBEB->SetMarkerStyle(20);
  mZ_vs_Eta_EBEB->SetMarkerColor(kRed);
  mZ_vs_Eta_EBEB->SetLineColor(kRed);
  mZ_vs_Eta_EBEB-> GetXaxis()->SetTitle("#eta_{SC}");
  mZ_vs_Eta_EBEB-> GetYaxis()->SetTitle("< M_{ee} > / M_{Z}");

  TProfile *mZc_vs_Eta_EBEB = new TProfile("mZc_vs_Eta_EBEB", "",50,-1.5,1.5,0.,2.);
  mZc_vs_Eta_EBEB->SetMarkerStyle(20);
  mZc_vs_Eta_EBEB->SetMarkerColor(kGreen+2);
  mZc_vs_Eta_EBEB->SetLineColor(kGreen+2);
  mZc_vs_Eta_EBEB-> GetXaxis()->SetTitle("#eta_{SC}");
  mZc_vs_Eta_EBEB-> GetYaxis()->SetTitle("< M_{ee} > / M_{Z}");

  TH1F *hspread = new TH1F("hspread","spread",800,0.8,1.2);
  TH1F *hspreadc = new TH1F("hspreadc","spread",800,0.8,1.2);

  //define local corrections
  TF1 * fData[4];
  TF1 * fMC[4];
  for(int ii=0;ii<4; ii++){
    char fName[80];
    sprintf(fName, "fData_%d", ii);
    fData[ii] = new TF1(fName,"[0]+([1]*(x-0.5)-[2]*pow(x-0.5,2)-[3]*pow(x-0.5,3)-[4]*pow(x-0.5,4))",0.,1.);
    sprintf(fName, "fMC_%d", ii);
    fMC[ii] = new TF1(fName,"[0]+([1]*(x-0.5)-[2]*pow(x-0.5,2)-[3]*pow(x-0.5,3)-[4]*pow(x-0.5,4))",0.,1.);
  }
  //Data
  //   //mod1
  //   fData[0]->SetParameters(1.00435,0.00527302,0.0733149,0.0123797,-0.00348826);
  //   //mod2
  //   fData[1]->SetParameters(1.00495,0.00749243,0.086362,0.0175724,-0.0181895);
  //   //mod3
  //   fData[2]->SetParameters(1.00667,0.0131371,0.125638,0.034474,-0.125617);
  //   //mod4
  //   fData[3]->SetParameters(1.01072,0.0179107,0.211964,0.0750981,-0.331104);
  //   //-----
  
  fData[0]->SetParameters(1.00564,1.24898e-03,5.28901e-02,-1.37391e-03,1.18978e-01); 
  fData[1]->SetParameters(1.00564,1.24898e-03,5.28901e-02,-1.37391e-03,1.18978e-01); 
  fData[2]->SetParameters(1.00564,1.24898e-03,5.28901e-02,-1.37391e-03,1.18978e-01); 
  fData[3]->SetParameters(1.00564,1.24898e-03,5.28901e-02,-1.37391e-03,1.18978e-01); 

  //MC
  //mod1
  fMC[0]->SetParameters(1.00251,-0.000418213,0.0484353,-0.011998,0.00278462);
  //mod2
  fMC[1]->SetParameters(1.00198,0.00349762,0.0466435,0.00418743,-0.00994096);
  //mod3
  fMC[2]->SetParameters(1.00223,0.00640348,0.0407435,0.0240559,0.0722518);
  //mod4
  fMC[3]->SetParameters(1.00469,0.00494316,0.117714,0.0110962,-0.174621); 
  
  
  // Loop over entries
  int nEntries = ntu_DA -> GetEntriesFast();
  //nEntries = 20000;
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%10000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_DA -> GetEntry(ientry); 
    
    if( isZ == 0 ) continue;
    
    // eta cut 
    if (isEB==0 || isEB2==0) continue;
    if( fabs(scEta) > etaMax ) continue;
    if( fabs(scEta2) > etaMax ) continue;
   
    // remove eta gaps
    if ( IsEtaGap(scEta) || IsEtaGap(scEta2) ) continue;
       
    // remove phi cracks
    float myphi = (scPhi+3.1415926536)/xtalWidth;
    float modphi = (int)myphi%20;
    if (fabs(modphi-10)<2.) continue;
    float myphi2 = (scPhi2+3.1415926536)/xtalWidth;
    float modphi2 = (int)myphi2%20;
    if (fabs(modphi2-10)<2.) continue;

    // select on r9
    if ( phR9 < r9min || phR9 > r9max ) continue; 
    if ( phR9_2 < r9min || phR9_2 > r9max ) continue; 

    // use photon energy for DK corrections
    scE  = phE;
    scE2 = phE2;
    

    PtEtaPhiELorentzVector sc1(scE *sin(2*atan(exp(-1.*eta))), eta, phi, scE);
    PtEtaPhiELorentzVector sc2(scE2*sin(2*atan(exp(-1.*eta2))),eta2,phi2,scE2);
    mZ = (sc1+sc2).mass();
 
    
    // corrected energy : corrections derived on E/p
    //     int mod = modId(scEta);
    //     int mod2 = modId(scEta2);
    //     float scEc  = scE/fData[mod]->Eval(scLocalEta+0.5);
    //     float scE2c = scE2/fData[mod2]->Eval(scLocalEta2+0.5);
    
    // corrected energy : local containement corrections (Tourner)
    //     float scEc  = scE*scLocalContCorr;
    //     float scE2c = scE2*scLocalContCorr2;

    
    //corrected energy : use DK corrections
    float scEc  = scE*scLocalCorr_DK;
    float scE2c = scE2*scLocalCorr_DK2;
    
        
    PtEtaPhiELorentzVector sc1c(scEc *sin(2*atan(exp(-1.*eta))), eta, phi, scEc);
    PtEtaPhiELorentzVector sc2c(scE2c*sin(2*atan(exp(-1.*eta2))),eta2,phi2,scE2c);
    mZc = (sc1c+sc2c).mass();

    scLocalEta = etaC_DK/xtalWidth;
    scLocalEta2 = etaC_DK2/xtalWidth;
    
    //float locPhi = scLocalPhi + 0.5;
    float locPhi = phiC_DK/xtalWidth + 0.5;
//     if (locPhi<0.) locPhi+=1.;
//     if (locPhi>=1.) locPhi-=1.;
        
    //float locPhi2 = scLocalPhi2 + 0.5; 
    float locPhi2 = phiC_DK2/xtalWidth + 0.5;
//     if (locPhi2<0.) locPhi2+=1.;
//     if (locPhi2>=1.) locPhi2-=1.;

    if( (isEB == 1) && (isEB2 == 1) )
    {
      h_mZ_EBEB -> Fill(mZ);
      h_mZc_EBEB -> Fill(mZc);

      mZ_vs_Eta_EBEB->Fill(scEta, mZ/zmass);
      mZc_vs_Eta_EBEB->Fill(scEta, mZc/zmass);

      mZ_vs_localEta_EBEB  -> Fill(scLocalEta+0.5,mZ/zmass);
      mZc_vs_localEta_EBEB -> Fill(scLocalEta+0.5,mZc/zmass);
      mZ_vs_localEta_EBEB  -> Fill(scLocalEta2+0.5,mZ/zmass);
      mZc_vs_localEta_EBEB -> Fill(scLocalEta2+0.5,mZc/zmass);

      if ( (scEta*charge) > 0  ){
	mZ_vs_localPhi_EBEB  -> Fill(locPhi,mZ/zmass);
	mZc_vs_localPhi_EBEB -> Fill(locPhi,mZc/zmass);
      }
      
      if ( (scEta2*charge2) > 0){
	mZ_vs_localPhi_EBEB -> Fill(locPhi2,mZ/zmass);
	mZc_vs_localPhi_EBEB -> Fill(locPhi2,mZc/zmass);
      }
    }
  }
  
  
  for (int i =0 ; i< mZ_vs_localEta_EBEB->GetNbinsX(); i++){
    hspread  ->Fill(mZ_vs_localEta_EBEB->GetBinContent(i+1));
    hspreadc ->Fill(mZc_vs_localEta_EBEB->GetBinContent(i+1));
  }


  TFile *fout = new TFile(outfilename,"recreate");
  h_mZ_EBEB->Write();
  h_mZc_EBEB->Write();
  mZ_vs_localEta_EBEB->Write();
  mZc_vs_localEta_EBEB->Write();
  mZ_vs_localPhi_EBEB->Write();
  mZc_vs_localPhi_EBEB->Write();
  hspread->Write();
  hspreadc->Write();
  fout->Close();

  int rebin = 1;

  TLegend   *tl = new TLegend(0.60,0.15,0.89,0.35);
  tl-> SetFillColor(0);
  tl-> AddEntry(mZ_vs_localEta_EBEB,"uncorrected","PL");
  tl-> AddEntry(mZc_vs_localEta_EBEB,"corrected","PL");
    
  TCanvas* c1 = new TCanvas("c1","c1",100,100,700,500);
  c1->cd();
  c1->SetGridx();
  c1->SetGridy();
  mZ_vs_localEta_EBEB->GetYaxis()->SetRangeUser(0.96,1.02);
  mZ_vs_localEta_EBEB->Draw("");
  mZc_vs_localEta_EBEB->Draw("same");
  tl-> Draw("same");

  TCanvas* c2 = new TCanvas("c2","c2",100,100,700,500);
  c2->cd();
  c2->SetGridx();
  c2->SetGridy();
  mZ_vs_localPhi_EBEB->GetYaxis()->SetRangeUser(0.96,1.02);
  mZ_vs_localPhi_EBEB->Draw("");
  mZc_vs_localPhi_EBEB->Draw("same");
  tl-> Draw("same");

//   TCanvas* c3 = new TCanvas("c3","c3",100,100,700,500);
//   c3->cd();
//   c3->SetGridx();
//   c3->SetGridy();
//   mZ_vs_Eta_EBEB->GetYaxis()->SetRangeUser(0.96,1.02);
//   mZ_vs_Eta_EBEB->Draw("");
//   mZc_vs_Eta_EBEB->Draw("same");
//   tl-> Draw("same");

  //------------------------
  //---------------- fitting

  RooRealVar  mass("mass","M(e^{+}e^{-})", -120, 120.0,"GeV/c^{2}");
  mass.setBins(10000) ;


  // Parameters for Crystal Ball Lineshape 
  RooRealVar  dm("#Delta m", "offset", 0.0, -1.0, 1.0,"GeV/c^{2}"); 
  RooRealVar  sigma("#sigma_{CB}","sigmaCB", 1.5,0.5,2.5,"GeV/c^{2}"); 
  RooRealVar  alpha("#alpha","alpha", 1.607,0.6,2.0); 
  RooRealVar  n("n","n", 1.7, 0.5, 100.0); 
  //alpha.setConstant();
  //n.setConstant();

  // Parameters for Breit-Wigner Distribution
  RooRealVar  MZ("M_{Z}", "M_{Z}", 91.188, 80.0, 100.0,"GeV/c^{2}"); 
  RooRealVar  Gamma("#Gamma", "#Gamma", 2.45, 2.0,3.0,"GeV/c^{2}"); 
  MZ.setConstant();
  Gamma.setConstant();

  // Exponencial Background
  RooRealVar  bkgshape("bkgshape", "Backgroung Shape", -0.1,-1.0,0.0, "1/GeV/c^{2}");
  RooRealVar  frac("frac", "Signal Fraction", 1.0,0.0,1.0);
  frac.setConstant();
  bkgshape.setConstant();
  
  // Crystal Ball Lineshape
  RooCBShape     cb("cb", "Crystal Ball Lineshape", mass, dm,sigma, alpha, n);
  // Breit-Wigner Distribution
  RooBreitWigner bw("bw","A Breit-Wigner Distribution",mass,MZ,Gamma);
  
  // Convolution p.d.f. using numeric convolution operator based on Fourier Transforms
  RooFFTConvPdf bwcb("bwcb","convolution", mass, bw, cb);
  
  // Background  p.d.f.
  RooExponential bkg("bkg", "Backgroung Distribution", mass, bkgshape);
  
  
  
  RooDataHist hdata("hdata","hdata",RooArgSet(mass), h_mZ_EBEB);
  RooAddPdf   model("model", "Signal + Background", bwcb, bkg, frac);  
  model.fitTo(hdata, Range(70., 110.) );
  TCanvas *c = new TCanvas("c","c",700,700);
  c->SetLeftMargin(0.15);
  RooPlot* plot = mass.frame(Range(70,110));
  hdata.plotOn(plot);
  model.plotOn(plot);
  model.plotOn(plot, LineColor(kRed));
  model.paramOn(plot);
  model.plotOn(plot, LineColor(kRed));
  plot->SetTitleOffset(1.7,"Y");
  plot->Draw();

  
  RooDataHist hdatac("hdatac","hdatac",RooArgSet(mass), h_mZc_EBEB);
  RooAddPdf   modelc("modelc", "Signal + Background", bwcb, bkg, frac);  
  modelc.fitTo(hdatac, Range(70., 110.));
  // Plot the fit results
  TCanvas *cc = new TCanvas("cc","cc",700,700);
  cc->SetLeftMargin(0.15);
  RooPlot* plotc = mass.frame(Range(70,110));
  hdatac.plotOn(plotc);
  modelc.plotOn(plotc, LineColor(kGreen+2));
  modelc.paramOn(plotc, Layout(0.15,0.5,0.89));
  modelc.plotOn(plotc, LineColor(kGreen+2));
  plotc->SetTitleOffset(1.7,"Y");
  plotc->Draw();

}






double crystalBall(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  
  
  double xx = x[0];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  
  if( (xx-mean)/sigma > -alpha )
  {
    return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
  }
  
  else
  {
    double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
    double B = n/fabs(alpha) - fabs(alpha);
    
    return par[0] * A * pow(B - (xx-mean)/sigma, -1.*n);
  }
}

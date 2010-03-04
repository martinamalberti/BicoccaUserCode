{
  using namespace RooFit ;
  
  gROOT->ProcessLine(".x Style.C") ;
  
  tdrStyle->SetCanvasDefH(800); //600
  tdrStyle->SetCanvasDefW(800);//800 //Width of canvas

  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);

  TFile f("Weighted_evts_HLT.root");
  f.cd();

  TH1F *myhisto = (TH1F*) f.Get("hInvMass");
  
  myhisto -> Sumw2() ;

  int numbins = myhisto->GetNbinsX();

  RooRealVar x("x","m_{e^{+}e^{-}}",3.2,12.);
  x.setBins(numbins);

  //Histo of data after QQ->l+l- selection
  RooDataHist hdata("hdata","hdata",RooArgList(x),myhisto);
   
  TFile f_same("../ppEleX_SameSign.root");
  f_same.cd();

  TH1F *myhisto_same = (TH1F*) f_same.Get("h_1_InvMass");
  
  myhisto_same -> Sumw2() ;

  //Histo of data after QQ->l+l+(--) selection
  RooDataHist hdata_same("hdata_same","hdata_same",RooArgList(x),myhisto_same);

  RooRealVar a0("a0","a0",0.9,-50,50);
  RooRealVar a1("a1","a1",0.1,-100,100);
  RooRealVar a2("a2","a2",-0.1,-100,100);
  RooRealVar a3("a3","a3",-0.1,-100,100);
  RooRealVar a4("a4","a4",-0.1,-100,100);
  
  //Fit the ppEleX bkg (same sign) with a polynomial
  RooChebychev poly("poly","Background Same ppMuX",x,RooArgList(a0,a1,a2,a3,a4));
  poly.fitTo(hdata_same,RooFit::Extended(1),RooFit::Minos(1),RooFit::SumW2Error(kTRUE));

  //Fix the plynomial function. When Readapting it to the opposite sign plot only its normalization will be changed
  a0.setConstant(kTRUE) ;
  a1.setConstant(kTRUE) ;
  a2.setConstant(kTRUE) ;
  a3.setConstant(kTRUE) ;
  a4.setConstant(kTRUE) ;


  //Initialize Upsilon masses and errors
  RooRealVar mean1("mean1","Y1S mean",9.46,9.,9.6);
  RooRealVar mean2("mean2","Y2S mean",10.02,9.7,10.3);
  RooRealVar mean3("mean3","Y3S mean",10.35,10.,10.6);
//   RooRealVar mean1("mean1","Y1S mean",9.46,9.40,10.);
//   RooRealVar mean2("mean2","Y2S mean",10.02,10.01,10.2);
//   RooRealVar mean3("mean3","Y3S mean",10.35,10.3,10.6);

  RooRealVar sigma1("sigma1","Y1S width",0.2,0.1,0.3);
  RooRealVar sigma2("sigma2","Y2S width",0.2,0.1,0.3);
  RooRealVar sigma3("sigma3","Y3S width",0.2,0.1,0.3);

  RooGaussian pdf1S("pdf1S","pdf1S",x,mean1,sigma1);
  RooGaussian pdf2S("pdf2S","pdf2S",x,mean2,sigma2);
  RooGaussian pdf3S("pdf3S","pdf3S",x,mean3,sigma3);

  RooRealVar frac1("frac1","frac1",0.5,0.,1.);
  RooRealVar frac2("frac2","frac2",0.5,0.,1.);

  RooAddPdf Ytot1("Ytot1","Ytot1",RooArgList(pdf1S,pdf2S),RooArgList(frac1)); //frac1*1S + (1-frac1)*2S
  RooAddPdf Ytot("Ytot","Ytot",RooArgList(Ytot1,pdf3S),RooArgList(frac2));    //frac2*(1S+2S) + (1-frac2)*3S

  RooRealVar NY("NY","NY",5000.,0.,10000000.);
  RooRealVar Ncont("Ncont","Ncont",5000.,0.,10000000000.);

  RooAddPdf TOTpdf("TOTpdf","TOTpdf",RooArgList(Ytot,poly),RooArgList(NY,Ncont));//NY*Ytot + Ncont*poly 

  TOTpdf.fitTo(hdata,RooFit::Extended(1),RooFit::Minos(1),RooFit::SumW2Error(kTRUE));
  
  //Bkg subtraction
  RooDataHist *hgen_bkg = poly.generateBinned(RooArgSet(x),Ncont.getVal(),RooFit::Extended(1));
  TH1F* hbkg = (TH1F*)hgen_bkg->createHistogram("x");
  hbkg -> Sumw2() ;
  
  myhisto->Add(hbkg,-1);

  RooDataHist hdata_sub("hdata_sub","hdata_sub",RooArgList(x),myhisto);
  hdata_sub -> Sumw2() ;

  //Initialize Upsilon masses and errors
//   RooRealVar Mean1("Mean1","Y1S Mean",9.46,9.,9.6);
//   RooRealVar Mean2("Mean2","Y2S Mean",10.02,9.7,10.3);
//   RooRealVar Mean3("Mean3","Y3S Mean",10.35,10.,10.6);
  RooRealVar Mean1("Mean1","Y1S Mean",9.85,9.8,10.);
  RooRealVar Mean2("Mean2","Y2S Mean",10.45,10.35,10.55);
  RooRealVar Mean3("Mean3","Y3S Mean",10.9,10.8,10.99);

//   RooRealVar Sigma1("Sigma1","Y1S Width",0.2,0.1,0.3);
//   RooRealVar Sigma2("Sigma2","Y2S Width",0.2,0.1,0.3);
//   RooRealVar Sigma3("Sigma3","Y3S Width",0.2,0.1,0.3);
  RooRealVar Sigma1("Sigma1","Y1S Width",0.3,0.2,0.4);
  RooRealVar Sigma2("Sigma2","Y2S Width",0.3,0.2,0.4);
  RooRealVar Sigma3("Sigma3","Y3S Width",0.25,0.2,0.4);

  RooGaussian Pdf1S("Pdf1S","Pdf1S",x,Mean1,Sigma1);
  RooGaussian Pdf2S("Pdf2S","Pdf2S",x,Mean2,Sigma2);
  RooGaussian Pdf3S("Pdf3S","Pdf3S",x,Mean3,Sigma3);
  
  RooRealVar NY1("NY1","NY1",1000.,0.,10000000.);
  RooRealVar NY2("NY2","NY2",1000.,0.,10000000.);
  RooRealVar NY3("NY3","NY3",1000.,0.,10000000.);
  
  RooAddPdf YTot("YTot","YTot",RooArgList(Pdf1S,Pdf2S,Pdf3S),RooArgList(NY1,NY2,NY3));
  
  x.setRange("R1",9.,9.8) ;
  
//   YTot.fitTo(hdata_sub,RooFit::Extended(1),RooFit::Minos(1),RooFit::SumW2Error(kTRUE));

  Pdf1S.fitTo(hdata_sub,Range("R1"),RooFit::Extended(1),RooFit::Minos(1),RooFit::SumW2Error(kTRUE));
  
  RooPlot *xplot = x.frame();
  xplot->GetYaxis()->SetTitle("Events");
  hdata_sub.plotOn(xplot,DataError(RooAbsData::SumW2));
  
//   hdata_sub_red.plotOn(xplot,DataError(RooAbsData::SumW2));
  
  
//   YTot.plotOn(xplot,Components(RooArgSet(Pdf1S)),LineStyle(kDashed),LineColor(kPink)) ;
//   YTot.plotOn(xplot,Components(RooArgSet(Pdf2S,Pdf1S)),LineStyle(kDashed),LineColor(kYellow)) ;
//   YTot.plotOn(xplot,Components(RooArgSet(Pdf3S,Pdf2S,Pdf1S)),LineStyle(kDashed),LineColor(kGreen)) ;

  Pdf1S.plotOn(xplot, LineColor(kRed));
  
  TCanvas c0;
  c0.cd(0);
  xplot->Draw();

  cout << "norm" << Pdf1S.analyticalIntegral() << endl ;
  
}

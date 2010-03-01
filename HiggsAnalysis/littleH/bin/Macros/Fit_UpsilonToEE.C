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

  RooPlot *xplot = x.frame();
  xplot->GetYaxis()->SetTitle("Events");
  hdata.plotOn(xplot,DataError(RooAbsData::SumW2));
  
  TOTpdf.plotOn(xplot,Components(poly),LineStyle(kDashed),LineColor(kBlue)) ;

  TOTpdf.plotOn(xplot,Components(RooArgSet(pdf1S,poly)),LineStyle(kDashed),LineColor(kPink)) ;
  TOTpdf.plotOn(xplot,Components(RooArgSet(pdf2S,pdf1S,poly)),LineStyle(kDashed),LineColor(kYellow)) ;
  TOTpdf.plotOn(xplot,Components(RooArgSet(pdf3S,pdf2S,pdf1S,poly)),LineStyle(kDashed),LineColor(kGreen)) ;

  TOTpdf.plotOn(xplot, LineColor(kRed));

  //Plot the pull values, i.e. the difference between the direct measurement of the variable and its value as 
  //obtained from the least squares fit, normalized by dividing by the estimated error of this difference
  RooPlot* pullFrame = x.frame() ;
  pullFrame->addPlotable(xplot->pullHist()) ;
  pullFrame->SetMaximum(50.);
  pullFrame->SetMinimum(-50.);

  TCanvas c1;
  c1.Divide(1,2);

  c1.cd(1);
  double a,b,c,d ;

  a = c1_1.GetXlowNDC() ;
  b = c1_1.GetYlowNDC() ;
  c = a+ c1_1.GetWNDC() ;
  d = b+ c1_1.GetHNDC() ;
  c1_1. SetPad(a,b-0.3,c,d);


  xplot->Draw();

  c1.cd(2);
  
  a = c1_2.GetXlowNDC() ;
  b = c1_2.GetYlowNDC() ;
  c = a+ c1_2.GetWNDC() ;
  d = b+ c1_2.GetHNDC() ;
  c1_2. SetPad(a,b,c,d-0.3);

  pullFrame->Draw();

  c1.SaveAs("../RooFit/hmass_fit.gif");

  RooPlot *xplot_same = x.frame();
  xplot_same->GetYaxis()->SetTitle("Events");
  hdata_same.plotOn(xplot_same,DataError(RooAbsData::SumW2));

  poly.plotOn(xplot_same, LineColor(kRed));
  
  TCanvas c2("c2","c2");
  c2.Divide(1,2);

  c2.cd(1);

  a = c2_1.GetXlowNDC() ;
  b = c2_1.GetYlowNDC() ;
  c = a+ c2_1.GetWNDC() ;
  d = b+ c2_1.GetHNDC() ;
  c2_1. SetPad(a,b-0.3,c,d);

  xplot_same->Draw();

  c2.cd(2);

  a = c2_2.GetXlowNDC() ;
  b = c2_2.GetYlowNDC() ;
  c = a+ c2_2.GetWNDC() ;
  d = b+ c2_2.GetHNDC() ;
  c2_2. SetPad(a,b,c,d-0.3);

  RooPlot* pullFrame_same = x.frame() ;
  pullFrame_same->addPlotable(xplot_same->pullHist()) ;
  pullFrame_same->SetMaximum(5.);
  pullFrame_same->SetMinimum(-5.);
  pullFrame_same->Draw();

  c2.SaveAs("../RooFit/hmass_sameSign_fit.gif");

}

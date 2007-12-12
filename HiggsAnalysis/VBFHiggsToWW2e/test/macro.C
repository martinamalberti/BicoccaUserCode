{
  // delta eta fra i tag

  histo_deltaEtaTag_VVf.SetLineWidth (2) ;
  histo_deltaEtaTag_VVf.SetFillStyle (3003) ;
  histo_deltaEtaTag_VVf.SetFillColor (4) ;
  histo_deltaEtaTag_ggf.SetLineWidth (2) ;
  histo_deltaEtaTag_ggf.SetLineColor (108) ;
  histo_deltaEtaTag_Wt.SetLineWidth (2) ;
  histo_deltaEtaTag_WW.SetLineWidth (2) ;
  histo_deltaEtaTag_WZ.SetLineWidth (2) ;

  histo_deltaEtaTag_VVf.SetStats (0) ;
  histo_deltaEtaTag_ggf.SetStats (0) ;
  histo_deltaEtaTag_Wt.SetStats (0) ;
  histo_deltaEtaTag_WW.SetStats (0) ;
  histo_deltaEtaTag_WZ.SetStats (0) ;

  histo_deltaEtaTag_Wt.SetMarkerStyle (8) ;
  histo_deltaEtaTag_WW.SetMarkerStyle (21) ;
  histo_deltaEtaTag_WZ.SetMarkerStyle (22) ;

  histo_deltaEtaTag_VVf.GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_ggf.GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_Wt.GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_WW.GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_WZ.GetYaxis ()->SetTitle ("a.u.") ;

  histo_deltaEtaTag_VVf.SetTitle ("VBF signal (blue)") ;
  histo_deltaEtaTag_ggf.SetTitle ("VBF signal (blue) and ggf") ;
  histo_deltaEtaTag_Wt.SetTitle ("VBF signal (blue) and Wt") ;
  histo_deltaEtaTag_WW.SetTitle ("VBF signal (blue) and WW") ;
  histo_deltaEtaTag_WZ.SetTitle ("VBF signal (blue) and WZ") ;

  histo_deltaEtaTag_VVf.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_ggf.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_Wt.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_WW.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_WZ.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;

  TCanvas * c1 = new TCanvas ;
  histo_deltaEtaTag_ggf.Draw () ;
  histo_deltaEtaTag_VVf.Draw ("same") ;
  c1->Print ("deltaEtaTag_ggf.gif","gif") ;

  TCanvas * c2 = new TCanvas ;
  histo_deltaEtaTag_Wt.Draw ("P") ;
  histo_deltaEtaTag_VVf.Draw ("same") ;
  c2->Print ("deltaEtaTag_Wt.gif","gif") ;
  
  TCanvas * c3 = new TCanvas ;
  histo_deltaEtaTag_WW.Draw ("P") ;
  histo_deltaEtaTag_VVf.Draw ("same") ;
  c3->Print ("deltaEtaTag_WW.gif","gif") ;

  TCanvas * c4 = new TCanvas ;
  histo_deltaEtaTag_WZ.Draw ("P") ;
  histo_deltaEtaTag_VVf.Draw ("same") ;
  c4->Print ("deltaEtaTag_WZ.gif","gif") ;

  int bmin = histo_deltaEtaTag_VVf->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_VVf->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_VVf->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_VVf = integral / (double) histo_deltaEtaTag_VVf->Integral () ;

  int bmin = histo_deltaEtaTag_ggf->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_ggf->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_ggf->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_ggf = integral / (double) histo_deltaEtaTag_ggf->Integral () ;

  int bmin = histo_deltaEtaTag_Wt->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_Wt->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_Wt->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_Wt = integral / (double) histo_deltaEtaTag_Wt->Integral () ;

  int bmin = histo_deltaEtaTag_WW->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_WW->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_WW->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_WW = integral / (double) histo_deltaEtaTag_WW->Integral () ;

  int bmin = histo_deltaEtaTag_WZ->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_WZ->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_WZ->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_WZ = integral / (double) histo_deltaEtaTag_WZ->Integral () ;

  std::cout << "\n  selection eff\n" ;
  std::cout << "  -------------\n" ;
  std::cout << "sample\tdeltaEta\n" ;
  std::cout << "......................................\n" ;
  std::cout << "VVf\t" << deltaEtaTag_eff_VVf << "\n" ;
  std::cout << "ggf\t" << deltaEtaTag_eff_ggf << "\n" ;
  std::cout << "Wt \t" << deltaEtaTag_eff_Wt  << "\n" ;
  std::cout << "WW \t" << deltaEtaTag_eff_WW  << "\n" ;
  std::cout << "WZ \t" << deltaEtaTag_eff_WZ  << "\n" ;

  // eta dei tag

  histo_etaTag_VVf.SetLineWidth (2) ;
  histo_etaTag_VVf.SetFillStyle (3003) ;
  histo_etaTag_VVf.SetFillColor (4) ;
  histo_etaTag_ggf.SetLineWidth (2) ;
  histo_etaTag_ggf.SetLineColor (108) ;
  histo_etaTag_Wt.SetLineWidth (2) ;
  histo_etaTag_WW.SetLineWidth (2) ;
  histo_etaTag_WZ.SetLineWidth (2) ;

  histo_etaTag_VVf.SetStats (0) ;
  histo_etaTag_ggf.SetStats (0) ;
  histo_etaTag_Wt.SetStats (0) ;
  histo_etaTag_WW.SetStats (0) ;
  histo_etaTag_WZ.SetStats (0) ;

  histo_etaTag_Wt.SetMarkerStyle (8) ;
  histo_etaTag_WW.SetMarkerStyle (21) ;
  histo_etaTag_WZ.SetMarkerStyle (22) ;

  histo_etaTag_VVf.GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_ggf.GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_Wt.GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_WW.GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_WZ.GetYaxis ()->SetTitle ("a.u.") ;

  histo_etaTag_VVf.SetTitle ("VBF signal (blue)") ;
  histo_etaTag_ggf.SetTitle ("VBF signal (blue) and ggf") ;
  histo_etaTag_Wt.SetTitle ("VBF signal (blue) and Wt") ;
  histo_etaTag_WW.SetTitle ("VBF signal (blue) and WW") ;
  histo_etaTag_WZ.SetTitle ("VBF signal (blue) and WZ") ;

  histo_etaTag_VVf.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_ggf.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_Wt.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_WW.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_WZ.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;

  TCanvas * c5 = new TCanvas ;
  histo_etaTag_ggf.Draw () ;
  histo_etaTag_VVf.Draw ("same") ;
  c5->Print ("etaTag_ggf.gif","gif") ;

  TCanvas * c6 = new TCanvas ;
  histo_etaTag_Wt.Draw ("P") ;
  histo_etaTag_VVf.Draw ("same") ;
  c6->Print ("etaTag_Wt.gif","gif") ;

  TCanvas * c7 = new TCanvas ;
  histo_etaTag_WW.Draw ("P") ;
  histo_etaTag_VVf.Draw ("same") ;
  c7->Print ("etaTag_WW.gif","gif") ;

  TCanvas * c8 = new TCanvas ;
  histo_etaTag_WZ.Draw ("P") ;
  histo_etaTag_VVf.Draw ("same") ;
  c8->Print ("etaTag_WZ.gif","gif") ;
}


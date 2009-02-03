void plotTagSigVstt ()
{
	gROOT->SetStyle("Plain");
	
    TFile *f0 = new TFile("HISTO/histo_tt0_noB.root") ;
    TH1F *histo_deltaEtaTag_tt0 = (TH1F*)f0->Get("histo_deltaEtaTag_tt");
	TH1F *histo_etaTag_tt0 = (TH1F*)f0->Get("histo_etaTag_tt"); 
	
	TFile *f1 = new TFile("HISTO/histo_tt1_noB.root") ;
    TH1F *histo_deltaEtaTag_tt1 = (TH1F*)f1->Get("histo_deltaEtaTag_tt");
	TH1F *histo_etaTag_tt1 = (TH1F*)f1->Get("histo_etaTag_tt"); 
	
	TFile *f2 = new TFile("HISTO/histo_tt2_noB.root") ;
    TH1F *histo_deltaEtaTag_tt2 = (TH1F*)f2->Get("histo_deltaEtaTag_tt");
	TH1F *histo_etaTag_tt2 = (TH1F*)f2->Get("histo_etaTag_tt"); 
	
	TFile *f3 = new TFile("HISTO/histo_tt3_noB.root") ;
    TH1F *histo_deltaEtaTag_tt3 = (TH1F*)f3->Get("histo_deltaEtaTag_tt");
	TH1F *histo_etaTag_tt3 = (TH1F*)f3->Get("histo_etaTag_tt"); 
	
	TFile *f4 = new TFile("HISTO/histo_tt4_noB.root") ;
    TH1F *histo_deltaEtaTag_tt4 = (TH1F*)f4->Get("histo_deltaEtaTag_tt");
	TH1F *histo_etaTag_tt4 = (TH1F*)f4->Get("histo_etaTag_tt"); 
	
	TFile *f = new TFile("HISTO/histoSigVsBkg_noB.root") ;
	TH1F *histo_deltaEtaTag_VVf = (TH1F*)f->Get("histo_deltaEtaTag_VVf");
	TH1F *histo_etaTag_VVf = (TH1F*)f->Get("histo_etaTag_VVf"); 
	
	histo_deltaEtaTag_VVf.SetLineWidth (2) ;
    histo_deltaEtaTag_VVf.SetFillStyle (3003) ;
    histo_deltaEtaTag_VVf.SetFillColor (4) ;
  	
	histo_deltaEtaTag_tt0.SetLineWidth (2) ;
	histo_deltaEtaTag_tt1.SetLineWidth (2) ;
	histo_deltaEtaTag_tt2.SetLineWidth (2) ;
	histo_deltaEtaTag_tt3.SetLineWidth (2) ;
	histo_deltaEtaTag_tt4.SetLineWidth (2) ;
	histo_deltaEtaTag_tt0.SetLineStyle (1) ;
	histo_deltaEtaTag_tt1.SetLineStyle (2) ;
	histo_deltaEtaTag_tt2.SetLineStyle (3) ;
	histo_deltaEtaTag_tt3.SetLineStyle (9) ;
	histo_deltaEtaTag_tt4.SetLineStyle (6) ;
	histo_deltaEtaTag_tt0.SetLineColor (1) ;
	histo_deltaEtaTag_tt1.SetLineColor (2) ;
	histo_deltaEtaTag_tt2.SetLineColor (97) ;
	histo_deltaEtaTag_tt3.SetLineColor (107) ;
	histo_deltaEtaTag_tt4.SetLineColor (106) ;
	
	  histo_deltaEtaTag_VVf.SetStats (0) ;
	  histo_deltaEtaTag_tt0.SetStats (0) ;
	  histo_deltaEtaTag_tt1.SetStats (0) ;
	  histo_deltaEtaTag_tt2.SetStats (0) ;
	  histo_deltaEtaTag_tt3.SetStats (0) ;
	  histo_deltaEtaTag_tt4.SetStats (0) ;
	  
	  histo_deltaEtaTag_VVf.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt0.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt1.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt2.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt3.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt4.GetYaxis ()->SetTitle ("a.u.") ;

	  histo_deltaEtaTag_VVf.SetTitle ("VBF signal (blue) and tt") ;
	  histo_deltaEtaTag_tt0.SetTitle ("VBF signal (blue) and tt") ;
	  histo_deltaEtaTag_tt1.SetTitle ("VBF signal (blue) and tt") ;
	  histo_deltaEtaTag_tt2.SetTitle ("VBF signal (blue) and tt") ;
	  histo_deltaEtaTag_tt3.SetTitle ("VBF signal (blue) and tt") ;
	  histo_deltaEtaTag_tt4.SetTitle ("VBF signal (blue) and tt") ;

	  histo_deltaEtaTag_VVf.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt0.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt1.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt2.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt3.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_deltaEtaTag_tt4.GetYaxis ()->SetTitle ("a.u.") ;

	  histo_deltaEtaTag_VVf.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
	  histo_deltaEtaTag_tt0.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
	  histo_deltaEtaTag_tt1.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
	  histo_deltaEtaTag_tt2.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
	  histo_deltaEtaTag_tt3.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
	  histo_deltaEtaTag_tt4.GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;

    TLegend *tleg = new TLegend (0.7, 0.6, 0.9, 0.85);
    tleg -> SetBorderSize (0);
    tleg -> SetFillColor (0);
	tleg-> AddEntry (histo_deltaEtaTag_VVf , "VVf","l") ;
	tleg-> AddEntry (histo_deltaEtaTag_tt0 , "tt0j","l") ;
	tleg-> AddEntry (histo_deltaEtaTag_tt1 , "tt1j","l") ;
	tleg-> AddEntry (histo_deltaEtaTag_tt2 , "tt2j","l") ;
	tleg-> AddEntry (histo_deltaEtaTag_tt3 , "tt3j","l") ;
	tleg-> AddEntry (histo_deltaEtaTag_tt4 , "tt4j","l") ;
	
	TCanvas * c1 = new TCanvas ;
    histo_deltaEtaTag_tt0.Draw () ;
	histo_deltaEtaTag_VVf.Draw ("same") ;
    histo_deltaEtaTag_tt1.Draw ("same") ;
	histo_deltaEtaTag_tt2.Draw ("same") ;
    histo_deltaEtaTag_tt3.Draw ("same") ;
	histo_deltaEtaTag_tt4.Draw ("same") ;
	tleg.Draw ("same") ;
    c1->Print ("deltaEtaTag_tt_noB.gif","gif") ;
	
	histo_etaTag_VVf.SetLineWidth (2) ;
    histo_etaTag_VVf.SetFillStyle (3003) ;
    histo_etaTag_VVf.SetFillColor (4) ;
  	
	histo_etaTag_tt0.SetLineWidth (2) ;
	histo_etaTag_tt1.SetLineWidth (2) ;
	histo_etaTag_tt2.SetLineWidth (2) ;
	histo_etaTag_tt3.SetLineWidth (2) ;
	histo_etaTag_tt4.SetLineWidth (2) ;
	histo_etaTag_tt0.SetLineStyle (1) ;
	histo_etaTag_tt1.SetLineStyle (2) ;
	histo_etaTag_tt2.SetLineStyle (3) ;
	histo_etaTag_tt3.SetLineStyle (9) ;
	histo_etaTag_tt4.SetLineStyle (6) ;
	histo_etaTag_tt0.SetLineColor (1) ;
	histo_etaTag_tt1.SetLineColor (2) ;
	histo_etaTag_tt2.SetLineColor (97) ;
	histo_etaTag_tt3.SetLineColor (107) ;
	histo_etaTag_tt4.SetLineColor (106) ;
	
	  histo_etaTag_VVf.SetStats (0) ;
	  histo_etaTag_tt0.SetStats (0) ;
	  histo_etaTag_tt1.SetStats (0) ;
	  histo_etaTag_tt2.SetStats (0) ;
	  histo_etaTag_tt3.SetStats (0) ;
	  histo_etaTag_tt4.SetStats (0) ;
	  
	  histo_etaTag_VVf.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt0.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt1.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt2.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt3.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt4.GetYaxis ()->SetTitle ("a.u.") ;

	  histo_etaTag_VVf.SetTitle ("VBF signal (blue) and tt") ;
	  histo_etaTag_tt0.SetTitle ("VBF signal (blue) and tt") ;
	  histo_etaTag_tt1.SetTitle ("VBF signal (blue) and tt") ;
	  histo_etaTag_tt2.SetTitle ("VBF signal (blue) and tt") ;
	  histo_etaTag_tt3.SetTitle ("VBF signal (blue) and tt") ;
	  histo_etaTag_tt4.SetTitle ("VBF signal (blue) and tt") ;

	  histo_etaTag_VVf.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt0.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt1.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt2.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt3.GetYaxis ()->SetTitle ("a.u.") ;
	  histo_etaTag_tt4.GetYaxis ()->SetTitle ("a.u.") ;

	  histo_etaTag_VVf.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
	  histo_etaTag_tt0.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
	  histo_etaTag_tt1.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
	  histo_etaTag_tt2.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
	  histo_etaTag_tt3.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
	  histo_etaTag_tt4.GetXaxis ()->SetTitle ("#eta_{tagJets}") ;

	
	TCanvas * c1 = new TCanvas ;
    histo_etaTag_tt0.Draw () ;
	histo_etaTag_VVf.Draw ("same") ;
    histo_etaTag_tt1.Draw ("same") ;
	histo_etaTag_tt2.Draw ("same") ;
    histo_etaTag_tt3.Draw ("same") ;
	histo_etaTag_tt4.Draw ("same") ;
	tleg.Draw ("same") ;
    c1->Print ("etaTag_tt_noB.gif","gif") ;	
	
  int bmin = histo_deltaEtaTag_VVf->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_VVf->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_VVf->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_VVf = integral / (double) histo_deltaEtaTag_VVf->Integral () ;

  int bmin = histo_deltaEtaTag_tt0->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_tt0->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_tt0->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_tt0 = integral / (double) histo_deltaEtaTag_tt0->Integral () ;
  
  int bmin = histo_deltaEtaTag_tt1->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_tt1->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_tt1->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_tt1 = integral / (double) histo_deltaEtaTag_tt1->Integral () ;
  
  int bmin = histo_deltaEtaTag_tt2->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_tt2->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_tt2->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_tt2 = integral / (double) histo_deltaEtaTag_tt2->Integral () ;
  
  int bmin = histo_deltaEtaTag_tt3->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_tt3->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_tt3->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_tt3 = integral / (double) histo_deltaEtaTag_tt3->Integral () ;
  
  int bmin = histo_deltaEtaTag_tt4->GetXaxis ()->FindBin (3) ; 
  int bmax = histo_deltaEtaTag_tt4->GetXaxis ()->FindBin (10) ; 
  double integral = histo_deltaEtaTag_tt4->Integral (bmin,bmax) ;
  double deltaEtaTag_eff_tt4 = integral / (double) histo_deltaEtaTag_tt4->Integral () ;


  std::cout << "\n  selection eff\n" ;
  std::cout << "  -------------\n" ;
  std::cout << "sample\tdeltaEta\n" ;
  std::cout << "......................................\n" ;
  std::cout << "VVf\t" << deltaEtaTag_eff_VVf << "\n" ;
  std::cout << "tt0j\t" << deltaEtaTag_eff_tt0 << "\n" ;
  std::cout << "tt1j\t" << deltaEtaTag_eff_tt1 << "\n" ;
  std::cout << "tt2j\t" << deltaEtaTag_eff_tt2 << "\n" ;
  std::cout << "tt3j\t" << deltaEtaTag_eff_tt3 << "\n" ;
  std::cout << "tt4j\t" << deltaEtaTag_eff_tt4 << "\n" ;

  
	
	
	
}

void plotTagSigBkg ()
{
  // delta eta fra i tag
  
    gROOT->SetStyle("Plain");
	
    TFile *f = new TFile("HISTO/histoSigVsBkg_noB.root") ;
    TH1F *histo_deltaEtaTag_VVf = (TH1F*)f->Get("histo_deltaEtaTag_VVf");
	TH1F *histo_etaTag_VVf = (TH1F*)f->Get("histo_etaTag_VVf"); 
	TH1F *histo_deltaEtaTag_ggf = (TH1F*)f->Get("histo_deltaEtaTag_ggf");
	TH1F *histo_etaTag_ggf = (TH1F*)f->Get("histo_etaTag_ggf");
	TH1F *histo_deltaEtaTag_Wt = (TH1F*)f->Get("histo_deltaEtaTag_Wt");
	TH1F *histo_etaTag_Wt = (TH1F*)f->Get("histo_etaTag_Wt");
	TH1F *histo_deltaEtaTag_WW = (TH1F*)f->Get("histo_deltaEtaTag_WW");
	TH1F *histo_etaTag_WW = (TH1F*)f->Get("histo_etaTag_WW");
	TH1F *histo_deltaEtaTag_WZ = (TH1F*)f->Get("histo_deltaEtaTag_WZ");
	TH1F *histo_etaTag_WZ = (TH1F*)f->Get("histo_etaTag_WZ"); 
	


  histo_deltaEtaTag_VVf->SetLineWidth (2) ;
  histo_deltaEtaTag_VVf->SetFillStyle (3003) ;
  histo_deltaEtaTag_VVf->SetFillColor (4) ;
  histo_deltaEtaTag_ggf->SetLineWidth (2) ;
  histo_deltaEtaTag_ggf->SetLineColor (108) ;
  histo_deltaEtaTag_Wt->SetLineWidth (2) ;
  histo_deltaEtaTag_WW->SetLineWidth (2) ;
  histo_deltaEtaTag_WZ->SetLineWidth (2) ;

  histo_deltaEtaTag_VVf->SetStats (0) ;
  histo_deltaEtaTag_ggf->SetStats (0) ;
  histo_deltaEtaTag_Wt->SetStats (0) ;
  histo_deltaEtaTag_WW->SetStats (0) ;
  histo_deltaEtaTag_WZ->SetStats (0) ;

  histo_deltaEtaTag_Wt->SetMarkerStyle (8) ;
  histo_deltaEtaTag_WW->SetMarkerStyle (21) ;
  histo_deltaEtaTag_WZ->SetMarkerStyle (22) ;

  histo_deltaEtaTag_VVf->GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_ggf->GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_Wt->GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_WW->GetYaxis ()->SetTitle ("a.u.") ;
  histo_deltaEtaTag_WZ->GetYaxis ()->SetTitle ("a.u.") ;

  histo_deltaEtaTag_VVf->SetTitle ("VBF signal (blue)") ;
  histo_deltaEtaTag_ggf->SetTitle ("VBF signal (blue) and ggf") ;
  histo_deltaEtaTag_Wt->SetTitle ("VBF signal (blue) and Wt") ;
  histo_deltaEtaTag_WW->SetTitle ("VBF signal (blue) and WW") ;
  histo_deltaEtaTag_WZ->SetTitle ("VBF signal (blue) and WZ") ;

  histo_deltaEtaTag_VVf->GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_ggf->GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_Wt->GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_WW->GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;
  histo_deltaEtaTag_WZ->GetXaxis ()->SetTitle ("#Delta#eta_{tagJets}") ;

  TCanvas * c1 = new TCanvas ;
  histo_deltaEtaTag_ggf->Draw () ;
  histo_deltaEtaTag_VVf->Draw ("same") ;
  c1->Print ("deltaEtaTag_ggf_noB.gif","gif") ;

  TCanvas * c2 = new TCanvas ;
  histo_deltaEtaTag_Wt->Draw ("P") ;
  histo_deltaEtaTag_VVf->Draw ("same") ;
  c2->Print ("deltaEtaTag_Wt_noB.gif","gif") ;
  
  TCanvas * c3 = new TCanvas ;
  histo_deltaEtaTag_WW->Draw ("P") ;
  histo_deltaEtaTag_VVf->Draw ("same") ;
  c3->Print ("deltaEtaTag_WW_noB.gif","gif") ;

  TCanvas * c4 = new TCanvas ;
  histo_deltaEtaTag_WZ->Draw ("P") ;
  histo_deltaEtaTag_VVf->Draw ("same") ;
  c4->Print ("deltaEtaTag_WZ_noB.gif","gif") ;

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

  histo_etaTag_VVf->SetLineWidth (2) ;
  histo_etaTag_VVf->SetFillStyle (3003) ;
  histo_etaTag_VVf->SetFillColor (4) ;
  histo_etaTag_ggf->SetLineWidth (2) ;
  histo_etaTag_ggf->SetLineColor (108) ;
  histo_etaTag_Wt->SetLineWidth (2) ;
  histo_etaTag_WW->SetLineWidth (2) ;
  histo_etaTag_WZ->SetLineWidth (2) ;

  histo_etaTag_VVf->SetStats (0) ;
  histo_etaTag_ggf->SetStats (0) ;
  histo_etaTag_Wt->SetStats (0) ;
  histo_etaTag_WW->SetStats (0) ;
  histo_etaTag_WZ->SetStats (0) ;

  histo_etaTag_Wt->SetMarkerStyle (8) ;
  histo_etaTag_WW->SetMarkerStyle (21) ;
  histo_etaTag_WZ->SetMarkerStyle (22) ;

  histo_etaTag_VVf->GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_ggf->GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_Wt->GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_WW->GetYaxis ()->SetTitle ("a.u.") ;
  histo_etaTag_WZ->GetYaxis ()->SetTitle ("a.u.") ;

  histo_etaTag_VVf->SetTitle ("VBF signal (blue)") ;
  histo_etaTag_ggf->SetTitle ("VBF signal (blue) and ggf") ;
  histo_etaTag_Wt->SetTitle ("VBF signal (blue) and Wt") ;
  histo_etaTag_WW->SetTitle ("VBF signal (blue) and WW") ;
  histo_etaTag_WZ->SetTitle ("VBF signal (blue) and WZ") ;

  histo_etaTag_VVf->GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_ggf->GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_Wt->GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_WW->GetXaxis ()->SetTitle ("#eta_{tagJets}") ;
  histo_etaTag_WZ->GetXaxis ()->SetTitle ("#eta_{tagJets}") ;

  TCanvas * c5 = new TCanvas ;
  histo_etaTag_ggf->Draw () ;
  histo_etaTag_VVf->Draw ("same") ;
  c5->Print ("etaTag_ggf_noB.gif","gif") ;

  TCanvas * c6 = new TCanvas ;
  histo_etaTag_Wt->Draw ("P") ;
  histo_etaTag_VVf->Draw ("same") ;
  c6->Print ("etaTag_Wt_noB.gif","gif") ;

  TCanvas * c7 = new TCanvas ;
  histo_etaTag_WW->Draw ("P") ;
  histo_etaTag_VVf->Draw ("same") ;
  c7->Print ("etaTag_WW_noB.gif","gif") ;

  TCanvas * c8 = new TCanvas ;
  histo_etaTag_WZ->Draw ("P") ;
  histo_etaTag_VVf->Draw ("same") ;
  c8->Print ("etaTag_WZ_noB.gif","gif") ;
}



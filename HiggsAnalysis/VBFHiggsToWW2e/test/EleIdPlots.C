void EleIdPlots ()
{

  gROOT->SetStyle("Plain") ;
  gStyle->SetOptStat(1111111) ;
  gStyle->SetOptFit(1111) ;
  gStyle->SetTitleBorderSize(0) ;
  gStyle->SetTitleX(0.08) ;
  gStyle->SetTitleY(0.97) ;
  gStyle->SetPalette(4) ;

  TFile summary ("TestEleId.root") ;

  TH1F * delta[6] ;
  TH2F * effVsPt[6] ;
  TH2F * effVsEta[6] ;
  TProfile * p_effVsPt[6] ;
  TProfile * p_effVsEta[6] ;

  delta[0] = (TH1F *) summary.Get ("testEleID/m_deltaR_PTDRLoose") ;	
  delta[1] = (TH1F *) summary.Get ("testEleID/m_deltaR_PTDRMedium") ;	
  delta[2] = (TH1F *) summary.Get ("testEleID/m_deltaR_PTDRTight") ;	
  delta[3] = (TH1F *) summary.Get ("testEleID/m_deltaR_OTHERRobust") ;	
  delta[4] = (TH1F *) summary.Get ("testEleID/m_deltaR_OTHERLoose") ;	
  delta[5] = (TH1F *) summary.Get ("testEleID/m_deltaR_OTHERTight") ;	

  effVsPt[0] = (TH2F *) summary.Get ("testEleID/m_effVSPt_PTDRLoose") ;	
  effVsPt[1] = (TH2F *) summary.Get ("testEleID/m_effVSPt_PTDRMedium") ;	
  effVsPt[2] = (TH2F *) summary.Get ("testEleID/m_effVSPt_PTDRTight") ;	
  effVsPt[3] = (TH2F *) summary.Get ("testEleID/m_effVSPt_OTHERRobust") ;
  effVsPt[4] = (TH2F *) summary.Get ("testEleID/m_effVSPt_OTHERLoose") ;	
  effVsPt[5] = (TH2F *) summary.Get ("testEleID/m_effVSPt_OTHERTight") ;	

  effVsEta[0] = (TH2F *) summary.Get ("testEleID/m_effVSEta_PTDRLoose") ;	
  effVsEta[1] = (TH2F *) summary.Get ("testEleID/m_effVSEta_PTDRMedium") ;
  effVsEta[2] = (TH2F *) summary.Get ("testEleID/m_effVSEta_PTDRTight") ;	
  effVsEta[3] = (TH2F *) summary.Get ("testEleID/m_effVSEta_OTHERRobust") ;
  effVsEta[4] = (TH2F *) summary.Get ("testEleID/m_effVSEta_OTHERLoose") ;
  effVsEta[5] = (TH2F *) summary.Get ("testEleID/m_effVSEta_OTHERTight") ;

  TCanvas c1 ;

  //PG loop over eleIDs
  for (int i = 0 ; i < 6 ; ++i)
    {
      p_effVsPt[i] = effVsPt[i]->ProfileX () ;
      effVsPt[i]->SetStats (0) ;
      effVsEta[i]->GetYaxis ()->SetRangeUser (0.5,1.05) ;
      effVsPt[i]->GetXaxis ()->SetTitle ("p_{T}") ;
      effVsPt[i]->GetYaxis ()->SetTitle ("efficiency") ;
      effVsPt[i]->Draw ("COLZ") ;
      p_effVsPt[i]->SetMarkerStyle (21) ;
      p_effVsPt[i]->Draw ("same") ;
      TString name = TString (p_effVsPt[i]->GetName ()) + "_" + i + ".gif" ;
      c1.Print (name,"gif") ;
      
      p_effVsEta[i] = effVsEta[i]->ProfileX () ;
      effVsEta[i]->GetXaxis ()->SetTitle ("#eta") ;
      effVsEta[i]->GetYaxis ()->SetTitle ("efficiency") ;
      effVsEta[i]->GetYaxis ()->SetRangeUser (0.8,1.05) ;
      effVsEta[i]->SetStats (0) ;
      effVsEta[i]->Draw ("COLZ") ;
      p_effVsEta[i]->SetMarkerStyle (8) ;
      p_effVsEta[i]->Draw ("same") ;
      TString name = TString (p_effVsEta[i]->GetName ()) + "_" + i + ".gif" ;
      c1.Print (name,"gif") ;

    } //PG loop over eleIDs


}
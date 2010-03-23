int AlCaSCTest_plots ()
{
  TFile * in = new TFile ("alCaSuperClustersTest.root") ;
  
  TCanvas * c1 = new TCanvas () ;
  readingSummary->Draw ("col") ;

  TCanvas * c2 = new TCanvas () ;
  EB_SC_E->Draw () ;
  EB_SC_corr_E->SetLineColor (2) ;
  EB_SC_corr_E->Draw ("same") ;
  
  TCanvas * c3 = new TCanvas () ;
  EE_SC_E->Draw () ;
  EE_SC_ES_E->SetLineColor (2) ;
  EE_SC_ES_E->Draw ("same") ;
  EE_SC_ES_corr_E->SetLineColor (4) ;
  EE_SC_ES_corr_E->Draw ("same") ;

}
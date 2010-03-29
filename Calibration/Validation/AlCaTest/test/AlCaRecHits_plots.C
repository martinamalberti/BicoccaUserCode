int AlCaRecHits_plots () 
{
  TFile * in = new TFile ("alCaRecHitsTest.root") ;
  gROOT->SetStyle("Plain") ;
  gStyle->SetPalette (1) ;
  TCanvas * c1 = new TCanvas () ;
  m_barrelGlobalCrystalsMap->SetStats (0) ;
  m_barrelGlobalCrystalsMap->Draw ("colz") ;
  TCanvas * c2 = new TCanvas () ;
  m_endcapGlobalCrystalsMap->SetStats (0) ;
  m_endcapGlobalCrystalsMap->Draw ("colz") ;
  TCanvas * c3 = new TCanvas () ;
  m_preshowerGlobalChannelsMap->SetStats (0) ;
  m_preshowerGlobalChannelsMap->Draw ("colz") ;
  TCanvas * c4 = new TCanvas () ;
  m_barrelLocalCrystalsMap->SetStats (0) ;
  m_barrelLocalCrystalsMap->Draw ("colz") ;
  TCanvas * c5 = new TCanvas () ;
  m_endcapLocalCrystalsMap->SetStats (0) ;
  m_endcapLocalCrystalsMap->Draw ("colz") ;
  TCanvas * c6 = new TCanvas () ;
  m_preshowerLocalChannelsMap->SetStats (0) ;
  m_preshowerLocalChannelsMap->Draw ("colz") ;
}

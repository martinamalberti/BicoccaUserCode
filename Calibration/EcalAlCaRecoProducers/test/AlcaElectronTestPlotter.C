struct histos 
{

  histos (TString baseName, TFile * input) :
    m_name (baseName) ,
    m_varDistr     ( (TH1F *) input->Get (baseName + "_d")) ,
    m_varDistr_EB  ( (TH1F *) input->Get (baseName + "_dEB")) ,
    m_varDistr_EE  ( (TH1F *) input->Get (baseName + "_dEE")) ,
    m_varDistr_eta ( (TH2F *) input->Get (baseName + "_eta")) ,
    m_varDistr_pt  ( (TH2F *) input->Get (baseName + "_pt")) ,
    m_varDistr_p   ( (TH2F *) input->Get (baseName + "_p"))
    {
      std::cout << m_varDistr     ->GetName () << std::endl ;
      std::cout << m_varDistr_EB  ->GetName () << std::endl ;
      std::cout << m_varDistr_EE  ->GetName () << std::endl ;
      std::cout << m_varDistr_eta ->GetName () << std::endl ;
      std::cout << m_varDistr_pt  ->GetName () << std::endl ;
      std::cout << m_varDistr_p   ->GetName () << std::endl ;
    }

  print (TCanvas & c1) 
    {
      m_varDistr->SetFillColor (38) ;
      m_varDistr->Draw () ;
      c1.Print ((m_varDistr->GetName () + std::string (".gif")).c_str (),"gif") ;
      THStack detail ;
      m_varDistr_EB->SetFillColor (30) ;
      detail.Add (m_varDistr_EB) ;
      m_varDistr_EE->SetFillColor (46) ;
      detail.Add (m_varDistr_EE) ;
      detail.Draw () ;
      c1.Print ((m_varDistr->GetName () + std::string ("_det.gif")).c_str (),"gif") ;
      std::cerr << "pietro zero" << std::endl ;
      TProfile * profEta = m_varDistr_eta->ProfileX () ;
      profEta->SetMarkerStyle (8) ;
      profEta->SetMarkerColor (2) ;
      m_varDistr_eta->SetMarkerStyle (4) ;
      m_varDistr_eta->SetMarkerColor (16) ;
      m_varDistr_eta->Draw () ;
      m_varDistr_eta->SetStats (0) ;
      m_varDistr_eta->GetXaxis ()->SetTitle ("eta") ;
      profEta->Draw ("same") ; 
      c1.Print ((m_varDistr_eta->GetName () + std::string (".gif")).c_str (),"gif") ;
      m_varDistr_eta->Draw ("COLZ") ;
      c1.Print ((m_varDistr_eta->GetName () + std::string ("_col.gif")).c_str (),"gif") ;
      TProfile * profPt = m_varDistr_pt->ProfileX () ;
      profPt->SetMarkerStyle (8) ;
      profPt->SetMarkerColor (2) ;
      m_varDistr_pt->SetMarkerStyle (4) ;
      m_varDistr_pt->SetMarkerColor (16) ;
      m_varDistr_pt->Draw () ;
      m_varDistr_pt->SetStats (0) ;
      m_varDistr_pt->GetXaxis ()->SetTitle ("pt") ;
      profPt->Draw ("same") ; 
      (m_varDistr_pt->GetName () + std::string (".gif")).c_str () ;
      c1.Print ((m_varDistr_pt->GetName () + std::string (".gif")).c_str (),"gif") ;
      m_varDistr_pt->Draw ("COLZ") ;
      c1.Print ((m_varDistr_pt->GetName () + std::string ("_col.gif")).c_str (),"gif") ;
      std::cerr << "pietro due" << std::endl ;
      TProfile * profP = m_varDistr_p->ProfileX () ;
      profP->SetMarkerStyle (8) ;
      profP->SetMarkerColor (2) ;
      m_varDistr_p->SetMarkerStyle (4) ;
      m_varDistr_p->SetMarkerColor (16) ;
      m_varDistr_p->Draw () ;
      m_varDistr_p->SetStats (0) ;
      m_varDistr_p->GetXaxis ()->SetTitle ("p") ;
      profP->Draw ("same") ; 
      c1.Print ((m_varDistr_p->GetName () + std::string (".gif")).c_str (),"gif") ;
      m_varDistr_p->Draw ("COLZ") ;
      c1.Print ((m_varDistr_p->GetName () + std::string ("_col.gif")).c_str (),"gif") ;
    }

  private :

  TString m_name ;
  TH1F * m_varDistr ;
  TH1F * m_varDistr_EB ;
  TH1F * m_varDistr_EE ;
  TH2F * m_varDistr_eta ;
  TH2F * m_varDistr_pt ;
  TH2F * m_varDistr_p ;
} ;


// --------------------------------------------------------


int AlcaElectronTestPlotter (TString fileName)
{
  TFile input (fileName) ;
  histos EseedOpout ("alcatesting/EseedOpout" , &input) ;
  histos ESCOpin ("alcatesting/ESCOpin" , &input) ;
  histos piMpoOpi ("alcatesting/piMpoOpi" , &input) ;

  TCanvas c1 ;
  EseedOpout.print (c1) ;
  ESCOpin.print (c1) ;
  piMpoOpi.print (c1) ;

  return 0 ;
}
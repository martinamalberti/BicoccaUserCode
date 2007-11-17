#ifndef varHisto_H
#define varHisto_H

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>

struct varHisto
{
  //! ctor
  varHisto (std::string baseName = "pippo", 
            int bins = 10, double min = 0., double max = 1.,
            edm::Service<TFileService> & fs) : 
    m_fs (fs) ,        
    m_name (baseName) ,
    m_varDistr (m_fs->make<TH1F> ((m_name+"_d").c_str (),"",bins, min, max)) ,
    m_varDistr_EB (m_fs->make<TH1F> ((m_name+"_dEB").c_str (),"",bins, min, max)) ,
    m_varDistr_EE (m_fs->make<TH1F> ((m_name+"_dEE").c_str (),"",bins, min, max)) ,
    m_varDistr_eta (m_fs->make<TH2F> ((m_name+"_eta").c_str (),"",100,0,3, bins, min, max)) ,
    m_varDistr_pt (m_fs->make<TH2F> ((m_name+"_pt").c_str (),"",100,0,200, bins, min, max)) ,
    m_varDistr_p (m_fs->make<TH2F> ((m_name+"_p").c_str (),"",100,0,200, bins, min, max)) 
      {
//        m_varDistr->SetDirectory (0) ;
//        m_varDistr_EB->SetDirectory (0) ;
//        m_varDistr_EE->SetDirectory (0) ;
//        m_varDistr_eta->SetDirectory (0) ;
//        m_varDistr_pt->SetDirectory (0) ;
//        m_varDistr_p->SetDirectory (0) ;      
      }

// needed in the future for containers
// need to change the architecture bacause of the reference
//  // copy ctor
//  varHisto (const varHisto & orig)
//    {
//       m_fs = orig.m_fs ;
//       m_name = orig.m_name ;
//       m_varDistr = orig.m_varDistr ;
//       m_varDistr_EB = orig.m_varDistr_EB ;
//       m_varDistr_EE = orig.m_varDistr_EE ;
//       m_varDistr_eta = orig.m_varDistr_eta ;
//       m_varDistr_pt = orig.m_varDistr_pt ;
//       m_varDistr_p = orig.m_varDistr_p ;
//    }

    void fill (double var, double eta, double p)
      {
        m_varDistr->Fill (var) ;
        if (fabs (eta) < 1.5) m_varDistr_EB->Fill (var) ;
        else m_varDistr_EE->Fill (var) ;
        m_varDistr_eta->Fill (eta, var) ;
//        double pt = p ; // FIXME
        //maybe too slow        
        double pt = p * cos (2 * atan (exp (-1 * eta) )) ;
        m_varDistr_pt->Fill (pt, var) ;
        m_varDistr_p->Fill (p, var) ;      
      } 

    void save (TFile * output) 
      {
        TDirectory * currDir = gDirectory ;
        output->cd () ;
        m_varDistr->Write () ;
        m_varDistr_EB->Write () ;
        m_varDistr_EE->Write () ;
        m_varDistr_eta->Write () ;
        m_varDistr_pt->Write () ;
        m_varDistr_p->Write () ;
        currDir->cd () ;      
      }

    //! PG this does not work in CMSSW unfortunately
    void print ()
      {
//        TCanvas * c1 = m_fs->make<TCanvas> (new TCanvas ()) ;
        TCanvas c1 ;
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
        m_fs->make<TProfile> (*profEta) ;
        profEta->SetMarkerStyle (8) ;
        profEta->SetMarkerColor (2) ;
        m_varDistr_eta->SetMarkerStyle (4) ;
        m_varDistr_eta->SetMarkerColor (16) ;
        m_varDistr_eta->Draw () ;
        m_varDistr_eta->SetStats (0) ;
        m_varDistr_eta->GetXaxis ()->SetTitle ("eta") ;
        profEta->Draw ("same") ; 
        c1.Print ((m_varDistr_eta->GetName () + std::string (".gif")).c_str (),"gif") ;
        TProfile * profPt = m_varDistr_pt->ProfileX () ;
        m_fs->make<TProfile> (*profPt) ;
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
        std::cerr << "pietro due" << std::endl ;
        TProfile * profP = m_varDistr_p->ProfileX () ;
        m_fs->make<TProfile> (*profP) ;
        profP->SetMarkerStyle (8) ;
        profP->SetMarkerColor (2) ;
        m_varDistr_p->SetMarkerStyle (4) ;
        m_varDistr_p->SetMarkerColor (16) ;
        m_varDistr_p->Draw () ;
        m_varDistr_p->SetStats (0) ;
        m_varDistr_p->GetXaxis ()->SetTitle ("p") ;
        profP->Draw ("same") ; 
        c1.Print ((m_varDistr_p->GetName () + std::string (".gif")).c_str (),"gif") ;
        std::cerr << "pietro tre" << std::endl ;
      }

  ~varHisto () 
    {
      //PG not sure can be done
//      delete m_varDistr ;
//      delete m_varDistr_EB ;
//      delete m_varDistr_EE ;
//      delete m_varDistr_eta ;
//      delete m_varDistr_pt ;
//      delete m_varDistr_p ;
    }
    
private :    
        
  edm::Service<TFileService> & m_fs ;
  std::string m_name ;
  TH1F * m_varDistr ;
  TH1F * m_varDistr_EB ;
  TH1F * m_varDistr_EE ;
  TH2F * m_varDistr_eta ;
  TH2F * m_varDistr_pt ;
  TH2F * m_varDistr_p ;
} ;


#endif

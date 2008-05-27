#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TString.h"

struct SumOnPthat
{

  int m_typeOfObject ;
  TH1F * eta_flav[5] ;
  TH1F * etaPercent_flav[5] ;
  TH1F * pT_flav[5]  ;
  TH1F * pTPercent_flav[5]  ;
  TH1F * phi_flav[5] ;
  TH1F * phiPercent_flav[5] ;
  std::string m_baseName ;

  SumOnPthat ()
    {
    char appoBasename[10] ;
    sprintf (appoBasename,"pthat_integrated_distribution") ;
    m_baseName = appoBasename ;

    std::string names[5] = {"light","c","b","failed","g"} ;
    //PG loop over possible flavours
    for (int i = 0 ; i < 5 ; ++i)
      {     
      eta_flav[i]  = new TH1F ((m_baseName + std::string ("_eta_") + names[i]).c_str () , (m_baseName + std::string ("_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
      etaPercent_flav[i]  = new TH1F ((m_baseName + std::string ("_etaPercent_") + names[i]).c_str () , (m_baseName + std::string ("_etaPercent_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
      pT_flav[i]  = new TH1F ((m_baseName + std::string ("_pT_") + names[i]).c_str () , (m_baseName + std::string ("_pT_") + names[i]).c_str () ,  40, 0, 200) ;
      pTPercent_flav[i]  = new TH1F ((m_baseName + std::string ("_pTPercent_") + names[i]).c_str () , (m_baseName + std::string ("_pTPercent") + names[i]).c_str () ,  40, 0, 200) ;
      phi_flav[i]  = new TH1F ((m_baseName + std::string ("_phi_") + names[i]).c_str () , (m_baseName + std::string ("_phi_") + names[i]).c_str () ,  50, -3.14, 3.14) ;
      phiPercent_flav[i]  = new TH1F ((m_baseName + std::string ("_phiPercent_") + names[i]).c_str () , (m_baseName + std::string ("_phiPercent_") + names[i]).c_str () ,  50, -3.14, 3.14) ;
      }
            std::cout<<"interno debug after flavour"<<std::endl;
    }

  ~SumOnPthat ()
    {
    }
};
                                            
struct histos
{

  int m_ptHat, m_typeOfObject ;
  std::string m_baseName ;
  int m_counter;

  TH1F * eta_flav[5] ;
  TH1F * pT_flav[5]  ;
  TH1F * phi_flav[5] ;

  histos (int ptHat = 0 , int typeOfObject = 0) : //utilizziamo basename per il  futuro impiego di histos per la cinematica anche di jetmatch
    m_ptHat (ptHat) ,
    m_typeOfObject (typeOfObject) ,
    m_counter (0)
    {
      char appoBasename[10] ;
      sprintf (appoBasename,"object%i_pthat%i",m_typeOfObject,m_ptHat) ;
      m_baseName = appoBasename ;

      std::string names[5] = {"light","c","b","failed","g"} ;
      //PG loop over possible flavours
      for (int i = 0 ; i < 5 ; ++i)
        {      
          eta_flav[i]  = new TH1F ((m_baseName + std::string ("_eta_") + names[i]).c_str () , (m_baseName + std::string ("_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
          pT_flav[i]  = new TH1F ((m_baseName + std::string ("_pT_") + names[i]).c_str () , (m_baseName + std::string ("_pT_") + names[i]).c_str () ,  40, 0, 200) ;
          phi_flav[i]  = new TH1F ((m_baseName + std::string ("_phi_") + names[i]).c_str () , (m_baseName + std::string ("_phi_") + names[i]).c_str () ,  50, -3.14, 3.14) ;
        }

    }

  void grow ()
    {
      m_counter++ ; 
    }

  void rescale (double factor)
    {
    factor /= m_counter;
    for (int i=0 ; i<5 ; ++i)
      {                                                  
      eta_flav[i]  ->Scale (factor) ;
      pT_flav[i]   ->Scale (factor) ;
      phi_flav[i]  ->Scale (factor) ;
      }
    }

  void write ()
    {
    TFile totale ("jetKinematic.root","UPDATE") ; //preferisco mettere tutti i pthat nello stesso file, quindi ricorda di aggiornare il file a ogni riesecuzione
    for (int i=0 ; i<5 ; ++i)
      {
      eta_flav[i]  ->Write () ;
      pT_flav[i]   ->Write () ;
      phi_flav[i]  ->Write () ;   
      }
    totale.Close () ;
    }
   
~histos ()
    {
    }
} ;

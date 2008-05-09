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

                                            
struct rates
{
  TH1F * rate_resol_ptJ ;
  TH1F * rate_tkIso_ptJ ;
  TH1F * rate_minimumPT_ptJ ;
  TH1F * rate_eleIdTight_ptJ ;
  TH1F * rate_eleIdDaskalakis_ptJ ;
  TH1F * rate_resol_eta ;
  TH1F * rate_tkIso_eta  ;
  TH1F * rate_minimumPT_eta ;
  TH1F * rate_eleIdTight_eta ;
  TH1F * rate_eleIdDaskalakis_eta ;
  TH1F * rate_resol_ptJ_flav[5]          ;
  TH1F * rate_resol_eta_flav[5]          ;
  TH1F * rate_eleIdTight_eta_flav[5]     ;
  TH1F * rate_eleIdTight_ptJ_flav[5]     ;
  TH1F * rate_eleIdDaskalakis_eta_flav[5]     ;
  TH1F * rate_eleIdDaskalakis_ptJ_flav[5]     ;
  TH1F * rate_minimumPT_eta_flav[5]         ;
  TH1F * rate_minimumPT_ptJ_flav[5]         ;
  TH1F * rate_tkIso_eta_flav[5]          ;
  TH1F * rate_tkIso_ptJ_flav[5]          ;

  int m_ptHat ;
  std::string m_baseName ;

  rates(int ptHat = 0) :
    m_ptHat (ptHat) ,
    m_baseName ("_appo_")
    {
    char basename[10] ;
    sprintf (basename,"%d_",m_ptHat) ;
    m_baseName = basename ;
    rate_resol_ptJ = new TH1F( (m_baseName+std::string("rate_resol_ptJ")).c_str(),(m_baseName+std::string("ptHat")).c_str(),40,0,200);
    rate_tkIso_ptJ = new TH1F( (m_baseName+std::string("rate_tkIso_ptJ")).c_str(),"+ track isolation",40,0,200);
    rate_minimumPT_ptJ = new TH1F( (m_baseName+std::string("rate_minimumPT_ptJ")).c_str(),"+ minimum P_T matching electron",40,0,200);
    rate_eleIdTight_ptJ = new TH1F( (m_baseName+std::string("rate_eleIdTight_ptJ")).c_str(),"+ Tight electron ID",40,0,200);
    rate_eleIdDaskalakis_ptJ = new TH1F( (m_baseName+std::string("rate_eleIdDaskalakis_ptJ")).c_str(),"+ Daskalakis electron ID",40,0,200);
    rate_resol_eta = new TH1F( (m_baseName+std::string("rate_resol_eta")).c_str(),(m_baseName+std::string("ptHat")).c_str(),100,-2.5,2.5);
    rate_tkIso_eta = new TH1F((m_baseName+std::string("rate_tkIso_eta")).c_str(),"+ track isolation",100,-2.5,2.5);
    rate_minimumPT_eta = new TH1F( (m_baseName+std::string("rate_minimumPT_eta")).c_str(),"+ minimum P_T matching electron",100,-2.5,2.5);
    rate_eleIdTight_eta = new TH1F( (m_baseName+std::string("rate_eleIdTight_eta")).c_str(),"+ Tight electron ID",100,-2.5,2.5);
    rate_eleIdDaskalakis_eta = new TH1F( (m_baseName+std::string("rate_eleIdDaskalakis_eta")).c_str(),"+ Daskalakis electron ID",100,-2.5,2.5);

    std::string names[5] = {"light","c","b","t","g"} ;
    //PG loop over possible flavours    
    for (int i = 0 ; i < 5 ; ++i)
      {      
       rate_resol_eta_flav[i]  = new TH1F ((m_baseName + std::string("rate_resol_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_resol_eta_") ).c_str () ,  50, -2.5, 2.5) ;
       rate_resol_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_resol_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("rate_resol_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_tkIso_eta_flav[i]  = new TH1F ((m_baseName + std::string ("rate_tkIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_tkIso_eta_") ).c_str () ,  50, -2.5, 2.5) ;
       rate_tkIso_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_tkIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string("rate_tkIso_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_minimumPT_eta_flav[i] = new TH1F ((m_baseName + std::string ("rate_minimumPT_eta_") + names[i]).c_str () , (m_baseName + std::string("rate_minimumPT_eta_")).c_str () ,  50, -2.5, 2.5) ;
       rate_minimumPT_ptJ_flav[i] = new TH1F ((m_baseName + std::string ("rate_minimumPT_ptJ_") + names[i]).c_str () , (m_baseName + std::string("rate_minimumPT_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_eleIdTight_eta_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdTight_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdTight_eta_")).c_str () ,  50, -2.5, 2.5) ;
       rate_eleIdTight_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdTight_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdTight_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_eleIdDaskalakis_eta_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdDaskalakis_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdDaskalakis_eta_")).c_str () ,  50, -2.5, 2.5) ;
       rate_eleIdDaskalakis_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdDaskalakis_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdDaskalakis_ptJ_") ).c_str () ,  40, 0, 200) ;
       }   
    }
  ~rates()
    {}
};

struct ratesIntegral
{
  TH1F * rate_resol_ptJ ;
  TH1F * rate_tkIso_ptJ ;
  TH1F * rate_minimumPT_ptJ ;
  TH1F * rate_eleIdTight_ptJ ;
  TH1F * rate_eleIdDaskalakis_ptJ ;
  TH1F * rate_resol_eta ;
  TH1F * rate_tkIso_eta  ;
  TH1F * rate_minimumPT_eta ;
  TH1F * rate_eleIdTight_eta ;
  TH1F * rate_eleIdDaskalakis_eta ;
  TH1F * rate_resol_ptJ_flav[5]          ;
  TH1F * rate_resol_eta_flav[5]          ;
  TH1F * rate_eleIdTight_eta_flav[5]     ;
  TH1F * rate_eleIdTight_ptJ_flav[5]     ;
  TH1F * rate_eleIdDaskalakis_eta_flav[5]     ;
  TH1F * rate_eleIdDaskalakis_ptJ_flav[5]     ;
  TH1F * rate_minimumPT_eta_flav[5]         ;
  TH1F * rate_minimumPT_ptJ_flav[5]         ;
  TH1F * rate_tkIso_eta_flav[5]          ;
  TH1F * rate_tkIso_ptJ_flav[5]          ;

  std::string m_baseName ;

  ratesIntegral() :
    m_baseName ("_appo_")
    {
    char basename[10] ;
    sprintf (basename,"integral_") ;
    m_baseName = basename ;
    rate_resol_ptJ = new TH1F( (m_baseName+std::string("rate_resol_ptJ")).c_str(),(m_baseName+std::string("ptHat")).c_str(),40,0,200);
    rate_tkIso_ptJ = new TH1F( (m_baseName+std::string("rate_tkIso_ptJ")).c_str(),"+ track isolation",40,0,200);
    rate_minimumPT_ptJ = new TH1F( (m_baseName+std::string("rate_minimumPT_ptJ")).c_str(),"+ minimum P_T matching electron",40,0,200);
    rate_eleIdTight_ptJ = new TH1F( (m_baseName+std::string("rate_eleIdTight_ptJ")).c_str(),"+ Tight electron ID",40,0,200);
    rate_eleIdDaskalakis_ptJ = new TH1F( (m_baseName+std::string("rate_eleIdDaskalakis_ptJ")).c_str(),"+ Daskalakis electron ID",40,0,200);
    rate_resol_eta = new TH1F( (m_baseName+std::string("rate_resol_eta")).c_str(),(m_baseName+std::string("ptHat")).c_str(),100,-2.5,2.5);
    rate_tkIso_eta = new TH1F((m_baseName+std::string("rate_tkIso_eta")).c_str(),"+ track isolation",100,-2.5,2.5);
    rate_minimumPT_eta = new TH1F( (m_baseName+std::string("rate_minimumPT_eta")).c_str(),"+ minimum P_T matching electron",100,-2.5,2.5);
    rate_eleIdTight_eta = new TH1F( (m_baseName+std::string("rate_eleIdTight_eta")).c_str(),"+ Tight electron ID",100,-2.5,2.5);
    rate_eleIdDaskalakis_eta = new TH1F( (m_baseName+std::string("rate_eleIdDaskalakis_eta")).c_str(),"+ Daskalakis electron ID",100,-2.5,2.5);

    std::string names[5] = {"light","c","b","t","g"} ;
    //PG loop over possible flavours    
    for (int i = 0 ; i < 5 ; ++i)
      {      
       rate_resol_eta_flav[i]  = new TH1F ((m_baseName + std::string("rate_resol_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_resol_eta_") ).c_str () ,  50, -2.5, 2.5) ;
       rate_resol_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_resol_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("rate_resol_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_tkIso_eta_flav[i]  = new TH1F ((m_baseName + std::string ("rate_tkIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_tkIso_eta_") ).c_str () ,  50, -2.5, 2.5) ;
       rate_tkIso_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_tkIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string("rate_tkIso_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_minimumPT_eta_flav[i] = new TH1F ((m_baseName + std::string ("rate_minimumPT_eta_") + names[i]).c_str () , (m_baseName + std::string("rate_minimumPT_eta_")).c_str () ,  50, -2.5, 2.5) ;
       rate_minimumPT_ptJ_flav[i] = new TH1F ((m_baseName + std::string ("rate_minimumPT_ptJ_") + names[i]).c_str () , (m_baseName + std::string("rate_minimumPT_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_eleIdTight_eta_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdTight_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdTight_eta_")).c_str () ,  50, -2.5, 2.5) ;
       rate_eleIdTight_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdTight_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdTight_ptJ_") ).c_str () ,  40, 0, 200) ;
       rate_eleIdDaskalakis_eta_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdDaskalakis_eta_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdDaskalakis_eta_")).c_str () ,  50, -2.5, 2.5) ;
       rate_eleIdDaskalakis_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("rate_eleIdDaskalakis_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("rate_eleIdDaskalakis_ptJ_") ).c_str () ,  40, 0, 200) ;
       }   
    }
  ~ratesIntegral()
    {}
};

struct histos
{

  int m_ptHat ;
  std::string m_baseName ;
  int m_counterNum , m_counterDen ;

  //per ora ci interessano solo le selezioni in sequenza!
  TH1F * m_e_sequence_resol_eta          ;
  TH1F * m_e_sequence_resol_ptJ          ;
  TH1F * m_e_sequence_tkIso_eta          ;
  TH1F * m_e_sequence_tkIso_ptJ          ;
  TH1F * m_e_sequence_minimumPT_eta         ;
  TH1F * m_e_sequence_minimumPT_ptJ         ;
  TH1F * m_e_sequence_eleIdTight_eta          ;
  TH1F * m_e_sequence_eleIdTight_ptJ          ;
  TH1F * m_e_sequence_eleIdDaskalakis_eta          ;
  TH1F * m_e_sequence_eleIdDaskalakis_ptJ          ;
    
  TH1F * cumulativeAll       ;
  
  TH1F * m_e_sequence_resol_ptJ_flav[5]          ;
  TH1F * m_e_sequence_resol_eta_flav[5]          ; 
  TH1F * m_e_sequence_tkIso_eta_flav[5]          ;
  TH1F * m_e_sequence_tkIso_ptJ_flav[5]          ;
  TH1F * m_e_sequence_minimumPT_eta_flav[5]         ;
  TH1F * m_e_sequence_minimumPT_ptJ_flav[5]         ;
  TH1F * m_e_sequence_eleIdTight_eta_flav[5]     ;
  TH1F * m_e_sequence_eleIdTight_ptJ_flav[5]     ; 
  TH1F * m_e_sequence_eleIdDaskalakis_eta_flav[5]     ;
  TH1F * m_e_sequence_eleIdDaskalakis_ptJ_flav[5]     ; 
  

  histos (int ptHat = 0 , int basename = 10) : 
    m_ptHat (ptHat) ,
    //m_baseName (basename) ,
    m_counterNum (0),
    m_counterDen (0)
    {
      char appoBasename[10] ;
      sprintf (appoBasename,"%i_%d",basename,ptHat) ;
      m_baseName = appoBasename ;

      m_e_sequence_resol_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_resol_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptJ" )).c_str (),  40, 0, 200) ;
      m_e_sequence_tkIso_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_tkIso_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptJ" )).c_str (),  40, 0, 200) ;
      m_e_sequence_minimumPT_eta = new TH1F ((m_baseName + std::string ("m_e_sequence_minimum_eta")).c_str () , (m_baseName + std::string ("m_e_sequence_minimum_eta")).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_minimumPT_ptJ = new TH1F ((m_baseName + std::string ("m_e_sequence_minimum_ptJ")).c_str () , (m_baseName + std::string ("m_e_sequence_minimum_ptJ")).c_str (),  40, 0, 200) ;
      m_e_sequence_eleIdTight_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_eleIdTight_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ" )).c_str (),  40, 0, 200) ;
      m_e_sequence_eleIdDaskalakis_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_eleIdDaskalakis_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ" )).c_str (),  40, 0, 200) ;

      cumulativeAll  = new TH1F ((m_baseName + std::string ("cumulativeAll" )).c_str () ,"cumulative distributions",  40, 0, 200) ;

      std::string names[5] = {"light","c","b","t","g"} ;
      //PG loop over possible flavours
      for (int i = 0 ; i < 5 ; ++i)
        {      
          m_e_sequence_resol_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_resol_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_tkIso_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_tkIso_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_minimumPT_eta_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_minimumPT_eta_") + names[i]).c_str () , (m_baseName + std::string("m_e_sequence_minimumPT_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_minimumPT_ptJ_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_minimum_ptE_") + names[i]).c_str () , (m_baseName + std::string("m_e_sequence_minimumPT_ptE_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_eleIdTight_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_eleIdTight_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_eleIdDaskalakis_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_eleIdDaskalakis_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;

        }

    }
/*
  void growNum ()
    {
      m_counterNum++ ; 
    }
  void growDen ()
    {
      m_counterDen++ ; 
    }
*/    
  void rescaleNum (double factor)
    {
      //factor /= m_counterNum ;
      m_e_sequence_resol_eta    ->Scale (factor) ;
      m_e_sequence_resol_ptJ    ->Scale (factor) ;
      m_e_sequence_tkIso_eta    ->Scale (factor) ;
      m_e_sequence_tkIso_ptJ    ->Scale (factor) ;
      m_e_sequence_minimumPT_eta   ->Scale (factor) ;
      m_e_sequence_minimumPT_ptJ   ->Scale (factor) ;
      m_e_sequence_eleIdTight_eta    ->Scale (factor) ;
      m_e_sequence_eleIdTight_ptJ    ->Scale (factor) ;
      m_e_sequence_eleIdDaskalakis_eta    ->Scale (factor) ;
      m_e_sequence_eleIdDaskalakis_ptJ    ->Scale (factor) ;

      for (int i=0 ; i<5 ; ++i)
        {                                                  
          m_e_sequence_resol_eta_flav[i]          ->Scale (factor) ;
          m_e_sequence_resol_ptJ_flav[i]          ->Scale (factor) ;
	  m_e_sequence_tkIso_eta_flav[i]    ->Scale (factor) ;
          m_e_sequence_tkIso_ptJ_flav[i]    ->Scale (factor) ;
          m_e_sequence_minimumPT_eta_flav[i]   ->Scale (factor) ;
          m_e_sequence_minimumPT_ptJ_flav[i]   ->Scale (factor) ;
          m_e_sequence_eleIdTight_eta_flav[i]    ->Scale (factor) ;
          m_e_sequence_eleIdTight_ptJ_flav[i]    ->Scale (factor) ;
          m_e_sequence_eleIdDaskalakis_eta_flav[i]    ->Scale (factor) ;
          m_e_sequence_eleIdDaskalakis_ptJ_flav[i]    ->Scale (factor) ;
        }
	
    }
    
  void rescaleDen (double factor)
    {
      //factor /= m_counterDen ;
      m_e_sequence_resol_eta         ->Scale (factor) ;
      m_e_sequence_resol_ptJ         ->Scale (factor) ;

      for (int i=0 ; i<5 ; ++i)
        {                                                  
          m_e_sequence_resol_eta_flav[i]          ->Scale (factor) ;
          m_e_sequence_resol_ptJ_flav[i]          ->Scale (factor) ;
        }      

    }
/*  void write ()
    {
      TFile totale ((m_baseName +"totale.root").c_str(),"RECREATE") ;
      m_e_single_resol_eta         ->Write () ;
      m_e_single_resol_ptE         ->Write () ;
      m_e_single_resol_ptJ         ->Write () ;
      m_e_single_resol_EMfrac      ->Write () ;   
      
      totale.Close () ;    
    
    }*/
/*salviamo solo cio' che ci serve per ora, con un Write normale    
  void writeNum ()
    {
      TFile totale ((m_baseName +"numerator.root").c_str(),"RECREATE") ;
      m_e_single_resol_eta         ->Write () ;
      m_e_single_resol_ptE         ->Write () ;
      m_e_single_resol_ptJ         ->Write () ;
      m_e_single_resol_EMfrac      ->Write () ;   
      
      totale.Close () ;    
    }

  void writeDen ()
    {
      TFile totale ((m_baseName +"denominator.root").c_str(),"RECREATE") ;
      m_e_single_resol_eta         ->Write () ;
      m_e_single_resol_ptJ         ->Write () ;   
      
      totale.Close () ;    
    
    }
*/    
~histos ()
    {
    }
} ;

struct histosIntegral
{
  std::string m_baseName ;

  //per ora ci interessano solo le selezioni in sequenza!
  TH1F * m_e_sequence_resol_eta          ;
  TH1F * m_e_sequence_resol_ptJ          ;
  TH1F * m_e_sequence_tkIso_eta          ;
  TH1F * m_e_sequence_tkIso_ptJ          ;
  TH1F * m_e_sequence_minimumPT_eta         ;
  TH1F * m_e_sequence_minimumPT_ptJ         ;
  TH1F * m_e_sequence_eleIdTight_eta          ;
  TH1F * m_e_sequence_eleIdTight_ptJ          ;
  TH1F * m_e_sequence_eleIdDaskalakis_eta          ;
  TH1F * m_e_sequence_eleIdDaskalakis_ptJ          ;
  
  TH1F * cumulativeAll       ;
  
  TH1F * m_e_sequence_resol_ptJ_flav[5]          ;
  TH1F * m_e_sequence_resol_eta_flav[5]          ; 
  TH1F * m_e_sequence_tkIso_eta_flav[5]          ;
  TH1F * m_e_sequence_tkIso_ptJ_flav[5]          ;
  TH1F * m_e_sequence_minimumPT_eta_flav[5]         ;
  TH1F * m_e_sequence_minimumPT_ptJ_flav[5]         ;
  TH1F * m_e_sequence_eleIdTight_eta_flav[5]     ;
  TH1F * m_e_sequence_eleIdTight_ptJ_flav[5]     ; 
  TH1F * m_e_sequence_eleIdDaskalakis_eta_flav[5]     ;
  TH1F * m_e_sequence_eleIdDaskalakis_ptJ_flav[5]     ;
  
  int appoInt ;  

  histosIntegral (int aux =0 ) :
  appoInt (aux),
  m_baseName("appo") 
    {
      char appoBasename[10] ;
      sprintf (appoBasename,"integral_%i",appoInt) ;
      m_baseName = appoBasename ;

      m_e_sequence_resol_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_resol_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptJ" )).c_str (),  40, 0, 200) ;
      m_e_sequence_tkIso_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_tkIso_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptJ" )).c_str (),  40, 0, 200) ;
      m_e_sequence_minimumPT_eta = new TH1F ((m_baseName + std::string ("m_e_sequence_minimum_eta")).c_str () , (m_baseName + std::string ("m_e_sequence_minimum_eta")).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_minimumPT_ptJ = new TH1F ((m_baseName + std::string ("m_e_sequence_minimum_ptJ")).c_str () , (m_baseName + std::string ("m_e_sequence_minimum_ptJ")).c_str (),  40, 0, 200) ;
      m_e_sequence_eleIdTight_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_eleIdTight_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ" )).c_str (),  40, 0, 200) ;
      m_e_sequence_eleIdDaskalakis_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta" )).c_str (),  100, -2.5, 2.5) ;
      m_e_sequence_eleIdDaskalakis_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ" )).c_str (),  40, 0, 200) ;

      cumulativeAll  = new TH1F ((m_baseName + std::string ("cumulativeAll" )).c_str () ,"cumulative distributions",  40, 0, 200) ;

      std::string names[5] = {"light","c","b","t","g"} ;
      //PG loop over possible flavours
      for (int i = 0 ; i < 5 ; ++i)
        {      
          m_e_sequence_resol_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_resol_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_tkIso_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_tkIso_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_minimumPT_eta_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_minimumPT_eta_") + names[i]).c_str () , (m_baseName + std::string("m_e_sequence_minimumPT_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_minimumPT_ptJ_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_minimum_ptE_") + names[i]).c_str () , (m_baseName + std::string("m_e_sequence_minimumPT_ptE_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_eleIdTight_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_eleIdTight_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;
          m_e_sequence_eleIdDaskalakis_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_eta_") + names[i]).c_str () ,  100, -2.5, 2.5) ;
          m_e_sequence_eleIdDaskalakis_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdDaskalakis_ptJ_") + names[i]).c_str () ,  40, 0, 200) ;
        }

    }

//grow e rescale per definizione non servono: servono solo ai singoli pTHat per poterli integrare!
~histosIntegral ()
    {
    }
} ;

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

struct histos
{

  int m_ptHat ;
  std::string m_baseName ;
  int m_counter ;

  TH1F * m_e_single_resol_eta         ;
  TH1F * m_e_single_resol_ptE         ;
  TH1F * m_e_single_resol_ptJ         ;
  TH1F * m_e_single_tkIso_eta         ;
  TH1F * m_e_single_tkIso_ptE         ;
  TH1F * m_e_single_tkIso_ptJ         ;
  TH1F * m_e_single_hadIso_eta        ;
  TH1F * m_e_single_hadIso_ptE        ;
  TH1F * m_e_single_hadIso_ptJ        ;
  TH1F * m_e_single_eleId_eta         ;
  TH1F * m_e_single_eleId_ptE         ;
  TH1F * m_e_single_eleId_ptJ         ;
  TH1F * m_e_single_eleIdLoose_eta    ;
  TH1F * m_e_single_eleIdLoose_ptE    ;
  TH1F * m_e_single_eleIdLoose_ptJ    ;
  TH1F * m_e_single_eleIdTight_eta    ;
  TH1F * m_e_single_eleIdTight_ptE    ;
  TH1F * m_e_single_eleIdTight_ptJ    ;
  TH1F * m_e_single_resol_EMfrac      ;
  TH1F * m_e_single_tkIso_EMfrac      ;
  TH1F * m_e_single_hadIso_EMfrac     ;
  TH1F * m_e_single_eleId_EMfrac      ;
  TH1F * m_e_single_eleIdLoose_EMfrac ;
  TH1F * m_e_single_eleIdTight_EMfrac ;

  TH1F * m_e_sequence_resol_eta          ;
  TH1F * m_e_sequence_resol_ptE          ;
  TH1F * m_e_sequence_resol_ptJ          ;
  TH1F * m_e_sequence_tkIso_eta          ;
  TH1F * m_e_sequence_tkIso_ptE          ;
  TH1F * m_e_sequence_tkIso_ptJ          ;
  TH1F * m_e_sequence_hadIso_eta         ;
  TH1F * m_e_sequence_hadIso_ptE         ;
  TH1F * m_e_sequence_hadIso_ptJ         ;
  TH1F * m_e_sequence_eleId_eta          ;
  TH1F * m_e_sequence_eleId_ptE          ;
  TH1F * m_e_sequence_eleId_ptJ          ;
  TH1F * m_e_sequence_eleIdLoose_eta     ;
  TH1F * m_e_sequence_eleIdLoose_ptE     ;
  TH1F * m_e_sequence_eleIdLoose_ptJ     ;
  TH1F * m_e_sequence_eleIdTight_eta     ;
  TH1F * m_e_sequence_eleIdTight_ptE     ;
  TH1F * m_e_sequence_eleIdTight_ptJ     ;
  TH1F * m_e_sequence_resol_EMfrac       ;
  TH1F * m_e_sequence_tkIso_EMfrac       ;
  TH1F * m_e_sequence_hadIso_EMfrac      ;
  TH1F * m_e_sequence_eleId_EMfrac       ;
  TH1F * m_e_sequence_eleIdLoose_EMfrac  ;
  TH1F * m_e_sequence_eleIdTight_EMfrac  ;

  TH1F * m_e_single_resol_eta_flav[5]            ;
  TH1F * m_e_single_resol_ptE_flav[5]            ;
  TH1F * m_e_single_resol_ptJ_flav[5]            ;
  TH1F * m_e_single_tkIso_eta_flav[5]            ;
  TH1F * m_e_single_tkIso_ptE_flav[5]            ;
  TH1F * m_e_single_tkIso_ptJ_flav[5]            ;
  TH1F * m_e_single_hadIso_eta_flav[5]           ;
  TH1F * m_e_single_hadIso_ptE_flav[5]           ;
  TH1F * m_e_single_hadIso_ptJ_flav[5]           ;
  TH1F * m_e_single_eleId_eta_flav[5]            ;
  TH1F * m_e_single_eleId_ptE_flav[5]            ;
  TH1F * m_e_single_eleId_ptJ_flav[5]            ;
  TH1F * m_e_single_eleIdLoose_eta_flav[5]       ;
  TH1F * m_e_single_eleIdLoose_ptE_flav[5]       ;
  TH1F * m_e_single_eleIdLoose_ptJ_flav[5]       ;
  TH1F * m_e_single_eleIdTight_eta_flav[5]       ;
  TH1F * m_e_single_eleIdTight_ptE_flav[5]       ;
  TH1F * m_e_single_eleIdTight_ptJ_flav[5]       ;
  TH1F * m_e_single_resol_EMfrac_flav[5]         ;
  TH1F * m_e_single_tkIso_EMfrac_flav[5]         ;
  TH1F * m_e_single_hadIso_EMfrac_flav[5]        ;
  TH1F * m_e_single_eleId_EMfrac_flav[5]         ;
  TH1F * m_e_single_eleIdLoose_EMfrac_flav[5]    ;
  TH1F * m_e_single_eleIdTight_EMfrac_flav[5]    ;
                                                 
  TH1F * m_e_sequence_resol_eta_flav[5]          ;
  TH1F * m_e_sequence_resol_ptE_flav[5]          ;
  TH1F * m_e_sequence_resol_ptJ_flav[5]          ;
  TH1F * m_e_sequence_tkIso_eta_flav[5]          ;
  TH1F * m_e_sequence_tkIso_ptE_flav[5]          ;
  TH1F * m_e_sequence_tkIso_ptJ_flav[5]          ;
  TH1F * m_e_sequence_hadIso_eta_flav[5]         ;
  TH1F * m_e_sequence_hadIso_ptE_flav[5]         ;
  TH1F * m_e_sequence_hadIso_ptJ_flav[5]         ;
  TH1F * m_e_sequence_eleId_eta_flav[5]          ;
  TH1F * m_e_sequence_eleId_ptE_flav[5]          ;
  TH1F * m_e_sequence_eleId_ptJ_flav[5]          ;
  TH1F * m_e_sequence_eleIdLoose_eta_flav[5]     ;
  TH1F * m_e_sequence_eleIdLoose_ptE_flav[5]     ;
  TH1F * m_e_sequence_eleIdLoose_ptJ_flav[5]     ;
  TH1F * m_e_sequence_eleIdTight_eta_flav[5]     ;
  TH1F * m_e_sequence_eleIdTight_ptE_flav[5]     ;
  TH1F * m_e_sequence_eleIdTight_ptJ_flav[5]     ;
  TH1F * m_e_sequence_resol_EMfrac_flav[5]       ;
  TH1F * m_e_sequence_tkIso_EMfrac_flav[5]       ;
  TH1F * m_e_sequence_hadIso_EMfrac_flav[5]      ;
  TH1F * m_e_sequence_eleId_EMfrac_flav[5]       ;
  TH1F * m_e_sequence_eleIdLoose_EMfrac_flav[5]  ;
  TH1F * m_e_sequence_eleIdTight_EMfrac_flav[5]  ;

  histos (int ptHat = 0) :
    m_ptHat (ptHat) ,
    m_baseName ("problem_") ,
    m_counter (0)
    {
      char basename[10] ;
      sprintf (basename,"%d_",m_ptHat) ;
      m_baseName = basename ;
      m_e_single_resol_eta  = new TH1F ((m_baseName + std::string ("m_e_single_resol_eta" )).c_str () , (m_baseName + std::string ("m_e_single_resol_eta" )).c_str (), 50, -2.5, 2.5) ;
      m_e_single_resol_ptE  = new TH1F ((m_baseName + std::string ("m_e_single_resol_ptE" )).c_str () , (m_baseName + std::string ("m_e_single_resol_ptE" )).c_str (), 18, 10, 100) ;
      m_e_single_resol_ptJ  = new TH1F ((m_baseName + std::string ("m_e_single_resol_ptJ" )).c_str () , (m_baseName + std::string ("m_e_single_resol_ptJ" )).c_str (), 18, 10, 100) ;
      m_e_single_tkIso_eta  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_eta" )).c_str () , (m_baseName + std::string ("m_e_single_tkIso_eta" )).c_str (), 50, -2.5, 2.5) ;
      m_e_single_tkIso_ptE  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_ptE" )).c_str () , (m_baseName + std::string ("m_e_single_tkIso_ptE" )).c_str (), 18, 10, 100) ;
      m_e_single_tkIso_ptJ  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_ptJ" )).c_str () , (m_baseName + std::string ("m_e_single_tkIso_ptJ" )).c_str (), 18, 10, 100) ;
      m_e_single_hadIso_eta = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_eta")).c_str () , (m_baseName + std::string ("m_e_single_hadIso_eta")).c_str (), 50, -2.5, 2.5) ; 
      m_e_single_hadIso_ptE = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_ptE")).c_str () , (m_baseName + std::string ("m_e_single_hadIso_ptE")).c_str (), 18, 10, 100) ;
      m_e_single_hadIso_ptJ = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_ptJ")).c_str () , (m_baseName + std::string ("m_e_single_hadIso_ptJ")).c_str (), 18, 10, 100) ;
      m_e_single_eleId_eta  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_eta" )).c_str () , (m_baseName + std::string ("m_e_single_eleId_eta" )).c_str (), 50, -2.5, 2.5) ;
      m_e_single_eleId_ptE  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_ptE" )).c_str () , (m_baseName + std::string ("m_e_single_eleId_ptE" )).c_str (), 18, 10, 100) ;
      m_e_single_eleId_ptJ  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_ptJ" )).c_str () , (m_baseName + std::string ("m_e_single_eleId_ptJ" )).c_str (), 18, 10, 100) ;
      m_e_single_eleIdLoose_eta  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_eta" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_eta")).c_str () , 50, -2.5, 2.5) ;
      m_e_single_eleIdLoose_ptE  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_ptE" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_ptE")).c_str () , 18, 10, 100) ;
      m_e_single_eleIdLoose_ptJ  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_ptJ" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_ptJ")).c_str () , 18, 10, 100) ;
      m_e_single_eleIdTight_eta  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_eta" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_eta")).c_str () , 50, -2.5, 2.5) ;
      m_e_single_eleIdTight_ptE  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_ptE" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_ptE")).c_str () , 18, 10, 100) ;
      m_e_single_eleIdTight_ptJ  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_ptJ" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_ptJ")).c_str () , 18, 10, 100) ;
      m_e_single_resol_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_single_resol_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_single_resol_EMfrac" )).c_str (), 50, -2.5, 2.5) ;
      m_e_single_tkIso_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_single_tkIso_EMfrac" )).c_str (), 50, -2.5, 2.5) ;
      m_e_single_hadIso_EMfrac = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_EMfrac")).c_str () , (m_baseName + std::string ("m_e_single_hadIso_EMfrac")).c_str (), 50, -2.5, 2.5) ; 
      m_e_single_eleId_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_single_eleId_EMfrac" )).c_str (), 50, -2.5, 2.5) ;
      m_e_single_eleIdLoose_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_EMfrac")).c_str () , 50, -2.5, 2.5) ;
      m_e_single_eleIdTight_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_EMfrac")).c_str () , 50, -2.5, 2.5) ;
    
      m_e_sequence_resol_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_eta" )).c_str (),  50, -2.5, 2.5) ;
      m_e_sequence_resol_ptE  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptE" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptE" )).c_str (),  18, 10, 100) ;
      m_e_sequence_resol_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptJ" )).c_str (),  18, 10, 100) ;
      m_e_sequence_tkIso_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_eta" )).c_str (),  50, -2.5, 2.5) ;
      m_e_sequence_tkIso_ptE  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptE" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptE" )).c_str (),  18, 10, 100) ;
      m_e_sequence_tkIso_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptJ" )).c_str (),  18, 10, 100) ;
      m_e_sequence_hadIso_eta = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_eta")).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_eta")).c_str (),  50, -2.5, 2.5) ;
      m_e_sequence_hadIso_ptE = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_ptE")).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_ptE")).c_str (),  18, 10, 100) ;
      m_e_sequence_hadIso_ptJ = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_ptJ")).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_ptJ")).c_str (),  18, 10, 100) ;
      m_e_sequence_eleId_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_eta" )).c_str (),  50, -2.5, 2.5) ;
      m_e_sequence_eleId_ptE  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_ptE" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_ptE" )).c_str (),  18, 10, 100) ;
      m_e_sequence_eleId_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_ptJ" )).c_str (),  18, 10, 100) ;
      m_e_sequence_eleIdLoose_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_eta")).c_str () ,  50, -2.5, 2.5) ;
      m_e_sequence_eleIdLoose_ptE  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_ptE" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_ptE")).c_str () ,  18, 10, 100) ;
      m_e_sequence_eleIdLoose_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_ptJ")).c_str () ,  18, 10, 100) ;
      m_e_sequence_eleIdTight_eta  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_eta" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_eta")).c_str () ,  50, -2.5, 2.5) ;
      m_e_sequence_eleIdTight_ptE  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptE" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptE")).c_str () ,  18, 10, 100) ;
      m_e_sequence_eleIdTight_ptJ  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ")).c_str () ,  18, 10, 100) ;
      m_e_sequence_resol_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_sequence_resol_EMfrac" )).c_str (), 50, -2.5, 2.5) ;
      m_e_sequence_tkIso_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_EMfrac" )).c_str (), 50, -2.5, 2.5) ;
      m_e_sequence_hadIso_EMfrac = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_EMfrac")).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_EMfrac")).c_str (), 50, -2.5, 2.5) ; 
      m_e_sequence_eleId_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_EMfrac" )).c_str (), 50, -2.5, 2.5) ;
      m_e_sequence_eleIdLoose_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_EMfrac")).c_str () , 50, -2.5, 2.5) ;
      m_e_sequence_eleIdTight_EMfrac  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_EMfrac" )).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_EMfrac")).c_str () , 50, -2.5, 2.5) ;
    
      std::string names[5] = {"light","c","b","t","g"} ;
      //PG loop over possible flavours
      for (int i = 0 ; i < 5 ; ++i)
        {
          std::cerr << "[pietro][ctor] test " << i << " " << (m_baseName + std::string ("m_e_single_resol_eta_") + names[i]).c_str () << std::endl ;
          m_e_single_resol_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_resol_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_resol_eta_") + names[i]).c_str () , 50, -2.5, 2.5) ;
          m_e_single_resol_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_resol_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_resol_ptE_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_resol_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_resol_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_resol_ptJ_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_tkIso_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_tkIso_eta_") + names[i]).c_str () , 50, -2.5, 2.5) ;
          m_e_single_tkIso_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_tkIso_ptE_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_tkIso_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_tkIso_ptJ_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_hadIso_eta_flav[i] = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_hadIso_eta_") + names[i]).c_str () , 50, -2.5, 2.5) ; 
          m_e_single_hadIso_ptE_flav[i] = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_hadIso_ptE_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_hadIso_ptJ_flav[i] = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_hadIso_ptJ_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleId_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleId_eta_") + names[i]).c_str () , 50, -2.5, 2.5) ;
          m_e_single_eleId_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleId_ptE_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleId_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleId_ptJ_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleIdLoose_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_eta_") + names[i]).c_str () , 50, -2.5, 2.5) ;
          m_e_single_eleIdLoose_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_ptE_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleIdLoose_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_ptJ_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleIdTight_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_eta_") + names[i]).c_str () , 50, -2.5, 2.5) ;
          m_e_single_eleIdTight_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_ptE_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleIdTight_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_ptJ_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_resol_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_resol_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_resol_EMfrac_") + names[i]).c_str ()  , 18, 10, 100) ;
          m_e_single_tkIso_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_tkIso_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_tkIso_EMfrac_") + names[i]).c_str ()  , 18, 10, 100) ;
          m_e_single_hadIso_EMfrac_flav[i] = new TH1F ((m_baseName + std::string ("m_e_single_hadIso_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_hadIso_EMfrac_") + names[i]).c_str () , 18, 10, 100) ; 
          m_e_single_eleId_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleId_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleId_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleIdLoose_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdLoose_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdLoose_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_single_eleIdTight_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_single_eleIdTight_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_single_eleIdTight_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
        
          m_e_sequence_resol_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
          m_e_sequence_resol_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptE_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_resol_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_ptJ_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_tkIso_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
          m_e_sequence_tkIso_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptE_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_tkIso_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_ptJ_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_hadIso_eta_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
          m_e_sequence_hadIso_ptE_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_ptE_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_hadIso_ptJ_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_ptJ_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_eleId_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
          m_e_sequence_eleId_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_ptE_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_eleId_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_ptJ_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_eleIdLoose_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
          m_e_sequence_eleIdLoose_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_ptE_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_eleIdLoose_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_ptJ_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_eleIdTight_eta_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_eta_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_eta_") + names[i]).c_str () ,  50, -2.5, 2.5) ;
          m_e_sequence_eleIdTight_ptE_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptE_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptE_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_eleIdTight_ptJ_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_ptJ_") + names[i]).c_str () ,  18, 10, 100) ;
          m_e_sequence_resol_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_resol_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_resol_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_sequence_tkIso_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_tkIso_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_tkIso_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_sequence_hadIso_EMfrac_flav[i] = new TH1F ((m_baseName + std::string ("m_e_sequence_hadIso_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_hadIso_EMfrac_") + names[i]).c_str () , 18, 10, 100) ; 
          m_e_sequence_eleId_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleId_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleId_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_sequence_eleIdLoose_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdLoose_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdLoose_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
          m_e_sequence_eleIdTight_EMfrac_flav[i]  = new TH1F ((m_baseName + std::string ("m_e_sequence_eleIdTight_EMfrac_") + names[i]).c_str () , (m_baseName + std::string ("m_e_sequence_eleIdTight_EMfrac_") + names[i]).c_str () , 18, 10, 100) ;
        }
//    
//      m_e_single_resol_eta         ->SetDirectory (0) ;
//      m_e_single_resol_ptE         ->SetDirectory (0) ;
//      m_e_single_resol_ptJ         ->SetDirectory (0) ;
//      m_e_single_tkIso_eta         ->SetDirectory (0) ;
//      m_e_single_tkIso_ptE         ->SetDirectory (0) ;
//      m_e_single_tkIso_ptJ         ->SetDirectory (0) ;
//      m_e_single_hadIso_eta        ->SetDirectory (0) ;
//      m_e_single_hadIso_ptE        ->SetDirectory (0) ;
//      m_e_single_hadIso_ptJ        ->SetDirectory (0) ;
//      m_e_single_eleId_eta         ->SetDirectory (0) ;
//      m_e_single_eleId_ptE         ->SetDirectory (0) ;
//      m_e_single_eleId_ptJ         ->SetDirectory (0) ;
//      m_e_single_eleIdLoose_eta    ->SetDirectory (0) ;
//      m_e_single_eleIdLoose_ptE    ->SetDirectory (0) ;
//      m_e_single_eleIdLoose_ptJ    ->SetDirectory (0) ;
//      m_e_single_eleIdTight_eta    ->SetDirectory (0) ;
//      m_e_single_eleIdTight_ptE    ->SetDirectory (0) ;
//      m_e_single_eleIdTight_ptJ    ->SetDirectory (0) ;
//      m_e_single_resol_EMfrac      ->SetDirectory (0) ;
//      m_e_single_tkIso_EMfrac      ->SetDirectory (0) ;
//      m_e_single_hadIso_EMfrac     ->SetDirectory (0) ;
//      m_e_single_eleId_EMfrac      ->SetDirectory (0) ;
//      m_e_single_eleIdLoose_EMfrac ->SetDirectory (0) ;
//      m_e_single_eleIdTight_EMfrac ->SetDirectory (0) ;
//    
//      m_e_sequence_resol_eta          ->SetDirectory (0) ;
//      m_e_sequence_resol_ptE          ->SetDirectory (0) ;
//      m_e_sequence_resol_ptJ          ->SetDirectory (0) ;
//      m_e_sequence_tkIso_eta          ->SetDirectory (0) ;
//      m_e_sequence_tkIso_ptE          ->SetDirectory (0) ;
//      m_e_sequence_tkIso_ptJ          ->SetDirectory (0) ;
//      m_e_sequence_hadIso_eta         ->SetDirectory (0) ;
//      m_e_sequence_hadIso_ptE         ->SetDirectory (0) ;
//      m_e_sequence_hadIso_ptJ         ->SetDirectory (0) ;
//      m_e_sequence_eleId_eta          ->SetDirectory (0) ;
//      m_e_sequence_eleId_ptE          ->SetDirectory (0) ;
//      m_e_sequence_eleId_ptJ          ->SetDirectory (0) ;
//      m_e_sequence_eleIdLoose_eta     ->SetDirectory (0) ;
//      m_e_sequence_eleIdLoose_ptE     ->SetDirectory (0) ;
//      m_e_sequence_eleIdLoose_ptJ     ->SetDirectory (0) ;
//      m_e_sequence_eleIdTight_eta     ->SetDirectory (0) ;
//      m_e_sequence_eleIdTight_ptE     ->SetDirectory (0) ;
//      m_e_sequence_eleIdTight_ptJ     ->SetDirectory (0) ;
//      m_e_sequence_resol_EMfrac       ->SetDirectory (0) ;
//      m_e_sequence_tkIso_EMfrac       ->SetDirectory (0) ;
//      m_e_sequence_hadIso_EMfrac      ->SetDirectory (0) ;
//      m_e_sequence_eleId_EMfrac       ->SetDirectory (0) ;
//      m_e_sequence_eleIdLoose_EMfrac  ->SetDirectory (0) ;
//      m_e_sequence_eleIdTight_EMfrac  ->SetDirectory (0) ;
//    
//      for (int i=0 ; i<5 ; ++i)
//        {
//          m_e_single_resol_eta_flav[i]            ->SetDirectory (0) ;
//          m_e_single_resol_ptE_flav[i]            ->SetDirectory (0) ;
//          m_e_single_resol_ptJ_flav[i]            ->SetDirectory (0) ;
//          m_e_single_tkIso_eta_flav[i]            ->SetDirectory (0) ;
//          m_e_single_tkIso_ptE_flav[i]            ->SetDirectory (0) ;
//          m_e_single_tkIso_ptJ_flav[i]            ->SetDirectory (0) ;
//          m_e_single_hadIso_eta_flav[i]           ->SetDirectory (0) ;
//          m_e_single_hadIso_ptE_flav[i]           ->SetDirectory (0) ;
//          m_e_single_hadIso_ptJ_flav[i]           ->SetDirectory (0) ;
//          m_e_single_eleId_eta_flav[i]            ->SetDirectory (0) ;
//          m_e_single_eleId_ptE_flav[i]            ->SetDirectory (0) ;
//          m_e_single_eleId_ptJ_flav[i]            ->SetDirectory (0) ;
//          m_e_single_eleIdLoose_eta_flav[i]       ->SetDirectory (0) ;
//          m_e_single_eleIdLoose_ptE_flav[i]       ->SetDirectory (0) ;
//          m_e_single_eleIdLoose_ptJ_flav[i]       ->SetDirectory (0) ;
//          m_e_single_eleIdTight_eta_flav[i]       ->SetDirectory (0) ;
//          m_e_single_eleIdTight_ptE_flav[i]       ->SetDirectory (0) ;
//          m_e_single_eleIdTight_ptJ_flav[i]       ->SetDirectory (0) ;
//          m_e_single_resol_EMfrac_flav[i]         ->SetDirectory (0) ;
//          m_e_single_tkIso_EMfrac_flav[i]         ->SetDirectory (0) ;
//          m_e_single_hadIso_EMfrac_flav[i]        ->SetDirectory (0) ;
//          m_e_single_eleId_EMfrac_flav[i]         ->SetDirectory (0) ;
//          m_e_single_eleIdLoose_EMfrac_flav[i]    ->SetDirectory (0) ;
//          m_e_single_eleIdTight_EMfrac_flav[i]    ->SetDirectory (0) ;
//                                                         
//          m_e_sequence_resol_eta_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_resol_ptE_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_resol_ptJ_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_tkIso_eta_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_tkIso_ptE_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_tkIso_ptJ_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_hadIso_eta_flav[i]         ->SetDirectory (0) ;
//          m_e_sequence_hadIso_ptE_flav[i]         ->SetDirectory (0) ;
//          m_e_sequence_hadIso_ptJ_flav[i]         ->SetDirectory (0) ;
//          m_e_sequence_eleId_eta_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_eleId_ptE_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_eleId_ptJ_flav[i]          ->SetDirectory (0) ;
//          m_e_sequence_eleIdLoose_eta_flav[i]     ->SetDirectory (0) ;
//          m_e_sequence_eleIdLoose_ptE_flav[i]     ->SetDirectory (0) ;
//          m_e_sequence_eleIdLoose_ptJ_flav[i]     ->SetDirectory (0) ;
//          m_e_sequence_eleIdTight_eta_flav[i]     ->SetDirectory (0) ;
//          m_e_sequence_eleIdTight_ptE_flav[i]     ->SetDirectory (0) ;
//          m_e_sequence_eleIdTight_ptJ_flav[i]     ->SetDirectory (0) ;
//          m_e_sequence_resol_EMfrac_flav[i]       ->SetDirectory (0) ;
//          m_e_sequence_tkIso_EMfrac_flav[i]       ->SetDirectory (0) ;
//          m_e_sequence_hadIso_EMfrac_flav[i]      ->SetDirectory (0) ;
//          m_e_sequence_eleId_EMfrac_flav[i]       ->SetDirectory (0) ;
//          m_e_sequence_eleIdLoose_EMfrac_flav[i]  ->SetDirectory (0) ;
//          m_e_sequence_eleIdTight_EMfrac_flav[i]  ->SetDirectory (0) ;
//        }    
    }

  void grow ()
    {
      m_counter++ ; 
    }

  void rescale (double factor)
    {
      factor /= m_counter ;
      factor /= 2 ;
      m_e_single_resol_eta         ->Scale (factor) ;
      m_e_single_resol_ptE         ->Scale (factor) ;
      m_e_single_resol_ptJ         ->Scale (factor) ;
      m_e_single_tkIso_eta         ->Scale (factor) ;
      m_e_single_tkIso_ptE         ->Scale (factor) ;
      m_e_single_tkIso_ptJ         ->Scale (factor) ;
      m_e_single_hadIso_eta        ->Scale (factor) ;
      m_e_single_hadIso_ptE        ->Scale (factor) ;
      m_e_single_hadIso_ptJ        ->Scale (factor) ;
      m_e_single_eleId_eta         ->Scale (factor) ;
      m_e_single_eleId_ptE         ->Scale (factor) ;
      m_e_single_eleId_ptJ         ->Scale (factor) ;
      m_e_single_eleIdLoose_eta    ->Scale (factor) ;
      m_e_single_eleIdLoose_ptE    ->Scale (factor) ;
      m_e_single_eleIdLoose_ptJ    ->Scale (factor) ;
      m_e_single_eleIdTight_eta    ->Scale (factor) ;
      m_e_single_eleIdTight_ptE    ->Scale (factor) ;
      m_e_single_eleIdTight_ptJ    ->Scale (factor) ;
      m_e_single_resol_EMfrac      ->Scale (factor) ;
      m_e_single_tkIso_EMfrac      ->Scale (factor) ;
      m_e_single_hadIso_EMfrac     ->Scale (factor) ;
      m_e_single_eleId_EMfrac      ->Scale (factor) ;
      m_e_single_eleIdLoose_EMfrac ->Scale (factor) ;
      m_e_single_eleIdTight_EMfrac ->Scale (factor) ;
    
      m_e_sequence_resol_eta          ->Scale (factor) ;
      m_e_sequence_resol_ptE          ->Scale (factor) ;
      m_e_sequence_resol_ptJ          ->Scale (factor) ;
      m_e_sequence_tkIso_eta          ->Scale (factor) ;
      m_e_sequence_tkIso_ptE          ->Scale (factor) ;
      m_e_sequence_tkIso_ptJ          ->Scale (factor) ;
      m_e_sequence_hadIso_eta         ->Scale (factor) ;
      m_e_sequence_hadIso_ptE         ->Scale (factor) ;
      m_e_sequence_hadIso_ptJ         ->Scale (factor) ;
      m_e_sequence_eleId_eta          ->Scale (factor) ;
      m_e_sequence_eleId_ptE          ->Scale (factor) ;
      m_e_sequence_eleId_ptJ          ->Scale (factor) ;
      m_e_sequence_eleIdLoose_eta     ->Scale (factor) ;
      m_e_sequence_eleIdLoose_ptE     ->Scale (factor) ;
      m_e_sequence_eleIdLoose_ptJ     ->Scale (factor) ;
      m_e_sequence_eleIdTight_eta     ->Scale (factor) ;
      m_e_sequence_eleIdTight_ptE     ->Scale (factor) ;
      m_e_sequence_eleIdTight_ptJ     ->Scale (factor) ;
      m_e_sequence_resol_EMfrac       ->Scale (factor) ;
      m_e_sequence_tkIso_EMfrac       ->Scale (factor) ;
      m_e_sequence_hadIso_EMfrac      ->Scale (factor) ;
      m_e_sequence_eleId_EMfrac       ->Scale (factor) ;
      m_e_sequence_eleIdLoose_EMfrac  ->Scale (factor) ;
      m_e_sequence_eleIdTight_EMfrac  ->Scale (factor) ;
    
      for (int i=0 ; i<5 ; ++i)
        {
          m_e_single_resol_eta_flav[i]            ->Scale (factor) ;
          m_e_single_resol_ptE_flav[i]            ->Scale (factor) ;
          m_e_single_resol_ptJ_flav[i]            ->Scale (factor) ;
          m_e_single_tkIso_eta_flav[i]            ->Scale (factor) ;
          m_e_single_tkIso_ptE_flav[i]            ->Scale (factor) ;
          m_e_single_tkIso_ptJ_flav[i]            ->Scale (factor) ;
          m_e_single_hadIso_eta_flav[i]           ->Scale (factor) ;
          m_e_single_hadIso_ptE_flav[i]           ->Scale (factor) ;
          m_e_single_hadIso_ptJ_flav[i]           ->Scale (factor) ;
          m_e_single_eleId_eta_flav[i]            ->Scale (factor) ;
          m_e_single_eleId_ptE_flav[i]            ->Scale (factor) ;
          m_e_single_eleId_ptJ_flav[i]            ->Scale (factor) ;
          m_e_single_eleIdLoose_eta_flav[i]       ->Scale (factor) ;
          m_e_single_eleIdLoose_ptE_flav[i]       ->Scale (factor) ;
          m_e_single_eleIdLoose_ptJ_flav[i]       ->Scale (factor) ;
          m_e_single_eleIdTight_eta_flav[i]       ->Scale (factor) ;
          m_e_single_eleIdTight_ptE_flav[i]       ->Scale (factor) ;
          m_e_single_eleIdTight_ptJ_flav[i]       ->Scale (factor) ;
          m_e_single_resol_EMfrac_flav[i]         ->Scale (factor) ;
          m_e_single_tkIso_EMfrac_flav[i]         ->Scale (factor) ;
          m_e_single_hadIso_EMfrac_flav[i]        ->Scale (factor) ;
          m_e_single_eleId_EMfrac_flav[i]         ->Scale (factor) ;
          m_e_single_eleIdLoose_EMfrac_flav[i]    ->Scale (factor) ;
          m_e_single_eleIdTight_EMfrac_flav[i]    ->Scale (factor) ;
                                                         
          m_e_sequence_resol_eta_flav[i]          ->Scale (factor) ;
          m_e_sequence_resol_ptE_flav[i]          ->Scale (factor) ;
          m_e_sequence_resol_ptJ_flav[i]          ->Scale (factor) ;
          m_e_sequence_tkIso_eta_flav[i]          ->Scale (factor) ;
          m_e_sequence_tkIso_ptE_flav[i]          ->Scale (factor) ;
          m_e_sequence_tkIso_ptJ_flav[i]          ->Scale (factor) ;
          m_e_sequence_hadIso_eta_flav[i]         ->Scale (factor) ;
          m_e_sequence_hadIso_ptE_flav[i]         ->Scale (factor) ;
          m_e_sequence_hadIso_ptJ_flav[i]         ->Scale (factor) ;
          m_e_sequence_eleId_eta_flav[i]          ->Scale (factor) ;
          m_e_sequence_eleId_ptE_flav[i]          ->Scale (factor) ;
          m_e_sequence_eleId_ptJ_flav[i]          ->Scale (factor) ;
          m_e_sequence_eleIdLoose_eta_flav[i]     ->Scale (factor) ;
          m_e_sequence_eleIdLoose_ptE_flav[i]     ->Scale (factor) ;
          m_e_sequence_eleIdLoose_ptJ_flav[i]     ->Scale (factor) ;
          m_e_sequence_eleIdTight_eta_flav[i]     ->Scale (factor) ;
          m_e_sequence_eleIdTight_ptE_flav[i]     ->Scale (factor) ;
          m_e_sequence_eleIdTight_ptJ_flav[i]     ->Scale (factor) ;
          m_e_sequence_resol_EMfrac_flav[i]       ->Scale (factor) ;
          m_e_sequence_tkIso_EMfrac_flav[i]       ->Scale (factor) ;
          m_e_sequence_hadIso_EMfrac_flav[i]      ->Scale (factor) ;
          m_e_sequence_eleId_EMfrac_flav[i]       ->Scale (factor) ;
          m_e_sequence_eleIdLoose_EMfrac_flav[i]  ->Scale (factor) ;
          m_e_sequence_eleIdTight_EMfrac_flav[i]  ->Scale (factor) ;
        }    

    }
  void write (std::string Name)
    {
      TFile totale ((m_baseName +"totale"+Name+".root").c_str(),"RECREATE") ;
      m_e_single_resol_eta         ->Write () ;
      m_e_single_resol_ptE         ->Write () ;
      m_e_single_resol_ptJ         ->Write () ;
      m_e_single_tkIso_eta         ->Write () ;
      m_e_single_tkIso_ptE         ->Write () ;
      m_e_single_tkIso_ptJ         ->Write () ;
      m_e_single_hadIso_eta        ->Write () ;
      m_e_single_hadIso_ptE        ->Write () ;
      m_e_single_hadIso_ptJ        ->Write () ;
      m_e_single_eleId_eta         ->Write () ;
      m_e_single_eleId_ptE         ->Write () ;
      m_e_single_eleId_ptJ         ->Write () ;
      m_e_single_eleIdLoose_eta    ->Write () ;
      m_e_single_eleIdLoose_ptE    ->Write () ;
      m_e_single_eleIdLoose_ptJ    ->Write () ;
      m_e_single_eleIdTight_eta    ->Write () ;
      m_e_single_eleIdTight_ptE    ->Write () ;
      m_e_single_eleIdTight_ptJ    ->Write () ;
      m_e_single_resol_EMfrac      ->Write () ;
      m_e_single_tkIso_EMfrac      ->Write () ;
      m_e_single_hadIso_EMfrac     ->Write () ;
      m_e_single_eleId_EMfrac      ->Write () ;
      m_e_single_eleIdLoose_EMfrac ->Write () ;
      m_e_single_eleIdTight_EMfrac ->Write () ;
    
      m_e_sequence_resol_eta          ->Write () ;
      m_e_sequence_resol_ptE          ->Write () ;
      m_e_sequence_resol_ptJ          ->Write () ;
      m_e_sequence_tkIso_eta          ->Write () ;
      m_e_sequence_tkIso_ptE          ->Write () ;
      m_e_sequence_tkIso_ptJ          ->Write () ;
      m_e_sequence_hadIso_eta         ->Write () ;
      m_e_sequence_hadIso_ptE         ->Write () ;
      m_e_sequence_hadIso_ptJ         ->Write () ;
      m_e_sequence_eleId_eta          ->Write () ;
      m_e_sequence_eleId_ptE          ->Write () ;
      m_e_sequence_eleId_ptJ          ->Write () ;
      m_e_sequence_eleIdLoose_eta     ->Write () ;
      m_e_sequence_eleIdLoose_ptE     ->Write () ;
      m_e_sequence_eleIdLoose_ptJ     ->Write () ;
      m_e_sequence_eleIdTight_eta     ->Write () ;
      m_e_sequence_eleIdTight_ptE     ->Write () ;
      m_e_sequence_eleIdTight_ptJ     ->Write () ;
      m_e_sequence_resol_EMfrac       ->Write () ;
      m_e_sequence_tkIso_EMfrac       ->Write () ;
      m_e_sequence_hadIso_EMfrac      ->Write () ;
      m_e_sequence_eleId_EMfrac       ->Write () ;
      m_e_sequence_eleIdLoose_EMfrac  ->Write () ;
      m_e_sequence_eleIdTight_EMfrac  ->Write () ;
    
      for (int i=0 ; i<5 ; ++i)
        {
          m_e_single_resol_eta_flav[i]            ->Write () ;
          m_e_single_resol_ptE_flav[i]            ->Write () ;
          m_e_single_resol_ptJ_flav[i]            ->Write () ;
          m_e_single_tkIso_eta_flav[i]            ->Write () ;
          m_e_single_tkIso_ptE_flav[i]            ->Write () ;
          m_e_single_tkIso_ptJ_flav[i]            ->Write () ;
          m_e_single_hadIso_eta_flav[i]           ->Write () ;
          m_e_single_hadIso_ptE_flav[i]           ->Write () ;
          m_e_single_hadIso_ptJ_flav[i]           ->Write () ;
          m_e_single_eleId_eta_flav[i]            ->Write () ;
          m_e_single_eleId_ptE_flav[i]            ->Write () ;
          m_e_single_eleId_ptJ_flav[i]            ->Write () ;
          m_e_single_eleIdLoose_eta_flav[i]       ->Write () ;
          m_e_single_eleIdLoose_ptE_flav[i]       ->Write () ;
          m_e_single_eleIdLoose_ptJ_flav[i]       ->Write () ;
          m_e_single_eleIdTight_eta_flav[i]       ->Write () ;
          m_e_single_eleIdTight_ptE_flav[i]       ->Write () ;
          m_e_single_eleIdTight_ptJ_flav[i]       ->Write () ;
          m_e_single_resol_EMfrac_flav[i]         ->Write () ;
          m_e_single_tkIso_EMfrac_flav[i]         ->Write () ;
          m_e_single_hadIso_EMfrac_flav[i]        ->Write () ;
          m_e_single_eleId_EMfrac_flav[i]         ->Write () ;
          m_e_single_eleIdLoose_EMfrac_flav[i]    ->Write () ;
          m_e_single_eleIdTight_EMfrac_flav[i]    ->Write () ;
                                                         
          m_e_sequence_resol_eta_flav[i]          ->Write () ;
          m_e_sequence_resol_ptE_flav[i]          ->Write () ;
          m_e_sequence_resol_ptJ_flav[i]          ->Write () ;
          m_e_sequence_tkIso_eta_flav[i]          ->Write () ;
          m_e_sequence_tkIso_ptE_flav[i]          ->Write () ;
          m_e_sequence_tkIso_ptJ_flav[i]          ->Write () ;
          m_e_sequence_hadIso_eta_flav[i]         ->Write () ;
          m_e_sequence_hadIso_ptE_flav[i]         ->Write () ;
          m_e_sequence_hadIso_ptJ_flav[i]         ->Write () ;
          m_e_sequence_eleId_eta_flav[i]          ->Write () ;
          m_e_sequence_eleId_ptE_flav[i]          ->Write () ;
          m_e_sequence_eleId_ptJ_flav[i]          ->Write () ;
          m_e_sequence_eleIdLoose_eta_flav[i]     ->Write () ;
          m_e_sequence_eleIdLoose_ptE_flav[i]     ->Write () ;
          m_e_sequence_eleIdLoose_ptJ_flav[i]     ->Write () ;
          m_e_sequence_eleIdTight_eta_flav[i]     ->Write () ;
          m_e_sequence_eleIdTight_ptE_flav[i]     ->Write () ;
          m_e_sequence_eleIdTight_ptJ_flav[i]     ->Write () ;
          m_e_sequence_resol_EMfrac_flav[i]       ->Write () ;
          m_e_sequence_tkIso_EMfrac_flav[i]       ->Write () ;
          m_e_sequence_hadIso_EMfrac_flav[i]      ->Write () ;
          m_e_sequence_eleId_EMfrac_flav[i]       ->Write () ;
          m_e_sequence_eleIdLoose_EMfrac_flav[i]  ->Write () ;
          m_e_sequence_eleIdTight_EMfrac_flav[i]  ->Write () ;
        }    
      
      totale.Close () ;
      //PG mancano i delete
    
    
    }
~histos ()
    {
    }
} ;

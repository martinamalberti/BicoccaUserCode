//COMPILO c++ -o doIt_ptTot -lm `root-config --cflags --libs --glibs` doIt_ptTot.cpp
// $Id: doIt_ptTot.cpp,v 1.1 2008/06/24 13:37:30 tancini Exp $

#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>


#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TText.h"
#include "TLegend.h"

/* mail di Guillelmo

cut 0: 2 leptons in the event with Pt>7
cut 1: preselection, with no jet requirement
cut 2: jet veto
cut 3: MET
cut 4: deltaPhi_ll
cut 5: m_ll<X
cut 6: Pt_max
cut 7: pt_min

*/


struct lepton 
{
    lepton () : m_flav (-1) , m_index (-1) {} ;
    lepton (TLorentzVector * kine, int flav, int index) :
      m_kine (kine),
      m_flav (flav),
      m_index (index)
      {}

    TLorentzVector * m_kine ;
    int m_flav ;
    int m_index ;
} ;           


//  ------------------------------------------------------------


struct lessThan : public std::binary_function<lepton, lepton, bool>
{
  bool operator ()(const lepton & lep1, const lepton & lep2)
    {
      return lep1.m_kine->Pt () < lep2.m_kine->Pt () ;
    }
} ;


//  ------------------------------------------------------------

struct ele 
{
	ele (int id, int iso, int index):
	myId (id),
	myIso (iso),
	myIndex (index)
	{}

	void print () 
	{
	std::cout << "myId " << myId << ", myIso " << myIso << ",myIndex  " << myIndex << std::endl; 
	}
	 int myId ;
	 int myIso ;
	 int myIndex;
};           


struct histos
{
  histos (TString name, int numberOfCuts) : 
    m_name (name) , 
    m_cuts (numberOfCuts) ,
    m_counter (numberOfCuts, 0) ,
    m_eff (numberOfCuts, 0.)
    {
      //PG use the name to build histos :
      TString histoName = m_name + "_histo" ;
    }


  void normalize ()
    {
      for (int x = 0 ; x < m_cuts ; ++x) 
        {
          if (m_counter.at (0) != 0) 
            m_eff.at (x) =  m_counter.at (x) / static_cast<double> (m_counter.at (0)) ;
          else m_eff.at (x) = 0 ;
        }
    }

  void increase (int cutId) { ++m_counter.at (cutId) ; } ;
    
  void printTrend ()
    {
      std::cout << "m_name = " << m_name << std::endl;

      TGraph * trend = new TGraph (m_counter.size ()) ;
      
      for (int i=0 ; i<m_counter.size () ; ++i)
        {
          trend->SetPoint (i,i,m_counter.at (i)) ;
          //std::cout << "trend->SetPoint (" << i << "," << i << "," << m_counter.at (i) << ")" << std::endl ;
        }
      TCanvas * c1 = new TCanvas ;
      c1->SetBottomMargin (0.2);
      char *cuts[11]= {"analyzed", "tag candidates", "tag pt cut", "tag DeltaEta cut", "tag ProdEta cut", "tag M(1,2) cut",
       "elePt cut", "ele Deltaphi cut", "ele M(1,2) cut", "jet veto", "MET"};
      
      /*TH2F *bkg = new TH2F ("bkg","Survived events after each cut",12,0,12,100,0,1800) ;
      bkg->GetXaxis () ->SetTitle ("cut") ;
      bkg->GetYaxis () ->SetTitle ("survived events") ;
      bkg->SetStats (0) ;
      bkg->Draw () ;
      bkg->GetYaxis()->SetTitleOffset(1.2);
      //bkg->GetYaxis()->SetLabelOffset(99);
      bkg->GetXaxis()->SetLabelOffset(99);
      float x, y;
      y = gPad->GetUymin() - 0.2*bkg->GetYaxis()->GetBinWidth(1);
      TText *t = new TText();
      t->SetName ("text") ;
      t->SetTextAngle(60);
      t->SetTextSize(0.03);
      t->SetTextAlign(33);
      for (int a=0; a<11; a++) {
        x = bkg->GetXaxis()->GetBinLowEdge(a+1);
        t->DrawText(x,y,cuts[a]);}
      */
      trend->SetMarkerStyle (8) ;
      trend->SetMarkerColor (2) ;
      trend->Draw ("AP") ;

      c1->Print ("/gwtera2/users/tancini/WWF/CMSSW_1_6_9/src/HiggsAnalysis/HiggsToWWto2l2nu/PLOT/" + m_name + "_trend.gif","gif") ;
      
      delete c1 ;
      delete trend ;
    }


  TGraph * getTrend (int color)
    {
      TGraph * trend = new TGraph (m_counter.size ()) ;
      
      for (int i=0 ; i<m_counter.size () ; ++i)
        {
          trend->SetPoint (i,i+0.5,m_counter.at (i) / static_cast<double> (m_counter.at (0))) ;
          //std::cout << "trend->SetPoint (" << i << "," << i << "," << m_counter.at (i) << ")" << std::endl ;
        }
      trend->SetMarkerStyle (8) ;
      trend->SetMarkerColor (color) ;
     return trend ;
    }

  //!PG histos name
  TString m_name ;

  //!PG number of cuts
  int m_cuts ;

  //!PG counter
  std::vector<int> m_counter ;
  std::vector<double> m_eff ;

  

} ;


//  ------------------------------------------------------------


double deltaPhi (double phi1, double phi2) ;
int selector (TChain * tree, histos & plots) ;


//  --------G L O B A L   V A R S ---------------------

int g_sub_channel;

/* parameters 1 refer to the electron as the most energetic lepton, 
   parameters 2 refer to the muon as the least energetic of the two leptons */

int g_ID1 ; 
int g_ID2 ;
int g_ISO1[2] ;
int g_ISO2[2] ;

double g_hardLEPPtMin[2] ;
double g_softLEPPtMin[2] ;
double g_LEPDPhiMin ;
double g_LEPDPhiMax ;
double g_LEPMinvMin ;
double g_LEPMinvMax ;

double g_METMin ;
double g_PtTotMax;
double g_PtTotMetMax;

double g_hardTAGPtMin ;
double g_softTAGPtMin ;
double g_TAGDProdEtaMax ;
double g_TAGDetaMin ;
double g_TAGMinv ;

double g_ojetPtMin ;
double g_ojetEtaFromMean ;
int g_ojetsMaxNum ;

int g_cutsNum ;

std::string g_prefix ;


//  ========== M A I N    P R O G R A M =========================


int main (int argc, char *argv[])
{ 
  //std::cout << " main " << std::endl;
  if (argc < 17)
    {
      std::cout << "usage:\ndoIt "
				<< "\n\tg_sub_channel"
                << "\n\tg_ID1"
                << "\n\tg_ID2"
                << "\n\tg_ISO1"
                << "\n\tg_ISO2"
                << "\n\tg_hardELEPtMin"    
                << "\n\tg_softELEPtMin"    
                << "\n\tg_ELEDPhiMin"      
                << "\n\tg_ELEDPhiMax"      
                << "\n\tg_ELEMinvMin"      
                << "\n\tg_ELEMinvMax"      
                << "\n\tg_METMin"
		<<"\n\tg_PtTotMax"          
                <<"\n\tg_PtTotMetMax"
                << "\n\tg_hardTAGPtMin"
                << "\n\tg_softTAGPtMin"
                << "\n\tg_TAGDProdEtaMax"
                << "\n\tg_TAGDetaMin"
                << "\n\tg_TAGMinv"
                << "\n\tg_ojetPtMin"
                << "\n\tg_ojetEtaFromMean"
                << "\n\tg_ojetsMaxNum"
                << "\n\tg_prefix"   
                << std::endl ;       
      return 1 ;
    }

  int i = 0 ;
  g_sub_channel = atoi (argv[++i]) ;
  //std::cout << "g_sub_channel = " << g_sub_channel << std::endl;
  g_ID1 = atoi (argv[++i]) ; 
  //std::cout << "g_ID1 = " << g_ID1 << std::endl;
  g_ID2 = atoi (argv[++i]) ;
  //std::cout << "g_ID2 = " << g_ID2 << std::endl; 
  g_ISO1[0] = atoi (argv[++i]) ; 
  g_ISO1[1] = atoi (argv[++i]) ; 
  g_ISO2[0] = atoi (argv[++i]) ;  
  g_ISO2[1] = atoi (argv[++i]) ;  
  //std::cout << "g_ISO1[0] = " << g_ISO1[0] << std::endl;
  //std::cout << "g_ISO1[1] = " << g_ISO1[1] << std::endl;
  //std::cout << "g_ISO2[0] = " << g_ISO2[0] << std::endl;
  //std::cout << "g_ISO2[1] = " << g_ISO2[1] << std::endl;

  g_hardLEPPtMin[0] = atof (argv[++i]) ;
  g_hardLEPPtMin[1] = atof (argv[++i]) ;
  g_softLEPPtMin[0] = atof (argv[++i]) ;
  g_softLEPPtMin[1] = atof (argv[++i]) ;
  //std::cout << "g_hardLEPPtMin[0] = " << g_hardLEPPtMin[0] << std::endl;  
  //std::cout << "g_hardLEPPtMin[1] = " << g_hardLEPPtMin[1] << std::endl;
  //std::cout << "g_softLEPPtMin[0] = " << g_softLEPPtMin[0] << std::endl;  
  //std::cout << "g_softLEPPtMin[1] = " << g_softLEPPtMin[1] << std::endl;
  
  g_LEPDPhiMin   = atof (argv[++i]) ;
  //std::cout << "g_LEPDPhiMin = " << g_LEPDPhiMin << std::endl;
  g_LEPDPhiMax   = atof (argv[++i]) ;
  //std::cout << "g_LEPDPhiMax = " << g_LEPDPhiMax << std::endl;
  g_LEPMinvMin   = atof (argv[++i]) ;
  //std::cout << "g_LEPMinvMin = " << g_LEPMinvMin << std::endl;
  g_LEPMinvMax   = atof (argv[++i]) ;
  //std::cout << "g_LEPMinvMax = " << g_LEPMinvMax << std::endl;

  g_METMin       = atof (argv[++i]) ;
  //std::cout << "g_METMin = " << g_METMin  << std::endl;

  g_PtTotMax = atof (argv[++i]) ;
  g_PtTotMetMax = atof (argv[++i]) ;

  g_hardTAGPtMin = atof (argv[++i]) ;
  //std::cout << "g_hardTAGPtMin = " << g_hardTAGPtMin << std::endl;
  
  g_softTAGPtMin = atof (argv[++i]) ;
  //std::cout << "g_softTAGPtMin = " << g_softTAGPtMin << std::endl;
  
  g_TAGDProdEtaMax = atof (argv[++i]) ;
  //std::cout << "g_TAGDProdEtaMax = " << g_TAGDProdEtaMax << std::endl;
  
  g_TAGDetaMin = atof (argv[++i]) ;
  //std::cout << "g_TAGDetaMin = " << g_TAGDetaMin  << std::endl;
  
  g_TAGMinv = atof (argv[++i]) ;
  //std::cout << "g_TAGMinv = " << g_TAGMinv << std::endl;

  g_ojetPtMin = atof (argv[++i]) ;
  //std::cout << "g_ojetPtMin = " << g_ojetPtMin << std::endl;

  g_ojetEtaFromMean = atof (argv[++i]) ;
  //std::cout << "g_ojetEtaFromMean = " << g_ojetEtaFromMean << std::endl;

  g_ojetsMaxNum = atoi (argv[++i]) ;
  //std::cout << "g_ojetsMaxNum = " << g_ojetsMaxNum << std::endl;

  g_cutsNum = i ;
  g_cutsNum += 1 ; //PG one for the counting before cuts,
  g_cutsNum += 1 ; //PG one for the request of having two tag jets

  g_cutsNum -= 2 ; //VT g_ID1 e g_ID2
  g_cutsNum -= 4 ; //VT g_ISO
  g_cutsNum -= 2 ; //VT g_hardLEPPtMin ans g_softLEPPtMin
  g_cutsNum -= 2 ; //VT u 3 tagli sugli other jet valgono come 1
  
  g_cutsNum += 1 ; //PG less than two leptons satisfying the ID is found     
  g_cutsNum += 1 ; //PG check if the two most transverse-energetic leptons have the same flavour
  g_cutsNum -= 1 ;//subchannel

  //std::cout << "g_cutsNum " << g_cutsNum << std::endl;
  g_prefix = argv[++i] ;


  TChain * chain_H160 = new TChain ("VBFSimpleTree") ;
  chain_H160->Add ("/gwtera2/users/govoni/preselected/VBFH160/NTUPLE*_output_*.root");
  histos h_H160 (g_prefix + "_H160", g_cutsNum);
  selector (chain_H160, h_H160) ;
  //std::cout << "segnale" << std::endl;
  
  TChain * chain_tt0j = new TChain ("VBFSimpleTree") ;
  chain_tt0j->Add ("/gwtera2/users/govoni/preselected/tt0j_alpgen_152/NTUPLE*_output_*.root");
  histos h_tt0j (g_prefix + "_tt0j", g_cutsNum);
  selector (chain_tt0j, h_tt0j) ;

  TChain * chain_tt1j = new TChain ("VBFSimpleTree") ;
  chain_tt1j->Add ("/gwtera2/users/govoni/preselected/tt1j_alpgen_152/NTUPLE*_output_*.root");
  histos h_tt1j (g_prefix + "_tt1j", g_cutsNum);
  selector (chain_tt1j, h_tt1j) ;

  TChain * chain_tt2j = new TChain ("VBFSimpleTree") ;
  chain_tt2j->Add ("/gwtera2/users/govoni/preselected/tt2j_alpgen_152/NTUPLE*_output_*.root");
  histos h_tt2j (g_prefix + "_tt2j", g_cutsNum);
  selector (chain_tt2j, h_tt2j) ;
  
  TChain * chain_tt3j = new TChain ("VBFSimpleTree") ;
  chain_tt3j->Add ("/gwtera2/users/govoni/preselected/tt3j_alpgen_152/NTUPLE*_output_*.root");
  histos h_tt3j (g_prefix + "_tt3j", g_cutsNum);
  selector (chain_tt3j, h_tt3j) ;

  TChain * chain_tt4j = new TChain ("VBFSimpleTree") ;
  chain_tt4j->Add ("/gwtera2/users/govoni/preselected/tt4j_alpgen_152/NTUPLE*_output_*.root");
  histos h_tt4j (g_prefix + "_tt4j", g_cutsNum);
  selector (chain_tt4j, h_tt4j) ;

  TChain * chain_tW = new TChain ("VBFSimpleTree") ;
  chain_tW->Add ("/gwtera2/users/govoni/preselected/tW/NTUPLE*_output_*.root");
  histos h_tW (g_prefix + "_tW", g_cutsNum);
  selector (chain_tW, h_tW) ;
 
  TChain * chain_Zee_0_15 = new TChain ("VBFSimpleTree") ;
  chain_Zee_0_15->Add ("/gwtera2/users/govoni/preselected/Zee_0_15/NTUPLE*_output_*.root");
  histos h_Zee_0_15 (g_prefix + "_Zee_0_15", g_cutsNum);
  selector (chain_Zee_0_15, h_Zee_0_15) ;
  
  TChain * chain_Zee_15_20 = new TChain ("VBFSimpleTree") ;
  chain_Zee_15_20->Add ("/gwtera2/users/govoni/preselected/Zee_15_20/NTUPLE*_output_*.root");
  histos h_Zee_15_20 (g_prefix + "_Zee_15_20", g_cutsNum);
  selector (chain_Zee_15_20, h_Zee_15_20) ;

  TChain * chain_Zee_20_30 = new TChain ("VBFSimpleTree") ;
  chain_Zee_20_30->Add ("/gwtera2/users/govoni/preselected/Zee_20_30/NTUPLE*_output_*.root");
  histos h_Zee_20_30 (g_prefix + "_Zee_20_30", g_cutsNum);
  selector (chain_Zee_20_30, h_Zee_20_30) ;
  
  TChain * chain_Zee_30_50 = new TChain ("VBFSimpleTree") ;
  chain_Zee_30_50->Add ("/gwtera2/users/govoni/preselected/Zee_30_50/NTUPLE*_output_*.root");
  histos h_Zee_30_50 (g_prefix + "_Zee_30_50", g_cutsNum);
  selector (chain_Zee_30_50, h_Zee_30_50) ;
  
  TChain * chain_Zee_50_80 = new TChain ("VBFSimpleTree") ;
  chain_Zee_50_80->Add ("/gwtera2/users/govoni/preselected/Zee_50_80/NTUPLE*_output_*.root");
  histos h_Zee_50_80 (g_prefix + "_Zee_50_80", g_cutsNum);
  selector (chain_Zee_50_80, h_Zee_50_80) ;
  
  TChain * chain_Zee_80_120 = new TChain ("VBFSimpleTree") ;
  chain_Zee_80_120->Add ("/gwtera2/users/govoni/preselected/Zee_80_120/NTUPLE*_output_*.root");
  histos h_Zee_80_120 (g_prefix + "_Zee_80_120", g_cutsNum);
  selector (chain_Zee_80_120, h_Zee_80_120) ;
  
  TChain * chain_Zee_120_170 = new TChain ("VBFSimpleTree") ;
  chain_Zee_120_170->Add ("/gwtera2/users/govoni/preselected/Zee_120_170/NTUPLE*_output_*.root");
  histos h_Zee_120_170 (g_prefix + "_Zee_120_170", g_cutsNum);
  selector (chain_Zee_120_170, h_Zee_120_170) ;
  
  TChain * chain_Zee_170_230 = new TChain ("VBFSimpleTree") ;
  chain_Zee_170_230->Add ("/gwtera2/users/govoni/preselected/Zee_170_230/NTUPLE*_output_*.root");
  histos h_Zee_170_230 (g_prefix + "_Zee_170_230", g_cutsNum);
  selector (chain_Zee_170_230, h_Zee_170_230) ;

  TChain * chain_Wenj_0_15 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_0_15->Add ("/gwtera2/users/govoni/preselected/Wenj_0_15/NTUPLE*_output_*.root");
  histos h_Wenj_0_15 (g_prefix + "_Wenj_0_15", g_cutsNum);
  selector (chain_Wenj_0_15, h_Wenj_0_15) ;
  
  TChain * chain_Wenj_15_20 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_15_20->Add ("/gwtera2/users/govoni/preselected/Wenj_15_20/NTUPLE*_output_*.root");
  histos h_Wenj_15_20 (g_prefix + "_Wenj_15_20", g_cutsNum);
  selector (chain_Wenj_15_20, h_Wenj_15_20) ;
  
  TChain * chain_Wenj_20_30 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_20_30->Add ("/gwtera2/users/govoni/preselected/Wenj_20_30/NTUPLE*_output_*.root");
  histos h_Wenj_20_30 (g_prefix + "_Wenj_20_30", g_cutsNum);
  selector (chain_Wenj_20_30, h_Wenj_20_30) ;
  
  TChain * chain_Wenj_30_50 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_30_50->Add ("/gwtera2/users/govoni/preselected/Wenj_30_50/NTUPLE*_output_*.root");
  histos h_Wenj_30_50 (g_prefix + "_Wenj_30_50", g_cutsNum);
  selector (chain_Wenj_30_50, h_Wenj_30_50) ;
  
  TChain * chain_Wenj_50_80 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_50_80->Add ("/gwtera2/users/govoni/preselected/Wenj_50_80/NTUPLE*_output_*.root");
  histos h_Wenj_50_80 (g_prefix + "_Wenj_50_80", g_cutsNum);
  selector (chain_Wenj_50_80, h_Wenj_50_80) ;
  
  TChain * chain_Wenj_80_120 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_80_120->Add ("/gwtera2/users/govoni/preselected/Wenj_80_120/NTUPLE*_output_*.root");
  histos h_Wenj_80_120 (g_prefix + "_Wenj_80_120", g_cutsNum);
  selector (chain_Wenj_80_120, h_Wenj_80_120) ;
  
  TChain * chain_Wenj_120_170 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_120_170->Add ("/gwtera2/users/govoni/preselected/Wenj_120_170/NTUPLE*_output_*.root");
  histos h_Wenj_120_170 (g_prefix + "_Wenj_120_170", g_cutsNum);
  selector (chain_Wenj_120_170, h_Wenj_120_170) ;

  TChain * chain_Wenj_170_230 = new TChain ("VBFSimpleTree") ;
  chain_Wenj_170_230->Add ("/gwtera2/users/govoni/preselected/Wenj_170_230/NTUPLE*_output_*.root");
  histos h_Wenj_170_230 (g_prefix + "_Wenj_170_230", g_cutsNum) ;

  TChain * chain_Zmumu_0_15 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_0_15->Add ("/gwtera2/users/govoni/preselected/Zmumu_0_15/NTUPLE*_output_*.root");
  histos h_Zmumu_0_15 (g_prefix + "_Zmumu_0_15", g_cutsNum);
  selector (chain_Zmumu_0_15, h_Zmumu_0_15) ;
  
  TChain * chain_Zmumu_15_20 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_15_20->Add ("/gwtera2/users/govoni/preselected/Zmumu_15_20/NTUPLE*_output_*.root");
  histos h_Zmumu_15_20 (g_prefix + "_Zmumu_15_20", g_cutsNum);
  selector (chain_Zmumu_15_20, h_Zmumu_15_20) ;

  TChain * chain_Zmumu_20_30 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_20_30->Add ("/gwtera2/users/govoni/preselected/Zmumu_20_30/NTUPLE*_output_*.root");
  histos h_Zmumu_20_30 (g_prefix + "_Zmumu_20_30", g_cutsNum);
  selector (chain_Zmumu_20_30, h_Zmumu_20_30) ;
  
  TChain * chain_Zmumu_30_50 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_30_50->Add ("/gwtera2/users/govoni/preselected/Zmumu_30_50/NTUPLE*_output_*.root");
  histos h_Zmumu_30_50 (g_prefix + "_Zmumu_30_50", g_cutsNum);
  selector (chain_Zmumu_30_50, h_Zmumu_30_50) ;
  
  TChain * chain_Zmumu_50_80 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_50_80->Add ("/gwtera2/users/govoni/preselected/Zmumu_50_80/NTUPLE*_output_*.root");
  histos h_Zmumu_50_80 (g_prefix + "_Zmumu_50_80", g_cutsNum);
  selector (chain_Zmumu_50_80, h_Zmumu_50_80) ;
  
  TChain * chain_Zmumu_80_120 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_80_120->Add ("/gwtera2/users/govoni/preselected/Zmumu_80_120/NTUPLE*_output_*.root");
  histos h_Zmumu_80_120 (g_prefix + "_Zmumu_80_120", g_cutsNum);
  selector (chain_Zmumu_80_120, h_Zmumu_80_120) ;
  
  TChain * chain_Zmumu_120_170 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_120_170->Add ("/gwtera2/users/govoni/preselected/Zmumu_120_170/NTUPLE*_output_*.root");
  histos h_Zmumu_120_170 (g_prefix + "_Zmumu_120_170", g_cutsNum);
  selector (chain_Zmumu_120_170, h_Zmumu_120_170) ;
  
  TChain * chain_Zmumu_170_230 = new TChain ("VBFSimpleTree") ;
  chain_Zmumu_170_230->Add ("/gwtera2/users/govoni/preselected/Zmumu_170_230/NTUPLE*_output_*.root");
  histos h_Zmumu_170_230 (g_prefix + "_Zmumu_170_230", g_cutsNum);
  selector (chain_Zmumu_170_230, h_Zmumu_170_230) ;

  TChain * chain_Wmunuj_0_15 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_0_15->Add ("/gwtera2/users/govoni/preselected/Wmunuj_0_15/NTUPLE*_output_*.root");
  histos h_Wmunuj_0_15 (g_prefix + "_Wmunuj_0_15", g_cutsNum);
  selector (chain_Wmunuj_0_15, h_Wmunuj_0_15) ;
  
  TChain * chain_Wmunuj_15_20 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_15_20->Add ("/gwtera2/users/govoni/preselected/Wmunuj_15_20/NTUPLE*_output_*.root");
  histos h_Wmunuj_15_20 (g_prefix + "_Wmunuj_15_20", g_cutsNum);
  selector (chain_Wmunuj_15_20, h_Wmunuj_15_20) ;
  
  TChain * chain_Wmunuj_20_30 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_20_30->Add ("/gwtera2/users/govoni/preselected/Wmunuj_20_30/NTUPLE*_output_*.root");
  histos h_Wmunuj_20_30 (g_prefix + "_Wmunuj_20_30", g_cutsNum);
  selector (chain_Wmunuj_20_30, h_Wmunuj_20_30) ;
  
  TChain * chain_Wmunuj_30_50 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_30_50->Add ("/gwtera2/users/govoni/preselected/Wmunuj_30_50/NTUPLE*_output_*.root");
  histos h_Wmunuj_30_50 (g_prefix + "_Wmunuj_30_50", g_cutsNum);
  selector (chain_Wmunuj_30_50, h_Wmunuj_30_50) ;
  
  TChain * chain_Wmunuj_50_80 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_50_80->Add ("/gwtera2/users/govoni/preselected/Wmunuj_50_80/NTUPLE*_output_*.root");
  histos h_Wmunuj_50_80 (g_prefix + "_Wmunuj_50_80", g_cutsNum);
  selector (chain_Wmunuj_50_80, h_Wmunuj_50_80) ;
  
  TChain * chain_Wmunuj_80_120 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_80_120->Add ("/gwtera2/users/govoni/preselected/Wmunuj_80_120/NTUPLE*_output_*.root");
  histos h_Wmunuj_80_120 (g_prefix + "_Wmunuj_80_120", g_cutsNum);
  selector (chain_Wmunuj_80_120, h_Wmunuj_80_120) ;
  
  TChain * chain_Wmunuj_120_170 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_120_170->Add ("/gwtera2/users/govoni/preselected/Wmunuj_120_170/NTUPLE*_output_*.root");
  histos h_Wmunuj_120_170 (g_prefix + "_Wmunuj_120_170", g_cutsNum);
  selector (chain_Wmunuj_120_170, h_Wmunuj_120_170) ;

  TChain * chain_Wmunuj_170_230 = new TChain ("VBFSimpleTree") ;
  chain_Wmunuj_170_230->Add ("/gwtera2/users/govoni/preselected/Wmunuj_170_230/NTUPLE*_output_*.root");
  histos h_Wmunuj_170_230 (g_prefix + "_Wmunuj_170_230", g_cutsNum);
  selector (chain_Wmunuj_170_230, h_Wmunuj_170_230) ;
 
  TChain * chain_WW_incl = new TChain ("VBFSimpleTree") ;
  chain_WW_incl->Add ("/gwtera2/users/govoni/preselected/WW_incl/NTUPLE*_output_*.root");
  histos h_WW_incl (g_prefix + "_WW_incl", g_cutsNum);
  selector (chain_WW_incl, h_WW_incl) ;

  TChain * chain_ZZ_incl = new TChain ("VBFSimpleTree") ;
  chain_ZZ_incl->Add ("/gwtera2/users/govoni/preselected/ZZ_incl/NTUPLE*_output_*.root");
  histos h_ZZ_incl (g_prefix + "_ZZ_incl", g_cutsNum);
  selector (chain_ZZ_incl, h_ZZ_incl) ;
  
  TChain * chain_WZ_incl = new TChain ("VBFSimpleTree") ;
  chain_WZ_incl->Add ("/gwtera2/users/govoni/preselected/WZ_incl/NTUPLE*_output_*.root");
  histos h_WZ_incl (g_prefix + "_WZ_incl", g_cutsNum);
  selector (chain_WZ_incl, h_WZ_incl) ;

  TChain * chain_WW0j = new TChain ("VBFSimpleTree") ;
  chain_WW0j->Add ("/gwtera2/users/govoni/preselected/WW0j/NTUPLE*_output_*.root");
  histos h_WW0j (g_prefix + "_WW0j", g_cutsNum);
  selector (chain_WW0j, h_WW0j) ;

  TChain * chain_WW1j = new TChain ("VBFSimpleTree") ;
  chain_WW1j->Add ("/gwtera2/users/govoni/preselected/WW1j/NTUPLE*_output_*.root");
  histos h_WW1j (g_prefix + "_WW1j", g_cutsNum);
  selector (chain_WW1j, h_WW1j) ;

  TChain * chain_WW2j = new TChain ("VBFSimpleTree") ;
  chain_WW2j->Add ("/gwtera2/users/govoni/preselected/WW2j/NTUPLE*_output_*.root");
  histos h_WW2j (g_prefix + "_WW2j", g_cutsNum);
  selector (chain_WW2j, h_WW2j) ;

  TChain * chain_WW3j = new TChain ("VBFSimpleTree") ;
  chain_WW3j->Add ("/gwtera2/users/govoni/preselected/WW3j/NTUPLE*_output_*.root");
  histos h_WW3j (g_prefix + "_WW3j", g_cutsNum);
  selector (chain_WW3j, h_WW3j) ;


  std::cout << "******************* Computing the significance" << std::endl;
        
  std::vector<double> signalEff ;
  std::vector<double> bkgEff ;
  std::vector<double> m_significance ;

  std::vector<double> m_significance_top ;
  std::vector<double> bkgEff_top ;
  std::vector<double> m_significance_Zeej ;
  std::vector<double> bkgEff_Zeej ;
  std::vector<double> m_significance_Wenj ;
  std::vector<double> bkgEff_Wenj ;
  std::vector<double> m_significance_Zmumuj ;
  std::vector<double> bkgEff_Zmumuj ;
  std::vector<double> m_significance_Wmunuj ;
  std::vector<double> bkgEff_Wmunuj ;
  std::vector<double> m_significance_VV ;
  std::vector<double> bkgEff_VV ;
  std::vector<double> m_significance_WWnj ;
  std::vector<double> bkgEff_WWnj ;

    
  std::stringstream prefisso ; 
  double Lumi = 1000 ; //pb-1
  prefisso << "[ " << g_prefix << " " << Lumi << " ] " ;
  prefisso << " " << g_ID1 ; 
  prefisso << " " << g_ID2 ; 
  prefisso << " " << g_ISO1[0] ; 
  prefisso << " " << g_ISO1[1] ; 
  prefisso << " " << g_ISO2[0] ; 
  prefisso << " " << g_ISO2[1] ; 
  prefisso << " " << g_hardLEPPtMin[0] ; 
  prefisso << " " << g_hardLEPPtMin[1] ; 
  prefisso << " " << g_softLEPPtMin[0] ; 
  prefisso << " " << g_softLEPPtMin[1] ;   
  prefisso << " " << g_LEPDPhiMin   ;
  prefisso << " " << g_LEPDPhiMax   ;
  prefisso << " " << g_LEPMinvMin   ;
  prefisso << " " << g_LEPMinvMax   ;
  prefisso << " " << g_METMin       ;
  prefisso << " " << g_PtTotMax     ;
  prefisso << " " << g_PtTotMetMax     ;
  prefisso << " " << g_hardTAGPtMin ;
  prefisso << " " << g_softTAGPtMin ;
  prefisso << " " << g_TAGDProdEtaMax ;
  prefisso << " " << g_TAGDetaMin ;
  prefisso << " " << g_TAGMinv ;
  prefisso << " " << g_ojetPtMin ;
  prefisso << " " << g_ojetEtaFromMean ;
  prefisso << " " << g_ojetsMaxNum ;
  prefisso << " | " ;  

  for (int i=0 ; i < h_H160.m_eff.size () ; i++)
    {

      std::cout << "i = " << i << " tt0j " << h_tt0j.m_counter.at (i) << std::endl;
      std::cout << "i = " << i << " tt1j " << h_tt1j.m_counter.at (i) << std::endl;
      std::cout << "i = " << i << " tt2j " << h_tt2j.m_counter.at (i) << std::endl;
      std::cout << "i = " << i << " tt3j " << h_tt3j.m_counter.at (i) << std::endl;
      std::cout << "i = " << i << " tt4j " << h_tt4j.m_counter.at (i) << std::endl;
      std::cout << "i = " << i << " tW " << h_tW.m_counter.at (i) << std::endl;

      double weightedSumBkg =

	0.0 * (h_WW0j.m_eff.at (i)) +
	0.00262186 * (h_WW1j.m_eff.at (i)) +
	0.01080531 * (h_WW2j.m_eff.at (i)) +
	0.03151275 * (h_WW3j.m_eff.at (i)) +

	0.02247843 * (h_WW_incl.m_eff.at (i)) +
	//	0.00092079 * (h_WZ.m_eff.at (i)) +
	0.01043897 * (h_WZ_incl.m_eff.at (i)) +
        0.00303743 * (h_ZZ_incl.m_eff.at (i)) +

	0.0 * (h_Wenj_0_15.m_eff.at (i)) +
	0.00548202 * (h_Wenj_120_170.m_eff.at (i)) +
	0.0 * (h_Wenj_15_20.m_eff.at (i)) +
	0.00983974 * (h_Wenj_170_230.m_eff.at (i)) +
	0.0 * (h_Wenj_20_30.m_eff.at (i)) +
	0.19756401 * (h_Wenj_30_50.m_eff.at (i)) +
	0.09886760 * (h_Wenj_50_80.m_eff.at (i)) +
	0.04654173 * (h_Wenj_80_120.m_eff.at (i)) +

	0.0 * (h_Wmunuj_0_15.m_eff.at (i)) +
	0.03263993 * (h_Wmunuj_120_170.m_eff.at (i)) +
	0.12299972 * (h_Wmunuj_15_20.m_eff.at (i)) +
	0.00995037 * (h_Wmunuj_170_230.m_eff.at (i)) +
	0.11258745 * (h_Wmunuj_20_30.m_eff.at (i)) +
	0.05973078 * (h_Wmunuj_30_50.m_eff.at (i)) +
	0.05329841 * (h_Wmunuj_50_80.m_eff.at (i)) +
	0.01952222 * (h_Wmunuj_80_120.m_eff.at (i)) +

	0.0 * (h_Zee_0_15.m_eff.at (i)) +
	0.02800757 * (h_Zee_120_170.m_eff.at (i)) +
	0.03037413 * (h_Zee_15_20.m_eff.at (i)) +
	0.01109598 * (h_Zee_170_230.m_eff.at (i)) +
	0.10617971 * (h_Zee_20_30.m_eff.at (i)) +
	0.13166704 * (h_Zee_30_50.m_eff.at (i)) +
	0.06211555 * (h_Zee_50_80.m_eff.at (i)) +
	0.03777594 * (h_Zee_80_120.m_eff.at (i)) +

	0.0 * (h_Zmumu_0_15.m_eff.at (i)) +
	0.03587226 * (h_Zmumu_120_170.m_eff.at (i)) +
	0.21525333 * (h_Zmumu_15_20.m_eff.at (i)) +
	0.01773629 * (h_Zmumu_170_230.m_eff.at (i)) +
	0.27472434 * (h_Zmumu_20_30.m_eff.at (i)) +
	0.24970454 * (h_Zmumu_30_50.m_eff.at (i)) +
	0.14178039 * (h_Zmumu_50_80.m_eff.at (i)) +
	0.05795135 * (h_Zmumu_80_120.m_eff.at (i)) +

	//	0.25087641 * (h_Ztautau.m_eff.at (i)) +

	0.13043034 * (h_tW.m_eff.at (i)) +
	0.18418359 * (h_tt0j.m_eff.at (i)) +
	0.25245912 * (h_tt1j.m_eff.at (i)) +
	0.05464223 * (h_tt2j.m_eff.at (i)) +
	0.03236199 * (h_tt3j.m_eff.at (i)) +
	0.00859073 * (h_tt4j.m_eff.at (i)) ;

      signalEff.push_back (0.01087805 * h_H160.m_eff.at (i)) ;  
      bkgEff.push_back (weightedSumBkg) ;  
      m_significance.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
				sqrt (Lumi * weightedSumBkg)) ;
							   
	 // valori parziali

double weightedSumBkg_WWnj =
        0.0 * (h_WW0j.m_eff.at (i)) +
        0.00262186 * (h_WW1j.m_eff.at (i)) +
        0.01080531 * (h_WW2j.m_eff.at (i)) +
        0.03151275 * (h_WW3j.m_eff.at (i)) ;

double weightedSumBkg_VV =
	0.00303743 * (h_ZZ_incl.m_eff.at (i)) +
        0.02247843 * (h_WW_incl.m_eff.at (i)) +
	  //        0.00092079 * (h_WZ.m_eff.at (i)) +
	0.01043897 * (h_WZ_incl.m_eff.at (i)) ;

double weightedSumBkg_Wenj =
        0.0 * (h_Wenj_0_15.m_eff.at (i)) +
        0.00548202 * (h_Wenj_120_170.m_eff.at (i)) +
        0.0 * (h_Wenj_15_20.m_eff.at (i)) +
        0.00983974 * (h_Wenj_170_230.m_eff.at (i)) +
        0.0 * (h_Wenj_20_30.m_eff.at (i)) +
        0.19756401 * (h_Wenj_30_50.m_eff.at (i)) +
        0.09886760 * (h_Wenj_50_80.m_eff.at (i)) +
  0.04654173 * (h_Wenj_80_120.m_eff.at (i)) ;

double weightedSumBkg_Wmunuj =
        0.0 * (h_Wmunuj_0_15.m_eff.at (i)) +
        0.03263993 * (h_Wmunuj_120_170.m_eff.at (i)) +
        0.12299972 * (h_Wmunuj_15_20.m_eff.at (i)) +
        0.00995037 * (h_Wmunuj_170_230.m_eff.at (i)) +
        0.11258745 * (h_Wmunuj_20_30.m_eff.at (i)) +
        0.05973078 * (h_Wmunuj_30_50.m_eff.at (i)) +
        0.05329841 * (h_Wmunuj_50_80.m_eff.at (i)) +
	  0.01952222 * (h_Wmunuj_80_120.m_eff.at (i));

double weightedSumBkg_Zeej =
        0.0 * (h_Zee_0_15.m_eff.at (i)) +
        0.02800757 * (h_Zee_120_170.m_eff.at (i)) +
        0.03037413 * (h_Zee_15_20.m_eff.at (i)) +
        0.01109598 * (h_Zee_170_230.m_eff.at (i)) +
        0.10617971 * (h_Zee_20_30.m_eff.at (i)) +
        0.13166704 * (h_Zee_30_50.m_eff.at (i)) +
        0.06211555 * (h_Zee_50_80.m_eff.at (i)) +
	0.03777594 * (h_Zee_80_120.m_eff.at (i)) ;

double weightedSumBkg_Zmumuj =
	  0.0 * (h_Zmumu_0_15.m_eff.at (i)) +
	  0.03587226 * (h_Zmumu_120_170.m_eff.at (i)) +
	  0.21525333 * (h_Zmumu_15_20.m_eff.at (i)) +
	  0.01773629 * (h_Zmumu_170_230.m_eff.at (i)) +
	  0.27472434 * (h_Zmumu_20_30.m_eff.at (i)) +
	  0.24970454 * (h_Zmumu_30_50.m_eff.at (i)) +
	  0.14178039 * (h_Zmumu_50_80.m_eff.at (i)) +
	  0.05795135 * (h_Zmumu_80_120.m_eff.at (i));

	  //	  0.25087641 * (h_Ztautau.m_eff.at (i)) +

double weightedSumBkg_top =
	  0.13043034 * (h_tW.m_eff.at (i)) +
	  0.18418359 * (h_tt0j.m_eff.at (i)) +
	  0.25245912 * (h_tt1j.m_eff.at (i)) +
	  0.05464223 * (h_tt2j.m_eff.at (i)) +
	  0.03236199 * (h_tt3j.m_eff.at (i)) +
	  0.00859073 * (h_tt4j.m_eff.at (i)) ;


      bkgEff_top.push_back (weightedSumBkg_top) ;  
      m_significance_top.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
                               sqrt (Lumi * weightedSumBkg_top)) ;

      bkgEff_Zeej.push_back (weightedSumBkg_Zeej) ;  
      m_significance_Zeej.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
                               sqrt (Lumi * weightedSumBkg_Zeej)) ;
							   
      bkgEff_Wenj.push_back (weightedSumBkg_Wenj) ;  
      m_significance_Wenj.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
                               sqrt (Lumi * weightedSumBkg_Wenj)) ;																				   							   						   
      bkgEff_Zmumuj.push_back (weightedSumBkg_Zmumuj) ;
      m_significance_Zmumuj.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
				     sqrt (Lumi * weightedSumBkg_Zmumuj)) ;

      bkgEff_Wmunuj.push_back (weightedSumBkg_Wmunuj) ;
      m_significance_Wmunuj.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
				     sqrt (Lumi * weightedSumBkg_Wmunuj)) ;         

      bkgEff_VV.push_back (weightedSumBkg_VV) ;  
      m_significance_VV.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
                               sqrt (Lumi * weightedSumBkg_VV)) ;

      bkgEff_WWnj.push_back (weightedSumBkg_WWnj) ;
      m_significance_WWnj.push_back (Lumi * 0.01087805 * h_H160.m_eff.at (i) /
				   sqrt (Lumi * weightedSumBkg_WWnj)) ;

    }
  std::cout << std::endl ;
  
  std::cout << prefisso.str () << "  S  "  ;
  for (int i=0 ; i < signalEff.size () ; ++i) std::cout << " " << signalEff.at (i) ;
  std::cout << std::endl ;
  std::cout << prefisso.str () << "  B  "  ;
  for (int i=0 ; i < bkgEff.size () ; ++i) std::cout << " " << bkgEff.at (i) ;
  std::cout << std::endl ;
  std::cout << prefisso.str () << " SoB "  ;
  for (int i=0 ; i < m_significance.size () ; ++i) std::cout << " " << m_significance.at (i) ;
  std::cout << std::endl ;

  
  std::cout << prefisso.str () << "  B_top  "  ;
  for (int i=0 ; i < bkgEff_top.size () ; ++i) std::cout << " " << bkgEff_top.at (i) ;
  std::cout << std::endl ;
    std::cout << prefisso.str () << " SoB_top "  ;
  for (int i=0 ; i < m_significance_top.size () ; ++i) std::cout << " " << m_significance_top.at (i) ;
  std::cout << std::endl ;
  
  std::cout << prefisso.str () << "  B_Zeej  "  ;
  for (int i=0 ; i < bkgEff_Zeej.size () ; ++i) std::cout << " " << bkgEff_Zeej.at (i) ;
  std::cout << std::endl ;
    std::cout << prefisso.str () << " SoB_Zeej "  ;
  for (int i=0 ; i < m_significance_Zeej.size () ; ++i) std::cout << " " << m_significance_Zeej.at (i) ;
  std::cout << std::endl ;
  
  std::cout << prefisso.str () << "  B_Wenj  "  ;
  for (int i=0 ; i < bkgEff_Wenj.size () ; ++i) std::cout << " " << bkgEff_Wenj.at (i) ;
  std::cout << std::endl ;
    std::cout << prefisso.str () << " SoB_Wenj "  ;
  for (int i=0 ; i < m_significance_Wenj.size () ; ++i) std::cout << " " << m_significance_Wenj.at (i) ;
  std::cout << std::endl ;
  
  std::cout << prefisso.str () << "  B_Zmumuj  "  ;
  for (int i=0 ; i < bkgEff_Zmumuj.size () ; ++i) std::cout << " " << bkgEff_Zmumuj.at (i) ;
  std::cout << std::endl ;
  std::cout << prefisso.str () << " SoB_Zmumuj "  ;
  for (int i=0 ; i < m_significance_Zmumuj.size () ; ++i) std::cout << " " << m_significance_Zmumuj.at (i) ;
  std::cout << std::endl ;

  std::cout << prefisso.str () << "  B_Wmunuj  "  ;
  for (int i=0 ; i < bkgEff_Wmunuj.size () ; ++i) std::cout << " " << bkgEff_Wmunuj.at (i) ;
  std::cout << std::endl ;
  std::cout << prefisso.str () << " SoB_Wmunuj "  ;
  for (int i=0 ; i < m_significance_Wmunuj.size () ; ++i) std::cout << " " << m_significance_Wmunuj.at (i) ;
  std::cout << std::endl ;

  std::cout << prefisso.str () << "  B_VV  "  ;
  for (int i=0 ; i < bkgEff_VV.size () ; ++i) std::cout << " " << bkgEff_VV.at (i) ;
  std::cout << std::endl ;
  std::cout << prefisso.str () << " SoB_VV "  ;
  for (int i=0 ; i < m_significance_VV.size () ; ++i) std::cout << " " << m_significance_VV.at (i) ;
  std::cout << std::endl ;

  std::cout << prefisso.str () << "  B_WWnj  "  ;
  for (int i=0 ; i < bkgEff_WWnj.size () ; ++i) std::cout << " " << bkgEff_WWnj.at (i) ;
  std::cout << std::endl ;
  std::cout << prefisso.str () << " SoB_WWnj "  ;
  for (int i=0 ; i < m_significance_WWnj.size () ; ++i) std::cout << " " << m_significance_WWnj.at (i) ;
  std::cout << std::endl ;
  
  return 0 ;
}


//  ------------------------------------------------------------


double 
deltaPhi (double phi1, double phi2)
  { 
    double deltaphi = fabs (phi1 - phi2) ;
    if (deltaphi > 6.283185308) deltaphi -= 6.283185308 ;
    if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi ;
    return deltaphi ;
  }


//  ------------------------------------------------------------


//!PG main function
int 
selector (TChain * tree, histos & plots)
{
  TClonesArray * tagJets = new TClonesArray ("TLorentzVector") ;
  tree->SetBranchAddress ("tagJets", &tagJets) ;
  TClonesArray * otherJets = new TClonesArray ("TLorentzVector") ;
  tree->SetBranchAddress ("otherJets", &otherJets) ;
  TClonesArray * electrons = new TClonesArray ("TLorentzVector") ;
  tree->SetBranchAddress ("electrons", &electrons) ;
  TClonesArray * muons = new TClonesArray ("TLorentzVector") ;
  tree->SetBranchAddress ("muons", &muons) ;
  TClonesArray * MET = new TClonesArray ("TLorentzVector") ;
  tree->SetBranchAddress ("MET", &MET) ;
  TClonesArray * tracks = new TClonesArray ("TLorentzVector") ;
  tree->SetBranchAddress ("tracks", &tracks) ;
  
  int EleId[100];
  float IsolEleSumPt[100];
  int nEle;
  tree->SetBranchAddress ("nEle", &nEle) ;
  tree->SetBranchAddress ("EleId",EleId ) ;
  tree->SetBranchAddress ("IsolEleSumPt",IsolEleSumPt ) ;

  float IsolMuSumPt[100];
  int nMu ;
  tree->SetBranchAddress ("nMu", &nMu) ;
  tree->SetBranchAddress ("IsolMuSumPt",IsolMuSumPt ) ;


  int nentries = (int) tree->GetEntries () ;
  //std::cout << "nentries " << nentries << std::endl;
  //PG loop over the events
  //  int nentries = 100 ;
  for (int evt = 0 ; evt < nentries ; ++evt)
    {
      tree->GetEntry (evt) ;
      int cutId = 0 ;

      plots.increase (cutId++) ; //PG 0

      if (tagJets->GetEntries () != 2) continue ; plots.increase (cutId++) ; //PG 1 FIXME ctrl numbering
      //if (electrons->GetEntries () < 1 ||
      //  muons->GetEntries () < 1) continue ; plots.increase (cutId++) ; //PG 2
            
      std::vector<lepton> leptons ;
            
      //PG pour electrons into leptons collection
      //PG ---------------------------------------

      //PG loop over electrons
      for (int iele = 0; iele < nEle ; ++iele)
        {
          TLorentzVector * theEle = (TLorentzVector*) (electrons->At (iele)) ;
          lepton dummy (theEle, 0, iele) ;
          leptons.push_back (dummy) ;
        } //PG loop over electrons

      //PG loop over muons
      for (int imu = 0 ; imu < nMu ; ++imu)
        {
          TLorentzVector * theMu = (TLorentzVector*) (muons->At (imu)) ;
          lepton dummy (theMu, 1, imu) ;
          leptons.push_back (dummy) ;
        } //PG loop over muons

//PG this check is not necessary
//PG      if (leptons.size () < 2) continue ;

      //PG 2 LEPTONS
      //PG ---------

/* 
applied after the leptons choice: 
in this case it is possible to differentiate the selections depending on the 
position of each lepton in the pt-sorting.
the algorithm searches the first two most energetic candidates which satisfy 
the ID selections required for the first and second lepton respectively.
If the so-identified first two leptons have the same flavour, the event
is rejected, since it is expected to fall into the 2e or 2mu sub-channel.
Is it certified that we do not have overlap?
*/

      sort (leptons.rbegin (), leptons.rend (), lessThan ()) ;

      lepton primoLEP ;
      lepton secondoLEP ;

      //PG find the first lepton
      int ilep = 0 ;
      for ( ; ilep < leptons.size () ; ++ilep)
        {
          if (leptons.at (ilep).m_flav == 0) //PG electron
            {
               //PG iso check
              bool eleIso = (IsolEleSumPt[leptons.at (ilep).m_index] /  
                             leptons.at (ilep).m_kine->Pt () ) < 0.2 ; // 0.2 per il momento
              if (g_ISO1[0] == 1 && eleIso != 1) continue;
              
              //PG eleID check
              int eleID = EleId[leptons.at (ilep).m_index] ;
              if      (g_ID1 == 100 && (eleID/100) != 1) continue;
              else if (g_ID1 == 10  && ((eleID%100)/10) != 1) continue;
              else if (g_ID1 == 1   && (eleID%10) != 1) continue;
            }
          else //PG muon
            {
              //PG iso check
              bool muIso = (IsolMuSumPt[leptons.at (ilep).m_index] /  
                            leptons.at (ilep).m_kine->Pt () ) < 0.2 ; // FIXME pass as parameter
              if (g_ISO1[1] == 1 && muIso != 1) continue;
            }  
          primoLEP = leptons[ilep] ;
          break ;
        } //PG find the first lepton

      //PG find the second lepton
      for (++ilep ; ilep < leptons.size () ; ++ilep)
        {
          if (leptons.at (ilep).m_flav == 0) //PG electron
            {
               //PG iso check
              bool eleIso = (IsolEleSumPt[leptons.at (ilep).m_index] /  
                             leptons.at (ilep).m_kine->Pt () ) < 0.2 ; // 0.2 per il momento
              if (g_ISO2[0] == 1 && eleIso != 1) continue;
              
              //PG eleID check
              int eleID = EleId[leptons.at (ilep).m_index] ;
              if      (g_ID2 == 100 && (eleID/100) != 1) continue;
              else if (g_ID2 == 10  && ((eleID%100)/10) != 1) continue;
              else if (g_ID2 == 1   && (eleID%10) != 1) continue;
            }
          else //PG muon
            {
              //PG iso check
              bool muIso = (IsolMuSumPt[leptons.at (ilep).m_index] /  
                            leptons.at (ilep).m_kine->Pt () ) < 0.2 ; // FIXME pass as parameter
              if (g_ISO2[1] == 1 && muIso != 1) continue;
            }  
          secondoLEP = leptons[ilep] ;
          break ;
        } //PG find the second lepton

      //PG less than two leptons satisfying the ID is found
      if (primoLEP.m_flav == -1 ||
          secondoLEP.m_flav == -1) continue ; plots.increase (cutId++) ; // 3
      
      //PG check if the two most transverse-energetic leptons have the same flavour
      //if (primoLEP.m_flav == secondoLEP.m_flav) continue ; plots.increase (cutId++) ; // 4
      
	      //PG choose the sub-channel
     //PG ----------------------

     bool select = true ;
     switch (g_sub_channel)
       {
         case 1 :
           if (primoLEP.m_flav != 0 || secondoLEP.m_flav != 0) select = false ;
           break ;
         case 2 :
           if (primoLEP.m_flav != 0 || secondoLEP.m_flav != 1) select = false ;
           break ;
         case 3 :
           if (primoLEP.m_flav != 1 || secondoLEP.m_flav != 0) select = false ;
           break ;
         case 4 :
           if (primoLEP.m_flav != 1 || secondoLEP.m_flav != 1) select = false ;
           break ;
       } ;
     if (!select) continue ; plots.increase (cutId++) ; //PG 2

	  
      //PG find the two most energetic leptons
      //PG -----------------------------------
      
      if (primoLEP.m_kine->Pt () < g_hardLEPPtMin[primoLEP.m_flav]) continue ; plots.increase (cutId++) ; // 5
      if (secondoLEP.m_kine->Pt () < g_softLEPPtMin[secondoLEP.m_flav]) continue ; plots.increase (cutId++) ; // 6
      double lep_deltaPhi = deltaPhi (primoLEP.m_kine->Phi (), secondoLEP.m_kine->Phi ());
      double lep_deltaEta = fabs(primoLEP.m_kine->Eta () - secondoLEP.m_kine->Eta ());
      if (deltaPhi (primoLEP.m_kine->Phi (), secondoLEP.m_kine->Phi ()) < g_LEPDPhiMin) continue ; plots.increase (cutId++) ; //PG 7
      if (deltaPhi (primoLEP.m_kine->Phi (), secondoLEP.m_kine->Phi ()) > g_LEPDPhiMax) continue ; plots.increase (cutId++) ; //PG 8
      TLorentzVector sumLEP = *(primoLEP.m_kine) + *(secondoLEP.m_kine) ;
      if (sumLEP.M () < g_LEPMinvMin) continue ; plots.increase (cutId++) ; //PG 9
      if (sumLEP.M () > g_LEPMinvMax) continue ; plots.increase (cutId++) ; //PG 10

      //PG MET
      //PG ---

      TLorentzVector* met = ((TLorentzVector*) (MET->At(0))) ;
      //correct for muons
      for (int i = 0 ; i < nMu ; i++)
        {
           TLorentzVector * mu_v = (TLorentzVector*) (muons->At (i)) ;
   
           if (mu_v->Pt () > 3)
             {
               met->SetPx (met->Px () - mu_v->Px ()) ;
               met->SetPy (met->Py () - mu_v->Py ()) ;
             }
         }      
      //      if (met->Pt () < g_METMin) continue ; plots.increase (cutId++) ; //PG 11
//      if (((TLorentzVector*) (MET->At (0)))->Pt () < g_METMin) continue ; plots.increase (cutId++) ; //PG 10
      
      //PG 2 TAGS
      //PG ------

      TLorentzVector * primoTAG = (TLorentzVector*) (tagJets->At (0)) ;
      TLorentzVector * secondoTAG = (TLorentzVector*) (tagJets->At (1)) ; 
      //PG get the first two in pt
      if (primoTAG->Pt () < secondoTAG->Pt ())
        {
          primoTAG = (TLorentzVector*) (tagJets->At (1)) ;
          secondoTAG = (TLorentzVector*) (tagJets->At (0)) ; 
        }

      if (met->Pt () < g_METMin) continue ; plots.increase (cutId++) ; //PG 11                                                                                                           

      TLorentzVector total = *primoTAG + *secondoTAG + sumLEP ;
      if (total.Pt () < g_PtTotMax) continue ; plots.increase (cutId++) ; //PG 11                                                                                                 
      total += *met ;
      if (total.Pt () > g_PtTotMetMax) continue ; plots.increase (cutId++) ; //PG 11                                                                                                                                       

      if (primoTAG->Pt () < g_hardTAGPtMin) continue ; plots.increase (cutId++) ; //PG 12
      if (secondoTAG->Pt () < g_softTAGPtMin) continue ; plots.increase (cutId++) ; //PG 13
      
      if (primoTAG->Eta () * secondoTAG->Eta () > g_TAGDProdEtaMax) continue ; plots.increase (cutId++) ; //PG 14
      if (fabs (primoTAG->Eta () - secondoTAG->Eta ()) < g_TAGDetaMin) continue ; plots.increase (cutId++) ; //PG 15
      TLorentzVector sumTAG = *primoTAG + *secondoTAG ;
      if (sumTAG.M () < g_TAGMinv) continue ; plots.increase (cutId++) ; //PG 16

      //PG JET VETO
      //PG --------

      //PG loop over other jets
      double etaMean = 0.5*(primoTAG->Eta () + secondoTAG->Eta ()); 
      int ojetsNum = 0 ;
      for (int ojetIt = 0 ; ojetIt < otherJets->GetEntries () ; ++ojetIt)
        {
          if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Pt () < g_ojetPtMin) continue ;
          if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () < primoTAG->Eta () ||
               ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () > secondoTAG->Eta ()) continue ;
	  //if ((((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () - etaMean) > g_ojetEtaFromMean) continue;
          ++ojetsNum ;        
        } //PG loop over other jets
      if (ojetsNum > g_ojetsMaxNum) continue ; plots.increase (cutId++) ; //PG 17


      /*
      double primoDR = primoELE->DeltaR (*primoTAG) ;
      if (primoDR < primoELE->DeltaR (*secondoTAG)) primoDR = fabs (primoELE->Eta () - secondoTAG->Eta ()) ;
      
      double secondoDR = secondoELE->DeltaR (*primoTAG) ;
      if (secondoDR < secondoELE->DeltaR (*secondoTAG)) secondoDR = fabs (secondoELE->Eta () - secondoTAG->Eta ()) ;

      if (secondoDR < g_eleTagDR || primoDR < g_eleTagDR) continue ; plots.increase (cutId++) ; //PG 12
      */

      //PG Ztautau vetos
      //PG -------------
      
      //PG the two electrons should not be opposite to each other
      
      TVector2 primoLEPT (primoLEP.m_kine->X (), primoLEP.m_kine->Y ()) ;
      TVector2 secondoLEPT (secondoLEP.m_kine->X (), secondoLEP.m_kine->Y ()) ;
      TVector2 METT (met->X (), met->Y ()) ;

      double Sum = METT * primoLEPT + METT * secondoLEPT / (1 + primoLEPT * secondoLEPT) ;
      double Dif = METT * primoLEPT - METT * secondoLEPT / (1 - primoLEPT * secondoLEPT) ;
      
      TVector2 METT1 = 0.5 * (Sum + Dif) * primoLEPT ;
      TVector2 METT2 = 0.5 * (Sum - Dif) * secondoLEPT ;
      
      double ptNu1 = METT1.Mod () / cos (primoLEP.m_kine->Theta ()) ;
      double ptNu2 = METT2.Mod () / cos (secondoLEP.m_kine->Theta ()) ;

    } //PG loop over the events

  plots.normalize () ;

  delete tagJets  ;  
  delete otherJets  ;
  delete electrons  ;
  delete muons  ;    
  delete MET  ;      
  delete tracks  ;   

  return 0;
  
}

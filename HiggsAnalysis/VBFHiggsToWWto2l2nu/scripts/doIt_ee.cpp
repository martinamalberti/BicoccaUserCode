//COMPILO c++ -o doIt_ee -lm `root-config --cflags --libs --glibs` doIt_ee.cpp 
// $Id: doIt_ee.cpp,v 1.3 2008/05/08 12:36:22 tancini Exp $

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

int g_ID1 ; 
int g_ID2 ;
int g_ISO1 ;
int g_ISO2 ;

double g_hardELEPtMin ;
double g_softELEPtMin ;
double g_ELEDPhiMin ;
double g_ELEDPhiMax ;
double g_ELEMinvMin ;
double g_ELEMinvMax ;

double g_METMin ;

double g_hardTAGPtMin;
double g_softTAGPtMin;
double g_TAGDProdEtaMax;
double g_TAGDetaMin;
double g_TAGMinv;

double g_ojetPtMin;
double g_ojetEtaFromMean;
int g_ojetsMaxNum;

int g_cutsNum ;

std::string g_prefix ;


//  ========== M A I N    P R O G R A M =========================


int main (int argc, char *argv[])
{ 

  if (argc < 16)
    {
      std::cout << "usage:\ndoIt "
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

  g_ID1 = atoi (argv[++i]) ; 
  g_ID2 = atoi (argv[++i]) ; 
  g_ISO1 = atoi (argv[++i]) ; 
  g_ISO2 = atoi (argv[++i]) ; 

  g_hardELEPtMin = atof (argv[++i]) ; 
  g_softELEPtMin = atof (argv[++i]) ; 
  g_ELEDPhiMin   = atof (argv[++i]) ;
  g_ELEDPhiMax   = atof (argv[++i]) ;
  g_ELEMinvMin   = atof (argv[++i]) ;
  g_ELEMinvMax   = atof (argv[++i]) ;

  g_METMin       = atof (argv[++i]) ;


 g_hardTAGPtMin = atof (argv[++i]) ;
 g_softTAGPtMin = atof (argv[++i]) ;
 g_TAGDProdEtaMax = atof (argv[++i]) ;
 g_TAGDetaMin = atof (argv[++i]) ;
 g_TAGMinv = atof (argv[++i]) ;

 g_ojetPtMin = atof (argv[++i]) ;
 g_ojetEtaFromMean = atof (argv[++i]) ;
 g_ojetsMaxNum = atoi (argv[++i]) ;

  g_cutsNum = i ;
  g_cutsNum += 1 ; //PG one for the counting before cuts,
  g_cutsNum += 1 ; //PG one for the request of having two tag jets
  g_cutsNum += 1 ; //PG num of electrons
  g_cutsNum -= 3 ; //VT i 4 tagli dell'eleid valgono come 1
  g_cutsNum -= 2 ; //VT u 3 tagli sugli other jet valgono come 1

  g_prefix = argv[++i] ;

  TChain * chain_H160 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_H160->Add ("/gwtera2/users/govoni/ntuples/VBFH160/NTUPLE*_output_*.root");
  histos h_H160 (g_prefix + "_H160", g_cutsNum);
  selector (chain_H160, h_H160) ;
  //h_H160.printTrend () ;

  TChain * chain_tt0j = new TChain ("ntpla/VBFSimpleTree") ;
  chain_tt0j->Add ("/gwtera2/users/govoni/ntuples/tt0j/NTUPLE*_output_*.root");
  histos h_tt0j (g_prefix + "_tt0j", g_cutsNum);
  selector (chain_tt0j, h_tt0j) ;
  //h_tt0j.printTrend () ;

  TChain * chain_tt1j = new TChain ("ntpla/VBFSimpleTree") ;
  chain_tt1j->Add ("/gwtera2/users/govoni/ntuples/tt1j/NTUPLE*_output_*.root");
  histos h_tt1j (g_prefix + "_tt1j", g_cutsNum);
  selector (chain_tt1j, h_tt1j) ;
  //h_tt1j.printTrend () ;

  TChain * chain_tt2j = new TChain ("ntpla/VBFSimpleTree") ;
  chain_tt2j->Add ("/gwtera2/users/govoni/ntuples/tt2j/NTUPLE*_output_*.root");
  histos h_tt2j (g_prefix + "_tt2j", g_cutsNum);
  selector (chain_tt2j, h_tt2j) ;
  //h_tt2j.printTrend () ;
  
  TChain * chain_tt3j = new TChain ("ntpla/VBFSimpleTree") ;
  chain_tt3j->Add ("/gwtera2/users/govoni/ntuples/tt3j/NTUPLE*_output_*.root");
  histos h_tt3j (g_prefix + "_tt3j", g_cutsNum);
  selector (chain_tt3j, h_tt3j) ;
  //h_tt3j.printTrend () ;

  TChain * chain_tt4j = new TChain ("ntpla/VBFSimpleTree") ;
  chain_tt4j->Add ("/gwtera2/users/govoni/ntuples/tt4j/NTUPLE*_output_*.root");
  histos h_tt4j (g_prefix + "_tt4j", g_cutsNum);
  selector (chain_tt4j, h_tt4j) ;
  //h_tt4j.printTrend () ;

  TChain * chain_tW = new TChain ("ntpla/VBFSimpleTree") ;
  chain_tW->Add ("/gwtera2/users/govoni/ntuples/tW/NTUPLE*_output_*.root");
  histos h_tW (g_prefix + "_tW", g_cutsNum);
  selector (chain_tW, h_tW) ;
  //h_tW.printTrend () ;
 
  TChain * chain_Zee_0_15 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_0_15->Add ("/gwtera2/users/govoni/ntuples/Zee_0_15/NTUPLE*_output_*.root");
  histos h_Zee_0_15 (g_prefix + "_Zee_0_15", g_cutsNum);
  selector (chain_Zee_0_15, h_Zee_0_15) ;
  //h_Zee_0_15.printTrend () ;
  
  TChain * chain_Zee_15_20 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_15_20->Add ("/gwtera2/users/govoni/ntuples/Zee_15_20/NTUPLE*_output_*.root");
  histos h_Zee_15_20 (g_prefix + "_Zee_15_20", g_cutsNum);
  selector (chain_Zee_15_20, h_Zee_15_20) ;
  //h_Zee_15_20.printTrend () ;

  TChain * chain_Zee_20_30 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_20_30->Add ("/gwtera2/users/govoni/ntuples/Zee_20_30/NTUPLE*_output_*.root");
  histos h_Zee_20_30 (g_prefix + "_Zee_20_30", g_cutsNum);
  selector (chain_Zee_20_30, h_Zee_20_30) ;
  //h_Zee_20_30.printTrend () ;
  
  TChain * chain_Zee_30_50 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_30_50->Add ("/gwtera2/users/govoni/ntuples/Zee_30_50/NTUPLE*_output_*.root");
  histos h_Zee_30_50 (g_prefix + "_Zee_30_50", g_cutsNum);
  selector (chain_Zee_30_50, h_Zee_30_50) ;
  //h_Zee_30_50.printTrend () ;
  
  TChain * chain_Zee_50_80 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_50_80->Add ("/gwtera2/users/govoni/ntuples/Zee_50_80/NTUPLE*_output_*.root");
  histos h_Zee_50_80 (g_prefix + "_Zee_50_80", g_cutsNum);
  selector (chain_Zee_50_80, h_Zee_50_80) ;
  //h_Zee_50_80.printTrend () ;
  
  TChain * chain_Zee_80_120 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_80_120->Add ("/gwtera2/users/govoni/ntuples/Zee_80_120/NTUPLE*_output_*.root");
  histos h_Zee_80_120 (g_prefix + "_Zee_80_120", g_cutsNum);
  selector (chain_Zee_80_120, h_Zee_80_120) ;
  //h_Zee_80_120.printTrend () ;
  
  TChain * chain_Zee_120_170 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_120_170->Add ("/gwtera2/users/govoni/ntuples/Zee_120_170/NTUPLE*_output_*.root");
  histos h_Zee_120_170 (g_prefix + "_Zee_120_170", g_cutsNum);
  selector (chain_Zee_120_170, h_Zee_120_170) ;
  //h_Zee_120_170.printTrend () ;
  
  TChain * chain_Zee_170_230 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Zee_170_230->Add ("/gwtera2/users/govoni/ntuples/Zee_170_230/NTUPLE*_output_*.root");
  histos h_Zee_170_230 (g_prefix + "_Zee_170_230", g_cutsNum);
  selector (chain_Zee_170_230, h_Zee_170_230) ;
  //h_Zee_170_230.printTrend () ;

  TChain * chain_Wenj_0_15 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_0_15->Add ("/gwtera2/users/govoni/ntuples/Wenj_0_15/NTUPLE*_output_*.root");
  histos h_Wenj_0_15 (g_prefix + "_Wenj_0_15", g_cutsNum);
  selector (chain_Wenj_0_15, h_Wenj_0_15) ;
  //h_Wenj_0_15.printTrend () ;
  
  TChain * chain_Wenj_15_20 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_15_20->Add ("/gwtera2/users/govoni/ntuples/Wenj_15_20/NTUPLE*_output_*.root");
  histos h_Wenj_15_20 (g_prefix + "_Wenj_15_20", g_cutsNum);
  selector (chain_Wenj_15_20, h_Wenj_15_20) ;
  //h_Wenj_15_20.printTrend () ;
  
  TChain * chain_Wenj_20_30 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_20_30->Add ("/gwtera2/users/govoni/ntuples/Wenj_20_30/NTUPLE*_output_*.root");
  histos h_Wenj_20_30 (g_prefix + "_Wenj_20_30", g_cutsNum);
  selector (chain_Wenj_20_30, h_Wenj_20_30) ;
  //h_Wenj_20_30.printTrend () ;
  
  TChain * chain_Wenj_30_50 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_30_50->Add ("/gwtera2/users/govoni/ntuples/Wenj_30_50/NTUPLE*_output_*.root");
  histos h_Wenj_30_50 (g_prefix + "_Wenj_30_50", g_cutsNum);
  selector (chain_Wenj_30_50, h_Wenj_30_50) ;
  //h_Wenj_30_50.printTrend () ;
  
  TChain * chain_Wenj_50_80 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_50_80->Add ("/gwtera2/users/govoni/ntuples/Wenj_50_80/NTUPLE*_output_*.root");
  histos h_Wenj_50_80 (g_prefix + "_Wenj_50_80", g_cutsNum);
  selector (chain_Wenj_50_80, h_Wenj_50_80) ;
  //h_Wenj_50_80.printTrend () ;
  
  TChain * chain_Wenj_80_120 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_80_120->Add ("/gwtera2/users/govoni/ntuples/Wenj_80_120/NTUPLE*_output_*.root");
  histos h_Wenj_80_120 (g_prefix + "_Wenj_80_120", g_cutsNum);
  selector (chain_Wenj_80_120, h_Wenj_80_120) ;
  //h_Wenj_80_120.printTrend () ;
  
  TChain * chain_Wenj_120_170 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_120_170->Add ("/gwtera2/users/govoni/ntuples/Wenj_120_170/NTUPLE*_output_*.root");
  histos h_Wenj_120_170 (g_prefix + "_Wenj_120_170", g_cutsNum);
  selector (chain_Wenj_120_170, h_Wenj_120_170) ;
  //h_Wenj_120_170.printTrend () ;

  TChain * chain_Wenj_170_230 = new TChain ("ntpla/VBFSimpleTree") ;
  chain_Wenj_170_230->Add ("/gwtera2/users/govoni/ntuples/Wenj_170_230/NTUPLE*_output_*.root");
  histos h_Wenj_170_230 (g_prefix + "_Wenj_170_230", g_cutsNum);
  selector (chain_Wenj_170_230, h_Wenj_170_230) ;
  //h_Wenj_170_230.printTrend () ;
 
  TChain * chain_WW_incl = new TChain ("ntpla/VBFSimpleTree") ;
  chain_WW_incl->Add ("/gwtera2/users/govoni/ntuples/WW_incl/NTUPLE*_output_*.root");
  histos h_WW_incl (g_prefix + "_WW_incl", g_cutsNum);
  selector (chain_WW_incl, h_WW_incl) ;
  //h_WW_incl.printTrend () ;

  TChain * chain_ZZ_incl = new TChain ("ntpla/VBFSimpleTree") ;
  chain_ZZ_incl->Add ("/gwtera2/users/govoni/ntuples/ZZ_incl/NTUPLE*_output_*.root");
  histos h_ZZ_incl (g_prefix + "_ZZ_incl", g_cutsNum);
  selector (chain_ZZ_incl, h_ZZ_incl) ;
  //h_ZZ_incl.printTrend () ;
  
  TChain * chain_WZ_incl = new TChain ("ntpla/VBFSimpleTree") ;
  chain_WZ_incl->Add ("/gwtera2/users/govoni/ntuples/WZ_incl/NTUPLE*_output_*.root");
  histos h_WZ_incl (g_prefix + "_WZ_incl", g_cutsNum);
  selector (chain_WZ_incl, h_WZ_incl) ;
  //h_WZ_incl.printTrend () ;


  std::cout << "******************* Computing the significance" << std::endl;
        
  std::vector<double> signalEff ;
  std::vector<double> bkgEff ;
  std::vector<double> m_significance ;
  
  std::stringstream prefisso ; 
  double Lumi = 1000 ; //pb-1
  prefisso << "[ " << g_prefix << " " << Lumi << " ] " ;
  prefisso << " " << g_ID1 ; 
  prefisso << " " << g_ID2 ; 
  prefisso << " " << g_ISO1 ; 
  prefisso << " " << g_ISO2 ; 
  prefisso << " " << g_hardELEPtMin ; 
  prefisso << " " << g_softELEPtMin ; 
  prefisso << " " << g_ELEDPhiMin   ;
  prefisso << " " << g_ELEDPhiMax   ;
  prefisso << " " << g_ELEMinvMin   ;
  prefisso << " " << g_ELEMinvMax   ;
  prefisso << " " << g_METMin       ;
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
      double weightedSumBkg = 
        334.51 * 0.470709 * (h_tt0j.m_eff.at (i)) + 
	90.23 *  0.557242 * (h_tt1j.m_eff.at (i)) + 
	8.75 * 0.624557 * (h_tt2j.m_eff.at (i)) + 
	3.16 * 0.681871 * (h_tt3j.m_eff.at (i)) + 
	0.82 * 0.715106 * (h_tt4j.m_eff.at (i)) + //ttnj
        62 * 0.454879 * (h_tW.m_eff.at (i)) + //tW

      353.1 * 0.293118 * (h_Zee_15_20.m_eff.at (i)) + 
        326.7 * 0.342509 * (h_Zee_20_30.m_eff.at (i)) + 
        227.0 * 0.418665 * (h_Zee_30_50.m_eff.at (i)) + 
        93.17 * 0.50172 * (h_Zee_50_80.m_eff.at (i)) + 
        31.48 * 0.57292 * (h_Zee_80_120.m_eff.at (i)) + 
        9.63 * 0.622112 * (h_Zee_120_170.m_eff.at (i)) + 
        2.92 * 0.67604 * (h_Zee_170_230.m_eff.at (i)) + // Zeej

	17170 * 0.1103 * 0.03775 * (h_Wenj_0_15.m_eff.at (i)) + 
        1722 * 0.0314208 * (h_Wenj_15_20.m_eff.at (i)) + 
        1914 * 0.0531364 * (h_Wenj_20_30.m_eff.at (i)) + 
        1541 * 0.0990513 * (h_Wenj_30_50.m_eff.at (i)) + 
        706.2 * 0.14962 * (h_Wenj_50_80.m_eff.at (i)) + 
        236.2 * 0.57292 * (h_Wenj_80_120.m_eff.at (i))+ 
        70.72 * 0.208953 * (h_Wenj_120_170.m_eff.at (i)) + 
        20.36 * 0.223205 * (h_Wenj_170_230.m_eff.at (i)) + // Wenj

        114.3 * 0.170052 * (h_WW_incl.m_eff.at (i)) + //WW_incl
        16.1 * 0.19741 * (h_ZZ_incl.m_eff.at (i)) + //ZZ_incl
        49.9 * 0.170128 * (h_WZ_incl.m_eff.at (i)) ; //WZ_incl

      signalEff.push_back (0.325 * 0.24 * 0.48371 * h_H160.m_eff.at (i)) ;  
      bkgEff.push_back (weightedSumBkg) ;  
      m_significance.push_back (Lumi * 0.325 * 0.24 * 0.48371 * h_H160.m_eff.at (i) /
                               sqrt (Lumi * weightedSumBkg)) ;
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

  int nentries = (int) tree->GetEntries () ;

  //PG loop over the events
  //  int nentries = 100 ;
  for (int evt = 0 ; evt < nentries ; ++evt)
    {
      tree->GetEntry (evt) ;
      int cutId = 0 ;

      plots.increase (cutId++) ; //PG 0

      if (tagJets->GetEntries () != 2) continue ; plots.increase (cutId++) ; //PG 1
      if (electrons->GetEntries () < 2) continue ; plots.increase (cutId++) ; //PG 2
	  
	  
	  // VT ele id
	  
	  std::vector<int> eleind; //VT vettore di indici di TLorentzVector sopra soglia
      float PtTh = 10; //VT modificabile //find the electron with Pt >10 GeV
      for (int i=0; i<nEle; i++)
	  {
		  TLorentzVector * theEle = (TLorentzVector*) (electrons->At (i)) ;   
		  if(theEle->Pt() > PtTh) eleind.push_back(i);
	  }

      std::vector<std::pair <ele,ele> > pairsv; //VT vettore di coppie di strutture
	  //VTfill the vector with all the possibile pair of (non identical) electrons with pt >10 GeV
      for (int u =0; u < eleind.size(); u++)
	  {
		  TLorentzVector * theEle1 = (TLorentzVector*) (electrons->At (eleind[u])) ;
		  bool ele1Iso = ( IsolEleSumPt[eleind[u]] /  theEle1->Pt() ) < 0.2; // 0.2 per il momento
		  ele ele1 (EleId[eleind[u]],int(ele1Iso), eleind[u] ); // per me struct a cui aggiungo indice u
		  for (int uu =0; uu < eleind.size(); uu++)
		  {
			  if (uu == u) continue;
			  TLorentzVector * theEle2 = (TLorentzVector*) (electrons->At (eleind[uu])) ;
			  bool ele2Iso = ( IsolEleSumPt[eleind[uu]] /  theEle2->Pt() ) < 0.2;
			  ele ele2 (EleId[eleind[uu]],int(ele2Iso), eleind[uu] );
			  pairsv.push_back ( std::pair<ele,ele> (ele1,ele2) );
		 }   
      }

     //VT cerco una coppia con le caratteristiche richieste dall'esterno g_ID1, g_ID2, g_ISO1, g_ISO2
	 //VT e riempio un vettore con le coppie degli indici sopravvisuti
	 std::vector<std::pair <int,int> > survived; // vettore di coppie di strutture
 
	 for(int u=0; u < pairsv.size(); u++)
	 {
		 if (g_ID1 == 100 && (pairsv[u].first.myId/100) != 1) continue;
		 else if (g_ID1 == 10 && ((pairsv[u].first.myId%100)/10) != 1) continue;
		 else if (g_ID1 == 1 && (pairsv[u].first.myId%10) != 1) continue;

		 if (g_ID2 == 100 && (pairsv[u].second.myId/100) != 1) continue;
		 else if (g_ID2 == 10 && ((pairsv[u].second.myId%100)/10) != 1) continue;
		 else if (g_ID2 == 1 && (pairsv[u].second.myId%10) != 1) continue;
		 
		 if (g_ISO1 == 1 && pairsv[u].first.myIso != 1) continue;
		 if (g_ISO2 == 1 && pairsv[u].second.myIso != 1) continue;
		 
		survived.push_back (std::pair<int,int> (pairsv[u].first.myIndex, pairsv[u].second.myIndex)) ;
	 }
	 
	if (survived.size() < 1) continue ; plots.increase (cutId++) ; //3 //VT coppie candidate di elettroni che superano eleID


	 //VT se trovo piu' di una coppia prendo quella con somma pt maggiore???
     int index1 = -1;
     int index2 = -1;
	 double previousSumPt = 0.0;
	 for (int u=0; u < survived.size(); u++)
	 {
    	double sumPt = ((TLorentzVector*) (electrons->At (survived[u].first)))->Pt() + ((TLorentzVector*) (electrons->At (survived[u].second)))->Pt();
		if (sumPt > previousSumPt)
		{
		index1 = (survived[u].first) ;
		index2 = (survived[u].second) ;
		}
	 }
	 
      //PG 2 ELECTRONS
      //PG -----------

      TLorentzVector * primoELE = (TLorentzVector*) (electrons->At (index1)) ;
      TLorentzVector * secondoELE = (TLorentzVector*) (electrons->At (index2)) ; 
      //VT get the first two in pt
      if (primoELE->Pt () < secondoELE->Pt ())
        {
          primoELE = (TLorentzVector*) (electrons->At (index2)) ;
          secondoELE = (TLorentzVector*) (electrons->At (index1)) ; 
        }		
		
      if (primoELE->Pt () < g_hardELEPtMin) continue ; plots.increase (cutId++) ; //PG 4
      if (secondoELE->Pt () < g_softELEPtMin) continue ; plots.increase (cutId++) ; //PG 5
      if (deltaPhi (primoELE->Phi (), secondoELE->Phi ()) < g_ELEDPhiMin) continue ; plots.increase (cutId++) ; //PG 6
      if (deltaPhi (primoELE->Phi (), secondoELE->Phi ()) > g_ELEDPhiMax) continue ; plots.increase (cutId++) ; //PG 7
      TLorentzVector sumELE = *primoELE + *secondoELE ;
      if (sumELE.M () < g_ELEMinvMin) continue ; plots.increase (cutId++) ; //PG 8
      if (sumELE.M () > g_ELEMinvMax) continue ; plots.increase (cutId++) ; //PG 9
//      double deltaR = primoELE->DeltaR (*secondoELE) ;

      //PG MET
      //PG ---

      if (((TLorentzVector*) (MET->At (0)))->Pt () < g_METMin) continue ; plots.increase (cutId++) ; //PG 10
      
      
      //PG 2 TAGS AND ELE
      //PG ------

      TLorentzVector * primoTAG = (TLorentzVector*) (tagJets->At (0)) ;
      TLorentzVector * secondoTAG = (TLorentzVector*) (tagJets->At (1)) ; 
      /*
      double primoDEta = fabs (primoELE->Eta () - primoTAG->Eta ());
      if (primoDEta < fabs (primoELE->Eta () - secondoTAG->Eta ())) primoDEta = fabs (primoELE->Eta () - secondoTAG->Eta ()) ;
      
      double secondoDEta = fabs (secondoELE->Eta () - primoTAG->Eta ());
      if (secondoDEta < fabs (secondoELE->Eta () - secondoTAG->Eta ())) secondoDEta = fabs (secondoELE->Eta () - secondoTAG->Eta ()) ;

      if (secondoDEta < g_eleTagDEta || primoDEta < g_eleTagDEta) continue ; plots.increase (cutId++) ; //PG 11

      */


      //PG 2 TAGS
      //PG ------

      TLorentzVector * primoPtTAG = (TLorentzVector*) (tagJets->At (0)) ;
      TLorentzVector * secondoPtTAG = (TLorentzVector*) (tagJets->At (1)) ; 
      //PG get the first two in pt
      if (primoPtTAG->Pt () < secondoPtTAG->Pt ())
        {
          primoPtTAG = (TLorentzVector*) (tagJets->At (1)) ;
          secondoPtTAG = (TLorentzVector*) (tagJets->At (0)) ; 
        }

      if (primoPtTAG->Pt () < g_hardTAGPtMin) continue ; plots.increase (cutId++) ; //PG 11
      if (secondoPtTAG->Pt () < g_softTAGPtMin) continue ; plots.increase (cutId++) ; //PG 12
      
      if (primoTAG->Eta () * secondoTAG->Eta () > g_TAGDProdEtaMax) continue ; plots.increase (cutId++) ; //PG 13
      if (fabs (primoTAG->Eta () - secondoTAG->Eta ()) < g_TAGDetaMin) continue ; plots.increase (cutId++) ; //PG 14
      TLorentzVector sumTAG = *primoTAG + *secondoTAG ;
      if (sumTAG.M () < g_TAGMinv) continue ; plots.increase (cutId++) ; //PG 15

      //PG 2 JET VETO
      //PG ----------

      //PG loop over other jets
      double etaMean = 0.5*(primoTAG->Eta () + secondoTAG->Eta ()); 
      int ojetsNum = 0 ;
      for (int ojetIt = 0 ; ojetIt < otherJets->GetEntries () ; ++ojetIt)
        {
          if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Pt () < g_ojetPtMin) continue ;
          //if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () < primoTAG->Eta () ||
          //     ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () > secondoTAG->Eta ()) continue ;
	  if ((((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () - etaMean) > g_ojetEtaFromMean) continue;
          ++ojetsNum ;        
        } //PG loop over other jets
      if (ojetsNum > g_ojetsMaxNum) continue ; plots.increase (cutId++) ; //PG 16


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
      
      TVector2 primoELET (primoELE->X (), primoELE->Y ()) ;
      TVector2 secondoELET (secondoELE->X (), secondoELE->Y ()) ;
      TVector2 METT (((TLorentzVector*)(MET->At (0)))->X (), ((TLorentzVector*)(MET->At (0)))->Y ()) ;

      double Sum = METT * primoELET + METT * secondoELET / (1 + primoELET * secondoELET) ;
      double Dif = METT * primoELET - METT * secondoELET / (1 - primoELET * secondoELET) ;
      
      TVector2 METT1 = 0.5 * (Sum + Dif) * primoELET ;
      TVector2 METT2 = 0.5 * (Sum - Dif) * secondoELET ;
      
      double ptNu1 = METT1.Mod () / cos (primoELE->Theta ()) ;
      double ptNu2 = METT2.Mod () / cos (secondoELE->Theta ()) ;

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

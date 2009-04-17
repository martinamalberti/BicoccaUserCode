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
#include "TParticle.h"
#include "TH1.h"
#include "TH2.h"
#include "TText.h"
#include "TLegend.h"


#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>


#include <Math/VectorUtil.h>
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"


//  ------------------------------------------------------------

TFile g_OutputFile("/tmp/amassiro/PLOT/out.root","recreate");
  


 char *g_selections[100]= {
  "Analyzed",
  "2 Jet tags",
  "Pt_min 1^{st} tagging jet",
  "Pt_min 2^{nd} tagging jet",
  "ProdEta tagging jets",
  "#Delta#eta_min tagging jets",
  "MInv_min tagging jets",
  "Jet Veto",
  ">= 1 lepton",
  "2 leptons after Id",
  "lepton channel selection",
  "pt_min 1^{st} lepton",
  "pt_min 2^{nd} lepton",
  "#Delta#phi_min leptons",
  "#Delta#phi_max leptons",
  "MInv_min leptons",
  "MInv_max leptons",
  "MET_min"
 };


//  ------------------------------------------------------------



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
   histoJetName = m_name + "_Jet_histo" ;
   numJetAfterJetTag = new TH1F(histoJetName.Data(),histoJetName.Data(),1000,0,1000);
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
   trend->SetName(m_name);
   
   TH1F * hTrend = new TH1F (m_name,m_name,m_counter.size (),0,m_counter.size ()) ;
   
   for (int i=0 ; i<m_counter.size () ; ++i)
   {
    hTrend->SetBinContent(i+1,m_counter.at (i)) ;
    hTrend->GetXaxis()->SetBinLabel(i+1,g_selections[i]);
    
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

    c1->Print ("/tmp/amassiro/PLOT/" + m_name + "_trend.gif","gif") ;
    
    g_OutputFile.cd(0);
    trend->Write();
    hTrend->Write();
    
    delete c1 ;
    delete trend ;
    delete hTrend ;
    
    //---- print for latex ----
    for (int i=0 ; i<m_counter.size () ; ++i)
    {
     std::cerr << " | " << m_counter.at (i) ;
    }
    std::cerr << std::endl ;
    //---- end print for latex ----       
    
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
  TString histoJetName;

  //!PG number of cuts
  int m_cuts ;

  //!PG counter
  std::vector<int> m_counter ;
  std::vector<double> m_eff ;

  //! AM ---- jet counter ----
  TH1F* numJetAfterJetTag;
    
  
} ;


//  ------------------------------------------------------------


double deltaPhi (double phi1, double phi2) ;
int selector (TChain * tree, histos & plots) ;


//  --------G L O B A L   V A R S ---------------------

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

double g_hardTAGPtMin ;
double g_softTAGPtMin ;
double g_TAGDProdEtaMax ;
double g_TAGDetaMin ;
double g_TAGMinv ;

double g_ojetPtMin ;
double g_ojetEtaFromMean ;
int g_ojetsMaxNum ;

int g_LepLep ;

int g_cutsNum ;

std::string g_prefix ;


//  ========== M A I N    P R O G R A M =========================



int main (int argc, char *argv[])
{ 
 
 //---- loading selections ----

 if (argc < 2) {
  std::cerr << "No input config file." << std:: endl;
  return 1;
 }
  
 std::string fileName (argv[1]) ;
 std::cerr << "fileName = " << fileName << std::endl;
 boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName) ;
 boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
   
 edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("Selections") ;

 g_ID1 = subPSetSelections.getUntrackedParameter<int> ("g_ID1",0) ;
 g_ID2 = subPSetSelections.getUntrackedParameter<int> ("g_ID2",0) ;
 g_ISO1[0] = subPSetSelections.getUntrackedParameter<int> ("g_ISO1_0",0) ;
 g_ISO1[1] = subPSetSelections.getUntrackedParameter<int> ("g_ISO1_1",0) ;
 g_ISO2[0] = subPSetSelections.getUntrackedParameter<int> ("g_ISO2_0",0) ;
 g_ISO2[1] = subPSetSelections.getUntrackedParameter<int> ("g_ISO2_1",0) ;

 g_hardLEPPtMin[0] = subPSetSelections.getUntrackedParameter<double> ("g_hardLEPPtMin_0",0) ;
 g_hardLEPPtMin[1] = subPSetSelections.getUntrackedParameter<double> ("g_hardLEPPtMin_1",0) ;

 g_softLEPPtMin[0] = subPSetSelections.getUntrackedParameter<double> ("g_softLEPPtMin_0",0) ;
 g_softLEPPtMin[1] = subPSetSelections.getUntrackedParameter<double> ("g_softLEPPtMin_1",0) ;
 
 g_LEPDPhiMin = subPSetSelections.getUntrackedParameter<double> ("g_LEPDPhiMin",0) ;
 g_LEPDPhiMax = subPSetSelections.getUntrackedParameter<double> ("g_LEPDPhiMax",0) ;
 g_LEPMinvMin = subPSetSelections.getUntrackedParameter<double> ("g_LEPMinvMin",0) ;
 g_LEPMinvMax = subPSetSelections.getUntrackedParameter<double> ("g_LEPMinvMax",0) ;

 g_METMin = subPSetSelections.getUntrackedParameter<double> ("g_METMin",0) ;
 
 g_hardTAGPtMin = subPSetSelections.getUntrackedParameter<double> ("g_hardTAGPtMin",0) ;
 g_softTAGPtMin = subPSetSelections.getUntrackedParameter<double> ("g_softTAGPtMin",0) ;
 g_TAGDProdEtaMax = subPSetSelections.getUntrackedParameter<double> ("g_TAGDProdEtaMax",0) ;
 g_TAGDetaMin = subPSetSelections.getUntrackedParameter<double> ("g_TAGDetaMin",0) ;

 g_TAGMinv = subPSetSelections.getUntrackedParameter<double> ("g_TAGMinv",0) ;
 g_ojetPtMin = subPSetSelections.getUntrackedParameter<double> ("g_ojetPtMin",0) ;
 g_ojetEtaFromMean = subPSetSelections.getUntrackedParameter<double> ("g_ojetEtaFromMean",0) ;
 g_ojetsMaxNum = subPSetSelections.getUntrackedParameter<int> ("g_ojetsMaxNum",0) ;
 
 g_LepLep = subPSetSelections.getUntrackedParameter<int> ("g_LepLep",0) ;
 
 g_prefix = subPSetSelections.getUntrackedParameter<std::string> ("g_prefix","pluto") ;
  
//  int i = 0;
 g_cutsNum = 23 ;
 g_cutsNum += 1 ; //PG one for the counting before cuts,
 g_cutsNum += 1 ; //PG one for the request of having two tag jets
 g_cutsNum += 1 ; //PG num of electrons
 g_cutsNum -= 3 ; //VT i 4 tagli dell'eleid valgono come 1
 g_cutsNum -= 2 ; //VT u 3 tagli sugli other jet valgono come 1

 g_prefix = subPSetSelections.getUntrackedParameter<std::string> ("g_prefix","pluto") ; 
 
  
 std::cerr << "******************* creating signal ****************" << std::endl;
 
 
 TChain * chain_H160_WW_2l_redigi = new TChain ("ntpla/VBFSimpleTree") ;
 chain_H160_WW_2l_redigi->Add ("/tmp/amassiro/Data/VBF_SimpleTree_H160_WW_2l_redigi.root");
 histos h_H160_WW_2l_redigi (g_prefix + "_H160_WW_2l_redigi", g_cutsNum);
 selector (chain_H160_WW_2l_redigi, h_H160_WW_2l_redigi) ;
h_H160_WW_2l_redigi.printTrend () ;

 
 std::cerr << "******************* creating background ****************" << std::endl;
 
 
 TChain * chain_MCatNLOTTbar = new TChain ("ntpla/VBFSimpleTree") ; 
 chain_MCatNLOTTbar->Add ("/tmp/amassiro/Data/VBF_SimpleTree_MCatNLOTTbar.root");
 histos h_MCatNLOTTbar (g_prefix + "_MCatNLOTTbar", g_cutsNum); 
 selector (chain_MCatNLOTTbar, h_MCatNLOTTbar) ;  
h_MCatNLOTTbar.printTrend () ;

//  TChain * chain_TauolaTTbar_Summer08_IDEAL_V9_v2 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_TauolaTTbar_Summer08_IDEAL_V9_v2->Add ("/tmp/amassiro/Data/VBF_SimpleTree_TauolaTTbar_Summer08_IDEAL_V9_v2.root");
//  histos h_TauolaTTbar_Summer08_IDEAL_V9_v2 (g_prefix + "_TauolaTTbar_Summer08_IDEAL_V9_v2", g_cutsNum); 
//  selector (chain_TauolaTTbar_Summer08_IDEAL_V9_v2, h_TauolaTTbar_Summer08_IDEAL_V9_v2) ;  
// h_TauolaTTbar_Summer08_IDEAL_V9_v2.printTrend () ;

//  TChain * chain_WW_2l_Summer08_IDEAL_V9_v2 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_WW_2l_Summer08_IDEAL_V9_v2->Add ("/tmp/amassiro/Data/VBF_SimpleTree_WW_2l_Summer08_IDEAL_V9_v2.root");
//  histos h_WW_2l_Summer08_IDEAL_V9_v2 (g_prefix + "_WW_2l_Summer08_IDEAL_V9_v2", g_cutsNum); 
//  selector (chain_WW_2l_Summer08_IDEAL_V9_v2, h_WW_2l_Summer08_IDEAL_V9_v2) ;  
// h_WW_2l_Summer08_IDEAL_V9_v2.printTrend () ;
// 
//  TChain * chain_WZ_3l_Summer08_IDEAL_V9_v2 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_WZ_3l_Summer08_IDEAL_V9_v2->Add ("/tmp/amassiro/Data/VBF_SimpleTree_WZ_3l_Summer08_IDEAL_V9_v2.root");
//  histos h_WZ_3l_Summer08_IDEAL_V9_v2 (g_prefix + "_WZ_3l_Summer08_IDEAL_V9_v2", g_cutsNum); 
//  selector (chain_WZ_3l_Summer08_IDEAL_V9_v2, h_WZ_3l_Summer08_IDEAL_V9_v2) ;  
// h_WZ_3l_Summer08_IDEAL_V9_v2.printTrend () ;
// 
//  TChain * chain_WZ_incl_Summer08_IDEAL_V9_v2 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_WZ_incl_Summer08_IDEAL_V9_v2->Add ("/tmp/amassiro/Data/VBF_SimpleTree_WZ_incl_Summer08_IDEAL_V9_v2.root");
//  histos h_WZ_incl_Summer08_IDEAL_V9_v2 (g_prefix + "_WZ_incl_Summer08_IDEAL_V9_v2", g_cutsNum); 
//  selector (chain_WZ_incl_Summer08_IDEAL_V9_v2, h_WZ_incl_Summer08_IDEAL_V9_v2) ;  
// h_WZ_incl_Summer08_IDEAL_V9_v2.printTrend () ;
// 
//  TChain * chain_Wtaunu_Summer08_IDEAL_V9_v1 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_Wtaunu_Summer08_IDEAL_V9_v1->Add ("/tmp/amassiro/Data/VBF_SimpleTree_Wtaunu_Summer08_IDEAL_V9_v1.root");
//  histos h_Wtaunu_Summer08_IDEAL_V9_v1 (g_prefix + "_Wtaunu_Summer08_IDEAL_V9_v1", g_cutsNum); 
//  selector (chain_Wtaunu_Summer08_IDEAL_V9_v1, h_Wtaunu_Summer08_IDEAL_V9_v1) ;  
// h_Wtaunu_Summer08_IDEAL_V9_v1.printTrend () ;
// 
//  TChain * chain_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2.root");
//  histos h_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2 (g_prefix + "_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2", g_cutsNum); 
//  selector (chain_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2, h_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2) ;  
// h_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2.printTrend () ;
// 
//  TChain * chain_ZZ = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZZ->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZZ.root");
//  histos h_ZZ (g_prefix + "_ZZ", g_cutsNum); 
//  selector (chain_ZZ, h_ZZ) ;  
// h_ZZ.printTrend () ;
// 
//  TChain * chain_ZZ_2l2n_Summer08_IDEAL_V9_v2 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZZ_2l2n_Summer08_IDEAL_V9_v2->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZZ_2l2n_Summer08_IDEAL_V9_v2.root");
//  histos h_ZZ_2l2n_Summer08_IDEAL_V9_v2 (g_prefix + "_ZZ_2l2n_Summer08_IDEAL_V9_v2", g_cutsNum); 
//  selector (chain_ZZ_2l2n_Summer08_IDEAL_V9_v2, h_ZZ_2l2n_Summer08_IDEAL_V9_v2) ;  
// h_ZZ_2l2n_Summer08_IDEAL_V9_v2.printTrend () ;
// 
//  TChain * chain_ZZ_4l_Summer08_IDEAL_V9_v2 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZZ_4l_Summer08_IDEAL_V9_v2->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZZ_4l_Summer08_IDEAL_V9_v2.root");
//  histos h_ZZ_4l_Summer08_IDEAL_V9_v2 (g_prefix + "_ZZ_4l_Summer08_IDEAL_V9_v2", g_cutsNum); 
//  selector (chain_ZZ_4l_Summer08_IDEAL_V9_v2, h_ZZ_4l_Summer08_IDEAL_V9_v2) ;  
// h_ZZ_4l_Summer08_IDEAL_V9_v2.printTrend () ;
// 
//  TChain * chain_Z_2l2jets = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_Z_2l2jets->Add ("/tmp/amassiro/Data/VBF_SimpleTree_Z_2l2jets.root");
//  histos h_Z_2l2jets (g_prefix + "_Z_2l2jets", g_cutsNum); 
//  selector (chain_Z_2l2jets, h_Z_2l2jets) ;  
// h_Z_2l2jets.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt0to15 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt0to15->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt0to15.root");
//  histos h_ZeeJet_Pt0to15 (g_prefix + "_ZeeJet_Pt0to15", g_cutsNum); 
//  selector (chain_ZeeJet_Pt0to15, h_ZeeJet_Pt0to15) ;  
// h_ZeeJet_Pt0to15.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt120to170 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt120to170->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt120to170.root");
//  histos h_ZeeJet_Pt120to170 (g_prefix + "_ZeeJet_Pt120to170", g_cutsNum); 
//  selector (chain_ZeeJet_Pt120to170, h_ZeeJet_Pt120to170) ;  
// h_ZeeJet_Pt120to170.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt170to230 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt170to230->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt170to230.root");
//  histos h_ZeeJet_Pt170to230 (g_prefix + "_ZeeJet_Pt170to230", g_cutsNum); 
//  selector (chain_ZeeJet_Pt170to230, h_ZeeJet_Pt170to230) ;  
// h_ZeeJet_Pt170to230.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt20to30 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt20to30->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt20to30.root");
//  histos h_ZeeJet_Pt20to30 (g_prefix + "_ZeeJet_Pt20to30", g_cutsNum); 
//  selector (chain_ZeeJet_Pt20to30, h_ZeeJet_Pt20to30) ;  
// h_ZeeJet_Pt20to30.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt230to300 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt230to300->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt230to300.root");
//  histos h_ZeeJet_Pt230to300 (g_prefix + "_ZeeJet_Pt230to300", g_cutsNum); 
//  selector (chain_ZeeJet_Pt230to300, h_ZeeJet_Pt230to300) ;  
// h_ZeeJet_Pt230to300.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt300toInf = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt300toInf->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt300toInf.root");
//  histos h_ZeeJet_Pt300toInf (g_prefix + "_ZeeJet_Pt300toInf", g_cutsNum); 
//  selector (chain_ZeeJet_Pt300toInf, h_ZeeJet_Pt300toInf) ;  
// h_ZeeJet_Pt300toInf.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt30to50 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt30to50->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt30to50.root");
//  histos h_ZeeJet_Pt30to50 (g_prefix + "_ZeeJet_Pt30to50", g_cutsNum); 
//  selector (chain_ZeeJet_Pt30to50, h_ZeeJet_Pt30to50) ;  
// h_ZeeJet_Pt30to50.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt50to80 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt50to80->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt50to80.root");
//  histos h_ZeeJet_Pt50to80 (g_prefix + "_ZeeJet_Pt50to80", g_cutsNum); 
//  selector (chain_ZeeJet_Pt50to80, h_ZeeJet_Pt50to80) ;  
// h_ZeeJet_Pt50to80.printTrend () ;
// 
//  TChain * chain_ZeeJet_Pt80to120 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZeeJet_Pt80to120->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZeeJet_Pt80to120.root");
//  histos h_ZeeJet_Pt80to120 (g_prefix + "_ZeeJet_Pt80to120", g_cutsNum); 
//  selector (chain_ZeeJet_Pt80to120, h_ZeeJet_Pt80to120) ;  
// h_ZeeJet_Pt80to120.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt0to15 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt0to15->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt0to15.root");
//  histos h_ZmumuJet_Pt0to15 (g_prefix + "_ZmumuJet_Pt0to15", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt0to15, h_ZmumuJet_Pt0to15) ;  
// h_ZmumuJet_Pt0to15.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt120to170 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt120to170->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt120to170.root");
//  histos h_ZmumuJet_Pt120to170 (g_prefix + "_ZmumuJet_Pt120to170", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt120to170, h_ZmumuJet_Pt120to170) ;  
// h_ZmumuJet_Pt120to170.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt15to20 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt15to20->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt15to20.root");
//  histos h_ZmumuJet_Pt15to20 (g_prefix + "_ZmumuJet_Pt15to20", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt15to20, h_ZmumuJet_Pt15to20) ;  
// h_ZmumuJet_Pt15to20.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt170to230 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt170to230->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt170to230.root");
//  histos h_ZmumuJet_Pt170to230 (g_prefix + "_ZmumuJet_Pt170to230", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt170to230, h_ZmumuJet_Pt170to230) ;  
// h_ZmumuJet_Pt170to230.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt20to30 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt20to30->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt20to30.root");
//  histos h_ZmumuJet_Pt20to30 (g_prefix + "_ZmumuJet_Pt20to30", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt20to30, h_ZmumuJet_Pt20to30) ;  
// h_ZmumuJet_Pt20to30.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt230to300 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt230to300->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt230to300.root");
//  histos h_ZmumuJet_Pt230to300 (g_prefix + "_ZmumuJet_Pt230to300", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt230to300, h_ZmumuJet_Pt230to300) ;  
// h_ZmumuJet_Pt230to300.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt300toInf = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt300toInf->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt300toInf.root");
//  histos h_ZmumuJet_Pt300toInf (g_prefix + "_ZmumuJet_Pt300toInf", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt300toInf, h_ZmumuJet_Pt300toInf) ;  
// h_ZmumuJet_Pt300toInf.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt30to50 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt30to50->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt30to50.root");
//  histos h_ZmumuJet_Pt30to50 (g_prefix + "_ZmumuJet_Pt30to50", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt30to50, h_ZmumuJet_Pt30to50) ;  
// h_ZmumuJet_Pt30to50.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt50to80 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt50to80->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt50to80.root");
//  histos h_ZmumuJet_Pt50to80 (g_prefix + "_ZmumuJet_Pt50to80", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt50to80, h_ZmumuJet_Pt50to80) ;  
// h_ZmumuJet_Pt50to80.printTrend () ;
// 
//  TChain * chain_ZmumuJet_Pt80to120 = new TChain ("ntpla/VBFSimpleTree") ; 
//  chain_ZmumuJet_Pt80to120->Add ("/tmp/amassiro/Data/VBF_SimpleTree_ZmumuJet_Pt80to120.root");
//  histos h_ZmumuJet_Pt80to120 (g_prefix + "_ZmumuJet_Pt80to120", g_cutsNum); 
//  selector (chain_ZmumuJet_Pt80to120, h_ZmumuJet_Pt80to120) ;  
// h_ZmumuJet_Pt80to120.printTrend () ;
   
 std::cout << "******************* Computing the significance" << std::endl;

 std::vector<double> signalEvents ;
 std::vector<double> bkgEvents ;

 std::vector<double> signalEff ;
 std::vector<double> bkgEff ;
 std::vector<double> m_significance ;
  
 std::stringstream prefisso ; 
 double Lumi = 10000 ; //pb-1
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
 prefisso << " " << g_hardTAGPtMin ;
 prefisso << " " << g_softTAGPtMin ;
 prefisso << " " << g_TAGDProdEtaMax ;
 prefisso << " " << g_TAGDetaMin ;
 prefisso << " " << g_TAGMinv ;
 prefisso << " " << g_ojetPtMin ;
 prefisso << " " << g_ojetEtaFromMean ;
 prefisso << " " << g_ojetsMaxNum ;
 prefisso << " | " ;  

 
 std::cerr << "[ g_prefix Lumi ] " ;
 std::cerr << " g_ID1 " ; 
 std::cerr << " g_ID2 " ; 
 std::cerr << " g_ISO1[0] " ; 
 std::cerr << " g_ISO1[1] " ; 
 std::cerr << " g_ISO2[0] " ; 
 std::cerr << " g_ISO2[1] " ; 
 std::cerr << "   g_hardLEPPtMin[0] "; 
 std::cerr << "   g_hardLEPPtMin[1] "; 
 std::cerr << "   g_softLEPPtMin[0] "; 
 std::cerr << "   g_softLEPPtMin[1] "; 
 std::cerr << "   g_LEPDPhiMin   ";
 std::cerr << "   g_LEPDPhiMax   ";
 std::cerr << "   g_LEPMinvMin   ";
 std::cerr << "   g_LEPMinvMax   ";
 std::cerr << "   g_METMin       ";
 std::cerr << "   g_hardTAGPtMin ";
 std::cerr << "   g_softTAGPtMin ";
 std::cerr << "   g_TAGDProdEtaMax ";
 std::cerr << "   g_TAGDetaMin ";
 std::cerr << "   g_TAGMinv ";
 std::cerr << "   g_ojetPtMin ";
 std::cerr << "   g_ojetEtaFromMean ";
 std::cerr << "   g_ojetsMaxNum ";
 std::cerr << " | " << std::endl;  

 
 
 for (int i=0 ; i < h_H160_WW_2l_redigi.m_eff.size () ; i++)
 {
  double weightedSumBkg = 
     352 * 0.884236 * (h_MCatNLOTTbar.m_eff.at (i)) // +
//      241.7  * 0.880993 * (h_TauolaTTbar_Summer08_IDEAL_V9_v2.m_eff.at (i)) +
//      0.00000000000 * 0.849105 * (h_WW_2l_Summer08_IDEAL_V9_v2.m_eff.at (i)) +
//      0.00000000000 * 0.921532 * (h_WZ_3l_Summer08_IDEAL_V9_v2.m_eff.at (i)) +
//      0.00000000000 * 0.581132 * (h_WZ_incl_Summer08_IDEAL_V9_v2.m_eff.at (i)) +
//      11840 * 0.276351 * (h_Wtaunu_Summer08_IDEAL_V9_v1.m_eff.at (i)) +
//      0.00000000000 * 0.717606 * (h_ZJets_madgraph_Fall08_IDEAL_V9_reco_v2.m_eff.at (i)) +
//      7.1 * 0.546053 * (h_ZZ.m_eff.at (i)) +
//      0.00000000000 * 0.782859 * (h_ZZ_2l2n_Summer08_IDEAL_V9_v2.m_eff.at (i)) +
//      0.00000000000 * 0.917859 * (h_ZZ_4l_Summer08_IDEAL_V9_v2.m_eff.at (i)) +
//      0.00000000000 * 0.922144 * (h_Z_2l2jets.m_eff.at (i)) +
//      6430  * 0.563105 * (h_ZeeJet_Pt0to15.m_eff.at (i)) +
//      5.4  * 0.960106  * (h_ZeeJet_Pt120to170.m_eff.at (i)) +
//      1.55  * 0.978073  * (h_ZeeJet_Pt170to230.m_eff.at (i)) +
//      211 * 0.782322 * (h_ZeeJet_Pt20to30.m_eff.at (i)) +
//      0.45 * 0.987873 * (h_ZeeJet_Pt230to300.m_eff.at (i)) +
//      0.2 * 0.993899 * (h_ZeeJet_Pt300toInf.m_eff.at (i)) +
//      142 * 0.842282 * (h_ZeeJet_Pt30to50.m_eff.at (i)) +
//      56.8 * 0.897029 * (h_ZeeJet_Pt50to80.m_eff.at (i)) +
//      18.8 * 0.932544 * (h_ZeeJet_Pt80to120.m_eff.at (i)) +
//      6430 * 0.599027 * (h_ZmumuJet_Pt0to15.m_eff.at (i)) +
//      5.4 * 0.963797 * (h_ZmumuJet_Pt120to170.m_eff.at (i)) +
//      230 * 0.754308 * (h_ZmumuJet_Pt15to20.m_eff.at (i)) +
//      1.55 * 0.981191 * (h_ZmumuJet_Pt170to230.m_eff.at (i)) +
//      211 * 0.799673 * (h_ZmumuJet_Pt20to30.m_eff.at (i)) +
//      0.45 * 0.99151 * (h_ZmumuJet_Pt230to300.m_eff.at (i)) +
//      0.2 * 0.997308 * (h_ZmumuJet_Pt300toInf.m_eff.at (i)) +
//      142 * 0.853935 * (h_ZmumuJet_Pt30to50.m_eff.at (i)) +
//      56.8 * 0.903584 * (h_ZmumuJet_Pt50to80.m_eff.at (i)) +
//      18.8 * 0.938867 * (h_ZmumuJet_Pt80to120.m_eff.at (i))
     ;

  signalEvents.push_back (Lumi * 0.119 * 0.70 * 0.897765 * h_H160_WW_2l_redigi.m_eff.at (i)) ;
  signalEff.push_back (0.897765 * h_H160_WW_2l_redigi.m_eff.at (i)) ;
  //---- 0.119 from Valentina table 6.1
  //---- 0.70 from http://ceballos.web.cern.ch/ceballos/hwwlnln/cross-sections_cmssw21x.txt 
   
  weightedSumBkg *= Lumi;
  bkgEvents.push_back (weightedSumBkg) ;  
  
  double normalization_factor = 
     352 * 0.884236  // +
//      241.7  * 0.880993  +
//      0.00000000000 * 0.849105  +
//      0.00000000000 * 0.921532  +
//      0.00000000000 * 0.581132  +
//      11840 * 0.276351  +
//      0.00000000000 * 0.717606  +
//      7.1 * 0.546053  +
//      0.00000000000 * 0.782859  +
//      0.00000000000 * 0.917859  +
//      0.00000000000 * 0.922144  +
//      6430  * 0.563105  +
//      5.4  * 0.960106   +
//      1.55  * 0.978073   +
//      211 * 0.782322  +
//      0.45 * 0.987873  +
//      0.2 * 0.993899  +
//      142 * 0.842282  +
//      56.8 * 0.897029  +
//      18.8 * 0.932544  +
//      6430 * 0.599027  +
//      5.4 * 0.963797  +
//      230 * 0.754308  +
//      1.55 * 0.981191  +
//      211 * 0.799673  +
//      0.45 * 0.99151  +
//      0.2 * 0.997308  +
//      142 * 0.853935  +
//      56.8 * 0.903584  +
//      18.8 * 0.938867 
     ;
   
  
  bkgEff.push_back (weightedSumBkg / normalization_factor) ;  
  m_significance.push_back (Lumi * 0.119 * 0.70 * 0.897765 * h_H160_WW_2l_redigi.m_eff.at (i) / sqrt (weightedSumBkg)) ;
 }
 std::cout << std::endl ;
  
 std::cout << prefisso.str () << "  eff S  "  ;
 for (int i=0 ; i < signalEff.size () ; ++i) std::cout << " " << signalEff.at (i) ;
 std::cout << std::endl ;
 std::cout << prefisso.str () << "  eff B  "  ;
 for (int i=0 ; i < bkgEff.size () ; ++i) std::cout << " " << bkgEff.at (i) ;
 std::cout << std::endl ;
 std::cout << prefisso.str () << " Sosqrt(B) = significance = "  ;
 for (int i=0 ; i < m_significance.size () ; ++i) std::cout << " " << m_significance.at (i) ;
 std::cout << std::endl ;

 std::cout << prefisso.str () << "  S  "  ;
 for (int i=0 ; i < signalEvents.size () ; ++i) std::cout << " " << signalEvents.at (i) ;
 std::cout << std::endl ;
 std::cout << prefisso.str () << "  B  "  ;
 for (int i=0 ; i < bkgEvents.size () ; ++i) std::cout << " " << bkgEvents.at (i) ;
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
//  TClonesArray * tagJets = new TClonesArray ("TLorentzVector") ; 
//  tree->SetBranchAddress ("tagJets", &tagJets) ;
 TClonesArray * otherJets_temp = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("otherJets", &otherJets_temp) ;
 
 TClonesArray * electrons = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("electrons", &electrons) ;
 TClonesArray * muons = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("muons", &muons) ;
 TClonesArray * MET = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("MET", &MET) ;
 TClonesArray * tracks = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("tracks", &tracks) ;
 
 TClonesArray * tagJets = new TClonesArray ("TLorentzVector") ; 
 TClonesArray * otherJets = new TClonesArray ("TLorentzVector") ;
 
  
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

  //PG loop over the events
  //  int nentries = 100 ;
 for (int evt = 0 ; evt < nentries ; ++evt)
 {
  tree->GetEntry (evt) ;
  
  tagJets -> Clear () ;  
  otherJets -> Clear () ;  
  
   //---- find Tagging Jets ----
 
 
  double m_jetPtMin = 15.;
  double m_jetEtaMax = 5.;
  double m_jetDEtaMin = -1;
  double m_jetMjjMin = -1;
 
  double ptMax1 = 0;
  double ptMax2 = 0;

  int Jet1 = -1;
  int Jet2 = -1;
  
  std::pair<int,int> tagJetCands;
 

  
  for (int l=0; l<otherJets_temp->GetEntries (); l++ ){
   TLorentzVector* jet_temp = (TLorentzVector*) otherJets_temp->At(l);
   //---- pt min threshold ----
   if (jet_temp->Pt()<m_jetPtMin) continue;
       //---- Eta max threshold ----
   if (jet_temp->Eta()>m_jetEtaMax) continue;
       //---- if it's not the first jet, I may check if DEta and Mjj selections are fullfilled
       //---- Check between current (1) and the new jet
   if (Jet1!=-1){//---- if it's not the first time
    TLorentzVector* myJet1 = (TLorentzVector*) otherJets_temp->At(Jet1);
    float etaA = myJet1->Eta();
    float etaB= jet_temp->Eta();
    float DEta = fabs(etaA - etaB);
    float Mjj = (*myJet1 + *jet_temp).Mag();
    if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin) continue;
   }
       //---- first threshold: (2) ----
   if (jet_temp->Pt() > ptMax2)
   {
    //---- second threshold: (1) ----
    if (jet_temp->Pt() > ptMax1){
     //---- if both (1) and (2) threshold succeded than (1) becomes (2)
     Jet2 = Jet1;
     ptMax2 = ptMax1;
     //----  and the new one is (1)
     Jet1 = l;
     TLorentzVector* myJet1 = (TLorentzVector*) otherJets_temp->At(Jet1);
     ptMax1 = myJet1->Pt();
    }
    else {
     Jet2 = l;
     TLorentzVector* myJet2 = (TLorentzVector*) otherJets_temp->At(Jet2);
     ptMax2 = myJet2->Pt();    
    }
   }
  }
  
  tagJetCands.first = Jet1 ;
  tagJetCands.second = Jet2 ;
  

  int counter = 0;
  if (tagJetCands.first != -1) {
   TClonesArray &jetOther = *tagJets;
   TLorentzVector* myJet = (TLorentzVector*) otherJets_temp->At(tagJetCands.first);
   new (jetOther[counter]) TLorentzVector (*myJet);
   counter++;
//    std::cerr << "*** found one jets tagging ***" << std::endl;
  }

  
  if (tagJetCands.second != -1) {
   TClonesArray &jetOther = *tagJets;
   TLorentzVector* myJet = (TLorentzVector*) otherJets_temp->At(tagJetCands.second);
   new (jetOther[counter]) TLorentzVector (*myJet);
   counter++;
//    std::cerr << "*** found two jets tagging ***" << std::endl;
  }
//   else std::cerr << "*** NOTNOTNOTNOTNOTNOT found two jets tagging ***" << std::endl;
  
  counter = 0;
  for (int l=0; l<otherJets_temp->GetEntries (); l++ ){
   if (tagJetCands.first == l || tagJetCands.second == l) continue;
   TClonesArray &jetOther = *otherJets;
   TLorentzVector* myJet = (TLorentzVector*) otherJets_temp->At(l);
   new (jetOther[counter]) TLorentzVector (*myJet);
   counter++;
  }
  
//   std::cerr << "*********** quanti jet ho = " << otherJets_temp->GetEntries () << std::endl;
//   std::cerr << "*********** quanti teg jet ho trovato = " << tagJets->GetEntries () << std::endl;
    
  //---- end find Tagging Jets ----
  
  
  
  
  int cutId = 0 ;

  plots.increase (cutId++) ; //AM 0 -> total number of events

  if (tagJets->GetEntries () != 2) continue ; plots.increase (cutId++) ; //AM 1 ctrl numbering jets -> number of jets with common preselections (pt_min and eta_max) may be < 2
  
  
  
      //PG 2 TAGS
      //PG ------

  TLorentzVector * primoTAG = (TLorentzVector*) (tagJets->At (0)) ; //--- primoTAG.Pt > secondoTAG.Pt !!!
  TLorentzVector * secondoTAG = (TLorentzVector*) (tagJets->At (1)) ; 
  
  double eta_1 = primoTAG->Eta();
  double eta_2 = secondoTAG->Eta();
  
  double eta_max = std::max(eta_1,eta_2);
  double eta_min = std::min(eta_1,eta_2);
  
//   std::cerr << "ptMax1 = " << ptMax1 << " === " << primoTAG->Pt () << " = primoTAG->Pt ()" << std::endl;
//   std::cerr << "ptMax2 = " << ptMax2 << " === " << secondoTAG->Pt () << " = secondoTAG->Pt ()" << std::endl;
//   std::cerr << std::endl;  
  
      //PG get the first two in pt
  if (primoTAG->Pt () < secondoTAG->Pt ())
  {
   primoTAG = (TLorentzVector*) (tagJets->At (1)) ;
   secondoTAG = (TLorentzVector*) (tagJets->At (0)) ; 
  }

  if (primoTAG->Pt () < g_hardTAGPtMin) continue ; plots.increase (cutId++) ;
  //---- AM 12 pt min highest pt jet
  
  if (secondoTAG->Pt () < g_softTAGPtMin) continue ; plots.increase (cutId++) ;
  //---- AM 13 pt min lowest pt jet
      
  if (primoTAG->Eta () * secondoTAG->Eta () > g_TAGDProdEtaMax) continue ; plots.increase (cutId++) ;
  //---- AM 14 eta_1 * eta_2 of the jets
  
  if (fabs (primoTAG->Eta () - secondoTAG->Eta ()) < g_TAGDetaMin) continue ; plots.increase (cutId++) ;
  //---- AM 15 DEta_min between jets
  
  TLorentzVector sumTAG = *primoTAG + *secondoTAG ;
  if (sumTAG.M () < g_TAGMinv) continue ; plots.increase (cutId++) ;
  //---- AM 16 MInv_min jets
  

      //PG JET VETO
      //PG --------

      //PG loop over other jets
  double etaMean = 0.5*(primoTAG->Eta () + secondoTAG->Eta ()); 
  int numJetOthers = 0;
  int ojetsNum = 0 ;
  for (int ojetIt = 0 ; ojetIt < otherJets->GetEntries () ; ++ojetIt)
  {
   if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Pt () < g_ojetPtMin) continue ;
   
   numJetOthers++;
   
   if (g_ojetEtaFromMean == -1) { //---- jet veto between the two tagging jets
    if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () < eta_min ||
           ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () > eta_max) continue ;
   } //---- variable jet veto
   else if ((((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () - etaMean) > g_ojetEtaFromMean) continue;
   
   ++ojetsNum ;        
  } //PG end loop over other jets
  
  plots.numJetAfterJetTag->Fill(numJetOthers);
  
  if (ojetsNum > g_ojetsMaxNum) continue ; plots.increase (cutId++) ;
  //---- AM 17 Jet Veto


      /*
  double primoDR = primoELE->DeltaR (*primoTAG) ;
  if (primoDR < primoELE->DeltaR (*secondoTAG)) primoDR = fabs (primoELE->Eta () - secondoTAG->Eta ()) ;
      
  double secondoDR = secondoELE->DeltaR (*primoTAG) ;
  if (secondoDR < secondoELE->DeltaR (*secondoTAG)) secondoDR = fabs (secondoELE->Eta () - secondoTAG->Eta ()) ;

  if (secondoDR < g_eleTagDR || primoDR < g_eleTagDR) continue ; plots.increase (cutId++) ; //PG 12
      */
  
  
  
  
  
  
  
  
  
  
  if (electrons->GetEntries () < 1 ||
      muons->GetEntries () < 1) continue ; plots.increase (cutId++) ; //AM 2 ctrl numbering leptons -> at least 1 lepton
            
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
  
  Then check for channel analysis according to "g_LepLep"
     0 == ee
     1 == mumu
     2 == emu
     3 == mue
  pt ordered
  
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
      secondoLEP.m_flav == -1) continue ; plots.increase (cutId++) ;
  //---- AM 3 --- 2 leptons after Id

        
  //---- AM 4 check for the two most transverse-energetic leptons have the right flavours
  
  if (g_LepLep == 0) {
   if (primoLEP.m_flav != 0 || secondoLEP.m_flav != 0) continue ; plots.increase (cutId++) ;
   }
   else if (g_LepLep == 1) {
    if (primoLEP.m_flav != 1 || secondoLEP.m_flav != 1) continue ; plots.increase (cutId++) ;
   } 
   else if (g_LepLep == 2) {
    if (primoLEP.m_flav != 0 || secondoLEP.m_flav != 1) continue ; plots.increase (cutId++) ;
   }
   else if (g_LepLep == 3) {
    if (primoLEP.m_flav != 1 || secondoLEP.m_flav != 0) continue ; plots.increase (cutId++) ;
   }

      
      
   if (primoLEP.m_kine->Pt () < g_hardLEPPtMin[primoLEP.m_flav]) continue ; plots.increase (cutId++) ;
   //---- AM 5 pt_min of the most energetic lepton
  
   if (secondoLEP.m_kine->Pt () < g_softLEPPtMin[secondoLEP.m_flav]) continue ; plots.increase (cutId++) ;
   //---- AM 6 pt_min of the least energetic lepton
   
  if (deltaPhi (primoLEP.m_kine->Phi (), secondoLEP.m_kine->Phi ()) < g_LEPDPhiMin) continue ; plots.increase (cutId++) ;
  //---- AM 7 Delta_phi_min between leptons
  
  if (deltaPhi (primoLEP.m_kine->Phi (), secondoLEP.m_kine->Phi ()) > g_LEPDPhiMax) continue ; plots.increase (cutId++) ;
  //---- AM 8 Delta_phi_max between leptons
  
  TLorentzVector sumLEP = *(primoLEP.m_kine) + *(secondoLEP.m_kine) ;
  if (sumLEP.M () < g_LEPMinvMin) continue ; plots.increase (cutId++) ;
  //---- AM 9 MInv_min of leptons
  
  if (sumLEP.M () > g_LEPMinvMax) continue ; plots.increase (cutId++) ;
  //---- AM 10 MInv_max of leptons
  

  
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
  if (met->Pt () < g_METMin) continue ; plots.increase (cutId++) ;
  //---- AM 11 Met_min ----------------> Met correction ?
  
//      if (((TLorentzVector*) (MET->At (0)))->Pt () < g_METMin) continue ; plots.increase (cutId++) ; //PG 10
      
  
  
  
  

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

 plots.numJetAfterJetTag->Write();
 plots.normalize () ;

 delete otherJets_temp ;
 delete tagJets  ;  
 delete otherJets  ;
 delete electrons  ;
 delete muons  ;    
 delete MET  ;      
 delete tracks  ;   

 return 0;
  
}

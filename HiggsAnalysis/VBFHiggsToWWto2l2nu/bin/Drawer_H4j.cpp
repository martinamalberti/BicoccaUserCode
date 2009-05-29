
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
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

//  ------------------------------------------------------------

// TFile g_OutputFile("/tmp/amassiro/PLOT/prova.root","recreate");
TFile *g_OutputFile;

//  ------------------------------------------------------------


struct myJet 
{
 myJet () : m_flav (-1) , m_index (-1) {} ;
    myJet (TLorentzVector * kine, int flav, int index) :
      m_kine (kine),
    m_flav (flav),
    m_index (index)
    {}

    TLorentzVector * m_kine ;
    int m_flav ;
    int m_index ;
} ;           


//  ------------------------------------------------------------


struct lessThan : public std::binary_function<myJet, myJet, bool>
{
 bool operator ()(const myJet & jet1, const myJet & jet2)
 {
  return jet1.m_kine->Pt () < jet2.m_kine->Pt () ;
 }
} ;

//  ------------------------------------------------------------


struct DeltaRSorting : public std::binary_function<
  std::pair<TLorentzVector*,TLorentzVector*>,
  std::pair<TLorentzVector*,TLorentzVector*>,
  bool
    >
{
 bool operator ()(
   const std::pair<TLorentzVector*,TLorentzVector*> & couple1,
 const std::pair<TLorentzVector*,TLorentzVector*> & couple2
                 )
 {
   
  return ROOT::Math::VectorUtil::DeltaR(couple1.first->BoostVector(),couple1.second->BoostVector()) < ROOT::Math::VectorUtil::DeltaR(couple2.first->BoostVector(),couple2.second->BoostVector());
 }
} ;


//  ------------------------------------------------------------

   
  struct PtSorting : public std::binary_function<
    std::pair<TLorentzVector*,TLorentzVector*>,
  std::pair<TLorentzVector*,TLorentzVector*>,
  bool
    >
{
 bool operator ()(
   const std::pair<TLorentzVector*,TLorentzVector*> & couple1,
 const std::pair<TLorentzVector*,TLorentzVector*> & couple2
                 )
 {
  return couple1.second->Pt() < couple2.second->Pt();
 }
} ;



//  ------------------------------------------------------------

struct DeltaRSame : public
  std::binary_function<
  std::pair<TLorentzVector*,TLorentzVector*>,
     std::pair<TLorentzVector*,TLorentzVector*>,
     bool
       >
{
 bool operator() (
   const std::pair<TLorentzVector*,TLorentzVector*> & couple1,
 const std::pair<TLorentzVector*,TLorentzVector*> & couple2
                 )
 {
  return ((couple1.second == couple2.second));
 }
};

//  ------------------------------------------------------------


struct histos
{
  histos (TString name, int numberOfCuts) : 
    m_name (name) , 
  m_cuts (numberOfCuts) ,
  m_counter (numberOfCuts, 0) ,
  m_eff (numberOfCuts, 0.)
  {
   TString m_tree_selections_Name = m_name + "_m_tree_selections" ;
   
   m_tree_selections = new TTree(m_tree_selections_Name,m_tree_selections_Name);
     
   m_tree_selections->Branch("v_eta_gj_1",&v_eta_gj_1,"v_eta_gj_1/D");
   m_tree_selections->Branch("v_eta_gj_2",&v_eta_gj_2,"v_eta_gj_2/D");

   m_tree_selections->Branch("v_eta_rj_1",&v_eta_rj_1,"v_eta_rj_1/D");
   m_tree_selections->Branch("v_eta_rj_2",&v_eta_rj_2,"v_eta_rj_2/D");
   m_tree_selections->Branch("v_eta_rj_3",&v_eta_rj_3,"v_eta_rj_3/D");
   m_tree_selections->Branch("v_eta_rj_4",&v_eta_rj_4,"v_eta_rj_4/D");
   m_tree_selections->Branch("v_eta_rj_5",&v_eta_rj_5,"v_eta_rj_5/D");
   m_tree_selections->Branch("v_eta_rj_6",&v_eta_rj_6,"v_eta_rj_6/D");
   m_tree_selections->Branch("v_eta_rj_7",&v_eta_rj_7,"v_eta_rj_7/D");
   m_tree_selections->Branch("v_eta_rj_8",&v_eta_rj_8,"v_eta_rj_8/D");
   m_tree_selections->Branch("v_eta_rj_9",&v_eta_rj_9,"v_eta_rj_9/D");
   m_tree_selections->Branch("v_eta_rj_10",&v_eta_rj_10,"v_eta_rj_10/D");
   m_tree_selections->Branch("v_eta_rj_11",&v_eta_rj_11,"v_eta_rj_11/D");
   m_tree_selections->Branch("v_eta_rj_12",&v_eta_rj_12,"v_eta_rj_12/D");
   m_tree_selections->Branch("v_eta_rj_13",&v_eta_rj_13,"v_eta_rj_13/D");
   m_tree_selections->Branch("v_eta_rj_14",&v_eta_rj_14,"v_eta_rj_14/D");
   m_tree_selections->Branch("v_eta_rj_15",&v_eta_rj_15,"v_eta_rj_15/D");
   
   m_tree_selections->Branch("v_phi_rj_1",&v_phi_rj_1,"v_phi_rj_1/D");
   m_tree_selections->Branch("v_phi_rj_2",&v_phi_rj_2,"v_phi_rj_2/D");
   m_tree_selections->Branch("v_phi_rj_3",&v_phi_rj_3,"v_phi_rj_3/D");
   m_tree_selections->Branch("v_phi_rj_4",&v_phi_rj_4,"v_phi_rj_4/D");
   m_tree_selections->Branch("v_phi_rj_5",&v_phi_rj_5,"v_phi_rj_5/D");
   m_tree_selections->Branch("v_phi_rj_6",&v_phi_rj_6,"v_phi_rj_6/D");
   m_tree_selections->Branch("v_phi_rj_7",&v_phi_rj_7,"v_phi_rj_7/D");
   m_tree_selections->Branch("v_phi_rj_8",&v_phi_rj_8,"v_phi_rj_8/D");
   m_tree_selections->Branch("v_phi_rj_9",&v_phi_rj_9,"v_phi_rj_9/D");
   m_tree_selections->Branch("v_phi_rj_10",&v_phi_rj_10,"v_phi_rj_10/D");
   m_tree_selections->Branch("v_phi_rj_11",&v_phi_rj_11,"v_phi_rj_11/D");
   m_tree_selections->Branch("v_phi_rj_12",&v_phi_rj_12,"v_phi_rj_12/D");
   m_tree_selections->Branch("v_phi_rj_13",&v_phi_rj_13,"v_phi_rj_13/D");
   m_tree_selections->Branch("v_phi_rj_14",&v_phi_rj_14,"v_phi_rj_14/D");
   m_tree_selections->Branch("v_phi_rj_15",&v_phi_rj_15,"v_phi_rj_15/D");

   m_tree_selections->Branch("v_DR_rj_1",&v_DR_rj_1,"v_DR_rj_1/D");
   m_tree_selections->Branch("v_DR_rj_2",&v_DR_rj_2,"v_DR_rj_2/D");
   m_tree_selections->Branch("v_DR_rj_3",&v_DR_rj_3,"v_DR_rj_3/D");
   m_tree_selections->Branch("v_DR_rj_4",&v_DR_rj_4,"v_DR_rj_4/D");
   m_tree_selections->Branch("v_DR_rj_5",&v_DR_rj_5,"v_DR_rj_5/D");
   m_tree_selections->Branch("v_DR_rj_6",&v_DR_rj_6,"v_DR_rj_6/D");

   m_tree_selections->Branch("v_eta_q_1",&v_eta_q_1,"v_eta_q_1/D");
   m_tree_selections->Branch("v_eta_q_2",&v_eta_q_2,"v_eta_q_2/D");
   m_tree_selections->Branch("v_eta_V_1_q_1",&v_eta_V_1_q_1,"v_eta_V_1_q_1/D");
   m_tree_selections->Branch("v_eta_V_1_q_2",&v_eta_V_1_q_2,"v_eta_V_1_q_2/D");
   m_tree_selections->Branch("v_eta_V_2_q_1",&v_eta_V_2_q_1,"v_eta_V_2_q_1/D");
   m_tree_selections->Branch("v_eta_V_2_q_2",&v_eta_V_2_q_2,"v_eta_V_2_q_2/D");

   m_tree_selections->Branch("v_phi_q_1",&v_phi_q_1,"v_phi_q_1/D");
   m_tree_selections->Branch("v_phi_q_2",&v_phi_q_2,"v_phi_q_2/D");
   m_tree_selections->Branch("v_phi_V_1_q_1",&v_phi_V_1_q_1,"v_phi_V_1_q_1/D");
   m_tree_selections->Branch("v_phi_V_1_q_2",&v_phi_V_1_q_2,"v_phi_V_1_q_2/D");
   m_tree_selections->Branch("v_phi_V_2_q_1",&v_phi_V_2_q_1,"v_phi_V_2_q_1/D");
   m_tree_selections->Branch("v_phi_V_2_q_2",&v_phi_V_2_q_2,"v_phi_V_2_q_2/D");
        
  }

  void increase (int cutId) { ++m_counter.at (cutId) ; } ;
    
  void printTrend ()
  {   
  }

  
  
  
 //!PG histos name
  TString m_name ;
 
   //!PG number of cuts
  int m_cuts ;

  //!PG counter
  std::vector<int> m_counter ;
  std::vector<double> m_eff ;

  
 
  //! AM ---- TTree ----
  TTree* m_tree_selections;
  double v_eta_q_1 ;
  double v_eta_q_2 ; 
  double v_eta_V_1_q_1;
  double v_eta_V_1_q_2;
  double v_eta_V_2_q_1;
  double v_eta_V_2_q_2;

  double v_phi_q_1 ;
  double v_phi_q_2 ; 
  double v_phi_V_1_q_1;
  double v_phi_V_1_q_2;
  double v_phi_V_2_q_1;
  double v_phi_V_2_q_2;

  
      
  double v_eta_gj_1 ;
  double v_eta_gj_2 ;

  double v_eta_rj_1 ;
  double v_eta_rj_2 ;
  double v_eta_rj_3 ;
  double v_eta_rj_4 ;
  double v_eta_rj_5 ;
  double v_eta_rj_6 ;
  double v_eta_rj_7 ;
  double v_eta_rj_8 ;
  double v_eta_rj_9 ;
  double v_eta_rj_10 ;
  double v_eta_rj_11 ;
  double v_eta_rj_12 ;
  double v_eta_rj_13 ;
  double v_eta_rj_14 ;
  double v_eta_rj_15 ;

  double v_phi_rj_1 ;
  double v_phi_rj_2 ;
  double v_phi_rj_3 ;
  double v_phi_rj_4 ;
  double v_phi_rj_5 ;
  double v_phi_rj_6 ;
  double v_phi_rj_7 ;
  double v_phi_rj_8 ;
  double v_phi_rj_9 ;
  double v_phi_rj_10 ;
  double v_phi_rj_11 ;
  double v_phi_rj_12 ;
  double v_phi_rj_13 ;
  double v_phi_rj_14 ;
  double v_phi_rj_15 ;

  double v_DR_rj_1 ;
  double v_DR_rj_2 ;
  double v_DR_rj_3 ;
  double v_DR_rj_4 ;
  double v_DR_rj_5 ;
  double v_DR_rj_6 ;
  
} ;


//  ------------------------------------------------------------


double deltaPhi (double phi1, double phi2) ;
double deltaEta (double eta1, double eta2) ;
double deltaR (double phi1, double eta1, double phi2,  double eta2) ;
int selector (TChain * tree, histos & plots, int if_signal=0) ;


//  --------G L O B A L   V A R S ---------------------

int g_ID1 ; 
int g_ID2 ;
int g_ISO1[2] ;
int g_ISO2[2] ;

double g_IsoElectron ;
double g_IsoMuon ;


double g_ojetPtMin ;
double g_ojetEtaMax ;
  
std::string g_prefix ;
std::string g_KindOfJet ;

int g_number_of_samples ; 
int g_numSignal ; 
std::string g_directory ;
std::string g_output ;


int g_cutsNum ;

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

 g_IsoElectron = subPSetSelections.getUntrackedParameter<double> ("g_IsoElectron",0.2) ;
 g_IsoMuon = subPSetSelections.getUntrackedParameter<double> ("g_IsoMuon",0.2) ;

 g_ojetPtMin = subPSetSelections.getUntrackedParameter<double> ("g_ojetPtMin",0) ;
 g_ojetEtaMax = subPSetSelections.getUntrackedParameter<double> ("g_ojetEtaMax",5.) ;

 g_prefix = subPSetSelections.getUntrackedParameter<std::string> ("g_prefix","pluto") ;
  
 
//  int i = 0;
 g_cutsNum = 23 ;
 g_cutsNum += 1 ; //PG one for the counting before cuts,
 g_cutsNum += 1 ; //PG one for the request of having two tag jets
 g_cutsNum += 1 ; //PG num of electrons
 g_cutsNum -= 3 ; //VT i 4 tagli dell'eleid valgono come 1
 g_cutsNum -= 2 ; //VT u 3 tagli sugli other jet valgono come 1

 g_prefix = subPSetSelections.getUntrackedParameter<std::string> ("g_prefix","pluto") ; 
 
 g_KindOfJet = subPSetSelections.getUntrackedParameter<std::string> ("g_KindOfJet","otherJets_IterativeCone5CaloJets") ; 
 

 //---- input file ----

 edm::ParameterSet subPSetInput =  parameterSet->getParameter<edm::ParameterSet> ("inputTree") ;
 
 
 std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("g_sample") ;
 
 g_number_of_samples = subPSetInput.getUntrackedParameter<int> ("g_number_of_samples",0) ;
 
 g_numSignal = subPSetInput.getUntrackedParameter<int> ("g_numSignal",1) ;

 
 
 g_directory = subPSetInput.getUntrackedParameter<std::string> ("g_directory","/media/amassiro/Data/SimpleTree_skipBadFiles_JetCorrector_JetCleaning_090328_Everything_Skimmed_4Cluster_AllJets") ; 
 
 g_output = subPSetInput.getUntrackedParameter<std::string> ("g_output","/tmp/amassiro/PLOT/new.root") ; 
 
 
 char *samples[100];
 
 int counter_files = 0;
 for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ; (listIt != inputFiles.end () && counter_files<g_number_of_samples); ++listIt) {
  samples[counter_files] = new char[100];
  sprintf(samples[counter_files],"%s",listIt->c_str ());
  std::cerr << "samples[" << counter_files << "] = " << samples[counter_files] << std::endl;
  counter_files ++;
 }
 
 
 

//  ------------------------------------------------------------

 g_OutputFile = new TFile(g_output.c_str(),"recreate");
//  TFile g_OutputFile(g_output.c_str(),"recreate");
 g_OutputFile->cd(0);

 std::cerr << "******************* creating samples ****************" << std::endl;  
 for (int yy=0; yy<g_number_of_samples; yy++){
  
  TChain * chain_ = new TChain ("ntpla/VBFSimpleTree") ; 
  
  char name_dir[1000];
  sprintf(name_dir,"%s/VBF_SimpleTree_%s.root",g_directory.c_str(),samples[yy]);
  
  chain_->Add (name_dir);
  
  char name_histo[1000];
  sprintf(name_histo,"%s_%s",g_prefix.c_str(),samples[yy]);
  
  std::cerr << "******************* creating sample ****************" << std::endl;  
  std::cerr << "*** " << name_histo << " ***" << std::endl;
  std::cerr << "*** " << samples[yy] << " ***" << std::endl;
  
  histos h_chain_ (name_histo,g_cutsNum); 
  int if_signal = 0;
  
  if (yy<g_numSignal) if_signal = 1;
  selector (chain_, h_chain_,if_signal) ;  

 }
 
 

 return 0 ;
}
















//  ------------------------------------------------------------


double 
   deltaPhi (double phi1, double phi2)
{ 
 double deltaphi = fabs (phi1 - phi2)  ;
 if (deltaphi > 6.283185308) deltaphi -= 6.283185308 ;
 if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi ;
 return deltaphi ;
}


//  ------------------------------------------------------------


double 
   deltaEta (double eta1, double eta2)
{ 
 double deltaeta = fabs (eta1 - eta2) ;
 return deltaeta ;
}



//  ------------------------------------------------------------


double 
   deltaR (double phi1, double eta1, double phi2, double eta2)
{ 
 double deltaphi = deltaPhi (phi1,phi2) ;
 double deltaeta = deltaEta (eta1,eta2) ;
 double deltar = sqrt(deltaphi*deltaphi + deltaeta*deltaeta) ;
 return deltar ;
}



//  ------------------------------------------------------------


//!PG main function
int 
  selector (TChain * tree, histos & plots, int if_signal)
{

 plots.v_eta_q_1 = -99;
 plots.v_eta_q_2 = -99;
 plots.v_eta_gj_1 = -99;
 plots.v_eta_gj_2 = -99;
 
 plots.v_eta_rj_1 = -99;
 plots.v_eta_rj_2 = -99;
 plots.v_eta_rj_3 = -99;
 plots.v_eta_rj_4 = -99;
 plots.v_eta_rj_5 = -99;
 plots.v_eta_rj_6 = -99;
 plots.v_eta_rj_7 = -99;
 plots.v_eta_rj_8 = -99;
 plots.v_eta_rj_9 = -99;
 plots.v_eta_rj_10 = -99;
 plots.v_eta_rj_11 = -99;
 plots.v_eta_rj_12 = -99;
 plots.v_eta_rj_13 = -99;
 plots.v_eta_rj_14 = -99;
 plots.v_eta_rj_15 = -99;
  
 plots.v_phi_rj_1 = -99;
 plots.v_phi_rj_2 = -99;
 plots.v_phi_rj_3 = -99;
 plots.v_phi_rj_4 = -99;
 plots.v_phi_rj_5 = -99;
 plots.v_phi_rj_6 = -99;
 plots.v_phi_rj_7 = -99;
 plots.v_phi_rj_8 = -99;
 plots.v_phi_rj_9 = -99;
 plots.v_phi_rj_10 = -99;
 plots.v_phi_rj_11 = -99;
 plots.v_phi_rj_12 = -99;
 plots.v_phi_rj_13 = -99;
 plots.v_phi_rj_14 = -99;
 plots.v_phi_rj_15 = -99;
 
 
 plots.v_DR_rj_1 = -99;
 plots.v_DR_rj_2 = -99;
 plots.v_DR_rj_3 = -99;
 plots.v_DR_rj_4 = -99;
 plots.v_DR_rj_5 = -99;
 plots.v_DR_rj_6 = -99;
 
 
//  TClonesArray * tagJets = new TClonesArray ("TLorentzVector") ; 
//  tree->SetBranchAddress ("tagJets", &tagJets) ;
 TClonesArray * otherJets_temp = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress (g_KindOfJet.c_str(), &otherJets_temp) ;
//  tree->SetBranchAddress ("otherJets", &otherJets_temp) ;
 
 
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
 
 
 TClonesArray * HiggsParticle = new TClonesArray ("TParticle") ;
 tree->SetBranchAddress ("HiggsParticle", &HiggsParticle) ;
 
  
 int EleId[100];
 float IsolEleSumPt_VBF[100];
 int nEle;
 tree->SetBranchAddress ("nEle", &nEle) ;
 tree->SetBranchAddress ("EleId",EleId ) ;
 tree->SetBranchAddress ("IsolEleSumPt_VBF",IsolEleSumPt_VBF ) ;

 float IsolMuTr[100];
 int nMu ;
 tree->SetBranchAddress ("nMu", &nMu) ;
 tree->SetBranchAddress ("IsolMuTr",IsolMuTr ) ;

 int IdEvent;
 tree->SetBranchAddress ("IdEvent", &IdEvent) ;
 
 
 int nentries = (int) tree->GetEntries () ;

 //PG loop over the events
//  std::cerr << " --- nentries = " << nentries << std::endl;
//  nentries = std::min(10000,nentries);
 nentries = 100;
 std::cerr << " --- nentries = " << nentries << std::endl;
 for (int evt = 0 ; evt < nentries ; ++evt)
 {
  if (!(evt%1000)) std::cerr << " --- evt = " << evt << std::endl;
  std::cerr << " --- evt = " << evt << std::endl;
 
  plots.v_eta_q_1 = -99;
  plots.v_eta_q_2 = -99;
  plots.v_eta_gj_1 = -99;
  plots.v_eta_gj_2 = -99;
  
  plots.v_eta_rj_1 = -99;
  plots.v_eta_rj_2 = -99;
  plots.v_eta_rj_3 = -99;
  plots.v_eta_rj_4 = -99;
  plots.v_eta_rj_5 = -99;
  plots.v_eta_rj_6 = -99;
  plots.v_eta_rj_7 = -99;
  plots.v_eta_rj_8 = -99;
  plots.v_eta_rj_9 = -99;
  plots.v_eta_rj_10 = -99;
  plots.v_eta_rj_11 = -99;
  plots.v_eta_rj_12 = -99;
  plots.v_eta_rj_13 = -99;
  plots.v_eta_rj_14 = -99;
  plots.v_eta_rj_15 = -99;
  
  plots.v_phi_rj_1 = -99;
  plots.v_phi_rj_2 = -99;
  plots.v_phi_rj_3 = -99;
  plots.v_phi_rj_4 = -99;
  plots.v_phi_rj_5 = -99;
  plots.v_phi_rj_6 = -99;
  plots.v_phi_rj_7 = -99;
  plots.v_phi_rj_8 = -99;
  plots.v_phi_rj_9 = -99;
  plots.v_phi_rj_10 = -99;
  plots.v_phi_rj_11 = -99;
  plots.v_phi_rj_12 = -99;
  plots.v_phi_rj_13 = -99;
  plots.v_phi_rj_14 = -99;
  plots.v_phi_rj_15 = -99;
  
  plots.v_DR_rj_1 = -99;
  plots.v_DR_rj_2 = -99;
  plots.v_DR_rj_3 = -99;
  plots.v_DR_rj_4 = -99;
  plots.v_DR_rj_5 = -99;
  plots.v_DR_rj_6 = -99;
  
  tree->GetEntry (evt) ;
  
  tagJets -> Clear () ;  
  otherJets -> Clear () ;    
  
  
  
    //---- check if signal ----
  if (if_signal && (IdEvent!=123 && IdEvent!=124)) continue;

  
  //---- MC data ----
  std::vector<TLorentzVector*> MCJets ;
  
  TLorentzVector* MCJets_temp[6] ;
  int counter = 0;
  
  if (if_signal && (IdEvent==123 || IdEvent==124)){
   for(int ii=0; ii<9; ii++){
    {
     if (ii==0 || ii==1 || ii==4 || ii==5 || ii==7 || ii==8) {
      TParticle* myparticle = (TParticle*) HiggsParticle->At(ii);
      MCJets_temp[counter] = new TLorentzVector;
      myparticle->Momentum(*(MCJets_temp[counter]));
      MCJets.push_back((MCJets_temp[counter]));
      if (ii==0) {
       plots.v_eta_q_1 = MCJets_temp[counter]->Eta();
       plots.v_phi_q_1 = MCJets_temp[counter]->Phi();
      }
      if (ii==1) {
       plots.v_eta_q_2 = MCJets_temp[counter]->Eta();
       plots.v_phi_q_2 = MCJets_temp[counter]->Phi();
      }
      if (ii==4) {
       plots.v_eta_V_1_q_1 = MCJets_temp[counter]->Eta();
       plots.v_phi_V_1_q_1 = MCJets_temp[counter]->Phi();
      }
      if (ii==5) {
       plots.v_eta_V_1_q_2 = MCJets_temp[counter]->Eta();
       plots.v_phi_V_1_q_2 = MCJets_temp[counter]->Phi();
      }
      if (ii==7) {
       plots.v_eta_V_2_q_1 = MCJets_temp[counter]->Eta();
       plots.v_phi_V_2_q_1 = MCJets_temp[counter]->Phi();
      }
      if (ii==8) {
       plots.v_eta_V_2_q_2 = MCJets_temp[counter]->Eta();
       plots.v_phi_V_2_q_2 = MCJets_temp[counter]->Phi();
      }

      counter++; 
     }
    }
   }
  }
  

   //---- find Tagging Jets ----
 
  double m_jetPtMin = 15.;
  double m_jetEtaMax = 5.;
  double m_jetDEtaMin = -1;
  double m_jetMjjMin = -1;
 
 
  std::vector<myJet> goodRJets ;

  for (int l=0; l<otherJets_temp->GetEntries (); l++ ){
   TLorentzVector* jet_temp = (TLorentzVector*) otherJets_temp->At(l);
   if (jet_temp->Pt()<m_jetPtMin) continue;
   //---- Eta max threshold ----
   if (jet_temp->Eta()>m_jetEtaMax) continue;
   //---- pt min threshold ----
   myJet dummy (jet_temp, 0, 0) ;
   goodRJets.push_back (dummy) ;
  }
    
  sort (goodRJets.rbegin (), goodRJets.rend (), lessThan ()) ;
  
  
  for (int l=0; l<goodRJets.size(); l++){
   if (l==0) {
    plots.v_eta_rj_1 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_1 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==1) {
    plots.v_eta_rj_2 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_2 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==2) {
    plots.v_eta_rj_3 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_3 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==3) {
    plots.v_eta_rj_4 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_4 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==4) {
    plots.v_eta_rj_5 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_5 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==5) {
    plots.v_eta_rj_6 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_6 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==6) {
    plots.v_eta_rj_7 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_7 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==7) {
    plots.v_eta_rj_8 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_8 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==8) {
    plots.v_eta_rj_9 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_9 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==9) {
    plots.v_eta_rj_10 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_10 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==10) {
    plots.v_eta_rj_11 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_11 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==11) {
    plots.v_eta_rj_12 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_12 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==12) {
    plots.v_eta_rj_13 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_13 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==13) {
    plots.v_eta_rj_14 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_14 = goodRJets.at (l).m_kine->Phi () ;
   }
   if (l==14) {
    plots.v_eta_rj_15 = goodRJets.at (l).m_kine->Eta () ;
    plots.v_phi_rj_15 = goodRJets.at (l).m_kine->Phi () ;
   }
  }
  
  
  std::cerr << " pippone 1" << std::endl;

  
  std::vector<std::pair<TLorentzVector*,TLorentzVector*> > Vect_PairQuark_RecoJet;
  std::vector<double> Map2D_PairQuark_RecoJet;

  int counter_map = 0;
    
  for (int rr=0; rr<std::min(6,static_cast<int>(goodRJets.size())); rr++ ){ //--- loop over recoJets ----
   for (int k=0; k<MCJets.size(); k++ ){ //--- loop over quarks ----
    TLorentzVector* quark_temp = MCJets.at(k);
    TLorentzVector* jet_temp = goodRJets.at(rr).m_kine;
    double DR = ROOT::Math::VectorUtil::DeltaR(quark_temp->BoostVector(),jet_temp->BoostVector());
    Map2D_PairQuark_RecoJet.push_back(DR);
    counter_map++;
   } //--- end loop over recoJet ----
  } //--- end loop over quarks ----
  
  std::cerr << " pippone 2" << std::endl;

  

  int selected_pair_jet[6] ;
  int selected_pair_quark[6] ;
  for (int jj=0; jj<MCJets.size(); jj++ ){  
   selected_pair_jet[jj] = -1;
   selected_pair_quark[jj] = -1;
  }

  std::cerr << " pippone 3" << std::endl;
  
  for (int jj=0; jj<MCJets.size(); jj++ ){  
   double DR_min = 1000;
   counter_map = 0;
   int temp_selected_pair_jet = -1;
   int temp_selected_pair_quark = -1;
   for (int rr=0; rr<std::min(6,static_cast<int>(goodRJets.size())); rr++ ){ //--- loop over recoJets ----
    for (int k=0; k<MCJets.size(); k++ ){ //--- loop over quarks ----
     bool already_done = false;
     for (int qq=0; qq<MCJets.size(); qq++) {
      if ((selected_pair_jet[qq] == rr) || (selected_pair_quark[qq] == k)) already_done = true;
     }
     if (!already_done){
      double DR_temp = Map2D_PairQuark_RecoJet.at(counter_map);
      if (DR_temp<DR_min) {
       DR_min = DR_temp;
       temp_selected_pair_jet = rr;
       temp_selected_pair_quark = k;
      }
     }
     counter_map++;
    }
   }
   selected_pair_jet[jj] = temp_selected_pair_jet;
   selected_pair_quark[jj] = temp_selected_pair_quark;
  }  
  
  std::cerr << " pippone 4" << std::endl;
  
  
  for (int rr=0; rr<std::min(6,static_cast<int>(goodRJets.size())); rr++ ){ //--- loop over recoJets ----
   for (int k=0; k<MCJets.size(); k++ ){ //--- loop over quarks ----
    bool used_one = false;
    for (int qq=0; qq<MCJets.size(); qq++) {
     if ((selected_pair_jet[qq] == rr) && (selected_pair_quark[qq] == k)) used_one = true;
    }
    if (used_one){
     TLorentzVector* quark_temp = MCJets.at(k);
     TLorentzVector* jet_temp = goodRJets.at(rr).m_kine;
     std::pair<TLorentzVector*,TLorentzVector*> PairQuark_RecoJet(quark_temp,jet_temp);
     Vect_PairQuark_RecoJet.push_back(PairQuark_RecoJet);
    }
   } //--- end loop over recoJet ----
  } //--- end loop over quarks ----

  
  std::cerr << " ******* son qui *********** " << std::endl;
    
    
  for (int iJet=0; iJet<std::min(6,static_cast<int>(Vect_PairQuark_RecoJet.size())); iJet++){
   std::cerr << "         ******* iJet = " << iJet << std::endl;
   double minDR = -1000;
   double eta_reco_temp = goodRJets.at (iJet).m_kine->Eta () ;
   double phi_reco_temp = goodRJets.at (iJet).m_kine->Phi () ;  
   std::cerr << "         ******* iJet = " << iJet << std::endl;
   
   for (int pp=0; pp<static_cast<int>(Vect_PairQuark_RecoJet.size()); pp++ ){
    double eta_1 = Vect_PairQuark_RecoJet.at(pp).second->Eta();
    double phi_1 = Vect_PairQuark_RecoJet.at(pp).second->Phi();

    double DR_temp = deltaR(phi_1,eta_1,phi_reco_temp,eta_reco_temp);
    if (DR_temp<0.001) {
     double eta_2 = Vect_PairQuark_RecoJet.at(pp).first->Eta();
     double phi_2 = Vect_PairQuark_RecoJet.at(pp).first->Phi();
     minDR = deltaR(phi_1,eta_1,phi_2,eta_2);
     break;
    }
   }
    
    
   if (iJet==0) {
    plots.v_eta_rj_1 = goodRJets.at (iJet).m_kine->Eta () ;
    plots.v_phi_rj_1 = goodRJets.at (iJet).m_kine->Phi () ;
    plots.v_DR_rj_1 = minDR;
   }
   if (iJet==1) {
    plots.v_eta_rj_2 = goodRJets.at (iJet).m_kine->Eta () ;
    plots.v_phi_rj_2 = goodRJets.at (iJet).m_kine->Phi () ;
    plots.v_DR_rj_2 = minDR;
   }
   if (iJet==2) {
    plots.v_eta_rj_3 = goodRJets.at (iJet).m_kine->Eta () ;
    plots.v_phi_rj_3 = goodRJets.at (iJet).m_kine->Phi () ;
    plots.v_DR_rj_3 = minDR;
   }
   if (iJet==3) {
    plots.v_eta_rj_4 = goodRJets.at (iJet).m_kine->Eta () ;
    plots.v_phi_rj_4 = goodRJets.at (iJet).m_kine->Phi () ;
    plots.v_DR_rj_4 = minDR;
   }
   if (iJet==4) {
    plots.v_eta_rj_5 = goodRJets.at (iJet).m_kine->Eta () ;
    plots.v_phi_rj_5 = goodRJets.at (iJet).m_kine->Phi () ;
    plots.v_DR_rj_5 = minDR;
   }
   if (iJet==5) {
    plots.v_eta_rj_6 = goodRJets.at (iJet).m_kine->Eta () ;
    plots.v_phi_rj_6 = goodRJets.at (iJet).m_kine->Phi () ;
    plots.v_DR_rj_6 = minDR;
   }
   std::cerr << "         ******* iJet = " << iJet << std::endl<< std::endl;
  }  

  std::cerr << "         ******* ciao " << std::endl ;
  
  plots.m_tree_selections->Fill();
  for (int hh=0; hh<counter; hh++) delete MCJets_temp[hh];
  
  std::cerr << "         ******* ciao " << std::endl<< std::endl;
  
 } //PG loop over the events

 g_OutputFile->cd(0);
 plots.m_tree_selections->Write();

 delete otherJets_temp ;
 delete tagJets  ;  
 delete otherJets  ;
 delete electrons  ;
 delete muons  ;    
 delete MET  ;      
 delete tracks  ;   

 
 return 0;
  
}

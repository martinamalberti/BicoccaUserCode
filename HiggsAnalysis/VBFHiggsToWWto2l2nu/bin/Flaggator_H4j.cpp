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

TFile g_OutputFile("/tmp/amassiro/PLOT/out_Flag_All_19May09_2000_H4j.root","recreate");
  

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
     
   m_tree_selections->Branch("v_Jet_1_Pt",&v_Jet_1_Pt,"v_Jet_1_Pt/D");
   m_tree_selections->Branch("v_Jet_2_Pt",&v_Jet_2_Pt,"v_Jet_2_Pt/D");
   m_tree_selections->Branch("v_Jet_3_Pt",&v_Jet_3_Pt,"v_Jet_3_Pt/D");
   m_tree_selections->Branch("v_Jet_4_Pt",&v_Jet_4_Pt,"v_Jet_4_Pt/D");
   m_tree_selections->Branch("v_Jet_5_Pt",&v_Jet_5_Pt,"v_Jet_5_Pt/D");
   m_tree_selections->Branch("v_Jet_6_Pt",&v_Jet_6_Pt,"v_Jet_6_Pt/D");

   m_tree_selections->Branch("v_Jet_1_x",&v_Jet_1_x,"v_Jet_1_x/D");
   m_tree_selections->Branch("v_Jet_2_x",&v_Jet_2_x,"v_Jet_2_x/D");
   m_tree_selections->Branch("v_Jet_3_x",&v_Jet_3_x,"v_Jet_3_x/D");
   m_tree_selections->Branch("v_Jet_4_x",&v_Jet_4_x,"v_Jet_4_x/D");
   m_tree_selections->Branch("v_Jet_5_x",&v_Jet_5_x,"v_Jet_5_x/D");
   m_tree_selections->Branch("v_Jet_6_x",&v_Jet_6_x,"v_Jet_6_x/D");

   m_tree_selections->Branch("v_Jet_1_y",&v_Jet_1_y,"v_Jet_1_y/D");
   m_tree_selections->Branch("v_Jet_2_y",&v_Jet_2_y,"v_Jet_2_y/D");
   m_tree_selections->Branch("v_Jet_3_y",&v_Jet_3_y,"v_Jet_3_y/D");
   m_tree_selections->Branch("v_Jet_4_y",&v_Jet_4_y,"v_Jet_4_y/D");
   m_tree_selections->Branch("v_Jet_5_y",&v_Jet_5_y,"v_Jet_5_y/D");
   m_tree_selections->Branch("v_Jet_6_y",&v_Jet_6_y,"v_Jet_6_y/D");

   m_tree_selections->Branch("v_Jet_1_z",&v_Jet_1_z,"v_Jet_1_z/D");
   m_tree_selections->Branch("v_Jet_2_z",&v_Jet_2_z,"v_Jet_2_z/D");
   m_tree_selections->Branch("v_Jet_3_z",&v_Jet_3_z,"v_Jet_3_z/D");
   m_tree_selections->Branch("v_Jet_4_z",&v_Jet_4_z,"v_Jet_4_z/D");
   m_tree_selections->Branch("v_Jet_5_z",&v_Jet_5_z,"v_Jet_5_z/D");
   m_tree_selections->Branch("v_Jet_6_z",&v_Jet_6_z,"v_Jet_6_z/D");

   m_tree_selections->Branch("v_Jet_1_e",&v_Jet_1_e,"v_Jet_1_e/D");
   m_tree_selections->Branch("v_Jet_2_e",&v_Jet_2_e,"v_Jet_2_e/D");
   m_tree_selections->Branch("v_Jet_3_e",&v_Jet_3_e,"v_Jet_3_e/D");
   m_tree_selections->Branch("v_Jet_4_e",&v_Jet_4_e,"v_Jet_4_e/D");
   m_tree_selections->Branch("v_Jet_5_e",&v_Jet_5_e,"v_Jet_5_e/D");
   m_tree_selections->Branch("v_Jet_6_e",&v_Jet_6_e,"v_Jet_6_e/D");
 
   m_tree_selections->Branch("v_Jet_1_DR",&v_Jet_1_DR,"v_Jet_1_DR/D");
   m_tree_selections->Branch("v_Jet_2_DR",&v_Jet_2_DR,"v_Jet_2_DR/D");
   m_tree_selections->Branch("v_Jet_3_DR",&v_Jet_3_DR,"v_Jet_3_DR/D");
   m_tree_selections->Branch("v_Jet_4_DR",&v_Jet_4_DR,"v_Jet_4_DR/D");
   m_tree_selections->Branch("v_Jet_5_DR",&v_Jet_5_DR,"v_Jet_5_DR/D");
   m_tree_selections->Branch("v_Jet_6_DR",&v_Jet_6_DR,"v_Jet_6_DR/D");
   
   m_tree_selections->Branch("v_numEle",&v_numEle,"v_numEle/D");
   m_tree_selections->Branch("v_numMu",&v_numMu,"v_numMu/D");
   m_tree_selections->Branch("v_numJets",&v_numJets,"v_numJets/D");
   
   
   TString m_efficiency_Name = m_name + "_m_efficiency" ;
   m_efficiency = new TTree(m_efficiency_Name,m_efficiency_Name);
   m_efficiency->Branch("analyzed",&analyzed,"analyzed/I");
   m_efficiency->Branch("passedJetAndLepNumberSelections",&passedJetAndLepNumberSelections,"passedJetAndLepNumberSelections/I");
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
  TTree* m_efficiency;
  int analyzed;
  int passedJetAndLepNumberSelections;
    

  //! AM ---- TTree ----
  TTree* m_tree_selections;
  double v_Jet_1_Pt ;
  double v_Jet_2_Pt ;
  double v_Jet_3_Pt ;
  double v_Jet_4_Pt ;
  double v_Jet_5_Pt ;
  double v_Jet_6_Pt ;

  double v_Jet_1_x ;
  double v_Jet_2_x ;
  double v_Jet_3_x ;
  double v_Jet_4_x ;
  double v_Jet_5_x ;
  double v_Jet_6_x ;

  double v_Jet_1_y ;
  double v_Jet_2_y ;
  double v_Jet_3_y ;
  double v_Jet_4_y ;
  double v_Jet_5_y ;
  double v_Jet_6_y ;

  double v_Jet_1_z ;
  double v_Jet_2_z ;
  double v_Jet_3_z ;
  double v_Jet_4_z ;
  double v_Jet_5_z ;
  double v_Jet_6_z ;

  double v_Jet_1_e ;
  double v_Jet_2_e ;
  double v_Jet_3_e ;
  double v_Jet_4_e ;
  double v_Jet_5_e ;
  double v_Jet_6_e ;

  double v_numEle ;
  double v_numMu ;
  double v_numJets ;

  double v_Jet_1_DR ;
  double v_Jet_2_DR ;
  double v_Jet_3_DR ;
  double v_Jet_4_DR ;
  double v_Jet_5_DR ;
  double v_Jet_6_DR ;


    
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
 
 char *samples[100];
 
 int counter_files = 0;
 for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ; (listIt != inputFiles.end () && counter_files<g_number_of_samples); ++listIt) {
  samples[counter_files] = new char[100];
  sprintf(samples[counter_files],"%s",listIt->c_str ());
  std::cerr << "samples[" << counter_files << "] = " << samples[counter_files] << std::endl;
  counter_files ++;
 }

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
 double deltaphi = fabs (phi1 - phi2) ;
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
 
 plots.v_Jet_1_Pt = -99;
 plots.v_Jet_2_Pt = -99;
 plots.v_Jet_3_Pt = -99;
 plots.v_Jet_4_Pt = -99;
 plots.v_Jet_5_Pt = -99;
 plots.v_Jet_6_Pt = -99;

 plots.v_Jet_1_x = -99;
 plots.v_Jet_2_x = -99;
 plots.v_Jet_3_x = -99;
 plots.v_Jet_4_x = -99;
 plots.v_Jet_5_x = -99;
 plots.v_Jet_6_x = -99;

 plots.v_Jet_1_y = -99;
 plots.v_Jet_2_y = -99;
 plots.v_Jet_3_y = -99;
 plots.v_Jet_4_y = -99;
 plots.v_Jet_5_y = -99;
 plots.v_Jet_6_y = -99;

 plots.v_Jet_1_z = -99;
 plots.v_Jet_2_z = -99;
 plots.v_Jet_3_z = -99;
 plots.v_Jet_4_z = -99;
 plots.v_Jet_5_z = -99;
 plots.v_Jet_6_z = -99;

 plots.v_Jet_1_e = -99;
 plots.v_Jet_2_e = -99;
 plots.v_Jet_3_e = -99;
 plots.v_Jet_4_e = -99;
 plots.v_Jet_5_e = -99;
 plots.v_Jet_6_e = -99;
 
 plots.v_Jet_1_DR = -99;
 plots.v_Jet_2_DR = -99;
 plots.v_Jet_3_DR = -99;
 plots.v_Jet_4_DR = -99;
 plots.v_Jet_5_DR = -99;
 plots.v_Jet_6_DR = -99;
 
 plots.v_numEle = -99;
 plots.v_numMu = -99;
 plots.v_numJets = -99;
 
  
 
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

 
 plots.passedJetAndLepNumberSelections = 0;
 plots.analyzed = 0;
 
 //PG loop over the events
 for (int evt = 0 ; evt < nentries ; ++evt)
 {
  plots.v_Jet_1_Pt = -99;
  plots.v_Jet_2_Pt = -99;
  plots.v_Jet_3_Pt = -99;
  plots.v_Jet_4_Pt = -99;
  plots.v_Jet_5_Pt = -99;
  plots.v_Jet_6_Pt = -99;

  plots.v_Jet_1_x = -99;
  plots.v_Jet_2_x = -99;
  plots.v_Jet_3_x = -99;
  plots.v_Jet_4_x = -99;
  plots.v_Jet_5_x = -99;
  plots.v_Jet_6_x = -99;

  plots.v_Jet_1_y = -99;
  plots.v_Jet_2_y = -99;
  plots.v_Jet_3_y = -99;
  plots.v_Jet_4_y = -99;
  plots.v_Jet_5_y = -99;
  plots.v_Jet_6_y = -99;

  plots.v_Jet_1_z = -99;
  plots.v_Jet_2_z = -99;
  plots.v_Jet_3_z = -99;
  plots.v_Jet_4_z = -99;
  plots.v_Jet_5_z = -99;
  plots.v_Jet_6_z = -99;

  plots.v_Jet_1_e = -99;
  plots.v_Jet_2_e = -99;
  plots.v_Jet_3_e = -99;
  plots.v_Jet_4_e = -99;
  plots.v_Jet_5_e = -99;
  plots.v_Jet_6_e = -99;
  
  plots.v_Jet_1_DR = -99;
  plots.v_Jet_2_DR = -99;
  plots.v_Jet_3_DR = -99;
  plots.v_Jet_4_DR = -99;
  plots.v_Jet_5_DR = -99;
  plots.v_Jet_6_DR = -99;
 
  plots.v_numEle = -99;
  plots.v_numMu = -99;
  plots.v_numJets = -99;
  
  
  tree->GetEntry (evt) ;
  
  tagJets -> Clear () ;  
  otherJets -> Clear () ;    
  
  
  
    //---- check if signal ----
  if (if_signal && (IdEvent!=123 && IdEvent!=124)) continue;
  plots.analyzed++;
  
  
  //---- MC data ----
  std::vector<TLorentzVector*> MCJets ;
  
  if (if_signal && (IdEvent==123 || IdEvent==124)){
   for(int ii=0; ii<9; ii++){
    if (ii!=2 && ii!=3 && ii!=6){
     TParticle* myparticle = (TParticle*) HiggsParticle->At(ii);
     TLorentzVector* momentum_myparticle;
     myparticle->Momentum(*momentum_myparticle);
     MCJets.push_back(momentum_myparticle);
    }
   }
  }
  
  
  
   //---- find Tagging Jets ----
 
  double m_jetPtMin = 15.;
  double m_jetEtaMax = 5.;
  double m_jetDEtaMin = -1;
  double m_jetMjjMin = -1;
 
  

  std::vector<myJet> goodJets ;

  
  for (int l=0; l<otherJets_temp->GetEntries (); l++ ){
   TLorentzVector* jet_temp = (TLorentzVector*) otherJets_temp->At(l);
   if (jet_temp->Pt()<m_jetPtMin) continue;
   //---- Eta max threshold ----
   if (jet_temp->Eta()>m_jetEtaMax) continue;
   //---- pt min threshold ----
   myJet dummy (jet_temp, 0, 0) ;
   goodJets.push_back (dummy) ;
  }
  
  sort (goodJets.rbegin (), goodJets.rend (), lessThan ()) ;
  
  int numJets = goodJets.size();
  plots.v_numJets = numJets;
  if (numJets >=6) {
   for (int iJet=0; iJet<6; iJet++){
    if (iJet==0) {
     plots.v_Jet_1_Pt = goodJets.at (iJet).m_kine->Pt () ;
     plots.v_Jet_1_x = goodJets.at (iJet).m_kine->X () ;
     plots.v_Jet_1_y = goodJets.at (iJet).m_kine->Y () ;
     plots.v_Jet_1_z = goodJets.at (iJet).m_kine->Z () ;
     double eta_reco = goodJets.at (iJet).m_kine->Eta () ;
     double phi_reco = goodJets.at (iJet).m_kine->Phi () ;
     int sizeMCJets = MCJets.size();
     double minDR = 1000;
     for (int kk=0; kk<sizeMCJets; kk++){
      double eta_MC = MCJets.at(kk)->Eta();
      double phi_MC = MCJets.at(kk)->Phi();
      double temp_DR = deltaR(phi_reco,eta_MC,phi_MC,eta_MC);
      if (temp_DR<minDR) minDR = temp_DR;
     }
     plots.v_Jet_1_DR = minDR;
    }
    if (iJet==1) {
     plots.v_Jet_2_Pt = goodJets.at (iJet).m_kine->Pt () ;
     plots.v_Jet_2_x = goodJets.at (iJet).m_kine->X () ;
     plots.v_Jet_2_y = goodJets.at (iJet).m_kine->Y () ;
     plots.v_Jet_2_z = goodJets.at (iJet).m_kine->Z () ;
     double eta_reco = goodJets.at (iJet).m_kine->Eta () ;
     double phi_reco = goodJets.at (iJet).m_kine->Phi () ;
     int sizeMCJets = MCJets.size();
     double minDR = 1000;
     for (int kk=0; kk<sizeMCJets; kk++){
      double eta_MC = MCJets.at(kk)->Eta();
      double phi_MC = MCJets.at(kk)->Phi();
      double temp_DR = deltaR(phi_reco,eta_MC,phi_MC,eta_MC);
      if (temp_DR<minDR) minDR = temp_DR;
     }
     plots.v_Jet_2_DR = minDR;
    }
    if (iJet==2) {
     plots.v_Jet_3_Pt = goodJets.at (iJet).m_kine->Pt () ;
     plots.v_Jet_3_x = goodJets.at (iJet).m_kine->X () ;
     plots.v_Jet_3_y = goodJets.at (iJet).m_kine->Y () ;
     plots.v_Jet_3_z = goodJets.at (iJet).m_kine->Z () ;
     double eta_reco = goodJets.at (iJet).m_kine->Eta () ;
     double phi_reco = goodJets.at (iJet).m_kine->Phi () ;
     int sizeMCJets = MCJets.size();
     double minDR = 1000;
     for (int kk=0; kk<sizeMCJets; kk++){
      double eta_MC = MCJets.at(kk)->Eta();
      double phi_MC = MCJets.at(kk)->Phi();
      double temp_DR = deltaR(phi_reco,eta_MC,phi_MC,eta_MC);
      if (temp_DR<minDR) minDR = temp_DR;
     }
     plots.v_Jet_3_DR = minDR;
    }
    if (iJet==3) {
     plots.v_Jet_4_Pt = goodJets.at (iJet).m_kine->Pt () ;
     plots.v_Jet_4_x = goodJets.at (iJet).m_kine->X () ;
     plots.v_Jet_4_y = goodJets.at (iJet).m_kine->Y () ;
     plots.v_Jet_4_z = goodJets.at (iJet).m_kine->Z () ;
     double eta_reco = goodJets.at (iJet).m_kine->Eta () ;
     double phi_reco = goodJets.at (iJet).m_kine->Phi () ;
     int sizeMCJets = MCJets.size();
     double minDR = 1000;
     for (int kk=0; kk<sizeMCJets; kk++){
      double eta_MC = MCJets.at(kk)->Eta();
      double phi_MC = MCJets.at(kk)->Phi();
      double temp_DR = deltaR(phi_reco,eta_MC,phi_MC,eta_MC);
      if (temp_DR<minDR) minDR = temp_DR;
     }
     plots.v_Jet_4_DR = minDR;
    }
    if (iJet==4) {
     plots.v_Jet_5_Pt = goodJets.at (iJet).m_kine->Pt () ;
     plots.v_Jet_5_x = goodJets.at (iJet).m_kine->X () ;
     plots.v_Jet_5_y = goodJets.at (iJet).m_kine->Y () ;
     plots.v_Jet_5_z = goodJets.at (iJet).m_kine->Z () ;
     double eta_reco = goodJets.at (iJet).m_kine->Eta () ;
     double phi_reco = goodJets.at (iJet).m_kine->Phi () ;
     int sizeMCJets = MCJets.size();
     double minDR = 1000;
     for (int kk=0; kk<sizeMCJets; kk++){
      double eta_MC = MCJets.at(kk)->Eta();
      double phi_MC = MCJets.at(kk)->Phi();
      double temp_DR = deltaR(phi_reco,eta_MC,phi_MC,eta_MC);
      if (temp_DR<minDR) minDR = temp_DR;
     }
     plots.v_Jet_5_DR = minDR;
    }
    if (iJet==5) {
     plots.v_Jet_6_Pt = goodJets.at (iJet).m_kine->Pt () ;
     plots.v_Jet_6_x = goodJets.at (iJet).m_kine->X () ;
     plots.v_Jet_6_y = goodJets.at (iJet).m_kine->Y () ;
     plots.v_Jet_6_z = goodJets.at (iJet).m_kine->Z () ;
     double eta_reco = goodJets.at (iJet).m_kine->Eta () ;
     double phi_reco = goodJets.at (iJet).m_kine->Phi () ;
     int sizeMCJets = MCJets.size();
     double minDR = 1000;
     for (int kk=0; kk<sizeMCJets; kk++){
      double eta_MC = MCJets.at(kk)->Eta();
      double phi_MC = MCJets.at(kk)->Phi();
      double temp_DR = deltaR(phi_reco,eta_MC,phi_MC,eta_MC);
      if (temp_DR<minDR) minDR = temp_DR;
     }
     plots.v_Jet_6_DR = minDR;
    }
   }
  }
  
  
  
  
  //---- leptons ----
  
  plots.v_numEle = 0;
  plots.v_numMu = 0;
 
  std::vector<lepton> leptons ;
            
  //---- AM ---- loop over electrons
  for (int iele = 0; iele < electrons->GetEntries () ; ++iele)
  {
   TLorentzVector * theEle = (TLorentzVector*) (electrons->At (iele)) ;
   lepton dummy (theEle, 0, iele) ;
   leptons.push_back (dummy) ;
  } 

  //---- AM ---- loop over muons
  for (int imu = 0 ; imu < nMu ; ++imu)
  {
   TLorentzVector * theMu = (TLorentzVector*) (muons->At (imu)) ;
   lepton dummy (theMu, 1, imu) ;
   leptons.push_back (dummy) ;
  }
  

  for (int ilep=0 ; ilep < leptons.size () ; ++ilep){
   if (leptons.at (ilep).m_flav == 0) {//PG electron
    plots.v_numEle += 1;
    }
   if (leptons.at (ilep).m_flav == 1) {//PG muon
     plots.v_numMu += 1;
   }
  }
  
  
  
  
  
  
  
  plots.m_tree_selections->Fill();
  plots.passedJetAndLepNumberSelections++;
  
 } //PG loop over the events


 plots.m_efficiency->Fill();
 plots.m_efficiency->Write();
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

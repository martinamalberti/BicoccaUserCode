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

// TFile g_OutputFile("/tmp/amassiro/PLOT/prova.root","recreate");
TFile *g_OutputFile;

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
//   if ((couple1.first == couple2.first) || (couple1.second == couple2.second)) return 0;
    
  return ROOT::Math::VectorUtil::DeltaR(couple1.first->BoostVector(),couple1.second->BoostVector()) < ROOT::Math::VectorUtil::DeltaR(couple2.first->BoostVector(),couple2.second->BoostVector());
 
 
 
 
  //--- se ho lo stesso jet, ordina ... 
//   if (couple1.second == couple2.second)
//    return ROOT::Math::VectorUtil::DeltaR(couple1.first->BoostVector(),couple1.second->BoostVector()) < ROOT::Math::VectorUtil::DeltaR(couple2.first->BoostVector(),couple2.second->BoostVector());

  //--- se il jet è diverso, lascialli dove sono ...
//   return 0;
 
 
 
 
 
 
 
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
 
struct PointerSorting_first : public std::binary_function<
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
  return couple1.first < couple2.first;
 }
} ;

//  ------------------------------------------------------------

struct PointerSorting_second : public std::binary_function<
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
  return couple1.second < couple2.second;
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
//   return ((couple1.first == couple2.first) || (couple1.second == couple2.second));
//   return ((couple1.first == couple2.first));
  return ((couple1.second == couple2.second));
 }
};

//  ------------------------------------------------------------

struct DeltaRSame_first : public
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
  return ((couple1.first == couple2.first));
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
     
   m_tree_selections->Branch("v_Jet_1_Pt",&v_Jet_1_Pt,"v_Jet_1_Pt/D");
   m_tree_selections->Branch("v_Jet_2_Pt",&v_Jet_2_Pt,"v_Jet_2_Pt/D");
   m_tree_selections->Branch("v_Jet_3_Pt",&v_Jet_3_Pt,"v_Jet_3_Pt/D");
   m_tree_selections->Branch("v_Jet_4_Pt",&v_Jet_4_Pt,"v_Jet_4_Pt/D");
   m_tree_selections->Branch("v_Jet_5_Pt",&v_Jet_5_Pt,"v_Jet_5_Pt/D");
   m_tree_selections->Branch("v_Jet_6_Pt",&v_Jet_6_Pt,"v_Jet_6_Pt/D");
   m_tree_selections->Branch("v_Jet_7_Pt",&v_Jet_7_Pt,"v_Jet_7_Pt/D");
   m_tree_selections->Branch("v_Jet_8_Pt",&v_Jet_8_Pt,"v_Jet_8_Pt/D");
   
   m_tree_selections->Branch("v_Jet_1_x",&v_Jet_1_x,"v_Jet_1_x/D");
   m_tree_selections->Branch("v_Jet_2_x",&v_Jet_2_x,"v_Jet_2_x/D");
   m_tree_selections->Branch("v_Jet_3_x",&v_Jet_3_x,"v_Jet_3_x/D");
   m_tree_selections->Branch("v_Jet_4_x",&v_Jet_4_x,"v_Jet_4_x/D");
   m_tree_selections->Branch("v_Jet_5_x",&v_Jet_5_x,"v_Jet_5_x/D");
   m_tree_selections->Branch("v_Jet_6_x",&v_Jet_6_x,"v_Jet_6_x/D");
   m_tree_selections->Branch("v_Jet_7_x",&v_Jet_7_x,"v_Jet_7_x/D");
   m_tree_selections->Branch("v_Jet_8_x",&v_Jet_8_x,"v_Jet_8_x/D");

   m_tree_selections->Branch("v_Jet_1_y",&v_Jet_1_y,"v_Jet_1_y/D");
   m_tree_selections->Branch("v_Jet_2_y",&v_Jet_2_y,"v_Jet_2_y/D");
   m_tree_selections->Branch("v_Jet_3_y",&v_Jet_3_y,"v_Jet_3_y/D");
   m_tree_selections->Branch("v_Jet_4_y",&v_Jet_4_y,"v_Jet_4_y/D");
   m_tree_selections->Branch("v_Jet_5_y",&v_Jet_5_y,"v_Jet_5_y/D");
   m_tree_selections->Branch("v_Jet_6_y",&v_Jet_6_y,"v_Jet_6_y/D");
   m_tree_selections->Branch("v_Jet_7_y",&v_Jet_7_y,"v_Jet_7_y/D");
   m_tree_selections->Branch("v_Jet_8_y",&v_Jet_8_y,"v_Jet_8_y/D");

   m_tree_selections->Branch("v_Jet_1_z",&v_Jet_1_z,"v_Jet_1_z/D");
   m_tree_selections->Branch("v_Jet_2_z",&v_Jet_2_z,"v_Jet_2_z/D");
   m_tree_selections->Branch("v_Jet_3_z",&v_Jet_3_z,"v_Jet_3_z/D");
   m_tree_selections->Branch("v_Jet_4_z",&v_Jet_4_z,"v_Jet_4_z/D");
   m_tree_selections->Branch("v_Jet_5_z",&v_Jet_5_z,"v_Jet_5_z/D");
   m_tree_selections->Branch("v_Jet_6_z",&v_Jet_6_z,"v_Jet_6_z/D");
   m_tree_selections->Branch("v_Jet_7_z",&v_Jet_7_z,"v_Jet_7_z/D");
   m_tree_selections->Branch("v_Jet_8_z",&v_Jet_8_z,"v_Jet_8_z/D");

   m_tree_selections->Branch("v_Jet_1_e",&v_Jet_1_e,"v_Jet_1_e/D");
   m_tree_selections->Branch("v_Jet_2_e",&v_Jet_2_e,"v_Jet_2_e/D");
   m_tree_selections->Branch("v_Jet_3_e",&v_Jet_3_e,"v_Jet_3_e/D");
   m_tree_selections->Branch("v_Jet_4_e",&v_Jet_4_e,"v_Jet_4_e/D");
   m_tree_selections->Branch("v_Jet_5_e",&v_Jet_5_e,"v_Jet_5_e/D");
   m_tree_selections->Branch("v_Jet_6_e",&v_Jet_6_e,"v_Jet_6_e/D");
   m_tree_selections->Branch("v_Jet_7_e",&v_Jet_7_e,"v_Jet_7_e/D");
   m_tree_selections->Branch("v_Jet_8_e",&v_Jet_8_e,"v_Jet_8_e/D");
 
   m_tree_selections->Branch("v_Jet_1_eta",&v_Jet_1_eta,"v_Jet_1_eta/D");
   m_tree_selections->Branch("v_Jet_2_eta",&v_Jet_2_eta,"v_Jet_2_eta/D");
   m_tree_selections->Branch("v_Jet_3_eta",&v_Jet_3_eta,"v_Jet_3_eta/D");
   m_tree_selections->Branch("v_Jet_4_eta",&v_Jet_4_eta,"v_Jet_4_eta/D");
   m_tree_selections->Branch("v_Jet_5_eta",&v_Jet_5_eta,"v_Jet_5_eta/D");
   m_tree_selections->Branch("v_Jet_6_eta",&v_Jet_6_eta,"v_Jet_6_eta/D");
   m_tree_selections->Branch("v_Jet_7_eta",&v_Jet_7_eta,"v_Jet_7_eta/D");
   m_tree_selections->Branch("v_Jet_8_eta",&v_Jet_8_eta,"v_Jet_8_eta/D");
   
   m_tree_selections->Branch("v_Jet_1_phi",&v_Jet_1_phi,"v_Jet_1_phi/D");
   m_tree_selections->Branch("v_Jet_2_phi",&v_Jet_2_phi,"v_Jet_2_phi/D");
   m_tree_selections->Branch("v_Jet_3_phi",&v_Jet_3_phi,"v_Jet_3_phi/D");
   m_tree_selections->Branch("v_Jet_4_phi",&v_Jet_4_phi,"v_Jet_4_phi/D");
   m_tree_selections->Branch("v_Jet_5_phi",&v_Jet_5_phi,"v_Jet_5_phi/D");
   m_tree_selections->Branch("v_Jet_6_phi",&v_Jet_6_phi,"v_Jet_6_phi/D");
   m_tree_selections->Branch("v_Jet_7_phi",&v_Jet_7_phi,"v_Jet_7_phi/D");
   m_tree_selections->Branch("v_Jet_8_phi",&v_Jet_8_phi,"v_Jet_8_phi/D");
   
   m_tree_selections->Branch("v_Jet_1_DR",&v_Jet_1_DR,"v_Jet_1_DR/D");
   m_tree_selections->Branch("v_Jet_2_DR",&v_Jet_2_DR,"v_Jet_2_DR/D");
   m_tree_selections->Branch("v_Jet_3_DR",&v_Jet_3_DR,"v_Jet_3_DR/D");
   m_tree_selections->Branch("v_Jet_4_DR",&v_Jet_4_DR,"v_Jet_4_DR/D");
   m_tree_selections->Branch("v_Jet_5_DR",&v_Jet_5_DR,"v_Jet_5_DR/D");
   m_tree_selections->Branch("v_Jet_6_DR",&v_Jet_6_DR,"v_Jet_6_DR/D");
   m_tree_selections->Branch("v_Jet_7_DR",&v_Jet_7_DR,"v_Jet_7_DR/D");
   m_tree_selections->Branch("v_Jet_8_DR",&v_Jet_8_DR,"v_Jet_8_DR/D");
   
   m_tree_selections->Branch("v_numEle",&v_numEle,"v_numEle/D");
   m_tree_selections->Branch("v_numMu",&v_numMu,"v_numMu/D");
   m_tree_selections->Branch("v_numJets",&v_numJets,"v_numJets/D");
   m_tree_selections->Branch("v_totNumJets",&v_totNumJets,"v_totNumJets/D");
   
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
  double v_Jet_7_Pt ;
  double v_Jet_8_Pt ;
  
  double v_Jet_1_x ;
  double v_Jet_2_x ;
  double v_Jet_3_x ;
  double v_Jet_4_x ;
  double v_Jet_5_x ;
  double v_Jet_6_x ;
  double v_Jet_7_x ;
  double v_Jet_8_x ;

  double v_Jet_1_y ;
  double v_Jet_2_y ;
  double v_Jet_3_y ;
  double v_Jet_4_y ;
  double v_Jet_5_y ;
  double v_Jet_6_y ;
  double v_Jet_7_y ;
  double v_Jet_8_y ;

  double v_Jet_1_z ;
  double v_Jet_2_z ;
  double v_Jet_3_z ;
  double v_Jet_4_z ;
  double v_Jet_5_z ;
  double v_Jet_6_z ;
  double v_Jet_7_z ;
  double v_Jet_8_z ;

  double v_Jet_1_e ;
  double v_Jet_2_e ;
  double v_Jet_3_e ;
  double v_Jet_4_e ;
  double v_Jet_5_e ;
  double v_Jet_6_e ;
  double v_Jet_7_e ;
  double v_Jet_8_e ;

  double v_Jet_1_eta ;
  double v_Jet_2_eta ;
  double v_Jet_3_eta ;
  double v_Jet_4_eta ;
  double v_Jet_5_eta ;
  double v_Jet_6_eta ;
  double v_Jet_7_eta ;
  double v_Jet_8_eta ;

  double v_Jet_1_phi ;
  double v_Jet_2_phi ;
  double v_Jet_3_phi ;
  double v_Jet_4_phi ;
  double v_Jet_5_phi ;
  double v_Jet_6_phi ;
  double v_Jet_7_phi ;
  double v_Jet_8_phi ;

  double v_numEle ;
  double v_numMu ;
  double v_numJets ;
  double v_totNumJets ;

  double v_Jet_1_DR ;
  double v_Jet_2_DR ;
  double v_Jet_3_DR ;
  double v_Jet_4_DR ;
  double v_Jet_5_DR ;
  double v_Jet_6_DR ;
  double v_Jet_7_DR ;
  double v_Jet_8_DR ;


    
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

int g_numJet ;

int g_numEvents ;


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
 
 g_numJet = subPSetInput.getUntrackedParameter<int> ("g_numJet",6) ; 
 g_numEvents = subPSetInput.getUntrackedParameter<int> ("g_numEvents",-1) ; 
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
 
 plots.v_Jet_1_eta = -99;
 plots.v_Jet_2_eta = -99;
 plots.v_Jet_3_eta = -99;
 plots.v_Jet_4_eta = -99;
 plots.v_Jet_5_eta = -99;
 plots.v_Jet_6_eta = -99;
 
 plots.v_Jet_1_phi = -99;
 plots.v_Jet_2_phi = -99;
 plots.v_Jet_3_phi = -99;
 plots.v_Jet_4_phi = -99;
 plots.v_Jet_5_phi = -99;
 plots.v_Jet_6_phi = -99;
 
 plots.v_Jet_1_DR = -99;
 plots.v_Jet_2_DR = -99;
 plots.v_Jet_3_DR = -99;
 plots.v_Jet_4_DR = -99;
 plots.v_Jet_5_DR = -99;
 plots.v_Jet_6_DR = -99;
 
 plots.v_numEle = -99;
 plots.v_numMu = -99;
 plots.v_numJets = -99;
 plots.v_totNumJets = -99;
  
 
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
//  std::cerr << " --- nentries = " << nentries << std::endl;
//  nentries = std::min(10000,nentries);
//  nentries = 10000;
 
 if (g_numEvents!= -1) nentries = std::min(g_numEvents,nentries);

 std::cerr << " --- nentries = " << nentries << std::endl;
 
 for (int evt = 0 ; evt < nentries ; ++evt)
 {
  if (!(evt%1000)) std::cerr << " --- evt = " << evt << std::endl;
  
  plots.v_Jet_1_Pt = -99;
  plots.v_Jet_2_Pt = -99;
  plots.v_Jet_3_Pt = -99;
  plots.v_Jet_4_Pt = -99;
  plots.v_Jet_5_Pt = -99;
  plots.v_Jet_6_Pt = -99;
  plots.v_Jet_7_Pt = -99;
  plots.v_Jet_8_Pt = -99;
  
  plots.v_Jet_1_x = -99;
  plots.v_Jet_2_x = -99;
  plots.v_Jet_3_x = -99;
  plots.v_Jet_4_x = -99;
  plots.v_Jet_5_x = -99;
  plots.v_Jet_6_x = -99;
  plots.v_Jet_7_x = -99;
  plots.v_Jet_8_x = -99;

  plots.v_Jet_1_y = -99;
  plots.v_Jet_2_y = -99;
  plots.v_Jet_3_y = -99;
  plots.v_Jet_4_y = -99;
  plots.v_Jet_5_y = -99;
  plots.v_Jet_6_y = -99;
  plots.v_Jet_7_y = -99;
  plots.v_Jet_8_y = -99;

  plots.v_Jet_1_z = -99;
  plots.v_Jet_2_z = -99;
  plots.v_Jet_3_z = -99;
  plots.v_Jet_4_z = -99;
  plots.v_Jet_5_z = -99;
  plots.v_Jet_6_z = -99;
  plots.v_Jet_7_z = -99;
  plots.v_Jet_8_z = -99;

  plots.v_Jet_1_e = -99;
  plots.v_Jet_2_e = -99;
  plots.v_Jet_3_e = -99;
  plots.v_Jet_4_e = -99;
  plots.v_Jet_5_e = -99;
  plots.v_Jet_6_e = -99;
  plots.v_Jet_7_e = -99;
  plots.v_Jet_8_e = -99;
  
  plots.v_Jet_1_eta = -99;
  plots.v_Jet_2_eta = -99;
  plots.v_Jet_3_eta = -99;
  plots.v_Jet_4_eta = -99;
  plots.v_Jet_5_eta = -99;
  plots.v_Jet_6_eta = -99;
  plots.v_Jet_7_eta = -99;
  plots.v_Jet_8_eta = -99;
 
  plots.v_Jet_1_phi = -99;
  plots.v_Jet_2_phi = -99;
  plots.v_Jet_3_phi = -99;
  plots.v_Jet_4_phi = -99;
  plots.v_Jet_5_phi = -99;
  plots.v_Jet_6_phi = -99;
  plots.v_Jet_7_phi = -99;
  plots.v_Jet_8_phi = -99;

  plots.v_Jet_1_DR = -99;
  plots.v_Jet_2_DR = -99;
  plots.v_Jet_3_DR = -99;
  plots.v_Jet_4_DR = -99;
  plots.v_Jet_5_DR = -99;
  plots.v_Jet_6_DR = -99;
  plots.v_Jet_7_DR = -99;
  plots.v_Jet_8_DR = -99;
 
  plots.v_numEle = -99;
  plots.v_numMu = -99;
  plots.v_numJets = -99;
  plots.v_totNumJets = -99;
  
  tree->GetEntry (evt) ;
  
  tagJets -> Clear () ;  
  otherJets -> Clear () ;    
  
  
  
    //---- check if signal ----
  if (if_signal && (IdEvent!=123 && IdEvent!=124)) continue;
  plots.analyzed++;
  
  
  //---- MC data ----
  std::vector<TLorentzVector*> MCJets ;
  
  TLorentzVector* MCJets_temp[6] ;
  int counter = 0;
  
  if (if_signal && (IdEvent==123 || IdEvent==124)){
   for(int ii=0; ii<9; ii++){
//     if (ii==0 || ii==1){
//     if (ii!=0 && ii!=1 && ii!=2 && ii!=3 && ii!=6){
    if (ii!=2 && ii!=3 && ii!=6){
     TParticle* myparticle = (TParticle*) HiggsParticle->At(ii);
//      std::cerr << "pdg = " << ii << " = " << myparticle->GetPdgCode() << std::endl;
     MCJets_temp[counter] = new TLorentzVector;
     myparticle->Momentum(*(MCJets_temp[counter]));
     MCJets.push_back((MCJets_temp[counter]));
     counter++;
    }
   }
  }
  

  
   //---- find Tagging Jets ----
 
  double m_jetPtMin = 15.;
  double m_jetEtaMax = 5.;
  double m_jetDEtaMin = -1;
  double m_jetMjjMin = -1;
 
  

  std::vector<myJet> goodJets ;

//   std::cerr << std::endl << std::endl << std::endl << std::endl << std::endl;
  
  for (int l=0; l<otherJets_temp->GetEntries (); l++ ){
   TLorentzVector* jet_temp = (TLorentzVector*) otherJets_temp->At(l);
   if (jet_temp->Pt()<m_jetPtMin) continue;
   //---- Eta max threshold ----
   if (jet_temp->Eta()>m_jetEtaMax) continue;
   //---- pt min threshold ----
   myJet dummy (jet_temp, 0, 0) ;
   goodJets.push_back (dummy) ;
  }
  
//   for (int gg=0; gg<goodJets.size(); gg++ ) std::cerr << " goodJets[" << gg << "] = " << &(goodJets.at(gg)) << std::endl;
  
  sort (goodJets.rbegin (), goodJets.rend (), lessThan ()) ;
  
  
  std::vector<std::pair<TLorentzVector*,TLorentzVector*> > Vect_PairQuark_RecoJet;
  std::vector<double> Map2D_PairQuark_RecoJet;

  int counter_map = 0;
    
  for (int rr=0; rr<std::min(g_numJet,static_cast<int>(goodJets.size())); rr++ ){ //--- loop over recoJets ----
   for (int k=0; k<MCJets.size(); k++ ){ //--- loop over quarks ----
    TLorentzVector* quark_temp = MCJets.at(k);
    TLorentzVector* jet_temp = goodJets.at(rr).m_kine;
    double DR = ROOT::Math::VectorUtil::DeltaR(quark_temp->BoostVector(),jet_temp->BoostVector());
    Map2D_PairQuark_RecoJet.push_back(DR);
    counter_map++;
   } //--- end loop over recoJet ----
  } //--- end loop over quarks ----

  

  int selected_pair_jet[6] ;
  int selected_pair_quark[6] ;
  for (int jj=0; jj<MCJets.size(); jj++ ){  
   selected_pair_jet[jj] = -1;
   selected_pair_quark[jj] = -1;
  }
  
  for (int jj=0; jj<MCJets.size(); jj++ ){  
   double DR_min = 1000;
   counter_map = 0;
   int temp_selected_pair_jet = -1;
   int temp_selected_pair_quark = -1;
   for (int rr=0; rr<std::min(g_numJet,static_cast<int>(goodJets.size())); rr++ ){ //--- loop over recoJets ----
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
  
  
  for (int rr=0; rr<std::min(g_numJet,static_cast<int>(goodJets.size())); rr++ ){ //--- loop over recoJets ----
   for (int k=0; k<MCJets.size(); k++ ){ //--- loop over quarks ----
    bool used_one = false;
    for (int qq=0; qq<MCJets.size(); qq++) {
     if ((selected_pair_jet[qq] == rr) && (selected_pair_quark[qq] == k)) used_one = true;
    }
    if (used_one){
     TLorentzVector* quark_temp = MCJets.at(k);
     TLorentzVector* jet_temp = goodJets.at(rr).m_kine;
     std::pair<TLorentzVector*,TLorentzVector*> PairQuark_RecoJet(quark_temp,jet_temp);
     Vect_PairQuark_RecoJet.push_back(PairQuark_RecoJet);
    }
   } //--- end loop over recoJet ----
  } //--- end loop over quarks ----

  
    
    
  for (int iJet=0; iJet<std::min(g_numJet,static_cast<int>(goodJets.size())); iJet++){
   
   double minDR = -1000;
   double eta_reco_temp = goodJets.at (iJet).m_kine->Eta () ;
   double phi_reco_temp = goodJets.at (iJet).m_kine->Phi () ;  
    
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
    plots.v_Jet_1_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_1_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_1_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_1_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_1_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_1_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_1_DR = minDR;
   }
   if (iJet==1) {
    plots.v_Jet_2_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_2_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_2_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_2_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_2_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_2_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_2_DR = minDR;
   }
   if (iJet==2) {
    plots.v_Jet_3_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_3_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_3_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_3_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_3_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_3_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_3_DR = minDR;
   }
   if (iJet==3) {
    plots.v_Jet_4_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_4_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_4_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_4_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_4_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_4_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_4_DR = minDR;
   }
   if (iJet==4) {
    plots.v_Jet_5_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_5_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_5_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_5_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_5_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_5_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_5_DR = minDR;
   }
   if (iJet==5) {
    plots.v_Jet_6_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_6_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_6_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_6_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_6_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_6_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_6_DR = minDR;
   }
   if (iJet==6) {
    plots.v_Jet_7_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_7_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_7_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_7_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_7_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_7_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_7_DR = minDR;
   }
   if (iJet==7) {
    plots.v_Jet_8_Pt = goodJets.at (iJet).m_kine->Pt () ;
    plots.v_Jet_8_x = goodJets.at (iJet).m_kine->X () ;
    plots.v_Jet_8_y = goodJets.at (iJet).m_kine->Y () ;
    plots.v_Jet_8_z = goodJets.at (iJet).m_kine->Z () ;
    plots.v_Jet_8_eta = goodJets.at (iJet).m_kine->Eta () ;
    plots.v_Jet_8_phi = goodJets.at (iJet).m_kine->Phi () ;
    plots.v_Jet_8_DR = minDR;
   }
  }  
  
  
  int numJets = goodJets.size();
  plots.v_numJets = numJets;
  plots.v_totNumJets = static_cast<double>(otherJets_temp->GetEntries ());
 
  
  
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
  if (numJets >=6 ) plots.passedJetAndLepNumberSelections++;
  

  for (int hh=0; hh<counter; hh++) delete MCJets_temp[hh];
  
 } //PG loop over the events

//  std::cerr << "---- Finishes ----" << std::endl;
 
 g_OutputFile->cd(0);
 plots.m_efficiency->Fill();
 plots.m_efficiency->Write();
 plots.m_tree_selections->Write();

//  std::cerr << "---- Written ----" << std::endl;
 
 delete otherJets_temp ;
 delete tagJets  ;  
 delete otherJets  ;
 delete electrons  ;
 delete muons  ;    
 delete MET  ;      
 delete tracks  ;   

//  std::cerr << "---- Deleted ----" << std::endl;
 
 return 0;
  
}

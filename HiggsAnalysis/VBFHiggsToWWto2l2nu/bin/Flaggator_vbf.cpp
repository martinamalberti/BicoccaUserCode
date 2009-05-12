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

TFile g_OutputFile("/tmp/amassiro/PLOT/out_Flag_All_8May09_1100_vbf.root","recreate");
  

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
   TString m_tree_selections_Name = m_name + "_m_tree_selections" ;
   
   m_tree_selections = new TTree(m_tree_selections_Name,m_tree_selections_Name);
     
   m_tree_selections->Branch("v_hardTAGPt",&v_hardTAGPt,"v_hardTAGPt/D");
   m_tree_selections->Branch("v_softTAGPt",&v_softTAGPt,"v_softTAGPt/D");
   m_tree_selections->Branch("v_TAGDProdEta",&v_TAGDProdEta,"v_TAGDProdEta/D");
   m_tree_selections->Branch("v_TAGDeta",&v_TAGDeta,"v_TAGDeta/D");
   m_tree_selections->Branch("v_TAGMinv",&v_TAGMinv,"v_TAGMinv/D");
   
   m_tree_selections->Branch("v_ojets",&v_ojets,"v_ojets/D");   
   m_tree_selections->Branch("v_ojetsCJV",&v_ojetsCJV,"v_ojetsCJV/D");   
   m_tree_selections->Branch("v_ojetsRegionalCJV",&v_ojetsRegionalCJV,"v_ojetsRegionalCJV/D");   
   
   m_tree_selections->Branch("v_ojetsZepp_01",&v_ojetsZepp_01,"v_ojetsZepp_01/D");   
   m_tree_selections->Branch("v_ojetsZepp_02",&v_ojetsZepp_02,"v_ojetsZepp_02/D");   
   m_tree_selections->Branch("v_ojetsZepp_03",&v_ojetsZepp_03,"v_ojetsZepp_03/D");   
   m_tree_selections->Branch("v_ojetsZepp_04",&v_ojetsZepp_04,"v_ojetsZepp_04/D");   
   m_tree_selections->Branch("v_ojetsZepp_05",&v_ojetsZepp_05,"v_ojetsZepp_05/D");   
   m_tree_selections->Branch("v_ojetsZepp_06",&v_ojetsZepp_06,"v_ojetsZepp_06/D");   
   m_tree_selections->Branch("v_ojetsZepp_07",&v_ojetsZepp_07,"v_ojetsZepp_07/D");   
   m_tree_selections->Branch("v_ojetsZepp_08",&v_ojetsZepp_08,"v_ojetsZepp_08/D");   
   m_tree_selections->Branch("v_ojetsZepp_09",&v_ojetsZepp_09,"v_ojetsZepp_09/D");   
   m_tree_selections->Branch("v_ojetsZepp_10",&v_ojetsZepp_10,"v_ojetsZepp_10/D");   
   m_tree_selections->Branch("v_ojetsZepp_11",&v_ojetsZepp_11,"v_ojetsZepp_11/D");   
   m_tree_selections->Branch("v_ojetsZepp_12",&v_ojetsZepp_12,"v_ojetsZepp_12/D");   
   m_tree_selections->Branch("v_ojetsZepp_13",&v_ojetsZepp_13,"v_ojetsZepp_13/D");   
   m_tree_selections->Branch("v_ojetsZepp_14",&v_ojetsZepp_14,"v_ojetsZepp_14/D");   

   
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
  double v_hardTAGPt ;
  double v_softTAGPt ;
  double v_TAGDProdEta ;
  double v_TAGDeta ;
  double v_TAGMinv ;
  double v_numLep ;
  double v_numEle ;
  double v_numMu ;
  double v_LepLep ;
  double v_hardLEPPt ;
  double v_softLEPPt ;
  double v_LEPDPhi ;
  double v_LEPMinv ;
  double v_MET ;

  double v_ojets ;
  double v_ojetsCJV ;
  double v_ojetsRegionalCJV ;
 
  double v_ojetsZepp_01 ;
  double v_ojetsZepp_02 ;
  double v_ojetsZepp_03 ;
  double v_ojetsZepp_04 ;
  double v_ojetsZepp_05 ;
  double v_ojetsZepp_06 ;
  double v_ojetsZepp_07 ;
  double v_ojetsZepp_08 ;
  double v_ojetsZepp_09 ;
  double v_ojetsZepp_10 ;
  double v_ojetsZepp_11 ;
  double v_ojetsZepp_12 ;
  double v_ojetsZepp_13 ;
  double v_ojetsZepp_14 ;

} ;


//  ------------------------------------------------------------


double deltaPhi (double phi1, double phi2) ;
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

 
//  char *samples[100]= {
//   "H160_WW_2l_redigi",
//   "ZeeJet_Pt0to15",
//   "ZeeJet_Pt120to170",
//   "ZeeJet_Pt170to230",
//   "ZeeJet_Pt20to30",
//   "ZeeJet_Pt230to300",
//   "ZeeJet_Pt300toInf",
//   "ZeeJet_Pt30to50",
//   "ZeeJet_Pt50to80",
//   "TauolaTTbar_Summer08_IDEAL_V9_v2"
//  };

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


//!PG main function
int 
  selector (TChain * tree, histos & plots, int if_signal)
{
 
 plots.v_hardTAGPt = -99;
 plots.v_softTAGPt = -99;
 plots.v_TAGDProdEta = -99;
 plots.v_TAGDeta = -99;
 plots.v_TAGMinv = -99;
 plots.v_LepLep = -99;
 plots.v_hardLEPPt = -99;
 plots.v_softLEPPt = -99;
 plots.v_LEPDPhi = -99;
 plots.v_LEPMinv = -99;
 plots.v_MET = -99;
 
 plots.v_ojets = -99 ;
 plots.v_ojetsCJV = -99 ;
 plots.v_ojetsRegionalCJV = -99 ;
 
 plots.v_ojetsZepp_01 = -99 ;
 plots.v_ojetsZepp_02 = -99 ;
 plots.v_ojetsZepp_03 = -99 ;
 plots.v_ojetsZepp_04 = -99 ;
 plots.v_ojetsZepp_05 = -99 ;
 plots.v_ojetsZepp_06 = -99 ;
 plots.v_ojetsZepp_07 = -99 ;
 plots.v_ojetsZepp_08 = -99 ;
 plots.v_ojetsZepp_09 = -99 ;
 plots.v_ojetsZepp_10 = -99 ;
 plots.v_ojetsZepp_11 = -99 ;
 plots.v_ojetsZepp_12 = -99 ;
 plots.v_ojetsZepp_13 = -99 ;
 plots.v_ojetsZepp_14 = -99 ;
 
 
 
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

  tree->GetEntry (evt) ;
  
  tagJets -> Clear () ;  
  otherJets -> Clear () ;    
  
  
  
    //---- check if signal ----
  if (if_signal && (IdEvent!=123 && IdEvent!=124)) continue;
  plots.analyzed++;
  
  
  
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

  plots.v_hardTAGPt = primoTAG->Pt () ;
  //---- AM 12 pt min highest pt jet
  
  plots.v_softTAGPt = secondoTAG->Pt () ;
  //---- AM 13 pt min lowest pt jet
      
  plots.v_TAGDProdEta = primoTAG->Eta () * secondoTAG->Eta () ;
  //---- AM 14 eta_1 * eta_2 of the jets
  
  plots.v_TAGDeta = fabs (primoTAG->Eta () - secondoTAG->Eta ()) ; 
  //---- AM 15 DEta_min between jets
  
  
  TLorentzVector sumTAG = *primoTAG + *secondoTAG ;
  plots.v_TAGMinv = sumTAG.M () ;
  //---- AM 16 MInv_min jets
  

      //PG JET VETO
      //PG --------

      //PG loop over other jets
  double etaMean = 0.5*(primoTAG->Eta () + secondoTAG->Eta ()); 
  
  double ojets  = 0 ;
  double ojetsCJV  = 0 ;
  double ojetsRegionalCJV  = 0 ;
 
  double ojetsZepp_01  = 0 ;
  double ojetsZepp_02  = 0 ;
  double ojetsZepp_03  = 0 ;
  double ojetsZepp_04  = 0 ;
  double ojetsZepp_05  = 0 ;
  double ojetsZepp_06  = 0 ;
  double ojetsZepp_07  = 0 ;
  double ojetsZepp_08  = 0 ;
  double ojetsZepp_09  = 0 ;
  double ojetsZepp_10  = 0 ;
  double ojetsZepp_11  = 0 ;
  double ojetsZepp_12  = 0 ;
  double ojetsZepp_13  = 0 ;
  double ojetsZepp_14  = 0 ;

   
  for (int ojetIt = 0 ; ojetIt < otherJets->GetEntries () ; ++ojetIt)
  {
   if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Pt () < g_ojetPtMin) continue ;
   
   ojets += 1;
   
   double zepp_temp = fabs(((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () - etaMean) / fabs (primoTAG->Eta () - secondoTAG->Eta ());
   
   if (zepp_temp < 0.1) ojetsZepp_01 += 1;
   if (zepp_temp < 0.2) ojetsZepp_02 += 1;
   if (zepp_temp < 0.3) ojetsZepp_03 += 1;
   if (zepp_temp < 0.4) ojetsZepp_04 += 1;
   if (zepp_temp < 0.5) ojetsZepp_05 += 1;
   if (zepp_temp < 0.6) ojetsZepp_06 += 1;
   if (zepp_temp < 0.7) ojetsZepp_07 += 1;
   if (zepp_temp < 0.8) ojetsZepp_08 += 1;
   if (zepp_temp < 0.9) ojetsZepp_09 += 1;
   if (zepp_temp < 1.0) ojetsZepp_10 += 1;
   if (zepp_temp < 1.1) ojetsZepp_11 += 1;
   if (zepp_temp < 1.2) ojetsZepp_12 += 1;
   if (zepp_temp < 1.3) ojetsZepp_13 += 1;
   if (zepp_temp < 1.4) ojetsZepp_14 += 1;

   if ( ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () < eta_min &&
          ((TLorentzVector*) (otherJets->At (ojetIt)))->Eta () < eta_max) ojetsCJV += 1;
   
   if ( fabs(((TLorentzVector*) (otherJets->At (ojetIt)))->Eta ()) < g_ojetEtaMax) ojetsRegionalCJV += 1;
  
  } //PG end loop over other jets
  
  plots.v_ojets = ojets ;
  plots.v_ojetsCJV = ojetsCJV ;
  plots.v_ojetsRegionalCJV = ojetsRegionalCJV;
 
  plots.v_ojetsZepp_01 = ojetsZepp_01 ;
  plots.v_ojetsZepp_02 = ojetsZepp_02 ;
  plots.v_ojetsZepp_03 = ojetsZepp_03 ;
  plots.v_ojetsZepp_04 = ojetsZepp_04 ;
  plots.v_ojetsZepp_05 = ojetsZepp_05 ;
  plots.v_ojetsZepp_06 = ojetsZepp_06 ;
  plots.v_ojetsZepp_07 = ojetsZepp_07 ;
  plots.v_ojetsZepp_08 = ojetsZepp_08 ;
  plots.v_ojetsZepp_09 = ojetsZepp_09 ;
  plots.v_ojetsZepp_10 = ojetsZepp_10 ;
  plots.v_ojetsZepp_11 = ojetsZepp_11 ;
  plots.v_ojetsZepp_12 = ojetsZepp_12 ;
  plots.v_ojetsZepp_13 = ojetsZepp_13 ;
  plots.v_ojetsZepp_14 = ojetsZepp_14 ;
  
  //---- AM 17 Jet Veto


//   std::cerr << "--- preambolo leptoni " << std::endl;           
  
  std::vector<lepton> leptons ;
            
      //PG pour electrons into leptons collection
      //PG ---------------------------------------

      //PG loop over electrons
  for (int iele = 0; iele < electrons->GetEntries () ; ++iele)
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

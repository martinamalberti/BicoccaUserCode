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

TFile g_OutputFile("/tmp/amassiro/PLOT/out_Flag_All_17May09_1530_lept.root","recreate");
  

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
   m_tree_selections->Branch("v_numLep",&v_numLep,"v_numLep/D");
   m_tree_selections->Branch("v_numEle",&v_numEle,"v_numEle/D");
   m_tree_selections->Branch("v_numMu",&v_numMu,"v_numMu/D");
   m_tree_selections->Branch("v_LepLep",&v_LepLep,"v_LepLep/D");
   m_tree_selections->Branch("v_hardLEPPt",&v_hardLEPPt,"v_hardLEPPt/D");
   m_tree_selections->Branch("v_softLEPPt",&v_softLEPPt,"v_softLEPPt/D");  
   m_tree_selections->Branch("v_LEPDPhi",&v_LEPDPhi,"v_LEPDPhi/D");
   m_tree_selections->Branch("v_LEPDEta",&v_LEPDEta,"v_LEPDEta/D");
   m_tree_selections->Branch("v_LEPDR",&v_LEPDR,"v_LEPDR/D");
   m_tree_selections->Branch("v_LEPMinv",&v_LEPMinv,"v_LEPMinv/D");
   m_tree_selections->Branch("v_LEPProdCharge",&v_LEPProdCharge,"v_LEPProdCharge/D");  
   m_tree_selections->Branch("v_hardLEPCharge",&v_hardLEPCharge,"v_hardLEPCharge/D");  
   m_tree_selections->Branch("v_softLEPCharge",&v_softLEPCharge,"v_softLEPCharge/D");  
   m_tree_selections->Branch("v_MET",&v_MET,"v_MET/D");   
   
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

   
   m_tree_selections->Branch("v_decay_Channel_e",&v_decay_Channel_e,"v_decay_Channel_e/I");
   m_tree_selections->Branch("v_decay_Channel_mu",&v_decay_Channel_mu,"v_decay_Channel_mu/I");
   m_tree_selections->Branch("v_decay_Channel_tau",&v_decay_Channel_tau,"v_decay_Channel_tau/I");
   
   TString m_efficiency_Name = m_name + "_m_efficiency" ;
   m_efficiency = new TTree(m_efficiency_Name,m_efficiency_Name);
   m_efficiency->Branch("analyzed",&analyzed,"analyzed/I");
   
   m_efficiency->Branch("analyzed_ee",&analyzed_ee,"analyzed_ee/I");
   m_efficiency->Branch("analyzed_mumu",&analyzed_mumu,"analyzed_mumu/I");
   m_efficiency->Branch("analyzed_tautau",&analyzed_tautau,"analyzed_tautau/I");
   m_efficiency->Branch("analyzed_emu",&analyzed_emu,"analyzed_emu/I");
   m_efficiency->Branch("analyzed_etau",&analyzed_etau,"analyzed_etau/I");
   m_efficiency->Branch("analyzed_mutau",&analyzed_mutau,"analyzed_mutau/I");
   
      
   m_efficiency->Branch("passedJetAndLepNumberSelections",&passedJetAndLepNumberSelections,"passedJetAndLepNumberSelections/I");
   
   m_efficiency->Branch("passedJetAndLepNumberSelections_ee",&passedJetAndLepNumberSelections_ee,"passedJetAndLepNumberSelections_ee/I");
   m_efficiency->Branch("passedJetAndLepNumberSelections_mumu",&passedJetAndLepNumberSelections_mumu,"passedJetAndLepNumberSelections_mumu/I");
   m_efficiency->Branch("passedJetAndLepNumberSelections_tautau",&passedJetAndLepNumberSelections_tautau,"passedJetAndLepNumberSelections_tautau/I");
   m_efficiency->Branch("passedJetAndLepNumberSelections_emu",&passedJetAndLepNumberSelections_emu,"passedJetAndLepNumberSelections_emu/I");
   m_efficiency->Branch("passedJetAndLepNumberSelections_etau",&passedJetAndLepNumberSelections_etau,"passedJetAndLepNumberSelections_etau/I");
   m_efficiency->Branch("passedJetAndLepNumberSelections_mutau",&passedJetAndLepNumberSelections_mutau,"passedJetAndLepNumberSelections_mutau/I");
   
   
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
  int analyzed_ee;
  int analyzed_mumu;
  int analyzed_tautau;
  int analyzed_emu;
  int analyzed_etau;
  int analyzed_mutau;
  
  int passedJetAndLepNumberSelections;
  int passedJetAndLepNumberSelections_ee;
  int passedJetAndLepNumberSelections_mumu;
  int passedJetAndLepNumberSelections_tautau;
  int passedJetAndLepNumberSelections_emu;
  int passedJetAndLepNumberSelections_etau;
  int passedJetAndLepNumberSelections_mutau;
    

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
  double v_LEPDEta ;
  double v_LEPDR ;
  double v_LEPMinv ;
  double v_LEPProdCharge ;
  double v_hardLEPCharge ;
  double v_softLEPCharge ;
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
  
  //! ---- MC ----
  int v_decay_Channel_e;
  int v_decay_Channel_mu;
  int v_decay_Channel_tau;

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
 
 plots.v_hardTAGPt = -99;
 plots.v_softTAGPt = -99;
 plots.v_TAGDProdEta = -99;
 plots.v_TAGDeta = -99;
 plots.v_TAGMinv = -99;
 plots.v_LepLep = -99;
 plots.v_hardLEPPt = -99;
 plots.v_softLEPPt = -99;
 plots.v_LEPDPhi = -99;
 plots.v_LEPDEta = -99;
 plots.v_LEPDR = -99;
 plots.v_LEPMinv = -99;
 plots.v_LEPProdCharge = -99;
 plots.v_hardLEPCharge = -99;
 plots.v_softLEPCharge = -99;
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
 
 plots.v_decay_Channel_e = -99 ;
 plots.v_decay_Channel_mu = -99 ;
 plots.v_decay_Channel_tau = -99 ;
 
 
 TClonesArray * genParticles = new TClonesArray ("TParticle") ;
 tree->SetBranchAddress ("genParticles", &genParticles) ;
 
 
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
 int EleCharge[30];
 tree->SetBranchAddress ("nEle", &nEle) ;
 tree->SetBranchAddress ("EleId",EleId ) ;
 tree->SetBranchAddress ("IsolEleSumPt_VBF",IsolEleSumPt_VBF ) ;
 tree->SetBranchAddress ("EleCharge",EleCharge ) ;
 
 float IsolMuTr[100];
 int nMu ;
 int MuCharge[30];
 tree->SetBranchAddress ("nMu", &nMu) ;
 tree->SetBranchAddress ("IsolMuTr",IsolMuTr ) ;
 tree->SetBranchAddress ("MuCharge", MuCharge) ;
 

 int IdEvent;
 tree->SetBranchAddress ("IdEvent", &IdEvent) ;
 
 
 int nentries = (int) tree->GetEntries () ;

 
 plots.passedJetAndLepNumberSelections = 0;
 plots.analyzed = 0;
 
 plots.analyzed_ee = 0;
 plots.analyzed_mumu = 0;
 plots.analyzed_tautau = 0;
 plots.analyzed_emu = 0;
 plots.analyzed_etau = 0;
 plots.analyzed_mutau = 0;
  
 plots.passedJetAndLepNumberSelections_ee = 0;
 plots.passedJetAndLepNumberSelections_mumu = 0;
 plots.passedJetAndLepNumberSelections_tautau = 0;
 plots.passedJetAndLepNumberSelections_emu = 0;
 plots.passedJetAndLepNumberSelections_etau = 0;
 plots.passedJetAndLepNumberSelections_mutau = 0;

 
 
 //PG loop over the events
 for (int evt = 0 ; evt < nentries ; ++evt)
 {

  tree->GetEntry (evt) ;
  
  tagJets -> Clear () ;  
  otherJets -> Clear () ;    
  
  
    //---- check if signal ----
  if (if_signal && (IdEvent!=123 && IdEvent!=124)) continue;
  plots.analyzed++;
  
   //!---- MC ----

  if (IdEvent==123 || IdEvent==124) { //---- VBF event ----
   plots.v_decay_Channel_e = 0;
   plots.v_decay_Channel_mu = 0;
   plots.v_decay_Channel_tau = 0;
   for (int iGen = 0; iGen < genParticles->GetEntries() ; ++iGen){
    TParticle* myparticle = (TParticle*) genParticles->At(iGen);
    if (abs(myparticle->GetPdgCode()) == 24) { //---- W
     Int_t mother1 = 0;
     mother1 = myparticle->GetMother(0);
     if (mother1 == 25) { //---- mother is higgs ----
      for (int iDaughter = 0; iDaughter<2; iDaughter++){
       if (abs(myparticle->GetDaughter(iDaughter)) == 11) {//---- W -> e
        plots.v_decay_Channel_e++;
       }
       if (abs(myparticle->GetDaughter(iDaughter)) == 13) {//---- W -> mu    
        plots.v_decay_Channel_mu++;
       }
       if (abs(myparticle->GetDaughter(iDaughter)) == 15) {//---- W -> tau
        plots.v_decay_Channel_tau++;
       }       
      }
     }
    }
   }
  }

  if (plots.v_decay_Channel_e == 2) plots.analyzed_ee++;
  if (plots.v_decay_Channel_mu == 2) plots.analyzed_mumu++;
  if (plots.v_decay_Channel_tau == 2) plots.analyzed_tautau++;
  if (plots.v_decay_Channel_e == 1 && plots.v_decay_Channel_mu == 1) plots.analyzed_emu++;
  if (plots.v_decay_Channel_e == 1 && plots.v_decay_Channel_tau == 1) plots.analyzed_etau++;
  if (plots.v_decay_Channel_mu == 1 && plots.v_decay_Channel_tau == 1) plots.analyzed_mutau++;
  
  
  
  
  
  
  
  int cutId = 0 ;

  plots.increase (cutId++) ; //AM 0 -> total number of events


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

//PG this check is not necessary
//PG      if (leptons.size () < 2) continue ;

  
  
//   std::cerr << "--- inizia leptoni " << std::endl;
  
  
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

  double first_lepton_charge = 0;
  double second_lepton_charge = 0;
  
  int lepton_counter = 0;
  int electron_counter = 0;
  int muon_counter = 0;
    
      //PG find the first lepton
  int ilep = 0 ;
  for ( ; ilep < leptons.size () ; ++ilep)
  {
   if (leptons.at (ilep).m_flav == 0) //PG electron
   {
               //PG iso check
    bool eleIso = (IsolEleSumPt_VBF[leptons.at (ilep).m_index] /  
      leptons.at (ilep).m_kine->Pt () ) < g_IsoElectron ; // 0.2 per il momento
    if (g_ISO1[0] == 1 && eleIso != 1) continue;
              
              //PG eleID check
    int eleID = EleId[leptons.at (ilep).m_index] ;
    if      (g_ID1 == 100 && (eleID/100) != 1) continue;
    else if (g_ID1 == 10  && ((eleID%100)/10) != 1) continue;
    else if (g_ID1 == 1   && (eleID%10) != 1) continue;
    first_lepton_charge = EleCharge[leptons.at (ilep).m_index];
   }
   else //PG muon
   {
              //PG iso check
    bool muIso = (IsolMuTr[leptons.at (ilep).m_index] /  
      leptons.at (ilep).m_kine->Pt () ) < g_IsoMuon ; 
    if (g_ISO1[1] == 1 && muIso != 1) continue;
    first_lepton_charge = MuCharge[leptons.at (ilep).m_index];
   }  
   primoLEP = leptons[ilep] ;
   lepton_counter++;
   if (leptons.at (ilep).m_flav == 0) electron_counter++;
   else muon_counter++;
   break ;
   } //PG find the first lepton

  
      //PG find the second lepton
  bool flag_secondoLEP = false;
  for (++ilep ; ilep < leptons.size () ; ++ilep)
  {
   if (leptons.at (ilep).m_flav == 0) //PG electron
   {
               //PG iso check
    bool eleIso = (IsolEleSumPt_VBF[leptons.at (ilep).m_index] /  
      leptons.at (ilep).m_kine->Pt () ) < g_IsoElectron ; // 0.2 per il momento
    if (g_ISO2[0] == 1 && eleIso != 1) continue;
              
              //PG eleID check
    int eleID = EleId[leptons.at (ilep).m_index] ;
    if      (g_ID2 == 100 && (eleID/100) != 1) continue;
    else if (g_ID2 == 10  && ((eleID%100)/10) != 1) continue;
    else if (g_ID2 == 1   && (eleID%10) != 1) continue;
    second_lepton_charge = EleCharge[leptons.at (ilep).m_index];
   }
   else //PG muon
   {
              //PG iso check
    bool muIso = (IsolMuTr[leptons.at (ilep).m_index] /  
      leptons.at (ilep).m_kine->Pt () ) < g_IsoMuon ; 
    if (g_ISO2[1] == 1 && muIso != 1) continue;
    second_lepton_charge = MuCharge[leptons.at (ilep).m_index];
   }  
   if (!flag_secondoLEP) {
    secondoLEP = leptons[ilep] ;
    flag_secondoLEP = true;
   }
   
   if (leptons.at (ilep).m_kine->Pt () > 0) {
    if (leptons.at (ilep).m_flav == 0) electron_counter++;
    else muon_counter++;
    lepton_counter++;
    }
   
  } //PG find the second lepton

 //---- AM 3 --- 2 leptons after Id      
 if (primoLEP.m_flav == -1 || secondoLEP.m_flav == -1) continue ;
  
 //---- AM 4 check for the two most transverse-energetic leptons have the right flavours
  
 plots.v_numLep = lepton_counter;
 plots.v_numEle = electron_counter;
 plots.v_numMu = muon_counter; 
 
 if (primoLEP.m_flav == 0 && secondoLEP.m_flav == 0) plots.v_LepLep = 0 ;
 if (primoLEP.m_flav == 1 && secondoLEP.m_flav == 1) plots.v_LepLep = 1 ;
 if (primoLEP.m_flav == 0 && secondoLEP.m_flav == 1) plots.v_LepLep = 2 ;
 if (primoLEP.m_flav == 1 && secondoLEP.m_flav == 0) plots.v_LepLep = 3 ;

      
      
 plots.v_hardLEPPt = primoLEP.m_kine->Pt () ; 
   //---- AM 5 pt_min of the most energetic lepton
  
 plots.v_softLEPPt = secondoLEP.m_kine->Pt () ;
   //---- AM 6 pt_min of the least energetic lepton
   
 plots.v_LEPDPhi = deltaPhi (primoLEP.m_kine->Phi (), secondoLEP.m_kine->Phi ()) ;
  //---- AM 7 Delta_phi_min between leptons

 plots.v_LEPDEta = deltaEta (primoLEP.m_kine->Eta (), secondoLEP.m_kine->Eta ()) ;
 plots.v_LEPDR = deltaR (primoLEP.m_kine->Phi (),primoLEP.m_kine->Eta (), secondoLEP.m_kine->Phi (), secondoLEP.m_kine->Eta ()) ;

 
   
  TLorentzVector sumLEP = *(primoLEP.m_kine) + *(secondoLEP.m_kine) ;
  plots.v_LEPMinv = sumLEP.M () ;
  //---- AM 9 MInv_min of leptons
  
  
  plots.v_LEPProdCharge = first_lepton_charge * second_lepton_charge ;
  plots.v_hardLEPCharge = first_lepton_charge ;
  plots.v_softLEPCharge = second_lepton_charge ;
      
  
      //PG MET
      //PG ---

//   std::cerr << "--- finito " << std::endl;
  
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
  
  plots.v_MET = met->Pt () ;
  
  
 
  

    
    
    
  //---- AM 11 Met_min ----------------> Met correction ?
  
//      if (((TLorentzVector*) (MET->At (0)))->Pt () < g_METMin) continue ; plots.increase (cutId++) ; //PG 10
      
  
  
  
  

      //PG Ztautau vetos
      //PG -------------
      
      //PG the two electrons should not be opposite to each other
//       
//   TVector2 primoLEPT (primoLEP.m_kine->X (), primoLEP.m_kine->Y ()) ;
//   TVector2 secondoLEPT (secondoLEP.m_kine->X (), secondoLEP.m_kine->Y ()) ;
//   TVector2 METT (met->X (), met->Y ()) ;
// 
//   double Sum = METT * primoLEPT + METT * secondoLEPT / (1 + primoLEPT * secondoLEPT) ;
//   double Dif = METT * primoLEPT - METT * secondoLEPT / (1 - primoLEPT * secondoLEPT) ;
//       
//   TVector2 METT1 = 0.5 * (Sum + Dif) * primoLEPT ;
//   TVector2 METT2 = 0.5 * (Sum - Dif) * secondoLEPT ;
//       
//   double ptNu1 = METT1.Mod () / cos (primoLEP.m_kine->Theta ()) ;
//   double ptNu2 = METT2.Mod () / cos (secondoLEP.m_kine->Theta ()) ;

  
  
  
  
  plots.m_tree_selections->Fill();
  plots.passedJetAndLepNumberSelections++;

  if (plots.v_decay_Channel_e == 2) plots.passedJetAndLepNumberSelections_ee++;
  if (plots.v_decay_Channel_mu == 2) plots.passedJetAndLepNumberSelections_mumu++;
  if (plots.v_decay_Channel_tau == 2) plots.passedJetAndLepNumberSelections_tautau++;
  if (plots.v_decay_Channel_e == 1 && plots.v_decay_Channel_mu == 1) plots.passedJetAndLepNumberSelections_emu++;
  if (plots.v_decay_Channel_e == 1 && plots.v_decay_Channel_tau == 1) plots.passedJetAndLepNumberSelections_etau++;
  if (plots.v_decay_Channel_mu == 1 && plots.v_decay_Channel_tau == 1) plots.passedJetAndLepNumberSelections_mutau++;
   
    
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

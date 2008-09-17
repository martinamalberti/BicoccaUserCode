#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <strstream>


//---- CMSSW includes
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
 

#include "DataFormats/Math/interface/LorentzVector.h"


//---- root includes
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TChain.h"
#include "TFile.h"

#define PI 3.14159265
#define TWOPI 6.28318530


double deltaPhi(double phi1,double phi2) {
 double deltaphi = fabs(phi1-phi2);  
 if (deltaphi > TWOPI) deltaphi -= TWOPI;  
 if (deltaphi > PI ) deltaphi = TWOPI - deltaphi;  
 return deltaphi; 
}



//! main program
int main (int argc, char** argv)
{
 
 //---- output File ---- where saving histograms ---- 
 bool flagOutput = false;
 std::string outputRootName = "outputHistos.root";
 std::string testName = "-o";
 
 if (argc>2){
  if (argv[1] ==  testName) {
   outputRootName = argv[2];
   flagOutput = true;
   std::cerr << " Name Output file = " << outputRootName << std::endl;
  }
 }
 if (argc>4){
  if (argv[3] ==  testName) {
   outputRootName = argv[4];
   flagOutput = true;
   std::cerr << " Name Output file = " << outputRootName << std::endl;
  }
 }
 
 
 //---- input File ---- where finding input parameters ---- 
  
 //---- Input Variables ----
 double R = 0.2; //---- Radius to match MC and Cluster
 double dEta_C_Cut = 0.05; //---- dEta to match MC and Cluster
 double dPhi_C_Cut = 0.1; //---- dPhi to match MC and Cluster
 
 
 double S4oS9C_Cut = 0.85; //---- S4oS9C_-Cluster cut ----
 double ptC_Cut = 1.;      //---- pt-Cluster cut ----
 double ptSum_Cut = 2.;    //---- pt-coppia cut ---- 
 
 double RMax_Cut = 0.2;            //---- RMax_Cut cut ---- isolation ----
 double DeltaEtaMax_Cut = 0.05;    //---- DeltaEtaMax_Cut ---- isolation ----
 double et_Cut = 1.;               //---- et_Cut ---- isolation ----
 double iso_Cut = 0.4;             //---- iso/pt-coppia cut ---- isolation ----
 
 double angle_Cut = 0.105; //---- ~6° ---- Angle between two SC ----
  
 double R_eta = 0.1; //---- summed cluster match with MC
 double dEta_eta = 0.05;
 double dPhi_eta = 0.1;
 double ptPh_Cut = 1.;      //---- pt-Photon cut ----
 
 int N_events = 10; //----numeber of events to analize -> -1 = all ----
 
 
 std::string inputName;
 bool flagInput = false;
 std::string testNameInput = "-i";
 if (argc>2){
  if (argv[1] ==  testNameInput) {
   inputName = argv[2];
   flagInput = true;
   std::cerr << " Name input file variables = " << inputName << std::endl;
  }
 }
 if (argc>4){
  if (argv[3] ==  testNameInput) {
   inputName = argv[4];
   flagInput = true;
   std::cerr << " Name input file variables = " << inputName << std::endl;
  }
 }
 if (flagInput){
  char buffer[1000];
  ifstream file(inputName.c_str());
  
  std::string variableNameR = "R";
  std::string variableNameS4oS9C_Cut = "S4oS9C_Cut";
  std::string variableNamePtC_Cut = "ptC_Cut";
  std::string variableNamePtSum_Cut = "ptSum_Cut";
  std::string variableNameMax_Cut = "RMax_Cut";
  std::string variableNameDeltaEtaMax_Cut = "DeltaEtaMax_Cut";
  std::string variableNameEt_Cut = "et_Cut";
  std::string variableNameIso_Cut = "iso_Cut";
  std::string variableNameAngle_Cut = "angle_Cut";
  std::string variableNameR_eta = "R_eta";
  std::string variableNameN_events = "N_events";
  std::string variableNameDEta_C_Cut = "dEta_C_Cut";
  std::string variableNameDPhi_C_Cut = "dPhi_C_Cut";
  std::string variableNameDEta_eta = "dEta_eta";
  std::string variableNameDPhi_eta = "dPhi_eta";
  std::string variableNamePtPh_Cut = "ptPh_Cut";
  
  std::cerr << " Reading  " << inputName << " ... " << std::endl;
  while (!file.eof()){
   file.getline (&buffer[0],1000);
   std::istrstream line(buffer);
   std::string variableName;
   line >> variableName;
   double variableValue;
   line >> variableValue;
   std::cerr << "  File ->  " << variableName << "  =  " << variableValue << std::endl;
   if (variableName == variableNameR) R = variableValue;
   if (variableName == variableNameS4oS9C_Cut) S4oS9C_Cut = variableValue;
   if (variableName == variableNamePtC_Cut) ptC_Cut = variableValue;
   if (variableName == variableNamePtSum_Cut) ptSum_Cut = variableValue;
   if (variableName == variableNameMax_Cut) RMax_Cut = variableValue;
   if (variableName == variableNameDeltaEtaMax_Cut) DeltaEtaMax_Cut = variableValue;
   if (variableName == variableNameEt_Cut) et_Cut = variableValue;
   if (variableName == variableNameIso_Cut) iso_Cut = variableValue;
   if (variableName == variableNameAngle_Cut) angle_Cut = variableValue;
   if (variableName == variableNameR_eta) R_eta = variableValue;
   if (variableName == variableNameN_events) N_events = variableValue;
   if (variableName == variableNameDEta_C_Cut) dEta_C_Cut = variableValue;
   if (variableName == variableNameDPhi_C_Cut) dPhi_C_Cut = variableValue;
   if (variableName == variableNameDEta_eta) dEta_eta = variableValue;
   if (variableName == variableNameDPhi_eta) dPhi_eta = variableValue;
   if (variableName == variableNamePtPh_Cut) ptPh_Cut = variableValue;
  }
 }
 
 std::string testHelp = "--help";
 if (argc==2){
  if (argv[1] == testHelp) {
   std::cout << "Help" << std::endl ;
   std::cout << " --help : display help" << std::endl ;
   std::cout << " -o : output root file name (eg histograms.root)" << std::endl ;
   std::cout << " -i : input file name with cuts values (eg Cuts.txt)" << std::endl ;
   std::cout << " name of input file : list name of input files ntuples" << std::endl ;     
   exit(1);
  }
 }
    
 if (argc < 2)
 {
  std::cerr << "ERROR : ntuple name missing" << std::endl ;
  exit (1) ;
 }

 
 
 TChain * chain = new TChain ("simplePhotonAnalyzer/tTreeUtilities") ;
 
 int numC_;
 int numCB_;
 int numCE_;
 std::vector<double> * etaC_;
 std::vector<double> * thetaC_;
 std::vector<double> * phiC_;
 std::vector<double> * S4oS9C_;
 std::vector<double> * S4C_;
 std::vector<double> * S9C_;
 std::vector<double> * S16C_;
 std::vector<double> * S25C_;
 std::vector<double> * pxC_;
 std::vector<double> * pyC_;
 std::vector<double> * pzC_;
 std::vector<double> * etC_;
 std::vector<int> * HitsC_;
 std::vector<double> * HitsEnergyC_;
  
 int numEta_;
 std::vector<int> * numPh_; //---- number of photons for each Eta ----
 std::vector<double> * thetaPh_;
 std::vector<double> * etaPh_;
 std::vector<double> * phiPh_;
 std::vector<double> * pxPh_;
 std::vector<double> * pyPh_;
 std::vector<double> * pzPh_;

 
 
 etaC_ = new std::vector<double>;
 thetaC_ = new std::vector<double>;
 phiC_ = new std::vector<double>;
 S4oS9C_ = new std::vector<double>;
 S4C_ = new std::vector<double>;
 S9C_ = new std::vector<double>;
 S16C_ = new std::vector<double>;
 S25C_ = new std::vector<double>;
 pxC_ = new std::vector<double>;
 pyC_ = new std::vector<double>;
 pzC_ = new std::vector<double>;
 etC_ = new std::vector<double>;
 HitsC_ = new std::vector<int>;
 HitsEnergyC_ = new std::vector<double>;
 
 numPh_ = new std::vector<int>;
 thetaPh_ = new std::vector<double>;
 etaPh_ = new std::vector<double>;
 phiPh_ = new std::vector<double>;
 pxPh_ = new std::vector<double>;
 pyPh_ = new std::vector<double>;
 pzPh_ = new std::vector<double>;
 
 
 std::cerr << "---- Tree initialization ---- " << std::endl; 

 //---- ---- Clusters ---- C = cluster ----
 chain->SetBranchAddress("numC_",&numC_);
 chain->SetBranchAddress("numCB_",&numCB_);
 chain->SetBranchAddress("numCE_",&numCE_);
 chain->SetBranchAddress("etaC_",&etaC_);
 chain->SetBranchAddress("thetaC_",&thetaC_);
 chain->SetBranchAddress("phiC_",&phiC_);
 chain->SetBranchAddress("S4oS9C_",&S4oS9C_);
 chain->SetBranchAddress("S4C_",&S4C_);
 chain->SetBranchAddress("S9C_",&S9C_);
 chain->SetBranchAddress("S16C_",&S16C_);
 chain->SetBranchAddress("S25C_",&S25C_);
 chain->SetBranchAddress("pxC_",&pxC_);
 chain->SetBranchAddress("pyC_",&pyC_);
 chain->SetBranchAddress("pzC_",&pzC_);
 chain->SetBranchAddress("etC_",&etC_);
 chain->SetBranchAddress("HitsC_",&HitsC_);
 chain->SetBranchAddress("HitsEnergyC_",&HitsEnergyC_); //---- non è presente per ora ----
 
 //---- ---- Photons from Eta ---- 
 chain->SetBranchAddress("numEta_",&numEta_);
 chain->SetBranchAddress("numPh_",&numPh_);
 chain->SetBranchAddress("thetaPh_",&thetaPh_);
 chain->SetBranchAddress("etaPh_",&etaPh_);
 chain->SetBranchAddress("phiPh_",&phiPh_);
 chain->SetBranchAddress("pxPh_",&pxPh_);
 chain->SetBranchAddress("pyPh_",&pyPh_);
 chain->SetBranchAddress("pzPh_",&pzPh_);
  
 
 
 std::cerr << "---- End Tree initialization ---- " << std::endl; 
 
 
 
 if (flagOutput && flagInput){
  chain->Add (argv[5]) ;
 }
 if ((flagOutput && !flagInput) || ((!flagOutput && flagInput))){
  chain->Add (argv[3]) ;
 }
 if (!flagOutput && !flagInput){
  chain->Add (argv[1]) ;
 }
 
 
 std::cerr << "---- End Chain initialization ---- " << std::endl; 
 
 
 
 //---- Histograms ----



//  TH1F hInvMassEtaC("hInvMassEtaC","Invariant mass Eta from Clusters",1000, 0., 1.);
//  TH2F hInvMassEtaC_numC("hInvMassEtaC_numC","Invariant mass Eta from Clusters versus number of cluster selected",1000, 0., 1.,100, 0., 100.);
//  TH1F hPhotonPt("hPhotonPt","Photon pt",10000, 0., 10.);
//  TH1F hClusterPt("hClusterPt","Single Cluster pt",30000, 0., 30.);

//  TH1F hPtSum("hPtSum","Summed cluster pt",40000, 0., 40.);
//  TH1F hInvMassEta2CNoCuts("hInvMassEta2CNoCuts","Invariant mass Eta two cluster selection. No Cuts",1000, 0., 1.); 
 //  


//  TH2F hInvMassEta2CAndPtC1("hInvMassEta2CAndPtC1","Invariant mass Eta two cluster selection versus PtC1. No Cuts.",1000, 0., 1.,1000, 0., 10.); 

//  TH2F hInvMassEta2CAndS4oS9C2("hInvMassEta2CAndS4oS9C2","Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC1 e S4oS9 e PtC2.",1000, 0., 1.,1000, 0., 10.); 

//  TH1F hIsolation("hIsolation","Isolation after all other cuts",1000, 0., 10.);
//  TH1F hAngle("hAngle","Angle between two BC, any pair of 2 BC",3600, 0., 2.*PI);
 //  
//  //---- MC matching ----

//  TH2F hInvMassEta2CAndPtC1MC("hInvMassEta2CAndPtC1MC","MC match. Invariant mass Eta two cluster selection versus PtC1. No Cuts.",1000, 0., 1.,1000, 0., 10.); 
//  TH2F hInvMassEta2CAndS4oS9C1MC("hInvMassEta2CAndS4oS9C1MC","MC match. Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC1.",1000, 0., 1.,1000, 0., 10.);
//  TH2F hInvMassEta2CAndPtC2MC("hInvMassEta2CAndPtC2MC","MC match. Invariant mass Eta two cluster selection versus PtC2. Cuts PtC1 e S4oS9.",1000, 0., 1.,1000, 0., 10.); 
//  TH2F hInvMassEta2CAndS4oS9C2MC("hInvMassEta2CAndS4oS9C2MC","MC match. Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC1 e S4oS9 e PtC2.",1000, 0., 1.,1000, 0., 10.); 



 
 
 //---- MC Analysis and MC matching ---- 
 TH1F hAngleMC("hAngleMC","MC Data. Angle between the two photons from eta",3600, 0., 2.*PI);
 TH1F hAngleEtaMC("hAngleEtaMC","MC Data. Angle Eta between the two photons from eta",1000,0.,10.);
 TH1F hAnglePhiMC("hAnglePhiMC","MC Data. Angle Phi between the two photons from eta",3600, 0., 2.*PI);
 TH1F hR_eta_C("hR_eta_C","Angle between reconstructed eta from BC without cuts and MC photons",1000, 0., 10.);
 TH1F hInvMassEtaPh("hInvMassEtaPh","Invariant mass Eta from MC Truth",10000, 0.547449999999, 0.547450000001);
 TH1F hInvMassEtaCMCTruth("hInvMassEtaCMCTruth","Invariant mass Eta from Clusters. Match with MC Truth",1000, 0., 1.);
 
 TH1F hAngleC_MCMatch("hAngleC_MCMatch","Angle between the two photons from BC and MC matching",3600, 0., 2.*PI);
 TH1F hNPhoton("hNPhoton","number of photons per eta from MC Truth",100, 0., 100.);
 TH1F hPhotonPt("hPhotonPt","Photon pt",10000, 0., 10.);
 
 //---- Data Analysis ---- before cuts -----
 TH1F hAngle("hAngle","Angle between two BC, any pair of 2 BC",3600, 0., 2.*PI);
 TH1F hInvMassEta2CNoCuts("hInvMassEta2CNoCuts","Invariant mass Eta two cluster selection. NO CUTS",1000, 0., 1.);  
 TH2F hInvMassEta2CETNoCuts("hInvMassEta2CETNoCuts","Invariant mass Eta two cluster selection Vs Et. No Cuts.",1000, 0., 1.,1000, 0., 10.); 
 TH2F hInvMassEta2CETNoCutsMC("hInvMassEta2CETNoCutsMC","MC match. Invariant mass Eta two cluster selection vs Et. No Cuts.",1000, 0., 1.,1000, 0., 10.); 
 TH2F h2CPtC1AndPtC2("h2CPtC1AndPtC2","PtC1 versus PtC2. No Cuts",1000, 0., 10.,1000, 0., 10.); 
 
  //---- Data Analysis ---- after cuts -----
 TH2F hInvMassEta2CAndS4oS9C("hInvMassEta2CAndS4oS9C","Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC.",1000, 0., 1.,1000, 0., 10.);
 TH2F hInvMassEta2CAndS4oS9CMC("hInvMassEta2CAndS4oS9CMC","MC match. Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC.",1000, 0., 1.,1000, 0., 10.);
 
 TH3F hInvMassEta2CAndS4oS9CAndPtC("hInvMassEta2CAndS4oS9CAndPtC","Invariant mass Eta two cluster selection versus S4oS9 and PtC. Cuts PtC",100, 0., 1.,100, 0., 10.,100, 0., 10.); 
 TH2F hEta2CS4oS9CAndPtC("hEta2CS4oS9CAndPtC","S4oS9 C versus PtC. Cuts PtC",1000, 0., 10.,1000, 0., 10.); 
   
 TH2F hInvMassEta2CAndPtCMC("hInvMassEta2CAndPtCMC","MC match. Invariant mass Eta two cluster selection versus PtC. Cuts PtC and S4oS9.",1000, 0., 1.,1000, 0., 10.);   
 TH2F hInvMassEta2CAndPtC("hInvMassEta2CAndPtC","Invariant mass Eta two cluster selection versus PtC. Cuts PtC and S4oS9.",1000, 0., 1.,1000, 0., 10.); 

 TH2F hInvMassEta2CAndIsolationMC("hInvMassEta2CAndIsolationMC","MC match. Invariant mass Eta two cluster selection versus Iso/PtSum. Cut ON",1000, 0., 1.,1000, 0., 10.);
 TH2F hInvMassEta2CAndIsolation("hInvMassEta2CAndIsolation","Invariant mass Eta two cluster selection versus Iso/PtSum. Cut ON",1000, 0., 1.,1000, 0., 10.);
   
 TH1F hIsolation("hIsolation","Isolation after all other cuts",1000, 0., 10.);
 TH1F hIsolationMC("hIsolationMC","MC match. Isolation after all other cuts",1000, 0., 10.);  
  
 TH1F hInvMassEta2C("hInvMassEta2C","Invariant mass Eta two cluster selection. After ALL CUTS",1000, 0., 1.); 
 TH1F hInvMassEta2CMC("hInvMassEta2CMC","MC match. Invariant mass Eta two cluster selection. After ALL CUTS",1000, 0., 1.); 

 TH1F hAngleC("hAngleC","Angle between two BC. After ALL CUTS",3600, 0., 2.*PI);
 
 TH2F hInvMassEta2PtSumMC("hInvMassEta2PtSumMC","MC Match. Invariant mass Eta two cluster selection versus PtC_Sum. All Cuts",1000, 0., 1.,1000, 0., 100.); 
 TH2F hInvMassEta2PtSum("hInvMassEta2PtSum","Invariant mass Eta two cluster selection versus PtC_Sum. All Cuts",1000, 0., 1.,1000, 0., 100.); 

 
 
 
 //---- Input Variables Loaded ----
 std::cerr << "Input Variables Loaded: " << std::endl; 
 std::cerr << " R = " << R << std::endl; 
 std::cerr << " S4oS9C_Cut = " << S4oS9C_Cut << std::endl; 
 std::cerr << " ptC_Cut = " << ptC_Cut << std::endl; 
 std::cerr << " ptSum_Cut = " << ptSum_Cut << std::endl; 
 std::cerr << " RMax_Cut = " << RMax_Cut << std::endl; 
 std::cerr << " DeltaEtaMax_Cut = " << DeltaEtaMax_Cut << std::endl; 
 std::cerr << " et_Cut = " << et_Cut << std::endl; 
 std::cerr << " iso_Cut = " << iso_Cut << std::endl;
 std::cerr << " angle_Cut = " << angle_Cut << std::endl;
 std::cerr << " R_eta = " << R_eta << std::endl;
 std::cerr << " N_events = " << N_events << std::endl;
 std::cerr << " dEta_C_Cut = " << dEta_C_Cut << std::endl;
 std::cerr << " dPhi_C_Cut = " << dPhi_C_Cut << std::endl;
 std::cerr << " dEta_eta = " << dEta_eta << std::endl;
 std::cerr << " dPhi_eta = " << dPhi_eta << std::endl;
 std::cerr << " ptPh_Cut = " << ptPh_Cut << std::endl;
 
 
 
 double RQ = R*R;
 double R_etaQ = R_eta*R_eta;
 
 //---- Starting analysis ---- 
 int nEntries = chain->GetEntries () ;

 //---- MC Truth analysis ----
 int FirstCluster = -1;
 int SecondCluster = -1;
 int numEntry = -1;
 TTree ClusterPairCMC("ClusterPairCMC","ClusterPairCMC");
 ClusterPairCMC.Branch("FirstCluster",&FirstCluster,"FirstCluster/I");
 ClusterPairCMC.Branch("SecondCluster",&SecondCluster,"SecondCluster/I");
 ClusterPairCMC.Branch("numEntry",&numEntry,"numEntry/I");
 bool flag2Photons;
 
 
 //---------------------
 //---- MC Analysis ----
 //---------------------
 
 for (int entry = 0 ; entry < nEntries ; ++entry) //---- TEST VELOCE ----
 {
  chain->GetEntry (entry) ;
  if (entry%10000 == 0) std::cout << "------> reading entry " << entry << " <------\n" ;
  
  numEntry = entry;
  
  //---- loop over etas ----
  int counterPhotons = 0;
  for (int ii=0; ii<numEta_; ii++){
   flag2Photons = false;
   FirstCluster = -1;
   SecondCluster = -1;
   
   math::XYZTLorentzVector pPh(0,0,0,0);
   math::XYZTLorentzVector pC(0,0,0,0);
   hNPhoton.Fill(numPh_->at(ii));
   
   //---- calculate angle between two photons from Eta MC ----
   double angle_MC = -1; //---- default value ----
   if (numPh_->at(ii) == 2) {
    int counterPhotons_Angle = counterPhotons;
    
    //---- first photon ----
    double pxPh_Angle_1 = pxPh_->at(counterPhotons_Angle);
    double pyPh_Angle_1 = pyPh_->at(counterPhotons_Angle);
    double pzPh_Angle_1 = pzPh_->at(counterPhotons_Angle);
    double EnergyPh_Angle_1 = sqrt(pxPh_Angle_1*pxPh_Angle_1 + pyPh_Angle_1*pyPh_Angle_1 + pzPh_Angle_1*pzPh_Angle_1);
    math::XYZTLorentzVector pPh_temp_Angle_1(pxPh_Angle_1,pyPh_Angle_1,pzPh_Angle_1,EnergyPh_Angle_1);
    double etaPh_Angle_1 = etaPh_->at(counterPhotons_Angle);
    double phiPh_Angle_1 = phiPh_->at(counterPhotons_Angle);
    
    //---- second photon ----
    counterPhotons_Angle++;
    double pxPh_Angle_2 = pxPh_->at(counterPhotons_Angle);
    double pyPh_Angle_2 = pyPh_->at(counterPhotons_Angle);
    double pzPh_Angle_2 = pzPh_->at(counterPhotons_Angle);
    double EnergyPh_Angle_2 = sqrt(pxPh_Angle_2*pxPh_Angle_2 + pyPh_Angle_2*pyPh_Angle_2 + pzPh_Angle_2*pzPh_Angle_2);
    math::XYZTLorentzVector pPh_temp_Angle_2(pxPh_Angle_2,pyPh_Angle_2,pzPh_Angle_2,EnergyPh_Angle_2);
    double etaPh_Angle_2 = etaPh_->at(counterPhotons_Angle);
    double phiPh_Angle_2 = phiPh_->at(counterPhotons_Angle);
    
    double cosAngle = pxPh_Angle_1 * pxPh_Angle_2 + pyPh_Angle_1 * pyPh_Angle_2 + pzPh_Angle_1 * pzPh_Angle_2;
    if (EnergyPh_Angle_1!=0 && EnergyPh_Angle_2!=0) cosAngle = cosAngle / (EnergyPh_Angle_1 * EnergyPh_Angle_2);
    angle_MC = acos(cosAngle);
    hAngleMC.Fill(angle_MC);

    hAngleEtaMC.Fill(fabs(etaPh_Angle_1-etaPh_Angle_2));
    hAnglePhiMC.Fill(fabs(phiPh_Angle_1-phiPh_Angle_2));
        
   }
   //---- end calculate angle between two photons from Eta MC ----
   
   int counterPh1 = -1;
   int counterPh2 = -1;
   
   bool flagFirstPhoton = false;
   bool flagSecondPhoton = false;
   //---- loop over photons generating an eta ----
   for (int jj=0; jj<numPh_->at(ii); jj++){
    double pxPh = pxPh_->at(counterPhotons);
    double pyPh = pyPh_->at(counterPhotons);
    double pzPh = pzPh_->at(counterPhotons);
    double ptPh = sqrt(pxPh * pxPh + pyPh * pyPh);
    double EnergyPh = sqrt(pxPh*pxPh + pyPh*pyPh + pzPh*pzPh);
    math::XYZTLorentzVector pPh_temp(pxPh,pyPh,pzPh,EnergyPh);
    double etaPh = etaPh_->at(counterPhotons);
    double phiPh = phiPh_->at(counterPhotons);
    pPh = pPh + pPh_temp;
    
    double bestTestValueRCQ = 100000;
   //---- loop over clusters ----
    for (int kk=0; kk<numC_; kk++){
     double etaC1 = etaC_->at(kk);
     double phiC1 = phiC_->at(kk);
     double RC1Q = (etaPh - etaC1) * (etaPh - etaC1) + (phiPh - phiC1) * (phiPh - phiC1); //---- Q at the end stands for ^2 ----
     if (RC1Q < RQ) { //---- found a cluster near a photon ----
      if (jj == 0) { //---- first photon ----
       if ((bestTestValueRCQ > RC1Q) && (fabs(etaPh - etaC1) < dEta_C_Cut) && (fabs(phiPh - phiC1) < dPhi_C_Cut) && (ptPh > ptPh_Cut)) {
        FirstCluster = kk;
        counterPh1 = counterPhotons;
        bestTestValueRCQ = RC1Q;
        flagFirstPhoton = true;
       }
      }
      else { //---- second photon ----
       if ((bestTestValueRCQ > RC1Q) && (fabs(etaPh - etaC1) < dEta_C_Cut) && (deltaPhi(phiPh,phiC1) < dPhi_C_Cut) && (ptPh > ptPh_Cut)) {
        SecondCluster = kk; //---- if I find a third photon ? --> Never found :D --> if it happens throw away eta event -> flag2Photons = false
        counterPh2= counterPhotons;
        bestTestValueRCQ = RC1Q;
        flagSecondPhoton = true;
        if (flagFirstPhoton) flag2Photons = true; //---- found 2 photons ----
       }
      }
      double pxC = pxC_->at(kk);
      double pyC = pyC_->at(kk);
      double pzC = pzC_->at(kk);
      double EnergyC = sqrt(pxC*pxC + pyC*pyC + pzC*pzC);
      math::XYZTLorentzVector pC_temp(pxC,pyC,pzC,EnergyC);
      pC = pC + pC_temp;
     }//---- end found a cluster near a photon ----
    }//---- end loop clusters ----
    counterPhotons++;
   }//---- end loop photons generating an eta ----
   
   if (numPh_->at(ii) != 2) flag2Photons = false; //---- I do NOT want != 2 photons events ----
   
   bool flagEtaDirection = true;
   if (flag2Photons){
   //---- match composite cluster <-> eta ----
   //---- first photon ----
    double etaPh1 = etaPh_->at(counterPh1);
    double phiPh1 = phiPh_->at(counterPh1);
    double pxPh1 = pxPh_->at(counterPh1);
    double pyPh1 = pyPh_->at(counterPh1);
    double pzPh1 = pzPh_->at(counterPh1);
    double EnergyPh1 = sqrt(pxPh1*pxPh1 + pyPh1*pyPh1 + pzPh1*pzPh1);
    math::XYZTLorentzVector pPh_temp1(pxPh1,pyPh1,pzPh1,EnergyPh1);
   //---- second photon ----
    double etaPh2 = etaPh_->at(counterPh2);
    double phiPh2 = phiPh_->at(counterPh2);
    double pxPh2 = pxPh_->at(counterPh2);
    double pyPh2 = pyPh_->at(counterPh2);
    double pzPh2 = pzPh_->at(counterPh2);
    double EnergyPh2 = sqrt(pxPh2*pxPh2 + pyPh2*pyPh2 + pzPh2*pzPh2);
    math::XYZTLorentzVector pPh_temp2(pxPh2,pyPh2,pzPh2,EnergyPh2);
    math::XYZTLorentzVector pPh_Eta = pPh_temp1 + pPh_temp2;
    double etaEta = pPh_Eta.eta();
    double phiEta = pPh_Eta.phi();
   //---- sum cluster ----
    double etaSumCluster = pC.eta();
    double phiSumCluster = pC.phi();
    double R_etaQ_C = (etaEta - etaSumCluster) * (etaEta - etaSumCluster) + deltaPhi(phiEta,phiSumCluster) * deltaPhi(phiEta,phiSumCluster);
    if ((R_etaQ_C < R_etaQ) && (fabs(etaSumCluster - etaEta) < dEta_eta) && (deltaPhi(phiSumCluster,phiEta) < dPhi_eta)) { //---- found a cluster near a photon ----
     flagEtaDirection = true;
    }
    hR_eta_C.Fill(sqrt(R_etaQ_C));
   }
   //---- end match composite cluster <-> eta ----
   
   if (flag2Photons && flagEtaDirection) ClusterPairCMC.Fill(); //---- tree filling ----
   
   //---- result in histograms ---- Only if I found two photons and if they are in the right position ----
   if (flag2Photons && flagEtaDirection){
    double ptPh = sqrt(pPh.x() * pPh.x() + pPh.y() * pPh.y());
    hPhotonPt.Fill(ptPh);
    double InvMassPh = pPh.mag();
    hInvMassEtaPh.Fill(InvMassPh);
    double InvMassC = pC.mag();
    if (InvMassC != 0) hInvMassEtaCMCTruth.Fill(InvMassC); //---- if zero -> no clusters found ----
   }
  }
  //---- end loop over etas ----
 }
   
 
 
 
 
 
 
 //---- NO MC Truth analysis ---- Pair Analysis ----
 //---- and MC Comparison ----
   
 int FirstClusterAfter = -1;
 int SecondClusterAfter = -1;
 int numEntryAfter = -1;
   
 ClusterPairCMC.SetBranchAddress("FirstCluster",&FirstClusterAfter);
 ClusterPairCMC.SetBranchAddress("SecondCluster",&SecondClusterAfter);
 ClusterPairCMC.SetBranchAddress("numEntry",&numEntryAfter);

 int nEntriesMC = ClusterPairCMC.GetEntries () ;

 if (N_events == -1) N_events = nEntries;
 for (int entry = 0 ; entry < N_events ; ++entry) //---- TEST VELOCE ----
 {
  chain->GetEntry (entry) ;
  if (entry%10000 == 0) std::cout << "------> No MC Truth ---- reading entry " << entry << " <------\n" ;
  std::cout << "------> No MC Truth ---- reading entry " << entry << " <------\n" ;
  
  std::vector<int> numC_Selected;
  math::XYZTLorentzVector pC(0,0,0,0);
 
  int numberC1;
  int numberC2;
  double EnergyPair;
  bool flagMCMatch;
  double angle_C;
  
  //---- Tree with Cluster data ----
  TTree PairCluster("PairCluster","PairCluster");
  PairCluster.Branch("numberC1",&numberC1,"numberC1/I");
  PairCluster.Branch("numberC2",&numberC2,"numberC2/I");
  PairCluster.Branch("EnergyPair",&EnergyPair,"EnergyPair/D");
  PairCluster.Branch("flagMCMatch",&flagMCMatch,"flagMCMatch/O");
  PairCluster.Branch("angle_C",&angle_C,"angle_C/D");
  
  //---- loop over clusters ----
  for (int kk=0; kk<numC_; kk++){
   
   double etaC1 = etaC_->at(kk);
   double phiC1 = phiC_->at(kk);
   double pxC1 = pxC_->at(kk);
   double pyC1 = pyC_->at(kk);
   double pzC1 = pzC_->at(kk);
   double ptC1 = sqrt(pxC1*pxC1 + pyC1*pyC1);
   double EnergyC1 = sqrt(pxC1*pxC1 + pyC1*pyC1 + pzC1*pzC1);
   double EnergyTC1 = sqrt(pxC1*pxC1 + pyC1*pyC1);
   
   numberC1 = kk;
   math::XYZTLorentzVector pC2C_1(pxC1,pyC1,pzC1,EnergyC1);
   
   //---- save all pair-photons ----
   for (int ll=kk+1; ll<numC_; ll++){ //---- inizio da kk così non rischio di avere doppioni ----
    if (ll!=kk) {
     double pxCIn = pxC_->at(ll);
     double pyCIn = pyC_->at(ll);
     double pzCIn = pzC_->at(ll);
     double EnergyCIn = sqrt(pxCIn*pxCIn + pyCIn*pyCIn + pzCIn*pzCIn);
     math::XYZTLorentzVector pC2C_2(pxCIn,pyCIn,pzCIn,EnergyCIn);
     math::XYZTLorentzVector pC2C_sum = pC2C_1 + pC2C_2;
     numberC2 = ll;
     EnergyPair = pC2C_sum.mag();
     
     //---- check if cluster pair correspond to a MC photon pair from eta ----
     for (int entryMC = 0 ; entryMC < nEntriesMC ; ++entryMC){
      ClusterPairCMC.GetEntry(entryMC);
      if (numEntryAfter == entry) {
       if (((FirstClusterAfter == ll) && (SecondClusterAfter == kk)) || ((FirstClusterAfter == kk) && (SecondClusterAfter == ll))) flagMCMatch  = true;
       else flagMCMatch = false; 
      }
     }    
     //---- angle ----
     double cosAngle = pxCIn * pxC1 + pyCIn * pyC1 + pzCIn * pzC1;
     if (EnergyCIn!=0 && EnergyC1!=0) cosAngle = cosAngle / (EnergyCIn * EnergyC1);
     else cosAngle = 0; //---- -> 90° -> rejected ----
     angle_C = acos(cosAngle);
     hAngle.Fill(angle_C);
     if (flagMCMatch) hAngleC_MCMatch.Fill(angle_C);
     //---- Add to pair cluster only if Angle between Basic Clusters is less than "angle_cut" ----
     if (angle_C < angle_Cut) PairCluster.Fill();
    }
   }//---- end save all pair-photons ----
  }//---- end loop clusters ----
   

  
   
   //---- pair-photon tree analysis ----
   //---- here add cuts ----
   
  int numberC1After;
  int numberC2After;
  double EnergyPairAfter;
  bool flagMCMatchAfter;
  double angle_C_After;
   
  int nEntriesTreePairCluster = PairCluster.GetEntries () ;
  PairCluster.SetBranchAddress("numberC1",&numberC1After);
  PairCluster.SetBranchAddress("numberC2",&numberC2After);
  PairCluster.SetBranchAddress("EnergyPair",&EnergyPairAfter);
  PairCluster.SetBranchAddress("flagMCMatch",&flagMCMatchAfter);
  PairCluster.SetBranchAddress("angle_C",&angle_C_After);
   
   //---- loop over cluster-pair ----
  for (int pairNumber = 0 ; pairNumber < nEntriesTreePairCluster ; ++pairNumber) {
   PairCluster.GetEntry(pairNumber);
    
   double etaC1_After = etaC_->at(numberC1After);
   double phiC1_After = phiC_->at(numberC1After);
   double pxC1_After = pxC_->at(numberC1After);
   double pyC1_After = pyC_->at(numberC1After);
   double pzC1_After = pzC_->at(numberC1After);
   double ptC1_After = sqrt(pxC1_After * pxC1_After + pyC1_After * pyC1_After);
   double EnergyC1_After = sqrt(pxC1_After * pxC1_After + pyC1_After * pyC1_After + pzC1_After * pzC1_After);
    
   math::XYZTLorentzVector pC1_After(pxC1_After,pyC1_After,pzC1_After,EnergyC1_After);
    
    
   double etaC2_After = etaC_->at(numberC2After);
   double phiC2_After = phiC_->at(numberC2After);
   double pxC2_After = pxC_->at(numberC2After);
   double pyC2_After = pyC_->at(numberC2After);
   double pzC2_After = pzC_->at(numberC2After);
   double ptC2_After = sqrt(pxC2_After * pxC2_After + pyC2_After * pyC2_After);
   double EnergyC2_After = sqrt(pxC2_After * pxC2_After + pyC2_After * pyC2_After + pzC2_After * pzC2_After);

   math::XYZTLorentzVector pC2_After(pxC2_After,pyC2_After,pzC2_After,EnergyC2_After);
    

   math::XYZTLorentzVector pC_Sum_After = pC1_After + pC2_After;
   double etaSum_After = pC_Sum_After.eta();
   double phiSum_After = pC_Sum_After.phi();
   double pxSum_After = pC_Sum_After.x();
   double pySum_After = pC_Sum_After.y();
   double pzSum_After = pC_Sum_After.z();
   double ptSum_After = sqrt(pxSum_After * pxSum_After + pySum_After * pySum_After);
   double EnergySum_After = sqrt(pxSum_After * pxSum_After + pySum_After * pySum_After + pzSum_After * pzSum_After);
    
    
        
    //---- some graphs before cuts ----
   hInvMassEta2CNoCuts.Fill(EnergyPairAfter);
   hInvMassEta2CETNoCuts.Fill(EnergyPairAfter,ptC1_After); //---- save only first cluster -> second cluster selected after ----
   if (flagMCMatchAfter) { //---- save only first cluster -> second cluster selected after ----
    hInvMassEta2CETNoCutsMC.Fill(EnergyPairAfter,ptC1_After);
   }
   h2CPtC1AndPtC2.Fill(ptC1_After,ptC2_After);
    //---- end some graphs before cuts ----
    
    //---- add cuts ----
    //---- cut in pair -> both Clusters ----
   
    
    
    //---- pt-Cluster cut Cluster 1 and Cluster 2 ----
   if ((ptC1_After > ptC_Cut) && (ptC2_After > ptC_Cut)) {

    if (flagMCMatchAfter) hInvMassEta2CAndS4oS9CMC.Fill(EnergyPairAfter,S4oS9C_->at(numberC1After));
    hInvMassEta2CAndS4oS9C.Fill(EnergyPairAfter,S4oS9C_->at(numberC1After));
     
    hInvMassEta2CAndS4oS9CAndPtC.Fill(EnergyPairAfter,S4oS9C_->at(numberC1After),ptC1_After);
    hEta2CS4oS9CAndPtC.Fill(S4oS9C_->at(numberC1After),ptC1_After);
     
     
     //---- S4oS9C_-Cluster cut Cluster 1 and Cluster 2 ----
    if ((S4oS9C_->at(numberC1After) > S4oS9C_Cut) && (S4oS9C_->at(numberC2After) > S4oS9C_Cut)){
      
     if (flagMCMatchAfter) hInvMassEta2CAndPtCMC.Fill(EnergyPairAfter,ptC1_After);
     hInvMassEta2CAndPtC.Fill(EnergyPairAfter,ptC1_After);
      
      //---- Isolatio cut ----
     double iso = 0;
     for (int ll=0; ll<numC_; ll++){
      if ((ll!=numberC1After) && (ll!=numberC2After)) {
       double etaCIso = etaC_->at(ll);
       double phiCIso = phiC_->at(ll);
       double R = sqrt((etaCIso-etaSum_After)*(etaCIso-etaSum_After) + deltaPhi(phiCIso,phiSum_After) * deltaPhi(phiCIso,phiSum_After));
       double deta = fabs(etaCIso - etaSum_After);
       double et = etC_->at(ll);
       if ( (R < RMax_Cut)  && (deta < DeltaEtaMax_Cut) && (et > et_Cut)  ) iso = iso + et ;
      }
     }
        
     if (flagMCMatchAfter) hInvMassEta2CAndIsolationMC.Fill(EnergyPairAfter,iso/ptSum_After);
     hInvMassEta2CAndIsolation.Fill(EnergyPairAfter,iso/ptSum_After);
     if (flagMCMatchAfter) hIsolationMC.Fill(iso/ptSum_After);
     hIsolation.Fill(iso/ptSum_After);
        
     if ((iso/ptSum_After) < iso_Cut) {
      if (flagMCMatchAfter) hInvMassEta2CMC.Fill(EnergyPairAfter);
      if (flagMCMatchAfter) hInvMassEta2PtSumMC.Fill(EnergyPairAfter,ptSum_Cut);
      hInvMassEta2PtSum.Fill(ptSum_Cut,EnergyPairAfter);
      if (ptSum_After < ptSum_Cut) hInvMassEta2C.Fill(EnergyPairAfter);
      hAngleC.Fill(angle_C_After); //---- cut on angle has been performed in pair filling ----
     }//---- end Isolatio cut ----        
    }//---- end S4oS9C_-Cluster cut Cluster 1 and Cluster 2 ----
   }//---- end pt-Cluster cut Cluster 1 and Cluster 2 
  }//---- end loop over cluster-pair ----
 } //---- end loop entries ----
 
 
 //---- save in file ---- 
 TFile saving (outputRootName.c_str (),"recreate") ;
 saving.cd () ;

 
 hInvMassEtaPh.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaPh.Write();
 
 hInvMassEtaCMCTruth.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaCMCTruth.Write();
 
 hInvMassEta2C.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2C.Write();
  
 hInvMassEta2CNoCuts.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CNoCuts.Write();
  
 hInvMassEta2CETNoCuts.GetYaxis()->SetTitle("Et (GeV)");
 hInvMassEta2CETNoCuts.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CETNoCuts.Write();
  
 hInvMassEta2CAndIsolation.GetYaxis()->SetTitle("iso/PtSum");
 hInvMassEta2CAndIsolation.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndIsolation.Write();
  
 hInvMassEta2CAndPtC.GetYaxis()->SetTitle("PtC (GeV)");
 hInvMassEta2CAndPtC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndPtC.Write();
  
 hInvMassEta2CAndS4oS9C.GetYaxis()->SetTitle("S4oS9");
 hInvMassEta2CAndS4oS9C.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndS4oS9C.Write();
 
 hInvMassEta2CAndS4oS9CAndPtC.GetYaxis()->SetTitle("S4oS9");
 hInvMassEta2CAndS4oS9CAndPtC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndS4oS9CAndPtC.GetZaxis()->SetTitle("Pt (GeV)");
 hInvMassEta2CAndS4oS9CAndPtC.Write();
  
 hEta2CS4oS9CAndPtC.GetYaxis()->SetTitle("Pt (GeV)");
 hEta2CS4oS9CAndPtC.GetXaxis()->SetTitle("S4oS9");
 hEta2CS4oS9CAndPtC.Write();
 
 h2CPtC1AndPtC2.GetYaxis()->SetTitle("Pt C2 (GeV)");
 h2CPtC1AndPtC2.GetXaxis()->SetTitle("Pt C1 (GeV)");
 h2CPtC1AndPtC2.Write();
   
 hIsolation.GetXaxis()->SetTitle("iso/PtSum");
 hIsolation.Write();

 hIsolationMC.GetXaxis()->SetTitle("iso/PtSum");
 hIsolationMC.Write();

 hAngleC.GetXaxis()->SetTitle("Angle between BC (rad)");
 hAngleC.Write();
  
 hAngle.GetXaxis()->SetTitle("Angle between BC (rad)");
 hAngle.Write();
  
 hInvMassEta2PtSum.GetYaxis()->SetTitle("Pt_Sum (GeV)");
 hInvMassEta2PtSum.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2PtSum.Write();
 
  
 //---- MC Match ----
 hInvMassEta2PtSumMC.GetYaxis()->SetTitle("Pt_Sum (GeV)");
 hInvMassEta2PtSumMC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2PtSumMC.Write();
 
 hInvMassEta2CETNoCutsMC.GetYaxis()->SetTitle("Et (GeV)");
 hInvMassEta2CETNoCutsMC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CETNoCutsMC.Write();
  
 hInvMassEta2CAndPtCMC.GetYaxis()->SetTitle("PtC (GeV/c)");
 hInvMassEta2CAndPtCMC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndPtCMC.Write();
  
 hInvMassEta2CAndS4oS9CMC.GetYaxis()->SetTitle("S4oS9");
 hInvMassEta2CAndS4oS9CMC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndS4oS9CMC.Write();
 
 hInvMassEta2CAndIsolationMC.GetYaxis()->SetTitle("Iso/PtSum");
 hInvMassEta2CAndIsolationMC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndIsolationMC.Write();
 
 hInvMassEta2CMC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CMC.Write();

 hAngleMC.GetXaxis()->SetTitle("Angle between Photons(rad)");
 hAngleMC.Write();
   
 hAngleC_MCMatch.GetXaxis()->SetTitle("Angle between BC (rad)");
 hAngleC_MCMatch.Write();
  
 hAngleEtaMC.GetXaxis()->SetTitle("Angle Eta photons (rad)");
 hAngleEtaMC.Write();
  
 hAnglePhiMC.GetXaxis()->SetTitle("Angle Phi between photons (rad)");
 hAnglePhiMC.Write();
  
 hR_eta_C.GetXaxis()->SetTitle("#eta and #phi plane radius");
 hR_eta_C.Write();
 
 hNPhoton.GetXaxis()->SetTitle("number of photons per Eta");
 hNPhoton.Write();
   
 hPhotonPt.GetXaxis()->SetTitle("Pt of summed photons from Eta (GeV/c)");
 hPhotonPt.Write();

  
 //---- Save Cuts in File ----
 
 TTree Cuts("Cuts","Cuts");
 Cuts.Branch("R",&R,"R/D");
 Cuts.Branch("S4oS9C_Cut",&S4oS9C_Cut,"S4oS9C_Cut/D");
 Cuts.Branch("ptC_Cut",&ptC_Cut,"ptC_Cut/D");
 Cuts.Branch("ptSum_Cut",&ptSum_Cut,"ptSum_Cut/D");
 Cuts.Branch("RMax_Cut",&RMax_Cut,"RMax_Cut/D");
 Cuts.Branch("DeltaEtaMax_Cut",&DeltaEtaMax_Cut,"DeltaEtaMax_Cut/D");
 Cuts.Branch("et_Cut",&et_Cut,"et_Cut/D");
 Cuts.Branch("iso_Cut",&iso_Cut,"iso_Cut/D");
 Cuts.Branch("angle_Cut",&angle_Cut,"angle_Cut/D");
 Cuts.Branch("R_eta",&R_eta,"R_eta/D");
 Cuts.Branch("N_events",&N_events,"N_events/I");
 Cuts.Branch("dEta_C_Cut",&dEta_C_Cut,"dEta_C_Cut/D");
 Cuts.Branch("dPhi_C_Cut",&dPhi_C_Cut,"dPhi_C_Cut/D");
 Cuts.Branch("dEta_eta",&dEta_eta,"dEta_eta/D");
 Cuts.Branch("dPhi_eta",&dPhi_eta,"dPhi_eta/D");
 Cuts.Branch("ptPh_Cut",&ptPh_Cut,"ptPh_Cut/D");

 
 Cuts.Fill();
 Cuts.Write();
 
 saving.Close ();
 delete chain;
 return 0 ;

}



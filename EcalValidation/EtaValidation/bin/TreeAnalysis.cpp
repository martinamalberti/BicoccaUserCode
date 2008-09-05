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
 double R = 0.2;
 double S4oS9C_Cut = 0.85; //---- S4oS9C_-Cluster cut ----
 double ptC_Cut = 1.;      //---- pt-Cluster cut ----
 double ptSum_Cut = 2.;    //---- pt-coppia cut ---- 
 
 double RMax_Cut = 0.2;            //---- RMax_Cut cut ---- isolation ----
 double DeltaEtaMax_Cut = 0.05;    //---- DeltaEtaMax_Cut ---- isolation ----
 double et_Cut = 1.;               //---- et_Cut ---- isolation ----
 double iso_Cut = 0.4;             //---- iso/pt-coppia cut ---- isolation ----
 
 
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
 //  chain->SetBranchAddress("HitsEnergyC_",&HitsEnergyC_); //---- non è presente per ora ----
 
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

 TH1F hInvMassEtaPh("hInvMassEtaPh","Invariant mass Eta from MC Truth",1000, 0.54741, 0.54749);
 TH1F hInvMassEtaCMCTruth("hInvMassEtaCMCTruth","Invariant mass Eta from Clusters. Match with MC Truth",1000, 0., 1.);
 TH1F hInvMassEtaC("hInvMassEtaC","Invariant mass Eta from Clusters",1000, 0., 1.);
 TH2F hInvMassEtaC_numC("hInvMassEtaC_numC","Invariant mass Eta from Clusters versus number of cluster selected",1000, 0., 1.,100, 0., 100.);
 TH1F hPhotonPt("hPhotonPt","Photon pt",10000, 0., 10.);
 TH1F hClusterPt("hClusterPt","Single Cluster pt",30000, 0., 30.);
 TH1F hNPhoton("hNPhoton","number of photons per eta from MC Truth",100, 0., 100.);
 TH1F hPtSum("hPtSum","Summed cluster pt",40000, 0., 40.);
 TH1F hInvMassEta2CNoCuts("hInvMassEta2CNoCuts","Invariant mass Eta two cluster selection. No Cuts",1000, 0., 1.); 
 TH1F hInvMassEta2C("hInvMassEta2C","Invariant mass Eta two cluster selection",1000, 0., 1.); 
 
 TH2F hInvMassEta2CETNoCuts("hInvMassEta2CETNoCuts","Invariant mass Eta two cluster selection. No Cuts. Et",1000, 0., 1.,1000, 0., 10.); 
 TH2F hInvMassEta2CAndIsolation("hInvMassEta2CAndIsolation","Invariant mass Eta two cluster selection versus Iso/PtSum. Cut On",1000, 0., 1.,1000, 0., 10.);
 TH2F hInvMassEta2CAndPtC1("hInvMassEta2CAndPtC1","Invariant mass Eta two cluster selection versus PtC1. No Cuts.",1000, 0., 1.,1000, 0., 10.); 
 TH2F hInvMassEta2CAndS4oS9C1("hInvMassEta2CAndS4oS9C1","Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC1.",1000, 0., 1.,1000, 0., 10.);
 TH2F hInvMassEta2CAndPtC2("hInvMassEta2CAndPtC2","Invariant mass Eta two cluster selection versus PtC2. Cuts PtC1 e S4oS9.",1000, 0., 1.,1000, 0., 10.); 
 TH2F hInvMassEta2CAndS4oS9C2("hInvMassEta2CAndS4oS9C2","Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC1 e S4oS9 e PtC2.",1000, 0., 1.,1000, 0., 10.); 
 TH2F hEta2CS4oS9C1AndPtC2("hEta2CS4oS9C1AndPtC2","S4oS9 C1 versus PtC2. Cuts PtC1",1000, 0., 10.,1000, 0., 10.); 
 TH3F hInvMassEta2CAndS4oS9C1AndPtC2("hInvMassEta2CAndS4oS9C1AndPtC2","Invariant mass Eta two cluster selection versus S4oS9 and PtC2. Cuts PtC1",100, 0., 1.,100, 0., 10.,100, 0., 10.); 
 TH2F h2CPtC1AndPtC2("h2CPtC1AndPtC2","PtC1 versus PtC2. No Cuts",1000, 0., 10.,1000, 0., 10.); 
 TH1F hIsolation("hIsolation","Isolation after all other cuts",1000, 0., 10.);
 
 
 
 
 
 
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

 double RQ = R*R;
 
 
 //---- Starting analysis ---- 
 int nEntries = chain->GetEntries () ;

 //---- MC Truth analysis ----
 for (int entry = 0 ; entry < nEntries ; ++entry) //---- TEST VELOCE ----
 {
  chain->GetEntry (entry) ;
  if (entry%10000 == 0) std::cout << "------> reading entry " << entry << " <------\n" ;
  
  //---- loop over etas ----
  int counterPhotons = 0;
  for (int ii=0; ii<numEta_; ii++){
   math::XYZTLorentzVector pPh(0,0,0,0);
   math::XYZTLorentzVector pC(0,0,0,0);
   hNPhoton.Fill(numPh_->at(ii));
   //---- loop over photons generating an eta ----
   for (int jj=0; jj<numPh_->at(ii); jj++){
    double pxPh = pxPh_->at(counterPhotons);
    double pyPh = pyPh_->at(counterPhotons);
    double pzPh = pzPh_->at(counterPhotons);
    double EnergyPh = sqrt(pxPh*pxPh + pyPh*pyPh + pzPh*pzPh);
    math::XYZTLorentzVector pPh_temp(pxPh,pyPh,pzPh,EnergyPh);
    double etaPh = etaPh_->at(counterPhotons);
    double phiPh = phiPh_->at(counterPhotons);
    pPh = pPh + pPh_temp;
   //---- loop over clusters ----
    for (int kk=0; kk<numC_; kk++){
     double etaC1 = etaC_->at(kk);
     double phiC1 = phiC_->at(kk);
     double RC1Q = (etaPh - etaC1) * (etaPh - etaC1) + (phiPh - phiC1) * (phiPh - phiC1); //---- Q at the end stands for ^2 ----
     if (RC1Q < RQ) { //---- found a cluster near a photon ----
      double pxC = pxC_->at(kk);
      double pyC = pyC_->at(kk);
      double pzC = pzC_->at(kk);
      double EnergyC = sqrt(pxC*pxC + pyC*pyC + pzC*pzC);
      math::XYZTLorentzVector pC_temp(pxC,pyC,pzC,EnergyC);
      pC = pC + pC_temp;
     }
    }//---- end loop clusters ----
    counterPhotons++;
   }//---- end loop photons generating an eta ----
   //---- result in histograms ----
   double ptPh = sqrt(pPh.x() * pPh.x() + pPh.y() * pPh.y());
   hPhotonPt.Fill(ptPh);
   double InvMassPh = pPh.mag();
   hInvMassEtaPh.Fill(InvMassPh);
   double InvMassC = pC.mag();
   if (InvMassC != 0) hInvMassEtaCMCTruth.Fill(InvMassC); //---- if zero -> no clusters found ----
  }
  //---- end loop over etas ----
 }
 
 
 
 //---- no MC Truth analysis ----
 
 //---- loop entries ----
//  for (int entry = 0 ; entry < 2 ; ++entry) //---- TEST VELOCE ----
//  {
//   chain->GetEntry (entry) ;
//   if (entry%10000 == 0) std::cout << "------> No MC Truth ---- reading entry " << entry << " <------\n" ;
//   
//   std::vector<int> numC_Selected;
//   std::vector<int> numC_Selected_Pair;
//   math::XYZTLorentzVector pC(0,0,0,0);
//  
//   int numberC1;
//   int numberC2;
//   double EnergyPair;
//   
//   TTree PairCluster("PairCluster","PairCluster");
//   PairCluster.Branch("numberC1",&numberC1,"numberC1/I");
//   PairCluster.Branch("numberC2",&numberC2,"numberC2/I");
//   PairCluster.Branch("EnergyPair",&EnergyPair,"EnergyPair/D");
//   
//   //---- loop over clusters ----
//   for (int kk=0; kk<numC_; kk++){
//    double etaC1 = etaC_->at(kk);
//    double phiC1 = phiC_->at(kk);
//    double pxC = pxC_->at(kk);
//    double pyC = pyC_->at(kk);
//    double pzC = pzC_->at(kk);
//    double ptC = sqrt(pxC*pxC + pyC*pyC);
//    double EnergyC = sqrt(pxC*pxC + pyC*pyC + pzC*pzC);
//    double etaC = etaC_->at(kk);
//    double phiC = phiC_->at(kk);
//    
//    numberC1 = kk;
//    math::XYZTLorentzVector pC2C_1(pxC,pyC,pzC,EnergyC);
//    
//    //---- save all pair-photons ----
//    for (int ll=0; ll<numC_; ll++){
//     if (ll!=kk) {
//      double pxCIn = pxC_->at(ll);
//      double pyCIn = pyC_->at(ll);
//      double pzCIn = pzC_->at(ll);
//      double EnergyCIn = sqrt(pxCIn*pxCIn + pyCIn*pyCIn + pzCIn*pzCIn);
//      math::XYZTLorentzVector pC2C_2(pxCIn,pyCIn,pzCIn,EnergyCIn);
//      math::XYZTLorentzVector pC2C_sum = pC2C_1 + pC2C_2;
//      numberC2 = ll;
//      EnergyPair = pC2C_sum.mag();
//      PairCluster.Fill();
//     }
//    }
   
   
   //---- pair-photon tree analysis ----
// //    int numberC1After;
// //    int numberC2After;
// //    double EnergyPairAfter;
// //    int nEntriesTreePairCluster = PairCluster.GetEntries () ;
// //    PairCluster.SetBranchAddress("numberC1",&numberC1After);
// //    PairCluster.SetBranchAddress("numberC2",&numberC2After);
// //    PairCluster.SetBranchAddress("EnergyPair",&EnergyPairAfter);
// // 
// //    int pairNumber = -1;
// //    double EnergyPairAfterTemp = 0;
// //    double EnergyPairEta = 0.547853; //---- from PDG ----
// //    for (int entryTree = 0 ; entryTree < nEntriesTreePairCluster ; ++entryTree)
// //    {
// //     PairCluster.GetEntry(entryTree);
// //     //---- near to eta energy ----
// //     if (((EnergyPairAfter-EnergyPairEta) * (EnergyPairAfter-EnergyPairEta)) < ((EnergyPairAfterTemp-EnergyPairEta) * (EnergyPairAfterTemp-EnergyPairEta))) {
// //      pairNumber = entryTree;
// //      EnergyPairAfterTemp = EnergyPairAfter;
// //     }
// //    }
// //       
// //    if (pairNumber != -1){//---- if I've found a cluster-pair ----
// //     PairCluster.GetEntry(pairNumber);
// // //     hInvMassEta2CNoCuts.Fill(EnergyPairAfter);
// //    }
// //    
//    
//    hClusterPt.Fill(ptC);
//    //---- pt-Cluster cut ----
//    if (ptC > ptC_Cut){
//     //---- S4oS9C_-Cluster cut ----
//     if (S4oS9C_->at(kk) > S4oS9C_Cut){
//      math::XYZTLorentzVector pC_temp(pxC,pyC,pzC,EnergyC);
//      pC = pC + pC_temp;
//      numC_Selected.push_back(kk);
//     }
//    }
//   }//---- end loop clusters ----
//   
//   double pxSum = pC.x();
//   double pySum = pC.y();
//   double pzSum = pC.z();
//   double ptSum = sqrt(pxSum*pxSum + pySum*pySum);
//   double EnergySum = sqrt(pxSum*pxSum + pySum*pySum + pzSum*pzSum);
//   double InvMassSum = EnergySum;
//   
//   
//    //---- Isolation cut ----
//   double iso = 0;
//   for (std::vector<int>::iterator numC_Selected_iterator = numC_Selected.begin(); numC_Selected_iterator != numC_Selected.end(); numC_Selected_iterator++){
//    int num = *numC_Selected_iterator;
//    double etaC = etaC_->at(num);
//    double phiC = phiC_->at(num);
//    for(int hh=0; hh<numC_; hh++)
//    {
//     bool flagIsolation = true;
//     for (std::vector<int>::iterator numC_Selected_iterator_in = numC_Selected.begin(); numC_Selected_iterator_in != numC_Selected.end(); numC_Selected_iterator_in++) {
//      if (hh == *numC_Selected_iterator_in) flagIsolation = false; //---- I'm watching an already selected cluster ----
//     }
//     if (!flagIsolation) 
//     {
//      double etaCIn = etaC_->at(hh);
//      double phiCIn = phiC_->at(hh);
//      double R = sqrt((etaCIn-etaC)*(etaCIn-etaC) + (phiCIn-phiC)*(phiCIn-phiC));
//      double deta = fabs(etaCIn - etaC);
//      double et = etC_->at(hh);
//      if ( (R < RMax_Cut)  && (deta < DeltaEtaMax_Cut) && (et > et_Cut)  ) iso = iso + et ;
//     }
//    }
//   }
//    //---- iso/pt-coppia cut ----
//   if ((iso/ptSum) > iso_Cut) {
//    hPtSum.Fill(ptSum);
//     //---- pt-coppia cut ----
//    if (ptSum > ptSum_Cut)
//     if (InvMassSum != 0) {
//     hInvMassEtaC.Fill(InvMassSum); //---- if zero -> no clusters found ----
//     hInvMassEtaC_numC.Fill(InvMassSum,numC_Selected.size());
//     }
//   }
//  } //---- end loop entries ----
 
 
 
 
  //---- no MC Truth analysis ---- Pair Analysis ----
 
 for (int entry = 0 ; entry < nEntries ; ++entry) //---- TEST VELOCE ----
 {
  chain->GetEntry (entry) ;
  if (entry%10000 == 0) std::cout << "------> No MC Truth ---- reading entry " << entry << " <------\n" ;
  
  std::vector<int> numC_Selected;
  math::XYZTLorentzVector pC(0,0,0,0);
 
  int numberC1;
  int numberC2;
  double EnergyPair;
  
  
  //---- loop over clusters ----
  for (int kk=0; kk<numC_; kk++){
   
   TTree PairCluster("PairCluster","PairCluster");
   PairCluster.Branch("numberC1",&numberC1,"numberC1/I");
   PairCluster.Branch("numberC2",&numberC2,"numberC2/I");
   PairCluster.Branch("EnergyPair",&EnergyPair,"EnergyPair/D");
 
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
   for (int ll=kk; ll<numC_; ll++){ //---- inizio da kk così non rischio di avere doppioni ----
    if (ll!=kk) {
     double pxCIn = pxC_->at(ll);
     double pyCIn = pyC_->at(ll);
     double pzCIn = pzC_->at(ll);
     double EnergyCIn = sqrt(pxCIn*pxCIn + pyCIn*pyCIn + pzCIn*pzCIn);
     math::XYZTLorentzVector pC2C_2(pxCIn,pyCIn,pzCIn,EnergyCIn);
     math::XYZTLorentzVector pC2C_sum = pC2C_1 + pC2C_2;
     numberC2 = ll;
     EnergyPair = pC2C_sum.mag();
     PairCluster.Fill();
    }
   }
   
   
   //---- pair-photon tree analysis ----
   int numberC1After;
   int numberC2After;
   double EnergyPairAfter;
   int nEntriesTreePairCluster = PairCluster.GetEntries () ;
   PairCluster.SetBranchAddress("numberC1",&numberC1After);
   PairCluster.SetBranchAddress("numberC2",&numberC2After);
   PairCluster.SetBranchAddress("EnergyPair",&EnergyPairAfter);

   //---- loop over cluster-pair ----
   for (int pairNumber = 0 ; pairNumber < nEntriesTreePairCluster ; ++pairNumber) {
    PairCluster.GetEntry(pairNumber);
    hInvMassEta2CNoCuts.Fill(EnergyPairAfter);
    hInvMassEta2CETNoCuts.Fill(EnergyPairAfter,EnergyTC1);
    
    //---- aggiungo i vari cuts ----
    //---- pt-Cluster cut Cluster 1----
    hInvMassEta2CAndPtC1.Fill(EnergyPairAfter,ptC1);
    //---- Graph ----    
    double pxC2_temp_temp = pxC_->at(numberC2After);
    double pyC2_temp_temp = pyC_->at(numberC2After);
    double ptC2_temp_temp = sqrt(pxC2_temp_temp*pxC2_temp_temp + pyC2_temp_temp*pyC2_temp_temp);
    h2CPtC1AndPtC2.Fill(ptC1,ptC2_temp_temp);
    //---- End Graph ----
    if (ptC1 > ptC_Cut){
     
    //---- S4oS9C_-Cluster cut Cluster 1 ----
     hInvMassEta2CAndS4oS9C1.Fill(EnergyPairAfter,S4oS9C_->at(kk));
       //---- Graph ----    
     double pxC2_temp = pxC_->at(numberC2After);
     double pyC2_temp = pyC_->at(numberC2After);
     double ptC2_temp = sqrt(pxC2_temp*pxC2_temp + pyC2_temp*pyC2_temp);
     hInvMassEta2CAndS4oS9C1AndPtC2.Fill(EnergyPairAfter,S4oS9C_->at(kk),ptC2_temp);
     hEta2CS4oS9C1AndPtC2.Fill(S4oS9C_->at(kk),ptC2_temp);
       //---- End Graph ----
     
     if (S4oS9C_->at(kk) > S4oS9C_Cut){
      
      double etaC2 = etaC_->at(numberC2After);
      double phiC2 = phiC_->at(numberC2After);
      double pxC2 = pxC_->at(numberC2After);
      double pyC2 = pyC_->at(numberC2After);
      double pzC2 = pzC_->at(numberC2After);
      double ptC2 = sqrt(pxC2*pxC2 + pyC2*pyC2);
      double EnergyC2 = sqrt(pxC2*pxC2 + pyC2*pyC2 + pzC2*pzC2);

      //---- pt-Cluster cut Cluster 2 ----
      hInvMassEta2CAndPtC2.Fill(EnergyPairAfter,ptC2);
      if (ptC2 > ptC_Cut){
       //---- S4oS9C_-Cluster cut Cluster 2 ----
       hInvMassEta2CAndS4oS9C2.Fill(EnergyPairAfter,S4oS9C_->at(numberC2After));
       if (S4oS9C_->at(numberC2After) > S4oS9C_Cut){
        math::XYZTLorentzVector pC2C_2(pxC2,pyC2,pzC2,EnergyC2);
        pC = pC2C_1 + pC2C_2;
        double etaC = pC.eta();
        double phiC = pC.phi();
        double pxC = pC.x();
        double pyC = pC.y();
        double ptSum = sqrt(pxC*pxC + pyC*pyC);
        double InvMassPair = pC.mag();
        //---- Isolatio cut ----
        double iso = 0;
        for (int ll=0; ll<numC_; ll++){
         if ((ll!=numberC1After) && (ll!=numberC2After)) {
          double etaCIso = etaC_->at(ll);
          double phiCIso = phiC_->at(ll);
          double R = sqrt((etaCIso-etaC)*(etaCIso-etaC) + (phiCIso-phiC)*(phiCIso-phiC));
          double deta = fabs(etaCIso - etaC);
          double et = etC_->at(ll);
          if ( (R < RMax_Cut)  && (deta < DeltaEtaMax_Cut) && (et > et_Cut)  ) iso = iso + et ;
         }
        }
        hInvMassEta2CAndIsolation.Fill(EnergyPairAfter,iso/ptSum);
        hIsolation.Fill(iso/ptSum);
        if ((iso/ptSum) > iso_Cut) {
         hInvMassEta2C.Fill(EnergyPairAfter); //---- dovrebbe essere lo stesso valore ----
//         hInvMassEta2C.Fill(InvMassPair);
        }//---- end Isolatio cut ----        
       }//---- end S4oS9C_-Cluster cut Cluster 2 ----
      }//---- end pt-Cluster cut Cluster 2 ----
     }//---- end S4oS9C_-Cluster cut Cluster 1 ----
    }//---- end pt-Cluster cut Cluster 1----
   }//---- end loop over cluster-pair ----
  }//---- end loop clusters ----
 } //---- end loop entries ----
 
 
 //---- save in file ---- 
 TFile saving (outputRootName.c_str (),"recreate") ;
 saving.cd () ;

 hInvMassEtaPh.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaPh.Write();
 hInvMassEtaCMCTruth.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaCMCTruth.Write();
 hInvMassEtaC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaC.Write();
 hInvMassEtaC_numC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaC_numC.GetYaxis()->SetTitle("Number of Cluster selected");
 hInvMassEtaC_numC.Write();
 
 hPhotonPt.GetXaxis()->SetTitle("pT (GeV)");
 hPhotonPt.Write();
 hClusterPt.GetXaxis()->SetTitle("pT (GeV)");
 hClusterPt.Write();
 
 hNPhoton.GetXaxis()->SetTitle("number of photons per #eta");
 hNPhoton.Write();
   
 hPtSum.GetXaxis()->SetTitle("pT (GeV)");
 hPtSum.Write();

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
 
 hInvMassEta2CAndPtC1.GetYaxis()->SetTitle("PtC1 (GeV)");
 hInvMassEta2CAndPtC1.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndPtC1.Write();
 
 hInvMassEta2CAndS4oS9C1.GetYaxis()->SetTitle("S4oS9 C1");
 hInvMassEta2CAndS4oS9C1.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndS4oS9C1.Write();

 hInvMassEta2CAndPtC2.GetYaxis()->SetTitle("PtC2 (GeV)");
 hInvMassEta2CAndPtC2.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndPtC2.Write();
 
 hInvMassEta2CAndS4oS9C2.GetYaxis()->SetTitle("S4oS9 C2");
 hInvMassEta2CAndS4oS9C2.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndS4oS9C2.Write();

 hInvMassEta2CAndS4oS9C1AndPtC2.GetYaxis()->SetTitle("S4oS9 C1");
 hInvMassEta2CAndS4oS9C1AndPtC2.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndS4oS9C1AndPtC2.GetZaxis()->SetTitle("Pt C2 (GeV)");
 hInvMassEta2CAndS4oS9C1AndPtC2.Write();
 
 hEta2CS4oS9C1AndPtC2.GetYaxis()->SetTitle("Pt C2 (GeV)");
 hEta2CS4oS9C1AndPtC2.GetXaxis()->SetTitle("S4oS9 C1");
 hEta2CS4oS9C1AndPtC2.Write();

 h2CPtC1AndPtC2.GetYaxis()->SetTitle("Pt C2 (GeV)");
 h2CPtC1AndPtC2.GetXaxis()->SetTitle("Pt C1 (GeV)");
 h2CPtC1AndPtC2.Write();
 
 hIsolation.GetXaxis()->SetTitle("iso/PtSum");
 hIsolation.Write();
 
 
 

 
 
 
 
 saving.Close ();
 delete chain;
 return 0 ;

}


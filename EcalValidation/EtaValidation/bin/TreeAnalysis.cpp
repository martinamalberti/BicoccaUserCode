#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

//---- CMSSW includes
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
 

#include "DataFormats/Math/interface/LorentzVector.h"


//---- root includes
#include "TH1.h"
#include "TChain.h"
#include "TFile.h"


//! main program
int main (int argc, char** argv)
{
  
 std::string outputRootName = "outputHistos.root";
 std::string testName = "-o";
 if (argc>2){
  if (argv[1] == testName) {
   outputRootName = argv[2];
  }
 }
  
 std::string testHelp = "--help";
 if (argc==2){
  if (argv[1] == testHelp) {
   std::cout << "Help" << std::endl ;
   std::cout << " --help : display help" << std::endl ;
   std::cout << " -o : output root file name (eg histograms.root)" << std::endl ;
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
 chain->SetBranchAddress("HitsEnergyC_",&HitsEnergyC_);
 
 //---- ---- Photons from Eta ---- 
 chain->SetBranchAddress("numEta_",&numEta_);
 chain->SetBranchAddress("numPh_",&numPh_);
 chain->SetBranchAddress("thetaPh_",&thetaPh_);
 chain->SetBranchAddress("etaPh_",&etaPh_);
 chain->SetBranchAddress("phiPh_",&phiPh_);
 chain->SetBranchAddress("pxPh_",&pxPh_);
 chain->SetBranchAddress("pyPh_",&pyPh_);
 chain->SetBranchAddress("pzPh_",&pzPh_);
  
 
 
 
 
 
 
 if (argv[1] != testName){
  chain->Add (argv[1]) ;
 }
 else{
  chain->Add (argv[3]) ;
 }
 

 //---- Histograms ----

 TH1F hInvMassEtaPh("hInvMassEtaPh","Invariant mass Eta from MC Truth",1000, 0., 1.);
 TH1F hInvMassEta("hInvMassEta","Invariant mass Eta from Clusters",1000, 0., 1.);
   
 //---- Starting analysis ---- 
 int nEntries = chain->GetEntries () ;

 
 
 for (int entry = 0 ; entry < nEntries ; ++entry)
 {
  chain->GetEntry (entry) ;
  std::cout << "------> reading entry " << entry << " <------\n" ;
  
  //---- loop over etas ----
  for (int ii=0; ii<numEta_; ii++){
   //---- loop over photons generating an eta ----
   int counterPhotons = 0;
   math::XYZTLorentzVector p(0,0,0,0);
   for (int jj=0; jj<numPh_->at(ii); jj++){
    double px = pxPh_->at(counterPhotons);
    double py = pyPh_->at(counterPhotons);
    double pz = pzPh_->at(counterPhotons);
    double Energy = sqrt(px*px + py*py + pz*pz);
    math::XYZTLorentzVector p_temp(px,py,pz,Energy);
    p = p + p_temp;
    counterPhotons++;
   }
   double InvMass = p.mag();
   hInvMassEtaPh.Fill(InvMass);
  }
 }
 
 
 //---- save in file ---- 
 TFile saving (outputRootName.c_str (),"recreate") ;
 saving.cd () ;

 hInvMassEtaPh.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaPh.Write () ;

 saving.Close ();
 delete chain;
 return 0 ;

}


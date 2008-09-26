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
 
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>


#include "DataFormats/Math/interface/LorentzVector.h"


//---- root includes
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TChain.h"
#include "TFile.h"
#include "TRandom3.h"

#define PI 3.14159265
#define TWOPI 6.28318530


double deltaPhi(double phi1,double phi2) {
 double deltaphi = fabs(phi1-phi2);  
 if (deltaphi > TWOPI) deltaphi -= TWOPI;  
 if (deltaphi > PI ) deltaphi = TWOPI - deltaphi;  
 return deltaphi; 
}



double Eta2Theta(double Eta) {
 double Theta = 2. * atan(exp(-Eta));
 return Theta; 
}


//! main program
int main (int argc, char* argv[])
{
 //---- random ----
 TRandom *gRandom = new TRandom3(429);
 
 //---- output File ---- where saving histograms ---- 

 std::string outputRootName = "outputHistos.root";
 std::string inputType = "simplePhotonAnalyzer/tTreeUtilities";
 
 //---- input File ---- where finding input parameters ---- 
  
 //---- Input Variables ----
 double R = 0.2; //---- Radius to match MC and Cluster
 double dEta_C_Cut = 0.05; //---- dEta to match MC and Cluster
 double dPhi_C_Cut = 0.1; //---- dPhi to match MC and Cluster
 double S9oS25_MC_Cut = 0.9; //---- S9oS25 to match MC and Cluster
 double S4oS9_MC_Cut = 0.9; //---- S4oS9 to match MC and Cluster
 
 double S9oS25C_Cut = 0.85; //---- S4oS9C_-Cluster cut ----
 double S4oS9C_Cut = 0.85; //---- S4oS9C_-Cluster cut ----
 double ptC_Cut = 1.;      //---- pt-Cluster cut ----
 double ptC_EE_Cut = 1.;      //---- pt-Cluster cut ---- EndCap ----
 
 double ptSum_Cut = 2.;    //---- pt-coppia cut ---- 
 
 double RMax_Cut = 0.2;            //---- RMax_Cut cut ---- isolation ----
 double DeltaEtaMax_Cut = 0.05;    //---- DeltaEtaMax_Cut ---- isolation ----
 double et_Cut = 1.;               //---- et_Cut ---- isolation ----
 double iso_Cut = 0.4;             //---- iso/pt-coppia cut ---- isolation ----
 
 double angle_Cut = 0.105; //---- ~6° ---- Angle between two SC ----
 double angle_Min_Cut = 0.105; //---- minimum angle between two SC ----
 double angleR_Cut = 0.5; //---- ~6° ---- Angle between two SC in Eta/Phi plane ---- 
 
 double R_eta = 0.1; //---- summed cluster match with MC
 double dEta_eta = 0.05;
 double dPhi_eta = 0.1;
 double ptPh_Cut = 1.;      //---- pt-Photon cut ----
 
 int N_events = 10; //---- number of events to analize -> -1 = all ----
 
 int OmegaSearch = 0; //---- 0 -> no Omega ---- 1 -> si Omega ----
 int E3x3 = 0; //---- 0 -> basic cluster ---- 1 -> si Energy 3x3 ----
 
 double Scalib = -1; //---- percentuale di scalibrazione: -1 -> calibrato ----
 
 
 
 
 //---- loading cuts ----
 
 std::string fileName (argv[1]) ;
 boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName) ;
 boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
   
 edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("Selections") ;

 R = subPSetSelections.getUntrackedParameter<double> ("R",R) ;
 S4oS9C_Cut = subPSetSelections.getUntrackedParameter<double> ("S4oS9C_Cut",S4oS9C_Cut) ;
 S9oS25C_Cut = subPSetSelections.getUntrackedParameter<double> ("S9oS25C_Cut",S9oS25C_Cut) ;
 ptC_Cut = subPSetSelections.getUntrackedParameter<double> ("ptC_Cut",ptC_Cut) ;
 ptC_EE_Cut = subPSetSelections.getUntrackedParameter<double> ("ptC_EE_Cut",ptC_EE_Cut) ;
 ptSum_Cut = subPSetSelections.getUntrackedParameter<double> ("ptSum_Cut",ptSum_Cut) ;
 RMax_Cut = subPSetSelections.getUntrackedParameter<double> ("RMax_Cut",RMax_Cut) ;
 DeltaEtaMax_Cut = subPSetSelections.getUntrackedParameter<double> ("DeltaEtaMax_Cut",DeltaEtaMax_Cut) ;
 et_Cut = subPSetSelections.getUntrackedParameter<double> ("et_Cut",et_Cut) ;
 iso_Cut = subPSetSelections.getUntrackedParameter<double> ("iso_Cut",iso_Cut) ;
 angle_Cut = subPSetSelections.getUntrackedParameter<double> ("angle_Cut",angle_Cut) ;
 angle_Min_Cut = subPSetSelections.getUntrackedParameter<double> ("angle_Min_Cut",angle_Min_Cut) ;
 R_eta = subPSetSelections.getUntrackedParameter<double> ("R_eta",R_eta) ;
 N_events = subPSetSelections.getUntrackedParameter<int> ("N_events",N_events) ;
 dEta_C_Cut = subPSetSelections.getUntrackedParameter<double> ("dEta_C_Cut",dEta_C_Cut) ;
 dPhi_C_Cut = subPSetSelections.getUntrackedParameter<double> ("dPhi_C_Cut",dPhi_C_Cut) ;
 dEta_eta = subPSetSelections.getUntrackedParameter<double> ("dEta_eta",dEta_eta) ;
 dPhi_eta = subPSetSelections.getUntrackedParameter<double> ("dPhi_eta",dPhi_eta) ;
 ptPh_Cut = subPSetSelections.getUntrackedParameter<double> ("ptPh_Cut",ptPh_Cut) ;
 OmegaSearch = subPSetSelections.getUntrackedParameter<int> ("OmegaSearch",OmegaSearch) ;
 S9oS25_MC_Cut = subPSetSelections.getUntrackedParameter<double> ("S9oS25_MC_Cut",S9oS25_MC_Cut) ;
 S4oS9_MC_Cut = subPSetSelections.getUntrackedParameter<double> ("S4oS9_MC_Cut",S4oS9_MC_Cut) ;
 angleR_Cut = subPSetSelections.getUntrackedParameter<double> ("angleR_Cut",angleR_Cut) ;
 E3x3 = subPSetSelections.getUntrackedParameter<int> ("E3x3",E3x3) ;
 Scalib = subPSetSelections.getUntrackedParameter<double> ("Scalib",Scalib) ;
 
  
 //---- output file ----
 
 edm::ParameterSet subPSetOutput = parameterSet->getParameter<edm::ParameterSet> ("outputTree") ;
 outputRootName = subPSetOutput.getParameter<std::string> ("TreeName") ;

 
 //---- input file ----

 edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputTree") ;
 std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
 
 inputType = subPSetInput.getUntrackedParameter<std::string> ("inputType",inputType) ;
 
 TChain * chain = new TChain (inputType.c_str()) ;
 
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
  
 
 
 std::cerr << "---- End Tree initialization ---- " << std::endl; 
 
 int numberInput = 0;
 for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ; listIt != inputFiles.end () ; ++listIt) {
  numberInput++;
  chain->Add (listIt->c_str ()) ;
  if (numberInput%4 == 0) std::cerr << "Input number " << numberInput << " ... " << listIt->c_str () << std::endl;
 }

 
 
 std::cerr << "---- End Chain initialization ---- " << std::endl; 
 
 
 
 //---- Histograms ----
 
 //---- MC Analysis and MC matching ---- 
 TH1F hAngleMC("hAngleMC","MC Data. Angle between the two photons from eta",3600, 0., 2.*PI);
 TH1F hAngleEtaMC("hAngleEtaMC","MC Data. Angle Eta between the two photons from eta",1000,0.,10.);
 TH1F hAnglePhiMC("hAnglePhiMC","MC Data. Angle Phi between the two photons from eta",3600, 0., 2.*PI);
 TH1F hR_eta_C("hR_eta_C","Angle between reconstructed eta from BC without cuts and MC photons",1000, 0., 1.);
 TH1F hInvMassEtaPh("hInvMassEtaPh","Invariant mass Eta from MC Truth",10000, 0.547449999999, 0.547450000001);
 TH1F hInvMassEtaCMCTruth("hInvMassEtaCMCTruth","Invariant mass Eta from Clusters. Match with MC Truth",1000, 0., 1.);
 
 TH1F hAngleC_MCMatch("hAngleC_MCMatch","Angle between the two photons from BC and MC matching",3600, 0., 2.*PI);
 TH1F hNPhoton("hNPhoton","number of photons per eta from MC Truth",100, 0., 10.);
 TH1F hPhotonPt("hPhotonPt","Photon pt",10000, 0., 100.);
 TH1F hSinglePhotonPt("hSinglePhotonPt","Single Photon pt",1000, 0., 40.);
 TH1F hNEtaPerEvent("hNEtaPerEvent","number of etas per event",1000, 0., 1000.);
 
 TH1F hS9oS25MC_1("hS9oS25MC_1","MC Data. C_1 S9oS25",1000, 0., 10.);
 TH1F hS9oS25MC_2("hS9oS25MC_2","MC Data. C_2 S9oS25",1000, 0., 10.);

 TH1F hS4oS9MC_1("hS4oS9MC_1","MC Data. C_1 S4oS9",1000, 0., 10.);
 TH1F hS4oS9MC_2("hS4oS9MC_2","MC Data. C_2 S4oS9",1000, 0., 10.);

 
 TH2F hS9oS25VsEnergyCMC_1("hS9oS25VsEnergyCMC_1","MC Data. Invariant mass Eta two cluster selection S9oS25 C_1",1000, 0., 1.,1000, 0., 2.); 
 TH2F hS9oS25VsEnergyCMC_2("hS9oS25VsEnergyCMC_2","MC Data. Invariant mass Eta two cluster selection S9oS25 C_2",1000, 0., 1.,1000, 0., 2.); 
 TH2F hS9oS25VsEnergyPhMC_1("hS9oS25VsEnergyPhMC_1","MC Data. Invariant mass Eta two photons selection S9oS25 C_1",1000, 0., 1.,1000, 0., 2.); 
 TH2F hS9oS25VsEnergyPhMC_2("hS9oS25VsEnergyPhMC_2","MC Data. Invariant mass Eta two photons selection S9oS25 C_2",1000, 0., 1.,1000, 0., 2.); 

 TH2F hS4oS9VsEnergyCMC_1("hS4oS9VsEnergyCMC_1","MC Data. Invariant mass Eta two cluster selection S4oS9 C_1",1000, 0., 1.,1000, 0., 2.); 
 TH2F hS4oS9VsEnergyCMC_2("hS4oS9VsEnergyCMC_2","MC Data. Invariant mass Eta two cluster selection S4oS9 C_2",1000, 0., 1.,1000, 0., 2.); 
 TH2F hS4oS9VsEnergyPhMC_1("hS4oS9VsEnergyPhMC_1","MC Data. Invariant mass Eta two photons selection S4oS9 C_1",1000, 0., 1.,1000, 0., 2.); 
 TH2F hS4oS9VsEnergyPhMC_2("hS4oS9VsEnergyPhMC_2","MC Data. Invariant mass Eta two photons selection S4oS9 C_2",1000, 0., 1.,1000, 0., 2.); 
 
 TH2F hS4oS9VsS9oS25CMC_1("hS4oS9VsS9oS25CMC_1","MC Data. Cluster S4oS9 and S4oS9 C_1",1000, 0., 2.,1000, 0., 2.); 
 TH2F hS4oS9VsS9oS25CMC_2("hS4oS9VsS9oS25CMC_2","MC Data. Cluster S4oS9 and S4oS9 C_2",1000, 0., 2.,1000, 0., 2.); 
 
 TH2F hEtSumPh_MC("hEtSumPh_MC","MC Data. Et Sum two photons selection",2000, 0., 2.,1000, 0., 10.); 
 TH2F hEtSumC_MC("hEtSumC_MC","MC Data. Et Sum two cluster selection",2000, 0., 2.,1000, 0., 10.); 

 
  
 TH2F hE3x3VsReal_1_MC("hE3x3VsReal_1_MC","MC Data. First Cluster E single cluster selection E3x3 vs real energy",2000, 0., 20.,2000, 0., 20.); 
 TH2F hE3x3VsReal_2_MC("hE3x3VsReal_2_MC","MC Data. Second Cluster E single cluster selection E3x3 vs real energy",2000, 0., 20.,2000, 0., 20.); 
 TH2F hE3x3VsReal_MC("hE3x3VsReal_MC","MC Data. Cluster E single cluster selection E3x3 vs real energy",2000, 0., 20.,2000, 0., 20.); 
 
 TH2F hBCVsReal_1_MC("hBCVsReal_1_MC","MC Data. First Cluster E single cluster selection BC vs real energy",2000, 0., 20.,2000, 0., 20.); 
 TH2F hBCVsReal_2_MC("hBCVsReal_2_MC","MC Data. Second Cluster E single cluster selection BC vs real energy",2000, 0., 20.,2000, 0., 20.); 
 TH2F hBCVsReal_MC("hBCVsReal_MC","MC Data. Cluster E single cluster selection BC vs real energy",2000, 0., 20.,2000, 0., 20.); 
 
 TH1F hBCOverReal_MC("hBCOverReal_MC","MC Data. Cluster E single cluster selection BC over Real energy",3600, 0., 2.*PI);
 TH1F hE3x3OverReal_MC("hE3x3OverReal_MC","MC Data. Cluster E single cluster selection E3x3 over Real energy",1000, 0., 2.);
 
 
 //---- Data Analysis ---- before cuts -----
 TH1F hAngle("hAngle","Angle between two BC, any pair of 2 BC",3600, 0., 2.*PI);
 TH1F hInvMassEta2CNoCuts("hInvMassEta2CNoCuts","Invariant mass Eta two cluster selection. CUTS on the first cluster",1000, 0., 1.);  
 TH2F hInvMassEta2CETNoCuts("hInvMassEta2CETNoCuts","Invariant mass Eta two cluster selection Vs Et. No Cuts.",1000, 0., 1.,1000, 0., 10.); 
 TH2F hInvMassEta2CETNoCutsMC("hInvMassEta2CETNoCutsMC","MC match. Invariant mass Eta two cluster selection vs Et. No Cuts.",1000, 0., 1.,1000, 0., 10.); 
 TH2F h2CPtC1AndPtC2("h2CPtC1AndPtC2","PtC1 versus PtC2. No Cuts",1000, 0., 10.,1000, 0., 10.); 
 
  //---- Data Analysis ---- after cuts -----
 TH2F hInvMassEta2CAndS4oS9C("hInvMassEta2CAndS4oS9C","Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC.",1000, 0., 1.,1000, 0., 10.);
 TH2F hInvMassEta2CAndS9oS25("hInvMassEta2CAndS9oS25","Invariant mass Eta two cluster selection versus S9oS25. ALL BUT REALLY ALL CUT",1000, 0., 1.,1000, 0., 10.);

 TH2F hInvMassEta2CAndS4oS9CMC("hInvMassEta2CAndS4oS9CMC","MC match. Invariant mass Eta two cluster selection versus S4oS9. Cuts PtC.",1000, 0., 1.,1000, 0., 10.);
 
 TH3F hInvMassEta2CAndS4oS9CAndPtC("hInvMassEta2CAndS4oS9CAndPtC","Invariant mass Eta two cluster selection versus S4oS9 and PtC. Cuts PtC",100, 0., 1.,100, 0., 10.,100, 0., 10.); 
 TH2F hEta2CS4oS9CAndPtC("hEta2CS4oS9CAndPtC","S4oS9 C versus PtC. Cuts PtC",1000, 0., 10.,1000, 0., 10.); 
   
 TH2F hInvMassEta2CAndPtCMC("hInvMassEta2CAndPtCMC","MC match. Invariant mass Eta two cluster selection versus PtC. Cuts PtC and S4oS9.",1000, 0., 1.,1000, 0., 10.);   
 TH2F hInvMassEta2CAndPtC("hInvMassEta2CAndPtC","Invariant mass Eta two cluster selection versus PtC. Cuts PtC and S4oS9.",1000, 0., 1.,1000, 0., 10.); 

 TH2F hInvMassEta2CAndIsolationMC("hInvMassEta2CAndIsolationMC","MC match. Invariant mass Eta two cluster selection versus Iso/PtSum. Cut ON",1000, 0., 1.,1000, 0., 3.);
 TH2F hInvMassEta2CAndIsolation("hInvMassEta2CAndIsolation","Invariant mass Eta two cluster selection versus Iso/PtSum. Cut ON",1000, 0., 1.,1000, 0., 3.);
   
 TH1F hIsolation("hIsolation","Isolation after all other cuts",1000, 0., 5.);
 TH1F hIsolationMC("hIsolationMC","MC match. Isolation after all other cuts",1000, 0., 5.);  
  
 TH1F hAngleC("hAngleC","Angle between two BC. After cuts on the first cluster",3600, 0., 2.*PI);
 
 TH1F hInvMassEta2CMC("hInvMassEta2CMC","MC match. Invariant mass Eta two cluster selection. After ALL CUTS",1000, 0., 1.); 
 //-------------------- Final Graph --------------------
 TH1F hInvMassEta2C("hInvMassEta2C","Invariant mass Eta two cluster selection. After ALL CUTS",1000, 0., 1.);
 TH1F hInvMassEta2CEE("hInvMassEta2CEE","Invariant mass Eta two cluster selection. After ALL CUTS. Endcap - Endcap",1000, 0., 1.); 
 TH1F hInvMassEta2CBB("hInvMassEta2CBB","Invariant mass Eta two cluster selection. After ALL CUTS Barrel - Barrel",1000, 0., 1.); 
 TH1F hInvMassEta2CEB("hInvMassEta2CEB","Invariant mass Eta two cluster selection. After ALL CUTS. Endcap - Barrel",1000, 0., 1.); 
 TH1F hInvMassEta2C_MinAngleCut("hInvMassEta2C_MinAngleCut","Invariant mass Eta two cluster selection. After ALL CUTS. Min angle cut",1000, 0., 1.);
 //-----------------------------------------------------
 TH1F hAngleEnd("hAngleEnd","Angle between two BC. After ALL cuts",3600, 0., 2.*PI);
 TH1F hAngleREnd("hAngleREnd","Angle R (eta,phi) between two BC. After ALL cuts",3600, 0., 2.*PI);

 
 TH2F hInvMassEtaVsAngle("hInvMassEtaVsAngle","Invariant mass Eta two cluster selection versus Angle (rad). Cut ON",2000, 0., 2.,1000, 0., 6.28);
  
 TH2F hInvMassEtaVsAngleR2C("hInvMassEtaVsAngleR2C","Invariant mass Eta two cluster selection versus AngleR. Cut ON",2000, 0., 2.,1000, 0., 3.);
 
 TH2F hInvMassEtaVsAngleR2C_NoCut("hInvMassEtaVsAngleR2C_NoCut","Invariant mass Eta two cluster selection versus AngleR. Cut OFF. 1 C cut",2000, 0., 2.,1000, 0., 3.);
 TH1F hInvMassEta2C_Trial("hInvMassEta2C_Trial","Invariant mass Eta two cluster selection. After ALL CUTS. Trial R > R_cut",1000, 0., 1.);
 TH1F hInvMassPi02C_Trial("hInvMassPi02C_Trial","Invariant mass Pi0 two cluster selection. After ALL CUTS. Trial R < R_cut",1000, 0., 1.);
 TH1F hInvMassOmegaC("hInvMassOmegaC","Invariant mass Omega three cluster selection.",4000, 0., 5.); 
 
 TH1F hR_2C_Eta_MC("hR_2C_Eta_MC","MC Data. Angle R (eta,phi) between two photons. After Eta direction cut",3600, 0., 2.*PI);
 TH1F hR_2C_MC("hR_2C_MC","MC Data. Angle R (eta,phi) between two photons.",3600, 0., 2.*PI);
   
 TH2F hPtSumVsInvMass("hPtSumVsInvMass","Invariant mass Eta two cluster selection versus PtSum. Cut On",2000, 0., 2.,4000, 0., 10.);
 
 TH2F hInvMassVsInvMass("hInvMassVsInvMass","Invariant mass Eta two cluster BC versus E3x3.",2000, 0., 2.,2000, 0., 2.);
 
 TH2F hNumHitVsAngle("hNumHitVsAngle","Num Xtal vs Angle",100, 0., 6.28,1000, 0., 100);
 TH2F hNumHitVsAngleR("hNumHitVsAngleR","Num Xtal vs Angle R",100, 0., 10,1000, 0., 100);
 TH2F hNumHitVsAngleR_Cut("hNumHitVsAngleR_Cut","Num Xtal vs Angle R After Cuts",1000, 0., 10.,1000, 0., 100);

 
 //---- Input Variables Loaded ----
 std::cerr << "Input Variables Loaded: " << std::endl; 
 std::cerr << " R = " << R << std::endl; 
 std::cerr << " S4oS9_MC_Cut = " << S4oS9_MC_Cut << std::endl; 
 std::cerr << " S9oS25_MC_Cut = " << S9oS25_MC_Cut << std::endl; 
 std::cerr << " S9oS25C_Cut = " << S9oS25C_Cut << std::endl; 
 std::cerr << " S4oS9C_Cut = " << S4oS9C_Cut << std::endl; 
 std::cerr << " ptC_Cut = " << ptC_Cut << std::endl; 
 std::cerr << " ptC_EE_Cut = " << ptC_EE_Cut << std::endl;
 std::cerr << " ptSum_Cut = " << ptSum_Cut << std::endl; 
 std::cerr << " RMax_Cut = " << RMax_Cut << std::endl; 
 std::cerr << " DeltaEtaMax_Cut = " << DeltaEtaMax_Cut << std::endl; 
 std::cerr << " et_Cut = " << et_Cut << std::endl; 
 std::cerr << " iso_Cut = " << iso_Cut << std::endl;
 std::cerr << " angle_Cut = " << angle_Cut << std::endl;
 std::cerr << " angle_Min_Cut = " << angle_Min_Cut << std::endl;
 std::cerr << " R_eta = " << R_eta << std::endl;
 std::cerr << " N_events = " << N_events << std::endl;
 std::cerr << " dEta_C_Cut = " << dEta_C_Cut << std::endl;
 std::cerr << " dPhi_C_Cut = " << dPhi_C_Cut << std::endl;
 std::cerr << " dEta_eta = " << dEta_eta << std::endl;
 std::cerr << " dPhi_eta = " << dPhi_eta << std::endl;
 std::cerr << " ptPh_Cut = " << ptPh_Cut << std::endl;
 std::cerr << " OmegaSearch = " << OmegaSearch << std::endl;
 std::cerr << " angleR_Cut = " << angleR_Cut << std::endl;
 std::cerr << " E3x3 = " << E3x3 << std::endl;
 std::cerr << " Scalib = " << Scalib << std::endl;
 std::cerr << " outputRootName = " << outputRootName << std::endl;
 
 
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
  hNEtaPerEvent.Fill(numEta_);
  
  //---- calculate number of photons ----
  int numberPhotons = 0;
  for (int ii=0; ii<numEta_; ii++){
   numberPhotons += numPh_->at(ii);
  }
  //---- end calculate number of photons ----
  
  //---- loop over etas ----
  int counterPhotons = 0;
  for (int ii=0; ii<numEta_; ii++){
   flag2Photons = false;
   FirstCluster = -1;
   SecondCluster = -1;
   
   math::XYZTLorentzVector pPh(0,0,0,0);
   math::XYZTLorentzVector pC(0,0,0,0);
   
//    std::cerr << "Uno" << std::endl;
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
//     std::cerr << "Due" << std::endl;
    double pxPh = pxPh_->at(counterPhotons);
    double pyPh = pyPh_->at(counterPhotons);
    double pzPh = pzPh_->at(counterPhotons);
    double ptPh = sqrt(pxPh * pxPh + pyPh * pyPh);
    double EnergyPh = sqrt(pxPh*pxPh + pyPh*pyPh + pzPh*pzPh);
    math::XYZTLorentzVector pPh_temp(pxPh,pyPh,pzPh,EnergyPh);
    double etaPh = etaPh_->at(counterPhotons);
    double phiPh = phiPh_->at(counterPhotons);
    pPh = pPh + pPh_temp;
    hSinglePhotonPt.Fill(ptPh);
    double bestTestValueRCQ = 100000;
    
    //---- loop over clusters ----
    for (int kk=0; kk<numC_; kk++){
     double etaC1 = etaC_->at(kk);
     double phiC1 = phiC_->at(kk);
     double thetaC1 = Eta2Theta(etaC1);
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
      else { //---- second photon ---- jj==1 ----
       if ((bestTestValueRCQ > RC1Q) && (fabs(etaPh - etaC1) < dEta_C_Cut) && (deltaPhi(phiPh,phiC1) < dPhi_C_Cut) && (ptPh > ptPh_Cut)) {
        SecondCluster = kk; //---- if I find a third photon ? --> Never found :D --> if it happens throw away eta event -> flag2Photons = false
        counterPh2= counterPhotons;
        bestTestValueRCQ = RC1Q;
        flagSecondPhoton = true;
        if (flagFirstPhoton) flag2Photons = true; //---- found 2 photons ----
       }
      }
      
      double pxC;
      double pyC;
      double pzC;
      
      if (E3x3 == 1) {
       double energy_temp = S9C_->at(kk);
       pxC = energy_temp * sin(thetaC1) * cos(phiC1);
       pyC = energy_temp * sin(thetaC1) * sin(phiC1);
       pzC = energy_temp * cos(thetaC1);
      }
      else {
       if (Scalib > 0) {
        double energy_temp = 0;
        int counterXtal = 0;
        for (int ww=0; ww < numC_; ww++){
         for (int ihit = 0; ihit < HitsC_->at(ww); ihit++){
          if (ww == kk) {
           double EnHit = HitsEnergyC_->at(counterXtal); 
           EnHit = EnHit * (gRandom->Gaus(1.,Scalib)); // ---- "manual" scalibration ----
           energy_temp = energy_temp + EnHit;
          }
          counterXtal++;
         }
        }
        pxC = energy_temp * sin(thetaC1) * cos(phiC1);
        pyC = energy_temp * sin(thetaC1) * sin(phiC1);
        pzC = energy_temp * cos(thetaC1);
       }
       else {
        pxC= pxC_->at(kk);
        pyC = pyC_->at(kk);
        pzC = pzC_->at(kk);
       }
      }
      double EnergyC = sqrt(pxC*pxC + pyC*pyC + pzC*pzC);
      math::XYZTLorentzVector pC_temp(pxC,pyC,pzC,EnergyC);
      pC = pC + pC_temp;
     }//---- end found a cluster near a photon ----
    }//---- end loop clusters ----
    counterPhotons++;
   }//---- end loop photons generating an eta ----
   
   //---- Isolation information ----
   if (flag2Photons){
//     std::cerr << "Quattro -> counterPh1 = " << counterPh1 << " counterPh2 = " << counterPh2 << " NumPh = " << numPh_->at(ii) << " NumTotPh = " << numberPhotons << std::endl;
    double InvMassPh = pPh.mag();
    double InvMassC = pC.mag();
    
    double S4oS9_MC_1 = S4C_->at(FirstCluster) / S9C_->at(FirstCluster);
    double S4oS9_MC_2 = S4C_->at(SecondCluster) / S9C_->at(SecondCluster);
    hS4oS9MC_1.Fill(S4oS9_MC_1);
    hS4oS9MC_2.Fill(S4oS9_MC_2);
    hS4oS9VsEnergyCMC_1.Fill(InvMassC,S4oS9_MC_1);
    hS4oS9VsEnergyPhMC_1.Fill(InvMassPh,S4oS9_MC_1);
    hS4oS9VsEnergyCMC_2.Fill(InvMassC,S4oS9_MC_2);
    hS4oS9VsEnergyPhMC_2.Fill(InvMassPh,S4oS9_MC_2);
    
//     std::cerr << "Quattro Tris " << std::endl;
      
    double S9oS25_MC_1 = S9C_->at(FirstCluster) / S25C_->at(FirstCluster);
    double S9oS25_MC_2 = S9C_->at(SecondCluster) / S25C_->at(SecondCluster);
    hS9oS25MC_1.Fill(S9oS25_MC_1);
    hS9oS25MC_2.Fill(S9oS25_MC_2);
    hS9oS25VsEnergyCMC_1.Fill(InvMassC,S9oS25_MC_1);
    hS9oS25VsEnergyPhMC_1.Fill(InvMassPh,S9oS25_MC_1);
    hS9oS25VsEnergyCMC_2.Fill(InvMassC,S9oS25_MC_2);
    hS9oS25VsEnergyPhMC_2.Fill(InvMassPh,S9oS25_MC_2);

    hS4oS9VsS9oS25CMC_1.Fill(S4oS9_MC_1,S9oS25_MC_1);
    hS4oS9VsS9oS25CMC_2.Fill(S4oS9_MC_2,S9oS25_MC_2);
      
//     std::cerr << "Cinque" << std::endl;
    if ((S9oS25_MC_1 > S9oS25_MC_Cut) && (S9oS25_MC_2 > S9oS25_MC_Cut) && (S4oS9_MC_2 > S4oS9_MC_Cut) && (S4oS9_MC_1 > S4oS9_MC_Cut)) flag2Photons = true;
    else flag2Photons = false;
   }
   //---- end Isolation information ----
   
   
   
   
   if (numPh_->at(ii) != 2) flag2Photons = false; //---- I do NOT want != 2 photons events ----
   
   bool flagEtaDirection = true;
   if (flag2Photons){
//     std::cerr << "Sei" << std::endl;
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
    double R_eta_2C = sqrt((etaPh1-etaPh2)*(etaPh1-etaPh2) + deltaPhi(phiPh1,phiPh2) * deltaPhi(phiPh1,phiPh2));
    hR_eta_C.Fill(sqrt(R_etaQ_C));
    hR_2C_MC.Fill(sqrt(R_eta_2C));
    if (flagEtaDirection) hR_2C_Eta_MC.Fill(sqrt(R_eta_2C));
   
    hEtSumPh_MC.Fill(pPh_Eta.mag(),pPh_Eta.Pt());
    hEtSumC_MC.Fill(pC.mag(),pC.Pt());
        
    
    
    if (flagEtaDirection){
     
     //---- BC ----
     //---- first cluster ----
     double pxC1_temp_MC = pxC_->at(FirstCluster);
     double pyC1_temp_MC = pyC_->at(FirstCluster);
     double pzC1_temp_MC = pzC_->at(FirstCluster);
     double EnergyC1_temp_MC = sqrt(pxC1_temp_MC*pxC1_temp_MC + pyC1_temp_MC*pyC1_temp_MC + pzC1_temp_MC*pzC1_temp_MC);
//     math::XYZTLorentzVector pC1_temp_MC(pxC1_temp_MC,pyC1_temp_MC,pzC1_temp_MC,EnergyC1_temp_MC);
   //---- Second cluster ----
     double pxC2_temp_MC = pxC_->at(SecondCluster);
     double pyC2_temp_MC = pyC_->at(SecondCluster);
     double pzC2_temp_MC = pzC_->at(SecondCluster);
     double EnergyC2_temp_MC = sqrt(pxC2_temp_MC*pxC2_temp_MC + pyC2_temp_MC*pyC2_temp_MC + pzC2_temp_MC*pzC2_temp_MC);
//     math::XYZTLorentzVector pC2_temp_MC(pxC2_temp_MC,pyC2_temp_MC,pzC2_temp_MC,EnergyC2_temp_MC);
    
     hBCVsReal_1_MC.Fill(EnergyC1_temp_MC,EnergyPh1);
     hBCVsReal_2_MC.Fill(EnergyC2_temp_MC,EnergyPh2);
     
     hBCVsReal_MC.Fill(EnergyC1_temp_MC,EnergyPh1);
     hBCVsReal_MC.Fill(EnergyC2_temp_MC,EnergyPh2);
    
     if (EnergyPh1 >0) hBCOverReal_MC.Fill(EnergyC1_temp_MC/EnergyPh1);
     if (EnergyPh2 >0) hBCOverReal_MC.Fill(EnergyC2_temp_MC/EnergyPh2);
     
     //---- E3x3 ----
     //---- first cluster ----
     double EnergyE3x31_temp_MC = S9C_->at(FirstCluster);
   //---- Second cluster ----
     double EnergyE3x32_temp_MC = S9C_->at(SecondCluster);
    
     hE3x3VsReal_1_MC.Fill(EnergyE3x31_temp_MC,EnergyPh1);
     hE3x3VsReal_2_MC.Fill(EnergyE3x32_temp_MC,EnergyPh2);
     
     hE3x3VsReal_MC.Fill(EnergyE3x31_temp_MC,EnergyPh1);
     hE3x3VsReal_MC.Fill(EnergyE3x32_temp_MC,EnergyPh2); 
     
     if (EnergyPh1 >0) hE3x3OverReal_MC.Fill(EnergyE3x31_temp_MC/EnergyPh1);
     if (EnergyPh2 >0) hE3x3OverReal_MC.Fill(EnergyE3x32_temp_MC/EnergyPh2);
    }
    
    
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
  if (entry%1000 == 0) std::cout << "------> No MC Truth ---- reading entry " << entry << " <------\n" ;
  
  std::vector<int> numC_Selected;
  math::XYZTLorentzVector pC(0,0,0,0);
 
  int numberC1;
  int numberC2;
  double EnergyPair;
  bool flagMCMatch = false;
  double angle_C;
  
  //---- loop over clusters ----
  for (int kk=0; kk<numC_; kk++){
   
   double etaC1 = etaC_->at(kk);
   double phiC1 = phiC_->at(kk);
   double thetaC1 = Eta2Theta(etaC1);
   double pxC1;
   double pyC1;
   double pzC1;
   
   if (E3x3 == 1) {
    double energy_temp = S9C_->at(kk);
    pxC1 = energy_temp * sin(thetaC1) * cos(phiC1);
    pyC1 = energy_temp * sin(thetaC1) * sin(phiC1);
    pzC1 = energy_temp * cos(thetaC1);
   }
   else {
    if (Scalib > 0) {
     double energy_temp = 0;
     int counterXtal = 0;
     for (int ww=0; ww < numC_; ww++){
      for (int ihit = 0; ihit < HitsC_->at(ww); ihit++){
       if (ww == kk) {
        double EnHit = HitsEnergyC_->at(counterXtal); 
        EnHit = EnHit * (gRandom->Gaus(1.,Scalib)); // ---- "manual" scalibration ----
        energy_temp = energy_temp + EnHit;
       }
       counterXtal++;
      }
     }
     pxC1 = energy_temp * sin(thetaC1) * cos(phiC1);
     pyC1 = energy_temp * sin(thetaC1) * sin(phiC1);
     pzC1 = energy_temp * cos(thetaC1);
    }
    else {
     pxC1 = pxC_->at(kk);
     pyC1 = pyC_->at(kk);
     pzC1 = pzC_->at(kk);
    }
   }
   
   double ptC1 = sqrt(pxC1*pxC1 + pyC1*pyC1);
   double EnergyC1 = sqrt(pxC1*pxC1 + pyC1*pyC1 + pzC1*pzC1);
   double EnergyTC1 = sqrt(pxC1*pxC1 + pyC1*pyC1);
   
   numberC1 = kk;
   math::XYZTLorentzVector pC2C_1(pxC1,pyC1,pzC1,EnergyC1);
   
   
   //---- cuts as soon as possible ----
   if ((etaC1*etaC1 > 1.560*1.560 && ptC1 > ptC_EE_Cut) || (etaC1*etaC1 < 1.4442*1.4442 && ptC1 > ptC_Cut)) {
    if (S4oS9C_->at(numberC1) > S4oS9C_Cut) {
   
   //---- inner loop over cluster ----
     for (int ll=kk+1; ll<numC_; ll++){ //---- inizio da kk così non rischio di avere doppioni ----
      double etaC2 = etaC_->at(ll);
      double phiC2 = phiC_->at(ll);
      double thetaC2 = Eta2Theta(etaC2);
      double pxC2;
      double pyC2;
      double pzC2;
      if (E3x3 == 1) {
       double energy_temp = S9C_->at(ll);
       pxC2 = energy_temp * sin(thetaC2) * cos(phiC2);
       pyC2 = energy_temp * sin(thetaC2) * sin(phiC2);
       pzC2 = energy_temp * cos(thetaC2);
      }
      else {
       if (Scalib > 0) {
        double energy_temp = 0;
        int counterXtal = 0;
        for (int ww=0; ww < numC_; ww++){
         for (int ihit = 0; ihit < HitsC_->at(ww); ihit++){
          if (ww == ll) {
           double EnHit = HitsEnergyC_->at(counterXtal); 
           EnHit = EnHit * (gRandom->Gaus(1.,Scalib)); // ---- "manual" scalibration ----
           energy_temp = energy_temp + EnHit;
          }
          counterXtal++;
         }
        }
        pxC2 = energy_temp * sin(thetaC2) * cos(phiC2);
        pyC2 = energy_temp * sin(thetaC2) * sin(phiC2);
        pzC2 = energy_temp * cos(thetaC2);
       }
       else {
        pxC2 = pxC_->at(ll);
        pyC2 = pyC_->at(ll);
        pzC2 = pzC_->at(ll);
       }
      }
     
      double ptC2 = sqrt(pxC2*pxC2 + pyC2*pyC2);
      double EnergyC2 = sqrt(pxC2*pxC2 + pyC2*pyC2 + pzC2*pzC2);
      double EnergyTC2 = sqrt(pxC2*pxC2 + pyC2*pyC2);
      math::XYZTLorentzVector pC2C_2(pxC2,pyC2,pzC2,EnergyC2);
     
      math::XYZTLorentzVector pCsum = pC2C_1 + pC2C_2;
      numberC2 = ll;
      double etaSum = pCsum.eta();
      double phiSum = pCsum.phi();
      double pxSum = pCsum.x();
      double pySum = pCsum.y();
      double pzSum = pCsum.z();
      double ptSum = sqrt(pxSum * pxSum + pySum * pySum);
      double EnergySum = sqrt(pxSum * pxSum + pySum * pySum + pzSum * pzSum);
      EnergyPair = pCsum.mag();
     
      double R_forHisto = sqrt((etaC1-etaC2)*(etaC1-etaC2) + deltaPhi(phiC1,phiC2) * deltaPhi(phiC1,phiC2));
      hInvMassEtaVsAngleR2C_NoCut.Fill(R_forHisto,EnergyPair);
     
     //---- check if cluster pair correspond to a MC photon pair from eta ----
      for (int entryMC = 0 ; entryMC < nEntriesMC ; ++entryMC){
       ClusterPairCMC.GetEntry(entryMC);
       if (numEntryAfter == entry) {
        if (((FirstCluster == ll) && (SecondCluster == kk)) || ((FirstCluster == kk) && (SecondCluster == ll))) flagMCMatch  = true;
        else flagMCMatch = false; 
       }
      }
     
     
     
     //--------------
     //---- Cuts ----
     //--------------
     
     //---- some graphs before cuts ----
      hInvMassEta2CNoCuts.Fill(EnergyPair);
      hInvMassEta2CETNoCuts.Fill(EnergyPair,ptC1); //---- save only first cluster -> second cluster selected after ----
      if (flagMCMatch) { //---- save only first cluster -> second cluster selected after ----
       hInvMassEta2CETNoCutsMC.Fill(EnergyPair,ptC1);
      }
      h2CPtC1AndPtC2.Fill(ptC1,ptC2);
    //---- end some graphs before cuts ----
    
    //---- add cuts ----
    //---- cut in pair -> both Clusters ----
     
     //---- angle ----
      double cosAngle = pxC2 * pxC1 + pyC2 * pyC1 + pzC2 * pzC1;
      if (EnergyC2!=0 && EnergyC1!=0) cosAngle = cosAngle / (EnergyC2 * EnergyC1);
      else cosAngle = 0; //---- -> 90° -> rejected ----
      angle_C = acos(cosAngle);
      hAngle.Fill(angle_C);
      if (flagMCMatch) hAngleC_MCMatch.Fill(angle_C);
      hAngleC.Fill(angle_C); //---- cut on angle has been performed in pair filling ----
      
      hNumHitVsAngleR.Fill(R_forHisto,HitsC_->at(numberC1));
      hNumHitVsAngle.Fill(angle_C,HitsC_->at(numberC1));
      
      
       //---- Angle cut ----
      if (angle_C < angle_Cut) {
    //---- pt-Cluster cut Cluster 1 and Cluster 2 ----
       if ((etaC2*etaC2 > 1.560*1.560 && ptC2 > ptC_EE_Cut) || (etaC2*etaC2 < 1.4442*1.4442 && ptC2 > ptC_Cut)) {

        if (flagMCMatch) hInvMassEta2CAndS4oS9CMC.Fill(EnergyPair,S4oS9C_->at(numberC1));
        hInvMassEta2CAndS4oS9C.Fill(EnergyPair,S4oS9C_->at(numberC1));
     
        hInvMassEta2CAndS4oS9CAndPtC.Fill(EnergyPair,S4oS9C_->at(numberC1),ptC1);
        hEta2CS4oS9CAndPtC.Fill(S4oS9C_->at(numberC1),ptC1);
     
     
     //---- S4oS9C_-Cluster cut Cluster 1 and Cluster 2 ----
        if (S4oS9C_->at(numberC2) > S4oS9C_Cut){
      
         if (flagMCMatch) hInvMassEta2CAndPtCMC.Fill(EnergyPair,ptC1);
         hInvMassEta2CAndPtC.Fill(EnergyPair,ptC1);
      
         hPtSumVsInvMass.Fill(EnergyPair,ptSum);
        
     //---- ptSum cut ----
         if (ptSum > ptSum_Cut){
         //---- Isolatio cut ----
          double iso = 0;
          for (int pp=0; pp<numC_; pp++){
           if ((pp!=numberC1) && (pp!=numberC2)) {
            double etaCIso = etaC_->at(pp);
            double phiCIso = phiC_->at(pp);
            double R = sqrt((etaCIso-etaSum)*(etaCIso-etaSum) + deltaPhi(phiCIso,phiSum) * deltaPhi(phiCIso,phiSum));
            double deta = fabs(etaCIso - etaSum);
            double et = etC_->at(pp);
            if ( (R < RMax_Cut)  && (deta < DeltaEtaMax_Cut) && (et > et_Cut)  ) iso = iso + et ;
           }
          }
        
          if (flagMCMatch) hInvMassEta2CAndIsolationMC.Fill(EnergyPair,iso/ptSum);
          hInvMassEta2CAndIsolation.Fill(EnergyPair,iso/ptSum);
          if (flagMCMatch) hIsolationMC.Fill(iso/ptSum);
          hIsolation.Fill(iso/ptSum);
        
          if ((iso/ptSum) < iso_Cut) {
         
           hInvMassEta2CAndS9oS25.Fill(EnergyPair,S9C_->at(numberC1) / S25C_->at(numberC1));    
         //---- cut S9oS25 ----
           if ((S9C_->at(numberC1) / S25C_->at(numberC1) > S9oS25C_Cut) && (S9C_->at(numberC2) / S25C_->at(numberC2) > S9oS25C_Cut))
           {
            hAngleEnd.Fill(angle_C);
//           double R_forHisto = sqrt((etaC1-etaC2)*(etaC1-etaC2) + deltaPhi(phiC1,phiC2) * deltaPhi(phiC1,phiC2));
            hAngleREnd.Fill(R_forHisto);
          
            hNumHitVsAngleR_Cut.Fill(R_forHisto,HitsC_->at(numberC1));
            if (flagMCMatch) hInvMassEta2CMC.Fill(EnergyPair);
            if (R_forHisto > angleR_Cut) hInvMassEta2C_Trial.Fill(EnergyPair);
            else hInvMassPi02C_Trial.Fill(EnergyPair);
            hInvMassEtaVsAngleR2C.Fill(R_forHisto,EnergyPair);
          
            hInvMassEtaVsAngle.Fill(angle_C,EnergyPair);
            if (angle_C > angle_Min_Cut) {
             hInvMassEta2C_MinAngleCut.Fill(EnergyPair);
            }
          
            //---- Inv Mass with the other method ----
            
            double etaC1_temp = etaC_->at(numberC1);
            double phiC1_temp = phiC_->at(numberC1);
            double thetaC1_temp = Eta2Theta(etaC1_temp);
            double pxC1_temp;
            double pyC1_temp;
            double pzC1_temp;
   
            if (E3x3 == 1) {
             pxC1_temp = pxC_->at(numberC1);
             pyC1_temp = pyC_->at(numberC1);
             pzC1_temp = pzC_->at(numberC1);
            }
            else {
             double energy_temp = S9C_->at(numberC1);
             pxC1_temp = energy_temp * sin(thetaC1_temp) * cos(phiC1_temp);
             pyC1_temp = energy_temp * sin(thetaC1_temp) * sin(phiC1_temp);
             pzC1_temp = energy_temp * cos(thetaC1_temp);
            }
   
            double EnergyC1_temp = sqrt(pxC1_temp*pxC1_temp + pyC1_temp*pyC1_temp + pzC1_temp*pzC1_temp);
   
            math::XYZTLorentzVector pC2C_1_temp(pxC1_temp,pyC1_temp,pzC1_temp,EnergyC1_temp);
   
               
            double etaC2_temp = etaC_->at(numberC2);
            double phiC2_temp = phiC_->at(numberC2);
            double thetaC2_temp = Eta2Theta(etaC2);
            double pxC2_temp;
            double pyC2_temp;
            double pzC2_temp;
            if (E3x3 == 1) {
             pxC2_temp = pxC_->at(numberC2);
             pyC2_temp = pyC_->at(numberC2);
             pzC2_temp = pzC_->at(numberC2);
            }
            else {
             double energy_temp = S9C_->at(numberC2);
             pxC2_temp = energy_temp * sin(thetaC2_temp) * cos(phiC2_temp);
             pyC2_temp = energy_temp * sin(thetaC2_temp) * sin(phiC2_temp);
             pzC2_temp = energy_temp * cos(thetaC2_temp);
            }
            double EnergyC2_temp = sqrt(pxC2_temp*pxC2_temp + pyC2_temp*pyC2_temp + pzC2_temp*pzC2_temp);
            math::XYZTLorentzVector pC2C_2_temp(pxC2_temp,pyC2_temp,pzC2_temp,EnergyC2_temp);
     
            math::XYZTLorentzVector pCsum_temp = pC2C_1_temp + pC2C_2_temp;

            double EnergyPair_temp = pCsum_temp.mag();
            
            hInvMassVsInvMass.Fill(EnergyPair,EnergyPair_temp);  
            //---- end Inv Mass with the other method ----
            
            hInvMassEta2C.Fill(EnergyPair);
         //---- Barrel - Barrel ----
            if (fabs(etaC2) < 1.49 && fabs(etaC1) < 1.49) hInvMassEta2CBB.Fill(EnergyPair);
         //---- Endcap - Barrel ----
            if ((fabs(etaC2) < 1.49 && fabs(etaC1) > 1.49) || (fabs(etaC2) > 1.49 && fabs(etaC1) < 1.49)) hInvMassEta2CEB.Fill(EnergyPair);
         //---- Endcap - Endcap ----
            if (fabs(etaC2) > 1.49 && fabs(etaC1) > 1.49) hInvMassEta2CEE.Fill(EnergyPair);
         
          
          
          //---- search for Omega peak ----
            if (OmegaSearch != 0){
             for (int zz=0; zz<numC_; zz++){
              if ((zz!=numberC1) && (zz!=numberC2)) {
               double etaC3 = etaC_->at(zz);
               double phiC3 = phiC_->at(zz);
               double thetaC3 = Eta2Theta(etaC3);
               double pxC3;
               double pyC3;
               double pzC3;
               if (E3x3 == 1) {
                double energy_temp = S9C_->at(zz);
                pxC3 = energy_temp * sin(thetaC3) * cos(phiC3);
                pyC3 = energy_temp * sin(thetaC3) * sin(phiC3);
                pzC3 = energy_temp * cos(thetaC3);
               }
               else {
                pxC3 = pxC_->at(zz);
                pyC3 = pyC_->at(zz);
                pzC3 = pzC_->at(zz);
               }
               double ptC3 = sqrt(pxC3*pxC3 + pyC3*pyC3);
               double EnergyC3 = sqrt(pxC3*pxC3 + pyC3*pyC3 + pzC3*pzC3);
               double EnergyTC3 = sqrt(pxC3*pxC3 + pyC3*pyC3);
               if (R_forHisto < 0.1){ //---- if the first two cluster are near ----
                if (ptC3 > ptC_Cut/5.){
                 if (S4oS9C_->at(zz) > S4oS9C_Cut){
                  if (S9C_->at(zz) / S25C_->at(zz) > S9oS25C_Cut) {
                   math::XYZTLorentzVector pC2C_3(pxC3,pyC3,pzC3,EnergyC3);
                   math::XYZTLorentzVector pC_Omega = pCsum + pC2C_3;
                   double EnergyOmega = pC_Omega.mag();
                   hInvMassOmegaC.Fill(EnergyOmega);
                  }
                 }
                }
               }
              }
             }
            }//---- end search for Omega peak ----
           }//---- end cut S9oS25 ----
          }//---- end Isolatio cut ---- 
         }//---- end ptSum cut  ----
        }//---- end S4oS9C_-Cluster cut  Cluster 2 ----
       }//---- end pt-Cluster cut Cluster 2 ----
      }//---- end cut Angle cut ----
     }//---- end inner loop over cluster ----
    }//---- end S4oS9C_-Cluster cut Cluster 1 ----
   }//---- end pt-Cluster cut Cluster 1 ---- 
  }//---- end loop clusters ----
 }//---- end loop entries ----
 
 
 //---- save in file ---- 
 TFile saving (outputRootName.c_str (),"recreate") ;
 saving.cd () ;

 hInvMassOmegaC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassOmegaC.Write();
 
 hInvMassEtaPh.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaPh.Write();
 
 hInvMassEtaCMCTruth.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEtaCMCTruth.Write();
 
 hInvMassEta2C.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2C.Write();
  
 hInvMassEta2CEE.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CEE.Write();

 hInvMassEta2CBB.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CBB.Write();

 hInvMassEta2CEB.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CEB.Write();

 
 hInvMassEta2CNoCuts.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CNoCuts.Write();
 
 
 hNumHitVsAngle.GetYaxis()->SetTitle("num Xtal");
 hNumHitVsAngle.GetXaxis()->SetTitle("Angle (rad)");
 hNumHitVsAngle.Write();

 hNumHitVsAngleR.GetYaxis()->SetTitle("num Xtal");
 hNumHitVsAngleR.GetXaxis()->SetTitle("Angle R (eta/phi)");
 hNumHitVsAngleR.Write();
 
 hNumHitVsAngleR_Cut.GetYaxis()->SetTitle("num Xtal");
 hNumHitVsAngleR_Cut.GetXaxis()->SetTitle("Angle R (eta/phi)");
 hNumHitVsAngleR_Cut.Write();
 
  
  
 if (E3x3 == 1) {
  hInvMassVsInvMass.GetYaxis()->SetTitle("Invariant Mass BC (GeV)");
  hInvMassVsInvMass.GetXaxis()->SetTitle("Invariant Mass E3x3(GeV)");
 }
 else {
  hInvMassVsInvMass.GetYaxis()->SetTitle("Invariant Mass E3x3 (GeV)");
  hInvMassVsInvMass.GetXaxis()->SetTitle("Invariant Mass BC (GeV)");
 }
 hInvMassVsInvMass.Write();
 
 hInvMassEta2CETNoCuts.GetYaxis()->SetTitle("Et (GeV)");
 hInvMassEta2CETNoCuts.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CETNoCuts.Write();
  
  
 hInvMassEta2CAndIsolation.GetYaxis()->SetTitle("iso/PtSum");
 hInvMassEta2CAndIsolation.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndIsolation.Write();
  
 hInvMassEta2CAndPtC.GetYaxis()->SetTitle("PtC (GeV)");
 hInvMassEta2CAndPtC.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndPtC.Write();
 
 hInvMassEta2CAndS9oS25.GetYaxis()->SetTitle("S9oS25");
 hInvMassEta2CAndS9oS25.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hInvMassEta2CAndS9oS25.Write();  
   
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
 
 hAngleEnd.GetXaxis()->SetTitle("Angle between BC (rad)");
 hAngleEnd.Write();
 
 hAngleREnd.GetXaxis()->SetTitle("Angle R (eta/phi) between BC");
 hAngleREnd.Write();
 
 hAngle.GetXaxis()->SetTitle("Angle between BC (rad)");
 hAngle.Write();
  
 //---- MC Match ----
 
 hR_2C_Eta_MC.GetXaxis()->SetTitle("Angle between photons (rad)");
 hR_2C_Eta_MC.Write();
 
 hR_2C_MC.GetXaxis()->SetTitle("Angle between photons (rad)");
 hR_2C_MC.Write();
  
 
 
 hE3x3VsReal_MC.GetYaxis()->SetTitle("E Real (GeV)");
 hE3x3VsReal_MC.GetXaxis()->SetTitle("E E3x3 (GeV)");
 hE3x3VsReal_MC.Write();
 
 hE3x3VsReal_1_MC.GetYaxis()->SetTitle("E Real (GeV)");
 hE3x3VsReal_1_MC.GetXaxis()->SetTitle("E E3x3 (GeV)");
 hE3x3VsReal_1_MC.Write();
 
 hE3x3VsReal_2_MC.GetYaxis()->SetTitle("E Real (GeV)");
 hE3x3VsReal_2_MC.GetXaxis()->SetTitle("E E3x3 (GeV)");
 hE3x3VsReal_2_MC.Write();

 hE3x3OverReal_MC.GetXaxis()->SetTitle("E_E3x3 / E_True");
 hE3x3OverReal_MC.Write();
 
 hBCVsReal_MC.GetYaxis()->SetTitle("E Real (GeV)");
 hBCVsReal_MC.GetXaxis()->SetTitle("E BC (GeV)");
 hBCVsReal_MC.Write();
 
 hBCVsReal_1_MC.GetYaxis()->SetTitle("E Real (GeV)");
 hBCVsReal_1_MC.GetXaxis()->SetTitle("E BC (GeV)");
 hBCVsReal_1_MC.Write();
 
 hBCVsReal_2_MC.GetYaxis()->SetTitle("E Real (GeV)");
 hBCVsReal_2_MC.GetXaxis()->SetTitle("E BC (GeV)");
 hBCVsReal_2_MC.Write();

 hBCOverReal_MC.GetXaxis()->SetTitle("E_BC / E_True");
 hBCOverReal_MC.Write();
 
 
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

 hPtSumVsInvMass.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hPtSumVsInvMass.GetYaxis()->SetTitle("Pt of summed photons from Eta (GeV/c)");
 hPtSumVsInvMass.Write();
 
 hSinglePhotonPt.GetXaxis()->SetTitle("Pt of single photon from Eta (GeV/c)");
 hSinglePhotonPt.Write();
 
 hS9oS25VsEnergyCMC_1.GetYaxis()->SetTitle("S9oS25");
 hS9oS25VsEnergyCMC_1.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS9oS25VsEnergyCMC_1.Write();
 
 hS9oS25VsEnergyCMC_2.GetYaxis()->SetTitle("S9oS25");
 hS9oS25VsEnergyCMC_2.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS9oS25VsEnergyCMC_2.Write();
 
 hS9oS25VsEnergyPhMC_1.GetYaxis()->SetTitle("S9oS25");
 hS9oS25VsEnergyPhMC_1.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS9oS25VsEnergyPhMC_1.Write();
 
 hS9oS25VsEnergyPhMC_2.GetYaxis()->SetTitle("S9oS25");
 hS9oS25VsEnergyPhMC_2.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS9oS25VsEnergyPhMC_2.Write();


 hS4oS9VsEnergyCMC_1.GetYaxis()->SetTitle("S4oS9");
 hS4oS9VsEnergyCMC_1.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS4oS9VsEnergyCMC_1.Write();
 
 hS4oS9VsEnergyCMC_2.GetYaxis()->SetTitle("S4oS9");
 hS4oS9VsEnergyCMC_2.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS4oS9VsEnergyCMC_2.Write();
 
 hS4oS9VsEnergyPhMC_1.GetYaxis()->SetTitle("S4oS9");
 hS4oS9VsEnergyPhMC_1.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS4oS9VsEnergyPhMC_1.Write();
 
 hS4oS9VsEnergyPhMC_2.GetYaxis()->SetTitle("S4oS9");
 hS4oS9VsEnergyPhMC_2.GetXaxis()->SetTitle("Invariant Mass (GeV)");
 hS4oS9VsEnergyPhMC_2.Write();

 hS4oS9VsS9oS25CMC_1.GetYaxis()->SetTitle("S9oS25");
 hS4oS9VsS9oS25CMC_1.GetXaxis()->SetTitle("S4oS9");
 hS4oS9VsS9oS25CMC_1.Write();
 
 hS4oS9VsS9oS25CMC_2.GetYaxis()->SetTitle("S9oS25");
 hS4oS9VsS9oS25CMC_2.GetXaxis()->SetTitle("S4oS9");
 hS4oS9VsS9oS25CMC_2.Write();
  
 hS9oS25MC_1.GetXaxis()->SetTitle("S9oS25");
 hS9oS25MC_1.Write();
  
 hS9oS25MC_2.GetXaxis()->SetTitle("S9oS25");
 hS9oS25MC_2.Write();
 
 
 hS4oS9MC_1.GetXaxis()->SetTitle("S4oS9");
 hS4oS9MC_1.Write();
  
 hS4oS9MC_2.GetXaxis()->SetTitle("S4oS9");
 hS4oS9MC_2.Write();
 
 hNEtaPerEvent.GetXaxis()->SetTitle("Number of etas per event");
 hNEtaPerEvent.Write();
 
 hEtSumPh_MC.GetXaxis()->SetTitle("Inv Mass (GeV)");
 hEtSumPh_MC.GetYaxis()->SetTitle("Pt (GeV)");
 hEtSumPh_MC.Write();
 
 hEtSumC_MC.GetXaxis()->SetTitle("Inv Mass (GeV)");
 hEtSumC_MC.GetYaxis()->SetTitle("Pt (GeV)");
 hEtSumC_MC.Write();
   
  
 hInvMassEtaVsAngleR2C.GetYaxis()->SetTitle("Inv Mass (GeV)");
 hInvMassEtaVsAngleR2C.GetXaxis()->SetTitle("Angle R (eta/phi) between BC");
 hInvMassEtaVsAngleR2C.Write();
 
 hInvMassEtaVsAngleR2C_NoCut.GetYaxis()->SetTitle("Inv Mass (GeV)");
 hInvMassEtaVsAngleR2C_NoCut.GetXaxis()->SetTitle("Angle R (eta/phi) between BC");
 hInvMassEtaVsAngleR2C_NoCut.Write();
 
 hInvMassEta2C_Trial.GetXaxis()->SetTitle("Inv Mass (GeV)");
 hInvMassEta2C_Trial.Write();
 
 hInvMassPi02C_Trial.GetXaxis()->SetTitle("Inv Mass (GeV)");
 hInvMassPi02C_Trial.Write();
  
 hInvMassEta2C_MinAngleCut.GetXaxis()->SetTitle("Inv Mass (GeV)");
 hInvMassEta2C_MinAngleCut.Write();
 
 hInvMassEtaVsAngle.GetYaxis()->SetTitle("Inv Mass (GeV)");
 hInvMassEtaVsAngle.GetXaxis()->SetTitle("Angle between BC (rad)");
 hInvMassEtaVsAngle.Write();
 
   
 //---- Save Cuts in File ----
 
 TTree Cuts("Cuts","Cuts");
 Cuts.Branch("R",&R,"R/D");
 Cuts.Branch("S9oS25_MC_Cut",&S9oS25_MC_Cut,"S9oS25_MC_Cut/D");
 Cuts.Branch("S4oS9_MC_Cut",&S4oS9_MC_Cut,"S4oS9_MC_Cut/D");
 Cuts.Branch("S9oS25C_Cut",&S9oS25C_Cut,"S9oS25C_Cut/D");
 Cuts.Branch("S4oS9C_Cut",&S4oS9C_Cut,"S4oS9C_Cut/D");
 Cuts.Branch("ptC_Cut",&ptC_Cut,"ptC_Cut/D");
 Cuts.Branch("ptC_EE_Cut",&ptC_EE_Cut,"ptC_EE_Cut/D");
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
 Cuts.Branch("angleR_Cut",&angleR_Cut,"angleR_Cut/D");
 Cuts.Branch("OmegaSearch",&OmegaSearch,"OmegaSearch/I");
 Cuts.Branch("E3x3",&E3x3,"E3x3/I");
 Cuts.Branch("Scalib",&Scalib,"Scalib/D");
 
 
 Cuts.Fill();
 Cuts.Write();
 
 saving.Close ();
 delete chain;
 return 0 ;

}



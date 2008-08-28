#include <map>
#include <string>
#include <sstream>
#include <fstream>

#include "TH1.h"
#include "TChain.h"

//PG for the cluster-based calibration 
#include "Calibration/EcalCalibAlgos/interface/VEcalCalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/IMACalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/L3CalibBlock.h"

//---- AM added
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "TFile.h"
#include <iostream>
#include "TH2.h"


int main (int argc, char** argv)
{
 std::map<int, TH1F *> dEdx_Histos ;

 int nEntries = -1 ;

  //---- AM Command line input ----
  
//  for (int i=0; i<argc; i++)
 
 std::string outputRootName = "outputHistos.root";
 std::string testNameOutput = "-o";
 if (argc>2){
  if (argv[1] == testNameOutput) {
   outputRootName = argv[2];
  }
 }
  
 std::string testHelp = "--help";
 if (argc==2){
  if (argv[1] == testHelp) {
   std::cout << "Help" << std::endl ;
   std::cout << " --help : display help" << std::endl ;
   std::cout << " -o : output root file name (eg histograms.root)" << std::endl ;
   std::cout << " -i : input root file name (eg histograms.root)" << std::endl ;
   std::cout << " name of input file : list name of input files ntuples" << std::endl ;     
   exit(1);
  }
 }
 
  
 if (argc < 2)
 {
  std::cerr << "ERROR : ntuple name missing" << std::endl ;
  exit (1) ;
 }

  
  

  //---- Tree construction: sum over different trees ----
 std::string testNameInput = "-i";
 TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
 
 if (argv[1] != testNameInput && argv[3] != testNameInput){
  if (argv[1] == testNameOutput) {
   for (int i=3; i< (argc); i++) {
    chain->Add (argv[i]) ;
    std::cout << "Open: " << argv[i] << std::endl;
   }
  }
  else{
   for (int i=1; i< (argc); i++) {
    chain->Add (argv[i]) ;
    std::cout << "Open: " << argv[i] << std::endl;
   }
  }
 }
 else {
  char inputRootName[100];
  for (int i=0; i< 10; i++) {
   sprintf (inputRootName,"/castor/cern.ch/user/m/mattia/Ntuples/EcalCosmicsTree-50908%d.tree.root",i); 
   chain->Add (inputRootName);
   std::cout << "Open: " << inputRootName << std::endl;
  }
 }
 
 EcalCosmicsTreeContent treeVars ; 
 setBranchAddresses (chain, treeVars) ;
 nEntries = chain->GetEntries () ;
   
 TH1F dEdx_Tutti("dEdx_Tutti","dEdx_Tutti",1000,0,2);
 TH1F dE_Tutti("dE_Tutti","dE_Tutti",1000,0,2);
 TH1F dX_Tutti("dX_Tutti","dX_Tutti",1000,0,0.1);  
 
 TH2F occupancyEnergy ("occupancyEnergy","single crystals counting Energy",180,0,360,171,-85,86) ;
 TH2F occupancy ("occupancy","single crystals counting",360,1,361,171,-85,86) ;
 
  //PG loop over entries
 for (int entry = 0 ; entry < nEntries ; ++entry)
 {
  chain->GetEntry (entry) ;
  std::cout << "------> reading entry " << entry << " <------\n" ; 

      //PG loop over crystals <-- in case the calib is based on single
      //PG                        xtal deposit
    
  for (int XTLindex = 0 ; XTLindex < treeVars.nXtals ; ++XTLindex)
  {
      //PG find a unique index for each xtal --> rawId()
   EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;   
      //PG verify if the xtal index is already present in the dEdx_Histos map
      //PG    (if not, add it)         
   
   std::map<int, TH1F *>::iterator dEdx_Histos_iter = dEdx_Histos.find(dummy.rawId());
   if (dEdx_Histos_iter == dEdx_Histos.end()) {
    std::ostringstream stm;
    stm << dummy.rawId();
    std::string TH1FName = "dEdX_" +  stm.str();
    std::string TH1FNameDescription = "dE over dX -> rawId = " + stm.str();
    dEdx_Histos[dummy.rawId()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),1000,0,2);
   }
//    std::cout << " hashedIndex = " << dummy.hashedIndex();
  
   //PG fill the histo with the E/p for the xtal
   if (treeVars.xtalTkLength[XTLindex] != 0) {
    dEdx_Tutti.Fill(treeVars.xtalEnergy[XTLindex] / treeVars.xtalTkLength[XTLindex]);
    dX_Tutti.Fill(treeVars.xtalTkLength[XTLindex]);
   }
   dE_Tutti.Fill(treeVars.xtalEnergy[XTLindex]);
   dEdx_Histos[dummy.rawId()]->Fill(treeVars.xtalEnergy[XTLindex]);
   occupancy.Fill (dummy.iphi (), dummy.ieta ()) ;
   occupancyEnergy.Fill (dummy.iphi (),dummy.ieta (),treeVars.xtalEnergy[XTLindex]);
  }
    
 } //PG loop over entries

 
 //---- AM Initiate output file ----
 TFile saving (outputRootName.c_str(),"recreate") ;
 saving.cd () ;

  //PG loop over dEdx_Histos
 for (std::map<int, TH1F*>::iterator mapIt = dEdx_Histos.begin () ;
      mapIt != dEdx_Histos.end () ;
      ++mapIt)
 {
  mapIt->second->Write();
//   mapIt->second->Fit ("gaus") ;
 } //PG loop over dEdx_Histos

 
 dEdx_Tutti.GetXaxis()->SetTitle("dE/dX");
 dEdx_Tutti.Write();
 
 dE_Tutti.GetXaxis()->SetTitle("dE");
 dE_Tutti.Write();
 
 dX_Tutti.GetXaxis()->SetTitle("dX");
 dX_Tutti.Write();
 
 occupancyEnergy.GetXaxis()->SetTitle("i#phi");
 occupancyEnergy.GetYaxis()->SetTitle("i#eta");
 occupancyEnergy.Write () ;

 occupancy.GetXaxis()->SetTitle("i#phi");
 occupancy.GetYaxis()->SetTitle("i#eta");
 occupancy.Write () ;
  
 saving.Close () ;
 delete chain ;
 return 0 ;

}

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//---- CMSSW includes
#include "DataFormats/Math/interface/LorentzVector.h"
#include "PhysicsTools/NtupleUtils/interface/treeReader.h"
#include "PhysicsTools/NtupleUtils/interface/ntpleUtils.h"


//---- root includes
#include "TH1.h"
#include "TH2.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

//==== parameter include ====
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#define PI 3.14159265
#define TWOPI 6.28318530

using namespace std;

float deltaPhi(float phi1,float phi2) {
 float deltaphi = fabs(phi1-phi2);
 if (deltaphi > TWOPI) deltaphi -= TWOPI;
 if (deltaphi > PI ) deltaphi = TWOPI - deltaphi;
 return deltaphi;
}

//! main program
int main (int argc, char** argv)
{
 if (argc != 2){
  std::cerr << ">>>>> Program::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1 ;
 }
 // Parse the config file
 std::string fileName (argv[1]) ;
 boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfig(fileName) ;
 boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;

 edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("Input") ;
 std::vector<std::string> inputFileList = subPSetInput.getParameter<std::vector<std::string> > ("inputFileList") ;
 std::string outputRootFileName = subPSetInput.getParameter<std::string > ("outputRootFileName") ;

 int WP = subPSetInput.getUntrackedParameter<int> ("WP",95) ;

 std::cout << "get totalEvents " << std::endl;
 std::map<int,int> totalEvents = GetTotalEvents ("AllPassFilterBegin/totalEvents", inputFileList) ;

 std::cout << "get Ntple " << std::endl;
 std::string treeName = "simpleNtple/SimpleNtple";
 TChain* chain = new TChain (treeName.c_str ()) ;
 if (!FillChain (*chain, inputFileList)) return 1 ;
 treeReader reader ( (TTree*) (chain));

 std::cout << "Prepare output " << std::endl;
 TFile outputRootFile (outputRootFileName.c_str(), "RECREATE") ;
 outputRootFile.cd () ;

 TTree* myTree_;

 int initialNumber_ = totalEvents[1];

 int electrons_classification_;
 int electrons_basicClustersSize_;

 double deltaEtaSuperClusterAtVtx_;
 double deltaEtaSeedClusterAtCalo_;
 double deltaEtaEleClusterAtCalo_;
 double deltaPhiEleClusterAtCalo_;
 double deltaPhiSuperClusterAtVtx_;
 double deltaPhiSeedClusterAtCalo_;
 int mishits_;
 int nAmbiguousGsfTracks_;
 double dist_;
 double dcot_;

 double HoE_;
 double SigmaIEtaIEta_;
 double eleTrkIso_;
 double eleEcalIso_;
 double eleHcalIsoD1_;
 double eleHcalIsoD2_;
 unsigned int eleMisHits_;
 double met_;
 double sumEt_;
 double eta_;
 double phi_;
 double pT_;
 double ET_;
 double MT_;
 double EoP_;
 double eleFBrem_;
 double eleES_;
 double E2x2_;
 double E3x3_;
 double E5x5_;
 double ESC_;
 double ETSC_;
 double Sigma_Phi_;
 double Sigma_Eta_;
 double pIn_;
 double pOut_;
 double pAtCalo_;
 double p_;
 double E9oE25_;
 double DeltaEtaIn_;
 double DeltaPhiIn_;
 double etaSC_;
 double phiSC_;
 int nGoodElectrons_;
 int iSM_;
 int iSC_;
 double dphiMETEle_;
 double eleCharge_;
 double eleSwissCross_;
 int seedSeverityLevel_;
 int* eleId_;
 int iDetEB_;
 int iDetEE_;


 myTree_  = new TTree("myTree","myTree");

 myTree_ -> Branch("electrons_classification",&electrons_classification_,"electrons_classification/I");
 myTree_ -> Branch("electrons_basicClustersSize",&electrons_basicClustersSize_,"electrons_basicClustersSize/I");

 myTree_ -> Branch("initialNumber",&initialNumber_,"initialNumber/I");
 myTree_ -> Branch("sumEt",&sumEt_,"sumEt/D");
 myTree_ -> Branch("met",&met_,"met/D");
 myTree_ -> Branch("eta",&eta_,"eta/D");
 myTree_ -> Branch("phi",&phi_,"phi/D");
 myTree_ -> Branch("pT",&pT_,"pT/D");
 myTree_ -> Branch("ET",&ET_,"ET/D");
 myTree_ -> Branch("MT",&MT_,"MT/D");
 myTree_ -> Branch("EoP",&EoP_,"EoP/D");
 myTree_ -> Branch("eleFBrem",&eleFBrem_,"eleFBrem/D");
 myTree_ -> Branch("eleES",&eleES_,"eleES/D");
 myTree_ -> Branch("E2x2",&E2x2_,"E2x2/D");
 myTree_ -> Branch("E3x3",&E3x3_,"E3x3/D");
 myTree_ -> Branch("E5x5",&E5x5_,"E5x5/D");
 myTree_ -> Branch("ESC",&ESC_,"ESC/D");
 myTree_ -> Branch("ETSC",&ETSC_,"ETSC/D");
 myTree_ -> Branch("Sigma_Phi",&Sigma_Phi_,"Sigma_Phi/D");
 myTree_ -> Branch("Sigma_Eta",&Sigma_Eta_,"Sigma_Eta/D");
 myTree_ -> Branch("pIn",&pIn_,"pIn/D");
 myTree_ -> Branch("pOut",&pOut_,"pOut/D");
 myTree_ -> Branch("pAtCalo",&pAtCalo_,"pAtCalo/D");
 myTree_ -> Branch("p",&p_,"p/D");
 myTree_ -> Branch("E9oE25",&E9oE25_,"E9oE25/D");
 myTree_ -> Branch("iSM",&iSM_,"iSM/I");
 myTree_ -> Branch("iSC",&iSC_,"iSC/I");

 myTree_ -> Branch("deltaEtaSuperClusterAtVtx",&deltaEtaSuperClusterAtVtx_,"deltaEtaSuperClusterAtVtx/D");
 myTree_ -> Branch("deltaEtaSeedClusterAtCalo",&deltaEtaSeedClusterAtCalo_,"deltaEtaSeedClusterAtCalo/D");
 myTree_ -> Branch("deltaEtaEleClusterAtCalo",&deltaEtaEleClusterAtCalo_,"deltaEtaEleClusterAtCalo/D");
 myTree_ -> Branch("deltaPhiEleClusterAtCalo",&deltaPhiEleClusterAtCalo_,"deltaPhiEleClusterAtCalo/D");
 myTree_ -> Branch("deltaPhiSuperClusterAtVtx",&deltaPhiSuperClusterAtVtx_,"deltaPhiSuperClusterAtVtx/D");
 myTree_ -> Branch("deltaPhiSeedClusterAtCalo",&deltaPhiSeedClusterAtCalo_,"deltaPhiSeedClusterAtCalo/D");

 myTree_ -> Branch("mishits",&mishits_,"mishits/I");
 myTree_ -> Branch("nAmbiguousGsfTracks",&nAmbiguousGsfTracks_,"nAmbiguousGsfTracks/I");
 myTree_ -> Branch("dist",&dist_,"dist/D");
 myTree_ -> Branch("dcot",&dcot_,"dcot/D");

 myTree_ -> Branch("SigmaIEtaIEta",&SigmaIEtaIEta_,"SigmaIEtaIEta/D");
 myTree_ -> Branch("HoE",&HoE_,"HoE/D");
 myTree_ -> Branch("eleTrkIso",&eleTrkIso_,"eleTrkIso/D");
 myTree_ -> Branch("eleEcalIso",&eleEcalIso_,"eleEcalIso/D");
 myTree_ -> Branch("eleHcalIsoD1",&eleHcalIsoD1_,"eleHcalIsoD1/D");
 myTree_ -> Branch("eleHcalIsoD2",&eleHcalIsoD2_,"eleHcalIsoD2/D");

 myTree_ -> Branch("DeltaEtaIn",&DeltaEtaIn_,"DeltaEtaIn/D");
 myTree_ -> Branch("DeltaPhiIn",&DeltaPhiIn_,"DeltaPhiIn/D");
 myTree_ -> Branch("etaSC",&etaSC_,"etaSC/D");
 myTree_ -> Branch("phiSC",&phiSC_,"phiSC/D");
 myTree_ -> Branch("nGoodElectrons",&nGoodElectrons_,"nGoodElectrons/I");
 myTree_ -> Branch("dphiMETEle",&dphiMETEle_,"dphiMETEle/D");    
 myTree_ -> Branch("eleCharge",&eleCharge_,"eleCharge/D");  
 myTree_ -> Branch("eleSwissCross",&eleSwissCross_,"eleSwissCross/D");  
 myTree_ -> Branch("seedSeverityLevel",&seedSeverityLevel_,"seedSeverityLevel/I");

 myTree_ -> Branch("iDetEB",&iDetEB_,"iDetEB/I");
 myTree_ -> Branch("iDetEE",&iDetEE_,"iDetEE/I");
 
 
 
 
 
 
 std::cout << ">>>>> Program::Read " << chain -> GetEntries () << " entries" << std::endl ;  
 for (int entry = 0 ; entry < chain -> GetEntries () ; ++entry)
 {
  reader.GetEntry (entry) ;
  if ( (entry%10000) == 0) std::cout << ">>>>> Program::GetEntry " << entry << std::endl ;   

  ///==== DATA ====
//   if( dataFlag == 1 )
//   {
//    std::pair<int,int> eventLSandID(reader.GetInt("lumiId")->at(0), reader.GetInt("eventId")->at(0));
//    std::pair<int,std::pair<int,int> > eventRUNandLSandID(reader.GetInt("runId")->at(0), eventLSandID);
//    
//    if( eventsMap[eventRUNandLSandID] == 1 ) continue;
//    else eventsMap[eventRUNandLSandID] = 1;
//   }
  ///==== DATA ====

  //start loop over electron candidates
  nGoodElectrons_ = reader.Get4V("electrons")->size();
  for (int iEle = 0; iEle < nGoodElectrons_; iEle++){

//   if (reader.GetInt("electrons_classification")->at(iEle) != 0) continue;
   
   electrons_classification_ = reader.GetInt("electrons_classification")->at(iEle);
   electrons_basicClustersSize_ = reader.GetInt("electrons_basicClustersSize")->at(iEle);

   deltaEtaSuperClusterAtVtx_ = reader.GetFloat("electrons_deltaEtaSuperClusterAtVtx")->at(iEle);
   deltaEtaSeedClusterAtCalo_ = reader.GetFloat("electrons_deltaEtaSeedClusterAtCalo")->at(iEle);
   deltaEtaEleClusterAtCalo_ = reader.GetFloat("electrons_deltaEtaEleClusterAtCalo")->at(iEle);
   deltaPhiEleClusterAtCalo_ = reader.GetFloat("electrons_deltaPhiEleClusterAtCalo")->at(iEle);
   deltaPhiSuperClusterAtVtx_ = reader.GetFloat("electrons_deltaPhiSuperClusterAtVtx")->at(iEle);
   deltaPhiSeedClusterAtCalo_ = reader.GetFloat("electrons_deltaPhiSeedClusterAtCalo")->at(iEle);

   mishits_ = reader.GetInt("electrons_mishits")->at(iEle);

   nAmbiguousGsfTracks_ = reader.GetInt("electrons_nAmbiguousGsfTracks")->at(iEle);
   dist_ = 0;
   dcot_ = 0;
//    dist_ = reader.GetFloat("electrons_dist")->at(iEle);
//    dcot_ = reader.GetFloat("electrons_dcot")->at(iEle);

   HoE_ = reader.GetFloat("electrons_hOverE")->at(iEle);
   SigmaIEtaIEta_ = reader.GetFloat("electrons_sigmaIetaIeta")->at(iEle);
   eleTrkIso_ = reader.GetFloat("electrons_tkIso03")->at(iEle);
   eleEcalIso_ = reader.GetFloat("electrons_emIso03")->at(iEle);
   eleHcalIsoD1_ = reader.GetFloat("electrons_hadIso03_1")->at(iEle);
   eleHcalIsoD2_ = reader.GetFloat("electrons_hadIso03_2")->at(iEle);
   met_ = reader.Get4V("PFMet")->at(0).Et(); ///==== PF MET
   sumEt_ = reader.GetFloat("PFSumEt")->at(0); ///==== PF SumET
   eta_ = reader.Get4V("electrons")->at(iEle).eta();
   phi_ = reader.Get4V("electrons")->at(iEle).phi();
   pT_ = reader.Get4V("electrons")->at(iEle).Pt();
   ET_ = reader.Get4V("electrons")->at(iEle).Et();
   EoP_ = reader.GetFloat("electrons_eOverP")->at(iEle);
   eleFBrem_ = reader.GetFloat("electrons_fbrem")->at(iEle);
   eleES_ = reader.GetFloat("electrons_ES")->at(iEle);
   E2x2_ = 0; //reader.GetFloat("electrons_e2x2")->at(iEle);
   E3x3_ = 0; //reader.GetFloat("electrons_e3x3")->at(iEle);
   E5x5_ = reader.GetFloat("electrons_e5x5")->at(iEle);
   ESC_ = reader.GetFloat("electrons_scE")->at(iEle);
   ETSC_ = reader.GetFloat("electrons_scEt")->at(iEle);
   Sigma_Phi_ = reader.GetFloat("electrons_SC_phiWidth")->at(iEle);
   Sigma_Eta_ = reader.GetFloat("electrons_SC_etaWidth")->at(iEle);
   pIn_ = reader.GetFloat("electrons_pin")->at(iEle);
   pOut_ = reader.GetFloat("electrons_pout")->at(iEle);
   pAtCalo_ = reader.GetFloat("electrons_pcalo")->at(iEle);
   p_ = reader.GetFloat("electrons_pin")->at(iEle);
   E9oE25_ = 0; //reader.GetFloat("electrons_e3x3")->at(iEle) / reader.GetFloat("electrons_e5x5")->at(iEle);
   DeltaEtaIn_ = reader.GetFloat("electrons_deltaEtaIn")->at(iEle);
   DeltaPhiIn_ = reader.GetFloat("electrons_deltaPhiIn")->at(iEle);
   etaSC_ = reader.GetFloat("electrons_scEta")->at(iEle);
   phiSC_ = reader.GetFloat("electrons_scPhi")->at(iEle);
   iSM_ = 0; //reader.GetInt("iSM")->at(iEle);
   iSC_ = 0; //reader.GetInt("iSC")->at(iEle);

   iDetEB_ = 0; //reader.GetInt("electrons_iDetEB")->at(iEle);
   iDetEE_ = 0; //reader.GetInt("electrons_iDetEE")->at(iEle);

   dphiMETEle_ = deltaPhi(reader.Get4V("PFMet")->at(0).Phi(),reader.Get4V("electrons")->at(iEle).phi());
   eleCharge_ = reader.GetFloat("electrons_charge")->at(iEle);
   eleSwissCross_ = 0; //reader.GetFloat("electrons_SwissCross")->at(iEle);
   seedSeverityLevel_ = 0; //reader.GetInt("electrons_seedSeverityLevel")->at(iEle);

   float cphi = (reader.Get4V("electrons")->at(iEle).x() * reader.Get4V("PFMet")->at(0).Px() 
     + reader.Get4V("electrons")->at(iEle).y() * reader.Get4V("PFMet")->at(0).Py()) 
     / (met_*pT_);

   MT_ = sqrt(2*ET_*met_*(1-cphi));

   myTree_ -> Fill();
   
   /*
   ///==== WP 70% ====
   if (WP==70 && (( fabs(etaSC_)<1.5 && (eleTrkIso_+eleEcalIso_+eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.04 && (eleTrkIso_)/pT_ < 0.05&& (eleEcalIso_)/pT_ < 0.06&& (eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.03&& HoE_ < 0.025&& SigmaIEtaIEta_ < 0.01)||( fabs(etaSC_)>1.5 && (eleTrkIso_+eleEcalIso_+eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.03&& (eleTrkIso_)/pT_ < 0.025&& (eleEcalIso_)/pT_ < 0.025&& (eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.02&& HoE_ < 0.025&& SigmaIEtaIEta_ < 0.03))){
    myTree_ -> Fill(); ///==== for every electron!
   }
   ///==== WP 80% ====
   if (WP==80 && (( fabs(etaSC_)<1.5 && (eleTrkIso_+eleEcalIso_+eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.07&& (eleTrkIso_)/pT_ < 0.09&& (eleEcalIso_)/pT_ < 0.07&& (eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.10&& HoE_ < 0.04&& SigmaIEtaIEta_ < 0.01)||( fabs(etaSC_)>1.5 && (eleTrkIso_+eleEcalIso_+eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.06&& (eleTrkIso_)/pT_ < 0.04&& (eleEcalIso_)/pT_ < 0.05&& (eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.025&& HoE_ < 0.025&& SigmaIEtaIEta_ < 0.03))){
    myTree_ -> Fill(); ///==== for every electron!
   }
   ///==== WP 95% ====
   if (WP==95 && (( fabs(etaSC_)<1.5 && (eleTrkIso_+eleEcalIso_+eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.15&& (eleTrkIso_)/pT_ < 0.15&& (eleEcalIso_)/pT_ < 2.00&& (eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.12&& HoE_ < 0.15&& SigmaIEtaIEta_ < 0.01)||( fabs(etaSC_)>1.5 && (eleTrkIso_+eleEcalIso_+eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.1&& (eleTrkIso_)/pT_ < 0.08&& (eleEcalIso_)/pT_ < 0.06&& (eleHcalIsoD1_+eleHcalIsoD2_)/pT_ < 0.05&& HoE_ < 0.07&& SigmaIEtaIEta_ < 0.03))){
    myTree_ -> Fill(); ///==== for every electron!
   }
   */
   
  }// end loop over ele cand
 } // end loop over entries
 
 outputRootFile.cd();
 myTree_ -> Write();

}







#include <iostream>
#include <string>

//PG CMSSW includes
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
//#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

//PG root includes
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TProfile2D.h"
#include "TLorentzVector.h"
#include "TApplication.h"

//MF read CFG files includes5
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>


#include "CRUZET/Calibration/interface/CRUtils.h"

//#include "CRUZET/Calibration/interface/AssociationUtils.h"


int main (int argc, char* argv[])
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  
  //edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;

  edm::ParameterSet subPSetOutput =  
    parameterSet->getParameter<edm::ParameterSet> ("outputNtuple") ;
  std::string outputFile = 
   subPSetOutput.getParameter<std::string> ("ntupleName") ;

  edm::ParameterSet subPSetInput =  
    parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = 
   subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  std::cout << "reading : " ;

   gROOT->Reset();
//   gSystem->Load("$ROOTSYS/test/libEvent");

  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;

  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ;
       ++listIt)
    {
//      std::cout << *listIt << " " << std::endl ;
      chain->Add (listIt->c_str ()) ;
    }

   int nentries = chain->GetEntries () ;

// qui il setbranchaddress
//   Event *event   = 0;
//   oldtree->SetBranchAddress("event",&event);

   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile(outputFile.c_str (),"recreate");
   TTree *newtree = chain->CloneTree (0) ;

   for (Int_t i=0;i<nentries; i++)
     {
       chain->GetEntry (i) ;
       if (i % 10000 == 0) std::cout << "reading entry " << i << std::endl ;



       std::vector<ect::association> associations ;
       ect::fillAssocVector (associations, treeVars) ;
       ect::selectOnDR (associations, treeVars, 0.1) ;



       //*********************************************
       // Select only events with at least 1 reco muon
       //*********************************************
       
       if (treeVars.nRecoMuons > 0) newtree->Fill () ;                                                                                                 


       //************************************************
       // Select only events with at least 1 association
       //************************************************
       //
       //if (associations.size () > 0) newtree->Fill () ;                                                                                                


       //**********************************************************
       // Select only events with at least 1 association and p < 15
       //**********************************************************
       //
       //int nRecoMuons_p_0_20 = 0;
       //for(unsigned int i = 0; i < associations.size (); ++i)
       // {
       //    int MUindex = associations.at(i).first;
       //    float muonP = treeVars.muonP[MUindex];
       //      
       //    if( (muonP > 0.) && (muonP < 20.) )
       //      ++nRecoMuons_p_0_20;
       //  }
       //
       //if (nRecoMuons_p_0_20 > 0) newtree->Fill () ;




   }

   newtree->Print();
   newtree->AutoSave();
   delete newfile;
   
   return 0 ;
}



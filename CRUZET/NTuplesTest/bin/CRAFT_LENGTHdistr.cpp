#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CRUZET/NTuplesTest/test/Langaus.cc"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "CLHEP/Units/PhysicalConstants.h"

#define PHI_MIN -3.1416
#define PHI_MAX +3.1416
#define PHI_BIN 360

#define ETA_MIN -1.5
#define ETA_MAX +1.5
#define ETA_BIN 170

#define IPHI_MIN 0.
#define IPHI_MAX +361
#define IPHI_BIN 361

#define IETA_MIN -80.
#define IETA_MAX +80.
#define IETA_BIN 361

#define X_BIN 500
#define X_MIN -800
#define X_MAX +800

#define Y_BIN 500
#define Y_MIN -800
#define Y_MAX +800

#define Z_BIN 800
#define Z_MIN -800
#define Z_MAX +800

#define P_BIN 500
#define P_MIN 0
#define P_MAX 1000



int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  double muond0MAX = subPSetSelections.getParameter<double> ("muond0MAX") ;
  double muondzMAX = subPSetSelections.getParameter<double> ("muondzMAX") ;
  
  edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  
  
  
  // load ntuple
  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;
  
  
  
  // input files
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ; ++listIt)
  {
    std::cout << *listIt << " " << std::endl ;
    chain->Add (listIt->c_str ()) ;
  }
  
  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;
  
  
  
  // Output file
  std::string outputRootName = "CRAFT_LENGTHdistr.root" ;
  TFile outputRootFile (outputRootName.c_str (), "recreate") ;
  outputRootFile.cd () ;
  
  
  
  // output distributions
  TH1F muonTkLengthInEcalDistr ("muonTkLengthInEcalDistr", "muonTkLengthInEcalDistr", 200, 0., 100.) ;
  TProfile2D muonTkLengthInEcal_ETAvsPHI ("muonTkLengthInEcal_ETAvsPHI", "muonTkLengthInEcal_ETAvsPHI",
                                          PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile muonTkLengthInEcal_PHI ("muonTkLengthInEcal_PHI", "muonTkLengthInEcal_PHI",
                                   PHI_BIN, PHI_MIN, PHI_MAX) ;
  TProfile muonTkLengthInEcal_ETA ("muonTkLengthInEcal_ETA", "muonTkLengthInEcal_ETA",
                                   ETA_BIN, ETA_MIN, ETA_MAX) ;
  
  TH1F muonTkLengthInEcalDistr_cut ("muonTkLengthInEcalDistr_cut", "muonTkLengthInEcalDistr_cut", 200, 0., 100.) ;
  TProfile2D muonTkLengthInEcal_ETAvsPHI_cut ("muonTkLengthInEcal_ETAvsPHI_cut", "muonTkLengthInEcal_ETAvsPHI_cut",
                                              PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile muonTkLengthInEcal_PHI_cut ("muonTkLengthInEcal_PHI_cut", "muonTkLengthInEcal_PHI_cut",
                                       PHI_BIN, PHI_MIN, PHI_MAX) ;
  TProfile muonTkLengthInEcal_ETA_cut ("muonTkLengthInEcal_ETA_cut", "muonTkLengthInEcal_ETA_cut",
                                       ETA_BIN, ETA_MIN, ETA_MAX) ;
  
  TH1F muonTkLengthInEcalCurvedDistr ("muonTkLengthInEcalCurvedDistr", "muonTkLengthInEcalCurvedDistr", 200, 0., 100.) ;
  TProfile2D muonTkLengthInEcalCurved_ETAvsPHI ("muonTkLengthInEcalCurved_ETAvsPHI", "muonTkLengthInEcalCurved_ETAvsPHI",
                                                PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile muonTkLengthInEcalCurved_PHI ("muonTkLengthInEcalCurved_PHI", "muonTkLengthInEcal_PHI",
                                         PHI_BIN, PHI_MIN, PHI_MAX) ;
  TProfile muonTkLengthInEcalCurved_ETA ("muonTkLengthInEcalCurved_ETA", "muonTkLengthInEcal_ETA",
                                         ETA_BIN, ETA_MIN, ETA_MAX) ;
  
  
  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    chain->GetEntry (entry) ;
    // if (entry == 100000) break ;
    
    
    for (int MUindex = 0 ; MUindex < treeVars.nRecoMuons ; ++MUindex)
    {
      int muonLeg = treeVars.muonLeg[MUindex] ;
      float muond0 = treeVars.muond0[MUindex] ;
      float muondz = treeVars.muondz[MUindex] ;
      GlobalPoint muonTkInternalPointInEcal (treeVars.muonTkInternalPointInEcalX[MUindex],
                                             treeVars.muonTkInternalPointInEcalY[MUindex],
                                             treeVars.muonTkInternalPointInEcalZ[MUindex]) ;
      float muonTkInternalPointInEcalPhi = muonTkInternalPointInEcal.phi () ;
      float muonTkInternalPointInEcalEta = muonTkInternalPointInEcal.eta () ;
      float muonTkLengthInEcal = treeVars.muonTkLengthInEcalDetail[MUindex] ;
      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex] ;
      
      
      
      if (muonTkLengthInEcal > 0.)
      {
        muonTkLengthInEcalDistr.Fill (muonTkLengthInEcal) ;
        muonTkLengthInEcal_PHI.Fill (muonTkInternalPointInEcalPhi,
                                     muonTkLengthInEcal) ;
        muonTkLengthInEcal_ETA.Fill (muonTkInternalPointInEcalEta,
                                     muonTkLengthInEcal) ;
        muonTkLengthInEcal_ETAvsPHI.Fill (muonTkInternalPointInEcalPhi,
                                          muonTkInternalPointInEcalEta,
                                          muonTkLengthInEcal) ;
        
        if ( (fabs(muond0) < muond0MAX) && (fabs(muondz) < muondzMAX) )
        {
          muonTkLengthInEcalDistr_cut.Fill (muonTkLengthInEcal) ;
          muonTkLengthInEcal_PHI_cut.Fill (muonTkInternalPointInEcalPhi,
                                       muonTkLengthInEcal) ;
          muonTkLengthInEcal_ETA_cut.Fill (muonTkInternalPointInEcalEta,
                                       muonTkLengthInEcal) ;
          muonTkLengthInEcal_ETAvsPHI_cut.Fill (muonTkInternalPointInEcalPhi,
                                            muonTkInternalPointInEcalEta,
                                            muonTkLengthInEcal) ;
        }
      }
      
      if ( (muonTkLengthInEcalCurved > 0.) && (muonLeg == -1) )
      {
        muonTkLengthInEcalCurvedDistr.Fill (muonTkLengthInEcalCurved) ;
        muonTkLengthInEcalCurved_PHI.Fill (muonTkInternalPointInEcalPhi,
                                     muonTkLengthInEcalCurved) ;
        muonTkLengthInEcalCurved_ETA.Fill (muonTkInternalPointInEcalEta,
                                     muonTkLengthInEcalCurved) ;
        muonTkLengthInEcalCurved_ETAvsPHI.Fill (muonTkInternalPointInEcalPhi,
                                                muonTkInternalPointInEcalEta,
                                                muonTkLengthInEcalCurved) ;
      }
    }
  }
  
  
  
  
  // Save histograms
  
  muonTkLengthInEcalDistr.Write () ;
  muonTkLengthInEcal_ETAvsPHI.Write () ;
  muonTkLengthInEcal_PHI.Write () ;
  muonTkLengthInEcal_ETA.Write () ;
  
  muonTkLengthInEcalDistr_cut.Write () ;
  muonTkLengthInEcal_ETAvsPHI_cut.Write () ;
  muonTkLengthInEcal_PHI_cut.Write () ;
  muonTkLengthInEcal_ETA_cut.Write () ;
  
  
  muonTkLengthInEcalCurvedDistr.Write () ;
  muonTkLengthInEcalCurved_ETAvsPHI.Write () ;
  muonTkLengthInEcalCurved_PHI.Write () ;
  muonTkLengthInEcalCurved_ETA.Write () ;
  
  outputRootFile.Close () ;
  
  
  return 0 ;
}


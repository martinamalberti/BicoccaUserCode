#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

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

#define P_BIN 1000
#define P_MIN 0
#define P_MAX 1000



int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  
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
  std::string outputRootName = "CRAFT_MATCHdistr.root" ;
  TFile outputRootFile (outputRootName.c_str (), "recreate") ;
  outputRootFile.cd () ;
  
  
  
  // output distributions
  TH2F looseAssoc_DETAvsDPHI ("looseAssoc_DETAvsDPHI", "looseAssoc_DETAvsDPHI", PHI_BIN, PHI_MIN, PHI_MAX, 2*ETA_BIN, 2*ETA_MIN, 2*ETA_MAX) ;
  TH1F looseAssoc_DR ("looseAssoc_DR", "looseAssoc_DR", 2*PHI_BIN, 0., 2*PHI_MAX) ;
  
  TH1F EoverP_loose ("EoverP_loose", "EoverP_loose", 1000, 0., 5.) ;
  
  
  TH2F tightAssoc_DETAvsDPHI ("tightAssoc_DETAvsDPHI", "tightAssoc_DETAvsDPHI", PHI_BIN, -0.5, 0.5, ETA_BIN, -0.5, 0.5) ;
  TH1F tightAssoc_DR ("tightAssoc_DR", "tightAssoc_DR", 2*PHI_BIN, 0., 2*PHI_MAX) ;
  
  TH1F EoverP_tight ("EoverP_tight", "EoverP_tight", 1000, 0., 5.) ;
  TProfile EoverPvsETA_tight ("EoverPvsETA_tight", "EoverPvsETA_tight", ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile EoverPvsPHI_tight ("EoverPvsPHI_tight", "EoverPvsPHI_tight", PHI_BIN, PHI_MIN, PHI_MAX) ;
  TProfile EoverPvsPT_tight ("EoverPvsPT_tight", "EoverPvsPT_tight", P_BIN, P_MIN, P_MAX) ;
  
  TProfile dEvsdX_tight ("dEvsdX_tight", "dEvsdX_tight", 200, 0., 100.) ;
  
  
  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    chain->GetEntry (entry) ;
    // if (entry == 100000) break ;
    
    // association MU-SC
    std::vector<ect::association> looseAssoc ;
    ect::fillAssocVector (looseAssoc, treeVars) ;
    ect::selectOnDR (looseAssoc, treeVars, 1000000.) ;
    
    std::vector<ect::association> tightAssoc ;
    ect::fillAssocVector (tightAssoc, treeVars) ;
    ect::selectOnDR (tightAssoc, treeVars, 0.1) ;
    
    
    
    //loop on associations vector
    for (unsigned int i = 0 ; i < looseAssoc.size () ; ++i)
    {
      int MUindex = looseAssoc.at (i).first  ;
      int SCindex = looseAssoc.at (i).second ;
      
      
      
      int muonLeg = treeVars.muonLeg[MUindex];
      float muonP = 0.;
      float muonPt = 0.;
      float muond0 = treeVars.muond0[MUindex];
      float muondz = treeVars.muondz[MUindex];
      float muonPhi = 0.;
      float muonEta = 0.;
      float muonTkLengthInEcal = treeVars.muonTkLengthInEcalDetail[MUindex];
      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex];
      float superClusterRawEnergy = treeVars.superClusterRawEnergy[SCindex]; 
      GlobalPoint muonTkInternalPointInEcal (treeVars.muonTkInternalPointInEcalX[MUindex],
                                             treeVars.muonTkInternalPointInEcalY[MUindex],
                                             treeVars.muonTkInternalPointInEcalZ[MUindex]) ;
      float Dphi = treeVars.superClusterPhi[SCindex] - muonTkInternalPointInEcal.phi () ;
      float Deta = treeVars.superClusterEta[SCindex] - muonTkInternalPointInEcal.eta () ;
      
      if (muonLeg == 1)
      {
        muonP = treeVars.muonInnTkInnerHitP[MUindex];
        muonPt = treeVars.muonInnTkInnerHitPt[MUindex];
        muonPhi = treeVars.muonInnTkInnerHitPhi[MUindex];
        muonEta = treeVars.muonInnTkInnerHitEta[MUindex];
      }
      
      else if (muonLeg == -1)
      {
        muonP = treeVars.muonInnTkOuterHitP[MUindex];
        muonPt = treeVars.muonInnTkOuterHitPt[MUindex];
        muonPhi = treeVars.muonInnTkOuterHitPhi[MUindex];
        muonEta = treeVars.muonInnTkOuterHitEta[MUindex];
      }
      
      
      
      looseAssoc_DETAvsDPHI.Fill (Dphi, Deta) ;
      looseAssoc_DR.Fill (sqrt (Dphi*Dphi + Deta*Deta)) ;
      
      if (muonLeg == -1)
      {
        EoverP_loose.Fill (superClusterRawEnergy/muonP) ;
      }
    }
    
    
    
    for (unsigned int i = 0 ; i < tightAssoc.size () ; ++i)
    {
      int MUindex = tightAssoc.at (i).first  ;
      int SCindex = tightAssoc.at (i).second ;
      
      
      
      int muonLeg = treeVars.muonLeg[MUindex];
      float muonP = 0.;
      float muonPt = 0.;
      float muond0 = treeVars.muond0[MUindex];
      float muondz = treeVars.muondz[MUindex];
      float muonPhi = 0.;
      float muonEta = 0.;
      float muonTkLengthInEcal = treeVars.muonTkLengthInEcalDetail[MUindex];
      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex];
      float superClusterRawEnergy = treeVars.superClusterRawEnergy[SCindex]/4*0.9 ;
      GlobalPoint muonTkInternalPointInEcal (treeVars.muonTkInternalPointInEcalX[MUindex],
                                             treeVars.muonTkInternalPointInEcalY[MUindex],
                                             treeVars.muonTkInternalPointInEcalZ[MUindex]) ;
      float Dphi = treeVars.superClusterPhi[SCindex] - muonTkInternalPointInEcal.phi () ;
      float Deta = treeVars.superClusterEta[SCindex] - muonTkInternalPointInEcal.eta () ;
      
      if (muonLeg == 1)
      {
        muonP = treeVars.muonInnTkInnerHitP[MUindex];
        muonPt = treeVars.muonInnTkInnerHitPt[MUindex];
        muonPhi = treeVars.muonInnTkInnerHitPhi[MUindex];
        muonEta = treeVars.muonInnTkInnerHitEta[MUindex];
      }
      
      else if (muonLeg == -1)
      {
        muonP = treeVars.muonInnTkOuterHitP[MUindex];
        muonPt = treeVars.muonInnTkOuterHitPt[MUindex];
        muonPhi = treeVars.muonInnTkOuterHitPhi[MUindex];
        muonEta = treeVars.muonInnTkOuterHitEta[MUindex];
      }
      
      
      
      tightAssoc_DETAvsDPHI.Fill (Dphi, Deta) ;
      tightAssoc_DR.Fill (sqrt (Dphi*Dphi + Deta*Deta)) ;
      
      if (muonLeg == -1)
      {
        EoverP_tight.Fill (superClusterRawEnergy/muonP) ;
        EoverPvsETA_tight.Fill (muonEta, superClusterRawEnergy/muonP) ;
        EoverPvsPHI_tight.Fill (muonPhi, superClusterRawEnergy/muonP) ;
        EoverPvsPT_tight.Fill (muonPt, superClusterRawEnergy/muonP) ;
        
        if(muonTkLengthInEcal > 0.)
        {
          dEvsdX_tight.Fill (muonTkLengthInEcal, superClusterRawEnergy) ;
        }
      }
    }
  
  } //PG loop over entries
  
  
  
  // Save histograms
  looseAssoc_DETAvsDPHI.Write () ;
  looseAssoc_DR.Write () ;
  
  EoverP_loose.Write () ;
  
  
  tightAssoc_DETAvsDPHI.Write () ;
  tightAssoc_DR.Write () ;
  
  EoverP_tight.Write () ;
  EoverPvsETA_tight.Write () ;
  EoverPvsPHI_tight.Write () ;
  EoverPvsPT_tight.Write () ;
  
  dEvsdX_tight.Write () ;
  
  
  outputRootFile.Close () ;
  
  return 0 ;
}



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
  std::string outputRootName = "CRAFT_MUdistr.root" ;
  TFile outputRootFile (outputRootName.c_str (), "recreate") ;
  outputRootFile.cd () ;
  
  
  
  // output distributions
  TH2F MUoccupancy ("MUoccupancy", "MUoccupancy", PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  TH1F MUangleWRTvertical ("MUangleWRTvertical", "MUangleWRTvertical", PHI_BIN, 0., PHI_MAX/2.) ;
  
  
  TH1F MUd0 ("MUd0", "MUd0", X_BIN, X_MIN, X_MAX) ;
  TH1F MUdz ("MUdz", "MUdz", Z_BIN, Z_MIN, Z_MAX) ;
  
  
  TH2F MUtkLengthInEcal_straigth_curved_pT ("MUtkLengthInEcal_straigth_curved_pT", "MUtkLengthInEcal_straigth_curved_pT", 5*P_BIN, P_MIN, P_MAX,
                                                                                                                          1000, -5., 5.) ;
  
  TH2F MUglbTkInnerHit_YvsX_legUp ("MUglbTkInnerHit_YvsX_legUp", "MUglbTkInnerHit_YvsX_legUp", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH2F MUglbTkOuterHit_YvsX_legUp ("MUglbTkOuterHit_YvsX_legUp", "MUglbTkOuterHit_YvsX_legUp", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH1F MUglbTkHit_DeltaY_legUp ("MUglbTkHit_DeltaY_legUp", "MUglbTkHit_DeltaY_legUp", Y_BIN, 2*Y_MIN, 2*Y_MAX) ;
  
  TH2F MUinnTkInnerHit_YvsX_legUp ("MUinnTkInnerHit_YvsX_legUp", "MUinnTkInnerHit_YvsX_legUp", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH2F MUinnTkOuterHit_YvsX_legUp ("MUinnTkOuterHit_YvsX_legUp", "MUinnTkOuterHit_YvsX_legUp", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH1F MUinnTkHit_DeltaY_legUp ("MUinnTkHit_DeltaY_legUp", "MUinnTkHit_DeltaY_legUp", Y_BIN, 2*Y_MIN, 2*Y_MAX) ;
  
  TH2F MUoutTkInnerHit_YvsX_legUp ("MUoutTkInnerHit_YvsX_legUp", "MUoutTkInnerHit_YvsX_legUp", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH2F MUoutTkOuterHit_YvsX_legUp ("MUoutTkOuterHit_YvsX_legUp", "MUoutTkOuterHit_YvsX_legUp", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH1F MUoutTkHit_DeltaY_legUp ("MUoutTkHit_DeltaY_legUp", "MUoutTkHit_DeltaY_legUp", Y_BIN, 2*Y_MIN, 2*Y_MAX) ;
  
  
  
  TH2F MUglbTkInnerHit_YvsX_legDown ("MUglbTkInnerHit_YvsX_legDown", "MUglbTkInnerHit_YvsX_legDown", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH2F MUglbTkOuterHit_YvsX_legDown ("MUglbTkOuterHit_YvsX_legDown", "MUglbTkOuterHit_YvsX_legDown", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH1F MUglbTkHit_DeltaY_legDown ("MUglbTkHit_DeltaY_legDown", "MUglbTkHit_DeltaY_legDown", Y_BIN, 2*Y_MIN, 2*Y_MAX) ;
  
  TH2F MUinnTkInnerHit_YvsX_legDown ("MUinnTkInnerHit_YvsX_legDown", "MUinnTkInnerHit_YvsX_legDown", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH2F MUinnTkOuterHit_YvsX_legDown ("MUinnTkOuterHit_YvsX_legDown", "MUinnTkOuterHit_YvsX_legDown", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH1F MUinnTkHit_DeltaY_legDown ("MUinnTkHit_DeltaY_legDown", "MUinnTkHit_DeltaY_legDown", Y_BIN, 2*Y_MIN, 2*Y_MAX) ;
  
  TH2F MUoutTkInnerHit_YvsX_legDown ("MUoutTkInnerHit_YvsX_legDown", "MUoutTkInnerHit_YvsX_legDown", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH2F MUoutTkOuterHit_YvsX_legDown ("MUoutTkOuterHit_YvsX_legDown", "MUoutTkOuterHit_YvsX_legDown", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX) ;
  TH1F MUoutTkHit_DeltaY_legDown ("MUoutTkHit_DeltaY_legDown", "MUoutTkHit_DeltaY_legDown", Y_BIN, 2*Y_MIN, 2*Y_MAX) ;
  
  
  
  TH1F MUglbTkP_legUp ("MUglbTkP_legUp", "MUglbTkP_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUglbTkPt_legUp ("MUglbTkPt_legUp", "MUglbTkPt_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUglbTkDeltaP_legUp ("MUglbTkDeltaP_legUp", "MUglbTkDeltaP_legUp", P_BIN, -50., 50.) ;
  TH1F MUglbTkDeltaPt_legUp ("MUglbTkDeltaPt_legUp", "MUglbTkDeltaPt_legUp", P_BIN, -50., 50.) ;
  
  TH1F MUinnTkInnerHitP_legUp ("MUinnTkInnerHitP_legUp", "MUinnTkInnerHitP_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkInnerHitPt_legUp ("MUinnTkInnerHitPt_legUp", "MUinnTkInnerHitPt_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkOuterHitP_legUp ("MUinnTkOuterHitP_legUp", "MUinnTkOuterHitP_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkOuterHitPt_legUp ("MUinnTkOuterHitPt_legUp", "MUinnTkOuterHitPt_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkDeltaP_legUp ("MUinnTkDeltaP_legUp", "MUinnTkDeltaP_legUp", P_BIN, -50., 50.) ;
  TH1F MUinnTkDeltaPt_legUp ("MUinnTkDeltaPt_legUp", "MUinnTkDeltaPt_legUp", P_BIN, -50., 50.) ;
  
  TH1F MUoutTkInnerHitP_legUp ("MUoutTkInnerHitP_legUp", "MUoutTkInnerHitP_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkInnerHitPt_legUp ("MUoutTkInnerHitPt_legUp", "MUoutTkInnerHitPt_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkOuterHitP_legUp ("MUoutTkOuterHitP_legUp", "MUoutTkOuterHitP_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkOuterHitPt_legUp ("MUoutTkOuterHitPt_legUp", "MUoutTkOuterHitPt_legUp", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkDeltaP_legUp ("MUoutTkDeltaP_legUp", "MUoutTkDeltaP_legUp", P_BIN, -50., 50.) ;
  TH1F MUoutTkDeltaPt_legUp ("MUoutTkDeltaPt_legUp", "MUoutTkDeltaPt_legUp", P_BIN, -50., 50.) ;
  
  TH1F MUcaloDeltaP_legUp ("MUcaloDeltaP_legUp", "MUcaloDeltaP_legUp", P_BIN, -50., 50.) ;
  TH1F MUcaloDeltaPt_legUp ("MUcaloDeltaPt_legUp", "MUcaloDeltaPt_legUp", P_BIN, -50., 50.) ;
  
  
  
  TH1F MUglbTkP_legDown ("MUglbTkP_legDown", "MUglbTkP_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUglbTkPt_legDown ("MUglbTkPt_legDown", "MUglbTkPt_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUglbTkDeltaP_legDown ("MUglbTkDeltaP_legDown", "MUglbTkDeltaP_legDown", P_BIN, -50., 50.) ;
  TH1F MUglbTkDeltaPt_legDown ("MUglbTkDeltaPt_legDown", "MUglbTkDeltaPt_legDown", P_BIN, -50., 50.) ;
  
  TH1F MUinnTkInnerHitP_legDown ("MUinnTkInnerHitP_legDown", "MUinnTkInnerHitP_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkInnerHitPt_legDown ("MUinnTkInnerHitPt_legDown", "MUinnTkInnerHitPt_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkOuterHitP_legDown ("MUinnTkOuterHitP_legDown", "MUinnTkOuterHitP_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkOuterHitPt_legDown ("MUinnTkOuterHitPt_legDown", "MUinnTkOuterHitPt_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUinnTkDeltaP_legDown ("MUinnTkDeltaP_legDown", "MUinnTkDeltaP_legDown", P_BIN, -50., 50.) ;
  TH1F MUinnTkDeltaPt_legDown ("MUinnTkDeltaPt_legDown", "MUinnTkDeltaPt_legDown", P_BIN, -50., 50.) ;
  
  TH1F MUoutTkInnerHitP_legDown ("MUoutTkInnerHitP_legDown", "MUoutTkInnerHitP_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkInnerHitPt_legDown ("MUoutTkInnerHitPt_legDown", "MUoutTkInnerHitPt_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkOuterHitP_legDown ("MUoutTkOuterHitP_legDown", "MUoutTkOuterHitP_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkOuterHitPt_legDown ("MUoutTkOuterHitPt_legDown", "MUoutTkOuterHitPt_legDown", P_BIN, P_MIN, P_MAX) ;
  TH1F MUoutTkDeltaP_legDown ("MUoutTkDeltaP_legDown", "MUoutTkDeltaP_legDown", P_BIN, -50., 50.) ;
  TH1F MUoutTkDeltaPt_legDown ("MUoutTkDeltaPt_legDown", "MUoutTkDeltaPt_legDown", P_BIN, -50., 50.) ;
  
  TH1F MUcaloDeltaP_legDown ("MUcaloDeltaP_legDown", "MUcaloDeltaP_legDown", P_BIN, -50., 50.) ;
  TH1F MUcaloDeltaPt_legDown ("MUcaloDeltaPt_legDown", "MUcaloDeltaPt_legDown", P_BIN, -50., 50.) ;
  // output distributions



  int nMuons = 0;
  int nMuons_cut = 0;


  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    chain->GetEntry (entry) ;
    // if (entry == 100000) break ;
    
    
    for (int MUindex = 0; MUindex < treeVars.nRecoMuons; ++MUindex)
    {
      nMuons++;

      MUd0.Fill (treeVars.muond0[MUindex]) ;
      MUdz.Fill (treeVars.muondz[MUindex]) ;
       
      if (fabs(treeVars.muond0[MUindex])<100 && fabs(treeVars.muondz[MUindex])<100)
        nMuons_cut++;
      
      
      GlobalPoint muonTkInternalPointInEcal (treeVars.muonTkInternalPointInEcalX[MUindex],
                                             treeVars.muonTkInternalPointInEcalY[MUindex],
                                             treeVars.muonTkInternalPointInEcalZ[MUindex]) ;
      MUoccupancy.Fill (muonTkInternalPointInEcal.phi (), muonTkInternalPointInEcal.eta ()) ;
      
      if (treeVars.muonLeg[MUindex] == 1)
      {
        //MUoccupancy.Fill (treeVars.muonInnTkInnerHitPhi[MUindex], treeVars.muonInnTkInnerHitEta[MUindex]) ;
        
        TVector3 vertical (0., -1., 0.) ;
        TVector3 MUdirection (treeVars.muonOutTkInnerHitPx[MUindex] / treeVars.muonOutTkInnerHitP[MUindex],
                              treeVars.muonOutTkInnerHitPy[MUindex] / treeVars.muonOutTkInnerHitP[MUindex],
                              treeVars.muonOutTkInnerHitPz[MUindex] / treeVars.muonOutTkInnerHitP[MUindex]) ;
        MUangleWRTvertical.Fill ( MUdirection.Angle(vertical) ) ;
        
        
        if ( (treeVars.muonTkLengthInEcalDetail[MUindex] > 0.) && (treeVars.muonTkLengthInEcalDetailCurved[MUindex] > 0.) )
          MUtkLengthInEcal_straigth_curved_pT.Fill (treeVars.muonInnTkInnerHitPt[MUindex], treeVars.muonTkLengthInEcalDetail[MUindex] - 
                                                    treeVars.muonTkLengthInEcalDetailCurved[MUindex]) ;
        
        
        MUglbTkInnerHit_YvsX_legUp.Fill (treeVars.muonInnerHitX[MUindex], treeVars.muonInnerHitY[MUindex]) ;
        MUglbTkOuterHit_YvsX_legUp.Fill (treeVars.muonOuterHitX[MUindex], treeVars.muonOuterHitY[MUindex]) ;
        MUglbTkHit_DeltaY_legUp.Fill (treeVars.muonInnerHitY[MUindex] - treeVars.muonOuterHitY[MUindex]) ;
      
        MUinnTkInnerHit_YvsX_legUp.Fill (treeVars.muonInnTkInnerHitX[MUindex], treeVars.muonInnTkInnerHitY[MUindex]) ;
        MUinnTkOuterHit_YvsX_legUp.Fill (treeVars.muonInnTkOuterHitX[MUindex], treeVars.muonInnTkOuterHitY[MUindex]) ;
        MUinnTkHit_DeltaY_legUp.Fill (treeVars.muonInnTkInnerHitY[MUindex] - treeVars.muonInnTkOuterHitY[MUindex]) ;
      
        MUoutTkInnerHit_YvsX_legUp.Fill (treeVars.muonOutTkInnerHitX[MUindex], treeVars.muonOutTkInnerHitY[MUindex]) ;
        MUoutTkOuterHit_YvsX_legUp.Fill (treeVars.muonOutTkOuterHitX[MUindex], treeVars.muonOutTkOuterHitY[MUindex]) ;
        MUoutTkHit_DeltaY_legUp.Fill (treeVars.muonOutTkInnerHitY[MUindex] - treeVars.muonOutTkOuterHitY[MUindex]) ;
        
        
        
        MUglbTkP_legUp.Fill (treeVars.muonP[MUindex]) ;
        MUglbTkPt_legUp.Fill (treeVars.muonPt[MUindex]) ;
        MUglbTkDeltaP_legUp.Fill (treeVars.muonOutTkInnerHitP[MUindex] - treeVars.muonInnTkOuterHitP[MUindex]) ;
        MUglbTkDeltaPt_legUp.Fill (treeVars.muonOutTkInnerHitPt[MUindex] - treeVars.muonInnTkOuterHitPt[MUindex]) ;
        
        MUinnTkInnerHitP_legUp.Fill (treeVars.muonInnTkInnerHitP[MUindex]) ;
        MUinnTkInnerHitPt_legUp.Fill (treeVars.muonInnTkInnerHitPt[MUindex]) ;
        MUinnTkOuterHitP_legUp.Fill (treeVars.muonInnTkOuterHitP[MUindex]) ;
        MUinnTkOuterHitPt_legUp.Fill (treeVars.muonInnTkOuterHitPt[MUindex]) ;
        MUinnTkDeltaP_legUp.Fill (treeVars.muonInnTkInnerHitP[MUindex] - treeVars.muonInnTkOuterHitP[MUindex]) ;
        MUinnTkDeltaPt_legUp.Fill (treeVars.muonInnTkInnerHitPt[MUindex] - treeVars.muonInnTkOuterHitPt[MUindex]) ;
        
        MUoutTkInnerHitP_legUp.Fill (treeVars.muonOutTkInnerHitP[MUindex]) ;
        MUoutTkInnerHitPt_legUp.Fill (treeVars.muonOutTkInnerHitPt[MUindex]) ;
        MUoutTkOuterHitP_legUp.Fill (treeVars.muonOutTkOuterHitP[MUindex]) ;
        MUoutTkOuterHitPt_legUp.Fill (treeVars.muonOutTkOuterHitPt[MUindex]) ;
        MUoutTkDeltaP_legUp.Fill (treeVars.muonOutTkInnerHitP[MUindex] - treeVars.muonOutTkOuterHitP[MUindex]) ;
        MUoutTkDeltaPt_legUp.Fill (treeVars.muonOutTkInnerHitPt[MUindex] - treeVars.muonOutTkOuterHitPt[MUindex]) ;

        MUcaloDeltaP_legUp.Fill (treeVars.muonOutTkOuterHitP[MUindex] - treeVars.muonInnTkInnerHitP[MUindex]) ;
        MUcaloDeltaPt_legUp.Fill (treeVars.muonOutTkOuterHitPt[MUindex] - treeVars.muonInnTkInnerHitPt[MUindex]) ;
      }
      
      
      
      if (treeVars.muonLeg[MUindex] == -1)
      {
        //MUoccupancy.Fill (treeVars.muonInnTkOuterHitPhi[MUindex], treeVars.muonInnTkOuterHitEta[MUindex]) ;
        
        if ( (treeVars.muonTkLengthInEcalDetail[MUindex] > 0.) && (treeVars.muonTkLengthInEcalDetailCurved[MUindex] > 0.) )
          MUtkLengthInEcal_straigth_curved_pT.Fill (treeVars.muonInnTkOuterHitPt[MUindex], treeVars.muonTkLengthInEcalDetail[MUindex] - 
                                                    treeVars.muonTkLengthInEcalDetailCurved[MUindex]) ;
        
        MUglbTkInnerHit_YvsX_legDown.Fill (treeVars.muonInnerHitX[MUindex], treeVars.muonInnerHitY[MUindex]) ;
        MUglbTkOuterHit_YvsX_legDown.Fill (treeVars.muonOuterHitX[MUindex], treeVars.muonOuterHitY[MUindex]) ;
        MUglbTkHit_DeltaY_legDown.Fill (treeVars.muonInnerHitY[MUindex] - treeVars.muonOuterHitY[MUindex]) ;
      
        MUinnTkInnerHit_YvsX_legDown.Fill (treeVars.muonInnTkInnerHitX[MUindex], treeVars.muonInnTkInnerHitY[MUindex]) ;
        MUinnTkOuterHit_YvsX_legDown.Fill (treeVars.muonInnTkOuterHitX[MUindex], treeVars.muonInnTkOuterHitY[MUindex]) ;
        MUinnTkHit_DeltaY_legDown.Fill (treeVars.muonInnTkInnerHitY[MUindex] - treeVars.muonInnTkOuterHitY[MUindex]) ;
      
        MUoutTkInnerHit_YvsX_legDown.Fill (treeVars.muonOutTkInnerHitX[MUindex], treeVars.muonOutTkInnerHitY[MUindex]) ;
        MUoutTkOuterHit_YvsX_legDown.Fill (treeVars.muonOutTkOuterHitX[MUindex], treeVars.muonOutTkOuterHitY[MUindex]) ;
        MUoutTkHit_DeltaY_legDown.Fill (treeVars.muonOutTkInnerHitY[MUindex] - treeVars.muonOutTkOuterHitY[MUindex]) ;
        
        
        
        MUglbTkP_legDown.Fill (treeVars.muonP[MUindex]) ;
        MUglbTkPt_legDown.Fill (treeVars.muonPt[MUindex]) ;
        MUglbTkDeltaP_legDown.Fill (treeVars.muonInnTkInnerHitP[MUindex] - treeVars.muonOutTkOuterHitP[MUindex]) ;
        MUglbTkDeltaPt_legDown.Fill (treeVars.muonInnTkInnerHitPt[MUindex] - treeVars.muonOutTkOuterHitPt[MUindex]) ;
        
        MUinnTkInnerHitP_legDown.Fill (treeVars.muonInnTkInnerHitP[MUindex]) ;
        MUinnTkInnerHitPt_legDown.Fill (treeVars.muonInnTkInnerHitPt[MUindex]) ;
        MUinnTkOuterHitP_legDown.Fill (treeVars.muonInnTkOuterHitP[MUindex]) ;
        MUinnTkOuterHitPt_legDown.Fill (treeVars.muonInnTkOuterHitPt[MUindex]) ;
        MUinnTkDeltaP_legDown.Fill (treeVars.muonInnTkInnerHitP[MUindex] - treeVars.muonInnTkOuterHitP[MUindex]) ;
        MUinnTkDeltaPt_legDown.Fill (treeVars.muonInnTkInnerHitPt[MUindex] - treeVars.muonInnTkOuterHitPt[MUindex]) ;
        
        MUoutTkInnerHitP_legDown.Fill (treeVars.muonOutTkInnerHitP[MUindex]) ;
        MUoutTkInnerHitPt_legDown.Fill (treeVars.muonOutTkInnerHitPt[MUindex]) ;
        MUoutTkOuterHitP_legDown.Fill (treeVars.muonOutTkOuterHitP[MUindex]) ;
        MUoutTkOuterHitPt_legDown.Fill (treeVars.muonOutTkOuterHitPt[MUindex]) ;
        MUoutTkDeltaP_legDown.Fill (treeVars.muonOutTkInnerHitP[MUindex] - treeVars.muonOutTkOuterHitP[MUindex]) ;
        MUoutTkDeltaPt_legDown.Fill (treeVars.muonOutTkInnerHitPt[MUindex] - treeVars.muonOutTkOuterHitPt[MUindex]) ;

        MUcaloDeltaP_legDown.Fill (treeVars.muonInnTkOuterHitP[MUindex] - treeVars.muonOutTkInnerHitP[MUindex]) ;
        MUcaloDeltaPt_legDown.Fill (treeVars.muonInnTkOuterHitPt[MUindex] - treeVars.muonOutTkInnerHitPt[MUindex]) ;
      }
    
    }
  
  
  } //PG loop over entries

  std::cout << "eff = " << 1.*nMuons_cut/nMuons << std::endl;

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  
  
  MUoccupancy.Write () ;
  MUangleWRTvertical.Write () ;
  
  MUd0.Write () ;
  MUdz.Write () ;
  
  MUtkLengthInEcal_straigth_curved_pT.Write () ;
  
  MUglbTkInnerHit_YvsX_legUp.Write () ;
  MUglbTkOuterHit_YvsX_legUp.Write () ;
  MUglbTkHit_DeltaY_legUp.Write () ;
  
  MUinnTkInnerHit_YvsX_legUp.Write () ;
  MUinnTkOuterHit_YvsX_legUp.Write () ;
  MUinnTkHit_DeltaY_legUp.Write () ;
  
  MUoutTkInnerHit_YvsX_legUp.Write () ;
  MUoutTkOuterHit_YvsX_legUp.Write () ;
  MUoutTkHit_DeltaY_legUp.Write () ;
    
  MUglbTkP_legUp.Write () ;
  MUglbTkPt_legUp.Write () ;
  MUglbTkDeltaP_legUp.Write () ;
  MUglbTkDeltaPt_legUp.Write () ;
  
  MUinnTkInnerHitP_legUp.Write () ;
  MUinnTkInnerHitPt_legUp.Write () ;
  MUinnTkOuterHitP_legUp.Write () ;
  MUinnTkOuterHitPt_legUp.Write () ;
  MUinnTkDeltaP_legUp.Write () ;
  MUinnTkDeltaPt_legUp.Write () ;
  
  MUoutTkInnerHitP_legUp.Write () ;
  MUoutTkInnerHitPt_legUp.Write () ;
  MUoutTkOuterHitP_legUp.Write () ;
  MUoutTkOuterHitPt_legUp.Write () ;
  MUoutTkDeltaP_legUp.Write () ;
  MUoutTkDeltaPt_legUp.Write () ;
  
  MUcaloDeltaP_legUp.Write () ;
  MUcaloDeltaPt_legUp.Write () ;
  
  
  
  MUglbTkInnerHit_YvsX_legDown.Write () ;
  MUglbTkOuterHit_YvsX_legDown.Write () ;
  MUglbTkHit_DeltaY_legDown.Write () ;
  
  MUinnTkInnerHit_YvsX_legDown.Write () ;
  MUinnTkOuterHit_YvsX_legDown.Write () ;
  MUinnTkHit_DeltaY_legDown.Write () ;
  
  MUoutTkInnerHit_YvsX_legDown.Write () ;
  MUoutTkOuterHit_YvsX_legDown.Write () ;
  MUoutTkHit_DeltaY_legDown.Write () ;
    
  MUglbTkP_legDown.Write () ;
  MUglbTkPt_legDown.Write () ;
  MUglbTkDeltaP_legDown.Write () ;
  MUglbTkDeltaPt_legDown.Write () ;
  
  MUinnTkInnerHitP_legDown.Write () ;
  MUinnTkInnerHitPt_legDown.Write () ;
  MUinnTkOuterHitP_legDown.Write () ;
  MUinnTkOuterHitPt_legDown.Write () ;
  MUinnTkDeltaP_legDown.Write () ;
  MUinnTkDeltaPt_legDown.Write () ;
  
  MUoutTkInnerHitP_legDown.Write () ;
  MUoutTkInnerHitPt_legDown.Write () ;
  MUoutTkOuterHitP_legDown.Write () ;
  MUoutTkOuterHitPt_legDown.Write () ;
  MUoutTkDeltaP_legDown.Write () ;
  MUoutTkDeltaPt_legDown.Write () ;
  
  MUcaloDeltaP_legDown.Write () ;
  MUcaloDeltaPt_legDown.Write () ;

  saving.Close () ;
  
  return 0 ;
}



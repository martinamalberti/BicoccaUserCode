#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "CRUZET/NTuplesTest/interface/NTuplesUtils.h"

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

#define PI 3.14159265

#define PHI_MIN -3.1416
#define PHI_MAX +3.1416
#define PHI_BIN 360

#define ETA_MIN -3.0
#define ETA_MAX +3.0
#define ETA_BIN 340

#define IPHI_MIN 0.
#define IPHI_MAX +361
#define IPHI_BIN 361

#define IETA_MIN -80.
#define IETA_MAX +80.
#define IETA_BIN 361

#define X_BIN 1000
#define X_MIN -800
#define X_MAX +800

#define Y_BIN 1000
#define Y_MIN -800
#define Y_MAX +800

#define Z_BIN 1000
#define Z_MIN -800
#define Z_MAX +800

#define R_BIN 1000
#define R_MIN -800
#define R_MAX +800

#define P_BIN 500
#define P_MIN 0
#define P_MAX 1000






int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  int maxEvent = subPSetSelections.getUntrackedParameter<int>("maxEvent", -1);
  
  double muonPMAX = subPSetSelections.getUntrackedParameter<double>("muonPMAX", 1000.);
  double muonPMIN = subPSetSelections.getUntrackedParameter<double>("muonPMIN", 1.);
  double muonPErrorOverPMAX = subPSetSelections.getUntrackedParameter<double>("muonPErrorOverPMAX", 1000000000000.);
  
  bool muonLegUpOK = subPSetSelections.getUntrackedParameter<bool>("muonLegUpOK", false);
  bool muonLegDownOK = subPSetSelections.getUntrackedParameter<bool>("muonLegDownOK", true);
  
  double muond0MAX = subPSetSelections.getUntrackedParameter<double>("muond0MAX", 1000000000000.);
  double muondzMAX = subPSetSelections.getUntrackedParameter<double>("muondzMAX", 1000000000000.);
  double muondRMAX = subPSetSelections.getUntrackedParameter<double>("muondRMAX", 1000000000000.);
  
  double muonNChi2MAX = subPSetSelections.getUntrackedParameter<double>("muonNChi2MAX", 1000000000000.);
  double muonNHitsMIN = subPSetSelections.getUntrackedParameter<double>("muonNHitsMIN", 1.);
  
  double muonTkLengthInEcalMIN = subPSetSelections.getUntrackedParameter<double>("muonTkLengthInEcalMIN", 1.);
  double muonTkLengthInEcalMAX = subPSetSelections.getUntrackedParameter<double>("muonTkLengthInEcalMAX", 1000000000000.);

  double muonAngleMAX = subPSetSelections.getUntrackedParameter<double>("muonAngleMAX", 1000000000000.);
  double muonAngleMIN = subPSetSelections.getUntrackedParameter<double>("muonAngleMIN", 1000000000000.);
  
  
  edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  
  
  
  
  
  
  // Load ntuple
  TChain* chain = new TChain("EcalCosmicsAnalysis");
  EcalCosmicsTreeContent treeVars;
  setBranchAddresses(chain, treeVars);
  // Load ntuple
  
  
  
  
  
  
  // Input files
  for(std::vector<std::string>::const_iterator listIt = inputFiles.begin();
      listIt != inputFiles.end(); ++listIt)
    {
      chain -> Add (listIt -> c_str());
      std::cout << *listIt << std::endl;
    }

  int nEntries = chain -> GetEntries();
  std::cout << "FOUND " << nEntries << " ENTRIES" << std::endl;
  // Input files






  // Output file
  std::string outRootFileName = "CRAFT_MUdistr.root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  // Output file
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // -------------------------------------------------------------------------------------- 
  
  TH1F events("events", "events", 100, 0., 100.);
  TH1F efficiencies("efficiencies", "efficiencies", 100, 0., 100.);
  
  
  
  TH1F muonLegDistr("muonLegDistr", "muonLegDistr", 3, -1., 2.);
  TH1F muonChargeDistr("muonChargeDistr", "muonChargeDistr", 3, -1., 2.);
  
  TH1F muonPDistr("muonPDistr", "muonPDistr", 250, 0., 5.);  
  BinLogX(muonPDistr);  
  TH1F muonPtDistr("muonPtDistr", "muonPtDistr", 250, 0., 5.);  
  BinLogX(muonPtDistr);  

  TH1F d0Distr("d0Distr", "d0Distr", R_BIN, -R_MAX, R_MAX);
  TH1F dzDistr("dzDistr", "dzDistr", Z_BIN, Z_MIN, Z_MAX);
  TH1F dRDistr("dRDistr", "dRDistr", R_BIN, R_MIN, R_MAX);
  
  TH1F nChi2Distr("nChi2Distr", "nChi2Distr", 1000, -2., 4);
  TH1F nHitsDistr("nHitsDistr", "nHitsDistr", 100, 0., 100.);
  
  TH1F muonTkLengthInEcalCurvedDistr("muonTkLengthInEcalCurvedDistr", "muonTkLengthInEcalCurvedDistr", 10000, 0., 100.);
  
  TH1F muonXtalAngleDistr("muonXtalAngleDistr", "muonXtalAngleDistr", 1000, 0., PI/2.);
  TH1F muonSkyAngleDistr("muonSkyAngleDistr", "muonSkyAngleDistr", 1000, 0., PI/2.);
  
  TH1F muonPErrorOverPDistr("muonPErrorOverPDistr", "muonPErrorOverPDistr", 1000, 0., 10.);
  TH2F muonPErrorOverP_vsP("muonPErrorOverP_vsP", "muonPErrorOverP_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 10.);
  BinLogX(muonPErrorOverP_vsP);  
  TProfile muonPErrorOverP_profile_vsP("muonPErrorOverP_profile_vsP", "muonPErrorOverP_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(muonPErrorOverP_profile_vsP);  
  
  
  TH2F internalPointInEcalEta_vsPhi("internalPointInEcalEta_vsPhi", "internalPointInEcalEta_vsPhi", PHI_BIN, PHI_MIN, PHI_MAX,
                                                                                                    ETA_BIN, ETA_MIN, ETA_MAX);
  TH2F internalPointInEcalY_vsX("internalPointInEcalY_vsX", "internalPointInEcalY_vsX", X_BIN, X_MIN, X_MAX,
                                                                                        Y_BIN, Y_MIN, Y_MAX);
  TH2F internalPointInEcalR_vsZ("internalPointInEcalR_vsZ", "internalPointInEcalR_vsZ", R_BIN, R_MIN, R_MAX,
                                                                                        Z_BIN, Z_MIN, Z_MAX);
  
  TH2F externalPointInEcalEta_vsPhi("externalPointInEcalEta_vsPhi", "externalPointInEcalEta_vsPhi", PHI_BIN, PHI_MIN, PHI_MAX,
                                                                                                    ETA_BIN, ETA_MIN, ETA_MAX);
  TH2F externalPointInEcalY_vsX("externalPointInEcalY_vsX", "externalPointInEcalY_vsX", X_BIN, X_MIN, X_MAX,
                                                                                        Y_BIN, Y_MIN, Y_MAX);
  TH2F externalPointInEcalR_vsZ("externalPointInEcalR_vsZ", "externalPointInEcalR_vsZ", R_BIN, R_MIN, R_MAX,
                                                                                        Z_BIN, Z_MIN, Z_MAX);
  
   TH2F innTkHitY_vsX("innTkHitY_vsX", "innTkHitY_vsX", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX);
   TH2F innTkHitR_vsZ("innTkHitR_vsZ", "innTkHitR_vsZ", R_BIN, R_MIN, R_MAX, Z_BIN, Z_MIN, Z_MAX);
   
   TH2F outTkHitY_vsX("outTkHitY_vsX", "outTkHitY_vsX", X_BIN, X_MIN, X_MAX, Y_BIN, Y_MIN, Y_MAX);
   TH2F outTkHitR_vsZ("outTkHitR_vsZ", "outTkHitR_vsZ", R_BIN, R_MIN, R_MAX, Z_BIN, Z_MIN, Z_MAX);
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Loop over entries
  // -------------------------------------------------------------------------------------- 
  
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    if(entry == maxEvent) break;
    
    chain -> GetEntry(entry);
    events.SetBinContent(1, events.GetBinContent(1) + 2);
    
    
    
    
    
    
    // Cut on number of reconstructed muons
    int nRecoMuons = treeVars.nRecoMuons;
    int nRecoMuons_up = 0;
    int nRecoMuons_down = 0;
    for(int MUit = 0; MUit < nRecoMuons; ++MUit)
      {
	if(treeVars.muonLeg[MUit] == 1) ++nRecoMuons_up;
	if(treeVars.muonLeg[MUit] == -1) ++nRecoMuons_down;
      }

    if( (nRecoMuons != 2) ||
        ( (nRecoMuons == 2) && (nRecoMuons_up > 1) ) ||
        ( (nRecoMuons == 2) && (nRecoMuons_down > 1) ) ) continue;

    events.SetBinContent(2, events.GetBinContent(2) + 2);
    // Cut on number of reconstructed muons
    
    
    
    

        
    // loop on muons
    for(int MUindex = 0; MUindex < treeVars.nRecoMuons; ++MUindex)
    {
      // Muon variables
      int muonLeg = treeVars.muonLeg[MUindex];
      float muonCharge = treeVars.muonCharge[MUindex];
      
      float muonP = treeVars.muonP[MUindex];
      float muonPt = treeVars.muonPt[MUindex];
      
      float muond0 = treeVars.muond0[MUindex];
      float muondz = treeVars.muondz[MUindex];
      float muondR = sqrt( pow(muond0, 2.) + pow(muondz, 2.) );
      
      float muonNChi2 = treeVars.muonNChi2[MUindex];
      float muonNHits = treeVars.muonNHits[MUindex];
      
      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex];
      
      GlobalPoint muonTkInternalPointInEcal(treeVars.muonTkInternalPointInEcalCurvedX[MUindex],
                                            treeVars.muonTkInternalPointInEcalCurvedY[MUindex],
                                            treeVars.muonTkInternalPointInEcalCurvedZ[MUindex]);
      GlobalPoint muonTkExternalPointInEcal(treeVars.muonTkExternalPointInEcalCurvedX[MUindex],
                                            treeVars.muonTkExternalPointInEcalCurvedY[MUindex],
                                            treeVars.muonTkExternalPointInEcalCurvedZ[MUindex]);
      
      TVector3 muonInnTkInnerPoint(treeVars.muonInnTkInnerHitX[MUindex],
                                   treeVars.muonInnTkInnerHitY[MUindex],
                                   treeVars.muonInnTkInnerHitZ[MUindex]);
      TVector3 muonInnTkOuterPoint(treeVars.muonInnTkOuterHitX[MUindex],
                                   treeVars.muonInnTkOuterHitY[MUindex],
                                   treeVars.muonInnTkOuterHitZ[MUindex]);
      TVector3 muonOutTkInnerPoint(treeVars.muonOutTkInnerHitX[MUindex],
                                   treeVars.muonOutTkInnerHitY[MUindex],
                                   treeVars.muonOutTkInnerHitZ[MUindex]);
      TVector3 muonOutTkOuterPoint(treeVars.muonOutTkOuterHitX[MUindex],
                                   treeVars.muonOutTkOuterHitY[MUindex],
                                   treeVars.muonOutTkOuterHitZ[MUindex]);

      TVector3 muonInnTkInnerMomentum(treeVars.muonInnTkInnerHitPx[MUindex],
                                      treeVars.muonInnTkInnerHitPy[MUindex],
                                      treeVars.muonInnTkInnerHitPz[MUindex]);
      TVector3 muonInnTkOuterMomentum(treeVars.muonInnTkOuterHitPx[MUindex],
                                      treeVars.muonInnTkOuterHitPy[MUindex],
                                      treeVars.muonInnTkOuterHitPz[MUindex]);
      TVector3 muonOutTkInnerMomentum(treeVars.muonOutTkInnerHitPx[MUindex],
                                      treeVars.muonOutTkInnerHitPy[MUindex],
                                      treeVars.muonOutTkInnerHitPz[MUindex]);
      TVector3 muonOutTkOuterMomentum(treeVars.muonOutTkOuterHitPx[MUindex],
                                      treeVars.muonOutTkOuterHitPy[MUindex],
                                      treeVars.muonOutTkOuterHitPz[MUindex]);

      TVector3 muonDirection(treeVars.muonTkInternalPointInEcalCurvedPx[MUindex],
                             treeVars.muonTkInternalPointInEcalCurvedPy[MUindex],
                             treeVars.muonTkInternalPointInEcalCurvedPz[MUindex]);
      
      TVector3 xtalAxisDirection;
      TVector3 yAxisDirection(0., -1., 0.);
      setVectorOnECAL(xtalAxisDirection, muonTkInternalPointInEcal.eta(), muonTkInternalPointInEcal.phi(), 129.);
      
      float muonXtalAngle = xtalAxisDirection.Angle(-1. * muonLeg * muonDirection);
      float muonSkyAngle = yAxisDirection.Angle(muonOutTkInnerMomentum);
      
      float muonQOverP = 1. * muonCharge / muonP;
      float muonQOverPError = treeVars.muonQOverPError[MUindex];
      float muonPError = 1. * muonQOverPError / muonQOverP / muonQOverP;
      float muonPErrorOverP = 1. * muonPError / muonP;
      
      
      
      
      
      
      // --------------------------------------------------------------------------------------
      // Cut event
      // -------------------------------------------------------------------------------------- 

      if ( (muonP < muonPMIN) || (muonP > muonPMAX) ) continue ;
      events.SetBinContent(3, events.GetBinContent(3) + 1);
      
      if ( muonPErrorOverP > muonPErrorOverPMAX) continue ;
      events.SetBinContent(4, events.GetBinContent(4) + 1);

      if (fabs(muond0) > muond0MAX) continue;
      events.SetBinContent(5, events.GetBinContent(5) + 1);
      
      if (fabs(muondz) > muondzMAX) continue;
      events.SetBinContent(6, events.GetBinContent(6) + 1);
      
      if (muondR > muondRMAX) continue;
      events.SetBinContent(7, events.GetBinContent(7) + 1);
      
      if (muonNChi2 > muonNChi2MAX) continue;
      events.SetBinContent(8, events.GetBinContent(8) + 1);

      if (muonNHits < muonNHitsMIN) continue;
      events.SetBinContent(9, events.GetBinContent(9) + 1);

      if ( (muonTkLengthInEcalCurved < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcalCurved > muonTkLengthInEcalMAX) ) continue;
      events.SetBinContent(10, events.GetBinContent(10) + 1);

      if (muonXtalAngle > muonAngleMAX) continue;
      events.SetBinContent(11, events.GetBinContent(11) + 1);
      
      if (muonXtalAngle < muonAngleMIN) continue;
      events.SetBinContent(12, events.GetBinContent(12) + 1);
      
      if ( (muonLegUpOK == true && muonLegDownOK == false && muonLeg != 1) ||
           (muonLegDownOK == true && muonLegUpOK == false && muonLeg != -1) ) continue;
      events.SetBinContent(13, events.GetBinContent(13) + 1);
      
      
      
      
      
      
      // --------------------------------------------------------------------------------------
      // Fill distributions
      // -------------------------------------------------------------------------------------- 
      
      muonLegDistr.Fill(muonLeg);
      muonChargeDistr.Fill(muonCharge);
      
      muonPDistr.Fill(muonP);
      muonPtDistr.Fill(muonPt);
      
      d0Distr.Fill(muond0);
      dzDistr.Fill(muondz);
      dRDistr.Fill(muondR);
      
      nChi2Distr.Fill(muonNChi2);
      nHitsDistr.Fill(muonNHits);
      
      muonTkLengthInEcalCurvedDistr.Fill(muonTkLengthInEcalCurved);

      muonXtalAngleDistr.Fill(muonXtalAngle);
      if(muonLeg == 1)
        muonSkyAngleDistr.Fill(muonSkyAngle);
      
      muonPErrorOverPDistr.Fill(muonPErrorOverP);
      muonPErrorOverP_vsP.Fill(muonP, muonPErrorOverP);
      muonPErrorOverP_profile_vsP.Fill(muonP, muonPErrorOverP);
      
      float internalSign = 1. * muonTkInternalPointInEcal.z() / fabs(muonTkInternalPointInEcal.z());    
      internalPointInEcalEta_vsPhi.Fill(muonTkInternalPointInEcal.phi(), muonTkInternalPointInEcal.eta());
      internalPointInEcalY_vsX.Fill    (muonTkInternalPointInEcal.x(),   muonTkInternalPointInEcal.y());
      internalPointInEcalR_vsZ.Fill    (muonTkInternalPointInEcal.z(),   internalSign * muonTkInternalPointInEcal.perp());
      
      float externalSign = 1. * muonTkExternalPointInEcal.z() / fabs(muonTkExternalPointInEcal.z());          
      externalPointInEcalEta_vsPhi.Fill(muonTkExternalPointInEcal.phi(), muonTkExternalPointInEcal.eta());
      externalPointInEcalY_vsX.Fill    (muonTkExternalPointInEcal.x(),   muonTkExternalPointInEcal.y());
      externalPointInEcalR_vsZ.Fill    (muonTkExternalPointInEcal.z(),   externalSign * muonTkExternalPointInEcal.perp());
      
      float innTkSign = 1. * muonInnTkInnerPoint.z() / fabs(muonInnTkInnerPoint.z());      
      innTkHitY_vsX.Fill(muonInnTkInnerPoint.x(), muonInnTkInnerPoint.y());
      innTkHitR_vsZ.Fill(muonInnTkInnerPoint.z(), innTkSign * muonInnTkInnerPoint.Perp());
      innTkSign = 1. * muonInnTkOuterPoint.z() / fabs(muonInnTkOuterPoint.z());      
      innTkHitY_vsX.Fill(muonInnTkOuterPoint.x(), muonInnTkOuterPoint.y());
      innTkHitR_vsZ.Fill(muonInnTkOuterPoint.z(), innTkSign * muonInnTkOuterPoint.Perp());
      
      float outTkSign = 1. * muonOutTkInnerPoint.z() / fabs(muonOutTkInnerPoint.z());      
      outTkHitY_vsX.Fill(muonOutTkInnerPoint.x(), muonOutTkInnerPoint.y());
      outTkHitR_vsZ.Fill(muonOutTkInnerPoint.z(), outTkSign * muonOutTkInnerPoint.Perp());
      outTkSign = 1. * muonOutTkOuterPoint.z() / fabs(muonOutTkOuterPoint.z());      
      outTkHitY_vsX.Fill(muonOutTkOuterPoint.x(), muonOutTkOuterPoint.y());
      outTkHitR_vsZ.Fill(muonOutTkOuterPoint.z(), outTkSign * muonOutTkOuterPoint.Perp());
      
    } // loop on muons
  
  } // loop over entries
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Cut event
  // -------------------------------------------------------------------------------------- 
  
  muonLegDistr.Write();
  muonChargeDistr.Write();
  
  muonPDistr.Write();
  muonPtDistr.Write();
  
  d0Distr.Write();
  dzDistr.Write();
  dRDistr.Write();
  
  nChi2Distr.Write();
  nHitsDistr.Write();
  
  muonTkLengthInEcalCurvedDistr.Write();
  
  muonXtalAngleDistr.Write();
  muonSkyAngleDistr.Write();
  
  muonPErrorOverPDistr.Write();
  muonPErrorOverP_vsP.Write();
  muonPErrorOverP_profile_vsP.Write();
  
  
  internalPointInEcalEta_vsPhi.Write();
  internalPointInEcalY_vsX.Write();
  internalPointInEcalR_vsZ.Write();

  externalPointInEcalEta_vsPhi.Write();
  externalPointInEcalY_vsX.Write();
  externalPointInEcalR_vsZ.Write();
  
  
  innTkHitY_vsX.Write();
  innTkHitR_vsZ.Write();

  outTkHitY_vsX.Write();
  outTkHitR_vsZ.Write();
  
  
  outRootFile.Close () ;
  
  return 0 ;
}



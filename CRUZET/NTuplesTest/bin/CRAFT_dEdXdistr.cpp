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
#include "CRUZET/Calibration/interface/CRUtils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

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

#define PI 3.14159265

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

#define P_MIN 0
#define P_MAX 3


template <class THisto>
void BinLogX(THisto& h);
void LangausFit(TF1** langaus, TH1D* histo, double* startValues);



int main (int argc, char** argv)
{
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet();
  
  edm::ParameterSet subPSetSelections =  parameterSet -> getParameter<edm::ParameterSet>("selections");
  int P_BIN = subPSetSelections.getUntrackedParameter<int>("P_BIN", 25);
  bool muonLegUpOK = subPSetSelections.getUntrackedParameter<bool>("muonLegUpOK", false);
  bool muonLegDownOK = subPSetSelections.getUntrackedParameter<bool>("muonLegDownOK", true);
  double muonPMAX = subPSetSelections.getUntrackedParameter<double>("muonPMAX", 10000.);
  double muonPMIN = subPSetSelections.getUntrackedParameter<double>("muonPMIN", 0.);
  double muond0MAX = subPSetSelections.getUntrackedParameter<double>("muond0MAX", 1000.);
  double muondzMAX = subPSetSelections.getUntrackedParameter<double>("muondzMAX", 1000.);
  double muondRMAX = subPSetSelections.getUntrackedParameter<double>("muondRMAX", 1000.);
  double muonNChi2MAX = subPSetSelections.getUntrackedParameter<double>("muonNChi2MAX", 1000000.);
  double muonNHitsMIN = subPSetSelections.getUntrackedParameter<double>("muonNHitsMIN", 0.);
  double muonTkLengthInEcalMIN = subPSetSelections.getUntrackedParameter<double>("muonTkLengthInEcalMIN", 0.);
  double muonTkLengthInEcalMAX = subPSetSelections.getUntrackedParameter<double>("muonTkLengthInEcalMAX", 1000.);
  double muonEoverPMAX = subPSetSelections.getUntrackedParameter<double>("muonEoverPMAX", 1000.);
  double muonAngleMAX = subPSetSelections.getUntrackedParameter<double>("muonAngleMAX", 3.15);
  
  double superClusterPhiMIN = subPSetSelections.getUntrackedParameter<double>("superClusterPhiMIN", -3.15);
  double superClusterPhiMAX = subPSetSelections.getUntrackedParameter<double>("superClusterPhiMAX", 3.15);
  double superClusterEtaMIN = subPSetSelections.getUntrackedParameter<double>("superClusterEtaMIN", -1000.);
  double superClusterEtaMAX = subPSetSelections.getUntrackedParameter<double>("superClusterEtaMAX", 1000.);
  
  std::vector<int> badRegionIPhiMIN = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIPhiMIN", std::vector<int>(0));
  std::vector<int> badRegionIPhiMAX = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIPhiMAX", std::vector<int>(0));
  std::vector<int> badRegionIEtaMIN = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIEtaMIN", std::vector<int>(0));
  std::vector<int> badRegionIEtaMAX = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIEtaMAX", std::vector<int>(0));
  
  
  edm::ParameterSet subPSetInput = parameterSet -> getParameter<edm::ParameterSet>("inputNtuples");
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles");
  
  
  
  // load ntuple
  TChain* chain = new TChain("EcalCosmicsAnalysis");
  EcalCosmicsTreeContent treeVars;
  setBranchAddresses(chain, treeVars);
  
  
  
  // input files
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin();
       listIt != inputFiles.end(); ++listIt)
  {
    std::cout << *listIt << " " << std::endl;
    chain -> Add (listIt->c_str());
  }
  
  int nEntries = chain -> GetEntries();
  std::cout << "FOUND " << nEntries << " ENTRIES\n";
  
  
  
  // Output file
  std::string outputRootName = "CRAFT_dEdXdistr.root";
  TFile outputRootFile(outputRootName.c_str(), "recreate");
  outputRootFile.cd();
  std::ofstream outFile("stoppingPower_exp.dat", std::ios::out);
  
  
  
  // output distributions
  TH2F muonOccupancyInt_ETAvsPHI("muonOccupancyInt_ETAvsPHI", "muonOccupancyInt_ETAvsPHI",
                                 PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  TH2F muonOccupancyExt_ETAvsPHI("muonOccupancyExt_ETAvsPHI", "muonOccupancyExt_ETAvsPHI",
                                 PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  TH2F superClusterOccupancy_ETAvsPHI("superClusterOccupancy_ETAvsPHI", "superClusterOccupancy_ETAvsPHI",
                                      PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  TH2F xtalOccupancy_iETAvsiPHI("xtalOccupancy_iETAvsiPHI", "xtalOccupancy_iETAvsiPHI",
                                361, 1., 361., 171, -85., 86.);
  
  
  TH1F muond0Distr("muond0Distr", "muond0Distr", 1000, -200., 200.);
  TH1F muondzDistr("muondzDistr", "muondzDistr", 1000, -200., 200.);
  TH1F muondRDistr("muondRDistr", "muondRDistr", 1000, 0., 400.);
  TH2F muondR_P("muondR_P", "muondR_P", P_BIN, P_MIN, P_MAX, 500, 0., 200.);
  BinLogX(muondR_P);
  TProfile muondR_profile_P("muondR_profile_P", "muondR_profile_P", P_BIN, P_MIN, P_MAX);
  BinLogX(muondR_profile_P);
  TH2F muondR_E("muondR_E", "muondR_E", 1000, 0., 10., 500, 0., 200.);
  TProfile muondR_profile_E("muondR_profile_E", "muondR_profile_E", 1000, 0., 10.);
  
  TH1F muonNChi2Distr("muonNChi2Distr", "muonNChi2Distr", 100000, 0., 10000.);
  TH1F muonNHitsDistr("muonNHitsDistr", "muonNHitsDistr", 100, 0., 100.);
  
  TH1F muonAngleDistr("muonAngleDistr", "muonAngleDistr", 1000, 0., 1.57);
  TH2F muonAngle_P("muonAngle_P", "muonAngle_P", P_BIN, P_MIN, P_MAX, 1000, 0., 1.57);
  BinLogX(muonAngle_P);
  TProfile muonAngle_profile_P("muonAngle_profile_P", "muonAngle_profile_P", P_BIN, P_MIN, P_MAX);
  BinLogX(muonAngle_profile_P);
  TH2F muonAngle_E("muonAngle_E", "muonAngle_E", 1000, 0., 10., 1000, 0., 1.57);
  TProfile muonAngle_profile_E("muonAngle_profile_E", "muonAngle_profile_E", 1000, 0., 10.);
  
  
  TH1F nClustersInSuperClusterDistr("nClustersInSuperClusterDistr", "nClustersInSuperClusterDistr", 50, 0., 50.);
  TProfile nClustersInSuperCluster_ETA("nClustersInSuperCluster_ETA", "nClustersInSuperCluster_ETA", ETA_BIN, ETA_MIN, ETA_MAX);
  TProfile nClustersInSuperCluster_PHI("nClustersInSuperCluster_PHI", "nClustersInSuperCluster_PHI", PHI_BIN, PHI_MIN, PHI_MAX);
  
  TH1F nXtalsInSuperClusterDistr("nXtalsInSuperClusterDistr", "nXtalsInSuperClusterDistr", 50, 0., 50.);
  TProfile nXtalsInSuperCluster_ETA("nXtalsInSuperCluster_ETA", "nXtalsInSuperCluster_ETA", ETA_BIN, ETA_MIN, ETA_MAX);
  TProfile nXtalsInSuperCluster_PHI("nXtalsInSuperCluster_PHI", "nXtalsInSuperCluster_PHI", PHI_BIN, PHI_MIN, PHI_MAX);
  
  TH1F nXtalsCrossedDistr("nXtalsCrossedDistr", "nXtalsCrossedDistr", 50, 0., 50.);
  TProfile nXtalsCrossed_ETA("nXtalsCrossed_ETA", "nXtalsCrossed_ETA", ETA_BIN, ETA_MIN, ETA_MAX);
  TProfile nXtalsCrossed_PHI("nXtalsCrossed_PHI", "nXtalsCrossed_PHI", PHI_BIN, PHI_MIN, PHI_MAX);
  
  
  
  TGraph BetheBloch_th;
  DrawBetheBloch (&BetheBloch_th);
  
  TH1D dEoverdX("dEoverdX", "dEoverdX", 1000, 0., 0.5);
  TH1D dEoverdX_E3x3("dEoverdX_E3x3", "dEoverdX_E3x3", 1000, 0., 0.5);
  TH1D dEoverdX_E5x5("dEoverdX_E5x5", "dEoverdX_E5x5", 1000, 0., 0.5);
  
  TH2F dE("dE", "dE", P_BIN, P_MIN, P_MAX, 100000, 0., 1000.);
  BinLogX(dE);
  TProfile dE_profile("dE_profile", "dE_profile", P_BIN, P_MIN, P_MAX);
  BinLogX(dE_profile) ;
  TH2F EoP("EoP", "EoP", P_BIN, P_MIN, P_MAX, 10000, 0., 10.);
  BinLogX(EoP);
  TProfile EoP_profile("EoP_profile", "EoP_profile", P_BIN, P_MIN, P_MAX);
  BinLogX(EoP_profile);
  TH2F EoP_over1 ("EoP_over1","EoP_over1", PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  TH1F EoP_BAD ("EoP_BAD","EoP_BAD", 10000, 0., 10.);
  
  
  TH2F BetheBloch("BetheBloch", "BetheBloch", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX(BetheBloch) ;
  TH2F BetheBloch_E3x3 ("BetheBloch_E3x3", "BetheBloch_E3x3", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX(BetheBloch_E3x3) ;
  TH2F BetheBloch_E5x5 ("BetheBloch_E5x5", "BetheBloch_E5x5", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX(BetheBloch_E5x5);
  std::vector<TH1D*> BetheBloch_pBin;
  
  TProfile BetheBloch_profile ("BetheBloch_profile", "BetheBloch_profile", P_BIN, P_MIN, P_MAX);
  BinLogX(BetheBloch_profile);
  TProfile BetheBloch_profile_E3x3 ("BetheBloch_profile_E3x3", "BetheBloch_profile_E3x3", P_BIN, P_MIN, P_MAX);
  BinLogX(BetheBloch_profile_E3x3);
  TProfile BetheBloch_profile_E5x5 ("BetheBloch_profile_E5x5", "BetheBloch_profile_E5x5", P_BIN, P_MIN, P_MAX);
  BinLogX(BetheBloch_profile_E5x5);
  
  TGraphAsymmErrors BetheBloch_graph;
  TGraphAsymmErrors BetheBloch_graph_E3x3;
  TGraphAsymmErrors BetheBloch_graph_E5x5;
  
  TGraphErrors BetheBloch_graph_residuals;
  TGraphErrors BetheBloch_graph_MPV;
  TGraphErrors BetheBloch_graph_GSigma;
  TGraphAsymmErrors BetheBloch_graph_mean;
  
  
  
  TH2F BetheBloch_curved("BetheBloch_curved", "BetheBloch_curved", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX(BetheBloch_curved);
  TH2F BetheBloch_E3x3_curved("BetheBloch_E3x3_curved", "BetheBloch_E3x3_curved", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX(BetheBloch_E3x3_curved);
  TH2F BetheBloch_E5x5_curved("BetheBloch_E5x5_curved", "BetheBloch_E5x5_curved", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX(BetheBloch_E3x3_curved);
  std::vector<TH1D*> BetheBloch_pBin_curved;
  
  TProfile BetheBloch_profile_curved("BetheBloch_profile_curved", "BetheBloch_profile_curved", P_BIN, P_MIN, P_MAX);
  BinLogX(BetheBloch_profile_curved);
  TProfile BetheBloch_profile_E3x3_curved("BetheBloch_profile_E3x3_curved", "BetheBloch_profile_E3x3_curved", P_BIN, P_MIN, P_MAX);
  BinLogX(BetheBloch_profile_E3x3_curved);
  TProfile BetheBloch_profile_E5x5_curved("BetheBloch_profile_E5x5_curved", "BetheBloch_profile_E5x5_curved", P_BIN, P_MIN, P_MAX);
  BinLogX(BetheBloch_profile_E5x5_curved);
  
  TGraphAsymmErrors BetheBloch_graph_curved;
  TGraphAsymmErrors BetheBloch_graph_E3x3_curved;
  TGraphAsymmErrors BetheBloch_graph_E5x5_curved;
  
  TGraphErrors BetheBloch_graph_residuals_curved;
  TGraphErrors BetheBloch_graph_MPV_curved;
  TGraphErrors BetheBloch_graph_GSigma_curved;
  TGraphAsymmErrors BetheBloch_graph_mean_curved;
  
  
  std::map<int,float> pMap;
  pMap.clear();
  std::map<int,float> pMapCurved;
  pMapCurved.clear();
  std::map<int,float> pMap_E3x3;
  pMap_E3x3.clear();
  std::map<int,float> pMapCurved_E3x3;
  pMapCurved_E3x3.clear();
  std::map<int,float> pMap_E5x5;
  pMap_E5x5.clear();
  std::map<int,float> pMapCurved_E5x5;
  pMapCurved_E5x5.clear();
  std::map<int, float> BetheBloch_mean;
  BetheBloch_mean.clear();
  std::map<int, float> BetheBloch_mean_curved;
  BetheBloch_mean_curved.clear();
  
  
  
  
  
  
  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    chain -> GetEntry (entry);
    // if (entry == 100000) break;
    
    
    // association MU-SC
    std::vector<ect::association> associations;
    ect::fillAssocVector(associations, treeVars);
    ect::selectOnDR(associations, treeVars, 0.1);
    
    
    
    //loop on associations vector
    for (unsigned int i = 0; i < associations.size (); ++i)
    {
      int MUindex = associations.at(i).first;
      int SCindex = associations.at(i).second;
      
      
      int muonLeg = treeVars.muonLeg[MUindex];
      float muonP = treeVars.muonP[MUindex];
      float muonPt = treeVars.muonPt[MUindex];
      float muonPhi = treeVars.muonPhi[MUindex];
      float muonEta = treeVars.muonEta[MUindex];
      float muond0 = treeVars.muond0[MUindex];
      float muondz = treeVars.muondz[MUindex];
      float muondR = sqrt( pow(muond0, 2.) + pow(muondz, 2.) );
      float muonNChi2 = treeVars.muonNChi2[MUindex];
      float muonNHits = treeVars.muonNHits[MUindex];
      float muonTkLengthInEcal = treeVars.muonTkLengthInEcalDetail[MUindex];
      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex];
      GlobalPoint muonTkInternalPointInEcalCurved(treeVars.muonTkInternalPointInEcalCurvedX[MUindex],
                                                  treeVars.muonTkInternalPointInEcalCurvedY[MUindex],
                                                  treeVars.muonTkInternalPointInEcalCurvedZ[MUindex]) ;
      GlobalPoint muonTkInternalPointInEcal(treeVars.muonTkInternalPointInEcalX[MUindex],
                                            treeVars.muonTkInternalPointInEcalY[MUindex],
                                            treeVars.muonTkInternalPointInEcalZ[MUindex]) ;
      GlobalPoint muonTkExternalPointInEcalCurved(treeVars.muonTkExternalPointInEcalCurvedX[MUindex],
                                                  treeVars.muonTkExternalPointInEcalCurvedY[MUindex],
                                                  treeVars.muonTkExternalPointInEcalCurvedZ[MUindex]) ;
      GlobalPoint muonTkExternalPointInEcal (treeVars.muonTkExternalPointInEcalX[MUindex],
                                             treeVars.muonTkExternalPointInEcalY[MUindex],
                                             treeVars.muonTkExternalPointInEcalZ[MUindex]) ;
      TVector3 muonDirection;
      
      
      float superClusterRawEnergy = treeVars.superClusterRawEnergy[SCindex] / 4. * 0.9;
      float superClusterEta = treeVars.superClusterEta[SCindex];
      float superClusterPhi = treeVars.superClusterPhi[SCindex];
      int nClustersInSuperCluster = treeVars.nClustersInSuperCluster[SCindex];
      int nXtalsInSuperCluster = treeVars.nXtalsInSuperCluster[SCindex];
      int xtalIndexInSuperCluster = treeVars.xtalIndexInSuperCluster[SCindex];
      TVector3 xtalAxis;
      setVectorOnECAL (xtalAxis, muonTkInternalPointInEcal.eta(), muonTkInternalPointInEcal.phi(), 129.);
      
      
      int pBin = -1;
      int pBinCurved = -1;
      
      
      if (muonLeg == 1)
      {
        muonP = treeVars.muonInnTkInnerHitP[MUindex];
        muonPt = treeVars.muonInnTkInnerHitPt[MUindex];
        muonPhi = treeVars.muonInnTkInnerHitPhi[MUindex];
        muonEta = treeVars.muonInnTkInnerHitEta[MUindex];
        
        TVector3 dummy(treeVars.muonInnTkInnerHitPx[MUindex],
                       treeVars.muonInnTkInnerHitPy[MUindex],
                       treeVars.muonInnTkInnerHitPz[MUindex]);
        muonDirection = dummy;
      }
      
      else if (muonLeg == -1)
      {
        muonP = treeVars.muonInnTkOuterHitP[MUindex];
        muonPt = treeVars.muonInnTkOuterHitPt[MUindex];
        muonPhi = treeVars.muonInnTkOuterHitPhi[MUindex];
        muonEta = treeVars.muonInnTkOuterHitEta[MUindex];
        
        TVector3 dummy(treeVars.muonInnTkOuterHitPx[MUindex],
                       treeVars.muonInnTkOuterHitPy[MUindex],
                       treeVars.muonInnTkOuterHitPz[MUindex]);
        muonDirection = dummy;
      }
      
      else continue;
      
      
      float muonEoverP = superClusterRawEnergy/muonP;
      float muonAngle = xtalAxis.Angle(muonDirection);
      if (muonAngle > PI ) muonAngle -= PI;
      if (muonAngle > PI/2.) muonAngle = PI - muonAngle; // [0->pi/2.]
      
      
      
      
      
      
      // Cut event
      
      //skip bad Xtals
      bool skip = false;
      for (unsigned int badIt = 0; badIt < badRegionIEtaMIN.size(); ++badIt)
      {
        double badRegionEtaMIN;
        double badRegionEtaMAX;
        double badRegionPhiMIN;
        double badRegionPhiMAX;
        
        if (badRegionIEtaMIN.at(badIt) >= 0) badRegionEtaMIN = (badRegionIEtaMIN.at(badIt) - 0.5) * 0.0175;
        else badRegionEtaMIN = (badRegionIEtaMIN.at(badIt) + 0.5) * 0.0175;
        badRegionPhiMIN = (badRegionIPhiMIN.at(badIt) - 10) * 0.0175;
        if(badRegionPhiMIN > PI) badRegionPhiMIN = - 2*PI + badRegionPhiMIN;        
        
        if (badRegionIEtaMAX.at(badIt) >= 0) badRegionEtaMAX = (badRegionIEtaMAX.at(badIt) - 0.5) * 0.0175;
        else badRegionEtaMAX = (badRegionIEtaMAX.at(badIt) + 0.5) * 0.0175;
        badRegionPhiMAX = (badRegionIPhiMAX.at(badIt) - 10) * 0.0175;
        if(badRegionPhiMAX > PI) badRegionPhiMAX = - 2*PI + badRegionPhiMAX;        
        
        if (muonTkExternalPointInEcal.eta() >= badRegionEtaMIN &&
            muonTkInternalPointInEcal.eta() <= badRegionEtaMAX &&
            muonTkInternalPointInEcal.phi() >= badRegionPhiMIN &&
            muonTkInternalPointInEcal.phi() <= badRegionPhiMAX) {skip = true; break;}
        
        if (muonTkExternalPointInEcal.eta() >= badRegionEtaMIN &&
            muonTkExternalPointInEcal.eta() <= badRegionEtaMAX &&
            muonTkExternalPointInEcal.phi() >= badRegionPhiMIN &&
            muonTkExternalPointInEcal.phi() <= badRegionPhiMAX) {skip = true; break;}
        
        if ((muonTkExternalPointInEcal.eta() <= badRegionEtaMIN &&    //int ed extcadono a cavallo della badregion
             muonTkInternalPointInEcal.eta() >= badRegionEtaMAX) ||
            (muonTkExternalPointInEcal.phi() <= badRegionPhiMIN &&
             muonTkInternalPointInEcal.phi() >= badRegionPhiMAX) ||
            (muonTkInternalPointInEcal.eta() <= badRegionEtaMIN &&
             muonTkExternalPointInEcal.eta() >= badRegionEtaMAX) ||
            (muonTkInternalPointInEcal.phi() <= badRegionPhiMIN &&
             muonTkExternalPointInEcal.phi() >= badRegionPhiMAX )) {skip = true; break;}
         
         if (superClusterEta                 >= badRegionEtaMIN &&
             superClusterEta                 <= badRegionEtaMAX &&
             superClusterPhi                 >= badRegionPhiMIN &&
             superClusterPhi                 <= badRegionPhiMAX) {skip = true; break;}
        }
      
      if (skip == true) 
      {
        EoP_BAD.Fill(muonEoverP);
        continue;
      }
      //end skip bad xtals
      
      
      // other cuts
      if ( (muonLegUpOK == true && muonLegDownOK == false && muonLeg != 1) ||
           (muonLegDownOK == true && muonLegUpOK == false && muonLeg != -1) ) continue;
      if ( (muonP < muonPMIN) || (muonP > muonPMAX) ) continue ;
      if (fabs(muond0) > muond0MAX) continue;
      if (fabs(muondz) > muondzMAX) continue;
      if (muondR > muondRMAX) continue;
      if (muonNChi2 > muonNChi2MAX) continue;
      if (muonNHits < muonNHitsMIN) continue;
      if ( (muonTkLengthInEcal < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcal > muonTkLengthInEcalMAX) ) continue;
      if ( (muonTkLengthInEcalCurved < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcalCurved > muonTkLengthInEcalMAX) ) continue;
      if (muonEoverP > muonEoverPMAX) continue;
      if (muonAngle > muonAngleMAX) continue;
      if ( (superClusterPhi < superClusterPhiMIN) ||
           (superClusterPhi > superClusterPhiMAX) ) continue;
      if ( (superClusterEta < superClusterEtaMIN) ||
           (superClusterEta > superClusterEtaMAX) ) continue;
      
      
      
      
      
      // Fill distribution
      muonOccupancyInt_ETAvsPHI.Fill(muonTkInternalPointInEcal.phi(), muonTkInternalPointInEcal.eta());
      muonOccupancyExt_ETAvsPHI.Fill(muonTkExternalPointInEcal.phi(), muonTkExternalPointInEcal.eta());
      superClusterOccupancy_ETAvsPHI.Fill(superClusterPhi, superClusterEta);
      
      
      muond0Distr.Fill(muond0);
      muondzDistr.Fill(muondz);
      muondRDistr.Fill(muondR);
      muondR_P.Fill(muonP, muondR);
      muondR_profile_P.Fill(muonP, muondR);
      muondR_E.Fill(superClusterRawEnergy, muondR);
      muondR_profile_E.Fill(superClusterRawEnergy, muondR);
      
      
      muonNChi2Distr.Fill(muonNChi2);
      muonNHitsDistr.Fill(muonNHits);
      
      
      muonAngleDistr.Fill(muonAngle);
      muonAngle_P.Fill(muonP, muonAngle);
      muonAngle_profile_P.Fill(muonP, muonAngle);
      muonAngle_E.Fill(superClusterRawEnergy, muonAngle);
      muonAngle_profile_E.Fill(superClusterRawEnergy, muonAngle);
      
      
      nClustersInSuperClusterDistr.Fill(nClustersInSuperCluster);
      nClustersInSuperCluster_ETA.Fill(superClusterEta, nClustersInSuperCluster);
      nClustersInSuperCluster_PHI.Fill(superClusterPhi, nClustersInSuperCluster);
      
      nXtalsInSuperClusterDistr.Fill(nXtalsInSuperCluster);
      nXtalsInSuperCluster_ETA.Fill(superClusterEta, nXtalsInSuperCluster);
      nXtalsInSuperCluster_PHI.Fill(superClusterPhi, nXtalsInSuperCluster);
      
      int nXtalsCrossed = 0;
      for (int XTLindex = xtalIndexInSuperCluster;
           XTLindex < xtalIndexInSuperCluster + nXtalsInSuperCluster; ++XTLindex)
      {
        EBDetId dummy = EBDetId::unhashIndex(treeVars.xtalHashedIndex[XTLindex]);
        xtalOccupancy_iETAvsiPHI.Fill(dummy.iphi(), dummy.ieta());
        
        if (treeVars.xtalTkLengthCurved[XTLindex] > 0.)
          ++nXtalsCrossed;
      }
      nXtalsCrossedDistr.Fill(nXtalsCrossed);
      nXtalsCrossed_ETA.Fill(superClusterEta, nXtalsCrossed);
      nXtalsCrossed_PHI.Fill(superClusterPhi, nXtalsCrossed);
      
      
      
      if (nClustersInSuperCluster == 1)
      {
        int Cindex = treeVars.clusterIndexInSuperCluster[SCindex];
        float clusterE3x3 = treeVars.clusterE3x3[Cindex] / 4. * 0.9;
        float clusterE5x5 = treeVars.clusterE5x5[Cindex] / 4. * 0.9;
        
        dEoverdX.Fill(superClusterRawEnergy / muonTkLengthInEcal);
        dEoverdX_E3x3.Fill(clusterE3x3 / muonTkLengthInEcal);
        dEoverdX_E5x5.Fill(clusterE5x5 / muonTkLengthInEcal);
      }
      
      
      
      dE.Fill(muonP, superClusterRawEnergy);
      dE_profile.Fill(muonP, superClusterRawEnergy);
      EoP.Fill(muonP, superClusterRawEnergy/muonP);
      EoP_profile.Fill(muonP, superClusterRawEnergy/muonP);
      if(muonEoverP > 1.) EoP_over1.Fill (superClusterPhi, superClusterEta);
      
      
      //if(muonTkLengthInEcal > 0.)
      {
        float dEdX = superClusterRawEnergy / muonTkLengthInEcal / 8.28 * 1000.;
        BetheBloch.Fill(muonP, dEdX);
        pBin = BetheBloch_profile.Fill(muonP, dEdX);
        pMap[pBin] += muonP;
        
        if (nClustersInSuperCluster == 1)
        {
          int Cindex = treeVars.clusterIndexInSuperCluster[SCindex];
          float clusterE3x3 = treeVars.clusterE3x3[Cindex] / 4. * 0.9;
          float clusterE5x5 = treeVars.clusterE5x5[Cindex] / 4. * 0.9;
          
          dEdX = clusterE3x3 / muonTkLengthInEcal / 8.28 * 1000. ;
          BetheBloch_E3x3.Fill(muonP, dEdX);
          pBin = BetheBloch_profile_E3x3.Fill(muonP, dEdX);
          pMap_E3x3[pBin] += muonP;
          
          dEdX = clusterE5x5 / muonTkLengthInEcal / 8.28 * 1000.;
          BetheBloch_E5x5.Fill(muonP, dEdX);
          pBin = BetheBloch_profile_E5x5.Fill(muonP, dEdX);
          pMap_E5x5[pBin] += muonP;
        }
      }
      
      //if(muonTkLengthInEcalCurved > 0.)
      {
        float dEdXCurved = superClusterRawEnergy / muonTkLengthInEcalCurved / 8.28 * 1000.;
        BetheBloch_curved.Fill(muonP, dEdXCurved);
        pBinCurved = BetheBloch_profile_curved.Fill(muonP, dEdXCurved);
        pMapCurved[pBinCurved] += muonP;
        
        if (nClustersInSuperCluster == 1)
        {
          int Cindex = treeVars.clusterIndexInSuperCluster[SCindex];
          float clusterE3x3 = treeVars.clusterE3x3[Cindex] / 4. * 0.9;
          float clusterE5x5 = treeVars.clusterE5x5[Cindex] / 4. * 0.9;
          
          dEdXCurved = clusterE3x3 / muonTkLengthInEcalCurved / 8.28 * 1000.;
          BetheBloch_E3x3_curved.Fill(muonP, dEdXCurved);
          pBinCurved = BetheBloch_profile_E3x3_curved.Fill(muonP, dEdXCurved);
          pMapCurved_E3x3[pBinCurved] += muonP;
          
          dEdXCurved = clusterE5x5 / muonTkLengthInEcalCurved / 8.28 * 1000.;
          BetheBloch_E5x5_curved.Fill(muonP, dEdXCurved);
          pBinCurved = BetheBloch_profile_E5x5_curved.Fill(muonP, dEdXCurved);
          pMapCurved_E5x5[pBinCurved] += muonP;
        }
      }
    }//loop on associations vector
    
  } // loop over entries
  
  
  


  
  outputRootFile.mkdir ("BetheBloch_pBin");
  outputRootFile.cd ("BetheBloch_pBin");
  
  std::cout << "*********** STRAIGHT ***********" << std::endl;
  for (int bin = 1; bin <= BetheBloch_profile.GetNbinsX(); ++bin)
  {
    if (pMap[bin] > 0)
      pMap[bin] /= BetheBloch_profile.GetBinEntries(bin);
    if (pMap_E3x3[bin] > 0)
      pMap_E3x3[bin] /= BetheBloch_profile_E3x3.GetBinEntries(bin);
    if (pMap_E5x5[bin] > 0)
      pMap_E5x5[bin] /= BetheBloch_profile_E5x5.GetBinEntries(bin);
    
    char histoName[50];
    sprintf(histoName, "BetheBloch_pBin_%d", bin);
    TH1D* histo = BetheBloch.ProjectionY(histoName, bin, bin);
    BetheBloch_pBin.push_back(histo);
    
    TF1* langaus = new TF1;
    //double startValues[4] = {0.1, histo -> GetMean(), histo -> GetEntries() / 10., 0.1};
    //LangausFit(&langaus, histo, startValues);
    histo -> Write ();
    
    if (pMap[bin] > 0)
    {
      BetheBloch_mean[bin] = langaus -> Mean (0., 1000.);
      BetheBloch_graph_MPV.SetPoint (bin-1, pMap[bin], langaus -> GetParameter(1));
      BetheBloch_graph_MPV.SetPointError (bin-1, 0., langaus -> GetParError(1));
      BetheBloch_graph_GSigma.SetPoint (bin-1, pMap[bin], langaus -> GetParameter(3));
      BetheBloch_graph_GSigma.SetPointError (bin-1, 0., langaus -> GetParError(3));
     }
    
    std::cout << "Bin " << std::fixed << std::setprecision(2) << std::setw(3) << bin 
              << ":   [" << std::setw(7) << BetheBloch_profile.GetBinLowEdge(bin)
              << "," << std::setw(7) << BetheBloch_profile.GetBinLowEdge(bin) + BetheBloch_profile.GetBinWidth(bin)
              << "]         BinCenter: " << std::setw(7) << BetheBloch_profile.GetBinCenter(bin)
              << "   BinAvgCenter: " << std::setw(7) << pMap[bin]
              << "         BinEntries: " << std::setprecision(0) << std::setw(5) << BetheBloch_profile.GetBinEntries(bin)
              << "         BinContent: " << std::setprecision(3) << std::setw(5) << BetheBloch_profile.GetBinContent(bin)
              << std::endl;
  }
  
  
  
  outputRootFile.mkdir("BetheBlochCurved_pBin");
  outputRootFile.cd("BetheBlochCurved_pBin");
  
  std::cout << "*********** CURVED ***********" << std::endl;
  for (int bin = 1; bin <= BetheBloch_profile_curved.GetNbinsX(); ++bin)
  {
    if (pMapCurved[bin] > 0)
      pMapCurved[bin] /= BetheBloch_profile_curved.GetBinEntries(bin);
    if (pMapCurved_E3x3[bin] > 0)
      pMapCurved_E3x3[bin] /= BetheBloch_profile_E3x3_curved.GetBinEntries(bin);
    if (pMapCurved_E5x5[bin] > 0)
      pMapCurved_E5x5[bin] /= BetheBloch_profile_E5x5_curved.GetBinEntries(bin);
    
    
    
    char histoName[50];
    sprintf(histoName, "BetheBloch_pBin_curved_%d", bin);
    TH1D* histo = BetheBloch_curved.ProjectionY(histoName, bin, bin);
    BetheBloch_pBin_curved.push_back(histo);
    
    TF1* langaus = new TF1;
    //double startValues[4] = {0.1, histo -> GetMean(), histo -> GetEntries() / 10., 0.1};
    //LangausFit(&langaus, histo, startValues);
    histo -> Write();
    
    if (pMapCurved[bin] > 0)
    {
      BetheBloch_mean_curved[bin] = langaus -> Mean(0., 1000.);
      BetheBloch_graph_MPV_curved.SetPoint(bin-1, pMapCurved[bin], langaus -> GetParameter (1));
      BetheBloch_graph_MPV_curved.SetPointError(bin-1, 0., langaus -> GetParError (1));
      BetheBloch_graph_GSigma_curved.SetPoint(bin-1, pMapCurved[bin], langaus -> GetParameter (3));
      BetheBloch_graph_GSigma_curved.SetPointError(bin-1, 0., langaus -> GetParError (3));
    }
    
    
    std::cout << "Bin " << std::fixed << std::setprecision(2) << std::setw(3) << bin 
              << ":   [" << std::setw(7) << BetheBloch_profile_curved.GetBinLowEdge(bin)
              << "," << std::setw(7) << BetheBloch_profile_curved.GetBinLowEdge(bin) + BetheBloch_profile_curved.GetBinWidth(bin)
              << "]         BinCenter: " << std::setw(7) << BetheBloch_profile_curved.GetBinCenter(bin)
              << "   BinAvgCenter: " << std::setw(7) << pMapCurved[bin]
              << "         BinEntries: " << std::setprecision(0) << std::setw(5) << BetheBloch_profile_curved.GetBinEntries(bin)
              << "         BinContent: " << std::setprecision(3) << std::setw(5) << BetheBloch_profile_curved.GetBinContent(bin)
              << std::endl;
  }
  
  outputRootFile.cd ();
  
  
  
  
  double errX_low = -1.;
  double errX_hig = -1.;
  double errY = -1.;
  
  for(int bin = 1; bin <= BetheBloch_profile.GetNbinsX(); ++bin)
  {
    errX_low = pMap[bin] - BetheBloch_profile.GetBinLowEdge(bin);
    errX_hig = BetheBloch_profile.GetBinLowEdge(bin) + BetheBloch_profile.GetBinWidth(bin) - pMap[bin];
    errY = BetheBloch_profile.GetBinError(bin);
    
    
    BetheBloch_graph.SetPoint(bin-1, pMap[bin], BetheBloch_profile.GetBinContent(bin));
    BetheBloch_graph.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E3x3.SetPoint(bin-1, pMap_E3x3[bin], BetheBloch_profile_E3x3.GetBinContent(bin));
    BetheBloch_graph_E3x3.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E5x5.SetPoint(bin-1, pMap_E5x5[bin], BetheBloch_profile_E5x5.GetBinContent (bin));
    BetheBloch_graph_E5x5.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    
    
    BetheBloch_graph_mean.SetPoint(bin-1, pMap[bin], BetheBloch_mean[bin]);
    BetheBloch_graph_mean.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
  }
  
  DrawBetheBlochResiduals(BetheBloch_graph, BetheBloch_th, BetheBloch_graph_residuals) ;
  
  
  
  for(int bin = 1; bin <= BetheBloch_profile_curved.GetNbinsX(); ++bin)
  {
    errX_low = pMapCurved[bin] - BetheBloch_profile_curved.GetBinLowEdge(bin);
    errX_hig = BetheBloch_profile_curved.GetBinLowEdge(bin) + BetheBloch_profile_curved.GetBinWidth(bin) - pMapCurved[bin];
    errY = BetheBloch_profile_curved.GetBinError(bin);
    
    BetheBloch_graph_curved.SetPoint(bin-1, pMapCurved[bin], BetheBloch_profile_curved.GetBinContent(bin));
    BetheBloch_graph_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E3x3_curved.SetPoint(bin-1, pMapCurved_E3x3[bin], BetheBloch_profile_E3x3_curved.GetBinContent(bin));
    BetheBloch_graph_E3x3_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E5x5_curved.SetPoint(bin-1, pMapCurved_E5x5[bin], BetheBloch_profile_E5x5_curved.GetBinContent(bin));
    BetheBloch_graph_E5x5_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    
    
    BetheBloch_graph_mean_curved.SetPoint(bin-1, pMapCurved[bin], BetheBloch_mean_curved[bin]);
    BetheBloch_graph_mean_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
  }
  
  DrawBetheBlochResiduals(BetheBloch_graph, BetheBloch_th, BetheBloch_graph_residuals_curved);
  
  
  
  
  
  
  // Save histograms
  muonOccupancyInt_ETAvsPHI.Write();
  muonOccupancyExt_ETAvsPHI.Write();
  superClusterOccupancy_ETAvsPHI.Write();
  xtalOccupancy_iETAvsiPHI.Write();
  
  
  muond0Distr.Write();
  muondzDistr.Write();
  muondRDistr.Write();
  muondR_P.Write();
  muondR_profile_P.Write();
  muondR_E.Write();
  muondR_profile_E.Write();
  
  
  muonNChi2Distr.Write();
  muonNHitsDistr.Write();
  
  
  muonAngleDistr.Write();
  muonAngle_P.Write();
  muonAngle_profile_P.Write();
  muonAngle_E.Write();
  muonAngle_profile_E.Write();
  
  
  nClustersInSuperClusterDistr.Write();
  nClustersInSuperCluster_ETA.Write();
  nClustersInSuperCluster_PHI.Write();
  
  nXtalsInSuperClusterDistr.Write();
  nXtalsInSuperCluster_ETA.Write();
  nXtalsInSuperCluster_PHI.Write();
  
  nXtalsCrossedDistr.Write();
  nXtalsCrossed_ETA.Write();
  nXtalsCrossed_PHI.Write();
  
  
  
  dE.Write();
  dE_profile.Write();
  EoP.Write();
  EoP_profile.Write();
  EoP_over1.Write();
  EoP_BAD.Write();
  
  
  double startValues1[4] = {0.000903, 0.01249, 1.708, 0.00187};
  TF1* langaus = new TF1;
  LangausFit(&langaus, &dEoverdX, startValues1);
  dEoverdX.Write();
  delete langaus ;
  
  double startValues2[4] = {0.000903, 0.01249, 1.708, 0.00187};
  langaus = new TF1;
  LangausFit(&langaus, &dEoverdX_E3x3, startValues2);
  dEoverdX_E3x3.Write();
  delete langaus;
  
  double startValues3[4] = {0.000903, 0.01249, 1.708, 0.00187};
  langaus = new TF1;
  LangausFit(&langaus, &dEoverdX_E5x5, startValues3);
  dEoverdX_E5x5.Write();
  delete langaus;
  
  BetheBloch.Write();
  BetheBloch_E3x3.Write();
  BetheBloch_E5x5.Write();
  
  BetheBloch_profile.Write();
  BetheBloch_profile_E3x3.Write();
  BetheBloch_profile_E5x5.Write();
  
  BetheBloch_graph.Write("BetheBloch_graph");
  BetheBloch_graph_E3x3.Write("BetheBloch_graph_E3x3");
  BetheBloch_graph_E5x5.Write("BetheBloch_graph_E5x5");
  
  BetheBloch_graph_residuals.Write("BetheBloch_graph_residuals");
  BetheBloch_graph_MPV.Write("BetheBloch_graph_MPV");
  BetheBloch_graph_GSigma.Write("BetheBloch_graph_GSigma");
  BetheBloch_graph_mean.Write("BetheBloch_graph_mean");
  
  
  
  BetheBloch_curved.Write();
  BetheBloch_E3x3_curved.Write();
  BetheBloch_E5x5_curved.Write();
  
  BetheBloch_profile_curved.Write();
  BetheBloch_profile_E3x3_curved.Write();
  BetheBloch_profile_E5x5_curved.Write();
  
  BetheBloch_graph_curved.Write("BetheBloch_graph_curved");
  BetheBloch_graph_E3x3_curved.Write("BetheBloch_graph_E3x3_curved");
  BetheBloch_graph_E5x5_curved.Write("BetheBloch_graph_E5x5_curved");
  
  BetheBloch_graph_residuals_curved.Write("BetheBloch_graph_residuals_curved");
  BetheBloch_graph_MPV_curved.Write("BetheBloch_graph_MPV_curved");
  BetheBloch_graph_GSigma_curved.Write("BetheBloch_graph_GSigma_curved");
  BetheBloch_graph_mean.Write("BetheBloch_graph_mean_curved");
  
  
  
  outputRootFile.Close();
  outFile.close();
  
  return 0;
}





template <class THisto>
void BinLogX(THisto& h)
{
  TAxis* axis = h.GetXaxis () ;
  int bins = axis -> GetNbins () ;
  
  Axis_t from = axis -> GetXmin () ;
  Axis_t to = axis -> GetXmax () ;
  Axis_t width = (to - from) / bins ;
  Axis_t* new_bins = new Axis_t[bins + 1] ;
  
  for (int i = 0; i <= bins; i++)
    new_bins[i] = pow(10, from + i * width) ;
  
  axis -> Set (bins, new_bins) ;
  delete new_bins ;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}



void LangausFit (TF1** langaus, TH1D* histo, double* startValues)
{
  
  
  // Langaus Fit
  double fitRange[2] ;
  fitRange[0] = 0.; 
  fitRange[1] = 1000. ;
  
  double parLimitsLow[4] ;
  parLimitsLow[0] = 0.00001 ;
  parLimitsLow[1] = 0.00001 ;
  parLimitsLow[2] = 0.00001 ;
  parLimitsLow[3] = 0.00001 ;
  
  double parLimitsHigh[4] ;
  parLimitsHigh[0] = 10000000. ;
  parLimitsHigh[1] = 10. ;
  parLimitsHigh[2] = 10000000. ;
  parLimitsHigh[3] = 1. ;
  
  double parameters[4], parErrors[4] ;
  double chi2 ;
  int ndf ;
  
  *langaus = langaufit ((TH1F*)histo, fitRange, startValues, parLimitsLow, parLimitsHigh, parameters, parErrors, &chi2, &ndf) ;
}

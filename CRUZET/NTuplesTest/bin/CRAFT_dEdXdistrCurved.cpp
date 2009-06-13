#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "CRUZET/NTuplesTest/test/Langaus.cc"
#include "CRUZET/Calibration/interface/CRUtils.h"
#include "CRUZET/Calibration/interface/ClusterCalibTools.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

#include "CLHEP/Units/PhysicalConstants.h"

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

#define ADGtoGeV 0.00814750045537949

#define PHI_MIN -3.1416
#define PHI_MAX +3.1416
#define PHI_BIN 360

#define ETA_MIN -1.5
#define ETA_MAX +1.5
#define ETA_BIN 170

#define R_MIN 0.
#define R_MAX 1000.
#define R_BIN 10000

#define Z_MIN -1000.
#define Z_MAX  1000.
#define Z_BIN 2000

#define IPHI_MIN 1.
#define IPHI_MAX +361.
#define IPHI_BIN 360 //360

#define IETA_MIN -85.
#define IETA_MAX +86.
#define IETA_BIN 171 //171

#define P_MIN 0
#define P_MAX 3

typedef std::map<int, TH1D*>::iterator map_iterator_int;
typedef std::map<int, TH1D*>::const_iterator map_const_iterator_int;

typedef std::map<float, TH1D*>::iterator map_iterator_float;
typedef std::map<float, TH1D*>::const_iterator map_const_iterator_float;






int main (int argc, char** argv)
{
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet();
  
  edm::ParameterSet subPSetSelections =  parameterSet -> getParameter<edm::ParameterSet>("selections");
  
  int P_BIN = subPSetSelections.getUntrackedParameter<int>("P_BIN", 25);
  int maxEvent = subPSetSelections.getUntrackedParameter<int>("maxEvent", -1);
  bool useTrueVal = subPSetSelections.getUntrackedParameter<bool>("useTrueVal", false);
  bool correctMuonP = subPSetSelections.getUntrackedParameter<bool>("correctMuonP", true);
  bool applyVeto = subPSetSelections.getUntrackedParameter<bool>("applyVeto", false);
  
  bool muonLegUpOK = subPSetSelections.getUntrackedParameter<bool>("muonLegUpOK", false);
  bool muonLegDownOK = subPSetSelections.getUntrackedParameter<bool>("muonLegDownOK", true);

  double muonPMAX = subPSetSelections.getUntrackedParameter<double>("muonPMAX", 1000.);
  double muonPMIN = subPSetSelections.getUntrackedParameter<double>("muonPMIN", 1.);
  double muonPErrorOverPMAX = subPSetSelections.getUntrackedParameter<double>("muonPErrorOverPMAX", 1000000000000.);
  double muonPCollTHRESH = subPSetSelections.getUntrackedParameter<double>("muonPCollTHRESH", 50.);

  double muond0MAX = subPSetSelections.getUntrackedParameter<double>("muond0MAX", 1000000000000.);
  double muondzMAX = subPSetSelections.getUntrackedParameter<double>("muondzMAX", 1000000000000.);
  double muondRMAX = subPSetSelections.getUntrackedParameter<double>("muondRMAX", 1000000000000.);

  double muonNChi2MAX = subPSetSelections.getUntrackedParameter<double>("muonNChi2MAX", 1000000000000.);
  double muonNHitsMIN = subPSetSelections.getUntrackedParameter<double>("muonNHitsMIN", 1.);

  double muonTkLengthInEcalMIN = subPSetSelections.getUntrackedParameter<double>("muonTkLengthInEcalMIN", 1.);
  double muonTkLengthInEcalMAX = subPSetSelections.getUntrackedParameter<double>("muonTkLengthInEcalMAX", 1000000000000.);

  double muonEOverPMAX = subPSetSelections.getUntrackedParameter<double>("muonEOverPMAX", 1000000000000.);
  double muonAngleMAX = subPSetSelections.getUntrackedParameter<double>("muonAngleMAX", 1000000000000.);
  double muonAngleMIN = subPSetSelections.getUntrackedParameter<double>("muonAngleMIN", 1000000000000.);

  double muonInnTkHitZMIN = subPSetSelections.getUntrackedParameter<double>("muonInnTkHitZMIN", 1000000000000.);
  double muonInnTkHitZMAX = subPSetSelections.getUntrackedParameter<double>("muonInnTkHitZMAX", 1000000000000.);
  
  double energyCorrFactor = subPSetSelections.getUntrackedParameter<double>("energyCorrFactor", 0.97);
  double superClusterPhiMIN = subPSetSelections.getUntrackedParameter<double>("superClusterPhiMIN", -3.15);
  double superClusterPhiMAX = subPSetSelections.getUntrackedParameter<double>("superClusterPhiMAX", 3.15);
  double superClusterEtaMIN = subPSetSelections.getUntrackedParameter<double>("superClusterEtaMIN", -1000000000000.);
  double superClusterEtaMAX = subPSetSelections.getUntrackedParameter<double>("superClusterEtaMAX", 1000000000000.);

  double xtalTkLengthSumMIN = subPSetSelections.getUntrackedParameter<double>("xtalTkLengthSumMIN", -1.);
  double xtalTkLengthSumMAX = subPSetSelections.getUntrackedParameter<double>("xtalTkLengthSumMAX", -1.);
  double nXtalsInSuperClusterMIN = subPSetSelections.getUntrackedParameter<double>("nXtalsInSuperClusterMIN", -1.);  
  double nXtalsInSuperClusterMAX = subPSetSelections.getUntrackedParameter<double>("nXtalsInSuperClusterMAX", -1.);  

  std::vector<int> badRegionIPhiMIN = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIPhiMIN", std::vector<int>(0));
  std::vector<int> badRegionIPhiMAX = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIPhiMAX", std::vector<int>(0));
  std::vector<int> badRegionIEtaMIN = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIEtaMIN", std::vector<int>(0));
  std::vector<int> badRegionIEtaMAX = subPSetSelections.getUntrackedParameter<std::vector<int> >("badRegionIEtaMAX", std::vector<int>(0));

  std::vector<int> badTowerFED = subPSetSelections.getUntrackedParameter<std::vector<int> >("badTowerFED", std::vector<int>(0));
  std::vector<int> badTowerId  = subPSetSelections.getUntrackedParameter<std::vector<int> >("badTowerId", std::vector<int>(0));
  
  edm::ParameterSet subPSetInput = parameterSet -> getParameter<edm::ParameterSet>("inputNtuples");
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles");


  edm::ParameterSet subPSetCalibration = parameterSet -> getParameter<edm::ParameterSet>("calibSelections");

  int etaSTART = subPSetCalibration.getParameter<int> ("etaSTART") ;
  int etaEND = subPSetCalibration.getParameter<int> ("etaEND") ;
  int etaWIDTH = subPSetCalibration.getParameter<int> ("etaWIDTH") ;
  int etaN = (etaEND - etaSTART) / etaWIDTH ;
  int phiSTART = subPSetCalibration.getParameter<int> ("phiSTART") ;
  int phiEND = subPSetCalibration.getParameter<int> ("phiEND") ;
  int phiWIDTH = subPSetCalibration.getParameter<int> ("phiWIDTH") ;
  int phi = phiSTART ; if (phiSTART > phiEND) phi = phiSTART - 360 ;
  int phiN = (phiEND - phi) / phiWIDTH ;
  int stat = subPSetCalibration.getParameter<int> ("stat") ;

  
  
  
  
  
  
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
  std::string outRootFileName = "CRAFT_dEdXdistrCurved.root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  // Output file
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  
  
  TH1F nEvents_TOT("nEvents_TOT", "nEvents_TOT", P_BIN, P_MIN, P_MAX);
  BinLogX(nEvents_TOT);
  TH1F nEvents_CUT("nEvents_CUT", "nEvents_CUT", P_BIN, P_MIN, P_MAX);
  BinLogX(nEvents_CUT);
  TH1F nEvents_EFF("nEvents_EFF", "nEvents_EFF", P_BIN, P_MIN, P_MAX);
  BinLogX(nEvents_EFF);
  
  
  // dEodX histograms
  TH1D dEdXDistrCurved("dEdXDistrCurved", "dEodXCurved", 20000, 0., 1000.);
  //TH1D dEdXDistrCurved_E3x3("dEdXDistrCurved_E3x3", "dEodXCurved_E3x3", 20000, 0., 1000.);
  //TH1D dEdXDistrCurved_E5x5("dEdXDistrCurved_E5x5", "dEodXCurved_E5x5", 20000, 0., 1000);
  // dEodX histograms
  
  
  
  // Theoretical BetheBloch
  TGraph BetheBloch_th;
  DrawBetheBloch (&BetheBloch_th);
  // Theoretical BetheBloch
  
  
  
  
  // Experimental BetheBloch - curved tkLength
  TH2F BetheBloch_curved("BetheBloch_curved", "BetheBloch_curved", P_BIN, P_MIN, P_MAX, 20000, 0., 1000.);
  BinLogX(BetheBloch_curved);
  TProfile BetheBloch_profile_curved ("BetheBloch_profile_curved", "BetheBloch_profile_curved", P_BIN, P_MIN, P_MAX);
  BinLogX(BetheBloch_profile_curved);
  TProfile BetheBloch_profile_vsPHI_curved("BetheBloch_profile_vsPHI_curved", "BetheBloch_profile_vsPHI_curved", IPHI_BIN, IPHI_MIN, IPHI_MAX);
  TProfile BetheBloch_profile_vsETA_curved("BetheBloch_profile_vsETA_curved", "BetheBloch_profile_vsETA_curved", IETA_BIN, IETA_MIN, IETA_MAX);
  TGraphAsymmErrors BetheBloch_graph_curved;
  TGraphErrors BetheBloch_graph_residuals_curved;  
  std::map<int,float> pMapCurved; pMapCurved.clear();
  std::map<int, float> BetheBloch_mean_curved; BetheBloch_mean_curved.clear();

  TGraphErrors BetheBloch_graph_MPV_curved;
  TGraphAsymmErrors BetheBloch_graph_mean_curved;
  TGraphErrors BetheBloch_graph_GSigma_curved;

  //TH2F BetheBloch_E3x3_curved ("BetheBloch_E3x3_curved", "BetheBloch_E3x3_curved", P_BIN, P_MIN, P_MAX, 20000, 0., 1000.);
  //BinLogX(BetheBloch_E3x3_curved);
  //TProfile BetheBloch_profile_E3x3_curved("BetheBloch_profile_E3x3_curved", "BetheBloch_profile_E3x3_curved", P_BIN, P_MIN, P_MAX);
  //BinLogX(BetheBloch_profile_E3x3_curved);
  //TGraphAsymmErrors BetheBloch_graph_E3x3_curved;
  //std::map<int,float> pMapCurved_E3x3; pMapCurved_E3x3.clear();
  
  //TH2F BetheBloch_E5x5_curved ("BetheBloch_E5x5_curved", "BetheBloch_E5x5_curved", P_BIN, P_MIN, P_MAX, 20000, 0., 1000.);
  //BinLogX(BetheBloch_E5x5_curved);
  //TProfile BetheBloch_profile_E5x5_curved("BetheBloch_profile_E5x5_curved", "BetheBloch_profile_E5x5_curved", P_BIN, P_MIN, P_MAX);
  //BinLogX(BetheBloch_profile_E5x5_curved);
  //TGraphAsymmErrors BetheBloch_graph_E5x5_curved;
  //std::map<int,float> pMapCurved_E5x5; pMapCurved_E5x5.clear();
  
  // Experimental BetheBloch - curved tkLength
  
  
  
  
  // Muon histograms
  TProfile nMuons_overTh("nMuons_overTh", "nMuons_overTh", 100, -3, 2); 
  BinLogX(nMuons_overTh);

  TH2F muonOccupancyInt_ETAvsPHI("muonOccupancyInt_ETAvsPHI", "muonOccupancyInt_ETAvsPHI",
                                 PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  TH2F muonOccupancyExt_ETAvsPHI("muonOccupancyExt_ETAvsPHI", "muonOccupancyExt_ETAvsPHI",
                                 PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  
  TH1F muonPDistr("muonPDistr", "muonPDistr", 10000, 1., 10000.);
  //BinLogX(muonPDistr);
  TH1F muonOutTkPoPDistr("muonOutTkPoPDistr", "muonOutTkPoPDistr", 1000, 0., 10.);
  TH2F muonOutTkPoP_vsP("muonOutTkPoP_vsP", "muonOutTkPoP_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 10.);
  BinLogX(muonOutTkPoP_vsP);
  std::vector<TH1F*> muonPDistr_pBin;
  for(int bin = 1; bin <= P_BIN; ++bin)
  {
    char histoName[50];
    sprintf(histoName, "muonPDistr_pBin_%d", bin);
    double errX_low = BetheBloch_profile_curved.GetBinLowEdge(bin);
    double errX_high = BetheBloch_profile_curved.GetBinLowEdge(bin) + BetheBloch_profile_curved.GetBinWidth(bin);
    muonPDistr_pBin.push_back(new TH1F(histoName, histoName, 1000, errX_low, errX_high));
  }
  
  TH1F muonQOverPDistr("muonQOverPDistr", "muonQOverPDistr", 10000, -1., 1.);
  TH1F muonQOverPErrorDistr("muonQOverPErrorDistr", "muonQOverPErrorDistr", 10000, 0., 1.);

  TH1F muonPErrorDistr("muonPErrorDistr", "muonPErrorDistr", 10000, 0., 1000.);
  TH2F muonPError_vsP("muonPError_vsP", "muonPError_vsP", P_BIN, P_MIN, P_MAX, 10000, 0., 1000.);
  BinLogX(muonPError_vsP);
  TProfile muonPError_profile_vsP("muonPError_profile_vsP", "muonPError_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(muonPError_profile_vsP);

  TH1F muonPErrorOverPDistr("muonPErrorOverPDistr", "muonPErrorOverPDistr", 1000, 0., 1.);
  TH2F muonPErrorOverP_vsP("muonPErrorOverP_vsP", "muonPErrorOverP_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 1.);
  BinLogX(muonPErrorOverP_vsP);
  TProfile muonPErrorOverP_profile_vsP("muonPErrorOverP_profile_vsP", "muonPErrorOverP_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(muonPErrorOverP_profile_vsP);
  
  TH1F muonChargeDistr("muonChargeDistr", "muonChargeDistr", 4, -2., 2.);
  TH1F muonNChi2Distr("muonNChi2Distr", "muonNChi2Distr", 100000, 0., 10000.);
  TH1F muonNHitsDistr("muonNHitsDistr", "muonNHitsDistr", 100, 0., 100.);
  
  TH1F muond0Distr("muond0Distr", "muond0Distr", 1000, -200., 200.);
  TH1F muondzDistr("muondzDistr", "muondzDistr", 1000, -200., 200.);
  
  TH1F muondRDistr("muondRDistr", "muondRDistr", 1000, 0., 400.);
  TH2F muondR_vsP("muondR_vsP", "muondR_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 400.);
  BinLogX(muondR_vsP);
  TProfile muondR_profile_vsP("muondR_profile_vsP", "muondR_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(muondR_profile_vsP);
  TH2F muondR_vsAngle("muondR_vsAngle", "muondR_vsAngle", 1000, 0., PI/2., 1000, 0., 400.);
  TProfile muondR_profile_vsAngle("muondR_profile_vsAngle", "muondR_profile_vsAngle", 1000, 0., PI/2.);

  TH1F muonTkInternalPointInEcalCurvedPhiDistr("muonTkInternalPointInEcalCurvedPhiDistr", "muonTkInternalPointInEcalCurvedPhiDistr", PHI_BIN, PHI_MIN, PHI_MAX);
  TH1F muonTkInternalPointInEcalCurvedEtaDistr("muonTkInternalPointInEcalCurvedEtaDistr", "muonTkInternalPointInEcalCurvedEtaDistr", ETA_BIN, ETA_MIN, ETA_MAX);
  TH1F muonTkInternalPointInEcalCurvedRDistr("muonTkInternalPointInEcalCurvedRDistr", "muonTkInternalPointInEcalCurvedRDistr", R_BIN, R_MIN, R_MAX);
  TH1F muonTkInternalPointInEcalCurvedZDistr("muonTkInternalPointInEcalCurvedZDistr", "muonTkInternalPointInEcalCurvedZDistr", Z_BIN, Z_MIN, Z_MAX);
  TProfile muonTkInternalPointInEcalCurved_RvsZ("muonTkInternalPointInEcalCurved_RvsZ", "muonTkInternalPointInEcalCurved_RvsZ", Z_BIN, Z_MIN, Z_MAX);
  TH1F muonTkExternalPointInEcalCurvedPhiDistr("muonTkExternalPointInEcalCurvedPhiDistr", "muonTkExternalPointInEcalCurvedPhiDistr", PHI_BIN, PHI_MIN, PHI_MAX);
  TH1F muonTkExternalPointInEcalCurvedEtaDistr("muonTkExternalPointInEcalCurvedEtaDistr", "muonTkExternalPointInEcalCurvedEtaDistr", ETA_BIN, ETA_MIN, ETA_MAX);
  TH1F muonTkExternalPointInEcalCurvedRDistr("muonTkExternalPointInEcalCurvedRDistr", "muonTkExternalPointInEcalCurvedRDistr", R_BIN, R_MIN, R_MAX);
  TH1F muonTkExternalPointInEcalCurvedZDistr("muonTkExternalPointInEcalCurvedZDistr", "muonTkExternalPointInEcalCurvedZDistr", Z_BIN, Z_MIN, Z_MAX);
  TProfile muonTkExternalPointInEcalCurved_RvsZ("muonTkExternalPointInEcalCurved_RvsZ", "muonTkExternalPointInEcalCurved_RvsZ", Z_BIN, Z_MIN, Z_MAX);  

  TH1F muonSkyAngleDistr("muonSkyAngleDistr", "muonSkyAngleDistr", 1000, 0., PI);
  TH1F muonAngleDistr("muonAngleDistr", "muonAngleDistr", 1000, 0., PI/2.);
  TH2F muonAngle_vsP("muonAngle_vsP", "muonAngle_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., PI/2.);
  BinLogX(muonAngle_vsP);
  TProfile muonAngle_profile_vsP("muonAngle_profile_vsP", "muonAngle_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(muonAngle_profile_vsP);
  TH2F muonAngle_vsDR("muonAngle_vsDR", "muonAngle_vsDR", 1000, 0., 400., 1000, 0., PI/2.);
  TProfile muonAngle_profile_vsDR("muonAngle_profile_vsDR", "muonAngle_profile_vsDR", 1000, 0., 400.);
  TProfile muonAngle_profile_vsETA("muonAngle_profile_vsETA", "muonAngle_profile_vsETA", ETA_BIN, ETA_MIN, ETA_MAX);
  TProfile muonAngle_profile_vsPHI("muonAngle_profile_vsPHI", "muonAngle_profile_vsPHI", PHI_BIN, PHI_MIN, PHI_MAX);
  
  TH1F muonInnTkDAngleDistr("muonInnTkDAngleDistr", "muonInnTkDAngleDistr", 1000, 0., PI);
  TH2F muonInnTkDAngle_vsPT("muonInnTkDAngle_vsPT", "muonInnTkDAngle_vsPT", P_BIN, P_MIN, P_MAX, 1000, 0., PI/2.);
  BinLogX(muonInnTkDAngle_vsPT);
  TProfile muonInnTkDAngle_profile_vsPT("muonInnTkDAngle_profile_vsPT", "muonInnTkDAngle_profile_vsPT", P_BIN, P_MIN, P_MAX);
  BinLogX(muonInnTkDAngle_profile_vsPT);
  
  TH1F muonTkLengthCurvedDistr("muonTkLengthCurvedDistr", "muonTkLengthCurvedDistr", 1000, 0., 100.);
  TH2F muonTkLengthCurved_vsDR("muonTkLengthCurved_vsDR", "muonTkLengthCurved_vsDR", 1000, 0., 400., 1000, 0., 100.);
  TProfile muonTkLengthCurved_profile_vsDR("muonTkLengthCurved_profile_vsDR", "muonTkLengthCurved_profile_vsDR", 1000, 0., 400.);
  TH2F muonTkLengthCurved_vsAngle("muonTkLengthCurved_vsAngle", "muonTkLengthCurved_vsAngle", 1000, 0., PI/2, 1000, 0., 100.);
  TProfile muonTkLengthCurved_profile_vsAngle("muonTkLengthCurved_profile_vsAngle", "muonTkLengthCurved_profile_vsAngle", 1000, 0., PI/2.);

  TH1F nMuonCrossedXtalsCurvedDistr("nMuonCrossedXtalsCurvedDistr", "nMuonCrossedXtalsCurvedDistr", 100, 0., 100.);
  TProfile nMuonCrossedXtalsCurved_profile_vsAngle("nMuonCrossedXtalsCurved_profile_vsAngle", "nMuonCrossedXtalsCurved_profile_vsAngle", 1000, 0., PI/2.);
  TProfile nMuonCrossedXtalsCurved_profile_vsETA("nMuonCrossedXtalsCurved_profile_vsETA", "nMuonCrossedXtalsCurved_profile_vsETA", ETA_BIN, ETA_MIN, ETA_MAX);
  TProfile nMuonCrossedXtalsCurved_profile_vsPHI("nMuonCrossedXtalsCurved_profile_vsPHI", "nMuonCrossedXtalsCurved_profile_vsPHI", PHI_BIN, PHI_MIN, PHI_MAX);
  TProfile2D nMuonCrossedXtalsCurved_profile_ETAvsPHI("nMuonCrossedXtalsCurved_profile_ETAvsPHI", "nMuonCrossedXtalsCurved_profile_ETAvsPHI", PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  TH1F muonCrossedXtalTkLengthCurvedSumDistr("muonCrossedXtalTkLengthCurvedSumDistr", "muonCrossedXtalTkLengthCurvedSumDistr", 1000, 0., 10.);
  TProfile muonCrossedXtalTkLengthCurvedSumZS_profile_vsAngle("muonCrossedXtalTkLengthCurvedSumZS_profile_vsAngle", "muonCrossedXtalTkLengthCurvedSumZS_profile_vsAngle", 1000, 0., PI/2.);

  TH1F ZSLengthDistr("ZSLengthDistr", "ZSLengthDistr", 1000, 0., 100.);
  TProfile ZSLength_vsAngle("ZSLength_vsAngle", "ZSLength_vsAngle", 1000, 0., PI/2.);
  // Muon histograms
  
  
  
  
  // SuperCluster histograms
  TH2F superClusterOccupancy_ETAvsPHI("superClusterOccupancy_ETAvsPHI", "superClusterOccupancy_ETAvsPHI",
                                      PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  
  TH1F superClusterRawEnergyDistr("superClusterRawEnergyDistr", "superClusterRawEnergyDistr", 10000, 0.01, 100.);
  //BinLogX(superClusterRawEnergyDistr);
  TH2F superClusterRawEnergy_vsP("superClusterRawEnergy_vsP", "superClusterRawEnergy_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 100.);
  BinLogX(superClusterRawEnergy_vsP);
  TProfile superClusterRawEnergy_profile_vsP("superClusterRawEnergy_profile_vsP", "superClusterRawEnergy_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(superClusterRawEnergy_profile_vsP);
  TH2F superClusterRawEnergy_vsAngle("superClusterRawEnergy_vsAngle", "superClusterRawEnergy_vsAngle", 1000, 0., PI/2., 1000, 0., 100.);  
  TProfile superClusterRawEnergy_profile_vsAngle("superClusterRawEnergy_profile_vsAngle", "superClusterRawEnergy_profile_vsAngle", 1000, 0., PI/2.);  

  TH1F EoPDistr("EoPDistr", "EoPDistr", 10000, 0., 10.);
  TH2F EoP_vsP("EoP_vsP", "EoP_vsP", P_BIN, P_MIN, P_MAX, 10000, 0., 10.);
  BinLogX(EoP_vsP);
  
  TH1F nXtalsInSuperClusterDistr("nXtalsInSuperClusterDistr", "nXtalsInSuperClusterDistr", 100, 0., 100.);
  TH2F nXtalsInSuperCluster_vsAngle("nXtalsInSuperCluster_vsAngle", "nXtalsInSuperCluster_vsAngle", 1000, 0., PI/2., 100, 0., 100.);
  TProfile nXtalsInSuperCluster_profile_vsAngle("nXtalsInSuperCluster_profile_vsAngle", "nXtalsInSuperCluster_profile_vsAngle", 1000, 0., PI/2.);
  TProfile nXtalsInSuperCluster_profile_vsPHI("nXtalsInSuperCluster_profile_vsPHI", "nXtalsInSuperCluster_profile_vsPHI", PHI_BIN, PHI_MIN, PHI_MAX);
  TProfile nXtalsInSuperCluster_profile_vsETA("nXtalsInSuperCluster_profile_vsETA", "nXtalsInSuperCluster_profile_vsETA", ETA_BIN, ETA_MIN, ETA_MAX);
  TProfile2D nXtalsInSuperCluster_profile_ETAvsPHI("nXtalsInSuperCluster_profile_ETAvsPHI", "nXtalsInSuperCluster_profile_ETAvsPHI", PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);

  TH1F nXtalsInSuperClusterOverCrossedXtalsCurvedDistr("nXtalsInSuperClusterOverCrossedXtalsCurvedDistr", "nXtalsInSuperClusterOverCrossedXtalsCurvedDistr", 1000, 0., 5.);
  // SuperCluster histograms
  
  
  
  
  // Xtal histograms
  TH2F xtalOccupancy_iETAvsiPHI("xtalOccupancy_iETAvsiPHI", "xtalOccupancy_iETAvsiPHI",
                                361, 1., 361., 171, -85., 86.);
  
  
  TH1F xtalEnergyDistr("xtalEnergyDistr", "xtalEnergyDistr", 10000, 0., 10.);

  TH1F xtalTkLengthCurvedSumDistr("xtalTkLengthCurvedSumDistr", "xtalTkLengthCurvedSumDistr", 1000, -5., 5.);
  TH2F xtalTkLengthCurvedSum_FRACvsAngle("xtalTkLengthCurvedSum_FRACvsAngle", "xtalTkLengthCurvedSum_FRACvsAngle", 1000, 0., PI/2., 1000, -5., 5.);
  TProfile xtalTkLengthCurvedSum_vsAngle("xtalTkLengthCurvedSum_vsAngle", "xtalTkLengthCurvedSum_vsAngle", 1000, 0., PI/2.);
  TProfile xtalTkLengthCurvedSum_vsPHI("xtalTkLengthCurvedSum_vsPHI", "xtalTkLengthCurvedSum_vsPHI", PHI_BIN, PHI_MIN, PHI_MAX);
  TProfile xtalTkLengthCurvedSum_vsETA("xtalTkLengthCurvedSum_vsETA", "xtalTkLengthCurvedSum_vsETA", ETA_BIN, ETA_MIN, ETA_MAX);
  
  TH1F xtalTkLengthCurvedDistr("xtalTkLengthCurvedDistr", "xtalTkLengthCurvedDistr", 1000, 0., 100.);
  TH2F xtalTkLengthCurved_vsAngle("xtalTkLengthCurved_vsAngle", "xtalTkLengthCurved_vsAngle", 1000, 0., PI/2., 1000, 0., 100.);
  TProfile xtalTkLengthCurved_profile_vsAngle("xtalTkLengthCurved_profile_vsAngle", "xtalTkLength_profile_vsAngle", 1000, 0., PI/2.);

  TH1F xtalTkLengthCurvedMaxDistr("xtalTkLengthCurvedDistrMax", "xtalTkLengthCurvedDistrMax", 10000, 0., 100.);
  TH2F xtalTkLengthCurvedMax_vsAngle("xtalTkLengthCurvedMax_vsAngle", "xtalTkLengthCurvedMax_vsAngle", 1000, 0., PI/2., 1000, 0., 100.);
  TProfile xtalTkLengthCurvedMax_profile_vsAngle("xtalTkLengthCurvedMax_profileCurvedMax_vsAngle", "xtalTkLength_profile_vsAngle", 1000, 0., PI/2.);
  // Xtal histograms
  
  
  
  
  // Calibration  
  EBregionBuilder region(etaSTART, etaEND, etaWIDTH, phiSTART, phiEND, phiWIDTH);

  TH2F regionOccupancy_ETAvsPHI("regionOccupancy_ETAvsPHI", "",
                                phiN, phiSTART, phiEND, etaN, etaShifter(etaSTART), etaShifter(etaEND));

  std::map<int, TH1D*>   calib_dEdXMap_vsREGIONID;
  std::map<float, TH1D*> calib_dEdXMap_vsPHI;
  std::map<float, TH1D*> calib_dEdXMap_vsETA;
  std::map<int, TH1D*>   calib_muonAngleMap_vsREGIONID;
  std::map<float, TH1D*> calib_muonAngleMap_vsPHI;
  std::map<float, TH1D*> calib_muonAngleMap_vsETA;

  TH1F calib_nEvents_vsREGIONID("calib_nEvents_vsREGIONID", "nEvents_vsREGIONID", phiN*etaN, 0., 1.*phiN*etaN);
  TH1F calib_nEvents_vsPHI("calib_nEvents_vsPHI", "nEvents_vsPHI", phiN, IPHI_MIN, IPHI_MAX);
  TH1F calib_nEvents_vsETA("calib_nEvents_vsETA", "nEvents_vsETA", etaN, IETA_MIN, IETA_MAX);

  TProfile calib_muonAngle_profile_vsREGIONID("calib_muonAngle_profile_vsREGIONID", "calib_muonAngle_profile_vsREGIONID", phiN*etaN, 0., 1.*phiN*etaN);
  TProfile calib_muonAngle_profile_vsPHI("calib_muonAngle_profile_vsPHI", "calib_muonAngle_profile_vsPHI", phiN, IPHI_MIN, IPHI_MAX);
  TProfile calib_muonAngle_profile_vsETA("calib_muonAngle_profile_vsETA", "calib_muonAngle_profile_vsETA", etaN, IETA_MIN, IETA_MAX);

  TH1F calibCoeffDistr_mean_vsREGIONID("calibCoeffDistr_mean_vsREGIONID", "calibCoeffDistr_mean_vsREGIONID", 10000, 0., 10.);
  TH1F calibCoeffDistr_mean_vsPHI("calibCoeffDistr_mean_vsPHI", "calibCoeffDistr_mean_vsPHI", 10000, 0., 10.);
  TH1F calibCoeffDistr_mean_vsETA("calibCoeffDistr_mean_vsETA", "calibCoeffDistr_mean_vsETA", 10000, 0., 10.);
  TGraphErrors calibCoeff_mean_vsREGIONID;
  TGraphErrors calibCoeff_mean_vsPHI;
  TGraphErrors calibCoeff_mean_vsETA;

  TH1F calibCoeffDistr_MPV_vsREGIONID("calibCoeffDistr_MPV_vsREGIONID", "calibCoeffDistr_MPV_vsREGIONID", 10000, 0., 10.);
  TH1F calibCoeffDistr_MPV_vsPHI("calibCoeffDistr_MPV_vsPHI", "calibCoeffDistr_MPV_vsPHI", 10000, 0., 10.);
  TH1F calibCoeffDistr_MPV_vsETA("calibCoeffDistr_MPV_vsETA", "calibCoeffDistr_MPV_vsETA", 10000, 0., 10.);
  TGraphErrors calibCoeff_MPV_vsREGIONID;
  TGraphErrors calibCoeff_MPV_vsPHI;
  TGraphErrors calibCoeff_MPV_vsETA;
  // Calibration  
  
  
  
  
 
 
  // --------------------------------------------------------------------------------------
  // Loop over entries
  // --------------------------------------------------------------------------------------
  
  
  // Loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    chain -> GetEntry (entry);
    if(entry == maxEvent) break;
    
    
    
    
    // Association MU-SC
    std::vector<ect::association> associations;
    ect::fillAssocVector(associations, treeVars);
    ect::selectOnDR(associations, treeVars, 0.1);
    // Association MU-SC
    
    
    
    
    // Cut on number of reconstructed muons
    int nRecoMuons = treeVars.nRecoMuons;
    int nRecoMuons_up = 0;
    int nRecoMuons_down = 0;
    for(int MUit = 0; MUit < nRecoMuons; ++MUit)
    {
      if(treeVars.muonLeg[MUit] == 1) ++nRecoMuons_up;
      if(treeVars.muonLeg[MUit] == -1) ++nRecoMuons_down;
    }
    
    if( (nRecoMuons > 2) ||
        ( (nRecoMuons == 2) && (nRecoMuons_up > 1) ) ||
        ( (nRecoMuons == 2) && (nRecoMuons_down > 1) ) ) continue;
    // Cut on number of reconstructed muons
    
    
    
    
    // Sort association vector: first leg 1, then leg -1
    if(associations.size() == 2)
    {
      int MUindex = associations.at(1).first;
      if(treeVars.muonLeg[MUindex] == 1)
      {
	ect::association dummyAssociation = associations.at(0);
        associations.at(0) = associations.at(1);
        associations.at(1) = dummyAssociation;
      }
    }
    // Sort association vector: first leg 1, then leg -1




    // Alessio's cut                                                                                                   
    bool upFound = false;
    bool downFound = false;

    float ESC_up = 0.;
    float ESC_down = 0.;
    // Alessio's cut                                                                                                   



    // Loop on associations vector
    for(unsigned int i = 0; i < associations.size(); ++i)
    {
      int MUindex = associations.at(i).first;
      int SCindex = associations.at(i).second;
      
      
      
      
      
      
      // SuperCluster variables
      float superClusterEta = treeVars.superClusterEta[SCindex];
      float superClusterPhi = treeVars.superClusterPhi[SCindex];
      int nClustersInSuperCluster = treeVars.nClustersInSuperCluster[SCindex];
      int Cindex = treeVars.clusterIndexInSuperCluster[SCindex];
      int nXtalsInSuperCluster = treeVars.nXtalsInSuperCluster[SCindex];
      int xtalIndexInSuperCluster = treeVars.xtalIndexInSuperCluster[SCindex];
      EBDetId maxXtalDetId = EBDetId::unhashIndex(treeVars.xtalHashedIndex[xtalIndexInSuperCluster]);
      
      float superClusterRawEnergy = treeVars.superClusterRawEnergy[SCindex] * energyCorrFactor;
      float clusterE3x3 =                      treeVars.clusterE3x3[Cindex] * energyCorrFactor;
      float clusterE5x5 =                      treeVars.clusterE5x5[Cindex] * energyCorrFactor;
      float xtalTkLengthCurvedSum = 0.;

      bool badTTflags = false;
      for(int XTLit = xtalIndexInSuperCluster; XTLit < xtalIndexInSuperCluster + nXtalsInSuperCluster; ++XTLit)
      {
        if(treeVars.xtalTkLengthCurved[XTLit] > 0.) xtalTkLengthCurvedSum += treeVars.xtalTkLengthCurved[XTLit];

        //-----------------
        //skip bad TT
	EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLit]) ;
        int iFED = 600 + dummy.ism();
        if(dummy.ism() > 18) iFED -= 9;
        else if( dummy.ism() >= 1 && dummy.ism() < 19) iFED += 27;
        int iTT = dummy.tower().iTT();
         
        for(unsigned int badTT = 0; badTT < badTowerId.size(); ++badTT)
        if(iTT == badTowerId.at(badTT) && iFED == badTowerFED.at(badTT)) badTTflags = true;       
        //END skip bad TT
        //-----------------
      }

      // SuperCluster variables
      
      
      
      
      
      // Muon variables
      int pBin = -1;

      int muonLeg = treeVars.muonLeg[MUindex];
      float muonCharge = treeVars.muonCharge[MUindex];

      float muonPx = treeVars.muonPx[MUindex];
      float muonPy = treeVars.muonPy[MUindex];
      float muonPz = treeVars.muonPz[MUindex];
      float muonP = treeVars.muonP[MUindex];
      float muonPt = treeVars.muonPt[MUindex];
      float muonOutTkP = treeVars.muonOutTkInnerHitP[MUindex];

      float muond0 = treeVars.muond0[MUindex];
      float muondz = treeVars.muondz[MUindex];
      float muondR = sqrt( pow(muond0, 2.) + pow(muondz, 2.) );

      float muonNChi2 = treeVars.muonNChi2[MUindex];
      float muonNHits = treeVars.muonNHits[MUindex];

      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex];
      int nMuonCrossedXtalsCurved = treeVars.nMuonCrossedXtalsCurved[MUindex];

      GlobalPoint muonTkInternalPointInEcalCurved(treeVars.muonTkInternalPointInEcalCurvedX[MUindex],
                                                  treeVars.muonTkInternalPointInEcalCurvedY[MUindex],
                                                  treeVars.muonTkInternalPointInEcalCurvedZ[MUindex]);
      GlobalPoint muonTkExternalPointInEcalCurved(treeVars.muonTkExternalPointInEcalCurvedX[MUindex],
                                                  treeVars.muonTkExternalPointInEcalCurvedY[MUindex],
                                                  treeVars.muonTkExternalPointInEcalCurvedZ[MUindex]);
      TVector3 muonInnTkInnerPoint(treeVars.muonInnTkInnerHitX[MUindex],
                                   treeVars.muonInnTkInnerHitY[MUindex],
                                   treeVars.muonInnTkInnerHitZ[MUindex]);
      TVector3 muonInnTkInnerDirection(treeVars.muonInnTkInnerHitPx[MUindex],
                                       treeVars.muonInnTkInnerHitPy[MUindex],
                                       treeVars.muonInnTkInnerHitPz[MUindex]);
      TVector3 muonInnTkOuterPoint(treeVars.muonInnTkOuterHitX[MUindex],
                                   treeVars.muonInnTkOuterHitY[MUindex],
                                   treeVars.muonInnTkOuterHitZ[MUindex]);
      TVector3 muonInnTkOuterDirection(treeVars.muonInnTkOuterHitPx[MUindex],
                                       treeVars.muonInnTkOuterHitPy[MUindex],
                                       treeVars.muonInnTkOuterHitPz[MUindex]);

      TVector3 muonOutTkInnerDirection(treeVars.muonOutTkInnerHitPx[MUindex],
                                       treeVars.muonOutTkInnerHitPy[MUindex],
                                       treeVars.muonOutTkInnerHitPz[MUindex]);

      float muonPhi = muonTkInternalPointInEcalCurved.phi();
      float muonEta = muonTkInternalPointInEcalCurved.eta();

      TVector3 muonDirection(treeVars.muonTkInternalPointInEcalCurvedPx[MUindex],
                             treeVars.muonTkInternalPointInEcalCurvedPy[MUindex],
                             treeVars.muonTkInternalPointInEcalCurvedPz[MUindex]);
      TVector3 xtalAxis; setVectorOnECAL(xtalAxis, muonTkInternalPointInEcalCurved.eta(), muonTkInternalPointInEcalCurved.phi(), 129.);
      float muonAngle = xtalAxis.Angle(-1. * muonLeg * muonDirection);

      float muonSkyAngle = PI;      
      TVector3 yAxis(0., -1., 0.);
      
      if(muonLeg == 1)
      {
        //muonPx = treeVars.muonInnTkInnerHitPx[MUindex];
        //muonPy = treeVars.muonInnTkInnerHitPy[MUindex];
        //muonPz = treeVars.muonInnTkInnerHitPz[MUindex];
        //muonP = treeVars.muonInnTkInnerHitP[MUindex];
        //muonPt = treeVars.muonInnTkInnerHitPt[MUindex];
        muonOutTkP = treeVars.muonOutTkOuterHitP[MUindex];
        
        muonSkyAngle = yAxis.Angle(muonOutTkInnerDirection);      


        if(correctMuonP == true)
        {
          float muonP_corrected = muonP + superClusterRawEnergy;
          muonPx = muonPx / muonP * muonP_corrected;
          muonPy = muonPy / muonP * muonP_corrected;
          muonPz = muonPz / muonP * muonP_corrected;
          muonPt = muonPt / muonP * muonP_corrected;
          muonP = muonP_corrected;
        }

	ESC_up = superClusterRawEnergy;
	upFound = true;
      }
      
      else if(muonLeg == -1)
      {
        //muonPx = treeVars.muonInnTkOuterHitPx[MUindex];
        //muonPy = treeVars.muonInnTkOuterHitPy[MUindex];
        //muonPz = treeVars.muonInnTkOuterHitPz[MUindex];
        //muonP = treeVars.muonInnTkOuterHitP[MUindex];
        //muonPt = treeVars.muonInnTkOuterHitPt[MUindex];
        muonOutTkP = treeVars.muonOutTkInnerHitP[MUindex];

 	ESC_down = superClusterRawEnergy; 
 	downFound = true;
      }
      
      else continue;


      float muonQOverP = 1. * muonCharge / muonP;
      float muonQOverPError = treeVars.muonQOverPError[MUindex];
      float muonPError = 1. * muonQOverPError / muonQOverP / muonQOverP;

      float muonCrossedXtalTkLengthCurvedSum = 0.;
      float muonCrossedXtalTkLengthCurvedSum_ZS = 0.;
      float Lcut_onZS = 2.5 * ADGtoGeV / 8.28 * 1000. / FindBetheBlochValue(&BetheBloch_th, muonP);
      for(int XTLit = 0; XTLit < nMuonCrossedXtalsCurved; ++XTLit)
      {
        muonCrossedXtalTkLengthCurvedSum += treeVars.muonCrossedXtalTkLengthCurved[MUindex][XTLit];

	if(treeVars.muonCrossedXtalTkLengthCurved[MUindex][XTLit] > Lcut_onZS)
          muonCrossedXtalTkLengthCurvedSum_ZS += treeVars.muonCrossedXtalTkLengthCurved[MUindex][XTLit];
      } 

      float muonEOverP = 1. * superClusterRawEnergy / muonP;
      // Muon variables
      
      
      
      
      
      
      // dEdX variables
      float dEdXCurved = superClusterRawEnergy / muonTkLengthInEcalCurved / 8.28 * 1000.;
      //float dEdXCurved_E3x3 =      clusterE3x3 / muonTkLengthInEcalCurved / 8.28 * 1000.;
      //float dEdXCurved_E5x5 =      clusterE5x5 / muonTkLengthInEcalCurved / 8.28 * 1000.;
      // dEdX variables
      
      
      
      
      
      
      // --------------------------------------------------------------------------------------
      // Cut event
      // --------------------------------------------------------------------------------------
      
      nEvents_TOT.Fill(muonP);
      
      // Skip bad Xtals
      bool skip = false;
      int border = 3;
      for(unsigned int badIt = 0; badIt < badRegionIEtaMIN.size(); ++badIt)
      {
        double badRegionEtaMIN;
        double badRegionEtaMAX;
        double badRegionPhiMIN;
        double badRegionPhiMAX;
        
        if(badRegionIEtaMIN.at(badIt) >= 0) badRegionEtaMIN = (badRegionIEtaMIN.at(badIt) - border -  0.5) * PI / 180.;
        else                                badRegionEtaMIN = (badRegionIEtaMIN.at(badIt) - border +  0.5) * PI / 180.;
                                            badRegionPhiMIN = (badRegionIPhiMIN.at(badIt) - border - 10.5) * PI / 180.;
        if(badRegionPhiMIN > PI) badRegionPhiMIN -= 2.*PI;
        
        if(badRegionIEtaMAX.at(badIt) >= 0) badRegionEtaMAX = (badRegionIEtaMAX.at(badIt) + border -  0.5) * PI / 180.;
        else                                badRegionEtaMAX = (badRegionIEtaMAX.at(badIt) + border +  0.5) * PI / 180.;
                                            badRegionPhiMAX = (badRegionIPhiMAX.at(badIt) + border - 10.5) * PI / 180.;
        if(badRegionPhiMAX > PI) badRegionPhiMAX -= 2.*PI;

        
        if(muonTkInternalPointInEcalCurved.eta() >= badRegionEtaMIN &&
           muonTkInternalPointInEcalCurved.eta() <= badRegionEtaMAX &&
           muonTkInternalPointInEcalCurved.phi() >= badRegionPhiMIN &&
           muonTkInternalPointInEcalCurved.phi() <= badRegionPhiMAX)
          {skip = true; break;}
        
        if(muonTkExternalPointInEcalCurved.eta() >= badRegionEtaMIN &&
           muonTkExternalPointInEcalCurved.eta() <= badRegionEtaMAX &&
           muonTkExternalPointInEcalCurved.phi() >= badRegionPhiMIN &&
           muonTkExternalPointInEcalCurved.phi() <= badRegionPhiMAX)
          {skip = true; break;}
        
        if( (muonTkInternalPointInEcalCurved.eta() <= badRegionEtaMIN &&
             muonTkInternalPointInEcalCurved.phi() <= badRegionPhiMIN &&
             muonTkExternalPointInEcalCurved.eta() >= badRegionEtaMAX &&
             muonTkExternalPointInEcalCurved.phi() >= badRegionPhiMAX) ||
            (muonTkExternalPointInEcalCurved.eta() <= badRegionEtaMIN &&
             muonTkExternalPointInEcalCurved.phi() <= badRegionPhiMIN &&
             muonTkInternalPointInEcalCurved.eta() >= badRegionEtaMAX &&
             muonTkInternalPointInEcalCurved.phi() >= badRegionPhiMAX) )
          {skip = true; break;}
         
        if(superClusterEta >= badRegionEtaMIN &&
           superClusterEta <= badRegionEtaMAX &&
           superClusterPhi >= badRegionPhiMIN &&
           superClusterPhi <= badRegionPhiMAX)
          {skip = true; break;}
      }
      
      if (skip == true) continue;

      if(badTTflags == true) continue;
      // Skip bad xtals
      
      
      // Other cuts
      if ( (muonLegUpOK == true && muonLegDownOK == false && muonLeg != 1) ||
           (muonLegDownOK == true && muonLegUpOK == false && muonLeg != -1) ) continue;
      if ( (muonP < muonPMIN) || (muonP > muonPMAX) ) continue ;
      if ( muonQOverPError != muonQOverPError) continue;
      if ( (1. * muonPError / muonP) > muonPErrorOverPMAX) continue ;
      if (fabs(muond0) > muond0MAX) continue;
      if (fabs(muondz) > muondzMAX) continue;
      if (muondR > muondRMAX) continue;
      if (muonNChi2 > muonNChi2MAX) continue;
      if (muonNHits < muonNHitsMIN) continue;
      if ( (muonTkLengthInEcalCurved < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcalCurved > muonTkLengthInEcalMAX) ) continue;
      if (xtalTkLengthCurvedSum/muonTkLengthInEcalCurved < xtalTkLengthSumMIN) continue;
      if (xtalTkLengthCurvedSum/muonTkLengthInEcalCurved > xtalTkLengthSumMAX) continue;
      if (muonEOverP > muonEOverPMAX) continue;
      if (muonAngle > muonAngleMAX) continue;
      if (muonAngle < muonAngleMIN) continue;

      if (1.*nXtalsInSuperCluster/nMuonCrossedXtalsCurved < nXtalsInSuperClusterMIN) continue;
      if (1.*nXtalsInSuperCluster/nMuonCrossedXtalsCurved > nXtalsInSuperClusterMAX) continue;
  
      if ( fabs(muonInnTkInnerPoint.z()) < muonInnTkHitZMIN) continue;
      if ( fabs(muonInnTkInnerPoint.z()) > muonInnTkHitZMAX) continue;
      if ( fabs(muonInnTkOuterPoint.z()) < muonInnTkHitZMIN) continue;
      if ( fabs(muonInnTkOuterPoint.z()) > muonInnTkHitZMAX) continue;

      if ( (superClusterPhi < superClusterPhiMIN) ||
           (superClusterPhi > superClusterPhiMAX) ) continue;
      if ( (superClusterEta < superClusterEtaMIN) ||
           (superClusterEta > superClusterEtaMAX) ) continue;

      if( ( applyVeto == true) && (muonP > muonPCollTHRESH) && (muonLeg ==  1) ) continue;
      if( ( applyVeto == true) && (muonP > muonPCollTHRESH) && (muonLeg == -1) &&
          (upFound == true) && (ESC_up > 0.5) ) continue;

      // Other cuts
      
      nEvents_CUT.Fill(muonP);
      
      
      
      
      
      
      // --------------------------------------------------------------------------------------
      // Fill distributions
      // --------------------------------------------------------------------------------------

      float trueVal = 1.;
      if(useTrueVal == true)
        trueVal = FindBetheBlochValue(&BetheBloch_th, muonP);


      // dEdX histograms
      if (nClustersInSuperCluster == 1)
      {
        dEdXDistrCurved.Fill(dEdXCurved / trueVal);
        //dEdXDistrCurved_E3x3.Fill(dEdXCurved_E3x3 / trueVal);
        //dEdXDistrCurved_E5x5.Fill(dEdXCurved_E5x5 / trueVal);
      }
      // dEdX histograms
      
      
      
      
      // BetheBloch - curved tkLength
      BetheBloch_curved.Fill(muonP, dEdXCurved / trueVal);
      pBin = BetheBloch_profile_curved.Fill(muonP, dEdXCurved / trueVal);
      BetheBloch_profile_vsPHI_curved.Fill(maxXtalDetId.iphi(), dEdXCurved / trueVal);
      BetheBloch_profile_vsETA_curved.Fill(maxXtalDetId.ieta(), dEdXCurved / trueVal);
      pMapCurved[pBin] += muonP;
      
      //if (nClustersInSuperCluster == 1)
      //{
      //  BetheBloch_E3x3_curved.Fill(muonP, dEdXCurved_E3x3 / trueVal);
      //  BetheBloch_profile_E3x3_curved.Fill(muonP, dEdXCurved_E3x3 / trueVal);
      //  pMapCurved_E3x3[pBin] += muonP;
      //  
      //  BetheBloch_E5x5_curved.Fill(muonP, dEdXCurved_E5x5 / trueVal);
      //  BetheBloch_profile_E5x5_curved.Fill(muonP, dEdXCurved_E5x5 / trueVal);
      //  pMapCurved_E5x5[pBin] += muonP;
      //}

      // BetheBloch - curved tkLength
      
      
      
      
      // Muon histograms
      if( (upFound == true) && (downFound == true) )
      {
        for(int bin = 1; bin <= nMuons_overTh.GetNbinsX(); ++bin)
        {
 	  if(ESC_up > nMuons_overTh.GetBinCenter(bin))
          {
	    if(ESC_down > nMuons_overTh.GetBinCenter(bin)) nMuons_overTh.Fill(nMuons_overTh.GetBinCenter(bin), 1.);
	    else                                           nMuons_overTh.Fill(nMuons_overTh.GetBinCenter(bin), 0.);
	  }
        } 
      }
      
      muonOccupancyInt_ETAvsPHI.Fill(muonTkInternalPointInEcalCurved.phi(), muonTkInternalPointInEcalCurved.eta());
      muonOccupancyExt_ETAvsPHI.Fill(muonTkExternalPointInEcalCurved.phi(), muonTkExternalPointInEcalCurved.eta());
      
      muonPDistr.Fill(muonP);
      muonOutTkPoPDistr.Fill(1. * muonOutTkP / muonP);
      muonOutTkPoP_vsP.Fill(muonP, 1. * muonOutTkP / muonP);
      if( (pBin > 0) && (pBin <= 25) )
        muonPDistr_pBin.at(pBin-1) -> Fill(muonP);

      muonQOverPDistr.Fill(muonQOverP);
      muonQOverPErrorDistr.Fill(muonQOverPError);

      muonPErrorDistr.Fill(muonPError);
      muonPError_vsP.Fill(muonP, muonPError);
      muonPError_profile_vsP.Fill(muonP, muonPError);
        
      muonPErrorOverPDistr.Fill(1. * muonPError / muonP);
      muonPErrorOverP_vsP.Fill(muonP, 1. * muonPError / muonP);
      muonPErrorOverP_profile_vsP.Fill(muonP, 1. * muonPError / muonP);
      
      muonChargeDistr.Fill(muonCharge);
      muonNChi2Distr.Fill(muonNChi2);
      muonNHitsDistr.Fill(muonNHits);
      
      muond0Distr.Fill(muond0);
      muondzDistr.Fill(muondz);
      
      muondRDistr.Fill(muondR);
      muondR_vsP.Fill(muonP, muondR);
      muondR_profile_vsP.Fill(muonP, muondR);
      muondR_vsAngle.Fill(muonAngle, muondR);
      muondR_profile_vsAngle.Fill(muonAngle, muondR);

      muonTkInternalPointInEcalCurvedPhiDistr.Fill(muonTkInternalPointInEcalCurved.phi());
      muonTkInternalPointInEcalCurvedEtaDistr.Fill(muonTkInternalPointInEcalCurved.eta());
      muonTkInternalPointInEcalCurvedRDistr.Fill(muonTkInternalPointInEcalCurved.perp());
      muonTkInternalPointInEcalCurvedZDistr.Fill(muonTkInternalPointInEcalCurved.z());
      muonTkInternalPointInEcalCurved_RvsZ.Fill(muonTkInternalPointInEcalCurved.z(), muonTkInternalPointInEcalCurved.perp());
      muonTkExternalPointInEcalCurvedPhiDistr.Fill(muonTkExternalPointInEcalCurved.phi());
      muonTkExternalPointInEcalCurvedEtaDistr.Fill(muonTkExternalPointInEcalCurved.eta());
      muonTkExternalPointInEcalCurvedRDistr.Fill(muonTkExternalPointInEcalCurved.perp());
      muonTkExternalPointInEcalCurvedZDistr.Fill(muonTkExternalPointInEcalCurved.z());
      muonTkExternalPointInEcalCurved_RvsZ.Fill(muonTkExternalPointInEcalCurved.z(), muonTkExternalPointInEcalCurved.perp());
      
      muonSkyAngleDistr.Fill(muonSkyAngle);
      muonAngleDistr.Fill(muonAngle);
      muonAngle_vsP.Fill(muonP, muonAngle);
      muonAngle_profile_vsP.Fill(muonP, muonAngle);
      muonAngle_vsDR.Fill(muondR, muonAngle);
      muonAngle_profile_vsDR.Fill(muondR, muonAngle);
      muonAngle_profile_vsETA.Fill(muonEta, muonAngle);
      muonAngle_profile_vsPHI.Fill(muonPhi, muonAngle);
      
      muonInnTkDAngleDistr.Fill(muonInnTkInnerDirection.Angle(muonInnTkOuterDirection));
      muonInnTkDAngle_vsPT.Fill(muonPt, muonInnTkInnerDirection.Angle(muonInnTkOuterDirection));
      muonInnTkDAngle_profile_vsPT.Fill(muonPt, muonInnTkInnerDirection.Angle(muonInnTkOuterDirection));
      
      muonTkLengthCurvedDistr.Fill(muonTkLengthInEcalCurved);
      muonTkLengthCurved_vsDR.Fill(muondR, muonTkLengthInEcalCurved);
      muonTkLengthCurved_profile_vsDR.Fill(muondR, muonTkLengthInEcalCurved);
      muonTkLengthCurved_vsAngle.Fill(muonAngle, muonTkLengthInEcalCurved);
      muonTkLengthCurved_profile_vsAngle.Fill(muonAngle, muonTkLengthInEcalCurved);
      
      nMuonCrossedXtalsCurvedDistr.Fill(nMuonCrossedXtalsCurved);
      nMuonCrossedXtalsCurved_profile_vsAngle.Fill(muonAngle, nMuonCrossedXtalsCurved);
      nMuonCrossedXtalsCurved_profile_vsPHI.Fill(muonPhi, nMuonCrossedXtalsCurved);
      nMuonCrossedXtalsCurved_profile_vsETA.Fill(muonEta, nMuonCrossedXtalsCurved);
      nMuonCrossedXtalsCurved_profile_ETAvsPHI.Fill(muonPhi, muonEta, nMuonCrossedXtalsCurved);
      muonCrossedXtalTkLengthCurvedSumDistr.Fill(muonCrossedXtalTkLengthCurvedSum/muonTkLengthInEcalCurved);
      muonCrossedXtalTkLengthCurvedSumZS_profile_vsAngle.Fill(muonAngle, muonCrossedXtalTkLengthCurvedSum_ZS/muonTkLengthInEcalCurved);

      ZSLengthDistr.Fill(Lcut_onZS); 
      ZSLength_vsAngle.Fill(muonAngle, Lcut_onZS);
      // Muon histograms
      
      
      
      
      // SuperCluster histograms
      superClusterOccupancy_ETAvsPHI.Fill(superClusterPhi, superClusterEta);
      
      superClusterRawEnergyDistr.Fill(superClusterRawEnergy);
      superClusterRawEnergy_vsP.Fill(muonP, superClusterRawEnergy);
      superClusterRawEnergy_profile_vsP.Fill(muonP, superClusterRawEnergy);
      superClusterRawEnergy_vsAngle.Fill(muonAngle, superClusterRawEnergy);
      superClusterRawEnergy_profile_vsAngle.Fill(muonAngle, superClusterRawEnergy);
      
      EoPDistr.Fill(1. * superClusterRawEnergy / muonP);
      EoP_vsP.Fill(muonP, 1. * superClusterRawEnergy / muonP);
      
      nXtalsInSuperClusterDistr.Fill(nXtalsInSuperCluster);
      nXtalsInSuperCluster_vsAngle.Fill(muonAngle, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_vsAngle.Fill(muonAngle, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_vsPHI.Fill(muonPhi, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_vsETA.Fill(muonEta, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_ETAvsPHI.Fill(muonPhi, muonEta, nXtalsInSuperCluster);

      nXtalsInSuperClusterOverCrossedXtalsCurvedDistr.Fill(1.*nXtalsInSuperCluster/nMuonCrossedXtalsCurved);
      // SuperCluster histograms
      
      
      
      
      // Xtal Histograms
      int XTLMAXCurvedindex = 0;
      float xtalTkLengthCurvedMax = 0.;
      
      for(int XTLindex = xtalIndexInSuperCluster;
          XTLindex < xtalIndexInSuperCluster + nXtalsInSuperCluster; ++XTLindex)
      {
        EBDetId dummy = EBDetId::unhashIndex(treeVars.xtalHashedIndex[XTLindex]);
        xtalOccupancy_iETAvsiPHI.Fill(dummy.iphi(), dummy.ieta());
        
        xtalEnergyDistr.Fill(treeVars.xtalEnergy[XTLindex]);

        xtalTkLengthCurvedDistr.Fill(treeVars.xtalTkLengthCurved[XTLindex]);
        xtalTkLengthCurved_vsAngle.Fill(muonAngle, treeVars.xtalTkLengthCurved[XTLindex]);
        xtalTkLengthCurved_profile_vsAngle.Fill(muonAngle, treeVars.xtalTkLengthCurved[XTLindex]);
        
        if(treeVars.xtalTkLengthCurved[XTLindex] > xtalTkLengthCurvedMax)
        {
          xtalTkLengthCurvedMax = treeVars.xtalTkLengthCurved[XTLindex];
          XTLMAXCurvedindex = XTLindex;
        }
      }
      
      xtalTkLengthCurvedSumDistr.Fill(xtalTkLengthCurvedSum/muonTkLengthInEcalCurved);
      xtalTkLengthCurvedSum_FRACvsAngle.Fill(muonAngle, xtalTkLengthCurvedSum/muonTkLengthInEcalCurved);
      xtalTkLengthCurvedSum_vsAngle.Fill(muonAngle, xtalTkLengthCurvedSum/muonTkLengthInEcalCurved);
      xtalTkLengthCurvedSum_vsPHI.Fill(muonPhi, xtalTkLengthCurvedSum/muonTkLengthInEcalCurved);
      xtalTkLengthCurvedSum_vsETA.Fill(muonEta, xtalTkLengthCurvedSum/muonTkLengthInEcalCurved);

      xtalTkLengthCurvedMaxDistr.Fill(treeVars.xtalTkLengthCurved[XTLMAXCurvedindex]);
      xtalTkLengthCurvedMax_vsAngle.Fill(muonAngle, treeVars.xtalTkLengthCurved[XTLMAXCurvedindex]);
      xtalTkLengthCurvedMax_profile_vsAngle.Fill(muonAngle, treeVars.xtalTkLengthCurved[XTLMAXCurvedindex]);
      // Xtal Histograms
      
      
      
      
      
      
      // Calibration
      int seedXtalIndex = treeVars.xtalIndexInSuperCluster[SCindex];
      int seedXtalHashedIndex = treeVars.xtalHashedIndex[seedXtalIndex];
      EBDetId seedXtalEBDetId = EBDetId::unhashIndex(seedXtalHashedIndex);
      int seedXtalIphi = seedXtalEBDetId.iphi ();
      int seedXtalIeta = seedXtalEBDetId.ieta ();

      
      // find region
      if(region.EBregionCheck(seedXtalIeta, seedXtalIphi) != 0) continue;

      int regionId = region.EBRegionId(seedXtalIeta, seedXtalIphi);
      int phiId = regionId%phiN;
      int etaId = regionId/phiN;

      float phiCenter = phiSTART + phiId*phiWIDTH + phiWIDTH/2.;
      float etaCenter = etaSTART + etaId*etaWIDTH + etaWIDTH/2.;






      // fill
      map_iterator_int mapIt = calib_dEdXMap_vsREGIONID.find(regionId);
      if( (mapIt == calib_dEdXMap_vsREGIONID.end()) && (regionId != -1) )
      {
        char histoName[100];
        sprintf(histoName, "calib_dEdX_vsREGIONID_region_%05d---phi_%d_%d---eta_%d_%d",
		   regionId,
		   phiSTART  + phiWIDTH * (regionId%phiN),
		   phiSTART  + phiWIDTH * (regionId%phiN + 1),
		   etaSTART  + etaWIDTH * (regionId/phiN),
		   etaSTART  + etaWIDTH * (regionId/phiN + 1));

        calib_dEdXMap_vsREGIONID[regionId] = new TH1D(histoName, histoName, 20000, 0., 1000.);


        sprintf(histoName, "calib_muonAngle_vsREGIONID_region_%05d---phi_%d_%d---eta_%d_%d",
		   regionId,
		   phiSTART  + phiWIDTH * (regionId%phiN),
		   phiSTART  + phiWIDTH * (regionId%phiN + 1),
		   etaSTART  + etaWIDTH * (regionId/phiN),
		   etaSTART  + etaWIDTH * (regionId/phiN + 1));

        calib_muonAngleMap_vsREGIONID[regionId] = new TH1D(histoName, histoName, 1000, 0., PI/2.);
      }




      map_iterator_float mapIt_float = calib_dEdXMap_vsPHI.find(phiCenter);
      if( (mapIt_float == calib_dEdXMap_vsPHI.end()) && (regionId != -1) )
      {
        char histoName[100];
        sprintf(histoName, "calib_dEdX_vsPHI_region_%05d---phi_%d_%d---eta_%d_%d",
		   regionId,
		   phiSTART  + phiWIDTH * (regionId%phiN),
		   phiSTART  + phiWIDTH * (regionId%phiN + 1),
		   etaSTART  + etaWIDTH * (regionId/phiN),
		   etaSTART  + etaWIDTH * (regionId/phiN + 1));

        calib_dEdXMap_vsPHI[phiCenter] = new TH1D(histoName, histoName, 20000, 0., 1000.);


        sprintf(histoName, "muonAngle_vsPHI_region_%05d---phi_%d_%d---eta_%d_%d",
		   regionId,
		   phiSTART  + phiWIDTH * (regionId%phiN),
		   phiSTART  + phiWIDTH * (regionId%phiN + 1),
		   etaSTART  + etaWIDTH * (regionId/phiN),
		   etaSTART  + etaWIDTH * (regionId/phiN + 1));

        calib_muonAngleMap_vsPHI[phiCenter] = new TH1D(histoName, histoName, 1000, 0., PI/2.);
      }



      mapIt_float = calib_dEdXMap_vsETA.find(etaCenter);
      if( (mapIt_float == calib_dEdXMap_vsETA.end()) && (regionId != -1) )
      {
        char histoName[100];
        sprintf(histoName, "calib_dEdX_vsETA_region_%05d---phi_%d_%d---eta_%d_%d",
		   regionId,
		   phiSTART  + phiWIDTH * (regionId%phiN),
		   phiSTART  + phiWIDTH * (regionId%phiN + 1),
		   etaSTART  + etaWIDTH * (regionId/phiN),
		   etaSTART  + etaWIDTH * (regionId/phiN + 1));

        calib_dEdXMap_vsETA[etaCenter] = new TH1D(histoName, histoName, 20000, 0., 1000.);


        sprintf(histoName, "muonAngle_vsETA_region_%05d---phi_%d_%d---eta_%d_%d",
		   regionId,
		   phiSTART  + phiWIDTH * (regionId%phiN),
		   phiSTART  + phiWIDTH * (regionId%phiN + 1),
		   etaSTART  + etaWIDTH * (regionId/phiN),
		   etaSTART  + etaWIDTH * (regionId/phiN + 1));

        calib_muonAngleMap_vsETA[etaCenter] = new TH1D(histoName, histoName, 1000, 0., PI/2.);
      }






      if(regionId != -1)
      {
        regionOccupancy_ETAvsPHI.Fill(seedXtalIphi, etaShifter(seedXtalIeta));

        calib_nEvents_vsREGIONID.Fill(regionId);
        calib_nEvents_vsPHI.Fill(phiCenter);
        calib_nEvents_vsETA.Fill(etaCenter);

        calib_muonAngleMap_vsREGIONID[regionId] -> Fill(muonAngle);
        calib_muonAngleMap_vsPHI[phiCenter] -> Fill(muonAngle);
        calib_muonAngleMap_vsETA[etaCenter] -> Fill(muonAngle);

        calib_muonAngle_profile_vsREGIONID.Fill(regionId, muonAngle);
        calib_muonAngle_profile_vsPHI.Fill(phiCenter, muonAngle);
        calib_muonAngle_profile_vsETA.Fill(etaCenter, muonAngle);

        calib_dEdXMap_vsREGIONID[regionId] -> Fill(dEdXCurved / trueVal);
        calib_dEdXMap_vsPHI[phiCenter] -> Fill(dEdXCurved / trueVal);
        calib_dEdXMap_vsETA[etaCenter] -> Fill(dEdXCurved / trueVal);
      }


      
    } // Loop on associations vector
    
  } // Loop over entries
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  
  nEvents_TOT.Write();
  nEvents_CUT.Write();
  for(int bin = 1; bin <= P_BIN; ++bin)
    if(nEvents_TOT.GetBinContent(bin) != 0)
      nEvents_EFF.SetBinContent(bin, 1. * nEvents_CUT.GetBinContent(bin) / nEvents_TOT.GetBinContent(bin));
  nEvents_EFF.Write();
  
  
  // dEdX histograms
  outRootFile.mkdir ("dEdX");
  outRootFile.cd ("dEdX");
  std::cout << ">>> Saving dEdX histograms" << std::endl;
  
  double startValues4[4] = {0.12, 1.5, dEdXDistrCurved.GetEntries() / 10., 0.25};
  TF1* langaus = new TF1;
  LangausFit(&langaus, &dEdXDistrCurved, startValues4);
  dEdXDistrCurved.Write();
  delete langaus ;
  
  //double startValues5[4] = {0.12, 1.5, dEdXDistrCurved_E3x3.GetEntries() / 10., 0.25};
  //langaus = new TF1;
  //LangausFit(&langaus, &dEdXDistrCurved_E3x3, startValues5);
  //dEdXDistrCurved_E3x3.Write();
  //delete langaus;
  
  //double startValues6[4] = {0.000903, 0.01249, 1.708, 0.00187};
  //double startValues6[4] = {0.12, 1.5, dEdXDistrCurved_E5x5.GetEntries() / 10., 0.25};
  //langaus = new TF1;
  //LangausFit(&langaus, &dEdXDistrCurved_E5x5, startValues6);
  //dEdXDistrCurved_E5x5.Write();
  //delete langaus;
  
  outRootFile.cd();
  // dEdX histograms
  
  
  
  
  
  
  // BetheBloch - curved tkLength
  outRootFile.mkdir("BetheBlochCurved_pBin");
  outRootFile.cd("BetheBlochCurved_pBin");
  std::cout << ">>> Saving BetheBloch histograms" << std::endl;

  double errX_low = -1.;
  double errX_hig = -1.;
  double errY = -1.;
  
  for(int bin = 1; bin <= P_BIN; ++bin)
  {
    if (pMapCurved[bin] > 0)
      pMapCurved[bin] /= BetheBloch_profile_curved.GetBinEntries(bin);
    //if (pMapCurved_E3x3[bin] > 0)
    //  pMapCurved_E3x3[bin] /= BetheBloch_profile_E3x3_curved.GetBinEntries(bin);
    //if (pMapCurved_E5x5[bin] > 0)
    //  pMapCurved_E5x5[bin] /= BetheBloch_profile_E5x5_curved.GetBinEntries(bin);
    
    
    char histoName[50];
    sprintf(histoName, "BetheBloch_pBin_curved_%d", bin);
    TH1D* histo = BetheBloch_curved.ProjectionY(histoName, bin, bin);

    double startValues[4] = {0.12, histo -> GetMean(), histo -> GetEntries() / 10., 0.25};
    TF1* langaus = new TF1;
    LangausFit(&langaus, histo, startValues);
    
    if (pMapCurved[bin] > 0)
    {
      BetheBloch_graph_MPV_curved.SetPoint(bin-1, pMapCurved[bin], langaus -> GetParameter(1));
      BetheBloch_graph_MPV_curved.SetPointError(bin-1, 0., langaus -> GetParError(1));
      BetheBloch_mean_curved[bin] = langaus -> Mean(0., 100.);    
      BetheBloch_graph_GSigma_curved.SetPoint(bin-1, pMapCurved[bin], langaus -> GetParameter(3));
      BetheBloch_graph_GSigma_curved.SetPointError(bin-1, 0., langaus -> GetParError(3));
    }
    histo -> Write();
    
    
    errX_low = pMapCurved[bin] - BetheBloch_profile_curved.GetBinLowEdge(bin);
    errX_hig = BetheBloch_profile_curved.GetBinLowEdge(bin) + BetheBloch_profile_curved.GetBinWidth(bin) - pMapCurved[bin];
    errY = BetheBloch_profile_curved.GetBinError(bin);
    
    
    BetheBloch_graph_curved.SetPoint(bin-1, pMapCurved[bin], BetheBloch_profile_curved.GetBinContent(bin));
    BetheBloch_graph_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);

    BetheBloch_graph_mean_curved.SetPoint(bin-1, pMapCurved[bin], BetheBloch_mean_curved[bin]);
    BetheBloch_graph_mean_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    //BetheBloch_graph_E3x3_curved.SetPoint(bin-1, pMapCurved_E3x3[bin], BetheBloch_profile_E3x3_curved.GetBinContent(bin));
    //BetheBloch_graph_E3x3_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    
    //BetheBloch_graph_E5x5_curved.SetPoint(bin-1, pMapCurved_E5x5[bin], BetheBloch_profile_E5x5_curved.GetBinContent(bin));
    //BetheBloch_graph_E5x5_curved.SetPointError(bin-1, errX_low, errX_hig, errY, errY);
    

    std::cout << "Bin " << std::fixed << std::setprecision(2) << std::setw(3) << bin 
              << ":   [" << std::setw(7) << BetheBloch_profile_curved.GetBinLowEdge(bin)
              << "," << std::setw(7) << BetheBloch_profile_curved.GetBinLowEdge(bin) + BetheBloch_profile_curved.GetBinWidth(bin)
              << "]         BinCenter: " << std::setw(7) << BetheBloch_profile_curved.GetBinCenter(bin)
              << "   BinAvgCenter: " << std::setw(7) << pMapCurved[bin]
              << "         BinEntries: " << std::setprecision(0) << std::setw(5) << BetheBloch_profile_curved.GetBinEntries(bin)
              << "         BinContent: " << std::setprecision(3) << std::setw(5) << BetheBloch_profile_curved.GetBinContent(bin)
              << std::endl;
  }
  
  DrawBetheBlochResiduals(BetheBloch_graph_curved, BetheBloch_th, BetheBloch_graph_residuals_curved);
  
  outRootFile.cd();
  // BetheBloch - curved tkLength
  

  
  
  
  
  
  // Muon histograms
  outRootFile.mkdir("Muon");
  outRootFile.cd("Muon");
  std::cout << ">>> Saving Muon histograms" << std::endl;
  
  nMuons_overTh.Write();

  muonOccupancyInt_ETAvsPHI.Write();
  muonOccupancyExt_ETAvsPHI.Write();
  
  muonPDistr.Write();
  muonOutTkPoPDistr.Write();
  muonOutTkPoP_vsP.Write();

  outRootFile.mkdir("muonPDistr_pBin");
  outRootFile.cd("muonPDistr_pBin");
  for(int bin = 0; bin < P_BIN; ++bin)
    muonPDistr_pBin.at(bin) -> Write();
  outRootFile.cd("Muon");

  muonQOverPDistr.Write();
  muonQOverPErrorDistr.Write();

  muonPErrorDistr.Write();
  muonPError_vsP.Write();
  muonPError_profile_vsP.Write();

  muonPErrorOverPDistr.Write();
  muonPErrorOverP_vsP.Write();
  muonPErrorOverP_profile_vsP.Write();
  
  muonChargeDistr.Write();
  muonNChi2Distr.Write();
  muonNHitsDistr.Write();
  
  muond0Distr.Write();
  muondzDistr.Write();
  
  muondRDistr.Write();
  muondR_vsP.Write();
  muondR_profile_vsP.Write();
  muondR_vsAngle.Write();
  muondR_profile_vsAngle.Write();

  muonTkInternalPointInEcalCurvedPhiDistr.Write();
  muonTkInternalPointInEcalCurvedEtaDistr.Write();
  muonTkInternalPointInEcalCurvedRDistr.Write();
  muonTkInternalPointInEcalCurvedZDistr.Write();
  muonTkInternalPointInEcalCurved_RvsZ.Write();
  muonTkExternalPointInEcalCurvedPhiDistr.Write();
  muonTkExternalPointInEcalCurvedEtaDistr.Write();
  muonTkExternalPointInEcalCurvedRDistr.Write();
  muonTkExternalPointInEcalCurvedZDistr.Write();
  muonTkExternalPointInEcalCurved_RvsZ.Write();  

  muonSkyAngleDistr.Write();
  muonAngleDistr.Write();
  muonAngle_vsP.Write();
  muonAngle_profile_vsP.Write();
  muonAngle_vsDR.Write();
  muonAngle_profile_vsDR.Write();
  muonAngle_profile_vsETA.Write();
  muonAngle_profile_vsPHI.Write();
  
  muonInnTkDAngleDistr.Write();
  muonInnTkDAngle_vsPT.Write();
  muonInnTkDAngle_profile_vsPT.Write();
  
  muonTkLengthCurvedDistr.Write();
  muonTkLengthCurved_vsDR.Write();
  muonTkLengthCurved_profile_vsDR.Write();
  muonTkLengthCurved_vsAngle.Write();
  muonTkLengthCurved_profile_vsAngle.Write();
  
  nMuonCrossedXtalsCurvedDistr.Write();
  nMuonCrossedXtalsCurved_profile_vsAngle.Write();  
  nMuonCrossedXtalsCurved_profile_vsPHI.Write();  
  nMuonCrossedXtalsCurved_profile_vsETA.Write();
  nMuonCrossedXtalsCurved_profile_ETAvsPHI.Write();
  muonCrossedXtalTkLengthCurvedSumDistr.Write();
  muonCrossedXtalTkLengthCurvedSumZS_profile_vsAngle.Write();

  ZSLengthDistr.Write();
  ZSLength_vsAngle.Write();

  outRootFile.cd();
  // Muon histograms
  
  
  
  
  
  
  // SuperCluster histograms
  outRootFile.mkdir ("SuperCluster");
  outRootFile.cd ("SuperCluster");
  std::cout << ">>> Saving SuperCluster histograms" << std::endl;  

  superClusterOccupancy_ETAvsPHI.Write();
  
  superClusterRawEnergyDistr.Write();
  superClusterRawEnergy_vsP.Write();
  superClusterRawEnergy_profile_vsP.Write();
  for (int bin = 1; bin <= P_BIN; ++bin)
  {
    char histoName[50];
    sprintf(histoName, "superClusterRawEnergy_pBin_%d", bin);
    TH1D* histo = superClusterRawEnergy_vsP.ProjectionY(histoName, bin, bin);
    histo -> Write();
  }
  superClusterRawEnergy_vsAngle.Write();
  superClusterRawEnergy_profile_vsAngle.Write();

  EoPDistr.Write();
  EoP_vsP.Write();
  
  nXtalsInSuperClusterDistr.Write();
  nXtalsInSuperCluster_vsAngle.Write();
  nXtalsInSuperCluster_profile_vsAngle.Write();
  nXtalsInSuperCluster_profile_vsPHI.Write();
  nXtalsInSuperCluster_profile_vsETA.Write();
  nXtalsInSuperCluster_profile_ETAvsPHI.Write();  

  nXtalsInSuperClusterOverCrossedXtalsCurvedDistr.Write();

  outRootFile.cd ();
  // SuperCluster histograms
  
  
  
  
  
  
  // Xtal histograms
  outRootFile.mkdir("Xtal");
  outRootFile.cd("Xtal");
  std::cout << ">>> Saving Xtal histograms" << std::endl;  
  
  xtalOccupancy_iETAvsiPHI.Write();

  xtalEnergyDistr.Write();
  
  xtalTkLengthCurvedSumDistr.Write();
  xtalTkLengthCurvedSum_FRACvsAngle.Write();
  xtalTkLengthCurvedSum_vsAngle.Write();
  xtalTkLengthCurvedSum_vsPHI.Write();
  xtalTkLengthCurvedSum_vsETA.Write();
  
  xtalTkLengthCurvedDistr.Write();
  xtalTkLengthCurved_vsAngle.Write();
  xtalTkLengthCurved_profile_vsAngle.Write();
  
  xtalTkLengthCurvedMaxDistr.Write();
  xtalTkLengthCurvedMax_vsAngle.Write();
  xtalTkLengthCurvedMax_profile_vsAngle.Write();

  outRootFile.cd();
  // Xtal histograms
  
  
  
  
  
  
  BetheBloch_curved.Write();
  BetheBloch_profile_curved.Write();
  BetheBloch_profile_vsPHI_curved.Write();
  BetheBloch_profile_vsETA_curved.Write();  
  BetheBloch_graph_curved.Write("BetheBloch_graph_curved");
  BetheBloch_graph_residuals_curved.Write("BetheBloch_graph_residuals_curved");

  BetheBloch_graph_MPV_curved.Write("BetheBloch_graph_MPV_curved");
  BetheBloch_graph_mean_curved.Write("BetheBloch_graph_mean_curved");
  BetheBloch_graph_GSigma_curved.Write("BetheBloch_graph_GSigma_curved");

  //BetheBloch_E3x3_curved.Write();
  //BetheBloch_profile_E3x3_curved.Write();
  //BetheBloch_graph_E3x3_curved.Write("BetheBloch_graph_E3x3_curved");

  //BetheBloch_E5x5_curved.Write();
  //BetheBloch_profile_E5x5_curved.Write();
  //BetheBloch_graph_E5x5_curved.Write("BetheBloch_graph_E5x5_curved");







  TDirectory* sd1 = outRootFile.mkdir("Calibration");
  sd1 -> cd();

  regionOccupancy_ETAvsPHI.Write();



  TDirectory* sd2 = sd1 -> mkdir("vsREGIONID");
  sd2 -> cd();

  calib_nEvents_vsREGIONID.Write();
  calib_muonAngle_profile_vsREGIONID.Write();

  int point = 0;
  for(map_const_iterator_int mapIt = calib_dEdXMap_vsREGIONID.begin();
      mapIt != calib_dEdXMap_vsREGIONID.end() ; ++mapIt)
  {
    int regionId = mapIt -> first;
    TH1D* histo = mapIt -> second;
    
    double startValues[4] = {0.12, histo -> GetMean(), histo -> GetEntries() / 10., 0.25};
    TF1* langaus = new TF1;
    LangausFit(&langaus, &(*histo), startValues);
    histo -> Write();
    
    calibCoeffDistr_mean_vsREGIONID.Fill(mapIt -> second -> GetMean());
    calibCoeff_mean_vsREGIONID.SetPoint(point, regionId, histo -> GetMean());
    calibCoeff_mean_vsREGIONID.SetPointError(point, 0., histo -> GetMeanError());
    
    calibCoeffDistr_MPV_vsREGIONID.Fill(langaus -> GetParameter(1));
    calibCoeff_MPV_vsREGIONID.SetPoint(point, regionId, langaus -> GetParameter(1));
    calibCoeff_MPV_vsREGIONID.SetPointError(point, 0., langaus -> GetParError(1));
    
    ++point;
  }


  calibCoeffDistr_mean_vsREGIONID.Write();
  calibCoeff_mean_vsREGIONID.Write("calibCoeff_mean_vsREGIONID");


  calibCoeffDistr_MPV_vsREGIONID.Write();
  calibCoeff_MPV_vsREGIONID.Write("calibCoeff_MPV_vsREGIONID");


  for(map_const_iterator_int mapIt = calib_muonAngleMap_vsREGIONID.begin();
      mapIt != calib_muonAngleMap_vsREGIONID.end() ; ++mapIt)
  {
    TH1D* histo = mapIt -> second;
    histo -> Write();
  }

  sd1 -> cd();




  sd2 = sd1 -> mkdir("vsPHI");
  sd2 -> cd();

  calib_nEvents_vsPHI.Write();
  calib_muonAngle_profile_vsPHI.Write();

  point = 0;
  for(map_const_iterator_float mapIt = calib_dEdXMap_vsPHI.begin();
      mapIt != calib_dEdXMap_vsPHI.end() ; ++mapIt)
  {
    float phiCenter = mapIt -> first;
    TH1D* histo = mapIt -> second;
    
    double startValues[4] = {0.12, histo -> GetMean(), histo -> GetEntries() / 10., 0.25};
    TF1* langaus = new TF1;
    LangausFit(&langaus, &(*histo), startValues);
    histo -> Write();

    calibCoeffDistr_mean_vsPHI.Fill(mapIt -> second -> GetMean());
    calibCoeff_mean_vsPHI.SetPoint(point, phiCenter, histo -> GetMean());
    calibCoeff_mean_vsPHI.SetPointError(point, 0., histo -> GetMeanError());
    
    calibCoeffDistr_MPV_vsPHI.Fill(langaus -> GetParameter(1));
    calibCoeff_MPV_vsPHI.SetPoint(point, phiCenter, langaus -> GetParameter(1));
    calibCoeff_MPV_vsPHI.SetPointError(point, 0., langaus -> GetParError(1));
    
    ++point;
  }


  calibCoeffDistr_mean_vsPHI.Write();
  calibCoeff_mean_vsPHI.Write("calibCoeff_mean_vsPHI");


  calibCoeffDistr_MPV_vsPHI.Write();
  calibCoeff_MPV_vsPHI.Write("calibCoeff_MPV_vsPHI");


  for(map_const_iterator_float mapIt = calib_muonAngleMap_vsPHI.begin();
      mapIt != calib_muonAngleMap_vsPHI.end() ; ++mapIt)
  {
    TH1D* histo = mapIt -> second;
    histo -> Write();
  }

  sd1 -> cd();



  sd2 = sd1 -> mkdir("vsETA");
  sd2 -> cd();

  calib_nEvents_vsETA.Write();
  calib_muonAngle_profile_vsETA.Write();

  point = 0;
  for(map_const_iterator_float mapIt = calib_dEdXMap_vsETA.begin();
      mapIt != calib_dEdXMap_vsETA.end() ; ++mapIt)
  {
    float etaCenter = mapIt -> first;
    TH1D* histo = mapIt -> second;
    
    double startValues[4] = {0.12, histo -> GetMean(), histo -> GetEntries() / 10., 0.25};
    TF1* langaus = new TF1;
    LangausFit(&langaus, &(*histo), startValues);
    histo -> Write();

    calibCoeffDistr_mean_vsETA.Fill(mapIt -> second -> GetMean());
    calibCoeff_mean_vsETA.SetPoint(point, etaCenter, histo -> GetMean());
    calibCoeff_mean_vsETA.SetPointError(point, 0., histo -> GetMeanError());
    
    calibCoeffDistr_MPV_vsETA.Fill(langaus -> GetParameter(1));
    calibCoeff_MPV_vsETA.SetPoint(point, etaCenter, langaus -> GetParameter(1));
    calibCoeff_MPV_vsETA.SetPointError(point, 0., langaus -> GetParError(1));
    
    ++point;
  }


  calibCoeffDistr_mean_vsETA.Write();
  calibCoeff_mean_vsETA.Write("calibCoeff_mean_vsETA");


  calibCoeffDistr_MPV_vsETA.Write();
  calibCoeff_MPV_vsETA.Write("calibCoeff_MPV_vsETA");


  for(map_const_iterator_float mapIt = calib_muonAngleMap_vsETA.begin();
      mapIt != calib_muonAngleMap_vsETA.end() ; ++mapIt)
  {
    TH1D* histo = mapIt -> second;
    histo -> Write();
  }

  sd1 -> cd();

  outRootFile.cd();


  
  outRootFile.Close();
  
  
  return 0;
}

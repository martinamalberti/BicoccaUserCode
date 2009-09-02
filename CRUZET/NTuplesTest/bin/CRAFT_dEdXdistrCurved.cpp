#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Calibration/Tools/interface/calibXMLwriter.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/CaloMiscalibTools.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/CaloMiscalibMapEcal.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/MiscalibReaderFromXMLEcalBarrel.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/MiscalibReaderFromXMLEcalEndcap.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondCore/Utilities/interface/CondIter.h"

#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "CRUZET/NTuplesTest/interface/Langaus.h"
#include "CRUZET/NTuplesTest/interface/NTuplesUtils.h"
#include "CRUZET/NTuplesTest/interface/ClusterCalibTools.h"

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
#include "TProfile2D.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TSystem.h"

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

#define DEDX_BIN 100000
#define DEDX_MIN 0.
#define DEDX_MAX 5000.

#define P_MIN 0
#define P_MAX 3

#define ANGLE_BIN 20
#define ANGLE_MIN 0.
#define ANGLE_MAX 1.

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
  bool correctAngle = subPSetSelections.getUntrackedParameter<bool>("correctAngle", true);
  bool correctZSSR = subPSetSelections.getUntrackedParameter<bool>("correctZSSR", true);
  bool applyVeto = subPSetSelections.getUntrackedParameter<bool>("applyVeto", false);
  double vetoEnergyMIN = subPSetSelections.getUntrackedParameter<double>("vetoEnergyMIN", 0.5);
  bool skipBadIC = subPSetSelections.getUntrackedParameter<bool>("skipBadIC", true);  

  bool muonLegUpOK = subPSetSelections.getUntrackedParameter<bool>("muonLegUpOK", false);
  bool muonLegDownOK = subPSetSelections.getUntrackedParameter<bool>("muonLegDownOK", true);

  double pol0_0 = subPSetSelections.getUntrackedParameter<double>("pol0_0", 1.79171);
  double pol1_0 = subPSetSelections.getUntrackedParameter<double>("pol1_0", 1.79171);
  double pol1_1 = subPSetSelections.getUntrackedParameter<double>("pol1_1", -0.52748);

  double ZSSR_fact = subPSetSelections.getUntrackedParameter<double>("ZSSR_fact",17.5);

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

  bool doCalibVsREGIONID = subPSetCalibration.getUntrackedParameter<bool>("doCalibVsREGIONID", false);
  bool doCalibVsETA = subPSetCalibration.getUntrackedParameter<bool>("doCalibVsETA", false);
  bool doCalibVsPHI = subPSetCalibration.getUntrackedParameter<bool>("doCalibVsPHI", false);

  std::vector<int> noCalibREGIONID = subPSetCalibration.getUntrackedParameter<std::vector<int> >("noCalibREGIONID", std::vector<int>(0));

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

  std::ofstream outFile("output.txt", std::ios::out);
  std::ofstream outFile2("output2.txt", std::ios::out);
  std::ofstream outFile3("printOuts.txt", std::ios::out);
  // Output file
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  

  int nTot = 0;


  TH1F events("events", "events", 100, 0., 100.);
  TH1F efficiencies("efficiencies", "efficiencies", 100, 0., 100.);

  TH1F nEvents_TOT("nEvents_TOT", "nEvents_TOT", P_BIN, P_MIN, P_MAX);
  BinLogX(nEvents_TOT);
  TH1F nEvents_CUT("nEvents_CUT", "nEvents_CUT", P_BIN, P_MIN, P_MAX);
  BinLogX(nEvents_CUT);
  TH1F nEvents_EFF("nEvents_EFF", "nEvents_EFF", P_BIN, P_MIN, P_MAX);
  BinLogX(nEvents_EFF);
  
  
  // dEodX histograms
  TH1D dEdXDistrCurved("dEdXDistrCurved", "dEodXCurved", DEDX_BIN, DEDX_MIN, DEDX_MAX);
  //TH1D dEdXDistrCurved_E3x3("dEdXDistrCurved_E3x3", "dEodXCurved_E3x3", DEDX_BIN, DEDX_MIN, DEDX_MAX);
  //TH1D dEdXDistrCurved_E5x5("dEdXDistrCurved_E5x5", "dEodXCurved_E5x5", DEDX_BIN, DEDX_MIN, DEDX_MAX);
  // dEodX histograms
  
  
  
  // Theoretical BetheBloch
  TGraph BetheBloch_th;
  DrawBetheBloch (&BetheBloch_th);
  // Theoretical BetheBloch
  
  
  
  
  // Experimental BetheBloch - curved tkLength
  TH2F BetheBloch_curved("BetheBloch_curved", "BetheBloch_curved", P_BIN, P_MIN, P_MAX, DEDX_BIN, DEDX_MIN, DEDX_MAX);
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

  //TH2F BetheBloch_E3x3_curved ("BetheBloch_E3x3_curved", "BetheBloch_E3x3_curved", P_BIN, P_MIN, P_MAX, DEDX_BIN, DEDX_MIN, DEDX_MAX);
  //BinLogX(BetheBloch_E3x3_curved);
  //TProfile BetheBloch_profile_E3x3_curved("BetheBloch_profile_E3x3_curved", "BetheBloch_profile_E3x3_curved", P_BIN, P_MIN, P_MAX);
  //BinLogX(BetheBloch_profile_E3x3_curved);
  //TGraphAsymmErrors BetheBloch_graph_E3x3_curved;
  //std::map<int,float> pMapCurved_E3x3; pMapCurved_E3x3.clear();
  
  //TH2F BetheBloch_E5x5_curved ("BetheBloch_E5x5_curved", "BetheBloch_E5x5_curved", P_BIN, P_MIN, P_MAX, DEDX_BIN, DEDX_MIN, DEDX_MAX);
  //BinLogX(BetheBloch_E5x5_curved);
  //TProfile BetheBloch_profile_E5x5_curved("BetheBloch_profile_E5x5_curved", "BetheBloch_profile_E5x5_curved", P_BIN, P_MIN, P_MAX);
  //BinLogX(BetheBloch_profile_E5x5_curved);
  //TGraphAsymmErrors BetheBloch_graph_E5x5_curved;
  //std::map<int,float> pMapCurved_E5x5; pMapCurved_E5x5.clear();

  TH2F BetheBloch_unc_vsAngle("BetheBloch_unc_vsAngle", "BetheBloch_unc_vsAngle", ANGLE_BIN, ANGLE_MIN, ANGLE_MAX, DEDX_BIN, DEDX_MIN, DEDX_MAX);
  TProfile BetheBloch_unc_profile_vsAngle("BetheBloch_unc_profile_vsAngle", "BetheBloch_unc_profile_vsAngle", ANGLE_BIN, ANGLE_MIN, ANGLE_MAX);

  TH2F BetheBloch_vsAngle("BetheBloch_vsAngle", "BetheBloch_vsAngle", ANGLE_BIN, ANGLE_MIN, ANGLE_MAX, DEDX_BIN, DEDX_MIN, DEDX_MAX);
  TProfile BetheBloch_profile_vsAngle("BetheBloch_profile_vsAngle", "BetheBloch_profile_vsAngle", ANGLE_BIN, ANGLE_MIN, ANGLE_MAX);
  // Experimental BetheBloch - curved tkLength
  
  
  
  
  // Muon histograms
  TProfile nMuons_overTh("nMuons_overTh", "nMuons_overTh", 100, -3, 2); 
  BinLogX(nMuons_overTh);

  TH2F muonOccupancyInt_ETAvsPHI("muonOccupancyInt_ETAvsPHI", "muonOccupancyInt_ETAvsPHI",
                                 PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  TH2F muonOccupancyExt_ETAvsPHI("muonOccupancyExt_ETAvsPHI", "muonOccupancyExt_ETAvsPHI",
                                 PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  
  TH1F muonPDistr("muonPDistr", "muonPDistr", 250, 0., 5.);
  //TH1F muonPDistr("muonPDistr", "muonPDistr", 10000, 0., 10000.);
  BinLogX(muonPDistr);
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

  TH2F muonVertex("muonVertex", "muonVertex", 1000, -800, 800, 1000, -800, 800);

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
  TH2F muonTkLengthCurved_vsP("muonTkLengthCurved_vsP", "muonTkLengthCurved_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 100.);
  BinLogX(muonTkLengthCurved_vsP);
  TProfile muonTkLengthCurved_profile_vsP("muonTkLengthCurved_profile_vsP", "muonTkLengthCurved_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(muonTkLengthCurved_profile_vsP);
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
  TH1F nSuperClustersDistr_noCut("nSuperClustersDistr_noCut", "nSuperClustersDistr_noCut", 100, 0., 100.);
  TH1F nSuperClustersDistr("nSuperClustersDistr", "nSuperClustersDistr", 100, 0., 100.);

  TH2F superClusterOccupancy_ETAvsPHI("superClusterOccupancy_ETAvsPHI", "superClusterOccupancy_ETAvsPHI",
                                      PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);
  
  TH1F superClusterRawEnergyDistr("superClusterRawEnergyDistr", "superClusterRawEnergyDistr", 250, -2., 3.);
  //TH1F superClusterRawEnergyDistr("superClusterRawEnergyDistr", "superClusterRawEnergyDistr", 10000, 0.01, 100.);
  BinLogX(superClusterRawEnergyDistr);
  TH2F superClusterRawEnergy_vsP("superClusterRawEnergy_vsP", "superClusterRawEnergy_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 100.);
  BinLogX(superClusterRawEnergy_vsP);
  TProfile superClusterRawEnergy_profile_vsP("superClusterRawEnergy_profile_vsP", "superClusterRawEnergy_profile_vsP", P_BIN, P_MIN, P_MAX);
  BinLogX(superClusterRawEnergy_profile_vsP);
  TH2F superClusterRawEnergy_vsAngle("superClusterRawEnergy_vsAngle", "superClusterRawEnergy_vsAngle", 1000, 0., PI/2., 1000, 0., 100.);  
  TProfile superClusterRawEnergy_profile_vsAngle("superClusterRawEnergy_profile_vsAngle", "superClusterRawEnergy_profile_vsAngle", 1000, 0., PI/2.);  

  TH1F EoPDistr("EoPDistr", "EoPDistr", 10000, 0., 10.);
  TH2F EoP_vsP("EoP_vsP", "EoP_vsP", P_BIN, P_MIN, P_MAX, 10000, 0., 10.);
  BinLogX(EoP_vsP);
  
  TH1F nClustersDistr_noCut("nClustersDistr_noCut", "nClustersDistr_noCut", 100, 0., 100.);
  TH1F nClustersPerSuperClusterDistr_noCut("nClustersPerSuperClusterDistr_noCut", "nClustersPerSuperClusterDistr_noCut", 100, 0., 100.);
  TH1F nClustersDistr("nClustersDistr", "nClustersDistr", 100, 0., 100.);
  TH1F nClustersPerSuperClusterDistr("nClustersPerSuperClusterDistr", "nClustersPerSuperClusterDistr", 100, 0., 100.);

  TH1F nClustersInSuperClusterDistr("nClustersInSuperClusterDistr", "nClustersInSuperClusterDistr", 10, 0., 10.);
  TH2F nClustersInSuperCluster_vsE("nClustersInSuperCluster_vsE", "nClustersInSuperCluster_vsE", 250, -2., 3., 10, 0., 10.);
  BinLogX(nClustersInSuperCluster_vsE);
  TH2F nClustersInSuperCluster_vsAngle("nClustersInSuperCluster_vsAngle", "nClustersInSuperCluster_vsAngle", 1000, 0., PI/2., 10, 0., 10.);

  TH1F nXtalsInSuperClusterDistr("nXtalsInSuperClusterDistr", "nXtalsInSuperClusterDistr", 100, 0., 100.);
  TH2F nXtalsInSuperCluster_vsE("nXtalsInSuperCluster_vsE", "nXtalsInSuperCluster_vsE", 250, -2, 3., 100, 0., 100.);
  BinLogX(nXtalsInSuperCluster_vsE);
  TH2F nXtalsInSuperCluster_vsAngle("nXtalsInSuperCluster_vsAngle", "nXtalsInSuperCluster_vsAngle", 1000, 0., PI/2., 100, 0., 100.);
  TProfile nXtalsInSuperCluster_profile_vsAngle("nXtalsInSuperCluster_profile_vsAngle", "nXtalsInSuperCluster_profile_vsAngle", 1000, 0., PI/2.);
  TProfile nXtalsInSuperCluster_profile_vsPHI("nXtalsInSuperCluster_profile_vsPHI", "nXtalsInSuperCluster_profile_vsPHI", PHI_BIN, PHI_MIN, PHI_MAX);
  TProfile nXtalsInSuperCluster_profile_vsETA("nXtalsInSuperCluster_profile_vsETA", "nXtalsInSuperCluster_profile_vsETA", ETA_BIN, ETA_MIN, ETA_MAX);
  TProfile2D nXtalsInSuperCluster_profile_ETAvsPHI("nXtalsInSuperCluster_profile_ETAvsPHI", "nXtalsInSuperCluster_profile_ETAvsPHI", PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX);

  TH1F nXtalsInSuperClusterOverCrossedXtalsCurvedDistr("nXtalsInSuperClusterOverCrossedXtalsCurvedDistr", "nXtalsInSuperClusterOverCrossedXtalsCurvedDistr", 1000, 0., 5.);
  // SuperCluster histograms
    
  
  
  
  // Association histograms
  TH1F assocDRDistr("assocDRDistr", "assocDRDistr", 1000, 0., 1.);
  TH2F assocDEta_vsDPhi("assocDEta_vsDPhi", "AssocDEta_vsDPhi", 1000, -1., 1., 1000, -1., 1.);
  
  TH2F assocDR_vsP("asscoDR_vsP", "asscoDR_vsP", P_BIN, P_MIN, P_MAX, 1000, 0., 1.); 
  BinLogX(assocDR_vsP);
  TProfile assocDR_profile_vsP("asscoDR_profile_vsP", "asscoDR_profile_vsP", P_BIN, P_MIN, P_MAX); 
  BinLogX(assocDR_profile_vsP);
  
  TH2F assocDR_vsAngle("asscoDR_vsAngle", "asscoDR_vsAngle", 1000, 0, PI/2., 1000, 0., 1.);
  TProfile assocDR_profile_vsAngle("asscoDR_profile_vsAngle", "asscoDR_profile_vsAngle", 1000, 0, PI/2);
  // Association histograms
  
  
  
  
  // Xtal histograms
  TH1F nXtalsDistr_noCut("nXtalsDistr_noCut", "nXtalsDistr_noCut", 100, 0., 100.);
  TH1F nXtalsPerSuperClusterDistr_noCut("nXtalsPerSuperClusterDistr_noCut", "nXtalsPerSuperClusterDistr_noCut", 100, 0., 100.);
  TH1F nXtalsPerClusterDistr_noCut("nXtalsPerClusterDistr_noCut", "nXtalsPerClusterDistr_noCut", 100, 0., 100.);

  TH1F nXtalsDistr("nXtalsDistr", "nXtalsDistr", 100, 0., 100.);
  TH1F nXtalsPerSuperClusterDistr("nXtalsPerSuperClusterDistr", "nXtalsPerSuperClusterDistr", 100, 0., 100.);
  TH1F nXtalsPerClusterDistr("nXtalsPerClusterDistr", "nXtalsPerClusterDistr", 100, 0., 100.);

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

  TH1F xtalEnergyMaxDistr("xtalEnergyMaxDistr", "xtalEnergyMaxDistr", 10000, 0., 10.);
  TH1F xtalTkLengthCurvedMaxDistr("xtalTkLengthCurvedDistrMax", "xtalTkLengthCurvedDistrMax", 10000, 0., 100.);
  TH2F xtalTkLengthCurvedMax_vsAngle("xtalTkLengthCurvedMax_vsAngle", "xtalTkLengthCurvedMax_vsAngle", 1000, 0., PI/2., 1000, 0., 100.);
  TProfile xtalTkLengthCurvedMax_profile_vsAngle("xtalTkLengthCurvedMax_profileCurvedMax_vsAngle", "xtalTkLength_profile_vsAngle", 1000, 0., PI/2.);
  // Xtal histograms
  
  
  
  
  // Calibration  
  EBregionBuilder region(etaSTART, etaEND, etaWIDTH, phiSTART, phiEND, phiWIDTH);

  TH2F regionOccupancy_ETAvsPHI("regionOccupancy_ETAvsPHI", "",
                                phiN, phiSTART, phiEND, etaN, region.etaShifter(etaSTART), region.etaShifter(etaEND));

  std::map<int, TH1D*>   calib_dEdXMap_vsREGIONID;
  std::map<float, TH1D*> calib_dEdXMap_vsPHI;
  std::map<float, TH1D*> calib_dEdXMap_vsETA;
  std::map<int, TH1D*>   calib_muonAngleMap_vsREGIONID;
  std::map<float, TH1D*> calib_muonAngleMap_vsPHI;
  std::map<float, TH1D*> calib_muonAngleMap_vsETA;
  std::map<int, TH1D*>   calib_muonPMap_vsREGIONID;
  std::map<float, TH1D*>   calib_muonPMap_vsPHI;
  std::map<float, TH1D*>   calib_muonPMap_vsETA;

  TH1F calib_nEvents_vsREGIONID("calib_nEvents_vsREGIONID", "nEvents_vsREGIONID", phiN*etaN, 0., 1.*phiN*etaN);
  TH1F calib_nEvents_vsPHI("calib_nEvents_vsPHI", "nEvents_vsPHI", phiN, IPHI_MIN, IPHI_MAX);
  TH1F calib_nEvents_vsETA("calib_nEvents_vsETA", "nEvents_vsETA", etaN, IETA_MIN, IETA_MAX);

  TProfile calib_muonAngle_profile_vsREGIONID("calib_muonAngle_profile_vsREGIONID", "calib_muonAngle_profile_vsREGIONID", phiN*etaN, 0., 1.*phiN*etaN);
  TProfile calib_muonAngle_profile_vsPHI("calib_muonAngle_profile_vsPHI", "calib_muonAngle_profile_vsPHI", phiN, IPHI_MIN, IPHI_MAX);
  TProfile calib_muonAngle_profile_vsETA("calib_muonAngle_profile_vsETA", "calib_muonAngle_profile_vsETA", etaN, IETA_MIN, IETA_MAX);

  TProfile calib_muonP_profile_vsREGIONID("calib_muonP_profile_vsREGIONID", "calib_muonP_profile_vsREGIONID", phiN*etaN, 0., 1.*phiN*etaN);
  TProfile calib_muonP_profile_vsPHI("calib_muonP_profile_vsPHI", "calib_muonP_profile_vsPHI", phiN*etaN, 0., 1.*phiN*etaN);
  TProfile calib_muonP_profile_vsETA("calib_muonP_profile_vsETA", "calib_muonP_profile_vsETA", phiN*etaN, 0., 1.*phiN*etaN);

  TH1F calib_dEdXDistrAngle_vsREGIONID("calib_dEdXDistrAngle_vsREGIONID", "dEdXDistrAngle_vsREGIONID", phiN*etaN, 0., 1.*phiN*etaN);
  TH1F calib_dEdXDistrAngle_vsPHI("calib_dEdXDistrAngle_vsPHI", "dEdXDistrAngle_vsPHI", phiN, IPHI_MIN, IPHI_MAX);
  TH1F calib_dEdXDistrAngle_vsETA("calib_dEdXDistrAngle_vsETA", "dEdXDistrAngle_vsETA", etaN, IETA_MIN, IETA_MAX);

  TH1F calib_dEdXDistrP_vsREGIONID("calib_dEdXDistrP_vsREGIONID", "dEdXDistrP_vsREGIONID", phiN*etaN, 0., 1.*phiN*etaN);
  TH1F calib_dEdXDistrP_vsPHI("calib_dEdXDistrP_vsPHI", "dEdXDistrP_vsPHI", phiN, IPHI_MIN, IPHI_MAX);
  TH1F calib_dEdXDistrP_vsETA("calib_dEdXDistrP_vsETA", "dEdXDistrP_vsETA", etaN, IETA_MIN, IETA_MAX);

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
  
  


  
  // Database
  std::string NameDBOracle2 = "oracle://cms_orcoff_prod/CMS_COND_21X_ECAL";   //GAIN 200
  std::string TagDBOracle2 = "EcalIntercalibConstants_AllCruzet_EEB0v2_offline";  
  std::string Command2LineStr2 = "cmscond_export_iov -s " + NameDBOracle2 + " -d sqlite_file:/tmp/abenagli/Due.db -D CondFormatsEcalObjects -t " + TagDBOracle2 + " -P /afs/cern.ch/cms/DB/conddb/";

  std::cout << Command2LineStr2 << std::endl;
  gSystem->Exec(Command2LineStr2.c_str());
  
  std::string NameDB;
  std::string FileData;
  
  
  //-----------------------------------
  //---- Second Database Analyzed -----
  //-----------------------------------
  NameDB = "sqlite_file:/tmp/abenagli/Due.db";  
  FileData = TagDBOracle2;
  CondIter<EcalIntercalibConstants> Iterator2;
  Iterator2.create(NameDB,FileData);

  
  //-------------------------------------------------
  //---- Ottengo Mappe da entrambi gli Iterators ----
  //-------------------------------------------------

  const EcalIntercalibConstants* EBconstants2;
  EBconstants2 = Iterator2.next();
  EcalIntercalibConstantMap iEBcalibMap_2 = EBconstants2->getMap () ;
  








  
  
 
  // --------------------------------------------------------------------------------------
  // Loop over entries
  // --------------------------------------------------------------------------------------
  
  
  // Loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    //    if (entry == 909735) continue;
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    //if (entry > 900000)
    //  std::cout << "Reading entry " << entry << std::endl;
    chain -> GetEntry (entry);
    if(entry == maxEvent) break;
    
    
    
    
    
    
    events.SetBinContent(1, events.GetBinContent(1) + 1);
    
    
    
    
    
    
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

    if( (nRecoMuons != 2) ||
        ( (nRecoMuons == 2) && (nRecoMuons_up > 1) ) ||
        ( (nRecoMuons == 2) && (nRecoMuons_down > 1) ) ) continue;

    events.SetBinContent(2, events.GetBinContent(2) + 1);
    // Cut on number of reconstructed muons
    
    
    
    nSuperClustersDistr_noCut.Fill(treeVars.nSuperClusters);
    
    int nClusters_noCut = 0;
    int nXtals_noCut = 0;
    for(int SCindex = 0; SCindex < treeVars.nSuperClusters; ++SCindex)
    {
      nClusters_noCut += treeVars.nClustersInSuperCluster[SCindex];
      nClustersPerSuperClusterDistr_noCut.Fill(treeVars.nClustersInSuperCluster[SCindex]);

      nXtals_noCut += treeVars.nXtalsInSuperCluster[SCindex];
      nXtalsPerSuperClusterDistr_noCut.Fill(treeVars.nXtalsInSuperCluster[SCindex]);
    }

    nClustersDistr_noCut.Fill(nClusters_noCut);
    nXtalsDistr_noCut.Fill(nXtals_noCut);



    // Cut on number of supercluster
    if(treeVars.nSuperClusters < 2) continue;
    
    events.SetBinContent(3, events.GetBinContent(3) + 1);     
    // Cut on number of supercluster





    // Sort association vector: first leg 1, then leg -1
    if(associations.size() != 2) continue;
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
    
    events.SetBinContent(4, events.GetBinContent(4) + 1);
    // Sort association vector: first leg 1, then leg -1




    // Alessio's cut                                                                                                   
    bool upFound = false;
    bool downFound = false;

    float ESC_up = 0.;
    float ESC_down = 0.;
    // Alessio's cut                                                                                                   
    
    
    
    // global number of xtals / clusters / superClusters 
    int nSuperClusters = 0;
    int nClusters = 0;
    int nXtals = 0;



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
      
      nSuperClusters += 1;
      nClusters += nClustersInSuperCluster;
      nXtals += nXtalsInSuperCluster;      

      float superClusterRawEnergy = treeVars.superClusterRawEnergy[SCindex] * energyCorrFactor;
      float clusterE3x3 =                      treeVars.clusterE3x3[Cindex] * energyCorrFactor;
      float clusterE5x5 =                      treeVars.clusterE5x5[Cindex] * energyCorrFactor;
      float xtalTkLengthCurvedSum = 0.;

      bool badTTflags = false;
      bool badICflags = false;
      for(int XTLit = xtalIndexInSuperCluster; XTLit < xtalIndexInSuperCluster + nXtalsInSuperCluster; ++XTLit)
      {
        if(treeVars.xtalTkLengthCurved[XTLit] > 0.) xtalTkLengthCurvedSum += treeVars.xtalTkLengthCurved[XTLit];

  
        // skip bad IC 
	EBDetId dummyDetId = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLit]) ;
	double coeff_2 = *(iEBcalibMap_2.find (dummyDetId.rawId ()));
        if(coeff_2 > 2.2) badICflags = true;       
        if(coeff_2 < 0.4) badICflags = true;       


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
	//qui sopra scommentato
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
	//qui sopra scommentato
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
      float dEdXCurved = (superClusterRawEnergy) / muonTkLengthInEcalCurved / 8.28 * 1000.;
      //float dEdXCurved_E3x3 =      clusterE3x3 / muonTkLengthInEcalCurved / 8.28 * 1000.;
      //float dEdXCurved_E5x5 =      clusterE5x5 / muonTkLengthInEcalCurved / 8.28 * 1000.;
      
      
//       float pol0_0 =  1.79171;
//       float pol1_0 =  1.87126;
//       float pol1_1 = -0.46092;
//       float ZSSR_factor = 17.5;
      if(correctAngle == true)
      {
        if(muonAngle > 0.1)  // era 0.2
        {
	  //	  dEdXCurved += - (muonAngle - 0.2)*pol1_1;
	  dEdXCurved += - (muonAngle - 0.1)*pol1_1;
	  //	  dEdXCurved += - (muonAngle * pol1_1);
        }
      }
      

      if(correctZSSR == true)
      {
        dEdXCurved = (dEdXCurved * muonTkLengthInEcalCurved * 8.28 / 0.97 - ZSSR_fact) / muonTkLengthInEcalCurved / 8.28 * 0.97;
      }
      // dEdX variables


      
      
      
      
      // --------------------------------------------------------------------------------------
      // Cut event
      // --------------------------------------------------------------------------------------
      
      nEvents_TOT.Fill(muonP);
      
      // keep only leg = -1 events
      if ( (muonLegUpOK == true && muonLegDownOK == false && muonLeg != 1) ||
           (muonLegDownOK == true && muonLegUpOK == false && muonLeg != -1) ) continue;
      events.SetBinContent(5, events.GetBinContent(5) + 1);
      
      
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

      if( (badICflags == true) && (skipBadIC == true) ) continue;

      events.SetBinContent(6, events.GetBinContent(6) + 1);
      // Skip bad xtals
       
      
      // Other cuts
      if ( (muonP < muonPMIN) || (muonP > muonPMAX) ) continue ;
      events.SetBinContent(7, events.GetBinContent(7) + 1);
      if ( muonQOverPError != muonQOverPError) continue;
      events.SetBinContent(8, events.GetBinContent(8) + 1);
      if ( (1. * muonPError / muonP) > muonPErrorOverPMAX) continue ;
      events.SetBinContent(9, events.GetBinContent(9) + 1);
      if (fabs(muond0) > muond0MAX) continue;
      events.SetBinContent(10, events.GetBinContent(10) + 1);
      if (fabs(muondz) > muondzMAX) continue;
      events.SetBinContent(11, events.GetBinContent(11) + 1);
      if (muondR > muondRMAX) continue;
      events.SetBinContent(12, events.GetBinContent(12) + 1);
      if (muonNChi2 > muonNChi2MAX) continue;
      events.SetBinContent(13, events.GetBinContent(13) + 1);
      if (muonNHits < muonNHitsMIN) continue;
      events.SetBinContent(14, events.GetBinContent(14) + 1);
      if ( (muonTkLengthInEcalCurved < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcalCurved > muonTkLengthInEcalMAX) ) continue;
      events.SetBinContent(15, events.GetBinContent(15) + 1);
      if (xtalTkLengthCurvedSum/muonTkLengthInEcalCurved < xtalTkLengthSumMIN) continue;
      events.SetBinContent(16, events.GetBinContent(16) + 1);
      if (xtalTkLengthCurvedSum/muonTkLengthInEcalCurved > xtalTkLengthSumMAX) continue;
      events.SetBinContent(17, events.GetBinContent(17) + 1);
      if (muonEOverP > muonEOverPMAX) continue;
      events.SetBinContent(18, events.GetBinContent(18) + 1);
      if (muonAngle > muonAngleMAX) continue;
      events.SetBinContent(19, events.GetBinContent(19) + 1);
      if (muonAngle < muonAngleMIN) continue;
      events.SetBinContent(20, events.GetBinContent(20) + 1);

      if (1.*nXtalsInSuperCluster/nMuonCrossedXtalsCurved < nXtalsInSuperClusterMIN) continue;
      events.SetBinContent(21, events.GetBinContent(21) + 1);
      if (1.*nXtalsInSuperCluster/nMuonCrossedXtalsCurved > nXtalsInSuperClusterMAX) continue;
      events.SetBinContent(22, events.GetBinContent(22) + 1);  

      if ( fabs(muonInnTkInnerPoint.z()) < muonInnTkHitZMIN) continue;
      if ( fabs(muonInnTkInnerPoint.z()) > muonInnTkHitZMAX) continue;
      if ( fabs(muonInnTkOuterPoint.z()) < muonInnTkHitZMIN) continue;
      if ( fabs(muonInnTkOuterPoint.z()) > muonInnTkHitZMAX) continue;
      events.SetBinContent(23, events.GetBinContent(23) + 1);

      if ( (superClusterPhi < superClusterPhiMIN) ||
           (superClusterPhi > superClusterPhiMAX) ) continue;
      if ( (superClusterEta < superClusterEtaMIN) ||
           (superClusterEta > superClusterEtaMAX) ) continue;
      events.SetBinContent(24, events.GetBinContent(24) + 1);

      if( ( applyVeto == true) && (muonP > muonPCollTHRESH) && (muonLeg ==  1) ) continue;
      if( ( applyVeto == true) && (muonP > muonPCollTHRESH) && (muonLeg == -1) &&
          (upFound == true) && (ESC_up > vetoEnergyMIN) ) continue;
      events.SetBinContent(25, events.GetBinContent(25) + 1);

      // Other cuts
      
      nEvents_CUT.Fill(muonP);
      
      
      
      
      
      
      // --------------------------------------------------------------------------------------
      // Fill distributions
      // --------------------------------------------------------------------------------------

      ++nTot;
      outFile2 << "entry " << entry << "   MUindex " << MUindex << "   SCindex " << SCindex << std::endl;
      if(muonEOverP > 1.)
        outFile3 << "runId = " << treeVars.runId << "   eventIt = " << treeVars.eventId
                 << "muonEOverP = " << muonEOverP << "   SCEta = " << superClusterEta << "   SCPhi = " << superClusterPhi << std::endl;
       
      
      
      float trueVal = 1.;
      if(useTrueVal == true)
        trueVal = FindBetheBlochValue(&BetheBloch_th, muonP);





      // dEdX histograms
      dEdXDistrCurved.Fill(dEdXCurved / trueVal);
      if (nClustersInSuperCluster == 1)
      {
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

      if(muonP >= 5. && muonP <= 10.)
      {
        BetheBloch_unc_vsAngle.Fill(muonAngle, superClusterRawEnergy / muonTkLengthInEcalCurved / 8.28 * 1000. / trueVal);
        BetheBloch_unc_profile_vsAngle.Fill(muonAngle, superClusterRawEnergy / muonTkLengthInEcalCurved / 8.28 * 1000. / trueVal);

        BetheBloch_vsAngle.Fill(muonAngle, dEdXCurved/trueVal);
        BetheBloch_profile_vsAngle.Fill(muonAngle, dEdXCurved/trueVal);
      }
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
      if( (pBin > 0) && (pBin <= P_BIN) )
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

      muonVertex.Fill(treeVars.muonX[MUindex], treeVars.muonY[MUindex]);
 
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
      muonTkLengthCurved_vsP.Fill(muonP, muonTkLengthInEcalCurved);
      muonTkLengthCurved_profile_vsP.Fill(muonP, muonTkLengthInEcalCurved);
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
      nXtalsInSuperCluster_vsE.Fill(superClusterRawEnergy, nXtalsInSuperCluster);
      nXtalsInSuperCluster_vsAngle.Fill(muonAngle, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_vsAngle.Fill(muonAngle, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_vsPHI.Fill(muonPhi, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_vsETA.Fill(muonEta, nXtalsInSuperCluster);
      nXtalsInSuperCluster_profile_ETAvsPHI.Fill(muonPhi, muonEta, nXtalsInSuperCluster);

      nXtalsInSuperClusterOverCrossedXtalsCurvedDistr.Fill(1.*nXtalsInSuperCluster/nMuonCrossedXtalsCurved);

      nClustersInSuperClusterDistr.Fill(nClustersInSuperCluster);
      nClustersInSuperCluster_vsAngle.Fill(muonAngle, nClustersInSuperCluster);
      nClustersInSuperCluster_vsE.Fill(superClusterRawEnergy, nClustersInSuperCluster);
      // SuperCluster histograms
      
      
      
      
      // Association histograms
      float assocDEta = superClusterEta - muonEta;
      float assocDPhi = superClusterPhi - muonPhi;
      float assocDR = sqrt(pow(assocDEta, 2) + pow(assocDPhi, 2));
      
      assocDRDistr.Fill(assocDR);
      assocDEta_vsDPhi.Fill(assocDPhi, assocDEta);
      
      assocDR_vsP.Fill(muonP, assocDR);
      assocDR_profile_vsP.Fill(muonP, assocDR);
      
      assocDR_vsAngle.Fill(muonAngle, assocDR);
      assocDR_profile_vsAngle.Fill(muonAngle, assocDR);
      // Association histograms
      
      
      
      
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

      xtalEnergyMaxDistr.Fill(treeVars.xtalEnergy[XTLMAXCurvedindex]);
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
      if(doCalibVsREGIONID == true)
      {
        map_iterator_int mapIt = calib_dEdXMap_vsREGIONID.find(regionId);
        if( (mapIt == calib_dEdXMap_vsREGIONID.end()) && (regionId != -1) &&
            ( (regionId ==  3) || (regionId ==  4) || (regionId ==  5) || (regionId ==  6) || 
              (regionId == 12) || (regionId == 13) || (regionId == 14) || (regionId == 15) ||
              (regionId == 21) || (regionId == 22) || (regionId == 23) || (regionId == 24) ||
              (regionId == 30) || (regionId == 31) || (regionId == 32) || (regionId == 33) ) )
        {
          char histoName[100];
          sprintf(histoName, "calib_dEdX_vsREGIONID_region_%05d---phi_%d_%d---eta_%d_%d",
		  regionId,
		  phiSTART  + phiWIDTH * (regionId%phiN),
		  phiSTART  + phiWIDTH * (regionId%phiN + 1),
		  etaSTART  + etaWIDTH * (regionId/phiN),
		  etaSTART  + etaWIDTH * (regionId/phiN + 1));
          
          calib_dEdXMap_vsREGIONID[regionId] = new TH1D(histoName, histoName, DEDX_BIN, DEDX_MIN, DEDX_MAX);
          
          
          sprintf(histoName, "calib_muonAngle_vsREGIONID_region_%05d---phi_%d_%d---eta_%d_%d",
		  regionId,
		  phiSTART  + phiWIDTH * (regionId%phiN),
		  phiSTART  + phiWIDTH * (regionId%phiN + 1),
		  etaSTART  + etaWIDTH * (regionId/phiN),
		  etaSTART  + etaWIDTH * (regionId/phiN + 1));
          
          calib_muonAngleMap_vsREGIONID[regionId] = new TH1D(histoName, histoName, 1000, 0., PI/2.);
          
          
          sprintf(histoName, "calib_muonP_vsREGIONID_region_%05d---phi_%d_%d---eta_%d_%d",
		 regionId,
	         phiSTART  + phiWIDTH * (regionId%phiN),
		 phiSTART  + phiWIDTH * (regionId%phiN + 1),
		 etaSTART  + etaWIDTH * (regionId/phiN),
		 etaSTART  + etaWIDTH * (regionId/phiN + 1));
          
          calib_muonPMap_vsREGIONID[regionId] = new TH1D(histoName, histoName, 2000, 0., 20.);
        }
      }
      
      

      if(doCalibVsPHI == true)
      {
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
          
          
          sprintf(histoName, "calib_muonP_vsPHI_region_%05d---phi_%d_%d---eta_%d_%d",
		 regionId,
	         phiSTART  + phiWIDTH * (regionId%phiN),
		 phiSTART  + phiWIDTH * (regionId%phiN + 1),
		 etaSTART  + etaWIDTH * (regionId/phiN),
		 etaSTART  + etaWIDTH * (regionId/phiN + 1));
          
          calib_muonPMap_vsPHI[regionId] = new TH1D(histoName, histoName, 2000, 0., 20.);
        }
      }



      if(doCalibVsETA == true)
      {
        map_iterator_float mapIt_float = calib_dEdXMap_vsETA.find(etaCenter);
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
          
          
          sprintf(histoName, "calib_muonP_vsETA_region_%05d---phi_%d_%d---eta_%d_%d",
		 regionId,
	         phiSTART  + phiWIDTH * (regionId%phiN),
		 phiSTART  + phiWIDTH * (regionId%phiN + 1),
		 etaSTART  + etaWIDTH * (regionId/phiN),
		 etaSTART  + etaWIDTH * (regionId/phiN + 1));
          
          calib_muonPMap_vsETA[regionId] = new TH1D(histoName, histoName, 2000, 0., 20.);
        }
      }





      bool doCalib = true;
      if(regionId == -1) doCalib = false;
      for(unsigned int ii = 0; ii < noCalibREGIONID.size(); ++ii)
      {
        if(regionId == noCalibREGIONID.at(ii))
          doCalib = false;
      }

      if(doCalib == true)
      {
        regionOccupancy_ETAvsPHI.Fill(seedXtalIphi, region.etaShifter(seedXtalIeta));

        if(doCalibVsREGIONID == true)
        {
          calib_nEvents_vsREGIONID.Fill(regionId);

          calib_muonAngleMap_vsREGIONID[regionId] -> Fill(muonAngle);
          calib_muonAngle_profile_vsREGIONID.Fill(regionId, muonAngle);
          float dEdX;
          if(muonAngle <= 0.2) dEdX = pol0_0;
          else dEdX = pol1_0 + muonAngle * pol1_1;
          calib_dEdXDistrAngle_vsREGIONID.Fill(dEdX);

          calib_muonPMap_vsREGIONID[regionId] -> Fill(muonP);
          calib_muonP_profile_vsREGIONID.Fill(regionId, muonP);
          calib_dEdXDistrP_vsREGIONID.Fill(FindBetheBlochValue(&BetheBloch_th, muonP));

          calib_dEdXMap_vsREGIONID[regionId] -> Fill(dEdXCurved / trueVal);
	}

        if(doCalibVsPHI == true)
        {
          calib_nEvents_vsPHI.Fill(phiCenter);

          calib_muonAngleMap_vsPHI[phiCenter] -> Fill(muonAngle);
          calib_muonAngle_profile_vsPHI.Fill(phiCenter, muonAngle);
          float dEdX;
          if(muonAngle <= 0.2) dEdX = pol0_0;
          else dEdX = pol1_0 + muonAngle * pol1_1;
          calib_dEdXDistrAngle_vsPHI.Fill(dEdX);

          calib_muonPMap_vsPHI[phiCenter] -> Fill(muonP);
          calib_muonP_profile_vsPHI.Fill(phiCenter, muonP);
          calib_dEdXDistrP_vsPHI.Fill(FindBetheBlochValue(&BetheBloch_th, muonP));

          calib_dEdXMap_vsPHI[phiCenter] -> Fill(dEdXCurved / trueVal);
	}

        if(doCalibVsETA == true)
        {
          calib_nEvents_vsETA.Fill(etaCenter);

          calib_muonAngleMap_vsETA[etaCenter] -> Fill(muonAngle);
          calib_muonAngle_profile_vsETA.Fill(etaCenter, muonAngle);
          float dEdX;
          if(muonAngle <= 0.2) dEdX = pol0_0;
          else dEdX = pol1_0 + muonAngle * pol1_1;
          calib_dEdXDistrAngle_vsETA.Fill(dEdX);

          calib_muonPMap_vsETA[etaCenter] -> Fill(muonP);
          calib_muonP_profile_vsETA.Fill(etaCenter, muonP);
          calib_dEdXDistrP_vsETA.Fill(FindBetheBlochValue(&BetheBloch_th, muonP));

          calib_dEdXMap_vsETA[etaCenter] -> Fill(dEdXCurved / trueVal);
	}
      }


      
    } // Loop on associations vector
    
    nSuperClustersDistr.Fill(nSuperClusters);
    nClustersDistr.Fill(nClusters);
    nXtalsDistr.Fill(nXtals);
    
  } // Loop over entries
  
  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  events.Write();
  for(int bin = 1; bin <= events.GetNbinsX(); ++bin)
    efficiencies.SetBinContent(bin, 1. * events.GetBinContent(bin) / events.GetBinContent(1));
  efficiencies.Write();

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
  
  double startValues[4] = {0.12, 1.5, dEdXDistrCurved.GetEntries() / 10., 0.25};
  TF1* langaus = new TF1;
  LangausFit(&langaus, &dEdXDistrCurved, startValues, 0., 100.);
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
  
  
  
  outFile << "Tot events = " << nTot << std::endl;

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
    //LangausFit(&langaus, histo, startValues, 0., 1000.);
    
    //if (pMapCurved[bin] > 0)
    //{
    //  BetheBloch_graph_MPV_curved.SetPoint(bin-1, pMapCurved[bin], langaus -> GetParameter(1));
    //  BetheBloch_graph_MPV_curved.SetPointError(bin-1, 0., langaus -> GetParError(1));
    //  BetheBloch_mean_curved[bin] = langaus -> Mean(0., 100.);    
    //  BetheBloch_graph_GSigma_curved.SetPoint(bin-1, pMapCurved[bin], langaus -> GetParameter(3));
    //  BetheBloch_graph_GSigma_curved.SetPointError(bin-1, 0., langaus -> GetParError(3));
    //}
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
    


    outFile << "Bin " << std::fixed << std::setprecision(2) << std::setw(3) << bin 
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

  muonVertex.Write();

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
  muonTkLengthCurved_vsP.Write();
  muonTkLengthCurved_profile_vsP.Write();
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

  nSuperClustersDistr_noCut.Write();
  nClustersDistr_noCut.Write();
  nClustersPerSuperClusterDistr_noCut.Write();

  nSuperClustersDistr.Write();
  nClustersDistr.Write();

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
  nXtalsInSuperCluster_vsE.Write();
  nXtalsInSuperCluster_vsAngle.Write();
  nXtalsInSuperCluster_profile_vsAngle.Write();
  nXtalsInSuperCluster_profile_vsPHI.Write();
  nXtalsInSuperCluster_profile_vsETA.Write();
  nXtalsInSuperCluster_profile_ETAvsPHI.Write();  

  nXtalsInSuperClusterOverCrossedXtalsCurvedDistr.Write();


  nClustersInSuperClusterDistr.Write();
  nClustersInSuperCluster_vsAngle.Write();
  nClustersInSuperCluster_vsE.Write();

  outRootFile.cd ();
  // SuperCluster histograms
  
  
  
  
  
  
  // Association histograms
  outRootFile.mkdir("Association");
  outRootFile.cd("Association");
  std::cout << ">>> Saving Association histograms" << std::endl;

  assocDRDistr.Write();
  assocDEta_vsDPhi.Write();
  
  assocDR_vsP.Write();
  assocDR_profile_vsP.Write();
  
  assocDR_vsAngle.Write();
  assocDR_profile_vsAngle.Write();
  
  outRootFile.cd ();
  // Association histograms  
  
  
  
  
  
  
  // Xtal histograms
  outRootFile.mkdir("Xtal");
  outRootFile.cd("Xtal");
  std::cout << ">>> Saving Xtal histograms" << std::endl;
  
  nXtalsDistr_noCut.Write();
  nXtalsPerSuperClusterDistr_noCut.Write();

  nXtalsDistr.Write();

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
  
  xtalEnergyMaxDistr.Write();
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

  TF1* Pol0 = new TF1("Pol0", "[0]", 0., 0.1);
  TF1* Pol1 = new TF1("Pol1", "[0] + [1]*(x-0.1) ", 0.1, 1.);
  Pol0 -> SetLineWidth(2);
  Pol0 -> SetLineColor(kRed);
  Pol1 -> SetLineWidth(2);
  Pol1 -> SetLineColor(kBlue);

  BetheBloch_unc_profile_vsAngle.Fit("Pol0", "+QR");
  Pol1->FixParameter(0, Pol0->GetParameter(0));
  BetheBloch_unc_profile_vsAngle.Fit("Pol1", "+QR");

  BetheBloch_unc_vsAngle.Write();  
  BetheBloch_unc_profile_vsAngle.Write();  

  BetheBloch_vsAngle.Write();  
  BetheBloch_profile_vsAngle.Write();  
  
  outFile << "****** Fit of <dE/dx> vs Angle ******" << std::endl;
  outFile << "Plateau:   y = [0]              with      [0] = " << std::fixed << std::setprecision(5) << Pol0 -> GetParameter(0) 
          << " +/- " << Pol0 -> GetParError(0) << std::endl; 
  outFile << "Trend:     y = [0] + [1]*x      with      [0] = " << std::fixed << std::setprecision(5) << Pol1 -> GetParameter(0) 
          << " +/- " << std::fixed << std::setprecision(5) << Pol1 -> GetParError(0)  
          << "      [1] = " << std::fixed << std::setprecision(5) << Pol1 -> GetParameter(1)   
          << " +/- " << std::fixed << std::setprecision(5) << Pol1 -> GetParError(1)
          << std::endl;  




  TDirectory* sd1 = outRootFile.mkdir("Calibration");
  sd1 -> cd();
  std::cout << ">>> Saving Calibration histograms" << std::endl;

  regionOccupancy_ETAvsPHI.Write();



  TDirectory* sd2;
  if(doCalibVsREGIONID == true)
  {
    sd2 = sd1 -> mkdir("vsREGIONID");
    sd2 -> cd();
    
    calib_nEvents_vsREGIONID.Write();

    calib_muonAngle_profile_vsREGIONID.Write();
    calib_dEdXDistrAngle_vsREGIONID.Write();

    calib_muonP_profile_vsREGIONID.Write();
    calib_dEdXDistrP_vsREGIONID.Write();    

    int point = 0;
    for(map_const_iterator_int mapIt = calib_dEdXMap_vsREGIONID.begin();
        mapIt != calib_dEdXMap_vsREGIONID.end() ; ++mapIt)
    {
      int regionId = mapIt -> first;
      TH1D* histo = mapIt -> second;
      
      double startValues[4] = {0.12, histo -> GetMean(), histo -> GetEntries() / 10., 0.25};
      TF1* langaus = new TF1;
      LangausFit(&langaus, &(*histo), startValues, 0., 50.);
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
    
    for(map_const_iterator_int mapIt = calib_muonPMap_vsREGIONID.begin();
        mapIt != calib_muonPMap_vsREGIONID.end() ; ++mapIt)
    {
      TH1D* histo = mapIt -> second;
      histo -> Write();
    }
    
    sd1 -> cd();
  }



  if(doCalibVsPHI == true)
  {
    sd2 = sd1 -> mkdir("vsPHI");
    sd2 -> cd();
    
    calib_nEvents_vsPHI.Write();

    calib_muonAngle_profile_vsPHI.Write();
    calib_dEdXDistrAngle_vsPHI.Write();

    calib_muonP_profile_vsPHI.Write();
    calib_dEdXDistrP_vsPHI.Write();    

    int point = 0;
    for(map_const_iterator_float mapIt = calib_dEdXMap_vsPHI.begin();
        mapIt != calib_dEdXMap_vsPHI.end() ; ++mapIt)
    {
      float phiCenter = mapIt -> first;
      TH1D* histo = mapIt -> second;
      
      double startValues[4] = {0.12, histo -> GetMean(), histo -> GetEntries() / 10., 0.25};
      TF1* langaus = new TF1;
      LangausFit(&langaus, &(*histo), startValues, 0., 50.);
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

    for(map_const_iterator_float mapIt = calib_muonPMap_vsPHI.begin();
        mapIt != calib_muonPMap_vsPHI.end() ; ++mapIt)
    {
      TH1D* histo = mapIt -> second;
      histo -> Write();
    }
    
    sd1 -> cd();
  }
  
  
  
  if(doCalibVsETA == true)
  {
    sd2 = sd1 -> mkdir("vsETA");
    sd2 -> cd();
    
    calib_nEvents_vsETA.Write();

    calib_muonAngle_profile_vsETA.Write();
    calib_dEdXDistrAngle_vsETA.Write();

    calib_muonP_profile_vsETA.Write();
    calib_dEdXDistrP_vsETA.Write();    

    int point = 0;
    for(map_const_iterator_float mapIt = calib_dEdXMap_vsETA.begin();
        mapIt != calib_dEdXMap_vsETA.end() ; ++mapIt)
    {
      float etaCenter = mapIt -> first;
      TH1D* histo = mapIt -> second;
      
      double startValues[4] = {0.12, histo -> GetMean(), histo -> GetEntries() / 10., 0.25};
      TF1* langaus = new TF1;
      LangausFit(&langaus, &(*histo), startValues, 0., 50.);
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

    for(map_const_iterator_float mapIt = calib_muonPMap_vsETA.begin();
        mapIt != calib_muonPMap_vsETA.end() ; ++mapIt)
    {
      TH1D* histo = mapIt -> second;
      histo -> Write();
    }
    
    sd1 -> cd();
  }

  outRootFile.cd();
  

  
  outRootFile.Close();
  outFile.close();
  outFile2.close();
  outFile3.close();
  
  
  return 0;
}

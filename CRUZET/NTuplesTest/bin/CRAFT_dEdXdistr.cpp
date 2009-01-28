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

#define P_BIN 50
#define P_MIN 0
#define P_MAX 3



void BinLogX (TH1F& h) ;
void BinLogX (TProfile& h) ;
void BinLogX (TH2F& h) ;
void LangausFit (TF1** langaus, TH1D* histo, double* startValues) ;



int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  double superClusterPhiMIN = subPSetSelections.getParameter<double> ("superClusterPhiMIN") ;
  double superClusterPhiMAX = subPSetSelections.getParameter<double> ("superClusterPhiMAX") ;
  double superClusterEtaMIN = subPSetSelections.getParameter<double> ("superClusterEtaMIN") ;
  double superClusterEtaMAX = subPSetSelections.getParameter<double> ("superClusterEtaMAX") ;
  bool muonLegUpOK = subPSetSelections.getParameter<bool> ("muonLegUpOK") ;
  bool muonLegDownOK = subPSetSelections.getParameter<bool> ("muonLegDownOK") ;
  double muond0MAX = subPSetSelections.getParameter<double> ("muond0MAX") ;
  double muondzMAX = subPSetSelections.getParameter<double> ("muondzMAX") ;
  double muonPMAX = subPSetSelections.getParameter<double> ("muonPMAX") ;
  double muonPMIN = subPSetSelections.getParameter<double> ("muonPMIN") ;
  double muonEoverPMAX = subPSetSelections.getParameter<double> ("muonEoverPMAX") ;
  double muonTkLengthInEcalMIN = subPSetSelections.getParameter<double> ("muonTkLengthInEcalMIN") ;
  double muonTkLengthInEcalMAX = subPSetSelections.getParameter<double> ("muonTkLengthInEcalMAX") ;
  
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
  std::string outputRootName = "CRAFT_dEdXdistr.root" ;
  TFile outputRootFile (outputRootName.c_str (), "recreate") ;
  outputRootFile.cd () ;
  std::ofstream outFile ("stoppingPower_exp.dat", std::ios::out) ;
  
  
  
  // output distributions
  TH1F nClustersInSuperClusterDistr ("nClustersInSuperClusterDistr", "nClustersInSuperClusterDistr", 50, 0., 50.) ;
  TProfile nClustersInSuperCluster_ETA ("nClustersInSuperCluster_ETA", "nClustersInSuperCluster_ETA", ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile nClustersInSuperCluster_PHI ("nClustersInSuperCluster_PHI", "nClustersInSuperCluster_PHI", PHI_BIN, PHI_MIN, PHI_MAX) ;
  
  TH1F nXtalsInSuperClusterDistr ("nXtalsInSuperClusterDistr", "nXtalsInSuperClusterDistr", 50, 0., 50.) ;
  TProfile nXtalsInSuperCluster_ETA ("nXtalsInSuperCluster_ETA", "nXtalsInSuperCluster_ETA", ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile nXtalsInSuperCluster_PHI ("nXtalsInSuperCluster_PHI", "nXtalsInSuperCluster_PHI", PHI_BIN, PHI_MIN, PHI_MAX) ;
  
  TH1F nXtalsCrossedDistr ("nXtalsCrossedDistr", "nXtalsCrossedDistr", 50, 0., 50.) ;
  TProfile nXtalsCrossed_ETA ("nXtalsCrossed_ETA", "nXtalsCrossed_ETA", ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile nXtalsCrossed_PHI ("nXtalsCrossed_PHI", "nXtalsCrossed_PHI", PHI_BIN, PHI_MIN, PHI_MAX) ;
  
  
  TH1F nClustersInSuperClusterDistr_CUT ("nClustersInSuperClusterDistr_CUT", "nClustersInSuperClusterDistr_CUT", 50, 0., 50.) ;
  TProfile nClustersInSuperCluster_ETA_CUT ("nClustersInSuperCluster_ETA_CUT", "nClustersInSuperCluster_ETA_CUT", ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile nClustersInSuperCluster_PHI_CUT ("nClustersInSuperCluster_PHI_CUT", "nClustersInSuperCluster_PHI_CUT", PHI_BIN, PHI_MIN, PHI_MAX) ;
  
  TH1F nXtalsInSuperClusterDistr_CUT ("nXtalsInSuperClusterDistr_CUT", "nXtalsInSuperClusterDistr_CUT", 50, 0., 50.) ;
  TProfile nXtalsInSuperCluster_ETA_CUT ("nXtalsInSuperCluster_ETA_CUT", "nXtalsInSuperCluster_ETA_CUT", ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile nXtalsInSuperCluster_PHI_CUT ("nXtalsInSuperCluster_PHI_CUT", "nXtalsInSuperCluster_PHI_CUT", PHI_BIN, PHI_MIN, PHI_MAX) ;
  
  TH1F nXtalsCrossedDistr_CUT ("nXtalsCrossedDistr_CUT", "nXtalsCrossedDistr_CUT", 50, 0., 50.) ;
  TProfile nXtalsCrossed_ETA_CUT ("nXtalsCrossed_ETA_CUT", "nXtalsCrossed_ETA_CUT", ETA_BIN, ETA_MIN, ETA_MAX) ;
  TProfile nXtalsCrossed_PHI_CUT ("nXtalsCrossed_PHI_CUT", "nXtalsCrossed_PHI_CUT", PHI_BIN, PHI_MIN, PHI_MAX) ;
  
  
  
  
  TH2F superClusterOccupancy_ETAvsPHI ("superClusterOccupancy_ETAvsPHI", "superClusterOccupancy_ETAvsPHI",
                                       PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  TH2F muonOccupancy_ETAvsPHI ("muonOccupancy_ETAvsPHI", "muonOccupancy_ETAvsPHI",
                               PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  
  TGraph BetheBloch_th ;
  DrawBetheBloch (&BetheBloch_th) ;
  
  TH1D dEoverdX ("dEoverdX", "dEoverdX", 1000, 0., 0.5) ;
  TH1D dEoverdX_E3x3 ("dEoverdX_E3x3", "dEoverdX_E3x3", 1000, 0., 0.5) ;
  TH1D dEoverdX_E5x5 ("dEoverdX_E5x5", "dEoverdX_E5x5", 1000, 0., 0.5) ;
  
  
  TH2F dE ("dE", "dE", P_BIN, P_MIN, P_MAX, 100000, 0., 1000.);
  BinLogX (dE) ;
  TProfile dE_profile ("dE_profile", "dE_profile", P_BIN, P_MIN, P_MAX);
  BinLogX (dE_profile) ;
  TH2F EoP ("EoP", "EoP", P_BIN, P_MIN, P_MAX, 10000, 0., 10.);
  BinLogX (EoP) ;
  TProfile EoP_profile ("EoP_profile", "EoP_profile", P_BIN, P_MIN, P_MAX);
  BinLogX (EoP_profile) ;
  
  
  TH2F BetheBloch ("BetheBloch", "BetheBloch", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch) ;
  TH2F BetheBloch_E3x3 ("BetheBloch_E3x3", "BetheBloch_E3x3", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch_E3x3) ;
  TH2F BetheBloch_E5x5 ("BetheBloch_E5x5", "BetheBloch_E5x5", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch_E5x5) ;
  std::vector<TH1D*> BetheBloch_pBin ;
  
  TProfile BetheBloch_profile ("BetheBloch_profile", "BetheBloch_profile", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile) ;
  TProfile BetheBloch_profile_E3x3 ("BetheBloch_profile_E3x3", "BetheBloch_profile_E3x3", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile_E3x3) ;
  TProfile BetheBloch_profile_E5x5 ("BetheBloch_profile_E5x5", "BetheBloch_profile_E5x5", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile_E5x5) ;
  
  TGraphAsymmErrors BetheBloch_graph ;
  TGraphAsymmErrors BetheBloch_graph_E3x3 ;
  TGraphAsymmErrors BetheBloch_graph_E5x5 ;
  
  TGraphErrors BetheBloch_graph_residuals ;
  TGraphErrors BetheBloch_graph_MPV ;
  TGraphErrors BetheBloch_graph_GSigma ;
  TGraphAsymmErrors BetheBloch_graph_mean ;
  
  
  
  TH2F BetheBloch_curved ("BetheBloch_curved", "BetheBloch_curved", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch_curved) ;
  TH2F BetheBloch_E3x3_curved ("BetheBloch_E3x3_curved", "BetheBloch_E3x3_curved", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch_E3x3_curved) ;
  TH2F BetheBloch_E5x5_curved ("BetheBloch_E5x5_curved", "BetheBloch_E5x5_curved", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch_E3x3_curved) ;
  std::vector<TH1D*> BetheBloch_pBin_curved ;
  
  TProfile BetheBloch_profile_curved ("BetheBloch_profile_curved", "BetheBloch_profile_curved", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile_curved) ;
  TProfile BetheBloch_profile_E3x3_curved ("BetheBloch_profile_E3x3_curved", "BetheBloch_profile_E3x3_curved", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile_E3x3_curved) ;
  TProfile BetheBloch_profile_E5x5_curved ("BetheBloch_profile_E5x5_curved", "BetheBloch_profile_E5x5_curved", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile_E5x5_curved) ;
  
  TGraphAsymmErrors BetheBloch_graph_curved ;
  TGraphAsymmErrors BetheBloch_graph_E3x3_curved ;
  TGraphAsymmErrors BetheBloch_graph_E5x5_curved ;
  
  TGraphErrors BetheBloch_graph_residuals_curved ;
  TGraphErrors BetheBloch_graph_MPV_curved ;
  TGraphErrors BetheBloch_graph_GSigma_curved ;
  TGraphAsymmErrors BetheBloch_graph_mean_curved ;
  
  TH2F muonTkLength_straight_curved_pT ("muonTkLength_straight_curved_pT", "muonTkLength_straight_curved_pT", P_BIN, P_MIN, P_MAX, 1000, -5., 5.) ;
  BinLogX (muonTkLength_straight_curved_pT) ;
  
  
  std::map<int,float> pMap ;
  pMap. clear() ;
  std::map<int,float> pMapCurved ;
  pMapCurved. clear() ;
  std::map<int,float> pMap_E3x3 ;
  pMap_E3x3. clear() ;
  std::map<int,float> pMapCurved_E3x3 ;
  pMapCurved_E3x3. clear() ;
  std::map<int,float> pMap_E5x5 ;
  pMap_E5x5. clear() ;
  std::map<int,float> pMapCurved_E5x5 ;
  pMapCurved_E5x5. clear() ;
  std::map<int, float> BetheBloch_mean ;
  BetheBloch_mean.clear () ;
  std::map<int, float> BetheBloch_mean_curved ;
  BetheBloch_mean_curved.clear () ;
  std::map<int, float> muonTkLengthMap_straight_curved_RMS ;
  muonTkLengthMap_straight_curved_RMS.clear () ;
  
  
  
  
  
  
  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    chain->GetEntry (entry) ;
    // if (entry == 100000) break ;
    
    // association MU-SC
    std::vector<ect::association> associations ;
    ect::fillAssocVector (associations, treeVars) ;
    ect::selectOnDR (associations, treeVars, 0.1) ;
    
    // numAssociations.Fill(associations.size()) ;
    
    
    
    //loop on associations vector
    for (unsigned int i = 0 ; i < associations.size () ; ++i)
    {
      int MUindex = associations.at (i).first  ;
      int SCindex = associations.at (i).second ;
      
      
      int muonLeg = treeVars.muonLeg[MUindex];
      float muonP = 0.;
      float muonPt = 0.;
      float muond0 = treeVars.muond0[MUindex];
      float muondz = treeVars.muondz[MUindex];
      float muonPhi = 0.;
      float muonEta = 0.;
      float muonTkLengthInEcal = treeVars.muonTkLengthInEcalDetail[MUindex] ;
      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex] ;
      GlobalPoint muonTkInternalPointInEcalCurved (treeVars.muonTkInternalPointInEcalCurvedX[MUindex],
                                                   treeVars.muonTkInternalPointInEcalCurvedY[MUindex],
                                                   treeVars.muonTkInternalPointInEcalCurvedZ[MUindex]) ;
      float superClusterRawEnergy = treeVars.superClusterRawEnergy[SCindex]/4.*0.9 ;
      float superClusterEta = treeVars.superClusterEta[SCindex] ;
      float superClusterPhi = treeVars.superClusterPhi[SCindex] ;
      int nClustersInSuperCluster = treeVars.nClustersInSuperCluster[SCindex] ;
      int nXtalsInSuperCluster = treeVars.nXtalsInSuperCluster[SCindex] ;
      int xtalIndexInSuperCluster = treeVars.xtalIndexInSuperCluster[SCindex] ;
      
      int pBin = -1;
      int pBinCurved = -1;
      
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
      
      else continue ;
      
      float Radius = muonPt / 0.3 /3.8 * 100.;     //pT (GeV) = 0.3 B(T) R(m)
      float correction =  muonTkLengthInEcal*muonTkLengthInEcal /Radius /Radius / 6.;
      float muonEoverP = superClusterRawEnergy/muonP ;
      
      
      
      nClustersInSuperClusterDistr.Fill (nClustersInSuperCluster) ;
      nClustersInSuperCluster_ETA.Fill (superClusterEta, nClustersInSuperCluster) ;
      nClustersInSuperCluster_PHI.Fill (superClusterPhi, nClustersInSuperCluster) ;
      
      nXtalsInSuperClusterDistr.Fill (nXtalsInSuperCluster) ;
      nXtalsInSuperCluster_ETA.Fill (superClusterEta, nXtalsInSuperCluster) ;
      nXtalsInSuperCluster_PHI.Fill (superClusterPhi, nXtalsInSuperCluster) ;
      
      int nXtalsCrossed = 0 ;
      for (int XTLindex = xtalIndexInSuperCluster ;
           XTLindex < xtalIndexInSuperCluster + nXtalsInSuperCluster ; ++XTLindex)
      {
        if (treeVars.xtalTkLengthCurved[XTLindex] > 0.)
          ++nXtalsCrossed ;
      }
      nXtalsCrossedDistr.Fill (nXtalsCrossed) ;
      nXtalsCrossed_ETA.Fill (superClusterEta, nXtalsCrossed) ;
      nXtalsCrossed_PHI.Fill (superClusterPhi, nXtalsCrossed) ;
      
      
      
      // Cut event
      if ( (fabs(muond0) > muond0MAX) || (fabs(muondz) > muondzMAX) ) continue ;
      if ( (muonP < muonPMIN) || (muonP > muonPMAX) ) continue ;
      if ( (muonLegUpOK == true && muonLegDownOK == false && muonLeg != 1) ||
           (muonLegDownOK == true && muonLegUpOK == false && muonLeg != -1) ) continue ;
      if (muonEoverP > muonEoverPMAX) continue ; 
      if ( (muonTkLengthInEcal < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcal > muonTkLengthInEcalMAX) ) continue ;
      if ( (muonTkLengthInEcalCurved < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcalCurved > muonTkLengthInEcalMAX) ) continue ;
      if ( (superClusterPhi < superClusterPhiMIN) || (superClusterPhi > superClusterPhiMAX) ) continue ;
      if ( (superClusterEta < superClusterEtaMIN) || (superClusterEta > superClusterEtaMAX) ) continue ;
      
      
      nClustersInSuperClusterDistr_CUT.Fill (nClustersInSuperCluster) ;
      nClustersInSuperCluster_ETA_CUT.Fill (superClusterEta, nClustersInSuperCluster) ;
      nClustersInSuperCluster_PHI_CUT.Fill (superClusterPhi, nClustersInSuperCluster) ;
      
      nXtalsInSuperClusterDistr_CUT.Fill (nXtalsInSuperCluster) ;
      nXtalsInSuperCluster_ETA_CUT.Fill (superClusterEta, nXtalsInSuperCluster) ;
      nXtalsInSuperCluster_PHI_CUT.Fill (superClusterPhi, nXtalsInSuperCluster) ;
      
      nXtalsCrossed = 0 ;
      for (int XTLindex = xtalIndexInSuperCluster ;
           XTLindex < xtalIndexInSuperCluster + nXtalsInSuperCluster ; ++XTLindex)
      {
        if (treeVars.xtalTkLengthCurved[XTLindex] > 0.)
          ++nXtalsCrossed ;
      }
      nXtalsCrossedDistr_CUT.Fill (nXtalsCrossed) ;
      nXtalsCrossed_ETA_CUT.Fill (superClusterEta, nXtalsCrossed) ;
      nXtalsCrossed_PHI_CUT.Fill (superClusterPhi, nXtalsCrossed) ;
      
      
      
      // Fill distribution
      if ( (nClustersInSuperCluster == 1) && (muonTkLengthInEcal > 0.) )
      {
        int Cindex = treeVars.clusterIndexInSuperCluster[SCindex] ;
        float clusterE3x3 = treeVars.clusterE3x3[Cindex]/4.*0.9 ;
        float clusterE5x5 = treeVars.clusterE5x5[Cindex]/4.*0.9 ;
        
        dEoverdX.Fill (superClusterRawEnergy / muonTkLengthInEcal) ;
        dEoverdX_E3x3.Fill (clusterE3x3 / muonTkLengthInEcal) ;
        dEoverdX_E5x5.Fill (clusterE5x5 / muonTkLengthInEcal) ;
      }
      
      
      superClusterOccupancy_ETAvsPHI.Fill (superClusterPhi, superClusterEta) ;
      muonOccupancy_ETAvsPHI.Fill (muonTkInternalPointInEcalCurved.phi (), muonTkInternalPointInEcalCurved.eta ()) ;
      
      
      dE.Fill (muonP, superClusterRawEnergy) ;
      dE_profile.Fill (muonP, superClusterRawEnergy) ;
      EoP.Fill (muonP, superClusterRawEnergy/muonP) ;
      EoP_profile.Fill (muonP, superClusterRawEnergy/muonP) ;
      
      
      //if(muonTkLengthInEcal > 0.)
      {
        float dEdX = superClusterRawEnergy / muonTkLengthInEcal / 8.28 * 1000. ;
        BetheBloch.Fill (muonP, dEdX) ;
        pBin = BetheBloch_profile.Fill (muonP, dEdX) ;
        pMap[pBin] += muonP;
        
        if (nClustersInSuperCluster == 1)
        {
          int Cindex = treeVars.clusterIndexInSuperCluster[SCindex] ;
          float clusterE3x3 = treeVars.clusterE3x3[Cindex]/4.*0.9 ;
          float clusterE5x5 = treeVars.clusterE5x5[Cindex]/4.*0.9 ;
          
          dEdX = clusterE3x3 / muonTkLengthInEcal / 8.28 * 1000. ;
          BetheBloch_E3x3.Fill (muonP, dEdX) ;
          pBin = BetheBloch_profile_E3x3.Fill (muonP, dEdX) ;
          pMap_E3x3[pBin] += muonP;
          
          dEdX = clusterE5x5 / muonTkLengthInEcal / 8.28 * 1000. ;
          BetheBloch_E5x5.Fill (muonP, dEdX) ;
          pBin = BetheBloch_profile_E5x5.Fill (muonP, dEdX) ;
          pMap_E5x5[pBin] += muonP;
        }
      }
      
      //if(muonTkLengthInEcalCurved > 0.)
      {
        float dEdXCurved = superClusterRawEnergy / muonTkLengthInEcalCurved / 8.28 * 1000. ;
        BetheBloch_curved.Fill (muonP, dEdXCurved);
        pBinCurved = BetheBloch_profile_curved.Fill (muonP, dEdXCurved);
        pMapCurved[pBinCurved] += muonP;
        
        if (nClustersInSuperCluster == 1)
        {
          int Cindex = treeVars.clusterIndexInSuperCluster[SCindex] ;
          float clusterE3x3 = treeVars.clusterE3x3[Cindex]/4.*0.9 ;
          float clusterE5x5 = treeVars.clusterE5x5[Cindex]/4.*0.9 ;
          
          dEdXCurved = clusterE3x3 / muonTkLengthInEcalCurved / 8.28 * 1000. ;
          BetheBloch_E3x3_curved.Fill (muonP, dEdXCurved) ;
          pBinCurved = BetheBloch_profile_E3x3_curved.Fill (muonP, dEdXCurved) ;
          pMapCurved_E3x3[pBinCurved] += muonP;
          
          dEdXCurved = clusterE5x5 / muonTkLengthInEcalCurved / 8.28 * 1000. ;
          BetheBloch_E5x5_curved.Fill (muonP, dEdXCurved) ;
          pBinCurved = BetheBloch_profile_E5x5_curved.Fill (muonP, dEdXCurved) ;
          pMapCurved_E5x5[pBinCurved] += muonP;
        }

        muonTkLength_straight_curved_pT.Fill (muonPt, (muonTkLengthInEcal - muonTkLengthInEcalCurved) );
      }
      
    }//loop on associations vector
    
  } // loop over entries
  
  

  // for (int bin = 1 ; bin <= muonTkLength_straight_curved_pT.GetNbinsX (); ++bin)
  // {
  //   char histoName[50] ;
  //   sprintf (histoName, "muonTkLength_straight_curved_pTbin_%d", bin) ;
  //   TH1D* histo = muonTkLength_straight_curved_pT.ProjectionY (histoName, bin, bin) ;
  //   histo -> Write () ;
  // }
  
  // muonTkLength_straight_curved_pT.FitSlicesY (0, 1, muonTkLength_straight_curved_pT.GetNbinsX (), 0, "Q") ;
  // TH1D *h1 = (TH1D*)gDirectory->Get("muonTkLength_straight_curved_pT_1");
  // TH1D *h2 = (TH1D*)gDirectory->Get("muonTkLength_straight_curved_pT_2");
  // h1 -> Write () ;
  // h2 -> Write () ;
  // for (int bin = 1 ; bin <= muonTkLength_straight_curved_pT.GetNbinsX () ; ++bin)
  //   muonTkLengthMap_straight_curved_RMS[bin] = 0. ; //h2 -> GetBinContent (bin) ;
  
  
  
  outputRootFile.mkdir ("BetheBloch_pBin") ;
  outputRootFile.cd ("BetheBloch_pBin") ;
  
  for (int bin = 1 ; bin <= BetheBloch_profile.GetNbinsX () ; ++bin)
  {
    if (pMap[bin] > 0)
      pMap[bin] /= BetheBloch_profile.GetBinEntries (bin) ;
    if (pMap_E3x3[bin] > 0)
      pMap_E3x3[bin] /= BetheBloch_profile_E3x3.GetBinEntries (bin) ;
    if (pMap_E5x5[bin] > 0)
      pMap_E5x5[bin] /= BetheBloch_profile_E5x5.GetBinEntries (bin) ;
    
    char histoName[50] ;
    sprintf (histoName, "BetheBloch_pBin_%d", bin) ;
    TH1D* histo = BetheBloch.ProjectionY (histoName, bin, bin) ;
    BetheBloch_pBin.push_back (histo);
    
    TF1* langaus = new TF1 ;
    double startValues[4] = {0.1, histo -> GetMean (), histo -> GetEntries () / 10., 0.1};
    //LangausFit (&langaus, histo, startValues) ;
    histo -> Write () ;
    
    if (pMap[bin] > 0)
    {
      BetheBloch_mean[bin] = langaus -> Mean (0., 1000.) ;
      BetheBloch_graph_MPV.SetPoint (bin-1, pMap[bin], langaus -> GetParameter (1)) ;
      BetheBloch_graph_MPV.SetPointError (bin-1, 0., langaus -> GetParError (1)) ;
      BetheBloch_graph_GSigma.SetPoint (bin-1, pMap[bin], langaus -> GetParameter (3)) ;
      BetheBloch_graph_GSigma.SetPointError (bin-1, 0., langaus -> GetParError (3)) ;
     }
  }
  
  
  
  outputRootFile.mkdir ("BetheBlochCurved_pBin") ;
  outputRootFile.cd ("BetheBlochCurved_pBin") ;
  
  for (int bin = 1 ; bin <= BetheBloch_profile_curved.GetNbinsX () ; ++bin)
  {
    if (pMapCurved[bin] > 0)
      pMapCurved[bin] /= BetheBloch_profile_curved.GetBinEntries (bin) ;
    if (pMapCurved_E3x3[bin] > 0)
      pMapCurved_E3x3[bin] /= BetheBloch_profile_E3x3_curved.GetBinEntries (bin) ;
    if (pMapCurved_E5x5[bin] > 0)
      pMapCurved_E5x5[bin] /= BetheBloch_profile_E5x5_curved.GetBinEntries (bin) ;
    
    
    
    char histoName[50] ;
    sprintf (histoName, "BetheBloch_pBin_curved_%d", bin) ;
    TH1D* histo = BetheBloch_curved.ProjectionY (histoName, bin, bin) ;
    BetheBloch_pBin_curved.push_back (histo);
    
    TF1* langaus = new TF1 ;
    double startValues[4] = {0.1, histo -> GetMean (), histo -> GetEntries () / 10., 0.1};
    //LangausFit (&langaus, histo, startValues) ;
    histo -> Write () ;
    
    if (pMapCurved[bin] >0)
    {
      BetheBloch_mean_curved[bin] = langaus -> Mean (0., 1000.) ;
      BetheBloch_graph_MPV_curved.SetPoint (bin-1, pMapCurved[bin], langaus -> GetParameter (1)) ;
      BetheBloch_graph_MPV_curved.SetPointError (bin-1, 0., langaus -> GetParError (1)) ;
      BetheBloch_graph_GSigma_curved.SetPoint (bin-1, pMapCurved[bin], langaus -> GetParameter (3)) ;
      BetheBloch_graph_GSigma_curved.SetPointError (bin-1, 0., langaus -> GetParError (3)) ;
    }
    
    
    
    std::cout << "Bin " << std::fixed << std::setprecision (2) << std::setw (3) << bin 
              << ":   [" << std::setw (7) << BetheBloch_profile_curved.GetBinLowEdge (bin)
              << "," << std::setw (7) << BetheBloch_profile_curved.GetBinLowEdge (bin) + BetheBloch_profile_curved.GetBinWidth (bin)
              << "]         BinCenter: " << std::setw (7) << BetheBloch_profile_curved.GetBinCenter (bin)
              << "   BinAvgCenter: " << std::setw (7) << pMapCurved[bin]
              << "         BinContent: " << std::setprecision (0) << std::setw (5) << BetheBloch_profile_curved.GetBinEntries (bin)
              << std::endl;
    
  }
  
  outputRootFile.cd () ;  
  
  
  
  
  double errX_low = -1.;
  double errX_hig = -1.;
  double errY = -1.;
  
  for(int bin = 1; bin <= BetheBloch_profile.GetNbinsX(); ++bin)
  {
    errX_low = pMap[bin] - BetheBloch_profile.GetBinLowEdge (bin) ;
    errX_hig = BetheBloch_profile.GetBinLowEdge (bin) + BetheBloch_profile.GetBinWidth (bin) - pMap[bin] ;
    errY = sqrt ( pow (muonTkLengthMap_straight_curved_RMS[bin], 2) + pow (BetheBloch_profile.GetBinError (bin), 2 ) ) ;
    
    BetheBloch_graph.SetPoint (bin-1, pMap[bin], BetheBloch_profile.GetBinContent (bin) );
    BetheBloch_graph.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E3x3.SetPoint (bin-1, pMap_E3x3[bin], BetheBloch_profile_E3x3.GetBinContent (bin) );
    BetheBloch_graph_E3x3.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E5x5.SetPoint (bin-1, pMap_E5x5[bin], BetheBloch_profile_E5x5.GetBinContent (bin) );
    BetheBloch_graph_E5x5.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    
    
    BetheBloch_graph_mean.SetPoint (bin-1, pMap[bin], BetheBloch_mean[bin]) ;
    BetheBloch_graph_mean.SetPointError (bin-1, errX_low, errX_hig, errY, errY) ;
    
    // BB_energy.SetPoint(bin-1, pMap[bin-1]/BetheBloch_profile.GetBinEntries(bin), BetheBloch_energy.GetBinContent(bin));
    // BB_energy.SetPointError(bin-1, 0., BetheBloch_energy.GetBinError(bin));
    // BB_25x25.SetPoint(bin+1, pMap[bin]/BetheBloch_profile.GetBinEntries(bin), BetheBloch_E5x5.GetBinContent(bin));
    // BB_E5x5.SetPointError(bin+1, 0., BetheBloch_E5x5.GetBinError(bin));
  }
  
  DrawBetheBlochResiduals (BetheBloch_graph, BetheBloch_th, BetheBloch_graph_residuals) ;
  
  
  
  for(int bin = 1; bin <= BetheBloch_profile_curved.GetNbinsX(); ++bin)
  {
    errX_low = pMapCurved[bin] - BetheBloch_profile_curved.GetBinLowEdge (bin) ;
    errX_hig = BetheBloch_profile_curved.GetBinLowEdge (bin) + BetheBloch_profile_curved.GetBinWidth (bin) - pMapCurved[bin] ;
    errY = sqrt ( pow (muonTkLengthMap_straight_curved_RMS[bin], 2) + pow (BetheBloch_profile_curved.GetBinError (bin), 2 ) ) ;
    
    BetheBloch_graph_curved.SetPoint (bin-1, pMapCurved[bin], BetheBloch_profile_curved.GetBinContent(bin) );
    BetheBloch_graph_curved.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E3x3_curved.SetPoint (bin-1, pMapCurved_E3x3[bin], BetheBloch_profile_E3x3_curved.GetBinContent(bin) );
    BetheBloch_graph_E3x3_curved.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_E5x5_curved.SetPoint (bin-1, pMapCurved_E5x5[bin], BetheBloch_profile_E5x5_curved.GetBinContent(bin) );
    BetheBloch_graph_E5x5_curved.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    
    
    BetheBloch_graph_mean_curved.SetPoint (bin-1, pMapCurved[bin], BetheBloch_mean_curved[bin]) ;
    BetheBloch_graph_mean_curved.SetPointError (bin-1, errX_low, errX_hig, errY, errY) ;
    
    // BB_energy.SetPoint(bin-1, pMap[bin-1]/BetheBloch_profile_curved.GetBinEntries(bin), BetheBloch_energy.GetBinContent(bin));
    // BB_energy.SetPointError(bin-1, 0., BetheBloch_energy.GetBinError(bin));
    // BB_25x25.SetPoint(bin+1, pMap[bin]/BetheBloch_profile_curved.GetBinEntries(bin), BetheBloch_E5x5.GetBinContent(bin));
    // BB_E5x5.SetPointError(bin+1, 0., BetheBloch_E5x5.GetBinError(bin));
  }
  
  DrawBetheBlochResiduals(BetheBloch_graph, BetheBloch_th, BetheBloch_graph_residuals_curved) ;
  
  
  
  
  
  
  // Save histograms
  nClustersInSuperClusterDistr.Write () ;
  nClustersInSuperCluster_ETA.Write () ;
  nClustersInSuperCluster_PHI.Write () ;
  
  nXtalsInSuperClusterDistr.Write () ;
  nXtalsInSuperCluster_ETA.Write () ;
  nXtalsInSuperCluster_PHI.Write () ;
  
  nXtalsCrossedDistr.Write () ;
  nXtalsCrossed_ETA.Write () ;
  nXtalsCrossed_PHI.Write () ;
  
  
  nClustersInSuperClusterDistr_CUT.Write () ;
  nClustersInSuperCluster_ETA_CUT.Write () ;
  nClustersInSuperCluster_PHI_CUT.Write () ;
  
  nXtalsInSuperClusterDistr_CUT.Write () ;
  nXtalsInSuperCluster_ETA_CUT.Write () ;
  nXtalsInSuperCluster_PHI_CUT.Write () ;
  
  nXtalsCrossedDistr_CUT.Write () ;
  nXtalsCrossed_ETA_CUT.Write () ;
  nXtalsCrossed_PHI_CUT.Write () ;
  
  
  superClusterOccupancy_ETAvsPHI.Write () ;
  muonOccupancy_ETAvsPHI.Write () ;
  
  
  dE.Write () ;
  dE_profile.Write () ;
  EoP.Write () ;
  EoP_profile.Write () ;
  
  
  double startValues1[4] = {0.000903, 0.01249, 1.708, 0.00187} ;
  TF1* langaus = new TF1 ;
  LangausFit (&langaus, &dEoverdX, startValues1) ;
  dEoverdX.Write () ;
  delete langaus ;
  
  double startValues2[4] = {0.000903, 0.01249, 1.708, 0.00187} ;
  langaus = new TF1 ;
  LangausFit (&langaus, &dEoverdX_E3x3, startValues2) ;
  dEoverdX_E3x3.Write () ;
  delete langaus ;
  
  double startValues3[4] = {0.000903, 0.01249, 1.708, 0.00187} ;
  langaus = new TF1 ;
  LangausFit (&langaus, &dEoverdX_E5x5, startValues3) ;
  dEoverdX_E5x5.Write () ;
  delete langaus ;
  
  BetheBloch.Write() ;
  BetheBloch_E3x3.Write() ;
  BetheBloch_E5x5.Write() ;
  
  BetheBloch_profile.Write () ;
  BetheBloch_profile_E3x3.Write () ;
  BetheBloch_profile_E5x5.Write () ;
  
  BetheBloch_graph.Write ("BetheBloch_graph") ;
  BetheBloch_graph_E3x3.Write ("BetheBloch_graph_E3x3") ;
  BetheBloch_graph_E5x5.Write ("BetheBloch_graph_E5x5") ;
  
  BetheBloch_graph_residuals.Write ("BetheBloch_graph_residuals") ;
  BetheBloch_graph_MPV.Write ("BetheBloch_graph_MPV") ;
  BetheBloch_graph_GSigma.Write ("BetheBloch_graph_GSigma") ;
  BetheBloch_graph_mean.Write ("BetheBloch_graph_mean") ;
  
  
  
  BetheBloch_curved.Write() ;
  BetheBloch_E3x3_curved.Write() ;
  BetheBloch_E5x5_curved.Write() ;
  
  BetheBloch_profile_curved.Write() ;
  BetheBloch_profile_E3x3_curved.Write() ;
  BetheBloch_profile_E5x5_curved.Write() ;
  
  BetheBloch_graph_curved.Write ("BetheBloch_graph_curved") ;
  BetheBloch_graph_E3x3_curved.Write ("BetheBloch_graph_E3x3_curved") ;
  BetheBloch_graph_E5x5_curved.Write ("BetheBloch_graph_E5x5_curved") ;
  
  BetheBloch_graph_residuals_curved.Write ("BetheBloch_graph_residuals_curved") ;
  BetheBloch_graph_MPV_curved.Write ("BetheBloch_graph_MPV_curved") ;
  BetheBloch_graph_GSigma_curved.Write ("BetheBloch_graph_GSigma_curved") ;
  BetheBloch_graph_mean.Write ("BetheBloch_graph_mean_curved") ;
  
  muonTkLength_straight_curved_pT.Write () ;
  
  outputRootFile.Close () ;
  outFile.close () ;  
  
  return 0 ;
}






void BinLogX(TProfile& h)
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



void BinLogX(TH2F& h)
{
  TAxis* axis = h.GetXaxis();
  int bins = axis->GetNbins();
  
  Axis_t from = axis->GetXmin();
  Axis_t to = axis->GetXmax();
  Axis_t width = (to - from) / bins;
  Axis_t* new_bins = new Axis_t[bins + 1];
  
  for (int i = 0; i <= bins; i++) 
    new_bins[i] = pow(10, from + i * width);
  
  axis->Set(bins, new_bins);
  delete new_bins;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}



void BinLogX (TH1F& h)
{
  
  TAxis* axis = h.GetXaxis();
  int bins = axis->GetNbins();

  Axis_t from = axis->GetXmin();
  Axis_t to = axis->GetXmax();
  Axis_t width = (to - from) / bins;
  Axis_t* new_bins = new Axis_t[bins + 1];
  
  for (int i = 0; i <= bins; i++) 
    new_bins[i] = pow(10, from + i * width);
  
  axis->Set(bins, new_bins);
  delete new_bins;
  
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

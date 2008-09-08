#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

//PG for the cluster-based calibration 
#include "Calibration/EcalCalibAlgos/interface/VEcalCalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/IMACalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/L3CalibBlock.h"

#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "CRUZET/Calibration/interface/ClusterCalibTools.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLorentzVector.h"

/**
TODOS

- rendere univoca l'uso dell'indice di cristallo: rawId o hashed index?
  - hashed index = + facile
  - rawID = universale anche con EE


*/



int findRegion (const EcalCosmicsTreeContent & treeVars,
                int SCindex,
                EBregionBuilder & EBRegionsTool) ;

int
findMaxXtalInSC (const EcalCosmicsTreeContent & treeVars,
                 int SCindex) ;


//PG ------------------------------------------------------------------


int main (int argc, char** argv)
{
  std::cerr << "Welcome to testCalibAlgos program" << std::endl;


  // read input parameters from config file
  if(!argv[1])
    {
      std::cout << ">>> No input config file given. Terminating program <<<" << std::endl;
      return -1;
    }

  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet() ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing

  edm::ParameterSet subPSetCalib =  
    parameterSet->getParameter<edm::ParameterSet> ("clusterCalib") ;
  int etaMin = subPSetCalib.getParameter<int> ("etaMin") ;
  int etaMax = subPSetCalib.getParameter<int> ("etaMax") ;
  int etaStep = subPSetCalib.getParameter<int> ("etaStep") ;
  int phiMin = subPSetCalib.getParameter<int> ("phiMin") ;
  int phiMax = subPSetCalib.getParameter<int> ("phiMax") ;
  int phiStep = subPSetCalib.getParameter<int> ("phiStep") ;
  std::string algorithm = subPSetCalib.getParameter<std::string> ("algorithm") ;
  double minEnergyPerCrystal = subPSetCalib.getParameter<double> ("minEnergyPerCrystal") ;
  double maxEnergyPerCrystal = subPSetCalib.getParameter<double> ("maxEnergyPerCrystal") ;
  double minCoeff = subPSetCalib.getParameter<double> ("minCoeff") ;
  double maxCoeff = subPSetCalib.getParameter<double> ("maxCoeff") ;
  double dEOdx = subPSetCalib.getParameter<double> ("dEOdx") ;
  int usingBlockSolver = subPSetCalib.getParameter<int> ("usingBlockSolver") ;
  int numberOfLoops = subPSetCalib.getParameter<int> ("numberOfLoops") ;

//PG FIXME some assert to check the params

//  EBregionBuilder EBRegionsTool (-85, -1, 2, 1, 181, 2) ;
  EBregionBuilder EBRegionsTool (etaMin, etaMax, etaStep, phiMin, phiMax, phiStep) ;

  //PG calibration coefficients for this step
  std::map<int,double> recalibMap ;
  for (int a=0; a<170; ++a)
    for (int b=0; b<360; ++b)
      {
        //PG use the hashed index
        int index = a*360+b ;
        recalibMap[index] = 1. ;
//        xtalNumOfHits[index] = 0 ;
      }

  //PG single blocks calibrators
  std::vector<VEcalCalibBlock *> EcalCalibBlocks ;

  //PG loop over the regions set
  for (int region = 0 ;
       region < EBRegionsTool.EBregionsNum () ;
       ++region)
    {
      if (algorithm == "IMA")
        EcalCalibBlocks.push_back (
            new IMACalibBlock (EBRegionsTool.xtalNumInRegion (region))
          ) ;
      else if (algorithm == "L3")
        EcalCalibBlocks.push_back (
            new L3CalibBlock (EBRegionsTool.xtalNumInRegion (region), 1)
          ) ;
      else
        {
          edm::LogError ("building") << algorithm
                          << " is not a valid calibration algorithm" ;
          exit (1) ;
        }
    } //PG loop over the regions set

  //PG FIXME one has to read and save calibration coeffs 
  //PG FIXME read from xml files or DB directly

  //PG FIXME would it be possible to do it for rings or crystals
  //PG FIXME in the same code?

  std::cout << ">>> testCalibAlgos::OpeningFiles::begin <<<" << std::endl;

  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;

  edm::ParameterSet subPSetInput =  
    parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = 
   subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  int maxEvents = subPSetInput.getParameter<int> ("maxEvents") ;
  std::cout << "reading : " ;
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ;
       ++listIt)
    {
      std::cout << *listIt << " " << std::endl ;
      chain->Add (listIt->c_str ()) ;
    }

  std::cout << ">>> testCalibAlgos::TreeBuilding::begin <<<" << std::endl;

  EcalCosmicsTreeContent treeVars ;
  setBranchAddresses (chain, treeVars) ;
  int nEntries = chain->GetEntries () ;
  if (maxEvents < 0) maxEvents = nEntries ;
  std::cout << ">>> testCalibAlgos::Found " << nEntries << " entries\n" ;
  std::cout << ">>> testCalibAlgos::TreeBuilding::end <<<" << std::endl;

  edm::ParameterSet subPSetSelections =  
    parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  double cut_angle_MU_SC = subPSetSelections.getParameter<double> ("cut_angle_MU_SC") ;
  double cut_min_maxEnergy = subPSetSelections.getParameter<double> ("cut_min_maxEnergy") ;


  TH2F eventsMap ("eventsMap","eventsMap",360,0,360,170,0,170) ;
  eventsMap.GetXaxis () -> SetTitle ("iPhi") ;  
  eventsMap.GetYaxis () -> SetTitle ("iEta") ; 

  //PG several loops on the dataset
  for (int iLoop = 0 ; iLoop < numberOfLoops ; ++iLoop)
    {
    
      std::cout << "Loop " << iLoop + 1 << "\n" ;
      
      //PG reset the calibration blocks
      //PG ----------------------------

      for (std::vector<VEcalCalibBlock *>::iterator calibBlock = EcalCalibBlocks.begin () ;
            calibBlock != EcalCalibBlocks.end () ;
            ++calibBlock) 
        (*calibBlock)->reset () ;

      //PG reset the histograms
      //PG --------------------

      eventsMap.Reset () ;

      //PG loop on the calibration entries
      //PG -------------------------------

      //PG loop over entries
      for (int entry = 0 ; entry < maxEvents ; ++entry)
        {
          chain->GetEntry (entry) ;
          if (entry%10000 == 0) std::cout << "reading entry " << entry << "\n" ;
      
          //PG association between muons and superclusters
          //PG -------------------------------------------
      
          std::vector<ect::association> associations ;
          ect::fillAssocVector (associations, treeVars) ;
          ect::selectOnDR (associations, treeVars, 0.3) ;
      
          //PG loop on associations vector
          for (unsigned int i = 0 ; i < associations.size () ; ++i)
            {
              int MUindex = associations.at (i).first ;
              int SCindex = associations.at (i).second ;
        
              //PG selections
              //PG ----------
                            
              TVector3 SC0_pos (treeVars.superClusterX[SCindex], 
                                treeVars.superClusterY[SCindex], 
                                treeVars.superClusterZ[SCindex]) ; 
    	       
              TVector3 MuonDir (treeVars.muonMomentumX[MUindex], 
                                treeVars.muonMomentumY[MUindex], 
                                treeVars.muonMomentumZ[MUindex]) ;
                    
              double angle = MuonDir.Angle (SC0_pos) ;
              if( angle > 3.1415/2. ) angle = 3.1415 - angle; // angle belongs to [0:90]
              if (angle > cut_angle_MU_SC) continue ;              


              int MaxXTLindex = findMaxXtalInSC (treeVars, SCindex) ;
              if (MaxXTLindex < 0) continue ;
              if (treeVars.xtalEnergy[MaxXTLindex] < 0.) continue ;
              if (treeVars.xtalEnergy[MaxXTLindex] < cut_min_maxEnergy) continue ;
            
              //PG geometry
              //PG --------
                            
              //PG find the region ID
//              int EBNumberOfRegion = findRegion (treeVars, SCindex, EBRegionsTool) ;

              EBDetId dummyDetId = EBDetId::unhashIndex (treeVars.xtalHashedIndex[MaxXTLindex]) ;
              int ieta = dummyDetId.ieta () ;
              int iphi = dummyDetId.iphi () ;
              int EBNumberOfRegion = EBRegionsTool.EBRegionId (ieta, iphi) ;

              if (EBNumberOfRegion == -1) continue ;
        
              //PG build the map of the supercluster content
              //PG -----------------------------------------
              
              std::map<int, double> SCComponentsMap ;
              
              //PG loop over xtals in supercluster
              for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
                   XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                              treeVars.nXtalsInSuperCluster[SCindex] ;
                   ++XTLindex)
                {
            
                  double dummy = treeVars.xtalEnergy[XTLindex] ;
                  if ( dummy < minEnergyPerCrystal || 
                       dummy > maxEnergyPerCrystal)
                    continue ;
      
                  eventsMap.Fill (treeVars.xtalHashedIndex[XTLindex]%360,
                                  treeVars.xtalHashedIndex[XTLindex]/360) ;
      
                  dummy *= recalibMap[treeVars.xtalHashedIndex[XTLindex]] ;     
      
                  //PG FIXME assuming there are no duplicates!
                  SCComponentsMap[treeVars.xtalHashedIndex[XTLindex]] = dummy ;
                } //PG loop over xtals in supercluster
      
              //PG run the calib algo
              //PG ------------------
              
              //PG xtal - energy association map
              std::map<int,double> EBxtlMap ;
              double pSubtract ;
              EBRegionsTool.fillEBMap (EBxtlMap, pSubtract,
                                       SCComponentsMap,
                                       EBNumberOfRegion) ;
      
              EcalCalibBlocks.at (EBNumberOfRegion) -> Fill 
                (
                  EBxtlMap.begin() , EBxtlMap.end (),
                  treeVars.muonTkLengthInEcalDetail[MUindex] * dEOdx,
                  pSubtract
                ) ;
      
            } //PG loop on associations vector
      
        } //PG loop over entries
      
      std::cout << ">>> testCalibAlgos::loop on entries terminated <<<" << std::endl;
      
      //PG extract the solution of the calibration
      //PG ---------------------------------------
      
      for (std::vector<VEcalCalibBlock *>::iterator calibBlock = EcalCalibBlocks.begin () ;
            calibBlock != EcalCalibBlocks.end () ;
            ++calibBlock) 
        (*calibBlock)->solve (usingBlockSolver, minCoeff, maxCoeff) ;
      
      TH2F calibCoeffMap ("calibCoeffMap","calibCoeffMap",360,0,360,170,0,170) ;
      calibCoeffMap.GetXaxis () -> SetTitle ("iPhi") ;  
      calibCoeffMap.GetYaxis () -> SetTitle ("iEta") ; 

      TH1F calibCoeff ("calibCoeff","calibCoeff",100,0,2) ;
      
      //PG loop over the barrel xtals to get the coeffs
      for (int eta=0; eta<170; ++eta)
        for (int phi=0; phi<360; ++phi)
          {
            EBDetId xtalDetId = EBDetId::unhashIndex (eta*360+phi) ;
            int index = xtalDetId.rawId () ; 
            if (EBRegionsTool.xtalRegionId (index) == -1) continue ;
            //std::cout << "inside region " << EBRegionsTool.xtalRegionId (index) << "\n" ;
            recalibMap[eta*360+phi] *= 
                EcalCalibBlocks.at (EBRegionsTool.xtalRegionId (index))->at 
                  (EBRegionsTool.xtalPositionInRegion (index)) ;
            //std::cout << "    calib coeff " << recalibMap[eta*360+phi] << "\n" ;
            calibCoeff.Fill (recalibMap[eta*360+phi]) ;      
            calibCoeffMap.Fill (phi,eta,recalibMap[eta*360+phi]) ;      
          } //PG loop over the barrel xtals to get the coeffs
      
      //PG save the histograms
      //PG -------------------

      char nome[80] ; 
      sprintf (nome,"outputHistos_%d.root", iLoop) ;
      TFile outputHistos (nome, "recreate") ;
      outputHistos.cd () ;
      calibCoeffMap.Write () ;
      calibCoeff.Write () ;
      eventsMap.Write () ;
      outputHistos.Close () ;

    } //PG several loops on the dataset

  //PG disegna la mappa

  return 0 ;

}


//PG ------------------------------------------------------------------


//!Find the region to which the SC belongs
int findRegion (const EcalCosmicsTreeContent & treeVars,
                int SCindex,
                EBregionBuilder & EBRegionsTool)
{

  int XTLindex = findMaxXtalInSC (treeVars, SCindex) ;
  if (XTLindex < 0) return XTLindex ;
  if (treeVars.xtalEnergy[XTLindex] < 0.) return -1 ;

  EBDetId dummyDetId = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
  int ieta = dummyDetId.ieta () ;
  int iphi = dummyDetId.iphi () ;
  return EBRegionsTool.EBRegionId (ieta, iphi) ;
}


//PG ------------------------------------------------------------------


int
findMaxXtalInSC (const EcalCosmicsTreeContent & treeVars,
                 int SCindex)
{
  double dummyEnergy = 0. ;
  int maxXtalIndex = -1 ;

  //PG loop over xtals in supercluster
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                  treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      if (treeVars.xtalEnergy[XTLindex] > dummyEnergy)
        {
          dummyEnergy = treeVars.xtalEnergy[XTLindex] ;
          maxXtalIndex = XTLindex ;
        }
    } //PG loop over xtals in supercluster

  return maxXtalIndex ;
}



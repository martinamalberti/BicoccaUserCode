#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "Calibration/Tools/interface/calibXMLwriter.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/CaloMiscalibTools.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/CaloMiscalibMapEcal.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/MiscalibReaderFromXMLEcalBarrel.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/MiscalibReaderFromXMLEcalEndcap.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "Calibration/EcalAlCaRecoProducers/interface/trivialParser.h"
//#include "Calibration/EcalAlCaRecoProducers/bin/trivialParser.h"




//---- new for IOV Iterator ----
#include "CondCore/Utilities/interface/CondIter.h"
#include "TSystem.h"

#include "fstream"
#include "map"
#include "TH2.h"
#include "TProfile.h"
#include "TH1.h"
#include "TFile.h"

#define PI_GRECO 3.14159265

inline int etaShifter (const int etaOld) 
   {
     if (etaOld < 0) return etaOld + 85 ;
     else if (etaOld > 0) return etaOld + 84 ;
   }


// ------------------------------------------------------------------------

// MF questo file prende due set di coefficienti e li confronta 
// 
 
//-------------------------------------------------------------------------

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

  edm::ParameterSet subPSetCalib = parameterSet->getParameter<edm::ParameterSet> ("clusterCalib") ;
  int EBetaStart = subPSetCalib.getParameter<int> ("etaMin") ;
  int EBetaEnd = subPSetCalib.getParameter<int> ("etaMax") ;
  int etaStep = subPSetCalib.getParameter<int> ("etaStep") ;
  int EBphiStart = subPSetCalib.getParameter<int> ("phiMin") ;
  int EBphiEnd = subPSetCalib.getParameter<int> ("phiMax") ;
  int phiStep = subPSetCalib.getParameter<int> ("phiStep") ;
  std::string algorithm = subPSetCalib.getParameter<std::string> ("algorithm") ;
  double minEnergyPerCrystal = subPSetCalib.getParameter<double> ("minEnergyPerCrystal") ;
  double maxEnergyPerCrystal = subPSetCalib.getParameter<double> ("maxEnergyPerCrystal") ;
  double minCoeff = subPSetCalib.getParameter<double> ("minCoeff") ;
  double maxCoeff = subPSetCalib.getParameter<double> ("maxCoeff") ;
  double dEOdx = subPSetCalib.getParameter<double> ("dEOdx") ;
  int usingBlockSolver = subPSetCalib.getParameter<int> ("usingBlockSolver") ;
  int numberOfLoops = subPSetCalib.getParameter<int> ("numberOfLoops") ;

    int power = -1 ;
    
    
    std::string filename = "coeffcompareEB.root" ;
    //std::string barrelfile = "/afs/cern.ch/user/m/mattia/CMSSW_2_0_12/src/CRUZET/Calibration/bin/pari.txt";
    //std::string calibBarrelfile = "/afs/cern.ch/user/m/mattia/CMSSW_2_0_12/src/CRUZET/Calibration/bin/dispari.txt";
    
    std::map<int,double> CoeffOdd;
    std::map<int,double> CoeffEven;  
 
  std::fstream File1("/afs/cern.ch/user/m/mattia/CMSSW_2_0_12/src/CRUZET/Calibration/bin/pari.txt");
  std::fstream File2("/afs/cern.ch/user/m/mattia/CMSSW_2_0_12/src/CRUZET/Calibration/bin/dispari.txt");
  
  //MF leggo txt
  for(int i=0; i <= 61200; i++)
  {
  int index;
  double calibcoeff;
  File1 >> index >> calibcoeff;
  CoeffOdd[index]=calibcoeff;
  //std::cout << index << " " << CoeffOdd.find(index)->second << " " << calibcoeff << std::endl; 
  File2 >> index >> calibcoeff;
  CoeffEven[index]=calibcoeff;
  }
  
  //PG fill the histograms
  //PG -------------------
  
  
   // ECAL barrel
  TH1F EBCompareCoeffDistr ("EBCompareCoeff","EBCompareCoeff",1000,0,2) ;
  TH2F EBCompareCoeffMap ("EBCompareCoeffMap","EBCompareCoeffMap",360,1,361,171,-85,86) ;
  TH2F EBCompareCoeffEtaTrend ("EBCompareCoeffEtaTrend","EBCompareCoeffEtaTrend",
                               171,-85,86,500,0,2) ;
  TProfile EBCompareCoeffEtaProfile ("EBCompareCoeffEtaProfile","EBCompareCoeffEtaProfile",
                                     171,-85,86,0,2) ;
  TH1F EBCompareCoeffDistr_M1 ("EBCompareCoeff_M1","EBCompareCoeff_M1",1000,0,2) ;
  TH1F EBCompareCoeffDistr_M2 ("EBCompareCoeff_M2","EBCompareCoeff_M2",1000,0,2) ;
  TH1F EBCompareCoeffDistr_M3 ("EBCompareCoeff_M3","EBCompareCoeff_M3",1000,0,2) ;
  TH1F EBCompareCoeffDistr_M4 ("EBCompareCoeff_M4","EBCompareCoeff_M4",1000,0,2) ;
  
  // ECAL barrel
  
  //PG loop over eta
  for (int ieta = EBetaStart ; ieta < EBetaEnd ; ++ieta)
  {
      double phiSum = 0. ; 
      double phiSumSq = 0. ; 
      double N = 0. ;
      for (int iphi = EBphiStart ; iphi <= EBphiEnd ; ++iphi)
      {
      
      
     //     if (!EBDetId::validDetId (ieta,iphi)) continue ;
       //   EBDetId det = EBDetId (ieta,iphi,EBDetId::ETAPHIMODE) ;
         // double factor = (CoeffOdd.find (det.hashedIndex ())->second ) * (CoeffEven.find (det.hashedIndex ())->second) ;
         // if (power != 1 && factor != 0) 
           //   factor = (CoeffOdd.find (det.hashedIndex ())->second) / (CoeffEven.find (det.hashedIndex ())->second);
         
	 
	 // in our txt files coefficients are ordered by ieta*360+iphi 
	  if (!EBDetId::validDetId (ieta,iphi)) continue ;
          EBDetId det = EBDetId (ieta,iphi,EBDetId::ETAPHIMODE) ;
          double factor = (CoeffOdd.find (ieta*360+iphi)->second ) * (CoeffEven.find (ieta*360+iphi)->second) ;
          if (power != 1 && factor != 0) 
              factor = (CoeffOdd.find (ieta*360+iphi)->second) / (CoeffEven.find (ieta*360+iphi)->second);
	  
        std::cout << " ieta = " << ieta  << " iphi = " << iphi << " iEBcalibMap --> " << CoeffOdd.find (ieta*360+iphi)->second;
        std::cout << " iEBscalibMap --> " << CoeffEven.find (ieta*360+iphi)->second << std::endl;

          
          phiSum += factor ;
          phiSumSq += factor * factor ;
          N += 1. ;
          EBCompareCoeffDistr.Fill (factor) ;
          EBCompareCoeffMap.Fill (iphi,ieta,factor) ;
          EBCompareCoeffEtaTrend.Fill (ieta,factor) ;
          EBCompareCoeffEtaProfile.Fill (ieta,factor) ;
          if (abs(ieta) < 26) EBCompareCoeffDistr_M1.Fill (factor) ;
          else if (abs(ieta) < 46) EBCompareCoeffDistr_M2.Fill (factor) ;
          else if (abs(ieta) < 66) EBCompareCoeffDistr_M3.Fill (factor) ;
          else EBCompareCoeffDistr_M4.Fill (factor) ;
      } //PG loop over phi
      double phiMean = phiSum / N ;
      double phiRMS = sqrt (phiSumSq / N - phiMean * phiMean) ;        
  } //PG loop over eta

  // trend vs eta FIXME to be renormalized
  TH1D * EBEtaTrend = EBCompareCoeffMap.ProjectionY () ;
  // trend vs phi FIXME to be renormalized
  TH1D * EBPhiTrend = EBCompareCoeffMap.ProjectionX () ;


  TFile out (filename.c_str (),"recreate") ;
  
  EBCompareCoeffMap.GetXaxis()->SetTitle("i#phi");
  EBCompareCoeffMap.GetYaxis()->SetTitle("i#eta");
  EBCompareCoeffMap.Write () ;
  
  EBCompareCoeffDistr.Write () ;  
  EBCompareCoeffEtaTrend.Write () ;
  EBCompareCoeffEtaProfile.Write () ;
  EBCompareCoeffDistr_M1.Write () ;
  EBCompareCoeffDistr_M2.Write () ;
  EBCompareCoeffDistr_M3.Write () ;
  EBCompareCoeffDistr_M4.Write () ;
  EBEtaTrend->Write () ;
  EBPhiTrend->Write () ;
  out.Close () ;
  
  
  

  
}

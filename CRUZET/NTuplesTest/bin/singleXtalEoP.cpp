#include <iostream>
#include <string>

//PG CMSSW includes
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
//#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>

//MF read CFG files includes5
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>

//PG root includes
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TFile.h"

//MF read CFG files includes5
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>

#include "CRUZET/NTuplesTest/interface/CRUtils.h"
#include "CRUZET/Calibration/interface/ClusterCalibTools.h"

//! main program
int main (int argc, char** argv)
{

  std::string outputRootName = "matchDistance.root" ;
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  //cuts on SC Energy
  double EnergyMaxSC = subPSetSelections.getParameter<double> ("EnergyMaxSC") ;
  double EnergyMinSC = subPSetSelections.getParameter<double> ("EnergyMinSC") ;
  
  //cuts on Angle Muon / SCdirection
  double angleMAX = subPSetSelections.getParameter<double> ("angleMAX") ;
  double angleMIN = subPSetSelections.getParameter<double> ("angleMIN") ;
 
  //cuts on Xtal Energy
  double XtalMaxEnergyMin = subPSetSelections.getParameter<double> ("XtalMaxEnergyMin") ;
  double XtalMaxEnergyMax = subPSetSelections.getParameter<double> ("XtalMaxEnergyMax") ;
  double XtalMinEnergy = subPSetSelections.getParameter<double> ("XtalMinEnergy") ;
  
  //cuts on Windows
  double phiWINDOW    = subPSetSelections.getParameter<double> ("phiWINDOW") ;
  double ietaMAX    = subPSetSelections.getUntrackedParameter<int> ("ietaMAX",85) ;

  edm::ParameterSet subPSetInput =  
    parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = 
   subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  std::cout << "reading : " ;

  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;

  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ;
       ++listIt)
    {   
      std::cout << *listIt << " " << std::endl ;
      chain->Add (listIt->c_str ()) ;
    }

  TProfile2D aveEoxMap ("aveEoxMap","aveEoxMap",360,1.,361.,172,-86.,86.); 

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;
      if (entry % 100000 == 0) std::cout << "reading entry " << entry << std::endl ;


      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;

      //PG loop on associations vector
      for (unsigned int i = 0 ; 
           i < associations.size () ; 
           ++i)
        {
          int MUindex = associations.at (i).first  ;
          int SCindex = associations.at (i).second ;

          TVector3 SC0_pos (treeVars.superClusterX[SCindex], 
                            treeVars.superClusterY[SCindex], 
                            treeVars.superClusterZ[SCindex]) ; 
	       
          TVector3 MuonDir (treeVars.muonMomentumX[MUindex], 
                            treeVars.muonMomentumY[MUindex], 
                            treeVars.muonMomentumZ[MUindex]) ;

          float dummyEmax = 0.;
          float dummyLmax = 0.;
          int numCrystalEMax = -1;
          int numCrystalLMax = -1;
          bool SclOk = false;
          double dummyLength = 0;
       
          for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
               XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                          treeVars.nXtalsInSuperCluster[SCindex] ; 
               ++XTLindex)
            {
              if(treeVars.xtalTkLength[XTLindex] == -1) continue;
          
              dummyLength+= treeVars.xtalTkLength[XTLindex];
          
              //---- check the link Xtal with max energy  == Xtal with max length ----
              if (treeVars.xtalEnergy[XTLindex] > dummyEmax) numCrystalEMax = XTLindex;
              if(treeVars.xtalTkLength[XTLindex] > dummyLmax) numCrystalLMax = XTLindex;
            }
       
          //   if( abs(treeVars.muonTkLengthInEcalDetail[associations.at(i).first] - dummyLength) > 0.5) continue;
       
          if ( (numCrystalEMax != numCrystalLMax) && 
               (numCrystalEMax != -1) && 
               (numCrystalLMax != -1)) 
          {
            if ( 3.*treeVars.xtalEnergy[numCrystalLMax] < 
                 treeVars.xtalTkLength[numCrystalLMax] * 0.0125) SclOk = false;
          }
          else SclOk = true;
          if ((numCrystalEMax == -1) || (numCrystalLMax == -1)) SclOk = false;
          if(SclOk == false) continue;

          double SCphi = fabs(SC0_pos.Phi()) / 3.1415 * 180. ;
          if ( (SCphi < 90. - phiWINDOW/2) || (SCphi > 90. + phiWINDOW/2) ) continue;

          int SCieta = SC0_pos.Eta () / 0.0175 ;
          if (fabs (SCieta) > ietaMAX) continue ;
        
          double angle = MuonDir.Angle ( SC0_pos ) ;
          if( angle > 3.1415/2. ) angle = 3.1415 - angle; // angle belongs to [0:90]

          if ((angle < angleMIN) || (angle >= angleMAX)) continue ;

          if ((treeVars.superClusterRawEnergy[SCindex] >= EnergyMaxSC) || 
              (treeVars.superClusterRawEnergy[SCindex] < EnergyMinSC)) continue ; 

          std::pair <int,int> maxima = findMaxXtalsInSC (treeVars, SCindex) ;
          double XtalEnergyMax = treeVars.xtalEnergy[maxima.first] ;

          if ((XtalEnergyMax < XtalMaxEnergyMin) || 
              (XtalEnergyMax >= XtalMaxEnergyMax)) continue ;
          
          //loop su cristalli di Supercluster Associato
          for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
                   XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                              treeVars.nXtalsInSuperCluster[SCindex] ;
                   ++XTLindex)
            {
              if (treeVars.xtalEnergy[XTLindex] < XtalMinEnergy) continue ;               
              if (treeVars.xtalTkLength[XTLindex] <= 0.) continue ;
              double eox = treeVars.xtalEnergy[XTLindex] / 
                           treeVars.xtalTkLength[XTLindex] ;
              EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
              aveEoxMap.Fill (dummy.iphi (), dummy.ieta (), eox) ;
            }
        }
    } //PG loop over entries

  TH1F aveEoxDistr ("aveEoxDistr","aveEoxDistr",500,0,0.5) ;
  for (int phiIndex = 1 ; phiIndex < 361 ; ++phiIndex)
    for (int etaIndex = 1 ; etaIndex < 173 ; ++etaIndex)
      aveEoxDistr.Fill (aveEoxMap.GetBinContent (phiIndex,etaIndex)) ;

  TFile saving ("singleXtalEox.root","recreate") ;
  saving.cd () ;  
  aveEoxMap.Write () ;
  aveEoxDistr.Write () ;
  saving.Close () ;

  return 0 ;
}



#include <iostream>
#include <string>

//PG CMSSW includes
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
//#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

//PG root includes
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TProfile2D.h"
#include "TLorentzVector.h"
#include "TApplication.h"

//MF read CFG files includes
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>


#include "CRUZET/NTuplesTest/interface/NTuplesUtils.h"

//#include "CRUZET/Calibration/interface/AssociationUtils.h"

int main (int argc, char** argv)
{

  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
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


  //MF CUTS VALUES ON ANGLES AND SC ENERGY
  
  //cuts on SC Energy
  double EnergyMaxSC = subPSetSelections.getParameter<double> ("EnergyMaxSC") ;
  double EnergyMinSC = subPSetSelections.getParameter<double> ("EnergyMinSC") ;
  
  //cuts on Angle Muon / SCdirection
  double angleMAX = subPSetSelections.getParameter<double> ("angleMAX") ;
  double angleMIN = subPSetSelections.getParameter<double> ("angleMIN") ;
 
  //cuts on Xtal Energy
  double XtalMaxEnergy = subPSetSelections.getParameter<double> ("XtalMaxEnergyMin") ;
  
  //cuts on Windows
  double phiWINDOW    = subPSetSelections.getParameter<double> ("phiWINDOW") ;
  double ietaMAX    = subPSetSelections.getUntrackedParameter<int> ("ietaMAX",85) ;

  std::cout << ">>> Entering XtalsDistr program <<<" << std::endl;

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  TH2F SCdistr ("SCdistr","SCdistr",360,-3.1416,3.1416,170,-1.5,1.5) ;

  TH1F EnergySConTrackLength ("EnergySConTrackLength","EnergySConTrackLength", 500, 0., 0.5) ;    
  
  TH2F OccupancyXtals ("OccupancyXtals","OccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F xtalEnergyMap("xtalEnergyMap","xtalEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile2D aveEnergyMap ("aveEnergyMap","aveEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile aveEnergyMapVsEta ("aveEnergyMapVsEta", "aveEnergyMapVsEta", 172, -86, 86);
  TProfile aveEnergyMapVsPhi ("aveEnergyMapVsPhi", "aveEnergyMapVsPhi", 360, 1, 360);
  TH2F EnergySCVSTrackLength ("EnergySCVSTrackLength", "EnergySCVSTrackLength", 400, 0., 40., 500,0., 5.);

  TH1F Emax ("Emax","Emax",100,0.,1.) ;
  TH1F Emin ("Emin","Emin",100,0.,1.) ;
  TH1F Angle("Angle", "Angle", 180, 0., 3.1415);
 
  TH1F AngleWCutEnergy("AngleWCutEnergy", "AngleWCutEnergy", 180, 0., 3.1415);

  TH1F SuperClusterEnergyWCutAngle ("SuperClusterEnergyWCutAngle","SuperClusterEnergyWCutAngle",100,0.,5.) ;
  TH1F SuperClusterEnergy ("SuperClusterEnergy","SuperClusterEnergy",100,0.,5.) ;
  TH2F AngleVsSCEnergy ("AngleVsSCEnergy","AngleVsSCEnergy",360, 0., 3.1415, 500,0.,5.) ;     

  TH2F cutOccupancyXtals ("cutOccupancyXtals","cutOccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F cutxtalEnergyMap("cutxtalEnergyMap","cutxtalEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile2D cutaveEnergyMap ("cutaveEnergyMap","cutaveEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile cutaveEnergyMapVsEta ("cutaveEnergyMapVsEta", "cutaveEnergyMapVsEta", 172, -86, 86);
  TProfile cutaveEnergyMapVsPhi ("cutaveEnergyMapVsPhi", "cutaveEnergyMapVsPhi", 360, 1, 360);


  
  double XtalEnergyMax = 0;		
  double XtalEnergyMin = 0;
  
  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;

      if (entry % 10000 == 0) std::cout << "reading entry " << entry << std::endl ;

 
      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;

      double angle = -99;  
      
      //PG loop on associations vector
      for (unsigned int i = 0 ; i < associations.size () ; ++i)

        {
          int MUindex = associations.at (i).first ;
          int SCindex = associations.at (i).second ;     
          
	  //if (treeVars.muonTkLengthInEcalDetail[MUindex] < 1) continue;   // length > 0        

          TVector3 SC0_pos (treeVars.superClusterX[SCindex], 
                            treeVars.superClusterY[SCindex], 
                            treeVars.superClusterZ[SCindex]) ; 
	       
          TVector3 MuonDir (treeVars.muonPx[MUindex], 
                            treeVars.muonPy[MUindex], 
                            treeVars.muonPz[MUindex]) ;
          	  
          double SCphi = fabs(SC0_pos.Phi()) / 3.1415 * 180. ;
          if ( (SCphi < 90. - phiWINDOW/2) || (SCphi > 90. + phiWINDOW/2) ) continue;

          int SCieta = int (SC0_pos.Eta () / 0.0175) ;
          if (fabs (SCieta) > ietaMAX) continue ;
        
          angle = MuonDir.Angle( SC0_pos ) ;
          if( angle > 3.1415/2. ) angle = 3.1415 - angle; // angle belongs to [0:90]
          Angle.Fill(angle);
	  
	 
	  double SCEnergy = 0;		
	  XtalEnergyMax = 0;		
	  XtalEnergyMin = 0;
	  
	  //loop su cristalli di Supercluster Associato
	  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
               XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
		 treeVars.nXtalsInSuperCluster[SCindex] ;
               ++XTLindex)
            {
                EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
	        OccupancyXtals.Fill(dummy.iphi(), dummy.ieta());
	        xtalEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
	        aveEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
	        aveEnergyMapVsEta.Fill(dummy.ieta(),treeVars.xtalEnergy[XTLindex]);
	        aveEnergyMapVsPhi.Fill(dummy.iphi(),treeVars.xtalEnergy[XTLindex]);
	    
	    
	        SCEnergy = SCEnergy + treeVars.xtalEnergy[XTLindex];	    
	        //MF determino energia max ed energia minima dei cristalli
	        if (treeVars.xtalEnergy[XTLindex] >= XtalEnergyMax) XtalEnergyMax = treeVars.xtalEnergy[XTLindex];
	        if (XTLindex == treeVars.xtalIndexInSuperCluster[SCindex]) XtalEnergyMin = treeVars.xtalEnergy[XTLindex];
	        if (treeVars.xtalEnergy[XTLindex] <= XtalEnergyMin) XtalEnergyMin = treeVars.xtalEnergy[XTLindex];     
	    }
	    
	 Emax.Fill(XtalEnergyMax);
	 Emin.Fill(XtalEnergyMin);    
	 AngleVsSCEnergy.Fill(angle,treeVars.superClusterRawEnergy[SCindex]);
	 SuperClusterEnergy.Fill(treeVars.superClusterRawEnergy[SCindex]);
	 //MF Energy with selections on angles
	 if((angle >= angleMIN) && (angle <= angleMAX)) 
	 {
	 SuperClusterEnergyWCutAngle.Fill(treeVars.superClusterRawEnergy[SCindex]);
	  }
         EnergySCVSTrackLength.Fill(treeVars.muonTkLengthInEcalDetail[MUindex], treeVars.superClusterRawEnergy[SCindex]);
 
          EnergySConTrackLength.Fill(treeVars.superClusterRawEnergy[SCindex]/treeVars.muonTkLengthInEcalDetail[MUindex]);
         //MF Angle with selections on energy
	 if((XtalEnergyMax >= XtalMaxEnergy) && (SCEnergy <= EnergyMaxSC) && (SCEnergy >= EnergyMinSC)) AngleWCutEnergy.Fill(angle);
	 
	 
	 //PLOTS CON CUTS
	   if ( (XtalEnergyMax >= XtalMaxEnergy) && (SCEnergy <= EnergyMaxSC) && (SCEnergy >= EnergyMinSC) && (angle >= angleMIN) && (angle <= angleMAX))	//MF loop on crystals with CUTS    

	   { 
              
	        for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
                     XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] + treeVars.nXtalsInSuperCluster[SCindex] ;
                     ++XTLindex)
                     {
	             EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
	    	     cutOccupancyXtals.Fill(dummy.iphi(), dummy.ieta());
	   	     cutxtalEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
	   	     cutaveEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
	    	     cutaveEnergyMapVsEta.Fill(dummy.ieta(),treeVars.xtalEnergy[XTLindex]);
	  	     cutaveEnergyMapVsPhi.Fill(dummy.iphi(),treeVars.xtalEnergy[XTLindex]);

	             }     

	    }



	  
	  
       }


  
	

 
	
    } //PG loop over entries

  TFile saving ("OutputXtals.root" ,"recreate") ;
  saving.cd () ;  
  SCdistr.Write () ;
  Emax.Write();
  Emin.Write();
  xtalEnergyMap.Write();
  OccupancyXtals.Write() ;
  aveEnergyMap.Write();
  aveEnergyMapVsPhi.Write();
  aveEnergyMapVsEta.Write();
  cutxtalEnergyMap.Write();
  cutOccupancyXtals.Write() ;
  cutaveEnergyMap.Write();
  Angle.Write();
  SuperClusterEnergyWCutAngle.Write();
  SuperClusterEnergy.Write();
  cutaveEnergyMapVsPhi.Write();
  cutaveEnergyMapVsEta.Write();
  AngleWCutEnergy.Write();
  EnergySCVSTrackLength.Write();
  EnergySConTrackLength.Write();
  AngleVsSCEnergy.Write();
  saving.Close () ;
  
  return 0 ;
}


double 
calcTheta (double eta) 
{
  return 2 * atan (exp ( -1 * eta));
}


void
setVectorOnECAL (TVector3 & vector, double eta, double phi, double radius) 
{
  double theta = calcTheta (eta) ;
  TVector3 dummy (
    radius * cos (phi), 
    radius * sin (phi),
    radius / tan (theta) 
     ) ;
  vector = dummy ;

}



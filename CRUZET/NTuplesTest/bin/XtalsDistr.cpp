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

#include "CRUZET/Calibration/interface/CRUtils.h"

//#include "CRUZET/Calibration/interface/AssociationUtils.h"


//! main program
int main ()
{





  //MF CUTS VALUES!
  double EnergyMaxCUT = 2;
  double EnergyMinCUT = 1;
  double angleMAX = 50;
  double angleMIN = 10;





  std::cout << ">>> Entering CosmicTreeTest program <<<" << std::endl;

  TH2F SCdistr ("SCdistr","SCdistr",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TH2F OccupancyXtals ("OccupancyXtals","OccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F xtalEnergyMap("xtalEnergyMap","xtalEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile2D aveEnergyMap ("aveEnergyMap","aveEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile aveEnergyMapVsEta ("aveEnergyMapVsEta", "aveEnergyMapVsEta", 172, -86, 86);
  TProfile aveEnergyMapVsPhi ("aveEnergyMapVsPhi", "aveEnergyMapVsPhi", 360, 1, 360);
  
  
   TH2F ASSOccupancyXtals ("ASSOccupancyXtals","ASSOccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F ASSxtalEnergyMap("ASSxtalEnergyMap","ASSxtalEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile2D ASSaveEnergyMap ("ASSaveEnergyMap","ASSaveEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile ASSaveEnergyMapVsEta ("ASSaveEnergyMapVsEta", "ASSaveEnergyMapVsEta", 172, -86, 86);
  TProfile ASSaveEnergyMapVsPhi ("ASSaveEnergyMapVsPhi", "ASSaveEnergyMapVsPhi", 360, 1, 360);
  
  TH1F Emax ("Emax","Emax",100,0.,1.) ;
  TH1F Emin ("Emin","Emin",100,0.,1.) ;
  TH1F Angle("Angle", "Angle", 180, 0., 3.1415);
  TH1F SuperClusterEnergy ("SuperClusterEnergy","SuperClusterEnergy",100,0.,5.) ;
	 
	 
	 
  TH2F cutOccupancyXtals ("cutOccupancyXtals","cutOccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F cutxtalEnergyMap("cutxtalEnergyMap","cutxtalEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile2D cutaveEnergyMap ("cutaveEnergyMap","cutaveEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile cutaveEnergyMapVsEta ("cutaveEnergyMapVsEta", "cutaveEnergyMapVsEta", 172, -86, 86);
  TProfile cutaveEnergyMapVsPhi ("cutaveEnergyMapVsPhi", "cutaveEnergyMapVsPhi", 360, 1, 360);
  
  
  // Tree construction
  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
  chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509081.tree.root"); 
chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509082.tree.root"); 
chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509083.tree.root"); 
chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509084.tree.root"); 
chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509085.tree.root"); 
chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509086.tree.root"); 
chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509087.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509088.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-509089.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090810.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090811.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090812.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090813.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090814.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090815.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090816.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090817.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090818.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090819.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090820.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090821.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090822.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090823.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090824.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090825.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090826.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090827.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090828.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090829.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090830.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090831.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090832.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090833.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090834.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090835.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090836.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090837.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090838.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090839.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090840.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090841.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090842.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090843.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090844.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090845.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090846.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090847.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090848.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090849.tree.root"); 
// chain->Add("/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-5090850.tree.root"); 
 
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    


  
  double EnergyMax = 0;		
  double EnergyMin = 0;
  
  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;

     //MF Selections on angles	
     //MF association between muons and superclusters
 
      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;

      
      double angle = -99;  

      //PG loop on associations vector
      for (unsigned int i = 0 ; i < associations.size () ; ++i)
	{
	  int MUindex = associations.at (i).first  ;
	  int SCindex = associations.at (i).second; 	
	  if (treeVars.muonTkLengthInEcalDetail[MUindex] < 1) continue;   // length > 0		
	  TVector3 SC0_pos (0., 0., 0.) ;
	  setVectorOnECAL (SC0_pos, treeVars.superClusterEta[SCindex], treeVars.superClusterPhi[SCindex],1);
			
	  TVector3 MuonDir (treeVars.muonMomentumX[MUindex], treeVars.muonMomentumY[MUindex], treeVars.muonMomentumZ[MUindex]);
			
	  angle = MuonDir.Angle( SC0_pos ) ;
	  if( angle > 3.1415/2. ) angle = 3.1415 - angle; // angle belongs to [0:90]
	  Angle.Fill(angle);
	 
	 
	 for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
               XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
		 treeVars.nXtalsInSuperCluster[SCindex] ;
               ++XTLindex)
            {
	    EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
	    ASSOccupancyXtals.Fill(dummy.iphi(), dummy.ieta());
	    ASSxtalEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
	    ASSaveEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
	    ASSaveEnergyMapVsEta.Fill(dummy.ieta(),treeVars.xtalEnergy[XTLindex]);
	    ASSaveEnergyMapVsPhi.Fill(dummy.iphi(),treeVars.xtalEnergy[XTLindex]);
	    
	                  
	      }
	  
	  
	}
	





      //PG loop on superclusters
      for (int SCindex = 0 ; 
           SCindex < treeVars.nSuperClusters ; 
           ++SCindex)
        {
          SCdistr.Fill (treeVars.superClusterPhi[SCindex],
                        treeVars.superClusterEta[SCindex]) ;             
	
			
	 double SCEnergy = 0;		
	 EnergyMax = 0;		
	 EnergyMin = 0;
	//MF loop on crystals in SClusters	
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
	    
	    //MF determino energia max ed energia minima
	    if (treeVars.xtalEnergy[XTLindex] >= EnergyMax) EnergyMax = treeVars.xtalEnergy[XTLindex];
	    if (XTLindex == treeVars.xtalIndexInSuperCluster[SCindex]) EnergyMin = treeVars.xtalEnergy[XTLindex];
	    if (treeVars.xtalEnergy[XTLindex] <= EnergyMin) EnergyMin = treeVars.xtalEnergy[XTLindex];
	                    
	      }
          SuperClusterEnergy.Fill(SCEnergy);
         //PLOTS CON CUTS
	   if ( (SCEnergy <= EnergyMaxCUT) && (SCEnergy >= EnergyMinCUT) && (angle >= angleMIN) && (angle <= angleMAX))	//MF loop on crystals with CUTS    

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


	Emax.Fill(EnergyMax);
	Emin.Fill(EnergyMin);

	//std::cout << "energy max , min " << EnergyMax << " " << EnergyMin << std::e
	    
	
	    
        } //PG loop on superclusters
	
	

      

	

	
	
	
	
	
	
	
	
	
    } //PG loop over entries

  TFile saving ("XtalsOutput.root","recreate") ;
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
  SuperClusterEnergy.Write();
  cutaveEnergyMapVsPhi.Write();
  cutaveEnergyMapVsEta.Write();
  ASSxtalEnergyMap.Write();
  ASSOccupancyXtals.Write() ;
  ASSaveEnergyMap.Write();
  ASSaveEnergyMapVsPhi.Write();
  aSSaveEnergyMapVsEta.Write();


  saving.Close () ;

  return 0 ;
}


double 
calcTheta (double eta) 
{
  return 2 * atan (exp ( -1 * eta)) ;ASS
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



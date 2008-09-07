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
int main (int argc, char** argv)
{

  std::string outputRootName = "changeme.root" ;
  std::string testName = "-o";
  if (argc>2){
      if (argv[1] == testName) {
        outputRootName = argv[2] ;
      }
  }
  
  std::string testHelp = "--help";
  if (argc==2){
      if (argv[1] == testHelp) {
          std::cout << "Help" << std::endl ;
          std::cout << " --help : display help" << std::endl ;
          std::cout << " -o : output root file name (eg histograms.root)" << std::endl ;
          std::cout << " name of input file : list name of input files ntuples" << std::endl ;     
          exit(1);
      }
  }
    
  if (argc < 2)
    {
      std::cerr << "ERROR : ntuple name missing" << std::endl ;
      exit (1) ;
    }

  //MF CUTS VALUES!
  double EnergyMaxCUT = 15;
  double EnergyMinCUT = 0;
  double angleMAX = 0.3925;
  double angleMIN = 0.;


  std::cout << ">>> Entering XtalsDistr program <<<" << std::endl;

  
  // Tree construction
  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;

  if (argv[1] == testName) {
      for (int i=3; i< (argc); i++) chain->Add (argv[i]) ;
  }
  else{
      for (int i=1; i< (argc); i++) chain->Add (argv[i]) ;
  }
 
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  TH2F SCdistr ("SCdistr","SCdistr",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TH2F OccupancyXtals ("OccupancyXtals","OccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F xtalEnergyMap("xtalEnergyMap","xtalEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile2D aveEnergyMap ("aveEnergyMap","aveEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile aveEnergyMapVsEta ("aveEnergyMapVsEta", "aveEnergyMapVsEta", 172, -86, 86);
  TProfile aveEnergyMapVsPhi ("aveEnergyMapVsPhi", "aveEnergyMapVsPhi", 360, 1, 360);

  
  TH1F Emax ("Emax","Emax",100,0.,1.) ;
  TH1F Emin ("Emin","Emin",100,0.,1.) ;
  TH1F Angle("Angle", "Angle", 180, 0., 3.1415);
  TH1F SuperClusterEnergy ("SuperClusterEnergy","SuperClusterEnergy",100,0.,5.) ;
  
  TH2F cutOccupancyXtals ("cutOccupancyXtals","cutOccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F cutxtalEnergyMap("cutxtalEnergyMap","cutxtalEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile2D cutaveEnergyMap ("cutaveEnergyMap","cutaveEnergyMap",360,1.,361.,172,-86.,86.);     
  TProfile cutaveEnergyMapVsEta ("cutaveEnergyMapVsEta", "cutaveEnergyMapVsEta", 172, -86, 86);
  TProfile cutaveEnergyMapVsPhi ("cutaveEnergyMapVsPhi", "cutaveEnergyMapVsPhi", 360, 1, 360);

  double EnergyMax = 0;        
  double EnergyMin = 0;
  
  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;

      if (entry % 10000 == 0) std::cout << "reading entry " << entry << std::endl ;
     //MF Selections on angles    
     //MF association between muons and superclusters
 
      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;
      
      double angle = -99;  
      
      //PG loop on associations vector
      for (unsigned int i = 0 ; i < associations.size () ; ++i)
        {
          int MUindex = associations.at (i).first ;
          int SCindex = associations.at (i).second ;     
//PG FIXME come mai c'era questo taglio?
//          if (treeVars.muonTkLengthInEcalDetail[MUindex] < 1) continue;   // length > 0        
          //TVector3 SC0_pos (0., 0., 0.) ;
          //setVectorOnECAL (SC0_pos, 
          //                 treeVars.superClusterEta[SCindex], 
          //                 treeVars.superClusterPhi[SCindex],
          //                 1) ;
          TVector3 SC0_pos (treeVars.superClusterX[SCindex], 
                            treeVars.superClusterY[SCindex], 
                            treeVars.superClusterZ[SCindex]) ; 
	       
          TVector3 MuonDir (treeVars.muonMomentumX[MUindex], 
                            treeVars.muonMomentumY[MUindex], 
                            treeVars.muonMomentumZ[MUindex]) ;
                
          angle = MuonDir.Angle( SC0_pos ) ;
          if( angle > 3.1415/2. ) angle = 3.1415 - angle; // angle belongs to [0:90]
          Angle.Fill(angle);
         
          SCdistr.Fill (treeVars.superClusterPhi[SCindex],
                        treeVars.superClusterEta[SCindex]) ;             
                  
          double SCEnergy = 0;        
          EnergyMax = 0;        
          EnergyMin = 0;

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
          if ( (SCEnergy <= EnergyMaxCUT) && 
               (SCEnergy >= EnergyMinCUT) && 
               (angle >= angleMIN) && 
               (angle <= angleMAX))    //MF loop on crystals with CUTS    
            { 
               for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
                    XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] + 
                               treeVars.nXtalsInSuperCluster[SCindex] ;
                    ++XTLindex)
                 {
                   EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
                   cutOccupancyXtals.Fill(dummy.iphi(), dummy.ieta());
                   cutxtalEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
                   cutaveEnergyMap.Fill(dummy.iphi(), dummy.ieta(), treeVars.xtalEnergy[XTLindex]);
                   cutaveEnergyMapVsEta.Fill(dummy.ieta(),treeVars.xtalEnergy[XTLindex]);
                   cutaveEnergyMapVsPhi.Fill(dummy.iphi(),treeVars.xtalEnergy[XTLindex]);
                 }     
            } //PLOTS CON CUTS
          Emax.Fill(EnergyMax);
          Emin.Fill(EnergyMin);

        } //PG loop on associations vector
    
	
    } //PG loop over entries

  TFile saving (outputRootName.c_str () ,"recreate") ;
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



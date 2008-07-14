//COMPILO c++ -o Mappe -lm `root-config --cflags --libs --glibs` Mappe.cpp CRUtils.cc

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3F.h"
#include "TText.h"
#include "TLegend.h"
#include "TF1.h"
#include "TApplication.h"
#include "CRUtils.h"

// ----------------------------------------------------


int main (int argc, char** argv)
{


  gROOT->SetStyle("Plain"); 
  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
  chain->Add ("/afs/cern.ch/user/m/mattia/MuonTree_43439/MuonTree_43439_*.root") ;

  EcalCosmicsAnalysisVariables treeVars ;

  chain->SetBranchAddress ("runId", &treeVars.runId) ;                           
  chain->SetBranchAddress ("eventId", &treeVars.eventId) ;                       
  chain->SetBranchAddress ("timeStampLow", &treeVars.timeStampLow) ;             
  chain->SetBranchAddress ("timeStampHigh", &treeVars.timeStampHigh) ;           
  chain->SetBranchAddress ("isECALL1", &treeVars.isECALL1) ;                     
  chain->SetBranchAddress ("isHCALL1", &treeVars.isHCALL1) ;                     
  chain->SetBranchAddress ("isDTL1", &treeVars.isDTL1) ;                         
  chain->SetBranchAddress ("isRPCL1", &treeVars.isRPCL1) ;                       
  chain->SetBranchAddress ("isCSCL1", &treeVars.isCSCL1) ;                       
  chain->SetBranchAddress ("nCosmicsCluster", &treeVars.nCosmicsCluster) ;       
  chain->SetBranchAddress ("cosmicClusterEnergy", treeVars.cosmicClusterEnergy) ;
  chain->SetBranchAddress ("cosmicClusterE1", treeVars.cosmicClusterE1) ;        
  chain->SetBranchAddress ("cosmicClusterE2", treeVars.cosmicClusterE2) ;        
  chain->SetBranchAddress ("cosmicClusterE9", treeVars.cosmicClusterE9) ;        
  chain->SetBranchAddress ("cosmicClusterE25", treeVars.cosmicClusterE25) ;      
  chain->SetBranchAddress ("cosmicClusterTime", treeVars.cosmicClusterTime) ;    
  chain->SetBranchAddress ("cosmicClusterEta", treeVars.cosmicClusterEta) ;      
  chain->SetBranchAddress ("cosmicClusterPhi", treeVars.cosmicClusterPhi) ;      
  chain->SetBranchAddress ("cosmicClusterXtals", treeVars.cosmicClusterXtals) ;  
  chain->SetBranchAddress ("cosmicClusterXtalsAbove3Sigma", treeVars.cosmicClusterXtalsAbove3Sigma) ;    
  chain->SetBranchAddress ("cosmicClusterMaxId", treeVars.cosmicClusterMaxId) ;      
  chain->SetBranchAddress ("cosmicCluster2ndId", treeVars.cosmicCluster2ndId) ;      
  chain->SetBranchAddress ("nRecoMuons", &treeVars.nRecoMuons) ;                     
  chain->SetBranchAddress ("muonPt", treeVars.muonPt) ;                              
  chain->SetBranchAddress ("muonEta", treeVars.muonEta) ;                            
  chain->SetBranchAddress ("muonPhi", treeVars.muonPhi) ;                            
  chain->SetBranchAddress ("muonNChi2", treeVars.muonNChi2) ;                        
  chain->SetBranchAddress ("muonNDof", treeVars.muonNDof) ;                          
  chain->SetBranchAddress ("muonNHits", treeVars.muonNHits) ;                        
  chain->SetBranchAddress ("muonCharge", treeVars.muonCharge) ;                      
  chain->SetBranchAddress ("muonQOverP", treeVars.muonQOverP) ;                      
  chain->SetBranchAddress ("muond0", treeVars.muond0) ;                              
  chain->SetBranchAddress ("muondz", treeVars.muondz) ;                              
  chain->SetBranchAddress ("muonTkAtEcalEta", treeVars.muonTkAtEcalEta) ;            
  chain->SetBranchAddress ("muonTkAtEcalPhi", treeVars.muonTkAtEcalPhi) ;            
  chain->SetBranchAddress ("muonTkAtHcalEta", treeVars.muonTkAtHcalEta) ;            
  chain->SetBranchAddress ("muonTkAtHcalPhi", treeVars.muonTkAtHcalPhi) ;            
  chain->SetBranchAddress ("muonEcalEnergy3x3", treeVars.muonEcalEnergy3x3) ;        
  chain->SetBranchAddress ("muonEcalEnergy5x5", treeVars.muonEcalEnergy5x5) ;        
  chain->SetBranchAddress ("muonEcalEnergyCrossed", treeVars.muonEcalEnergyCrossed) ;
  chain->SetBranchAddress ("muonHcalEnergy3x3", treeVars.muonHcalEnergy3x3) ;        
  chain->SetBranchAddress ("muonHcalEnergyCrossed", treeVars.muonHcalEnergyCrossed) ;
  chain->SetBranchAddress ("muonNCrossedEcalDetId", treeVars.muonNCrossedEcalDetId) ;
  chain->SetBranchAddress ("muonMaxEneEcalDetIdCrossed", treeVars.muonMaxEneEcalDetIdCrossed) ; 
  chain->SetBranchAddress ("cosmicClusterEnergyXtals", treeVars.cosmicClusterEnergyXtals) ;
  chain->SetBranchAddress ("cosmicClusterLengthXtals_0", treeVars.cosmicClusterLengthXtals_0) ; 
  chain->SetBranchAddress ("cosmicClusterLengthXtals_1", treeVars.cosmicClusterLengthXtals_1) ; 

  TApplication *theApp = new TApplication( "app", &argc, argv );

  //eta phi
  TH2F Occupancy("Occupancy","Occupancy",170,-1.47,1.47,360,-3.14,3.14); 
  TH2F EnergyOnCrystals("EnergyOnCrystals","EnergyOnCrystals",170,-1.47,1.47,360,-3.14,3.14);
  TH2F MeanEnergy("Occupancy","Occupancy",170,-85,85,360,0,360);  
  TH2F EoPCrystals("EoPCrystals","EoPCrystals",170,-1.47,1.47,360,-3.14,3.14);
  
 
  int nEvents = (int) chain->GetEntries () ;
  //PG loop over entries
  for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)
    //for (int iEvent = 0 ; iEvent < 1000000 ; ++iEvent)
    {
      if(iEvent%10000 == 0) std::cout << "event n. " << iEvent << std::endl;
      
      chain->GetEntry (iEvent) ;
     
     //base selections
      if (treeVars.nCosmicsCluster != 2) continue ;
      if(deltaPhi(treeVars.cosmicClusterPhi[0],treeVars.cosmicClusterPhi[1]) < 3.14159265358979 / 2./*90gradi*/) continue; //TAGLIO IN DELTAPHI
      
      
      
      Occupancy.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterPhi[0]);
      Occupancy.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterPhi[1]);
      EnergyOnCrystals.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterPhi[0],treeVars.cosmicClusterE1[0]);
      EnergyOnCrystals.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterPhi[1],treeVars.cosmicClusterE1[1]);
     //EoPCrystals.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterPhi[0],enerTop/lunghTop);
     //EoPCrystals.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterPhi[1],enerBot/lunghBot); 
      
      
       
    } //PG loop over entries 
  
    /*
    //MF loop over crystals    
    int nOverCrystals = 0; //numero eventi per cristallo
    int EOverCrystals = 0; //energia x cristallo
    for(int indexeta = 0 ; indexeta < 170 ; indexeta++)  // eta 170,-1.47,1.47,
    for(int indexphi = 0 ; indexphi < 360 ; indexphi++)  // phi 360,-3.14,3.14
    {
    nOverCrystals = Occupancy.GetBinContent(indexeta,indexphi);
    EOverCrystals = EnergyOnCrystals.GetBinContent(indexeta,indexphi);
    MeanEnergy.Fill(-85+indexeta,indexphi,EOverCrystals/nOverCrystals);
    }
    
    //MF loop over crystals
   */
    
  //Writing Histos
  TFile out ("Mappe.root","recreate") ;
  TDirectory* Rings = gDirectory->mkdir("Rings");
  TDirectory* Slices = gDirectory->mkdir("Slices");
  Occupancy.Write();
  EnergyOnCrystals.Write();
  EoPCrystals.Write();
  MeanEnergy.Write();
  
  return(0);
}

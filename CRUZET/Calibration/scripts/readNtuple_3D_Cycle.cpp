//COMPILO c++ -o readNtuple_3D_Cycle -lm `root-config --cflags --libs --glibs` readNtuple_3D_Cycle.cpp

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
#include "TStyle.h"



double calcTheta (double eta) ;
double calcEta (double theta) ;
double getEcalXLenght (const TVector3& G1,const TVector3& G2) ;
double get3DAlpha (const TVector3& G1,const TVector3& G2) ;
double get2DAlphaXY (const TVector3& G1,const TVector3& G2) ;
double get2DAlphaYZ (const TVector3& G1,const TVector3& G2) ;
bool isBefore (const TVector3& pOld) ;
double where (double deltaAlpha, const TVector3& G1,const TVector3& G2, double alphaOld);
std::pair<TVector3,TVector3> getEcalCross (const TVector3& G1,const TVector3& G2, double ECALrad) ;
std::pair<double,double> getAlphaEcalCross (const TVector3& G1,const TVector3& G2, double ECALrad) ;
double deltaPhi (double phi1,double phi2) ;
double get2DThickness (double z);
void setVectorOnECAL (TVector3 & vector, double eta, double phi, double radius = 129.) ;
double superGausCumCauda (double *x, double *par);

double twopi  = 2*acos(-1.);

// ----------------------------------------------------


  struct EcalCosmicsAnalysisVariables
  {
    unsigned int  runId; //
    unsigned int  eventId; // 
    unsigned int  timeStampLow; //
    unsigned int  timeStampHigh; //
    bool isECALL1; // 
    bool isRPCL1; // 
    bool isDTL1; //
    bool isCSCL1; //
    bool isHCALL1; //
    int nCosmicsCluster; //
    float cosmicClusterEnergy[100];//
    float cosmicClusterE1[100];//
    float cosmicClusterE2[100];//
    float cosmicClusterE9[100];
    float cosmicClusterE25[100];
    float cosmicClusterTime[100];// 
    float cosmicClusterEta[100];//
    float cosmicClusterPhi[100];//
    int cosmicClusterXtals[100];//
    int cosmicClusterXtalsAbove3Sigma[100];//
    unsigned int cosmicClusterMaxId[100];//
    unsigned int cosmicCluster2ndId[100];//
    int nRecoMuons; //
    float muonPt[20]; //
    float muonEta[20]; //
    float muonPhi[20]; //
    float muonNChi2[20]; //
    float muonNDof[20]; //
    float muonNHits[20]; //
    float muonCharge[20]; //
    float muonQOverP[20]; //
    float muond0[20]; //
    float muondz[20]; //
    int muonNCrossedEcalDetId[20]; //
    unsigned int muonMaxEneEcalDetIdCrossed[20];//
    float muonTkAtEcalEta[20];//
    float muonTkAtEcalPhi[20];//
    float muonTkAtHcalEta[20];//
    float muonTkAtHcalPhi[20];//
    float muonEcalEnergy3x3[20];//
    float muonEcalEnergy5x5[20];//
    float muonEcalEnergyCrossed[20];//
    float muonHcalEnergy3x3[20];// 
    float muonHcalEnergyCrossed[20];//
    float cosmicClusterEnergyXtals[100];//
    float cosmicClusterLengthXtals_0[100];//
    float cosmicClusterLengthXtals_1[100];//
  };


// ----------------------------------------------------


int main (int argc, char** argv)
{
<<<<<<< readNtuple_3D_Cycle.cpp
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);

=======
  gROOT->SetStyle("Plain"); 
>>>>>>> 1.2
  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
<<<<<<< readNtuple_3D_Cycle.cpp
  chain->Add ("~/public/MuonTreeLungTestMatrix_43439_*.root") ;
  //chain->Add ("/data/deguio/CRUZET/MuonTreeLungTestMatrix_43439/MuonTreeLungTestMatrix_43439_*.root") ;
=======
  chain->Add ("/afs/cern.ch/user/m/mattia/MuonTree_43439/MuonTree_43439_*.root") ;
>>>>>>> 1.2

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

  TH1F ip3D ("ip3D","3D impact parameter",100,0.,1000.) ;
  TH1F ip2DXY ("ip2DXY","2DXY impact parameter",100,0.,1000.) ;
  TH1F ip2DYZ ("ip2DYZ","2DYZ impact parameter",100,0.,1000.) ;
  TH2F SCdistr ("SCdistr","SCdistr",400,-200,200,400,-200,200) ;
  TH2F dEondX3DVSEta ("dEondX3DVSEta","dEondX3DVSEta",170,-1.5,1.5,500,0.,0.5) ;
  TH2F dX2DVSEta ("dX2DVSEta","dX2DVSEta",170,-1.5,1.5,600,0.,60.) ;
  TH2F dX2DVSZeta ("dX2DVSZeta","dX2DVSZeta",600,-300.,300.,600,0.,60.) ;
  TH2F dEondX2DXYVSEta ("dEondX2DXYVSEta","dEondX2DXYVSEta",170,-1.5,1.5,500,0.,0.5) ;   //170 == num xtals
  TH2F dEondX2DYZVSEta ("dEondX2DYZVSEta","dEondX2DYZVSEta",170,-1.5,1.5,500,0.,0.5) ;
  TH2F Spessore ("Spessore","Spessore",700,-370.,370,400,0.,200.) ;
  TH2F Radius ("Radius","Radius",1000,-250.,250.,1000,-250.,250.) ;
  TH2F RadiusTop ("RadiusTop","RadiusTop",1000,-250.,250.,1000,-250.,250.) ;
  TH2F RadiusBot ("RadiusBot","RadiusBot",1000,-250.,250.,1000,-250.,250.) ;
 //TH3F *Scan3D = new TH3F("Scan3D","Scan3D",500,-250.,250.,500,-250.,250.,700,-350.,350.) ;

 //MATTIA
  TH2F Occupancy("Occupancy","Occupancy",170,-1.47,1.47,360,-3.14,3.14); 
  TH2F EnergyOnCrystals("EnergyOnCrystals","EnergyOnCrystals",170,-1.47,1.47,360,-3.14,3.14); 
  TH2F EoPCrystals("EoPCrystals","EoPCrystals",170,-1.47,1.47,360,-3.14,3.14);
  
  
  TH1F dEondX_AllDir ("dEondX_AllDir","dEondX_AllDir",3000,0.,0.5) ;
  TH2F dEondXVSIP_AllDir  ("dEondXVSIP_AllDir","dEondXVSIP_AllDir",500,0.,500.,1000,0.,0.5) ;
  TH2F dEondXVSdX_AllDir ("dEondXVSdX_AllDir","dEondXVSdX_AllDir",100,0.,100.,1000,0.,0.5) ;
  TH1F lunghTop_h ("lunghTop_h","lunghTop_h",100,0.,100.) ;
  TH1F lunghBot_h ("lunghBot_h","lunghBot_h",100,0.,100.) ;
  TH2F dEondXVSAngle_AllDir ("dEondXVSAngle_AllDir","dEondXVSAngle_AllDir",180,0.,twopi/2,1000,0.,0.5);
  TH1F AngleTop ("AngleTop","AngleTop",180,0.,twopi/2);
  TH1F AngleBot ("AngleBot","AngleBot",180,0.,twopi/2);
  TH2F dXVSIP_transverse ("dXVSIP_transverse","dXVSIP_transverse",135,0.,135.,100,0.,100.);
  TH2F EVSdX ("EVSdX","EVSdX",300,0.,100.,2000,0.,20.);
  TH2F EnergyVSclusterSize ("EnergyVSclusterSize","EnergyVSclusterSize",25,0.,25.,2000,0.,20.);
  TH1F dEondX_AllDirTop ("dEondX_AllDirTop","dEondX_AllDirTop",3000,0.,0.5) ;
  TH1F dEondX_AllDirBot ("dEondX_AllDirBot","dEondX_AllDirBot",3000,0.,0.5) ;
  TH1F diff_dEondX ("diff_dEondX","diff_dEondX sotto - sopra",2000,-2.,2.);
  TH1F diff_ring ("diff_ring","eta positive - eta negative",5000,-1.,1.);
  TH1F diff_slices ("diff_slices","eta positive - eta negative",3000,0.,0.5);
  //-------lungh-------
  TH1F dEondX_lung ("dEondX_lung","dEondX_lung",3000,0.,0.5);
  TH2F lunghVSE ("lunghVSE","lunghVSE",3000,0.,30.,2500,0.,25.);
  TH1F dEondX_lung_sum ("dEondX_lung_sum","dEondX_lung_sum",3000,0.,0.5);
  TH2F dEondX_lung_sumVSeta ("dEondX_lung_sumVSeta","dEondX_lung_sumVSeta",170,-1.5,1.5,3000,0.,0.5);
  TH1F diff_lungh ("diff_lungh","diff_lungh: geom - approx",500,-50.,50.);
  TH2F diff_lungVSeta ("diff_lungVSeta","diff_lungVSeta",170,-1.5,1.5,500,-50.,50.);
  TH1F zero ("zero","zero",500,0.,100.);
  TH1F uno ("uno","uno",500,0.,100.);
  TH1F zeroPlusUno ("zeroPlusUno","zeroPlusUno",500,0.,100.);

  TH2F Occupancy("Occupancy","Occupancy",170,-1.47,1.47,360,-3.14,3.14); 
  



  //analysis SM by SM (5 by 5)
  TH1F *energy_rings[34];
  for(int ring = -17; ring < 17; ++ring)
    {
      char ring_num[80];
      sprintf (ring_num,"energy_ring_%d",ring);
      energy_rings[ring+17] = new TH1F (ring_num,ring_num,2000,0.,20.);
    }
  //analysis slice by slice
  TH1F *energy_slice[34];
  for(int slice = -17; slice < 17; ++slice)
    {
      char slice_num[80];
      sprintf (slice_num,"energy_slice_%d",slice);
      energy_slice[slice+17] = new TH1F (slice_num,slice_num,2000,0.,20.);
    }
  
  
  double ECALinRad = 129. ;
  double g_deltaAlpha = 0.3 ;
  TVector3 Vertex (0.,0.,0.);
  int nEvents = (int) chain->GetEntries () ;
  //PG loop over entries
  for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)
    //for (int iEvent = 0 ; iEvent < 1000000 ; ++iEvent)
    {
      if(iEvent%10000 == 0) std::cout << "event n. " << iEvent << std::endl;
      
      chain->GetEntry (iEvent) ;
      
//       //--------------dati dalla geometria. Analysi Xtal per Xtal---------------
//       double lungTot = 0.;
//       //double enerTot = 0.;

//       if(deltaPhi(treeVars.cosmicClusterPhi[0],treeVars.cosmicClusterPhi[1]) > twopi / 4./*90gradi*/)
//       for(int clus = 0; clus < treeVars.nCosmicsCluster; ++clus)                //saranno 2 da costruzione dell'ntupla
// 	{
// 	  //double somLung = 0.;
// 	  //double somEner = 0.;
// 	  //for(int xtal = 0; xtal < treeVars.cosmicClusterXtals[clus]; ++xtal)
// 	  //  {
// 	      //if(treeVars.cosmicClusterEnergyXtals[clus][xtal] < 0.027) continue;
// 	      //if(treeVars.cosmicClusterLengthXtals[clus][xtal] < 20./*cm*/) continue; //selezione su lunghezza di cammino nei cristalli
// 	      //else
// 	      //{
// 	      //std::cout << "clus = " << clus << "; xtal = " << xtal << std::endl;
// 	  if((treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]) == 0.) continue;
// 	  dEondX_lung.Fill(treeVars.cosmicClusterEnergy[clus] / (treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]));
// 	  lunghVSE.Fill(treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus],treeVars.cosmicClusterEnergy[clus]);

// 	  //std::cout << "LengthXtals_0[" << clus << "] = " << treeVars.cosmicClusterLengthXtals_0[clus] << "; LengthXtals_1[" << clus << "] = " << treeVars.cosmicClusterLengthXtals_1[clus] << ";  Energy[clus]" << treeVars.cosmicClusterEnergy[clus] <<std::endl;


// 	  //somLung += treeVars.cosmicClusterLengthXtals[clus][xtal];
// 	  //somEner += treeVars.cosmicClusterEnergyXtals[clus][xtal];
// 	  //std::cout << "clus = " << clus << ";    treeVars.cosmicClusterLengthXtals[clus][xtal] = " << treeVars.cosmicClusterLengthXtals[clus][xtal] << std::endl;
// 	  //}
// 	  //}
// // 	  if(clus == 0) 
// // 	    {
// // 	      std::cout << "AAA" << std::endl;
// 	  zero.Fill(treeVars.cosmicClusterLengthXtals_0[clus]);
// 	  // 	    }
// 	  // 	  if(clus == 1) 
// 	  // 	    {
// 	  // 	      std::cout << "BBB" << std::endl;
// 	  uno.Fill(treeVars.cosmicClusterLengthXtals_1[clus]);
// 	  zeroPlusUno.Fill(treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]);
// 	  // 	    }

//  	  lungTot = lungTot + treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus];
// // 	  enerTot = enerTot + somEner;
// // 	  dEondX_lung_sum.Fill(somEner/somLung);
// 	  dEondX_lung_sumVSeta.Fill(treeVars.cosmicClusterEta[clus],treeVars.cosmicClusterEnergy[clus] / (treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]));
// 	}
//       //dEondX_lung_sum.Fill(enerTot/lungTot);
//       //------------------------------------------------------------------------
      
      //base selections
      if (treeVars.nCosmicsCluster != 2) continue ;
      if(deltaPhi(treeVars.cosmicClusterPhi[0],treeVars.cosmicClusterPhi[1]) < twopi / 4./*90gradi*/) continue; //TAGLIO IN DELTAPHI
      //      std::cout << "num SC : " << treeVars.nCosmicsCluster << std::endl ;

      TVector3 SC0_pos (0., 0., 0.) ;
      setVectorOnECAL (SC0_pos, treeVars.cosmicClusterEta[0], treeVars.cosmicClusterPhi[0]) ;
      TVector3 SC1_pos (0., 0., 0.) ;      
      setVectorOnECAL (SC1_pos, treeVars.cosmicClusterEta[1], treeVars.cosmicClusterPhi[1]) ;
               

      double alpha3D = get3DAlpha (SC0_pos, SC1_pos) ;
      TVector3 closestAppr3D = SC0_pos + alpha3D * (SC1_pos - SC0_pos) ;
            
      double alpha2DXY = get2DAlphaXY (SC0_pos, SC1_pos) ;
      TVector3 closestAppr2DXY = SC0_pos + alpha2DXY * (SC1_pos - SC0_pos) ;
      
      double alpha2DYZ = get2DAlphaYZ (SC0_pos, SC1_pos) ;
      TVector3 closestAppr2DYZ = SC0_pos + alpha2DYZ * (SC1_pos - SC0_pos) ;
            
      //-----intersez superficie interna--------------
      std::pair <double,double> CrossAlphaIn = getAlphaEcalCross(SC0_pos, SC1_pos, ECALinRad);
      if (CrossAlphaIn.first == 100. && CrossAlphaIn.second == 100.) continue;       //2clust con deltaPhi==0 a meta' altezza nell'xtal
      TVector3 int1 = SC0_pos + CrossAlphaIn.first * (SC1_pos - SC0_pos);
      TVector3 int2 = SC0_pos + CrossAlphaIn.second * (SC1_pos - SC0_pos);      

      //-----intersez superficie esterna--------------
      //       double averageAlpha = 0.5 * (CrossAlphaIn.first + CrossAlphaIn.second);
      double averageAlpha = 0.5;
      double alphaWhere1 = where (g_deltaAlpha, SC0_pos, SC1_pos, averageAlpha) ;
      double alphaWhere2 = where (-1 * g_deltaAlpha, SC0_pos, SC1_pos, averageAlpha) ;
      TVector3 ext1 = SC0_pos + alphaWhere1*(SC1_pos - SC0_pos) ;
      TVector3 ext2 = SC0_pos + alphaWhere2*(SC1_pos - SC0_pos) ;

      //-----      controlli sulla z-------------------
      if(int1.z() > 300. || int1.z() < -300. || int2.z() > 300. || int2.z() < -300.) continue;  //controllo sulla Z delle intersezioni interne (inutile)
      if(ext1.z() > 300. || ext1.z() < -300. || ext2.z() > 300. || ext2.z() < -300.) continue;  //controllo sulla Z delle intersezioni esterne
      
      //------------ACCOPPIO LE INTERSEZIONI e le ORDINO------
      std::vector <std::pair<TVector3,TVector3> > segmenti;                                     //il controllo sui 90gradi iniziale mi garantisce
      if (ext1.DeltaR(int1) < ext1.DeltaR(int2)) 
	{
	  std::pair<TVector3,TVector3> primo (int1, ext1);
	  std::pair<TVector3,TVector3> secondo (int2, ext2);
	  if(deltaPhi(primo.first.Phi(),twopi/4.) > deltaPhi(secondo.first.Phi(),twopi/4.)) std::swap(primo,secondo);  //primo el a phi + alto
	  segmenti.push_back(primo);
	  segmenti.push_back(secondo);
	}
      else
	{
	  std::pair<TVector3,TVector3> primo (int1, ext2);
	  std::pair<TVector3,TVector3> secondo (int2, ext1);
	  if(deltaPhi(primo.first.Phi(),twopi/4.) > deltaPhi(secondo.first.Phi(),twopi/4.)) std::swap(primo,secondo);  //il primo elemento di segmenti e' a phi + alto
	  segmenti.push_back(primo);
	  segmenti.push_back(secondo);
	}
      double lunghTop = getEcalXLenght (segmenti.at(0).first,segmenti.at(0).second);  //sopra
      double lunghBot = getEcalXLenght (segmenti.at(1).first,segmenti.at(1).second);  //sotto

      //-------ASSOCIO LE ENERGIE ALLE COPPIE-----
      double enerTop = 0.;
      double enerBot = 0.;
      if(segmenti.at(0).first.DeltaR(SC0_pos) < segmenti.at(0).first.DeltaR(SC1_pos))   // controllo con l'interno (first)
	{
	  enerTop = treeVars.cosmicClusterEnergy[0];
	  enerBot = treeVars.cosmicClusterEnergy[1];
	}
      else 
	{
	  enerTop = treeVars.cosmicClusterEnergy[1];
	  enerBot = treeVars.cosmicClusterEnergy[0];
	}

      /* *****************************************
	 
      SOPRA INTERNO = segmenti.at(0).first()
      SOPRA ESTERNO = segmenti.at(0).second()

      SOTTO INTERNO = segmenti.at(1).first()
      SOTTO ESTERNO = segmenti.at(1).second()

      LUNGH SOPRA   = lunghTop
      LUNGH SOTTO   = lunghBot

      ENER  SOPRA   = enerTop
      ENER  SOTTO   = enerBot
      
      ****************************************** */

      //------------CALCOLO E SELEZIONO ANGOLI------------
      //double angle1 = (int2 - int1).Angle(Vertex - int1);
      //double angle2 = (int1 - int2).Angle(Vertex - int2);

      double angleTop = (segmenti.at(1).first - segmenti.at(0).first).Angle(Vertex - segmenti.at(0).first); //sotto - sopra
      double angleBot = (segmenti.at(0).first - segmenti.at(1).first).Angle(Vertex - segmenti.at(1).first); //sopra - sotto


      //if(angleTop < 0.436 || angleTop > 0.610) continue;       //compreso fra 25 e 35
      //if(angleBot < 0.436 || angleBot > 0.610) continue;
	

      //-----------Filling Histos-----------------
		
		Occupancy.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterPhi[0]); // il vett di 20 colonne è il cluster. Noi selezioniamo solo 2 cluster
      Occupancy.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterPhi[1]); // perciò così becchi cristalli sopra e sotto.
            
      
      //MATTIA
      Occupancy.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterPhi[0]);
      Occupancy.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterPhi[1]);
      EnergyOnCrystals.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterPhi[0],treeVars.cosmicClusterEnergy[0]);
      EnergyOnCrystals.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterPhi[1],treeVars.cosmicClusterEnergy[1]);
      EoPCrystals.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterPhi[0],enerTop/lunghTop);
      EoPCrystals.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterPhi[1],enerBot/lunghBot);
      
      EVSdX.Fill(lunghTop,enerTop);
      EVSdX.Fill(lunghBot,enerBot);
      
      dEondX_AllDirTop.Fill(enerTop / lunghTop);
      dEondX_AllDirBot.Fill(enerBot / lunghBot);

      diff_dEondX.Fill(enerBot / lunghBot - enerTop / lunghTop);                                      //dEondX sotto - dEondX sopra
      
      EnergyVSclusterSize.Fill(treeVars.cosmicClusterXtals[0],treeVars.cosmicClusterEnergy[0]);
      EnergyVSclusterSize.Fill(treeVars.cosmicClusterXtals[1],treeVars.cosmicClusterEnergy[1]);

      AngleTop.Fill(angleTop);
      AngleBot.Fill(angleBot);
      dEondXVSAngle_AllDir.Fill(angleTop,(treeVars.cosmicClusterEnergy[0] + treeVars.cosmicClusterEnergy[1]) / (lunghTop + lunghBot));
      
      dEondX_AllDir.Fill(enerTop / lunghTop);
      dEondX_AllDir.Fill(enerBot / lunghBot);
      //dEondX_AllDir.Fill((enerTop + enerBot) / (lunghTop + lunghBot));

      dEondXVSIP_AllDir.Fill(closestAppr3D.Mag(),(treeVars.cosmicClusterEnergy[0] + treeVars.cosmicClusterEnergy[1]) / (lunghTop + lunghBot));
      dEondXVSdX_AllDir.Fill((lunghTop + lunghBot),(treeVars.cosmicClusterEnergy[0] + treeVars.cosmicClusterEnergy[1]) / (lunghTop + lunghBot));
      lunghTop_h.Fill(lunghTop);
      lunghBot_h.Fill(lunghBot);
      dXVSIP_transverse.Fill(closestAppr2DXY.Perp(),lunghTop + lunghBot);   //IP trasverso
      
      RadiusTop.Fill(segmenti.at(0).first.x(),segmenti.at(0).first.y());
      RadiusTop.Fill(segmenti.at(0).second.x(),segmenti.at(0).second.y());
      RadiusBot.Fill(segmenti.at(1).first.x(),segmenti.at(1).first.y());
      RadiusBot.Fill(segmenti.at(1).second.x(),segmenti.at(1).second.y());
  
     

      Radius.Fill(ext1.x(),ext1.y());
      Radius.Fill(ext2.x(),ext2.y());
      Radius.Fill(int1.x(),int1.y());
      Radius.Fill(int2.x(),int2.y());
      Spessore.Fill(ext1.z(),ext1.Perp()) ;
      Spessore.Fill(ext2.z(),ext2.Perp()) ;
      Spessore.Fill(int1.z(),int1.Perp()) ;
      Spessore.Fill(int2.z(),int2.Perp()) ;
      //Scan3D->Fill(ext.x(),ext.y(),ext.z());
 
      SCdistr.Fill (SC0_pos.x (), SC0_pos.y ()) ;
      SCdistr.Fill (SC1_pos.x (), SC1_pos.y ()) ;
      ip3D.Fill (closestAppr3D.Mag ()) ;                  // volendo controllo su deltaPhi
      ip2DXY.Fill (closestAppr2DXY.Mag ()) ;              // volendo controllo su deltaPhi -- mi interessa il .mag() ?
      ip2DYZ.Fill (closestAppr2DYZ.Mag ()) ;              // volendo controllo su deltaPhi -- mi interessa il .mag() ?

      //diff_lungh.Fill(lungTot - lunghTop - lunghBot);
      //diff_lungVSeta.Fill(lungTot - lunghTop - lunghBot);

      //double theta0 = calcTheta(treeVars.cosmicClusterEta[0]);
      //double theta1 = calcTheta(treeVars.cosmicClusterEta[1]);
      //double z0 = 129./tan(theta0);
      //double z1 = 129./tan(theta1);

      //-------------------TAGLI SUI PARAMETRI DI IMPATTO && Filling Histos-----------
      //fdg cut on IP3D   10 cm
      if(closestAppr3D.Mag () < 10.)   
	{
	  //fdg alalysis SM by SM
	  for(int ring = -17; ring < 17; ++ring)
	    {
	      if(treeVars.cosmicClusterEta[0] > ring*0.01745*5 && treeVars.cosmicClusterEta[0] < (ring + 1)*0.01745*5)
		energy_rings[ring+17]->Fill(treeVars.cosmicClusterEnergy[0]);
	      
	      if(treeVars.cosmicClusterEta[1] > ring*0.01745*5 && treeVars.cosmicClusterEta[1] < (ring + 1)*0.01745*5) //i 2 cluster nella stessa fetta
		energy_rings[ring+17]->Fill(treeVars.cosmicClusterEnergy[1]);
	      
	    }
	  
	  dEondX3DVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/23./*cm*/);
	  dEondX3DVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/23./*cm*/);

	  //----------------------------------------------------

	  double lungTot = 0.;
	  for(int clus = 0; clus < treeVars.nCosmicsCluster; ++clus)                //saranno 2 da costruzione dell'ntupla
	    {
	      
	      if((treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]) == 0.) continue;
	      if(fabs(treeVars.cosmicClusterEta[0]) > 1.3 || fabs(treeVars.cosmicClusterEta[1]) > 1.3) continue;

	      dEondX_lung.Fill(treeVars.cosmicClusterEnergy[clus] / (treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]));
	      lunghVSE.Fill(treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus],treeVars.cosmicClusterEnergy[clus]);
	      
	      zero.Fill(treeVars.cosmicClusterLengthXtals_0[clus]);
	      
	      uno.Fill(treeVars.cosmicClusterLengthXtals_1[clus]);
	      zeroPlusUno.Fill(treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]);
	      
	      
	      lungTot = lungTot + treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus];
	      
	      dEondX_lung_sumVSeta.Fill(treeVars.cosmicClusterEta[clus],treeVars.cosmicClusterEnergy[clus] / (treeVars.cosmicClusterLengthXtals_0[clus] + treeVars.cosmicClusterLengthXtals_1[clus]));
	    }
	  diff_lungh.Fill(lungTot - lunghTop - lunghBot);
	  
	  //---------------------------------------------------
	  
	}
      //fdg cut on IP2DYZ   10 cm
      //if(sqrt(closestAppr2DYZ.y()*closestAppr2DYZ.y() + closestAppr2DYZ.z()*closestAppr2DYZ.z()) < 10.) //sbagliato il concetto!!!
      if(treeVars.cosmicClusterEta[0] + treeVars.cosmicClusterEta[1] < 0.05/*3Xtals*/) //approccio tommaso: puntanti in polar view
	{
	  double lungh = getEcalXLenght(SC0_pos, SC1_pos);

	  double deltaX = fabs(SC0_pos.x() - SC1_pos.x());
	  double deltaY = fabs(SC0_pos.y() - SC1_pos.y());
	  double lunghXY = sqrt(deltaX*deltaX + deltaY*deltaY);

	  double deltaS0 = 2.*129.*23. / lunghXY;   //avevo sostituito la mag
	  double deltaS1 = 2.*129.*23. / lunghXY;
	  
	  //deltaS0 = deltaS0/cos(calcTheta(treeVars.cosmicClusterEta[0]));
	  //deltaS1 = deltaS1/cos(calcTheta(treeVars.cosmicClusterEta[1]));
	  //deltaS0 = sqrt(deltaS0*deltaS0 + 23.*23.);
	  //deltaS1 = sqrt(deltaS1*deltaS1 + 23.*23.);
	  	  
	  dEondX2DYZVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/deltaS0);
	  dEondX2DYZVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/deltaS1);
	}
      //fdg cut on IP2DXY   15 cm && deltaEta < 3Xtals
      if(closestAppr2DXY.Perp () < 15. && fabs(treeVars.cosmicClusterEta[0] - treeVars.cosmicClusterEta[1]) < 0.105/*6Xtals*/) //salami approach
      //if(closestAppr2DXY.Perp () < 10. && fabs(treeVars.cosmicClusterEta[0] - treeVars.cosmicClusterEta[1]) < 0.05/*3Xtals*/) //salami approach
	{
	  //fdg alalysis slice by slice (non mi interessa calcolare le lunghezze perch comparo destra sinistra)
	  for(int slice = -17; slice < 17; ++slice)
	    {
	      if(SC0_pos.z() > slice*18 && SC0_pos.z() < (slice + 1)*18)
		energy_slice[slice+17]->Fill(treeVars.cosmicClusterEnergy[0]);
	      
	      if(SC1_pos.z() > slice*18 && SC1_pos.z() < (slice + 1)*18) //i 2 cluster nella stessa fetta
		energy_slice[slice+17]->Fill(treeVars.cosmicClusterEnergy[1]);
	      
	    }
	  
	  dX2DVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/0.0115);
	  dX2DVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/0.0115);
    
	  dX2DVSZeta.Fill(SC0_pos.z(),treeVars.cosmicClusterEnergy[0]/0.0115);
	  dX2DVSZeta.Fill(SC1_pos.z(),treeVars.cosmicClusterEnergy[1]/0.0115);

	  dEondX2DXYVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/get2DThickness(SC0_pos.z()));
	  dEondX2DXYVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/get2DThickness(SC1_pos.z()));
	}
      
      //test
      //std::cout << "treeVars.cosmicClusterMaxId = " << treeVars.cosmicClusterMaxId[0] << std::endl;

    } //PG loop over entries 
  
  //Writing Histos
  TFile out ("histos_3D_Cycle_Matrix.root","recreate") ;
  TDirectory* Rings = gDirectory->mkdir("Rings");
  TDirectory* Slices = gDirectory->mkdir("Slices");


  Occupancy.SetOption("COLZ");
  Occupancy.Write();
  ip3D.Write () ;
  ip2DXY.Write () ;
  ip2DYZ.Write () ;
  SCdistr.Write () ;
  dEondX3DVSEta.Write ();
  dX2DVSEta.Write ();
  dX2DVSZeta.Write();
  dEondX2DXYVSEta.Write();
  dEondX2DYZVSEta.Write();
  Spessore.Write();
  Radius.Write();
  RadiusTop.Write();
  RadiusBot.Write();
  //Scan3D->Write();
  dEondX_AllDir.Write();
  dEondX_AllDirTop.Write();
  dEondX_AllDirBot.Write();
  dEondXVSIP_AllDir.Write();
  dEondXVSdX_AllDir.Write();
  dEondXVSAngle_AllDir.Write();
  lunghTop_h.Write();
  lunghBot_h.Write();
  AngleTop.Write();
  AngleBot.Write();
  dXVSIP_transverse.Write();
  EVSdX.Write();
  EnergyVSclusterSize.Write();
  diff_dEondX.Write();
  dEondX_lung.Write();
  lunghVSE.Write();
  dEondX_lung_sum.Write();
  dEondX_lung_sumVSeta.Write();
  diff_lungh.Write();
  diff_lungVSeta.Write();
  zero.Write();
  uno.Write();
  zeroPlusUno.Write();

  //MATTIA
  Occupancy.Write();
  EnergyOnCrystals.Write();
  EoPCrystals.Write();
  
  Rings->cd();
  for(int ring = 0; ring < 34; ++ring)
    energy_rings[ring] -> Write();
  out.cd();

  Slices->cd();
  for(int slice = 0; slice < 34; ++slice)
    energy_slice[slice] -> Write();
  out.cd();

  //FIT rings
  TCanvas *Crings = new TCanvas("Crings","Crings");
  Crings->Divide(6,6);
  TF1 *gaussiana = new TF1("gaussiana","gaus",0.,0.6);
  std::vector <double> ring_peak_position;
  for(int ring = -17; ring < 17; ++ring)   //GetBinContent per sottrazioni lavorando sugli histo creati?
    {
      Crings->cd(ring+18);
      gaussiana->SetParameter(0,6.);
      gaussiana->SetParameter(1,0.31);
      gaussiana->SetParameter(2,0.1);
      energy_rings[ring+17]->Fit("gaussiana","R");
      
      ring_peak_position.push_back(gaussiana->GetParameter(1));
      //int nbinx = mappa->GetXaxis ()->GetNbins () ;
      //energy_rings[ring+17]->GetBinContent(ring);
      
      std::cout << ring_peak_position.at(ring+17) << std::endl;
      energy_rings[ring+17]->Draw();
    }

  //FIT slices
  TCanvas *Cslices = new TCanvas("Cslices","Cslices");
  Cslices->Divide(6,6);
  std::vector <double> slice_peak_position;
  for(int slice = -17; slice < 17; ++slice)   //GetBinContent per sottrazioni lavorando sugli histo creati?
    {
      Cslices->cd(slice+18);
      gaussiana->SetParameter(0,6.);         //???
      gaussiana->SetParameter(1,0.31);       //???
      gaussiana->SetParameter(2,0.1);        //???
      energy_slice[slice+17]->Fit("gaussiana","R");
      
      slice_peak_position.push_back(gaussiana->GetParameter(1));
      //int nbinx = mappa->GetXaxis ()->GetNbins () ;
      //energy_rings[ring+17]->GetBinContent(ring);
      
      std::cout << slice_peak_position.at(slice+17) << std::endl;
      energy_slice[slice+17]->Draw();
    }

  for(int kk = 0; kk < 17; ++kk)
    {
      diff_ring.Fill(ring_peak_position.at(kk + 17) - ring_peak_position.at(16 - kk));    //positive - negative
      std::cout << "-----------> kk + 17 = " << kk + 17 << "; 16 - kk  = " << 16 - kk << std::endl;

      std::cout << "----> " << "diff rings = " << ring_peak_position.at(kk + 17) - ring_peak_position.at(16 - kk) << std::endl;

      diff_slices.Fill(slice_peak_position.at(kk + 17) - slice_peak_position.at(16 - kk));    //positive - negative
      std::cout << "----> " << "diff slices = " << slice_peak_position.at(kk + 17) - slice_peak_position.at(16 - kk) << std::endl;
    }

  diff_ring.Write();
  diff_slices.Write();

  out.Close () ;

  //theApp->Run();
  return(0);
}


// ----------------------------------------------------


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


// ----------------------------------------------------


double 
calcTheta (double eta) 
{
  return 2 * atan (exp ( -1 * eta)) ;
}


// ----------------------------------------------------


double 
calcEta (double theta) 
{
  return -log (tan(theta/2)) ;
}


// ----------------------------------------------------


double
deltaPhi (double phi1, double phi2)
{
  double deltaphi = fabs(phi1-phi2);
  if (deltaphi > twopi) deltaphi -= twopi;
  if (deltaphi > twopi/2) deltaphi = twopi-deltaphi;
  return deltaphi;
}


// ----------------------------------------------------


double 
getEcalXLenght (const TVector3& G1, const TVector3& G2)
{
  double deltaX = fabs(G1.x() - G2.x());
  double deltaY = fabs(G1.y() - G2.y());
  double deltaZ = fabs(G1.z() - G2.z());
  double lungh = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
  return(lungh);
}


// ----------------------------------------------------


double 
get3DAlpha (const TVector3& G1, const TVector3& G2)
{
  double vx = G2.x () - G1.x () ;
  double vy = G2.y () - G1.y () ;
  double vz = G2.z () - G1.z () ;

  double alfa = - G1.x () * vx - G1.y () * vy - G1.z () * vz ;
  alfa = alfa / (vx * vx + vy * vy + vz * vz) ;

  return (alfa) ;
}


// ----------------------------------------------------


double 
get2DAlphaXY (const TVector3& G1, const TVector3& G2)
{
  double vx = G2.x () - G1.x () ;
  double vy = G2.y () - G1.y () ;

  double alfa = - G1.x () * vx - G1.y () * vy ;
  alfa = alfa / (vx * vx + vy * vy);

  return (alfa) ;
}


// ----------------------------------------------------


double 
get2DAlphaYZ (const TVector3& G1, const TVector3& G2)
{
  double vy = G2.y () - G1.y () ;
  double vz = G2.z () - G1.z () ;

  double alfa = - G1.y () * vy - G1.z () * vz ;
  alfa = alfa / ( vy * vy + vz * vz);

  return (alfa) ;
}


// ----------------------------------------------------


double 
get2DThickness (double z)
{
  return 23. / sqrt(1 + z*z / (129.*129.));
}


// ----------------------------------------------------


double 
where (double deltaAlpha, const TVector3& G1, const TVector3& G2, double alphaOld)
{
  double delta = fabs(deltaAlpha*(G2 - G1).Mag());
  if(delta < 0.01)
    {
      return(alphaOld + deltaAlpha);                                   //stop
    }
  TVector3 point = G1 + alphaOld*(G2 - G1);
  if ((isBefore(point) == true && isBefore(point + deltaAlpha * (G2 - G1)) == true) || (isBefore(point) == false && isBefore(point + deltaAlpha * (G2 - G1)) == false))
    {
      return where(deltaAlpha, G1, G2, (alphaOld + deltaAlpha));       //avanti    concordi
    }
  return where(-0.5*deltaAlpha, G1, G2, (alphaOld + deltaAlpha));       //indietro discordi
}


// ----------------------------------------------------


bool
isBefore (const TVector3& pOld)
{
  double surfZOld = 129. + 23. / sqrt(1 + pOld.z()*pOld.z() / 17161.);
  return surfZOld > pOld.Perp () ;
}



// ----------------------------------------------------



std::pair<TVector3,TVector3> 
getEcalCross (const TVector3& G1,const TVector3& G2, double ECALrad)
{
  TVector3 nonsense (0.,0.,0.) ;
  std::pair<TVector3,TVector3> Pair (nonsense,nonsense);
  double dx = G2.x() - G1.x();
  double dy = G2.y() - G1.y();
  
  double rad = G1.x() * dx + G1.y() * dy ;
  rad = rad * rad - (dx*dx + dy*dy) * (G1.x()*G1.x() + G1.y()*G1.y() - ECALrad*ECALrad) ;
  if (rad >= 0)
  {      
    rad = sqrt (rad);

    double alpha1 =  (- G1.x()*dx - G1.y()*dy + rad) / (dx*dx + dy*dy) ;
    double alpha2 =  (- G1.x()*dx - G1.y()*dy - rad) / (dx*dx + dy*dy) ;
    
    double xi1 = G1.x() + alpha1 * (G2.x() - G1.x());
    double yi1 = G1.y() + alpha1 * (G2.y() - G1.y());
    double zi1 = G1.z() + alpha1 * (G2.z() - G1.z());
    
    double xi2 = G1.x() + alpha2 * (G2.x() - G1.x());
    double yi2 = G1.y() + alpha2 * (G2.y() - G1.y());
    double zi2 = G1.z() + alpha2 * (G2.z() - G1.z());
    
    TVector3 p1(xi1,yi1,zi1);   //GlobalPoints on ECAL surface
    TVector3 p2(xi2,yi2,zi2);
    
    if(p1.z() < 300. && p1.z() > -300. && p2.z() < 300. && p2.z() > -300.)
      {
	if(p1.Phi() > 0 && p2.Phi() < 0)
	  Pair = std::pair<TVector3,TVector3> (p1,p2);
	else if(p1.Phi() < 0 && p2.Phi() > 0)
	  Pair = std::pair<TVector3,TVector3> (p2,p1);
      }
  }
  return Pair ;
}


// --------------------------------------------------


std::pair<double,double> 
getAlphaEcalCross (const TVector3& G1,const TVector3& G2, double ECALrad)
{
  std::pair<double,double> Pair (100.,100.);
  double dx = G2.x() - G1.x();
  double dy = G2.y() - G1.y();
  
  double rad = G1.x() * dx + G1.y() * dy ;
  rad = rad * rad - (dx*dx + dy*dy) * (G1.x()*G1.x() + G1.y()*G1.y() - ECALrad*ECALrad) ;
  if (isnan(rad)) return Pair;
  if (rad >= 0.) 
  {      
    rad = sqrt (rad);

    double alpha1 =  (- G1.x()*dx - G1.y()*dy + rad) / (dx*dx + dy*dy) ;
    double alpha2 =  (- G1.x()*dx - G1.y()*dy - rad) / (dx*dx + dy*dy) ;

    Pair = std::pair<double,double> (alpha1,alpha2);
  }
  return Pair ;
}


//---------------------------------------------------



double
superGausCumCauda (double *x, double *par)
{
/* legenda
ampiezza = par[0]
picco = par[1]
sigma = par[2]
alpha = par[3]
*/
  if (par[2] == 0) return -1 ;
  if (x[0] > par[1] + par[3] * par[2])
  return par[0] / (2*par[2]*par[2]) *
         exp (par[3] * par[3] + 2 * par[3] * (- x[0] + par[1])/par[2]) ; // exp decrescente
  return par[0] / (2 * par[2]*par[2]) *
         exp (-1 * (x[0]-par[1])*(x[0]-par[1])/(par[2]*par[2])) ; // gaus
}

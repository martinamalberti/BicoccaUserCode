//COMPILO c++ -o readNtuple_3D -lm `root-config --cflags --libs --glibs` readNtuple_3D.cpp 

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>


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
void setVectorOnECAL (TVector3 & vector, double eta, double phi, double radius = 131) ;
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
  };


// ----------------------------------------------------


int main ()
{
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


  double ECALinRad = 131. ;
  double g_deltaAlpha = 0.3 ;
  TVector3 Vertex (0.,0.,0.);
  int nEvents = (int) chain->GetEntries () ;
  //PG loop over entries
  for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)
  //for (int iEvent = 0 ; iEvent < 1000000 ; ++iEvent)
    {
      if(iEvent%100000 == 0) std::cout << "event n. " << iEvent << std::endl;
      

      chain->GetEntry (iEvent) ;
      //PG base selections
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
      if (CrossAlphaIn.first == 100. && CrossAlphaIn.second == 100.) continue;                 //2clust con deltaPhi==0 a meta' altezza nell'xtal
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

      //------------CALCOLO E SELEZIONE ANGOLI------------
      //double angle1 = (int2 - int1).Angle(Vertex - int1);
      //double angle2 = (int1 - int2).Angle(Vertex - int2);

      double angleTop = (segmenti.at(1).first - segmenti.at(0).first).Angle(Vertex - segmenti.at(0).first); //sotto - sopra
      double angleBot = (segmenti.at(0).first - segmenti.at(1).first).Angle(Vertex - segmenti.at(1).first); //sopra - sotto


      //if(angleTop < 0.436 || angleTop > 0.610) continue;       //compreso fra 25 e 35
      //if(angleBot < 0.436 || angleBot > 0.610) continue;
	

      //-----------Filling Histos-----------------
            
      EVSdX.Fill(lunghTop,enerTop);
      EVSdX.Fill(lunghBot,enerBot);
      
      dEondX_AllDirTop.Fill(enerTop / lunghTop);
      dEondX_AllDirBot.Fill(enerBot / lunghBot);
      
      EnergyVSclusterSize.Fill(treeVars.cosmicClusterXtals[0],treeVars.cosmicClusterEnergy[0]);
      EnergyVSclusterSize.Fill(treeVars.cosmicClusterXtals[1],treeVars.cosmicClusterEnergy[1]);

      AngleTop.Fill(angleTop);
      AngleBot.Fill(angleBot);
      dEondXVSAngle_AllDir.Fill(angleTop,(treeVars.cosmicClusterEnergy[0] + treeVars.cosmicClusterEnergy[1]) / (lunghTop + lunghBot));
      
      //dEondX_AllDir.Fill(enerTop / lunghTop);
      //dEondX_AllDir.Fill(enerBot / lunghBot);
      dEondX_AllDir.Fill((enerTop + enerBot) / (lunghTop + lunghBot));
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

      double theta0 = calcTheta(treeVars.cosmicClusterEta[0]);
      double theta1 = calcTheta(treeVars.cosmicClusterEta[1]);
      double z0 = 131./tan(theta0);
      double z1 = 131./tan(theta1);

      //-------------------TAGLI SUI PARAMETRI DI IMPATTO && Filling Histos-----------
      //fdg cut on IP3D   10 cm
      if(closestAppr3D.Mag () < 10.)   
	{
	  dEondX3DVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/23./*cm*/);
	  dEondX3DVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/23./*cm*/);
	}
      //fdg cut on IP2DYZ   10 cm
      //if(sqrt(closestAppr2DYZ.y()*closestAppr2DYZ.y() + closestAppr2DYZ.z()*closestAppr2DYZ.z()) < 10.) //sbagliato!!!
      if(treeVars.cosmicClusterEta[0] + treeVars.cosmicClusterEta[1] < 0.05/*3Xtals*/)
	{
	  double lungh = getEcalXLenght(SC0_pos, SC1_pos);

	  double deltaX = fabs(SC0_pos.x() - SC1_pos.x());
	  double deltaY = fabs(SC0_pos.y() - SC1_pos.y());
	  double lunghXY = sqrt(deltaX*deltaX + deltaY*deltaY);

	  double deltaS0 = 2.*131.*23. / lunghXY;   //avevo sostituito la mag
	  double deltaS1 = 2.*131.*23. / lunghXY;
	  
	  //deltaS0 = deltaS0/cos(calcTheta(treeVars.cosmicClusterEta[0]));
	  //deltaS1 = deltaS1/cos(calcTheta(treeVars.cosmicClusterEta[1]));
	  //deltaS0 = sqrt(deltaS0*deltaS0 + 23.*23.);
	  //deltaS1 = sqrt(deltaS1*deltaS1 + 23.*23.);
	  	  
	  dEondX2DYZVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/deltaS0);
	  dEondX2DYZVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/deltaS1);
	}
      //fdg cut on IP2DXY   15 cm && deltaEta < 3Xtals
      if(closestAppr2DXY.Perp () < 15. && fabs(treeVars.cosmicClusterEta[0] - treeVars.cosmicClusterEta[1]) < 0.105/*?Xtals*/)
	{
	  dX2DVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/0.0115);
	  dX2DVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/0.0115);
    
	  dX2DVSZeta.Fill(z0,treeVars.cosmicClusterEnergy[0]/0.0115);
	  dX2DVSZeta.Fill(z1,treeVars.cosmicClusterEnergy[1]/0.0115);

	  dEondX2DXYVSEta.Fill(treeVars.cosmicClusterEta[0],treeVars.cosmicClusterEnergy[0]/get2DThickness(z0));
	  dEondX2DXYVSEta.Fill(treeVars.cosmicClusterEta[1],treeVars.cosmicClusterEnergy[1]/get2DThickness(z1));
	}



    } //PG loop over entries

  TFile out ("histos_3D_2.root","recreate") ;
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

  out.Close () ;

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
  return 23. / sqrt(1 + z*z / (131.*131.));
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
  double surfZOld = 131. + 23. / sqrt(1 + pOld.z()*pOld.z() / 17161.);
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

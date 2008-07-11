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

 
  int nEvents = (int) chain->GetEntries () ;
  //PG loop over entries
  for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)
    //for (int iEvent = 0 ; iEvent < 1000000 ; ++iEvent)
    {
      if(iEvent%10000 == 0) std::cout << "event n. " << iEvent << std::endl;
      
      chain->GetEntry (iEvent) ;
      
    } //PG loop over entries 
  
  //Writing Histos
  TFile out ("MATTIAhistos_3D_CycleLungTest_Matrix.root","recreate") ;
  TDirectory* Rings = gDirectory->mkdir("Rings");
  TDirectory* Slices = gDirectory->mkdir("Slices");
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

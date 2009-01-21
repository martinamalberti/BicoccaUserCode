//COMPILO c++ -o readNtuple_3D_Cycle -lm `root-config --cflags --libs --glibs` readNtuple_3D_Cycle.cpp

#include <iostream>
#include <fstream>
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
#include "CRUZET/Calibration/interface/CRUtils.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "CRUZET/NTuplesTest/test/Langaus.cc"

double twopi  = 2*acos(-1.);

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

void  
setMuonTkAtECAL (GlobalPoint & AtEcal , int MUindex, const EcalCosmicsTreeContent & event)
{
  const GlobalPoint muonInnerHit (event.muonInnerHitX[MUindex],
				  event.muonInnerHitY[MUindex],
				  event.muonInnerHitZ[MUindex]);
 
  const GlobalPoint muonOuterHit (event.muonOuterHitX[MUindex],
				  event.muonOuterHitY[MUindex],
				  event.muonOuterHitZ[MUindex]);
  
  std::pair<GlobalPoint, GlobalPoint> muonTkAtEcal = ect::EtaPhiMuonAtEcal(muonInnerHit, muonOuterHit) ;

  AtEcal = muonTkAtEcal.first;
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
  if (deltaphi > 3.1415926535 ) deltaphi = twopi-deltaphi;
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
  if ((isBefore(point) == true && isBefore(point + deltaAlpha * (G2 - G1)) == true) || 
      (isBefore(point) == false && isBefore(point + deltaAlpha * (G2 - G1)) == false))
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

// ------------------------------------------------------

//parametri da settare 
std::pair<double,double>  fitdEdx (TH1F*  dEdx)
{
  // get the peak of dEondX
  
  float range_1neg = 1.1 ;
  float range_1pos = 1.1 ;

  float range_2neg = 1.1 ;
  float range_2pos = 1.4 ;


  TF1 * gaus1 = new TF1("gaus1","gaus", 0., 0.1);
  gaus1->SetLineColor(kBlue);
  
  gaus1->SetRange(dEdx->GetMean() - range_1neg * dEdx->GetRMS(), dEdx->GetMean() + range_1pos * dEdx->GetRMS() );			
  gaus1->SetParameters(100,dEdx->GetMean(), dEdx->GetRMS() );
  dEdx->Fit("gaus1","RL+");
  

  TF1 * landau1 = new TF1("landau1", "landau", 0., 0.1);
  landau1->SetLineColor(kRed);

  landau1->SetRange(gaus1->GetParameter(1) - range_2neg * gaus1->GetParameter(2), gaus1->GetParameter(1) + range_2pos * gaus1->GetParameter(2) );	
  landau1->SetParameters(100,gaus1->GetParameter(1), gaus1->GetParameter(2));
  dEdx->Fit("landau1","RL+");

  std::pair<double,double> PairReturn(landau1->GetParameter(1),landau1->GetParError(1));
  return PairReturn;
}

//----------------------------------------------------------------------------------------------

std::pair<double,double> LangausFit (TH1F* histo)
{
 // Langaus Fit
 double fitRange[2] ;
 fitRange[0] = 0.; 
 fitRange[1] = 0.2 ;
 
 double startValues[4] ;
 startValues[0] = 0.003 ;
 startValues[1] = histo -> GetMean () ;
 startValues[2] = 500.; //= histo -> GetEntries () / 10. ;
 startValues[3] = 0.003 ;
 
 double parLimitsLow[4] ;
 parLimitsLow[0] = 0.0001 ;
 parLimitsLow[1] = 0.0001 ;
 parLimitsLow[2] = 0.0001 ;
 parLimitsLow[3] = 0.0001 ;
 
 double parLimitsHigh[4] ;
 parLimitsHigh[0] = 1000000 ;
 parLimitsHigh[1] = 10 ;
 parLimitsHigh[2] = 100000 ;
 parLimitsHigh[3] = 10 ;
 
 double parameters[4], parErrors[4] ;
 double chi2 ;
 int ndf ;
 
 TF1* langaus = langaufit ((TH1F*)histo, fitRange, startValues, parLimitsLow, parLimitsHigh, parameters, parErrors, &chi2, &ndf) ;

 std::pair<double,double> PairReturn (langaus -> GetParameter(1), langaus -> GetParError(1)) ;
 return PairReturn;
}


// --------------------------------------------------

void DrawBetheBloch()
{
 std::ifstream inFile ("/afs/cern.ch/user/a/abenagli/public/betheBlock_PbWO4.dat", std::ios::in) ;
 
 
 TGraph BetheBloch;
 TGraph BetheBloch_corrected;
 TGraph BetheBloch_ion;
 TGraph BetheBloch_irr;
 TGraph BetheBloch_irr_corrected;
 int entry = 0;
 
 float dummy, p, dEdX, ion;
 while (!inFile.eof ())
 {
   inFile >> dummy;
   inFile >> p ;
   inFile >> ion;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dEdX;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dummy;
   
   
   //   if( (p/1000. > pow (10, P_MIN)) && (p/1000. < pow (10, P_MAX)) )
   {
     
     BetheBloch.SetPoint (entry, p/1000., dEdX) ;
     BetheBloch_ion.SetPoint (entry, p/1000., ion) ;
     BetheBloch_irr.SetPoint (entry, p/1000., dEdX-ion) ;
     BetheBloch_irr_corrected.SetPoint (entry, p/1000., (dEdX-ion)*0.95 ) ;
     BetheBloch_corrected.SetPoint (entry, p/1000., ion + (dEdX-ion)*0.95) ;
     entry++;
   }
 }
 
 
 // BetheBloch.Draw ("APL") ;
 BetheBloch.Write ("BetheBloch_th") ;
 
 BetheBloch_ion.SetLineStyle (2) ;
 BetheBloch_ion.SetLineColor (kRed) ;
 //BetheBloch_ion.Draw ("PL") ;
 BetheBloch_ion.Write ("BetheBloch_th_ion") ;
 
 BetheBloch_irr.SetLineColor (kBlue) ;
 BetheBloch_irr.SetLineStyle (2) ;
 //BetheBloch_irr.Draw ("PL") ;
 BetheBloch_irr.Write ("BetheBloch_th_irr") ;
 
 BetheBloch_corrected.SetLineStyle (2) ;
 // BetheBloch_corrected.Draw ("PL") ;
 // BetheBloch_corrected.Write () ;
}
//-----------------------------------------

void DrawBetheBloch(TGraph* BetheBloch)
{
 std::ifstream inFile ("/afs/cern.ch/user/a/abenagli/public/betheBlock_PbWO4.dat", std::ios::in) ;
 
 
 TGraph BetheBloch_corrected;
 TGraph BetheBloch_ion;
 TGraph BetheBloch_irr;
 TGraph BetheBloch_irr_corrected;
 int entry = 0;
 
 float dummy, p, dEdX, ion;
 while (!inFile.eof ())
 {
   inFile >> dummy;
   inFile >> p ;
   inFile >> ion;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dEdX;
   inFile >> dummy;
   inFile >> dummy;
   inFile >> dummy;
   
   
   //   if( (p/1000. > pow (10, P_MIN)) && (p/1000. < pow (10, P_MAX)) )
   {
     
     BetheBloch -> SetPoint (entry, p/1000., dEdX) ;
     BetheBloch_ion.SetPoint (entry, p/1000., ion) ;
     BetheBloch_irr.SetPoint (entry, p/1000., dEdX-ion) ;
     BetheBloch_irr_corrected.SetPoint (entry, p/1000., (dEdX-ion)*0.95 ) ;
     BetheBloch_corrected.SetPoint (entry, p/1000., ion + (dEdX-ion)*0.95) ;
     entry++;
   }
 }
 
 
 // BetheBloch.Draw ("APL") ;
 BetheBloch -> Write ("BetheBloch_th") ;
 
 BetheBloch_ion.SetLineStyle (2) ;
 BetheBloch_ion.SetLineColor (kRed) ;
 //BetheBloch_ion.Draw ("PL") ;
 BetheBloch_ion.Write ("BetheBloch_th_ion") ;
 
 BetheBloch_irr.SetLineColor (kBlue) ;
 BetheBloch_irr.SetLineStyle (2) ;
 //BetheBloch_irr.Draw ("PL") ;
 BetheBloch_irr.Write ("BetheBloch_th_irr") ;
 
 BetheBloch_corrected.SetLineStyle (2) ;
 // BetheBloch_corrected.Draw ("PL") ;
 // BetheBloch_corrected.Write () ;
}

//--------------------------------------
double FindBetheBlochValue(TGraph* BetheBloch, float& muonP)
{
 
  double x = -1.;
  double y = -1.;

  double x_low = -1.;
  double y_low = -1.;

  BetheBloch->GetPoint(0, x, y);
  if(muonP < x) return y;

  for (int j=0; j<BetheBloch->GetN(); ++j)
    {
      BetheBloch->GetPoint(j,x,y);
      //      cout << j << " " << x << endl ;
      if (x > muonP) 
	{
	  BetheBloch->GetPoint (j-1,x_low,y_low);
	  break ;
	}
      if( muonP<x && j==(BetheBloch->GetN()-1) ) return y;
    }

      double result = (y - y_low)/(x - x_low) * (muonP - x_low) + y_low;
      return result;
}



//--------------------------------------
void DrawBetheBlochResiduals(TGraphAsymmErrors* BetheBloch_exp, TGraph* BetheBloch_th, TGraphErrors* BetheBloch_residuals)
{
  
  double x ;
  double y ;
  
  
  double yTrue ;
  
  
  for (int point = 0 ; point < BetheBloch_exp -> GetN() ; ++point)
  {
    BetheBloch_exp -> GetPoint (point, x, y) ;
    float muonP = (float)x ;
    yTrue = FindBetheBlochValue (BetheBloch_th, muonP) ;
    
    BetheBloch_residuals -> SetPoint (point, x, 1.*y/yTrue - 1.) ;
    BetheBloch_residuals -> SetPointError (point, 0., 1.*BetheBloch_exp -> GetErrorY (point) / yTrue) ;
  }
  
  BetheBloch_residuals -> SetMarkerStyle (20) ;
  BetheBloch_residuals -> SetMarkerColor (kBlue) ;
}

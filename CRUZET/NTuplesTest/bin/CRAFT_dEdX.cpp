#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <string>
#include <boost/foreach.hpp>
#include <cmath>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TProfile.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "CLHEP/Units/PhysicalConstants.h"
//#include <TMath.h>
//#include <CLHEP/Vector/LorentzVector.h>


#define PHI_MIN -3.1416
#define PHI_MAX +3.1416
#define PHI_BIN 360

#define ETA_MIN -1.5
#define ETA_MAX +1.5
#define ETA_BIN 170

#define IPHI_MIN 0.
#define IPHI_MAX +361
#define IPHI_BIN 361

#define IETA_MIN -80.
#define IETA_MAX +80.
#define IETA_BIN 361

#define X_BIN 500
#define X_MIN -800
#define X_MAX +800

#define Y_BIN 500
#define Y_MIN -800
#define Y_MAX +800

 #define P_BIN 67
 #define P_MIN -2
 #define P_MAX 6


//#define P_BIN 25
//#define P_MIN 0
//#define P_MAX 3


void BinLogX(TProfile& h);
void BinLogX(TH2F& h);

int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  // edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("Selections") ;
  edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;

  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;


  // input files
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ; ++listIt)
  {
    std::cout << *listIt << " " << std::endl ;
    chain->Add (listIt->c_str ()) ;
  }

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    
  // input files


  // output file
  std::string outputRootName = "CRAFT_dEdXdistrib.root" ;
  // output file  



  // output distributions
  TProfile BetheBlock_p ("BetheBlock_p", "BetheBlock_p", P_BIN, P_MIN, P_MAX);
  TH2F BetheBlock ("BetheBlock", "BetheBlock", P_BIN, P_MIN, P_MAX, 100, 0., 100.);

  TH1F numAssociations ("numAssociations", "numAssociations", 20, 0., 20.);

  // output distributions
  // TH2F ptAngle ("ptAngle", "ptAngle",500, 0.,1000., 300, 0, 3.14 );
  //  TH2F ptDArco ("ptDArco", "ptDArco",500, 0.,1000., 300, 0., 1000);
  //  TH2F ptR ("ptR", "ptR",500, 0.,1000., 300, 0., 1000);
  //  TH2F RDArco ("RDArco", "RDArco",300, 0.,1000., 300, 0., 1000);
  TH1F Curved_Straight ("Curved_Straight", "Curved_Straight", 1000, -0.05, 0.01); 
  //  TH1F Corda_Curved ("Corda_Curved", "Corda_Curved", 1000, -1., 5.); 
 
  TH2F Curved_Straight_pt ("Curved_Straight_pt", "Curved_Straight_pt", 1000, 0., 100., 600, -0.015, 0.005); 
  //  TH2F Corda_Curved_pt ("Corda_Curved_pt", "Corda_Curved_pt", 500, 0., 1000., 500, -2., 2.); 
  TH1F Radius_d ("Radius_d", "Radius_d", 500, 0., 1000.);
  TH1F Curved ("Curved", "Curved", 500, 0., 1000.);
  TH1F Straight ("Straight", "Straight", 500, 0., 1000.);
  //  TH1F Diff_curved ("Diff_curved", "Diff_curved", 500, 0., 1000.);


  TH2F DEta_DPhi ("DEta_DPhi", "DEta_DPhi", 400, -2.5, 2.5, 400, -3.16, 3.16);
  TH2F DPhi_Pt ("DPhi_Pt", "DPhi_Pt", 1000, 0., 1000., 400, -3.16, 3.16);
  TH2F DR_Pt ("DR_Pt", "DR_Pt", 1000, 0., 1000., 400, -5., 5.);

  TH2F DPhi_leg ("DPhi_leg", "DPhi_leg", 100, -10., 10., 400, -3.16, 3.16);

  BinLogX(BetheBlock);
  BinLogX(BetheBlock_p);

  

  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
    {
      chain->GetEntry (entry) ;
      
      //////////////////
      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.4) ;
      
      
      numAssociations.Fill(associations.size());      

      //loop on associations vector
      for (unsigned int i = 0 ; i < associations.size () ; ++i)
	{
	  int MUindex = associations.at (i).first  ;
	  int SCindex = associations.at (i).second ;
	  

	  if(treeVars.muonTkLengthInEcalDetail[MUindex] <= 0.) continue;

	  float muonP;
	  float muonPt;
	  TVector3 innerPt;
	  //	  TVector3 externPt;

	  float Radius = -1.;
	  float Length_curved = -1.;
	  float Length_straight = -1.;
	  //	  float  Length_curved_exp = -1.;

	  //	  GlobalPoint Inn;
	  //	  GlobalPoint Out;


	  float muonPhi = -1;
	  float muonEta = -1;


	  if (treeVars.muonLeg[MUindex] == 1)  
	    {
	      TVector3 dummyIn(treeVars.muonInnTkInnerHitPx[MUindex], treeVars.muonInnTkInnerHitPy[MUindex], 0.);
	      innerPt = dummyIn;
	      //	      TVector3 dummyOut(treeVars.muonOutTkOuterHitPx[MUindex], treeVars.muonOutTkOuterHitPy[MUindex], 0.);
	      //	      externPt = dummyOut;

	      //	      GlobalPoint InnPoint(treeVars.muonInnTkInnerHitX[MUindex], treeVars.muonInnTkInnerHitY[MUindex], treeVars.muonInnTkInnerHitZ[MUindex] );
	      //	      Inn = InnPoint;
	      //	      GlobalPoint OutPoint(treeVars.muonOutTkOuterHitX[MUindex], treeVars.muonOutTkOuterHitY[MUindex], treeVars.muonOutTkOuterHitZ[MUindex] );
	      //	      Out = OutPoint;

	      muonP = treeVars.muonInnTkInnerHitP[MUindex];
	      muonPt = treeVars.muonInnTkInnerHitPt[MUindex];
	      muonPhi = treeVars.muonInnTkInnerHitPhi[MUindex];
	      muonEta = treeVars.muonInnTkInnerHitEta[MUindex];

	      //	      std::cout << " treeVars.muonLeg[MUindex] " << treeVars.muonLeg[MUindex] <<std::endl;

	    }

	  else if (treeVars.muonLeg[MUindex] == -1)  
	    {
	      TVector3 dummyIn(treeVars.muonInnTkOuterHitPx[MUindex], treeVars.muonInnTkOuterHitPy[MUindex], 0.);
	      innerPt = dummyIn;
	      //	      TVector3 dummyOut(treeVars.muonOutTkInnerHitPx[MUindex], treeVars.muonOutTkInnerHitPy[MUindex], 0.);
	      //	      externPt = dummyOut;

	      //	      GlobalPoint InnPoint(treeVars.muonInnTkOuterHitX[MUindex], treeVars.muonInnTkOuterHitY[MUindex], treeVars.muonInnTkOuterHitZ[MUindex] );
	      //	      Inn = InnPoint;
	      //	      GlobalPoint OutPoint(treeVars.muonOutTkInnerHitX[MUindex], treeVars.muonOutTkInnerHitY[MUindex], treeVars.muonOutTkInnerHitZ[MUindex] );
	      //	      Out = OutPoint;

	      muonPt = treeVars.muonInnTkOuterHitPt[MUindex];
	      muonP = treeVars.muonInnTkOuterHitP[MUindex];
	      BetheBlock.Fill( muonP, treeVars.superClusterRawEnergy[SCindex]/treeVars.muonTkLengthInEcalDetail[MUindex]/8.28*250.);
	      BetheBlock_p.Fill( muonP, treeVars.superClusterRawEnergy[SCindex]/treeVars.muonTkLengthInEcalDetail[MUindex]/8.28*250.);

	      muonPhi = treeVars.muonInnTkOuterHitPhi[MUindex];
	      muonEta = treeVars.muonInnTkOuterHitEta[MUindex];

	      //	      std::cout << " treeVars.muonLeg[MUindex] " << treeVars.muonLeg[MUindex] <<std::endl;

	    }

	  else continue;

	  //	  std::cout << " RAGGIO CAMERE A MU INTERNE " << sqrt(Out.x()*Out.x() + Inn.y()*Inn.y()) << std::endl; 

	  //	  Radius = sqrt( (Out.x()-Inn.x())*(Out.x()-Inn.x()) +  (Out.y()-Inn.y())*(Out.y()-Inn.y()) );
	  // std::cout << " RADIUS = " << Radius << std::endl;
	  //	  double angle = innerPt.Angle(externPt);

	  //Pt(GeV) = 0.3 B(T) R(m)
	  Radius =  muonPt / 0.3 /3.8 * 100.;
	  Length_straight = treeVars.muonTkLengthInEcalDetail[MUindex];
	  Length_curved = Radius * atan(Length_straight / Radius);

	  //	  Length_curved_exp = Radius/2. /sin(angle/2.) *angle;


	  // ptAngle.Fill(muonPt, angle/2.);
	  //	  ptDArco.Fill(muonPt, Radius * angle/2./Length_curved_exp *100.);
	  //	  ptR.Fill(muonPt, Radius);
	  //	  RDArco.Fill(Radius, Radius * angle/2.);

	 //  Curved_Straight.Fill((Length_curved-Length_straight)/Length_curved);
	  // 	  Corda_Curved.Fill((Length_curved-Radius)/Length_curved);

	  Curved_Straight.Fill((Length_curved-Length_straight)/Length_curved);
	  //	  Corda_Curved.Fill((Length_curved-Radius)/Length_curved);

	  Curved_Straight_pt.Fill(muonPt, (Length_curved-Length_straight)/Length_curved);
	  //	  Corda_Curved_pt.Fill(muonPt, (Length_curved-Radius)/Length_curved);

	  Curved.Fill(Length_curved);
	  Straight.Fill(Length_straight);
	  Radius_d.Fill(Radius);
	  //	  Diff_curved.Fill(Length_curved - Length_curved);

      
      /////////////////////////////////////////////////////


	  float dphi = muonPhi - treeVars.superClusterPhi[SCindex];
 	  if(dphi>CLHEP::pi) 
	    {
	      //	      std::cout << "dphi = " << dphi << std::endl; 	     
	      dphi-= 2*CLHEP::pi ;
	      //	      std::cout << "ORA dphi = " << dphi << std::endl; 	     
	    }
 	  else if(dphi<-CLHEP::pi)
	    {
	      //	      std::cout << "dphi = " << dphi << std::endl; 	     
	      dphi+= 2*CLHEP::pi;
	      //	      std::cout << "ORA dphi = " << dphi << std::endl; 	     
	    }
	  float deta = muonEta - treeVars.superClusterEta[SCindex];
	  float deltaR = sqrt(pow(deta,2) + pow(dphi,2));

	  //	  std::cout << " treeVars.muonLeg[MUindex] " << treeVars.muonLeg[MUindex] <<std::endl;


	  DEta_DPhi.Fill(deta, dphi);
	  DPhi_Pt.Fill(muonPt, dphi);
	  DR_Pt.Fill(muonPt, deltaR);

	  DPhi_leg.Fill(treeVars.muonLeg[MUindex], dphi);
	   

	}

    //   for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
// 	   XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
// 	     treeVars.nXtalsInSuperCluster[SCindex] ; ++XTLindex)
// 	{
// 	  if(treeVars.xtalTkLength[XTLindex] == -1) continue;
	  
// 	  //---- get Xtal direction
// 	  TVector3 Xtal_pos (0., 0., 0.) ;
// 	  double xtalEta = (dummy.ieta() - 0.5) * 0.0175;
// 	  if(dummy.ieta() < 0.) xtalEta = (dummy.ieta() + 0.5) * 0.0175;
// 	  double xtalPhi = (dummy.iphi() - 10) * 0.0175;
// 	  if(xtalPhi > PI) xtalPhi = - 2*PI + xtalPhi;
// 	  setVectorOnECAL (Xtal_pos, xtalEta, xtalPhi, 131.);
	  
// 	  TVector3 MuonDir (treeVars.muonPx[MUindex], treeVars.muonMomentumY[MUindex], treeVars.muonMomentumZ[MUindex]);
// 	  double angle_Mu_Xtal = MuonDir.Angle( Xtal_pos ) ;
//     if(angle_Mu_Xtal > PI ) angle_Mu_Xtal -= PI ;
//     if( angle_Mu_Xtal > PI/2. ) angle_Mu_Xtal = PI - angle_Mu_Xtal; // angle belongs to [0:90]

//     if(dummy.iphi() < 10 || dummy.iphi() > 190) alpha_down.Fill(angle_Mu_Xtal*180. /PI);
//     else alpha_up.Fill(angle_Mu_Xtal*180. /PI);
//     EtaPhi_alpha_xtalMU.Fill(dummy.ieta(), dummy.iphi(), angle_Mu_Xtal*180./PI);
//     dEdx_alpha.Fill(angle_Mu_Xtal*180./PI, treeVars.xtalEnergy[XTLindex] / treeVars.xtalTkLength[XTLindex]);
//     if(dummy.iphi() < 10 || dummy.iphi() > 190) dEdx_alpha_down.Fill(angle_Mu_Xtal*180./PI, treeVars.xtalEnergy[XTLindex] / treeVars.xtalTkLength[XTL$
//     else dEdx_alpha_up.Fill(angle_Mu_Xtal*180./PI, treeVars.xtalEnergy[XTLindex] / treeVars.xtalTkLength[XTLindex]);
//     EtaPhi_events.Fill(dummy.ieta(), dummy.iphi(), angle_Mu_Xtal*180./PI);
//     dEdx.Fill(dummy.ieta(), dummy.iphi(), treeVars.xtalEnergy[XTLindex] / treeVars.xtalTkLength[XTLindex]);

    } // loop over entries


  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  


  BetheBlock.Write();
  BetheBlock_p.Write();
  numAssociations.Write();

  //  aSlices[1].Write();

  // ptAngle.Write();
  //  ptDArco.Write();
  // ptR.Write();
  //  RDArco.Write();

  Curved_Straight.Write();
  //  Corda_Curved.Write();
  
  Curved_Straight_pt.Write();
  //  Corda_Curved_pt.Write();

  Radius_d.Write();
  Curved.Write();
  Straight.Write();

  //  Diff_curved.Write();

  DEta_DPhi.Write();
  DPhi_Pt.Write();
  DR_Pt.Write();
  DPhi_leg.Write();

  saving.Close () ;

  return 0 ;
}


//////////////////////////


void BinLogX(TProfile& h)
//void BinLogX(TH2F& h)
{
  
  TAxis* axis = h.GetXaxis();
  int bins = axis->GetNbins();

  Axis_t from = axis->GetXmin();
  Axis_t to = axis->GetXmax();
  Axis_t width = (to - from) / bins;
  Axis_t* new_bins = new Axis_t[bins + 1];
  
  for (int i = 0; i <= bins; i++) 
    {
      //    new_bins[i] = log10(from + i * width);
      new_bins[i] = pow(10, from + i * width);
      std::cout << "bin " << i << " valore " << new_bins[i] << std::endl; 
    }
  axis->Set(bins, new_bins);
  delete new_bins;
} 


void BinLogX(TH2F& h)
{
  
  TAxis* axis = h.GetXaxis();
  int bins = axis->GetNbins();

  Axis_t from = axis->GetXmin();
  Axis_t to = axis->GetXmax();
  Axis_t width = (to - from) / bins;
  Axis_t* new_bins = new Axis_t[bins + 1];
  
  for (int i = 0; i <= bins; i++) 
    {
      //    new_bins[i] = log10(from + i * width);
      new_bins[i] = pow(10, from + i * width);
      std::cout << "bin " << i << " valore " << new_bins[i] << std::endl; 
    }
  axis->Set(bins, new_bins);
  delete new_bins;
} 

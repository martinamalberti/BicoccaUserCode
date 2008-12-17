#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CRUZET/NTuplesTest/test/Langaus.cc"

#include <iostream>
#include <string>
#include <boost/foreach.hpp>
#include <cmath>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
//#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "CLHEP/Units/PhysicalConstants.h"

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

 #define P_BIN 30
 #define P_MIN 0
 #define P_MAX 4


//#define P_BIN 25
//#define P_MIN 0
//#define P_MAX 3


void BinLogX (TH1F& h) ;
void BinLogX (TProfile& h) ;
void BinLogX (TH2F& h) ;



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
  std::string outputRootName = "CRAFT_dEdXdistrib_new.root" ;
  // output file  



  // output distributions
  TH1F muon_P("muon_P", "muon_P", P_BIN, P_MIN, P_MAX);
  TH1F Pmuon_P("Pmuon_P", "Pmuon_P", P_BIN, P_MIN, P_MAX);

  TProfile BetheBlock_p ("BetheBlock_p", "BetheBlock_p", P_BIN, P_MIN, P_MAX);
  TProfile BetheBlock_curva ("BetheBlock_curva", "BetheBlock_curva", P_BIN, P_MIN, P_MAX);
  TProfile BetheBlock_energy ("BetheBlock_energy", "BetheBlock_energy", P_BIN, P_MIN, P_MAX);
  TProfile BetheBlock_25x25 ("BetheBlock_25x25", "BetheBlock_25x25", P_BIN, P_MIN, P_MAX);

  TProfile BetheBlock_corrected ("BetheBlock_corrected", "BetheBlock_corrected", P_BIN, P_MIN, P_MAX);
  TH2F BetheBlock ("BetheBlock", "BetheBlock", P_BIN, P_MIN, P_MAX, 1000, 0., 100.);
  TGraphErrors BetheBlock_fitGaus ;
  TGraphErrors BetheBlock_fitLangaus ;

  TH1F numAssociations ("numAssociations", "numAssociations", 20, 0., 20.);

  // output distributions
  TH1F Curved_Straight ("Curved_Straight", "Curved_Straight", 1000, -0.2, 0.2); 
  //  TH2F Curved_Straight_pt ("Curved_Straight_pt", "Curved_Straight_pt", 1000, 0., 100., 600, -0.2, 0.2); 
  TH2F Curved_Straight_pt ("Curved_Straight_pt", "Curved_Straight_pt", P_BIN, P_MIN, P_MAX, 600, -0.2, 0.2); 

  TH1F Radius_d ("Radius_d", "Radius_d", 500, 0., 1000.);
  TH1F Curved ("Curved", "Curved", 500, -10., 10.);
  TH1F Straight ("Straight", "Straight", 500, 0., 1000.);
  
  TH2F DEta_DPhi ("DEta_DPhi", "DEta_DPhi", 400, -2.5, 2.5, 400, -3.16, 3.16);
  TH2F DPhi_Pt ("DPhi_Pt", "DPhi_Pt", 1000, 0., 1000., 400, -3.16, 3.16);
  TH2F DR_Pt ("DR_Pt", "DR_Pt", 1000, 0., 1000., 400, -5., 5.);

  TH2F DPhi_leg ("DPhi_leg", "DPhi_leg", 100, -10., 10., 400, -3.16, 3.16);


  BinLogX(muon_P);
  BinLogX(Pmuon_P);
  BinLogX(Curved_Straight_pt);

  BinLogX(BetheBlock);
  BinLogX(BetheBlock_p);
  BinLogX(BetheBlock_curva);
  BinLogX(BetheBlock_energy);
  BinLogX(BetheBlock_25x25);

  BinLogX(BetheBlock_corrected);

  std::map<int,float> momenti_curva ;
  //  std::vector<float> momenti_energy ;
  //  std::vector<float> momenti_25x25 ;

  TGraphAsymmErrors BB_curva ;
  TGraphErrors BB_energy ;
  TGraphErrors BB_25x25 ;

  momenti_curva.clear();

  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
    {
      chain->GetEntry (entry) ;
      
     
      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;
      
      
      numAssociations.Fill(associations.size());      

      //loop on associations vector
      for (unsigned int i = 0 ; i < associations.size () ; ++i)
	{
	  int MUindex = associations.at (i).first  ;
	  int SCindex = associations.at (i).second ;
	  
	  if(treeVars.muonTkLengthInEcalDetail[MUindex] <= 0.) continue;
	  if(treeVars.muonTkLengthInEcalDetailCurved[MUindex] <= 0.) continue;
  
	  float muonP;
	  float muonPt;
	  float muond0;
	  float muondz;

	  TVector3 innerPt;

	  float Radius = -1.;
	  float Length_curved = -1.;
	  float Length_straight = -1.;

	  float muonPhi = -1;
	  float muonEta = -1;

	  bool OKFillBB = false;

	  if (treeVars.muonLeg[MUindex] == 1)  
	    {
	      TVector3 dummyIn(treeVars.muonInnTkInnerHitPx[MUindex], treeVars.muonInnTkInnerHitPy[MUindex], 0.);
	      innerPt = dummyIn;
	    
	      muonP = treeVars.muonInnTkInnerHitP[MUindex];
	      muonPt = treeVars.muonInnTkInnerHitPt[MUindex];
	      muonPhi = treeVars.muonInnTkInnerHitPhi[MUindex];
	      muonEta = treeVars.muonInnTkInnerHitEta[MUindex];
	      muond0 = treeVars.muond0[MUindex];
	      muondz = treeVars.muondz[MUindex];
	    
	      //      OKFillBB = true;
	    }

	  else if (treeVars.muonLeg[MUindex] == -1)  
	    {
	      TVector3 dummyIn(treeVars.muonInnTkOuterHitPx[MUindex], treeVars.muonInnTkOuterHitPy[MUindex], 0.);
	      innerPt = dummyIn;


	      muonP = treeVars.muonInnTkOuterHitP[MUindex];
	      muonPt = treeVars.muonInnTkOuterHitPt[MUindex];
	      muonPhi = treeVars.muonInnTkOuterHitPhi[MUindex];
	      muonEta = treeVars.muonInnTkOuterHitEta[MUindex];
	      muond0 = treeVars.muond0[MUindex];
	      muondz = treeVars.muondz[MUindex];

	      OKFillBB = true;
	    }

	  else continue;

	  
	  //impact parameter
	  if( fabs(muond0) > 100. || fabs(muondz) > 100.) continue;
	  //if( fabs(muond0) > 50. || fabs(muondz) > 50.) continue;


	  
	  int bin_energy = -1;
	  int bin_curva = -1;
	  int bin_25x25 = -1;



	  muon_P.Fill(muonP);
	  //	  Pmuon_P.Fill(muonP, pow(muonP, -2.7)  );
	  Pmuon_P.Fill(muonP * pow(muonP, -2.7)  );


	  //Pt(GeV) = 0.3 B(T) R(m)
	  Radius =  muonPt / 0.3 /3.8 * 100.;
	  Length_straight = treeVars.muonTkLengthInEcalDetail[MUindex];
	  // Length_curved = Radius * asin(Length_straight / Radius);
	  Length_curved = treeVars.muonTkLengthInEcalDetailCurved[MUindex];
	  //	  float correction =  (Length_curved - Length_straight) /  Length_straight; 
	  float correction =  Length_straight*Length_straight /Radius /Radius /6.; 
	
	  if(OKFillBB == true && muonP > 3.5 && muonP < 250.)
	    //if(OKFillBB == true)
	    {
	      BetheBlock_corrected.Fill( muonP, treeVars.superClusterRawEnergy[SCindex]/(treeVars.muonTkLengthInEcalDetail[MUindex]+correction)/8.28*250. * 0.9);
	      BetheBlock.Fill( muonP, treeVars.superClusterRawEnergy[SCindex]/treeVars.muonTkLengthInEcalDetail[MUindex]/8.28*250.* 0.9);
	      BetheBlock_p.Fill( muonP, treeVars.superClusterRawEnergy[SCindex]/treeVars.muonTkLengthInEcalDetail[MUindex]/8.28*250.* 0.90);
	      bin_curva = BetheBlock_curva.Fill( muonP, treeVars.superClusterRawEnergy[SCindex]/treeVars.muonTkLengthInEcalDetailCurved[MUindex]/8.28*250. * 0.9);

	      //	      std::cerr<<" il bin vale "<<bin_curva<<"*********************"<<std::endl;	
	      //	      std::cerr<<" ci metto "<<muonP <<std::endl;	
	      momenti_curva[bin_curva] += muonP;
	      //	      std::cerr<<" vale "<<momenti_curva[bin_curva] <<std::endl;	
	      Curved_Straight_pt.Fill(muonPt, (Length_straight-Length_curved)/Length_curved);
	      //loop su cluster del Supercluster Associato
	      for (int CLUSTERindex = treeVars.clusterIndexInSuperCluster[SCindex] ;
		   CLUSTERindex < treeVars.clusterIndexInSuperCluster[SCindex] + treeVars.nClustersInSuperCluster[SCindex] ; ++CLUSTERindex)
		{
		  bin_energy = BetheBlock_energy.Fill( muonP, treeVars.clusterEnergy[CLUSTERindex]/treeVars.muonTkLengthInEcalDetailCurved[MUindex]/8.28*250. * 0.9);
		  bin_25x25 = BetheBlock_25x25.Fill( muonP, treeVars.clusterE5x5[CLUSTERindex]/treeVars.muonTkLengthInEcalDetailCurved[MUindex]/8.28*250. * 0.9);
		}
	    }

	  Curved_Straight.Fill((Length_straight-Length_curved)/Length_curved);
	  //	  Curved_Straight_pt.Fill(muonPt, (Length_straight-Length_curved)/Length_curved);
	  
	  Curved.Fill(Length_curved);
	  Straight.Fill(Length_straight);
	  Radius_d.Fill(Radius);

	  /////////////////////////////////////////////////////

	  float dphi = muonPhi - treeVars.superClusterPhi[SCindex];
 	  if      (dphi>CLHEP::pi) dphi-= 2*CLHEP::pi ;
 	  else if(dphi<-CLHEP::pi) dphi+= 2*CLHEP::pi;

	  float deta = muonEta - treeVars.superClusterEta[SCindex];
	  float deltaR = sqrt(pow(deta,2) + pow(dphi,2));


	  DEta_DPhi.Fill(deta, dphi);
	  DPhi_Pt.Fill(muonPt, dphi);
	  DR_Pt.Fill(muonPt, deltaR);

	  DPhi_leg.Fill(treeVars.muonLeg[MUindex], dphi);
	}

    } // loop over entries


  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  


  BetheBlock.Write();
  BetheBlock_p.Write();
  BetheBlock_curva.Write();
  BetheBlock_energy.Write();
  BetheBlock_25x25.Write();

  BetheBlock_corrected.Write();
  numAssociations.Write();

  Pmuon_P.Write();
  muon_P.Write();

  std::vector<TH1D*> BetheBlock_sliceY ;
  std::vector<float> errorY_bb ;
  for (int slice = 0; slice < BetheBlock.GetNbinsX(); ++slice)
    {
      char histoName[50] ;
      sprintf (histoName, "BetheBlock_sliceY_%d", slice) ;
      TH1D* histo = BetheBlock.ProjectionY (histoName, slice, slice) ;
      BetheBlock_sliceY.push_back (histo);
      
      TF1* gaus = new TF1 ("gaus", "gaus") ;
      gaus -> SetNpx (10000) ;
      gaus -> SetLineWidth (2) ; 
      gaus -> SetLineColor (kRed) ; 
      gaus -> SetParameter (1, histo -> GetMean()) ;
      gaus -> SetParameter (2, histo -> GetRMS()) ;
      histo -> Fit ("gaus", "Q+") ;


      // Langaus Fit
      double fitRange[2] ;
      fitRange[0] = 0.; 
      fitRange[1] = 50. ;
     
      double startValues[4];   
      startValues[0] = 1. ; 
      startValues[1] = gaus->GetParameter (1);      
      startValues[2] = gaus->GetParameter (0);      
      startValues[3] = gaus->GetParameter (2);      

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

      double parameters[4], parErrors[4];
      double chi2;
      int ndf;
 
      langaufit((TH1F*)histo, fitRange, startValues, parLimitsLow, parLimitsHigh, parameters, parErrors, &chi2, &ndf);

      histo -> Write () ;      

      BetheBlock_fitGaus.SetPoint (slice, BetheBlock.GetBinCenter (slice), gaus -> GetParameter (1)) ;      
      BetheBlock_fitGaus.SetPointError (slice, 0., gaus -> GetParError (1)) ;       

      BetheBlock_fitLangaus.SetPoint (slice, BetheBlock.GetBinCenter (slice), parameters[1]) ;      
      BetheBlock_fitLangaus.SetPointError (slice, 0., parErrors[1]) ;       
      char baubau[50] = "baubau" ;
      TH1D* histo_bb = Curved_Straight_pt.ProjectionY(baubau, slice, slice+1) ;
      errorY_bb.push_back(histo_bb->GetRMS());
    }
  
  BetheBlock_fitGaus.SetName ("BetheBlock_fitGaus") ;
  BetheBlock_fitGaus.SetTitle ("BetheBlock_fitGaus") ;
  BetheBlock_fitGaus.Write () ;

  BetheBlock_fitLangaus.SetName ("BetheBlock_fitLangaus") ;
  BetheBlock_fitLangaus.SetTitle ("BetheBlock_fitLangaus") ;
  BetheBlock_fitLangaus.Write () ;

  //  TObjArray aSlices;
  TF1* landau = new TF1 ("landau", "gaus") ;
  BetheBlock.FitSlicesY (landau, 0, -1, 0, 0) ;
  TH1D* mean = new TH1D(*(TH1D*)gDirectory->Get("BetheBlock_1"));
  mean -> Write () ; 

  Curved_Straight.Write();
  Curved_Straight_pt.Write();

  Radius_d.Write();
  Curved.Write();
  Straight.Write();

  DEta_DPhi.Write();
  DPhi_Pt.Write();
  DR_Pt.Write();
  DPhi_leg.Write();


//////////////////////////
//TGraphs  with re-mediated x-bins


  double erX_low = -1.;
  double erX_hig = -1.;

  double erY_low = -1.;
  double erY_hig = -1.;

  //  BetheBlock_curved.GetBinError(bin_a);

  for(int bin_a = 0; bin_a <BetheBlock_curva.GetNbinsX(); ++bin_a)
    {
      //      std::cerr<<"bin qui vale "<< bin_a << std::endl;
      BB_curva.SetPoint(bin_a+1, momenti_curva[bin_a]/BetheBlock_curva.GetBinEntries(bin_a), BetheBlock_curva.GetBinContent(bin_a));
      erX_low = BetheBlock_curva.GetBinWidth(bin_a)/2. + momenti_curva[bin_a]/BetheBlock_curva.GetBinEntries(bin_a) - BetheBlock_curva.GetBinCenter(bin_a);
      erX_hig = BetheBlock_curva.GetBinWidth(bin_a)/2. - momenti_curva[bin_a]/BetheBlock_curva.GetBinEntries(bin_a) + BetheBlock_curva.GetBinCenter(bin_a);
 
      erY_low = sqrt(errorY_bb.at(bin_a)/2.*errorY_bb.at(bin_a)/2. + BetheBlock_curva.GetBinError(bin_a)*BetheBlock_curva.GetBinError(bin_a));
      erY_hig = erY_low;
      //      BB_curva.SetPointError(bin_a, BetheBlock_curva.GetBinWidth(bin_a), BetheBlock_curva.GetBinError(bin_a));
      BB_curva.SetPointError(bin_a+1, erX_low, erX_hig, erY_low, erY_hig);
      std::cerr<<"errore su Y  "<<  errorY_bb.at(bin_a)/2.<<  std::endl;
      //      std::cerr<<"errore su Y+1  "<<  errorY_bb.at(bin_a+1)/2.<<  std::endl;

      BB_energy.SetPoint(bin_a+1, momenti_curva[bin_a]/BetheBlock_curva.GetBinEntries(bin_a), BetheBlock_energy.GetBinContent(bin_a));
      BB_energy.SetPointError(bin_a+1, 0., BetheBlock_energy.GetBinError(bin_a));

      BB_25x25.SetPoint(bin_a+1, momenti_curva[bin_a]/BetheBlock_curva.GetBinEntries(bin_a), BetheBlock_25x25.GetBinContent(bin_a));
      BB_25x25.SetPointError(bin_a+1, 0., BetheBlock_25x25.GetBinError(bin_a));
    }



  //  BB_curva.SavePrimitive();

  BB_curva.Write("BB_curva");
  BB_energy.Write("BB_energy");
  BB_25x25.Write("BB_25x25");




///////////////////////



  saving.Close () ;

  return 0 ;
}
/////////////////////






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
      //std::cout << "bin " << i << " valore " << new_bins[i] << std::endl; 
    }
  axis->Set(bins, new_bins);
  delete new_bins;
} 




void BinLogX(TH1F& h)
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
      //      std::cout << "bin " << i << " valore " << new_bins[i] << std::endl; 
    }
  axis->Set(bins, new_bins);
  delete new_bins;
} 


#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"
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
#include "CRUZET/Calibration/interface/CRUtils.h"


int main (int argc, char** argv)
{
	gROOT->SetStyle("Plain"); 
	 // Tree construction
	TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
// 	chain->Add ("~/public/MuonTreeLungTestMatrix_43439_*.root") ;
	chain->Add ("/tmp/snidero/EcalCosmicsTree50908.tree.root") ;
//       chain->Add ("/afs/cern.ch/user/s/snidero/CMSSW_2_0_9/src/CRUZET/Calibration/bin/EcalCosmicsTree-50908.tree.root") ;
	 //
	EcalCosmicsAnalysisVariables treeVars ;
	 //
	chain->SetBranchAddress ("runId", &treeVars.runId) ;                           
	chain->SetBranchAddress ("eventId", &treeVars.eventId) ;                       
	chain->SetBranchAddress ("timeStampLow", &treeVars.timeStampLow) ;             
	chain->SetBranchAddress ("timeStampHigh", &treeVars.timeStampHigh) ;           
	chain->SetBranchAddress ("isECALL1", &treeVars.isECALL1) ;                     
	chain->SetBranchAddress ("isHCALL1", &treeVars.isHCALL1) ;                     
	chain->SetBranchAddress ("isDTL1", &treeVars.isDTL1) ;                         
	chain->SetBranchAddress ("isRPCL1", &treeVars.isRPCL1) ;                       
	chain->SetBranchAddress ("isCSCL1", &treeVars.isCSCL1) ;                       
	chain->SetBranchAddress ("nClusters", &treeVars.nCosmicsCluster) ;       
	chain->SetBranchAddress ("clusterEnergy", treeVars.cosmicClusterEnergy) ;
	chain->SetBranchAddress ("clusterE1", treeVars.cosmicClusterE1) ;        
	chain->SetBranchAddress ("clusterE2", treeVars.cosmicClusterE2) ;        
	chain->SetBranchAddress ("clusterE9", treeVars.cosmicClusterE9) ;        
	chain->SetBranchAddress ("clusterE25", treeVars.cosmicClusterE25) ;      
	chain->SetBranchAddress ("clusterTime", treeVars.cosmicClusterTime) ;    
	chain->SetBranchAddress ("clusterEta", treeVars.cosmicClusterEta) ;      
	chain->SetBranchAddress ("clusterPhi", treeVars.cosmicClusterPhi) ;      
	chain->SetBranchAddress ("clusterXtals", treeVars.cosmicClusterXtals) ;  
	chain->SetBranchAddress ("clusterXtalsAbove3Sigma", treeVars.cosmicClusterXtalsAbove3Sigma) ;    
	chain->SetBranchAddress ("clusterMaxId", treeVars.cosmicClusterMaxId) ;      
	chain->SetBranchAddress ("cluster2ndId", treeVars.cosmicCluster2ndId) ;      
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
	
	chain->SetBranchAddress ("muonMomentumX", treeVars.muonMomentumX) ;
	chain->SetBranchAddress ("muonMomentumY", treeVars.muonMomentumY) ; 
	chain->SetBranchAddress ("muonMomentumZ", treeVars.muonMomentumZ) ;
	
// 	chain->SetBranchAddress ("clusterEnergyXtals", treeVars.cosmicClusterEnergyXtals) ;
// 	chain->SetBranchAddress ("clusterLengthXtals_0", treeVars.cosmicClusterLengthXtals_0) ; 
// 	chain->SetBranchAddress ("clusterLengthXtals_1", treeVars.cosmicClusterLengthXtals_1) ; 
	 //
	int nEvents = (int) chain->GetEntries () ; 
	std::cout << "events " << nEvents << std::endl;
	std::cout << " " << std::endl;
	 //
	double ECALinRad = 129. ;
	#define PI 3.14159265
	const double deg = PI/180;  // 1 deg in radians
	double g_deltaAlpha = 0.3 ;
	TVector3 Vertex (0.,0.,0.);
	 //	
	TH1F dEondX("dEondX","dEondX", 100, 0., 0.05); 
	TH1F Angle("Angle","Angle",180,0.,2*PI/2);
	 //
	TH2F AngleVsdiff_dEondX ("AngleVsdiff_dEondX", "AngleVsdiff_dEondX", 180, 0., 90*deg, 200, -0.2, 0.2);
	TH2F Occupancy("Occupancy","Occupancy",360,-3.14,3.14,170,-1.47,1.47); 
	
	//PG loop over entries
	for (int iEvent = 0 ; iEvent < 1000 ; ++iEvent)   
// 	for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)   
  	{
		if(iEvent%10000 == 0) std::cout << "event n. " << iEvent << std::endl;
		 //
		chain->GetEntry (iEvent);
		
		// -------  SELECTIONS  -------
		if (treeVars.nCosmicsCluster != 1) continue ; 
	
		TVector3 SC0_pos (0., 0., 0.) ;
		setVectorOnECAL (SC0_pos, treeVars.cosmicClusterEta[0], treeVars.cosmicClusterPhi[0]);
		
		TVector3 MuonDir (treeVars.muonMomentumX[0], treeVars.muonMomentumY[0], treeVars.muonMomentumZ[0]);
		
		Angle = fabs(MuonDir.Angle( SC0_pos ));
		
		// fill histos
		dEondX.Fill(treeVars.cosmicClusterEnergy[0] / treeVars.muonTkLengthInEcalDetail[0]); 
		Angle.Fill(Angle);

		// ----- SELECTIONS -------:  
// 		if( Angle < 45*deg ) continue;	      
// 		Occupancy.Fill(treeVars.cosmicClusterPhi[0], treeVars.cosmicClusterEta[0]);  
		
	}//PG loop over entries 
	
/*
	// fit histos of intervals
	double norm = 100.;
	double mean = 0.;
	double sigma = 1.;
	TF1 * gaussiana = new TF1("gaussiana","gaus", -0.02, 0.02);
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
	{  
		mean = diff_dEondX_histo_interv.at(iInterval)->GetMean();
		sigma = diff_dEondX_histo_interv.at(iInterval)->GetRMS();
		gaussiana->SetParameters(norm, mean, sigma);
		 //
 		diff_dEondX_histo_interv.at(iInterval)->Fit("gaussiana","R");
		diff_dEondX_interv_mean[iInterval] = gaussiana->GetParameter(1);
		diff_dEondX_interv_sigma[iInterval] = gaussiana->GetParError(1);
// 		diff_dEondX_interv_sigma[iInterval] = gaussiana->GetParError(1);
// 		std::cout <<  " mean_aft_fit " << diff_dEondX_interv_mean[iInterval] << " sigma_aft_fit " << diff_dEondX_interv_sigma[iInterval] << std::endl;
	std::cout << " sigma media " << diff_dEondX_interv_mean[iInterval]/sqrt(diff_dEondX_histo_interv.at(iInterval)->GetEntries()) << " par err " << diff_dEondX_interv_sigma[iInterval] << std::endl;
	}
	
	 // final graph diff_dEondX to see cherenkov eff
	TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 400, 400);
	TGraphErrors * cherenkov = new TGraphErrors(nIntervals, mean_angle_interv, diff_dEondX_interv_mean, 0, diff_dEondX_interv_sigma);
	TF1 * g1 = new TF1("g1", "pol1", 0., 80.);
	 //
	cherenkov->SetTitle("diff_dEondX Bot-Top");
	cherenkov->GetXaxis()->SetTitle("angle (deg)");
	cherenkov->GetYaxis()->SetTitle("diff_dEondX (GeV/cm)");
	cherenkov->GetYaxis()->SetRangeUser(-0.02,0.02);
	cherenkov->GetYaxis()->SetTitleOffset(1.1);
	 //
	cherenkov->SetMarkerColor(kBlue);
	cherenkov->SetMarkerStyle(7);
	 //
   g1->SetParameters(0.0, 0.0);
	g1->SetLineWidth(1); 
	g1->SetLineColor(kRed); 
	 //
	c1->cd();
   cherenkov->Draw("AP");
   g1->Draw("same");
*/
	
	// writing histos 
	TFile out ("Ang_histos.root","recreate");
	TDirectory* Intervals = gDirectory->mkdir("Intervals");
	 //
	dEondX.Write(); 
	Angle.Write(); 

/*	Occupancy.SetDrawOption("COLZ");
	AngleVsdiff_dEondX.Write(); 	
	 //
	TCanvas * c2 = new TCanvas("c2", "c2", 0, 0, 400, 400);
	c2->cd();
	Occupancy.GetXaxis()->SetTitle("#phi");
	Occupancy.GetYaxis()->SetTitle("#eta");
	Occupancy.Draw("COLZ");
	c2->Write("Occupancy");
	 //
	Intervals->cd();
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
		diff_dEondX_histo_interv.at(iInterval)-> Write();
	out.cd();
	 // write graph
	c1->Write("cherenkov");
	 //
	out.Close();
	
	// deleting
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
		delete diff_dEondX_histo_interv.at (iInterval);
	delete gaussiana;	
*/	
	 
	return(0);
}



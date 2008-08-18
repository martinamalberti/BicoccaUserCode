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
	
	chain->SetBranchAddress ("muonTkLengthInEcalDetail", treeVars.muonTkLengthInEcalDetail);
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
	
	
	#define PI 3.14159265
	const double deg = PI/180;  // 1 deg in radians
// 	TVector3 Vertex (0.,0.,0.);

	TH1F dEondXAllEvents("dEondXAllEvents", "dEondXAllEvents", 130, 0., 0.07); 
	TH1F dEondX("dEondX", "dEondX", 130, 0., 0.07);
// 	TH1F dEondXTop("dEondXTop", "dEondXTop", 150, 0., 0.07); 
// 	TH1F dEondXBottom("dEondXBottom", "dEondXBottom", 150, 0., 0.07); 		 
	TH1F Angle("Angle", "Angle", 180, 0., PI);
// 	TH1F AngleTop("AngleTop", "AngleTop", 180, 0., PI);
// 	TH1F AngleBottom("AngleBottom", "AngleBottom", 180, 0., PI);
	 //
	TH2F Occupancy("Occupancy","Occupancy",360,-3.14,3.14,170,-1.47,1.47); 
// 	TH2F OccupancyTop("OccupancyTop","OccupancyTop",360,-3.14,3.14,170,-1.47,1.47); 
// 	TH2F OccupancyBottom("OccupancyBottom","OccupancyBottom",360,-3.14,3.14,170,-1.47,1.47); 

	// define angle intervals
	int AngleInterval = 7; 
	double step = 1;
	int nIntervals = (int)(90./AngleInterval/step);
	std::cout << "n. of intervals: "<< nIntervals << std::endl; 
	std::cout << " " << std::endl;	

	// creating array of angles = medium point of each interval
	double IntervalMeanAngle[50];
	 
	// creating array of peaks dEondX = mean of each gaussian fit
	double IntervaldEondXPeakBottom[50];
	double IntervaldEondXPeakErrorBottom[50];
	double IntervaldEondXPeakTop[50];
	double IntervaldEondXPeakErrorTop[50];
	double IntervaldEondXPeakTBDiff[50];
	double IntervaldEondXPeakErrorTBDiff[50];
	double IntervaldEondXPeakTBRatio[50];
	double IntervaldEondXPeakErrorTBRatio[50];
	
	 
	// create vector of histos
	std::vector <TH1F*> HistodEondXTop;		
	std::vector <TH1F*> HistodEondXBottom;
	 
	// create histos with different names and put them into vector && fill angle array               
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval){  
		
		char number[80];
		
		sprintf (number, "dEondXTop_%d", iInterval );
		TH1F* tempTop = new TH1F(number, number, 90, 0., 0.07);
		HistodEondXTop.push_back(tempTop);
		
		sprintf (number, "dEondXBottom_%d", iInterval );
		TH1F* tempBottom = new TH1F(number, number, 90, 0., 0.07);
		HistodEondXBottom.push_back(tempBottom);
		
		IntervalMeanAngle[iInterval] = (iInterval*step + iInterval*step +1 )*AngleInterval/2 ;
		std::cout << iInterval*AngleInterval*step << " - " << (iInterval*step+1)*AngleInterval << " ;  interv mean:" << (iInterval*step + iInterval*step +1 )*AngleInterval/2 << std::endl ;
   }
	
	//PG loop over entries
	for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)   
	{
		if(iEvent%10000 == 0) std::cout << "event n. " << iEvent << std::endl;
		chain->GetEntry (iEvent);
		
		// fill histo
		dEondXAllEvents.Fill( treeVars.cosmicClusterEnergy[0] / treeVars.muonTkLengthInEcalDetail[0] ); 
		
		// SELECTIONS 
		if (treeVars.nCosmicsCluster != 1) continue ; 
	
		// get directions
		TVector3 SC0_pos (0., 0., 0.) ;
		setVectorOnECAL (SC0_pos, treeVars.cosmicClusterEta[0], treeVars.cosmicClusterPhi[0]);
		
		TVector3 MuonDir (treeVars.muonMomentumX[0], treeVars.muonMomentumY[0], treeVars.muonMomentumZ[0]);
		
		double angle;
		angle = fabs( MuonDir.Angle( SC0_pos ) );
		
		// fill histos
		dEondX.Fill( treeVars.cosmicClusterEnergy[0] / treeVars.muonTkLengthInEcalDetail[0] ); 
		Angle.Fill(angle);
		Occupancy.Fill(treeVars.cosmicClusterPhi[0], treeVars.cosmicClusterEta[0]);   
		
		// fill angle histos
		if( treeVars.cosmicClusterPhi[0] > 0 )
		{
			for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval)
			{
				if( !( angle > iInterval*step*AngleInterval*deg  &&  angle < (iInterval*step+ 1)*AngleInterval*deg )  &&  
					 !( angle > (180-(iInterval*step+1)*AngleInterval)*deg  &&  angle < (180-iInterval*step*AngleInterval)*deg ) ) continue;
				{
					HistodEondXTop.at(iInterval)->Fill(treeVars.cosmicClusterEnergy[0] / treeVars.muonTkLengthInEcalDetail[0]);
				}	
			}
		}	
		
		else 
		{
			for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval)
			{
				if( !( angle > iInterval*step*AngleInterval*deg  &&  angle < (iInterval*step+ 1)*AngleInterval*deg )  &&  
					 !( angle > (180-(iInterval*step+1)*AngleInterval)*deg  &&  angle < (180-iInterval*step*AngleInterval)*deg ) ) continue;
				{
					HistodEondXBottom.at(iInterval)->Fill(treeVars.cosmicClusterEnergy[0] / treeVars.muonTkLengthInEcalDetail[0]);
				}	
			}
		}
// 		// ----- SELECTIONS -------:  
//  		if( !(angle > 55*deg && angle < 65*deg) && !(angle > 145*deg && angle < 155*deg)  ) continue;
		
		// divide Top Bottom && // refill histos
// 		if( treeVars.cosmicClusterPhi[0] > 0 ){
// 			dEondXTop.Fill( treeVars.cosmicClusterEnergy[0] / treeVars.muonTkLengthInEcalDetail[0] ); 
// 			AngleTop.Fill(angle);
// 			OccupancyTop.Fill(treeVars.cosmicClusterPhi[0], treeVars.cosmicClusterEta[0]);  
// 		}
// 			
// 		else{
// 			dEondXBottom.Fill( treeVars.cosmicClusterEnergy[0] / treeVars.muonTkLengthInEcalDetail[0] ); 
// 			AngleBottom.Fill(angle);
// 			OccupancyBottom.Fill(treeVars.cosmicClusterPhi[0], treeVars.cosmicClusterEta[0]);  
// 		}
	}//PG loop over entries 
	
	
	
	// get the peak of dEondX
	TF1 * gaussianast = new TF1("gaussianast","gaus", 0., 0.05);
	gaussianast->SetLineColor(kBlue);
	TF1 * gaussianand = new TF1("gaussianand","gaus", 0., 0.05);
	gaussianand->SetLineColor(kRed);	
	
// 	TF1 * fTop = new TF1("fTop","gaus", 0.005, 0.014);
// 	fTop->SetParameters(1000, 0.01, 0.001);
// 	dEondXTop.Fit("fTop","R");
// 	
// 	TF1 * fBottom = new TF1("fBottom","gaus", 0.005, 0.014);
// 	fBottom->SetParameters(1000, 0.01, 0.001);
// 	dEondXBottom.Fit("fBottom","R");
	
	//DEBUG
	double entries=0;
	
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
	{  
		// histos entries
		std::cout << entries << std::endl ;	
		entries += HistodEondXTop.at(iInterval)->GetEntries();
		std::cout << entries << std::endl ;	
		entries += HistodEondXBottom.at(iInterval)->GetEntries();
		std::cout << entries << std::endl ;
		
		// fit histos
		gaussianast->SetRange(HistodEondXTop.at(iInterval)->GetMean() - 1.1*HistodEondXTop.at(iInterval)->GetRMS(), HistodEondXTop.at(iInterval)->GetMean() - 0.2*HistodEondXTop.at(iInterval)->GetRMS() );			
		gaussianast->SetParameters( 100, HistodEondXTop.at(iInterval)->GetMean(), HistodEondXTop.at(iInterval)->GetRMS() );
 		HistodEondXTop.at(iInterval)->Fit("gaussianast","R");

		gaussianand->SetRange(gaussianast->GetParameter(1) - 1.6*gaussianast->GetParameter(2), gaussianast->GetParameter(1) + 0.7*gaussianast->GetParameter(2) );	
		gaussianand->SetParameters( gaussianast->GetParameter(0), gaussianast->GetParameter(1), gaussianast->GetParameter(2) );
		HistodEondXTop.at(iInterval)->Fit("gaussianand","R+");
		
		IntervaldEondXPeakTop[iInterval] = gaussianand->GetParameter(1);
		IntervaldEondXPeakErrorTop[iInterval] = gaussianand->GetParError(1);
		
		gaussianast->SetRange(HistodEondXBottom.at(iInterval)->GetMean() - 1.1*HistodEondXBottom.at(iInterval)->GetRMS(), HistodEondXBottom.at(iInterval)->GetMean() - 0.2*HistodEondXBottom.at(iInterval)->GetRMS() );	
		gaussianast->SetParameters( 100, HistodEondXBottom.at(iInterval)->GetMean(), HistodEondXBottom.at(iInterval)->GetRMS() );
		HistodEondXBottom.at(iInterval)->Fit("gaussianast","R");
		
		gaussianand->SetRange(gaussianast->GetParameter(1) - 1.6*gaussianast->GetParameter(2), gaussianast->GetParameter(1) + 0.7*gaussianast->GetParameter(2) );	
		gaussianand->SetParameters( gaussianast->GetParameter(0), gaussianast->GetParameter(1), gaussianast->GetParameter(2) );		
		HistodEondXBottom.at(iInterval)->Fit("gaussianand","R+");
		
		IntervaldEondXPeakBottom[iInterval] = gaussianand->GetParameter(1);
		IntervaldEondXPeakErrorBottom[iInterval] = gaussianand->GetParError(1);
		
		IntervaldEondXPeakTBDiff[iInterval]=IntervaldEondXPeakTop[iInterval]  - IntervaldEondXPeakBottom[iInterval] ;
		IntervaldEondXPeakErrorTBDiff[iInterval]=sqrt( pow(IntervaldEondXPeakErrorBottom[iInterval],2) + pow(IntervaldEondXPeakErrorTop[iInterval],2) );
		
		IntervaldEondXPeakTBRatio[iInterval]=IntervaldEondXPeakTop[iInterval]/IntervaldEondXPeakBottom[iInterval] ;
		IntervaldEondXPeakErrorTBRatio[iInterval]=1/IntervaldEondXPeakBottom[iInterval] * sqrt( pow(IntervaldEondXPeakErrorBottom[iInterval],2) + pow(IntervaldEondXPeakTop[iInterval]/IntervaldEondXPeakBottom[iInterval]*IntervaldEondXPeakErrorBottom[iInterval], 2) );
		
// 		IntervaldEondXPeakDiff[iInterval]  = IntervaldEondXPeakTop[iInterval]  - IntervaldEondXPeakBottom[iInterval] 
// 		IntervaldEondXPeakDiffError[iInterval]  = IntervaldEondXPeakErrorTop[iInterval]  + IntervaldEondXPeakErrorBottom[iInterval] 
// 		std::cout <<  " mean_aft_fit " << diff_dEondX_interv_mean[iInterval] << " sigma_aft_fit " << diff_dEondX_interv_sigma[iInterval] << std::endl;
// 	std::cout << " sigma media " << diff_dEondX_interv_mean[iInterval]/sqrt(diff_dEondX_histo_interv.at(iInterval)->GetEntries()) << " par err " << diff_dEondX_interv_sigma[iInterval] << std::endl;
	}

	std::cout << " " << std::endl ;		
	std::cout << " " << std::endl ;		
	std::cout << entries << std::endl ;	
	std::cout << " " << std::endl ;		
	std::cout << " " << std::endl ;		
	
	TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 400, 400);
	TGraphErrors * gTop = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTop, 0, IntervaldEondXPeakErrorTop);
	TGraphErrors * gBottom = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakBottom, 0, IntervaldEondXPeakErrorBottom);
	TGraphErrors * gTBdiff = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTBDiff, 0, IntervaldEondXPeakErrorTBDiff);
	
	TCanvas* c2 = new TCanvas("c2", "c2", 0, 0, 400, 400);	
	TGraphErrors * gTBratio = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTBRatio, 0, IntervaldEondXPeakErrorTBRatio);	
// 	TGraphErrors * gdiffTB = new TGraphErrors(nIntervals, IntervalMeanAngle, (IntervaldEondXPeakTop - IntervaldEondXPeakBottom), 0, IntervaldEondXPeakErrorBottom);	
	
// 	TGraphErrors * gdiffTB = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTop-IntervaldEondXPeakBottom, 0, IntervaldEondXPeakErrorTop+IntervaldEondXPeakErrorBottom);
// 	TF1 * g1 = new TF1("g1", "pol1", 0., 80.);
	 
// 	g->SetTitle("diff_dEondX Bot-Top");
// 	cherenkov->GetXaxis()->SetTitle("angle (deg)");
// 	cherenkov->GetYaxis()->SetTitle("diff_dEondX (GeV/cm)");
// 	cherenkov->GetYaxis()->SetRangeUser(-0.01,0.01);
// 	cherenkov->GetYaxis()->SetTitleOffset(1.1);

	gTop->SetMarkerColor(kBlue);
	gTop->SetMarkerStyle(7);
	gBottom->SetMarkerColor(kRed);
	gBottom->SetMarkerStyle(7);
	
	gTBdiff->SetMarkerColor(kMagenta);
	gTBdiff->SetMarkerStyle(7);
	gTBratio->SetMarkerColor(kGreen);
	gTBratio->SetMarkerStyle(7);	


//    g1->SetParameters(0.0, 0.0);
// 	g1->SetLineWidth(1); 
// 	g1->SetLineColor(kRed); 

	c1->cd();
   gTop->Draw("AP");
	gBottom->Draw("P");
	gTBdiff->Draw("P");
	
	c2->cd();
	gTBratio->Draw("AP");

	
// 	// fit dEondX
// 	TF1 * f = new TF1("f","gaus", 0.006, 0.014);
// 	f->SetParameters(1000, 0.01, 0.001);
// 	dEondX.Fit("f","R");
// 	
// 	TF1 * fTop = new TF1("fTop","gaus", 0.005, 0.014);
// 	fTop->SetParameters(1000, 0.01, 0.001);
// 	dEondXTop.Fit("fTop","R");
// 	
// 	TF1 * fBottom = new TF1("fBottom","gaus", 0.005, 0.014);
// 	fBottom->SetParameters(1000, 0.01, 0.001);
// 	dEondXBottom.Fit("fBottom","R");

	
	// writing on file 
	TFile out ("angoli_histos.root","recreate");
	
 	TDirectory * Intervals = gDirectory->mkdir("Intervals");
	
	dEondXAllEvents.Write(); 
	
	dEondX.Write(); 
	Angle.Write(); 
	Occupancy.Write();
	
	Intervals->cd();
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
	{
		HistodEondXTop.at(iInterval)->Write();
		HistodEondXBottom.at(iInterval)->Write();
	}
	out.cd();

	c1->Write("FinalGraph");
	c2->Write("FinalGraph_2");	
	 
	out.Close();
	
	// deleting
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval)
	{ 
		delete HistodEondXTop.at(iInterval);
		delete HistodEondXBottom.at(iInterval);
	}
		
// 	delete gaussianast;	

	out.Close();
	
		 	
// 	dEondXTop.Write(); 
// 	AngleTop.Write(); 
// 	OccupancyTop.Write();
// 	
// 	dEondXBottom.Write(); 
// 	AngleBottom.Write(); 
// 	OccupancyBottom.Write();
	
	
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


*/	
	 
	return(0);
}



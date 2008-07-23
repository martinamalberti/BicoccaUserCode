//COMPILO c++ -o angoli -lm `root-config --cflags --libs --glibs` angoli.cc angoli.cpp


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
	chain->Add ("~/public/MuonTreeLungTestMatrix_43439_*.root") ;
	
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
	 //
	int nEvents = (int) chain->GetEntries () ; 
	std::cout << "events " << nEvents << std::endl;
	std::cout << " " << std::endl;
	 //
	double ECALinRad = 129. ;
	#define PI 3.14159265
	const double deg = PI/180; // 1 deg in radians
	double g_deltaAlpha = 0.3 ;
	TVector3 Vertex (0.,0.,0.);
	 //	
	TH1F diff_dEondX ("diff_dEondX","diff_dEondX Bot-Top",200,-0.2,0.2);
	TH1F diff_dEondX_good ("diff_dEondX_good","diff_dEondX Bot-Top good", 200, -0.2, 0.2);
	TH1F AngleTop ("AngleTop","AngleTop",180,0.,2*PI/2);
	TH1F AngleBot ("AngleBot","AngleBot",180,0.,2*PI/2);
	TH1F diff_Angle_TB ("diff_Angle_TB","diff_Angle_TB",100, 0, 45*deg);
	TH1F Angle ("Angle","Angle", 180, 0., PI);
	
	 // creating histos for angle intervals division of muon tracks
	double delta_angle; 
	std::cout << "choose angular interval in deg ( 80° is the max TB angle ) " << std::endl;
	std::cin >> delta_angle;
// 	std::cout << "delta_angle is "  << delta_angle << std::endl;
	std::cout << " " << std::endl;
	 //
	int nIntervals = (int) (80. / delta_angle) ; 
	std::cout << "nIntervals is " << nIntervals << std::endl;
	std::cout << " " << std::endl;
	
	// creating array of angles = medium point of each interval
	double mean_angle_interv[50];
	
	// creating array of diff_dEondX = mean of each gaussian fit
	double diff_dEondX_interv_mean[50];
	double diff_dEondX_interv_sigma[50];
	
	double diff_angle_TB; 
	std::cout << "choose the max difference for TB angle in deg" << std::endl;
	std::cin >> diff_angle_TB;
// 	std::cout << "diff_angle_TB is " << diff_angle_TB << std::endl;

	// create vector of histos
	std::vector <TH1F*> diff_dEondX_histo_interv;
	
	// create histos with different names and put them in histo vector && fill the vector of medium points of intervals
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
   {  
		char number[80];
		sprintf (number, "diff_dEondX_%d", iInterval );
		TH1F* temp = new TH1F(number, number, 200, -0.1, 0.1);
		diff_dEondX_histo_interv.push_back(temp);
		 //
		mean_angle_interv[iInterval] = (iInterval + iInterval+1)*delta_angle/2 ;
// 		std::cout << "medium point of " << iInterval << " interval is: " << mean_angle_interv[iInterval] << std::endl;
   }
 
/************************************************************************************************************************************/	 
	//PG loop over entries
	for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)    // 	for (int iEvent = 0 ; iEvent < 20 ; ++iEvent)    
  	{
		if(iEvent%10000 == 0) std::cout << "event n. " << iEvent << std::endl;
		
		chain->GetEntry (iEvent);
		
		// base selections
		if (treeVars.nCosmicsCluster != 2) continue ; // events 2 clusters
		if(deltaPhi(treeVars.cosmicClusterPhi[0],treeVars.cosmicClusterPhi[1]) < 2*PI / 4./*90gradi*/) continue; // DELTAPHI
// std::cout << "num SC : " << treeVars.nCosmicsCluster << std::endl ;
		
		//calculating muon trajectory
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
		if(deltaPhi(primo.first.Phi(),2*PI/4.) > deltaPhi(secondo.first.Phi(),2*PI/4.)) std::swap(primo,secondo);  //primo el a phi + alto
		segmenti.push_back(primo);
		segmenti.push_back(secondo);
		}
		else
		{
		std::pair<TVector3,TVector3> primo (int1, ext2);
		std::pair<TVector3,TVector3> secondo (int2, ext1);
		if(deltaPhi(primo.first.Phi(),2*PI/4.) > deltaPhi(secondo.first.Phi(),2*PI/4.)) std::swap(primo,secondo);  //il primo elemento di segmenti e' a phi + alto
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
	
/***********************************************************************************************************************************/
			
		// filling histos
		diff_dEondX.Fill(enerBot / lunghBot - enerTop / lunghTop); 
		AngleTop.Fill(angleTop);
		AngleBot.Fill(angleBot);
		diff_Angle_TB.Fill(fabs(angleTop-angleBot));  
// 		if(iEvent%5000 == 0) std::cout << "diff_Angle_TB " << fabs(angleTop-angleBot) << std::endl;

		// select events with minimun diff in TB angle to find cherenkov eff.  // Ideally you want only trajectories perfectly perpendicular to the 
		if( fabs(angleTop-angleBot) > diff_angle_TB*deg ) continue;	           // cylinder axis 

		diff_dEondX_good.Fill(enerBot / lunghBot - enerTop / lunghTop);
		
		// define angle of muon trajectory 
		double angle = (angleTop + angleBot)/2;  
		Angle.Fill(angle);  
// 	 	std::cout << "angle " << angle<< std::endl;
// 		if(iEvent%5000 == 0) std::cout << "angle " << angle<< std::endl;

		// define angle intervals
		for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
		{
			if( iInterval*delta_angle*deg < angle && angle < (iInterval+1)*delta_angle*deg ) 
				diff_dEondX_histo_interv.at(iInterval)->Fill(enerBot / lunghBot - enerTop / lunghTop);
		}
	}//PG loop over entries 
	
	// fit histos of intervals
	int peakBin = 0;
	double norm = 100.;
	double mean = 0.;
	double sigma = 1.;
	TF1 * gaussiana = new TF1("gaussiana","gaus", -0.2, 0.2);
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
	{  
// 	   peakBin = diff_dEondX_histo_interv.at(iInterval)->GetMaximumBin();
// 		norm = diff_dEondX_histo_interv.at(iInterval)->GetMaximum();
// 		norm = diff_dEondX_histo_interv.at(iInterval)->GetIntegral();
// 		std::cout <<  " norm " << norm << "     " << *norm << std::endl;
// 		mean = diff_dEondX_histo_interv.at(iInterval)->GetBinCenter(peakBin);
		mean = diff_dEondX_histo_interv.at(iInterval)->GetMean();
		sigma = diff_dEondX_histo_interv.at(iInterval)->GetRMS();
// 		std::cout << " mean " << mean << " sigma " << sigma <<   " norm " << norm << std::endl;
		gaussiana->SetParameters(norm, mean, sigma);

 		diff_dEondX_histo_interv.at(iInterval)->Fit("gaussiana","R");
//  		diff_dEondX_histo_interv.at(iInterval)->Fit("gaussiana");
		
		diff_dEondX_interv_mean[iInterval] = gaussiana->GetParameter(1);
		diff_dEondX_interv_sigma[iInterval] = gaussiana->GetParameter(2);
// 		std::cout <<  " mean_aft_fit " << diff_dEondX_interv_mean[iInterval] << " sigma_aft_fit " << diff_dEondX_interv_sigma[iInterval] << std::endl;
	}
	
	// final graph diff_dEondX to see cherenkov eff
	TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 500);
	TGraphErrors * cherenkov = new TGraphErrors(nIntervals, mean_angle_interv, diff_dEondX_interv_mean, 0, diff_dEondX_interv_sigma);
	TF1 * g1 = new TF1("g1", "pol1", 0., 80.);

	cherenkov->SetTitle("diff_dEondX Bot-Top");
	cherenkov->GetXaxis()->SetTitle("angle (deg)");
	cherenkov->GetYaxis()->SetTitle("diff_dEondX (?)");

	cherenkov->SetMarkerColor(kBlue);
	cherenkov->SetMarkerStyle(7);
	
   g1->SetParameters(0.0, 0.0);
	g1->SetLineWidth(1); 
	g1->SetLineColor(kRed); 
	
	c1->cd();
   cherenkov->Draw("AP");
   g1->Draw("same");

	//writing histos 
	TFile out ("angoli_histos.root","recreate");
	TDirectory* Intervals = gDirectory->mkdir("Intervals");
		//
	diff_dEondX.Write();
	AngleTop.Write();
	AngleBot.Write();
	diff_Angle_TB.Write();
	diff_dEondX_good.Write();
	Angle.Write();	
	 //
	Intervals->cd();
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
		diff_dEondX_histo_interv.at(iInterval)-> Write();
	out.cd();
	 
	// write graph
// 	cherenkov->Write("cherenkov");
	c1->Write("c1");


	out.Close();
	
	// deleting
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
		delete diff_dEondX_histo_interv.at (iInterval);
	delete gaussiana;	
	 
	return(0);
}

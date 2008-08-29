#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"


#include "datiSimple.h"
#include "histo.h"

//COMPILO c++ -o merge `root-config --cflags --libs --glibs` merge.cpp 

/// DeltaR square Function -------------------------------------------------

double DeltaR2(double eleEta, double jetEta, double elePhi, double jetPhi) {
             double deltaEta = jetEta - eleEta ;
	     double deltaPhi = jetPhi - elePhi ;
             if (deltaPhi > M_PI) deltaPhi -= 2.*M_PI ;
             if (deltaPhi < -M_PI) deltaPhi += 2.*M_PI ;

             double deltaR2 = pow(deltaEta,2.) + pow(deltaPhi, 2.) ;
             return(deltaR2);
	     }



/// MAIN ---------------------------------------------------------------


int main (int argc, char ** argv)
{

//Root SetStyle 
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1111111);
  gStyle->SetOptFit(1111);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.08);
  gStyle->SetTitleY(0.97);
  gStyle->SetPalette(1,0);


//CC - Non giro su tutti i dati ma solo su una decina di file altrimenti ci mette troppo
///CC - A chain is a collection of files containg TTree objects. When the chain is created, the first parameter is the default name for the Tree to be processed later on. Enter a new element in the chain via the TChain::Add function. Once a chain is defined, one can use the normal TTree functions to Draw,Scan,etc.

  TChain fChain ("elminitree") ;
  fChain.Add("rfio:/castor/cern.ch/user/c/cconso/ntuple/*");
  
  ///CC - dati is defined in DatiSimple.h
  dati Input(&fChain);

  ///Map for rescale - weights from CSA07EventWeightproducer
  std::map<int,double> rescale;
  //number = cross section in 1 pb-1 div by #events (MB HS+ QCD bin)
  rescale[15] = 53.0E+9 / (13.75E+06 * 53.0 / 55.0 + 0.75E+06);   
  rescale[20] =  1.46E+9 / (13.75E+06 * 1.46 / 55.0 + 1.3E+06);  
  rescale[30] =  0.63E+9 / (13.75E+06 * 0.63 / 55.0 + 2.5E+06);  
  rescale[50] =  0.163E+9 / (13.75E+06 * 0.163 / 55.0 + 2.5E+06);  
  rescale[80] =  21.6E+06 / (13.75E+06 * 0.0216 / 55.0 + 2.5E+06);     
  rescale[120] =  3.08E+06 / (13.75E+06 * 0.00308 / 55.0 + 1.18E+06);
  //number = cross section in 1 pb-1 div by #events (QCD bin). MB negligible  
  rescale[170] =  0.494E+06 / (1.25E+06);                         
  rescale[230] =  0.101E+06 / (1.16E+06);     
  rescale[300] =  24.5E+03 / (1.20E+06);      
  rescale[380] =  6.24E+03 / (1.18E+06);  
  rescale[470] =  1.78E+03 / (1.19E+06);    
  rescale[600] =  0.683E+03 / (1.23E+06);  
  rescale[800] =  0.204E+03 / (0.5E+06);
  rescale[1000] =  35.1E+00 / (0.1E+06);   
  rescale[1400] =  10.9E+00 / (3.0E+04);  
  rescale[1800] =  1.6E+00 / (3.0E+04); 
  rescale[2200] =  0.145E+00 / (2.0E+04);

  ///CC - numeratore e denominatore
  std::map<int,histos*> hnum;
  std::map<int,histos*> hden;

  //loop over the events
  ///CC - stops when the number of entries in fChain is reached

  ///CC - Variables used in the loop
  int pthatevent = 0 ;   
  int indexMax = -999 ;
  float dummyMax = -999. ; 
  int fIndex = 3 ; 
  int selected = 0 ;

  TH2F * distrib = new TH2F("elePT_jetPTMatch_vs_jetPTMatch", "elePT_jetPTMatch_vs_jetPTMatch", 18, 10., 100., 240, 0., 4.);
    
  for (int index = 0 ; index < fChain.GetEntries () ; ++index ){
	  /**Get entry from the file to memory.
	  getall = 0 : get only active branches
	  getall = 1 : get all branches
          Return the total number of bytes read,0 bytes read indicates a failure. **/

	  ///CC - Get the event "index"
	  fChain.GetEntry (index);
	  
	  ///CC - Prints event number
	  if (index%1000 == 0) std::cout << "Numero evento : " << index << std::endl;

          //for production problems consider only ptHat<600
	  if(Input.csa07Info.ptHat <= 300){
	     //LM consider only INTERISTING events
	     if ( (Input.csa07Info.procId==11)||
		  (Input.csa07Info.procId==12)||
		  (Input.csa07Info.procId==13)||
		  (Input.csa07Info.procId==28)||
		  (Input.csa07Info.procId==53)||
		  (Input.csa07Info.procId==68) ){

			  pthatevent = 0 ;   ///azzero il pt hat precedente
			  if (Input.csa07Info.ptHat >= 0 && Input.csa07Info.ptHat < 15)
        		      pthatevent = 15 ;
			  if (Input.csa07Info.ptHat >= 15 && Input.csa07Info.ptHat < 20)
        		      pthatevent = 20 ;
			  if (Input.csa07Info.ptHat >=20 && Input.csa07Info.ptHat < 30)
        		      pthatevent = 30 ;
			  if (Input.csa07Info.ptHat >=30 && Input.csa07Info.ptHat < 50)
        		      pthatevent = 50 ;
			  if (Input.csa07Info.ptHat >= 50 && Input.csa07Info.ptHat < 80)
        		      pthatevent = 80 ;
			  if (Input.csa07Info.ptHat >= 80 && Input.csa07Info.ptHat < 120)
        		      pthatevent = 120 ;
			  if (Input.csa07Info.ptHat >= 120 && Input.csa07Info.ptHat < 170)
        		      pthatevent = 170 ;     
			  if (Input.csa07Info.ptHat >= 170 && Input.csa07Info.ptHat < 230)
        		      pthatevent = 230 ;
			  if (Input.csa07Info.ptHat >= 230 && Input.csa07Info.ptHat < 300)
        		      pthatevent = 300 ;	  
			  if (Input.csa07Info.ptHat >= 300 && Input.csa07Info.ptHat < 380)
        		      pthatevent = 380 ;
			  if (Input.csa07Info.ptHat >= 380 && Input.csa07Info.ptHat < 470)
        		      pthatevent = 470 ;
			  if (Input.csa07Info.ptHat >= 470 && Input.csa07Info.ptHat < 600)
        		      pthatevent = 600 ;
			  if (Input.csa07Info.ptHat >= 600 && Input.csa07Info.ptHat < 800)
        		      pthatevent = 800 ;
			  if (Input.csa07Info.ptHat >= 800 && Input.csa07Info.ptHat < 1000)
        		      pthatevent = 1000 ;
			  if (Input.csa07Info.ptHat >= 1000 && Input.csa07Info.ptHat < 1400)
        		      pthatevent = 1400 ;    

			  ///-----> ripristino questa riga che puo' essere utile
			  //std::cout<<"PThat event "<< pthatevent <<std::endl;
			 
			  ///CC - Looks for the histogram of the pthatevent selected unless a new histo is created (via dummy)
			  if (!hnum.count (pthatevent)){
				  histos* dummy = new histos(pthatevent) ;
				  hnum[pthatevent] = dummy ;
			  }

			  ///CC - Looks for the histogram of the pthatevent selected unless a new histo is created (via dummy)
			  if (!hden.count (pthatevent)){
				  histos* dummy = new histos(pthatevent) ;
				  hden[pthatevent] = dummy ;
			  }
			  hden[pthatevent]->grow () ; 

			 /// reset variables
			 indexMax = -999;
			 dummyMax = -999.;
			 for (int i = 0 ; i < 50 ; ++i){
				 if ( Input.jetPT[i] == 0 &&
				      Input.jetEta[i] == 0 &&
				      Input.jetPhi[i] == 0 &&
				      Input.jetFlav[i] == 0 )
					 continue ;
				 ///CC - get the max PT Jet
				 if (Input.jetPT[i] > dummyMax){
					 indexMax = i ;
					 dummyMax = Input.jetPT[i] ;
				 }
				 //std::cout << "indexMax: " << indexMax <<" - " << dummyMax << std::endl;
			 }

			  //start loop over the RECO jets
			  for (int i = 0 ; i < 50 ; ++i){
					fIndex = 3 ; //failed flavour
					if (    Input.jetFlav[i] == 1 ||
						Input.jetFlav[i] == 2 ||
						Input.jetFlav[i] == 3 )		fIndex = 0 ; //light flavour uds
					if (    Input.jetFlav[i] == 4 )		fIndex = 1 ; //c quark
					if (    Input.jetFlav[i] == 5 )		fIndex = 2 ; //b quark
					if (    Input.jetFlav[i] > 20 &&
						Input.jetFlav[i] < 25 )         fIndex = 4 ; //gloun
					
					/// CC - fill the denominator
					/// CC - Cuts over JetPT, use CandHLT1jetPE7 (High Level Trigger Bit - cuts at 30 Gev)
					/// CC - isolation eta < 2.5, 
					/// i!= indexMax excludes leading jet
					if (    Input.HLT1jetPE7Bit &&
						//Input.jetPT[indexMax] > 30. &&
						i!= indexMax &&
						fabs(Input.jetEta[i]) < 2.5) {
						hden[pthatevent]->m_e_single_resol_eta->Fill (Input.jetEta[i]) ;
						hden[pthatevent]->m_e_single_resol_ptJ->Fill (Input.jetPT[i]) ;
						hden[pthatevent]->m_e_single_resol_eta_flav[fIndex]->Fill (Input.jetEta[i]) ;
						hden[pthatevent]->m_e_single_resol_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
						}
			  }//end loop over the RECO jets
			  hnum[pthatevent]->grow () ; //PG questo serve ancora?

			  //start loop over the RECO electrons
			  for (int i = 0 ; i < Input.eleNum ; ++i){
				  
				  //check the flavour if the closest jet to the electron
				  fIndex = 3 ;
				  if (  Input.jetFlavour[i] == 1 ||
					Input.jetFlavour[i] == 2 ||
					Input.jetFlavour[i] == 3 )	fIndex = 0 ;
				  if (  Input.jetFlavour[i] == 4 )	fIndex = 1 ;
				  if (  Input.jetFlavour[i] == 5 )      fIndex = 2 ;
				  if (  Input.jetFlavour[i] > 20 &&
					Input.jetFlavour[i] < 25 )      fIndex = 4 ;
				  
				distrib->Fill(Input.jetPTMatch[i], Input.elePT[i]/Input.jetPTMatch[i]);	
					
				  selected = 0 ;
				  //PG amb resolving passed (qui c'e' il taglio su jet pt)
				  ///CC - fill the numerator
				  ///CC - Cuts over elePT > 10, eleEta < 2.5, DeltaR < 0.3
				  ///CC - DeltaR = sqrt( DeltaEta^2 + DeltaPhi^2)
				  if (  Input.ambiguityBit[i] &&
					Input.elePT[i] > 10. &&
					Input.HLT1jetPE7Bit &&
					fabs(Input.eleEta[i]) < 2.5 &&
					fabs(Input.jetEtaMatch[i]) < 2.5 &&					
					(DeltaR2(Input.eleEta[i], Input.jetmaxEta[0], Input.elePhi[i], Input.jetmaxPhi[0]) > 0.09) )
					{
					  hnum[pthatevent]->m_e_single_resol_eta->Fill (Input.jetEtaMatch[i]) ;
					  hnum[pthatevent]->m_e_single_resol_ptE->Fill (Input.elePT[i]) ;
					  hnum[pthatevent]->m_e_single_resol_ptJ->Fill (Input.jetPTMatch[i]) ;
					  hnum[pthatevent]->m_e_single_resol_EMfrac->Fill (Input.EMjetCompon[i]) ;
					  hnum[pthatevent]->m_e_single_resol_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
					  hnum[pthatevent]->m_e_single_resol_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
					  hnum[pthatevent]->m_e_single_resol_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
					  hnum[pthatevent]->m_e_single_resol_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_eta->Fill (Input.jetEtaMatch[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_ptE->Fill (Input.elePT[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_ptJ->Fill (Input.jetPTMatch[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_EMfrac->Fill (Input.EMjetCompon[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
					  hnum[pthatevent]->m_e_sequence_resol_EMfrac_flav[fIndex]->Fill(Input.EMjetCompon[i]) ;
					  selected = 1 ;
					}
				 else selected = 0 ;
				
				if (    Input.eleIdTightBit[i] &&
				        Input.eleClass[i] != 40 &&
					Input.ambiguityBit[i] &&
					Input.elePT[i] > 10. &&
					Input.HLT1jetPE7Bit &&
					fabs(Input.eleEta[i]) < 2.5 &&
					fabs(Input.jetEtaMatch[i]) < 2.5 &&					
					(DeltaR2(Input.eleEta[i], Input.jetmaxEta[0], Input.elePhi[i], Input.jetmaxPhi[0]) > 0.09)
				   )
				{
					hnum[pthatevent]->m_e_single_eleIdTight_eta->Fill (Input.jetEtaMatch[i]) ;
					hnum[pthatevent]->m_e_single_eleIdTight_ptE->Fill (Input.elePT[i]) ;
					hnum[pthatevent]->m_e_single_eleIdTight_ptJ->Fill (Input.jetPTMatch[i]) ;
					hnum[pthatevent]->m_e_single_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
					hnum[pthatevent]->m_e_single_eleIdTight_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
					hnum[pthatevent]->m_e_single_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
					hnum[pthatevent]->m_e_single_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
					hnum[pthatevent]->m_e_single_eleIdTight_EMfrac_flav[fIndex]->Fill(Input.EMjetCompon[i]) ;
					if (selected == 1)
					{
						hnum[pthatevent]->m_e_sequence_eleIdTight_eta->Fill (Input.jetEtaMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_eleIdTight_ptE->Fill (Input.elePT[i]) ;
						hnum[pthatevent]->m_e_sequence_eleIdTight_ptJ->Fill (Input.jetPTMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
						hnum[pthatevent]->m_e_sequence_eleIdTight_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
						hnum[pthatevent]->m_e_sequence_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_eleIdTight_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
					}
				} else selected = 0 ;
				
				if (    Input.tkIsoBit[i] &&
					Input.ambiguityBit[i] &&
					Input.elePT[i] > 10. &&
					Input.HLT1jetPE7Bit &&
					fabs(Input.eleEta[i]) < 2.5 &&
					fabs(Input.jetEtaMatch[i]) < 2.5 &&					
					(DeltaR2(Input.eleEta[i], Input.jetmaxEta[0], Input.elePhi[i], Input.jetmaxPhi[0]) > 0.09) )
				{
					hnum[pthatevent]->m_e_single_tkIso_eta->Fill (Input.jetEtaMatch[i]) ;
					hnum[pthatevent]->m_e_single_tkIso_ptE->Fill (Input.elePT[i]) ;
					hnum[pthatevent]->m_e_single_tkIso_ptJ->Fill (Input.jetPTMatch[i]) ;
					hnum[pthatevent]->m_e_single_tkIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
					hnum[pthatevent]->m_e_single_tkIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
					hnum[pthatevent]->m_e_single_tkIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
					hnum[pthatevent]->m_e_single_tkIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
					hnum[pthatevent]->m_e_single_tkIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
					if (selected == 1) 
					{
						hnum[pthatevent]->m_e_sequence_tkIso_eta->Fill (Input.jetEtaMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_tkIso_ptE->Fill (Input.elePT[i]) ;
						hnum[pthatevent]->m_e_sequence_tkIso_ptJ->Fill (Input.jetPTMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_tkIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
						hnum[pthatevent]->m_e_sequence_tkIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_tkIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
						hnum[pthatevent]->m_e_sequence_tkIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
						hnum[pthatevent]->m_e_sequence_tkIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
					}
				} else selected = 0 ;
				
				/*
				if (Input.hadIsoBit[i])
				{
				istogrammi[pthatevent]->m_e_single_hadIso_eta->Fill (Input.jetEtaMatch[i]) ;
				istogrammi[pthatevent]->m_e_single_hadIso_ptE->Fill (Input.elePT[i]) ;
				istogrammi[pthatevent]->m_e_single_hadIso_ptJ->Fill (Input.jetPTMatch[i]) ;
				istogrammi[pthatevent]->m_e_single_hadIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
				istogrammi[pthatevent]->m_e_single_hadIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
				istogrammi[pthatevent]->m_e_single_hadIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
				istogrammi[pthatevent]->m_e_single_hadIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
				istogrammi[pthatevent]->m_e_single_hadIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
				if (selected == 1)
					{
					istogrammi[pthatevent]->m_e_sequence_hadIso_eta->Fill (Input.jetEtaMatch[i]) ;
					istogrammi[pthatevent]->m_e_sequence_hadIso_ptE->Fill (Input.elePT[i]) ;
					istogrammi[pthatevent]->m_e_sequence_hadIso_ptJ->Fill (Input.jetPTMatch[i]) ;
					istogrammi[pthatevent]->m_e_sequence_hadIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
					istogrammi[pthatevent]->m_e_sequence_hadIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
					istogrammi[pthatevent]->m_e_sequence_hadIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
					istogrammi[pthatevent]->m_e_sequence_hadIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
					istogrammi[pthatevent]->m_e_sequence_hadIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
					}
				} else selected = 0 ;
				if (Input.eleIdBit[i])
				{
				istogrammi[pthatevent]->m_e_single_eleId_eta->Fill (Input.jetEtaMatch[i]) ;
				istogrammi[pthatevent]->m_e_single_eleId_ptE->Fill (Input.elePT[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleId_ptJ->Fill (Input.jetPTMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleId_EMfrac->Fill (Input.EMjetCompon[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleId_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleId_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleId_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleId_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              			if (selected == 1) 
					{
                  			istogrammi[pthatevent]->m_e_sequence_eleId_eta->Fill (Input.jetEtaMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleId_ptE->Fill (Input.elePT[i]) ;
					istogrammi[pthatevent]->m_e_sequence_eleId_ptJ->Fill (Input.jetPTMatch[i]) ;
					istogrammi[pthatevent]->m_e_sequence_eleId_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleId_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleId_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleId_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleId_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                			}
            			} 

          			if (Input.eleIdLooseBit[i]) 
				{
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_eta->Fill (Input.jetEtaMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_ptE->Fill (Input.elePT[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_ptJ->Fill (Input.jetPTMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_EMfrac->Fill (Input.EMjetCompon[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdLoose_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              			if (selected == 1) 
                			{
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_eta->Fill (Input.jetEtaMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptE->Fill (Input.elePT[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptJ->Fill (Input.jetPTMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdLoose_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                			}
            			} 
          			
				if (Input.eleIdTightBit[i] && Input.jetmaxPT[i] > 30 )//&& (Input.eleClass[i] != 40)) 
            			{
              			istogrammi[pthatevent]->m_e_single_eleIdTight_eta->Fill (Input.jetEtaMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdTight_ptE->Fill (Input.elePT[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdTight_ptJ->Fill (Input.jetPTMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdTight_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
              			istogrammi[pthatevent]->m_e_single_eleIdTight_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              			if (selected == 1 )//&& (Input.eleClass[i] == 0 || Input.eleClass[i] == 100)) 
                			{        
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta->Fill (Input.jetEtaMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptE->Fill (Input.elePT[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ->Fill (Input.jetPTMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                  			istogrammi[pthatevent]->m_e_sequence_eleIdTight_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                			}
            			} 
*/
           }
	 }
       }  //PG loop of the electrons per event
    }//end of the loop over the events
  
  ///rescale histos (hnum e hden) per pthat
  for (std::map<int,histos*>::iterator istoIt = hnum.begin () ;
       istoIt != hnum.end () ;
       ++istoIt)
    {
      (istoIt->second)->rescale(rescale[istoIt->first]) ;
    }
  for (std::map<int,histos*>::iterator istoIt = hden.begin () ;
       istoIt != hden.end () ;
       ++istoIt)
    {
      (istoIt->second)->rescale(rescale[istoIt->first]) ;
    }
  
  ///write histos
  for (std::map<int,histos*>::iterator istoIt = hnum.begin () ;
       istoIt != hnum.end () ;
       ++istoIt)
  {
	    (istoIt->second)->write("NUM") ;
  }
  for (std::map<int,histos*>::iterator istoIt = hden.begin () ;
       istoIt != hden.end () ;
       ++istoIt)
  {
	  (istoIt->second)->write("DEN") ;
  }

  
  TFile shape("Shape.root", "RECREATE");
  
  distrib->Write();
  shape.Close();
  
   return(0);
} // int main

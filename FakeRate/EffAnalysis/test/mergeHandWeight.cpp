//comment line
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

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
#include "histoHandWeight.h"

//COMPILO c++ -o mergeHandWeight `root-config --cflags --libs --glibs` mergeHandWeight.cpp 


// ------------------------------------------------------------------------


int main (int argc, char ** argv) 
{  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1111111);
  gStyle->SetOptFit(1111);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.08);
  gStyle->SetTitleY(0.97);
  gStyle->SetPalette(1,0);

  TChain fChain ("elminitree") ;
<<<<<<< mergeHandWeight.cpp
  fChain.Add ("/misc/cms/users/mucib/FakeRateFase2NewProduction/*.root"); 
  //fChain.Add ("/misc/cms/users/mucib/FakeRateFase2AllEvents/*.root"); 
  //fChain.Add ("rfio:/castor/cern.ch/user/m/mucib/python/NewProductionRootuples/*.root");
=======
  //fChain.Add ("/misc/cms/users/mucib/FakeRateFase2AllEvents/*.root"); 
  fChain.Add ("rfio:/castor/cern.ch/user/m/mucib/python/newProductionRootuples/*.root");
>>>>>>> 1.4
  dati Input (&fChain) ;  
  std::cout << "numero di eventi linkati "<< fChain.GetEntries () << std::endl;

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


/* PG FIXME
- prepara gli histo di normalizzazione, cioe' un histos
- riempire loop-ando sui singoli getti
- ripesare per i pthat
*/

  //LM histograms per pTHat
  std::map<int,histos*> istogrammi ;
  std::map<int,histos*> denomin ;
  std::map<int,rates*> rate ;
  std::map<int,histos*> cumulate ; //dell'histos uso solo gli istogr per le cumulative 
  //LM integrated histograms
  histosIntegral * istogrammiOverAll = new histosIntegral(1) ;
  histosIntegral * denominOverAll = new histosIntegral(2);
  ratesIntegral * rateOverAll = new ratesIntegral();
  histosIntegral * cumulateOverAll = new histosIntegral(3);

  int eventCounter = 0;

  for (int index = 0 ; index < fChain.GetEntries () ; ++index )
    {
    fChain.GetEntry (index) ;
    //for production problems consider only ptHat<600
    //verify in new dataset
    if(Input.csa07Info.ptHat <= 600)
    {
    //LM consider only INTERISTING events
    if ( (Input.csa07Info.procId==11)||(Input.csa07Info.procId==12)||(Input.csa07Info.procId==13)||(Input.csa07Info.procId==28)||(Input.csa07Info.procId==53)||(Input.csa07Info.procId==68) )
      {
      if (index==2285) std::cout<<"sono arrivato al 2285"<<std::endl;
      if (index==2286) std::cout<<"sono arrivato al 2286"<<std::endl;
      if (index==2287) std::cout<<"sono arrivato al 2287"<<std::endl;
      eventCounter++;
      //prendo i primi piu' importanti
      int pthatevent = 0 ;
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

      //std::cout<<"pthat event "<<pthatevent<<std::endl;
   
      //se non ho ancora creato l'oggetto, crealo
      if (!denomin.count (pthatevent))
        {
           histos* dummy = new histos(pthatevent,2) ;
           denomin[pthatevent] = dummy ;
        }
      //adottiamo una normalizzazione al numero totale di eventi a dato pthat, 
      //non ad eventi d'interesse (e cioe che soddisfano i cut cinematici di selezione dei jet o elettroni):
      //ma tantofa',e' solo una normalizzazione, basta mettersi daccordo sia su num che su den
      denomin[pthatevent]->growDen(); //coi pesi alla CSA07Weight non serve questo,avendo tutto il dataset!!! Ma noi non ce l'abbiamo...

      //PG loop over jets per event
      //PG NB usati due istogrammi di un histos
      for (int i = 0 ; i < 30 ; ++i)
        {
           //PG FIXME manca il numero di getti, ora faccio cosi'
           //PG FIXME da controllare 
           if (Input.jetPT[i] == 0 &&  
               Input.jetEta[i] == 0 && 
               Input.jetPhi[i] == 0 && 
               Input.jetFlav[i] == 0 ) 
             {
               //std::cout << "number of jets: " << i << std::endl ;  
               break ;
             }
	   //identificazione del flavour  
           int fIndex = 3 ; // mettiamo in '3' cio' che non identifichiamo in questi processi
           if (Input.jetFlav[i] == 1 || Input.jetFlav[i] == 2 || Input.jetFlav[i] == 3 )
             fIndex = 0 ;
           if (Input.jetFlav[i] == 4 )
             fIndex = 1 ;
           if (Input.jetFlav[i] == 5 )
             fIndex = 2 ;
           if (Input.jetFlav[i] > 20 && Input.jetFlav[i] < 25)
             fIndex = 4 ;
	   
	   if ( (Input.jetPT[i]>20.) && (fabs(Input.jetEta[i])<2.5) && (fIndex!=3) ) //cut in pT && geometric acceptance && excluding etaBug 
	     {
             denomin[pthatevent]->m_e_sequence_resol_eta->Fill (Input.jetEta[i]) ;
             denomin[pthatevent]->m_e_sequence_resol_ptJ->Fill (Input.jetPT[i]) ;
             denomin[pthatevent]->m_e_sequence_resol_eta_flav[fIndex]->Fill (Input.jetEta[i]) ;
             denomin[pthatevent]->m_e_sequence_resol_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
	     }
        } //PG end loop over jets per event

     //std::cout<<"fuori dai jets"<<std::endl;
     if (!istogrammi.count (pthatevent))
        {
	   
           histos* dummy = new histos(pthatevent,1) ;
           istogrammi[pthatevent] = dummy ;
	   //LM automatically we know that we need new histograms for rate and cumulative distributions
	   rates* dummy2 = new rates (pthatevent) ;
	   rate[pthatevent] = dummy2 ;
	   histos* dummy3 = new histos (pthatevent,9) ;
	   cumulate[pthatevent] = dummy3 ;	   
        }
      istogrammi[pthatevent]->growNum () ;

      //PG loop over the electrons per event
      for (int i = 0 ; i < Input.eleNum ; ++i)
       {

          int fIndex = 3 ;
          if (Input.jetFlavour[i] == 1 || Input.jetFlavour[i] == 2 || Input.jetFlavour[i] == 3 )
            {fIndex = 0 ;}
          if (Input.jetFlavour[i] == 4 )
            {fIndex = 1 ;}
          if (Input.jetFlavour[i] == 5 )
            {fIndex = 2 ;}
          if (Input.jetFlavour[i] > 20 && Input.jetFlavour[i] < 25)
            {fIndex = 4 ;}
       
          if (Input.rawBit[i]) {; }
          int selected = 0 ;
//inserisco subito ecalIso per non uccidere troppo la statistica
/*          if (Input.ecalIsoBit[i])
            {
              //if (selected == 1) 
                {
		  std::cout<<"lo go becca!!!!"<<std::endl;
                  istogrammi[pthatevent]->m_e_sequence_hcalIso_eta->Fill (Input.jetEtaMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hcalIso_ptJ->Fill (Input.jetPTMatch[i]) ;
                  //questi due qui sotto ancora non esistono, ma verra' il loro tempo...
		  //istogrammi[pthatevent]->m_e_sequence_ecalIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                  //istogrammi[pthatevent]->m_e_sequence_ecalIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                }
            } else selected = 0 ;
*/
          //PG amb resolving passed (qui c'e' il taglio su jet pt)
	  //LM: occhio, stai escludendo i jet failed flavourID
          if (Input.ambiguityBit[i] && Input.jetmaxPT[i] > 30. && (fabs(Input.jetEtaMatch[i]<2.5)) ) //&& (fIndex!=3) )//&& (Input.eleClass[i] != 40)) 
            {
	    if ( (Input.pdgIdTruth[i]==11)||(Input.pdgIdTruth[i]==-11) ) {istogrammi[pthatevent]->m_DeltaRMatchRealElectronPreselection->Fill(Input.DelatRMatch[i]) ;}
	    else  {istogrammi[pthatevent]->m_DeltaRMatchMisElectronPreselection->Fill(Input.DelatRMatch[i]) ;}

              istogrammi[pthatevent]->m_e_sequence_resol_eta->Fill (Input.jetEtaMatch[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_ptJ->Fill (Input.jetPTMatch[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
              selected = 1 ;
            }
          else selected = 0 ;

          if (Input.jetPTMatch[i]>20.) //pT cut for observed objects
            {
              if (selected == 1) 
                {
                  istogrammi[pthatevent]->m_e_sequence_minimumPT_eta->Fill (Input.jetEtaMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_minimumPT_ptJ->Fill (Input.jetPTMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_minimumPT_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_minimumPT_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                }
            } else selected = 0 ;

          if (Input.tkIsoBit[i]) //track isolation
            {
              if (selected == 1) 
                {
                  istogrammi[pthatevent]->m_e_sequence_tkIso_eta->Fill (Input.jetEtaMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_ptJ->Fill (Input.jetPTMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                }
            } else selected = 0 ;
	    
//ecalIsolation	  
	if (Input.ecalIsoValue[i]<0.02)
	  {
	        if (index==2286) std::cout<<"ma passa ecalIsol??"<<std::endl;

	  if (selected == 1)
	    {
            istogrammi[pthatevent]->m_e_sequence_ecalIso_eta->Fill (Input.jetEtaMatch[i]) ;
            istogrammi[pthatevent]->m_e_sequence_ecalIso_ptJ->Fill (Input.jetPTMatch[i]) ;
           istogrammi[pthatevent]->m_e_sequence_ecalIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
           istogrammi[pthatevent]->m_e_sequence_ecalIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;	    
	    }
	  }  else selected = 0 ;

//hcalIsolation
          if(Input.eleIsBarrel[i])
	    {
            if (Input.hcalIsoValue[i]<0.1)
	      {
	      if (selected == 1)
	        {
                istogrammi[pthatevent]->m_e_sequence_hcalIso_eta->Fill (Input.jetEtaMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_hcalIso_ptJ->Fill (Input.jetPTMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_hcalIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_hcalIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;	    		
		}
  	      }  else selected = 0 ;
            }
	  else
	    {
            if (Input.hcalIsoValue[i]<0.075)
	      {
	      if (selected == 1)
	        {
                istogrammi[pthatevent]->m_e_sequence_hcalIso_eta->Fill (Input.jetEtaMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_hcalIso_ptJ->Fill (Input.jetPTMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_hcalIso_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_hcalIso_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;	    				
		}
  	      }  else selected = 0 ;
	    
	    }

// eID: Daskalakis
//dividere eID in EE e EB
          if(Input.eleIsBarrel[i])
	    {
	    //EB electron ID with kinematics variables (daskalakis)	    
            if ( (fabs(Input.eleDeltaPhi[i])<0.007)&&(fabs(Input.eleDeltaEta[i])<0.0030)&&(Input.eleHE[i]<0.026)&&(Input.eleSigmaEtaEta[i]<0.0092) ) 
              {
                if (selected == 1) 
                  {
                  istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_eta->Fill (Input.jetEtaMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_ptJ->Fill (Input.jetPTMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                  }
              } else selected = 0 ;
	    }
	  else  
	    {//EE daskalakis - covEtaEta + Robust
	    if ( (fabs(Input.eleDeltaPhi[i])<0.007)&&(fabs(Input.eleDeltaEta[i])<0.0040)&&(Input.eleHE[i]<0.018)&&(Input.eleIdTightBit[i]) )
	      {
              if (selected == 1) 
                {
                istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_eta->Fill (Input.jetEtaMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_ptJ->Fill (Input.jetPTMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
                istogrammi[pthatevent]->m_e_sequence_eleIdDaskalakis_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
                }
              } else selected = 0 ;
	    }

//seconda eID: tight
//obsoleta
/*
          if ( (Input.eleIdTightBit[i]) )
	    {
            if (selected == 1) 
              {
              istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta->Fill (Input.jetEtaMatch[i]) ;
              istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ->Fill (Input.jetPTMatch[i]) ;
              istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta_flav[fIndex]->Fill (Input.jetEtaMatch[i]) ;
              istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPTMatch[i]) ;
              }
            }
*/
       //facciamo statistica solo degli elettroni che hanno passato tutte le selezioni: quelli cioe' che davvero ci farebbero da bias nel segnale:
       if (selected == 1 )
         {
	 if ( (Input.pdgIdTruth[i]==11)||(Input.pdgIdTruth[i]==-11) ) {std::cout<<"real electron that passed all selection cuts"<<std::endl; istogrammi[pthatevent]->m_DeltaRMatchRealElectron->Fill(Input.DelatRMatch[i]) ;}
	 else  {istogrammi[pthatevent]->m_DeltaRMatchMisElectron->Fill(Input.DelatRMatch[i]) ;}
	 }
	 
       }  //PG end loop of the electrons per event

      }//end of interisting events
           //std::cout<<"arrivo alla fine degli interisting events??"<<std::endl;
    }//end of ptHat<600 events  
           //std::cout<<"arrivo dopo il controllo pthat min 600?? index "<<index<<std::endl;
    }//end of the loop over the events
    std::cout << "numero di eventi d'interesse "<< eventCounter << std::endl;
    
//qui devo inserire l'errore, prima di riscalare gli istogrammi
  for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
       istoIt != istogrammi.end () ;
       ++istoIt)
    {(istoIt->second)->errorizzamituttoNum() ;}
         
  for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
       istoIt != denomin.end () ;
       ++istoIt)
    {(istoIt->second)->errorizzamituttoDen() ;}
  
  //rescale histo per pt hat
  for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
       istoIt != istogrammi.end () ;
       ++istoIt)
    {(istoIt->second)->rescaleNum(rescale[istoIt->first]) ;}
         
  for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
       istoIt != denomin.end () ;
       ++istoIt)
    {(istoIt->second)->rescaleDen(rescale[istoIt->first]) ;}
   
  //filling degli istogrammi di rate divisi per pthat: obsoleto
/*  for (std::map<int,rates*>::iterator istoIt = rate.begin () ;
       istoIt != rate.end () ;
       ++istoIt)
    {
    //eta filling
    for (int bin = 1 ; bin <=50 ; bin++)
      {
      if (  denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin) != 0. )
        {
        (istoIt->second)->rate_resol_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_tkIso_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_tkIso_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdTight_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdTight_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdDaskalakis_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdDaskalakis_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_minimumPT_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_minimumPT_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_ecalIso_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_ecalIso_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_hcalIso_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_hcalIso_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        }
      else
        {
        (istoIt->second)->rate_resol_eta->SetBinContent(bin , 0.);
        (istoIt->second)->rate_tkIso_eta->SetBinContent( bin , 0.);
        (istoIt->second)->rate_minimumPT_eta->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdTight_eta->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdDaskalakis_eta->SetBinContent( bin , 0.);	
        (istoIt->second)->rate_ecalIso_eta->SetBinContent( bin ,0.);
        (istoIt->second)->rate_hcalIso_eta->SetBinContent( bin ,0.);
        }
      }
    //eta flavour filling
    for (int bin=1 ; bin<=50;bin++)
      {
      for (int f=0 ; f<5 ; f++)
	{
        if (  denomin[istoIt->first]->m_e_sequence_resol_eta_flav[f]->GetBinContent(bin) != 0. )
	  {
          (istoIt->second)->rate_resol_eta_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_resol_eta_flav[f]->GetBinContent(bin) /  denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
          (istoIt->second)->rate_tkIso_eta_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_tkIso_eta_flav[f]->GetBinContent(bin) /  denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
          (istoIt->second)->rate_eleIdTight_eta_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdTight_eta_flav[f]->GetBinContent(bin) /  denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
          (istoIt->second)->rate_eleIdDaskalakis_eta_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdDaskalakis_eta_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
          (istoIt->second)->rate_minimumPT_eta_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_minimumPT_eta_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
          (istoIt->second)->rate_ecalIso_eta_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_ecalIso_eta_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
          (istoIt->second)->rate_hcalIso_eta_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_hcalIso_eta_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
          }
        else
          {
          (istoIt->second)->rate_resol_eta_flav[f]->SetBinContent(bin , 0.);
          (istoIt->second)->rate_tkIso_eta_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_minimumPT_eta_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_eleIdTight_eta_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_eleIdDaskalakis_eta_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_ecalIso_eta_flav[f]->SetBinContent( bin ,0.);
          (istoIt->second)->rate_hcalIso_eta_flav[f]->SetBinContent( bin ,0.);
          }	  
	}//end eta flavour filling
      }
      
    //ptJ filling  
    for (int bin = 1 ; bin<=40 ; bin++)
      {      
      if ( denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin) != 0 )
        {
        (istoIt->second)->rate_resol_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_tkIso_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_tkIso_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdTight_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdTight_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdDaskalakis_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdDaskalakis_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_minimumPT_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_minimumPT_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_ecalIso_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_ecalIso_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_hcalIso_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_hcalIso_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        }
      else
        {
        (istoIt->second)->rate_resol_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_tkIso_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_minimumPT_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdTight_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdDaskalakis_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_ecalIso_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_hcalIso_ptJ->SetBinContent( bin , 0.);
        }
      //Ptransverse flavour filling
      for (int f=0 ; f<5 ; f++)
	{
        if (  denomin[istoIt->first]->m_e_sequence_resol_ptJ_flav[f]->GetBinContent(bin) != 0. )
	  {
          (istoIt->second)->rate_resol_ptJ_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_resol_ptJ_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
          (istoIt->second)->rate_tkIso_ptJ_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_tkIso_ptJ_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
          (istoIt->second)->rate_eleIdTight_ptJ_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdTight_ptJ_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
          (istoIt->second)->rate_eleIdDaskalakis_ptJ_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdDaskalakis_ptJ_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
          (istoIt->second)->rate_minimumPT_ptJ_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_minimumPT_ptJ_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
          (istoIt->second)->rate_ecalIso_ptJ_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_ecalIso_ptJ_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
          (istoIt->second)->rate_hcalIso_ptJ_flav[f]->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_hcalIso_ptJ_flav[f]->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
          }
        else
          {
          (istoIt->second)->rate_resol_ptJ_flav[f]->SetBinContent(bin , 0.);
          (istoIt->second)->rate_tkIso_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_minimumPT_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_eleIdTight_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_eleIdDaskalakis_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_ecalIso_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_hcalIso_ptJ_flav[f]->SetBinContent( bin , 0.);
          }	  
	}//end ptJ flavour filling
      }//end filling rate

    }
*/
    
    //cumulative distributions per each pTHat: sugli elettroni che si salvano da eID
    float appoNum = 0.;
    float appoDen = 0.;
    for (std::map<int,histos*>::iterator istoIt = cumulate.begin () ;
         istoIt != cumulate.end () ;
         ++istoIt)
      {
      for(int bin=1;bin<=40;bin++)
        {
        appoNum = 0.;
        appoDen = 0.;
      	for(int extremOfIntegration=bin;extremOfIntegration<=40;extremOfIntegration++)
	  {
          appoNum=appoNum + istogrammi[istoIt->first]->m_e_sequence_eleIdDaskalakis_ptJ -> GetBinContent(extremOfIntegration);
          appoDen=appoDen + denomin[istoIt->first]-> m_e_sequence_resol_ptJ   -> GetBinContent(extremOfIntegration);	  
	  }
        if (appoDen!= 0.) (istoIt->second)->cumulativeAll->SetBinContent(bin,appoNum/appoDen);
        else (istoIt->second)->cumulativeAll->SetBinContent(bin,0.); 
	}
      }
//distribuzioni integrate sui ptHat:
//qui devo settare l'errore istogramma per istogramma bin per bin, sommandolo in quadratura
  //DeltaRMatch
  for (int i=1 ; i<50 ; i++)
  {
    double appoFillMis  = 0. ;
    double appoFillReal = 0. ;
    double appoFillMisPre  = 0. ;
    double appoFillRealPre = 0. ;
    double appoFillMisError  = 0. ;
    double appoFillRealError = 0. ;
    double appoFillMisPreError  = 0. ;
    double appoFillRealPreError = 0. ;
    for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
         istoIt != istogrammi.end () ;
         ++istoIt)
      {
      appoFillMis  = appoFillMis  + (istoIt->second)->m_DeltaRMatchMisElectron->GetBinContent(i);
      appoFillReal = appoFillReal + (istoIt->second)->m_DeltaRMatchRealElectron->GetBinContent(i);
      appoFillMisPre  = appoFillMisPre  + (istoIt->second)->m_DeltaRMatchMisElectronPreselection->GetBinContent(i);
      appoFillRealPre = appoFillRealPre + (istoIt->second)->m_DeltaRMatchRealElectronPreselection->GetBinContent(i);

      appoFillMisError  = appoFillMisError  + pow( (istoIt->second)->m_DeltaRMatchMisElectron->GetBinError(i),2 );
      appoFillRealError = appoFillRealError + pow( (istoIt->second)->m_DeltaRMatchRealElectron->GetBinError(i),2 );
      appoFillMisPreError  = appoFillMisPreError  + pow( (istoIt->second)->m_DeltaRMatchMisElectronPreselection->GetBinError(i),2 );
      appoFillRealPreError = appoFillRealPreError + pow( (istoIt->second)->m_DeltaRMatchRealElectronPreselection->GetBinError(i),2 );
      }
    istogrammiOverAll->m_DeltaRMatchMisElectron ->SetBinContent(i,appoFillMis );
    istogrammiOverAll->m_DeltaRMatchRealElectron->SetBinContent(i,appoFillReal);  
    istogrammiOverAll->m_DeltaRMatchMisElectronPreselection ->SetBinContent(i,appoFillMisPre );
    istogrammiOverAll->m_DeltaRMatchRealElectronPreselection->SetBinContent(i,appoFillRealPre);  

    istogrammiOverAll->m_DeltaRMatchMisElectron ->SetBinError(i,sqrt(appoFillMisError) );
    istogrammiOverAll->m_DeltaRMatchRealElectron->SetBinError(i,sqrt(appoFillRealError) );  
    istogrammiOverAll->m_DeltaRMatchMisElectronPreselection ->SetBinError(i,sqrt(appoFillMisPreError ) );
    istogrammiOverAll->m_DeltaRMatchRealElectronPreselection->SetBinError(i,sqrt(appoFillRealPreError) );  
  }
  //pT distributions
  float appoFillResol , appoFillMinimumPT , appoFilleIdTight , appoFillTrackIso , appoFillEcalIso, appoFillHcalIso, appoFilleIdDaskalakis;
  float appoFillResolError, appoFillMinimumPTError, appoFilleIdTightError, appoFillTrackIsoError, appoFillEcalIsoError, appoFillHcalIsoError, appoFilleIdDaskalakisError;
  //numeratore
  for (int i = 1 ; i<=40 ; i++)
    {
    appoFillResol = 0.;         appoFillResolError = 0.;
    appoFillMinimumPT = 0.;     appoFillMinimumPTError = 0.;
    appoFilleIdTight = 0.;      appoFilleIdTightError = 0.;
    appoFilleIdDaskalakis = 0.; appoFilleIdDaskalakisError = 0.;
    appoFillTrackIso = 0.;      appoFillTrackIsoError = 0.;
    appoFillEcalIso = 0.;       appoFillEcalIsoError = 0.;
    appoFillHcalIso = 0.;       appoFillHcalIsoError = 0.;  
    //ciclo sui ptHat
    for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
         istoIt != istogrammi.end () ;
         ++istoIt)
      {
      appoFillResol = appoFillResol + (istoIt->second)->m_e_sequence_resol_ptJ->GetBinContent(i);
      appoFillMinimumPT = appoFillMinimumPT + (istoIt->second)->m_e_sequence_minimumPT_ptJ->GetBinContent(i);
      appoFilleIdTight = appoFilleIdTight + (istoIt->second)->m_e_sequence_eleIdTight_ptJ->GetBinContent(i);
      appoFilleIdDaskalakis = appoFilleIdDaskalakis + (istoIt->second)->m_e_sequence_eleIdDaskalakis_ptJ->GetBinContent(i);    
      appoFillTrackIso = appoFillTrackIso + (istoIt->second)->m_e_sequence_tkIso_ptJ->GetBinContent(i);
      appoFillEcalIso = appoFillEcalIso + (istoIt->second)->m_e_sequence_ecalIso_ptJ->GetBinContent(i);
      appoFillHcalIso = appoFillHcalIso + (istoIt->second)->m_e_sequence_hcalIso_ptJ->GetBinContent(i);

      appoFillResolError = appoFillResolError + pow(  (istoIt->second)->m_e_sequence_resol_ptJ->GetBinContent(i),2 );
      appoFillMinimumPTError = appoFillMinimumPTError + pow(  (istoIt->second)->m_e_sequence_minimumPT_ptJ->GetBinContent(i),2 );
      appoFilleIdTightError = appoFilleIdTightError + pow(  (istoIt->second)->m_e_sequence_eleIdTight_ptJ->GetBinContent(i),2 );
      appoFilleIdDaskalakisError = appoFilleIdDaskalakisError + pow(  (istoIt->second)->m_e_sequence_eleIdDaskalakis_ptJ->GetBinContent(i),2 );    
      appoFillTrackIsoError = appoFillTrackIsoError + pow(  (istoIt->second)->m_e_sequence_tkIso_ptJ->GetBinContent(i),2 );
      appoFillEcalIsoError = appoFillEcalIsoError + pow(  (istoIt->second)->m_e_sequence_ecalIso_ptJ->GetBinContent(i),2 );
      appoFillHcalIsoError = appoFillHcalIsoError + pow(  (istoIt->second)->m_e_sequence_hcalIso_ptJ->GetBinContent(i),2 );
      }
    istogrammiOverAll->m_e_sequence_resol_ptJ->SetBinContent(i,appoFillResol) ; 
    istogrammiOverAll->m_e_sequence_minimumPT_ptJ->SetBinContent(i,appoFillMinimumPT) ;      
    istogrammiOverAll->m_e_sequence_tkIso_ptJ->SetBinContent(i,appoFillTrackIso) ;      
    istogrammiOverAll->m_e_sequence_eleIdTight_ptJ->SetBinContent(i,appoFilleIdTight) ;        
    istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ->SetBinContent(i,appoFilleIdDaskalakis) ;                   
    istogrammiOverAll->m_e_sequence_ecalIso_ptJ->SetBinContent(i,appoFillEcalIso) ;                   
    istogrammiOverAll->m_e_sequence_hcalIso_ptJ->SetBinContent(i,appoFillHcalIso) ;                   

    istogrammiOverAll->m_e_sequence_resol_ptJ->SetBinError(i,sqrt(appoFillResolError) ) ; 
    istogrammiOverAll->m_e_sequence_minimumPT_ptJ->SetBinError(i,sqrt(appoFillMinimumPTError) ) ;      
    istogrammiOverAll->m_e_sequence_tkIso_ptJ->SetBinError(i,sqrt(appoFillTrackIsoError) ) ;      
    istogrammiOverAll->m_e_sequence_eleIdTight_ptJ->SetBinError(i,sqrt(appoFilleIdTightError) ) ;        
    istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ->SetBinError(i,sqrt(appoFilleIdDaskalakisError) ) ;                   
    istogrammiOverAll->m_e_sequence_ecalIso_ptJ->SetBinError(i,sqrt(appoFillEcalIsoError) ) ;                   
    istogrammiOverAll->m_e_sequence_hcalIso_ptJ->SetBinError(i,sqrt(appoFillHcalIsoError) ) ;                   
   }
  //denominatore    
  for (int i = 1 ; i<=40 ; i++)
    {
    appoFillResol = 0.; appoFillResolError = 0.;
    //ciclo sui ptHat
    for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
         istoIt != denomin.end () ;
         ++istoIt)
      {
      appoFillResol = appoFillResol + (istoIt->second)->m_e_sequence_resol_ptJ->GetBinContent(i);
      appoFillResolError = appoFillResolError + pow( (istoIt->second)->m_e_sequence_resol_ptJ->GetBinError(i),2 );
      }
    denominOverAll->m_e_sequence_resol_ptJ->SetBinContent(i,appoFillResol) ;                   
    denominOverAll->m_e_sequence_resol_ptJ->SetBinError(i,sqrt(appoFillResolError) );                   
    }
    
  //numeratore/denominatore: rate sulle distribuzioni integrate
  //da qui in poi devo settare gli errori isto per isto bin per bin, da propagazione:
  //r=a/b -> Dr=sqrt((Da/b)^2+(a*Db/b^2)^2)
  for (int i = 1 ; i<=40 ; i++)
    {
    if (denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i) != 0. )
      {
      rateOverAll->rate_resol_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)); 
      rateOverAll->rate_minimumPT_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_minimumPT_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;      
      rateOverAll->rate_tkIso_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;      
      rateOverAll->rate_eleIdTight_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_eleIdTight_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;
      rateOverAll->rate_eleIdDaskalakis_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;
      rateOverAll->rate_ecalIso_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_ecalIso_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;
      rateOverAll->rate_hcalIso_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_hcalIso_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;

      rateOverAll->rate_resol_ptJ->SetBinError( i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 
      rateOverAll->rate_minimumPT_ptJ->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_minimumPT_ptJ->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_minimumPT_ptJ->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 
      rateOverAll->rate_tkIso_ptJ->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_tkIso_ptJ->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_tkIso_ptJ->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 
      rateOverAll->rate_eleIdTight_ptJ->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_eleIdTight_ptJ->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_eleIdTight_ptJ->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 
      rateOverAll->rate_eleIdDaskalakis_ptJ->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 
      rateOverAll->rate_ecalIso_ptJ->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_ecalIso_ptJ->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_ecalIso_ptJ->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 
      rateOverAll->rate_hcalIso_ptJ->SetBinError(i,sqrt( 
        pow(istogrammiOverAll->m_e_sequence_hcalIso_ptJ->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_hcalIso_ptJ->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 
      }
    else
      {
      rateOverAll->rate_resol_ptJ->SetBinContent(i,0.) ; 
      rateOverAll->rate_minimumPT_ptJ->SetBinContent(i,0.) ;      
      rateOverAll->rate_tkIso_ptJ->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdTight_ptJ->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdDaskalakis_ptJ->SetBinContent(i,0.) ;      
      rateOverAll->rate_ecalIso_ptJ->SetBinContent(i,0.);
      rateOverAll->rate_hcalIso_ptJ->SetBinContent(i,0.);
      }  
    }
    
  //integrazione per il flavour:
  //considero la probabilita' che il fake sia dato da un certo flavour= num_flavour/denomin_totale: queste sommate (in stack) danno il rate totale
  //(altra cosa sarebbe num_flavour/denomin_flavour, cioe la prob che un jet di un certo flavour dia fake)
  float appoFillFlavour=0.;
  float appoFillFlavourError=0.;
  //numeratore Flavour: considero solo il taglio di daskalakis, anzi la trackisolation che se no uccido la statistica
  //ciclo sui flavour
  for (int flav=0;flav<5;flav++)
    {
    //ciclo sui bin
    for (int i = 1 ; i<=40 ; i++)
      {
      appoFillFlavour = 0.;
      appoFillFlavourError = 0.;
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
           istoIt != istogrammi.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i);
        appoFillFlavourError = appoFillFlavourError + (istoIt->second)->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinError(i);
        }
      istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
      istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->SetBinError(i,sqrt(appoFillFlavourError) ) ; 
      }
    }
  //denominatore Flavour
  //ciclo sui flavour
  for (int flav=0;flav<5;flav++)
    {
    //ciclo sui bin
    for (int i = 1 ; i<=40 ; i++)
      {
      appoFillFlavour = 0.;
      appoFillFlavourError = 0.;
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
           istoIt != denomin.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i);
        appoFillFlavourError = appoFillFlavourError + (istoIt->second)->m_e_sequence_resol_ptJ_flav[flav]->GetBinError(i);
        }
      denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
      denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->SetBinError(i,sqrt(appoFillFlavour) ) ; 
      }
    }
  //integral flavour rate
  for(int flav=0;flav<5;flav++)
    {
    for (int i = 1 ; i<=40 ; i++)
      {
      if (denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i) != 0. )
        {
	rateOverAll->rate_tkIso_ptJ_flav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i));
        rateOverAll->rate_tkIso_ptJ_flav[flav]->SetBinError(i,sqrt(
          pow(istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2) +     
          pow( istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i),2),2))); 

	}
      else
        {rateOverAll->rate_tkIso_ptJ_flav[flav]->SetBinContent(i,0.);} 
      if (denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i) != 0.)     
        {
	rateOverAll->rate_afterEverything_ptJ_flavOnflav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i));
        rateOverAll->rate_tkIso_ptJ_flav[flav]->SetBinError(i,sqrt(
          pow(istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinError(i)/denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i),2) +     
          pow( istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i)*denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i),2),2))); 

	}
      else
        {rateOverAll->rate_afterEverything_ptJ_flavOnflav[flav]->SetBinContent(i,0.);}		 
      }  
    }  
  //end dell'analisi integrata per ptJ 

  //eta distributions
  //numeratori
  for (int i = 1 ; i<=50 ; i++)
    {
    appoFillResol = 0.;
    appoFillMinimumPT = 0.;
    appoFilleIdTight = 0.;
    appoFilleIdDaskalakis = 0.;
    appoFillTrackIso = 0.;
    appoFillEcalIso = 0.;
    appoFillHcalIso = 0.;

    appoFillResolError = 0.;
    appoFillMinimumPTError = 0.;
    appoFilleIdTightError = 0.;
    appoFilleIdDaskalakisError = 0.;
    appoFillTrackIsoError = 0.;
    appoFillEcalIsoError = 0.;
    appoFillHcalIsoError = 0.;
      //ciclo sui ptHat
    for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
         istoIt != istogrammi.end () ;
         ++istoIt)
      {
      appoFillResol = appoFillResol + (istoIt->second)->m_e_sequence_resol_eta->GetBinContent(i);
      appoFillMinimumPT = appoFillMinimumPT + (istoIt->second)->m_e_sequence_minimumPT_eta->GetBinContent(i);
      appoFilleIdTight = appoFilleIdTight + (istoIt->second)->m_e_sequence_eleIdTight_eta->GetBinContent(i);
      appoFilleIdDaskalakis = appoFilleIdDaskalakis + (istoIt->second)->m_e_sequence_eleIdDaskalakis_eta->GetBinContent(i);
      appoFillTrackIso = appoFillTrackIso + (istoIt->second)->m_e_sequence_tkIso_eta->GetBinContent(i);
      appoFillEcalIso = appoFillEcalIso + (istoIt->second)->m_e_sequence_ecalIso_eta->GetBinContent(i);
      appoFillHcalIso = appoFillHcalIso + (istoIt->second)->m_e_sequence_hcalIso_eta->GetBinContent(i);

      appoFillResolError = appoFillResolError + pow(  (istoIt->second)->m_e_sequence_resol_eta->GetBinContent(i),2 ) ;
      appoFillMinimumPTError = appoFillMinimumPTError + pow(  (istoIt->second)->m_e_sequence_minimumPT_eta->GetBinContent(i),2 ) ;
      appoFilleIdTightError = appoFilleIdTightError + pow(  (istoIt->second)->m_e_sequence_eleIdTight_eta->GetBinContent(i),2 ) ;
      appoFilleIdDaskalakisError = appoFilleIdDaskalakisError + pow(  (istoIt->second)->m_e_sequence_eleIdDaskalakis_eta->GetBinContent(i),2 ) ;
      appoFillTrackIsoError = appoFillTrackIsoError + pow(  (istoIt->second)->m_e_sequence_tkIso_eta->GetBinContent(i),2 ) ;
      appoFillEcalIsoError = appoFillEcalIsoError + pow(  (istoIt->second)->m_e_sequence_ecalIso_eta->GetBinContent(i),2 ) ;
      appoFillHcalIsoError = appoFillHcalIsoError + pow(  (istoIt->second)->m_e_sequence_hcalIso_eta->GetBinContent(i),2 ) ;
      }
    istogrammiOverAll->m_e_sequence_resol_eta->SetBinContent(i,appoFillResol) ; 
    istogrammiOverAll->m_e_sequence_minimumPT_eta->SetBinContent(i,appoFillMinimumPT) ;      
    istogrammiOverAll->m_e_sequence_tkIso_eta->SetBinContent(i,appoFillTrackIso) ;      
    istogrammiOverAll->m_e_sequence_eleIdTight_eta->SetBinContent(i,appoFilleIdTight) ;                   
    istogrammiOverAll->m_e_sequence_eleIdDaskalakis_eta->SetBinContent(i,appoFilleIdDaskalakis) ;                   
    istogrammiOverAll->m_e_sequence_ecalIso_eta->SetBinContent(i,appoFillEcalIso) ;                   
    istogrammiOverAll->m_e_sequence_hcalIso_eta->SetBinContent(i,appoFillHcalIso) ;                   

    istogrammiOverAll->m_e_sequence_resol_eta->SetBinError(i,sqrt(appoFillResolError) ) ; 
    istogrammiOverAll->m_e_sequence_minimumPT_eta->SetBinError(i,sqrt(appoFillMinimumPTError) ) ;      
    istogrammiOverAll->m_e_sequence_tkIso_eta->SetBinError(i,sqrt(appoFillTrackIsoError) ) ;      
    istogrammiOverAll->m_e_sequence_eleIdTight_eta->SetBinError(i,sqrt(appoFilleIdTightError) ) ;                   
    istogrammiOverAll->m_e_sequence_eleIdDaskalakis_eta->SetBinError(i,sqrt(appoFilleIdDaskalakisError) ) ;                   
    istogrammiOverAll->m_e_sequence_ecalIso_eta->SetBinError(i,sqrt(appoFillEcalIsoError) ) ;                   
    istogrammiOverAll->m_e_sequence_hcalIso_eta->SetBinError(i,sqrt(appoFillHcalIsoError) ) ;                   
    }
  //denominatore  
  for (int i = 1 ; i<=50 ; i++)
    {
    appoFillResol = 0.;
    appoFillResolError = 0.;
    //ciclo sui ptHat
    for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
         istoIt != denomin.end () ;
         ++istoIt)
      {
      appoFillResol = appoFillResol + (istoIt->second)->m_e_sequence_resol_eta->GetBinContent(i);
      appoFillResolError = appoFillResolError + pow( (istoIt->second)->m_e_sequence_resol_eta->GetBinError(i),2 ) ;
      }
    denominOverAll->m_e_sequence_resol_eta->SetBinContent(i,appoFillResol) ;                   
    denominOverAll->m_e_sequence_resol_eta->SetBinError(i,sqrt(appoFillResolError) ) ;                   
    }
  //rate sulle distribuzioni integrate
  for (int i = 1 ; i<=50 ; i++)
    {
    if (denominOverAll->m_e_sequence_resol_eta->GetBinContent(i) != 0. )
      {
      rateOverAll->rate_resol_eta->SetBinContent(i,istogrammiOverAll->m_e_sequence_resol_eta->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i)); 
      rateOverAll->rate_minimumPT_eta->SetBinContent(i,istogrammiOverAll->m_e_sequence_minimumPT_eta->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i)) ;      
      rateOverAll->rate_tkIso_eta->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_eta->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i)) ;      
      rateOverAll->rate_eleIdTight_eta->SetBinContent(i,istogrammiOverAll->m_e_sequence_eleIdTight_eta->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i)) ;
      rateOverAll->rate_eleIdDaskalakis_eta->SetBinContent(i,istogrammiOverAll->m_e_sequence_eleIdDaskalakis_eta->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i)) ;
      rateOverAll->rate_ecalIso_eta->SetBinContent(i,istogrammiOverAll->m_e_sequence_ecalIso_eta->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i)) ;
      rateOverAll->rate_hcalIso_eta->SetBinContent(i,istogrammiOverAll->m_e_sequence_hcalIso_eta->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i)) ;

      rateOverAll->rate_resol_eta->SetBinError( i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_resol_eta->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_resol_eta->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
      rateOverAll->rate_minimumPT_eta->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_minimumPT_eta->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_minimumPT_eta->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
      rateOverAll->rate_tkIso_eta->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_tkIso_eta->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_tkIso_eta->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
      rateOverAll->rate_eleIdTight_eta->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_eleIdTight_eta->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_eleIdTight_eta->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
      rateOverAll->rate_eleIdDaskalakis_eta->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_eleIdDaskalakis_eta->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_eleIdDaskalakis_eta->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
      rateOverAll->rate_ecalIso_eta->SetBinError(i,sqrt(
        pow(istogrammiOverAll->m_e_sequence_ecalIso_eta->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_ecalIso_eta->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
      rateOverAll->rate_hcalIso_eta->SetBinError(i,sqrt( 
        pow(istogrammiOverAll->m_e_sequence_hcalIso_eta->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
        pow( istogrammiOverAll->m_e_sequence_hcalIso_eta->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
      }
    else
      {
      rateOverAll->rate_resol_eta->SetBinContent(i,0.) ; 
      rateOverAll->rate_minimumPT_eta->SetBinContent(i,0.) ;      
      rateOverAll->rate_tkIso_eta->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdTight_eta->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdDaskalakis_eta->SetBinContent(i,0.) ;      
      rateOverAll->rate_ecalIso_eta->SetBinContent(i,0.);
      rateOverAll->rate_hcalIso_eta->SetBinContent(i,0.);
      } 
    }
  //eta flavour breakout
  //numeratore Flavour: considero solo il taglio di daskalakis,anzi no quello trackiso se no uccido la statistica
  //ciclo sui flavour
  for (int flav=0;flav<5;flav++)
    {
    //ciclo sui bin
    for (int i = 1 ; i<=50 ; i++)
      {
      appoFillFlavour = 0.;
      appoFillFlavourError = 0.;
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
           istoIt != istogrammi.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i);
        appoFillFlavourError = appoFillFlavourError + pow( (istoIt->second)->m_e_sequence_tkIso_eta_flav[flav]->GetBinError(i),2 );
        }
      istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
      istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->SetBinError(i,sqrt(appoFillFlavour) ) ; 
      }
    }
  //denominatore Flavour
  //ciclo sui flavour
  for (int flav=0;flav<5;flav++)
    {
    //ciclo sui bin
    for (int i = 1 ; i<=50 ; i++)
      {
      appoFillFlavour = 0.;
      appoFillFlavourError = 0.;
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
           istoIt != denomin.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i);
        appoFillFlavourError = appoFillFlavourError + pow( (istoIt->second)->m_e_sequence_resol_eta_flav[flav]->GetBinError(i),2 ) ;
        }
      denominOverAll->m_e_sequence_resol_eta_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
      denominOverAll->m_e_sequence_resol_eta_flav[flav]->SetBinError(i,sqrt(appoFillFlavour) ) ; 
      }
    }
  //integral flavour rate
  for(int flav=0;flav<5;flav++)
    {
    for (int i = 1 ; i<=50 ; i++)
      {
      if (denominOverAll->m_e_sequence_resol_eta->GetBinContent(i) != 0. )
        {
	rateOverAll->rate_tkIso_eta_flav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i));
        rateOverAll->rate_tkIso_eta_flav[flav]->SetBinError(i,sqrt(
          pow(istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2) +     
          pow(istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta->GetBinContent(i),2),2))); 
	}
      else
        {rateOverAll->rate_tkIso_eta_flav[flav]->SetBinContent(i,0.);} 
      if (denominOverAll->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i) != 0.)     
        {
	rateOverAll->rate_afterEverything_eta_flavOnflav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i));
        rateOverAll->rate_tkIso_eta_flav[flav]->SetBinError(i,sqrt(
          pow(istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinError(i)/denominOverAll->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i),2) +     
          pow(istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i)*denominOverAll->m_e_sequence_resol_eta_flav[flav]->GetBinError(i)/pow(denominOverAll->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i),2),2))); 
	}
      else
        {rateOverAll->rate_afterEverything_eta_flavOnflav[flav]->SetBinContent(i,0.);}		 
      }  
    }      
  //end dell'analisi integrata per eta

  //cumulative sui pthat integrati
  for(int bin=1;bin<=40;bin++)
    {
    appoNum = 0.;
    appoDen = 0.;
    for(int extremOfIntegration=bin;extremOfIntegration<=40;extremOfIntegration++)
      {
      appoNum=appoNum + istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ -> GetBinContent(extremOfIntegration);
      appoDen=appoDen + denominOverAll-> m_e_sequence_resol_ptJ   -> GetBinContent(extremOfIntegration);	  
      }
    if (appoDen!= 0.) cumulateOverAll->cumulativeAll->SetBinContent(bin,appoNum/appoDen);
    else cumulateOverAll->cumulativeAll->SetBinContent(bin,0.); 
    }
    //end cumulative sui pthat integrati
//end distribuzioni integrate sui ptHat

//plots: rate divisi per pTHat [obsoleto], rate integrati, cumulative divise per pTHat, cumulative integrate, deltaRMatch
  TFile plotting("plotsNewRootuples.root","RECREATE");


/*  for (std::map<int,rates*>::iterator istoIt = rate.begin () ;
       istoIt != rate.end () ;
       ++istoIt)
    {
      (istoIt->second)->rate_resol_eta->Write() ;
      (istoIt->second)->rate_resol_ptJ->Write() ;
      (istoIt->second)->rate_tkIso_eta->Write() ;
      (istoIt->second)->rate_tkIso_ptJ->Write() ;
      (istoIt->second)->rate_minimumPT_eta->Write() ;
      (istoIt->second)->rate_minimumPT_ptJ->Write() ;
      (istoIt->second)->rate_eleIdTight_eta->Write() ;
      (istoIt->second)->rate_eleIdTight_ptJ->Write() ;
      (istoIt->second)->rate_eleIdDaskalakis_eta->Write() ;
      (istoIt->second)->rate_eleIdDaskalakis_ptJ->Write() ;      
      for (int f=0 ; f<5 ; f++)
        {
       //(istoIt->second)->rate_resol_eta_flav[f]->Write() ;
        (istoIt->second)->rate_resol_ptJ_flav[f]->Write() ;
        //(istoIt->second)->rate_tkIso_eta_flav[f]->Write() ;
        (istoIt->second)->rate_tkIso_ptJ_flav[f]->Write() ;
        //(istoIt->second)->rate_minimumPT_eta_flav[f]->Write() ;
        (istoIt->second)->rate_minimumPT_ptJ_flav[f]->Write() ;
        //(istoIt->second)->rate_eleIdTight_eta_flav[f]->Write() ;
        (istoIt->second)->rate_eleIdTight_ptJ_flav[f]->Write() ;
        //(istoIt->second)->rate_eleIdDaskalakis_eta_flav[f]->Write() ;
        (istoIt->second)->rate_eleIdDaskalakis_ptJ_flav[f]->Write() ;	      
        }
    }*/
/*  for (std::map<int,histos*>::iterator istoIt = cumulate.begin () ;
         istoIt != cumulate.end () ;
         ++istoIt)
    {
    (istoIt->second)->cumulativeAll->Write();
    }
*/

  rateOverAll->rate_resol_ptJ->Write() ; 
  rateOverAll->rate_minimumPT_ptJ->Write() ;      
  rateOverAll->rate_tkIso_ptJ->Write() ;      
  rateOverAll->rate_eleIdTight_ptJ->Write() ;
  rateOverAll->rate_eleIdDaskalakis_ptJ->Write() ;
  rateOverAll->rate_ecalIso_ptJ->Write();    
  rateOverAll->rate_hcalIso_ptJ->Write();    
  rateOverAll->rate_resol_eta->Write() ; 
  rateOverAll->rate_minimumPT_eta->Write() ;      
  rateOverAll->rate_tkIso_eta->Write() ;      
  rateOverAll->rate_eleIdTight_eta->Write() ;         
  rateOverAll->rate_eleIdDaskalakis_eta->Write() ;         
  rateOverAll->rate_ecalIso_eta->Write();
  rateOverAll->rate_hcalIso_eta->Write();
  cumulateOverAll->cumulativeAll->Write() ;   
  istogrammiOverAll->m_DeltaRMatchMisElectron->Write();
  istogrammiOverAll->m_DeltaRMatchRealElectron->Write();
  istogrammiOverAll->m_DeltaRMatchMisElectronPreselection->Write();
  istogrammiOverAll->m_DeltaRMatchRealElectronPreselection->Write();
  for (int f=0 ; f<5 ; f++)
    {
    rateOverAll->rate_tkIso_ptJ_flav[f]->Write() ;      
    rateOverAll->rate_tkIso_eta_flav[f]->Write() ;      
    rateOverAll->rate_afterEverything_ptJ_flavOnflav[f]->Write() ;
    rateOverAll->rate_afterEverything_eta_flavOnflav[f]->Write() ;
    }
<<<<<<< mergeHandWeight.cpp
     
=======
    
  //salvo gli istogrammi di ecalIso divisi per pthat:
  for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
       istoIt != istogrammi.end () ;
       ++istoIt)
    {
      (istoIt->second)->m_e_sequence_hcalIso_eta->Write() ;
      (istoIt->second)->m_e_sequence_hcalIso_ptJ->Write() ;
    } 
>>>>>>> 1.4
  plotting.Close();

} // int main

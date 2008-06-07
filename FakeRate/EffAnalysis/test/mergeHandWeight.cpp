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
  fChain.Add ("/misc/cms/users/mucib/FakeRateFase2AllEvents/*.root"); 
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
  std::map<int,histos*> cumulate ;
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
    if(Input.csa07Info.ptHat <= 600)
    {
    //LM consider only INTERISTING events
    if ( (Input.csa07Info.procId==11)||(Input.csa07Info.procId==12)||(Input.csa07Info.procId==13)||(Input.csa07Info.procId==28)||(Input.csa07Info.procId==53)||(Input.csa07Info.procId==68) )
      {
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
          //PG amb resolving passed (qui c'e' il taglio su jet pt)
          if (Input.ambiguityBit[i] && Input.jetmaxPT[i] > 30. && (fabs(Input.jetEtaMatch[i]<2.5)) && (fIndex!=3) )//&& (Input.eleClass[i] != 40)) 
            {
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
//prima eID: Daskalakis
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
              }
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
              }
	    }
//seconda eID: tight
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

       }  //PG end loop of the electrons per event
      
      }//end of interisting events
    }//end of ptHat<600 events  
    }//end of the loop over the events
    std::cout << "numero di eventi d'interesse "<< eventCounter << std::endl;
    
  
  //rescale histo per pt hat: il flavour devo rinormalizzarlo a parte,perche il numeratore vuole il branchin
  for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
       istoIt != istogrammi.end () ;
       ++istoIt)
    {(istoIt->second)->rescaleNum(rescale[istoIt->first]) ;}
         
  for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
       istoIt != denomin.end () ;
       ++istoIt)
    {(istoIt->second)->rescaleDen(rescale[istoIt->first]) ;}
   
  //filling degli istogrammi di rate
  for (std::map<int,rates*>::iterator istoIt = rate.begin () ;
       istoIt != rate.end () ;
       ++istoIt)
    {
    for (int bin = 1 ; bin <=50 ; bin++)
      {
      if (  denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin) != 0. )
        {
        (istoIt->second)->rate_resol_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_tkIso_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_tkIso_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdTight_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdTight_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdDaskalakis_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdDaskalakis_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        (istoIt->second)->rate_minimumPT_eta->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_minimumPT_eta->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_eta->GetBinContent(bin)) ;
        }
      else
        {
        (istoIt->second)->rate_resol_eta->SetBinContent(bin , 0.);
        (istoIt->second)->rate_tkIso_eta->SetBinContent( bin , 0.);
        (istoIt->second)->rate_minimumPT_eta->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdTight_eta->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdDaskalakis_eta->SetBinContent( bin , 0.);	
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
            }
          else
            {
            (istoIt->second)->rate_resol_eta_flav[f]->SetBinContent(bin , 0.);
            (istoIt->second)->rate_tkIso_eta_flav[f]->SetBinContent( bin , 0.);
            (istoIt->second)->rate_minimumPT_eta_flav[f]->SetBinContent( bin , 0.);
            (istoIt->second)->rate_eleIdTight_eta_flav[f]->SetBinContent( bin , 0.);
            (istoIt->second)->rate_eleIdDaskalakis_eta_flav[f]->SetBinContent( bin , 0.);
            }	  
	  }//end eta flavour filling
	}
      
    //ptJ flavour filling  
    for (int bin = 1 ; bin<=40 ; bin++)
      {      
      if ( denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin) != 0 )
        {
        (istoIt->second)->rate_resol_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_tkIso_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_tkIso_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdTight_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdTight_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_eleIdDaskalakis_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_eleIdDaskalakis_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        (istoIt->second)->rate_minimumPT_ptJ->SetBinContent( bin , istogrammi[istoIt->first]->m_e_sequence_minimumPT_ptJ->GetBinContent(bin) / denomin[istoIt->first]->m_e_sequence_resol_ptJ->GetBinContent(bin)) ;
        }
      else
        {
        (istoIt->second)->rate_resol_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_tkIso_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_minimumPT_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdTight_ptJ->SetBinContent( bin , 0.);
        (istoIt->second)->rate_eleIdDaskalakis_ptJ->SetBinContent( bin , 0.);
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
          }
        else
          {
          (istoIt->second)->rate_resol_ptJ_flav[f]->SetBinContent(bin , 0.);
          (istoIt->second)->rate_tkIso_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_minimumPT_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_eleIdTight_ptJ_flav[f]->SetBinContent( bin , 0.);
          (istoIt->second)->rate_eleIdDaskalakis_ptJ_flav[f]->SetBinContent( bin , 0.);
          }	  
	}//end ptJ flavour filling
      }//end filling rate

    }
    
    //cumulative distributions per each pTHat: immagino di doverlo fare sulla Daskalakis (cut piu' efficiente)
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
  //pT distributions
  float appoFillResol , appoFillMinimumPT , appoFilleIdTight , appoFillTrackIso , appoFilleIdDaskalakis;
  //numeratore
  for (int i = 1 ; i<=40 ; i++)
    {
    appoFillResol = 0.;
    appoFillMinimumPT = 0.;
    appoFilleIdTight = 0.;
    appoFilleIdDaskalakis = 0.;
    appoFillTrackIso = 0.;    
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
      }
    istogrammiOverAll->m_e_sequence_resol_ptJ->SetBinContent(i,appoFillResol) ; 
    istogrammiOverAll->m_e_sequence_minimumPT_ptJ->SetBinContent(i,appoFillMinimumPT) ;      
    istogrammiOverAll->m_e_sequence_tkIso_ptJ->SetBinContent(i,appoFillTrackIso) ;      
    istogrammiOverAll->m_e_sequence_eleIdTight_ptJ->SetBinContent(i,appoFilleIdTight) ;        
    istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ->SetBinContent(i,appoFilleIdDaskalakis) ;                   
   }
  //denominatore    
  for (int i = 1 ; i<=40 ; i++)
    {
    appoFillResol = 0.;
    //ciclo sui ptHat
    for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
         istoIt != denomin.end () ;
         ++istoIt)
      {
      appoFillResol = appoFillResol + (istoIt->second)->m_e_sequence_resol_ptJ->GetBinContent(i);
      }
    denominOverAll->m_e_sequence_resol_ptJ->SetBinContent(i,appoFillResol) ;                   
    }
  //numeratore/denominatore: rate sulle distribuzioni integrate
  for (int i = 1 ; i<=40 ; i++)
    {
    if (denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i) != 0. )
      {
      rateOverAll->rate_resol_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)); 
      rateOverAll->rate_minimumPT_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_minimumPT_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;      
      rateOverAll->rate_tkIso_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;      
      rateOverAll->rate_eleIdTight_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_eleIdTight_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;
      rateOverAll->rate_eleIdDaskalakis_ptJ->SetBinContent(i,istogrammiOverAll->m_e_sequence_eleIdDaskalakis_ptJ->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i)) ;
      }
    else
      {
      rateOverAll->rate_resol_ptJ->SetBinContent(i,0.) ; 
      rateOverAll->rate_minimumPT_ptJ->SetBinContent(i,0.) ;      
      rateOverAll->rate_tkIso_ptJ->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdTight_ptJ->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdDaskalakis_ptJ->SetBinContent(i,0.) ;      
      }  
    }
    
  //integrazione per il flavour:
  //considero la probabilita' che il fake sia dato da un certo flavour= num_flavour/denomin_totale: queste sommate (in stack) danno il rate totale
  //(altra cosa sarebbe num_flavour/denomin_flavour, cioe la prob che un jet di un certo flavour dia fake)
  float appoFillFlavour=0.;
  //numeratore Flavour: considero solo il taglio di daskalakis
  //ciclo sui flavour
  for (int flav=0;flav<5;flav++)
    {
    //ciclo sui bin
    for (int i = 1 ; i<=40 ; i++)
      {
      appoFillFlavour = 0.;
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
           istoIt != istogrammi.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i);
        }
      istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
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
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
           istoIt != denomin.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i);
        }
      denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
      }
    }
  //integral flavour rate
  for(int flav=0;flav<5;flav++)
    {
    for (int i = 1 ; i<=40 ; i++)
      {
      if (denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i) != 0. )
        {rateOverAll->rate_tkIso_ptJ_flav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ->GetBinContent(i));}
      else
        {rateOverAll->rate_tkIso_ptJ_flav[flav]->SetBinContent(i,0.);} 
      if (denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i) != 0.)     
        {rateOverAll->rate_afterEverything_ptJ_flavOnflav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_ptJ_flav[flav]->GetBinContent(i));}
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
      }
    istogrammiOverAll->m_e_sequence_resol_eta->SetBinContent(i,appoFillResol) ; 
    istogrammiOverAll->m_e_sequence_minimumPT_eta->SetBinContent(i,appoFillMinimumPT) ;      
    istogrammiOverAll->m_e_sequence_tkIso_eta->SetBinContent(i,appoFillTrackIso) ;      
    istogrammiOverAll->m_e_sequence_eleIdTight_eta->SetBinContent(i,appoFilleIdTight) ;                   
    istogrammiOverAll->m_e_sequence_eleIdDaskalakis_eta->SetBinContent(i,appoFilleIdDaskalakis) ;                   
    }
  //denominatore  
  for (int i = 1 ; i<=50 ; i++)
    {
    appoFillResol = 0.;
    //ciclo sui ptHat
    for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
         istoIt != denomin.end () ;
         ++istoIt)
      {
      appoFillResol = appoFillResol + (istoIt->second)->m_e_sequence_resol_eta->GetBinContent(i);
      }
    denominOverAll->m_e_sequence_resol_eta->SetBinContent(i,appoFillResol) ;                   
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
      }
    else
      {
      rateOverAll->rate_resol_eta->SetBinContent(i,0.) ; 
      rateOverAll->rate_minimumPT_eta->SetBinContent(i,0.) ;      
      rateOverAll->rate_tkIso_eta->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdTight_eta->SetBinContent(i,0.) ;      
      rateOverAll->rate_eleIdDaskalakis_eta->SetBinContent(i,0.) ;      
      }  
    }
  //eta flavour breakout
  //numeratore Flavour: considero solo il taglio di daskalakis
  //ciclo sui flavour
  for (int flav=0;flav<5;flav++)
    {
    //ciclo sui bin
    for (int i = 1 ; i<=50 ; i++)
      {
      appoFillFlavour = 0.;
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = istogrammi.begin () ;
           istoIt != istogrammi.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i);
        }
      istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
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
      //ciclo sui ptHat
      for (std::map<int,histos*>::iterator istoIt = denomin.begin () ;
           istoIt != denomin.end () ;
           ++istoIt)
        { 
        appoFillFlavour = appoFillFlavour + (istoIt->second)->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i);
        }
      denominOverAll->m_e_sequence_resol_eta_flav[flav]->SetBinContent(i,appoFillFlavour) ; 
      }
    }
  //integral flavour rate
  for(int flav=0;flav<5;flav++)
    {
    for (int i = 1 ; i<=50 ; i++)
      {
      if (denominOverAll->m_e_sequence_resol_eta->GetBinContent(i) != 0. )
        {rateOverAll->rate_tkIso_eta_flav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta->GetBinContent(i));}
      else
        {rateOverAll->rate_tkIso_eta_flav[flav]->SetBinContent(i,0.);} 
      if (denominOverAll->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i) != 0.)     
        {rateOverAll->rate_afterEverything_eta_flavOnflav[flav]->SetBinContent(i,istogrammiOverAll->m_e_sequence_tkIso_eta_flav[flav]->GetBinContent(i)/denominOverAll->m_e_sequence_resol_eta_flav[flav]->GetBinContent(i));}
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

//plots: rate divisi per pTHat, rate integrati, cumulative divise per pTHat, cumulative integrate
  TFile plotting("plotsEtaBug.root","RECREATE");

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
  rateOverAll->rate_resol_eta->Write() ; 
  rateOverAll->rate_minimumPT_eta->Write() ;      
  rateOverAll->rate_tkIso_eta->Write() ;      
  rateOverAll->rate_eleIdTight_eta->Write() ;         
  rateOverAll->rate_eleIdDaskalakis_eta->Write() ;         
  cumulateOverAll->cumulativeAll->Write() ;   

  for (int f=0 ; f<5 ; f++)
    {
//    istogrammiOverAll->m_e_sequence_tkIso_ptJ_flav[f]->Write() ;
//    denominOverAll->m_e_sequence_resol_ptJ_flav[f]->Write() ;
    rateOverAll->rate_tkIso_ptJ_flav[f]->Write() ;      
    rateOverAll->rate_tkIso_eta_flav[f]->Write() ;      
    rateOverAll->rate_afterEverything_ptJ_flavOnflav[f]->Write() ;
    rateOverAll->rate_afterEverything_eta_flavOnflav[f]->Write() ;
    }
  plotting.Close();

} // int main
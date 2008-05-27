/*
Programma di analisi della cinematica dei jet degli eventi:
salviamo, divisi per flavour (compresi i failed), gli istogrammi delle distribuzioni in eta, pT e phi.
salviamo anche le distribuzioni percentuali, per vedere se (nelle tre distrubuzioni) cambia l'abbondanza relativa dei diversi flavour.
Nota che prima di tutto bisogna lavoare a divisione in pthat, quindi riscalare e infine poter sommare le grandezze

i plot sono svolti dalla macro macroPlotkinematic.cpp
*/
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
#include "TString.h"
#include "TLegend.h"

#include "datiSimple.h"
#include "histoKinematic.h"

//COMPILO c++ -o kinematicAnalysis `root-config --cflags --libs --glibs` kinematicAnalysis.cpp 


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

  //mappa dei pesi
  std::map<int,double> weight; 
  weight[15] = 53.0E+9 / (13.75E+06 * 53.0 / 55.0 + 0.75E+06);   //number = cross section in 1 pb-1 div by #events (MB HS+ QCD bin)
  weight[20] =  1.46E+9 / (13.75E+06 * 1.46 / 55.0 + 1.3E+06);  
  weight[30] =  0.63E+9 / (13.75E+06 * 0.63 / 55.0 + 2.5E+06);  
  weight[50] =  0.163E+9 / (13.75E+06 * 0.163 / 55.0 + 2.5E+06);  
  weight[80] =  21.6E+06 / (13.75E+06 * 0.0216 / 55.0 + 2.5E+06);     
  weight[120] =  3.08E+06 / (13.75E+06 * 0.00308 / 55.0 + 1.18E+06);  
  weight[170] =  0.494E+06 / (1.25E+06);                         //number = cross section in 1 pb-1 div by #events (QCD bin). MB negligible
  weight[230] =  0.101E+06 / (1.16E+06);     
  weight[300] =  24.5E+03 / (1.20E+06);      
  weight[380] =  6.24E+03 / (1.18E+06);  
  weight[470] =  1.78E+03 / (1.19E+06);    
  weight[600] =  0.683E+03 / (1.23E+06);  
  weight[800] =  0.204E+03 / (0.5E+06);
  weight[1000] =  35.1E+00 / (0.1E+06);   
  weight[1400] =  10.9E+00 / (3.0E+04);  
  weight[1800] =  1.6E+00 / (3.0E+04); 
  weight[2200] =  0.145E+00 / (2.0E+04); 

  //flavour breakout di eta,phi,pT (a partire anche dalla divisione pre-rescaling in pthat)
  std::map<int,histos*> Jet;
  //pthat breakout, come integrazione sui flavour dei precedenti istogrammi, e relative percentuali
  TString name;
  std::map<int,TH1D*>   pTPthatBreakout;
  std::map<int,TH1D*>   etaPthatBreakout;
  std::map<int,TH1D*>   phiPthatBreakout;
  std::map<int,TH1D*>   pTPthatBreakoutPercent;
  std::map<int,TH1D*>   etaPthatBreakoutPercent;
  std::map<int,TH1D*>   phiPthatBreakoutPercent;

  for (int index = 0 ; index < fChain.GetEntries () ; ++index )
    {
    fChain.GetEntry (index) ;
    //for weight problems consider only ptHat<600
    if(Input.csa07Info.ptHat <= 600) //finche' uso il datset Allevents col problema dei pesi devo mantenerlo
    {
    //LM consider only INTERISTING events
    if ( (Input.csa07Info.procId==11)||(Input.csa07Info.procId==12)||(Input.csa07Info.procId==13)||(Input.csa07Info.procId==28)||(Input.csa07Info.procId==53)||(Input.csa07Info.procId==68) )
      {
      //memorizzazione dell'informazione di pthat
      int pthatevent = 0 ;
      //prendo i primi piu' importanti
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

      //creazione degli istogrammi di mappa se necessario
      if(!Jet.count(pthatevent))
        {
	std::cout<<"nuovo pthat "<<pthatevent<<std::endl;
	histos* dummy = new histos(pthatevent,1) ; Jet[pthatevent] = dummy ;

	name.Form("pTDistribution_pthat_%i",pthatevent);
	TH1D* dummy2= new TH1D(name.Data(),name.Data(),40,0.,200.);	 
	pTPthatBreakout[pthatevent] = dummy2;
	name.Form("etaDistribution_pthat_%i",pthatevent);
	TH1D* dummy3 = new TH1D(name.Data(),name.Data(),50,-2.5,2.5);	
	etaPthatBreakout[pthatevent] = dummy3;
	name.Form("phiDistribution_pthat_%i",pthatevent);
	TH1D* dummy4 = new TH1D(name.Data(),name.Data(),50,-3.14,3.14);
	phiPthatBreakout[pthatevent] = dummy4;

	name.Form("pTDistributionPercent_pthat_%i",pthatevent);
	TH1D* dummy5= new TH1D(name.Data(),name.Data(),40,0.,200.);	 
	pTPthatBreakoutPercent[pthatevent] = dummy5;
	name.Form("etaDistributionPercent_pthat_%i",pthatevent);
	TH1D* dummy6 = new TH1D(name.Data(),name.Data(),50,-2.5,2.5);	
	etaPthatBreakoutPercent[pthatevent] = dummy6;
	name.Form("phiDistributionPercent_pthat_%i",pthatevent);
	TH1D* dummy7 = new TH1D(name.Data(),name.Data(),50,-3.14,3.14);
	phiPthatBreakoutPercent[pthatevent] = dummy7;
	}

       Jet[pthatevent]->grow();

      //PG loop over jets per event
      for (int i = 0 ; i < 50 ; ++i)
        {
        //controllo sull'effettiva esistenza di altri jet su cui ciclare           
	if (Input.jetPT[i] == 0 &&  
            Input.jetEta[i] == 0 && 
            Input.jetPhi[i] == 0 && 
            Input.jetFlavour[i] == 0 ) 
          {
          //std::cout << "number of jets: " << i << std::endl ;  
          break ;
          }	  
	//memorizzazione dell'info di flavour
	int fIndex = 3 ; //failed flavour ID
        if (Input.jetFlav[i] == 1 || Input.jetFlav[i] == 2 || Input.jetFlav[i] == 3 )
          fIndex = 0 ;
        if (Input.jetFlav[i] == 4 )
          fIndex = 1 ;
        if (Input.jetFlav[i] == 5 )
          fIndex = 2 ;
        if (Input.jetFlav[i] > 20 && Input.jetFlav[i] < 25)
          fIndex = 4 ;

	if ( (Input.jetPT[i]>20.) && (fabs(Input.jetEta[i])<2.5) )  //adottiamo gli stessi cut cinematici dell'analisi di rate
	  {//filling degli istogrammi
	  Jet[pthatevent]->eta_flav[fIndex]->Fill(Input.jetEta[i]);
	  Jet[pthatevent]->pT_flav[fIndex]->Fill(Input.jetPT[i]);
	  Jet[pthatevent]->phi_flav[fIndex]->Fill(Input.jetPhi[i]);
          }
        } //PG end loop over jets per event
      }//end of interisting events
    }//end of ptHat<600 events  
    }//end of the loop over the events    

  //rescale histo per pt hat
  std::cout<<"primo debug"<<std::endl;
  for (std::map<int,histos*>::iterator istoIt = Jet.begin () ;
       istoIt != Jet.end () ;
       ++istoIt)
    {
    (istoIt->second)->rescale(weight[istoIt->first]) ;
    }
  std::cout<<"secondo debug"<<std::endl;
    
  //sum bin per bin histograms over ptHat maintaining flavour breakout
      std::cout<<"terzo debug"<<std::endl;
  SumOnPthat *IntegratedDistr = new SumOnPthat();
      std::cout<<"quarto debug"<<std::endl;

  float appoEta, appoPT, appoPhi ;
  //ciclo prima sui flavour, devo mantenerne la separazione !!!
  for (int fIndex=0 ; fIndex<5 ; fIndex++)
    {
    for (int bin=1;bin<=40; bin++)
      {
      appoPT = 0. ;
      for (std::map<int,histos*>::iterator istoIt = Jet.begin () ;
           istoIt != Jet.end () ;
           ++istoIt)
        {
        appoPT = appoPT + (istoIt->second)->pT_flav[fIndex]->GetBinContent(bin) ;  
        }

       IntegratedDistr->pT_flav[fIndex]->SetBinContent(bin,appoPT);
      }     
     for (int bin=1;bin<=50; bin++)
      {
      appoEta = 0.;
      for (std::map<int,histos*>::iterator istoIt = Jet.begin () ;
           istoIt != Jet.end () ;
           ++istoIt)
        {
        appoEta = appoEta + (istoIt->second)->eta_flav[fIndex]->GetBinContent(bin) ;  
        }
        IntegratedDistr->eta_flav[fIndex]->SetBinContent(bin,appoEta);
      }   
     for (int bin=1;bin<=50; bin++)
      {
      appoPhi = 0.;
      for (std::map<int,histos*>::iterator istoIt = Jet.begin () ;
           istoIt != Jet.end () ;
           ++istoIt)
        {
        appoPhi = appoPhi + (istoIt->second)->phi_flav[fIndex]->GetBinContent(bin) ;  
        }
        IntegratedDistr->phi_flav[fIndex]->SetBinContent(bin,appoPhi);
      } 
    }
    
      std::cout<<"quinto debug"<<std::endl;

  //filling degli istogrammi di percentuale
  float appoDen;
  for (int bin=1;bin<=40;bin++)
    {
    appoDen = 0.;
    for (int fIndex=0;fIndex<5;fIndex++)
      {
      appoDen = appoDen + IntegratedDistr->pT_flav[fIndex]->GetBinContent(bin);
      }
    for (int fIndex=0;fIndex<5;fIndex++)
      {
      if (appoDen!=0) IntegratedDistr->pTPercent_flav[fIndex]->SetBinContent(bin,IntegratedDistr->pT_flav[fIndex]->GetBinContent(bin)/appoDen);
      else	      IntegratedDistr->pTPercent_flav[fIndex]->SetBinContent(bin,0.);
      }
    }
  for (int bin=1;bin<=50;bin++)
    {
    appoDen = 0.;
    for (int fIndex=0;fIndex<5;fIndex++)
      {
      appoDen = appoDen + IntegratedDistr->phi_flav[fIndex]->GetBinContent(bin);
      }
    for (int fIndex=0;fIndex<5;fIndex++)
      {
      if (appoDen!=0) IntegratedDistr->phiPercent_flav[fIndex]->SetBinContent(bin,IntegratedDistr->phi_flav[fIndex]->GetBinContent(bin)/appoDen);
      else	      IntegratedDistr->phiPercent_flav[fIndex]->SetBinContent(bin,0.);
      }
    }
  for (int bin=1;bin<=50;bin++)
    {
    appoDen = 0.;
    for (int fIndex=0;fIndex<5;fIndex++)
      {
      appoDen = appoDen + IntegratedDistr->eta_flav[fIndex]->GetBinContent(bin);
      }
    for (int fIndex=0;fIndex<5;fIndex++)
      {
      if (appoDen!=0) IntegratedDistr->etaPercent_flav[fIndex]->SetBinContent(bin,IntegratedDistr->eta_flav[fIndex]->GetBinContent(bin)/appoDen);
      else	      IntegratedDistr->etaPercent_flav[fIndex]->SetBinContent(bin,0.);
      }
    }    
   //end percentage filling  
      std::cout<<"sesto debug"<<std::endl;

  //integriamo (pthat per pthat) i flavour in modo da avere la mappa dei pthat-breakout
  float appoFill,appoFill2;
  for (std::map<int,histos*>::iterator istoIt = Jet.begin () ;
       istoIt != Jet.end () ;
       ++istoIt)
    {
    for (int bin=1; bin<=40 ; bin++)
      {
      appoFill = 0.;
      for (int fIndex=0; fIndex<5 ; fIndex++)
        {
        appoFill += (istoIt->second)->pT_flav[fIndex]->GetBinContent(bin) ;
        }
      pTPthatBreakout[(istoIt->first)]->SetBinContent(bin,appoFill) ;
      }
    }
  for (std::map<int,histos*>::iterator istoIt = Jet.begin () ;
       istoIt != Jet.end () ;
       ++istoIt)
    {
    for (int bin=1; bin<=50 ; bin++)
      {
      appoFill = 0.;
      appoFill2 = 0.;     
      for (int fIndex=0; fIndex<5 ; fIndex++)
        {
        appoFill += (istoIt->second)->eta_flav[fIndex]->GetBinContent(bin) ;
	std::cout<<"appoFill "<<appoFill<<std::endl;

        appoFill2 += (istoIt->second)->phi_flav[fIndex]->GetBinContent(bin) ;
	std::cout<<"appoFill2 "<<appoFill2<<std::endl;
        }
      etaPthatBreakout[(istoIt->first)]->SetBinContent(bin,appoFill) ;
      phiPthatBreakout[(istoIt->first)]->SetBinContent(bin,appoFill2) ;      
      }
    }
  //ora il breakout in pthat, in percentuale
  for (int bin=1 ; bin<=40 ; bin++)
    {
    appoFill = 0.;
    for (std::map<int,TH1D*>::iterator istoIt = pTPthatBreakout.begin () ;
         istoIt != pTPthatBreakout.end () ;
         ++istoIt)
      {
      appoFill += (istoIt->second)->GetBinContent(bin) ;
      }
    for (std::map<int,TH1D*>::iterator istoIt = pTPthatBreakoutPercent.begin () ;
         istoIt != pTPthatBreakoutPercent.end () ;
         ++istoIt)
      {
      if (appoFill!=0.) (istoIt->second)->SetBinContent(bin, pTPthatBreakout[(istoIt->first)]->GetBinContent(bin)/appoFill);
      else              (istoIt->second)->SetBinContent(bin, 0.);
      }      
    } 
  for (int bin=1 ; bin<=50 ; bin++)
    {
    appoFill = 0.;
    appoFill2 = 0.;    
    for (std::map<int,TH1D*>::iterator istoIt = etaPthatBreakout.begin () ;
         istoIt != etaPthatBreakout.end () ;
         ++istoIt)
      {
      appoFill += (istoIt->second)->GetBinContent(bin) ;
      appoFill2 += phiPthatBreakout[(istoIt->first)]->GetBinContent(bin) ;
      }
    for (std::map<int,TH1D*>::iterator istoIt = etaPthatBreakoutPercent.begin () ;
         istoIt != etaPthatBreakoutPercent.end () ;
         ++istoIt)
      {
      if (appoFill!=0.) (istoIt->second)->SetBinContent(bin, etaPthatBreakout[(istoIt->first)]->GetBinContent(bin)/appoFill);
      else              (istoIt->second)->SetBinContent(bin, 0.);
      if (appoFill2!=0.) phiPthatBreakoutPercent[(istoIt->first)]->SetBinContent(bin, phiPthatBreakout[(istoIt->first)]->GetBinContent(bin)/appoFill2);
      else               phiPthatBreakoutPercent[(istoIt->first)]->SetBinContent(bin, 0.);
      }      
    }              
  TFile fileToOpen("jetKinematic.root","RECREATE") ;
    std::cout<<"quarto debug"<<std::endl;

  for (std::map<int,TH1D*>::iterator istoIt = pTPthatBreakout.begin () ;
       istoIt != pTPthatBreakout.end () ;
       ++istoIt)
    {
    (istoIt->second)->Write() ;
    etaPthatBreakout[(istoIt->first)]->Write() ;
    phiPthatBreakout[(istoIt->first)]->Write() ;    
    }
  for (std::map<int,TH1D*>::iterator istoIt = pTPthatBreakoutPercent.begin () ;
       istoIt != pTPthatBreakoutPercent.end () ;
       ++istoIt)
    {
    (istoIt->second)->Write();
    etaPthatBreakoutPercent[(istoIt->first)]->Write();
    phiPthatBreakoutPercent[(istoIt->first)]->Write();    
    }     
  for (int fIndex=0;fIndex<5;fIndex++)
    {
    IntegratedDistr->eta_flav[fIndex]->Write();
    IntegratedDistr->etaPercent_flav[fIndex]->Write();
    IntegratedDistr->pT_flav[fIndex]->Write();
    IntegratedDistr->pTPercent_flav[fIndex]->Write();
    IntegratedDistr->phi_flav[fIndex]->Write();
    IntegratedDistr->phiPercent_flav[fIndex]->Write();
    }
  fileToOpen.Close();

    
  //plotting: proviamo a salvare automaticamente solo i .gif finali
  /*THStack *EtaStack = new THStack("EtaStack","stack of eta distribution flavour breakout") ;
  THStack *PTStack = new THStack("PTStack","stack of p_{T} distribution flavour breakout") ;
  THStack *PhiStack = new THStack("PhiStack","stack of phi distribution flavour breakout") ;

    for (int fIndex = 0 ; fIndex < 5 ; fIndex++)
      {
      EtaStack ->Add( IntegratedDistr->eta_flav[fIndex] ) ;
      PTStack  ->Add( IntegratedDistr->pT_flav[fIndex] ) ;
      PhiStack ->Add( IntegratedDistr->phi_flav[fIndex] ) ; 
      }
  TFile myFile("jetKinematic.root","RECREATE");
  EtaStack->Write(); PTStack->Write(); PhiStack->Write(); leg->Write();
  myFile.Close();*/
/*      
  TCanvas c1, c2, c3 ;
  c1.cd();
  c1.SetLogy () ; c2.SetLogy() ; c3.SetLogy();
  EtaStack->Draw () ;      
  c1.Print ("Eta_stack.gif","gif") ; 
  c2.cd();
  PTStack->Draw () ;      
  c2.Print ("PT_stack.gif","gif") ; 
  c3.cd();
  PhiStack->Draw () ;      
  c3.Print ("Phi_stack.gif","gif") ; 
*/
   
} // int main

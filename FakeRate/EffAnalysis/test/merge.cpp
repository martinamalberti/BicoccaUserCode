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

#include "dati.h"
#include "histo2.h"

//COMPILO c++ -o merge3 `root-config --cflags --libs --glibs` merge3.cpp 

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
  fChain.Add ("../dataFakeNew/pt20_30_*"); 
  fChain.Add ("../dataFakeNew/pt30_50_*"); 
  fChain.Add ("../dataFakeNew/pt50_80_*"); 
  fChain.Add ("../dataFakeNew/pt80_120_*"); 
  dati Input (&fChain) ;  

/* CHE COSA MANCA

- vedere se funziona
- gli histo per il jet max
- gli histo per gli elettroni prima delle selezioni
- l'indice per il flavour
- gli istogrammi somma (modo furbo di farli? dallo stack? Add-ando i vari ptHat?)

*/
  std::map<int,double> crossSections ;
  double totalCrossSection = 0.63+0.163+0.0216+0.00308+0.000494;
  crossSections[20] = 0.63/totalCrossSection  ;
  crossSections[30] = 0.163/totalCrossSection ;
  crossSections[50] = 0.0216/totalCrossSection ;
  crossSections[80] = 0.00308/totalCrossSection ;
  crossSections[120] = 0.000494/totalCrossSection ;

  std::map<int,histos2*> istogrammi ;
  //loop over the events
  for (int index = 0 ; index < fChain.GetEntries () ; ++index )
    {
      fChain.GetEntry (index) ;

      int pthatevent = 0 ;
      if (Input.ptHat > 19 && Input.ptHat < 30)
          pthatevent = 20 ;
      if (Input.ptHat > 29 && Input.ptHat < 50)
          pthatevent = 30 ;
      if (Input.ptHat > 49 && Input.ptHat < 80)
          pthatevent = 50 ;
      if (Input.ptHat > 79 && Input.ptHat < 120)
          pthatevent = 80 ;
      if (Input.ptHat > 119 && Input.ptHat < 170)
          pthatevent = 120 ;
      //pthatevent = 120 ;
      
      if (!istogrammi.count (pthatevent)){
        histos2* dummy = new histos2(pthatevent) ;
	istogrammi[pthatevent] = dummy ;
      }
      istogrammi[pthatevent]->grow () ;
      //PG loop of objects per event
      for (int i = 0 ; i < Input.eleNum ; ++i)
       {
	  int fIndex = 3 ;
          if (Input.jetFlavour[i] == 1 || Input.jetFlavour[i] == 2 || Input.jetFlavour[i] == 3 )
	      fIndex = 0 ;
          if (Input.jetFlavour[i] == 4 )
	      fIndex = 1 ;
          if (Input.jetFlavour[i] == 5 )
	      fIndex = 2 ;
          if (Input.jetFlavour[i] > 20 && Input.jetFlavour[i] < 25)
	      fIndex = 4 ;
	  
	  if (Input.rawBit[i]) {; }

          int selected = 0 ;
          //PG amb resolving passed
          if (Input.ambiguityBit[i] && Input.jetmaxPT[i] > 30 )//&& (Input.eleClass[i] != 40)) 
            {
	      istogrammi[pthatevent]->m_e_single_resol_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_resol_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_resol_ptJ->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_ptJ->Fill (Input.jetPT[i]) ;   
              istogrammi[pthatevent]->m_e_single_resol_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_single_resol_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_resol_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_resol_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;   
              istogrammi[pthatevent]->m_e_single_resol_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_sequence_resol_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              selected = 1 ;
            }
	  else selected = 0 ;
/*  
          if (Input.eleIdTightBit[i]) 
            {
              istogrammi[pthatevent]->m_e_single_eleIdTight_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptJ->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              if (selected == 1 ) 
                {        
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptE->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                }
              
	    } else selected = 0 ;
*/
          if (Input.tkIsoBit[i]) 
            {
              istogrammi[pthatevent]->m_e_single_tkIso_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_tkIso_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_tkIso_ptJ->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_tkIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_single_tkIso_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_tkIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_tkIso_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_tkIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              if (selected == 1) 
                {
                  istogrammi[pthatevent]->m_e_sequence_tkIso_eta->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_ptE->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_ptJ->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_tkIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                }
            } else selected = 0 ;

          if (Input.hadIsoBit[i]) 
            {
              istogrammi[pthatevent]->m_e_single_hadIso_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_hadIso_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_hadIso_ptJ->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_hadIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_single_hadIso_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_hadIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_hadIso_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_hadIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              if (selected == 1) 
                {
                  istogrammi[pthatevent]->m_e_sequence_hadIso_eta->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hadIso_ptE->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hadIso_ptJ->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hadIso_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hadIso_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hadIso_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hadIso_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_hadIso_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                }
            } else selected = 0 ;

          if (Input.eleIdBit[i]) 
            {
              istogrammi[pthatevent]->m_e_single_eleId_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleId_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleId_ptJ->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleId_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_single_eleId_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleId_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleId_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleId_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              if (selected == 1) 
                {
                  istogrammi[pthatevent]->m_e_sequence_eleId_eta->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleId_ptE->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleId_ptJ->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleId_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleId_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleId_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleId_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleId_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                }
            } 

          if (Input.eleIdLooseBit[i]) 
            {
              istogrammi[pthatevent]->m_e_single_eleIdLoose_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdLoose_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdLoose_ptJ->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdLoose_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdLoose_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdLoose_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdLoose_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdLoose_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              if (selected == 1) 
                {        
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_eta->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptE->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptJ->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdLoose_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                }
            } 

          if (Input.eleIdTightBit[i] && Input.jetmaxPT[i] > 30 )//&& (Input.eleClass[i] != 40)) 
            {
              istogrammi[pthatevent]->m_e_single_eleIdTight_eta->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptE->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptJ->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
              istogrammi[pthatevent]->m_e_single_eleIdTight_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
              if (selected == 1 )//&& (Input.eleClass[i] == 0 || Input.eleClass[i] == 100)) 
                {        
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptE->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_EMfrac->Fill (Input.EMjetCompon[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_eta_flav[fIndex]->Fill (Input.eleEta[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptE_flav[fIndex]->Fill (Input.elePT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_ptJ_flav[fIndex]->Fill (Input.jetPT[i]) ;
                  istogrammi[pthatevent]->m_e_sequence_eleIdTight_EMfrac_flav[fIndex]->Fill (Input.EMjetCompon[i]) ;
                }
            } 

       }  //PG loop of objects per event
    }//end of the loop over the events
  
  //rescale histo per pt hat
  for (std::map<int,histos2*>::iterator istoIt = istogrammi.begin () ;
       istoIt != istogrammi.end () ;
       ++istoIt)
    {
      (istoIt->second)->rescale(crossSections[istoIt->first]) ;
    }     
  
  //save histo per pt hat
  for (std::map<int,histos2*>::iterator istoIt = istogrammi.begin () ;
       istoIt != istogrammi.end () ;
       ++istoIt)
    {
      (istoIt->second)->write() ;
    }     
  
/*  
  //PG ----=-=-=-=-=-=-=-=----
  //PG     P L O T T I N G 
  //PG ----=-=-=-=-=-=-=-=----
  std::cout << "Debug create stacks" << std::endl ;
  std::map<std::string,THStack> stacks ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_resol_eta"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_resol_ptE"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_resol_ptJ"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_tkIso_eta"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_tkIso_ptE"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_tkIso_ptJ"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_hadIso_eta"         ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_hadIso_ptE"         ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_hadIso_ptJ"         ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleId_eta"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleId_ptE"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleId_ptJ"          ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdLoose_eta"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdLoose_ptE"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdLoose_ptJ"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdTight_eta"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdTight_ptE"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdTight_ptJ"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_resol_EMfrac"       ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_tkIso_EMfrac"       ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_hadIso_EMfrac"      ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleId_EMfrac"       ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdLoose_EMfrac"  ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_single_eleIdTight_EMfrac"  ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_resol_eta"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_resol_ptE"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_resol_ptJ"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_tkIso_eta"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_tkIso_ptE"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_tkIso_ptJ"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_hadIso_eta"       ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_hadIso_ptE"       ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_hadIso_ptJ"       ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleId_eta"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleId_ptE"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleId_ptJ"        ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdLoose_eta"   ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdLoose_ptE"   ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdLoose_ptJ"   ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdTight_eta"   ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdTight_ptE"   ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdTight_ptJ"   ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_resol_EMfrac"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_tkIso_EMfrac"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_hadIso_EMfrac"    ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleId_EMfrac"     ,THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdLoose_EMfrac",THStack ())) ;
  stacks.insert (std::map<std::string,THStack>::value_type ("m_e_sequence_eleIdTight_EMfrac",THStack ())) ;
  
  //PG loop sui pthat
  std::cout << "Debug loop over pt hat" << std::endl ;
  for (std::map<int,histos2*>::iterator istoIt = istogrammi.begin () ;
       istoIt != istogrammi.end () ;
       ++istoIt)
    {
      //PG loop sulle distribuzioni
      std::cout << "Debug inside loop over pt hat" << std::endl ;
      for (std::map<std::string,THStack>::iterator stacksIt = stacks.begin () ;
           stacksIt != stacks.end () ;
           ++stacksIt)
        {
          char pthatname[10] ;
          sprintf (pthatname,"%d_",istoIt->first) ;
          std::string name = pthatname + stacksIt->first ;  
          TH1F * dummy = (TH1F *) gDirectory->Get (name.c_str ()) ;
          //seg violation?!? Why?!?
	  //dummy->SetFillColor (50 + istoIt->first) ;
          (stacksIt->second).Add (dummy) ;
        } //PG loop sulle distribuzioni  
    } //PG loop sui pthat   

/*
  TCanvas c1 ;
  c1.SetLogy () ;
  //PG loop sulle distribuzioni
  for (std::map<std::string,THStack>::iterator stacksIt = stacks.begin () ;
       stacksIt != stacks.end () ;
       ++stacksIt)
    {
      (stacksIt->second).Draw () ;      
      c1.Print ((stacksIt->first + "_stack.gif").c_str (),"gif") ; 
      (stacksIt->second).Draw ("nostack") ;      
      //PG qui ci manca l'histo somma FIXME
      c1.Print ((stacksIt->first + "_nostack.gif").c_str (),"gif") ; 
    } //PG loop sulle distribuzioni  

  
//   TCanvas * c1 = new TCanvas ;
//   m_e_single_resol_ptE->Draw;
//   c1->Print ("Dummy.gif","gif") ; 
*/ 
 
  
  /*
  
  TCanvas * c1 = new TCanvas ;
  c1->SetLogy () ;
  for (std::vector<std::string>::iterator histosIt = histos.begin () ;
       histosIt != histos.end () ;
       ++histosIt)
    {
      //PG ptHat
      TH1F * global_histogr = (TH1F *) global.Get (histosIt->c_str ()) ;
      TH1F * pt20_30_histogr = (TH1F *) pt20_30.Get (histosIt->c_str ()) ;
      TH1F * pt30_50_histogr = (TH1F *) pt30_50.Get (histosIt->c_str ()) ;
      TH1F * pt50_80_histogr = (TH1F *) pt50_80.Get (histosIt->c_str ()) ;
      TH1F * pt80_120_histogr = (TH1F *) pt80_120.Get (histosIt->c_str ()) ;
      
      THStack pippo ((std::string (global_histogr->GetName ()) + std::string ("_stack")).c_str (), 
                     global_histogr->GetName ()) ;
      pippo.Add (pt20_30_histogr) ;
      pippo.Add (pt30_50_histogr) ;
      pippo.Add (pt50_80_histogr) ;
      pippo.Add (pt80_120_histogr) ;
      pippo.Add (global_histogr) ;
 
      global_histogr->SetFillColor (0) ;
      global_histogr->SetLineColor (1) ;
      global_histogr->SetLineWidth (2) ;
      pt20_30_histogr->SetFillColor(47) ;
      pt30_50_histogr->SetFillColor(48) ;
      pt50_80_histogr->SetFillColor(49) ;
      pt80_120_histogr->SetFillColor(50) ;
      pt20_30_histogr->SetFillStyle(3001) ;
      pt30_50_histogr->SetFillStyle(3001) ;
      pt50_80_histogr->SetFillStyle(3001) ;
      pt80_120_histogr->SetFillStyle(3001) ;
      //pt20_30_histogr->GetYaxis()->SetTitle("(jet #rightarrow fake e efficiency)(5 GeV/c)") ;
      //pt20_30_histogr->GetXaxis()->SetTitle("p_T (GeV/c)") ;
      TH2F bkgGlobal ((std::string (global_histogr->GetName ()) + std::string ("_bkg")).c_str (),"",
                      10,0.000001,0.1,
		      10,global_histogr->GetXaxis ()->GetXmin (),global_histogr->GetXaxis ()->GetXmax ()) ;
      bkgGlobal.SetStats (0) ;
      bkgGlobal.Draw () ;
      global_histogr->Draw ("same") ;
      c1->Print ((std::string (global_histogr->GetName ()) + std::string (".gif")).c_str (),"gif") ;
      TH2F bkgpippo ((std::string (global_histogr->GetName ()) + std::string ("_)bkg")).c_str (),"",
                      10,0.000001,0.1,
		      10,global_histogr->GetXaxis ()->GetXmin (),global_histogr->GetXaxis ()->GetXmax ()) ;
      bkgGlobal.SetStats (0) ;
      bkgGlobal.Draw () ;
      pippo.Draw () ;
      global_histogr   ->GetYaxis()->SetRangeUser(0.000001,0.1) ;
      global_histogr   ->GetYaxis()->SetTitle("(jet #rightarrow fake e efficiency)(5 GeV/c)") ;
      global_histogr   ->GetZaxis()->SetTitle("p_T (GeV/c)") ;
      pt20_30_histogr  ->GetYaxis()->SetRangeUser(0.000001,0.1) ;
      pt30_50_histogr  ->GetYaxis()->SetRangeUser(0.000001,0.1) ;
      pt50_80_histogr  ->GetYaxis()->SetRangeUser(0.000001,0.1) ;
      pt80_120_histogr ->GetYaxis()->SetRangeUser(0.000001,0.1) ;
      pippo.Draw () ;
      //c1->Print ((std::string (global_histogr->GetName ()) + std::string ("_stack.gif")).c_str (),"gif") ;
      pippo.Draw ("nostack") ;
      global_histogr->Draw("same");
      c1->Print ((std::string (global_histogr->GetName ()) + std::string ("_NOstack.gif")).c_str (),"gif") ;

      //PG flavours
      THStack pluto ((std::string (global_histogr->GetName ()) + std::string ("_flav_stack")).c_str (), 
                     global_histogr->GetName ()) ;
//      std::string names[5] = {"_light","_c","_b","_t","_g"} ;
      std::string names[4] = {"_g","_light","_c","_t"} ;
      TH1F * global_histogr_flav[4] ;
      for (int i=0 ; i<4 ; ++i)
        {
          global_histogr_flav[i] = (TH1F *) global.Get ((*histosIt + names[i]).c_str ()) ;        
          if (i == 2)
	    {
	       global_histogr_flav[i]->Add ((TH1F *) global.Get ((*histosIt + std::string("_b")).c_str ()),1) ;
	    }
	  global_histogr_flav[i]->SetFillStyle(2002) ;
          pluto.Add (global_histogr_flav[i]) ;
          pluto.Add (global_histogr) ;
	}
      global_histogr_flav[0]->SetFillColor (64) ;
      global_histogr_flav[1]->SetFillColor (65) ;
      global_histogr_flav[2]->SetFillColor (66) ;
      global_histogr_flav[3]->SetFillColor (28) ;

      pluto.Draw () ;
      for (int i=0 ; i<4 ; ++i)
	  global_histogr_flav[i]->GetYaxis()->SetRangeUser(0.000001,0.1) ;
      pluto.Draw () ;
      //c1->Print ((std::string (global_histogr->GetName ()) + std::string ("_flav_stack.gif")).c_str (),"gif") ;
      for (int i=0 ; i<4 ; ++i) global_histogr_flav[i]->SetFillStyle(3001) ;
      pluto.Draw ("nostack") ;
      c1->Print ((std::string (global_histogr->GetName ()) + std::string ("_flav_NOstack.gif")).c_str (),"gif") ; 
//      pluto.Draw ("nostack") ;
//      c1->Print ((std::string (global_histogr->GetName ()) + std::string ("_flav_NOstack_log.gif")).c_str (),"gif") ; 
    }   
*/

} // int main



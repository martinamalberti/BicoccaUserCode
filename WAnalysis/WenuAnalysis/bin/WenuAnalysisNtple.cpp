#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//---- CMSSW includes
#include "DataFormats/Math/interface/LorentzVector.h"
// #include "WAnalysis/WenuAnalysis/interface/WenuTreeContent.h"
#include "PhysicsTools/NtupleUtils/interface/treeReader.h"

//---- root includes
#include "TH1.h"
#include "TH2.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#define PI 3.14159265
#define TWOPI 6.28318530

using namespace std;

float deltaPhi(float phi1,float phi2) {
 float deltaphi = fabs(phi1-phi2);
 if (deltaphi > TWOPI) deltaphi -= TWOPI;
 if (deltaphi > PI ) deltaphi = TWOPI - deltaphi;
 return deltaphi;
}

//! main program
int main (int argc, char** argv)
{
 
 int ff = atoi(argv[1]);
  
 
 
 char nameOutput[1000];
 if (ff == -1){
  sprintf(nameOutput,"%s",argv[3]);
  std::cout << " output = " << nameOutput << std::endl;
 }
 
 TFile *fout;
 if (ff == 0){
  fout  = new TFile("Wenu_MC_4analysis.root","recreate");
 }

 if (ff == 1) {
  fout  = new TFile("Wenu_DATA_4analysis.root","recreate");
 }

 if (ff == -1) {
  fout  = new TFile(nameOutput,"recreate");
 }
 
  // load the tree
 
 TFile* FileIn = NULL; 
 if ( ff == 0 ){
  FileIn = new TFile("/tmp/amassiro/treeWenuMC.root");
 }
 
 TChain *chain = new TChain ("myanalysis/SimpleNtple") ;
  
 if ( ff == 0 ){
  chain->Add("/tmp/amassiro/treeWenuMC.root");
//   FileIn = new TFile("/tmp/amassiro/treeWenuMC.root");
 }

 if (ff == 1){
  //chain->Add("/tmp/amassiro/treeWenu_Commissioning10-SD_EG-v9.root");
//  chain->Add("/tmp/amassiro/treeWenu_EG-Run2010A-PromptReco-v1.root");
//   chain->Add("/tmp/amassiro/treeWenu_MinimumBias-Commissioning10-SD_EG-Jun14thSkim-v1.root");
//   chain->Add("/tmp/amassiro/treeWenu_EG-Run2010A-Jun14thReReco-v1.root");
//   chain->Add("/tmp/amassiro/treeWenu_EG-Run2010A-PromptReco-v4.root");
  chain->Add("/tmp/amassiro/treeWenu_EG-Run2010A-Jul16thReReco-v2_4analysis.root");
  chain->Add("/tmp/amassiro/treeWenu_EG-Run2010A-PromptReco-v4_4analysis.root");
 }

 char nameInput[1000];
 if (ff == -1){
//   sprintf(nameOutput,"%s",argv[3]);
//   std::cout << " output = " << nameOutput << std::endl;
  sprintf(nameInput,"%s",argv[2]);
  std::cout << " input  = " << nameInput << std::endl;
  FileIn = new TFile(nameInput);
  chain->Add(nameInput);
 }

 treeReader reader((TTree*)(chain));
 int nEntries = reader.GetEntries() ;

 double w = 1.;
  
 int initialNumber = -1;
 if (FileIn != NULL && FileIn->Get("AllPassFilterW/totalEvents")){
  TH1F* hTotalEvents = (TH1F*) FileIn->Get("AllPassFilterW/totalEvents");
  initialNumber = hTotalEvents->GetEntries();
 }
 
 std::cout << "FOUND " << nEntries << " ENTRIES : " << initialNumber << std::endl;

 fout->cd();
 
  //INITIALIZING HISTOGRAMS
 TH1F *hNGoodElectrons = new TH1F("hNGoodElectrons","hNGoodElectrons",5,0,5);
 
 TH1F *hSumEt = new TH1F("hSumEt","hSumEt",500,0,500);
 TH1F *hmet_o_SumEt = new TH1F("hmet_o_SumEt","hmet_o_SumEt",100,0,10);
 TH2F *hmet_vs_metOverSumEt = new TH2F("hmet_vs_metOverSumEt","hmet_vs_metOverSumEt",100,0,10,100,0,100);

 TH1F *het    = new TH1F("het","het",25,2,102);
 TH1F *hetEB  = new TH1F("hetEB","hetEB",25,2,102);
 TH1F *hetEE  = new TH1F("hetEE","hetEE",25,2,102);

 TH1F *hmet   = new TH1F("hmet","hmet",25,2,102);
 TH1F *hmetEB = new TH1F("hmetEB","hmetEB",25,2,102);
 TH1F *hmetEE = new TH1F("hmetEE","hmetEE",25,2,102);
 
 TH1F *hmt    = new TH1F("hmt","hmt",50,0,200);
 TH1F *hmtEB  = new TH1F("hmtEB","hmtEB",50,0,200);
 TH1F *hmtEE  = new TH1F("hmtEE","hmtEE",50,0,200);

 TH1F *heop   = new TH1F("heop","heop",50,0,2);
 TH1F *heopEB = new TH1F("heopEB","heopEB",50,0,2);
 TH1F *heopEE = new TH1F("heopEE","heopEE",50,0,2);

 TH1F *hetscaled[200];
 TH1F *hetEBscaled[200];
 TH1F *hetEEscaled[200];
 TH1F *hmtEBscaled[200];
 TH1F *hmtEEscaled[200];
 TH1F *heopEBscaled[200];
 TH1F *heopEEscaled[200];

 char hname[100];
 for (int i = 0; i < 200 ; i++){
  
  sprintf(hname,"hetscaled_%d",i);
  hetscaled[i] = new TH1F(hname,hname,25,2,102);

  sprintf(hname,"hetEBscaled_%d",i);
  hetEBscaled[i] = new TH1F(hname,hname,25,2,102);

  sprintf(hname,"hetEEscaled_%d",i);
  hetEEscaled[i] = new TH1F(hname,hname,25,2,102);

  sprintf(hname,"hmtEBscaled_%d",i);
  hmtEBscaled[i] = new TH1F(hname,hname,50,0,200);

  sprintf(hname,"hmtEEscaled_%d",i);
  hmtEEscaled[i] = new TH1F(hname,hname,50,0,200);

  sprintf(hname,"heopEBscaled_%d",i);
  heopEBscaled[i] = new TH1F(hname,hname,50,0,2);

  sprintf(hname,"heopEEscaled_%d",i);
  heopEEscaled[i] = new TH1F(hname,hname,50,0,2);
  
 }

//   float EtaCutEB    = 1.4442;
//   float EtaCutEE    = 1.560;
 float EtaCutEB    = 1.5;
 float EtaCutEE    = 1.5;
 float EtaMax      = 3.0;

 double eta;
 double pT;
 double ET;
 double MT;
 double EoP;
 double eleFBrem;
 double eleES;
 double E2x2;
 double E3x3;
 double E5x5;
 double ESC;
 double Sigma_Phi;
 double Sigma_Eta;
 double pIn;
 double pOut;
 double pAtCalo;
 double p;
 double E9oE25;

 int HLT_Ele15_LW_L1R;
 int HLT_Photon10_L1R;
 int HLT_Photon15_L1R;
 int HLT_Photon20_L1R;

 TTree* myTree = new TTree("myTree","myTree");
 myTree -> Branch("eta",&eta,"eta/D");
 myTree -> Branch("pT",&pT,"pT/D");
 myTree -> Branch("ET",&ET,"ET/D");
 myTree -> Branch("MT",&MT,"MT/D");
 myTree -> Branch("EoP",&EoP,"EoP/D");
 myTree -> Branch("eleFBrem",&eleFBrem,"eleFBrem/D");
 myTree -> Branch("eleES",&eleES,"eleES/D");
 myTree -> Branch("E2x2",&E2x2,"E2x2/D");
 myTree -> Branch("E3x3",&E3x3,"E3x3/D");
 myTree -> Branch("E5x5",&E5x5,"E5x5/D");
 myTree -> Branch("ESC",&ESC,"ESC/D");
 myTree -> Branch("Sigma_Phi",&Sigma_Phi,"Sigma_Phi/D");
 myTree -> Branch("Sigma_Eta",&Sigma_Eta,"Sigma_Eta/D");
 myTree -> Branch("pIn",&pIn,"pIn/D");
 myTree -> Branch("pOut",&pOut,"pOut/D");
 myTree -> Branch("pAtCalo",&pAtCalo,"pAtCalo/D");

 myTree -> Branch("p",&p,"p/D");
 myTree -> Branch("E9oE25",&E9oE25,"E9oE25/D");

 myTree -> Branch("initialNumber",&initialNumber,"initialNumber/I");
 myTree -> Branch("HLT_Ele15_LW_L1R",&HLT_Ele15_LW_L1R,"HLT_Ele15_LW_L1R/I");
 myTree -> Branch("HLT_Photon10_L1R",&HLT_Photon10_L1R,"HLT_Photon10_L1R/I");
 myTree -> Branch("HLT_Photon15_L1R",&HLT_Photon15_L1R,"HLT_Photon15_L1R/I");
 myTree -> Branch("HLT_Photon20_L1R",&HLT_Photon20_L1R,"HLT_Photon20_L1R/I");

 int nWele = 0;
 int nWeleSel = 0;


  // loop over entries
 for (int entry = 0; entry < nEntries; ++entry) { 
  reader.GetEntry(entry);

  if(entry%100000 == 0) std::cout << "event n. " << entry << std::endl;

  // HLT selection
  //if (treeVars.HLT_Photon15_L1R==0) continue; ///==== always 1 for DATA
  //if (treeVars.HLT_Ele15_LW_L1R==0) continue;
        
  int nGoodElectrons = 0;
  int chosenEle = 0;

    //start loop over electron candidates
  int nEles = reader.Get4V("electrons")->size();
  for (int iEle = 0; iEle < nEles; iEle++){
   float eta = reader.GetFloat("electrons_scEta")->at(iEle);
      // select electrons in ECAL fiducial volume
   if ( fabs(eta)> EtaCutEB && fabs(eta)< EtaCutEE ) continue;
   if ( fabs(eta)> EtaMax ) continue;
      
   float pt = reader.Get3V("electrons_p_atVtx")->at(iEle).Rho();
   p  = reader.Get3V("electrons_p_atVtx")->at(iEle).R();
   float et = reader.GetFloat("electrons_scEt")->at(iEle);
      
   float met  = reader.Get4V("CALOMet")->at(0).Et();

   float cphi = (reader.Get3V("electrons_p_atVtx")->at(iEle).x() * reader.Get4V("CALOMet")->at(0).Px() 
     + reader.Get3V("electrons_p_atVtx")->at(iEle).y() * reader.Get4V("CALOMet")->at(0).Py()) 
     / (met*pt);

   
   float mt   = sqrt(2*et*met*(1-cphi));

   float dphi = deltaPhi(reader.Get4V("CALOMet")->at(0).Phi(),reader.GetFloat("electrons_scPhi")->at(iEle));

   float sumEt = 0;
   for (unsigned int iJet=0; iJet<reader.Get4V("jets")->size(); iJet++){
    float deta = reader.Get4V("jets")->at(iJet).Eta() -  eta;
    float dphi = deltaPhi(reader.Get4V("jets")->at(iJet).Phi(),reader.GetFloat("electrons_scPhi")->at(iEle));
    float dR = sqrt(deta*deta+dphi*dphi);
    if ( dR < 0.4 && reader.Get4V("jets")->at(iJet).Pt()>15.) continue;
    sumEt += reader.Get4V("jets")->at(iJet).Pt();
   }

   hmet_vs_metOverSumEt ->Fill(met/sumEt,met);
   hSumEt->Fill(sumEt,w);
   hmet_o_SumEt->Fill(met/sumEt,w);

 //      if ( et < 18.)              continue ;
//       if ( treeVars.eleId[i] != 15)              continue ;

//       cout << "Run : " << treeVars.runId  
//     << "  lumi : " << treeVars.lumiId   
//     << "  evt  : " << treeVars.eventId 
//     << "  eleEt : " << et 
//     << "  met : " <<  treeVars.caloMet
//     << "  sumEt : " <<  sumEt
//     << "  met/sumEt : " << (met/sumEt) 
//     << "  dphi : " << dphi 
     
//     << endl;

      // apply selections : Jim
      //if ( et < 18.)              continue ;
   if ( met < 18. )           continue;
   if ( dphi < 0.75)          continue;    
   if ( sumEt!=0 && (met/sumEt) < 0.5 )   continue;
//    if ( met < (6 - 10.*(met/sumEt - 2))) continue;
      

   // electron ID homemade
   
   
   float HoE = reader.GetFloat("electrons_hOverE")->at(iEle);
   float SigmaIEtaIEta = reader.GetFloat("electrons_sigmaIetaIeta")->at(iEle);
   float DeltaPhiIn = reader.GetFloat("electrons_deltaPhiIn")->at(iEle);
   float DeltaEtaIn = reader.GetFloat("electrons_deltaEtaIn")->at(iEle);
   
   
 //  if (fabs(eta) <= EtaCutEB && (HoE>0.030 || SigmaIEtaIEta>0.01)) continue;
 //  if (fabs(eta) > EtaCutEB  && (HoE>0.025 || SigmaIEtaIEta>0.03)) continue;
      
 //  if (DeltaPhiIn > 0.05) continue;

   // electron isolation 
   
   float eleTrkIso = reader.GetFloat("electrons_tkIso")->at(iEle);
   float eleEcalIso = reader.GetFloat("electrons_emIso03")->at(iEle);
   float eleHcalIsoD1 = reader.GetFloat("electrons_hadIso03_1")->at(iEle);
   float eleHcalIsoD2 = reader.GetFloat("electrons_hadIso03_2")->at(iEle);

   unsigned int eleMisHits = reader.GetInt("electrons_mishits")->at(iEle);

   
 //  if (fabs(eta) <= EtaCutEB && (eleTrkIso/pt>0.05 || eleEcalIso/pt>0.07 || (eleHcalIsoD1+eleHcalIsoD2)/pt>0.03)) continue;
 //  if (fabs(eta) > EtaCutEB  && (eleTrkIso/pt>0.05 || eleEcalIso/pt>0.05 || (eleHcalIsoD1+eleHcalIsoD2)/pt>0.02)) continue;

   // Spike cleaning - swiss cross
   if (reader.GetFloat("electrons_SwissCross")->at(iEle) > 0.95) continue;

   // eleId 80% cfr https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
   if (eleMisHits > 0) continue;
   if (fabs(eta) <= EtaCutEB && ((eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pt > 0.07)) continue;
   if (fabs(eta) <= EtaCutEB && (eleTrkIso/pt>0.09 || eleEcalIso/pt>0.07 || (eleHcalIsoD1+eleHcalIsoD2)/pt>0.09)) continue;
   if (fabs(eta) <= EtaCutEB && (HoE>0.040 || SigmaIEtaIEta>0.01)) continue;
   if (fabs(eta) <= EtaCutEB && DeltaPhiIn > 0.06) continue;
   if (fabs(eta) <= EtaCutEB && DeltaEtaIn > 0.004) continue;
   
   if (fabs(eta) > EtaCutEB && ((eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pt > 0.06)) continue;
   if (fabs(eta) > EtaCutEB && (eleTrkIso/pt>0.04 || eleEcalIso/pt>0.05 || (eleHcalIsoD1+eleHcalIsoD2)/pt>0.025)) continue;
   if (fabs(eta) > EtaCutEB && (HoE>0.025 || SigmaIEtaIEta>0.03)) continue;
   if (fabs(eta) > EtaCutEB && DeltaPhiIn > 0.03) continue;
   
  // eleId 60% cfr https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
//   if (eleMisHits > 0) continue;

//   if (fabs(eta) <= EtaCutEB && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2 > 0.03)) continue;
//   if (fabs(eta) <= EtaCutEB && (eleTrkIso/pt>0.04 || eleEcalIso/pt>0.04 || (eleHcalIsoD1+eleHcalIsoD2)/pt>0.03)) continue;
//   if (fabs(eta) <= EtaCutEB && (HoE>0.025 || SigmaIEtaIEta>0.01)) continue;
//   if (fabs(eta) <= EtaCutEB && DeltaPhiIn > 0.025) continue;
//   if (fabs(eta) <= EtaCutEB && DeltaEtaIn > 0.004) continue;
   
//   if (fabs(eta) > EtaCutEB && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2 > 0.02)) continue;
//   if (fabs(eta) > EtaCutEB && (eleTrkIso/pt>0.025 || eleEcalIso/pt>0.02 || (eleHcalIsoD1+eleHcalIsoD2)/pt>0.02)) continue;
//   if (fabs(eta) > EtaCutEB && (HoE>0.025 || SigmaIEtaIEta>0.03)) continue;
//   if (fabs(eta) > EtaCutEB && DeltaPhiIn > 0.02) continue;

// electron ID
//    if ( treeVars.eleId[i] != 15 ) continue;
            
   nGoodElectrons++;
   chosenEle = iEle;
      
  }// end loop over ele cand
    

  hNGoodElectrons->Fill( nGoodElectrons,w);
  if ( nGoodElectrons != 1 ) continue; ///=== only one electron selected!
//   if ( nGoodElectrons == 0 ) continue;
    
  eta = reader.GetFloat("electrons_scEta")->at(chosenEle);
  
  float pt = reader.Get3V("electrons_p_atVtx")->at(chosenEle).Rho();
  p  = reader.Get3V("electrons_p_atVtx")->at(chosenEle).R();
    
  float et = reader.GetFloat("electrons_scEt")->at(chosenEle);
    
  float met  = reader.Get4V("CALOMet")->at(0).Et();
  
  float cphi = (reader.Get3V("electrons_p_atVtx")->at(chosenEle).x() * reader.Get4V("CALOMet")->at(0).Px() 
    + reader.Get3V("electrons_p_atVtx")->at(chosenEle).y() * reader.Get4V("CALOMet")->at(0).Py()) 
     / (met*pt);

  float mt   = sqrt(2*et*met*(1-cphi));
    
  float dphi = deltaPhi(reader.Get4V("CALOMet")->at(0).Phi(),reader.GetFloat("electrons_scPhi")->at(chosenEle));

  float sumEt = 0;
  for (unsigned int iJet=0; iJet<reader.Get4V("jets")->size(); iJet++){
   float deta = reader.Get4V("jets")->at(iJet).Eta() -  eta;
   float dphi = deltaPhi(reader.Get4V("jets")->at(iJet).Phi(),reader.GetFloat("electrons_scPhi")->at(chosenEle));
   float dR = sqrt(deta*deta+dphi*dphi);
   if ( dR < 0.4 && reader.Get4V("jets")->at(iJet).Pt()>15.) continue;
   sumEt += reader.Get4V("jets")->at(iJet).Pt();
  }

  if ( et > 18.) {
   nWele++;
//   cout << "Run : " << treeVars.runId  
//        << "  lumi : " << treeVars.lumiId   
//        << "  evt  : " << treeVars.eventId 
//        << "  eleEt : " << et 
//        << "  eleId : " <<  treeVars.eleId[chosenEle] 
//        << "   eta : " << treeVars.eleEta[chosenEle] 
//        << "   phi : " << treeVars.elePhi[chosenEle] 
//        << endl;

   
   pT = pt;
   ET = et;
   MT = mt;
   EoP = reader.GetFloat("electrons_scE")->at(chosenEle) /p;
   E9oE25 = reader.GetFloat("electrons_e3x3")->at(chosenEle) / reader.GetFloat("electrons_e5x5")->at(chosenEle);

   E2x2 = reader.GetFloat("electrons_e2x2")->at(chosenEle);
   E3x3 = reader.GetFloat("electrons_e3x3")->at(chosenEle);
   E5x5 = reader.GetFloat("electrons_e5x5")->at(chosenEle);
   ESC = reader.GetFloat("electrons_scE")->at(chosenEle);
   Sigma_Phi = reader.GetFloat("electrons_SC_phiWidth")->at(chosenEle);
   Sigma_Eta = reader.GetFloat("electrons_SC_etaWidth")->at(chosenEle);
   pIn = reader.GetFloat("electrons_pin")->at(chosenEle);
   pOut = reader.GetFloat("electrons_pout")->at(chosenEle);
   pAtCalo = reader.Get3V("electrons_p_atCalo")->at(chosenEle).R();
   
   
   ///==== p ====

   // fill histos EB+EE
   het  ->Fill(et,w);
   hmet ->Fill(met,w);
   hmt  ->Fill(mt,w);
   heop ->Fill(EoP ,w);

      // fill histos EB
   if (fabs(eta) < EtaCutEB){
    hetEB  -> Fill(et,w);
    hmtEB  -> Fill(mt,w);
    hmetEB -> Fill(met,w);
    heopEB ->Fill(EoP ,w);
   }

      // fill histos EE
   if (fabs(eta) > EtaCutEE && fabs(eta) < EtaMax ){
    hetEE  -> Fill(et,w);
    hmtEE  -> Fill(mt,w);
    hmetEE -> Fill(met,w);
    heopEE ->Fill(EoP ,w);
   }
   
   
   
   //if (treeVars.HLT_Photon10_L1R==0) continue;
  //if (treeVars.HLT_Ele15_LW_L1R==0) continue;
   
   HLT_Ele15_LW_L1R = -2;
   HLT_Photon10_L1R = -2;
   HLT_Photon15_L1R = -2;
   HLT_Photon20_L1R = -2;
   
   eleFBrem  = reader.GetFloat("electrons_fbrem")->at(chosenEle);
   eleES = reader.GetFloat("electrons_ES")->at(chosenEle);

   myTree -> Fill(); ///==== only if there is only one electron then save
   nWeleSel++;
  }
    
    

    // scaled distr.
  for (int i = 0; i< 200; i++){
   float alpha = 1. + 0.001*(i-100);
      // EB
   if (fabs(eta) < EtaCutEB && (alpha*et) > 18.){
    hetscaled[i]   ->Fill(alpha*et);
    hetEBscaled[i] ->Fill(alpha*et);
    hmtEBscaled[i] ->Fill(alpha*mt); 
    heopEBscaled[i]->Fill(alpha * reader.GetFloat("electrons_scE")->at(chosenEle) /p ,w);
   }
      
      // EE
   if ( fabs(eta) > EtaCutEE && fabs(eta) < EtaMax && (alpha*et) > 18.){
    hetscaled[i]   ->Fill(alpha*et);
    hetEEscaled[i] ->Fill(alpha*et);
    hmtEEscaled[i] ->Fill(alpha*mt); 
    heopEEscaled[i]->Fill(alpha * reader.GetFloat("electrons_scE")->at(chosenEle) /p ,w);
   }
  }
 } // end loop over entries
  
 cout << "Found " << nWele << " electrons from W" << endl;
 cout << "Found " << nWeleSel << ":" << nWele << " only 1 e/W " << endl;

  // FILE to save histos

 
 fout->cd();

 myTree -> Write();

 hNGoodElectrons->Write();
 het    -> Write();
 hmet   -> Write();
 hmt    -> Write();
 hetEB  -> Write();
 hmetEB -> Write();
 hmtEB  -> Write();
 hetEE  -> Write();
 hmetEE -> Write();
 hmtEE  -> Write();
  
 heop   -> Write();
 heopEB -> Write();
 heopEE -> Write();

  
 hSumEt-> Write();
 hmet_o_SumEt->Write();
 hmet_vs_metOverSumEt ->Write();

  // if MC , save also scale d histograms
 if ( ff == 0 ){
  for (int i = 0; i < 200 ; i++){ 
   hetscaled[i]  ->Write();
   hetEBscaled[i]->Write();
   hetEEscaled[i]->Write();
   hmtEBscaled[i]->Write();
   hmtEEscaled[i]->Write();
   heopEBscaled[i]->Write();
   heopEEscaled[i]->Write();
  }
 }







}

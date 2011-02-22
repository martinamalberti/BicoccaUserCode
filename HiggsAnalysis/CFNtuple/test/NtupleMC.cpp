
///==== Legge un NTupla e seleziona solo coppie di jet con pT>27 e posizione di pseudorapidità desiderata: sceglie il jet forard e central con pt Massimo ====

#include "treeReader.h"
#include "hFactory.h"
#include "hFunctions.h"
#include "stdHisto.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "Math/GenVector/VectorUtil.h"
#include "TRandom3.h"
#include <time.h>
#include <sstream>
#include "CFNtuple.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif

using namespace std;

int main(int argc, char** argv)	// chiede in ingresso il file di configurazione .cfg
{ 
 //Check if all nedeed arguments to parse are there
 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }
 
 // Parse the config file --> legge le info del file .cfg
 parseConfigFile (argv[1]) ;
 std::string treeName= gConfigParser -> readStringOption("Input::treeName");	//HADRON LEVEL
 std::string inputFile = gConfigParser -> readStringOption("Input::inputFile");
 std::string nameGenJet = gConfigParser -> readStringOption("Input::nameGenJet"); //
 
 double inputXSection  = gConfigParser -> readDoubleOption("Input::inputXSection");
  
 int entryMAX = gConfigParser -> readIntOption("Input::entryMAX");	//numero totale di eventi?entrate?
 int entryMIN = gConfigParser -> readIntOption("Input::entryMIN");
 int entryMOD = gConfigParser -> readIntOption("Input::entryMOD");
 
 double ptMin = gConfigParser -> readDoubleOption("Input::ptMin"); //
 double DRmax = gConfigParser -> readDoubleOption("Input::DRmax"); //
 
 
 std::cout << ">>>>> input::entryMIN  " << entryMIN  << std::endl;  
 std::cout << ">>>>> input::entryMAX  " << entryMAX  << std::endl;   
 std::cout << ">>>>> input::entryMOD  " << entryMOD  << std::endl;  
 std::cout << ">>>>> input::ptMin  " << ptMin  << std::endl;   
 std::cout << ">>>>> input::DRmax  " << DRmax  << std::endl;  
 
 // Open ntple --> crea una catena di Tree
 TChain* chain = new TChain(treeName.c_str());	//treeName e inputFile sono definti nel file .cfg
 chain->Add(inputFile.c_str());
 treeReader reader((TTree*)(chain));	//qui definisco reader
 

 ///****************************
 ///**** DATA JSON file ****
 ///***************************************************

  
 int dataFlag = 0;
 if (inputXSection == -1) dataFlag = 1; //==== it's a data sample!!!
  // define map with events
  std::map<std::pair<int,std::pair<int,int> >,int> eventsMap;  
 
/* 
  std::string inFileNameJSON;
  try {
  inFileNameJSON = gConfigParser -> readStringOption("Input::inFileNameJSON");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 std::cout << ">>>>> Input::inFileNameJSON  " << inFileNameJSON  << std::endl;  
 std::map<int, std::vector<std::pair<int, int> > > jsonMap;
 if( dataFlag == 1 ) {
   jsonMap = readJSONFile(inFileNameJSON);	//Definisco la jsonMap!
}*/


 //======================== Istogrammi di controllo ==========================================
 
 std::string OutFileName    = gConfigParser -> readStringOption("Output::outFileName");
 std::cout << ">>>>> Output::outFileName  " << OutFileName  << std::endl;  
 
 TFile outFile(OutFileName.c_str(),"RECREATE");
 outFile.cd();
 
 TH1F hPtC("hPtC","central jets",1000,0,500);	//jet centrali
 TH1F hPtF("hPtF","forward jets",1000,0,500);	//jet forward
 
 TH1F hPtC_reco("hPtC_reco","central jets",1000,0,500);	//jet centrali
 TH1F hPtF_reco("hPtF_reco","forward jets",1000,0,500);	//jet forward

 TH2F EtaPtF_reco("EtaPtF_reco","EtaPtF_reco", 200,-7,7, 1000,0,500);	//eta sulla x, pt sulle y
 TH2F EtaPtF_had("EtaPtF_had","EtaPtF_had", 200,-7,7, 1000,0,500);	//eta sulla x, pt sulle y
 
 TH1F efficiency("efficiency","efficiency",0,1,4);	//jet centrali


 

 ///==============
 ///==== Jets Tree ====
 //G--> hadron level
 //S--> reco level
 
 TTree AnaHiggs("AnaHiggs","AnaHiggs");
 
 double G_FJet_Pt;
 double G_FJet_Eta;
 double G_FJet_Phi;
 double G_FJet_Energy;
 double G_CJet_Pt;
 double G_CJet_Eta;
 double G_CJet_Phi;
 double G_CJet_Energy;
 
 double G_Mjj;
 double G_D_Eta;
 double G_D_Phi;
 double G_D_R;
 
 double S_FJet_Pt;
 double S_FJet_Eta;
 double S_FJet_Phi;
 double S_FJet_Energy;
 double S_CJet_Pt;
 double S_CJet_Eta;
 double S_CJet_Phi;
 double S_CJet_Energy;
 
 double S_Mjj;
 double S_D_Eta;
 double S_D_Phi;
 double S_D_R;
 
 /*double Trig_Jet15;
 double Trig_Jet30;
 double Trig_Jet50;
 double Trig_DiJ15;
 double Trig_DiJ30;*/
 
 int S_RunNb;
 int S_LumiBlk;
 int S_EventNb;
 //int S_Vtxnum;
 
 double DR_C;	//DR tra had e reco jets
 double DR_F;	//DR tra had e reco jets
 
 int eventSel;
 
 //int EtaSel;	//trigger su eta
 //int PtSel_had;	// trigger su ptMin
 //int PtSel_reco;	// trigger su ptMin

 
 //double MET;
 // int AnalysisStep; 
  

 AnaHiggs.Branch("G_FJet_Pt",&G_FJet_Pt,"G_FJet_Pt/D"); 	//HADRON
 AnaHiggs.Branch("G_FJet_Eta",&G_FJet_Eta,"G_FJet_Eta/D");
 AnaHiggs.Branch("G_FJet_Phi",&G_FJet_Phi,"G_FJet_Phi/D");
 AnaHiggs.Branch("G_FJet_Energy",&G_FJet_Energy,"G_FJet_Energy/D");

 AnaHiggs.Branch("G_CJet_Pt",&G_CJet_Pt,"G_CJet_Pt/D");
 AnaHiggs.Branch("G_CJet_Eta",&G_CJet_Eta,"G_CJet_Eta/D");
 AnaHiggs.Branch("G_CJet_Phi",&G_CJet_Phi,"G_CJet_Phi/D");
 AnaHiggs.Branch("G_CJet_Energy",&G_CJet_Energy,"G_CJet_Energy/D");
 
 AnaHiggs.Branch("G_Mjj",&G_Mjj,"G_Mjj/D");
 AnaHiggs.Branch("G_D_Eta",&G_D_Eta,"G_D_Eta/D");
 AnaHiggs.Branch("G_D_Phi",&G_D_Phi,"G_D_Phi/D");
 AnaHiggs.Branch("G_D_R",&G_D_R,"G_D_R/D");
 
 AnaHiggs.Branch("S_FJet_Pt",&S_FJet_Pt,"S_FJet_Pt/D");  	//RECO
 AnaHiggs.Branch("S_FJet_Eta",&S_FJet_Eta,"S_FJet_Eta/D");
 AnaHiggs.Branch("S_FJet_Phi",&S_FJet_Phi,"S_FJet_Phi/D");
 AnaHiggs.Branch("S_FJet_Energy",&S_FJet_Energy,"S_FJet_Energy/D");

 AnaHiggs.Branch("S_CJet_Pt",&S_CJet_Pt,"S_CJet_Pt/D");
 AnaHiggs.Branch("S_CJet_Eta",&S_CJet_Eta,"S_CJet_Eta/D");
 AnaHiggs.Branch("S_CJet_Phi",&S_CJet_Phi,"S_CJet_Phi/D");
 AnaHiggs.Branch("S_CJet_Energy",&S_CJet_Energy,"S_CJet_Energy/D");
 
 AnaHiggs.Branch("S_Mjj",&S_Mjj,"S_Mjj/D");
 AnaHiggs.Branch("S_D_Eta",&S_D_Eta,"S_D_Eta/D");
 AnaHiggs.Branch("S_D_Phi",&S_D_Phi,"S_D_Phi/D");
 AnaHiggs.Branch("S_D_R",&S_D_R,"S_D_R/D");

 AnaHiggs.Branch("DR_C",&DR_C,"DR_C/D");
 AnaHiggs.Branch("DR_F",&DR_F,"DR_F/D");
 
 /*AnaHiggs.Branch("Trig_Jet15",&Trig_Jet15,"Trig_Jet15/D");
 AnaHiggs.Branch("Trig_Jet30",&Trig_Jet30,"Trig_Jet30/D");
 AnaHiggs.Branch("Trig_Jet50",&Trig_Jet50,"Trig_Jet50/D");
 AnaHiggs.Branch("Trig_DiJ15",&Trig_DiJ15,"Trig_DiJ15/D");
 AnaHiggs.Branch("Trig_DiJ30",&Trig_DiJ30,"Trig_DiJ30/D");*/
 
 AnaHiggs.Branch("S_RunNb",&S_RunNb,"S_RunNb/I");
 AnaHiggs.Branch("S_LumiBlk",&S_LumiBlk,"S_LumiBlk/I");
 AnaHiggs.Branch("S_EventNb",&S_EventNb,"S_EventNb/I");
 //AnaHiggs.Branch("S_Vtxnum",&S_Vtxnum,"S_Vtxnum/I");
 
 AnaHiggs.Branch("S_D_Eta",&S_D_Eta,"S_D_Eta/D");
 AnaHiggs.Branch("S_D_Phi",&S_D_Phi,"S_D_Phi/D");
 AnaHiggs.Branch("S_D_R",&S_D_R,"S_D_R/D");

 //AnaHiggs.Branch("eventSel", &eventSel, "eventSel/I");
// AnaHiggs.Branch("PtSel_had", &PtSel_had, "PtSel_had/I");
// AnaHiggs.Branch("PtSel_reco", &PtSel_reco, "PtSel_reco/I");



 
 

 ///Selezione sugli eventi Dijet at HADRON LEVEL
 if (entryMAX == -1) entryMAX = reader.GetEntries();	// GetEntries restituisce il numero totale di eventi, qui utilizzo il reader per leggere dal Tree il numero di entrate! GetEntries è una funzione del reader
 else if (reader.GetEntries() < entryMAX) entryMAX = reader.GetEntries();	//imposto entryMAX leggendo dal TREE in questione
 

 double totalJets_had = 0;
 double totalJets_reco = 0;
 //double eventSel_had = 0;
 //double eventSel_reco = 0;
 
 for(int iEvent = entryMIN ; iEvent < entryMAX ; ++iEvent) {	//faccio un ciclo su tutte le entrate del Tree
  reader.GetEntry(iEvent);	//un'entrata in questo caso è data da un TREE intero
  if((iEvent%entryMOD) == 0) std::cout << ">>>>> analysis::GetEntry " << iEvent  << ":" << reader.GetEntries() << " (" << entryMAX << ")" << std::endl;   
  
  
  ///***************************************************
  ///**** STEP -1 - Check no copies in DATA ****
  ///***************************************************
  
  
  /*
  if( dataFlag == 1 )
  {
   std::pair<int,int> eventLSandID(reader.GetInt("lumiId")->at(0), reader.GetInt("eventId")->at(0));
   std::pair<int,std::pair<int,int> > eventRUNandLSandID(reader.GetInt("runId")->at(0), eventLSandID);
   
   if( eventsMap[eventRUNandLSandID] == 1 ) continue;
   else eventsMap[eventRUNandLSandID] = 1;
  }*/

  ///*************************************************
  ///**** Check comparison with JSON file ***
  ///*************************************************

/*
  if( dataFlag == 1 )
    {
      int runId  = reader.GetInt("runId")->at(0);
      int lumiId = reader.GetInt("lumiId")->at(0);
      if(AcceptEventByRunAndLumiSection(runId, lumiId, jsonMap) == false) continue;      
    }

  
  */
//   std::cerr << " 1 ... " << std::endl;  
  int nJets_had = reader.Get4V(nameGenJet.c_str())->size();	//qui dovrei accedere al GenJet
  totalJets_had = totalJets_had + nJets_had;
  
//   std::cerr << " 2 ... " << std::endl;
  int nJets_reco = reader.Get4V("jets")->size();	// qui è il jet a livello reco
  totalJets_reco = totalJets_reco + nJets_reco;
  
   
  //const double ptMin = 10.0;	//le passo dal file di configurazione!
  //const double D_R_cut = 100;
  
//  cout << "=================== Selezione sugli eventi Dijet at HADRON LEVEL =============="<<endl;

  ///HADRON BLACKLIST
  std::vector <int> blacklistCentral_had;
  std::vector <int> blacklistForward_had;
  
//   std::cerr << " 3 ... " << std::endl;
  for (int i = 0; i < nJets_had; ++i) {
    if (fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())>2.8){
      blacklistCentral_had.push_back(i);
    }
  // Riempio blacklist Forward (con tutti i jet non forward)
    else if ( fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())>4.7 || fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())<3.2 ) {
     blacklistForward_had.push_back(i);
    }
   }
   
  //Seleziono i due jet non presenti sulle black list con Pt massimo
//   std::cerr << " beginning ... " << std::endl;
  int Central_i_had = SelectObject(*(reader.Get4V(nameGenJet.c_str())), "maxPt", ptMin, &blacklistCentral_had);
//   std::cerr << " Central_i_had  = " << Central_i_had  << std::endl;
  int Forward_i_had = SelectObject(*(reader.Get4V(nameGenJet.c_str())), "maxPt", ptMin, &blacklistForward_had);
//   std::cerr << " Forward_i_had  = " << Forward_i_had  << std::endl;
  
  ///RECO BLACKLIST  
  std::vector <int> blacklistCentral_reco;
  std::vector <int> blacklistForward_reco;
  
  for(int i = 0; i < nJets_reco; ++i){
    if(fabs(reader.Get4V("jets")->at(i).Eta())>2.8){
      blacklistCentral_reco.push_back(i);
    }
  // Riempio blacklist Forward (con tutti i jet non forward)
    else if(fabs(reader.Get4V("jets")->at(i).Eta())>4.7 || fabs(reader.Get4V("jets")->at(i).Eta())<3.2) {
     blacklistForward_reco.push_back(i);
    }
   }
   
  //Seleziono i due jet non presenti sulle black list con Pt massimo
 
  int Central_i_reco = SelectObject(*(reader.Get4V("jets")), "maxPt", ptMin, &blacklistCentral_reco);
//   std::cerr << " Central_i_reco  = " << Central_i_reco  << std::endl;
  int Forward_i_reco = SelectObject(*(reader.Get4V("jets")), "maxPt", ptMin, &blacklistForward_reco);
//   std::cerr << " Forward_i_reco  = " << Forward_i_reco  << std::endl;
  
  ///FILLING VARIABLES
  if (dataFlag!=1){ 	//se si tratta di Dati non cercare nemmeno i GenJet!
    if (Central_i_had !=-1 && Forward_i_had !=-1){
      //&& 
     //riempi l'istogramma e il file con:
//      std::cerr << ">>> Central_i_had  = " << Central_i_had  << std::endl;
//      std::cerr << ">>> Forward_i_had  = " << Forward_i_had  << std::endl;
     
     hPtC.Fill(reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Pt());	//riempio l'istogramma
     hPtF.Fill(reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Pt());	//riempio l'istogramma

     EtaPtF_had.Fill(reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Eta(), reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Pt());	//riempio l'istogramma

     //qui devo impostare tutte le variabili del tree!
     //Variabili at hadron level
     G_FJet_Pt = reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Pt();
     G_FJet_Eta = reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Eta();
     G_FJet_Phi = reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Phi();
     G_FJet_Energy = reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).energy();
     
     G_CJet_Pt = reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Pt();
     G_CJet_Eta = reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Eta();
     G_CJet_Phi = reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Phi();
     G_CJet_Energy= reader.Get4V(nameGenJet.c_str())->at(Central_i_had).energy();
  
     G_D_Eta = deltaEta(reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Eta(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Eta());
     G_D_Phi = deltaPhi(reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Phi(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Phi());
     G_D_R = deltaR(reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Eta(),reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Phi(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Eta(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Phi());
    
    }
  }
    
     else {
       //valori di default in assenza di jet at hadron level
     G_FJet_Pt = -100.;
     G_FJet_Eta = -100.;
     G_FJet_Phi = -100.;
     G_FJet_Energy = -100.;
     
     G_CJet_Pt = -100.;
     G_CJet_Eta = -100.;
     G_CJet_Phi = -100.;
     G_CJet_Energy= -100.;
  
     G_D_Eta = -100.;
     G_D_Phi = -100.;
     G_D_R = -100.;
    
     }
     
     //Variabili at detector level
     if(Central_i_reco !=-1 && Forward_i_reco !=-1) {
      
     EtaPtF_reco.Fill(reader.Get4V("jets")->at(Forward_i_reco).Eta(), reader.Get4V("jets")->at(Forward_i_reco).Pt());	//riempio l'istogramma
      
     hPtC_reco.Fill(reader.Get4V("jets")->at(Central_i_reco).Pt());	//riempio l'istogramma
     hPtF_reco.Fill(reader.Get4V("jets")->at(Forward_i_reco).Pt());	//riempio l'istogramma
     
     S_FJet_Pt = reader.Get4V("jets")->at(Forward_i_reco).Pt();
     S_FJet_Eta = reader.Get4V("jets")->at(Forward_i_reco).Eta();
     S_FJet_Phi = reader.Get4V("jets")->at(Forward_i_reco).Phi();
     S_FJet_Energy = reader.Get4V("jets")->at(Forward_i_reco).energy();
     
     S_CJet_Pt = reader.Get4V("jets")->at(Central_i_reco).Pt();
     S_CJet_Eta = reader.Get4V("jets")->at(Central_i_reco).Eta();
     S_CJet_Phi = reader.Get4V("jets")->at(Central_i_reco).Phi();
     S_CJet_Energy= reader.Get4V("jets")->at(Central_i_reco).energy();
  
     S_D_Eta = deltaEta(reader.Get4V("jets")->at(Forward_i_reco).Eta(),reader.Get4V("jets")->at(Central_i_reco).Eta());
     S_D_Phi = deltaPhi(reader.Get4V("jets")->at(Forward_i_reco).Phi(),reader.Get4V("jets")->at(Central_i_reco).Phi());
     S_D_R = deltaR(reader.Get4V("jets")->at(Forward_i_reco).Eta(),reader.Get4V("jets")->at(Forward_i_reco).Phi(),reader.Get4V("jets")->at(Central_i_reco).Eta(),reader.Get4V("jets")->at(Central_i_reco).Phi());
      
     DR_F =  deltaR(reader.Get4V("jets")->at(Forward_i_reco).Eta(),reader.Get4V("jets")->at(Forward_i_reco).Phi(),reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Eta(),reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Phi());
     DR_C =  deltaR(reader.Get4V("jets")->at(Central_i_reco).Eta(),reader.Get4V("jets")->at(Central_i_reco).Phi(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Eta(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Phi());
     
      
     }
    
     
     else {
       //valori di default in assenza di jet at reco level
     S_FJet_Pt = -100.;
     S_FJet_Eta = -100.;
     S_FJet_Phi = -100.;
     S_FJet_Energy = -100.;
     
     S_CJet_Pt = -100.;
     S_CJet_Eta = -100.;
     S_CJet_Phi = -100.;
     S_CJet_Energy= -100.;
  
     S_D_Eta = -100.;
     S_D_Phi = -100.;
     S_D_R = -100.;
      
     DR_F = -100.;
     DR_C = -100.;
     }
     
     S_RunNb = reader.GetInt("runId")->at(0);
     S_LumiBlk = reader.GetInt("lumiId")->at(0);
     S_EventNb = reader.GetInt("eventId")->at(0);
     //S_Vtxnum = ;
          
     AnaHiggs.Fill();    
     eventSel++;
 }    
    
//     std::cerr << " here " << std::endl;
    efficiency.SetBinContent(1,entryMAX-entryMIN);
    efficiency.SetBinContent(2,eventSel);
 
 
    /*
 //end = clock();
 //std::cout <<"Time = " <<  ((double) (end - start)) << " (a.u.)" << std::endl;  
 cout<<"==================================== HADRON ==========================================="<<endl;
 std::cout<<"eventSel_had = "<<eventSel_had<<std::endl;
 std::cout<<"eventSel_had/entryMAX = "<<eventSel_had/entryMAX<<std::endl;
 
 cout<<"==================================== RECO ==========================================="<<endl;
 std::cout<<"eventSel_reco = "<<eventSel_reco<<std::endl;
 std::cout<<"eventSel_reco/entryMAX = "<<eventSel_reco/entryMAX<<std::endl;
 */
 
 //hPt.Write();
 //Eta.Write();
 outFile.Write();
 
 
 return 0;
}

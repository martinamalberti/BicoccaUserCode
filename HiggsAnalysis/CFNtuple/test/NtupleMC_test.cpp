
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
 std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList"); //--- nome di un file txt con l'elenco di root file da anlizzare: elenco dei file su più righe con path assoluto
 std::string nameGenJet = gConfigParser -> readStringOption("Input::nameGenJet"); //
 
 double inputXSection  = gConfigParser -> readDoubleOption("Input::inputXSection");
  
 int entryMAX = gConfigParser -> readIntOption("Input::entryMAX");	//numero totale di eventi?entrate?
 int entryMIN = gConfigParser -> readIntOption("Input::entryMIN");
 int entryMOD = gConfigParser -> readIntOption("Input::entryMOD");
 
 double ptMinF = gConfigParser -> readDoubleOption("Input::ptMinF"); //
 double ptMinC = gConfigParser -> readDoubleOption("Input::ptMinC"); //
 
 double DRmax = gConfigParser -> readDoubleOption("Input::DRmax"); //
 int debug = gConfigParser -> readIntOption("Input::debug"); // se uguale a 1 esegui il debug altrimenti no.
 
 
 std::cout << ">>>>> input::entryMIN  " << entryMIN  << std::endl;  
 std::cout << ">>>>> input::entryMAX  " << entryMAX  << std::endl;   
 std::cout << ">>>>> input::entryMOD  " << entryMOD  << std::endl;  
 std::cout << ">>>>> input::ptMinF  " << ptMinF  << std::endl;   
 std::cout << ">>>>> input::ptMinC  " << ptMinC  << std::endl;   
 std::cout << ">>>>> input::DRmax  " << DRmax  << std::endl;  
 
 // Open ntple --> crea una catena di Tree

 TChain* chain = new TChain (treeName.c_str ()) ;
 if (!FillChain (*chain, inputFileList.c_str ())) return 1 ;
 treeReader reader ( (TTree*) (chain)) ;

// TChain* chain = new TChain(treeName.c_str());	//treeName e inputFile sono definti nel file .cfg
// chain->Add(inputFile.c_str());
// treeReader reader((TTree*)(chain));	//qui definisco reader
 ///check Ntuple type: if -1 skip vertex filter and events counting
 
 int ntupleTYPE = 1;
 
  try {
  ntupleTYPE = gConfigParser -> readIntOption("Input::ntupleTYPE");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 std::cout << ">>>>> Input::ntupleTYPE  " << ntupleTYPE << std::endl; 
  

 TH1F* events = new TH1F ("events", "events", 1, 0., 1.) ;

 if (ntupleTYPE!=-1){
   
  std::map<int,int> totalEvents = GetTotalEvents ("AllEvents/totalEvents", inputFileList.c_str ()) ;  
  std::map<int, int> stepEvents ;
  stepEvents[0] = totalEvents[1] ;

  events -> SetBinContent (1, stepEvents[0]) ;
 }


 ///****************************
 ///**** DATA JSON file ****
 ///***************************************************

  
 int dataFlag = 0;
 if (inputXSection == -1) dataFlag = 1; //==== it's a data sample!!!
  // define map with events
  std::map<std::pair<int,std::pair<int,int> >,int> eventsMap;  
  
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
}

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
 
 
 int NBIN_mult = 20;
 
 TH1F hMultiplicity("hMultiplicity","central jets multiplicity",NBIN_mult,0,6);		//jet centrali
  
 

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



 
 int nHLT = -1;

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
  

  
  ///***************************************
  ///**** STEP -2 - Event preselections ****
  ///***************************************
 if (ntupleTYPE!=-1){
   //**** primaryVertexFilter
   if (fabs(reader.GetFloat("PV_z")->at(0))> 24) continue;
   if (fabs(reader.GetFloat("PV_d0")->at(0))> 2) continue;
   if (fabs(reader.GetInt("PV_ndof")->at(0)) <= 4) continue;

   if (reader.GetInt("PV_nTracks")->at(0) < 10) continue;

 }
 
  ///***************************************************
  ///**** STEP -1 - Check no copies in DATA ****
  ///***************************************************
  
  
  
  if( dataFlag == 1 )
  {
   std::pair<int,int> eventLSandID(reader.GetInt("lumiId")->at(0), reader.GetInt("eventId")->at(0));
   std::pair<int,std::pair<int,int> > eventRUNandLSandID(reader.GetInt("runId")->at(0), eventLSandID);
   
   if( eventsMap[eventRUNandLSandID] == 1 ) continue;
   else eventsMap[eventRUNandLSandID] = 1;
  }

  ///*************************************************
  ///**** Check comparison with JSON file ***
  ///*************************************************


  if( dataFlag == 1 )
    {
      int runId  = reader.GetInt("runId")->at(0);
      int lumiId = reader.GetInt("lumiId")->at(0);
      if(AcceptEventByRunAndLumiSection(runId, lumiId, jsonMap) == false) continue;      
    }


  ///************************
  ///**** HLT simulation ****
  ///************************
  
  if (iEvent == entryMIN) {
   for (int iHLT = 0; iHLT < reader.GetString("HLT_Names")->size(); iHLT++){
    if (reader.GetString("HLT_Names")->at(iHLT) == "HLT_DiJetAve15U" || reader.GetString("HLT_Names")->at(iHLT) == "HLT_DiJetAve15U_8E29") nHLT = iHLT;
   }
  }
  if (nHLT != -1 && reader.GetFloat("HLT_Accept")->at(nHLT) == 0) {
    if (debug == 1) std::cerr << " HLT = " << reader.GetString("HLT_Names")->at(nHLT) << std::endl;
    continue;
  }

  ///********************
  ///**** HCAL Noise ****
  ///********************

//  if (reader.GetInt("HCAL_noise")->at(0) == 1) continue;
  
  
  if (debug == 1) std::cerr << " 1 ... " << std::endl;  
  int nJets_had = reader.Get4V(nameGenJet.c_str())->size();	//qui dovrei accedere al GenJet
  totalJets_had = totalJets_had + nJets_had;
  
  if (debug == 1)  std::cerr << " 2 ... " << std::endl;
  int nJets_reco = reader.Get4V("jets")->size();	// qui è il jet a livello reco
  totalJets_reco = totalJets_reco + nJets_reco;
   
  ///HADRON BLACKLIST
  std::vector <int> blacklistCentral_had;
  std::vector <int> blacklistForward_had;
  
//   std::cerr << " 3 ... " << std::endl;
  for (int i = 0; i < nJets_had; ++i) {
    if (fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())>2.8){
      blacklistCentral_had.push_back(i);
    }
  // Riempio blacklist Forward (con tutti i jet non forward)
    if ( fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())>4.7 || fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())<3.2 ) {
     blacklistForward_had.push_back(i);
    }
   }
   
  //Seleziono i due jet non presenti sulle black list con Pt massimo
//   std::cerr << " beginning ... " << std::endl;
  int Central_i_had = SelectObject(*(reader.Get4V(nameGenJet.c_str())), "maxPt", ptMinC, &blacklistCentral_had);
  if (debug == 1) std::cerr << " Central_i_had  = " << Central_i_had  << std::endl;
  int Forward_i_had = SelectObject(*(reader.Get4V(nameGenJet.c_str())), "maxPt", ptMinF, &blacklistForward_had);
  if (debug == 1)  std::cerr << " Forward_i_had  = " << Forward_i_had  << std::endl;
  
  ///RECO BLACKLIST  
  std::vector <int> blacklistCentral_reco;
  std::vector <int> blacklistForward_reco;
  
  for(int i = 0; i < nJets_reco; ++i){
    if(fabs(reader.Get4V("jets")->at(i).Eta())>2.8){
      blacklistCentral_reco.push_back(i);
    }
  // Riempio blacklist Forward (con tutti i jet non forward)
    if(fabs(reader.Get4V("jets")->at(i).Eta())>4.7 || fabs(reader.Get4V("jets")->at(i).Eta())<3.2) {
     blacklistForward_reco.push_back(i);
    }
   }
   
//==== Jet ID FWD ====
//  for(int i = 0; i < nJets_reco; ++i){
//   if (reader.GetFloat("jets_n90Hits")->at(i) < (1 + 2.4 *( log(reader.GetFloat("jets_corrFactor_raw")->at(i) * reader.Get4V("jets")->at(i).Pt() ) - 1 ))) 
//    {
//      blacklistForward_reco.push_back(i);
//    }
//  }
   
  //Seleziono i due jet non presenti sulle black list con Pt massimo
 
  int Central_i_reco = SelectObject(*(reader.Get4V("jets")), "maxPt", ptMinC, &blacklistCentral_reco);
  if (debug == 1) std::cerr << " Central_i_reco  = " << Central_i_reco  << std::endl;
  int Forward_i_reco = SelectObject(*(reader.Get4V("jets")), "maxPt", ptMinF, &blacklistForward_reco);
  if (debug == 1) std::cerr << " Forward_i_reco  = " << Forward_i_reco  << std::endl;
  
  
  
  ///MULTIPLICITY
  
  int nC_Jets = 0;
  
  if (Forward_i_had !=-1){
    
   for(int i = 0; i < nJets_reco; ++i){
    if(fabs(reader.Get4V("jets")->at(i).Eta())>2.8 && reader.Get4V("jets")->at(i).Pt()>ptMinC){
      
      nC_Jets ++;
     }
    }
    
    
    hMultiplicity.Fill(nC_Jets);
    
   }
      
  
  
  
  
  
  
  
  
  ///FILLING VARIABLES
  if (Central_i_had !=-1 && Forward_i_had !=-1){
     if (debug == 1) std::cerr << ">>> Central_i_had  = " << Central_i_had  << std::endl;
     if (debug == 1) std::cerr << ">>> Forward_i_had  = " << Forward_i_had  << std::endl;
     
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
    
     S_Mjj = (reader.Get4V(nameGenJet.c_str())->at(Forward_i_had) + reader.Get4V(nameGenJet.c_str())->at(Central_i_had)).mass();      
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
     S_Mjj = (reader.Get4V("jets")->at(Forward_i_reco) + reader.Get4V("jets")->at(Central_i_reco)).mass();      

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
     }
     
      if(Central_i_reco !=-1 && Forward_i_reco !=-1 && Central_i_had !=-1 && Forward_i_had !=-1){
     DR_F =  deltaR(reader.Get4V("jets")->at(Forward_i_reco).Eta(),reader.Get4V("jets")->at(Forward_i_reco).Phi(),reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Eta(),reader.Get4V(nameGenJet.c_str())->at(Forward_i_had).Phi());
     DR_C =  deltaR(reader.Get4V("jets")->at(Central_i_reco).Eta(),reader.Get4V("jets")->at(Central_i_reco).Phi(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Eta(),reader.Get4V(nameGenJet.c_str())->at(Central_i_had).Phi());
           
     }
     
     else{      
     DR_F = -100.;
     DR_C = -100.;
     }
     
     
     
     S_RunNb = reader.GetInt("runId")->at(0);
     S_LumiBlk = reader.GetInt("lumiId")->at(0);
     S_EventNb = reader.GetInt("eventId")->at(0);
          
     AnaHiggs.Fill();    
     eventSel++;
 }    
    
//     std::cerr << " here " << std::endl;
    efficiency.SetBinContent(1,entryMAX-entryMIN);
    efficiency.SetBinContent(2,eventSel);
  
    
 if (ntupleTYPE!=-1){
 events -> Write () ;
 }
 
 outFile.Write();
 
 
 return 0;
}

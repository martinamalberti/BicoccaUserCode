
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
 
  Float_t lowEdge[8] = {35,45,57,72,90,120,150,1000};
  int NBIN = 7;
 
  TH1F hPtF("hPtF","forward jets",1000,0,500);	//jet forward
  
  TH1F hPtF_rebinned("hPtF","forward jets",NBIN,lowEdge);	//jet forward
  

  TH2F EtaPtF_had("EtaPtF_had","EtaPtF_had", 200,-7,7, 1000,0,500);	//eta sulla x, pt sulle y
 
 TH1F efficiency("efficiency","efficiency",0,1,4);	//jet centrali
 
 
 int NBIN_mult = 20;
 
 TH1F hMultiplicityC("hMultiplicityC","central jets multiplicity",NBIN_mult,0,6);		//jet centrali
 TH1F hMultiplicityF("hMultiplicityF","forward jets multiplicity",NBIN_mult,0,6);		//jet forward

 TH1F hMultiplicityC_had("hMultiplicityC_had","had central jets multiplicity",NBIN_mult,0,6);		//jet centrali
 TH1F hMultiplicityF_had("hMultiplicityF_had","had forward jets multiplicity",NBIN_mult,0,6);		//jet forward


 

 ///==============
 ///==== Jets Tree ====
 //G--> hadron level
 //S--> reco level
 
 TTree AnaHiggs("AnaHiggs","AnaHiggs");
 
 double G_FJet_Pt;
 double G_FJet_Eta;
 double G_FJet_Phi;
 double G_FJet_Energy;
 
 int nCJet_G_FJet;
 int nFJet_G_CJet;

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
 
 int nCJet_S_FJet;
 int nFJet_S_CJet;
 
 int nJets_had_sel;
 
 
 double S_CJet_Pt;
 double S_CJet_Eta;
 double S_CJet_Phi;
 double S_CJet_Energy;
 
 double S_Mjj;
 double S_D_Eta;
 double S_D_Phi;
 double S_D_R;
 
  

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

 
 AnaHiggs.Branch("nCJet_G_FJet",&nCJet_G_FJet,"nCJet_G_FJet/I");  	//had
 AnaHiggs.Branch("nFJet_G_CJet",&nFJet_G_CJet,"nFJet_G_CJet/I");  	//had
 AnaHiggs.Branch("nCJet_S_FJet",&nCJet_S_FJet,"nCJet_S_FJet/I");  	//RECO
 AnaHiggs.Branch("nFJet_S_CJet",&nFJet_S_CJet,"nFJet_S_CJet/I");  	//RECO

 AnaHiggs.Branch("nJets_had_sel",&nJets_had_sel,"nJets_had_sel/I");




 
 int nHLT = -1;

 ///Selezione sugli eventi Dijet at HADRON LEVEL
 if (entryMAX == -1) entryMAX = reader.GetEntries();	// GetEntries restituisce il numero totale di eventi, qui utilizzo il reader per leggere dal Tree il numero di entrate! GetEntries è una funzione del reader
 else if (reader.GetEntries() < entryMAX) entryMAX = reader.GetEntries();	//imposto entryMAX leggendo dal TREE in questione
 

 double totalJets_had = 0;
 
 //double eventSel_had = 0;
 //double eventSel_reco = 0;
 
 for(int iEvent = entryMIN ; iEvent < entryMAX ; ++iEvent) {	//faccio un ciclo su tutte le entrate del Tree
  reader.GetEntry(iEvent);	//un'entrata in questo caso è data da un TREE intero
  if((iEvent%entryMOD) == 0) std::cout << ">>>>> analysis::GetEntry " << iEvent  << ":" << reader.GetEntries() << " (" << entryMAX << ")" << std::endl;   
  

  ///************************
  ///**** HLT simulation ****
  ///************************
  
//   if (iEvent == entryMIN) {
   for (int iHLT = 0; iHLT < reader.GetString("HLT_Names")->size(); iHLT++){
    if (reader.GetString("HLT_Names")->at(iHLT) == "HLT_DiJetAve15U") nHLT = iHLT;
   }
//   }
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

  
  ///******************************************************************************************
  ///ciclo su tutti i jet!!! Riempio l'intero tree per ogni singolo jet che passa la selezione!
  ///******************************************************************************************
  
   for (int i = 0; i<nJets_had; i++){
  
  
  ///FILLING VARIABLES
  
  
  
  if (fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())<4.7 && fabs(reader.Get4V(nameGenJet.c_str())->at(i).Eta())>3.2 && reader.Get4V(nameGenJet.c_str())->at(i).Pt()>35){
     
     hPtF.Fill(reader.Get4V(nameGenJet.c_str())->at(i).Pt());	//riempio l'istogramma
     hPtF_rebinned.Fill(reader.Get4V(nameGenJet.c_str())->at(i).Pt());

     EtaPtF_had.Fill(reader.Get4V(nameGenJet.c_str())->at(i).Eta(), reader.Get4V(nameGenJet.c_str())->at(i).Pt());	//riempio l'istogramma

     //qui devo impostare tutte le variabili del tree!
     //Variabili at hadron level
     G_FJet_Pt = reader.Get4V(nameGenJet.c_str())->at(i).Pt();
     G_FJet_Eta = reader.Get4V(nameGenJet.c_str())->at(i).Eta();
     G_FJet_Phi = reader.Get4V(nameGenJet.c_str())->at(i).Phi();
     G_FJet_Energy = reader.Get4V(nameGenJet.c_str())->at(i).energy();
     
     
   
     S_Mjj = (reader.Get4V(nameGenJet.c_str())->at(i) + reader.Get4V(nameGenJet.c_str())->at(i)).mass();      
    }    
   }
 }
   
 return 0;
   
 }

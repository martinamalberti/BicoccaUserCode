#include "ConfigParser.h"


#include "ECALAlignmentFramework.h"


#include "TPie.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMinuit.h"
#include "THStack.h"

#include "../test/TDRStyle.cc"
#include "../test/Read.cc"
// #include "../test/DrawTools.h"

#include "PUclass.h"





#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif

///
///
///   ___|                                       _)                           \  |   ___|         /      __ \     \  __ __|   \    
///  |       _ \   __ `__ \   __ \    _` |   __|  |   __|   _ \   __ \       |\/ |  |            /       |   |   _ \    |    _ \   
///  |      (   |  |   |   |  |   |  (   |  |     | \__ \  (   |  |   |      |   |  |           /        |   |  ___ \   |   ___ \  
/// \____| \___/  _|  _|  _|  .__/  \__,_| _|    _| ____/ \___/  _|  _|     _|  _| \____|     _/        ____/ _/    _\ _| _/    _\ 
///                          _|                                                                                                    
///
///

// std::vector<double> PUWeight;

int GetNumList(std::vector<int> &list){
 int result = 0;
 for (int it = 0; it<list.size(); it++) result += list.at(it);
 return result;
}


int main(int argc, char** argv)
{ 
 TDRStyle();
 
 gStyle->SetPadTopMargin(0.2);
 gStyle->SetPadBottomMargin(0.2);
 gStyle->SetPadLeftMargin(0.07);
 gStyle->SetPadRightMargin(0.23);
 gStyle->cd(); 
 
 
 std::cout << " " << std::endl;
 std::cout << " " << std::endl;
 std::cout << "   ___|                                       _)                           \\  |   ___|         /      __ \\     \\  __ __|   \\     " << std::endl;
 std::cout << "  |       _ \\   __ `__ \\   __ \\    _` |   __|  |   __|   _ \\   __ \\       |\\/ |  |            /       |   |   _ \\    |    _ \\    " << std::endl;
 std::cout << "  |      (   |  |   |   |  |   |  (   |  |     | \\__ \\  (   |  |   |      |   |  |           /        |   |  ___ \\   |   ___ \\   " << std::endl;
 std::cout << " \\____| \\___/  _|  _|  _|  .__/  \\__,_| _|    _| ____/ \\___/  _|  _|     _|  _| \\____|     _/        ____/ _/    _\\ _| _/    _\\  " << std::endl;
 std::cout << "                          _|                                                                                                     " << std::endl;
 std::cout << " " << std::endl;
 
 char normal[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };
 char black[] = { 0x1b, '[', '0', ';', '3', '0', 'm', 0 };
 char red[] = { 0x1b, '[', '0', ';', '3', '1', 'm', 0 };
 char green[] = { 0x1b, '[', '0', ';', '3', '2', 'm', 0 };
 char yellow[] = { 0x1b, '[', '0', ';', '3', '3', 'm', 0 };
 char blue[] = { 0x1b, '[', '0', ';', '3', '4', 'm', 0 };
 char purple[] = { 0x1b, '[', '0', ';', '3', '5', 'm', 0 };
 char cyan[] = { 0x1b, '[', '0', ';', '3', '6', 'm', 0 };
 char Lgray[] = { 0x1b, '[', '0', ';', '3', '7', 'm', 0 };
 char Dgray[] = { 0x1b, '[', '0', ';', '3', '8', 'm', 0 };
 char Bred[] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
 //for bold colors, just change the 0 after the [ to a 1
 
 EColor vColor[1000] = {
  kGreen,
  //kMagenta,(EColor) (kMagenta+1),(EColor) (kMagenta+2),
  kTeal,//(EColor) (kTeal+1),
  kRed,
  kBlack,
  kGray,
  kOrange,(EColor) (kOrange+1),
  kBlue,//(EColor)(kBlue+1),(EColor) (kBlue+2),
  (EColor) (kPink+2),//(EColor) (kPink+1),(EColor) (kPink+2),
  kViolet,
  kYellow,
  kGray,(EColor) (kGray+1),(EColor) (kViolet),(EColor) (kYellow),(EColor) (kGray),
  (EColor) (kPink+1),(EColor) (kPink+2),(EColor) (kPink+3),(EColor) (kPink+4),(EColor) (kPink+5), (EColor) (kPink+6),(EColor) (kPink+7),(EColor) (kPink+8),(EColor) (kPink+9),(EColor) (kPink+10),(EColor) (kPink+11), (EColor) (kPink+12),(EColor) (kPink+13),(EColor) (kPink+14),(EColor) (kPink+15),(EColor) (kPink+16),(EColor) (kPink+17),(EColor) (kPink+18),(EColor) (kPink+19),(EColor) (kPink+20),
  (EColor) (kTeal+1),(EColor) (kTeal+2),(EColor) (kTeal+3),(EColor) (kTeal+4),(EColor) (kTeal+5), (EColor) (kTeal+6),(EColor) (kTeal+7),(EColor) (kTeal+8),(EColor) (kTeal+9),(EColor) (kTeal+10),(EColor) (kTeal+11), (EColor) (kTeal+12),(EColor) (kTeal+13),(EColor) (kTeal+14),(EColor) (kTeal+15),(EColor) (kTeal+16),(EColor) (kTeal+17),(EColor) (kTeal+18),(EColor) (kTeal+19),(EColor) (kTeal+20),
  (EColor) (kGreen+1),(EColor) (kGreen+2),(EColor) (kGreen+3),(EColor) (kGreen+4),(EColor) (kGreen+5), (EColor) (kGreen+6),(EColor) (kGreen+7),(EColor) (kGreen+8),(EColor) (kGreen+9),(EColor) (kGreen+10),(EColor) (kGreen+11), (EColor) (kGreen+12),(EColor) (kGreen+13),(EColor) (kGreen+14),(EColor) (kGreen+15),(EColor) (kGreen+16),(EColor) (kGreen+17),(EColor) (kGreen+18),(EColor) (kGreen+19),(EColor) (kGreen+20),
  (EColor) (kYellow+1),(EColor) (kYellow+2),(EColor) (kYellow+3),(EColor) (kYellow+4),(EColor) (kYellow+5), (EColor) (kYellow+6),(EColor) (kYellow+7),(EColor) (kYellow+8),(EColor) (kYellow+9),(EColor) (kYellow+10),(EColor) (kYellow+11), (EColor) (kYellow+12),(EColor) (kYellow+13),(EColor) (kYellow+14),(EColor) (kYellow+15),(EColor) (kYellow+16),(EColor) (kYellow+17),(EColor) (kYellow+18),(EColor) (kYellow+19),(EColor) (kYellow+20),
  (EColor) (kBlue+1),(EColor) (kBlue+2),(EColor) (kBlue+3),(EColor) (kBlue+4),(EColor) (kBlue+5), (EColor) (kBlue+6),(EColor) (kBlue+7),(EColor) (kBlue+8),(EColor) (kBlue+9),(EColor) (kBlue+10),(EColor) (kBlue+11), (EColor) (kBlue+12),(EColor) (kBlue+13),(EColor) (kBlue+14),(EColor) (kBlue+15),(EColor) (kBlue+16),(EColor) (kBlue+17),(EColor) (kBlue+18),(EColor) (kBlue+19),(EColor) (kBlue+20),
  (EColor) (kOrange+1),(EColor) (kOrange+2),(EColor) (kOrange+3),(EColor) (kOrange+4),(EColor) (kOrange+5), (EColor) (kOrange+6),(EColor) (kOrange+7),(EColor) (kOrange+8),(EColor) (kOrange+9),(EColor) (kOrange+10),(EColor) (kOrange+11), (EColor) (kOrange+12),(EColor) (kOrange+13),(EColor) (kOrange+14),(EColor) (kOrange+15),(EColor) (kOrange+16),(EColor) (kOrange+17),(EColor) (kOrange+18),(EColor) (kOrange+19),(EColor) (kOrange+20),
  (EColor) (kViolet+1),(EColor) (kViolet+2),(EColor) (kViolet+3),(EColor) (kViolet+4),(EColor) (kViolet+5), (EColor) (kViolet+6),(EColor) (kViolet+7),(EColor) (kViolet+8),(EColor) (kViolet+9),(EColor) (kViolet+10),(EColor) (kViolet+11), (EColor) (kViolet+12),(EColor) (kViolet+13),(EColor) (kViolet+14),(EColor) (kViolet+15),(EColor) (kViolet+16),(EColor) (kViolet+17),(EColor) (kViolet+18),(EColor) (kViolet+19),(EColor) (kViolet+20),
  (EColor) (kGray+1),(EColor) (kGray+2),(EColor) (kGray+3),(EColor) (kGray+4),(EColor) (kGray+5), (EColor) (kGray+6),(EColor) (kGray+7),(EColor) (kGray+8),(EColor) (kGray+9),(EColor) (kGray+10),(EColor) (kGray+11), (EColor) (kGray+12),(EColor) (kGray+13),(EColor) (kGray+14),(EColor) (kGray+15),(EColor) (kGray+16),(EColor) (kGray+17),(EColor) (kGray+18),(EColor) (kGray+19),(EColor) (kGray+20),
  (EColor) (kMagenta+1),(EColor) (kMagenta+2),(EColor) (kMagenta+3),(EColor) (kMagenta+4),(EColor) (kMagenta+5), (EColor) (kMagenta+6),(EColor) (kMagenta+7),(EColor) (kMagenta+8),(EColor) (kMagenta+9),(EColor) (kMagenta+10),(EColor) (kMagenta+11), (EColor) (kMagenta+12),(EColor) (kMagenta+13),(EColor) (kMagenta+14),(EColor) (kMagenta+15),(EColor) (kMagenta+16),(EColor) (kMagenta+17),(EColor) (kMagenta+18),(EColor) (kMagenta+19),(EColor) (kMagenta+20),
  (EColor) (kBlack+1),(EColor) (kBlack+2),(EColor) (kBlack+3),(EColor) (kBlack+4),(EColor) (kBlack+5), (EColor) (kBlack+6),(EColor) (kBlack+7),(EColor) (kBlack+8),(EColor) (kBlack+9),(EColor) (kBlack+10),(EColor) (kBlack+11), (EColor) (kBlack+12),(EColor) (kBlack+13),(EColor) (kBlack+14),(EColor) (kBlack+15),(EColor) (kBlack+16),(EColor) (kBlack+17),(EColor) (kBlack+18),(EColor) (kBlack+19),(EColor) (kBlack+20)
 };
 
 
 
 
 //Check if all nedeed arguments to parse are there                                                                                                                               
 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }


 // Parse the config file                                                                                                                                                          
 parseConfigFile (argv[1]) ;
 
 std::string treeName  = gConfigParser -> readStringOption("Input::treeName");
 std::string fileSamples = gConfigParser -> readStringOption("Input::fileSamples");
 std::string inputDirectory = gConfigParser -> readStringOption("Input::inputDirectory");
 std::string inputRootName  = gConfigParser -> readStringOption("Input::inputRootName");
 
 
 double LUMI = gConfigParser -> readDoubleOption("Input::Lumi");
 
 double LumiSyst = gConfigParser -> readDoubleOption("Input::LumiSyst");
 
 double Discovery = gConfigParser -> readDoubleOption("Input::Discovery");
 
 int Normalize = gConfigParser -> readIntOption("Input::Normalize");
 
 
 std::vector<std::string> SignalName;
 if (Discovery == 1) SignalName = gConfigParser -> readStringListOption("Input::SignalName");
 
 ///==== PU reweight (begin) ====
 std::vector<double> PUMC   = gConfigParser -> readDoubleListOption("PU::PUMC");
 std::vector<double> PUDATA = gConfigParser -> readDoubleListOption("PU::PUDATA");
 PUclass PU;
 
 std::cout << " PUMC.size()   = " << PUMC.size()   << std::endl;
 std::cout << " PUDATA.size() = " << PUDATA.size() << std::endl;
 
 if (PUMC.size() != PUDATA.size()) {
  std::cerr << " ERROR " << std::endl;
  return 1;
 }
 
 double sumPUMC = 0;
 for (int itVPU = 0; itVPU < PUMC.size(); itVPU++ ){
  sumPUMC += PUMC.at(itVPU);  
 }
 double sumPUDATA = 0;
 for (int itVPU = 0; itVPU < PUDATA.size(); itVPU++ ){
  sumPUDATA += PUDATA.at(itVPU);  
 } 
 
 for (int itVPU = 0; itVPU < PUMC.size(); itVPU++ ){
  PU.PUWeight.push_back(PUDATA.at(itVPU) / PUMC.at(itVPU) * sumPUMC / sumPUDATA);
 }

 PU.Write("autoWeight.cxx");
 gROOT->ProcessLine(".L autoWeight.cxx");
 ///==== PU reweight (end) ====
 
 ///==== save PU distribution in TH1F ====
 TH1F* hPUMC   = new TH1F("hPUMC","hPUMC",PUMC.size(),0,PUMC.size());
 TH1F* hPUDATA = new TH1F("hPUDATA","hPUDATA",PUDATA.size(),0,PUDATA.size());
 TH1F* hPUWeight = new TH1F("hPUWeight","hPUWeight",PUDATA.size(),0,PUDATA.size());
 
 for (int itVPU = 0; itVPU < PUMC.size(); itVPU++ ){
  hPUMC     -> SetBinContent(itVPU+1,PUMC.at(itVPU) / sumPUMC);
  hPUDATA   -> SetBinContent(itVPU+1,PUDATA.at(itVPU) / sumPUDATA);
  hPUWeight -> SetBinContent(itVPU+1,PUDATA.at(itVPU) / PUMC.at(itVPU) * sumPUMC / sumPUDATA);
 }
 
 
 
 std::vector<std::string> DataName;
 DataName = gConfigParser -> readStringListOption("Input::DataName");
 std::vector<int> numDataName;

 for (uint iDataName=0; iDataName < DataName.size(); iDataName++){
  std::cout << " DATA[" << iDataName << "] = " << DataName.at(iDataName) << std::endl;
 }
 
 
 TTree *treeJetLepVect[100]; 
 TH1F*  MyHistoCounterMC[100];
 

 
  //  [iCut][iVar] 
 TString* infoString[192][11];
 TLatex *infoLatex[192][11]; 
 TCanvas* ccCanvas[192][11];
 TCanvas* ccCanvasPull[192][11];
 TH1F* histoSumMC[192][11];
 //  [iName][iCut][iVar]
 TH1F* histo[22][192][11];
 TH1F* histo_temp[22][192][11];

 //  [iName][iCut]
 double numEvents[22][192];
 
 char *nameSample[1000];
 char *nameHumanReadable[1000];
 char* xsectionName[1000];
 
 double Normalization[1000];
 double xsection[1000];
 char nameFileIn[1000];
 sprintf(nameFileIn,"%s",fileSamples.c_str());

 int numberOfSamples = ReadFile(nameFileIn, nameSample, nameHumanReadable, xsectionName); 
 
 ///==== list of variables to plot ====
 std::vector<double> vMin;
 std::vector<double> vMax;
 std::vector<int> vNBin;
 std::vector<std::string> vVarName;
 std::vector<std::string> vVarNameHR;
 std::string VarFile = gConfigParser -> readStringOption("Plot::VarFile");
 
 int numVar = ReadFileVar(VarFile,vMin,vMax,vNBin,vVarName,vVarNameHR);
  
 
 double XSection  = gConfigParser -> readDoubleOption("Plot::XSection");
 
 ///==== list of selections to perform (NOT sequential additive selections) ====
 std::string CutFile = gConfigParser -> readStringOption("Selections::CutFile");

 std::string CutHRFile = "";
 try {
  CutHRFile = gConfigParser -> readStringOption("Selections::CutHRFile");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 
 std::vector<std::string> vCut;
 std::vector<std::string> vCutHR;
 
 std::cout << " nCuts   = " << ReadFileCut(CutFile, vCut) << std::endl;
 if (CutHRFile != "") {
  std::cout << " nCutsHR = " << ReadFileCutHR(CutHRFile, vCutHR) << std::endl;
 }
 
 
 
 if (vCutHR.size() < vCut.size()) {
  int size1 = vCut.size();
  int size2 = vCutHR.size();
  for (int i=0; i<(size1-size2+2); i++) {
   vCutHR.push_back("test");
  }
 }
 
 
 ///==== output file ====
 std::string OutFileName    = gConfigParser -> readStringOption("Output::outFileName");
 std::cout << ">>>>> Output::outFileName  " << OutFileName  << std::endl;  
 
 TFile outFile(OutFileName.c_str(),"RECREATE");
 outFile.cd();
 
 
 ///==== debug flag ====
 
 bool  debug = false; 
 try {
  debug = gConfigParser -> readBoolOption("Input::debug");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 std::cout << ">>>>> input::debug  " << debug  << std::endl;  
 
 ///==== program ====
 
 
 
 double start, end;
 start = clock();
 
 
 for (int iSample=0; iSample<numberOfSamples; iSample++){
  xsection[iSample] = atof(xsectionName[iSample]);
 }
 
 for (int iSample=0; iSample<numberOfSamples; iSample++){
  if (debug) std::cout << "iSample = " << iSample << " : " << numberOfSamples << " :: " << nameHumanReadable[iSample] << std::endl;
  
  
  char nameFile[20000];
  sprintf(nameFile,"%s/%s_%s.root",inputDirectory.c_str(),inputRootName.c_str(),nameSample[iSample]);  
  TFile* f = new TFile(nameFile, "READ");
  
  MyHistoCounterMC[iSample] = (TH1F*) f->Get("AllEvents/totalEvents");
  char nameHistoJetLep[100];
  sprintf(nameHistoJetLep,"histo_%d",iSample); 
  MyHistoCounterMC[iSample]->SetName(nameHistoJetLep);
  
  treeJetLepVect[iSample] = (TTree*) f->Get(treeName.c_str());
  char nameTreeJetLep[100];
  sprintf(nameTreeJetLep,"treeJetLep_%d",iSample); 
  treeJetLepVect[iSample]->SetName(nameTreeJetLep);
 }
 
 ///===== create map for joint sample ====
 
 std::vector<int> join_samples;
 std::vector<std::string> name_samples;
 for (int iSample=0; iSample<numberOfSamples; iSample++){
  name_samples.push_back(nameHumanReadable[iSample]);
  join_samples.push_back(-1);
 }
 
 std::vector<std::string> reduced_name_samples;
 std::vector<int>         reduced_name_samples_flag;
 for (int iSample = (numberOfSamples-1); iSample>= 0; iSample--){
  bool flag_name = false;
  for (uint iName=0; iName<reduced_name_samples.size(); iName++){
   if (reduced_name_samples.at(iName) == name_samples.at(iSample)) flag_name = true;
  }
  if (flag_name == false) {
   reduced_name_samples.push_back(name_samples.at(iSample));
   reduced_name_samples_flag.push_back(-1);
  }
 }
 
 std::cout << " numberOfSamples = " << numberOfSamples << std::endl;
 
 for (int iSample = (numberOfSamples-1); iSample>= 0; iSample--){
  int numEntriesBefore;
  double preselection_efficiency = 1;
  
  XSection = 100;
  
  numEntriesBefore = MyHistoCounterMC[iSample]->GetBinContent(1);
  std::cout << " Xsection = " << XSection << " ~~~> " << xsection[iSample] << " ::: " << name_samples.at(iSample) << std::endl;
  XSection = xsection[iSample];
  
  if (numEntriesBefore != 0) {
   Normalization[iSample] = LUMI * XSection * preselection_efficiency / numEntriesBefore;
  }
  else {
   Normalization[iSample] = 0; 
  }    
 }

 
 TLegend* leg = new TLegend(0.8,0.25,0.98,0.78);
 bool LegendBuilt = false;

 TString lumiName = Form("#splitline{L = %.1f pb^{-1}}{#splitline{#sqrt{s} = 7}{CMS preliminary}}", LUMI);
//  TString lumiName = Form("#sqrt{s}=7 TeV   L=%.1f pb^{-1}", LUMI);
 TLatex *latex = new TLatex(0.80, 0.90, lumiName); 
 latex->SetTextAlign(12);
 latex->SetNDC();
 latex->SetTextFont(42);
 latex->SetTextSize(0.03);
  
 ///==== get number in sample list that correspond to DATA ====
 for (uint iDataName=0; iDataName < DataName.size(); iDataName++){
  for (uint iName=0; iName<reduced_name_samples.size(); iName++){
   if (reduced_name_samples.at(iName) == DataName.at(iDataName)) {
    numDataName.push_back(iName);
   }
  }
 }
 
 std::cout << " numDataName.size() = " << numDataName.size() << " DataName.size() = " << DataName.size() << std::endl;
 
 if (numDataName.size() != DataName.size()) {
  std::cout << " ERROR " << std::endl;
  std::cout << " different list of data samples " << std::endl;
  return 1;
 }
 
 std::cerr << std::endl;
 std::cerr << std::endl;
 for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
  std::cerr << " numDataName[" << inumDataName << "] = " << numDataName.at(inumDataName) << " ~~~> " << reduced_name_samples.at(numDataName.at(inumDataName)) << std::endl;
 }
 std::cerr << std::endl;
 std::cerr << std::endl;
 
 if (debug) std::cout << " Cut size = " << vCut.size() << " ~~ " << std::endl;
 std::cout.precision (2) ;
 
 ///==== cicle on selections ====
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  TString Cut = Form ("%s",vCut.at(iCut).c_str());
  if (debug) std::cout << " Cut[" << iCut << ":" << vCut.size() << "] = " << Cut.Data() << " ~~ " << std::endl;
  ///==== cicle on variables to plot ====
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   if (debug) std::cout << " Var[" << iVar << ":" << vVarName.size() << "] = " << vVarName.at(iVar) << " ~~ " << std::endl;   
   ///==== cicle on samples ====
   for (int iSample = (numberOfSamples-1); iSample>= 0; iSample--){
    if (debug) std::cout << " Sample[" << iSample << ":" << numberOfSamples << "] = " << nameSample[iSample] << " ~~ " << std::endl;
    TString name_histo_temp = Form("%s_%d_%d_temp",nameSample[iSample], iCut, iVar);
    histo_temp[iSample][iCut][iVar] = new TH1F(name_histo_temp,name_histo_temp,vNBin.at(iVar),vMin.at(iVar), vMax.at(iVar));
    char toDraw[1000];
    sprintf(toDraw,"%s >> %s",vVarName.at(iVar).c_str(),name_histo_temp.Data());      

    histo_temp[iSample][iCut][iVar] -> Sumw2(); //---- così mette l'errore giusto!
    
    TString CutExtended;
    bool isData = false;
    for (uint iName=0; iName<reduced_name_samples.size(); iName++){
     if (name_samples.at(iSample) == reduced_name_samples.at(iName)){
      for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
       if (numDataName.at(inumDataName) == iName) {
	isData = true;
       }
      }
     }
    } 
    if (!isData) {
     CutExtended = Form ("(%s) * autoWeight(numPUMC)",Cut.Data());    
    }
    else {
     CutExtended = Form ("(%s)",Cut.Data());    
    }
    
//     if (iSample != numDATA) {
//      CutExtended = Form ("(%s)",Cut.Data());    //FIXME !!!
//      CutExtended = Form ("(%s) * autoWeight(numPUMC)",Cut.Data());    
//     }
//     else {
//      CutExtended = Form ("(%s)",Cut.Data());    
//     }
     
    treeJetLepVect[iSample]->Draw(toDraw,CutExtended,"");
    
    if (Normalization[iSample]>0) { 
//      histo_temp[iSample][iCut][iVar] -> Sumw2();
     histo_temp[iSample][iCut][iVar] -> Scale(Normalization[iSample]); 
     if (debug) std::cerr << " ... scaling ..." << std::endl;
    }
//     std::cout << "Processing: " << blue << (((double) numberOfSamples - iSample)/numberOfSamples) << "% \r"  << normal << std::flush;
   } ///==== end cicle on samples ====
//    std::cout << "###";
  std::cout << "Processing: " << blue << (((double) iCut)/vCut.size())*100. << "% "  << normal <<  " -- " <<  blue << (((double) iVar)/vVarName.size())*100. << "% \r"  << normal << std::flush;   
  } ///==== end cicle on variables to plot ====
  //   std::cout << "***";
 } ///==== end cicle on selections ====
 std::cout << std::endl; 
 
 
 
 
 ///---- create "big" histos ----
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   ///---- initialize (begin) ----
   for (uint iName=0; iName<reduced_name_samples.size(); iName++){
    reduced_name_samples_flag.at(iName) = -1;
   }
   ///---- initialize (end) ----
   for (int iSample = (numberOfSamples-1); iSample>= 0; iSample--){    
    for (uint iName=0; iName<reduced_name_samples.size(); iName++){
     bool isSig = false;
     for (uint inumDataName=0; inumDataName < 1; inumDataName++){ //---- only the first here!
      if (numDataName.at(inumDataName) == iName) {
       isSig = true;
      }
     }
     if (name_samples.at(iSample) == reduced_name_samples.at(iName) && (isSig)){
      if (reduced_name_samples_flag.at(iName) == -1){
       TString name_histoTot_temp = Form("%s_%d_%d_Tot_temp",reduced_name_samples.at(iName).c_str(),iCut, iVar);
       if (debug) std::cerr << " Creating : " << name_histoTot_temp.Data() << std::endl;
       TString name_HR_histoTot_temp = Form("%s %s %s",vVarNameHR.at(iVar).c_str(), vCutHR.at(iCut).c_str(), reduced_name_samples.at(iName).c_str());
       //        TString name_HR_histoTot_temp = Form("%s %d %s",vVarNameHR.at(iVar).c_str(), iCut, reduced_name_samples.at(iName).c_str());
       histo[iName][iCut][iVar] = new TH1F(name_histoTot_temp,name_HR_histoTot_temp,vNBin.at(iVar),vMin.at(iVar), vMax.at(iVar));
       histo[iName][iCut][iVar] -> Sumw2(); //---- così mette l'errore giusto!
       reduced_name_samples_flag.at(iName) = 1;
      }
      histo[iName][iCut][iVar] -> Add(histo_temp[iSample][iCut][iVar]);
      if (debug) std::cerr << " histo[" << reduced_name_samples.at(iName) << "," << iName << "][" << iCut << "][" << iVar <<"].Integral() = " << histo[iName][iCut][iVar]->Integral() << std::endl;
     }
    }
   }
  }
 }
 
 
 //---- normalize to Data ----
 if (Normalize == 1) {
  std::cout << ">>> Normalize to Data <<<" << std::endl;
  for (uint iCut = 0; iCut<vCut.size(); iCut++){
   for (uint iVar = 0; iVar<vVarName.size(); iVar++){
    ///---- initialize (begin) ----
    for (uint iName=0; iName<reduced_name_samples.size(); iName++){
     reduced_name_samples_flag.at(iName) = -1;
    }
    ///---- initialize (end) ----
    double data_int = histo[numDataName.at(0)][iCut][iVar] -> Integral(); ///----> normalize to the first of data sample
    double mc_int = 0;
    if (debug) std::cout << " data_int  = " << data_int << std::endl;
    
    for (int iSample = (numberOfSamples-1); iSample>= 0; iSample--){
     for (uint iName=0; iName<reduced_name_samples.size(); iName++){     
      bool isSig = false;
      for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
       if (numDataName.at(inumDataName) == iName) {
	isSig = true;
       }
      }
      if (name_samples.at(iSample) == reduced_name_samples.at(iName) && (!isSig)){
       mc_int += histo_temp[iSample][iCut][iVar] -> Integral();
      }
     }
    }
    
    if (debug) std::cout << " data_int / mc_int = " << data_int / mc_int << std::endl;
    for (int iSample = (numberOfSamples-1); iSample>= 0; iSample--){
     bool isSig = false;
     int numSig = -1;
     for (uint iName=0; iName<reduced_name_samples.size(); iName++){ 
      if (name_samples.at(iSample) == reduced_name_samples.at(iName)){
       for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
	if (numDataName.at(inumDataName) == iName) {
	 isSig = true;
	 numSig = inumDataName;
	}
       }
      }
     }
     
     if (!isSig) {
      if (debug) std::cout << "histo_temp[" << iSample <<"] = " << name_samples.at(iSample) << std::endl;
      histo_temp[iSample][iCut][iVar] -> Scale (data_int / mc_int);
     }
     else {
      if (debug) std::cerr << " >>> numSig = " << numSig << std::endl;
      if (numSig != 0) {
       if (debug) std::cerr << " histo[" << numDataName.at(0) << "][" << iCut << "][" << iVar << "] -> Integral() = " <<  histo[numDataName.at(0)][iCut][iVar] -> Integral() << std::endl;
       if (debug) std::cerr << " numDataName.at(" << numSig << ") = " << numDataName.at(numSig) << " ~~> " << DataName.at(numSig) << " !!== " << numDataName.at(0) << " ~ " << numSig << std::endl;
       if (debug) std::cerr << " histo_temp[" << iSample << "][" << iCut << "][" << iVar << "] -> Integral() = " <<  histo_temp[iSample][iCut][iVar] -> Integral() << std::endl;
       double temp_integral = histo_temp[iSample][iCut][iVar] -> Integral();
       histo_temp[iSample][iCut][iVar] -> Scale (data_int / temp_integral);
       if (debug) std::cerr << " ~~~ >  histo_temp[" << iSample << "][" << iCut << "][" << iVar << "] -> Integral() = " <<  histo_temp[iSample][iCut][iVar] -> Integral() << std::endl;
       if (debug) std::cerr << "        histo_temp[" << iSample << "][" << iCut << "][" << iVar << "] -> GetName() = " <<  histo_temp[iSample][iCut][iVar] -> GetName() << std::endl;
       
      }
     }
     
     for (uint iName=0; iName<reduced_name_samples.size(); iName++){     
      if (name_samples.at(iSample) == reduced_name_samples.at(iName)  && (numSig != 0)){
       if (reduced_name_samples_flag.at(iName) == -1){
	TString name_histoTot_temp = Form("%s_%d_%d_Tot_temp",reduced_name_samples.at(iName).c_str(),iCut, iVar);
	TString name_HR_histoTot_temp = Form("%s %s",vVarNameHR.at(iVar).c_str(), vCutHR.at(iCut).c_str());
	//        TString name_HR_histoTot_temp = Form("%s %d %s",vVarNameHR.at(iVar).c_str(), iCut, reduced_name_samples.at(iName).c_str());
	histo[iName][iCut][iVar] = new TH1F(name_histoTot_temp,name_HR_histoTot_temp,vNBin.at(iVar),vMin.at(iVar), vMax.at(iVar));
	histo[iName][iCut][iVar] -> Sumw2(); //---- così mette l'errore giusto!
	reduced_name_samples_flag.at(iName) = 1;
       }
       histo[iName][iCut][iVar] -> Add(histo_temp[iSample][iCut][iVar]);
      }
     }
    }
   }
  }
 }
 else { //---- normalize to Lumi
  std::cout << ">>> Normalize to Lumi <<<" << std::endl;
  for (uint iCut = 0; iCut<vCut.size(); iCut++){
   for (uint iVar = 0; iVar<vVarName.size(); iVar++){
    ///---- initialize (begin) ----
    for (uint iName=0; iName<reduced_name_samples.size(); iName++){
     reduced_name_samples_flag.at(iName) = -1;
    }
    ///---- initialize (end) ----
    for (int iSample = (numberOfSamples-1); iSample>= 0; iSample--){     
     for (uint iName=0; iName<reduced_name_samples.size(); iName++){
      int numSig = -1;
      bool isSig = false;
      for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
       if (numDataName.at(inumDataName) == iName) {
	isSig = true;
       }
      }

      if (name_samples.at(iSample) == reduced_name_samples.at(iName) && (numSig != 0) ){
       if (reduced_name_samples_flag.at(iName) == -1){
	TString name_histoTot_temp = Form("%s_%d_%d_Tot_temp",reduced_name_samples.at(iName).c_str(),iCut, iVar);
	TString name_HR_histoTot_temp = Form("%s %s",vVarNameHR.at(iVar).c_str(), vCutHR.at(iCut).c_str());
	//        TString name_HR_histoTot_temp = Form("%s %d %s",vVarNameHR.at(iVar).c_str(), iCut, reduced_name_samples.at(iName).c_str());
	histo[iName][iCut][iVar] = new TH1F(name_histoTot_temp,name_HR_histoTot_temp,vNBin.at(iVar),vMin.at(iVar), vMax.at(iVar));
	histo[iName][iCut][iVar] -> Sumw2(); //---- così mette l'errore giusto!
	reduced_name_samples_flag.at(iName) = 1;
       }
       histo[iName][iCut][iVar] -> Add(histo_temp[iSample][iCut][iVar]);
      }
     }
    }
   }
  }
 }

//  [iName]
 TH1F* hTrend[22];
 THStack* hsTrend;
 //  [iCut]
 TPie* hTrendPie[192];
 
 
 //  [iCut][iVar]
 THStack* hs[192][11];
 TH1F* hPull[192][11];
 TH1F* hPullTrace[192][11];
 
 std::cout << std::endl;
 
 ///==== cicle on selections ====
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  ///==== cicle on variables to plot ====
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   TString nameStack = Form("%d_%d_stack",iCut,iVar);
   hs[iCut][iVar] = new THStack(nameStack,nameStack);
   
   for (uint iName=0; iName<reduced_name_samples.size(); iName++){
    histo[iName][iCut][iVar]->GetXaxis()->SetTitle(vVarNameHR.at(iVar).c_str());
    histo[iName][iCut][iVar]->SetMarkerColor(vColor[iName]);
    histo[iName][iCut][iVar]->SetLineColor(vColor[iName]);
    histo[iName][iCut][iVar]->SetFillColor(vColor[iName]);
    histo[iName][iCut][iVar]->SetLineWidth(2);
    histo[iName][iCut][iVar]->SetFillStyle(3001);
    
    bool isSig = false;
    for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
     if (numDataName.at(inumDataName) == iName) {
      isSig = true;
     }
    }
    
    if (!isSig) {
     hs[iCut][iVar]->Add(histo[iName][iCut][iVar]);
    }
    else {
     histo[iName][iCut][iVar]->SetMarkerStyle(20);
     histo[iName][iCut][iVar]->SetMarkerSize(1);
     histo[iName][iCut][iVar]->SetLineWidth(2);
     histo[iName][iCut][iVar]->SetFillStyle(0);  
    }
   }
   ///==== histo sum MC ====    
   ///==== Add systrematic error ====
   AddError(hs[iCut][iVar],LumiSyst);
   histoSumMC[iCut][iVar] = ((TH1F*)(hs[iCut][iVar]->GetStack()->Last()));
   ///==== histo with pull plot ====
   hPull[iCut][iVar] = PullPlot(histo[numDataName.at(0)][iCut][iVar], histoSumMC[iCut][iVar]);
   hPullTrace[iCut][iVar] = GetTrendInfo(hPull[iCut][iVar],-1.5,1.5);
   
   
   std::cout << " MC / DATA[" << iCut << "][" << iVar << "]{" << DataName.at(0) << "," << histo[numDataName.at(0)][iCut][iVar]->GetName() << "} = "<< histoSumMC[iCut][iVar]->Integral() << " / " << histo[numDataName.at(0)][iCut][iVar]->Integral() << " = " << (histo[numDataName.at(0)][iCut][iVar]->Integral() ? histoSumMC[iCut][iVar]->Integral()/ histo[numDataName.at(0)][iCut][iVar]->Integral() : 0) << std::endl;
   if (debug) if (DataName.size() > 1) {std::cout << " MC / DATA[" << iCut << "][" << iVar << "]{" << DataName.at(1) << "," << histo[numDataName.at(1)][iCut][iVar]->GetName() << "} = "<< histoSumMC[iCut][iVar]->Integral() << " / " << histo[numDataName.at(1)][iCut][iVar]->Integral() << " = " << (histo[numDataName.at(0)][iCut][iVar]->Integral() ? histoSumMC[iCut][iVar]->Integral()/ histo[numDataName.at(1)][iCut][iVar]->Integral() : 0) << std::endl;}
   
   
   ///==== legend ====
   if (!LegendBuilt){
    for (uint iName=0; iName<reduced_name_samples.size(); iName++){
     leg->AddEntry(histo[iName][iCut][iVar],reduced_name_samples.at(iName).c_str(),"pf");    
     LegendBuilt = true;
    }
   }
  }
 }
 std::cout << std::endl << std::endl;
 
 ///==== calculate number of events after each step of the analysis ====
 //  [iName][iCut]
 hsTrend = new THStack("Trend","Trend");
 
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  TString nameTHTrendPie = Form("%d_Trend_Pie",iCut);
  hTrendPie[iCut] = new TPie (nameTHTrendPie,nameTHTrendPie,reduced_name_samples.size());
 }
 
 for (uint iName=0; iName<reduced_name_samples.size(); iName++){
  TString nameTHTrend = Form("%d_Trend",iName);
  hTrend[iName] = new TH1F (nameTHTrend,nameTHTrend,vCut.size()+1,0,vCut.size()+1);
  hTrend[iName]->GetXaxis()->SetTitle("Selections");

  bool isSig = false;
  for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
   if (numDataName.at(inumDataName) == iName) {
    isSig = true;
   }
  }
  
  if (isSig) {
   hTrend[iName]->SetMarkerStyle(20);
   hTrend[iName]->SetMarkerSize(1);
   hTrend[iName]->SetMarkerColor(kBlack);
   hTrend[iName]->SetLineColor(kBlack);
   hTrend[iName]->SetFillColor(kBlack);
   hTrend[iName]->SetLineWidth(2);
   hTrend[iName]->SetFillStyle(3001);  
  }
  else {
   hTrend[iName]->SetMarkerColor(vColor[iName]);
   hTrend[iName]->SetLineColor(vColor[iName]);
   hTrend[iName]->SetFillColor(vColor[iName]);
   hTrend[iName]->SetLineWidth(2);
   hTrend[iName]->SetFillStyle(3001);
  }
  
  for (uint iCut = 0; iCut<vCut.size(); iCut++){
   double error = 0;
   numEvents[iName][iCut] = histo[iName][iCut][0]->IntegralAndError(0,histo[iName][iCut][0]->GetNbinsX()+1,error); //--- iVar = 0, it should be the same whatever var you use
//    numEvents[iName][iCut] = histo[iName][iCut][0]->Integral(); //--- iVar = 0, it should be the same whatever var you use
   hTrend[iName]->SetBinContent(iCut+1,numEvents[iName][iCut]);
   hTrend[iName]->SetBinError(iCut+1,error);
   TString nameBin = Form("%d",iCut);
   
   if (vCutHR.size() == 0) {
    hTrend[iName]->GetXaxis()->SetBinLabel(iCut+1,nameBin);
   }
   else {
    hTrend[iName]->GetXaxis()->SetBinLabel(iCut+1,vCutHR.at(iCut).c_str());
   }
   
   
   
//     IntegralAndError
//     Double_t IntegralAndError(Int_t binx1, Int_t binx2, Double_t& err, Option_t* option = "") const
   std::cout << ">>>  numEvents[" << iName << "," << reduced_name_samples.at(iName) << "][" << iCut << "] = " << numEvents[iName][iCut] << " , " << histo[iName][iCut][0]->GetEntries() << " , " << histo[iName][iCut][0]->GetEffectiveEntries() << std::endl;
      
   if (!isSig) {
    hTrendPie[iCut]->SetTextSize(0.04);
    hTrendPie[iCut]->SetTextFont(12);
    hTrendPie[iCut]->SetEntryFillColor(iName,vColor[iName]);
    hTrendPie[iCut]->SetEntryFillStyle(iName,3001);
    hTrendPie[iCut]->SetEntryLabel(iName, reduced_name_samples.at(iName).c_str());
    hTrendPie[iCut]->SetEntryLineColor(iName, vColor[iName]);
    hTrendPie[iCut]->SetEntryLineStyle(iName, 2);
    hTrendPie[iCut]->SetEntryLineWidth(iName, 2);
    hTrendPie[iCut]->SetEntryRadiusOffset(iName, 0.01);
    hTrendPie[iCut]->SetEntryVal(iName,numEvents[iName][iCut]);
   }
   else {
    hTrendPie[iCut]->SetEntryLabel(iName, "");
//     hTrendPie[iCut]->SetEntryVal(iName,10e-1);
   }
  }
  if (!isSig) {
   hsTrend->Add(hTrend[iName]);
  }
 }
 AddError(hsTrend,LumiSyst);
 
 TH1F* hTrendSumMC = ((TH1F*)(hsTrend->GetStack()->Last()));
 ///==== hTrend with pull plot ====
 TH1F* hPullTrendSumMC = PullPlot(hTrend[numDataName.at(0)], hTrendSumMC);

 ///==== calculate agreement data-MC: Kolmogorov-Smirnov test ==== 
 ///==== cicle on selections ====
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  ///==== cicle on variables to plot ====
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   
   std::cerr << " iVar = " << iVar << " iCut = " << iCut << std::endl;
   
   std::string nameMeanRMSValue = "";
   
   /*
   std::string nameMeanRMSValue = "#splitline{";
   
   for (uint iDataName=1; iDataName < DataName.size(); iDataName++){
    nameMeanRMSValue += "#splitline{";
   }
   std::ostringstream strs;
   strs.precision (3) ;
   strs.unsetf(std::ios::scientific);
   
   for (uint iDataName=0; iDataName < DataName.size(); iDataName++){
    if (iDataName == 0) nameMeanRMSValue += "";
    else  nameMeanRMSValue += "{";
    nameMeanRMSValue += "";
    nameMeanRMSValue += DataName.at(iDataName);
    nameMeanRMSValue += " : ";
    strs.str("");
    if (debug) std::cerr << " numDataName.at(" << iDataName << ") = " << numDataName.at(iDataName) << " iCut = " << iCut << " iVar = " << iVar << std::endl;
    strs << histo[numDataName.at(iDataName)][iCut][iVar]->GetMean();
    nameMeanRMSValue += strs.str();
//     nameMeanRMSValue += histo[numDataName.at(iDataName)][iCut][iVar]->GetMean();
    nameMeanRMSValue += " #pm ";
    strs.str("");
    strs << histo[numDataName.at(iDataName)][iCut][iVar]->GetRMS() / sqrt(histo[numDataName.at(iDataName)][iCut][iVar]->GetEntries());
    nameMeanRMSValue += strs.str();
//     nameMeanRMSValue += histo[numDataName.at(iDataName)][iCut][iVar]->GetRMS();
    nameMeanRMSValue += "}";
    if (iDataName%2) nameMeanRMSValue += "}"; 
   }
   
   nameMeanRMSValue += "{";
   nameMeanRMSValue += "MC";
   nameMeanRMSValue += " : ";
   strs.str("");
   strs << histoSumMC[iCut][iVar]->GetMean();
//    nameMeanRMSValue += boost::lexical_cast<std::string>(histoSumMC[iCut][iVar]->GetMean());
   nameMeanRMSValue += strs.str();
   nameMeanRMSValue += " #pm ";
   strs.str("");
   strs << histoSumMC[iCut][iVar]->GetRMS()  / sqrt(histoSumMC[iCut][iVar]->GetEntries());
//    nameMeanRMSValue += histoSumMC[iCut][iVar]->GetRMS();
   nameMeanRMSValue += strs.str();
   nameMeanRMSValue += "}";
   */
   std::cerr << nameMeanRMSValue << std::endl;
   
   infoString[iCut][iVar] = new TString(Form("%s",nameMeanRMSValue.c_str()));
   infoLatex[iCut][iVar] = new TLatex(0.80, 0.10, *(infoString[iCut][iVar])); 
   infoLatex[iCut][iVar]->SetTextAlign(12);
   infoLatex[iCut][iVar]->SetNDC();
   infoLatex[iCut][iVar]->SetTextFont(42);
   infoLatex[iCut][iVar]->SetTextSize(0.03);
   
   
//    double KS = -1;
//    double Chi2 = -1;
//    if (histo[numDataName.at(0)][iCut][iVar]->GetEntries() != 0 && histoSumMC[iCut][iVar]->GetEntries() != 0) {
//     KS = histo[numDataName.at(0)][iCut][iVar]->KolmogorovTest(histoSumMC[iCut][iVar],"M");
//     Chi2 = histo[numDataName.at(0)][iCut][iVar]->Chi2Test(histoSumMC[iCut][iVar],"UW");
//    }
//    std::cout << " KS[" << iCut << "][" << iVar << "] = " << KS << std::endl;
//    infoString[iCut][iVar] = new TString(Form("#splitline{KS prob = %.4f}{#chi^{2} prob = %.4f}",KS,Chi2));
//    infoLatex[iCut][iVar] = new TLatex(0.80, 0.10, *(infoString[iCut][iVar])); 
//    infoLatex[iCut][iVar]->SetTextAlign(12);
//    infoLatex[iCut][iVar]->SetNDC();
//    infoLatex[iCut][iVar]->SetTextFont(42);
//    infoLatex[iCut][iVar]->SetTextSize(0.03);
  }
 }
 
 
 LumiSyst = 0; ///---- bug fix
 
 
 
 
 TCanvas* cTrendPie[100];
 TCanvas* cTrendPieAll = new TCanvas("cTrendPieAll","cTrendPieAll",400 * vCut.size(),400);
 cTrendPieAll -> Divide (vCut.size());
 TCanvas* cTrend = new TCanvas("cTrend","cTrend",400,400);
 TCanvas* cTrendPull = new TCanvas("cTrendPull","cTrendPull",400,800);
 
 TCanvas* cCompareCutPull[100];
 TCanvas* cCompareVarPull[100];
 
 TCanvas* cCompareCut[100];
 TCanvas* cCompareVar[100];
 
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  TString nameCanvas = Form("%d_Cut_Canvas",iCut);
  cCompareCut[iCut] = new TCanvas(nameCanvas,nameCanvas,400 * vVarName.size(),400);
  cCompareCut[iCut] -> Divide (vVarName.size(),1);
  TString nameCanvasPull = Form("%d_Cut_Canvas_Pull",iCut);
  cCompareCutPull[iCut] = new TCanvas(nameCanvasPull,nameCanvasPull,400 * vVarName.size(),400*2);
  cCompareCutPull[iCut] -> Divide (vVarName.size(),2);
 }
 
 for (uint iVar = 0; iVar<vVarName.size(); iVar++){ ///==== cicle on variables to plot ====
   TString nameCanvas = Form("%d_Var_Canvas",iVar);
   cCompareVar[iVar] = new TCanvas(nameCanvas,nameCanvas,400,400 * vCut.size());
   cCompareVar[iVar] -> Divide (1,vCut.size());
   TString nameCanvasPull = Form("%d_Var_Canvas_Pull",iVar);
   cCompareVarPull[iVar] = new TCanvas(nameCanvasPull,nameCanvasPull,400*3,400 * vCut.size());
   cCompareVarPull[iVar] -> Divide (3,vCut.size());
 }
 
 
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   TString nameCanvas = Form("%d_%d_Canvas",iCut,iVar);
   ccCanvas[iCut][iVar] = new TCanvas(nameCanvas,nameCanvas,400,400);
   TString nameCanvasPull = Form("%d_%d_CanvasPull",iCut,iVar);
   ccCanvasPull[iCut][iVar] = new TCanvas(nameCanvasPull,nameCanvasPull,400*2,400);
  }
 } 
 
 
 ///==== draw trend vs cut (begin)
 cTrend->cd();
 DrawStack(hsTrend,1,LumiSyst);
 for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
  hTrend[numDataName.at(inumDataName)] -> Draw("EsameP");
 }
 gPad->SetLogy();
 gPad->SetGrid();
 leg->Draw();
 latex->Draw();
 
 
 cTrendPull->Divide(1,2);
 cTrendPull->cd(1);
 DrawStack(hsTrend,1,LumiSyst);
 for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
  hTrend[numDataName.at(inumDataName)] -> Draw("EsameP");
 }
 gPad->SetLogy();
 gPad->SetGrid();
 leg->Draw();
 latex->Draw();
 cTrendPull->cd(2);
 hPullTrendSumMC->Draw("EP");
 gPad->SetGrid();
 
 
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  TString nameCanvas = Form("%d_Canvas_Trend",iCut);
  cTrendPie[iCut] = new TCanvas(nameCanvas,nameCanvas,400,400);
  cTrendPie[iCut]->cd();
  hTrendPie[iCut] -> Draw("3d t nol");
  hTrendPie[iCut]->SetX(.45);
  hTrendPie[iCut]->SetRadius(.22);
  leg->Draw();
  latex->Draw();
  
  cTrendPieAll->cd(iCut+1);
  hTrendPie[iCut] -> Draw("3d t nol");
  hTrendPie[iCut]->SetX(.45);
  hTrendPie[iCut]->SetRadius(.22);
  leg->Draw();
  latex->Draw();
 }
 ///==== draw trend vs cut (end)
 
 
 ///==== cicle on selections ====
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  ///==== cicle on variables to plot ====
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   ///==== draw in canvas ====
   cCompareCut[iCut] -> cd(iVar+1);
   DrawStack(hs[iCut][iVar],1,LumiSyst);
   gPad->SetLogy();
   gPad->SetGrid();
   leg->Draw();
   latex->Draw();
   infoLatex[iCut][iVar]->Draw();
   
   cCompareVar[iVar] -> cd(iCut+1);
   DrawStack(hs[iCut][iVar],1,LumiSyst);
   gPad->SetLogy();
   gPad->SetGrid();
   leg->Draw();
   latex->Draw();
   infoLatex[iCut][iVar]->Draw();
   
   
   cCompareCutPull[iCut] -> cd(iVar+1);
   DrawStack(hs[iCut][iVar],1,LumiSyst);
   gPad->SetLogy();
   gPad->SetGrid();
   leg->Draw();
   latex->Draw();
   infoLatex[iCut][iVar]->Draw();
   
   cCompareVarPull[iVar] -> cd(iCut*3+1);
   DrawStack(hs[iCut][iVar],1,LumiSyst);
   gPad->SetLogy();
   gPad->SetGrid();
   leg->Draw();
   latex->Draw();
   infoLatex[iCut][iVar]->Draw();
   
   
   
   
   TVirtualPad* tempPad = cCompareCutPull[iCut] -> cd(iVar+1+vVarName.size());
   tempPad->Divide(2,1,0.01);
   tempPad->cd(1);
   hPull[iCut][iVar]->Draw("EP");
   gPad->SetLeftMargin(0.17);
   gPad->SetRightMargin(0.07);
   gPad->SetTopMargin(0.30);
   gPad->SetBottomMargin(0.30);
   gPad->SetGrid();
   tempPad->cd(2);
   hPullTrace[iCut][iVar]->Draw("hbar2");
   gPad->SetTopMargin(0.30);
   gPad->SetBottomMargin(0.30);
   gPad->SetGrid();
   
   
   
   
   cCompareVarPull[iVar] -> cd(iCut*3+2);
   hPull[iCut][iVar]->Draw("EP");
   gPad->SetGrid();
   gPad->SetLeftMargin(0.17);
   gPad->SetRightMargin(0.07);
   cCompareVarPull[iVar] -> cd(iCut*3+3);
   hPullTrace[iCut][iVar]->Draw("hbar2");
   gPad->SetGrid();
   
   
   ccCanvas[iCut][iVar]-> cd();
   DrawStack(hs[iCut][iVar],1,LumiSyst);
   gPad->SetLogy();
   gPad->SetGrid();
   leg->Draw();
   latex->Draw();
   infoLatex[iCut][iVar]->Draw();
   
   
   
   ccCanvasPull[iCut][iVar]-> cd();
   ccCanvasPull[iCut][iVar]-> Divide(2,1);
   ccCanvasPull[iCut][iVar]-> cd(1);
   hPull[iCut][iVar]->Draw("EP");
   gPad->SetGrid();
   ccCanvasPull[iCut][iVar]-> cd(1);
   hPullTrace[iCut][iVar]->Draw("hbar2");
   gPad->SetGrid();
   
//    gPad->SetLeftMargin(0.17);
//    gPad->SetRightMargin(0.07);
   
   
   
   for (uint iName=0; iName<reduced_name_samples.size(); iName++){
    
    bool isSig = false;
    for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
     if (numDataName.at(inumDataName) == iName) {
      isSig = true;
     }
    }
    
    if (isSig) {
     cCompareCut[iCut] -> cd(iVar+1);
     histo[iName][iCut][iVar]->Draw("EsameB");
     
     cCompareVar[iVar] -> cd(iCut+1);
     histo[iName][iCut][iVar]->Draw("EsameB");
     
     cCompareCutPull[iCut] -> cd(iVar+1);
     histo[iName][iCut][iVar]->Draw("EsameB");

     cCompareVarPull[iVar] -> cd(iCut*3+1);
     histo[iName][iCut][iVar]->Draw("EsameB");
     
     ccCanvas[iCut][iVar]-> cd();
     histo[iName][iCut][iVar]->Draw("EsameB");
    }
   }
   
   cCompareCut[iCut] -> cd(iVar+1);
   leg->Draw();
   cCompareVar[iVar] -> cd(iCut+1);
   leg->Draw();
   cCompareCutPull[iCut] -> cd(iVar+1);
   leg->Draw();
   cCompareVarPull[iVar] -> cd(iCut*3+1);
   leg->Draw();
   ccCanvas[iCut][iVar]-> cd();
   leg->Draw();
   
  } ///==== end cicle on variables to plot ====
 } ///==== end cicle on selections ====
 
 std::cerr << " ******************************************* end *******************************************" << std::endl;
 end = clock();
 std::cout <<"Time = " <<  ((double) (end - start)) << " (a.u.)" << std::endl;  
 
 
 
 ///==== save output ====
 outFile.cd();
 cTrend -> Write();
 cTrendPull -> Write();
 
 outFile.mkdir("Trend");
 outFile.cd("Trend");
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  cTrendPie[iCut] -> Write();
 }
 cTrendPieAll -> Write();
 
 
 outFile.cd();
 outFile.mkdir("PU");
 outFile.cd("PU");
 
 hPUMC     -> Write();
 hPUDATA   -> Write();
 hPUWeight -> Write();
 
 outFile.cd();
 outFile.mkdir("Cut");
 outFile.cd("Cut");
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  cCompareCut[iCut] -> Write();
  cCompareCutPull[iCut] -> Write();
  TString nameCut = Form ("Cut_%d",iCut);
  TString Cut = Form ("%s",vCut.at(iCut).c_str());
  TNamed nameCutNamed (nameCut,Cut);
  nameCutNamed.Write();
 }
 
 outFile.cd();
 outFile.mkdir("Var");
 outFile.cd("Var");
 for (uint iVar = 0; iVar<vVarName.size(); iVar++){
  cCompareVar[iVar] -> Write();
  cCompareVarPull[iVar] -> Write();
 }
 
 outFile.cd();
 TDirectory* cdAll = (TDirectory*) outFile.mkdir("All");
 cdAll->mkdir("Var");
 cdAll->mkdir("Pull");
 outFile.cd("All/Var");
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   ccCanvas[iCut][iVar]-> Write();
  }
 }
 
 outFile.cd();
 outFile.cd("All/Pull");
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   ccCanvasPull[iCut][iVar]-> Write();
  }
 }
 
 outFile.cd();
 outFile.mkdir("Data");
 outFile.cd("Data");
 for (uint iCut = 0; iCut<vCut.size(); iCut++){
  for (uint iVar = 0; iVar<vVarName.size(); iVar++){
   hs[iCut][iVar] -> Write() ;
   ((TH1F*)(hs[iCut][iVar] ->GetStack()->Last()))->Write() ;
   for (uint iName=0; iName<reduced_name_samples.size(); iName++){
    bool isSig = false;
    for (uint inumDataName=0; inumDataName < numDataName.size(); inumDataName++){
     if (numDataName.at(inumDataName) == iName) {
      isSig = true;
     }
    }
    if (isSig) {
     histo[iName][iCut][iVar] -> Write();
    }
   }
  }
 }
 
 leg->Write();
 
}






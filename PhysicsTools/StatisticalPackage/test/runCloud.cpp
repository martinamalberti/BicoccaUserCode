///==== Produce Cluod for optimization procedure ====

#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <sstream>

#include "TTree.h"
#include "TEntryList.h"



std::vector<std::string> stdstrVariable;
std::vector<int> stdintVariableBin;
std::vector<double> stddoubleVariableMin;
std::vector<double> stddoubleVariableMax;


std::vector<double> stddoubleVariableMin_temp;
std::vector<double> stddoubleVariableMax_temp;


TString Cut;

int numSig = 0;

TTree *treeEffVect[100];
TTree *treeLepVect[100];
TTree *treeJetVect[100];
TTree *treeJetLepVect[100];

char *nameSample[1000];
char *nameSamplePrefix[1000];
char *nameSampleTree[1000];

int numberOfSamples = 0;

double LUMI = 0;
std::vector<double> vNormalization;

///=================================
///==== Calculate Normalization ====
///=================================
void CalculateNormalization(){
 for (int iSample=0; iSample<numberOfSamples; iSample++){
  double XSection;
  double eff_Channel_Filter;
  double preselection_efficiency;
  int numEntriesBefore;
  treeEffVect[iSample]->SetBranchAddress("XSection",&XSection);
  treeEffVect[iSample]->SetBranchAddress("preselection_efficiency",&preselection_efficiency);
  treeEffVect[iSample]->SetBranchAddress("eff_Channel_Filter",&eff_Channel_Filter);
  treeEffVect[iSample]->SetBranchAddress("numEntriesBefore",&numEntriesBefore);
  treeEffVect[iSample]->GetEntry(0);
 
  vNormalization.push_back(LUMI * XSection * preselection_efficiency / numEntriesBefore);
 }
}

///======================================================
///==== count number of signal and background events ====
///======================================================

std::pair<std::pair<double,double>, std::pair< std::vector<double>, std::vector<double> > > CalculateSB(){
 TString Cut_Var = Cut;
 for (int iVar = 0; iVar < stdintVariableBin.size(); iVar++){
  Cut_Var = Form("%s && (%s>%f && %s<%f)",Cut_Var.Data(),stdstrVariable.at(iVar).c_str(),stddoubleVariableMin_temp.at(iVar),stdstrVariable.at(iVar).c_str(),stddoubleVariableMax_temp.at(iVar));
 }
 
//  std::cerr << " Cut_Var = " << Cut_Var.Data() << std::endl;
 
 double nSig = 0;
 double nBkg = 0;
 for (int iSample=0; iSample<numberOfSamples; iSample++){
  
  treeJetLepVect[iSample]->SetEntryList(0); 
  treeJetLepVect[iSample]->Draw(">> myList",Cut_Var.Data(),"entrylist");
  TEntryList *myList = (TEntryList*)gDirectory->Get("myList");
  treeJetLepVect[iSample]->SetEntryList(myList);
  
  double normalization = vNormalization.at(iSample);
  int numEvents = myList->GetN();
  double scaledNumEvents = numEvents * normalization;
  
  if (iSample<numSig) {
//    std::cout << " nSig = " << nSig << " + " << scaledNumEvents <<  " = " << nSig + scaledNumEvents << "( " << numEvents << " * " << normalization <<" )" << std::endl;
   nSig += scaledNumEvents;
  }
  else {
//    std::cout << " nBkg = " << nBkg << " + " << scaledNumEvents <<  " = " << nBkg + scaledNumEvents << "( " << numEvents << " * " << normalization <<" )" << std::endl;
   nBkg += scaledNumEvents;
  }   
 }
 
 std::pair<std::pair<double,double>, std::pair< std::vector<double>, std::vector<double> > > result;
 std::pair<double,double> resultSB;
 resultSB.first = nSig;
 resultSB.second = nBkg;
 
 std::pair< std::vector<double>, std::vector<double> > selection(stddoubleVariableMin_temp,stddoubleVariableMax_temp);
 
 result.first = resultSB;
 result.second = selection;
 
 return result;
}

///======================================================
///======================================================
///======================================================



TString Cut_Var;
std::vector<std::pair<std::pair<double,double>, std::pair< std::vector<double>, std::vector<double> > > > SigBkg;

///==== Recursive function ====

void runFor(int iVar){ 
 int nVar = stdstrVariable.size();
//  std::cerr << " iVar = " << iVar << " : " << nVar << std::endl;
 if (iVar == nVar){
//   std::pair<double,double> tempResult = CalculateSB();
  SigBkg.push_back(CalculateSB()); 
//   std::cerr << " new entry == > S = " << tempResult.first << " B = " << tempResult.second << std::endl;
 }
 else {  
  double min = stddoubleVariableMin.at(iVar);
  double max = stddoubleVariableMax.at(iVar);
  int bin = stdintVariableBin.at(iVar); 
//   std::cerr << " min = " << min << " max = " << max << " bin = " << bin << std::endl;
  for (int iBin = 0; iBin < stdintVariableBin.at(iVar); iBin ++){
   double newMin = (max - min) / bin * iBin + min;
   stddoubleVariableMin_temp.at(iVar) = newMin;
   stddoubleVariableMax_temp.at(iVar) = max;
//    std::cerr << " iVar = " << iVar << " iBin = " << iBin << " : " << stdintVariableBin.at(iVar) << " min = " << stddoubleVariableMin_temp.at(iVar) << " max = " << stddoubleVariableMax_temp.at(iVar) << std::endl;
   runFor(iVar+1);
  }
 }
}

///=======================================




int main(int argc, char** argv){
 
 ///=======================================================================================
 ///==== Produce Cluod for optimization procedure ====
 ///======================================================================================= 
 
 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }
 
 
 // Parse the config file                                                                                                                                                          
 parseConfigFile (argv[1]) ;
 
 std::string inputFile = gConfigParser -> readStringOption("Input::inputFile");
 std::cout << ">>>>> Input::inputFile  " << inputFile << std::endl;  
 
 numSig = gConfigParser -> readIntOption("Input::numSig");
 std::cout << ">>>>> Input::numSig  " << numSig << std::endl;  
  
 std::string inputDirectory = gConfigParser -> readStringOption("Input::inputDirectory");
 std::cout << ">>>>> Input::inputDirectory  " << inputDirectory << std::endl;  
 
 std::string inputPrefix = gConfigParser -> readStringOption("Input::inputPrefix");
 std::cout << ">>>>> Input::inputPrefix  " << inputPrefix << std::endl;  
 
 LUMI = gConfigParser -> readDoubleOption ("Data::Lumi");
 std::cout << ">>>>> Data::Lumi  " << LUMI << std::endl;  
 

 try {
  stdstrVariable = gConfigParser -> readStringListOption ("Options::Variables");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 

 try {
  stdintVariableBin = gConfigParser -> readIntListOption ("Options::VariableBin");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 

 try {
  stddoubleVariableMin = gConfigParser -> readDoubleListOption ("Options::VariablesMin");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 

 try {
  stddoubleVariableMax = gConfigParser -> readDoubleListOption ("Options::VariablesMax");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 
 std::string additionalCutFile;
 try {
  additionalCutFile = gConfigParser -> readStringOption ("Input::additionalCutFile");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 

 std::string buffer;

 std::string additionalCut; 
 std::ifstream inFileCut(additionalCutFile.c_str());
 while(!inFileCut.eof()){
  getline(inFileCut,buffer);
  std::cout << "buffer = " << buffer << std::endl;
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer.at(0) != '#'){
    additionalCut  = buffer;
   } 
  }
 }
 
 std::cout << " additionalCut = " << additionalCut << std::endl;
 Cut = Form("%s",additionalCut.c_str());
 
 
 ///============================================
 

 std::ifstream inFile(inputFile.c_str());
 while(!inFile.eof()){
  getline(inFile,buffer);
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer.at(0) != '#'){
    std::stringstream line( buffer );       
    nameSample[numberOfSamples] = new char [1000];
    line >> nameSample[numberOfSamples]; 
    std::cout << nameSample[numberOfSamples] << " ";
    
    nameSamplePrefix[numberOfSamples] = new char [1000];
    line >> nameSamplePrefix[numberOfSamples];
    
    nameSampleTree[numberOfSamples] = new char [1000];
    line >> nameSampleTree[numberOfSamples];
    
    std::cout << std::endl;
    
    char nameFile[1000];
    sprintf(nameFile,"%s/%s_%s.root",inputDirectory.c_str(),inputPrefix.c_str(),nameSample[numberOfSamples]);  
    TFile* f = new TFile(nameFile, "READ");
    
    treeEffVect[numberOfSamples] = (TTree*) f->Get("outTreeSelections");
    char nameTreeEff[100];
    sprintf(nameTreeEff,"treeEff_%d",numberOfSamples); 
    treeEffVect[numberOfSamples]->SetName(nameTreeEff);      
    
    treeJetLepVect[numberOfSamples] = (TTree*) f->Get("outTreeJetLep");
    char nameTreeJetLep[100];
    sprintf(nameTreeJetLep,"treeJetLep_%d",numberOfSamples); 
    treeJetLepVect[numberOfSamples]->SetName(nameTreeJetLep);

    numberOfSamples++;
   } 
  }
 }
 

 ///===============================================================
 
 int numVariables = stdstrVariable.size();
 std::cout << " numVariables = " << numVariables << std::endl;
 
 ///==== init ====
 for (int iVar = 0; iVar < stdintVariableBin.size(); iVar++){
  stddoubleVariableMin_temp.push_back(stddoubleVariableMin.at(iVar));
  stddoubleVariableMax_temp.push_back(stddoubleVariableMax.at(iVar));
 }
 CalculateNormalization();
 
 ///==== cloud ====
 runFor(0);
 ///==== end cloud ====
 


///==== output ====
 std::string outputFile = gConfigParser -> readStringOption("Output::outputFile");
 std::cout << ">>>>> Output::outputFile  " << outputFile << std::endl;  
 
 TFile* outFile = new TFile(outputFile.c_str(),"RECREATE");
 outFile->cd();
 TTree* myOutTree = new TTree("myOutTree","myOutTree");
 double S;
 double B;
 myOutTree->Branch("S",&S,"S/D");
 myOutTree->Branch("B",&B,"B/D");
 
 double* VarName_Min = new double[stdintVariableBin.size()];
 double* VarName_Max = new double[stdintVariableBin.size()];
 for (int iVar = 0; iVar < stdintVariableBin.size(); iVar++){
  TString nameVar_0_Min = Form("%s_Min",stdstrVariable.at(iVar).c_str());
  TString nameVar_0_Max = Form("%s_Max",stdstrVariable.at(iVar).c_str());
  
  TString nameVar_Min = Form("%s_Min/D",stdstrVariable.at(iVar).c_str());
  TString nameVar_Max = Form("%s_Max/D",stdstrVariable.at(iVar).c_str());

  myOutTree->Branch(nameVar_0_Min,(VarName_Min+iVar),nameVar_Min);
  myOutTree->Branch(nameVar_0_Max,(VarName_Max+iVar),nameVar_Max);
 } 
 
 for (int iPoint = 0; iPoint < SigBkg.size(); iPoint++) {
  std::cout << " " << iPoint << " : " << SigBkg.size() << " S = " << SigBkg.at(iPoint).first.first << " B = " << 
  SigBkg.at(iPoint).first.second << std::endl;
  
  for (int iVar = 0; iVar < stdintVariableBin.size(); iVar++){
//    std::cerr << " SigBkg.at(iPoint).second.first.size() = " << SigBkg.at(iPoint).second.first.size() << std::endl;
   
//    std::cerr << " SigBkg.at(iPoint).second.first.at(" << iVar << ") = " << SigBkg.at(iPoint).second.first.at(iVar) << std::endl;
//    std::cerr << " SigBkg.at(iPoint).second.second.at(" << iVar << ") = " << SigBkg.at(iPoint).second.second.at(iVar) << std::endl;
   VarName_Min[iVar] = SigBkg.at(iPoint).second.first.at(iVar);
   VarName_Max[iVar] = SigBkg.at(iPoint).second.second.at(iVar);
  }  
  S = SigBkg.at(iPoint).first.first;
  B = SigBkg.at(iPoint).first.second;
  myOutTree->Fill();
 }
 
 myOutTree->Write();
 
 
}



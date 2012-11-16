// g++ -Wall -o studyNoiseMC `root-config --cflags --glibs` avgPUList.cc TEndcapRings.cc -lTMVA studyNoiseMC.cpp
// To Run ./studyNoise <inputList> <outputFile> <VariableOfCut> <GoodRange> <Type of Var> --> ntupla to be sorted on that variable
// otherwise change break into continue in the cycle

#include "avgPUList.h"
#include "TEndcapRings.h"

#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TApplication.h"

#include <cmath>

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#endif


bool FillChain(TChain& chain, const std::string& inputFileList);


int nBins_nVtx = 35;
float nVtxMin = 0.5;
float nVtxMax = 35.5;

int nBins_nAvgPU = 30;
float nAvgPUMin = 5.;
float nAvgPUMax = 35;

int nBins_iEta = 35;
float iEtaMin = 1.;
float iEtaMax = 86.;

int nBins_iRing = 20;
float iRingMin = 0.;
float iRingMax = 40.;



int main(int argc, char**argv){

  // open files and fill the tree chain
  std::string inputFileList = argv[1];
  std::string outFileName   = argv[2];
  std::string inputVariable = argv[3];
  std::string varRange      = argv[4];
  std::string varType       = argv[5];

  if(argc !=6) {std::cerr<<" Errors in parsing the parameters "<<std::endl; return 1; }
  

  TChain* chain = new TChain("simpleNtupleRecHits/SimpleNtupleEoverP");
  FillChain(*chain,inputFileList);
    
  // EE geometry
  TEndcapRings* eRings = new TEndcapRings();
  
  // define outfile
  TFile* outFile = new TFile(outFileName.c_str(),"RECREATE");
  
  // define histograms
  TH1F* h_occupancy_vsNvtx_EB = new TH1F("h_occupancy_vsNvtx_EB","",nBins_nVtx,nVtxMin,nVtxMax);
  TH1F* h_occupancy_vsNvtx_EE = new TH1F("h_occupancy_vsNvtx_EE","",nBins_nVtx,nVtxMin,nVtxMax);
  h_occupancy_vsNvtx_EB -> Sumw2();
  h_occupancy_vsNvtx_EE -> Sumw2();
  
  TH1F* h_occupancy_vsNavgPU_EB = new TH1F("h_occupancy_vsNavgPU_EB","",nBins_nAvgPU,nAvgPUMin,nAvgPUMax);
  TH1F* h_occupancy_vsNavgPU_EE = new TH1F("h_occupancy_vsNavgPU_EE","",nBins_nAvgPU,nAvgPUMin,nAvgPUMax);
  h_occupancy_vsNavgPU_EB -> Sumw2();
  h_occupancy_vsNavgPU_EE -> Sumw2();
  
  TH1F* h_occupancy_vsIeta_EB = new TH1F("h_occupancy_vsIeta_EB","",nBins_iEta,iEtaMin,iEtaMax);
  h_occupancy_vsIeta_EB -> Sumw2();
  TH1F* h_occupancy_vsIring_EE = new TH1F("h_occupancy_vsIring_EE","",nBins_iRing,iRingMin,iRingMax);
  h_occupancy_vsIring_EE -> Sumw2();
  
  std::map<float,TH1F*> map_recHitE_vsNvtx_EB; // for each vertex or PU number --> associated distribution
  std::map<float,TH1F*> map_recHitE_vsNvtx_EE;
  std::map<float,TH1F*> map_recHitE_nAvgPU10_vsNvtx_EB;
  std::map<float,TH1F*> map_recHitE_nAvgPU10_vsNvtx_EE;
  std::map<float,TH1F*> map_recHitE_nAvgPU20_vsNvtx_EB;
  std::map<float,TH1F*> map_recHitE_nAvgPU20_vsNvtx_EE;
  
  std::map<float,TH1F*> map_recHitE_vsNavgPU_EB;
  std::map<float,TH1F*> map_recHitE_vsNavgPU_EE;
  std::map<float,TH1F*> map_recHitE_nVtx10_vsNavgPU_EB;
  std::map<float,TH1F*> map_recHitE_nVtx10_vsNavgPU_EE;
  std::map<float,TH1F*> map_recHitE_nVtx20_vsNavgPU_EB;
  std::map<float,TH1F*> map_recHitE_nVtx20_vsNavgPU_EE;
  
  std::map<float,std::map<float,TH1F*> > map_recHitE_vsNavgPU_vsIeta_EB;
  std::map<float,std::map<float,TH1F*> > map_recHitE_vsNavgPU_vsIring_EE;
  
  // EB
  for(int bin = 1; bin <= nBins_nVtx; ++bin){

    char histoName[50];
    
    float binCenter  = h_occupancy_vsNvtx_EB -> GetBinCenter(bin);
    float binLowEdge = h_occupancy_vsNvtx_EB -> GetBinLowEdge(bin);
    float binHigEdge = h_occupancy_vsNvtx_EB -> GetBinLowEdge(bin) + h_occupancy_vsNvtx_EB->GetBinWidth(bin);
    sprintf(histoName,"hEB_recHitE_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_vsNvtx_EB[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
    
    sprintf(histoName,"hEB_recHitE_nAvgPU10_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nAvgPU10_vsNvtx_EB[binCenter] = new TH1F(histoName,"",2000,-4.,4.);
    sprintf(histoName,"hEB_recHitE_nAvgPU20_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nAvgPU20_vsNvtx_EB[binCenter] = new TH1F(histoName,"",2000,-4.,4.);
  }
  
  for(int bin = 1; bin <= nBins_nAvgPU; ++bin){

    char histoName[50];
    
    float binCenter  = h_occupancy_vsNavgPU_EB -> GetBinCenter(bin);
    float binLowEdge = h_occupancy_vsNavgPU_EB -> GetBinLowEdge(bin);
    float binHigEdge = h_occupancy_vsNavgPU_EB -> GetBinLowEdge(bin) + h_occupancy_vsNavgPU_EB->GetBinWidth(bin);
    sprintf(histoName,"hEB_recHitE_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_vsNavgPU_EB[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
    
    sprintf(histoName,"hEB_recHitE_nVtx10_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nVtx10_vsNavgPU_EB[binCenter] = new TH1F(histoName,"",2000,-4.,4.);
    sprintf(histoName,"hEB_recHitE_nVtx20_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nVtx20_vsNavgPU_EB[binCenter] = new TH1F(histoName,"",2000,-4.,4.);
    
    for(int iEtaBin = 1; iEtaBin <= nBins_iEta; ++iEtaBin)
    {
      float iEtaBinCenter  = h_occupancy_vsIeta_EB -> GetBinCenter(iEtaBin);
      float iEtaBinLowEdge = h_occupancy_vsIeta_EB -> GetBinLowEdge(iEtaBin);
      float iEtaBinHigEdge = h_occupancy_vsIeta_EB -> GetBinLowEdge(iEtaBin) + h_occupancy_vsIeta_EB->GetBinWidth(bin);
      sprintf(histoName,"hEB_recHitE_nAvgPU%02.1f-%02.1f_iEta%02.1f-%02.1f",binLowEdge,binHigEdge,iEtaBinLowEdge,iEtaBinHigEdge);
      (map_recHitE_vsNavgPU_vsIeta_EB[binCenter])[iEtaBinCenter] = new TH1F(histoName,"",2000,-2.,2.);
    }
  }
  
  // EE
  for(int bin = 1; bin <= nBins_nVtx; ++bin){

    char histoName[50];
    
    float binCenter  = h_occupancy_vsNvtx_EE -> GetBinCenter(bin);
    float binLowEdge = h_occupancy_vsNvtx_EE -> GetBinLowEdge(bin);
    float binHigEdge = h_occupancy_vsNvtx_EE -> GetBinLowEdge(bin) + h_occupancy_vsNvtx_EE->GetBinWidth(bin);
    sprintf(histoName,"hEE_recHitE_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_vsNvtx_EE[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
    
    sprintf(histoName,"hEE_recHitE_nAvgPU10_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nAvgPU10_vsNvtx_EE[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
    sprintf(histoName,"hEE_recHitE_nAvgPU20_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nAvgPU20_vsNvtx_EE[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
  }
  
  for(int bin = 1; bin <= nBins_nAvgPU; ++bin){

    char histoName[50];
    
    float binCenter  = h_occupancy_vsNavgPU_EE -> GetBinCenter(bin);
    float binLowEdge = h_occupancy_vsNavgPU_EE -> GetBinLowEdge(bin);
    float binHigEdge = h_occupancy_vsNavgPU_EE -> GetBinLowEdge(bin) + h_occupancy_vsNavgPU_EE->GetBinWidth(bin);
    sprintf(histoName,"hEE_recHitE_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_vsNavgPU_EE[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
    
    sprintf(histoName,"hEE_recHitE_nVtx10_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nVtx10_vsNavgPU_EE[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
    sprintf(histoName,"hEE_recHitE_nVtx20_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    map_recHitE_nVtx20_vsNavgPU_EE[binCenter] = new TH1F(histoName,"",2000,-4.,4.);    
    
    for(int iRingBin = 1; iRingBin <= nBins_iRing; ++iRingBin)
    {
      float iRingBinCenter  = h_occupancy_vsIring_EE -> GetBinCenter(iRingBin);
      float iRingBinLowEdge = h_occupancy_vsIring_EE -> GetBinLowEdge(iRingBin);
      float iRingBinHigEdge = h_occupancy_vsIring_EE -> GetBinLowEdge(iRingBin) + h_occupancy_vsIring_EE->GetBinWidth(bin);
      sprintf(histoName,"hEE_recHitE_nAvgPU%02.1f-%02.1f_iRing%02.1f-%02.1f",binLowEdge,binHigEdge,iRingBinLowEdge,iRingBinHigEdge);
      (map_recHitE_vsNavgPU_vsIring_EE[binCenter])[iRingBinCenter] = new TH1F(histoName,"",2000,-2.,2.);
    }
  }
  
  
  
  // define variables;
  int runId, lumiId;
  int timeStampHigh;
  int PV_n;
  float PUit_TrueNumInteractions;

  std::vector<float>* EBrecHit_E = new std::vector<float>;
  std::vector<float>* EErecHit_E = new std::vector<float>;
 
  std::vector<int>* EBrecHit_ietaORix = new std::vector<int>;
  std::vector<int>* EBrecHit_iphiORiy = new std::vector<int>;
  std::vector<int>* EErecHit_ietaORix = new std::vector<int>;
  std::vector<int>* EErecHit_iphiORiy = new std::vector<int>;
  std::vector<int>* EErecHit_zside = new std::vector<int>;
                                                                    
  chain -> SetBranchStatus("*",0); 
  chain -> SetBranchStatus("timeStampHigh",1);            chain -> SetBranchAddress("timeStampHigh",          &timeStampHigh);
  chain -> SetBranchStatus("runId",1);                    chain -> SetBranchAddress("runId",                  &runId);
  chain -> SetBranchStatus("lumiId",1);                   chain -> SetBranchAddress("lumiId",                 &lumiId);
  chain -> SetBranchStatus("PV_n",1);                     chain -> SetBranchAddress("PV_n",                   &PV_n);
  chain -> SetBranchStatus("PUit_TrueNumInteractions",1); chain -> SetBranchAddress("PUit_TrueNumInteractions",&PUit_TrueNumInteractions);

  
  chain -> SetBranchStatus("EBRecHit_E",1);               chain -> SetBranchAddress("EBRecHit_E",        &EBrecHit_E);
  chain -> SetBranchStatus("EERecHit_E",1);               chain -> SetBranchAddress("EERecHit_E",        &EErecHit_E);
  chain -> SetBranchStatus("EBRecHit_ietaORix",1);        chain -> SetBranchAddress("EBRecHit_ietaORix",        &EBrecHit_ietaORix);
  chain -> SetBranchStatus("EBRecHit_iphiORiy",1);        chain -> SetBranchAddress("EBRecHit_iphiORiy",        &EBrecHit_iphiORiy);
  chain -> SetBranchStatus("EERecHit_ietaORix",1);        chain -> SetBranchAddress("EERecHit_ietaORix",        &EErecHit_ietaORix);
  chain -> SetBranchStatus("EERecHit_iphiORiy",1);        chain -> SetBranchAddress("EERecHit_iphiORiy",        &EErecHit_iphiORiy);
  chain -> SetBranchStatus("EERecHit_zside",1);           chain -> SetBranchAddress("EERecHit_zside",           &EErecHit_zside);

  int    Var0;
  double Var1;
 
  if(varType == "int")    { chain -> SetBranchStatus(inputVariable.c_str(),1);  chain -> SetBranchAddress(inputVariable.c_str(),&Var0);}
  if(varType == "double") { chain -> SetBranchStatus(inputVariable.c_str(),1);  chain -> SetBranchAddress(inputVariable.c_str(),&Var1);}

  
  // loop over events                                                                                                                                             
  int nSelectedEvents = 0;

  std::vector<TString> mlist = TMVA::gTools().SplitString(varRange, '-' ); // min and max in the range
  
 
  for( int entry = 0; entry <chain->GetEntries() ; entry++ ) {


    if( entry%100 == 0 ) std::cout << ">>> reading entry " << entry << " / " << chain->GetEntries() << "\r" << std::flush;
    chain->GetEntry(entry);

    if( varType == "int"){ if( Var0 <= atoi(mlist.at(0).Data()) ) continue;
                           if( Var0 >= atoi(mlist.at(1).Data())) break;}  // ntuples should be ordered on the inputVariabe 
  
    if( varType == "double"){ if( Var1 <= atof(mlist.at(0).Data()) ) continue;
                              if( Var1 >= atof(mlist.at(1).Data())) break;}  // ntuples should be ordered on the inputVariab

    //    if( entry%100 == 0 ) std::cout << ">>> reading entry " << entry << " / " << chain->GetEntries() << "\r" << std::flush;
 
    ++nSelectedEvents;
    
          
    // fill ele1
    int bin = -1;
    int iEtaBin = -1;
    int iRingBin = -1;
    float iEtaBinCenter = -1.;
    float iRingBinCenter = -1.;
    float binCenter = -1.;
    TH1F* histo;
    
    for(unsigned int iEle = 0; iEle<EBrecHit_E->size(); iEle++){

      
      bin = h_occupancy_vsNvtx_EB -> Fill(PV_n);
      binCenter = h_occupancy_vsNvtx_EB -> GetBinCenter(bin);
      histo = map_recHitE_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax ) histo->Fill(EBrecHit_E->at(iEle));
      histo = map_recHitE_nAvgPU10_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && PUit_TrueNumInteractions >= 9. && PUit_TrueNumInteractions < 11.) histo->Fill(EBrecHit_E->at(iEle));
      histo = map_recHitE_nAvgPU20_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && PUit_TrueNumInteractions >= 19. && PUit_TrueNumInteractions < 21.) histo->Fill(EBrecHit_E->at(iEle));     
      bin = h_occupancy_vsNavgPU_EB -> Fill(PUit_TrueNumInteractions);
      binCenter = h_occupancy_vsNavgPU_EB -> GetBinCenter(bin);
      histo = map_recHitE_vsNavgPU_EB[binCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax ) histo->Fill(EBrecHit_E->at(iEle));     
      histo = map_recHitE_nVtx10_vsNavgPU_EB[binCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax && PV_n >= 9. && PV_n < 11.) histo->Fill(EBrecHit_E->at(iEle));
      histo = map_recHitE_nVtx20_vsNavgPU_EB[binCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax && PV_n >= 19. && PV_n < 21.) histo->Fill(EBrecHit_E->at(iEle));
      iEtaBin = h_occupancy_vsIeta_EB -> Fill(fabs(EBrecHit_ietaORix->at(iEle)));
      iEtaBinCenter = h_occupancy_vsIeta_EB -> GetBinCenter(iEtaBin);
      //  std::cout<<"binCenter "<<binCenter<<" EtaBin "<<iEtaBinCenter<<" EBrecHit_ietaORix "<<EBrecHit_ietaORix->at(iEle)<<std::endl;
       histo = (map_recHitE_vsNavgPU_vsIeta_EB[binCenter])[iEtaBinCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax ) histo->Fill(EBrecHit_E->at(iEle));
    }
    
    
    for(unsigned int iEle = 0; iEle<EErecHit_E->size(); iEle++){

      int Iring = eRings -> GetEndcapRing(EErecHit_ietaORix->at(iEle),EErecHit_iphiORiy->at(iEle),EErecHit_zside->at(iEle));
      
      bin = h_occupancy_vsNvtx_EE -> Fill(PV_n);
      binCenter = h_occupancy_vsNvtx_EE -> GetBinCenter(bin);
      histo = map_recHitE_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax )  histo->Fill(EErecHit_E->at(iEle));
      histo = map_recHitE_nAvgPU10_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && PUit_TrueNumInteractions >= 9. && PUit_TrueNumInteractions < 11.) histo->Fill(EErecHit_E->at(iEle));
      histo = map_recHitE_nAvgPU20_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && PUit_TrueNumInteractions >= 19. && PUit_TrueNumInteractions < 21.) histo->Fill(EErecHit_E->at(iEle));
      
      bin = h_occupancy_vsNavgPU_EE -> Fill(PUit_TrueNumInteractions);
      binCenter = h_occupancy_vsNavgPU_EE -> GetBinCenter(bin);
      histo = map_recHitE_vsNavgPU_EE[binCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax ) histo->Fill(EErecHit_E->at(iEle));
      histo = map_recHitE_nVtx10_vsNavgPU_EE[binCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax && PV_n >= 9. && PV_n < 11.) histo->Fill(EErecHit_E->at(iEle));
      histo = map_recHitE_nVtx20_vsNavgPU_EE[binCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax && PV_n >= 19. && PV_n < 21.) histo->Fill(EErecHit_E->at(iEle));
 
      iRingBin = h_occupancy_vsIring_EE -> Fill(fabs(Iring));
      iRingBinCenter = h_occupancy_vsIring_EE -> GetBinCenter(iRingBin);
      histo = (map_recHitE_vsNavgPU_vsIring_EE[binCenter])[iRingBinCenter];
      if( PUit_TrueNumInteractions > nAvgPUMin && PUit_TrueNumInteractions < nAvgPUMax ) histo->Fill(EErecHit_E->at(iEle));

    }
    
    
  }
  
  std::cout << std::endl;
  std::cout << ">>> selected " << nSelectedEvents << " / " << chain->GetEntries() << " good events" << std::endl;
  
  
  
  outFile -> cd();
  
  h_occupancy_vsNvtx_EB -> Write();
  h_occupancy_vsNvtx_EE -> Write();
  
  h_occupancy_vsNavgPU_EB -> Write();
  h_occupancy_vsNavgPU_EE -> Write();
  
  h_occupancy_vsIeta_EB  -> Write();
  h_occupancy_vsIring_EE -> Write();
  
  for(int bin = 1; bin <= nBins_nVtx; ++bin)
  {
    float binCenter;
    
    binCenter = h_occupancy_vsNvtx_EB -> GetBinCenter(bin);    
    map_recHitE_vsNvtx_EB[binCenter] -> Write();
    map_recHitE_nAvgPU10_vsNvtx_EB[binCenter] -> Write();
    map_recHitE_nAvgPU20_vsNvtx_EB[binCenter] -> Write();
    
    binCenter  = h_occupancy_vsNvtx_EE -> GetBinCenter(bin);    
    map_recHitE_vsNvtx_EE[binCenter] -> Write();
    map_recHitE_nAvgPU10_vsNvtx_EE[binCenter] -> Write();
    map_recHitE_nAvgPU20_vsNvtx_EE[binCenter] -> Write();
  }
  
  for(int bin = 1; bin <= nBins_nAvgPU; ++bin)
  {
    float binCenter;
    
    binCenter  = h_occupancy_vsNavgPU_EB -> GetBinCenter(bin);    
    map_recHitE_vsNavgPU_EB[binCenter] -> Write();
    map_recHitE_nVtx10_vsNavgPU_EB[binCenter] -> Write();
    map_recHitE_nVtx20_vsNavgPU_EB[binCenter] -> Write();
    
    for(int iEtaBin = 1; iEtaBin <= nBins_iEta; ++iEtaBin)
    {
      float iEtaBinCenter  = h_occupancy_vsIeta_EB -> GetBinCenter(iEtaBin);
      (map_recHitE_vsNavgPU_vsIeta_EB[binCenter])[iEtaBinCenter] -> Write();
    }
    
    binCenter  = h_occupancy_vsNavgPU_EE -> GetBinCenter(bin);    
    map_recHitE_vsNavgPU_EE[binCenter] -> Write();
    map_recHitE_nVtx10_vsNavgPU_EE[binCenter] -> Write();
    map_recHitE_nVtx20_vsNavgPU_EE[binCenter] -> Write();
    
    for(int iRingBin = 1; iRingBin <= nBins_iRing; ++iRingBin)
    {
      float iRingBinCenter  = h_occupancy_vsIring_EE -> GetBinCenter(iRingBin);
      (map_recHitE_vsNavgPU_vsIring_EE[binCenter])[iRingBinCenter] -> Write();
    }
    
  }
  
  outFile -> Close();
  
  
  return 0;
}






bool FillChain(TChain& chain, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;

  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }

  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;
    if( buffer.at(0) == '#' ) continue;
    chain.Add(buffer.c_str());
    std::cout << ">>> ntupleUtils::FillChain - treeName = " << chain.GetName() << " from file " << buffer << std::endl;
  }

  return true;
}







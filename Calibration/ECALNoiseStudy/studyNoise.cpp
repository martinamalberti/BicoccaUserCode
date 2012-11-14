// g++ -Wall -o studyNoise `root-config --cflags --glibs` avgPUList.cc TEndcapRings.cc studyNoise.cpp

#include "avgPUList.h"
#include "TEndcapRings.h"

#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"

bool FillChain(TChain& chain, const std::string& inputFileList);

void FillHisto(TH1F* histo,
              int seedIeta, int seedIphi, 
	       std::vector<float> recHitMatrix_E, std::vector<float> recHitMatrix_LaserCorr, std::vector<int> recHitMatrix_ieta, std::vector<int> recHitMatrix_iphi);


int nBins_nVtx = 35;
float nVtxMin = 0.5;
float nVtxMax = 35.5;

int nBins_nAvgPU = 30;
float nAvgPUMin = 5.;
float nAvgPUMax = 35;

int nBins_iEta = 17;
float iEtaMin = 1.;
float iEtaMax = 86.;

int nBins_iRing = 20;
float iRingMin = 0.;
float iRingMax = 40.;



int main(int argc, char**argv)
{
  // open files and fill the tree chain
  std::string inputFileList = argv[1];
  std::string outFileName = argv[2];

  TChain* chain = new TChain("simpleNtupleEoverP/SimpleNtupleEoverP");
  FillChain(*chain,inputFileList);
  
    
  
  // define list of avg PU
  avgPUList* myAvgPUList = new avgPUList("avgPUList.txt");
  
  
  
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
  
  std::map<float,TH1F*> map_recHitE_vsNvtx_EB;
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
  for(int bin = 1; bin <= nBins_nVtx; ++bin)
  {
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
  
  for(int bin = 1; bin <= nBins_nAvgPU; ++bin)
  {
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
  for(int bin = 1; bin <= nBins_nVtx; ++bin)
  {
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
  
  for(int bin = 1; bin <= nBins_nAvgPU; ++bin)
  {
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
  int isZ;
  int PV_n;
  int ele1_isEB, ele2_isEB;
  int ele1_seedIeta, ele2_seedIeta;
  int ele1_seedIphi, ele2_seedIphi;
  int ele1_seedIx, ele2_seedIx;
  int ele1_seedIy, ele2_seedIy;
  int ele1_seedIz, ele2_seedIz;
  int ele1_seedIring, ele2_seedIring;
  float ele1ele2_scM_regression;
  
  std::vector<float>* ele1_recHitMatrix_E = new std::vector<float>;
  std::vector<float>* ele2_recHitMatrix_E = new std::vector<float>;
  std::vector<float>* ele1_recHitMatrix_LaserCorr = new std::vector<float>;
  std::vector<float>* ele2_recHitMatrix_LaserCorr = new std::vector<float>;
  //  std::vector<float>* ele1_recHitMatrix_ICConst = new std::vector<float>;
  //std::vector<float>* ele2_recHitMatrix_ICConst = new std::vector<float>; 
 
  std::vector<int>* ele1_recHitMatrix_ietaORix = new std::vector<int>;
  std::vector<int>* ele2_recHitMatrix_ietaORix = new std::vector<int>;
  std::vector<int>* ele1_recHitMatrix_iphiORiy = new std::vector<int>;
  std::vector<int>* ele2_recHitMatrix_iphiORiy = new std::vector<int>;

                                                                    
  chain -> SetBranchStatus("*",0); 
  chain -> SetBranchStatus("timeStampHigh",1);           chain -> SetBranchAddress("timeStampHigh",          &timeStampHigh);
  chain -> SetBranchStatus("runId",1);                   chain -> SetBranchAddress("runId",                  &runId);
  chain -> SetBranchStatus("lumiId",1);                  chain -> SetBranchAddress("lumiId",                 &lumiId);
  chain -> SetBranchStatus("isZ",1);                     chain -> SetBranchAddress("isZ",                    &isZ);
  chain -> SetBranchStatus("PV_n",1);                    chain -> SetBranchAddress("PV_n",                   &PV_n);
  chain -> SetBranchStatus("ele1_isEB",1);               chain -> SetBranchAddress("ele1_isEB",              &ele1_isEB);
  chain -> SetBranchStatus("ele2_isEB",1);               chain -> SetBranchAddress("ele2_isEB",              &ele2_isEB);
  chain -> SetBranchStatus("ele1_seedIeta",1);           chain -> SetBranchAddress("ele1_seedIeta",          &ele1_seedIeta);
  chain -> SetBranchStatus("ele2_seedIeta",1);           chain -> SetBranchAddress("ele2_seedIeta",          &ele2_seedIeta);
  chain -> SetBranchStatus("ele1_seedIphi",1);           chain -> SetBranchAddress("ele1_seedIphi",          &ele1_seedIphi);
  chain -> SetBranchStatus("ele2_seedIphi",1);           chain -> SetBranchAddress("ele2_seedIphi",          &ele2_seedIphi);
  chain -> SetBranchStatus("ele1_seedIx",1);             chain -> SetBranchAddress("ele1_seedIx",            &ele1_seedIx);
  chain -> SetBranchStatus("ele2_seedIx",1);             chain -> SetBranchAddress("ele2_seedIx",            &ele2_seedIx);
  chain -> SetBranchStatus("ele1_seedIy",1);             chain -> SetBranchAddress("ele1_seedIy",            &ele1_seedIy);
  chain -> SetBranchStatus("ele2_seedIy",1);             chain -> SetBranchAddress("ele2_seedIy",            &ele2_seedIy);
  chain -> SetBranchStatus("ele1_seedZside",1);          chain -> SetBranchAddress("ele1_seedZside",         &ele1_seedIz);
  chain -> SetBranchStatus("ele2_seedZside",1);          chain -> SetBranchAddress("ele2_seedZside",         &ele2_seedIz);
  chain -> SetBranchStatus("ele1ele2_scM_regression",1); chain -> SetBranchAddress("ele1ele2_scM_regression",&ele1ele2_scM_regression);
  
  chain -> SetBranchStatus("ele1_recHitMatrix_E",1);        chain -> SetBranchAddress("ele1_recHitMatrix_E",        &ele1_recHitMatrix_E);
  chain -> SetBranchStatus("ele2_recHitMatrix_E",1);        chain -> SetBranchAddress("ele2_recHitMatrix_E",        &ele2_recHitMatrix_E);
  chain -> SetBranchStatus("ele1_recHitMatrix_laserCorrection",1); chain -> SetBranchAddress("ele1_recHitMatrix_laserCorrection", &ele1_recHitMatrix_LaserCorr);
  chain -> SetBranchStatus("ele2_recHitMatrix_laserCorrection",1); chain -> SetBranchAddress("ele2_recHitMatrix_laserCorrection", &ele2_recHitMatrix_LaserCorr);
  //  chain -> SetBranchStatus("ele1_recHitMatrix_ICConstant",1); chain -> SetBranchAddress("ele1_recHitMatrix_ICConstant", &ele1_recHitMatrix_ICConst);
  //  chain -> SetBranchStatus("ele2_recHitMatrix_ICConstant",1); chain -> SetBranchAddress("ele2_recHitMatrix_ICConstant", &ele2_recHitMatrix_ICConst);          

  chain -> SetBranchStatus("ele2_recHitMatrix_E",1);        chain -> SetBranchAddress("ele2_recHitMatrix_E",        &ele2_recHitMatrix_E);
  chain -> SetBranchStatus("ele1_recHitMatrix_ietaORix",1); chain -> SetBranchAddress("ele1_recHitMatrix_ietaORix", &ele1_recHitMatrix_ietaORix);
  chain -> SetBranchStatus("ele2_recHitMatrix_ietaORix",1); chain -> SetBranchAddress("ele2_recHitMatrix_ietaORix", &ele2_recHitMatrix_ietaORix);
  chain -> SetBranchStatus("ele1_recHitMatrix_iphiORiy",1); chain -> SetBranchAddress("ele1_recHitMatrix_iphiORiy", &ele1_recHitMatrix_iphiORiy);
  chain -> SetBranchStatus("ele2_recHitMatrix_iphiORiy",1); chain -> SetBranchAddress("ele2_recHitMatrix_iphiORiy", &ele2_recHitMatrix_iphiORiy);  
  
  // loop over events                                                                                                                                             
  int nSelectedEvents = 0;
 
  for( int entry = 0; entry <chain->GetEntries() ; entry++ ) {

    if( entry%10000 == 0 ) std::cout << ">>> reading entry " << entry << " / " << chain->GetEntries() << "\r" << std::flush;
    chain->GetEntry(entry);
    //    std::cout<<runId<<std::endl;    
    
    // selections
    if( isZ == 0 ) continue;
    if( fabs(ele1ele2_scM_regression - 91.19) > 5. ) continue;
    ++nSelectedEvents;
    
    
    
    
    // fill ele1
    int bin = -1;
    int iEtaBin = -1;
    int iRingBin = -1;
    float iEtaBinCenter = -1.;
    float iRingBinCenter = -1.;
    float binCenter = -1.;
    float nAvgPU = myAvgPUList -> GetAvgPU(runId,lumiId);
    TH1F* histo;
    
    if( ele1_isEB == 1 )
    {
      bin = h_occupancy_vsNvtx_EB -> Fill(PV_n);
      binCenter = h_occupancy_vsNvtx_EB -> GetBinCenter(bin);
      histo = map_recHitE_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax ) FillHisto(histo,ele1_seedIeta,ele1_seedIphi,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU10_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 9. && nAvgPU < 11.) FillHisto(histo,ele1_seedIeta,ele1_seedIphi,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU20_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 19. && nAvgPU < 21.) FillHisto(histo,ele1_seedIeta,ele1_seedIphi,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      
      bin = h_occupancy_vsNavgPU_EB -> Fill(nAvgPU);
      binCenter = h_occupancy_vsNavgPU_EB -> GetBinCenter(bin);
      histo = map_recHitE_vsNavgPU_EB[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele1_seedIeta,ele1_seedIphi,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx10_vsNavgPU_EB[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 9. && PV_n < 11.) FillHisto(histo,ele1_seedIeta,ele1_seedIphi,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx20_vsNavgPU_EB[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 19. && PV_n < 21.) FillHisto(histo,ele1_seedIeta,ele1_seedIphi,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      
      iEtaBin = h_occupancy_vsIeta_EB -> Fill(fabs(ele1_seedIeta));
      iEtaBinCenter = h_occupancy_vsIeta_EB -> GetBinCenter(iEtaBin);
      histo = (map_recHitE_vsNavgPU_vsIeta_EB[binCenter])[iEtaBinCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele1_seedIeta,ele1_seedIphi,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
    }
    else
    {
      ele1_seedIring = eRings -> GetEndcapRing(ele1_seedIx,ele1_seedIy,ele1_seedIz);
      
      bin = h_occupancy_vsNvtx_EE -> Fill(PV_n);
      binCenter = h_occupancy_vsNvtx_EE -> GetBinCenter(bin);
      histo = map_recHitE_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax ) FillHisto(histo,ele1_seedIx,ele1_seedIy,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU10_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 9. && nAvgPU < 11.) FillHisto(histo,ele1_seedIx,ele1_seedIy,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU20_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 19. && nAvgPU < 21.) FillHisto(histo,ele1_seedIx,ele1_seedIy,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      
      bin = h_occupancy_vsNavgPU_EE -> Fill(nAvgPU);
      binCenter = h_occupancy_vsNavgPU_EE -> GetBinCenter(bin);
      histo = map_recHitE_vsNavgPU_EE[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele1_seedIx,ele1_seedIy,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx10_vsNavgPU_EE[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 9. && PV_n < 11.) FillHisto(histo,ele1_seedIx,ele1_seedIy,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx20_vsNavgPU_EE[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 19. && PV_n < 21.) FillHisto(histo,ele1_seedIx,ele1_seedIy,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
      
      iRingBin = h_occupancy_vsIring_EE -> Fill(fabs(ele1_seedIring));
      iRingBinCenter = h_occupancy_vsIring_EE -> GetBinCenter(iRingBin);
      histo = (map_recHitE_vsNavgPU_vsIring_EE[binCenter])[iRingBinCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele1_seedIx,ele1_seedIy,*ele1_recHitMatrix_E,*ele1_recHitMatrix_LaserCorr,*ele1_recHitMatrix_ietaORix,*ele1_recHitMatrix_iphiORiy);
    }
    
    
    
    // fill ele2
    if( ele2_isEB == 1 )
    {
      bin = h_occupancy_vsNvtx_EB -> Fill(PV_n);
      binCenter = h_occupancy_vsNvtx_EB -> GetBinCenter(bin);
      histo = map_recHitE_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax ) FillHisto(histo,ele2_seedIeta,ele2_seedIphi,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU10_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 9. && nAvgPU < 11.) FillHisto(histo,ele2_seedIeta,ele2_seedIphi,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU20_vsNvtx_EB[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 19. && nAvgPU < 21.) FillHisto(histo,ele2_seedIeta,ele2_seedIphi,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      
      bin = h_occupancy_vsNavgPU_EB -> Fill(nAvgPU);
      binCenter = h_occupancy_vsNavgPU_EB -> GetBinCenter(bin);
      histo = map_recHitE_vsNavgPU_EB[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele2_seedIeta,ele2_seedIphi,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx10_vsNavgPU_EB[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 9. && PV_n < 11.) FillHisto(histo,ele2_seedIeta,ele2_seedIphi,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx20_vsNavgPU_EB[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 19. && PV_n < 21.) FillHisto(histo,ele2_seedIeta,ele2_seedIphi,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      
      iEtaBin = h_occupancy_vsIeta_EB -> Fill(fabs(ele2_seedIeta));
      iEtaBinCenter = h_occupancy_vsIeta_EB -> GetBinCenter(iEtaBin);
      histo = (map_recHitE_vsNavgPU_vsIeta_EB[binCenter])[iEtaBinCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele2_seedIeta,ele2_seedIphi,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
    }
    else
    {
      ele2_seedIring = eRings -> GetEndcapRing(ele2_seedIx,ele2_seedIy,ele2_seedIz);
      
      bin = h_occupancy_vsNvtx_EE -> Fill(PV_n);
      binCenter = h_occupancy_vsNvtx_EE -> GetBinCenter(bin);
      histo = map_recHitE_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax ) FillHisto(histo,ele2_seedIx,ele2_seedIy,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU10_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 9. && nAvgPU < 11.) FillHisto(histo,ele2_seedIx,ele2_seedIy,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nAvgPU20_vsNvtx_EE[binCenter];
      if( PV_n > nVtxMin && PV_n < nVtxMax && nAvgPU >= 19. && nAvgPU < 21.) FillHisto(histo,ele2_seedIx,ele2_seedIy,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      
      bin = h_occupancy_vsNavgPU_EE -> Fill(nAvgPU);
      binCenter = h_occupancy_vsNavgPU_EE -> GetBinCenter(bin);
      histo = map_recHitE_vsNavgPU_EE[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele2_seedIx,ele2_seedIy,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx10_vsNavgPU_EE[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 9. && PV_n < 11.) FillHisto(histo,ele2_seedIx,ele2_seedIy,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      histo = map_recHitE_nVtx20_vsNavgPU_EE[binCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax && PV_n >= 19. && PV_n < 21.) FillHisto(histo,ele2_seedIx,ele2_seedIy,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
      
      iRingBin = h_occupancy_vsIring_EE -> Fill(fabs(ele2_seedIring));
      iRingBinCenter = h_occupancy_vsIring_EE -> GetBinCenter(iRingBin);
      histo = (map_recHitE_vsNavgPU_vsIring_EE[binCenter])[iRingBinCenter];
      if( nAvgPU > nAvgPUMin && nAvgPU < nAvgPUMax ) FillHisto(histo,ele2_seedIx,ele2_seedIy,*ele2_recHitMatrix_E,*ele2_recHitMatrix_LaserCorr,*ele2_recHitMatrix_ietaORix,*ele2_recHitMatrix_iphiORiy);
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






void FillHisto(TH1F* histo,
              int seedIeta, int seedIphi, 
	       std::vector<float> recHitMatrix_E,std::vector<float> recHitMatrix_LaserCorr ,std::vector<int> recHitMatrix_ieta, std::vector<int> recHitMatrix_iphi)
{
  //if( ((seedIeta-3)%5 != 0) || ((seedIphi-3)%5 != 0) ) return;
  //std::cout << ">>>>>> seed: (" << seedIeta << "," << seedIphi << ")   size: " << recHitMatrix_E.size() << std::endl;
  
  for(unsigned int it = 0; it < recHitMatrix_E.size(); ++it)
  {
    if(recHitMatrix_LaserCorr.at(it)==0) continue;
    //std::cout << ">>>>>>>>> recHit: (" << recHitMatrix_ieta.at(it) << "," << recHitMatrix_iphi.at(it) << ")" << std::endl;
    if( fabs(seedIeta - recHitMatrix_ieta.at(it)) > 5 ) histo -> Fill( recHitMatrix_E.at(it)/recHitMatrix_LaserCorr.at(it) );
  }
}


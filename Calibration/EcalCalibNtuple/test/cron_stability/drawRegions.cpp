// g++ -Wall -o drawRegions `root-config --cflags --glibs` ntupleUtils.cc geometryUtils.cc drawRegions.cpp

#include "ntupleUtils.h"
#include "geometryUtils.h"

#include <iostream>

#include "TH2F.h"
#include "TFile.h"

TH1F* templateHisto;
TF1* templateFunc;
std::vector<double>* mydata;



int main()
{
  TFile* outFile = new TFile("drawRegions.root","RECREATE");
  outFile -> cd();
  
  char EBEE[50];
  
  sprintf(EBEE,"EB");
  
  TH2F* h2_EB_SM  = new TH2F("h2_EB_SM", "",171,-85,86,360,1,361);
  TH2F* h2_EB_LMR = new TH2F("h2_EB_LMR","",171,-85,86,360,1,361);
  TH2F* h2_EB_TT  = new TH2F("h2_EB_TT", "",171,-85,86,360,1,361);

  TH2F* h2_EE_RING = new TH2F("h2_EE_RING","",100,1.,101.,100,1.,101.);
  
  TH2F* h2_EB_LMR_min  = new TH2F("h2_EB_LMR_min", "",171,-85,86,360,1,361);

  //EB
  //initializing EE geometry                                                                                                                                                 
  TBarrelRegions *eRegion = new TBarrelRegions();
  for(int iBin = 1; iBin <= h2_EB_SM->GetNbinsX(); ++iBin)
    for(int jBin = 1; jBin <= h2_EB_SM->GetNbinsY(); ++jBin)
    {
      int iEta = int( h2_EB_SM->GetXaxis()->GetBinLowEdge(iBin) );
      int iPhi = int( h2_EB_SM->GetYaxis()->GetBinLowEdge(jBin) );

      int regionId = eRegion->GetRegionId(iEta,iPhi,"SM");
      h2_EB_SM -> SetBinContent(iBin,jBin,regionId);

      regionId = eRegion->GetRegionId(iEta,iPhi,"LMR");
      h2_EB_LMR -> SetBinContent(iBin,jBin,regionId);
       
      h2_EB_LMR_min -> SetBinContent(iBin,jBin,eRegion->GetRegionIdIeta(regionId,"LMR"));


      regionId = eRegion->GetRegionId(iEta,iPhi,"TT");
      h2_EB_TT -> SetBinContent(iBin,jBin,regionId);
    }

  //EE
  //initializing EE geometry
  int nRings = 5;
  TEndcapRegions *eRings = new TEndcapRegions();

  for(int iBin = 1; iBin <= h2_EE_RING->GetNbinsX(); ++iBin)
    for(int jBin = 1; jBin <= h2_EE_RING->GetNbinsY(); ++jBin)
      {
	int ix = int( h2_EE_RING->GetXaxis()->GetBinLowEdge(iBin) );
	int iy = int( h2_EE_RING->GetYaxis()->GetBinLowEdge(jBin) );

	int regionId = eRings->GetEndcapRing(iBin,jBin,1,nRings);
	h2_EE_RING -> SetBinContent(iBin,jBin,regionId);
      }
  
  
  h2_EB_SM -> Write();
  h2_EB_LMR -> Write();
  h2_EB_LMR_min -> Write();
  h2_EB_TT -> Write();

  h2_EE_RING->Write();
  
  outFile -> Close();
  
  
  return 0;
}

#ifdef _MAKECINT_
#pragma link C++ class map<int,TGraphErrors*>+;
#endif

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TChain.h"
#include "TVirtualFitter.h"



void compareLaserCorr(const std::string& inFile1, const std::string& inFile2)
{
  TFile* f1 = TFile::Open(inFile1.c_str(),"READ");
  TFile* f2 = TFile::Open(inFile2.c_str(),"READ");
  
  TTree* t1 = (TTree*)(f1->Get("ntu"));
  TTree* t2 = (TTree*)(f2->Get("ntu"));
  
  int runId1, lumiId1, eventId1;
  int timeStampHigh1;
  int isW1;
  float seedLaserCorr1;
  float scEta1;
  int seedIeta1, seedIphi1, seedIx1, seedIy1, seedZside1;
  t1 -> SetBranchAddress("runId",             &runId1);
  t1 -> SetBranchAddress("lumiId",            &lumiId1);
  t1 -> SetBranchAddress("eventId",           &eventId1);
  t1 -> SetBranchAddress("timeStampHigh",     &timeStampHigh1);
  t1 -> SetBranchAddress("isW",               &isW1);
  t1 -> SetBranchAddress("ele1_seedLaserCorr",&seedLaserCorr1);
  t1 -> SetBranchAddress("ele1_scEta",        &scEta1);
  t1 -> SetBranchAddress("ele1_seedIeta",     &seedIeta1);
  t1 -> SetBranchAddress("ele1_seedIphi",     &seedIphi1);
  t1 -> SetBranchAddress("ele1_seedIx",       &seedIx1);
  t1 -> SetBranchAddress("ele1_seedIy",       &seedIy1);
  t1 -> SetBranchAddress("ele1_seedZside",    &seedZside1);
  
  int runId2, lumiId2, eventId2;
  int timeStampHigh2;
  int isW2;
  float seedLaserCorr2;
  float scEta2;
  int seedIeta2, seedIphi2, seedIx2, seedIy2, seedZside2;
  t2 -> SetBranchAddress("runId",             &runId2);
  t2 -> SetBranchAddress("lumiId",            &lumiId2);
  t2 -> SetBranchAddress("eventId",           &eventId2);
  t2 -> SetBranchAddress("timeStampHigh",     &timeStampHigh2);
  t2 -> SetBranchAddress("isW",               &isW2);
  t2 -> SetBranchAddress("ele1_seedLaserCorr",&seedLaserCorr2);  
  t2 -> SetBranchAddress("ele1_scEta",        &scEta2);    
  t2 -> SetBranchAddress("ele1_seedIeta",     &seedIeta2);
  t2 -> SetBranchAddress("ele1_seedIphi",     &seedIphi2);
  t2 -> SetBranchAddress("ele1_seedIx",       &seedIx2);
  t2 -> SetBranchAddress("ele1_seedIy",       &seedIy2);
  t2 -> SetBranchAddress("ele1_seedZside",    &seedZside2);  
  
  
  
  TH2F* h_seedLaserCorrOccupancy_EB  = new TH2F("p_seedLaserCorrOccupancy_EB","",  171, -85., 86., 361,   0.,361.);
  TH2F* h_seedLaserCorrOccupancy_EEp = new TH2F("p_seedLaserCorrOccupancy_EEp","", 101,   0.,101., 101,   0.,101.);
  TH2F* h_seedLaserCorrOccupancy_EEm = new TH2F("p_seedLaserCorrOccupancy_EEm","", 101,   0.,101., 101,   0.,101.);
    
  TProfile2D* p_seedLaserCorrMap1_EB  = new TProfile2D("p_seedLaserCorrMap1_EB","",  171, -85., 86., 361,   0.,361., 0.8,1.2);
  TProfile2D* p_seedLaserCorrMap1_EEp = new TProfile2D("p_seedLaserCorrMap1_EEp","", 101,   0.,101., 101,   0.,101., 0.8,1.2);
  TProfile2D* p_seedLaserCorrMap1_EEm = new TProfile2D("p_seedLaserCorrMap1_EEm","", 101,   0.,101., 101,   0.,101., 0.8,1.2);
  
  TProfile2D* p_seedLaserCorrMap2_EB  = new TProfile2D("p_seedLaserCorrMap2_EB","",  171, -85., 86., 361,   0.,361., 0.8,1.2);
  TProfile2D* p_seedLaserCorrMap2_EEp = new TProfile2D("p_seedLaserCorrMap2_EEp","", 101,   0.,101., 101,   0.,101., 0.8,1.2);
  TProfile2D* p_seedLaserCorrMap2_EEm = new TProfile2D("p_seedLaserCorrMap2_EEm","", 101,   0.,101., 101,   0.,101., 0.8,1.2);
      
  TGraph* g_seedLaserCorr_EB = new TGraph();
  TGraph* g_seedLaserCorr_EEp = new TGraph();
  TGraph* g_seedLaserCorr_EEm = new TGraph();
  
  std::map<int,TGraphErrors*> g_seedLaserCorr_channel;
  
  
  
  int entry2old = 0;
  int nPoints_EB = 0;
  int nPoints_EEp = 0;
  int nPoints_EEm = 0;
  std::map<int,int> nPoints_channel;
  
  //-----------------------
  // loop on the first file
  for(int entry1 = 0; entry1 < t1->GetEntries(); ++entry1)
  {
    if( entry1%100000 == 0 )
      std::cout << ">>> reading entry " << entry1 << std::endl;
    t1 -> GetEntry(entry1);
    
    // skip Z events
    if( isW1 == 0 ) continue;
    
    
    //------------------------
    // loop on the second file    
    for(int entry2 = entry2old; entry2 < t2->GetEntries(); ++entry2)
    {    
      t2 -> GetEntry(entry2);
      
      if( timeStampHigh2 > timeStampHigh1 ) break;
      
      
      // the two events match
      if( (runId1 == runId2) && (lumiId1 == lumiId2) && (eventId1 == eventId2) )
      {
        entry2old = entry2;
        
        if( (seedZside1 != seedZside2) ||
            ( (seedZside1 == 0) && ( (seedIeta1 != seedIeta2) || (seedIphi1 != seedIphi2) ) ) ||
            ( (seedZside1 != 0) && (   (seedIx1 != seedIx2)   ||   (seedIy1 != seedIy2) ) ) )
        {
          //std::cout << ">>> Error: events in the two files do not match " << std::endl;
          //std::cout << "scEta1 = " << scEta1 << "   scEta2 = " << scEta2 << std::endl;
          //std::cout << "seedIeta1 = " << seedIeta1 << "   seedIeta2 = " << seedIeta2 << std::endl;
          //std::cout << "seedIphi1 = " << seedIphi1 << "   seedIphi2 = " << seedIphi2 << std::endl;
          //std::cout << "seedIx1 = " << seedIx1 << "   seedIx2 = " << seedIx2 << std::endl;
          //std::cout << "seedIy1 = " << seedIy1 << "   seedIy2 = " << seedIy2 << std::endl;
          break;
        }
        
        
        
        if( seedZside1 == 0 )
        {
          h_seedLaserCorrOccupancy_EB -> Fill(seedIeta1,seedIphi1);
          
          p_seedLaserCorrMap1_EB -> Fill(seedIeta1,seedIphi1,seedLaserCorr1);
          p_seedLaserCorrMap2_EB -> Fill(seedIeta2,seedIphi2,seedLaserCorr2);
          
          g_seedLaserCorr_EB -> SetPoint(nPoints_EB,seedLaserCorr1,seedLaserCorr2);
          ++nPoints_EB;
        }
        
        else
        {
          if( seedZside1 > 0 )
          {
            h_seedLaserCorrOccupancy_EEp -> Fill(seedIx1,seedIy1);
            
            p_seedLaserCorrMap1_EEp -> Fill(seedIx1,seedIy1,seedLaserCorr1);
            p_seedLaserCorrMap2_EEp -> Fill(seedIx2,seedIy2,seedLaserCorr2);
            
            g_seedLaserCorr_EEp -> SetPoint(nPoints_EEp,seedLaserCorr1,seedLaserCorr2);
            ++nPoints_EEp;
          }
          
          else if( seedZside1 < 0 )
          {
            h_seedLaserCorrOccupancy_EEm -> Fill(seedIx1,seedIy1);
            
            p_seedLaserCorrMap1_EEm -> Fill(seedIx1,seedIy1,seedLaserCorr1);
            p_seedLaserCorrMap2_EEm -> Fill(seedIx2,seedIy2,seedLaserCorr2);          
            
            g_seedLaserCorr_EEm -> SetPoint(nPoints_EEm,seedLaserCorr1,seedLaserCorr2);
            ++nPoints_EEm;
          }
          
        }
        
        if( seedZside1 == 0 && abs(seedIeta1) <= 20 )
        {
          if( seedLaserCorr1 <= 1. ) continue;
          
          if( g_seedLaserCorr_channel[seedIeta1*360+seedIphi1] == NULL )
          {
            g_seedLaserCorr_channel[seedIeta1*360+seedIphi1] = new TGraphErrors();
            nPoints_channel[seedIeta1*360+seedIphi1] = 0;
          }
          
          g_seedLaserCorr_channel[seedIeta1*360+seedIphi1] -> SetPoint(nPoints_channel[seedIeta1*360+seedIphi1],seedLaserCorr1,seedLaserCorr2);
          g_seedLaserCorr_channel[seedIeta1*360+seedIphi1] -> SetPointError(nPoints_channel[seedIeta1*360+seedIphi1],0.0002,0.0002);
          ++nPoints_channel[seedIeta1*360+seedIphi1];
        }
        
        break;
      }
    }
  }
  
  
  
  
  
  
  TCanvas* c_seedLaserCorrOccupancy = new TCanvas("c_seedLaserCorrOccupancy","c_seedLaserCorrOccupancy",100,100,1350,500);
  c_seedLaserCorrOccupancy -> Divide(3,1);
  
  c_seedLaserCorrOccupancy -> cd(1);
  h_seedLaserCorrOccupancy_EB -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserCorrOccupancy_EB -> GetYaxis() -> SetTitle("i#phi");
  h_seedLaserCorrOccupancy_EB -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  h_seedLaserCorrOccupancy_EB -> Draw("COLZ");
  
  c_seedLaserCorrOccupancy -> cd(2);
  h_seedLaserCorrOccupancy_EEp -> GetXaxis() -> SetTitle("ix");
  h_seedLaserCorrOccupancy_EEp -> GetYaxis() -> SetTitle("iy");
  h_seedLaserCorrOccupancy_EEp -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  h_seedLaserCorrOccupancy_EEp -> Draw("COLZ");
  
  c_seedLaserCorrOccupancy -> cd(3);
  h_seedLaserCorrOccupancy_EEm -> GetXaxis() -> SetTitle("ix");
  h_seedLaserCorrOccupancy_EEm -> GetYaxis() -> SetTitle("iy");
  h_seedLaserCorrOccupancy_EEm -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  h_seedLaserCorrOccupancy_EEm -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorrMap1 = new TCanvas("c_seedLaserCorrMap1","c_seedLaserCorrMap1",100,100,1350,500);
  c_seedLaserCorrMap1 -> Divide(3,1);
  
  c_seedLaserCorrMap1 -> cd(1);
  p_seedLaserCorrMap1_EB -> GetXaxis() -> SetTitle("i#eta");
  p_seedLaserCorrMap1_EB -> GetYaxis() -> SetTitle("i#phi");
  p_seedLaserCorrMap1_EB -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap1_EB -> GetZaxis() -> SetRangeUser(0.95,1.1);
  p_seedLaserCorrMap1_EB -> Draw("COLZ");
  
  c_seedLaserCorrMap1 -> cd(2);
  p_seedLaserCorrMap1_EEp -> GetXaxis() -> SetTitle("ix");
  p_seedLaserCorrMap1_EEp -> GetYaxis() -> SetTitle("iy");
  p_seedLaserCorrMap1_EEp -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap1_EEp -> GetZaxis() -> SetRangeUser(0.95,1.15);
  p_seedLaserCorrMap1_EEp -> Draw("COLZ");
  
  c_seedLaserCorrMap1 -> cd(3);
  p_seedLaserCorrMap1_EEm -> GetXaxis() -> SetTitle("ix");
  p_seedLaserCorrMap1_EEm -> GetYaxis() -> SetTitle("iy");
  p_seedLaserCorrMap1_EEm -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap1_EEm -> GetZaxis() -> SetRangeUser(0.95,1.15);
  p_seedLaserCorrMap1_EEm -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorrMap2 = new TCanvas("c_seedLaserCorrMap2","c_seedLaserCorrMap2",100,100,1350,500);
  c_seedLaserCorrMap2 -> Divide(3,1);
  
  c_seedLaserCorrMap2 -> cd(1);
  p_seedLaserCorrMap2_EB -> GetXaxis() -> SetTitle("i#eta");
  p_seedLaserCorrMap2_EB -> GetYaxis() -> SetTitle("i#phi");
  p_seedLaserCorrMap2_EB -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap2_EB -> GetZaxis() -> SetRangeUser(0.95,1.1);
  p_seedLaserCorrMap2_EB -> Draw("COLZ");
  
  c_seedLaserCorrMap2 -> cd(2);
  p_seedLaserCorrMap2_EEp -> GetXaxis() -> SetTitle("ix");
  p_seedLaserCorrMap2_EEp -> GetYaxis() -> SetTitle("iy");
  p_seedLaserCorrMap2_EEp -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap2_EEp -> GetZaxis() -> SetRangeUser(0.95,1.15);
  p_seedLaserCorrMap2_EEp -> Draw("COLZ");
  
  c_seedLaserCorrMap2 -> cd(3);
  p_seedLaserCorrMap2_EEm -> GetXaxis() -> SetTitle("ix");
  p_seedLaserCorrMap2_EEm -> GetYaxis() -> SetTitle("iy");
  p_seedLaserCorrMap2_EEm -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap2_EEm -> GetZaxis() -> SetRangeUser(0.95,1.15);
  p_seedLaserCorrMap2_EEm -> Draw("COLZ");
  
  
  
  
  
  
  TCanvas* c_seedLaserCorr = new TCanvas("c_seedLaserCorr","c_seedLaserCorr",100,100,1350,500);
  c_seedLaserCorr -> Divide(3,1);
  
  c_seedLaserCorr -> cd(1);
  g_seedLaserCorr_EB -> GetXaxis() -> SetTitle("seedLaserCorr (old)");
  g_seedLaserCorr_EB -> GetYaxis() -> SetTitle("seedLaserCorr (new)");
  g_seedLaserCorr_EB -> Draw("AP");
  
  c_seedLaserCorr -> cd(2);
  g_seedLaserCorr_EEp -> GetXaxis() -> SetTitle("seedLaserCorr (old)");
  g_seedLaserCorr_EEp -> GetYaxis() -> SetTitle("seedLaserCorr (new)");
  g_seedLaserCorr_EEp -> Draw("AP");
  
  c_seedLaserCorr -> cd(3);
  g_seedLaserCorr_EEm -> GetXaxis() -> SetTitle("seedLaserCorr (old)");
  g_seedLaserCorr_EEm -> GetYaxis() -> SetTitle("seedLaserCorr (new)");
  g_seedLaserCorr_EEm -> Draw("AP");  
  
  
  
  
  
  
  TGraphErrors* g_seedLaserCorr_slopes = new TGraphErrors();
  TH1F* h_seedLaserCorr_slopes = new TH1F("h_seedLaserCorr_slopes","",175,-2.,5.);
  
  int nPoints_slopes = 0;
  TFile* outFile = new TFile("compareSeedLaserCorrHistos.root","RECREATE");
  outFile -> cd();
  
  for(int ieta = 1; ieta <= 20; ++ieta)
  {
    for(int iphi = 1; iphi <= 360; ++iphi)
    {
      if( nPoints_channel[ieta*360+iphi] < 5 ) continue;
      
      TF1* f = new TF1("f","[0]+[1]*x",1.,1.2);
      f->SetParameters(0.,1.);
      g_seedLaserCorr_channel[ieta*360+iphi] -> Fit("f","RQ+");

      if( f->GetParameter(1) < -5. ) continue;
      if( f->GetParameter(1) > +5. ) continue;
      if( f->GetParError(1) < 0.005 ) continue;
      
      g_seedLaserCorr_slopes -> SetPoint(nPoints_slopes,ieta*360+iphi,f->GetParameter(1));
      g_seedLaserCorr_slopes -> SetPointError(nPoints_slopes,0.,f->GetParError(1));
      h_seedLaserCorr_slopes -> Fill(f->GetParameter(1));
      
      char graphName[50];
      sprintf(graphName,"g_seedLaserCorr_channel_%d-%d",ieta,iphi);
      g_seedLaserCorr_channel[ieta*360+iphi] -> Write(graphName);
      
      delete f;
      ++nPoints_slopes;
    }
  }
  
  outFile -> Close();
  
  
  TCanvas* c_seedLaserCorr_slopes = new TCanvas("c_seedLaserCorr_slopes","c_seedLaserCorr_slopes",100,100,900,500);
  c_seedLaserCorr_slopes -> Divide(2,1);
  
  c_seedLaserCorr_slopes -> cd(1);
  g_seedLaserCorr_slopes -> GetXaxis() -> SetTitle("channel (i#eta*360 + i#phi)");
  g_seedLaserCorr_slopes -> GetYaxis() -> SetTitle("slope");
  g_seedLaserCorr_slopes -> GetYaxis() -> SetRangeUser(0.,2.5);
  g_seedLaserCorr_slopes -> SetMarkerColor(kRed+2);
  g_seedLaserCorr_slopes -> SetMarkerSize(0.7);
  g_seedLaserCorr_slopes -> Draw("AP");
  
  TF1* pol0 = new TF1("pol0","[0]",0.,50000.);
  pol0 -> SetLineColor(kBlue+2);
  pol0 -> SetLineWidth(2);
  pol0 -> SetLineStyle(2);
  g_seedLaserCorr_slopes -> Fit("pol0","QR+");
  
  c_seedLaserCorr_slopes -> cd(2);
  h_seedLaserCorr_slopes -> GetXaxis() -> SetTitle("slope");
  h_seedLaserCorr_slopes -> Draw();
  
  
  
//  TCanvas* c_seedLaserCorr_channel = new TCanvas("c_seedLaserCorr_channel","c_seedLaserCorr_channel");
//  
//  c_seedLaserCorr_channel -> cd();
//  g_seedLaserCorr_channel -> GetXaxis() -> SetTitle("seedLaserCorr (old)");
//  g_seedLaserCorr_channel -> GetYaxis() -> SetTitle("seedLaserCorr (new)");
//  g_seedLaserCorr_channel -> Draw("AP");
//  
//  TF1* f = new TF1("f","x",0.,2.);
//  f -> SetLineColor(kBlue);
//  f -> SetLineWidth(2);
//  f -> SetLineStyle(2);
//  f -> Draw("same");
}
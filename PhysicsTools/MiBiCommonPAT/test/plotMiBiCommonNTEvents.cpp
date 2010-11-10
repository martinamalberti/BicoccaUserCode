// c++ -o plotMiBiCommonNTEvents `root-config --cflags --ldflags --glibs` plotMiBiCommonNTEvents.cpp

#include "TH1F.h" 
#include "TFile.h"

#include <iostream>
#include <string> 



int GetEventsFromHisto(const std::string& histoName, TFile* f)
{
  TH1F* histo = (TH1F*)f->Get(histoName.c_str());
  return histo->GetBinContent(1);
}






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> plotMiBiCommonNTEvents::usage: " << argv[0] << " rootFileName" << std::endl ;
    return 1;
  }
  
  std::string fileName = argv[1];  
  TFile* f = TFile::Open(fileName.c_str(), "READ");
  
  
  
  
  
  
  // define event histograms
  int nSteps = 5;
  
  TH1F* h_events_AK5Calo = new TH1F("h_events_AK5Calo", "h_events_AK5Calo", nSteps, 0, nSteps);
  TH1F* h_efficiencies_AK5Calo = new TH1F("h_efficiencies_AK5Calo", "h_efficiencies_AK5Calo", nSteps, 0, nSteps);
  TH1F* h_relativeEfficiencies_AK5Calo = new TH1F("h_relativeEfficiencies_AK5Calo", "h_relativeEfficiencies_AK5Calo", nSteps, 0, nSteps);
  
  TH1F* h_events_AK5PF = new TH1F("h_events_AK5PF", "h_events_AK5PF", nSteps, 0, nSteps);
  TH1F* h_efficiencies_AK5PF = new TH1F("h_efficiencies_AK5PF", "h_efficiencies_AK5PF", nSteps, 0, nSteps);
  TH1F* h_relativeEfficiencies_AK5PF = new TH1F("h_relativeEfficiencies_AK5PF", "h_relativeEfficiencies_AK5PF", nSteps, 0, nSteps);
  
  TH1F* h_events_PFlow = new TH1F("h_events_PFlow", "h_events_PFlow", nSteps, 0, nSteps);
  TH1F* h_efficiencies_PFlow = new TH1F("h_efficiencies_PFlow", "h_efficiencies_PFlow", nSteps, 0, nSteps);
  TH1F* h_relativeEfficiencies_PFlow = new TH1F("h_relativeEfficiencies_PFlow", "h_relativeEfficiencies_PFlow", nSteps, 0, nSteps);
  
  
  
  
  
  
  // fill histograms at each step
  std::vector<std::string> stepNames(nSteps);
  
  int step = 1;
  stepNames.at(step-1) = "All";
  h_events_AK5Calo -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_AK5PF   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_PFlow   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  
  ++step;
  stepNames.at(step-1) = "NonScraped";
  h_events_AK5Calo -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_AK5PF   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_PFlow   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  
  ++step;
  stepNames.at(step-1) = "GoodVtx";
  h_events_AK5Calo -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_AK5PF   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_PFlow   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  
  ++step;
  stepNames.at(step-1) = "LeptonsFilter";
  h_events_AK5Calo -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_AK5PF   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"Events/passedEvents").c_str(), f));  
  h_events_PFlow   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"PFlowEvents/passedEvents").c_str(), f));  
  
  ++step;
  stepNames.at(step-1) = "JetFilter";
  h_events_AK5Calo -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"AK5CaloEvents/passedEvents").c_str(), f));  
  h_events_AK5PF   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"AK5PFEvents/passedEvents").c_str(), f));  
  h_events_PFlow   -> SetBinContent(step, GetEventsFromHisto( (stepNames.at(step-1)+"PFlowEvents/passedEvents").c_str(), f));  
  
  
  
  
  
  
  // set histogram labels
  for(step = 1; step <= nSteps; ++step)
  {
    h_events_AK5Calo -> GetXaxis() -> SetBinLabel(step, stepNames[step-1].c_str());
    h_events_AK5PF -> GetXaxis()   -> SetBinLabel(step, stepNames[step-1].c_str());
    h_events_PFlow -> GetXaxis()   -> SetBinLabel(step, stepNames[step-1].c_str());
    
    h_efficiencies_AK5Calo -> GetXaxis() -> SetBinLabel(step, stepNames[step-1].c_str());
    h_efficiencies_AK5PF   -> GetXaxis() -> SetBinLabel(step, stepNames[step-1].c_str());
    h_efficiencies_PFlow   -> GetXaxis() -> SetBinLabel(step, stepNames[step-1].c_str());
    
    h_relativeEfficiencies_AK5Calo -> GetXaxis() -> SetBinLabel(step, stepNames[step-1].c_str());
    h_relativeEfficiencies_AK5PF -> GetXaxis() -> SetBinLabel(step, stepNames[step-1].c_str());
    h_relativeEfficiencies_PFlow -> GetXaxis() -> SetBinLabel(step, stepNames[step-1].c_str());
    
    
    if( h_events_AK5Calo->GetBinContent(1) > 0 )
      h_efficiencies_AK5Calo -> SetBinContent(step, 1.*h_events_AK5Calo->GetBinContent(step)/h_events_AK5Calo->GetBinContent(1));
    if( h_events_AK5PF->GetBinContent(1) > 0 )
      h_efficiencies_AK5PF -> SetBinContent(step, 1.*h_events_AK5PF->GetBinContent(step)/h_events_AK5PF->GetBinContent(1));
    if( h_events_PFlow->GetBinContent(1) > 0 )
      h_efficiencies_PFlow -> SetBinContent(step, 1.*h_events_PFlow->GetBinContent(step)/h_events_PFlow->GetBinContent(1));
    
    if( step == 1 )
    { 
      h_relativeEfficiencies_AK5Calo -> SetBinContent(step, 1.);
      h_relativeEfficiencies_AK5PF -> SetBinContent(step, 1.);
      h_relativeEfficiencies_PFlow -> SetBinContent(step, 1.);
    }
    else
    {
      if( h_events_AK5Calo->GetBinContent(step-1) > 0 )
        h_relativeEfficiencies_AK5Calo -> SetBinContent(step, 1.*h_events_AK5Calo->GetBinContent(step)/h_events_AK5Calo->GetBinContent(step-1));
      if( h_events_AK5PF->GetBinContent(step-1) > 0 )
        h_relativeEfficiencies_AK5PF -> SetBinContent(step, 1.*h_events_AK5PF->GetBinContent(step)/h_events_AK5PF->GetBinContent(step-1));
      if( h_events_PFlow->GetBinContent(step-1) > 0 )
        h_relativeEfficiencies_PFlow -> SetBinContent(step, 1.*h_events_PFlow->GetBinContent(step)/h_events_PFlow->GetBinContent(step-1));
    }
  }
  
  
  
  // Save histograms
  TFile* outFile = new TFile("MiBiCommonNTEvents.root", "RECREATE");
  outFile -> cd();
  
  h_events_AK5Calo -> Write();  
  h_efficiencies_AK5Calo -> Write();  
  h_relativeEfficiencies_AK5Calo -> Write();
  
  h_events_AK5PF -> Write();  
  h_efficiencies_AK5PF -> Write();  
  h_relativeEfficiencies_AK5PF -> Write();
  
  h_events_PFlow -> Write();  
  h_efficiencies_PFlow -> Write();  
  h_relativeEfficiencies_PFlow -> Write();
  
  outFile -> Close();
  
  delete outFile;
  
  delete h_events_AK5Calo;
  delete h_efficiencies_AK5Calo;
  delete h_relativeEfficiencies_AK5Calo;
  
  delete h_events_AK5PF;
  delete h_efficiencies_AK5PF;
  delete h_relativeEfficiencies_AK5PF;
  
  delete h_events_PFlow;
  delete h_efficiencies_PFlow;
  delete h_relativeEfficiencies_PFlow;
}

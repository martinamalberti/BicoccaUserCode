#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TSystem.h"







int main (int argc, char** argv)
{
  std::cout << argv[0] << " " << argv[1] << std::endl; 
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet();
  
  
  
  edm::ParameterSet subPSetSelections =  parameterSet -> getParameter<edm::ParameterSet>("selections");
  std::string saveFolder = subPSetSelections.getParameter<std::string>("saveFolder");
  std::string eventType = subPSetSelections.getParameter<std::string>("eventType");
  double crossSection = subPSetSelections.getParameter<double>("crossSection");
  
  int nJetAlgo = 3;
  std::vector<std::string> jetAlgo;
  jetAlgo.push_back("sisCone5CaloJets");
  jetAlgo.push_back("correctedSisCone5CaloJets");
  jetAlgo.push_back("sisCone5PFJets");

  std::vector<std::string> h3Name;
  h3Name.push_back("VBFEtMinCaloJetCountFilterSisCone5CaloJets/passedEvents");
  h3Name.push_back("VBFEtMinCaloJetCountFilterCorrectedSisCone5CaloJets/passedEvents");
  h3Name.push_back("VBFEtMinPFJetCountFilterSisCone5PFJets/passedEvents");
  
  
  
  edm::ParameterSet subPSetInput = parameterSet -> getParameter<edm::ParameterSet>("inputEfficiencies");
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles");
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  
  std::vector<TH1F*> events;
  std::vector<TH1F*> eventsScaled;
  std::vector<TH1F*> efficiencies;
  
  for(int i = 0; i < nJetAlgo; ++i)
  {
    events.push_back(new TH1F(("events_"+jetAlgo.at(i)).c_str(), ("events_"+jetAlgo.at(i)).c_str(), 3, 0., 3.));
    eventsScaled.push_back(new TH1F(("eventsScaled_"+jetAlgo.at(i)).c_str(), ("eventsScales_"+jetAlgo.at(i)).c_str(), 3, 0., 3.));
    efficiencies.push_back(new TH1F(("efficiencies_"+jetAlgo.at(i)).c_str(), ("efficiencies_"+jetAlgo.at(i)).c_str(), 3, 0., 3.));
  }
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Loop over input files
  // --------------------------------------------------------------------------------------
  
  int nEvents_total = 0;
  int nEvents_VBFPtMinLeptonCountFilter = 0;
  std::vector<int> nEvents_VBFEtMinCaloJetCountFilter(nJetAlgo);
  
  
  // loop over files  
  for(std::vector<std::string>::const_iterator listIt = inputFiles.begin();
      listIt != inputFiles.end(); ++listIt)
  {
    // open the root file
    std::cout << *listIt << std::endl;
    TFile* inFile = new TFile(listIt -> c_str(), "READ");
    if(!inFile->IsOpen()) continue;
    
    
    
    // get the histograms in each file
    TH1F* h1 = static_cast<TH1F*>(inFile -> Get("MCDecayModeFilterAll/passedEvents"));
    nEvents_total += static_cast<int>(h1 -> GetBinContent(1));
    
    
    TH1F* h2 = static_cast<TH1F*>(inFile -> Get("VBFPtMinLeptonCountFilterAll/passedEvents"));
    nEvents_VBFPtMinLeptonCountFilter += static_cast<int>(h2 -> GetBinContent(1));
    
    
    std::vector<TH1F*> h3;
    for(int i = 0; i < nJetAlgo; ++i)
    {
      h3.push_back( static_cast<TH1F*>(inFile -> Get(h3Name.at(i).c_str())) );
      
      
      
      // fill the histogram
      events.at(i)       -> Fill(0.5, h1 -> GetBinContent(1));
      eventsScaled.at(i) -> Fill(0.5, h1 -> GetBinContent(1));

      events.at(i)       -> GetXaxis() -> SetBinLabel(1, "All events");
      eventsScaled.at(i) -> GetXaxis() -> SetBinLabel(1, "All events");
      efficiencies.at(i) -> GetXaxis() -> SetBinLabel(1, "All events");
      
      
      
      events.at(i)       -> Fill(1.5, h2 -> GetBinContent(1));
      eventsScaled.at(i) -> Fill(1.5, h2 -> GetBinContent(1));
      
      events.at(i)       -> GetXaxis() -> SetBinLabel(2, "VBFPtMinLeptonCountFilter");
      eventsScaled.at(i) -> GetXaxis() -> SetBinLabel(2, "VBFPtMinLeptonCountFilter");
      efficiencies.at(i) -> GetXaxis() -> SetBinLabel(2, "VBFPtMinLeptonCountFilter");
      
      
      
      events.at(i)       -> Fill(2.5, (h3.at(i)) -> GetBinContent(1));
      eventsScaled.at(i) -> Fill(2.5, (h3.at(i)) -> GetBinContent(1));
      
      events.at(i)       -> GetXaxis() -> SetBinLabel(3, "VBFEtMinCaloJetCountFilter");
      eventsScaled.at(i) -> GetXaxis() -> SetBinLabel(3, "VBFEtMinCaloJetCountFilter");
      efficiencies.at(i) -> GetXaxis() -> SetBinLabel(3, "VBFEtMinCaloJetCountFilter");
      
      nEvents_VBFEtMinCaloJetCountFilter.at(i) += static_cast<int>(h3.at(i) -> GetBinContent(1));
    }
    
    
    
    inFile -> Close();
    
  } // loop over files  
  
  
  
  // rescale histograms for cross-section --- 1. pb-1 equivalent
  for(int i = 0; i < nJetAlgo; ++i)
  {
    double scaling = 1. / nEvents_total * crossSection * 1;
    
    eventsScaled.at(i) -> Scale(scaling);
  }
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  // Output file
  std::string outRootFileName = saveFolder + "VBFPreselectionPlots_" + eventType + ".root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  
  // Save histograms
  for(int i = 0; i < nJetAlgo; ++i)
  {
    events.at(i)       -> Write();
    eventsScaled.at(i) -> Write();
    
    for(int bin = 1; bin <= events.at(i) -> GetNbinsX(); ++bin)
      efficiencies.at(i) -> SetBinContent(bin, 1. * events.at(i) -> GetBinContent(bin) / events.at(i) -> GetBinContent(1));
    efficiencies.at(i) -> Write();
  }
  
  
  outRootFile.Close();
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Print out
  // --------------------------------------------------------------------------------------
  
  std::cout << ">>>>> SUMMARY <<<<<" << std::endl;
  std::cout << "Event type = " << eventType << "   with cross-section   sigma = " << crossSection << std::endl;
  
  std::cout << "Total events analyzed = " << nEvents_total  
            << "   corresponding to    L = " << nEvents_total / crossSection
            << std::endl;
  
  return 0;
}

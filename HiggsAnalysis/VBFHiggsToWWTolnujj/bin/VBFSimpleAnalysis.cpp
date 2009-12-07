#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleUtils.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/MCHistograms.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/RecoHistograms.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <map>

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
  std::string saveFolder = subPSetSelections.getParameter<std::string> ("saveFolder");
  std::string eventType        = subPSetSelections.getParameter<std::string> ("eventType");
  bool isSignal                = subPSetSelections.getParameter<bool>        ("isSignal");
  double crossSection          = subPSetSelections.getParameter<double>      ("crossSection");
  std::string jetAlgorithm     = subPSetSelections.getParameter<std::string> ("jetAlgorithm");
  int printModulo              = subPSetSelections.getUntrackedParameter<int>("printModulo", 10000);
  int maxEvent                 = subPSetSelections.getUntrackedParameter<int>("maxEvent", -1);
  
  edm::ParameterSet subPSetCuts =  parameterSet -> getParameter<edm::ParameterSet>("cuts");
  double mJJ_tag_MIN  = subPSetCuts.getParameter<double>("mJJ_tag_MIN"); 
  double Deta_tag_MIN = subPSetCuts.getParameter<double>("Deta_tag_MIN"); 
  double mJJ_W_MIN  = subPSetCuts.getParameter<double>("mJJ_W_MIN");
  double mJJ_W_MAX  = subPSetCuts.getParameter<double>("mJJ_W_MAX");
  double absEta_CJV_MAX = subPSetCuts.getParameter<double>("absEta_CJV_MAX");
  double Dphi_LJJ_MAX = subPSetCuts.getParameter<double>("Dphi_LJJ_MAX");
  double mass_LJJ_MAX = subPSetCuts.getParameter<double>("mass_LJJ_MAX");
  double met_MIN = subPSetCuts.getParameter<double>("met_MIN");
  double met_MAX = subPSetCuts.getParameter<double>("met_MAX");

  edm::ParameterSet subPSetInput = parameterSet -> getParameter<edm::ParameterSet>("inputNtuples");
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > (("inputFiles_"+jetAlgorithm).c_str());
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  
  int nSteps = 10;
  std::map<int, int> nEvents;
  
  TH1F* events = new TH1F("events", "events", nSteps, 0., 1.*nSteps);
  TH1F* eventsScaled = new TH1F("eventsScaled", "eventsScaled", nSteps, 0., 1.*nSteps);
  TH1F* efficiencies = new TH1F("efficiencies", "efficiencies", nSteps, 0., 1.*nSteps);
  
  // mc histograms
  std::map<int, MCHistograms*> MCHistos;
  if(isSignal)
  {
    for(int i = 0; i < nSteps; ++i)
      MCHistos[i] = new MCHistograms(i);
  }
  
  // reco histograms
  std::map<int, RecoHistograms*> RecoHistos;
  for(int i = 0; i < nSteps; ++i)
    RecoHistos[i] = new RecoHistograms(i);
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Loop over input files
  // --------------------------------------------------------------------------------------
  
  // Load ntuple
  TChain* chain = new TChain("ntu");
  VBFNtupleContent treeVars;
  SetBranchAddresses(chain, treeVars);
  
  for(std::vector<std::string>::const_iterator listIt = inputFiles.begin();
      listIt != inputFiles.end(); ++listIt)
  {
    chain -> Add(listIt -> c_str());
    std::cout << *listIt << std::endl;
  }
  
  int nEntries = chain -> GetEntries();
  std::cout << "FOUND " << nEntries << " ENTRIES" << std::endl;
  // Load ntuple
  
  
  
  
  
  
  // Input files
  std::string inputRootFileName = saveFolder + "VBFPreselectionPlots_" + eventType + ".root";
  TFile* inputRootFile = new TFile(inputRootFileName.c_str(), "READ");
  
  std::string histoName = "events_" + jetAlgorithm;
  TH1F* input_events = static_cast<TH1F*>( inputRootFile -> Get(histoName.c_str()) );
  
  histoName = "eventsScaled_" + jetAlgorithm;
  TH1F* input_eventsScaled = static_cast<TH1F*>( inputRootFile -> Get(histoName.c_str()) );
  
  histoName = "efficiencies_" + jetAlgorithm;
  TH1F* input_efficiencies = static_cast<TH1F*>( inputRootFile -> Get(histoName.c_str()) );
  // Input files
  
  
  
  
  
  
  // Output file
  std::string outputRootFileName = saveFolder + "/VBFSimpleAnalysis_" + eventType + ".root";
  TFile* outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
  outputRootFile -> cd();
  // Output file
  
  
  
  
  
  
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // STEP 0
  // --------------------------------------------------------------------------------------
  
  events       -> SetBinContent(1, input_events       -> GetBinContent(1));    
  eventsScaled -> SetBinContent(1, input_eventsScaled -> GetBinContent(1));    
  efficiencies -> SetBinContent(1, input_efficiencies -> GetBinContent(1));    
  
  
  
  
  
  
  for(int entry = 0; entry < nEntries; ++entry)
  {
    // get the i-th entry
    chain -> GetEntry(entry);
    if(entry == maxEvent) break;
    if((entry % printModulo) == 0) std::cout << "Reading entry " << entry << std::endl;
    // get the i-th entry    
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 1
    // --------------------------------------------------------------------------------------    
    
    int step = 1;
    
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);
    RecoHistos[step] -> FillRecoHistograms(treeVars);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 2
    // --------------------------------------------------------------------------------------
    
    ++step;
    
    std::vector<int> recoJet_tag_it;
    std::vector<int> recoJet_W_it;
    double maxMJJ = SelectJets(recoJet_tag_it, treeVars, "maxMJJ", 15.);
    
    if( (recoJet_tag_it.at(0) == -1) ||
        (recoJet_tag_it.at(1) == -1) )
      continue;
    
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);    
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 3
    // --------------------------------------------------------------------------------------
    
    ++step;

    if(maxMJJ < mJJ_tag_MIN) continue;
    
    if( fabs( treeVars.recoJet_eta[recoJet_tag_it.at(0)] - 
              treeVars.recoJet_eta[recoJet_tag_it.at(1)])  < Deta_tag_MIN )
      continue;
    
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 4
    // --------------------------------------------------------------------------------------
    
    ++step;
    

    SelectJets(recoJet_W_it, treeVars, "maxSumPt", 15., &recoJet_tag_it);
    
    if( (recoJet_W_it.at(0) == -1) ||
        (recoJet_W_it.at(1) == -1) )
      continue;
    
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);    
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it, &recoJet_W_it);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 5
    // --------------------------------------------------------------------------------------
    
    ++step;
    
    int it1 = recoJet_W_it.at(0);
    int it2 = recoJet_W_it.at(1);
    
    TLorentzVector recoJ_W_1(treeVars.recoJet_px[it1],
                             treeVars.recoJet_py[it1],
                             treeVars.recoJet_pz[it1],
                             treeVars.recoJet_energy[it1]);
    
    TLorentzVector recoJ_W_2(treeVars.recoJet_px[it2],
                             treeVars.recoJet_py[it2],
                             treeVars.recoJet_pz[it2],
                             treeVars.recoJet_energy[it2]);
    
    TLorentzVector recoJJ_W = recoJ_W_1 + recoJ_W_2;
    
    
    
    if(recoJJ_W.M() < mJJ_W_MIN) continue;
    if(recoJJ_W.M() > mJJ_W_MAX) continue;
    
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);    
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it, &recoJet_W_it);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 6
    // --------------------------------------------------------------------------------------
    
    ++step;
    
    int nCandE = 0;
    int nCandMu = 0;

    std::vector<int> recoL_W_it;
    recoL_W_it.push_back(-1);
    recoL_W_it.push_back(-1);
    
    
    for(unsigned int eleIt = 0; eleIt < treeVars.nRecoE; ++eleIt)
    {
      if( treeVars.recoE_tkIso[eleIt]  / treeVars.recoE_pt[eleIt] > 0.2 ) continue;
      if( treeVars.recoE_emIso[eleIt]  / treeVars.recoE_pt[eleIt] > 0.2 ) continue;
      if( treeVars.recoE_hadIso[eleIt] / treeVars.recoE_pt[eleIt] > 0.2 ) continue;
      if( treeVars.recoE_eleIdRobustTight[eleIt] != 1 ) continue;
      
      recoL_W_it.at(0) = 0;
      recoL_W_it.at(1) = eleIt;
      ++nCandE;
    }
    
    for(unsigned int muIt = 0; muIt < treeVars.nRecoMu; ++muIt)
    {
      if( treeVars.recoMu_tkIsoR03[muIt]  / treeVars.recoMu_pt[muIt] > 0.2 ) continue;
      if( treeVars.recoMu_emIsoR03[muIt]  / treeVars.recoMu_pt[muIt] > 0.2 ) continue;
      if( treeVars.recoMu_hadIsoR03[muIt] / treeVars.recoMu_pt[muIt] > 0.2 ) continue;
      
      recoL_W_it.at(0) = 1;
      recoL_W_it.at(1) = muIt;
      ++nCandMu;
    }
    
    
    if(nCandE + nCandMu < 1) continue;
    
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);    
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it, &recoJet_W_it);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 7
    // --------------------------------------------------------------------------------------
    
    ++step;
    
    if(nCandE + nCandMu > 1) continue;
    
    TLorentzVector recoL_W;
    
    if(nCandE > 0)
    {
      recoL_W.SetXYZT(treeVars.recoE_px[recoL_W_it.at(1)],
                      treeVars.recoE_py[recoL_W_it.at(1)],
                      treeVars.recoE_pz[recoL_W_it.at(1)],
                      treeVars.recoE_energy[recoL_W_it.at(1)]);
    }

    if(nCandMu > 0)
    {
      recoL_W.SetXYZT(treeVars.recoMu_px[recoL_W_it.at(1)],
                      treeVars.recoMu_py[recoL_W_it.at(1)],
                      treeVars.recoMu_pz[recoL_W_it.at(1)],
                      treeVars.recoMu_energy[recoL_W_it.at(1)]);
    }
        
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);    
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it, &recoJet_W_it, &recoL_W_it);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 8
    // --------------------------------------------------------------------------------------
    
    ++step;
    
    TLorentzVector recoLJJ = recoL_W + recoJJ_W;
    
    if( fabs(deltaPhi(recoL_W.Phi(), recoJJ_W.Phi())) > Dphi_LJJ_MAX) continue;
    if( recoLJJ.M() > mass_LJJ_MAX ) continue;
    if( treeVars.muonCorrectedCaloMet < met_MIN ) continue;
    if( treeVars.muonCorrectedCaloMet > met_MAX ) continue;
    
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);    
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it, &recoJet_W_it, &recoL_W_it);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // STEP 9
    // --------------------------------------------------------------------------------------
    
    ++step;
    
    int nRecoJets_central = 0;
    
    for(unsigned int jetIt = 0; jetIt < treeVars.nRecoJet; ++jetIt)
    {
      bool skipJet = false;
      
      for(unsigned int kk = 0; kk < recoJet_tag_it.size(); ++kk)
        if(recoJet_tag_it.at(kk) == static_cast<int>(jetIt)) skipJet = true;
      
      for(unsigned int kk = 0; kk < recoJet_W_it.size(); ++kk)
        if(recoJet_W_it.at(kk) == static_cast<int>(jetIt)) skipJet = true;
      
      if(skipJet) continue;
      
      
      if( fabs(treeVars.recoJet_eta[jetIt]) < absEta_CJV_MAX ) ++ nRecoJets_central;
    }
    
    if(nRecoJets_central > 0) continue;
     
    nEvents[step] += 1;
    if(isSignal) MCHistos[step] -> FillMCHistograms(treeVars);    
    RecoHistos[step] -> FillRecoHistograms(treeVars, &recoJet_tag_it, &recoJet_W_it, &recoL_W_it);
    
  }
  
  
  
  
  
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  for(int step = 1; step < nSteps; ++step)
  {
    events       -> SetBinContent(step+1, nEvents[step]);
    eventsScaled -> SetBinContent(step+1, nEvents[step] / events ->  GetBinContent(1) * crossSection);
    efficiencies -> SetBinContent(step+1, nEvents[step] / events -> GetBinContent(1));
    
    if(isSignal) MCHistos[step] -> WriteMCHistograms(outputRootFile);
    RecoHistos[step] -> WriteRecoHistograms(outputRootFile);
  }
  
  events       -> Write();
  eventsScaled -> Write();
  efficiencies -> Write();
  
  outputRootFile -> Close();
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Print out
  // --------------------------------------------------------------------------------------
  
  //std::cout << ">>>>> SUMMARY <<<<<" << std::endl;
  //std::cout << "Event type = " << eventType << "   with cross-section   sigma = " << crossSection << std::endl;
  //
  //std::cout << "Total events analyzed = " << nEvents_total  
  //          << "   corresponding to    L = " << nEvents_total / crossSection
  //          << std::endl;
  
  return 0;
}

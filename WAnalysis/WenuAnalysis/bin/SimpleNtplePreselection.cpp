#include "PhysicsTools/NtupleUtils/interface/treeReader.h"
#include "PhysicsTools/NtupleUtils/interface/ConfigParser.h"
#include "PhysicsTools/NtupleUtils/interface/ntpleUtils.h"
#include "PhysicsTools/NtupleUtils/interface/readJSONFile.h"

#include <iomanip>

#include "TH1F.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there                                                                                                                               
  if(argc != 2)
  {
    std::cerr << ">>>>> SimpleNtplePreselection::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file                                                                                                                                                          
  parseConfigFile (argv[1]) ;
  
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jsonFileName  = gConfigParser -> readStringOption("Input::jsonFileName");  
    
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX    = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int dataFlag    = gConfigParser -> readIntOption("Options::dataFlag");
  
  int nEleMIN    = gConfigParser -> readIntOption  ("Cuts::nEleMIN");
  float scEtMIN  = gConfigParser -> readFloatOption("Cuts::scEtMIN");
  float eleTkIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEBMAX");
  float eleEmIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEBMAX");
  float eleHadIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEBMAX");
  float eleTkIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEEMAX");
  float eleEmIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEEMAX");
  float eleHadIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEEMAX");
  float eleHOverEEBMAX = gConfigParser -> readFloatOption("Cuts::eleHoverEEBMAX");
  float eleDetaInEBMAX = gConfigParser -> readFloatOption("Cuts::eleDetaInEBMAX");
  float eleDphiInEBMAX = gConfigParser -> readFloatOption("Cuts::eleDphiInEBMAX");
  float eleSigmaIetaIetaEBMAX = gConfigParser -> readFloatOption("Cuts::eleSigmaIetaIetaEBMAX");
  float eleHOverEEEMAX = gConfigParser -> readFloatOption("Cuts::eleHoverEEEMAX");
  float eleDetaInEEMAX = gConfigParser -> readFloatOption("Cuts::eleDetaInEEMAX");
  float eleDphiInEEMAX = gConfigParser -> readFloatOption("Cuts::eleDphiInEEMAX");
  float eleSigmaIetaIetaEEMAX = gConfigParser -> readFloatOption("Cuts::eleSigmaIetaIetaEEMAX");  
  
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  
  
  
  
  
  
  // Get number of events
  std::cout << ">>> SimpleNtplePreselection::Get number of events" << std::endl;
  std::map<int,int> totalEvents = GetTotalEvents("AllPassFilterBegin/totalEvents", inputFileList.c_str());  
  
  std::string L1FilterName = "AllPassFilterL1Filter/totalEvents"; 
  std::map<int,int> L1FilterEvents = GetTotalEvents(L1FilterName.c_str(), inputFileList.c_str());  
  
  std::string GoodVertexFilterName = "AllPassFilterGoodVertexFilter/totalEvents"; 
  std::map<int,int> GoodVertexFilterEvents = GetTotalEvents(GoodVertexFilterName.c_str(), inputFileList.c_str());  
  
  std::string NoScrapingFilterName = "AllPassFilterNoScrapingFilter/totalEvents"; 
  std::map<int,int> NoScrapingFilterEvents = GetTotalEvents(NoScrapingFilterName.c_str(), inputFileList.c_str());  
  
  std::string ElectronFilterName = "AllPassFilterElectronFilter/totalEvents"; 
  std::map<int,int> ElectronFilterEvents = GetTotalEvents(ElectronFilterName.c_str(), inputFileList.c_str());  
  
  
  
  
  
  
  // Get run/LS map from JSON file
  std::cout << ">>> SimpleNtplePreselection::Get run/LS map from JSON file" << std::endl;
  std::map<int, std::vector<std::pair<int, int> > > jsonMap;
  if(dataFlag == 1) jsonMap = readJSONFile(jsonFileName);
  
  
  
  
  
  
  // Open old tree
  std::cout << ">>> SimpleNtplePreselection::Open old tree" << std::endl;
  std::string treeName = "simpleNtple/SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain), false);
  
  
  
  // Open output root file for clone tree
  TFile outputRootFile((outputRootFilePath+outputRootFileName).c_str(), "RECREATE");
  outputRootFile.cd();
  
  TTree* cloneTree = chain -> CloneTree(0);
  
  
  
  // define histograms
  std::cout << ">>> SimpleNtplePreselection::Define histograms" << std::endl;
  int nStep = 9;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;

  int step = 1;
  stepEvents[step] = totalEvents[1];
  stepName[step] = "total events";

  step = 2;
  stepEvents[step] = L1FilterEvents[1];
  stepName[step] = "L1Filter";
  
  step = 3;
  stepEvents[step] = GoodVertexFilterEvents[1];
  stepName[step] = "GoodVertexFilter";
  
  step = 4;
  stepEvents[step] = NoScrapingFilterEvents[1];
  stepName[step] = "NoScrapingFilter";
  
  step = 5;
  stepEvents[step] = ElectronFilterEvents[1];
  stepName[step] = "ElectronFilter";
  
  
  
  
  
  
  // Loop over events
  std::cout << ">>>>> SimpleNtplePreselection::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> SimpleNtplePreselection::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    //***********************
    // STEP 6 - HLT selection
    step = 6;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "HLT";
    
    bool skipEvent = true;
    
    std::vector<std::string> HLT_names = *(reader.GetString("HLT_Names"));
    for(unsigned int HLTIt = 0; HLTIt < HLT_names.size(); ++HLTIt)
    {
      if( (reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Photon15_L1R") &&
	  (reader.GetFloat("HLT_Accept")->at(HLTIt) == 1) )
        skipEvent = false;
      
      if( (reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Ele15_LW_L1R") &&
          (reader.GetFloat("HLT_Accept")->at(HLTIt) == 1) )
        skipEvent = false;
      
      if( (reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Ele15_SW_L1R") &&
           (reader.GetFloat("HLT_Accept")->at(HLTIt) == 1) )
        skipEvent = false;
      
      if( (reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Ele15_SW_CaloEleId_L1R") &&
          (reader.GetFloat("HLT_Accept")->at(HLTIt) == 1) )
        skipEvent = false;
    }
    
    
    if( skipEvent == true ) continue;
    stepEvents[step] += 1;    
    
    
    
    
    
    
    //**************************
    // STEP 7 - run/LS selection
    step = step+1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "run/LS";    
    
    skipEvent = false;
    if(dataFlag == 1)
    {
      int runId  = reader.GetInt("runId")->at(0);
      int lumiId = reader.GetInt("lumiId")->at(0);
      if(AcceptEventByRunAndLumiSection(runId, lumiId, jsonMap) == false) skipEvent = true;      
    }
    
    if( skipEvent == true ) continue;
    stepEvents[step] += 1;
    
    
    
    
    
    
    //*************************
    // STEP 8 - cut on electron
    step = step+1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "electron selection";
    
    int nEle = 0;
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      // SC Et
      if( reader.GetFloat("electrons_scEt")->at(eleIt) < scEtMIN ) continue;
      
      // isolation + eleId
      if( (reader.GetInt("electrons_isEB")->at(eleIt)) == 1 )
      {      
        if( (reader.GetFloat("electrons_tkIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > eleTkIsoOverPtEBMAX ) continue;
        if( (reader.GetFloat("electrons_emIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > eleEmIsoOverPtEBMAX ) continue;
        if( ( (reader.GetFloat("electrons_hadIsoR03_1")->at(eleIt)) +
              (reader.GetFloat("electrons_hadIsoR03_2")->at(eleIt)) ) / reader.Get4V("electrons")->at(eleIt).pt() > eleHadIsoOverPtEBMAX ) continue;
        if( (reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt)) > eleSigmaIetaIetaEBMAX ) continue;
        if( fabs((reader.GetFloat("electrons_deltaPhiIn")->at(eleIt))) > eleDphiInEBMAX ) continue;
        if( fabs((reader.GetFloat("electrons_deltaEtaIn")->at(eleIt))) > eleDetaInEBMAX ) continue;
        if( (reader.GetFloat("electrons_hOverE")->at(eleIt)) > eleHOverEEBMAX ) continue;
      }
      
      else
      {      
        if( (reader.GetFloat("electrons_tkIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > eleTkIsoOverPtEEMAX ) continue;
        if( (reader.GetFloat("electrons_emIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > eleEmIsoOverPtEEMAX ) continue;
        if( ( (reader.GetFloat("electrons_hadIsoR03_1")->at(eleIt)) +
              (reader.GetFloat("electrons_hadIsoR03_2")->at(eleIt)) ) / reader.Get4V("electrons")->at(eleIt).pt() > eleHadIsoOverPtEEMAX ) continue;
        if( (reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt)) > eleSigmaIetaIetaEEMAX ) continue;
        if( fabs((reader.GetFloat("electrons_deltaPhiIn")->at(eleIt))) > eleDphiInEEMAX ) continue;
        if( fabs((reader.GetFloat("electrons_deltaEtaIn")->at(eleIt))) > eleDetaInEEMAX ) continue;
        if( (reader.GetFloat("electrons_hOverE")->at(eleIt)) > eleHOverEEEMAX ) continue;
      }
      
      
      
      // conversion rejection
      if( reader.GetInt("electrons_mishits")->at(eleIt) > 0 ) continue;
      if( reader.GetInt("electrons_nAmbiguousTracks")->at(eleIt) > 0 ) continue;
      if( ( fabs(reader.GetFloat("electrons_dist")->at(eleIt)) < 0.02 ) &&
          ( fabs(reader.GetFloat("electrons_dcot")->at(eleIt)) < 0.02 ) ) continue;
      
      
      
      // spike removal
      if( reader.GetInt("electrons_seedSeverityLevel")->at(eleIt) != 0 ) continue;
      if( reader.GetInt("electrons_seedFlag")->at(eleIt) != 0 ) continue;
      
      
      
      ++nEle;
    }
    
    if( nEle < nEleMIN ) continue;
    stepEvents[step] += 1;
    
    
    
    
    
    
    //********************
    // STEP 9 - cut on met
    step = step+1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "met selection";
    
    if( (reader.Get4V("PFMet")->at(0)).Et() < metEtMIN ) continue;
    
    stepEvents[step] += 1;
    
    
    
    
    
    
    //***********
    // CLONE TREE
    
    cloneTree -> Fill();
    
  } // loop over the events
  
  
  
  
  
  
  // save histograms
  for(step = 1; step <= nStep; ++step)
  {
    events -> SetBinContent(step, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step, stepName[step].c_str());
  }
  
  events -> Write();
  
  cloneTree -> AutoSave();
  outputRootFile.Close();
  
  
  
  return 0;
}

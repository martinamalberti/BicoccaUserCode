import FWCore.ParameterSet.Config as cms

def makeSimpleNtple(process, GlobalTag, MC=False, HLT='HLT'):
    # Setup the process
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
        # fileMode = cms.untracked.string('MERGE'),
    )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag

    # save important information into the ntuple
    process.load("HiggsAnalysis.littleH.SimpleNtple_cfi")
    process.makeNtple = cms.EDAnalyzer('SimpleNtple',
         Onia2MuMuTag            = cms.InputTag("onia2MuMuPat"),
         Onia2EleEleTag          = cms.InputTag("onia2EleElePat"),
         MuTag                   = cms.InputTag("patMuons"),
         EleTag                  = cms.InputTag("patElectrons"),  # pixelMatchGsfElectrons
         PrimaryVertexTag        = cms.InputTag("offlinePrimaryVertices"),
         beamSpotTag             = cms.InputTag("offlineBeamSpot"),
         MCtruthTag              = cms.InputTag("genLeptons"),
         eleIDCut_LooseInputTag  = cms.string("eidLoose"),
         eleIDCut_RLooseInputTag = cms.string("eidRobustLoose"),
         eleIDCut_TightInputTag  = cms.string("eidTight"), 
         eleIDCut_RTightInputTag = cms.string("eidRobustTight"),
    
         saveEvt    = cms.untracked.bool (not MC) ,
         saveVtx    = cms.untracked.bool (True) ,
         saveMu     = cms.untracked.bool (True) ,
         saveEle    = cms.untracked.bool (True) ,
         saveMC     = cms.untracked.bool (MC) ,
         saveTrigger     = cms.untracked.bool (True) ,
         saveBeamSpot    = cms.untracked.bool (True) ,
         saveOniaCand    = cms.untracked.bool (True) ,
         
         oniaMaxCat    = cms.untracked.int32(2) ,
             
         treeName = cms.untracked.string ("OniaTree") ,
         verbosity = cms.untracked.bool(False),
         eventType = cms.untracked.int32(0), # 2 for data, 1 for signal, 0 for bkg
    
    )
    
    
    # save essential HLT infos
    process.load("PhysicsTools.NtupleUtils.HLTrigResultsDumper_cfi")
    process.TriggerResults = process.HLTrigResultsDumper.clone()
    process.TriggerResults.HLTriggerResults = cms.InputTag("TriggerResults","",HLT)
    process.TriggerResults.HLTPaths = cms.vstring(
      "HLT_Mu3", 
      "HLT_Mu5", 
      "HLT_Mu9", 
      "HLT_DoubleMu0", 
      "HLT_DoubleMu3",
      "HLT_Ele10_LW_L1R", 
      "HLT_DoubleEle5_SW_L1R",
      "HLT_DoubleEle4_SW_eeRes_L1R"
    )   # provide list of HLT paths (or patterns) you want

    
    # Make PAT+Ntuple path
    process.makeNtpleSequence = cms.Path(
        process.makeNtple *
        process.TriggerResults
    )
      

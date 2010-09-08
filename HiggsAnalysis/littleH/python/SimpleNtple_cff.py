import FWCore.ParameterSet.Config as cms

def makeSimpleNtple(process, GlobalTag, MC=False, HLT='HLT', HLT_filter_ele=False, HLT_filter_mu=False):
    # Setup the process
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
        # fileMode = cms.untracked.string('MERGE'),
    )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.Reconstruction_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    # Drop the DQM stuff on input
    process.source = cms.Source("PoolSource",
        inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*"),
        fileNames = cms.untracked.vstring()
    )

    #from PhysicsTools.NtupleUtils.AllPassFilter_cfi import *
    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")
    #Counter1
    process.AllEvents = process.AllPassFilter.clone()
 
    
    #HLT Filter to avoid double counting when running on Mu and EGamma datasets
    # filter events which have passed any of Mu HLTs
    process.load('HLTrigger/HLTfilters/hltHighLevel_cfi')
    process.hltHighLevelFilterMu = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","",HLT),
     HLTPaths = cms.vstring('HLT_*Mu*'),
     #HLTPaths = cms.vstring('HLT_MinBias*'),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True),
     throw = cms.bool(True)
    ) 
    
    # filter events which have passed any of EGamma HLTs
    process.load('HLTrigger/HLTfilters/hltHighLevel_cfi')
    process.hltHighLevelFilterEle = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","",HLT),
     HLTPaths = cms.vstring('HLT_*Photon*','HLT_*Ele*'),
     #HLTPaths = cms.vstring('HLT_MinBias*'),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True),
     throw = cms.bool(True)
    ) 
    
    #Counter2
    process.TriggeredEvents = process.AllPassFilter.clone()

    # Filter dimuon candidates - requirement on PV-SV compatibility (a0 does not fly)
    process.selectedOnia2MuMuPatTrkTrk = cms.EDFilter("CandViewSelector",
        src = cms.InputTag('onia2MuMuPatTrkTrk'),
        cut = cms.string(" userFloat('vProb') > 0.05 & -4 < userFloat('ppdlPV')/userFloat('ppdlErrPV') < 4 "),
    )

    # Filter diele candidates - requirement on PV-SV compatibility (upsilons do not fly)
    process.selectedOnia2EleElePat = cms.EDFilter("CandViewSelector",
        src = cms.InputTag('onia2EleElePat'),
        cut = cms.string(" userFloat('vProb') > 0.05 & -4 < userFloat('ppdlPV')/userFloat('ppdlErrPV') < 4 "),
    )
    
    # merge dimuons and dielectrons into leptons
    process.selectedOnia2LepLep = cms.EDProducer("CandViewMerger",
        src = cms.VInputTag('selectedOnia2MuMuPatTrkTrk','selectedOnia2EleElePat')
    )
    
    # check if there is at least one (inclusive) onia candidate surviving the above cuts
    process.selectedOnia2LepLepFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('selectedOnia2LepLep'),
        minNumber = cms.uint32(1),
    )
    
    #Counter3
    process.OniaEvents = process.AllPassFilter.clone()

    # save important information into the ntuple
    process.load("HiggsAnalysis.littleH.SimpleNtple_cfi")
    process.makeNtple = cms.EDAnalyzer('SimpleNtple',
         Onia2MuMuTag            = cms.InputTag("onia2MuMuPatTrkTrk"),
         Onia2EleEleTag          = cms.InputTag("onia2EleElePat"),
         MuTag                   = cms.InputTag("patMuons"),
         EleTag                  = cms.InputTag("patElectrons"),  # pixelMatchGsfElectrons
         TracksTag               = cms.InputTag("generalTracks"),
         PrimaryVertexTag        = cms.InputTag("offlinePrimaryVertices"),
         beamSpotTag             = cms.InputTag("offlineBeamSpot"),
         MCtruthTag              = cms.InputTag("genLeptons"),
         eleIDCut_LooseInputTag  = cms.string("eidLoose"),
         eleIDCut_RLooseInputTag = cms.string("eidRobustLoose"),
         eleIDCut_TightInputTag  = cms.string("eidTight"), 
         eleIDCut_RTightInputTag = cms.string("eidRobustTight"),
    
         saveVtx    = cms.untracked.bool (True) ,
         saveMu     = cms.untracked.bool (True) ,
         saveTracks = cms.untracked.bool (True) , 
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
    process.TriggerResults.HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
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
        process.AllEvents *
        ~process.hltHighLevelFilterMu *         #exclude events triggered by muons
        ~process.hltHighLevelFilterEle *         #exclude events triggered by ECAL
        process.TriggeredEvents *
        process.selectedOnia2MuMuPatTrkTrk *
        process.selectedOnia2EleElePat *
        process.selectedOnia2LepLep *
        process.selectedOnia2LepLepFilter *
        process.OniaEvents *
        process.makeNtple *
        process.TriggerResults
    )
    if not HLT_filter_ele:
      process.makeNtpleSequence.remove(~process.hltHighLevelFilterEle)
    if not HLT_filter_mu:
      process.makeNtpleSequence.remove(~process.hltHighLevelFilterMu)
      
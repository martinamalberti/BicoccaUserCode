import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *


def makeMiBiCommonPAT(process, GlobalTag, MC=False, Filter=False, SavePAT=True):

    # Setup the process
    process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 10
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag

    
    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    # Output
    process.out = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
        outputCommands = cms.untracked.vstring(),
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('TwoPhotonsPath','OneLeptonTwoJetsPath') ) if Filter else cms.untracked.PSet()
        )

    if SavePAT :
        process.e = cms.EndPath(process.out)


     

    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")

    #--------------------------
    # Counter1: All read events
    process.AllEvents = process.AllPassFilter.clone()
    


    #--------------------------
    # Counter2: Scraping filter
    process.scrapingFilter = cms.EDFilter("FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
    )
    
    process.NonScrapedEvents = process.AllPassFilter.clone()
    
    
    
    #-----------------------------------
    # Counter3: Filter on primary vertex
    process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
        minimumNDOF = cms.uint32(4) ,
        maxAbsZ = cms.double(24),
        maxd0 = cms.double(2)
    )
    
    process.GoodVtxEvents = process.AllPassFilter.clone()
    
    



    
    #------------------
    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    process.load("PhysicsTools.PatAlgos.tools.pfTools")
    postfix = "PFlow"
    usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=MC, postfix=postfix)  
    
    if not MC:
     removeMCMatching(process, ['All'])
    
    process.patJets.addTagInfos = cms.bool(False)    #bugfix related to btagging
    
    
    
    # ---------------
    # add collections
    addTcMET(process, 'TC')
    addPfMET(process, 'PF')
    
    addJetCollection(
        process,
        cms.InputTag('ak5CaloJets'),
        'AK5',
        'Calo',
        doJTA        = True,
        doBTagging   = True,
        jetCorrLabel = ('AK5', 'Calo'),
        doType1MET   = True,
        doL1Cleaning = True,
        doL1Counters = False,
        genJetCollection=cms.InputTag("ak5GenJets"),
        doJetID      = True
        )
    
    addJetCollection(
        process,
        cms.InputTag('ak5PFJets'),
        'AK5',
        'PF',
        doJTA        = True,
        doBTagging   = True,
        jetCorrLabel = ('AK5', 'PF'),
        doType1MET   = False,
        doL1Cleaning = True,
        doL1Counters = False,
        genJetCollection=cms.InputTag("ak5GenJets"),
        doJetID      = True
        )
    
    

    #---------
    #PG PF2PAT sequence from /CMSSW/PhysicsTools/PatAlgos/test/patTuple_PATandPF2PAT_cfg.py
    # An empty postfix means that only PF2PAT is run,
    # otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
    # collections have standard names + postfix (e.g. patElectronPFlow)  
    
    
    
    # -------------------
    # pat selection layer
    process.selectedPatElectrons.cut      = cms.string("pt() > 15.")
    process.selectedPatElectronsPFlow.cut = cms.string("pt() > 15.")    

    process.selectedPatMuons.cut      = cms.string("pt() > 15.")
    process.selectedPatMuonsPFlow.cut = cms.string("pt() > 15.")

    process.selectedPatJets.cut        = cms.string("pt() > 15.")
    process.selectedPatJetsPFlow.cut   = cms.string("pt() > 15.")    
    process.selectedPatJetsAK5Calo.cut = cms.string("pt() > 15.")
    process.selectedPatJetsAK5PF.cut   = cms.string("pt() > 15.")

    process.selectedPatPhotons.cut      = cms.string("pt() > 10.")
    process.selectedPatPhotonsPFlow.cut = cms.string("pt() > 10.")    

    #process.selectedPatTaus.cut      = cms.string("pt() > 10.")
    #process.selectedPatTausPFlow.cut = cms.string("pt() > 10.")    
    
    
    
    
    

    #--------
    # Filters
    #process.PhotonFilter = cms.EDFilter(
    #    "CandViewCountFilter",
    #     src = cms.InputTag("selectedPatElectrons"),
    #     minNumber = cms.uint32(2)
    #     )

    #process.ElectronFilter = cms.EDFilter(
    #    "CandViewCountFilter",
    #     src = cms.InputTag("selectedPatElectrons"),
    #     minNumber = cms.uint32(1)
    #     )

    #process.MuonFilter = cms.EDFilter(
    #    "CandViewCountFilter",
    #     src = cms.InputTag("selectedPatMuons"),
    #     minNumber = cms.uint32(1)
    #     )
    
    #process.JetFilter = cms.EDFilter(
    #    "CandViewCountFilter",
    #     src = cms.InputTag("selectedPatJets"),
    #     minNumber = cms.uint32(2)
    #     )    


    
    #process.OneLeptonTwoJetsPath = cms.Path(
    #    process.ElectronFilter*
    #    process.JetFilter 
    #    )

    #process.TwoPhotonsPath = cms.Path(
    #    process.PhotonFilter
    #    )
    
    
    
    
    # the HCAL Noise Filter
    #process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
    
    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Path(
        process.AllEvents * # -> Counter
        process.scrapingFilter *
        process.NonScrapedEvents * # -> Counter
        process.primaryVertexFilter *
        process.GoodVtxEvents * # -> Counter
        getattr(process,"patPF2PATSequence"+postfix) *
        process.patDefaultSequence
    )



    #process.MiBiSchedule = cms.Schedule([process.MiBiCommonPAT,process.OneLeptonTwoJetsPath,process.TwoPhotonsPath])

    
    
    process.out.outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_*_*_*PAT',                             # All PAT objects
        'keep *_offlinePrimaryVertices*_*_*',          # Primary vertices: you want these to compute impact parameters
        'keep *_offlineBeamSpot_*_*',                  # Beam spot: you want this for the same reason
        'keep edmTriggerResults_TriggerResults_*_*',   # HLT info, per path (cheap)
        'keep *_genParticles_*_*',                     # GenParticles
        'keep *_generalTracks_*_*',                    # tracks
        'keep recoGsfElectronCores_*_*_*'              # gsfElectrons
    )
    

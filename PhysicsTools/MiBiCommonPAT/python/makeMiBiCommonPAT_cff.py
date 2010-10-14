import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *



def makeMiBiCommonPAT(process, GlobalTag, MC=False, HLT='HLT', Filter=False, SavePAT=True):
    # Setup the process
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
    )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 10
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    # Drop the DQM stuff on input
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")
    #Counter1:All red events
    process.AllEvents = process.AllPassFilter.clone()

    # Scraping filter
    process.scrapingFilter = cms.EDFilter("FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
    )
    
    #Counter2:Non scraped events
    process.NonScrapedEvents = process.AllPassFilter.clone()
    
    # filter on primary vertex
    process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
        minimumNDOF = cms.uint32(4) ,
        maxAbsZ = cms.double(24),
        maxd0 = cms.double(2)
    )
    
    #Counter3:Good vertex events
    process.GoodVtxEvents = process.AllPassFilter.clone()
    
    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")

#    process.selectedPatMuons.cut = cms.string("pt > 5")
#    process.selectedPatElectrons.cut = cms.string("pt > 10")
#    process.selectedPatTaus = cms.string("pt > 10")
#    process.selectedPatPhotons = cms.string("pt > 10")
#    process.selectedPatJets = cms.string("pt > 10")


    #Bufix related to btagging
    process.patJets.addTagInfos = cms.bool(False)

    # Add tcMET and pfMET
#    addTcMET(process, 'TC')
#    addPfMET(process, 'PF')
    
# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATTools#Jet_Tools

#    addJetCollection(
#        process,
#        cms.InputTag('ak5CaloJets'),
#        'AK5',
#        'Calo',
#        doJTA        = True,
#        doBTagging   = True,
#        jetCorrLabel = ('AK5', 'Calo'),
#        doType1MET   = True,
#        doL1Cleaning = True,
#        doL1Counters = False,
#        genJetCollection=cms.InputTag("ak5GenJets"),
#        doJetID      = True
#        )
    
#    addJetCollection(
#        process,
#        cms.InputTag('ak5PFJets'),
#        'AK5',
#        'PF',
#        doJTA        = True,
#        doBTagging   = True,
#        jetCorrLabel = ('AK5', 'PF'),
#        doType1MET   = False,
#        doL1Cleaning = True,
#        doL1Counters = False,
#        genJetCollection=cms.InputTag("ak5GenJets"),
#        doJetID      = True
#        )
    
    
    if not MC:
     removeMCMatching(process, ['All'])
    
    

    # the HCAL Noise Filter
    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
    
    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Path(
        process.AllEvents * # -> Counter
        process.scrapingFilter *
        process.NonScrapedEvents * # -> Counter
        process.primaryVertexFilter *
        process.GoodVtxEvents * # -> Counter
        process.patDefaultSequence 
    )
    
    #output
    process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
        outputCommands = cms.untracked.vstring(
            'drop *',
            #'keep *_Particles_*_Onia2LepLepPAT',           # generated leptons and parents
            'keep *_*_*_MiBiCommonPAT',                    # All PAT muons including general tracks
            #'keep *_offlinePrimaryVertices_*_*',           # Primary vertices: you want these to compute impact parameters
            #'keep *_offlineBeamSpot_*_*',                  # Beam spot: you want this for the same reason                                   
            #'keep edmTriggerResults_TriggerResults_*_*',   # HLT info, per path (cheap)
            #'keep recoGsfElectronCores_*_*_*'
        ),
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('MiBiCommonPAT') ) if Filter else cms.untracked.PSet()
    )
    if SavePAT :
     process.e = cms.EndPath(process.out)


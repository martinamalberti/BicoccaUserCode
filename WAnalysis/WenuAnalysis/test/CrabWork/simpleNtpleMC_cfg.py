import FWCore.ParameterSet.Config as cms

process = cms.Process("simpleNtpleMC")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START38_V14::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(100000)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        #/EG/Run2010A-Sep17ReReco_v2/RECO
        #'/store/data/Run2010A/EG/RECO/Sep17ReReco_v2/0030/004BCF4D-40C6-DF11-8235-002481E14D72.root'
        'file:/tmp/amassiro/D056A87C-66D0-DF11-AAE1-0018FE286DB2.root'
        )
    )

process.out = cms.OutputModule(
      "PoolOutputModule",
      fileName = cms.untracked.string('file:PAT.root'),
      outputCommands = cms.untracked.vstring()
  )



#--------------------------
#Define PAT sequence
#--------------------------

# Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.coreTools import *
## remove MC matching from the default sequence
removeMCMatching(process, ['All'])


##
#process.patElectrons.addGenMatch = cms.bool(False)
#process.patElectrons.embedGenMatch = cms.bool(False)

# Add tcMET and pfMET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process, 'PF')


# get the jet corrections
##from PhysicsTools.PatAlgos.tools.jetTools import *
##switchJECSet( process, "Summer09_7TeV_ReReco332")

#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
## uncomment this line to run on an 35X input sample
#run36xOn35xInput(process)



#--------------------------
# AllPassFilter
#--------------------------

process.load('PhysicsTools.NtupleUtils.AllPassFilter_cfi')

process.AllPassFilterBegin = process.AllPassFilter.clone()
process.AllPassFilterL1Filter = process.AllPassFilter.clone()
process.AllPassFilterGoodVertexFilter = process.AllPassFilter.clone()
process.AllPassFilterNoScrapingFilter = process.AllPassFilter.clone()
process.AllPassFilterElectronFilter = process.AllPassFilter.clone()

#--------------------------
# Ntple
#--------------------------

process.simpleNtple = cms.EDAnalyzer(
    'SimpleNtple',
    recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    EleTag              = cms.InputTag("patElectrons"),
    JetTag              = cms.InputTag("patJets"),
    TrackTag            = cms.InputTag("generalTracks"),
    CALOMetTag          = cms.InputTag("patMETs"),
    TCMetTag            = cms.InputTag("patMETsTC"),
    PFMetTag            = cms.InputTag("patMETsPF"),
    MuTag               = cms.InputTag("patMuons"),
    HLTTag              = cms.InputTag("TriggerResults::RECO"),
    L1Tag               = cms.InputTag("gtDigis"),
    #eleId_names         = cms.untracked.vstring('eidLoose','eidMedium','eidSuperTight','eidTight','eidVeryLoose'),
    saveL1        = cms.untracked.bool(True),
    saveHLT       = cms.untracked.bool(True),
    saveMu        = cms.untracked.bool(True),
    saveEle       = cms.untracked.bool(True),
    saveEleShape  = cms.untracked.bool(True),
    saveJet       = cms.untracked.bool(True),
    saveCALOMet   = cms.untracked.bool(True),
    saveTCMet     = cms.untracked.bool(True),
    savePFMet     = cms.untracked.bool(True),
    verbosity_    = cms.untracked.bool(False) #---- true = loquacious     false = silence
    )



process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("simpleNtple.root")
    )



#--------------------------
# filters
#--------------------------

# filter on PhysDeclared bit
process.skimming = cms.EDFilter(
    "PhysDecl",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    HLTriggerResults = cms.InputTag("TriggerResults","","RECO")
    )

# filter on bit = and (40 || 41) and !(bit36 || bit37 || bit38 || bit39)
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# filter on primary vertex
process.primaryVertexFilter = cms.EDFilter(
    "GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(4) ,
    maxAbsZ = cms.double(24),
    maxd0 = cms.double(2)
    )

# FilterOutScraping
process.noscraping = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )

# select events with at least one gsf electron
process.highetele = cms.EDFilter(
    "GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("superCluster().get().energy()*sin(theta())> 0 ")
    )

process.highetFilter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("highetele"),
    minNumber = cms.uint32(1)
    )



#--------------------------
# paths
#--------------------------

process.p = cms.Path(
    process.AllPassFilterBegin
#    *process.skimming
    *process.AllPassFilterL1Filter
#    *process.hltLevel1GTSeed
#    *process.AllPassFilterGoodVertexFilter    
#    *process.primaryVertexFilter
#    *process.AllPassFilterNoScrapingFilter    
#    *process.noscraping
    *process.highetele
    *process.highetFilter
    *process.AllPassFilterElectronFilter    
    *process.patDefaultSequence
    *process.simpleNtple
    )



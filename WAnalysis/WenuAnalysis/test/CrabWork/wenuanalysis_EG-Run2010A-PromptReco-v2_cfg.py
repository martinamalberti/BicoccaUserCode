import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START3X_V26A::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
   '/store/relval/CMSSW_3_5_7/RelValWE/GEN-SIM-RECO/START3X_V26-v1/0012/EC9F278B-6949-DF11-99F2-003048678B0E.root'
                                                )
)





#Define PAT sequence
# Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.coreTools import *
## remove MC matching from the default sequence
removeMCMatching(process, ['All'])

# add cIc electron ID
process.load("WAnalysis.WenuAnalysis.CiC_eIDSequence_cff")

process.patElectronIDs   = cms.Sequence(process.CiC_eIDSequence)
process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.patElectrons)

process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
    eidVeryLoose  = cms.InputTag("eidVeryLoose"),
    eidLoose      = cms.InputTag("eidLoose"),
    eidMedium     = cms.InputTag("eidMedium"),
    eidTight      = cms.InputTag("eidTight"),
    eidSuperTight = cms.InputTag("eidSuperTight")
    )
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

from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
## uncomment this line to run on an 35X input sample
#run36xOn35xInput(process)


#Analysis
process.myanalysis = cms.EDAnalyzer('WenuTree',
                                    
   recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
   recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
   electronTag         = cms.InputTag("patElectrons"),
   jetTag              = cms.InputTag("patJets"),
   calometTag          = cms.InputTag("patMETs"),
   tcmetTag            = cms.InputTag("patMETsTC"),
   pfmetTag            = cms.InputTag("patMETsPF"),
   muonTag             = cms.InputTag("patMuons"),
   electronID          = cms.untracked.string("eidTight"),
   btagAlgo            = cms.untracked.string("jetBProbabilityBJetTags"),
   HLTInputTag         = cms.InputTag("TriggerResults::HLT"),
   L1InputTag          = cms.InputTag("gtDigis"),

   runOnMC             = cms.bool(False),                     
   storePDFWeights     = cms.bool(False),
   pdfWeightsTag       = cms.InputTag("pdfWeights:cteq65")                            
)


process.TFileService = cms.Service("TFileService",
   fileName = cms.string("treeWenu_EG-Run2010A-PromptReco-v2.root")
)



#filters

# filter on PhysDeclared bit
process.skimming = cms.EDFilter("PhysDecl",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
)

# filter on bit = and (40 || 41) and !(bit36 || bit37 || bit38 || bit39)
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# filter on primary vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
   vertexCollection = cms.InputTag('offlinePrimaryVertices'),
   minimumNDOF = cms.uint32(4) ,
   maxAbsZ = cms.double(15),
   maxd0 = cms.double(2)
)

# FilterOutScraping
process.noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)

# select events with at least one gsf electron
process.highetele = cms.EDFilter("GsfElectronSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string("superCluster().get().energy()*sin(theta())> 0 ")
)

process.highetFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag("highetele"),
   minNumber = cms.uint32(1)
)


# 
process.p = cms.Path(
    process.skimming
    *process.hltLevel1GTSeed
    *process.noscraping
    *process.primaryVertexFilter
    #*process.highetele
    #*process.highetFilter
    *process.patDefaultSequence
    *process.myanalysis
    )



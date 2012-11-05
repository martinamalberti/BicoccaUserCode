# Auto generated configuration file
# using: 
# Revision: 1.222.2.6 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,L1Reco,RECO --data --datatier RECO --eventcontent RECO --conditions GR10_P_V11::All --scenario pp --no_exec --magField AutoFromDBCurrent --process reRECO --customise Configuration/GlobalRuns/reco_TLR_38X.py --cust_function customisePPData --filein=outSkim_1_1_GlC.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('EcalAlignment')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/amassiro/2EB0D0B7-C275-E111-8CFB-001A64789E04.root')
  )
#process.source.inputCommands = cms.untracked.vstring("drop *_*_*_RECO", "drop *_MEtoEDMConverter_*_*", "keep FEDRawDataCollection_*_*_*")

# Output definition


process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('outSkim_V3.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

process.out = cms.OutputModule("PoolOutputModule",
  outputCommands = cms.untracked.vstring(
#'keep *',
       'drop *'
#'keep *_gsfElectrons_*_*'
#'keep *_*_*_EcalAlignmentBIS'
#'drop *_gsfElectron_*_*',
#'drop *_gsfElectronCores_*_*'
        ),
  fileName = cms.untracked.string("outCMSSW.root")
)

  
# Additional output definition

# Other statements
#process.GlobalTag.globaltag = 'GR_R_38X_V13::All'
#process.GlobalTag.globaltag = 'GR_R_39X_V5::All'
#process.GlobalTag.globaltag = 'MC_39Y_V7::All'
process.GlobalTag.globaltag = 'START52_V11C::All'

################################################################################
################################################################################
################################################################################
################## create ntuple for ECAL alignment purposes ###################


#--------------------------
#Define PAT sequence
#--------------------------

# Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.coreTools import *
## remove MC matching from the default sequence
removeMCMatching(process, ['All'])


#process.patElectrons.electronSource = cms.InputTag("gsfElectrons::EcalAlignment")

process.patElectrons.addElectronID = cms.bool(False)

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



#--------------------------
# AllPassFilter
#--------------------------

process.AllEvents                      = cms.EDProducer("EventCountProducer")
process.FilterL1FilterEvents           = cms.EDProducer("EventCountProducer")
process.FilterGoodVertexFilterEvents   = cms.EDProducer("EventCountProducer")
process.FilterNoScrapingFilterEvents   = cms.EDProducer("EventCountProducer")
process.FilterElectronFilterEvents     = cms.EDProducer("EventCountProducer")
process.FilterReRECOEvents             = cms.EDProducer("EventCountProducer")
process.FilterPatDefaultSequenceEvents = cms.EDProducer("EventCountProducer")


#--------------------------
# Z skim
#--------------------------

#### cut on electron tag
#ELECTRON_ET_CUT_MIN_TIGHT = 20.0
#ELECTRON_ET_CUT_MIN_LOOSE = 10.0

#MASS_CUT_MIN = 40.
###    _____ _           _                     ___    _ 
###   | ____| | ___  ___| |_ _ __ ___  _ __   |_ _|__| |
###   |  _| | |/ _ \/ __| __| '__/ _ \| '_ \   | |/ _` |
###   | |___| |  __/ (__| |_| | | (_) | | | |  | | (_| |
###   |_____|_|\___|\___|\__|_|  \___/|_| |_| |___\__,_|
###   
## Electron ID  ######
#from DPGAnalysis.Skims.WElectronSkim_cff import *

#process.PassingVeryLooseId = goodElectrons.clone(
#cut = cms.string(
#goodElectrons.cut.value() +
#" && (gsfTrack.trackerExpectedHitsInner.numberOfHits<=1 )" #wrt std WP90 allowing 1 numberOfMissingExpectedHits 
#" && (ecalEnergy*sin(superClusterPosition.theta)>" + str(ELECTRON_ET_CUT_MIN_LOOSE) + ")"
#" && ((isEB"
#" && ( dr03TkSumPt/p4.Pt <0.2 && dr03EcalRecHitSumEt/p4.Pt < 0.3 && dr03HcalTowerSumEt/p4.Pt  < 0.3 )"
#" && (sigmaIetaIeta<0.012)"
#" && ( -0.8<deltaPhiSuperClusterTrackAtVtx<0.8 )"
#" && ( -0.01<deltaEtaSuperClusterTrackAtVtx<0.01 )"
#" && (hadronicOverEm<0.15)"
#")"
#" || (isEE"
#" && ( dr03TkSumPt/p4.Pt <0.2 && dr03EcalRecHitSumEt/p4.Pt < 0.3 && dr03HcalTowerSumEt/p4.Pt  < 0.3 )"
#" && (sigmaIetaIeta<0.033)"
#" && ( -0.7<deltaPhiSuperClusterTrackAtVtx<0.7 )" 
#" && ( -0.01<deltaEtaSuperClusterTrackAtVtx<0.01 )"
#" && (hadronicOverEm<0.15) "
#"))"
#)
#)
 
#process.PassingTightId = process.PassingVeryLooseId.clone(
#cut = cms.string(
#process.PassingVeryLooseId.cut.value() +
#" && (ecalEnergy*sin(superClusterPosition.theta)>" + str(ELECTRON_ET_CUT_MIN_TIGHT) + ")"
#)
#)

#process.Zele_sequence = cms.Sequence(
#process.PassingVeryLooseId
#*process.PassingTightId 
#)

###    _____ ___   ____    ____       _          
###   |_   _( _ ) |  _ \  |  _ \ __ _(_)_ __ ___ 
###     | | / _ \/\ |_) | | |_) / _` | | '__/ __|
###     | || (_>  <  __/  |  __/ (_| | | |  \__ \
###     |_| \___/\/_|     |_|   \__,_|_|_|  |___/
###                                              
###   
#import copy

#process.tagGsf = cms.EDProducer("CandViewShallowCloneCombiner",
##    decay = cms.string("PassingWP90 goodElectrons"),
## decay = cms.string("PassingVeryLooseId PassingVeryLooseId"),
#decay = cms.string("PassingTightId PassingVeryLooseId"),
#checkCharge = cms.bool(False), 
#cut   = cms.string("mass > " + str(MASS_CUT_MIN))
#)
#process.tagGsfCounter = cms.EDFilter("CandViewCountFilter",
#src = cms.InputTag("tagGsf"),
#minNumber = cms.uint32(1)
#)
#process.tagGsfFilter = cms.Sequence(process.tagGsf * process.tagGsfCounter)
#process.tagGsfSeq = cms.Sequence( process.Zele_sequence * process.tagGsfFilter )

#--------------------------
# Ntple
#--------------------------

process.ntupleEcalAlignment = cms.EDAnalyzer(
    'EcalAlignment',
    recHitCollection_EB = cms.InputTag("reducedEcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("reducedEcalRecHitsEE"),
#    recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
#    recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    EleTag              = cms.InputTag("patElectrons"),
    TrackTag            = cms.InputTag("generalTracks"),
    CALOMetTag          = cms.InputTag("patMETs"),
    vtxTag              = cms.InputTag("goodPrimaryVertices"),
    isMC                = cms.untracked.bool(True),
    )



process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("EcalAlignment.root")
    )


#--------------------------
# filters
#--------------------------

# filter on PhysDeclared bit
process.skimming = cms.EDFilter(
    "PhysDecl",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
    )

# filter on bit = and (40 || 41) and !(bit36 || bit37 || bit38 || bit39)
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')


VERTEX_SEL=("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2")

process.goodPrimaryVertices = cms.EDFilter("VertexSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string(VERTEX_SEL),
  filter = cms.bool(True),
)

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

process.pEcalAlignment = cms.Path(
    process.AllEvents   # |-> counter
#*process.skimming
#*process.FilterL1FilterEvents   # |-> counter
#*process.hltLevel1GTSeed
#*process.FilterGoodVertexFilterEvents   # |-> counter   
#*process.primaryVertexFilter
    *process.goodPrimaryVertices
#*process.FilterNoScrapingFilterEvents   # |-> counter    
#*process.noscraping
#*process.FilterElectronFilterEvents   # |-> counter
#*process.tagGsfSeq
    *process.highetele
    *process.highetFilter
    *process.FilterReRECOEvents   # |-> counter   
    *process.patDefaultSequence
    *process.FilterPatDefaultSequenceEvents   # |-> counter
    *process.ntupleEcalAlignment
    )

#process.outpath = cms.EndPath(process.out)

  
process.schedule = cms.Schedule(
   process.pEcalAlignment        # | -> selections and ntuple
)


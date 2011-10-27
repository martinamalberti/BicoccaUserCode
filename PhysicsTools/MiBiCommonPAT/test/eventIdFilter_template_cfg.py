import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *

MC = False


process = cms.Process("EventIdFilter")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = 'INFO'
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

if not MC:
    process.GlobalTag.globaltag = "START42_V13::All"
if MC:
    process.GlobalTag.globaltag = "GR_R_42_V21::All"


# source
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
        LISTOFFILES
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


# output
process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_*'),
    fileName = cms.untracked.string('JOBDIR/runRUNID_evtEVTID.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
        )
    )


# Event id filter
from PhysicsTools.MiBiCommonPAT.eventIdFilter_cfi import *
process.myEventIdFilter = eventIdFilter.clone()
process.myEventIdFilter.runId = cms.int32(RUNID)
process.myEventIdFilter.eventId = cms.int32(EVTID)



# PAT sequences
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.tools.pfTools")
postfix = "PFlow"

if not MC:
    usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=MC, postfix = postfix, jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute','L2L3Residual']) )
if MC:
    usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=MC, postfix = postfix, jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute']) )
process.pfPileUpPFlow.Enable = True
process.pfPileUpPFlow.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
process.pfPileUpPFlow.checkClosestZVertex = cms.bool(False)
process.pfJetsPFlow.doAreaFastjet = True
process.pfJetsPFlow.doRhoFastjet = False

if not MC:
    removeMCMatching(process, ['All'])


### vertices ###
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(3.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )


#### jets ####
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
from RecoJets.JetProducers.kt4PFJets_cfi import *

# compute FastJet rho to correct jets
process.kt6PFJets = kt4PFJets.clone(
    rParam = cms.double(0.6),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
    )
process.patJetCorrFactors.rho = cms.InputTag("kt6PFJets","rho")

# compute CHS rho to correct jets
process.kt6PFJetsPFlow = kt4PFJets.clone(
    rParam = cms.double(0.6),
    src = cms.InputTag('pfNoElectron'+postfix),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
    )
process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJetsPFlow", "rho")

# compute FastJet rho to correct isolation
process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

# compute area for ak5PFJets
process.ak5PFJets.doAreaFastjet = True


# add collections
addTcMET(process, 'TC')
addPfMET(process, 'PF')

if not MC:
    addJetCollection(
        process,
        cms.InputTag('ak5PFJets'),
        'AK5',
        'PF',
        doJTA        = True,
        doBTagging   = True,
        jetCorrLabel = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])),
        doType1MET   = True,
        doL1Cleaning = True,
        doL1Counters = False,
        genJetCollection=cms.InputTag("ak5GenJets"),
        doJetID      = True,
        jetIdLabel   = "ak5"
        )

if MC:
    addJetCollection(
        process,
        cms.InputTag('ak5PFJets'),
        'AK5',
        'PF',
        doJTA        = True,
        doBTagging   = True,
        jetCorrLabel = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])),
        doType1MET   = True,
        doL1Cleaning = True,
        doL1Counters = False,
        genJetCollection=cms.InputTag("ak5GenJets"),
        doJetID      = True,
        jetIdLabel   = "ak5"
        )


# Add the KT6 producer to the sequence
getattr(process,"patPF2PATSequence"+postfix).replace(
    getattr(process,"pfNoElectron"+postfix),
    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJetsPFlow
    )


process.out.outputCommands = cms.untracked.vstring('keep *_*_*_*')


# Paths
process.p = cms.Path(process.myEventIdFilter*
                     process.goodOfflinePrimaryVertices *
                     getattr(process,"patPF2PATSequence"+postfix) *
                     process.kt6PFJets *
                     process.kt6PFJetsForIsolation *
                     process.ak5PFJets *
                     process.patDefaultSequence)

process.o = cms.EndPath(process.out)

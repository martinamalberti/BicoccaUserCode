import FWCore.ParameterSet.Config as cms


# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR_R_311_V2::All')

# get laser corrections
process.GlobalTag.toGet = cms.VPSet(
    cms.PSet(
        record = cms.string("EcalLaserAPDPNRatiosRcd"),
        tag = cms.string("EcalLaserAPDPNRatios_v3_online"),
        connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_311X_ECAL_LAS")
        )
    )


# source
process.source.fileNames = cms.untracked.vstring(
    #Run2011A - /SingleElectron/Run2011A-PromptReco-v1/RECO
    'file:/data_CMS/cms/abenagli/DATA_SingleElectron_412_RECO.root'
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)


# out
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('file:SimplePATple.root'),
    outputCommands = cms.untracked.vstring()
    )






#--------------------------
#Define PAT sequence
#--------------------------

# Standard PAT Configuration File
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *


## remove MC matching from the default sequence
## remove tau from the default sequence
removeMCMatching(process, ['All'])
removeSpecificPATObjects( process, ['Taus'] )
process.patDefaultSequence.remove( process.patTaus )

# add electron ID
#process.load("Calibration.EcalCalibNtuple.CiC_eIDSequence_cff")
#
#process.patElectronIDs   = cms.Sequence(process.CiC_eIDSequence)
#process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.patElectrons)
#
#process.patElectrons.addElectronID = cms.bool(True)
#process.patElectrons.electronIDSources = cms.PSet(
#    eidVeryLoose  = cms.InputTag("eidVeryLoose"),
#    eidLoose      = cms.InputTag("eidLoose"),
#    eidMedium     = cms.InputTag("eidMedium"),
#    eidTight      = cms.InputTag("eidTight"),
#    eidSuperTight = cms.InputTag("eidSuperTight")
#    )
##
#process.patElectrons.addGenMatch = cms.bool(False)
#process.patElectrons.embedGenMatch = cms.bool(False)


# Add tcMET and pfMET
addTcMET(process, 'TC')
addPfMET(process, 'PF')


# Jet energy corrections to use:
inputJetCorrLabel = ('AK5PF', ['L1Offset', 'L2Relative', 'L3Absolute', 'L2L3Residual'])

# Add PF jets
switchJetCollection(process,
                    cms.InputTag('ak5PFJets'),
                    doJTA        = True,
                    doBTagging   = True,
                    jetCorrLabel = inputJetCorrLabel,
                    doType1MET   = True,
                    genJetCollection=cms.InputTag("ak5GenJets"),
                    doJetID      = True
                    )

process.patJets.addTagInfos = True
process.patJets.tagInfoSources  = cms.VInputTag( cms.InputTag("secondaryVertexTagInfosAOD") )

# PAT selection layer
process.selectedPatElectrons.cut = cms.string("pt > 15.")
process.selectedPatMuons.cut     = cms.string("pt >  5.")
process.selectedPatJets.cut      = cms.string("pt > 15.")






#--------------------------
# Ntuple
#--------------------------

process.load("Calibration/EcalCalibNtuple/simpleNtuple_cfi")

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("simpleNtuple.root")
    )






#--------------------------
# filters
#--------------------------

process.load('PhysicsTools.NtupleUtils.AllPassFilter_cfi')
process.AllPassFilterBegin = process.AllPassFilter.clone()


# filter on primary vertex
process.primaryVertexFilter = cms.EDFilter(
    "GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(4) ,
    maxAbsZ = cms.double(24),
    maxd0 = cms.double(2)
    )

process.AllPassFilterGoodVertexFilter = process.AllPassFilter.clone()


# FilterOutScraping
process.noScrapingFilter = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )

process.AllPassFilterNoScrapingFilter = process.AllPassFilter.clone()


# HB + HE noise filtering
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')

process.AllPassFilterHBHENoiseFilter = process.AllPassFilter.clone()


# select events with at least one gsf electron
process.highetele = cms.EDFilter(
    "GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("superCluster().get().energy()*sin(theta())> 15.")
    )

process.highetFilter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("highetele"),
    minNumber = cms.uint32(1)
    )

process.AllPassFilterElectronFilter = process.AllPassFilter.clone()






#--------------------------
# paths
#--------------------------

process.p = cms.Path(
    process.AllPassFilterBegin *
    process.primaryVertexFilter *
    process.AllPassFilterGoodVertexFilter *
    process.noScrapingFilter *
    process.AllPassFilterNoScrapingFilter *
    process.HBHENoiseFilter *
    process.AllPassFilterHBHENoiseFilter *
    process.highetele *
    process.highetFilter *
    process.AllPassFilterElectronFilter *
    process.patDefaultSequence *
    process.simpleNtuple
    )

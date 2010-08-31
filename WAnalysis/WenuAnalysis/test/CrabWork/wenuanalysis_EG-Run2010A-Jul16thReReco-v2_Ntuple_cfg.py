import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR_R_36X_V12B::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
   #'/store/relval/CMSSW_3_5_7/RelValWE/GEN-SIM-RECO/START3X_V26-v1/0012/EC9F278B-6949-DF11-99F2-003048678B0E.root'
 '/store/data/Run2010A/EG/RECO/Jul16thReReco-v2/0064/F432DDB9-5691-DF11-BD17-002618943940.root'
                                                )
)



#Electron Correction
process.load("RecoEgamma.EgammaTools.correctedElectronsProducer_cfi")

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
process.myanalysis = cms.EDAnalyzer('SimpleNtple',
                                    
   recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
   recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
   EleTag              = cms.InputTag("patElectrons"),
   JetTag              = cms.InputTag("patJets"),
   CALOMetTag          = cms.InputTag("patMETs"),
   TCMetTag            = cms.InputTag("patMETsTC"),
   PFMetTag            = cms.InputTag("patMETsPF"),
   MuTag               = cms.InputTag("patMuons"),
   #HLTTag              = cms.InputTag("TriggerResults::RECO"),
   #HLTTag              = cms.InputTag("TriggerResults::REDIGI36X"),
   HLTTag              = cms.InputTag("TriggerResults::HLT"),
   L1Tag               = cms.InputTag("gtDigis"),

   HLTTag_names        = cms.vstring('HLT_L1SingleEG2','HLT_L1SingleEG5','HLT_L1SingleEG8','HLT_L1DoubleEG5','HLT_Ele10_LW_L1R','HLT_Ele10_LW_EleId_L1R','HLT_Ele15_LW_L1R','HLT_Ele20_LW_L1R','HLT_DoubleEle5_SW_L1R','HLT_Photon10_Cleaned_L1R','HLT_Photon15_L1R','HLT_Photon20_L1R','HLT_Photon30_L1R_8E29','HLT_DoublePhoton5_L1R','HLT_DoublePhoton10_L1R','HLT_EcalCalibration','HLT_L1_BPTX','HLT_L1_BPTX_MinusOnly','HLT_L1_BPTX_PlusOnly'),


   eleId_names         = cms.vstring('eidLoose','eidMedium','eidSuperTight','eidTight','eidVeryLoose'),
   runOnMC             = cms.bool(True),

   saveL1        = cms.untracked.bool(True),
   saveHLT       = cms.untracked.bool(True),
   saveMu        = cms.untracked.bool(True),
   saveEle       = cms.untracked.bool(True),
   saveEleShape  = cms.untracked.bool(True), # !!!
   saveJet       = cms.untracked.bool(True),
   saveCALOMet   = cms.untracked.bool(True),
   saveTCMet     = cms.untracked.bool(True),
   savePFMet     = cms.untracked.bool(True),

   verbosity_    = cms.untracked.bool(False) #---- true = loquacious     false = silence

 )


process.TFileService = cms.Service("TFileService",
   fileName = cms.string("treeNtupleWenu_EG-Run2010A-Jul16thReReco-v2.root")
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
    *process.gsfElectrons
    #*process.highetele
    #*process.highetFilter
    *process.patDefaultSequence
    *process.myanalysis
    )



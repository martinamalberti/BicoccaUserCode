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
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

#process.load('Configuration.StandardSequences.EndOfProcess_cff')
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
  # fileNames = cms.untracked.vstring('file:/data2/amassiro/CMSSWRoot/WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/F0F54048-4A50-E011-9CDE-003048D47792.root')   

    fileNames = cms.untracked.vstring(
      'file:/data2/amassiro/CMSSWRoot/WElectron-May10ReReco-v1_SingleElectron_RAW-RECO/F26A57B7-6D7B-E011-A737-0025901D4C74.root'
    ) )
#file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_AOD/outSkim_1_1_Fq4.root')     )

#.root
#process.source.inputCommands = cms.untracked.vstring("drop *_MEtoEDMConverter_*_*", "keep FEDRawDataCollection_*_*_*")

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
process.GlobalTag.globaltag = 'FT_R_42_V13A::All'

# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions?redirectedfrom=CMS.SWGuideFrontierConditions#311X_Releases  

################################################################################
################################################################################
################################################################################
################## create ntuple for ECAL alignment purposes ###################


#--------------------------
# Define PAT sequence
#--------------------------

# Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.coreTools import *
## remove MC matching from the default sequence
removeMCMatching(process, ['All'])

# bugfix for DATA Run2011 (begin)
removeSpecificPATObjects( process, ['Taus'] )
process.patDefaultSequence.remove( process.patTaus )
# bugfix for DATA Run2011 (end)

# not used! (begin)
removeSpecificPATObjects( process, ['Jets'] )
process.patDefaultSequence.remove( process.patJetCorrections )
process.patDefaultSequence.remove( process.patJets )
# not used (end)

# process.patElectrons.electronSource = cms.InputTag("gsfElectrons::EcalAlignment")

process.patElectrons.addElectronID = cms.bool(False)
  
# Add tcMET and pfMET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process, 'PF')

#--------------------------
# AllPassFilter
#--------------------------


#process.AllEvents                      = cms.EDProducer("EventCountProducer")
process.load("EcalValidation.EcalAlignment.AllPassFilter_cfi")
#--------------------------
# Counter1: All read events
process.AllEvents = process.AllPassFilter.clone()

process.FilterL1FilterEvents           = cms.EDProducer("EventCountProducer")
process.FilterGoodVertexFilterEvents   = cms.EDProducer("EventCountProducer")
process.FilterNoScrapingFilterEvents   = cms.EDProducer("EventCountProducer")
process.FilterElectronFilterEvents     = cms.EDProducer("EventCountProducer")
process.FilterReRECOEvents             = cms.EDProducer("EventCountProducer")
process.FilterPatDefaultSequenceEvents = cms.EDProducer("EventCountProducer")


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
    #src = cms.InputTag("gsfElectrons","REDIGI311X"),
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
    *process.FilterGoodVertexFilterEvents   # |-> counter   
    #*process.primaryVertexFilter
    *process.FilterNoScrapingFilterEvents   # |-> counter    
    #*process.noscraping
    *process.FilterElectronFilterEvents   # |-> counter   
    #*process.highetele
    #*process.highetFilter
    *process.FilterReRECOEvents   # |-> counter   
    *process.patDefaultSequence
    *process.FilterPatDefaultSequenceEvents   # |-> counter
    *process.ntupleEcalAlignment
    )

process.schedule = cms.Schedule(
   process.pEcalAlignment        # | -> selections and ntuple
)


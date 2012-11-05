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
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.7 $'),
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
fileNames = cms.untracked.vstring(
        #'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-WElectron-PromptSkim-v4_RAW-RECO/9220C129-B886-E011-AA20-002481E94B4E.root'
    'file:/data2/amassiro/CMSSWRoot/DATA2012/SingleElectron_Run2012B-WElectron-PromptSkim-v1_USER/908C3428-21A5-E111-B31F-001A92971AAA.root'      
    )
  )
process.source.inputCommands = cms.untracked.vstring("drop *_*_*_RECO", "drop *_MEtoEDMConverter_*_*", "keep FEDRawDataCollection_*_*_*")

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
#process.GlobalTag.globaltag = 'GR_P_V20::All'
#process.GlobalTag.globaltag = 'GR_R_42_V18::All'
#process.GlobalTag.globaltag = 'GR_R_44_V1::All'
process.GlobalTag.globaltag = 'GR_R_52_V9D::All'

  
# http://cms-conddb.cern.ch/gtlist/?GlobalTag=GR_R_39X_V5
  
#process.GlobalTag.toGet = cms.VPSet(
      ##cms.PSet(record = cms.string("EcalIntercalibConstantsRcd"),
             ##tag = cms.string("EcalIntercalibConstants_Bon_V20100803"),
             ##connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             ##),
    ##cms.PSet(record = cms.string("EcalADCToGeVConstantRcd"),
             ##tag = cms.string("EcalADCToGeVConstant_Bon_V20100803"),
             ##connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             ##),
    #cms.PSet(record = cms.string("EBAlignmentRcd"),
             #tag = cms.string("EBAlignment_measured_v05_offline"),
             #connect = cms.untracked.string("sqlite_file:EBAlign_2011.db")   #### New ####
             #),
    #cms.PSet(record = cms.string("EEAlignmentRcd"),
             #tag = cms.string("EEAlignment_measured_v05_offline"),
             #connect = cms.untracked.string("sqlite_file:EEAlign_2011.db")  #### New ####
             #)
   ##cms.PSet(record = cms.string("ESAlignmentRcd"),
             ##tag = cms.string("ESAlignment_measured_v01_offline"),
             ##connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_PRESHOWER")
             ##),
    ##cms.PSet(record = cms.string("EcalLaserAlphasRcd"),
             ##tag = cms.string("EcalLaserAlphas_test_prompt"),
             ##connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             ##),
    ##cms.PSet(record = cms.string("EcalLaserAPDPNRatiosRcd"),
             ##tag = cms.string("EcalLaserAPDPNRatios_v0_online"),
             ##connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
             ##),
    ##cms.PSet(record = cms.string("GlobalPositionRcd"),
             ##tag = cms.string("GlobalAlignment_v2_offline"),
             ##connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             ##),
    #cms.PSet(record = cms.string("TrackerAlignmentRcd"),
             #tag = cms.string("TrackerAlignment_GR10_v5_offline"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             #)
    ##cms.PSet(record = cms.string("TrackerAlignmentErrorRcd"),
             ##tag = cms.string("TrackerAlignmentErrors_GR10_v2_offline"),
             ##connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             ##)
    #)



#from CondCore.DBCommon.CondDBSetup_cfi import *
#process.trackerBows = cms.ESSource(
   #"PoolDBESSource",
   #CondDBSetup,
   #connect = cms.string('frontier://FrontierProd/CMS_COND_310X_ALIGN'),
   #toGet = cms.VPSet(cms.PSet(
           #record = cms.string('TrackerSurfaceDeformationRcd'),
           #tag = cms.string('TrackerSurfaceDeformations_v1_offline')
           #))
   #)
#process.es_prefer_Bows = cms.ESPrefer("PoolDBESSource", "trackerBows")




#process.poolDBESSource2 = cms.ESSource("PoolDBESSource",
   #BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
   #DBParameters = cms.PSet(
        #messageLevel = cms.untracked.int32(2),
        #authenticationPath = cms.untracked.string('/path/to/authentication') ),
    #timetype = cms.untracked.string('runnumber'),
    #connect = cms.string('frontier://PromptProd/CMS_COND_31X_STRIP'),
    #toGet = cms.VPSet(cms.PSet(
        #record = cms.string('SiStripConfObjectRcd'),
        #tag = cms.string('SiStripShiftAndCrosstalk_GR10_v2_offline') )))
#process.es_prefer_BP = cms.ESPrefer('PoolDBESSource','poolDBESSource2')

  
#process.poolDBESSource1 = cms.ESSource("PoolDBESSource",
   #BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
   #DBParameters = cms.PSet(
        #messageLevel = cms.untracked.int32(2),
        #authenticationPath = cms.untracked.string('/path/to/authentication') ),
    #timetype = cms.untracked.string('runnumber'),
    #connect = cms.string('frontier://PromptProd/CMS_COND_31X_STRIP'),
    #toGet = cms.VPSet(cms.PSet(
        #record = cms.string('SiStripLorentzAngleRcd'),
        #tag = cms.string('SiStripLorentzAngle_GR10_v2_offline') )))
#process.es_prefer_LA = cms.ESPrefer('PoolDBESSource','poolDBESSource1')
    
    

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.Path(process.endOfProcess)
#process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

  
    
      
        
          
            

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

process.patElectrons.electronSource = cms.InputTag("gsfElectrons::EcalAlignment")

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
    recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    EleTag              = cms.InputTag("patElectrons"),
    TrackTag            = cms.InputTag("generalTracks"),
    CALOMetTag          = cms.InputTag("patMETs"),
    #eleId_names         = cms.untracked.vstring('eidRobustLoose','eidRobustTight','eidRobustHighEnergy') 
      #eleId_names         = cms.untracked.vstring('eidLoose','eidMedium','eidSuperTight','eidTight','eidVeryLoose'),
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

#process.outpath = cms.EndPath(process.out)

   #PERBACCO !!!! 
        #python encountered the error: <type 'exceptions.AttributeError'> 'Process' object has no attribute 'out'



process.schedule = cms.Schedule(
   process.raw2digi_step,        # | -> reconstruction
   process.L1Reco_step,          # | -> reconstruction
   process.reconstruction_step,  # | -> reconstruction
   process.endjob_step,          # | -> reconstruction
   #process.RECOoutput_step,      # | -> reconstruction
   process.pEcalAlignment        # | -> selections and ntuple
)



################## create ntuple for ECAL alignment purposes ################### 
################################################################################
################################################################################
################################################################################
   

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
    version = cms.untracked.string('$Revision: 1.6 $'),
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
    #fileNames = cms.untracked.vstring('/store/data/Run2010B/Electron/RAW-RECO/v2/000/147/048/9EA6B756-24CF-DF11-A3D9-00261894387C.root')
    #fileNames = cms.untracked.vstring('file:/tmp/amassiro/669A4128-43D0-DF11-AE93-001A92810ACE.root')
    fileNames = cms.untracked.vstring('file:/tmp/amassiro/FADD5997-A711-E011-9C52-003048D436EA.root')   
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
#process.GlobalTag.globaltag = 'GR_R_39X_V5::All'

process.GlobalTag.globaltag = 'GR_R_311_V1::All'
  
# http://cms-conddb.cern.ch/gtlist/?GlobalTag=GR_R_39X_V5
  
  
process.GlobalTag.toGet = cms.VPSet(
      #cms.PSet(record = cms.string("EcalIntercalibConstantsRcd"),
             #tag = cms.string("EcalIntercalibConstants_Bon_V20100803"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             #),
    #cms.PSet(record = cms.string("EcalADCToGeVConstantRcd"),
             #tag = cms.string("EcalADCToGeVConstant_Bon_V20100803"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             #),
    cms.PSet(record = cms.string("EBAlignmentRcd"),
             tag = cms.string("EBAlignment_measured_v04_offline"),
             connect = cms.untracked.string("sqlite_file:EBAlign_2010_NoAlign.db")   #### New ####
             ),
    cms.PSet(record = cms.string("EEAlignmentRcd"),
             tag = cms.string("EEAlignment_measured_v04_offline"),
             connect = cms.untracked.string("sqlite_file:EEAlign_2010_NoAlign.db")  #### New ####
             ),
   #cms.PSet(record = cms.string("ESAlignmentRcd"),
             #tag = cms.string("ESAlignment_measured_v01_offline"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_PRESHOWER")
             #),
    #cms.PSet(record = cms.string("EcalLaserAlphasRcd"),
             #tag = cms.string("EcalLaserAlphas_test_prompt"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             #),
    #cms.PSet(record = cms.string("EcalLaserAPDPNRatiosRcd"),
             #tag = cms.string("EcalLaserAPDPNRatios_v0_online"),
             #connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
             #),
    cms.PSet(record = cms.string("GlobalPositionRcd"),
             tag = cms.string("GlobalAlignment_TkRotMuonFromLastIovV2_offline"),
             connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             ),
    cms.PSet(record = cms.string("TrackerAlignmentRcd"),
             tag = cms.string("TrackerAlignment_GR10_v4_offline"),
             connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             )
    #cms.PSet(record = cms.string("TrackerAlignmentErrorRcd"),
             #tag = cms.string("TrackerAlignmentErrors_GR10_v2_offline"),
             #connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ALIGNMENT")
             #)
    )


#process.newTKAlignment = cms.ESSource("PoolDBESSource",
                                        #CondDBSetup,
                    #connect = cms.string('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT'),
                                        #timetype = cms.string('runnumber'),
                                        #toGet = cms.VPSet(cms.PSet(
                                                #record = cms.string('TrackerAlignmentRcd'),
                                                #tag = cms.string('TrackerAlignment_GR10_v4_offline')
                                                #))
                                        #)
#process.es_prefer_trackerAlignment = cms.ESPrefer("PoolDBESSource", "newTKAlignment")

#process.newGlobalPosition = cms.ESSource("PoolDBESSource",
                                          #CondDBSetup,
                                          #connect = cms.string('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT'),
                                          #toGet= cms.VPSet(cms.PSet(record = cms.string("GlobalPositionRcd"), 
                                                                     #tag = cms.string('GlobalAlignment_TkRotMuonFromLastIovV2_offline'))
                                                           #)
                                         #)
#process.es_prefer_GlobalPositionDB = cms.ESPrefer("PoolDBESSource", "newGlobalPosition")

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
#Define PAT sequence
#--------------------------

# Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.coreTools import *
## remove MC matching from the default sequence
removeMCMatching(process, ['All'])


# add cIc electron ID
#process.load("EcalValidation.EcalAlignment.CiC_eIDSequence_cff")
#process.patElectronIDs   = cms.Sequence(process.CiC_eIDSequence)
#process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.patElectrons)

process.patElectrons.electronSource = cms.InputTag("gsfElectrons::EcalAlignment")

process.patElectrons.addElectronID = cms.bool(False)
  
### AM ### With promt reco eidRobustLoose, eidRobustTight, eidRobustHighEnergy shold be used
### AM ### but has to be defined from scratch. In 
### AM ### EcalValidation.EcalAlignment.CiC_eIDSequence_cff
### AM ### ElectronIdentification.cutsInCategoriesElectronIdentification_cfi
### AM ### they are not defined.
### AM ### Since eleId is not used (in this way) for this analysis
### AM ### but selection are applied "by hand"
### AM ### no eleID is run here.

#process.patElectrons.addElectronID = cms.bool(True)
#process.patElectrons.electronIDSources = cms.PSet(
    #eidRobustLoose      = cms.InputTag("eidRobustLoose"),
    #eidRobustTight      = cms.InputTag("eidRobustTight"),
    #eidRobustHighEnergy = cms.InputTag("eidRobustHighEnergy"),
    #eidVeryLoose  = cms.InputTag("eidVeryLoose"),
    #eidLoose      = cms.InputTag("eidLoose"),
    #eidMedium     = cms.InputTag("eidMedium"),
    #eidTight      = cms.InputTag("eidTight"),
    #eidSuperTight = cms.InputTag("eidSuperTight")
    #)
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
    *process.skimming
    *process.FilterL1FilterEvents   # |-> counter
    *process.hltLevel1GTSeed
    *process.FilterGoodVertexFilterEvents   # |-> counter   
    *process.primaryVertexFilter
    *process.FilterNoScrapingFilterEvents   # |-> counter    
    *process.noscraping
    *process.FilterElectronFilterEvents   # |-> counter   
    *process.highetele
    *process.highetFilter
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
   

# customisation of the process


# Automatic addition of the customisation functionfrom Configuration.GlobalRuns.reco_TLR_38X

####def customiseCommon(process):
    
    #########################################################################################################
    ########
    ########  Top level replaces for handling strange scenarios of early collisions
    ########

    ###### TRACKING:
    ###### Skip events with HV off
    ####process.newSeedFromTriplets.ClusterCheckPSet.MaxNumberOfPixelClusters=2000
    ####process.newSeedFromPairs.ClusterCheckPSet.MaxNumberOfCosmicClusters=20000
    ####process.secTriplets.ClusterCheckPSet.MaxNumberOfPixelClusters=2000
    ####process.fifthSeeds.ClusterCheckPSet.MaxNumberOfCosmicClusters = 20000
    ####process.fourthPLSeeds.ClusterCheckPSet.MaxNumberOfCosmicClusters=20000
    ####process.thTripletsA.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000
    ####process.thTripletsB.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000

    ########## FIXES TRIPLETS FOR LARGE BS DISPLACEMENT ######

    ####### prevent bias in pixel vertex
    ####process.pixelVertices.useBeamConstraint = False
    
    ####### pixelTracks
    #####---- new parameters ----
    ####process.pixelTracks.RegionFactoryPSet.RegionPSet.nSigmaZ  = 4.06
    ####process.pixelTracks.RegionFactoryPSet.RegionPSet.originHalfLength = cms.double(40.6)

    ####### 0th step of iterative tracking
    #####---- new parameters ----
    ####process.newSeedFromTriplets.RegionFactoryPSet.RegionPSet.nSigmaZ   = cms.double(4.06)  
    ####process.newSeedFromTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = 40.6

    ####### 2nd step of iterative tracking
    #####---- new parameters ----
    ####process.secTriplets.RegionFactoryPSet.RegionPSet.nSigmaZ  = cms.double(4.47)  
    ####process.secTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = 44.7

    ###### ECAL 
    ####process.ecalRecHit.ChannelStatusToBeExcluded = [ 1, 2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 78, 142 ]

    #######
    #######  end of top level replacements
    #######
    ###################################################################################################

    ####return (process)


##################################################################################
####def customisePPData(process):
    ####process= customiseCommon(process)

    ###### particle flow HF cleaning
    ####process.particleFlowRecHitHCAL.LongShortFibre_Cut = 30.
    ####process.particleFlowRecHitHCAL.ApplyPulseDPG = True

    ###### HF cleaning for data only
    ####process.hcalRecAlgos.SeverityLevels[3].RecHitFlags.remove("HFDigiTime")
    ####process.hcalRecAlgos.SeverityLevels[4].RecHitFlags.append("HFDigiTime")

    ######beam-halo-id for data only
    ####process.CSCHaloData.ExpectedBX = cms.int32(3)

    ###### hcal hit flagging
    ####process.hfreco.PETstat.flagsToSkip  = 2
    ####process.hfreco.S8S1stat.flagsToSkip = 18
    ####process.hfreco.S9S1stat.flagsToSkip = 26
    
    ####return process


##################################################################################
####def customisePPMC(process):
    ####process=customiseCommon(process)
    
    ####return process

##################################################################################
####def customiseCosmicData(process):

    ####return process

##################################################################################
####def customiseCosmicMC(process):
    
    ####return process
        

##################################################################################
####def customiseExpress(process):
    ####process= customisePPData(process)

    ####import RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi
    ####process.offlineBeamSpot = RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi.onlineBeamSpotProducer.clone()
    
    ####return process

##################################################################################
####def customisePrompt(process):
    ####process= customisePPData(process)

    ####import RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi
    ####process.offlineBeamSpot = RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi.onlineBeamSpotProducer.clone()
    
    ####return process


####process = customisePPData(process)

# End of customisation functions



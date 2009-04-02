import FWCore.ParameterSet.Config as cms

process = cms.Process("COSMICSANALYSIS")



# Geometry
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")



# Magnetic Field
process.load("Configuration.StandardSequences.MagneticField_cff")



# Global Tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
process.GlobalTag.globaltag = 'CRAFT_ALL_V11::All'



# Track Associator
process.load("TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")



# Trigger
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtBoardMapsConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup2_v2_Unprescaled_cff")
import FWCore.Modules.printContent_cfi
process.dumpEv = FWCore.Modules.printContent_cfi.printContent.clone()

import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
process.gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()






process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = (cms.untracked.vstring(
        #CRAFT - run 66615 - TrackerPointing_V1P
        #'rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_TrackerPointing_v1/0011/0886BC80-19A5-DD11-8531-003048D15E50.root'
       
        #CRAFT - run 67147 - CRAFT_ALL_V4_TrackingPointing_TrackerPointing_v2
        #'rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V4_TrackingPointing_TrackerPointing_v2/0151/1C40D4DB-13DD-DD11-BA32-0019B9E4FB31.root'

        #CRAFT - run 67147 - CRAFT_ALL_V9_TrackingPointing_225-v3
        'rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_TrackingPointing_225-v3/0005/5032B463-5EFF-DD11-BE82-001731AF6A49.root'

        #CRAFT - run 67147 - CRAFT_ALL_V11_225_ReReco_FromTrackerPointing_v1
        #'rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V11_225_ReReco_FromTrackerPointing_v1/0009/225A9299-7B0B-DE11-BFDD-0030486790B0.root'
         )
     )
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))






process.load("CaloOnlineTools.EcalTools.ecalCosmicsTree_cfi")

process.ecalCosmicsTree.fileName = 'EcalCosmicsTree'
process.ecalCosmicsTree.muonCollection = cms.InputTag("muons")
process.ecalCosmicsTree.runNum = RUNNUMBER

process.p = cms.Path(process.ecalCosmicsTree)






process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    categories = cms.untracked.vstring('EcalCosmicsTree'),
    destinations = cms.untracked.vstring('cout')
)

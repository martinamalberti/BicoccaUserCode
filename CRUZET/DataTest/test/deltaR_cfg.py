import FWCore.ParameterSet.Config as cms

process = cms.Process("DATATEST")


import FWCore.Modules.printContent_cfi
process.dumpEv = FWCore.Modules.printContent_cfi.printContent.clone()

#PG chissa' che cosa manca per il trackassociator FIXME

#PG process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#PG process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
#PG process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
#PG process.load("Configuration.StandardSequences.Geometry_cff")
#PG process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
#PG 
#PG process.load("Configuration.StandardSequences.MagneticField_cff")
#PG process.load("MagneticField.Engine.volumeBasedMagneticField_cfi")
#PG process.load("TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff")
#PG process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
#PG process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
#PG process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
#PG process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")


#PG --- TUTTA LA MERDA --------------------------------------------------------


process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
process.load("CalibCalorimetry.EcalTrivialCondModules.EcalTrivialCondRetriever_cfi")
process.GlobalTag.globaltag = 'CRUZET4_V5P::All'
process.EcalTrivialConditionRetriever.producedEcalWeights = False
process.EcalTrivialConditionRetriever.producedEcalPedestals = False
process.EcalTrivialConditionRetriever.producedEcalIntercalibConstants = False
process.EcalTrivialConditionRetriever.producedEcalIntercalibErrors = False
process.EcalTrivialConditionRetriever.producedEcalGainRatios = False
process.EcalTrivialConditionRetriever.producedEcalADCToGeVConstant = False
process.EcalTrivialConditionRetriever.producedEcalLaserCorrection = False
process.EcalTrivialConditionRetriever.producedEcalChannelStatus = cms.untracked.bool(True)
process.EcalTrivialConditionRetriever.channelStatusFile = 'CaloOnlineTools/EcalTools/data/listCRUZET4.v5.hashed.txt'
process.es_prefer_EcalTrivialConditionRetriever = cms.ESPrefer("EcalTrivialConditionRetriever")
process.load("CalibCalorimetry.EcalLaserCorrection.ecalLaserCorrectionService_cfi")
import CalibTracker.Configuration.Common.PoolDBESSource_cfi
process.siStripPedestalFrontier = CalibTracker.Configuration.Common.PoolDBESSource_cfi.poolDBESSource.clone()
process.siStripPedestalFrontier.connect = 'frontier://PromptProd/CMS_COND_21X_STRIP'
process.siStripPedestalFrontier.toGet = cms.VPSet(cms.PSet(
            record = cms.string('SiStripPedestalsRcd'),
                        tag = cms.string('SiStripPedestals_TKCC_21X_v3_hlt')
                    ))
process.siStripPedestalFrontier.BlobStreamerName = 'TBufferBlobStreamingService'
process.es_prefer_SiStripFake = cms.ESPrefer("PoolDBESSource","siStripPedestalFrontier")
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerMapping_cfi")
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi")
import RecoLocalCalo.EcalRecProducers.ecalFixedAlphaBetaFitUncalibRecHit_cfi
process.ecalUncalibHit = RecoLocalCalo.EcalRecProducers.ecalFixedAlphaBetaFitUncalibRecHit_cfi.ecalFixedAlphaBetaFitUncalibRecHit.clone()
process.load("RecoLocalCalo.EcalRecProducers.ecalRecHit_cfi")
process.load("RecoEcal.EgammaClusterProducers.geometryForClustering_cff")
process.load("RecoEcal.EgammaClusterProducers.cosmicClusteringSequence_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtBoardMapsConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
#PG anche questo non funziona, non so come recuperare la sua origine. bof.
#PG process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup_v3_Unprescaled_cff")
import FWCore.Modules.printContent_cfi
process.dumpEv = FWCore.Modules.printContent_cfi.printContent.clone()
import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
process.gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()
process.load("CaloOnlineTools.EcalTools.ecalCosmicsHists_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("MagneticField.Engine.volumeBasedMagneticField_cfi")
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")
process.load("TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")


#PG --- INPUT FILES ---------------------------------------------------------------


process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = (cms.untracked.vstring(
    #'rfio:/castor/cern.ch/cms/store/data/Commissioning08/BeamHalo/RECO/GRtoBeam_v1/000/062/068/12E17E04-3A7F-DD11-B369-000423D6CA02.root'
    #'rfio:/castor/cern.ch/user/c/ccecal/BEAM/Skims/61642/Run61642_EventNumberSkim_RAW.root'        
    'rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/RECO/CRUZET4_v1/000/058/537/001E1CDD-0671-DD11-A06C-000423D99AAE.root'
    #'file:/tmp/govoni/02007F35-2671-DD11-B1A0-000423D98868.root'
))
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


#PG --- TFileService ---------------------------------------------------------------


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('deltaR.root')
)


#PG --- Message Logger ---------------------------------------------------------------


process.MessageLogger = cms.Service("MessageLogger",
    suppressWarning = cms.untracked.vstring('ecalEBunpacker','ecalUncalibHit','ecalRecHit',
                                            'hcalDigis','hcalLocalRecoSequence'),
    suppressInfo = cms.untracked.vstring('ecalEBunpacker', 'ecalUncalibHit', 
                                         'ecalRecHit', 'hcalDigis','hcalLocalRecoSequence'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    categories = cms.untracked.vstring('EcalCosmicsHists','CosmicClusterAlgo','CosmicClusterProducer'),
    destinations = cms.untracked.vstring('cout')
)


#PG --- ANALYSIS ---------------------------------------------------------------



process.deltaR = cms.EDFilter("deltaR",
    barrelSuperclusterCollection = cms.InputTag("cosmicSuperClusters","CosmicBarrelSuperClusters"),
    ecalRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dREcal = cms.double(9999.0),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        crossedEnergyType = cms.string('SinglePointAlongTrajectory'),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        accountForTrajectoryChangeCalo = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    endcapSuperclusterCollection = cms.InputTag("cosmicSuperClusters","CosmicEndcapSuperClusters"),
    muonTracksCollection = cms.InputTag("cosmicMuons")
)


#PG --- MAIN PATH ---------------------------------------------------------------


process.p = cms.Path(process.deltaR)



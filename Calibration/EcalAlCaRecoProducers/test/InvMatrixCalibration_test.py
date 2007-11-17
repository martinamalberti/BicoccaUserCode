# Meta configuration: Usually only 5/6 parameters are relevant..

number_of_loops=1
filename='file_name_list.txt' # the name of the file containing the names of the datasets

###############################################################

# The module that allows the configuration with Python.
import FWCore.ParameterSet.Config as cms

# Instance of the process Object. The configuration will take place 
# setting the attributes of process.
# Note that the name must be exatly process.
process=cms.Process("AlCaElectronsProductionTesting")

process.maxEvents=cms.untracked.PSet(input=cms.untracked.int32(-1))
process.options=cms.untracked.PSet\
                (wantSummary=cms.untracked.bool(True)) #default is false

# Message logger
process.MessageLogger=cms.Service\
                ("MessageLogger",
                    destinations=cms.untracked.vstring('cout'),
                    categories=cms.untracked.vstring('IML'),
                    cout=cms.untracked.PSet(
                    threshold=cms.untracked.string('Warning'),
                    default=cms.untracked.PSet(limit=cms.untracked.int32(0)),
                    IML=cms.untracked.PSet(limit=cms.untracked.int32(100))
                                           )
                )                              
                        
# Event Setup

for config_initialiser in ("Geometry/CMSCommonData/data/cmsIdealGeometryXML.cfi",
                           "Geometry/CaloEventSetup/data/CaloGeometry.cfi",
                           "Geometry/CaloEventSetup/data/CaloTopology.cfi"):
    process.include(config_initialiser)

# Process Source!    
    
file_name_list_file=file()

calib_data_files=cms.untracked.vstring()
for file in file_name_list_file.readlines()[0:5]: # Fill the list of filenames
    calib_data_files.append(file)
    
file_name_list_file.close(filename)
            
process.source=cms.Source\
                ("PoolSource",
                 fileNames=calib_data_files,
                 debugVerbosity=cms.untracked.uint32(1),
                 debugFlag=cms.untracked.bool(True)
                )

# EsSouce

process.essources=cms.ESSource\
                ("CaloMiscalibTools")
                #("CaloMiscalibTools",
                # fileNameBarrel=cms.untracked.string='miscalib_barrel_0.05.xml',
                # fileNameEndcap=cms.untracked.string='miscalib_endcap_0.05.xml'
                #) 

# Electrons filter                         
process.electronFilter = cms.EDFilter\
                ('EtaPtMinPixelMatchGsfElectronSelector',
                filter = cms.bool(True),
                src = cms.InputTag('pixelMatchGsfElectrons'),
                etaMin = cms.double(-3.0),
                etaMax = cms.double(3.0),
                ptMin = cms.double(2.0)
                )

# AlCaReco producer                    
process.alCaIsolatedElectrons = cms.EDProducer\
                ("AlCaElectronsProducer",
                    electronLabel = cms.InputTag("electronFilter"),
                    alcaEndcapHitCollection = cms.string('alcaEndcapHits'),
                    phiSize = cms.int32(11),
                    etaSize = cms.int32(5),
                    ebRecHitsLabel = cms.InputTag('ecalRecHit','EcalRecHitsEB'),
                    alcaBarrelHitCollection = cms.string('alcaBarrelHits'),
                    eeRecHitsLabel = cms.InputTag('ecalRecHi','alRecHitsEE')
                )

# Results testing
process.looper=cms.Looper\
               ('InvMatrixLooper',
                alcaBarrelHitCollection=cms.InputTag('alCaIsolatedElectrons','alcaBarrelHits'),
                alcaEndcapHitCollection=cms.InputTag('alCaIsolatedElectrons','alcaEndcapHits'),
                recoWindowSide=cms.int32(5),
                etaWidth=cms.int32(20),
                etaBorder=cms.int32(2),
                etaStart=cms.int32(21),
                etaEnd=cms.int32(61),
                phiStartEB=cms.int32(20),
                phiEndEB=cms.int32(40),
                phiWidthEB=cms.int32(20),
                phiBorderEB=cms.int32(2),
                radStart=cms.int32(20),
                radEnd=cms.int32(20),
                radWidth=cms.int32(15),
                radBorder=cms.int32(2),
                phiStartEE=cms.int32(20),
                phiEndEE=cms.int32(60),
                phiWidthEE=cms.int32(20),
                halfXBand=cms.double(6.0),
                halfYBand=cms.double(6.0),
                maxSelectedNumPerCrystal=cms.int32(2000),
                outputFolder=cms.string(''),
                outputFileName=cms.string('coeffs'),  
                ADCtoGeV=cms.double(120.3),
                minEnergyPerCrystal=cms.double(0.),
                maxEnergyPerCrystal=cms.double(150),
                minCoeff=cms.double(-0.7),
                maxCoeff=cms.double(1.4),
                minAccept=cms.double(0.),
                maxAccept=cms.double(1.),
                usingBlockSolver=cms.int32(1),
                loops=cms.int32(number_of_loops),
                smallestFraction=cms.int32(4),
                howManyFraction=cms.int32(32),
                xCrossMtrFile=cms.string(''),
                yCrossMtrFile=cms.string(''),
                electronLabel=cms.InputTag('electronFilter'),
                halfSelecting=cms.int32(2),
                takeOdd=cms.int32(1),
                etaMax=cms.double(2.7),
                phiMax=cms.int32(360)
               )
                             
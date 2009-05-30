import FWCore.ParameterSet.Config as cms

process = cms.Process("TT")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNINGS'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
      # replace 'myfile.root' with the source file you want to use
      fileNames = cms.untracked.vstring(
    
    ### shot in 1 xtal
    #'file:/gwtera1/users/martelli/data/proveTT/Fast_withTP/prova_EBneg_100k.root'
    'file:/gwtera1/users/martelli/data/proveTT/Fast_withTP/prova_EBpos_100k.root'

    )
)



process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.load("FastSimulation/Configuration/RandomServiceInitialization_cff")
process.load("FastSimulation/Configuration/CommonInputs_cff")
process.load("FastSimulation/Configuration/FamosSequences_cff")

process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')

process.towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("caloRecHits","EcalRecHitsEB","RECO"),
                                              cms.InputTag("caloRecHits","EcalRecHitsEE","RECO")
                                              )

process.calotoweroptmaker.ecalInputs = cms.VInputTag(cms.InputTag("caloRecHits","EcalRecHitsEB","RECO"),
                                                     cms.InputTag("caloRecHits","EcalRecHitsEE","RECO")
                                                     )



process.tpparams = cms.ESSource("EmptyESSource",
                                recordName = cms.string('EcalTPGLinearizationConstRcd'),
                                iovIsRunNotTime = cms.bool(True),
                                firstValid = cms.vuint32(1)
                                )

process.tpparams2 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGPedestalsRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )

process.tpparams3 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGSlidingWindowRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )

process.tpparams4 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGWeightIdMapRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )

process.tpparams5 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGWeightGroupRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )
process.tpparams6 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGLutGroupRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )

process.tpparams7 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGLutIdMapRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )

process.tpparams8 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGFineGrainEBIdMapRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )

process.tpparams9 = cms.ESSource("EmptyESSource",
                                 recordName = cms.string('EcalTPGFineGrainEBGroupRcd'),
                                 iovIsRunNotTime = cms.bool(True),
                                 firstValid = cms.vuint32(1)
                                 )
process.tpparams10 = cms.ESSource("EmptyESSource",
                                  recordName = cms.string('EcalTPGFineGrainStripEERcd'),
                                  iovIsRunNotTime = cms.bool(True),
                                  firstValid = cms.vuint32(1)
                                  )

process.tpparams11 = cms.ESSource("EmptyESSource",
                                  recordName = cms.string('EcalTPGFineGrainTowerEERcd'),
                                  iovIsRunNotTime = cms.bool(True),
                                  firstValid = cms.vuint32(1)
                                  )

process.tpparams12 = cms.ESSource("EmptyESSource",
                                  recordName = cms.string('EcalTPGPhysicsConstRcd'),
                                  iovIsRunNotTime = cms.bool(True),
                                  firstValid = cms.vuint32(1)
                                  )


process.EcalTrigPrimESProducer = cms.ESProducer("EcalTrigPrimESProducer",
                                                DatabaseFile = cms.untracked.string('TPG_startup.txt.gz')
                                                )

process.TTOccupancy = cms.EDAnalyzer('TTOccupancy',
                              recHitCollection = cms.InputTag("caloRecHits", "EcalRecHitsEB", "RECO"),
                              OutputFile = cms.string('TTOccupancy_28-05-09EBpos.root')
                                     )


#process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.p = cms.Path(
    process.caloTowersRec*
    process.TTOccupancy
    )


#process.outpath = cms.EndPath(process.out)


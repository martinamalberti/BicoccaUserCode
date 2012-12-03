import FWCore.ParameterSet.Config as cms



def makeRecoTags(process):
    
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")    
    
    process.GlobalTag.toGet = cms.VPSet(
        
        cms.PSet(record = cms.string("EcalIntercalibConstantsRcd"),
                 tag = cms.string("EcalIntercalibConstants_V20121130_PizC_EleABC_2012HR9EtaScaleABC")
                 connect = cms.untracked.string("frontier://FrontierInt/CMS_COND_ECAL")
                 ),
        cms.PSet(record = cms.string("EcalLaserAPDPNRatiosRcd"),
                 tag = cms.string("EcalLaserAPDPNRatios_20121020_447_p1_v2"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_42X_ECAL_LAS")
                 ),
        cms.PSet(record = cms.string('EcalLaserAlphasRcd'),
                 tag = cms.string('EcalLaserAlphas_EB_sic1_btcp152_EE_sic1_btcp116'),
                 connect = cms.untracked.string('frontier://FrontierInt/CMS_COND_ECAL')
                 ),
        cms.PSet(record = cms.string("EcalADCToGeVConstantRcd"),
                 tag = cms.string("EcalADCToGeVConstant_Bon_V20111129"),
                 connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
                 )
        )



def makeSqliteTags(process):
    
    from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
    process.conddb = cms.ESSource(
        "PoolDBESSource",
        CondDBSetup,
        connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_ECAL/deguio/laser_2011_new1_.db'),
        toGet = cms.VPSet(cms.PSet(
            record = cms.string('EcalLaserAPDPNRatiosRcd'),
            tag = cms.string('test')
            ))
        )
    process.es_prefer_conddb = cms.ESPrefer("PoolDBESSource","conddb")

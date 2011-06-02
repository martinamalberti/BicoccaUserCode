import FWCore.ParameterSet.Config as cms



def makeRecoTags(process):
    
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")    

    process.GlobalTag.toGet = cms.VPSet(
        
        #cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatios_v3_online"),
        #    connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_311X_ECAL_LAS")
        #    ),
        
        cms.PSet(
            record = cms.string("EcalLaserAPDPNRatiosRcd"),
            tag = cms.string("EcalLaserAPDPNRatios_v6_noVPT_online"),
            connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_311X_ECAL_LAS")
            ),
        
        # to correct the bug in ES from run 160176
        #cms.PSet(
        #    record = cms.string("ESRecHitRatioCutsRcd"),
        #    tag = cms.string("ESRecHitRatioCuts_V02_offline"),
        #    connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_PRESHOWER")
        #    )
        
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
            )
        )
    )
    process.es_prefer_conddb = cms.ESPrefer("PoolDBESSource","conddb")

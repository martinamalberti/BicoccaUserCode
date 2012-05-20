import FWCore.ParameterSet.Config as cms



def makeRecoTags(process):
    
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")    

    process.GlobalTag.toGet = cms.VPSet(
    
               cms.PSet(record = cms.string("EcalLaserAPDPNRatiosRcd"),
                                     tag = cms.string("EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc"),
                                     connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_42X_ECAL_LAS")
                                     ),
	       cms.PSet(record = cms.string('EcalLaserAlphasRcd'),
                                           tag = cms.string('EcalLaserAlphas_EB_sic1_btcp152_EE_sic1_btcp116'),
                                           connect = cms.untracked.string('frontier://FrontierPrep/CMS_COND_ECAL')
                                           ),
               cms.PSet(record = cms.string('ESIntercalibConstantsRcd'),
                                           tag = cms.string('ESIntercalibConstants_V02_offline'),
                                           connect = cms.untracked.string('frontier://FrontierProd/CMS_COND_31X_PRESHOWER')
                                           )
                                                                                              
	#cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatios_v3_online"),
        #    connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_311X_ECAL_LAS")
        #    ),
        
        #cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatios_v6_noVPT_online"),
        #    connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_311X_ECAL_LAS")
        #    ),
        
        #cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRefRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatiosRef_v6_noVPT_13_3_online"),
        #    connect =cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
        #    ),
        
        #cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatios_2011fit_noVPT_nolim_online"),
        #    connect =cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
        #    ),
        
        #cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatios_test_20110625"),
        #    connect =cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
        #    ),
        
        #cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatios_data_110831"),
        #    connect =cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
        #    ),
        
        #cms.PSet(
        #    record = cms.string("EcalLaserAPDPNRatiosRcd"),
        #    tag = cms.string("EcalLaserAPDPNRatios_p1p2p3_v2_mc"),
        #    connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
        #    ),
        

        
        # to correct the bug in ES from run 160176
        #cms.PSet(
        #    record = cms.string("ESRecHitRatioCutsRcd"),
        #    tag = cms.string("ESRecHitRatioCuts_V02_offline"),
        #    connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_PRESHOWER")
        #    )


        
        # new intercalibration constants
        #cms.PSet(
        #    record = cms.string("EcalIntercalibConstantsRcd"),
        #    tag = cms.string("EcalIntercalibConstants_Bon_V20110616_weightedAverage"),
        #    connect =cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
        #    )
        
        #cms.PSet(
        #    record = cms.string("EcalIntercalibConstantsRcd"),
        #    tag = cms.string("EcalIntercalibConstants_v10_offline"),
        #    connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
        #    ),
        
        
        
        # new ADCToGeV
        #cms.PSet(
        #    record = cms.string("EcalADCToGeVConstantRcd"),
        #    tag = cms.string("EcalADCToGeVConstant_v10_offline"),
        #    connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
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

import FWCore.ParameterSet.Config as cms



def makeRecoTags(process):
    
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")    

    process.GlobalTag.toGet = cms.VPSet(
        
        cms.PSet(
            record = cms.string("EcalLaserAPDPNRatiosRcd"),
            tag = cms.string("EcalLaserAPDPNRatios_v3_online"),
            connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_311X_ECAL_LAS")
            ),

        # to correct the bug in ES from run 160176
        cms.PSet(
            record = cms.string("ESRecHitRatioCutsRcd"),
            tag = cms.string("ESRecHitRatioCuts_V02_offline"),
            connect =cms.untracked.string("frontier://FrontierProd/CMS_COND_311X_PRESHOWER")
            )
        
        )

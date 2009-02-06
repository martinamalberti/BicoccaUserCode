import FWCore.ParameterSet.Config as cms



# Full Event content 
VBFHWW2l2nuEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
             'drop *_*_*_*',
            
        'keep *_offlineBeamSpot_*_*',
        'keep recoGsfTracks_*_*_*',
        'keep recoGsfTrackExtras_*_*_*',
        'keep recoBasicClusters_*_*_*',
        
#         'keep *_pixelMatchGsfFit_*_*',                       
#         'keep recoTracks_*_*_*',
#         'keep recoTrackExtras_*_*_*',
       
       
                  
        'keep *_hbhereco_*_*',
        'keep *_hfreco_*_*',
        'keep *_horeco_*_*', 
        'keep *_ecalRecHit_*_*',
        'keep EcalRecHitsSorted_*_*_*',  
        'keep *_generalTracks_*_*',  
        'keep *_globalMuons_*_*',  
       
        'keep *_eleIsoFromDepsEcalFromHits_*_*',
        'keep *_eleIsoFromDepsHcalFromHits_*_*',
        'keep *_eleIsoFromDepsTk_*_*',
        
        'keep *_eleIsoDepositEcalFromHits_*_*',
        'keep *_eleIsoDepositHcalFromHits_*_*',
        'keep *_eleIsoDepositTk_*_*',
       
        'keep edmTriggerResults_*_*_*',
        'keep edmHepMCProduct_*_*_*',
        
        'keep *_eidRobustLoose_*_*',
        'keep *_eidRobustTight_*_*',
        'keep *_eidLoose_*_*',
        'keep *_eidTight_*_*',
       

       
        'keep *_iterativeCone5CaloJets_*_*',
        'keep *_met_*_*',
        'keep *_iterativeCone5GenJets_*_*',
        'keep *_pixelMatchGsfElectrons_*_*',
       
        'keep *_genMetNoNuBSM_*_*',
        'keep *_genParticles_*_*',
        'keep recoIsoDepositedmValueMap_*_*_*',
              
        'keep edmGenInfoProduct_*_*_*',
        'keep *_muons_*_*',

        'keep *_offlinePrimaryVertices_*_*',
        'keep *_offlinePrimaryVerticesWithBS_*_*', 
      
        'keep recoSuperClusters_*_*_*',
        'keep *_towerMaker_*_*'
        )
    )

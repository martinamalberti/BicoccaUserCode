import FWCore.ParameterSet.Config as cms



# Full Event content 
VBFHWW2l2nuEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'drop *_*_*_*',
        'keep edmHepMCProduct_*_*_*',
        'keep edmGenInfoProduct_*_*_*',
        'keep edmTriggerResults_*_*_*',
        'keep *_pixelMatchGsfElectrons_*_*',
        'keep *_muons_*_*',
        'keep *_met_*_*',
        'keep *_iterativeCone5CaloJets_*_*',
        #electron isolation output
        'keep *_electronEcalRecHitIsolationLcone_*_*',                                   
        'keep *_electronEcalRecHitIsolationScone_*_*',
        'keep *_electronHcalTowerIsolationLcone_*_*',
        'keep *_electronHcalDepth1TowerIsolationLcone_*_*',
        'keep *_electronHcalDepth2TowerIsolationLcone_*_*',
        'keep *_electronHcalTowerIsolationScone_*_*',
        'keep *_electronHcalDepth1TowerIsolationScone_*_*',
        'keep *_electronHcalDepth2TowerIsolationScone_*_*',
        'keep *_electronTrackIsolationLcone_*_*',
        'keep *_electronTrackIsolationScone_*_*',
        #muon isolation output
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*',
        #electronID output
        'keep *_eidRobustLoose_*_*',
        'keep *_eidRobustTight_*_*',
        'keep *_eidLoose_*_*',
        'keep *_eidTight_*_*',
        'keep *_eidRobustHighEnergy_*_*',
        'keep *_eidClassLoose_*_*',
        'keep *_eidClassMedium_*_*',
        'keep *_eidClassTight_*_*',
        'keep *_eidLikelihood_*_*',
        #vertex output
        'keep *_*_vtxInfo*_*',
        'keep *_*_IpToVtx*_*',
        'keep *_*_TipLipToVtx*_*',
        'keep *_muonCorrectedMET_*_*',
        'keep *_selectedMuons_*_*',
        'keep *_selectedElectrons_*_*',
        'keep *_selectedIsoElectrons_*_*',
        'keep *_selectedJets_*_*',
        'keep *_isolatedElectrons_*_*',
        'keep *_isolatedMuons_*_*'
        )
    )

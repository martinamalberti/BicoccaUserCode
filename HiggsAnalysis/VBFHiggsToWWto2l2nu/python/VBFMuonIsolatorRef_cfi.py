import FWCore.ParameterSet.Config as cms

isolatedMuonsRef = cms.EDFilter("VBFMuonIsolationRef",
                                src = cms.InputTag("muons"),
                                SelectedMuonRefCollectionLabel = cms.InputTag("selectedMuonsRef"),
                                
                                # isolation parameters
                                hcalIsoDepositLabel = cms.InputTag("muIsoDepositCalByAssociatorTowers","hcal"),
                                ecalIsoDepositLabel = cms.InputTag("muIsoDepositCalByAssociatorTowers","ecal"),
                                trackerIsoDepositLabel = cms.InputTag("muIsoDepositTk"),
                             
                                trackIsolCut = cms.double(10),
                                caloIsolCut = cms.double(10),
                                doRefCheck = cms.bool(True)
                                )

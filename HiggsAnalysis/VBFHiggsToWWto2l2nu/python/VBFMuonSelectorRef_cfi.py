import FWCore.ParameterSet.Config as cms

selectedMuonsRef = cms.EDFilter("VBFMuonSelectionRef",
                                src = cms.InputTag("muons"),
                                MuonEtaMax = cms.double(2.5),
                                MuonPtMin = cms.double(5.0)
                                )



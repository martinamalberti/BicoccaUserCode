import FWCore.ParameterSet.Config as cms



VBFEtMinCaloJetCountFilter = cms.EDFilter(
    "VBFEtMinCaloJetCountFilter",
    srcJets = cms.InputTag("VBFSelectedCaloJets"),
    etMin  = cms.double(30.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    minNumber = cms.int32(2)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

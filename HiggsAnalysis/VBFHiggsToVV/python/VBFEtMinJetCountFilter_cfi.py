import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFEtMinCaloJetCountFilter = cms.EDFilter(
    "VBFEtMinCaloJetCountFilter",
    srcJets = cms.InputTag("sisCone5CaloJets"),
    etMin  = cms.double(15.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    minNumber = cms.int32(2)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFEtMinPFJetCountFilter = cms.EDFilter(
    "VBFEtMinPFJetCountFilter",
    srcJets = cms.InputTag("sisCone5PFJets"),
    etMin  = cms.double(15.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    minNumber = cms.int32(2)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

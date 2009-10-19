import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JET CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

from JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff import *


VBFCorrectedSisCone5CaloJets = L2L3CorJetSC5Calo.clone(
    src = cms.InputTag("sisCone5CaloJets"),
    correctors = cms.vstring('L2L3JetCorrectorSC5Calo')
    )

VBFCorrectedSisCone5CaloJetsSequence = cms.Sequence(
        VBFCorrectedSisCone5CaloJets
        )

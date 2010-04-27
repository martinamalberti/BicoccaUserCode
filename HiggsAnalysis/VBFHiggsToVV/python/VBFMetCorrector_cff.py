import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# MET CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

from JetMETCorrections.Type1MET.MetType1Corrections_cff import *






VBFType1SisCone5CaloMet = metJESCorIC5CaloJet.clone(
    inputUncorJetsLabel = "sisCone5CaloJets",
    corrector = "L2L3JetCorrectorSC5Calo",
    inputUncorMetLabel = "corMetGlobalMuons"
    )

VBFType1SisCone5CaloMetSequence = cms.Sequence(
    VBFType1SisCone5CaloMet
    )






VBFType1Antikt5CaloMet = metJESCorIC5CaloJet.clone(
    inputUncorJetsLabel = "ak5CaloJets",
    corrector = "L2L3JetCorrectorAK5Calo",
    inputUncorMetLabel = "corMetGlobalMuons"
    )

VBFType1Antikt5CaloMetSequence = cms.Sequence(
    VBFType1Antikt5CaloMet
    )

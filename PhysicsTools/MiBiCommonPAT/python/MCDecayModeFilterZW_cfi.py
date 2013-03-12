import FWCore.ParameterSet.Config as cms



MCDecayModeFilterZW = cms.EDFilter(
    "MCDecayModeFilterZW",
    srcGenParticles = cms.InputTag("genParticles"),
    eventType = cms.untracked.int32(0),
    verbosity = cms.untracked.bool(False),    
    
    # Z boson decay modes
    Z_e_e     = cms.bool(False),
    Z_mu_mu   = cms.bool(False),
    Z_tau_tau = cms.bool(False),
    Z_nue_nue     = cms.bool(False),
    Z_numu_numu   = cms.bool(False),
    Z_nutau_nutau = cms.bool(False),

    # W boson decay modes
    W_e_nue     = cms.bool(False),
    W_mu_numu   = cms.bool(False),
    W_tau_nutau = cms.bool(False),    
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JET CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

from JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff import *






VBFL2L3SisCone5CaloJets = L2L3CorJetSC5Calo.clone(
    src = cms.InputTag("sisCone5CaloJets"),
    correctors = cms.vstring('L2L3JetCorrectorSC5Calo')
    )

VBFL2L3SisCone5CaloJetsSequence = cms.Sequence(
        VBFL2L3SisCone5CaloJets
        )






VBFL2L3Antikt5CaloJets = L2L3CorJetAK5Calo.clone(
    src = cms.InputTag("antikt5CaloJets"),
    correctors = cms.vstring('L2L3JetCorrectorAK5Calo')
    )

VBFL2L3Antikt5CaloJetsSequence = cms.Sequence(
        VBFL2L3Antikt5CaloJets
        )






VBFL2L3Antikt5PFJets = L2L3CorJetAK5PF.clone(
    src = cms.InputTag("antikt5PFJets"),
    correctors = cms.vstring('L2L3JetCorrectorAK5PF')
    )

VBFL2L3Antikt5PFJetsSequence = cms.Sequence(
        VBFL2L3Antikt5PFJets
        )

       
       

       
#############   Define the L2 correction service #####
L2RelativeJetCorrector = cms.ESSource("L2RelativeCorrectionService", 
    tagName = cms.string('Summer09_L2Relative_SC5Calo'),
    label = cms.string('L2RelativeJetCorrector')
)
##############   Define the L3 correction service #####
L3AbsoluteJetCorrector = cms.ESSource("L3AbsoluteCorrectionService", 
    tagName = cms.string('Summer09_L3Absolute_SC5Calo'),
    label = cms.string('L3AbsoluteJetCorrector')
)
#############   Define the L5 correction service #####
L5JetCorrector = cms.ESSource("L5FlavorCorrectionService",
    section = cms.string('qJ'),  
    tagName = cms.string('L5Flavor_IC5'),# IMPORTANT: the L5 correction was derived from IC5 but it is the same for all algos
    label = cms.string('L5FlavorJetCorrector')
)
#############   Define the L7 correction service #####
L7JetCorrector = cms.ESSource("L7PartonCorrectionService", 
    section = cms.string('qJ'),
    tagName = cms.string('L7Parton_SC5'),
    label = cms.string('L7PartonJetCorrector')
)
#############   Define the chain corrector service ###
L2L3L5L7JetCorrector = cms.ESSource("JetCorrectionServiceChain",  
    correctors = cms.vstring('L2RelativeJetCorrector','L3AbsoluteJetCorrector','L5FlavorJetCorrector','L7PartonJetCorrector'),
    label = cms.string('L2L3L5L7JetCorrector')
)
#############   Define the chain corrector module ####
L2L3L5L7CorJet = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("antikt5CaloJets"),
    correctors = cms.vstring('L2L3L5L7JetCorrector')
)

VBFL2L3L5L7Antikt5CaloJetsSequence = cms.Sequence(
        L2L3L5L7CorJet
        )
       
       
          
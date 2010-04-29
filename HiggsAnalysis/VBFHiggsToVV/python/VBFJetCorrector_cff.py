import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JET CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#from JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff import *
from JetMETCorrections.Configuration.DefaultJEC_cff import *
#from JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff import *




#VBFL2L3SisCone5CaloJets = L2L3CorJetSC5Calo.clone(
    #src = cms.InputTag("sisCone5CaloJets"),
    #correctors = cms.vstring('L2L3JetCorrectorSC5Calo')
    #)

#VBFL2L3SisCone5CaloJetsSequence = cms.Sequence(
        #VBFL2L3SisCone5CaloJets
        #)




# look at http://cmslxr.fnal.gov/lxr/source/JetMETCorrections/Configuration/python/JetCorrectionProducers_cff.py for details
#from http://cmslxr.fnal.gov/lxr/source/JetMETCorrections/Configuration/python


VBFL2L3Antikt5CaloJets = ak5CaloJetsL2L3.clone(
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL2L3')
    )

VBFL2L3Antikt5CaloJetsSequence = cms.Sequence(
        VBFL2L3Antikt5CaloJets
        )


VBFL2L3Antikt5PFJets = ak5PFJetsL2L3.clone(
    src = cms.InputTag("ak5PFJets"),
    correctors = cms.vstring('ak5PFL2L3')
    )

VBFL2L3Antikt5PFJetsSequence = cms.Sequence(
        VBFL2L3Antikt5PFJets
        )

       
       

#############   Define the L5 correction service #####
#L5JetCorrector = cms.ESSource("L5FlavorCorrectionService",
    #section = cms.string('qT'),  
    #tagName = cms.string('L5Flavor_IC5'),# IMPORTANT: the L5 correction was derived from IC5 but it is the same for all algos
    #label = cms.string('L5FlavorJetCorrector')
#)
##############   Define the L7 correction service #####
#L7JetCorrector = cms.ESSource("L7PartonCorrectionService", 
    #section = cms.string('qT'),
    #tagName = cms.string('L7Parton_SC5'),
    #label = cms.string('L7PartonJetCorrector')
#)
##############   Define the chain corrector service ###
#VBFL2L3L5L7Antikt5CaloJetsES = cms.ESSource("JetCorrectionServiceChain",  
    #correctors = cms.vstring('L2L3JetCorrectorAK5Calo','L5FlavorJetCorrector','L7PartonJetCorrector'),
    #label = cms.string('L2L3L5L7AK5JetCorrector')
#)
##############   Define the chain corrector module ####
#VBFL2L3L5L7Antikt5CaloJets = cms.EDProducer("CaloJetCorrectionProducer",
    #src = cms.InputTag("antikt5CaloJets"),
    #correctors = cms.vstring('L2L3L5L7AK5JetCorrector')
#)

#VBFL2L3L5L7Antikt5CaloJetsSequence = cms.Sequence(
        #VBFL2L3L5L7Antikt5CaloJets
#)

       
       
       
                  
#VBFL2L3L5L7SisCone5CaloJetsES = cms.ESSource("JetCorrectionServiceChain",  
    #correctors = cms.vstring('L2L3JetCorrectorSC5Calo','L5FlavorJetCorrector','L7PartonJetCorrector'),
    #label = cms.string('L2L3L5L7SC5JetCorrector')
#)
##############   Define the chain corrector module ####
#VBFL2L3L5L7SisCone5CaloJets = cms.EDProducer("CaloJetCorrectionProducer",
    #src = cms.InputTag("sisCone5CaloJets"),
    #correctors = cms.vstring('L2L3L5L7SC5JetCorrector')
#)

#VBFL2L3L5L7SisCone5CaloJetsSequence = cms.Sequence(
        #VBFL2L3L5L7SisCone5CaloJets
#)

               

import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# correctedSisCone5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_all_cff import *






# --- THE JET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCorrector_cff import *


# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedCaloJetsCorrectedSisCone5CaloJets = VBFCleanedCaloJets.clone()
VBFCleanedCaloJetsRefCorrectedSisCone5CaloJets = VBFCleanedCaloJetsRef.clone()

VBFCleanedCaloJetsCorrectedSisCone5CaloJets.src = cms.InputTag("VBFCorrectedSisCone5CaloJets")
VBFCleanedCaloJetsCorrectedSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsCorrectedSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedCaloJetsRefCorrectedSisCone5CaloJets.src = cms.InputTag("VBFCorrectedSisCone5CaloJets")
VBFCleanedCaloJetsRefCorrectedSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsRefCorrectedSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsCorrectedSisCone5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefCorrectedSisCone5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsCorrectedSisCone5CaloJets.src = cms.InputTag("VBFCorrectedSisCone5CaloJets")
VBFSelectedCaloJetsCorrectedSisCone5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsCorrectedSisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefCorrectedSisCone5CaloJets")

VBFSelectedCaloJetsRefCorrectedSisCone5CaloJets.src = cms.InputTag("VBFCorrectedSisCone5CaloJets")
VBFSelectedCaloJetsRefCorrectedSisCone5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsRefCorrectedSisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefCorrectedSisCone5CaloJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinCaloJetCountFilterCorrectedSisCone5CaloJets = VBFEtMinCaloJetCountFilter.clone()

VBFEtMinCaloJetCountFilterCorrectedSisCone5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsCorrectedSisCone5CaloJets")
VBFEtMinCaloJetCountFilterCorrectedSisCone5CaloJets.minNumber = cms.int32(3)






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToWWTolnujj.VBFNtuple_cfi import *
VBFNtupleCaloJetsCorrectedSisCone5CaloJets = VBFNtupleCaloJets.clone()

VBFNtupleCaloJetsCorrectedSisCone5CaloJets.eventType = cms.untracked.int32(0)
VBFNtupleCaloJetsCorrectedSisCone5CaloJets.fileName = cms.untracked.string("VBFPreselectionNtuple_correctedSisCone5CaloJets.root")

VBFNtupleCaloJetsCorrectedSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
VBFNtupleCaloJetsCorrectedSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFNtupleCaloJetsCorrectedSisCone5CaloJets.doMuonRefCheck = cms.bool(True)
VBFNtupleCaloJetsCorrectedSisCone5CaloJets.srcMuonsRef = cms.InputTag("VBFIsolatedMuonsRefAll")

VBFNtupleCaloJetsCorrectedSisCone5CaloJets.doRecoJetRefCheck = cms.bool(True)
VBFNtupleCaloJetsCorrectedSisCone5CaloJets.srcRecoJets = cms.InputTag("VBFCorrectedSisCone5CaloJets")
VBFNtupleCaloJetsCorrectedSisCone5CaloJets.srcRecoJetsRef = cms.InputTag("VBFSelectedCaloJetsRefCorrectedSisCone5CaloJets")

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----












VBFPreselectionSequenceCorrectedSisCone5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    
    
    VBFCorrectedSisCone5CaloJetsSequence +
        
    VBFCleanedCaloJetsCorrectedSisCone5CaloJets +
    VBFCleanedCaloJetsRefCorrectedSisCone5CaloJets +
    
    VBFSelectedCaloJetsCorrectedSisCone5CaloJets +
    VBFSelectedCaloJetsRefCorrectedSisCone5CaloJets +
    
    
    
    VBFEtMinCaloJetCountFilterCorrectedSisCone5CaloJets +
    
    
    
    VBFNtupleCaloJetsCorrectedSisCone5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----







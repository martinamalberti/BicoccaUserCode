import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# sisCone5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_all_cff import *






# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedCaloJetsSisCone5CaloJets = VBFCleanedCaloJets.clone()
VBFCleanedCaloJetsRefSisCone5CaloJets = VBFCleanedCaloJetsRef.clone()

VBFCleanedCaloJetsSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedCaloJetsRefSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsRefSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsSisCone5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefSisCone5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsSisCone5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsSisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefSisCone5CaloJets")

VBFSelectedCaloJetsRefSisCone5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsRefSisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefSisCone5CaloJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinCaloJetCountFilterSisCone5CaloJets = VBFEtMinCaloJetCountFilter.clone()

VBFEtMinCaloJetCountFilterSisCone5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsSisCone5CaloJets")
VBFEtMinCaloJetCountFilterSisCone5CaloJets.minNumber = cms.int32(4)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
VBFFwdCaloJetCountFilterSisCone5CaloJets = VBFFwdCaloJetCountFilter.clone()

VBFFwdCaloJetCountFilterSisCone5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsSisCone5CaloJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToWWTolnujj.VBFNtuple_cfi import *
VBFNtupleCaloJetsSisCone5CaloJets = VBFNtupleCaloJets.clone()

VBFNtupleCaloJetsSisCone5CaloJets.eventType = cms.untracked.int32(0)
VBFNtupleCaloJetsSisCone5CaloJets.fileName = cms.untracked.string("VBFPreselectionNtuple_sisCone5CaloJets.root")

VBFNtupleCaloJetsSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
VBFNtupleCaloJetsSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFNtupleCaloJetsSisCone5CaloJets.doMuonRefCheck = cms.bool(True)
VBFNtupleCaloJetsSisCone5CaloJets.srcMuonsRef = cms.InputTag("VBFIsolatedMuonsRefAll")

VBFNtupleCaloJetsSisCone5CaloJets.doRecoJetRefCheck = cms.bool(True)
VBFNtupleCaloJetsSisCone5CaloJets.srcRecoJetsRef = cms.InputTag("VBFSelectedCaloJetsRefSisCone5CaloJets")

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----












VBFPreselectionSequenceSisCone5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    
    
    VBFCleanedCaloJetsSisCone5CaloJets +
    VBFCleanedCaloJetsRefSisCone5CaloJets +
    
    VBFSelectedCaloJetsSisCone5CaloJets +
    VBFSelectedCaloJetsRefSisCone5CaloJets +
    
    
    
    VBFEtMinCaloJetCountFilterSisCone5CaloJets +
    VBFFwdCaloJetCountFilterSisCone5CaloJets +
    
    
    
    VBFNtupleCaloJetsSisCone5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

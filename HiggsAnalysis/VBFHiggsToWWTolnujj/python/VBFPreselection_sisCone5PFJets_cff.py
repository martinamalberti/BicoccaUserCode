import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# sisCone5PFJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_all_cff import *






# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedPFJetsSisCone5PFJets = VBFCleanedPFJets.clone()
VBFCleanedPFJetsRefSisCone5PFJets = VBFCleanedPFJetsRef.clone()

VBFCleanedPFJetsSisCone5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsSisCone5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedPFJetsRefSisCone5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsRefSisCone5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedPFJetsSisCone5PFJets = VBFSelectedPFJets.clone()
VBFSelectedPFJetsRefSisCone5PFJets = VBFSelectedPFJetsRef.clone()

VBFSelectedPFJetsSisCone5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsSisCone5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefSisCone5PFJets")

VBFSelectedPFJetsRefSisCone5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsRefSisCone5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefSisCone5PFJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinPFJetCountFilterSisCone5PFJets = VBFEtMinPFJetCountFilter.clone()

VBFEtMinPFJetCountFilterSisCone5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsSisCone5PFJets")
VBFEtMinPFJetCountFilterSisCone5PFJets.minNumber = cms.int32(4)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
VBFFwdPFJetCountFilterSisCone5PFJets = VBFFwdPFJetCountFilter.clone()

VBFFwdPFJetCountFilterSisCone5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsSisCone5PFJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToWWTolnujj.VBFNtuple_cfi import *
VBFNtuplePFJetsSisCone5PFJets = VBFNtuplePFJets.clone()

VBFNtuplePFJetsSisCone5PFJets.eventType = cms.untracked.int32(0)
VBFNtuplePFJetsSisCone5PFJets.fileName = cms.untracked.string("VBFPreselectionNtuple_sisCone5PFJets.root")

VBFNtuplePFJetsSisCone5PFJets.doElectronRefCheck = cms.bool(True)
VBFNtuplePFJetsSisCone5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFNtuplePFJetsSisCone5PFJets.doMuonRefCheck = cms.bool(True)
VBFNtuplePFJetsSisCone5PFJets.srcMuonsRef = cms.InputTag("VBFIsolatedMuonsRefAll")

VBFNtuplePFJetsSisCone5PFJets.doRecoJetRefCheck = cms.bool(True)
VBFNtuplePFJetsSisCone5PFJets.srcRecoJetsRef = cms.InputTag("VBFSelectedPFJetsRefSisCone5PFJets")

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----












VBFPreselectionSequenceSisCone5PFJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    

    
    VBFCleanedPFJetsSisCone5PFJets +
    VBFCleanedPFJetsRefSisCone5PFJets +
    
    VBFSelectedPFJetsSisCone5PFJets +
    VBFSelectedPFJetsRefSisCone5PFJets +
    
    
    
    VBFEtMinPFJetCountFilterSisCone5PFJets +
    VBFFwdPFJetCountFilterSisCone5PFJets +
    
    
    
    VBFNtuplePFJetsSisCone5PFJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----







#  LocalWords:  MCDecayModeFilterSisCone

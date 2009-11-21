import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# all
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE MC FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.MCDecayModeFilter_cfi import *
MCDecayModeFilterAll = MCDecayModeFilter.clone()

MCDecayModeFilterAll.eventType = cms.untracked.int32(0)
MCDecayModeFilterAll.H_W_W = cms.bool(True)
MCDecayModeFilterAll.V1_e_nue = cms.bool(True)
MCDecayModeFilterAll.V1_mu_numu = cms.bool(True)
MCDecayModeFilterAll.V1_tau_nutau = cms.bool(False)
MCDecayModeFilterAll.V2_d_u = cms.bool(True)
MCDecayModeFilterAll.V2_d_c = cms.bool(True)
MCDecayModeFilterAll.V2_s_u = cms.bool(True)
MCDecayModeFilterAll.V2_s_c = cms.bool(True)
MCDecayModeFilterAll.V2_b_u = cms.bool(True)
MCDecayModeFilterAll.V2_b_c = cms.bool(True)






# --- THE ELECTRON AMBIGUITY RESOLVER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronAmbiguityResolver_cfi import *
VBFAmbiguityResolvedElectronsAll = VBFAmbiguityResolvedElectrons.clone()
VBFAmbiguityResolvedElectronsRefAll = VBFAmbiguityResolvedElectronsRef.clone()


# --- THE ELECTRON SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronSelector_cfi import *
VBFSelectedElectronsAll = VBFSelectedElectrons.clone()
VBFSelectedElectronsRefAll = VBFSelectedElectronsRef.clone()

VBFSelectedElectronsAll.doRefCheck = cms.bool(True)
VBFSelectedElectronsAll.srcElectronsRef = cms.InputTag("VBFAmbiguityResolvedElectronsRefAll")

VBFSelectedElectronsRefAll.doRefCheck = cms.bool(True)
VBFSelectedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFAmbiguityResolvedElectronsRefAll")


# --- THE ELECTRON ISOLATOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronIsolator_cfi import *
VBFIsolatedElectronsAll = VBFIsolatedElectrons.clone()
VBFIsolatedElectronsRefAll = VBFIsolatedElectronsRef.clone()

VBFIsolatedElectronsAll.tkIsoCut = cms.double(1.)
VBFIsolatedElectronsAll.doRefCheck = cms.bool(True)
VBFIsolatedElectronsAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")

VBFIsolatedElectronsRefAll.tkIsoCut = cms.double(1.)
VBFIsolatedElectronsRefAll.doRefCheck = cms.bool(True)
VBFIsolatedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")


# --- THE ELECTRON ID SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronIdSelector_cfi import *
VBFIdSelectedElectronsAll = VBFIdSelectedElectrons.clone()
VBFIdSelectedElectronsRefAll = VBFIdSelectedElectronsRef.clone()

VBFIdSelectedElectronsAll.doRefCheck = cms.bool(True)
VBFIdSelectedElectronsAll.srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRefAll")

VBFIdSelectedElectronsRefAll.doRefCheck = cms.bool(True)
VBFIdSelectedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRefAll")






# --- THE MUON SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMuonSelector_cfi import *
VBFSelectedMuonsAll = VBFSelectedMuons.clone()
VBFSelectedMuonsRefAll = VBFSelectedMuonsRef.clone()


# --- THE MUON ISOLATOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMuonIsolator_cfi import *
VBFIsolatedMuonsAll = VBFIsolatedMuons.clone()
VBFIsolatedMuonsRefAll = VBFIsolatedMuonsRef.clone()

VBFIsolatedMuonsAll.tkIsoCut = cms.double(1.)
VBFIsolatedMuonsAll.doRefCheck = cms.bool(True)
VBFIsolatedMuonsAll.srcMuonsRef = cms.InputTag("VBFSelectedMuonsRefAll")

VBFIsolatedMuonsRefAll.tkIsoCut = cms.double(1.)
VBFIsolatedMuonsRefAll.doRefCheck = cms.bool(True)
VBFIsolatedMuonsRefAll.srcMuonsRef = cms.InputTag("VBFSelectedMuonsRefAll")






# --- THE LEPTON FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPtMinLeptonCountFilter_cfi import *
VBFPtMinLeptonCountFilterAll = VBFPtMinLeptonCountFilter.clone()

VBFPtMinLeptonCountFilterAll.srcElectrons = cms.InputTag("VBFIdSelectedElectronsAll")
VBFPtMinLeptonCountFilterAll.srcMuons = cms.InputTag("VBFIsolatedMuonsAll")






# --- THE MUON CORRECTED MET  --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFCaloMetMuonCorrector_cff import *

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----












VBFPreselectionSequenceAll = cms.Sequence(
    
    MCDecayModeFilterAll +
    
    
    
    VBFAmbiguityResolvedElectronsAll +
    VBFAmbiguityResolvedElectronsRefAll +  
    
    VBFSelectedElectronsAll +
    VBFSelectedElectronsRefAll +
    
    VBFIsolatedElectronsAll +
    VBFIsolatedElectronsRefAll +
    
    VBFIdSelectedElectronsAll +
    VBFIdSelectedElectronsRefAll +
    
    
    
    VBFSelectedMuonsAll +
    VBFSelectedMuonsRefAll +
    
    VBFIsolatedMuonsAll +
    VBFIsolatedMuonsRefAll +
    
    
    
    VBFPtMinLeptonCountFilterAll +
    
    
    
    VBFMuonCorrectedCaloMetSequence
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

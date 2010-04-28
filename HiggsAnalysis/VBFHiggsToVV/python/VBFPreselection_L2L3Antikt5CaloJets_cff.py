import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# ak5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff import *






# --- THE JET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCorrector_cff import *
from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedCaloJetsL2L3Antikt5CaloJets = VBFCleanedCaloJets.clone()
VBFCleanedCaloJetsRefL2L3Antikt5CaloJets = VBFCleanedCaloJetsRef.clone()

VBFCleanedCaloJetsL2L3Antikt5CaloJets.src = cms.InputTag("VBFL2L3Antikt5CaloJets")
VBFCleanedCaloJetsL2L3Antikt5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsL2L3Antikt5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedCaloJetsRefL2L3Antikt5CaloJets.src = cms.InputTag("VBFL2L3Antikt5CaloJets")
VBFCleanedCaloJetsRefL2L3Antikt5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsRefL2L3Antikt5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsL2L3Antikt5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefL2L3Antikt5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsL2L3Antikt5CaloJets.src = cms.InputTag("VBFL2L3Antikt5CaloJets")
VBFSelectedCaloJetsL2L3Antikt5CaloJets.etMin = cms.double(15.)
VBFSelectedCaloJetsL2L3Antikt5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsL2L3Antikt5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsL2L3Antikt5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsL2L3Antikt5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3Antikt5CaloJets")

VBFSelectedCaloJetsRefL2L3Antikt5CaloJets.src = cms.InputTag("VBFL2L3Antikt5CaloJets")
VBFSelectedCaloJetsRefL2L3Antikt5CaloJets.etMin = cms.double(15.)
VBFSelectedCaloJetsRefL2L3Antikt5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsL2L3Antikt5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsRefL2L3Antikt5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsRefL2L3Antikt5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3Antikt5CaloJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinCaloJetCountFilterL2L3Antikt5CaloJets = VBFEtMinCaloJetCountFilter.clone()

VBFEtMinCaloJetCountFilterL2L3Antikt5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3Antikt5CaloJets")
VBFEtMinCaloJetCountFilterL2L3Antikt5CaloJets.etMin = cms.double(15.)
VBFEtMinCaloJetCountFilterL2L3Antikt5CaloJets.etaMin = cms.double(-5.0)
VBFEtMinCaloJetCountFilterL2L3Antikt5CaloJets.etaMax = cms.double(+5.0)
VBFEtMinCaloJetCountFilterL2L3Antikt5CaloJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdCaloJetCountFilterL2L3Antikt5CaloJets = VBFFwdCaloJetCountFilter.clone()

#VBFFwdCaloJetCountFilterL2L3Antikt5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3Antikt5CaloJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
VBFNtupleL2L3Antikt5CaloJets = SimpleNtple.clone()

VBFNtupleL2L3Antikt5CaloJets.EleTag = cms.InputTag("gsfElectrons")
VBFNtupleL2L3Antikt5CaloJets.doEleRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
VBFNtupleL2L3Antikt5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
VBFNtupleL2L3Antikt5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
VBFNtupleL2L3Antikt5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

VBFNtupleL2L3Antikt5CaloJets.MuTag = cms.InputTag("muons")
VBFNtupleL2L3Antikt5CaloJets.doMuRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
VBFNtupleL2L3Antikt5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
VBFNtupleL2L3Antikt5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
VBFNtupleL2L3Antikt5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

VBFNtupleL2L3Antikt5CaloJets.JetTag = cms.InputTag("VBFL2L3Antikt5CaloJets")
VBFNtupleL2L3Antikt5CaloJets.doJetRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.JetRefTag = cms.InputTag("VBFSelectedCaloJetsRefL2L3Antikt5CaloJets")
VBFNtupleL2L3Antikt5CaloJets.genJetTag = cms.InputTag("sisCone5GenJets")

VBFNtupleL2L3Antikt5CaloJets.Type1MetTag = cms.InputTag("VBFType1Antikt5CaloMet")

VBFNtupleL2L3Antikt5CaloJets.saveMu          = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.saveTrack       = cms.untracked.bool(False)
VBFNtupleL2L3Antikt5CaloJets.saveEle         = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.saveJet         = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.savePFJet       = cms.untracked.bool(False)
VBFNtupleL2L3Antikt5CaloJets.saveJetBTagging = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.saveMet         = cms.untracked.bool(True)
VBFNtupleL2L3Antikt5CaloJets.saveGenJet      = cms.untracked.bool(False)
VBFNtupleL2L3Antikt5CaloJets.saveGenMet      = cms.untracked.bool(False)
VBFNtupleL2L3Antikt5CaloJets.saveMC          = cms.untracked.bool(False)






VBFPreselectionSequenceL2L3Antikt5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    
    
    VBFL2L3Antikt5CaloJetsSequence +
    VBFType1Antikt5CaloMetSequence +



    VBFCleanedCaloJetsL2L3Antikt5CaloJets +
    VBFCleanedCaloJetsRefL2L3Antikt5CaloJets +
    
    VBFSelectedCaloJetsL2L3Antikt5CaloJets +
    VBFSelectedCaloJetsRefL2L3Antikt5CaloJets +
    
    
    
    VBFEtMinCaloJetCountFilterL2L3Antikt5CaloJets +
    #VBFFwdCaloJetCountFilterL2L3Antikt5CaloJets +
    
    
    
    VBFNtupleL2L3Antikt5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

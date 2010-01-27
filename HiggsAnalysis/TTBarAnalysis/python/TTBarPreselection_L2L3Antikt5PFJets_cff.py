import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# antikt5PFJets
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
VBFCleanedPFJetsL2L3Antikt5PFJets = VBFCleanedPFJets.clone()
VBFCleanedPFJetsRefL2L3Antikt5PFJets = VBFCleanedPFJetsRef.clone()

VBFCleanedPFJetsL2L3Antikt5PFJets.src = cms.InputTag("VBFL2L3Antikt5PFJets")
VBFCleanedPFJetsL2L3Antikt5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsL2L3Antikt5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedPFJetsRefL2L3Antikt5PFJets.src = cms.InputTag("VBFL2L3Antikt5PFJets")
VBFCleanedPFJetsRefL2L3Antikt5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsRefL2L3Antikt5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedPFJetsL2L3Antikt5PFJets = VBFSelectedPFJets.clone()
VBFSelectedPFJetsRefL2L3Antikt5PFJets = VBFSelectedPFJetsRef.clone()

VBFSelectedPFJetsL2L3Antikt5PFJets.src = cms.InputTag("VBFL2L3Antikt5PFJets")
VBFSelectedPFJetsL2L3Antikt5PFJets.etMin = cms.double(15.)
VBFSelectedPFJetsL2L3Antikt5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsL2L3Antikt5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsL2L3Antikt5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsL2L3Antikt5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefL2L3Antikt5PFJets")

VBFSelectedPFJetsRefL2L3Antikt5PFJets.src = cms.InputTag("VBFL2L3Antikt5PFJets")
VBFSelectedPFJetsRefL2L3Antikt5PFJets.etMin = cms.double(15.)
VBFSelectedPFJetsRefL2L3Antikt5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsL2L3Antikt5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsRefL2L3Antikt5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsRefL2L3Antikt5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefL2L3Antikt5PFJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinPFJetCountFilterL2L3Antikt5PFJets = VBFEtMinPFJetCountFilter.clone()

VBFEtMinPFJetCountFilterL2L3Antikt5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsL2L3Antikt5PFJets")
VBFEtMinPFJetCountFilterL2L3Antikt5PFJets.etMin = cms.double(20.)
VBFEtMinPFJetCountFilterL2L3Antikt5PFJets.etaMin = cms.double(-5.0)
VBFEtMinPFJetCountFilterL2L3Antikt5PFJets.etaMax = cms.double(+5.0)
VBFEtMinPFJetCountFilterL2L3Antikt5PFJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdPFJetCountFilterL2L3Antikt5PFJets = VBFFwdPFJetCountFilter.clone()

#VBFFwdPFJetCountFilterL2L3Antikt5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsL2L3Antikt5PFJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.TTBarAnalysis.SimpleNtpleTTBar_cfi import *
TTBarNtupleL2L3Antikt5PFJets = SimpleNtpleTTBar.clone()

TTBarNtupleL2L3Antikt5PFJets.EleTag = cms.InputTag("gsfElectrons")
TTBarNtupleL2L3Antikt5PFJets.doEleRefCheck = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
TTBarNtupleL2L3Antikt5PFJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
TTBarNtupleL2L3Antikt5PFJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
TTBarNtupleL2L3Antikt5PFJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

TTBarNtupleL2L3Antikt5PFJets.MuTag = cms.InputTag("muons")
TTBarNtupleL2L3Antikt5PFJets.doMuRefCheck = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
TTBarNtupleL2L3Antikt5PFJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
TTBarNtupleL2L3Antikt5PFJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
TTBarNtupleL2L3Antikt5PFJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

TTBarNtupleL2L3Antikt5PFJets.JetTag = cms.InputTag("VBFL2L3Antikt5PFJets")
TTBarNtupleL2L3Antikt5PFJets.doJetRefCheck = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.JetRefTag = cms.InputTag("VBFSelectedPFJetsRefL2L3Antikt5PFJets")
TTBarNtupleL2L3Antikt5PFJets.genJetTag = cms.InputTag("antikt5GenJets")

TTBarNtupleL2L3Antikt5PFJets.Type1MetTag = cms.InputTag("VBFType1Antikt5CaloMet")

TTBarNtupleL2L3Antikt5PFJets.saveMu          = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.saveTrack       = cms.untracked.bool(False)
TTBarNtupleL2L3Antikt5PFJets.saveEle         = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.saveJet         = cms.untracked.bool(False)
TTBarNtupleL2L3Antikt5PFJets.savePFJet       = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.saveJetBTagging = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.saveMet         = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.saveGenJet      = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.saveGenMet      = cms.untracked.bool(True)
TTBarNtupleL2L3Antikt5PFJets.saveMC          = cms.untracked.bool(True)






TTBarPreselectionSequenceL2L3Antikt5PFJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    VBFL2L3Antikt5PFJetsSequence +
    VBFType1Antikt5CaloMetSequence +

    VBFCleanedPFJetsL2L3Antikt5PFJets +
    VBFCleanedPFJetsRefL2L3Antikt5PFJets +
    
    VBFSelectedPFJetsL2L3Antikt5PFJets +
    VBFSelectedPFJetsRefL2L3Antikt5PFJets +
    
    VBFEtMinPFJetCountFilterL2L3Antikt5PFJets +
    #VBFFwdPFJetCountFilterL2L3Antikt5PFJets +
    
    TTBarNtupleL2L3Antikt5PFJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

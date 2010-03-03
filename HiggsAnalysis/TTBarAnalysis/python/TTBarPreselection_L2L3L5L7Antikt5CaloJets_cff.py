import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# antikt5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff import *






# --- THE JET & MET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCorrector_cff import *
from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *

VBFType1Antikt5CaloMetL2L3L5L7 = VBFType1Antikt5CaloMet.clone(
 corrector = "L2L3L5L7AK5JetCorrector"
)

VBFType1Antikt5CaloMetL2L3L5L7Sequence = cms.Sequence(
    VBFType1Antikt5CaloMetL2L3L5L7
    )


# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedCaloJetsL2L3L5L7Antikt5CaloJets = VBFCleanedCaloJets.clone()
VBFCleanedCaloJetsRefL2L3L5L7Antikt5CaloJets = VBFCleanedCaloJetsRef.clone()

VBFCleanedCaloJetsL2L3L5L7Antikt5CaloJets.src = cms.InputTag("L2L3L5L7AK5JetCorrector")
VBFCleanedCaloJetsL2L3L5L7Antikt5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsL2L3L5L7Antikt5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedCaloJetsRefL2L3L5L7Antikt5CaloJets.src = cms.InputTag("L2L3L5L7AK5JetCorrector")
VBFCleanedCaloJetsRefL2L3L5L7Antikt5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsRefL2L3L5L7Antikt5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets.src = cms.InputTag("L2L3L5L7CorJet")
VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets.etMin = cms.double(15.)
VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3L5L7Antikt5CaloJets")

VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets.src = cms.InputTag("L2L3L5L7CorJet")
VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets.etMin = cms.double(15.)
VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3L5L7Antikt5CaloJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinCaloJetCountFilterL2L3L5L7Antikt5CaloJets = VBFEtMinCaloJetCountFilter.clone()

VBFEtMinCaloJetCountFilterL2L3L5L7Antikt5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets")
VBFEtMinCaloJetCountFilterL2L3L5L7Antikt5CaloJets.etMin = cms.double(20.)
VBFEtMinCaloJetCountFilterL2L3L5L7Antikt5CaloJets.etaMin = cms.double(-5.0)
VBFEtMinCaloJetCountFilterL2L3L5L7Antikt5CaloJets.etaMax = cms.double(+5.0)
VBFEtMinCaloJetCountFilterL2L3L5L7Antikt5CaloJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdCaloJetCountFilterL2L3L5L7Antikt5CaloJets = VBFFwdCaloJetCountFilter.clone()

#VBFFwdCaloJetCountFilterL2L3L5L7Antikt5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.TTBarAnalysis.SimpleNtpleTTBar_cfi import *
TTBarNtupleL2L3L5L7Antikt5CaloJets = SimpleNtpleTTBar.clone()

TTBarNtupleL2L3L5L7Antikt5CaloJets.EleTag = cms.InputTag("gsfElectrons")
TTBarNtupleL2L3L5L7Antikt5CaloJets.doEleRefCheck = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
TTBarNtupleL2L3L5L7Antikt5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
TTBarNtupleL2L3L5L7Antikt5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
TTBarNtupleL2L3L5L7Antikt5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

TTBarNtupleL2L3L5L7Antikt5CaloJets.MuTag = cms.InputTag("muons")
TTBarNtupleL2L3L5L7Antikt5CaloJets.doMuRefCheck = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
TTBarNtupleL2L3L5L7Antikt5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
TTBarNtupleL2L3L5L7Antikt5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
TTBarNtupleL2L3L5L7Antikt5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

TTBarNtupleL2L3L5L7Antikt5CaloJets.JetTag = cms.InputTag("L2L3L5L7CorJet")
TTBarNtupleL2L3L5L7Antikt5CaloJets.doJetRefCheck = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.JetRefTag = cms.InputTag("VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets")
TTBarNtupleL2L3L5L7Antikt5CaloJets.genJetTag = cms.InputTag("antikt5GenJets")

TTBarNtupleL2L3L5L7Antikt5CaloJets.Type1MetTag = cms.InputTag("VBFType1Antikt5CaloMetL2L3L5L7")

TTBarNtupleL2L3L5L7Antikt5CaloJets.saveMu          = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveTrack       = cms.untracked.bool(False)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveEle         = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveJet         = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.savePFJet       = cms.untracked.bool(False)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveJetBTagging = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveMet         = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveGenJet      = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveGenMet      = cms.untracked.bool(True)
TTBarNtupleL2L3L5L7Antikt5CaloJets.saveMC          = cms.untracked.bool(True)






TTBarPreselectionSequenceL2L3L5L7Antikt5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    VBFL2L3L5L7Antikt5CaloJetsSequence +
    VBFType1Antikt5CaloMetL2L3L5L7Sequence +

    VBFCleanedCaloJetsL2L3L5L7Antikt5CaloJets +
    VBFCleanedCaloJetsRefL2L3L5L7Antikt5CaloJets +
    
    VBFSelectedCaloJetsL2L3L5L7Antikt5CaloJets +
    VBFSelectedCaloJetsRefL2L3L5L7Antikt5CaloJets +
    
    VBFEtMinCaloJetCountFilterL2L3L5L7Antikt5CaloJets +
#VBFFwdCaloJetCountFilterL2L3L5L7Antikt5CaloJets +
    
    TTBarNtupleL2L3L5L7Antikt5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

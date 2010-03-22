import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# antikt5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff import *






# --- THE JET/MET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedCaloJetsAntikt5CaloJets = VBFCleanedCaloJets.clone()
VBFCleanedCaloJetsRefAntikt5CaloJets = VBFCleanedCaloJetsRef.clone()

VBFCleanedCaloJetsAntikt5CaloJets.src = cms.InputTag("antikt5CaloJets")
VBFCleanedCaloJetsAntikt5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsAntikt5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedCaloJetsRefAntikt5CaloJets.src = cms.InputTag("antikt5CaloJets")
VBFCleanedCaloJetsRefAntikt5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsRefAntikt5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsAntikt5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefAntikt5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsAntikt5CaloJets.src = cms.InputTag("antikt5CaloJets")
VBFSelectedCaloJetsAntikt5CaloJets.etMin = cms.double(10.)
VBFSelectedCaloJetsAntikt5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsAntikt5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsAntikt5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsAntikt5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefAntikt5CaloJets")

VBFSelectedCaloJetsRefAntikt5CaloJets.src = cms.InputTag("antikt5CaloJets")
VBFSelectedCaloJetsRefAntikt5CaloJets.etMin = cms.double(10.)
VBFSelectedCaloJetsRefAntikt5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsAntikt5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsRefAntikt5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsRefAntikt5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefAntikt5CaloJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinCaloJetCountFilterAntikt5CaloJets = VBFEtMinCaloJetCountFilter.clone()

VBFEtMinCaloJetCountFilterAntikt5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsAntikt5CaloJets")
VBFEtMinCaloJetCountFilterAntikt5CaloJets.etMin = cms.double(15.)
VBFEtMinCaloJetCountFilterAntikt5CaloJets.etaMin = cms.double(-5.0)
VBFEtMinCaloJetCountFilterAntikt5CaloJets.etaMax = cms.double(+5.0)
VBFEtMinCaloJetCountFilterAntikt5CaloJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdCaloJetCountFilterAntikt5CaloJets = VBFFwdCaloJetCountFilter.clone()

#VBFFwdCaloJetCountFilterAntikt5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsAntikt5CaloJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.TTBarAnalysis.SimpleNtpleTTBar_cfi import *
TTBarNtupleAntikt5CaloJets = SimpleNtpleTTBar.clone()

TTBarNtupleAntikt5CaloJets.EleTag = cms.InputTag("gsfElectrons")
TTBarNtupleAntikt5CaloJets.doEleRefCheck = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
TTBarNtupleAntikt5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
TTBarNtupleAntikt5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
TTBarNtupleAntikt5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

TTBarNtupleAntikt5CaloJets.MuTag = cms.InputTag("muons")
TTBarNtupleAntikt5CaloJets.doMuRefCheck = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
TTBarNtupleAntikt5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
TTBarNtupleAntikt5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
TTBarNtupleAntikt5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

TTBarNtupleAntikt5CaloJets.JetTag = cms.InputTag("antikt5CaloJets")
TTBarNtupleAntikt5CaloJets.doJetRefCheck = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.JetRefTag = cms.InputTag("VBFSelectedCaloJetsRefAntikt5CaloJets")
TTBarNtupleAntikt5CaloJets.genJetTag = cms.InputTag("antikt5GenJets")

TTBarNtupleAntikt5CaloJets.Type1MetTag = cms.InputTag("VBFType1Antikt5CaloMet")

TTBarNtupleAntikt5CaloJets.saveMu          = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.saveTrack       = cms.untracked.bool(False)
TTBarNtupleAntikt5CaloJets.saveEle         = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.saveJet         = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.savePFJet       = cms.untracked.bool(False)
TTBarNtupleAntikt5CaloJets.saveJetBTagging = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.saveMet         = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.saveGenJet      = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.saveGenMet      = cms.untracked.bool(True)
TTBarNtupleAntikt5CaloJets.saveMC          = cms.untracked.bool(True)






TTBarPreselectionSequenceAntikt5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    VBFType1Antikt5CaloMetSequence +
    
    VBFCleanedCaloJetsAntikt5CaloJets +
    VBFCleanedCaloJetsRefAntikt5CaloJets +
    
    VBFSelectedCaloJetsAntikt5CaloJets +
    VBFSelectedCaloJetsRefAntikt5CaloJets +
    
    VBFEtMinCaloJetCountFilterAntikt5CaloJets +
#VBFFwdCaloJetCountFilterAntikt5CaloJets +
    
    TTBarNtupleAntikt5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



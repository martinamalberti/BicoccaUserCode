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

from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedPFJetsAntikt5PFJets = VBFCleanedPFJets.clone()
VBFCleanedPFJetsRefAntikt5PFJets = VBFCleanedPFJetsRef.clone()

VBFCleanedPFJetsAntikt5PFJets.src = cms.InputTag("antikt5PFJets")
VBFCleanedPFJetsAntikt5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsAntikt5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedPFJetsRefAntikt5PFJets.src = cms.InputTag("antikt5PFJets")
VBFCleanedPFJetsRefAntikt5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsRefAntikt5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedPFJetsAntikt5PFJets = VBFSelectedPFJets.clone()
VBFSelectedPFJetsRefAntikt5PFJets = VBFSelectedPFJetsRef.clone()

VBFSelectedPFJetsAntikt5PFJets.src = cms.InputTag("antikt5PFJets")
VBFSelectedPFJetsAntikt5PFJets.etMin = cms.double(10.)
VBFSelectedPFJetsAntikt5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsAntikt5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsAntikt5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsAntikt5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefAntikt5PFJets")

VBFSelectedPFJetsRefAntikt5PFJets.src = cms.InputTag("antikt5PFJets")
VBFSelectedPFJetsRefAntikt5PFJets.etMin = cms.double(10.)
VBFSelectedPFJetsRefAntikt5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsAntikt5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsRefAntikt5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsRefAntikt5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefAntikt5PFJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinPFJetCountFilterAntikt5PFJets = VBFEtMinPFJetCountFilter.clone()

VBFEtMinPFJetCountFilterAntikt5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsAntikt5PFJets")
VBFEtMinPFJetCountFilterAntikt5PFJets.etMin = cms.double(15.)
VBFEtMinPFJetCountFilterAntikt5PFJets.etaMin = cms.double(-5.0)
VBFEtMinPFJetCountFilterAntikt5PFJets.etaMax = cms.double(+5.0)
VBFEtMinPFJetCountFilterAntikt5PFJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdPFJetCountFilterAntikt5PFJets = VBFFwdPFJetCountFilter.clone()

#VBFFwdPFJetCountFilterAntikt5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsAntikt5PFJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.TTBarAnalysis.SimpleNtpleTTBar_cfi import *
TTBarNtupleAntikt5PFJets = SimpleNtpleTTBar.clone()

TTBarNtupleAntikt5PFJets.EleTag = cms.InputTag("gsfElectrons")
TTBarNtupleAntikt5PFJets.doEleRefCheck = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
TTBarNtupleAntikt5PFJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
TTBarNtupleAntikt5PFJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
TTBarNtupleAntikt5PFJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

TTBarNtupleAntikt5PFJets.MuTag = cms.InputTag("muons")
TTBarNtupleAntikt5PFJets.doMuRefCheck = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
TTBarNtupleAntikt5PFJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
TTBarNtupleAntikt5PFJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
TTBarNtupleAntikt5PFJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

TTBarNtupleAntikt5PFJets.JetTag = cms.InputTag("antikt5PFJets")
TTBarNtupleAntikt5PFJets.doJetRefCheck = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.JetRefTag = cms.InputTag("VBFSelectedPFJetsRefAntikt5PFJets")
TTBarNtupleAntikt5PFJets.genJetTag = cms.InputTag("antikt5GenJets")

TTBarNtupleAntikt5PFJets.Type1MetTag = cms.InputTag("VBFType1Antikt5CaloMet")

TTBarNtupleAntikt5PFJets.saveMu          = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.saveTrack       = cms.untracked.bool(False)
TTBarNtupleAntikt5PFJets.saveEle         = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.saveJet         = cms.untracked.bool(False)
TTBarNtupleAntikt5PFJets.savePFJet       = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.saveJetBTagging = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.saveMet         = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.saveGenJet      = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.saveGenMet      = cms.untracked.bool(True)
TTBarNtupleAntikt5PFJets.saveMC          = cms.untracked.bool(True)






VBFPreselectionSequenceAntikt5PFJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    VBFType1Antikt5CaloMetSequence +
    
    VBFCleanedPFJetsAntikt5PFJets +
    VBFCleanedPFJetsRefAntikt5PFJets +
    
    VBFSelectedPFJetsAntikt5PFJets +
    VBFSelectedPFJetsRefAntikt5PFJets +
    
    VBFEtMinPFJetCountFilterAntikt5PFJets +
    #VBFFwdPFJetCountFilterAntikt5PFJets +

    
    TTBarNtupleAntikt5PFJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

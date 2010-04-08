import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer(
    "SimpleNtple",
     
     MuTag           = cms.InputTag("muons"),
     Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducer:Mu3DipSignificanceMap"),
     MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:MuTipSignificanceMap"),
     MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:MuLipSignificanceMap"),
     
     EleTag          = cms.InputTag("gsfElectrons"),
     Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducer:Ele3DipSignificanceMap"),
     EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:EleTipSignificanceMap"),
     EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:EleLipSignificanceMap"),
     eleIDCut_LooseInputTag  = cms.InputTag("eidLoose"),
     eleIDCut_RLooseInputTag = cms.InputTag("eidRobustLoose"),
     eleIDCut_TightInputTag  = cms.InputTag("eidTight"), 
     eleIDCut_RTightInputTag = cms.InputTag("eidRobustTight"),
     
     TracksTag       = cms.InputTag("generalTracks"),

     JetTag          = cms.InputTag("sisCone5CaloJets"),
     genJetTag       = cms.InputTag("sisCone5GenJets"),

     MetTag          = cms.InputTag("corMetGlobalMuons"),
     Type1MetTag     = cms.InputTag("VBFType1SisCone5CaloMet"),
     PFMetTag        = cms.InputTag("pfMet"),         
     genMetTag       = cms.InputTag("genMetTrue"),

     MCtruthTag      = cms.InputTag("genParticles"),

     doMuRefCheck = cms.untracked.bool (False),
     MuRefTag = cms.InputTag("MuRef"),
     doEleRefCheck = cms.untracked.bool (False),
     EleRefTag = cms.InputTag("EleRef"),
     doJetRefCheck = cms.untracked.bool (False),
     JetRefTag = cms.InputTag("JetRef"),
     
     saveMu           = cms.untracked.bool (True),
     saveTrack        = cms.untracked.bool (True), 
     saveEle          = cms.untracked.bool (True),
     saveJet          = cms.untracked.bool (True),
     savePFJet        = cms.untracked.bool (False),
     saveJetBTagging  = cms.untracked.bool (True),
     saveMet          = cms.untracked.bool (True),
     saveGenJet       = cms.untracked.bool (True),
     saveGenMet       = cms.untracked.bool (True),                        
     saveMCHiggs      = cms.untracked.bool (True),
     saveMCHiggsDecay = cms.untracked.bool (True),    
     saveMCEle        = cms.untracked.bool (True),
     saveMCMu         = cms.untracked.bool (True),    
     
        
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0) 
)

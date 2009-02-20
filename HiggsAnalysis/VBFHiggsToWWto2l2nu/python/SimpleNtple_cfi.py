import FWCore.ParameterSet.Config as cms

ntpla = cms.EDAnalyzer(
    "SimpleNtple",
     EleTag = cms.InputTag("pixelMatchGsfElectrons"),
     IsolEleTag = cms.InputTag("dummy"),
     MuTag = cms.InputTag("muons"),
     IsolMuTag = cms.InputTag("dummy"),
     MetTag = cms.InputTag("met"),

     TagJetTag = cms.InputTag("dummy"),
    
     JetTag = cms.InputTag("iterativeCone5CaloJets"),
     TracksTag = cms.InputTag("generalTracks"),
     eleIDPTDRLooseInputTag = cms.InputTag("eidLoose"),#caso
     eleIDPTDRMediumInputTag = cms.InputTag("eidRobustLoose"),#caso
     eleIDPTDRTightInputTag = cms.InputTag("eidTight"),#caso
       
     MCtruthTag = cms.InputTag("genParticles"),
     genJetTag = cms.InputTag("iterativeCone5GenJets"), 
#      genMetTag = cms.InputTag("genMet"), 
     genMetTag = cms.InputTag("genMetNoNuBSM"), 
      
## Electron Isolation
      
     coneRadius = cms.double(0.3),#dummy
     vetoRadius = cms.double(0.02),#dummy
     otherVetoRadius = cms.double(0.015),#dummy
     ptMin = cms.double(1.5),#dummy
     lipMax = cms.double(100.),#dummy
     useTkQuality = cms.untracked.bool(True)#dummy


)
   
# from CMSSW/HiggsAnalysis/VBFHiggsToWWto2l2nu/data/SimpleNtple.cfi 

   

import FWCore.ParameterSet.Config as cms

ntpla = cms.EDAnalyzer(
    "SimpleNtple",
     EleTag = cms.InputTag("pixelMatchGsfElectrons"),
     IsolEleTag = cms.InputTag("dummy"),
     MuTag = cms.InputTag("muons"),
     IsolMuTag = cms.InputTag("dummy"),
     MetTag = cms.InputTag("met"),

     TagJetTag = cms.InputTag("dummy"),

############         
### Jets ###         
#      JetTag = cms.InputTag("iterativeCone5CaloJets"),
     JetTag = cms.InputTag("cleanedJets"),
### different jet tags ###    
     bool_JetTagSisCone5CaloJets       = cms.untracked.bool(False),
     bool_JetTagIterativeCone5CaloJets = cms.untracked.bool(True),
     bool_JetTagSisCone5PFJets         = cms.untracked.bool(True),
     bool_JetTagIterativeCone5PFJets   = cms.untracked.bool(True),         
      
     JetTagSisCone5CaloJets       = cms.InputTag("sisCone5CaloJets"),
     JetTagIterativeCone5CaloJets = cms.InputTag("iterativeCone5CaloJets"),
     JetTagSisCone5PFJets         = cms.InputTag("sisCone5PFJets"),      
     JetTagIterativeCone5PFJets   = cms.InputTag("iterativeCone5PFJets"),
                
##############         
### Tracks ###         
     TracksTag = cms.InputTag("generalTracks"),
     
###################        
### Electron ID ###         
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

   

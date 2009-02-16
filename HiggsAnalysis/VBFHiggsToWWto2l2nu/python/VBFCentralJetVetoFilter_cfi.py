import FWCore.ParameterSet.Config as cms

centralJetVeto = cms.EDFilter(
    "VBFCentralJetVetoFilter",
     jetInputTag = cms.InputTag("otherJets"),
     tagJetInputTag = cms.string("taggedJets"),
     jetDeltaEta = cms.double(-0.5), # negative -> smaller region       eta jet + jetDeltaEta
     jetPtMin = cms.double(15.), # GeV
     maxJetsNum = cms.int32(0) # max number of jets
    )
   
# from CMSSW/HiggsAnalysis/VBFHiggsToWWto2l2nu/data/VBFCentralJetVetoFilter.cfi  

   
   
    
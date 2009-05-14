import FWCore.ParameterSet.Config as cms
process = cms.Process("TEST")

process.Selections = cms.PSet(

 g_ID1 = cms.untracked.int32(100), # electron
 g_ID2 = cms.untracked.int32(100), # electron

# variable to decide if make Isolation selection:
#    1 = true
#    0 = false 
# 0 = electron     1 = muon  
 g_ISO1_0 = cms.untracked.int32(1), # electron
 g_ISO1_1 = cms.untracked.int32(1), # muon
 g_ISO2_0 = cms.untracked.int32(1), # electron
 g_ISO2_1 = cms.untracked.int32(1), # muon

 g_IsoElectron = cms.untracked.double(0.2),
 g_IsoMuon = cms.untracked.double(0.2),
 
 g_hardLEPPtMin_0 = cms.untracked.double(25), # electron
 g_hardLEPPtMin_1 = cms.untracked.double(25), # muon
 g_softLEPPtMin_0 = cms.untracked.double(20), # electron
 g_softLEPPtMin_1 = cms.untracked.double(20), # muon
 
 g_LEPDPhiMin = cms.untracked.double(1),
 g_LEPDPhiMax = cms.untracked.double(1000),
 g_LEPMinvMin = cms.untracked.double(12),
 g_LEPMinvMax = cms.untracked.double(80),
 
 g_METMin = cms.untracked.double(30),
  
 g_hardTAGPtMin = cms.untracked.double(40),
 g_softTAGPtMin = cms.untracked.double(0),
 g_TAGDProdEtaMax = cms.untracked.double(0),
 g_TAGDetaMin = cms.untracked.double(3.0),
 g_TAGMinv = cms.untracked.double(650.),
 
 g_ojetPtMin = cms.untracked.double(15),
 g_ojetEtaMax = cms.untracked.double(5.),
 g_ojetEtaFromMean = cms.untracked.double(-1),
 g_ojetsMaxNum = cms.untracked.int32(0),

#  g_cutsNum = cms.untracked.int32(1), 


 g_LepLep = cms.untracked.int32(0), ### 0 == ee, 1 == mumu, 2 == emu, 3 == mue



  
 g_prefix = cms.untracked.string("MyHisto_"),


 ### useful ###
          
 g_KindOfJet = cms.untracked.string("otherJets_IterativeCone5CaloJets") 
          
)

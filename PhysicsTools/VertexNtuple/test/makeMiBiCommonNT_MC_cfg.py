import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, Other, ZW
makeMiBiCommonNT(process, GlobalTag="START39_V8::All", HLT='', MC=True, MCType='ZW')
  
process.source.fileNames = cms.untracked.vstring(
    #'file:/tmp/deguio/HGG_RawReco_GluGluToHToGG_M-120_7TeV-powheg-pythia6_PU8_GluGluToHToGG_M-120_7TeV-powheg-pythia6_RAWRECOMixingE7TeV_AVE_8_BX78_PU.root'
    
    #'/store/relval/CMSSW_3_9_7/RelValZEE/GEN-SIM-RECO/START39_V8-v1/0049/2A40A789-BB0D-E011-A60A-00261894395A.root',
    #'/store/relval/CMSSW_3_9_7/RelValZEE/GEN-SIM-RECO/START39_V8-v1/0047/9A9056BF-840D-E011-B4E7-00261894397E.root'

  #'/store/relval/CMSSW_3_9_7/RelValH130GGgluonfusion/GEN-SIM-RECO/START39_V8-v1/0047/FE7D17B2-930D-E011-AE62-002618943913.root'

  #'/store/relval/CMSSW_3_9_7/RelValMinBias/GEN-SIM-RECO/START39_V8-v1/0049/1A1F705B-BB0D-E011-8999-00261894386B.root',
  #'/store/relval/CMSSW_3_9_7/RelValMinBias/GEN-SIM-RECO/START39_V8-v1/0047/929A413E-820D-E011-9AC6-00261894380A.root'
  'file:/tmp/deguio/Winter10_WPlusToENu_CT10_TuneZ2_7TeV-powheg-pythia_AODSIM_E7TeV_ProbDist_2010Data_BX156_START39_V8-v1.root'
  #'file:/tmp/dimatteo/963B8D02-12E7-DF11-A8E0-003048D3E452.root'
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/FA4BFB16-7AD3-DF11-AD7C-0019B9F72BFF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/F023A43C-83D3-DF11-9EC7-001D09F24047.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E6DEB3D2-81D3-DF11-82A4-001D09F297EF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E4EE523D-83D3-DF11-9564-001D09F28F1B.root'
  #'file:/gwpool/users/amassiro/VBF/Releases/CMSSW_3_8_3/src/PhysicsTools/MiBiCommonPAT/669A4128-43D0-DF11-AE93-001A92810ACE.root'  

)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)


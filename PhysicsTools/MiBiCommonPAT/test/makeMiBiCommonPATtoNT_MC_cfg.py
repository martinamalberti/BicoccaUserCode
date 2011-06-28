import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
makeMiBiCommonNT(process, GlobalTag="START311_V2::All", HLT='', MC=True, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
  #'file:/grid_mnt/data__DATA/data.polcms/cms/abenagli/MC_GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6_PU_S1_START311_V1G1-v1_AODSIM.root'
  #'file:/grid_mnt/data__DATA/data.polcms/cms/abenagli/VBFH170ToWWToLNuQQ_397_ProbDist_2011Flat_AODSIM.root'
  #'file:/tmp/dimatteo/963B8D02-12E7-DF11-A8E0-003048D3E452.root'
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/FA4BFB16-7AD3-DF11-AD7C-0019B9F72BFF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/F023A43C-83D3-DF11-9EC7-001D09F24047.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E6DEB3D2-81D3-DF11-82A4-001D09F297EF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E4EE523D-83D3-DF11-9564-001D09F28F1B.root'
  #'file:/gwpool/users/amassiro/VBF/Releases/CMSSW_3_8_3/src/PhysicsTools/MiBiCommonPAT/669A4128-43D0-DF11-AE93-001A92810ACE.root'  
  #'file:/gwterax1/users/amassiro/CMSSWroot/Fall10/WPlusToENu_CT10_TuneZ2_7TeV-powheg-pythia/D056A87C-66D0-DF11-AAE1-0018FE286DB2.root.1'
  #'file:/tmp/amassiro/ACC59E3D-860D-E011-BCE6-002354EF3BDE.root'   # Tau test
  #'file:/tmp/amassiro/1038652E-FA3D-E011-8EF9-003048679296.root'   # PU test
#   'file:/data2/amassiro/CMSSWRoot/WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/F0F54048-4A50-E011-9CDE-003048D47792.root',
   'file:/data2/amassiro/CMSSWRoot/WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/106AFBB1-2D50-E011-A529-00E0817918AD.root'

)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)


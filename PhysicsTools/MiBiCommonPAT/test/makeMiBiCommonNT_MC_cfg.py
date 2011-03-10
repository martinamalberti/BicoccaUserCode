import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
makeMiBiCommonNT(process, GlobalTag="START39_V9::All", HLT='', MC=True, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
  #'file:/grid_mnt/data__DATA/data.polcms/cms/abenagli/WENu_38X_AOD.root'
  #'file:/tmp/dimatteo/963B8D02-12E7-DF11-A8E0-003048D3E452.root'
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/FA4BFB16-7AD3-DF11-AD7C-0019B9F72BFF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/F023A43C-83D3-DF11-9EC7-001D09F24047.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E6DEB3D2-81D3-DF11-82A4-001D09F297EF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E4EE523D-83D3-DF11-9564-001D09F28F1B.root'
  #'file:/gwpool/users/amassiro/VBF/Releases/CMSSW_3_8_3/src/PhysicsTools/MiBiCommonPAT/669A4128-43D0-DF11-AE93-001A92810ACE.root'  
  # 'file:/gwterax1/users/amassiro/CMSSWroot/Fall10/WPlusToENu_CT10_TuneZ2_7TeV-powheg-pythia/D056A87C-66D0-DF11-AAE1-0018FE286DB2.root.1'
    'file:/gwterax1/users/amassiro/CMSSWroot/Fall10/DY_ZTauTau_Fall10/ACC59E3D-860D-E011-BCE6-002354EF3BDE.root'   
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)


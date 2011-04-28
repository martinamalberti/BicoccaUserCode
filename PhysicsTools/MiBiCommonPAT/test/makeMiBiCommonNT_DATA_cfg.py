import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT
makeMiBiCommonNT(process, GlobalTag="GR_R_311_V2::All", HLT='', MC=False, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
  # Dec22ReReco
  #'/store/data/Run2010B/Electron/RECO/Dec22ReReco_v1/0000/049351B8-E20D-E011-A421-00304867C1BC.root'
  #'file:/tmp/deguio/4AB1C58E-730F-E011-AA63-002354EF3BE4.root'
  #'file:/grid_mnt/data__DATA/data.polcms/cms/abenagli/DATA_38X_RECO.root'
  #'file:/tmp/dimatteo/D8700674-35CB-DF11-8C60-0024E85A3ED8.root'
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/FA4BFB16-7AD3-DF11-AD7C-0019B9F72BFF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/F023A43C-83D3-DF11-9EC7-001D09F24047.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E6DEB3D2-81D3-DF11-82A4-001D09F297EF.root',
  #'/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/451/E4EE523D-83D3-DF11-9564-001D09F28F1B.root'

  #'file:/gwpool/users/amassiro/VBF/Releases/CMSSW_3_8_3/src/PhysicsTools/MiBiCommonPAT/669A4128-43D0-DF11-AE93-001A92810ACE.root'  
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_73_1_YNf.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_74_1_Rm2.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_75_1_MMi.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_76_1_tEe.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_77_1_GO1.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_78_1_FLX.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_79_1_3xO.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_7_1_NsY.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_80_1_ZbP.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_81_1_tcF.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_82_1_2Ju.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_83_1_tc7.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_8_1_OeI.root',
'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v1_RECO_Bis/outSkim_9_1_XQk.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)


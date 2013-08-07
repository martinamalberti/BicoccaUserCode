
import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT
#makeMiBiCommonNT(process, GlobalTag="FT_53_V21_AN4::All", HLT='', MC=False, MCType='Other')
makeMiBiCommonNT(process, GlobalTag="FT_R_53_V21::All", HLT='', MC=False, MCType='Other')#2012
#makeMiBiCommonNT(process, GlobalTag="FT_R_53_LV3::All", HLT='', MC=False, MCType='Other')#2011

  
process.source.fileNames = cms.untracked.vstring(
#     '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/190/702/8EAD0A55-5383-E111-BEA9-001D09F295A1.root'
#    '/store/data/Run2012A/DoubleMu/AOD/22Jan2013-v1/20000/D63F3959-CE81-E211-9F95-002618943971.root'
#    '/store/data/Run2012B/DoubleMuParked/AOD/22Jan2013-v1/20000/FAC76060-8568-E211-9371-00261834B57E.root'
    '/store/data/Run2012B/DoubleMuParked/AOD/22Jan2013-v1/20000/64315823-2B67-E211-A58C-20CF3027A637.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)


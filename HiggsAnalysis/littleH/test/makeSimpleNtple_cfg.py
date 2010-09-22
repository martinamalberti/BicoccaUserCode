import FWCore.ParameterSet.Config as cms

process = cms.Process("Onia2LepLepNtuple")

from HiggsAnalysis.littleH.littleHPAT_cff import *

from HiggsAnalysis.littleH.SimpleNtple_cff import *

#littleHPAT(process, GlobalTag="START36_V9::All", MC=False, HLT="HLT", Filter=False, SavePAT=False)

#makeSimpleNtple(process, GlobalTag="START36_V9::All", MC=False, HLT='HLT', HLT_filter_ele=False, HLT_filter_mu=False)

littleHPAT(process, GlobalTag="START36_V9::All", MC=False, HLT="REDIGI36X.", Filter=False, SavePAT=False)

makeSimpleNtple(process, GlobalTag="START36_V9::All", MC=False, HLT='REDIGI36X.', HLT_filter_ele=False, HLT_filter_mu=False)

process.source.fileNames = cms.untracked.vstring(
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/EEAA24FA-25B4-DF11-A5F1-000423D98950.root',
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/C40EDB4E-1DB4-DF11-A83C-0030487C90C2.root',
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/C2497931-2CB4-DF11-A92C-003048F1183E.root',
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/AC68ABE0-19B4-DF11-BB93-0030487C7E18.root'
        'file:/tmp/dimatteo/Data.root'
        #'file:/tmp/dimatteo/ZEE.root'
        #'file:/afs/cern.ch/user/d/dimatteo/scratch0/PHD_MiBi/smallH/CMSSW_3_6_1_patch6/src/HiggsAnalysis/littleH/test/onia2LepLepPAT.root'
        #'file:/tmp/dimatteo/B841FC1F-E4A1-DF11-80CC-001A92971ACE.root'
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0011/B841FC1F-E4A1-DF11-80CC-001A92971ACE.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0011/4AE205A9-E2A1-DF11-8BB4-002618943959.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0011/10CE04BD-31A2-DF11-B6D2-002618943821.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0010/8C889611-DEA1-DF11-A5A9-0026189438C0.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0010/129B9E8F-DDA1-DF11-9856-001A92971BA0.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0010/06679815-DFA1-DF11-8A3D-001A9281170C.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("SimpleNtple.root"),
      closeFileFast = cms.untracked.bool(True)
)


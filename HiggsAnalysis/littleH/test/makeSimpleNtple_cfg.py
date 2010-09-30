import FWCore.ParameterSet.Config as cms

process = cms.Process("Onia2LepLepNtuple")

from HiggsAnalysis.littleH.SimpleNtple_cff import *

makeSimpleNtple(process, GlobalTag="START38_V12::All", MC=False, HLT="HLT", Filter=True, SavePAT=False, HLT_filter_ele=False, HLT_filter_mu=False,Pat2Ntuple=False)

process.source.fileNames = cms.untracked.vstring(
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/EEAA24FA-25B4-DF11-A5F1-000423D98950.root',
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/C40EDB4E-1DB4-DF11-A83C-0030487C90C2.root',
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/C2497931-2CB4-DF11-A92C-003048F1183E.root',
        #'/store/data/Run2010A/EG/RECO/v4/000/144/114/AC68ABE0-19B4-DF11-BB93-0030487C7E18.root'
        #'file:/tmp/dimatteo/AA80C1F7-9AC2-DF11-817E-00261894398B.root'
        'file:/tmp/dimatteo/CA5F22F2-9AC2-DF11-9CC4-00304867915A.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("SimpleNtple.root"),
      closeFileFast = cms.untracked.bool(True)
)


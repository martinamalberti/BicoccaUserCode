import FWCore.ParameterSet.Config as cms

process = cms.Process("Onia2LepLepPAT")

from HiggsAnalysis.littleH.littleHPAT_cff import *

littleHPAT(process, GlobalTag="START38_V8::All", MC=True, HLT="HLT", Filter=False)

process.source.fileNames = cms.untracked.vstring(
  'file:/tmp/dimatteo/DAB30B48-4A8E-DF11-888B-002618FDA211.root'
  #'file:/tmp/dimatteo/8ACD4E9E-2DA2-DF11-B6EA-0026189438FD.root'
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0011/B841FC1F-E4A1-DF11-80CC-001A92971ACE.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0011/4AE205A9-E2A1-DF11-8BB4-002618943959.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0011/10CE04BD-31A2-DF11-B6D2-002618943821.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0010/8C889611-DEA1-DF11-A5A9-0026189438C0.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0010/129B9E8F-DDA1-DF11-9856-001A92971BA0.root',
        #'/store/relval/CMSSW_3_8_1/RelValJpsiMM/GEN-SIM-RECO/START38_V8-v1/0010/06679815-DFA1-DF11-8A3D-001A9281170C.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


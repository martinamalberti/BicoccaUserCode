import FWCore.ParameterSet.Config as cms

process = cms.Process("Onia2LepLepNtuple")

from HiggsAnalysis.littleH.SimpleNtple_cff import *

makeSimpleNtple(process, GlobalTag="START38_V12::All", MC=True, HLT="HLT", Filter=True, SavePAT=False, HLT_filter_ele=False, HLT_filter_mu=False,Pat2Ntuple=False)

process.source.fileNames = cms.untracked.vstring(
    '/store/mc/Summer10/Upsilon1SToMuMu_2MuEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START36_V9_S09-v1/0065/5684C8D4-8779-DF11-9D40-90E6BA0D0988.root'
#    '/store/mc/Fall10/Upsilon1SToMuMu_2MuEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v1/0023/F83E1E4F-7FCD-DF11-B253-002219826BD1.root'
    
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("SimpleNtple.root"),
      closeFileFast = cms.untracked.bool(True)
)


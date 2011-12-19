import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.photonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

def makeMiBiCommonNT_noPAT(process, GlobalTag, HLT='HLT', MC=False, MCType='Other'):

    # Setup the process
    process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryDB_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    
    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    # Out
    process.out = cms.OutputModule(
        "PoolOutputModule",
        outputCommands = cms.untracked.vstring(),
        fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
    )
    #process.e = cms.EndPath(process.out)
    
    
    
    process.load("PhysicsTools.NtupleUtils.PUDumper_cfi")
    process.PUDumper.mcFlag = cms.untracked.bool(MC)
    
    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")
    
    
    
    #--------------------------
    # Counter1: All read events
    process.AllEvents = process.AllPassFilter.clone()
    

    
    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Sequence(
        process.PUDumper * 
        process.AllEvents # -> Counter
        )
    
    
    
    #----------------------
    # Lepton/Photon Filters
    process.load('PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi')
    
    process.LeptonsFilter = countPatLeptons.clone(
      electronSource = cms.InputTag("selectedPatElectrons"),
      muonSource     = cms.InputTag("selectedPatMuons"),
      minNumber      = cms.uint32(1)
     )

    process.LeptonsFilterPFlow = countPatLeptons.clone(
      electronSource = cms.InputTag("selectedPatElectronsPFlow"),
      muonSource     = cms.InputTag("selectedPatMuonsPFlow"),
      minNumber      = cms.uint32(1)
     )
    
    
    
    #------------
    # Jet Filters
    process.load('PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi')    
    
    process.JetFilterAK5PF = countPatJets.clone(
      src = cms.InputTag("selectedPatJetsAK5PF"),
      minNumber      = cms.uint32(2)
    )
    
    process.JetFilterPFlow = countPatJets.clone(
      src = cms.InputTag("selectedPatJetsPFlow"),
      minNumber      = cms.uint32(2)
    )
    
    
    
    #-----------
    # Sequences & Other counters
    process.LeptonsFilterEvents = process.AllPassFilter.clone()
    process.JetFilterAK5PFEvents = process.AllPassFilter.clone()
    process.LeptonsFilterPFlowEvents = process.AllPassFilter.clone()
    process.JetFilterPFlowEvents = process.AllPassFilter.clone()
    
    process.OneLeptonTwoJetsAK5PFSeq = cms.Sequence(
        process.LeptonsFilter*
        process.LeptonsFilterEvents*
        process.JetFilterAK5PF*
        process.JetFilterAK5PFEvents
        )
    
    process.OneLeptonTwoJetsPFlowSeq = cms.Sequence(
        process.LeptonsFilterPFlow*
        process.LeptonsFilterPFlowEvents*
        process.JetFilterPFlow*
        process.JetFilterPFlowEvents
        )
    
    
    
    
    
    
    # the MiBiNTUPLE
    process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuple_noPAT_cfi")
    process.MiBiCommonNT = process.SimpleNtuple_noPAT.clone()
    
    
    if MC:
        process.MiBiCommonNT.eleHLT_names       = cms.vstring('HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20')
        process.MiBiCommonNT.muHLT_names        = cms.vstring('HLT_IsoMu24','HLT_IsoMu24_eta2p1','HLT_Mu40')
        process.MiBiCommonNT.eleFilterHLT_names = cms.vstring('')
        process.MiBiCommonNT.muFilterHLT_names  = cms.vstring('hltSingleMuL2QualIsoL3IsoFiltered24','hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered','hltSingleMu40L2QualL3Filtered40')
        process.MiBiCommonNT.jetFilterHLT_names = cms.vstring('hltEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralDiJet25Cleaned','hltEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralJet30Cleaned')
        
    if not MC:
        process.MiBiCommonNT.eleHLT_names      = cms.vstring('HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT','HLT_Ele17_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT15','HLT_Ele22_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20','HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20','HLT_Ele30_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30_PFMHT25')
        process.MiBiCommonNT.muHLT_names       = cms.vstring('HLT_IsoMu24','HLT_IsoMu24_eta2p1','HLT_Mu40')

        process.MiBiCommonNT.eleFilterHLT_names = cms.vstring()
        process.MiBiCommonNT.muFilterHLT_names  = cms.vstring('hltSingleMuL2QualIsoL3IsoFiltered24','hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered','hltSingleMu40L2QualL3Filtered40')        
        process.MiBiCommonNT.jetFilterHLT_names = cms.vstring('hltEle17CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralDiJet25Cleaned','hltEle17CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralJet30Cleaned','hltEle22CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralDiJet25Cleaned','hltEle22CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralJet30Cleaned','hltEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralDiJet25Cleaned','hltEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTCentralJet30Cleaned','hltEle27WP80CentralDiPFJet25Cleaned','hltIsoMu172p1DiCentralPFJet25Filter')
    
    
    process.MiBiCommonNT.saveMCPU              = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveProcessId         = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveGenJet            = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCPtHat           = cms.untracked.bool (MC)
    
    if MCType == 'ZW':
        process.MiBiCommonNT.saveMCZW = cms.untracked.bool (True)
    if MCType == 'TTBar':
        process.MiBiCommonNT.saveMCTTBar = cms.untracked.bool (True)
    if MCType == 'Higgs':
        process.MiBiCommonNT.saveMCHiggs = cms.untracked.bool (True)
    if MCType == 'HiggsWW':
        process.MiBiCommonNT.saveMCHiggs   = cms.untracked.bool (True)
        process.MiBiCommonNT.saveMCHiggsWW = cms.untracked.bool (True)
    
    
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.MuTag         = cms.InputTag("selectedPatMuons")
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.EleTag        = cms.InputTag("selectedPatElectrons")
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.JetTag        = cms.InputTag("selectedPatJetsAK5PF")
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.MetTag        = cms.InputTag("patMETsAK5PF")
    
    process.MiBiCommonNTOneLeptonTwoJetsPFlow = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.MuTag         = cms.InputTag("selectedPatMuonsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.EleTag        = cms.InputTag("selectedPatElectronsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.JetTag        = cms.InputTag("selectedPatJetsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.MetTag        = cms.InputTag("patMETsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.saveBS        = cms.untracked.bool(True)
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.saveRho       = cms.untracked.bool(True)
    
    
    
    
    
    
    # paths
    process.MiBiPathAK5PF = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsAK5PFSeq*process.MiBiCommonNTOneLeptonTwoJetsAK5PF)
    process.MiBiPathPFlow = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsPFlowSeq*process.MiBiCommonNTOneLeptonTwoJetsPFlow)    
    
    #process.out.outputCommands = cms.untracked.vstring('keep *')

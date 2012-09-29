import FWCore.ParameterSet.Config as cms

def makeSimpleNtuple(process,GlobalTag,runOverSandbox,runOverData):
    
    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
        )
    
    
    # Out
    process.out = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string('out.root'),
        outputCommands = cms.untracked.vstring()
        )
    
    # Standard Sequences
    process.load('Configuration.StandardSequences.Services_cff')
    process.load('Configuration.StandardSequences.GeometryDB_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load('Configuration.StandardSequences.RawToDigi_Data_cff')    
    process.load('Configuration.StandardSequences.L1Reco_cff')
    process.load('Configuration.StandardSequences.Reconstruction_cff')
    process.load('Configuration.StandardSequences.EndOfProcess_cff')
    
    # GlobalTag
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    
    #--------------------------
    # Ntuple
    #--------------------------
    
    process.load("Calibration/EcalCalibNtuple/simpleNtupleEoverP_cfi")

    if not runOverSandbox:                
        process.simpleNtupleEoverP.recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB","RECO")
        process.simpleNtupleEoverP.recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE","RECO")
        process.simpleNtupleEoverP.EleTag              = cms.InputTag("gsfElectrons")
        process.simpleNtupleEoverP.rhoTag              = cms.InputTag("kt6PFJets","rho")
    
    if not runOverData:
        process.simpleNtupleEoverP.dataFlag            = cms.untracked.bool(False)
        process.simpleNtupleEoverP.saveMCPU            = cms.untracked.bool(True)
        process.simpleNtupleEoverP.saveMCInfo          = cms.untracked.bool(True)
        
    #--------------------------
    # paths
    #--------------------------
    
    process.simpleNtuple_step = cms.Path(
        #process.hltfilter
        process.simpleNtupleEoverP
        )

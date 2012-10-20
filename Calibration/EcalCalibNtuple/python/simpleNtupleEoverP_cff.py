import FWCore.ParameterSet.Config as cms

def makeSimpleNtuple(process,GlobalTag,runOverSandbox,runOverAlcaReco,runOverData):
    
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

    if runOverAlcaReco:
        process.simpleNtupleEoverP.recHitCollection_EB =  cms.InputTag("alCaIsolatedElectrons","alcaBarrelHits")
        process.simpleNtupleEoverP.recHitCollection_EE =  cms.InputTag("alCaIsolatedElectrons","alcaEndcapHits")
        process.simpleNtupleEoverP.EleTag              =  cms.InputTag("gsfElectrons")
        process.simpleNtupleEoverP.rhoTag              =  cms.InputTag("kt6PFJetsForRhoCorrection","rho")

    if runOverSandbox:                
        process.simpleNtupleEoverP.recHitCollection_EB =  cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEB","ALCARERECO")
        process.simpleNtupleEoverP.recHitCollection_EE =  cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEE","ALCARERECO")
        process.simpleNtupleEoverP.EleTag              =  cms.InputTag("electronRecalibSCAssociator","","ALCARERECO")
        process.simpleNtupleEoverP.rhoTag              =  cms.InputTag("kt6PFJetsForRhoCorrection","rho")

    if not runOverData:
        process.simpleNtupleEoverP.recHitCollection_EB = cms.InputTag("ecalRecHit","reducedEcalRecHitsEB","RECO")
        process.simpleNtupleEoverP.recHitCollection_EE = cms.InputTag("ecalRecHit","reducedEcalRecHitsEE","RECO")
        process.simpleNtupleEoverP.dataFlag            = cms.untracked.bool(False)
        process.simpleNtupleEoverP.saveMCPU            = cms.untracked.bool(True)
        process.simpleNtupleEoverP.saveMCInfo          = cms.untracked.bool(True)

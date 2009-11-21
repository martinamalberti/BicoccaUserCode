import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag="IDEAL_V9::All"
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("Geometry.CommonDetUnit.bareGlobalTrackingGeometry_cfi")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service(
    "MessageLogger",
    default = cms.untracked.PSet(
        FwkReport = cms.untracked.PSet(
            reportEvery = cms.untracked.int32(5000),
            limit = cms.untracked.int32(10000000)
            )
        )
    )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("efficiencies.root"),
    closeFileFast = cms.untracked.bool(True)
    )






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        )
    )






# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_sisCone5CaloJets_cff")
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_correctedSisCone5CaloJets_cff")
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_sisCone5PFJets_cff")






# --- THE MC FILTER --- --- --- --- --- --- --- --- --- --- ---

process.MCDecayModeFilterAll.eventType = cms.untracked.int32(EVENTTYPE)


# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

process.VBFNtupleCaloJetsSisCone5CaloJets.eventType = cms.untracked.int32(EVENTTYPE)
process.VBFNtupleCaloJetsSisCone5CaloJets.fileName = cms.untracked.string("OUTPUTFILENAME_sisCone5CaloJets.root")

process.VBFNtupleCaloJetsCorrectedSisCone5CaloJets.eventType = cms.untracked.int32(EVENTTYPE)
process.VBFNtupleCaloJetsCorrectedSisCone5CaloJets.fileName = cms.untracked.string("OUTPUTFILENAME_correctedSisCone5CaloJets.root")

process.VBFNtuplePFJetsSisCone5PFJets.eventType = cms.untracked.int32(EVENTTYPE)
process.VBFNtuplePFJetsSisCone5PFJets.fileName = cms.untracked.string("OUTPUTFILENAME_sisCone5PFJets.root")












# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# --- PATHS  --- --- --- --- --- --- --- --- --- --- ---
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.p1 = cms.Path(
        process.VBFPreselectionSequenceSisCone5CaloJets
        )

process.p2 = cms.Path(
        process.VBFPreselectionSequenceCorrectedSisCone5CaloJets
        )

process.p3 = cms.Path(
        process.VBFPreselectionSequenceSisCone5PFJets
        )

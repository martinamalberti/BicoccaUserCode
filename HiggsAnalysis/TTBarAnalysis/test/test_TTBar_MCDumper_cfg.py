import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "MC_31X_V5::All"

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'


process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("MCDumper_TTBar.root"),
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
        'file:/tmp/amassiro/FE230B1F-E3DB-DE11-8620-002219826BD1.root'
        )
    )




# --- ====== ---
# --- Dumper --- 
# --- ====== ---

process.NtupleMC = cms.EDAnalyzer(
    "NtupleMC",
     genJetTag       = cms.InputTag("sisCone5GenJets"),
     MCtruthTag      = cms.InputTag("genParticles"),
     verbosity       = cms.untracked.bool(False),
)

# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- ---
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("Configuration.EventContent.EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('TTBarPreselectionTEST.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))

# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
          

process.p1 = cms.Path(
    process.NtupleMC
    )

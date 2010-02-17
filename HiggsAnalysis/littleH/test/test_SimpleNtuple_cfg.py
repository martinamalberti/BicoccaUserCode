import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_31X_V9::All'

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'


# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'file:/tmp/dimatteo/EA493E35-D787-DE11-843A-00215E221692.root',
#         'file:/tmp/dimatteo/RECO_A0prod_1.root',
)
    )

# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- SEQUENCES --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
     
# --- SimpleNtple --- --- --- --- --- --- --- --- --- --- ---                           
process.load("HiggsAnalysis.littleH.SimpleNtple_cfi")

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

process.SimpleNtpleSequence = cms.Sequence(
process.SimpleNtple
)

# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.p1 = cms.Path(
process.SimpleNtpleSequence               )

# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("SimpleTree_ppMuX.root"),
    closeFileFast = cms.untracked.bool(True),
    )

process.options = cms.untracked.PSet(
  fileMode = cms.untracked.string('FULLMERGE')
)

# --- ======== --- --- --- --- --- --- --- --- --- --- --- 
# --- SCHEDULE --- --- --- --- --- --- --- --- --- --- --- 
# --- ======== --- --- --- --- --- --- --- --- --- --- --- 


# Schedule definition
process.schedule = cms.Schedule(process.p1)



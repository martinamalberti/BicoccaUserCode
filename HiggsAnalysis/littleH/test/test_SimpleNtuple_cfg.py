import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# process.GlobalTag.globaltag = 'IDEAL_V11::All'
# process.GlobalTag.globaltag = 'DESIGN_3X_V8A::All'
process.GlobalTag.globaltag = 'MC_31X_V8::All'

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
#         'file:/tmp/dimatteo/66E38069-7EDB-DE11-9027-002264055CE4.root',
        'file:/tmp/dimatteo/EA493E35-D787-DE11-843A-00215E221692.root',
        )
    )

process.load("HiggsAnalysis.littleH.SimpleNtple_cfi")

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("SimpleTree_Upsilon.root"),
    closeFileFast = cms.untracked.bool(True),
    )

process.SimpleNtpleSequence = cms.Sequence(
 process.SimpleNtple
)

process.options = cms.untracked.PSet(
  fileMode = cms.untracked.string('FULLMERGE')
)

process.p1 = cms.Path(
process.SimpleNtpleSequence               )

# Schedule definition
process.schedule = cms.Schedule(process.p1)



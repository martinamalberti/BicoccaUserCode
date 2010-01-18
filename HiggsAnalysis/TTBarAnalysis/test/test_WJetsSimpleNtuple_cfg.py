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
process.GlobalTag.globaltag = 'MC_31X_V3::All'

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'


# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
#         'file:/tmp/amassiro/F03520F8-5EE1-DE11-98A8-001AA0095655.root',      
        'file:/tmp/amassiro/VBFHWW2l2nuTest_1712.root',      
        )
    )





# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 
# --- THE VBF SELECTIONS   --- --- --- --- --- --- --- --- --- --- --- 
# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 

# --- SimpleNtple --- --- --- --- --- --- --- --- --- --- --- 


process.SimpleNtpleWJets = cms.EDAnalyzer(
    "SimpleNtpleWJets",
     MCtruthTag      = cms.InputTag("genParticles"),
     verbosity = cms.untracked.bool(True),
     eventType = cms.untracked.int32(0), 
)

process.WJetsSimpleNtpleSequence = cms.Sequence(
 process.SimpleNtpleWJets
)



# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

                              
process.SimpleNtpleWJetsPath = cms.Path(
   process.WJetsSimpleNtpleSequence
)



# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.load("Configuration.EventContent.EventContent_cff")

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("/tmp/amassiro/VBF_SimpleTree_WJets.root"),
    closeFileFast = cms.untracked.bool(True),
    )




# --- ======== --- --- --- --- --- --- --- --- --- --- --- 
# --- SCHEDULE --- --- --- --- --- --- --- --- --- --- --- 
# --- ======== --- --- --- --- --- --- --- --- --- --- --- 


# process.schedule = cms.Schedule(process.newBtaggingPath,process.SimpleNtpleWJetsPath) 
process.schedule = cms.Schedule(process.SimpleNtpleWJetsPath) 



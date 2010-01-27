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
    fileName = cms.string("VBF_SimpleTree_TTBar.root"),
    closeFileFast = cms.untracked.bool(True)
    )


 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'file:/tmp/amassiro/FE230B1F-E3DB-DE11-8620-002219826BD1.root'
        )
    )




# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PRESELECTION  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff")
process.MCDecayModeFilterAll.eventType = cms.untracked.int32(1)



# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# antikt5CaloJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.TTBarAnalysis.TTBarPreselection_antikt5CaloJets_cff")
process.TTBarNtupleAntikt5CaloJets.eventType = cms.untracked.int32(0)
process.TTBarNtupleAntikt5CaloJets.saveMC = cms.untracked.bool(True)

process.load("HiggsAnalysis.TTBarAnalysis.TTBarPreselection_L2L3Antikt5CaloJets_cff")
process.TTBarNtupleL2L3Antikt5CaloJets.eventType = cms.untracked.int32(0)
process.TTBarNtupleL2L3Antikt5CaloJets.saveMC = cms.untracked.bool(True)



# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# antikt5PFJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.TTBarAnalysis.TTBarPreselection_antikt5PFJets_cff")
process.TTBarNtupleAntikt5PFJets.eventType = cms.untracked.int32(0)
process.TTBarNtupleAntikt5PFJets.saveMC = cms.untracked.bool(True)

process.load("HiggsAnalysis.TTBarAnalysis.TTBarPreselection_L2L3Antikt5PFJets_cff")
process.TTBarNtupleL2L3Antikt5PFJets.eventType = cms.untracked.int32(0)
process.TTBarNtupleL2L3Antikt5PFJets.saveMC = cms.untracked.bool(True)




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
          

process.p3 = cms.Path(
    process.TTBarPreselectionSequenceAntikt5CaloJets
    )

process.p4 = cms.Path(
    process.TTBarPreselectionSequenceL2L3Antikt5CaloJets
    )

process.p5 = cms.Path(
    process.TTBarPreselectionSequenceAntikt5PFJets
    )

process.p6 = cms.Path(
    process.TTBarPreselectionSequenceL2L3Antikt5PFJets
    )




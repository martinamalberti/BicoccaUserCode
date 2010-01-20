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
process.GlobalTag.globaltag="MC_31X_V5::All"

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'


process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("./efficiencies.root"),
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
        #'file:/data/NTUPLES/VBF/qqHWW_lnujj/H200/CMSSWfile_10.root',
        'file:/gwtera5/users/data/NTUPLES/VBF/CMSSWfile_3_1_X.root'
        )
    )






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PRESELECTION  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_sisCone5CaloJets_cff")
#process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_correctedSisCone5CaloJets_cff")
#process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_sisCone5PFJets_cff")






# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- ---
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("Configuration.EventContent.EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('VBFPreselectionTEST.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
process.p1 = cms.Path(
    process.VBFPreselectionSequenceSisCone5CaloJets
    )

#process.p2 = cms.Path(
#    process.VBFPreselectionSequenceCorrectedSisCone5CaloJets
#    )

#process.p3 = cms.Path(
#    process.VBFPreselectionSequenceSisCone5PFJets
#    )

#process.o = cms.EndPath( process.out )

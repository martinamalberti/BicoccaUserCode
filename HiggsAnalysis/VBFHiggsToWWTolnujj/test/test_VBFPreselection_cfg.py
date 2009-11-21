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
    #destinations = cms.untracked.vstring('detailedInfo'),
    #detailedInfo = cms.untracked.PSet(
    #FwkReport = cms.untracked.PSet(
    #    reportEvery = cms.untracked.int32(5000),
    #    limit = cms.untracked.int32(10000000)
    #    )
    #),
    default = cms.untracked.PSet(
        FwkReport = cms.untracked.PSet(
            reportEvery = cms.untracked.int32(1),
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
        'file:/data/NTUPLES/VBF/qqHWW_lnujj/H200/CMSSWfile_10.root',
        #'rfio:/castor/cern.ch/user/a/amassiro/VBF/090603_Everything_Skimmed_4Cluster/VVJets_madgraph_Fall08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_1.root'
        )
    )






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PRESELECTION  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_sisCone5CaloJets_cff")
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_correctedSisCone5CaloJets_cff")
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPreselection_sisCone5PFJets_cff")






# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- ---
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("Configuration.EventContent.EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('VBFCazziTEST.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))






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

#process.o = cms.EndPath( process.out )

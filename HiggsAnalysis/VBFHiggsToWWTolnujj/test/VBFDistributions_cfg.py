import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

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
        'file:/data/NTUPLES/VBF/qqHWW_lnujj/H200/CMSSWfile_10.root',
        )
    )






# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 
# --- THE VBF SELECTIONS   --- --- --- --- --- --- --- --- --- --- --- 
# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 

# --- THE MC FILTER --- --- --- --- --- --- --- --- --- --- --- 

process.load("HiggsAnalysis.VBFHiggsToVV.MCDecayModeFilter_cfi")
process.MCDecayModeFilter.H_W_W = cms.bool(True)
process.MCDecayModeFilter.V1_e_nue = cms.bool(True)
process.MCDecayModeFilter.V1_mu_numu = cms.bool(True)
process.MCDecayModeFilter.V1_tau_nutau = cms.bool(False)
process.MCDecayModeFilter.V2_d_u = cms.bool(True)
process.MCDecayModeFilter.V2_d_c = cms.bool(True)
process.MCDecayModeFilter.V2_s_u = cms.bool(True)
process.MCDecayModeFilter.V2_s_c = cms.bool(True)
process.MCDecayModeFilter.V2_b_u = cms.bool(True)
process.MCDecayModeFilter.V2_b_c = cms.bool(True)



# --- THE DISTRIBUTIONS --- --- --- --- --- --- --- --- --- --- --- 

process.load("HiggsAnalysis.VBFHiggsToVV.VBFElectronDistributions_cfi")
process.VBFElectronDistributions.fileName = cms.untracked.string("./VBFElectronDistributions.root")

process.load("HiggsAnalysis.VBFHiggsToVV.VBFMuonDistributions_cfi")
process.VBFMuonDistributions.fileName = cms.untracked.string("./VBFMuonDistributions.root")

process.load("HiggsAnalysis.VBFHiggsToVV.VBFLeptonDistributions_cfi")
process.VBFLeptonDistributions.fileName = cms.untracked.string("./VBFLeptonDistributions.root")

process.load("HiggsAnalysis.VBFHiggsToVV.VBFJetDistributions_cfi")
process.VBFCaloJetDistributions.fileName = cms.untracked.string("./VBFCaloJetDistributions.root")






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
process.p = cms.Path(
    process.MCDecayModeFilter + 
    process.VBFElectronDistributionsSequence +
    process.VBFMuonDistributionsSequence +
    process.VBFLeptonDistributionsSequence +
    process.VBFCaloJetDistributionsSequence
    )

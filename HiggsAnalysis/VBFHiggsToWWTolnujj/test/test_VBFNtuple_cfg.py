import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')

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
                     reportEvery = cms.untracked.int32(5000),
                     limit = cms.untracked.int32(10000000)
                     )
                 )
             )

process.TFileService = cms.Service("TFileService",
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



# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- --- 

process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFNtuple_cfi")
process.VBFNtupleCaloJets.fileName = cms.untracked.string("VBFNtupleCaloJetsTEST.root")





# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
process.p = cms.Path(
    process.MCDecayModeFilter + 
    process.VBFNtupleCaloJets
    )

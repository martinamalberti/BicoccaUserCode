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
#process.GlobalTag.globaltag = "MC_31X_V5::All"
process.GlobalTag.globaltag = "START3X_V26::All"

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger = cms.Service(
#    "MessageLogger",
#    #destinations = cms.untracked.vstring('detailedInfo'),
#    #detailedInfo = cms.untracked.PSet(
#    #FwkReport = cms.untracked.PSet(
#    #    reportEvery = cms.untracked.int32(5000),
#    #    limit = cms.untracked.int32(10000000)
#    #    )
#    #),
#    default = cms.untracked.PSet(
#        FwkReport = cms.untracked.PSet(
#            reportEvery = cms.untracked.int32(1000),
#            limit = cms.untracked.int32(10000000)
#            )
#        )
#    )


process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("./SimpleNtple.root"),
    closeFileFast = cms.untracked.bool(True)
    )






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source(
    "PoolSource",
    #debugFlag = cms.untracked.bool(True),
    #debugVebosity = cms.untracked.uint32(1),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
        #'file:/data/NTUPLES/VBF/qqHWW_lnujj/H200/CMSSWfile_10.root',
        #'file:/gwtera5/users/data/NTUPLES/VBF/CMSSWfile_3_1_X.root'
        #'file:/tmp/amassiro/0C5ECFC1-C445-DF11-8133-E0CB4E19F99B.root'
          'file:669A4128-43D0-DF11-AE93-001A92810ACE.root'
        )
    )






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PRESELECTION  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

# ---- number of leptons ----
# select events with at least one gsf electron
process.highPtEle = cms.EDFilter(
    "GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("pt > 10")
    )

process.highPtEleFilter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("highPtEle"),
    minNumber = cms.uint32(1)
    )

# select events with at least one muon
process.highPtMuon = cms.EDFilter(
    "MuonSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("pt > 10")
    )

process.highPtMuonFilter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("highPtMuon"),
    minNumber = cms.uint32(1)
    )

# select events with at least one lepton
process.highPtLeptonFilter = cms.Sequence(
    highPtEleFilter + 
    highPtMuonFilter
)


# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# antikt5CaloJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

# ---- number of jets ----
process.highEtJetAKT5Calo = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("chetipodijet???"),
    cut = cms.string("pt > 10")
    )

process.highEtJetAKT5CaloFilter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("highEtJetAKT5Calo"),
    minNumber = cms.uint32(2)
    )

# ---- ntuple ----
from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
process.NtupleL2L3Antikt5CaloJets = SimpleNtple.clone()

process.NtupleL2L3Antikt5CaloJets.eventType = cms.untracked.int32(1)
process.NtupleL2L3Antikt5CaloJets.saveMCHiggs = cms.untracked.bool(False)
process.NtupleL2L3Antikt5CaloJets.saveMCHiggsDecay = cms.untracked.bool(False)

process.SeqAk5Calo = cms.Sequence(
 process.highPtLeptonFilter
 process.highEtJetAKT5CaloFilter
 process.NtupleL2L3Antikt5CaloJets
)

# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# antikt5PFJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---



# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- ---
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("Configuration.EventContent.EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('Preselection.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))


# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
    
process.pAk5Calo = cms.Path(
 process.SeqAk5Calo
)

import FWCore.ParameterSet.Config as cms



process = cms.Process("TEST")
process.selections = cms.PSet(

    saveFolder = cms.string("/gwpool/users/benaglia/VBF/CMSSW_2_2_13/src/HiggsAnalysis/VBFHiggsToWWTolnujj/data/"),
    eventType = cms.string("EVENTTYPE"),
    isSignal = cms.bool(False),
    crossSection = cms.double(CROSSECTION),
    jetAlgorithm = cms.string("sisCone5CaloJets"),
    printModulo = cms.untracked.int32(10000),
    maxEvent = cms.untracked.int32(-1)
)



process.cuts = cms.PSet(
    
    # VBF cuts
    mJJ_tag_MIN  = cms.double(500.),
    Deta_tag_MIN = cms.double(3.),
    
    # other cuts
    mJJ_W_MIN    = cms.double(30.),
    mJJ_W_MAX    = cms.double(70.),
    
    # CJV
    absEta_CJV_MAX = cms.double(2.5),
    
    # higgs cut
    Dphi_LJJ_MAX = cms.double(1.5),
    mass_LJJ_MAX = cms.double(160.),
    
    # higgs cut
    met_MIN = cms.double(25.),
    met_MAX = cms.double(125.)
)



process.inputEfficiencies = cms.PSet(
    inputFiles = cms.vstring(
INPUTEFFICIENCIES
    )
)



process.inputNtuples = cms.PSet(
    
    inputFiles_sisCone5CaloJets = cms.vstring(
INPUTNTUPLES_SISCONE5CALOJETS 
    ),
    
    inputFiles_correctedSisCone5CaloJets = cms.vstring(
INPUTNTUPLES_CORRECTEDSISCONE5CALOJETS 
    ),
    
    inputFiles_sisCone5PFJets = cms.vstring(
INPUTNTUPLES_SISCONE5PFJETS 
    )

)

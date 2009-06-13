import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.selections = cms.PSet(

    P_BIN = cms.untracked.int32(25),
    maxEvent = cms.untracked.int32(-1),
    useTrueVal = cms.untracked.bool(False),
    applyVeto = cms.untracked.bool(True),
    correctMuonP = cms.untracked.bool(True),
    energyCorrFactor = cms.untracked.double(0.97),


    muonLegUpOK = cms.untracked.bool(False),
    muonLegDownOK = cms.untracked.bool(True),

    
    muonPMIN = cms.untracked.double(5.0),    
    muonPMAX = cms.untracked.double(1000.),    
    muonPErrorOverPMAX = cms.untracked.double(0.275),
    muonPCollTHRESH = cms.untracked.double(5.0),
    muonEOverPMAX = cms.untracked.double(1e+12),
    

    muond0MAX = cms.untracked.double(1e+12),
    muondzMAX = cms.untracked.double(1e+12),
    muondRMAX = cms.untracked.double(1e+12),


    muonNChi2MAX = cms.untracked.double(1e+12),
    muonNHitsMIN = cms.untracked.double(1.0),


    muonTkLengthInEcalMIN = cms.untracked.double(0.0),
    muonTkLengthInEcalMAX = cms.untracked.double(1e+12),
    xtalTkLengthSumMIN = cms.untracked.double(0.0),
    xtalTkLengthSumMAX = cms.untracked.double(1e+12),


    nXtalsInSuperClusterMIN = cms.untracked.double(0.0),
    nXtalsInSuperClusterMAX = cms.untracked.double(1e+12),


    muonAngleMIN = cms.untracked.double(0.0),
    muonAngleMAX = cms.untracked.double(0.2),
    muonAngleXYPlaneMAX = cms.untracked.double(1e+12),

    muonInnTkHitZMIN = cms.untracked.double(0.0),
    muonInnTkHitZMAX = cms.untracked.double(1e+12),




    superClusterPhiMIN = cms.untracked.double(-3.14159),
    superClusterPhiMAX = cms.untracked.double(0.),
    superClusterEtaMIN = cms.untracked.double(-1e+12),
    superClusterEtaMAX = cms.untracked.double(1e+12),




    badRegionIPhiMIN = cms.untracked.vint32(301, 121),
    badRegionIPhiMAX = cms.untracked.vint32(320, 140),
    badRegionIEtaMIN = cms.untracked.vint32(1, 1),
    badRegionIEtaMAX = cms.untracked.vint32(85, 85)
)





process.calibSelections = cms.PSet(
    stat = cms.int32(-1),
    phiSTART = cms.int32(1),
    algorithm = cms.string('L3'),
    etaWIDTH = cms.int32(5),
    etaSTART = cms.int32(-85),
    maxCoeff = cms.double(5.0),
    phiWIDTH = cms.int32(360),
    etaEND = cms.int32(86),
    minCoeff = cms.double(0.0),
    numberOfLoops = cms.int32(1),
    phiEND = cms.int32(361),
    usingBlockSolver = cms.int32(1),
    stat_xtalEvtMAX = cms.int32(100000)
)
process.inputNtuples = cms.PSet(
    maxEvents = cms.int32(-1),
    inputFiles = cms.vstring(
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66615.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66621.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66627.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66637.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66657.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66668.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66676.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66692.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66703.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66706.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66709.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66711.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66714.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66716.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66720.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66722.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66733.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66739.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66740.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66746.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66748.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66752.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66756.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_66757.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_67128.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_67139.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_67141.root', 
        '/gwtera5/users/data/NTUPLES/CRAFT1/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/cloned_matching_67147.root'
    )
)


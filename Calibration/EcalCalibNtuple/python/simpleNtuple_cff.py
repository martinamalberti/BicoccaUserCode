import FWCore.ParameterSet.Config as cms

# Standard PAT Configuration File
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *



def makeSimpleNtuple(process,GlobalTag,ReReco=False):

    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
        )
    
    if ReReco:
        process.source.inputCommands = cms.untracked.vstring(
            "drop *_*_*_RECO",
            "drop *_MEtoEDMConverter_*_*",
            "keep FEDRawDataCollection_*_*_*"
            )
    
    # Out
    process.out = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string('out.root'),
        outputCommands = cms.untracked.vstring()
        )

    # Standard Sequences
    process.load('Configuration.StandardSequences.Services_cff')
    process.load('Configuration.StandardSequences.Geometry_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load('Configuration.StandardSequences.RawToDigi_Data_cff')    
    process.load('Configuration.StandardSequences.L1Reco_cff')
    process.load('Configuration.StandardSequences.Reconstruction_cff')
    process.load('Configuration.StandardSequences.EndOfProcess_cff')

    # GlobalTag
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    
    
    #--------------------------
    #Define PAT sequence
    #--------------------------
    
    ## remove MC matching from the default sequence
    ## remove tau from the default sequence
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    process.load("PhysicsTools.PatAlgos.tools.pfTools")
    removeMCMatching(process, ['All'])
    removeSpecificPATObjects( process, ['Photons'] )
    removeSpecificPATObjects( process, ['Taus'] )    
    process.patDefaultSequence.remove( process.patPhotons )
    process.patDefaultSequence.remove( process.patTaus )    
    
    # add electron ID
    #process.load("Calibration.EcalCalibNtuple.CiC_eIDSequence_cff")
    #
    #process.patElectronIDs   = cms.Sequence(process.CiC_eIDSequence)
    #process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.patElectrons)
    #
    #process.patElectrons.addElectronID = cms.bool(True)
    #process.patElectrons.electronIDSources = cms.PSet(
    #    eidVeryLoose  = cms.InputTag("eidVeryLoose"),
    #    eidLoose      = cms.InputTag("eidLoose"),
    #    eidMedium     = cms.InputTag("eidMedium"),
    #    eidTight      = cms.InputTag("eidTight"),
    #    eidSuperTight = cms.InputTag("eidSuperTight")
    #    )
    ##
    #process.patElectrons.addGenMatch = cms.bool(False)
    #process.patElectrons.embedGenMatch = cms.bool(False)
    
    
    # Add tcMET and pfMET
    addTcMET(process, 'TC')
    addPfMET(process, 'PF')
    
    
    # Jet energy corrections to use:
    inputJetCorrLabel = ('AK5PF', ['L1Offset', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
    
    # Add PF jets
    switchJetCollection(process,
                        cms.InputTag('ak5PFJets'),
                        doJTA        = True,
                        doBTagging   = True,
                        jetCorrLabel = inputJetCorrLabel,
                        doType1MET   = True,
                        genJetCollection=cms.InputTag("ak5GenJets"),
                        doJetID      = True
                        )
    
    process.patJets.addTagInfos = True
    process.patJets.tagInfoSources  = cms.VInputTag( cms.InputTag("secondaryVertexTagInfosAOD") )
    
    # PAT selection layer
    process.selectedPatElectrons.cut = cms.string("pt > 15.")
    process.selectedPatMuons.cut     = cms.string("pt >  5.")
    process.selectedPatJets.cut      = cms.string("pt > 15.")
    
    
    
    
    
    
    #--------------------------
    # Ntuple
    #--------------------------
    
    process.load("Calibration/EcalCalibNtuple/simpleNtuple_cfi")


    
    #--------------------------
    # filters
    #--------------------------
    
    process.load('PhysicsTools.NtupleUtils.AllPassFilter_cfi')
    process.AllPassFilterBegin = process.AllPassFilter.clone()
    
    
    # filter on primary vertex
    process.primaryVertexFilter = cms.EDFilter(
        "GoodVertexFilter",
        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
        minimumNDOF = cms.uint32(4) ,
        maxAbsZ = cms.double(24),
        maxd0 = cms.double(2)
        )
    
    process.AllPassFilterGoodVertexFilter = process.AllPassFilter.clone()
    
    
    # FilterOutScraping
    process.noScrapingFilter = cms.EDFilter(
        "FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
        )
    
    process.AllPassFilterNoScrapingFilter = process.AllPassFilter.clone()
    
    
    # HB + HE noise filtering
    process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
    
    process.AllPassFilterHBHENoiseFilter = process.AllPassFilter.clone()
    
    
    # select events with at least one gsf electron
    process.highetele = cms.EDFilter(
        "GsfElectronSelector",
        src = cms.InputTag("gsfElectrons"),
        cut = cms.string("superCluster().get().energy()*sin(theta())> 15.")
        )
    
    process.highetFilter = cms.EDFilter(
        "CandViewCountFilter",
        src = cms.InputTag("highetele"),
        minNumber = cms.uint32(1)
        )
    
    process.AllPassFilterElectronFilter = process.AllPassFilter.clone()
    
    
    
    
    
    
    #--------------------------
    # paths
    #--------------------------
    
    process.simpleNtuple_step = cms.Path(
        process.AllPassFilterBegin *
        process.primaryVertexFilter *
        process.AllPassFilterGoodVertexFilter *
        process.noScrapingFilter *
        process.AllPassFilterNoScrapingFilter *
        process.HBHENoiseFilter *
        process.AllPassFilterHBHENoiseFilter *
        process.highetele *
        process.highetFilter *
        process.AllPassFilterElectronFilter *
        process.patDefaultSequence *
        process.simpleNtuple
        )
    
    process.raw2digi_step = cms.Path(process.RawToDigi)
    process.L1Reco_step = cms.Path(process.L1Reco)
    process.reconstruction_step = cms.Path(process.reconstruction)
    process.endjob_step = cms.Path(process.endOfProcess)

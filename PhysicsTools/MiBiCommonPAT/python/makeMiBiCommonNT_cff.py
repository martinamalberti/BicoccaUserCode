import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *

from PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.photonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

def makeMiBiCommonNT(process, GlobalTag, HLT='HLT', MC=False, MCType='Other'):

    # Setup the process
    process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryDB_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    

    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    # Out
    process.out = cms.OutputModule(
        "PoolOutputModule",
        outputCommands = cms.untracked.vstring(),
        fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
    )
    #process.e = cms.EndPath(process.out)
    
    
    
    process.load("PhysicsTools.NtupleUtils.PUDumper_cfi")
    process.PUDumper.mcFlag = cms.untracked.bool(MC)
    
    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")
    
    #--------------------------
    # Counter1: All read events
    process.AllEvents = process.AllPassFilter.clone()

    
    #--------------------------
    # Counter2: Scraping filter
    process.scrapingFilter = cms.EDFilter(
        "FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
    )
    
    process.NonScrapedEvents = process.AllPassFilter.clone()

    
    #-----------------------------------
    # Counter3: Filter on primary vertex
    
    # Get a list of good primary vertices, in 42x, these are DAF vertices
    from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
    process.goodOfflinePrimaryVertices = cms.EDFilter(
        "PrimaryVertexObjectFilter",
        filterParams = pvSelector.clone( minNdof = cms.double(3.0), maxZ = cms.double(24.0) ),
        src=cms.InputTag('offlinePrimaryVertices')
    )
    
    process.GoodVtxEvents = process.AllPassFilter.clone()

    
    #------------------
    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    process.load("PhysicsTools.PatAlgos.tools.pfTools")
    postfix = "PFlow"
    
    usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=MC, postfix = postfix, jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute']) )
    process.pfPileUpPFlow.Enable = True
    process.pfPileUpPFlow.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
    process.pfPileUpPFlow.checkClosestZVertex = cms.bool(False)
    process.pfJetsPFlow.doAreaFastjet = True
    process.pfJetsPFlow.doRhoFastjet = False
    
    if not MC:
     removeMCMatching(process, ['All'])
    
    
    
    
    
    #### jets ####    
    process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
    process.load('RecoJets.Configuration.RecoPFJets_cff')
    from RecoJets.JetProducers.kt4PFJets_cfi import *
    
    # compute FastJet rho to correct jets
    process.kt6PFJets = kt4PFJets.clone(
        rParam = cms.double(0.6),
        doAreaFastjet = cms.bool(True),
        doRhoFastjet = cms.bool(True)
        )
    process.patJetCorrFactors.rho = cms.InputTag("kt6PFJets","rho")
    
    # compute CHS rho to correct jets
    process.kt6PFJetsPFlow = kt4PFJets.clone(
        rParam = cms.double(0.6),
        src = cms.InputTag('pfNoElectron'+postfix),
        doAreaFastjet = cms.bool(True),
        doRhoFastjet = cms.bool(True)
        )
    process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJetsPFlow", "rho")
    
    # compute FastJet rho to correct isolation
    process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
    process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)
    
    # compute area for ak5PFJets
    process.ak5PFJets.doAreaFastjet = True
    
    
    
    # ---------------
    # add collections
    addTcMET(process, 'TC')
    addPfMET(process, 'PF')
    
    if not MC:
        addJetCollection(
            process,
            cms.InputTag('ak5PFJets'),
            'AK5',
            'PF',
            doJTA        = True,
            doBTagging   = True,
            jetCorrLabel = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])),
            doType1MET   = True,
            doL1Cleaning = True,
            doL1Counters = False,
            genJetCollection=cms.InputTag("ak5GenJets"),
            doJetID      = True,
            jetIdLabel   = "ak5"
            )
    
    if MC:
        addJetCollection(
            process,
            cms.InputTag('ak5PFJets'),
            'AK5',
            'PF',
            doJTA        = True,
            doBTagging   = True,
            jetCorrLabel = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])),
            doType1MET   = True,
            doL1Cleaning = True,
            doL1Counters = False,
            genJetCollection=cms.InputTag("ak5GenJets"),
            doJetID      = True,
            jetIdLabel   = "ak5"
            )        
    
    # -------------------
    # pat selection layer
    #process.selectedPatElectrons.cut      = cms.string("pt > 35. & abs(eta) < 2.5")  #fede
    #process.selectedPatElectronsPFlow.cut = cms.string("pt > 35. & abs(eta) < 2.5")  #fede
    
    process.selectedPatElectrons.cut      = cms.string("pt > 10. & abs(eta) < 2.5")
    process.selectedPatElectronsPFlow.cut = cms.string("pt > 10. & abs(eta) < 2.5")
    
    process.selectedPatMuons.cut      = cms.string("pt > 10. & abs(eta) < 2.5")
    process.selectedPatMuonsPFlow.cut = cms.string("pt > 10. & abs(eta) < 2.5")
    
    process.selectedPatJets.cut        = cms.string("pt > 15. & abs(eta) < 5")
    process.selectedPatJetsPFlow.cut   = cms.string("pt > 15. & abs(eta) < 5")    
    process.selectedPatJetsAK5PF.cut   = cms.string("pt > 15. & abs(eta) < 5")
    
    process.selectedPatPhotons.cut      = cms.string("pt > 10. & abs(eta) < 5")
    process.selectedPatPhotonsPFlow.cut = cms.string("pt > 10. & abs(eta) < 5")    
    
    # the HCAL Noise Filter
    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')



    # PF isolation
    process.load("PhysicsTools.MiBiCommonPAT.muonPFIsoMapProd_cfi")
    process.muSmurfPF = process.muonPFIsoMapProd.clone()
    process.preMuonSequence = cms.Sequence(process.muSmurfPF)
    
    process.patMuons.userData.userFloats.src = cms.VInputTag(
      cms.InputTag("muSmurfPF")
    )
    
    process.load("PhysicsTools.MiBiCommonPAT.electronPFIsoMapProd_cfi")
    process.eleSmurfPF = process.electronPFIsoMapProd.clone()
    process.preElectronSequence = cms.Sequence(process.eleSmurfPF)
    
    process.load("RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi")
    process.egammaIDLikelihood = process.eidLikelihoodExt.clone()
    process.electronIDLH = cms.Sequence( process.egammaIDLikelihood )
    
    process.patElectrons.userData.userFloats.src = cms.VInputTag(
      cms.InputTag("eleSmurfPF"),
      cms.InputTag("egammaIDLikelihood")
    )

    process.prePatSequence = cms.Sequence( process.electronIDLH + process.preElectronSequence + process.preMuonSequence )
    
    
    
    # Add the KT6 producer to the sequence
    getattr(process,"patPF2PATSequence"+postfix).replace(
        getattr(process,"pfNoElectron"+postfix),
        getattr(process,"pfNoElectron"+postfix)*process.kt6PFJetsPFlow
        )
    
    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Sequence(
        process.PUDumper * 
        process.AllEvents * # -> Counter
        process.scrapingFilter *
        process.NonScrapedEvents * # -> Counter
        process.goodOfflinePrimaryVertices *
        process.GoodVtxEvents * # -> Counter
        process.HBHENoiseFilterResultProducer *
        process.prePatSequence *
        getattr(process,"patPF2PATSequence"+postfix) *
        process.kt6PFJets *        
        process.kt6PFJetsForIsolation *
        process.ak5PFJets *
        process.patDefaultSequence
        )
    
    
    
    #----------------------
    # Lepton/Photon Filters
    process.load('PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi')
    
    process.LeptonsFilter = countPatLeptons.clone(
      electronSource = cms.InputTag("selectedPatElectrons"),
      muonSource     = cms.InputTag("selectedPatMuons"),
      minNumber      = cms.uint32(1)
     )

    process.LeptonsFilterPFlow = countPatLeptons.clone(
      electronSource = cms.InputTag("selectedPatElectronsPFlow"),
      muonSource     = cms.InputTag("selectedPatMuonsPFlow"),
      minNumber      = cms.uint32(1)
     )
    
    
    process.load('PhysicsTools.PatAlgos.selectionLayer1.photonCountFilter_cfi')

    process.PhotonsFilter = countPatPhotons.clone(
      src       = cms.InputTag("selectedPatPhotons"),
      minNumber = cms.uint32(2)
     )

    
    process.load('PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi')
    
    process.ElectronsFilter = countPatElectrons.clone(
      src       = cms.InputTag("selectedPatElectrons"),
      minNumber = cms.uint32(1)
     )
    
    process.ElectronsPFlowFilter = countPatElectrons.clone(
      src       = cms.InputTag("selectedPatElectronsPFlow"),
      minNumber = cms.uint32(1)
     )
    
    
    
    #------------
    # Jet Filters
    process.load('PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi')    
    
    process.JetFilterAK5PF = countPatJets.clone(
      src = cms.InputTag("selectedPatJetsAK5PF"),
      minNumber      = cms.uint32(2)
    )
    
    process.JetFilterPFlow = countPatJets.clone(
      src = cms.InputTag("selectedPatJetsPFlow"),
      minNumber      = cms.uint32(2)
    )
    
    
    
    #-----------
    # Sequences & Other counters
    process.LeptonsFilterEvents = process.AllPassFilter.clone()
    process.LeptonsFilterPFlowEvents = process.AllPassFilter.clone()
    process.ElectronsFilterEvents = process.AllPassFilter.clone()
    process.ElectronsPFlowFilterEvents = process.AllPassFilter.clone()
    process.JetFilterAK5PFEvents = process.AllPassFilter.clone()
    process.JetFilterPFlowEvents = process.AllPassFilter.clone()
    process.PhotonsFilterEvents = process.AllPassFilter.clone()
    
    process.OneLeptonTwoJetsAK5PFSeq = cms.Sequence(
        process.LeptonsFilter*
        process.LeptonsFilterEvents*
        process.JetFilterAK5PF*
        process.JetFilterAK5PFEvents
        )
    
    
    process.OneLeptonTwoJetsPFlowSeq = cms.Sequence(
        process.LeptonsFilterPFlow*
        process.LeptonsFilterPFlowEvents*
        process.JetFilterPFlow*
        process.JetFilterPFlowEvents
        )

    process.OneEleSeq = cms.Sequence(
        process.ElectronsFilter*
        process.ElectronsFilterEvents
        )

    process.OneElePFlowSeq = cms.Sequence(
        process.ElectronsPFlowFilter*
        process.ElectronsPFlowFilterEvents
        )


    process.TwoPhotonsSeq = cms.Sequence(
        process.PhotonsFilter*
        process.PhotonsFilterEvents
        )
    
    process.TwoJetsPFlowSeq = cms.Sequence(
        process.JetFilterPFlow*
        process.JetFilterPFlowEvents
    )
    
    process.TwoJetsAK5PFSeq = cms.Sequence(
        process.JetFilterAK5PF*
        process.JetFilterAK5PFEvents
        )
        
    

    
    # the MiBiNTUPLE
    process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuple_cfi")
    process.MiBiCommonNT = process.SimpleNtuple.clone()
    process.MiBiCommonNT.saveMCPU              = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveProcessId         = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveGenJet            = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCPtHat           = cms.untracked.bool (MC)
    process.MiBiCommonNT.savePhotonsMother     = cms.untracked.bool (False)
    process.MiBiCommonNT.saveMCTTBar           = cms.untracked.bool (False)    
    process.MiBiCommonNT.saveMCHiggs           = cms.untracked.bool (False)
    process.MiBiCommonNT.saveMCHiggsWW         = cms.untracked.bool (False)
    process.MiBiCommonNT.saveMCHiggsGammaGamma = cms.untracked.bool (False)
    process.MiBiCommonNT.saveMCZW              = cms.untracked.bool (False)
    
    if MCType == 'TTBar':
        process.MiBiCommonNT.saveMCTTBar = cms.untracked.bool (True)
    if MCType == 'Higgs':
        process.MiBiCommonNT.saveMCHiggs = cms.untracked.bool (True)
    if MCType == 'HiggsWW':
        process.MiBiCommonNT.saveMCHiggs   = cms.untracked.bool (True)
        process.MiBiCommonNT.saveMCHiggsWW = cms.untracked.bool (True)
    if MCType == 'HiggsGammaGamma':
        process.MiBiCommonNT.saveMCHiggs           = cms.untracked.bool (True)
        process.MiBiCommonNT.saveMCHiggsGammaGamma = cms.untracked.bool (True)                
    if MCType == 'ZW':
        process.MiBiCommonNT.saveMCZW              = cms.untracked.bool (True)


    process.MiBiCommonNTOneLeptonTwoJetsAK5PF = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.JetTag    = cms.InputTag("patJetsAK5PF")
    
    process.MiBiCommonNTTwoJetsAK5PF = process.MiBiCommonNT.clone()
    process.MiBiCommonNTTwoJetsAK5PF.JetTag    = cms.InputTag("patJetsAK5PF")

    process.MiBiCommonNTOneLeptonTwoJetsPFlow = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.TauTag    = cms.InputTag("patTausPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.MuTag     = cms.InputTag("patMuonsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.EleTag    = cms.InputTag("patElectronsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.JetTag    = cms.InputTag("patJetsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.MetTag    = cms.InputTag("patMETsPFlow")
    
    process.MiBiCommonNTTwoJetsPFlow = process.MiBiCommonNT.clone()
    process.MiBiCommonNTTwoJetsPFlow.MuTag     = cms.InputTag("patMuonsPFlow")
    process.MiBiCommonNTTwoJetsPFlow.EleTag    = cms.InputTag("patElectronsPFlow")
    process.MiBiCommonNTTwoJetsPFlow.JetTag    = cms.InputTag("patJetsPFlow")
    process.MiBiCommonNTTwoJetsPFlow.MetTag    = cms.InputTag("patMETsPFlow")

    process.MiBiCommonNTTwoPhotons = process.MiBiCommonNT.clone()
    process.MiBiCommonNTTwoPhotons.JetTag = cms.InputTag("patJetsAK5PF")

    process.MiBiCommonNTOneElectron = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneElectron.JetTag = cms.InputTag("patJetsAK5PF")
    
    process.MiBiCommonNTOneElectronPFlow = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneElectronPFlow.MuTag     = cms.InputTag("patMuonsPFlow")
    process.MiBiCommonNTOneElectronPFlow.EleTag    = cms.InputTag("patElectronsPFlow")
    process.MiBiCommonNTOneElectronPFlow.JetTag    = cms.InputTag("patJetsPFlow")
    process.MiBiCommonNTOneElectronPFlow.MetTag    = cms.InputTag("patMETsPFlow")
    #process.MiBiCommonNTOneElectronPFlow.PhotonTag    = cms.InputTag("patPhotonsPFlow")
    
    
    
    
    # VBF paths
    process.MiBiPathAK5PF = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsAK5PFSeq*process.MiBiCommonNTOneLeptonTwoJetsAK5PF)
    process.MiBiPathPFlow = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsPFlowSeq*process.MiBiCommonNTOneLeptonTwoJetsPFlow)
    
    # GammaGamma paths
    #process.MiBiPathPhotons = cms.Path(process.MiBiCommonPAT*process.TwoPhotonsSeq*process.MiBiCommonNTTwoPhotons)
    
    # Di-jet paths
    #process.MiBiPathTwoJetsAK5PF = cms.Path(process.MiBiCommonPAT*process.TwoJetsAK5PFSeq*process.MiBiCommonNTTwoJetsAK5PF)
    #process.MiBiPathTwoJetsPFlow = cms.Path(process.MiBiCommonPAT*process.TwoJetsPFlowSeq*process.MiBiCommonNTTwoJetsPFlow)
    
    #ele path
    #process.MiBiPathOneElectron      = cms.Path(process.MiBiCommonPAT*process.OneEleSeq*process.MiBiCommonNTOneElectron)
    #process.MiBiPathOneElectronPFlow = cms.Path(process.MiBiCommonPAT*process.OneElePFlowSeq*process.MiBiCommonNTOneElectronPFlow)

    #process.out.outputCommands = cms.untracked.vstring('keep *')

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
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *


def makeMiBiCommonNT(process, GlobalTag, HLT='HLT', MC=False, MCType='Other'):


    # Setup the process
    process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryDB_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    process.options.allowUnscheduled = cms.untracked.bool(True)

    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    # Out
    from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
    process.out = cms.OutputModule(
        "PoolOutputModule",
        outputCommands = cms.untracked.vstring(),
        fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
    )
    process.e = cms.EndPath(process.out)
    
    
    
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

    #-------------------------------------
    # build muonless vertices
    #add track no muon producer
    process.load("PhysicsTools.MiBiCommonPAT.NoMuonTrackProducer_cfi")

    # re-do vertices
    process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
    from RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi import *

    process.offlinePrimaryVerticesNoMu=offlinePrimaryVertices.clone()
    process.offlinePrimaryVerticesNoMu.TrackLabel = cms.InputTag("NoMuonTrackProducer")

    process.noMuonVertexReco = cms.Sequence(process.NoMuonTrackProducer*process.offlinePrimaryVerticesNoMu)

    #-------------------------------------
    # build electron less vertices
    #add track no electron producer
    process.load("PhysicsTools.MiBiCommonPAT.NoElectronTrackProducer_cfi")

    # re-do vertices
    process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
    from RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi import *

    process.offlinePrimaryVerticesNoEle=offlinePrimaryVertices.clone()
    process.offlinePrimaryVerticesNoEle.TrackLabel = cms.InputTag("NoElectronTrackProducer")

    process.noElectronVertexReco = cms.Sequence(process.NoElectronTrackProducer*process.offlinePrimaryVerticesNoEle)

    
    #-------------------------------------
    # build electron+muon less vertices
    #add track no lepton producer
    process.load("PhysicsTools.MiBiCommonPAT.NoLeptonTrackProducer_cfi")

    # re-do vertices
    process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
    from RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi import *

    process.offlinePrimaryVerticesNoLep=offlinePrimaryVertices.clone()
    process.offlinePrimaryVerticesNoLep.TrackLabel = cms.InputTag("NoLeptonTrackProducer")

    process.noLeptonVertexReco = cms.Sequence(process.NoLeptonTrackProducer*process.offlinePrimaryVerticesNoLep)
    
    
    #------------------
    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    process.load("PhysicsTools.PatAlgos.tools.pfTools")
    postfix = "PFlow"

    if not MC:
        usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=MC, postfix = postfix )
    if MC:
        usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=MC, postfix = postfix )
    process.pfPileUpPFlow.Enable = True
    process.pfPileUpPFlow.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
    process.pfPileUpPFlow.checkClosestZVertex = cms.bool(False)
    process.pfJetsPFlow.doAreaFastjet = True
    process.pfJetsPFlow.doRhoFastjet = False
    
    if not MC:
     removeMCMatching(process, ['All'])
    
    # remove taus from the sequence       
    #removeSpecificPATObjects( process, ['Taus'] )
    #process.patDefaultSequence.remove( process.patTaus )
    
    #### jets ####    
    #process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
           
    # compute area for ak5PFJets
    from RecoJets.JetProducers.ak5PFJets_cfi import *
    process.ak5PFJets = ak5PFJets.clone()
    process.ak5PFJets.doAreaFastjet = True
        
    # ---------------
    # add collections
    addMETCollection(process, labelName='patMETTC', metSource='tcMet')
    addMETCollection(process, labelName='patMETPF', metSource='pfType1CorrectedMet')

    if not MC:
        addJetCollection(
            process,
            labelName = 'AK5PF',
            jetSource = cms.InputTag('ak5PFJets'),
            jetCorrections = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute','L2L3Residuals']), 'Type-1')
            )     
        
    
    if MC:
        addJetCollection(
            process,
            labelName = 'AK5PF',
            jetSource = cms.InputTag('ak5PFJets'),
            jetCorrections = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-1'),
            )

            
    # -------------------
    # pat selection layer
    
    process.selectedPatElectrons.cut      = cms.string("pt > 17. & abs(eta) < 2.5")
    process.selectedPatElectronsPFlow.cut = cms.string("pt > 17. & abs(eta) < 2.5")
    
    process.selectedPatMuons.cut      = cms.string("pt > 17. & abs(eta) < 2.5")
    process.selectedPatMuonsPFlow.cut = cms.string("pt > 17. & abs(eta) < 2.5")
    
    process.selectedPatJets.cut        = cms.string("pt > 15. & abs(eta) < 5")
    process.selectedPatJetsPFlow.cut   = cms.string("pt > 15. & abs(eta) < 5")    
    process.selectedPatJetsAK5PF.cut   = cms.string("pt > 15. & abs(eta) < 5")
    
    process.selectedPatPhotons.cut      = cms.string("pt > 10. & abs(eta) < 5")
    process.selectedPatPhotonsPFlow.cut = cms.string("pt > 10. & abs(eta) < 5")    
    
    # the HCAL Noise Filter
    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')

    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Sequence(
        process.PUDumper * 
        process.AllEvents * # -> Counter
        process.scrapingFilter *
        process.NonScrapedEvents * # -> Counter
        process.goodOfflinePrimaryVertices *
        process.GoodVtxEvents * # -> Counter
        process.HBHENoiseFilterResultProducer *
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


    process.load('PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi')

    process.MuonsFilter = countPatMuons.clone(
      src       = cms.InputTag("selectedPatMuons"),
      minNumber = cms.uint32(2)
    )

    

    # add inv mass cut
    # build Z-> MuMu candidates
    process.dimuons = cms.EDProducer("CandViewShallowCloneCombiner",
                                     checkCharge = cms.bool(False),
                                     cut = cms.string('mass > 60.'),
                                     decay = cms.string("selectedPatMuons selectedPatMuons")
                                     )
    # Z filter
    process.dimuonsFilter = cms.EDFilter("CandViewCountFilter",
                                         src = cms.InputTag("dimuons"),
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
    process.MuonsFilterEvents = process.AllPassFilter.clone()
     
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

    process.TwoMuonsSeq = cms.Sequence(
        process.MuonsFilter*
        process.dimuons*
        process.dimuonsFilter*
        process.MuonsFilterEvents
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
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.JetTag    = cms.InputTag("selectedPatJetsAK5PF")
    
    process.MiBiCommonNTTwoJetsAK5PF = process.MiBiCommonNT.clone()
    process.MiBiCommonNTTwoJetsAK5PF.JetTag    = cms.InputTag("patJetsAK5PF")

    process.MiBiCommonNTOneLeptonTwoJetsPFlow = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.TauTag    = cms.InputTag("patTausPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.MuTag     = cms.InputTag("patMuonsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.EleTag    = cms.InputTag("patElectronsPFlow")
    process.MiBiCommonNTOneLeptonTwoJetsPFlow.JetTag    = cms.InputTag("selectedPatJetsPFlow")
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
    
    
    
    
    # paths
    #process.MiBiPathAK5PF = cms.Path(process.MiBiCommonPAT)
    
    #process.MiBiPathAK5PF = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsAK5PFSeq*process.MiBiCommonNTOneLeptonTwoJetsAK5PF)
    #process.MiBiPathPFlow = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsPFlowSeq*process.MiBiCommonNTOneLeptonTwoJetsPFlow)    
    
    # GammaGamma paths
    #process.Mibipathphotons = cms.Path(process.MiBiCommonPAT*process.TwoPhotonsSeq*process.MiBiCommonNTTwoPhotons)

    process.MiBiPathPhotons = cms.Path( process.noMuonVertexReco*process.MiBiCommonPAT*process.TwoMuonsSeq*process.MiBiCommonNTTwoPhotons)
    #process.MiBiPathPhotons = cms.Path( process.MiBiCommonPAT )
    
    #process.MiBiPathPhotons = cms.Path( process.noMuonVertexReco*process.noElectronVertexReco*process.noLeptonVertexReco*process.MiBiCommonPAT*process.MiBiCommonNTTwoPhotons)
   
  
    # Di-jet paths
    #process.MiBiPathTwoJetsAK5PF = cms.Path(process.MiBiCommonPAT*process.TwoJetsAK5PFSeq*process.MiBiCommonNTTwoJetsAK5PF)
    #process.MiBiPathTwoJetsPFlow = cms.Path(process.MiBiCommonPAT*process.TwoJetsPFlowSeq*process.MiBiCommonNTTwoJetsPFlow)
    
    #ele path
    #process.MiBiPathOneElectron      = cms.Path(process.MiBiCommonPAT*process.OneEleSeq*process.MiBiCommonNTOneElectron)
    #process.MiBiPathOneElectronPFlow = cms.Path(process.MiBiCommonPAT*process.OneElePFlowSeq*process.MiBiCommonNTOneElectronPFlow)


    # save pat content
    process.out.outputCommands = cms.untracked.vstring('keep *')

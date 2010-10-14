import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *


def makeMiBiCommonPAT(process, GlobalTag, MC=False, HLT='HLT', Filter=False, SavePAT=True):
    # Setup the process
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
    )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 10
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    # Drop the DQM stuff on input
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")
    #Counter1:All red events
    process.AllEvents = process.AllPassFilter.clone()

    # Scraping filter
    process.scrapingFilter = cms.EDFilter("FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
    )
    
    #Counter2:Non scraped events
    process.NonScrapedEvents = process.AllPassFilter.clone()
    
    # filter on primary vertex
    process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
        minimumNDOF = cms.uint32(4) ,
        maxAbsZ = cms.double(24),
        maxd0 = cms.double(2)
    )
    
    #Counter3:Good vertex events
    process.GoodVtxEvents = process.AllPassFilter.clone()
    
    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")

    #process.selectedPatMuons.cut = cms.string("pt() > 5")
    #process.selectedPatElectrons.cut = cms.string("pt() > 10")
    #process.selectedPatTaus.cut = cms.string("pt() > 10")
    #process.selectedPatPhotons.cut = cms.string("pt() > 10")
    process.selectedPatJets.cut = cms.string("pt() > 10")


    #Bufix related to btagging
    process.patJets.addTagInfos = cms.bool(False)

    # Add tcMET and pfMET
    addTcMET(process, 'TC')
    addPfMET(process, 'PF')
    
# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATTools#Jet_Tools

#    addJetCollection(
#        process,
#        cms.InputTag('ak5CaloJets'),
#        'AK5',
#        'Calo',
#        doJTA        = True,
#        doBTagging   = True,
#        jetCorrLabel = ('AK5', 'Calo'),
#        doType1MET   = True,
#        doL1Cleaning = True,
#        doL1Counters = False,
#        genJetCollection=cms.InputTag("ak5GenJets"),
#        doJetID      = True
#        )
    
#    addJetCollection(
#        process,
#        cms.InputTag('ak5PFJets'),
#        'AK5',
#        'PF',
#        doJTA        = True,
#        doBTagging   = True,
#        jetCorrLabel = ('AK5', 'PF'),
#        doType1MET   = False,
#        doL1Cleaning = True,
#        doL1Counters = False,
#        genJetCollection=cms.InputTag("ak5GenJets"),
#        doJetID      = True
#        )
    
    if not MC:
     removeMCMatching(process, ['All'])

    #output
    process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
        outputCommands = cms.untracked.vstring(
            'drop *',
             'keep recoTracks_generalTracks__HLT',
             #'keep recoTracks_globalMuons__HLT',
             #'keep recoTracks_hltBLifetimeRegionalCtfWithMaterialTracksStartupU__HLT',
             #'keep recoTracks_hltCtfL1IsoWithMaterialTracks__HLT',
             #'keep recoTracks_hltCtfL1NonIsoWithMaterialTracks__HLT',
             #'keep recoTracks_hltL25TauCtfWithMaterialTracks__HLT',
             #'keep recoTracks_hltL2Muons__HLT',
             #'keep recoTracks_hltL3Muons__HLT',
             #'keep recoTracks_hltL3TauCtfWithMaterialTracks__HLT',
             #'keep recoTracks_hltL3TkTracksFromL2__HLT',
             #'keep recoTracks_hltL3TrackCandidateFromL2__HLT',
             #'keep recoTracks_hltPixelTracks__HLT',
             #'keep recoTracks_pixelTracks__HLT',
             #'keep recoTracks_standAloneMuons__HLT',
             #'keep recoTracks_hltL3Muons_L2Seeded_HLT',
             #'keep recoTracks_hltL2Muons_UpdatedAtVtx_HLT',
             #'keep recoTracks_standAloneMuons_UpdatedAtVtx_HLT',
             #'keep recoTracks_tevMuons_default_HLT',
             #'keep recoTracks_tevMuons_dyt_HLT',
             #'keep recoTracks_tevMuons_firstHit_HLT',
             #'keep recoTracks_impactParameterTagInfos_ghostTracks_HLT',
             #'keep recoTracks_tevMuons_picky_HLT',
            'keep *_selected*_*_*',                        # selected PAT objects
            'keep *_patMETs*_*_*',                         # All PAT objects
            'keep *_offlinePrimaryVertices*_*_*',          # Primary vertices: you want these to compute impact parameters
            'keep *_offlineBeamSpot_*_*',                  # Beam spot: you want this for the same reason
            'keep edmTriggerResults_TriggerResults_*_*',   # HLT info, per path (cheap)
            'keep *_genParticles_*_*',                     # HLT info, per path (cheap)
            'keep recoGsfElectronCores_*_*_*'              #
        ),
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('MiBiCommonPAT') ) if Filter else cms.untracked.PSet()
    )
    if SavePAT :
     process.e = cms.EndPath(process.out)



    #PG PF2PAT sequence
    #PG NB selected objects will be saevd automatically,
    #PG for cleaned objects it would be necessaty to add some "keep" in the output module
    
    # Configure PAT to use PF2PAT instead of AOD sources
    # this function will modify the PAT sequences. It is currently 
    # not possible to run PF2PAT+PAT and standart PAT at the same time
    from PhysicsTools.PatAlgos.tools.pfTools import *

    # An empty postfix means that only PF2PAT is run,
    # otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
    # collections have standard names + postfix (e.g. patElectronPFlow)  
    postfix = "PFlow"
    usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 

    # turn to false when running on data
    getattr(process, "patElectrons"+postfix).embedGenMatch = True
    getattr(process, "patMuons"+postfix).embedGenMatch = True
    if not MC:
        getattr(process, "patElectrons"+postfix).embedGenMatch = False
        getattr(process, "patMuons"+postfix).embedGenMatch = False
		#PG FIXME questo e' ridondante con quanto fatto sopra?

    # the HCAL Noise Filter
#    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
    
    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Path(
        process.AllEvents * # -> Counter
        process.scrapingFilter *
        process.NonScrapedEvents * # -> Counter
        process.primaryVertexFilter *
        process.GoodVtxEvents * # -> Counter
        process.patDefaultSequence *
        getattr(process,"patPF2PATSequence"+postfix)
    )
    




import FWCore.ParameterSet.Config as cms

def littleHPAT(process, GlobalTag, MC=False, HLT='HLT', Filter=True, SavePAT=True, HLT_filter_ele=False, HLT_filter_mu=False):
    # Setup the process
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
        # fileMode = cms.untracked.string('MERGE'),
    )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    # Drop the DQM stuff on input
    process.source = cms.Source("PoolSource",
        inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*"),
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
        maxAbsZ = cms.double(15),
        maxd0 = cms.double(2)
    )
    
    #Counter3:Good vertex events
    process.GoodVtxEvents = process.AllPassFilter.clone()
    
    #HLT Filter to avoid double counting when running on Mu and EGamma datasets
    # filter events which have passed any of Mu HLTs
    process.load('HLTrigger/HLTfilters/hltHighLevel_cfi')
    process.hltHighLevelFilterMu = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","",HLT),
     HLTPaths = cms.vstring('HLT_*Mu*'),
     #HLTPaths = cms.vstring('HLT_MinBias*'),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True),
     throw = cms.bool(True)
    ) 
    
    # filter events which have passed any of EGamma HLTs
    process.load('HLTrigger/HLTfilters/hltHighLevel_cfi')
    process.hltHighLevelFilterEle = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","",HLT),
     HLTPaths = cms.vstring('HLT_*Photon*','HLT_*Ele*'),
     #HLTPaths = cms.vstring('HLT_MinBias*'),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True),
     throw = cms.bool(True)
    ) 
    
    #Counter4:HLT filtered Events
    process.TrigFilteredEvents = process.AllPassFilter.clone()

    
    # Prune generated particles to muons, electrons and their parents
    process.genLeptons = cms.EDProducer("GenParticlePruner",
        src = cms.InputTag("genParticles"),
        select = cms.vstring(
            "drop  *  ",                     # this is the default
            "++keep abs(pdgId) = 13",        # keep muons and their parents
            "++keep abs(pdgId) = 11",        # keep muons and their parents
            "drop pdgId == 21 && status = 2" # remove intermediate qcd spam carrying no flavour info
        )
    )

    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")    
    
    # Make PAT Muons
    process.load("PhysicsTools.PatAlgos.mcMatchLayer0.muonMatch_cfi")
    process.muonMatch.matched = "genLeptons"
    process.patMuons.embedTrack = cms.bool(True)

    if not MC:
          process.patMuons.addGenMatch = cms.bool(False)
          process.patMuons.embedGenMatch = cms.bool(False)
    
    # Make PAT Electrons
    process.load("PhysicsTools.PatAlgos.mcMatchLayer0.electronMatch_cfi")
    process.electronMatch.matched = "genLeptons"
    process.patElectrons.embedTrack = cms.bool(True)


    if not MC:
          process.patElectrons.addGenMatch = cms.bool(False)
          process.patElectrons.embedGenMatch = cms.bool(False)
    
    # Make PAT lepton sequence
    process.patLeptonSequence = cms.Sequence(
        process.genLeptons *
        process.muonMatch *
        process.patMuons *
        process.electronMatch *
        process.patElectrons
    )
    if not MC:
        process.patLeptonSequence.remove(process.genLeptons)
        process.patLeptonSequence.remove(process.muonMatch)
        process.patLeptonSequence.remove(process.electronMatch)

    # Make dimuon candidates
    process.load("HiggsAnalysis.littleH.Onia2MuMu_cfi")
    process.onia2MuMuPat = cms.EDProducer('Onia2MuMuPAT',
        muons = cms.InputTag("patMuons"),
        # At least one muon must pass this selection
        higherPuritySelection = cms.string("(isGlobalMuon || isTrackerMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
        # BOTH muons must pass this selection
        lowerPuritySelection  = cms.string("(isGlobalMuon || isTrackerMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
        dimuonSelection  = cms.string("4 < mass"), ## The dimuon must pass this selection before vertexing
        addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
        addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
                 
    )
    
    # Filter dimuon candidates - requirement on common vertex fit probability
    process.selectedOnia2MuMuPat = cms.EDFilter("CandViewSelector",
        src = cms.InputTag('onia2MuMuPat'),
        cut = cms.string(" userFloat('vProb') > 0.05 "),
    )
    
    # Make dielectron candidates
    process.load("HiggsAnalysis.littleH.Onia2EleEle_cfi")
    process.onia2EleElePat = cms.EDProducer('Onia2EleElePAT',
        electrons = cms.InputTag("patElectrons"),
        # At least one electron must pass this selection
        higherPuritySelection = cms.string("abs(gsfTrack.dxy)<4 && abs(gsfTrack.dz)<35"),
        # BOTH electrons must pass this selection
        lowerPuritySelection  = cms.string("abs(gsfTrack.dxy)<4 && abs(gsfTrack.dz)<35"),
        dieleSelection  = cms.string("4 < mass"), ## The diele must pass this selection before vertexing
        addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
        addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
                 
    )
    
    # Filter diele candidates - requirement on common vertex fit probability
    process.selectedOnia2EleElePat = cms.EDFilter("CandViewSelector",
        src = cms.InputTag('onia2EleElePat'),
        cut = cms.string(" userFloat('vProb') > 0.05 "),
    )

    # merge dimuons and dielectrons into leptons
    process.selectedOnia2LepLep = cms.EDProducer("CandViewMerger",
        src = cms.VInputTag('selectedOnia2MuMuPat','selectedOnia2EleElePat')
    )

    # check if there is at least one (inclusive) onia candidate
    process.selectedOnia2LepLepFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('selectedOnia2LepLep'),
        minNumber = cms.uint32(1),
    )
    
    #Counter5:Onia Events
    process.SelectedOniaEvents = process.AllPassFilter.clone()
    
    # the onia2LepLep path
    process.Onia2LepLepPAT = cms.Path(
        process.AllEvents *
        process.scrapingFilter *
        process.NonScrapedEvents *
        process.primaryVertexFilter *
        process.GoodVtxEvents *
        ~process.hltHighLevelFilterMu *         #exclude events triggered by muons
        ~process.hltHighLevelFilterEle *         #exclude events triggered by ECAL
        process.TrigFilteredEvents *
        process.patLeptonSequence *
        process.onia2MuMuPat *
        process.selectedOnia2MuMuPat *
        process.onia2EleElePat *
        process.selectedOnia2EleElePat *
        process.selectedOnia2LepLep *
        process.selectedOnia2LepLepFilter *
        process.SelectedOniaEvents
    )
    if not HLT_filter_ele:
      process.Onia2LepLepPAT.remove(~process.hltHighLevelFilterEle)
    if not HLT_filter_mu:
      process.Onia2LepLepPAT.remove(~process.hltHighLevelFilterMu)
    
    #output
    process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('onia2LepLepPAT.root'),
        outputCommands = cms.untracked.vstring('drop *',
            'keep *_genLeptons_*_Onia2LepLepPAT',                  # generated leptons and parents
            'keep patMuons_*_*_Onia2LepLepPAT',  # All PAT muons including general tracks and matches to triggers
            'keep patElectrons_*_*_Onia2LepLepPAT',                # All PAT eles including general tracks and matches to triggers
            'keep patCompositeCandidates_*__Onia2LepLepPAT',       # PAT di-lep
            'keep *_offlinePrimaryVertices_*_*',                   # Primary vertices: you want these to compute impact parameters
            'keep *_offlineBeamSpot_*_*',                          # Beam spot: you want this for the same reason                                   
            'keep edmTriggerResults_TriggerResults_*_*',           # HLT info, per path (cheap)
            'keep *_generalTracks_*_*',
            'keep recoGsfElectronCores_*_*_*'
        ),
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Onia2LepLepPAT') ) if Filter else cms.untracked.PSet()
    )
    if SavePAT :
     process.e = cms.EndPath(process.out)


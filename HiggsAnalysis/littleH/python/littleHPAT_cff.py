import FWCore.ParameterSet.Config as cms

def littleHPAT(process, GlobalTag, MC=False, HLT='HLT', Filter=True):
    # Setup the process
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
        # fileMode = cms.untracked.string('MERGE'),
    )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.Reconstruction_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    
    # Drop the DQM stuff on input
    process.source = cms.Source("PoolSource",
        inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*"),
        fileNames = cms.untracked.vstring()
    )

    # Scraping filter
    process.scrapingFilter = cms.EDFilter("FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
    )

    # Merge CaloMuons and General Tracks into the collection of reco::Muons
    process.load("RecoMuon.MuonIdentification.mergedMuons_cfi")
    process.mergedMuons.mergeTracks = True
    process.mergedMuons.tracksCut = '(abs(eta) <= 1.3 && pt > 3.3) || (1.3 < abs(eta) <= 2.2 && p > 2.9) || (2.2 < abs(eta) <= 2.4  && pt > 0.8)'
    process.mergedMuons.caloMuonsCut = process.mergedMuons.tracksCut
    
    #LEO::Merge electron collections?

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

    # Make PAT Muons
    process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
    from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import addMCinfo, changeRecoMuonInput, useL1MatchingWindowForSinglets, changeTriggerProcessName, switchOffAmbiguityResolution
    # with some customization
    if MC:
        addMCinfo(process)
        # since we match inner tracks, keep the matching tight and make it one-to-one
        process.muonMatch.maxDeltaR = 0.05
        process.muonMatch.resolveByMatchQuality = True
        process.muonMatch.matched = "genLeptons"
        #process.muonMatch.matched = "genMuons"
    changeRecoMuonInput(process, "mergedMuons")
    useL1MatchingWindowForSinglets(process)
    changeTriggerProcessName(process, HLT)
    switchOffAmbiguityResolution(process) # Switch off ambiguity resolution: allow multiple reco muons to match to the same trigger muon
    process.muonMatchHLTL3.maxDeltaR = 0.1
    process.muonMatchHLTL3.maxDPtRel = 10.0
    process.muonMatchHLTCtfTrack.maxDeltaR = 0.1
    process.muonMatchHLTCtfTrack.maxDPtRel = 10.0
    process.muonMatchHLTTrackMu.maxDeltaR = 0.1
    process.muonMatchHLTTrackMu.maxDPtRel = 10.0

    # Make PAT Electrons
    process.load("PhysicsTools.PatAlgos.mcMatchLayer0.electronMatch_cfi")
    process.electronMatch.matched = "genLeptons"

    
    process.load("PhysicsTools.PatAlgos.patSequences_cff")    
    
    # Make PAT lepton sequence
    process.patLeptonSequence = cms.Sequence(
        process.scrapingFilter *
        process.mergedMuons *
        process.genLeptons *
        process.patMuonsWithTriggerSequence*
        process.electronMatch*
        process.patElectrons
    )
    if not MC:
        process.patLeptonSequence.remove(process.genLeptons)
        process.patLeptonSequence.remove(process.electronMatch)

    # Make dimuon candidates
    process.load("HiggsAnalysis.littleH.Onia2MuMu_cfi")
    process.onia2MuMuPatTrkTrk = cms.EDProducer('Onia2MuMuPAT',
        muons = cms.InputTag("patMuonsWithTrigger"),
        beamSpotTag = cms.InputTag("offlineBeamSpot"),
        primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
        # At least one muon must pass this selection
        higherPuritySelection = cms.string("(isGlobalMuon || isTrackerMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
        # BOTH muons must pass this selection
        lowerPuritySelection  = cms.string("(isGlobalMuon || isTrackerMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
        dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
        addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
        addMuonlessPrimaryVertex = cms.bool(True), ## Embed the primary vertex re-made from all the tracks except the two muons
        addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
        resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
    )
    
    # Make dielectron candidates
    process.load("HiggsAnalysis.littleH.Onia2EleEle_cfi")
    process.onia2EleElePat = cms.EDProducer('Onia2EleElePAT',
        electrons = cms.InputTag("patElectrons"),
        beamSpotTag = cms.InputTag("offlineBeamSpot"),
        primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
        # At least one electron must pass this selection
        higherPuritySelection = cms.string("abs(gsfTrack.dxy)<4 && abs(gsfTrack.dz)<35"),
        # BOTH electrons must pass this selection
        lowerPuritySelection  = cms.string("abs(gsfTrack.dxy)<4 && abs(gsfTrack.dz)<35"),
        dieleSelection  = cms.string("2 < mass"), ## The diele must pass this selection before vertexing
        addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
        addEleclessPrimaryVertex = cms.bool(True), ## Embed the primary vertex re-made from all the tracks except the two electrons
        addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
        resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
    )

    # merge dimuons and dielectrons into leptons
    process.onia2LepLep = cms.EDProducer("CandViewMerger",
        src = cms.VInputTag('onia2MuMuPatTrkTrk','onia2EleElePat')
    )

    # check if there is at least one (inclusive) onia candidate
    process.onia2LepLepFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('onia2LepLep'),
        minNumber = cms.uint32(1),
    )
    
    # the onia2LepLep path
    process.Onia2LepLepPAT = cms.Path(
        process.patLeptonSequence *
        process.onia2MuMuPatTrkTrk *
        process.onia2EleElePat *
        process.onia2LepLep *
        process.onia2LepLepFilter
    )

    #output
    process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('onia2LepLepPAT.root'),
        outputCommands = cms.untracked.vstring('drop *',
            'keep *_genLeptons_*_Onia2LepLepPAT',                  # generated leptons and parents
            'keep patMuons_patMuonsWithTrigger_*_Onia2LepLepPAT',  # All PAT muons including general tracks and matches to triggers
            'keep patElectrons_*_*_Onia2LepLepPAT',                # All PAT eles including general tracks and matches to triggers
            'keep patCompositeCandidates_*__Onia2LepLepPAT',       # PAT di-lep
            'keep *_offlinePrimaryVertices_*_*',                   # Primary vertices: you want these to compute impact parameters
            'keep *_offlineBeamSpot_*_*',                          # Beam spot: you want this for the same reason                                   
            'keep edmTriggerResults_TriggerResults_*_*',           # HLT info, per path (cheap)
            'keep l1extraL1MuonParticles_l1extraParticles_*_*',    # L1 info (cheap)
            'keep *_generalTracks_*_RECO'                          # Track info (all RECO)
        ),
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Onia2LepLepPAT') ) if Filter else cms.untracked.PSet()
    )
    process.e = cms.EndPath(process.out)

  

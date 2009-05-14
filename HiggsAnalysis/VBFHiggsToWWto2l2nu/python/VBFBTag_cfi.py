import FWCore.ParameterSet.Config as cms



#############################
#############################
######### B tagging #########
#############################
#############################


# default configuration with frontier conditions
load("Configuration.StandardSequences.MagneticField_cff")
load("Configuration.StandardSequences.Reconstruction_cff")
load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
GlobalTag.globaltag = 'IDEAL_V9::All'


# b-tagging general configuration
load("RecoBTag.Configuration.RecoBTag_cff")
load("RecoJets.JetAssociationProducers.ic5JetTracksAssociatorAtVertex_cfi")

# create a new jets and tracks association
newJetTracksAssociatorAtVertex = ic5JetTracksAssociatorAtVertex.clone()
newJetTracksAssociatorAtVertex.jets = "sisCone5CaloJets"
newJetTracksAssociatorAtVertex.tracks = "generalTracks"

# impact parameter b-tag
newImpactParameterTagInfos = impactParameterTagInfos.clone()
newImpactParameterTagInfos.jetTracks = "newJetTracksAssociatorAtVertex"
newTrackCountingHighEffBJetTags = trackCountingHighEffBJetTags.clone()
newTrackCountingHighEffBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newImpactParameterTagInfos") )
newTrackCountingHighPurBJetTags = trackCountingHighPurBJetTags.clone()
newTrackCountingHighPurBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newImpactParameterTagInfos") )
newJetProbabilityBJetTags = jetProbabilityBJetTags.clone()
newJetProbabilityBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newImpactParameterTagInfos") )
newJetBProbabilityBJetTags = jetBProbabilityBJetTags.clone()
newJetBProbabilityBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newImpactParameterTagInfos") )


# secondary vertex b-tag
newSecondaryVertexTagInfos = secondaryVertexTagInfos.clone()
newSecondaryVertexTagInfos.trackIPTagInfos = "newImpactParameterTagInfos"
newSimpleSecondaryVertexBJetTags = simpleSecondaryVertexBJetTags.clone()
newSimpleSecondaryVertexBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newSecondaryVertexTagInfos") )
newCombinedSecondaryVertexBJetTags = combinedSecondaryVertexBJetTags.clone()
newCombinedSecondaryVertexBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newImpactParameterTagInfos"), cms.InputTag("newSecondaryVertexTagInfos") )
newCombinedSecondaryVertexMVABJetTags = combinedSecondaryVertexMVABJetTags.clone()
newCombinedSecondaryVertexMVABJetTags.tagInfos = cms.VInputTag( cms.InputTag("newImpactParameterTagInfos"), cms.InputTag("newSecondaryVertexTagInfos") )

# soft electron b-tag
newSoftElectronTagInfos = softElectronTagInfos.clone()
newSoftElectronTagInfos.jets = "sisCone5CaloJets"
newSoftElectronBJetTags = softElectronBJetTags.clone()
newSoftElectronBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newSoftElectronTagInfos") )

# soft muon b-tag
newSoftMuonTagInfos = softMuonTagInfos.clone()
newSoftMuonTagInfos.jets = "sisCone5CaloJets"
newSoftMuonBJetTags = softMuonBJetTags.clone()
newSoftMuonBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newSoftMuonTagInfos") )
newSoftMuonByIP3dBJetTags = softMuonByIP3dBJetTags.clone()
newSoftMuonByIP3dBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newSoftMuonTagInfos") )
newSoftMuonByPtBJetTags = softMuonByPtBJetTags.clone()
newSoftMuonByPtBJetTags.tagInfos = cms.VInputTag( cms.InputTag("newSoftMuonTagInfos") )

# prepare a path running the new modules
newJetTracksAssociator = cms.Sequence(
    newJetTracksAssociatorAtVertex
)

newJetBtaggingIP = cms.Sequence(
    newImpactParameterTagInfos * (
        newTrackCountingHighEffBJetTags +
        newTrackCountingHighPurBJetTags +
        newJetProbabilityBJetTags +
        newJetBProbabilityBJetTags
    )
)

newJetBtaggingSV = cms.Sequence(
    newImpactParameterTagInfos *
    newSecondaryVertexTagInfos * (
        newSimpleSecondaryVertexBJetTags +
        newCombinedSecondaryVertexBJetTags +
        newCombinedSecondaryVertexMVABJetTags
    )
)

newJetBtaggingEle = cms.Sequence(
    btagSoftElectrons *
    newSoftElectronTagInfos *
    newSoftElectronBJetTags
)

newJetBtaggingMu = cms.Sequence(
    newSoftMuonTagInfos * (
        newSoftMuonBJetTags +
        newSoftMuonByIP3dBJetTags +
        newSoftMuonByPtBJetTags
    )
)

newJetBtagging = cms.Sequence(
    newJetBtaggingIP +
    newJetBtaggingSV +
    newJetBtaggingEle +
    newJetBtaggingMu
)

newBtaggingPath = cms.Path(
    newJetTracksAssociator *
    newJetBtagging
)

#################################
#################################
######### end B tagging #########
#################################
#################################


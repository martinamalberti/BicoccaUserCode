import FWCore.ParameterSet.Config as cms


# b-tagging general configuration

from RecoJets.JetAssociationProducers.ic5JetTracksAssociatorAtVertex_cfi import *
from RecoBTag.Configuration.RecoBTag_cff import *

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
softPFElectrons *
# btagSoftElectrons *
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


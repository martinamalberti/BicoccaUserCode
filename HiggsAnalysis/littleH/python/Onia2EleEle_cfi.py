import FWCore.ParameterSet.Config as cms

onia2EleElePAT = cms.EDProducer('Onia2EleElePAT',
  electrons = cms.InputTag("patElectrons"),
  beamSpotTag = cms.InputTag("offlineBeamSpot"),
  primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
  higherPuritySelection = cms.string("2 < pt"), ## At least one electron must pass this selection
  lowerPuritySelection  = cms.string("2 < pt"), ## BOTH electrons must pass this selection
  dieleSelection  = cms.string("2 < mass && abs(daughter('ele1').gsfTrack.dz - daughter('ele2').gsfTrack.dz) < 25"), ## The diele must pass this selection before vertexing
  addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
  addEleclessPrimaryVertex = cms.bool(True), ## Embed the primary vertex re-made from all the tracks except the two electrons
  addMCTruth = cms.bool(True),      ## Add the common MC mother of the two muons, if any
  resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)

import FWCore.ParameterSet.Config as cms

# ## load geometry
from Configuration.StandardSequences.Geometry_cff import *
from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
from Geometry.CommonDetUnit.globalTrackingGeometry_cfi import *
GlobalTag.globaltag = cms.string('IDEAL_V9::All')
from Configuration.StandardSequences.MagneticField_cff import *

from TrackingTools.TrackAssociator.default_cfi import *
from TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff import *


from JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff import *
from JetMETCorrections.Configuration.L7PartonCorrections_cff import *

es_prefer_L2L3CorJetIcone5 = cms.ESPrefer("JetCorrectionServiceChain","L2L3JetCorrectorIC5Calo")

L2L3CorJetIcone5 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('L2L3JetCorrectorIC5Calo')
)

# MET corrections from JES
from JetMETCorrections.Type1MET.MetType1Corrections_cff import *
corMetType1Icone5.corrector = cms.string('L2L3JetCorrectorIC5Calo')
# MET corrections from muons
from JetMETCorrections.Type1MET.MetMuonCorrections_cff import corMetGlobalMuons, goodMuonsforMETCorrection
# muon MET correction maker
globalMuonsForMET = goodMuonsforMETCorrection.clone(
   cut = cms.string('isGlobalMuon = 1')
)
goodGlobalMuonsForMET = goodMuonsforMETCorrection.clone(
    src = cms.InputTag("globalMuonsForMET")
)
corMetType1Icone5Muons = corMetGlobalMuons.clone(
    inputUncorMetLabel = cms.InputTag('corMetType1Icone5'),
    muonsInputTag      = cms.InputTag('goodGlobalMuonsForMET')
)

corMetType1Icone5Muons.TrackAssociatorParameters.useEcal    = False ## RecoHits
corMetType1Icone5Muons.TrackAssociatorParameters.useHcal    = False ## RecoHits
corMetType1Icone5Muons.TrackAssociatorParameters.useHO      = False ## RecoHits
corMetType1Icone5Muons.TrackAssociatorParameters.useCalo    = True  ##
corMetType1Icone5Muons.TrackAssociatorParameters.useMuon    = False ## RecoHits
corMetType1Icone5Muons.TrackAssociatorParameters.truthMatch = False

# default sequence for JetMET corrections
jetMETCorrectionsSequence = cms.Sequence(
 L2L3CorJetIcone5 +
 globalMuonsForMET *
 goodGlobalMuonsForMET *
 corMetType1Icone5 *
 corMetType1Icone5Muons
)

# default sequence for JetMET corrections w/o muon corrections
jetMETCorrections_withoutMuonCorrSequence = cms.Sequence(
 L2L3CorJetIcone5 +
 corMetType1Icone5
)
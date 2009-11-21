import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO MET MUON CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

from JetMETCorrections.Type1MET.MetMuonCorrections_cff import *

from TrackingTools.TrackAssociator.default_cfi import *
from TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff import *



VBFMuonCorrectedCaloMetSequence = cms.Sequence(
    MetMuonCorrections
    )

import FWCore.ParameterSet.Config as cms

from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentification_cfi import *


CiC_eIDSequence = cms.Sequence(
    eidVeryLoose+
    eidLoose+
    eidMedium+
    eidTight+
    eidSuperTight
    )

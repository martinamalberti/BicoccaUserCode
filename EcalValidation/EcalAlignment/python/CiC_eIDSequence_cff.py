import FWCore.ParameterSet.Config as cms

from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentification_cfi import *


CiC_eIDSequence = cms.Sequence(
    #eidRobustLoose+
    #eidRobustTight+
    #eidRobustHighEnergy 
    eidVeryLoose+
    eidLoose+
    eidMedium+
    eidTight+
    eidSuperTight
)

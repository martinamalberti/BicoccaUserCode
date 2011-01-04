import FWCore.ParameterSet.Config as cms

# Z2 tune with pT-ordered showers
from Configuration.Generator.PythiaUEZ2Settings_cfi import *
from GeneratorInterface.ExternalDecays.TauolaSettings_cff import *

generator = cms.EDFilter(
    "Pythia6GeneratorFilter",
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    comEnergy = cms.double(8000.0),
    crossSection = cms.untracked.double(94.3),
    filterEfficiency = cms.untracked.double(1.0),
    ExternalDecays = cms.PSet(
        Tauola = cms.untracked.PSet(
            TauolaPolar,
            TauolaDefaultInputCards
        ),
        parameterSets = cms.vstring('Tauola')
    ),
    UseExternalGenerators = cms.untracked.bool(True),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring(
            'MSEL=0              ! User defined processes',
            'MSUB(81)    = 1     ! qqbar to QQbar',
            'MSUB(82)    = 1     ! gg to QQbar',
            'MSTP(7)     = 6     ! flavor = top',
            'PMAS(5,1)   = 4.16  ! b quark mass',
            'PMAS(6,1)   = 172.5 ! t quark mass', 
            'MDME(190,1) = 0     ! W decay into dbar u',
            'MDME(191,1) = 0     ! W decay into dbar c',
            'MDME(192,1) = 0     ! W decay into dbar t',
            'MDME(194,1) = 0     ! W decay into sbar u',
            'MDME(195,1) = 0     ! W decay into sbar c',
            'MDME(196,1) = 0     ! W decay into sbar t',
            'MDME(198,1) = 0     ! W decay into bbar u',
            'MDME(199,1) = 0     ! W decay into bbar c',
            'MDME(200,1) = 0     ! W decay into bbar t',
            'MDME(206,1) = 1     ! W decay into e+ nu_e',
            'MDME(207,1) = 1     ! W decay into mu+ nu_mu',
            'MDME(208,1) = 1     ! W decay into tau+ nu_tau'
            ),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring(
            'pythiaUESettings', 
            'processParameters'
            )
        )
    )

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/GenProduction/python/PYTHIA6_Tauola_ttbar_lnublnub_TuneZ2_8TeV_cff.py,v $'),
    annotation = cms.untracked.string('PYTHIA6 + Tauola ttbar -> lnub lnub at 8TeV')
)


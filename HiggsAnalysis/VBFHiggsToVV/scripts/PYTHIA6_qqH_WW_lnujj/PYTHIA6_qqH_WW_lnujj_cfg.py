# Auto generated configuration file
# using: 
# Revision: 1.123.4.4 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: PYTHIA6_H170WWlnujj_pythiaHad_cfi.py -s GEN:ProductionFilterSequence,FASTSIM --pileup=NoPileUp --conditions=FrontierConditions_GlobalTag,MC_31X_V5::All --eventcontent=RECOSIM --beamspot=Early10TeVCollision --datatier=GEN-SIM-RECO -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('FastSimulation/Configuration/RandomServiceInitialization_cff')
process.load('FastSimulation.PileUpProducer.PileUpSimulator10TeV_cfi')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Generator_cff')
process.load('FastSimulation/Configuration/FamosSequences_cff')
process.load('FastSimulation/Configuration/HLT_1E31_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedParameters_cfi')
process.load('FastSimulation/Configuration/CommonInputs_cff')
process.load('FastSimulation/Configuration/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123.4.4 $'),
    annotation = cms.untracked.string('PYTHIA6_H170WWlnujj_pythiaHad_cfi.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("EmptySource")

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('PYTHIA6_H170WWlnujj_pythiaHad.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.famosPileUp.PileUpSimulator = process.PileUpSimulatorBlock.PileUpSimulator
process.famosPileUp.PileUpSimulator.averageNumber = 0
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.famosSimHits.ActivateDecays.comEnergy = 10000
process.simulation = cms.Sequence(process.simulationWithFamos)
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)

# set correct vertex smearing
process.Early10TeVCollisionVtxSmearingParameters.type = cms.string("BetaFunc")
process.famosSimHits.VertexGenerator = process.Early10TeVCollisionVtxSmearingParameters
process.famosPileUp.VertexGenerator = process.Early10TeVCollisionVtxSmearingParameters
# Apply Tracker and Muon misalignment
process.famosSimHits.ApplyAlignment = True
process.misalignedTrackerGeometry.applyAlignment = True

process.misalignedDTGeometry.applyAlignment = True
process.misalignedCSCGeometry.applyAlignment = True

process.GlobalTag.globaltag = 'MC_31X_V5::All'
process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    ExternalDecays = cms.PSet(
        Tauola = cms.untracked.PSet(
            UseTauolaPolarization = cms.bool(True),
            InputCards = cms.PSet(
                mdtau = cms.int32(0),
                pjak2 = cms.int32(0),
                pjak1 = cms.int32(0)
            )
        ),
        parameterSets = cms.vstring('Tauola')
    ),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(10000.0),
    maxEventsToPrint = cms.untracked.int32(5),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model', 
            'MSTU(21)=1     ! Check on possible errors during program execution', 
            'PARP(82)=1.8387   ! pt cutoff for multiparton interactions', 
            'PARP(89)=1960. ! sqrts for which PARP82 is set', 
            'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
            'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
            'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
            'PARP(67)=2.5    ! amount of initial-state radiation', 
            'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(86)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(62)=1.25   ! ', 
            'PARP(64)=0.2    ! ', 
            'MSTP(91)=1      !', 
            'PARP(91)=2.1   ! kt distribution', 
            'PARP(93)=15.0  ! '),
        processParameters = cms.vstring('MSEL=0            ! User defined processes', 
            'MSUB(102)=0       ! gg->H', 
            'MSUB(123)=1       ! qq-ZZqq->Hqq', 
            'MSUB(124)=1       ! qq-WWqq->Hqq', 
            'PMAS(23,1)=91.188 ! Z mass', 
            'PMAS(24,1)=80.425 ! W mass', 
            'PMAS(25,1)=170.0  ! mass of Higgs', 
            'MDME(210,1)=0    ! Higgs decay into dd', 
            'MDME(211,1)=0    ! Higgs decay into uu', 
            'MDME(212,1)=0    ! Higgs decay into ss', 
            'MDME(213,1)=0    ! Higgs decay into cc', 
            'MDME(214,1)=0    ! Higgs decay into bb', 
            'MDME(215,1)=0    ! Higgs decay into tt', 
            'MDME(216,1)=0    ! Higgs decay into', 
            'MDME(217,1)=0    ! Higgs decay into Higgs decay', 
            'MDME(218,1)=0    ! Higgs decay into e e', 
            'MDME(219,1)=0    ! Higgs decay into mu mu', 
            'MDME(220,1)=0    ! Higgs decay into tau tau', 
            'MDME(221,1)=0    ! Higgs decay into Higgs decay', 
            'MDME(222,1)=0    ! Higgs decay into g g', 
            'MDME(223,1)=0    ! Higgs decay into gam gam', 
            'MDME(224,1)=0    ! Higgs decay into gam Z', 
            'MDME(225,1)=0    ! Higgs decay into Z Z', 
            'MDME(226,1)=1    ! Higgs decay into W W', 
            'MDME(190,1)=4    ! W decay into dbar u', 
            'MDME(191,1)=4    ! W decay into dbar c', 
            'MDME(192,1)=4    ! W decay into dbar t', 
            'MDME(194,1)=4    ! W decay into sbar u', 
            'MDME(195,1)=4    ! W decay into sbar c', 
            'MDME(196,1)=4    ! W decay into sbar t', 
            'MDME(198,1)=4    ! W decay into bbar u', 
            'MDME(199,1)=4    ! W decay into bbar c', 
            'MDME(200,1)=4    ! W decay into bbar t', 
            'MDME(206,1)=5    ! W decay into e+ nu_e', 
            'MDME(207,1)=5    ! W decay into mu+ nu_mu', 
            'MDME(208,1)=5    ! W decay into tau+ nu_tau'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)
process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(cms.SequencePlaceholder("randomEngineStateProducer"))
process.reconstruction = cms.Path(process.reconstructionWithFamos)
process.out_step = cms.EndPath(process.output)



#anti-kt Jet
from RecoJets.Configuration.GenJetParticles_cff import *

from RecoJets.JetProducers.antikt5GenJets_cff import * 
from RecoJets.JetProducers.antikt7GenJets_cff import *

from RecoJets.JetProducers.antikt5CaloJets_cff import *
from RecoJets.JetProducers.antikt7CaloJets_cff import *

#from RecoJets.JetProducers.antikt5PFJets_cff import *
#from RecoJets.JetProducers.antikt7PFJets_cff import *

process.recoMyJets = cms.Path(process.genJetParticles +
                              process.antikt5GenJets  + # process.antikt7GenJets +
                              process.antikt5CaloJets # + process.antikt7CaloJets +
                              #process.antikt5PFJets   # + process.antikt7PFJets
                              )                              

# Schedule definition
process.schedule = cms.Schedule(process.generation_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.recoMyJets])
process.schedule.extend([process.reconstruction,process.out_step])
# special treatment in case of production filter sequence  
for path in process.paths: 
    getattr(process,path)._seq = process.ProductionFilterSequence*getattr(process,path)._seq

import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('Configuration.StandardSequences.VtxSmearedRealistic7TeVCollision_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('PYTHIA6 + Tauola ttbar -> lnub lnub at 8TeV'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/GenProduction/python/PYTHIA6_Tauola_ttbar_lnublnub_TuneZ2_8TeV_cff.py,v $')
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
    )

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# Input source
process.source = cms.Source("EmptySource")

# Output definition
process.RECOSIMoutput = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('PYTHIA6_Tauola_ttbar_lnublnub_TuneZ2_8TeV_GEN.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
        )
    )

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'START38_V12::All'
process.generator = cms.EDFilter(
    "Pythia6GeneratorFilter",
    UseExternalGenerators = cms.untracked.bool(True),
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
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(8000.0),
    crossSection = cms.untracked.double(94.3),
    filterEfficiency = cms.untracked.double(1.0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring(
            'MSTU(21)=1     ! Check on possible errors during program execution', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'PARP(82)=1.832 ! pt cutoff for multiparton interactions', 
            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
            'PARP(90)=0.275 ! Multiple interactions: rescaling power', 
            'MSTP(95)=6     ! CR (color reconnection parameters)', 
            'PARP(77)=1.016 ! CR', 
            'PARP(78)=0.538 ! CR', 
            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
            'PARP(62)=1.025 ! ISR cutoff', 
            'MSTP(91)=1     ! Gaussian primordial kT', 
            'PARP(93)=10.0  ! primordial kT-max', 
            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model'),
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
            'MDME(208,1) = 1     ! W decay into tau+ nu_tau'),
        parameterSets = cms.vstring(
            'pythiaUESettings', 
            'processParameters'
            )
        )
    )

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.endjob_step = cms.Path(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.endjob_step,process.RECOSIMoutput_step)

# special treatment in case of production filter sequence
for path in process.paths: 
    getattr(process,path)._seq = process.generator*getattr(process,path)._seq

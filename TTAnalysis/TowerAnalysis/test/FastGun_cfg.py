import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

# The number of events to be processed.
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
# Include the RandomNumberGeneratorService definition
process.load("FastSimulation/Configuration/RandomServiceInitialization_cff")

# Generate H -> ZZ -> l+l- l'+l'- (l,l'=e or mu), with mH=200GeV/c2
process.source = cms.Source("FlatRandomEGunSource",
                            PGunParameters = cms.untracked.PSet(
    # you can request more than 1 particle
    # since PartID is a vector, you can place in as many
    # PDG id's as you wish, comma seaparated
    #
    PartID = cms.untracked.vint32(11),
    MaxEta = cms.untracked.double(0.2),
    MaxPhi = cms.untracked.double(1.),
    MinEta = cms.untracked.double(-0.4),
    MinE = cms.untracked.double(50.0),
    MinPhi = cms.untracked.double(0.), ## in radians
    
    MaxE = cms.untracked.double(50.0)
    ),
                            Verbosity = cms.untracked.int32(0), ## set to 1 (or greater)  for printouts
                            
                            AddAntiParticle = cms.untracked.bool(False) ## back-to-back particles
                            
                                                                                    )
# Famos sequences (Frontier conditions)
process.load("FastSimulation/Configuration/CommonInputs_cff")
process.GlobalTag.globaltag = "IDEAL_V11::All"
# L1 Emulator and HLT Setup
process.load("FastSimulation.HighLevelTrigger.HLTSetup_cff")

process.load("FastSimulation/Configuration/FamosSequences_cff")

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

# HLT paths -- defined from ConfigDB
# This one is created on the fly by FastSimulation/Configuration/test/ExampleWithHLT_py.csh
#process.load("FastSimulation.Configuration.HLT_cff")


# If you want to turn on/off pile-up
process.famosPileUp.PileUpSimulator.averageNumber = 0
# You may not want to simulate everything for your study
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
# process.famosSimHits.SimulateMuons = False

# Produce Tracks and Clusters
process.p1 = cms.Path(process.famosWithEverything)

# To write out events (not need: FastSimulation _is_ fast!)
process.o1 = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string("/gwtera5/users/data/10TeV-RECO/FAST_PhysVal-DiElectron-Ene50-phi0_1-eta-0.4_0.2-10k.root"),
    outputCommands = cms.untracked.vstring("keep *"
                                           #,"drop *_mix_*_*"
                                           )
    )

process.outpath = cms.EndPath(process.o1)

# Keep the logging output to a nice level #

#process.Timing =  cms.Service("Timing")
#process.load("FWCore/MessageService/MessageLogger_cfi")
#process.MessageLogger.destinations = cms.untracked.vstring("pyDetailedInfo.txt","cout")
#process.MessageLogger.categories.append("FamosManager")
#process.MessageLogger.cout = cms.untracked.PSet(threshold=cms.untracked.string("INFO"),
#                                                default=cms.untracked.PSet(limit=cms.untracked.int32(0)),
#                                                FamosManager=cms.untracked.PSet(limit=cms.untracked.int32(100000)))

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )
    

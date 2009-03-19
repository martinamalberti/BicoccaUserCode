import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNINGS'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# Vertex smearing
# There're several types of Vertex smearing module (Gauss, Flat, BeamProfile, Betafunc, BetafuncEarlyCollisions);
# the cff's are available for each one :
# Configuration/StandardSequences/data/VtxSmearedGauss.cff
# Configuration/StandardSequences/data/VtxSmearedFlat.cff
# Configuration/StandardSequences/data/VtxSmearedBeamProfile.cff
# Configuration/StandardSequences/data/VtxSmearedBetafuncNominalCollision.cff
# Configuration/StandardSequences/data/VtxSmearedBetafuncEarlyCollision.cff
# Currently, we suggest using Betafunc vertex smearing in the desired LHC configuration
process.load("Configuration.StandardSequences.VtxSmearedBetafuncEarlyCollision_cff")

# Generator information and event analysis
process.load("Configuration.StandardSequences.Generator_cff")
process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.L1Emulator_cff")
process.load("Configuration.StandardSequences.DigiToRaw_cff")
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.VtxSmearedBetafuncEarlyCollision_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")


# Event output
process.load("Configuration.EventContent.EventContent_cff")

RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                           theSource = cms.PSet(
    initialSeed = cms.untracked.uint32(153456789),
    engineName = cms.untracked.string('HepJamesRandom')
    )
                                           )


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



### Some standard sequences needed for reconstruction ###
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

### BE CAREFUL WITH CONDITIONS!
#process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')
process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")



process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.TFileService = cms.Service("TFileService",
        	                   fileName = cms.string(
				    'deadTowers_FED_PG-DiEle10-redigi2.root'
					        	 )
                                   )


process.FEVT = cms.OutputModule("PoolOutputModule",
                                process.FEVTSIMEventContent,
                                fileName = cms.untracked.string('/gwtera5/users/data/10TeV-RECO/PhysVal-DiElectron-Ene50-phi0_1-eta-0.4_0.2.root')
                                #fileName = cms.untracked.string('test2.root')
                                
                                )

process.p0 = cms.Path(process.pgen)
process.p1 = cms.Path(process.psim)
process.p2 = cms.Path(process.pdigi)
process.p3 = cms.Path(process.L1Emulator)
process.p4 = cms.Path(process.DigiToRaw)
process.p5= cms.Path(process.RawToDigi)
process.p6= cms.Path(process.reconstruction)
process.outpath = cms.EndPath(process.FEVT)


process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.p4,process.p5,process.p6,process.outpath)

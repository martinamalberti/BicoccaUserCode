#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuAllChainTest")

process.load("Configuration.StandardSequences.Geometry_cff") # for CaloGeometryRecord

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

##########
# Source #
##########


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
#         'file:/tmp/amassiro/VBFHWW2l2nuTest_8.root'
        'file:/tmp/amassiro/CMSSWfile.root' 
        )
    )

    
   

#####################
# MC channel filter #

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCChannelFilter_cfi")

process.channelFilterMCSequence = cms.Sequence(
 process.channelFilterMC
)

# end MC channel filter #
#########################



##############################
# particle generation viewer #

process.include( "SimGeneral/HepPDTESSource/data/pythiapdt.cfi" )

process.printTree = cms.EDAnalyzer( "ParticleTreeDrawer",
  src = cms.InputTag("genParticles"),
#    printP4 = cms.untracked.bool( True ),
#    printPtEtaPhi = cms.untracked.bool( True ),
  printStatus = cms.untracked.bool( True ),
#   status = cms.untracked.vint32( 3 ),
  printIndex = cms.untracked.bool(True )
)


process.printDecay = cms.EDAnalyzer( "ParticleDecayDrawer",
   src = cms.InputTag("genParticles"),
   printP4 = cms.untracked.bool( False ),
   printPtEtaPhi = cms.untracked.bool( False ),
   printVertex = cms.untracked.bool( False )
#   printStatus = cms.untracked.bool( True ),
#   status = cms.untracked.vint32( 3 )
#   printIndex = cms.untracked.bool(True )
)




process.VBFParticleViewer = cms.Sequence(
#  process.channelFilterMC *
 process.printTree *
 process.printDecay
)

# end particle generation viewer #
##################################




########
# Path #
########

process.pathParticleViewer = cms.Path(process.VBFParticleViewer)

##########
# Output #
##########

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFHWW2l2nuEventContent_cff import *

process.VBFHWW2l2nuOutputModule = cms.OutputModule(
    "PoolOutputModule",
    VBFHWW2l2nuEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('USER')),
    fileName = cms.untracked.string('/tmp/amassiro/VBF_todayTest.root')
)

process.VBFHWW2l2nuOutputModule.outputCommands.append(
         'keep *_*_*_VBFHWW2l2nuAllChainTest'
)


process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("/tmp/amassiro/VBF_tree_todayTest.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )


process.o = cms.EndPath ( process.VBFHWW2l2nuOutputModule )




###########
# shedule #
###########

## only viewer ##
process.schedule = cms.Schedule(process.pathParticleViewer) # exec only these paths


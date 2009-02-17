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


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
#         'file:/tmp/amassiro/VBFHWW2l2nuTest_8.root'
           'rfio:/castor/cern.ch/user/a/amassiro/VBF/Dump/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_1.root'
#         'rfio:/castor/cern.ch/cms/store/path/filename.root'
        )
    )

    
##############################   
## muon and electron filter ## 
   
process.muonFilter = cms.EDFilter("PtMinMuonCountFilter",
  src = cms.InputTag("muons"),
  minNumber = cms.uint32(1),
  ptMin = cms.double(5.0)
)

process.electronFilter = cms.EDFilter("PtMinPixelMatchGsfElectronCountFilter",
  src = cms.InputTag("pixelMatchGsfElectrons"),
  minNumber = cms.uint32(1),
  ptMin = cms.double(5.0) 
)

process.EleSequence = cms.Sequence(
 process.electronFilter
)

process.MuSequence = cms.Sequence(
 process.muonFilter
)


## end muon and electron filter ## 
##################################


#######################
## for Tree producer ##

process.leptonsMinNumber = cms.EDFilter("VBFLeptonsMinNumFilter",
                       GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
                       muInputTag = cms.InputTag("muons"),  
                       elePtMin = cms.double(5),
                       muPtMin = cms.double(5),                      
                       minLeptonsNum = cms.int32(1)                                             
)

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIsolationSequence_cff")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIDSequence_cff")

process.VBFFirstFilterTree = cms.EDAnalyzer("VBFFirstFilterTreeProducer",
     verbosity = cms.bool(True),
     verbosity_mc = cms.bool(True),
 
     srcElectrons = cms.InputTag("pixelMatchGsfElectrons"),
     srcMuons = cms.InputTag("muons"),         
     srcJets = cms.InputTag("iterativeCone5CaloJets"),

     eleTkIso = cms.InputTag("electronTrackIsolationScone"), 
     eleEcalIso = cms.InputTag("electronEcalRecHitIsolationScone"),
     eleHcalIso = cms.InputTag("electronHcalDepth1TowerIsolationScone")
     
)
                               
process.VBFTreeSequence = cms.Sequence(
 process.VBFElectronIdSequence * 
 process.VBFElectronIsolationSequence *
 process.VBFFirstFilterTree
)

## end for Tree producer ##
###########################


##################
## VBF sequence ##
## * JetTagger ##
## * centralJetVeto ##
## * jetMCTagger ##
## * jetDiffTagFinder ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetTagger_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFCentralJetVetoFilter_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFDiffTagFinderComparison_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCJetTagger_cfi")


process.VBFSequence = cms.Sequence(
 process.jetTagger *
 process.centralJetVeto *
 process.jetMCTagger *
 process.jetDiffTagFinder
)

## end VBF sequence ##
######################


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
 process.channelFilterMC *
 process.printTree *
 process.printDecay
)

# end particle generation viewer #
##################################




########
# Path #
########

# process.countMu= cms.Path (process.muonFilter)
# process.countEle = cms.Path (process.electronFilter)
#process.countMuAndEle= cms.Path (process.muonFilter + process.electronFilter)

process.pathVBF = cms.Path(process.VBFSequence)
process.pathEle = cms.Path(process.EleSequence)
process.pathMu = cms.Path(process.MuSequence)
process.pathTree = cms.Path(process.leptonsMinNumber * process.VBFTreeSequence)
process.pathParticleViewer = cms.Path(process.VBFParticleViewer)

##########
# Output #
##########

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFHWW2l2nuEventContent_cff import *

process.VBFHWW2l2nuOutputModule = cms.OutputModule(
    "PoolOutputModule",
    VBFHWW2l2nuEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('USER')),
#     fileName = cms.untracked.string('VBFHWW2l2nuTest.root'),
    fileName = cms.untracked.string('/tmp/amassiro/VBF_todayTest.root'),
   
    SelectEvents = cms.untracked.PSet(
#                 SelectEvents = cms.vstring('countMu','countEle')
#                 SelectEvents = cms.vstring('countMuAndEle')
                  SelectEvents = cms.vstring('pathEle','pathMu')
    )
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
# process.schedule = cms.Schedule(process.pathParticleViewer) # exec only these paths

process.schedule = cms.Schedule(process.pathEle,process.pathMu,process.pathVBF,process.pathTree,process.o) # exec only these paths
# process.schedule = cms.Schedule(process.pathEle,process.pathMu,process.pathVBF,process.o) # exec only these paths

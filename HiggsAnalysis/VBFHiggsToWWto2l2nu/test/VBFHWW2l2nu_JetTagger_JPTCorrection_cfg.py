#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuJetTagger")

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
    skipBadFiles = cms.untracked.bool(True), 
    fileNames = cms.untracked.vstring(
           'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_1.root'
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

process.leptonsMinNumber = cms.EDFilter("VBFLeptonsMinNumFilter",
                       GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
                       muInputTag = cms.InputTag("muons"),  
                       elePtMin = cms.double(5),
                       muPtMin = cms.double(5),                      
                       minLeptonsNum = cms.int32(1) 
)

process.leptonsMinNumberSequence = cms.Sequence(
 process.leptonsMinNumber
)

## end muon and electron filter ## 
##################################


###################
## Jet cleaning  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleaner_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerRef_cfi")

process.cleanedJets.src = cms.InputTag("JetPlusTrackZSPCorJetIcone5")
process.cleanedJetsRef.src = cms.InputTag("JetPlusTrackZSPCorJetIcone5")


## end Jet cleaning  ##
#######################


######################
## Jet JPT orrector ##
## * VBFJetJPTCorrector ##
### https://twiki.cern.ch/twiki/bin/view/CMS/JetPlusTracksCorrections
### http://cmslxr.fnal.gov/lxr/source/JetMETCorrections/JetPlusTrack/test/JetAnalysis_cfg.py
### http://cmslxr.fnal.gov/lxr/source/JetMETCorrections/Configuration/python/JetPlusTrackCorrections_cff.py
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("JetMETCorrections.Configuration.JetPlusTrackCorrections_cff")
process.load("JetMETCorrections.Configuration.ZSPJetCorrections219_cff")

process.ZSPJetCorJetIcone5.src = cms.InputTag("iterativeCone5CaloJets") ### default value is alrerady this one!

process.VBFJetJPTCorrector = cms.Sequence(
 process.ZSPJetCorrections *
 process.JetPlusTrackCorrections
)

## end Jet JPT corrector ##
###########################



##################
## VBF sequence ##
## * VBFNtupleSequence ##
## * VBFJetTaggerSequence ##
## * VBFElectronSequence ##
## * VBFEventSelectorSequence ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetTagger_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFCentralJetVetoFilter_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFDiffTagFinderComparison_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCJetTagger_cfi")

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.SimpleNtple_cfi")

process.VBFJetTaggerSequence = cms.Sequence(
 process.VBFJetJPTCorrector * # Jet energy correction ZSP + JPT
 process.cleanedJets *        # Jet cleaning
 process.cleanedJetsRef *     # Jet cleaning ref creation
#  process.jetTagger *          # Jet tagger -> taggedJets + otherJets
#  process.centralJetVeto *     # Central Jet Veto -> (filter) bool
#  process.jetMCTagger *        # MC tagger: genParticles -> VBFMCTagJet (RecoChargedCandidate)
#  process.jetDiffTagFinder *   # MC tagger ntuple creator: jetTagger:taggedJets -> ntuple
 process.ntpla                # ntuple crator -> all particles/jets/met, reco/gen 
)

process.VBFNtupleSequence = cms.Sequence(
 process.VBFJetJPTCorrector * # Jet energy correction ZSP + JPT
 process.cleanedJets *                # Jet cleaning
 process.cleanedJetsRef *             # Jet cleaning ref creation
 process.ntpla                        # ntuple crator -> all particles/jets/met, 
)

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIsolationSequence_cff")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIDSequence_cff")

process.VBFElectronSequence = cms.Sequence(
 process.VBFElectronIdSequence * 
 process.VBFElectronIsolationSequence
)

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEventSelector_cfi")
process.VBFEventSelectorSequence = cms.Sequence(
 process.VBFJetTaggerSequence *
 process.selectedEvents
)

## end VBF sequence ##
######################


#####################
# MC channel filter #

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCChannelFilter_cfi")

process.channelFilterMCSequence = cms.Sequence(
 process.channelFilterMC      # Select VBF event with ee,mumu,emu,all combinations
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


process.VBFParticleViewerSequence = cms.Sequence(
 process.channelFilterMC *
 process.printTree *
 process.printDecay
)

# end particle generation viewer #
##################################




########
# Path #
########

process.pathEle = cms.Path(process.EleSequence)
process.pathMu = cms.Path(process.MuSequence)
process.pathleptonsMinNumber = cms.Path(process.leptonsMinNumberSequence)

process.pathVBFJetTagger = cms.Path(process.VBFJetTaggerSequence)

process.pathVBFNtuple = cms.Path(process.VBFNtupleSequence)

process.pathVBFEventSelector = cms.Path(process.VBFEventSelectorSequence)

process.pathVBFElectron = cms.Path(process.VBFElectronSequence)
process.pathParticleViewer = cms.Path(process.VBFParticleViewerSequence)


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
   
)

process.VBFHWW2l2nuOutputModule.outputCommands.append(
         'keep *_*_*_VBFHWW2l2nuJetTagger'
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

# process.schedule = cms.Schedule(process.pathVBFEventSelector,process.o)
# process.schedule = cms.Schedule(process.pathVBFJetTagger,process.o) 
process.schedule = cms.Schedule(process.pathVBFNtuple,process.o) 
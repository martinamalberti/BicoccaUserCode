import FWCore.ParameterSet.Config as cms

process = cms.Process("kineWW6TeV")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_11.root' ,
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_14.root',
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_16.root',
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_20.root' ,
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_4.root',
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_5.root' ,
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_6.root' ,
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_7.root' ,
      'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/10TeV_ttbar/CMSSWfile_9.root' 
    )
)

# from HiggsAnalysis.PhantomTest.VBFJetCleaner_cfi import *
from HiggsAnalysis.PhantomTest.VBFJetCleaner_cfi import jetCleaner as somethingElse
# process.load("HiggsAnalysis.PhantomTest.VBFJetCleaner_cfi")

process.filtro = somethingElse

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service ("TFileService", 
    fileName = cms.string ("/tmp/amassiro/histo_ttbar_10TeV_cleaned.root")
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.MessageLogger = cms.Service("MessageLogger")

process.kinematics = cms.EDAnalyzer("VBFKinematics",
    eleIDInputTag = cms.InputTag("eidTight"),
    GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
    muInputTag = cms.InputTag("muons"),
    jetTagsInputTag = cms.InputTag("iterativeCone5CaloJets"),
    metInputTag = cms.InputTag("met")
)

process.reading = cms.Path(process.filtro * process.kinematics)
# process.reading = cms.Path(jetCleaner * process.kinematics)

#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFMCDumper")

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCDumper_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'






# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'file:/tmp/amassiro/VBFHWW2l2nuTest_9.root',
        'file:/tmp/amassiro/VBFHWW2l2nuTest_8.root',
        'file:/tmp/amassiro/VBFHWW2l2nuTest_7.root'
        'file:/tmp/amassiro/VBFHWW2l2nuTest_6.root',
        'file:/tmp/amassiro/VBFHWW2l2nuTest_5.root',
        'file:/tmp/amassiro/VBFHWW2l2nuTest_4.root',
        'file:/tmp/amassiro/VBFHWW2l2nuTest_3.root'
        
        #'file:/tmp/abenagli/VBFHWW2l2nuTest_11.root',
        #'file:/tmp/abenagli/VBFHWW2l2nuTest_13.root',
        #'file:/tmp/abenagli/VBFHWW2l2nuTest_15.root'
       	)
    )


#####################
# MC channel filter #

# process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCChannelFilter_cfi")
# 
# process.channelFilterMC.channel = 0
# 



process.channelFilterMC = cms.EDFilter("MCProcessFilter",
    ProcessID = cms.untracked.vint32(123, 124) ### fifj-> fifjh0  || fifj-> fkflh0
#     ProcessID = cms.untracked.vint32(102)  ### gg-> h0
)


# end MC channel filter #
#########################




# Dump MC information
process.VBFMCDumper.fileName = "VBFMCDumper_qqH"
# process.VBFMCDumper.fileName = "VBFMCDumper_ggH"
process.VBFMCDumper.mass = "130"
process.VBFMCDumper.verbosity = False
process.VBFMCDumper.eventsToPrint = 0

process.p0 = cms.Path(process.channelFilterMC * process.VBFMCDumper)






process.schedule = cms.Schedule(process.p0)

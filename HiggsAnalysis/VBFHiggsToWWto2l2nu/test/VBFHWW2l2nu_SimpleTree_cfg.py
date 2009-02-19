#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuSImpleTree")

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
# 'rfio:/castor/cern.ch/user/a/amassiro/VBF/Dump/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_1.root'
#     'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H130_WW_2l/VBFHWW2l2nuTest_10.root'

'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_1.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_10.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_11.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_12.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_13.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_14.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_15.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_16.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_17.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_18.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_19.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_2.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_20.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_21.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_22.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_23.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_24.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_25.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_26.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_27.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_3.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_4.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_5.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_6.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_7.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_8.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Everything_Skimmed_4Cluster/H160_WW_2l/VBFHWW2l2nuTest_9.root'




     )
    )

    
   
##########################
## SimpleTree sequence ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.SimpleNtple_cfi")

process.ntpleSequence = cms.Sequence(
 process.ntpla
)

## end SimpleTree sequence ##
#####################################




########
# Path #
########

process.pathNtuple = cms.Path(process.ntpleSequence)

##########
# Output #
##########

process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("VBF_SimpleTree.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )



###########
# shedule #
###########


process.schedule = cms.Schedule(process.pathNtuple) 
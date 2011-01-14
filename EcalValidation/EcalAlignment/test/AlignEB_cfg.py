import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.inputTree = cms.PSet(
  nameTree = cms.string("ntupleEcalAlignment/myTree"),
  selection = cms.string("(eleFBrem<0.8)"),
#  selection = cms.string("(eleFBrem<0.3)"),
#  selection = cms.string("(eleFBrem<0.8&&eleCharge>0)"),
#  selection = cms.string("(eleFBrem<0.8&&eleCharge<0)"),
 # selection = cms.string("1"),
  traslationX = cms.bool(True),
  traslationY = cms.bool(True),
  traslationZ = cms.bool(True),
  rotationPhi   = cms.bool(True),
  rotationTheta = cms.bool(False),
  rotationPsi   = cms.bool(False),

  setRotationPhi   = cms.untracked.double(0),
  setRotationTheta = cms.untracked.double(0),
  setRotationPsi   = cms.untracked.double(0),

  inputFiles    = cms.vstring(
         'EcalAlignmentJoin.root'
    )
)


process.outputTree = cms.PSet(
  outputFile = cms.string("myEBAlignment_2010.txt")
#  outputFile = cms.string("myEBAlignment_2010_TightFbrem.txt")
#  outputFile = cms.string("myEBAlignment_2010_Plus.txt")
  #outputFile = cms.string("myEBAlignment_2010_Minus.txt")
)

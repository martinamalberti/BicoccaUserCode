import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.inputTree = cms.PSet(
  nameTree = cms.string("ntupleEcalAlignment/myTree"),
#  selection = cms.string("(eleFBrem<0.8&&eleCharge>0)"),
  selection = cms.string("1"),
  traslationX = cms.bool(True),
  traslationY = cms.bool(True),
  traslationZ = cms.bool(True),
  rotationPhi   = cms.bool(False),
  rotationTheta = cms.bool(False),
  rotationPsi   = cms.bool(False),
  inputFiles    = cms.vstring(
        'EcalAlignment.root'
#        'EcalAlignment2.root'
    )
)


process.outputTree = cms.PSet(
 outputFile = cms.string("outEB.txt"),
# outputFile = cms.string("outEE.txt"),
)

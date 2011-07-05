import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.inputTree = cms.PSet(
  nameTree = cms.string("ntupleEcalAlignment/myTree"),
  #selection = cms.string("(ETSC>20 && met>20 && mishits <= 0 && MT>30)"),
   
   selection = cms.string("(ETSC>30 && met>30 && mishits <= 0 && MT>50) && ((abs(eta) <= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.07 && abs(SigmaIEtaIEta)<0.01) || (abs(eta) >= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.06 && abs(SigmaIEtaIEta)<0.03))"),
     
  #selection = cms.string("(1)"),
   #selection = cms.string("(eleFBrem<0.8)"),
#  selection = cms.string("(eleFBrem<0.3)"),
#  selection = cms.string("(eleFBrem<0.8&&eleCharge>0)"),
#  selection = cms.string("(eleFBrem<0.8&&eleCharge<0)"),
 # selection = cms.string("1"),
  traslationX = cms.bool(True),
  traslationY = cms.bool(True),
  traslationZ = cms.bool(True),
  rotationPhi   = cms.bool(False),
  rotationTheta = cms.bool(False),
  rotationPsi   = cms.bool(False),

  setRotationPhi   = cms.untracked.double(0),
  setRotationTheta = cms.untracked.double(0),
  setRotationPsi   = cms.untracked.double(0),

  #DetaBias = cms.untracked.string("0*x*y"), 
  #DphiBias = cms.untracked.string("0*x*y"),
  #DetaBias = cms.untracked.string("(x>0) * (-2e-4) + (x<0) * (2e-4) + y*0"), 
  #DphiBias = cms.untracked.string("(y>0 && x>0) * (6e-4) + (y>0 && x<0) * (-2e-4) + (y<0 && x>0) * (1e-4) + (y<0 && x<0) * (-8e-4)"),

  #DetaBias = cms.untracked.string("(x>0 && x<1.5) * -3e-4 + (x>1.5) * 0 + (x<0 && x>-1.5) * 3e-4 + (x<-1.5) * 0 + y*0"),
  #DphiBias = cms.untracked.string("(y>0)*((x>0 && x<1.5) * 1.59e-3 + (x>1.5) * 3.2e-3 + (x<0 && x>-1.5) * 7.5e-4 + (x<-1.5) * 3.09e-3) + (y<0)*((x>0 && x<1.5) * -8.75e-4 + (x>1.5) * -3.2e-3 + (x<0 && x>-1.5) * -1.54e-3 + (x<-1.5) * -3.18e-5)"),

  DetaBias = cms.untracked.string("(x>0 && x<1.5) * -2.93e-4 + (x>1.5) * 0 + (x<0 && x>-1.5) * (3.25e-4) + (x<-1.5) * 0 + y*0"), 
  
  DphiBias = cms.untracked.string("(y>0)*((x>0 && x<1.5) * 1.57e-3 + (x>1.5) * 3.24e-3 + (x<0 && x>-1.5) * 7.5e-4 + (x<-1.5) * 3.08e-3) + (y<0)*((x>0 && x<1.5) * (-8.88e-4) + (x>1.5) * (-3.19e-3) + (x<0 && x>-1.5) * (-1.65e-3) + (x<-1.5) * (-3.13e-3))"),

  #inputFilesPosition    = cms.untracked.string(
         #'myEBAlignment_2010_OLD.txt'
    #),

  inputFiles    = cms.vstring(
         #'EcalAlignmentJoin.root'
         #'crab/DATAJSONRawRecoRealign_Correct_Rome_CorrectReAlignDB/SingleElectron_Run2011A-WElectron-PromptSkim-v4_RAW-RECO/newsum.root'
         'crab/DATAJSONPrompt_SingleElectron_Run2011A-PromptReco-v4_AOD.root'
    )
)


process.outputTree = cms.PSet(
  outputFile = cms.string("myEBAlignment_2011.txt")
#  outputFile = cms.string("myEBAlignment_2010_TightFbrem.txt")
#  outputFile = cms.string("myEBAlignment_2010_Plus.txt")
  #outputFile = cms.string("myEBAlignment_2010_Minus.txt")
)


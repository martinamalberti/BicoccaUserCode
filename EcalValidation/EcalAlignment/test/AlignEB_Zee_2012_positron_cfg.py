import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.inputTree = cms.PSet(
  nameTree = cms.string("ntupleEcalAlignment/myTree"),
  #selection = cms.string("(ETSC>20 && met>20 && mishits <= 0 && MT>30)"),
   
   selection = cms.string("eleCharge>0 && (ETSC>30 && mishits <= 0 ) && ((abs(eta) <= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.07 && abs(SigmaIEtaIEta)<0.01) || (abs(eta) >= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.06 && abs(SigmaIEtaIEta)<0.03))"),
     
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


  DetaBias = cms.untracked.string("(x>0 && x<1.5) * (-0.000236) + (x>1.5) * (-4.83e-05) + (x<0 && x>-1.5) * (0.000272) + (x<-1.5) * (6.91e-05) + y*0"),
  DphiBias = cms.untracked.string("(y>0)*((x>0 && x<1.5) * 0.00136 + (x>1.5) * 0.000878 + (x<0 && x>-1.5) * 0.000579 + (x<-1.5) * 0.000797) + (y<0)*((x>0 && x<1.5) * (-0.000721) + (x>1.5) * (-0.000863) + (x<0 && x>-1.5) * (-0.00141) + (x<-1.5) * (-0.000819))"),

	
  #inputFilesPosition    = cms.untracked.string(
         #'myEBAlignment_2010_OLD.txt'
    #),

  inputFiles    = cms.vstring(
         #'EcalAlignmentJoin.root'
         #'crab/DATAJSONRawRecoRealign_Correct_Rome_CorrectReAlignDB/SingleElectron_Run2011A-WElectron-PromptSkim-v4_RAW-RECO/newsum.root'
         #'crab/DATAJSONPrompt_SingleElectron_Run2011A-PromptReco-v4_AOD.root'
          '/tmp/amassiro/Run2012ABEE.root'
    )
)


process.outputTree = cms.PSet(
   outputFile = cms.string("myEBAlignment_2012_new2012MCbias_positron.txt")
   #outputFile = cms.string("myEBAlignment_2011.txt")
#  outputFile = cms.string("myEBAlignment_2010_TightFbrem.txt")
#  outputFile = cms.string("myEBAlignment_2010_Plus.txt")
  #outputFile = cms.string("myEBAlignment_2010_Minus.txt")
)


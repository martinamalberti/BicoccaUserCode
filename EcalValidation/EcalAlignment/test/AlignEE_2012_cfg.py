import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.inputTree = cms.PSet(
  nameTree = cms.string("ntupleEcalAlignment/myTree"),
  #selection = cms.string("(ETSC>20 && met>30 && mishits <= 0 && MT>30)"),
  
   selection = cms.string("(ETSC>30 && met>30 && mishits <= 0 && MT>50) && ((abs(eta) <= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.07 && abs(SigmaIEtaIEta)<0.01) || (abs(eta) >= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.06 && abs(SigmaIEtaIEta)<0.03))"),
    
    
  #selection = cms.string("(1)"),
  #selection = cms.string("(eleFBrem<0.8)"),
#  selection = cms.string("(eleFBrem<0.8&&eleCharge>0)"),
  #selection = cms.string("(eleFBrem<0.8&&eleCharge<0)"),
#  selection = cms.string("1"),
  traslationX = cms.bool(True),
  traslationY = cms.bool(True),
  traslationZ = cms.bool(True),
  rotationPhi   = cms.bool(True),
  rotationTheta = cms.bool(False),
  rotationPsi   = cms.bool(True),

  setRotationPhi   = cms.untracked.double(0),
  setRotationTheta = cms.untracked.double(0),
  setRotationPsi   = cms.untracked.double(0),

  DetaBias = cms.untracked.string("(x>0 && x<1.5) * (-0.000255) + (x>1.5) * (-2.21e-05) + (x<0 && x>-1.5) * (0.000275) + (x<-1.5) * (-9.08e-06) + y*0"),
  DphiBias = cms.untracked.string("(y>0)*((x>0 && x<1.5) * 0.00136 + (x>1.5) * 0.000927 + (x<0 && x>-1.5) * 0.000602 + (x<-1.5) * 0.00265) + (y<0)*((x>0 && x<1.5) * (-0.000827) + (x>1.5) * (-0.00274) + (x<0 && x>-1.5) * (-0.00158) + (x<-1.5) * (-0.00273))"),
        
  #inputFilesPosition    = cms.untracked.string(
         #'myEEAlignment_2010_OLD.txt'
  #),
   
  inputFiles    = cms.vstring(
          '/tmp/amassiro/Run2012B_OldECAL_NewTrk.root' 
         #'crab/DATAJSONPrompt_SingleElectron_Run2011A-PromptReco-v4_AOD.root'
         #'crab/DATAJSONRawRecoRealign_Correct_Rome_CorrectReAlignDB/SingleElectron_Run2011A-WElectron-PromptSkim-v4_RAW-RECO/newsum.root'
         #'EcalAlignmentJoin.root'
    )
)


process.outputTree = cms.PSet(
#  outputFile = cms.string("myEEAlignment_2012_withOutRotations_new2012MCbias.txt")
  outputFile = cms.string("myEEAlignment_2012_withRotations_new2012MCbias.txt")
  #outputFile = cms.string("myEEAlignment_2011_withRotations.txt")
# outputFile = cms.string("myEEAlignment_2010_Plus.txt")
# outputFile = cms.string("myEEAlignment_2010_Minus.txt")
)


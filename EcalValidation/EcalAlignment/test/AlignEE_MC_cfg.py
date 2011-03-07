import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.inputTree = cms.PSet(
  nameTree = cms.string("ntupleEcalAlignment/myTree"),
  #selection = cms.string("(ETSC>20 && met>30 && mishits <= 0 && MT>30)"),
  selection = cms.string("(ETSC>20 && met>30 && mishits <= 0 && MT>30) && ((abs(eta) <= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.07 && (eleTrkIso/pT<0.09 && eleEcalIso/pT<0.07 && (eleHcalIsoD1+eleHcalIsoD2)/pT<0.09) && (HoE<0.040 && abs(SigmaIEtaIEta)<0.01)) || (abs(eta) >= 1.5 && (eleTrkIso + eleEcalIso + eleHcalIsoD1 + eleHcalIsoD2)/pT < 0.06 && (eleTrkIso/pT<0.04 && eleEcalIso/pT<0.05 && (eleHcalIsoD1+eleHcalIsoD2)/pT<0.025) && (HoE<0.025 && abs(SigmaIEtaIEta)<0.03) ) )"),
    
    
  #selection = cms.string("(1)"),
  #selection = cms.string("(eleFBrem<0.8)"),
#  selection = cms.string("(eleFBrem<0.8&&eleCharge>0)"),
  #selection = cms.string("(eleFBrem<0.8&&eleCharge<0)"),
#  selection = cms.string("1"),
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
  DetaBias = cms.untracked.string("(x>1.5) * (1.3e-3 - 5e-4*x) + (x<-1.5) * (-1.3e-3 - 5e-4*x) + y*0"), 
  DphiBias = cms.untracked.string("(y>0 && x>1.5) * (-4e-4 + 7e-4*x) + (y>0 && x<-1.5) * (-1.2e-3-7e-4*x) + (y<0 && x>1.5) * (1.3e-3 -7e-4*x) + (y<0 && x<-1.5) * (1.2e-3+7e-4*x)"),
      
     
  #inputFilesPosition    = cms.untracked.string(
         #'myEBAlignment_2010_OLD.txt'
  #),
   
  inputFiles    = cms.vstring(
         'EcalAlignmentJoinMyAlign_MC.root'
    )
)


process.outputTree = cms.PSet(
  outputFile = cms.string("myEEAlignment_2010_FullStat.txt")
# outputFile = cms.string("myEEAlignment_2010_Plus.txt")
# outputFile = cms.string("myEEAlignment_2010_Minus.txt")
)


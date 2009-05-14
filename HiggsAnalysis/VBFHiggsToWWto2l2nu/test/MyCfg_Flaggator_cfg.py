import FWCore.ParameterSet.Config as cms
process = cms.Process("TEST")

process.Selections = cms.PSet(

# Electron Id
#   from SimpleNtuple -> VBFHWW2l2nu_SimpleTree_Template_cfg.py
#    1   = eidLoose
#    10  = eidRobustLoose 
#    100 = eidTight
 
 g_ID1 = cms.untracked.int32(100), # electron
 g_ID2 = cms.untracked.int32(100), # electron

# variable to decide if make Isolation selection:
#    1 = true
#    0 = false 
# 0 = electron     1 = muon  
 g_ISO1_0 = cms.untracked.int32(1), # electron
 g_ISO1_1 = cms.untracked.int32(1), # muon
 g_ISO2_0 = cms.untracked.int32(1), # electron
 g_ISO2_1 = cms.untracked.int32(1), # muon

 g_IsoElectron = cms.untracked.double(0.2),
 g_IsoMuon = cms.untracked.double(0.2),
 
 g_ojetPtMin = cms.untracked.double(15),
 g_ojetEtaMax = cms.untracked.double(5.),
 
 
 g_KindOfJet = cms.untracked.string("otherJets_IterativeCone5CaloJets"), 


          

# output #
 g_prefix = cms.untracked.string("MyHisto_"),
          


)
 

process.inputTree = cms.PSet(

# input #
 g_number_of_samples = cms.untracked.int32(8),
 g_numSignal = cms.untracked.int32(1),
  
 g_directory = cms.untracked.string("/media/amassiro/Data/SimpleTree_skipBadFiles_JetCorrector_JetCleaning_090430_Everything_Skimmed_4Cluster_AllJets"),
    
 g_sample = cms.vstring(

  'H160_WW_2l_Summer08_IDEAL_V11_redigi_v1',
  'SingleTop_sChannel_Summer08_IDEAL_V11_redigi_v3',
  'SingleTop_tChannel_Summer08_IDEAL_V11_redigi_v3',
  'SingleTop_tWChannel_Summer08_IDEAL_V11_redigi_v3',
  'TauolaTTbar_Summer08_IDEAL_V11_redigi_v2',
  'VVJets_madgraph_Fall08_IDEAL_V9_v2',
#   'VQQ_madgraph_Summer08_IDEAL_V11_redigi_v2',
  'WJets_madgraph',
  'ZJets_madgraph_Summer08_IDEAL_V11_redigi_v1'

# # # 
# # #     
# # # 'H115_tautau_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H130_tautau_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H130_WW_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H130_ZZ_2lqq_Summer08_IDEAL_V11_redigi_v1',
# # # 'H150_WW_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H160_WW_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H170_WW_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H180_ZZ_2lqq_Summer08_IDEAL_V11_redigi_v1',
# # # 'H200_WW_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H200_ZZ_2lqq_Summer08_IDEAL_V11_redigi_v1',
# # # 'H500_WW_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'H500_ZZ_2lqq_Summer08_IDEAL_V11_redigi_v1',
# # # 'SingleTop_sChannel_Summer08_IDEAL_V11_redigi_v3',
# # # 'SingleTop_tChannel_Summer08_IDEAL_V11_redigi_v3',
# # # 'SingleTop_tWChannel_Summer08_IDEAL_V11_redigi_v3',
# # # 'TauolaTTbar_Summer08_IDEAL_V11_redigi_v2',
# # # 'TTJets_madgraph_Fall08_IDEAL_V11_redigi_v10',
# # # 'VVJets_madgraph_Fall08_IDEAL_V9_v2',
# # # 'WW_2l_Summer08_IDEAL_V11_redigi_v1',
# # # 'WW_Summer08_IDEAL_V11_redigi_v1',
# # # 'WZ_3l_Summer08_IDEAL_V11_redigi_v1',
# # # 'WZ_incl_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt0to15_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt120to170_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt15to20_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt170to230_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt20to30_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt230to300_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt300toInf_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt30to50_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt50to80_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZeeJet_Pt80to120_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt0to15_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt120to170_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt15to20_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt170to230_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt20to30_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt230to300_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt300toInf_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt30to50_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZmumuJet_Pt50to80_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZZ_2l2n_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZZ_4l_Summer08_IDEAL_V11_redigi_v1',
# # # 'ZZ_Summer08_IDEAL_V11_redigi_v1'






         
#    'H130_WW_2l_Summer08_IDEAL_V11_redigi_v1',
#    'H160_WW_2l_Summer08_IDEAL_V11_redigi_v1',
#    'H170_WW_2l_Summer08_IDEAL_V11_redigi_v1',
#    'H200_WW_2l_Summer08_IDEAL_V11_redigi_v1',
# 
#    'SingleTop_sChannel_Summer08_IDEAL_V11_redigi_v3', 
# #    'SingleTop_tChannel_Summer08_IDEAL_V11_redigi_v3', 
#    
#    'TauolaTTbar_Summer08_IDEAL_V11_redigi_v2', 
#    'VVJets_madgraph_Fall08_IDEAL_V9_v2', 
# 
#    'WW_2l_Summer08_IDEAL_V11_redigi_v1', 
#    'WZ_3l_Summer08_IDEAL_V11_redigi_v1', 
#    
#    'ZeeJet_Pt0to15_Summer08_IDEAL_V11_redigi_v1',     
#    'ZeeJet_Pt120to170_Summer08_IDEAL_V11_redigi_v1',     
#    'ZeeJet_Pt15to20_Summer08_IDEAL_V11_redigi_v1',     
#    'ZeeJet_Pt170to230_Summer08_IDEAL_V11_redigi_v1',     
#    'ZeeJet_Pt20to30_Summer08_IDEAL_V11_redigi_v1',     
#    'ZeeJet_Pt230to300_Summer08_IDEAL_V11_redigi_v1_cfg',        
#    'ZeeJet_Pt300toInf_Summer08_IDEAL_V11_redigi_v1',     
#    'ZeeJet_Pt30to50_Summer08_IDEAL_V11_redigi_v1',          
#    'ZeeJet_Pt50to80_Summer08_IDEAL_V11_redigi_v1',          
#    'ZeeJet_Pt80to120_Summer08_IDEAL_V11_redigi_v1',        
# 
#    'ZmumuJet_Pt0to15_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt120to170_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt15to20_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt170to230_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt20to30_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt230to300_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt300toInf_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt30to50_Summer08_IDEAL_V11_redigi_v1',     
#    'ZmumuJet_Pt50to80_Summer08_IDEAL_V11_redigi_v1',     
#        
#    'ZZ_2l2n_Summer08_IDEAL_V11_redigi_v1',     
#    'ZZ_Summer08_IDEAL_V11_redigi_v1' 
         
  )
      
)

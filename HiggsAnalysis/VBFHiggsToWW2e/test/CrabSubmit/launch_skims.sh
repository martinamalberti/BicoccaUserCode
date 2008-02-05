# SIGNALS
# ----------

perl launch_a_skim.pl H160 /H160_WW_2l/CMSSW_1_6_7-CSA07-1192836764/RECO -1
perl launch_a_skim.pl H120 /H120_WW_2l/CMSSW_1_6_7-CSA07-1194963339/RECO -1
perl launch_a_skim.pl H130 /H130_WW_2l/CMSSW_1_6_7-CSA07-1194126306/RECO -1
perl launch_a_skim.pl H140 /H140_WW_2l/CMSSW_1_6_7-CSA07-1192835331/RECO -1
perl launch_a_skim.pl H150 /H150_WW_2l/CMSSW_1_6_7-CSA07-1194126384/RECO -1
perl launch_a_skim.pl H170 /H170_WW_2l/CMSSW_1_6_7-CSA07-1194126536/RECO -1
perl launch_a_skim.pl H180 /H180_WW_2l/CMSSW_1_6_7-CSA07-1192835387/RECO -1
perl launch_a_skim.pl H190 /H190_WW_2l/CMSSW_1_6_7-CSA07-1194126661/RECO -1
perl launch_a_skim.pl H200 /H200_WW_2l/CMSSW_1_6_7-CSA07-1192835385/RECO -1

# Wt, WW, WZ
# ----------

perl launch_a_skim.pl Wt /tW_inclusive/CMSSW_1_6_7-CSA07-1195471738/RECO 20000
perl launch_a_skim.pl WW /WW_incl/CMSSW_1_6_7-CSA07-1196178448/RECO 20000
perl launch_a_skim.pl WZ_incl /WZ_incl/CMSSW_1_6_7-CSA07-1195629996/RECO 20000
perl launch_a_skim.pl WZ /WZ/CMSSW_1_6_7-CSA07-1195663763/RECO 20000

# TTBAR
# ----------

perl launch_a_skim.pl tt0j_alpgen /tt0j_mT_70-alpgen/CMSSW_1_6_0-PreCSA07-HLT-A2/GEN-SIM-DIGI-RECO 20000
#no data found
perl launch_a_skim.pl tt2j_alpgen /tt2j_mT_70-alpgen/CMSSW_1_6_0-PreCSA07-HLT-A2/GEN-SIM-DIGI-RECO 20000
perl launch_a_skim.pl tt3j_alpgen /tt3j_mT_70-alpgen/CMSSW_1_6_0-PreCSA07-HLT-A2/GEN-SIM-DIGI-RECO -1
perl launch_a_skim.pl tt4j_alpgen /tt4j_mT_70-alpgen/CMSSW_1_6_0-PreCSA07-HLT-A2/GEN-SIM-DIGI-RECO -1

perl launch_a_skim.pl tt0j_alpgen_152 /tt0j_mT_70-alpgen/CMSSW_1_5_2-CSA07-2231/GEN-SIM-DIGI-RECO 20000
perl launch_a_skim.pl tt1j_alpgen_152 /tt1j_mT_70-alpgen/CMSSW_1_5_2-CSA07-2211/GEN-SIM-DIGI-RECO 20000
perl launch_a_skim.pl tt2j_alpgen_152 /tt2j_mT_70-alpgen/CMSSW_1_5_2-CSA07-2232/GEN-SIM-DIGI-RECO 20000
perl launch_a_skim.pl tt3j_alpgen_152 /tt3j_mT_70-alpgen/CMSSW_1_5_2-CSA07-2212/GEN-SIM-DIGI-RECO 20000
perl launch_a_skim.pl tt4j_alpgen_152 /tt4j_mT_70-alpgen/CMSSW_1_5_2-CSA07-2233/GEN-SIM-DIGI-RECO 20000

# DY
# ----------

perl launch_a_skim.pl ZZ_incl /ZZ_incl/CMSSW_1_6_7-CSA07-1194964234/RECO 20000

# ZEE JETS
# ----------

perl launch_a_skim.pl Zeej_0_15 /ZeeJets_Pt_0_15/CMSSW_1_6_7-CSA07-1192836918/RECO 5000
perl launch_a_skim.pl Zeej_15_20 /ZeeJets_Pt_15_20/CMSSW_1_6_7-CSA07-1192836131/RECO 5000
perl launch_a_skim.pl Zeej_15_20 /ZeeJets_Pt_20_30/CMSSW_1_6_7-CSA07-1200647491/RECO
perl launch_a_skim.pl Zeej_30_50 /ZeeJets_Pt_30_50/CMSSW_1_6_7-CSA07-1195560538/RECO 20000
perl launch_a_skim.pl Zeej_50_80 /ZeeJets_Pt_50_80/CMSSW_1_6_7-CSA07-1194964023/RECO 5000
perl launch_a_skim.pl Zeej_80_120 /ZeeJets_Pt_80_120/CMSSW_1_6_7-CSA07-1192837232/RECO 5000
perl launch_a_skim.pl Zeej_120_170 /ZeeJets_Pt_120_170/CMSSW_1_6_7-CSA07-1192837023/RECO 5000
perl launch_a_skim.pl Zeej_170_230 /ZeeJets_Pt_170_230/CMSSW_1_6_7-CSA07-1192837076/RECO 5000

#perl makeConfigs.pl Zeej_230_300 /ZeeJets_Pt_230_300/CMSSW_1_6_7-CSA07-1192835650/RECO
#perl makeConfigs.pl Zeej_380_470 /ZeeJets_Pt_380_470/CMSSW_1_6_7-CSA07-1192837180/RECO
#perl makeConfigs.pl Zeej_470_600 /ZeeJets_Pt_470_600/CMSSW_1_6_7-CSA07-1192835807/RECO
#perl makeConfigs.pl Zeej_600_800 /ZeeJets_Pt_600_800/CMSSW_1_6_7-CSA07-1192835860/RECO
#perl makeConfigs.pl Zeej_800_1000 /ZeeJets_Pt_800_1000/CMSSW_1_6_7-CSA07-1194964076/RECO
#perl makeConfigs.pl Zeej_1000_1400 /ZeeJets_Pt_1000_1400/CMSSW_1_6_7-CSA07-1192835543/RECO
#perl makeConfigs.pl Zeej_1400_1800 /ZeeJets_Pt_1400_1800/CMSSW_1_6_7-CSA07-1192835597/RECO
#perl makeConfigs.pl Zeej_1800_2200 /ZeeJets_Pt_1800_2200/CMSSW_1_6_7-CSA07-1192837127/RECO
#perl makeConfigs.pl Zeej_2200_2600 /ZeeJets_Pt_2200_2600/CMSSW_1_6_7-CSA07-1192836022/RECO
#perl makeConfigs.pl Zeej_2600_3000 /ZeeJets_Pt_2600_3000/CMSSW_1_6_7-CSA07-1192836237/RECO
#perl makeConfigs.pl Zeej_2600_3000 /ZeeJets_Pt_2600_3000/CMSSW_1_6_7-CSA07-1194963918/RECO
#perl makeConfigs.pl Zeej_3000_3500 /ZeeJets_Pt_3000_3500/CMSSW_1_6_7-CSA07-1192835702/RECO
#perl makeConfigs.pl Zeej_00_380 /ZeeJets_Pt_300_380/CMSSW_1_6_7-CSA07-1192835754/RECO
#perl makeConfigs.pl Zeej_3500_1 /ZeeJets_Pt_3500_-1/CMSSW_1_6_7-CSA07-1192835650/RECO

# WEN JETS
# ----------

perl launch_a_skim.pl Wenj_0_15 /WenuJets_Pt_0_15/CMSSW_1_6_7-CSA07-1192837492/RECO 20000
perl launch_a_skim.pl Wenj_15_20 /WenuJets_Pt_15_20/CMSSW_1_6_7-CSA07-1192837647/RECO 20000
perl launch_a_skim.pl Wenj_20_30 /WenuJets_Pt_20_30/CMSSW_1_6_7-CSA07-1192837803/RECO 20000
perl launch_a_skim.pl Wenj_30_50 /WenuJets_Pt_30_50/CMSSW_1_6_7-CSA07-1192838114/RECO 20000
perl launch_a_skim.pl Wenj_50_80 /WenuJets_Pt_50_80/CMSSW_1_6_7-CSA07-1192838322/RECO 20000
perl launch_a_skim.pl Wenj_80_120 /WenuJets_Pt_80_120/CMSSW_1_6_7-CSA07-1192836181/RECO 20000
perl launch_a_skim.pl Wenj_120_170 /WenuJets_Pt_120_170/CMSSW_1_6_7-CSA07-1194806653/RECO 20000
perl launch_a_skim.pl Wenj_170_230 /WenuJets_Pt_170_230/CMSSW_1_6_7-CSA07-1192837699/RECO 20000

#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_230_300/CMSSW_1_6_7-CSA07-1192837907/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_300_380/CMSSW_1_6_7-CSA07-1192838062/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_380_470/CMSSW_1_6_7-CSA07-1192838218/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_470_600/CMSSW_1_6_7-CSA07-1192838270/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_600_800/CMSSW_1_6_7-CSA07-1192838374/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_800_1000/CMSSW_1_6_7-CSA07-1192838425/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_1000_1400/CMSSW_1_6_7-CSA07-1192837543/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_1400_1800/CMSSW_1_6_7-CSA07-1192837595/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_1800_2200/CMSSW_1_6_7-CSA07-1192837750/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_2200_2600/CMSSW_1_6_7-CSA07-1192837855/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_2600_3000/CMSSW_1_6_7-CSA07-1192837959/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_3000_3500/CMSSW_1_6_7-CSA07-1192838011/RECO
#perl makeConfigs.pl Wenj_0_15 /WenuJets_Pt_3500_-1/CMSSW_1_6_7-CSA07-1192838167/RECO


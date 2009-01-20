import FWCore.ParameterSet.Config as cms

process = cms.Process("myanalyzerprocess")
process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(

'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_1.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_10.root',
#'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_100.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_101.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_102.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_104.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_105.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_106.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_107.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_108.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_109.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_11.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_110.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_111.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_112.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_113.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_114.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_115.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_116.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_117.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_118.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_119.root',
#'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_12.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_120.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_121.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_122.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_123.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_124.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_125.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_126.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_127.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_128.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_129.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_13.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_131.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_132.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_133.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_134.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_135.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_136.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_137.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_138.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_14.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_140.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_141.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_142.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_145.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_146.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_147.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_148.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_149.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_15.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_150.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_151.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_153.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_154.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_155.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_157.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_158.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_159.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_16.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_160.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_161.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_162.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_163.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_164.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_165.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_166.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_167.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_169.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_17.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_170.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_171.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_172.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_173.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_175.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_176.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_177.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_178.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_179.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_18.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_180.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_181.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_182.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_183.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_184.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_185.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_186.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_187.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_188.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_189.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_19.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_190.root',
#'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_191.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_192.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_193.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_194.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_195.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_196.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_197.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_198.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_2.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_20.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_200.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_22.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_23.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_24.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_25.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_26.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_27.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_28.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_29.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_3.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_30.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_31.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_33.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_34.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_35.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_36.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_38.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_39.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_4.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_40.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_41.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_42.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_43.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_44.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_45.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_46.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_47.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_48.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_49.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_5.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_50.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_51.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_52.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_53.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_54.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_55.root',
#'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_56.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_57.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_58.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_59.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_6.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_60.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_61.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_62.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_63.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_64.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_65.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_66.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_67.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_68.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_7.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_70.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_71.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_72.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_73.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_74.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_75.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_76.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_77.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_78.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_79.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_8.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_80.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_81.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_82.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_83.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_84.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_85.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_86.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_87.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_88.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_89.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_9.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_90.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_91.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_92.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_93.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_94.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_96.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_97.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_98.root',
'rfio:/castor/cern.ch/user/g/govoni/LowEnergyTest/bis/6TeV_WW/CMSSWfile_99.root'


 
)
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service ("TFileService", 
    fileName = cms.string ("Test_6TeV_WW.root")
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.MessageLogger = cms.Service("MessageLogger")

process.myanalyzer = cms.EDAnalyzer("VBF_BK_Analyzer",
    eleIDInputTag = cms.InputTag("eidTight"),
    GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
    muInputTag = cms.InputTag("muons"),
    jetTagsInputTag = cms.InputTag("iterativeCone5CaloJets"),
    metInputTag = cms.InputTag("met"),
   
    CJV_cut = cms.double(1.),
    MET_cut = cms.double(80.),
    pTLMax_cut = cms.double(56.),
    pTLMin_cut = cms.double(20.),
    mll_cut = cms.double(60.),
    DPhi_cut = cms.double(2.),
    jetDeltaEta = cms.double(1.),
    jetPtMin = cms.double(15.), #GeV
    maxJetsNum = cms.int32(1),  
    jetEtaMax = cms.double(2.5)

#     double CJV_cut = 1.
#     double m_MET_cut 1.
#     double m_pTLMax_cut 1.
#     double m_pTLMin_cut = 1.
#     double m_mll_cut = 1.
#     double m_DPhi_cut = 1.
#     double m_jetDeltaEta = 1.
#     double m_jetPtMin = 1.
#     int m_maxJetsNum = 1    
)


from HiggsAnalysis.PhantomTest.VBFJetCleaner_cfi import jetCleaner as somethingElse
process.filtro = somethingElse

from HiggsAnalysis.HiggsToWW2Leptons.HWWMetCorrector_cfi import muonCorrectedMET as METcorrector
process.correttore = METcorrector


# process.reading = cms.Path(process.myanalyzer)
process.reading = cms.Path(process.correttore * process.filtro * process.myanalyzer)

import FWCore.ParameterSet.Config as cms

process = cms.Process("Validation")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr = cms.untracked.PSet(threshold = cms.untracked.string("ERROR"))
# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

# initialize magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(

    #/MinimumBias/CMSSW_4_2_8-GR_R_42_V14_RelVal_run2010B-v1/RECO
    #'/store/relval/CMSSW_4_2_8/MinimumBias/RECO/GR_R_42_V14_RelVal_run2010B-v1/0024/E6B86936-BFBA-E011-BE6E-001A92810A98.root'

    #/MinimumBias/CMSSW_4_4_0_pre5-GR_R_44_V1_RelVal_run2010B-v1/RECO
    #'/store/relval/CMSSW_4_4_0_pre5/MinimumBias/RECO/GR_R_44_V1_RelVal_run2010B-v1/0018/565D438F-5DB2-E011-9970-00261894393F.root'

    #/MinimumBias/CMSSW_4_4_0_pre6-GR_R_44_V1_RelVal_run2010B-v1/RECO    
    #'/store/relval/CMSSW_4_4_0_pre6/MinimumBias/RECO/GR_R_44_V1_RelVal_run2010B-v1/0025/7AAC29CD-30BB-E011-B1B8-003048678F06.root'


    #/MinimumBias/CMSSW_4_4_0_pre7-GR_R_44_V1_RelVal_run2010B-v1/RECO    
    #'/store/relval/CMSSW_4_4_0_pre7/MinimumBias/RECO/GR_R_44_V1_RelVal_run2010B-v1/0143/9A6F4BE5-2FBD-E011-B38E-0018F3D09708.root'

    #/MinimumBias/CMSSW_4_4_0_pre8-GR_R_44_V2_RelVal_run2010B-v1/RECO
    #'/store/relval/CMSSW_4_4_0_pre8/MinimumBias/RECO/GR_R_44_V2_RelVal_run2010B-v1/0029/08D49032-6BC7-E011-8035-00261894395A.root'

    #/MinimumBias/CMSSW_4_4_0_pre9-GR_R_44_V4_RelVal_run2010B-v1/RECO
    #'/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_run2010B-v1/0173/86143081-AED2-E011-B4A3-001A92810A9A.root'

    #/MinimumBias/CMSSW_4_4_0_pre9-GR_R_44_V4_RelVal_mb2011A-v1/RECO
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/FC5E98B1-91D2-E011-8634-001A9281171E.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/F2C1CFB2-91D2-E011-A0BC-001A928116F2.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/F279650A-92D2-E011-A46D-001A928116C6.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/F230EDB2-91D2-E011-8E8E-001A92971B90.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/E8E72CB2-91D2-E011-97A0-001A92810AD4.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/DC97668E-91D2-E011-A587-0026189438D5.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/D8F6E1B7-91D2-E011-B9E1-0018F3D0960A.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/D28EBC91-91D2-E011-98BD-002618943864.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/CC32BFAF-91D2-E011-BB3A-0018F3D096A4.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/BE32DF89-91D2-E011-94D3-0026189437ED.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/BCA773B4-91D2-E011-B526-001A92971B54.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/AC8B1A8A-91D2-E011-86E5-0026189438B0.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/AA8D7B83-91D2-E011-957C-003048678B44.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/A2245DAE-91D2-E011-B011-001A92971B08.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/A0C31C84-91D2-E011-9A15-0030486791DC.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/8EB2D6B0-91D2-E011-8237-001A92971ACE.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/7C8404FF-91D2-E011-9F59-00304867C0F6.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/5695FE8D-91D2-E011-B374-002618FDA259.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/324F8D8C-91D2-E011-B237-002618FDA248.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/2879F284-91D2-E011-86E1-002618943932.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/1EECA9AF-91D2-E011-A251-001BFCDBD19E.root",
"/store/relval/CMSSW_4_4_0_pre9/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/080CBE82-91D2-E011-AAD0-003048679030.root"

#/MinimumBias/CMSSW_4_4_0_pre10-GR_R_44_V4_RelVal_mb2011A-v1/RECO
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/F8F7EF33-F0DC-E011-BE85-002618943958.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/EC0AB030-F0DC-E011-AE3D-0026189438E4.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/E04CA230-F0DC-E011-9EF2-0030486792AC.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/CA8C6232-F0DC-E011-BD9D-00261894396A.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/CA190D37-F0DC-E011-B32D-00261894386F.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/C4EF4137-F0DC-E011-B8D1-0026189438D2.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/AA6ABC38-F0DC-E011-A54B-002618943865.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/A0010734-F0DC-E011-AF0E-0026189438BD.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/8A5D2237-F0DC-E011-95F9-0026189438AE.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/78102331-F0DC-E011-9BF5-0026189438A9.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/74C9BA34-F0DC-E011-A7C4-002618943907.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/62401732-F0DC-E011-BF9D-003048679162.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/50085737-F0DC-E011-BCC1-00248C55CC7F.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/46676A38-F0DC-E011-8BB7-002618943961.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/42F3FD30-F0DC-E011-8ABF-0026189438DC.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/424EDF35-F0DC-E011-B4E3-00261894388D.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/3857E535-F0DC-E011-81F1-0026189438F6.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/2A251F32-F0DC-E011-917E-002618943858.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/1417F836-F0DC-E011-B848-002618943983.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/0E675733-F0DC-E011-AA92-00304867BED8.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/08FC5E38-F0DC-E011-804F-0026189438B3.root",
#"/store/relval/CMSSW_4_4_0_pre10/MinimumBias/RECO/GR_R_44_V4_RelVal_mb2011A-v1/0000/023D8F36-F0DC-E011-8777-0018F3D09702.root"

)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


# filter on PhysDeclared bit
process.skimming = cms.EDFilter("PhysDecl",
applyfilter = cms.untracked.bool(True),
debugOn = cms.untracked.bool(False),
HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
)

# filter on bit 40 || 41 nad !(bit36 || bit37 || bit38 || bit39)
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# filter on MinBias HLT
process.load('HLTrigger/HLTfilters/hltHighLevel_cfi')
process.hltHighLevel = cms.EDFilter("HLTHighLevel",
 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
 HLTPaths = cms.vstring('HLT_MinBiasBSC'),
 #HLTPaths = cms.vstring('HLT_MinBias*'),
 eventSetupPathsKey = cms.string(''),
 andOr = cms.bool(True),
 throw = cms.bool(True)
) 

#Good Vertex Filter (see GOODCOLL skim)
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 3 && abs(z) <= 24 && position.Rho <= 2"), 
  filter = cms.bool(True)   
)

# FilterOutScraping
process.noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)


process.load("Validation.EcalValidation.ecalvalidation_cfi")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('EcalValidation.root')
)

process.p = cms.Path(
    #process.skimming*
    #process.hltLevel1GTSeed*
    #process.hltHighLevel*
    process.noscraping*
    process.primaryVertexFilter*
    process.ecalvalidation
    )


import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.selections = cms.PSet(
   nEvents =  cms.untracked.int32(-1),

   MCpresent = cms.untracked.bool(True),
   MCpho = cms.untracked.bool(False),
   MCpair = cms.untracked.bool(False),
   MCphoEne = cms.untracked.bool(False),
   MCphoTheta = cms.untracked.bool(False),
    
   nEnergyBins = cms.untracked.int32(12),

   nEtaBins = cms.untracked.int32(10),  #larghezza fissa per eta
   etaMin = cms.untracked.double(-1.5),
   etaMax = cms.untracked.double(1.5),

   reclustering = cms.untracked.bool(False),  #FEDE     ##taglio sui vicini dopo la riclusterizzaz
   singleSeedThr = cms.untracked.double(0.23),   ##PF: 0.23 in barrel; 0.80 in endcap
   doubleSeedThr = cms.untracked.double(0.23),   #entrambe a 40 MeV
   secondThr = cms.untracked.double(0.08), ##PF: 0.08 in barrel; 0.30 in endcap

   checkComplete3x3 = cms.untracked.bool(False),      ##chiedo che ci siano almeno 8 xtals intorno al seed accesi (3x3 completa per fissare il noise)       

   evaluateEneSpectrum = cms.untracked.bool(True),
   eneSpectrum = cms.string("conversionTreeFiltered_MinBias_TuneD6T_7TeV_12bins.root"),
   fixedBinning = cms.untracked.bool(False),
   forcedEneBins = cms.vdouble(0.,0.3515,0.4175,0.4845,0.5515,0.6255,0.7115,0.8175,0.9545,1.1475,1.4485,1.996),

   outFile = cms.string("conversionTreeFiltered_MinBias_TuneD6T_7TeV_match_20bins_3x3.root"),
        )


process.inputNtuples = cms.PSet(
            inputFiles = cms.vstring(

    #'/media/amassiro/deguio/Dalitz/MinBias_MC_7TeV_Spring10-START3X_V26A_356ReReco-v1_vertexPosition/conversionTreeFiltered_MC_7TeV_v26A.root'
    #'/media/amassiro/deguio/Dalitz/conversionTreeFiltered_MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2.root'
    #'/media/amassiro/deguio/Dalitz/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2_OK/conversionTreeFiltered_MC_TuneD6T_7TeV_ALL.root'

    #'/gwtera/cms/users/deguio/Commissioning10-GOODCOLL-May27thSkim_v5/conversionTreeFiltered_GOODCOLL-May27thSkim_v5_42_1.root',
    #'/gwtera/cms/users/deguio/Commissioning10-GOODCOLL-May27thSkim_v5/conversionTreeFiltered_GOODCOLL-May27thSkim_v5_179_1.root'

    '/tmp/deguio/MinBias_TuneD6T_7TeV-pythia6_Spring10-START3X_V26B-v2_3x3_refit.root'

    )
)

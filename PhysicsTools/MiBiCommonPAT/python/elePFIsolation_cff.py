import FWCore.ParameterSet.Config as cms

from CommonTools.ParticleFlow.Isolation.tools_cfi import *


#Now prepare the iso deposits
elePFIsoDepositCharged=isoDepositReplace('pfIsolatedElectronsPFlow','pfAllChargedHadrons')
elePFIsoDepositChargedAll=isoDepositReplace('pfIsolatedElectronsPFlow','pfAllChargedCandidates')
elePFIsoDepositNeutral=isoDepositReplace('pfIsolatedElectronsPFlow','pfAllNeutralHadrons')
elePFIsoDepositGamma=isoDepositReplace('pfIsolatedElectronsPFlow','pfAllPhotons')
elePFIsoDepositPU=isoDepositReplace('pfIsolatedElectronsPFlow','pfPUChargedCandidates')



#Now create isolation values for those isolation deposits 

elePFIsoValueCharged03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositCharged"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('0.0001','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
            )
     )
)

elePFIsoValueChargedAll03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositChargedAll"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('0.0001','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
     )
   )
)

elePFIsoValueGamma03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositGamma"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
      )
   )
)

elePFIsoValueNeutral03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositNeutral"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
    )
 )
)

elePFIsoValuePU03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositPU"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
      )
   )
)



elePFIsoValueCharged04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositCharged"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('0.0001','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
            )
     )
)




elePFIsoValueChargedAll04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositChargedAll"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('0.0001','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
     )
   )
)

elePFIsoValueGamma04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositGamma"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
      )
   )
)


elePFIsoValueNeutral04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositNeutral"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
    )
 )

)
elePFIsoValuePU04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
            cms.PSet(
            src = cms.InputTag("elePFIsoDepositPU"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
      )
   )
)


elePFIsolationSequence =  cms.Sequence(elePFIsoDepositCharged+
                                        elePFIsoDepositChargedAll+
                                        elePFIsoDepositGamma+
                                        elePFIsoDepositNeutral+
                                        elePFIsoDepositPU+
                                        ##############################
                                        #elePFIsoValueCharged03+
                                        #elePFIsoValueChargedAll03+
                                        #elePFIsoValueGamma03+
                                        #elePFIsoValueNeutral03+
                                        #elePFIsoValuePU03+
                                       ############################## 
                                        elePFIsoValueCharged04+
                                        elePFIsoValueChargedAll04+
                                        elePFIsoValueGamma04+
                                        elePFIsoValueNeutral04+
                                        elePFIsoValuePU04

)                                         





                 


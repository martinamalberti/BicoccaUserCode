// system include files
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "Validation/AlCaTest/interface/AlCaL1Efficiencies.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsProducer.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GtLogicParser.h"

//DS momentum
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/EcalRecHitRecalib.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include <math.h>
#include "TFile.h"
#include <Math/VectorUtil.h>


using namespace reco ;
using namespace edm ;
using namespace std ;


// ----------------------------------------------------------------


AlCaL1Efficiencies::AlCaL1Efficiencies (const edm::ParameterSet& iConfig) :
  m_L1GtReadoutRecordTag (iConfig.getParameter<edm::InputTag> ("L1GtRecordInputTag")),
  m_outputFileName (iConfig.getUntrackedParameter<std::string>
                      ("HistOutFile",std::string ("AlCaL1Efficiencies.root"))) 
{
}


// ----------------------------------------------------------------


void 
AlCaL1Efficiencies::beginJob ()
{
  return ;
}


// ----------------------------------------------------------------


void 
AlCaL1Efficiencies::endJob ()
{      
  TFile output (m_outputFileName.c_str (),"recreate") ;
  output.Close () ;
  return ;
}


// ----------------------------------------------------------------


void 
AlCaL1Efficiencies::analyze (const edm::Event& iEvent, 
                            const edm::EventSetup& iSetup)
{
  edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord ;
  iEvent.getByLabel (m_L1GtReadoutRecordTag, gtReadoutRecord) ;
  const L1GlobalTriggerReadoutRecord* gtReadoutRecordPtr =
          gtReadoutRecord.product () ;

  if (!gtReadoutRecord.isValid ()) {
      edm::LogWarning ("AlCaL1Efficiencies")
              << "\nWarning: L1GlobalTriggerReadoutRecord with input tag "
              << m_L1GtReadoutRecordTag
              << "\nrequested in configuration, but not found in the event."
              << std::endl ;

      return ;
    }

  std::cout << gtReadoutRecord->decision () << std::endl ;
  gtReadoutRecord->decisionWord () ;
   
}



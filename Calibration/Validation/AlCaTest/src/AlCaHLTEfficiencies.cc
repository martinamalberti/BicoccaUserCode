// system include files
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "Validation/AlCaTest/interface/AlCaHLTEfficiencies.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsProducer.h"
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

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"

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


AlCaHLTEfficiencies::AlCaHLTEfficiencies (const edm::ParameterSet& iConfig) :
  m_HLTResultsTag (iConfig.getParameter<edm::InputTag> ("HLTResultsTag")),
  m_outputFileName (iConfig.getUntrackedParameter<std::string>
                      ("HistOutFile",std::string ("AlCaHLTEfficiencies.root"))) 
{
}


// ----------------------------------------------------------------


void 
AlCaHLTEfficiencies::beginJob ()
{
  return ;
}


// ----------------------------------------------------------------


void 
AlCaHLTEfficiencies::endJob ()
{      
  TFile output (m_outputFileName.c_str (),"recreate") ;
  output.Close () ;
  return ;
}


// ----------------------------------------------------------------


void 
AlCaHLTEfficiencies::analyze (const edm::Event& iEvent, 
                            const edm::EventSetup& iSetup)
{
  Handle<TriggerResults> HLTHandle ;
  iEvent.getByLabel (m_HLTResultsTag, HLTHandle) ;
  if (!HLTHandle.isValid ()) return ;

  const edm::TriggerNames & triggerNames = iEvent.triggerNames (*HLTHandle) ;
  //PG this is used to see whether the trigger settings are changed
  const ParameterSetID & HLTHandle->parameterSetID () ;

  return ; 
}



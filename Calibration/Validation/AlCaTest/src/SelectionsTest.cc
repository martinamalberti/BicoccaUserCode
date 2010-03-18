// system include files
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "Validation/AlCaTest/interface/SelectionsTest.h"
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
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

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


// ----------------------------------------------------------------


SelectionsTest::SelectionsTest (const edm::ParameterSet& iConfig) :
  m_ElectronLabel (iConfig.getParameter<edm::InputTag> ("electronLabel")),
  m_outputFileName (iConfig.getUntrackedParameter<std::string>
                      ("HistOutFile",std::string ("SelectionsTest.root"))) 
{}


// ----------------------------------------------------------------


void 
SelectionsTest::beginJob ()
{

 return ;
}


// ----------------------------------------------------------------


void 
SelectionsTest::endJob ()
{      
  TFile outputFile (m_outputFileName.c_str (), "recreate") ;
//  edm::LogWarning ("loop") << "PIETRO endjob" ;
  outputFile.Close () ;
  return ;
}


// ----------------------------------------------------------------


void 
SelectionsTest::analyze (const edm::Event& iEvent, 
                            const edm::EventSetup& iSetup)
{
  edm::Handle<reco::GsfElectronCollection> pElectrons ;
  iEvent.getByLabel (m_ElectronLabel, pElectrons) ;
//  try {
//       iEvent.getByLabel (m_ElectronLabel, pElectrons) ;
//      }
//  catch (...) {
//       edm::LogError ("reading") << m_ElectronLabel << "NotFound" ;
//       return ;
//     }
  
  //PG loop on the electrons
  for (reco::GsfElectronCollection::const_iterator eleIt = pElectrons->begin () ;
       eleIt != pElectrons->end () ;
       ++eleIt) 
    {

    } //PG loop over electrons
}



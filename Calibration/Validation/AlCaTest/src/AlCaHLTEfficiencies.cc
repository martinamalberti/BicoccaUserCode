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
#include "FWCore/Common/interface/TriggerNames.h"

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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


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
                      ("HistOutFile",std::string ("AlCaHLTEfficiencies.root"))),
  m_HLTPSetID () 
{
 edm::Service<TFileService> fs ;
 mytree_  = fs->make <TTree>("myTree","myTree");
}


// ----------------------------------------------------------------


void 
AlCaHLTEfficiencies::beginJob ()
{
 mytree_->Branch("HLT_wasrun",HLT_wasrun,"HLT_wasrun[300]/I");
 mytree_->Branch("HLT_accept",HLT_accept,"HLT_accept[300]/I");
 mytree_->Branch("HLT_error",HLT_error,"HLT_error[300]/I");
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
  const ParameterSetID & HLTPSetID = HLTHandle->parameterSetID () ;

  std::vector<std::string> names = triggerNames.triggerNames () ;
  // decision for each HL algorithm
//  const unsigned int HLTNum (triggerNames.triggerNames ().size ()) ;
  const unsigned int HLTNum = std::min(static_cast<int>(300),static_cast<int>(HLTHandle->size())) ;
  for (unsigned int iHLT = 0 ; iHLT < HLTNum ; ++iHLT) 
  {
    if (HLTHandle->wasrun (iHLT)) {
    HLT_wasrun[iHLT] = 1;
    }
    else {
    HLT_wasrun[iHLT] = 0;
    } // ++hlWasRun_[iHLT] ;
    if (HLTHandle->accept (iHLT)) {
    HLT_accept[iHLT] = 1;
    }
    else {
    HLT_accept[iHLT] = 0;
    } // ++hlAccept_[iHLT] ;
    if (HLTHandle->error (iHLT) ) {
    HLT_error[iHLT] = 1;
    }
    else {
    HLT_error[iHLT] = 0;
    } // ++hlErrors_[iHLT] ;
  }


 mytree_->Fill();
  return ; 
}



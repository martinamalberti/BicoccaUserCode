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

#include <cmath>
#include <vector>

#include "TFile.h"
#include "TGraph.h"
#include <Math/VectorUtil.h>


using namespace reco ;
using namespace edm ;


// ----------------------------------------------------------------


SelectionsTest::SelectionsTest (const edm::ParameterSet& iConfig) :
  m_ElectronLabel (iConfig.getParameter<edm::InputTag> ("electronLabel")),
  m_outputFileName (iConfig.getUntrackedParameter<std::string>
                      ("HistOutFile",std::string ("SelectionsTest.root"))),
  m_PinMPoutOPinMin (iConfig.getParameter<double> ("PinMPoutOPinMin")),
  m_PinMPoutOPinMax (iConfig.getParameter<double> ("PinMPoutOPinMax")),
  m_ESeedOPoutMin   (iConfig.getParameter<double> ("ESeedOPoutMin")),
  m_ESeedOPoutMax   (iConfig.getParameter<double> ("ESeedOPoutMax")),
  m_ESCOPinMin      (iConfig.getParameter<double> ("ESCOPinMin")),
  m_ESCOPinMax      (iConfig.getParameter<double> ("ESCOPinMax")),
  m_EMPoutMin       (iConfig.getParameter<double> ("EMPoutMin")),
  m_EMPoutMax       (iConfig.getParameter<double> ("EMPoutMax")),
  m_counter (std::vector<int> (0., 4)),
  m_h1_poMpiOpi     ("m_h1_poMpiOpi", "m_h1_poMpiOpi", 100, 0., 1.2) , 
  m_h1_poMpiOpi_sel ("m_h1_poMpiOpi_sel", "m_h1_poMpiOpi_sel", 100, 0., 1.2) ,  
  m_EseedOPout      ("m_EseedOPout", "m_EseedOPout", 100, 0., 2.) , 
  m_EseedOPout_sel  ("m_EseedOPout_sel", "m_EseedOPout_sel", 100, 0., 2.) , 
  m_EoPin           ("m_EoPin", "m_EoPin", 100, 0., 2.) , 
  m_EoPin_sel       ("m_EoPin_sel", "m_EoPin_sel", 100, 0., 2.) , 
  m_EoPout          ("m_EoPout", "m_EoPout", 100, 0., 2.) , 
  m_EoPout_sel      ("m_EoPout_sel", "m_EoPout_sel", 100, 0., 2.)  
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
  TGraph trend ;
  trend.SetPoint (0, 1, m_counter.at (0)) ;
  trend.SetPoint (1, 2, m_counter.at (1)) ;
  trend.SetPoint (2, 3, m_counter.at (2)) ;
  trend.SetPoint (4, 4, m_counter.at (3)) ;

  TFile outputFile (m_outputFileName.c_str (), "recreate") ;
  trend.Write () ;
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
      double pin = eleIt->trackMomentumAtVtx ().R () ;
      double poMpiOpi = (pin - eleIt->trackMomentumOut ().R ()) / pin ;
      double ESC = eleIt->energy () ;     
      double pOut = eleIt->trackMomentumOut().R();
      double EseedOPout = eleIt->eSeedClusterOverPout () ;
      double EoPin = eleIt->eSuperClusterOverP () ;
      double EoPout = (ESC)/pOut;
      if (poMpiOpi   > m_PinMPoutOPinMin && poMpiOpi   < m_PinMPoutOPinMax)   // min < fbrem < max
        {
          ++m_counter.at (0) ;
        } 
      if (EseedOPout > m_ESeedOPoutMin   && EseedOPout < m_ESeedOPoutMax  )   // min < Eseed/pout < max
        {
          ++m_counter.at (1) ;
        } 
      if (EoPin      > m_ESCOPinMin      && EoPin      < m_ESCOPinMax     )   // min < ESC/pin < max
        {
          ++m_counter.at (2) ;
        } 
  	  if (EoPout     > m_EMPoutMin       && EoPout     < m_EMPoutMax      )   // min < ESC/pout < max
        {
          ++m_counter.at (3) ;
        } 

    } //PG loop over electrons
}



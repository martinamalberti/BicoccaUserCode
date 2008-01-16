// $Id: VBFEleIDMeter.cc,v 1.4 2008/01/15 20:47:17 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleIDMeter.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

VBFEleIDMeter::VBFEleIDMeter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_eleIDPTDRLooseInputTag (iConfig.getParameter<edm::InputTag> ("eleIDPTDRLooseInputTag")) ,
  m_eleIDPTDRMediumInputTag (iConfig.getParameter<edm::InputTag> ("eleIDPTDRMediumInputTag")) ,
  m_eleIDPTDRTightInputTag (iConfig.getParameter<edm::InputTag> ("eleIDPTDRTightInputTag")) ,
  m_eleIDOTHERRobustInputTag (iConfig.getParameter<edm::InputTag> ("eleIDOTHERRobustInputTag")) ,
  m_eleIDOTHERMediumInputTag (iConfig.getParameter<edm::InputTag> ("eleIDOTHERMediumInputTag")) ,
  m_eleIDOTHERTightInputTag (iConfig.getParameter<edm::InputTag> ("eleIDOTHERTightInputTag")) 
{}


// --------------------------------------------------------------------


VBFEleIDMeter::~VBFEleIDMeter ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFEleIDMeter::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{
  //PG get the GSF electrons collection
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  std::cout << "[VBFEleIDMeter][analyze] number of GSF electrons : "
            << GSFHandle->size () 
            << std::endl ;

  //PG get the electron ID collection
  edm::Handle<reco::ElectronIDAssociationCollection> eleIDPTDRLooseAssocHandle;
  iEvent.getByLabel (m_eleIDPTDRLooseInputTag, eleIDPTDRLooseAssocHandle);

  std::cout << "[VBFEleIDMeter][analyze] number of electron ID electrons : "
            << eleIDPTDRLooseAssocHandle->size () 
            << std::endl ;

}


// --------------------------------------------------------------------


void 
VBFEleIDMeter::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFEleIDMeter::endJob () 
{
}


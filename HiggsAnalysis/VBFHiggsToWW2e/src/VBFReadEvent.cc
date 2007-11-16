// $Id: VBFReadEvent.h,v 1.1 2007/11/16 09:07:08 govoni Exp $

#include "HiggsAnalysis/VBFReadEvent/interface/VBFReadEvent.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "CLHEP/HepMC/GenEvent.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"


VBFReadEvent::VBFReadEvent (const edm::ParameterSet& iConfig) :
      m_metColletion (iConfig.getParameter<edm::InputTag> ("metCollection")) ,
      m_genMetColletion (iConfig.getParameter<edm::InputTag> ("genMetCollection")) ,
      m_jetColletion (iConfig.getParameter<edm::InputTag> ("jetCollection")) ,
      m_genJetColletion (iConfig.getParameter<edm::InputTag> ("genJetCollection")) ,
      m_eleColletion (iConfig.getParameter<edm::InputTag> ("eleCollection")) ,
      m_MCtruthColletion (iConfig.getParameter<edm::InputTag> ("MCtruthCollection"))
      m_muColletion (iConfig.getParameter<edm::InputTag> ("muCollection"))
// il resto del MC
// il trigger
// gli elettroni, guarda il codice ftto con roberto

{
   //now do what ever initialization is needed

}


// --------------------------------------------------------------------


VBFReadEvent::~VBFReadEvent ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFReadEvent::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle<reco::CaloMETCollection> metCollectionHandle ;
  iEvent.getByLabel (m_metColletion, metCollectionHandle) ;

//PG FIXME quale dei due?
  edm::Handle<reco::CandidateCollection> genMetCollectionHandle ;
//  edm::Handle<reco::GenMETCollection> genMetCollectionHandle ;
  iEvent.getByLabel (m_genMetColletion, genMetCollectionHandle) ;

  edm::Handle<reco::CandidateCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetColletion, jetCollectionHandle) ;
 
  edm::Handle<reco::CandidateCollection> genJetCollectionHandle ;
  iEvent.getByLabel (m_genJetColletion, genJetCollectionHandle) ;
 
}


// --------------------------------------------------------------------


void 
VBFReadEvent::beginJob (const edm::EventSetup&)
{}


// --------------------------------------------------------------------


void 
VBFReadEvent::endJob () 
{}


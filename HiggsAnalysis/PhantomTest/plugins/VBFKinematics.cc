// $Id: VBFKinematics.cc,v 1.1 2008/12/19 14:08:41 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/PhantomTest/plugins/VBFKinematics.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

VBFKinematics::VBFKinematics (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag")) ,
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_metInputTag (iConfig.getParameter<edm::InputTag> ("metInputTag")) 
{
  edm::Service<TFileService> fileService ;
  m_jet_eta = fileService->make<TH1F> ("m_jet_eta","m_jet_eta",10,-3,3) ;
  m_ele_eta = fileService->make<TH1F> ("m_ele_eta","m_ele_eta",10,-3,3) ;
  m_mu_eta = fileService->make<TH1F> ("m_mu_eta","m_mu_eta",10,-3,3) ;

}


// --------------------------------------------------------------------


VBFKinematics::~VBFKinematics ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFKinematics::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{
  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;
  for (int index = 0; index < jetTagsHandle->size () ; ++index)
    {
      m_jet_eta->Fill ((*jetTagsHandle)[index].p4().eta ()) ;
    }

  //PG get the GSF electrons collection
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag, GSFHandle) ; 
  for (int index = 0; index < GSFHandle->size () ; ++index)
    {
      m_ele_eta->Fill ((*GSFHandle)[index].p4().eta ()) ;
    }

  //PG get the electron ID collection
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle ;
  iEvent.getByLabel (m_electronIDInputTag, electronIDAssocHandle) ;

  //VT get the Global muons collection
  edm::Handle<reco::MuonCollection> MuonHandle ;
  iEvent.getByLabel (m_muInputTag, MuonHandle) ; 
  for (int index = 0; index < MuonHandle->size () ; ++index)
    {
      m_mu_eta->Fill ((*MuonHandle)[index].p4().eta ()) ;
    }
    
  //PG get the calo MET
  edm::Handle<reco::CaloMETCollection> metCollectionHandle ;
  iEvent.getByLabel (m_metInputTag, metCollectionHandle) ;
  const CaloMETCollection *calometcol = metCollectionHandle.product () ;
  const CaloMET *calomet = &(calometcol->front ()) ;   
}


// --------------------------------------------------------------------


void 
VBFKinematics::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFKinematics::endJob () 
{
}


// $Id: VBFKinematics.cc,v 1.1 2008/12/19 13:31:47 govoni Exp $
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
  TH1F * h_pt = fileService->make<TH1F> ("test","test",10 ,0. ,10) ;
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

  std::cerr << "[VBFKinematics][analyze] number of jet Tags : "
            << jetTagsHandle->size () 
            << std::endl ;

  //PG get the GSF electrons collection
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  std::cerr << "[VBFKinematics][analyze] number of GSF electrons : "
            << GSFHandle->size () 
            << std::endl ;

  //PG get the electron ID collection
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle;
  iEvent.getByLabel (m_electronIDInputTag, electronIDAssocHandle);

  std::cerr << "[VBFKinematics][analyze] number of electron ID electrons : "
            << electronIDAssocHandle->size () 
            << std::endl ;

  //VT get the Global muons collection
  edm::Handle<reco::MuonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 
    
  std::cerr << "[VBFKinematics][analyze] number of global muons : "
            << MuonHandle->size () 
            << std::endl ;

  std::cerr << "[VBFKinematics][analyze] number of leptons : "
            << MuonHandle->size () + GSFHandle->size ()
            << std::endl ;

  //PG get the calo MET
  edm::Handle<reco::CaloMETCollection> metCollectionHandle;
  iEvent.getByLabel (m_metInputTag, metCollectionHandle);
  const CaloMETCollection *calometcol = metCollectionHandle.product () ;
  const CaloMET *calomet = &(calometcol->front ()) ;  
 
  std::cerr << "[VBFKinematics][analyze] number of met : "
            << metCollectionHandle->size () 
            << std::endl ;
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


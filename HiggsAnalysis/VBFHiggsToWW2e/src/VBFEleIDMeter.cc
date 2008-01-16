// $Id: VBFEleIDMeter.cc,v 1.4 2008/01/15 20:47:17 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleIDMeter.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include <TLorentzVector.h>


VBFEleIDMeter::VBFEleIDMeter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")) ,
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

  //PG MC thruth candidates collection  
  edm::Handle<CandidateCollection> genParticles;
  iEvent.getByLabel(m_MCtruthInputTag, genParticles);

  int MCeleNumber = 0 ;
  std::vector<TLorentzVector> MCelectrons ; 

  //PG loop over generated particles
  for (CandidateCollection::const_iterator p = genParticles->begin(); 
       p != genParticles->end(); ++ p) 
    {
        int mumPDG = p->pdgId();
        int mumSTATUS = p->status();
        if (abs (mumPDG) == 24 &&  mumSTATUS == 3) //W±
            {
                for ( size_t i = 0; i < p->numberOfDaughters () ; ++ i ) 
                    {
                        const Candidate * daughter = p->daughter ( i ) ;
                        int PDG = daughter -> pdgId () ;    
                        if (PDG==11) 
                          {
                            TLorentzVector dummy ;
                            setMomentum (dummy, *daughter) ;
                            MCelectrons.push_back (dummy) ;
                            ++MCeleNumber ;
                          }
                    } //PG loop over daughters
            } //PG if W
        
    } //PG loop over generated particles





  //PG get the GSF electrons collection
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  std::cout << "[VBFEleIDMeter][analyze] number of GSF electrons : "
            << GSFHandle->size () 
            << std::endl ;

  //PG get the electron ID collections
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
  edm::Service<TFileService> fs ;

  m_deltaR_PTDRLoose     = fs->make<TH1F> ("m_deltaR_PTDRLoose","m_deltaR_PTDRLoose",100,0,2) ;
  m_deltaR_PTDRMedium    = fs->make<TH1F> ("m_deltaR_PTDRMedium","m_deltaR_PTDRMedium",100,0,2) ;
  m_deltaR_PTDRTight     = fs->make<TH1F> ("m_deltaR_PTDRTight","m_deltaR_PTDRTight",100,0,2) ;
  m_deltaR_OTHERRobust   = fs->make<TH1F> ("m_deltaR_OTHERRobust","m_deltaR_OTHERRobust",100,0,2) ;
  m_deltaR_OTHERMedium   = fs->make<TH1F> ("m_deltaR_OTHERMedium","m_deltaR_OTHERMedium",100,0,2) ;
  m_deltaR_OTHERTight    = fs->make<TH1F> ("m_deltaR_OTHERTight","m_deltaR_OTHERTight",100,0,2) ;
  
  m_effVSPt_PTDRLoose    = fs->make<TH1F> ("m_effVSPt_PTDRLoose","m_effVSPt_PTDRLoose",50,0,500) ;
  m_effVSPt_PTDRMedium   = fs->make<TH1F> ("m_effVSPt_PTDRMedium","m_effVSPt_PTDRMedium",50,0,500) ;
  m_effVSPt_PTDRTight    = fs->make<TH1F> ("m_effVSPt_PTDRTight","m_effVSPt_PTDRTight",50,0,500) ;
  m_effVSPt_OTHERRobust  = fs->make<TH1F> ("m_effVSPt_OTHERRobust","m_effVSPt_OTHERRobust",50,0,500) ;
  m_effVSPt_OTHERMedium  = fs->make<TH1F> ("m_effVSPt_OTHERMedium","m_effVSPt_OTHERMedium",50,0,500) ;
  m_effVSPt_OTHERTight   = fs->make<TH1F> ("m_effVSPt_OTHERTight","m_effVSPt_OTHERTight",50,0,500) ;     

  m_effVSEta_PTDRLoose   = fs->make<TH1F> ("m_effVSEta_PTDRLoose","m_effVSEta_PTDRLoose",170,0,3) ;
  m_effVSEta_PTDRMedium  = fs->make<TH1F> ("m_effVSEta_PTDRMedium","m_effVSEta_PTDRMedium",170,0,3) ;
  m_effVSEta_PTDRTight   = fs->make<TH1F> ("m_effVSEta_PTDRTight","m_effVSEta_PTDRTight",170,0,3) ;
  m_effVSEta_OTHERRobust = fs->make<TH1F> ("m_effVSEta_OTHERRobust","m_effVSEta_OTHERRobust",170,0,3) ; 
  m_effVSEta_OTHERMedium = fs->make<TH1F> ("m_effVSEta_OTHERMedium","m_effVSEta_OTHERMedium",170,0,3) ; 
  m_effVSEta_OTHERTight  = fs->make<TH1F> ("m_effVSEta_OTHERTight","m_effVSEta_OTHERTight",170,0,3) ;

// fs->make<TH2F> ("m_barrelGlobalCrystalsMap","m_barrelGlobalCrystalsMap",170,-85,85,360,0,360) ;
}


// --------------------------------------------------------------------


void 
VBFEleIDMeter::endJob () 
{
}


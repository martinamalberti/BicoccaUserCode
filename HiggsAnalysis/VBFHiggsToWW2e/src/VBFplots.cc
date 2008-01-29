// $Id: VBFTrivialAnalysis.cc,v 1.4 2008/01/15 20:47:17 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFplots.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
//for TH1F
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

VBFplots::VBFplots (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag")) ,
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_metInputTag (iConfig.getParameter<edm::InputTag> ("metInputTag")) 
{}


// --------------------------------------------------------------------


VBFplots::~VBFplots ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFplots::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{
  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;

  std::cout << "[VBFplots][analyze] number of jet Tags : "
            << jetTagsHandle->size () 
            << std::endl ;

  m_deltaEta -> Fill (fabs ((*jetTagsHandle)[0].p4 ().Eta () - (*jetTagsHandle)[1].p4 ().Eta ()) ) ;
  m_sumEta -> Fill (((*jetTagsHandle)[0].p4 ().Eta () + (*jetTagsHandle)[1].p4 ().Eta ())) ;
}


// --------------------------------------------------------------------


void 
VBFplots::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_deltaEta = fs->make<TH1F> ("m_deltaEta","m_deltaEta",100,0,10) ;
  m_sumEta = fs->make<TH1F> ("m_sumEta","m_sumEta",100,-12,12) ;
}


// --------------------------------------------------------------------


void 
VBFplots::endJob () 
{
}


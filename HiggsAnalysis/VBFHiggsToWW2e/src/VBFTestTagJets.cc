// $Id: VBFTestTagJets.cc,v 1.4 2008/01/15 20:47:17 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFTestTagJets.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

VBFTestTagJets::VBFTestTagJets (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag")) ,
  m_MCjetTagsInputTag (iConfig.getParameter<edm::InputTag> ("MCjetTagsInputTag"))
{}


// --------------------------------------------------------------------


VBFTestTagJets::~VBFTestTagJets ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFTestTagJets::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{
  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;

  std::cout << "[VBFTestTagJets][analyze] number of jet Tags : "
            << jetTagsHandle->size () 
            << std::endl ;

  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> MCjetTagsHandle ;
  iEvent.getByLabel (m_MCjetTagsInputTag, MCjetTagsHandle) ;

  std::cout << "[VBFTestTagJets][analyze] number of jet Tags : "
            << MCjetTagsHandle->size () 
            << std::endl ;

}


// --------------------------------------------------------------------


void 
VBFTestTagJets::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFTestTagJets::endJob () 
{
}


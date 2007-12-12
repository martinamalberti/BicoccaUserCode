// $Id: VBFTrivialAnalysis.cc,v 1.41 2007/12/12 09:35:56 tancini Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFTrivialAnalysis.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"

VBFTrivialAnalysis::VBFTrivialAnalysis (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag"))
{}


// --------------------------------------------------------------------


VBFTrivialAnalysis::~VBFTrivialAnalysis ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFTrivialAnalysis::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Get the tag jets
  edm::Handle<LorentzVectorCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;

  std::cout << "[VBFTrivialAnalysis][analyze] number of jet Tags : "
            << jetTagsHandle->size () 
            << std::endl ;
 
}


// --------------------------------------------------------------------


void 
VBFTrivialAnalysis::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFTrivialAnalysis::endJob () 
{
}


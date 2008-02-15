// $Id: VBFJetCleanerPlots.cc,v 1.1 2008/02/15 15:49:00 tancini Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetCleanerPlots.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include <Math/VectorUtil.h>

VBFJetCleanerPlots::VBFJetCleanerPlots (const edm::ParameterSet& iConfig) :
      m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
      m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) 
{}


// --------------------------------------------------------------------


VBFJetCleanerPlots::~VBFJetCleanerPlots ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


// --------------------------------------------------------------------


void
VBFJetCleanerPlots::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle);
 
  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  //PG loop over jets
  for (collection::const_iterator jetIt = jetCollectionHandle->begin () ;
       jetIt != jetCollectionHandle->end () ;
       ++jetIt)
    {
       for (electronCollection::const_iterator eleIt = GSFHandle->begin () ;
           eleIt != GSFHandle->end () ;
           ++eleIt)
        {
          double deltaR = ROOT::Math::VectorUtil::DeltaR (eleIt->momentum (),jetIt->momentum ()) ;
	  double HEoverEmE = eleIt->hadronicOverEm();
	  double EleOJetEratio = eleIt->caloEnergy() / jetIt->energy () ;  

	  m_deltaR -> Fill (deltaR) ;
	  m_HEoverEmE -> Fill (HEoverEmE) ;
	  m_EleOJetEratio -> Fill (EleOJetEratio) ;
	}
    }

  
   
}
// --------------------------------------------------------------------


void 
VBFJetCleanerPlots::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_deltaR = fs->make<TH1F> ("m_deltaR", "dR between jet and ele", 100, 0, 10);
  m_HEoverEmE = fs->make<TH1F> ("m_HEoverEmE", "H/E", 100, -0.2,0.2) ;
  m_EleOJetEratio = fs->make<TH1F> ("m_EleOJetEratio", "ratio between ele and jet energies", 100, 0, 50);

}


// --------------------------------------------------------------------


void 
VBFJetCleanerPlots::endJob () 
{
}


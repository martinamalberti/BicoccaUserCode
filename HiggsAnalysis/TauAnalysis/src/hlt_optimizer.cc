#include <HiggsAnalysis/TauAnalysis/interface/hlt_optimizer.h>

#include "DataFormats/Common/interface/RefToBase.h"
//#include "FWCore/Framework/interface/Event.h"

//#include "DataFormats/Common/interface/Handle.h"
//#include "DataFormats/Common/interface/RefToBase.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"



#include "HLTrigger/Muon/interface/HLTMuonL1Filter.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h" // FIXME find what to include to have reco::

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"


//
//#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
//#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
//#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"





hlt_optimizer::hlt_optimizer(const edm::ParameterSet& iConfig)
{
}


hlt_optimizer::~hlt_optimizer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void hlt_optimizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	edm::Handle<reco::HLTFilterObjectWithRefs> mucands;
	iEvent.getByLabel("hltMuonLevel1Seed", mucands);
	std::cout << "--> Going to go through " << mucands->size() << " candidates..." << std::endl;
	for (unsigned int i=0; i<mucands->size(); i++) {
		edm::RefToBase<reco::Candidate> cand = mucands->getParticleRef(i);
		if (typeid(*cand)!=typeid(l1extra::L1MuonParticle)) continue;
		l1extra::L1MuonParticleRef muon = cand.castTo<l1extra::L1MuonParticleRef>();
		if (muon.isNull()) continue;
		std::cout << "-->  mu_eta = " << muon->eta() << "   mu_pt = " << muon->pt() << std::endl;
	}
}


// ------------ method called once each job just before starting event loop  ------------
void hlt_optimizer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void hlt_optimizer::endJob() {
}

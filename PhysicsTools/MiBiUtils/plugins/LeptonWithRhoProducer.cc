#include "PhysicsTools/MiBiUtils/plugins/LeptonWithRhoProducer.h"

using namespace edm ;
using namespace std ;
using namespace reco ;


template<class object >
LeptonWithRhoProducer<object>::LeptonWithRhoProducer (const edm::ParameterSet & iConfig) :
  leptonTag_ (iConfig.getParameter<edm::InputTag>("leptonTag")) ,
  rhoTag_    (iConfig.getParameter<edm::InputTag> ("rhoTag")) 
{
  produces<vector<object> > () ;  
//  produces< edm::RefToBaseVector<object> >() ; //PG fixme che diff c'e' fra qsto e l'altro?
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


template<class object>
LeptonWithRhoProducer<object>::~LeptonWithRhoProducer () {}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


template<class object>
void 
LeptonWithRhoProducer<object>::produce (edm::Event & iEvent, const edm::EventSetup & iSetup)
{

  Handle<vector<object> > leptonsHandle ;
  iEvent.getByLabel(leptonTag_, leptonsHandle) ;
  //const obejct * leptons = leptonsHandle.product () ;

  edm::Handle<double> rhoForJetsPFlow ;
//  iEvent.getByLabel ("kt6PFJetsChsPFlow", "rho", rhoForJetsPFlow) ;
  iEvent.getByLabel (rhoTag_, rhoForJetsPFlow) ;

  double rho = *rhoForJetsPFlow.product () ;

  auto_ptr<vector<object> > outputCollection ( new vector<object> () ) ;

  for (unsigned int i = 0; i < leptonsHandle->size () ; ++i)
    {
      object aLepton (leptonsHandle->at (i)) ;
      aLepton.addUserFloat ("rho", rho) ;
      outputCollection->push_back (aLepton) ;
    }

  iEvent.put (outputCollection) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
typedef LeptonWithRhoProducer< pat::Muon > MuonWithRhoProducer ;
DEFINE_FWK_MODULE (MuonWithRhoProducer) ;

#include "DataFormats/PatCandidates/interface/Electron.h"
typedef LeptonWithRhoProducer< pat::Electron > ElectronWithRhoProducer ;
DEFINE_FWK_MODULE (ElectronWithRhoProducer) ;







/**
add the PU rho to pat:Electron or pat::Muon collection, to be usable in the EleID filter
*/

#ifndef LeptonWithRhoProducer_h
#define LeptonWithRhoProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
//#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

//#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
//#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
//#include "DataFormats/PatCandidates/interface/Isolation.h"


template<class object>
class LeptonWithRhoProducer : public edm::EDProducer
{
 public: 

  explicit LeptonWithRhoProducer (const edm::ParameterSet&) ;
  virtual ~LeptonWithRhoProducer () ;

 private:

  virtual void produce (edm::Event & iEvent, const edm::EventSetup & iSetup) ;

  edm::InputTag leptonTag_ ;
  edm::InputTag rhoTag_ ;

} ;


#endif

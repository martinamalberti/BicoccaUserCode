// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDProducer.h"
 
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//
// class declaration
//

class NoMuonTrackProducer : public edm::EDProducer 
{
public:
  explicit NoMuonTrackProducer(const edm::ParameterSet&);
  ~NoMuonTrackProducer();
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------
  
  double ptMin_;
};


//
// constructors and destructor
//
NoMuonTrackProducer::NoMuonTrackProducer(const edm::ParameterSet& iConfig)
{
  ptMin_ = iConfig.getUntrackedParameter<double>("ptMin", 8.0);
  produces< reco::TrackCollection >();
}


NoMuonTrackProducer::~NoMuonTrackProducer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
void
NoMuonTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  std::cout << " Entering NoMuonTrackProducer " << std::endl;

  edm::Handle<reco::MuonCollection> muH;
  iEvent.getByLabel("muons", muH); //FIXME --> passa da fuori
  
  edm::Handle<reco::TrackCollection> tkH;
  iEvent.getByLabel("generalTracks",   tkH); //FIXME --> passa da fuori

  std::auto_ptr<reco::TrackCollection> MuonLess(new reco::TrackCollection);
  
  unsigned int muIndex = -1;
  //  unsigned int counter = 0;
  for( unsigned int trk = 0; trk < tkH->size(); ++trk )
    {
      for ( unsigned int mu = 0; mu < muH->size(); ++mu )
	{
	  reco::Muon muon = muH->at(mu);
	  
	  reco::TrackRef tkRef_tk(tkH, trk); 
	  reco::TrackRef tkRef_mu;
	  if( muon.isTrackerMuon() )
	    tkRef_mu = muon.innerTrack();
	  else 
	    continue;  
	  
	  if(muon.pt() < ptMin_){continue;}
	  
	  //exact matching betweek muon track and pv track.
	  if (tkRef_tk == tkRef_mu) muIndex = trk;
	  
	}
      if (trk != muIndex ){ MuonLess->push_back( (*tkH)[trk]);}
      
    }
  
  iEvent.put(MuonLess);  
}

// ------------ method called once each job just before starting event loop  ------------
void 
NoMuonTrackProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NoMuonTrackProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(NoMuonTrackProducer);

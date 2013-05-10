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

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

//
// class declaration
//

class NoLeptonTrackProducer : public edm::EDProducer 
{
public:
  explicit NoLeptonTrackProducer(const edm::ParameterSet&);
  ~NoLeptonTrackProducer();
  
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
NoLeptonTrackProducer::NoLeptonTrackProducer(const edm::ParameterSet& iConfig)
{
  ptMin_ = iConfig.getUntrackedParameter<double>("ptMin", 8.0);
  produces< reco::TrackCollection >();
}


NoLeptonTrackProducer::~NoLeptonTrackProducer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
void
NoLeptonTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  std::cout << " Entering NoLeptonTrackProducer " << std::endl;

  edm::Handle<reco::MuonCollection> muH;
  iEvent.getByLabel("muons", muH); //FIXME --> passa da fuori
  
  edm::Handle<reco::GsfElectronCollection> eleH;
  iEvent.getByLabel("gsfElectrons", eleH); //FIXME --> passa da fuori  

  edm::Handle<reco::TrackCollection> tkH;
  iEvent.getByLabel("generalTracks",   tkH); //FIXME --> passa da fuori

  std::auto_ptr<reco::TrackCollection> LeptonLess(new reco::TrackCollection);
  
  unsigned int muIndex = -1;
  unsigned int eleIndex = -1;
  unsigned int counter = 0;
  for( unsigned int trk = 0; trk < tkH->size(); ++trk )
    {
      reco::TrackRef tkRef_tk(tkH, trk); 

      // check if the track is a muon track
      for ( unsigned int mu = 0; mu < muH->size(); ++mu )
	{
	  reco::Muon muon = muH->at(mu);
	  reco::TrackRef tkRef_mu;
	  if( muon.isTrackerMuon() )
	    tkRef_mu = muon.innerTrack();
	  else 
	    continue;  
	  
	  if(muon.pt() < ptMin_){continue;}
	  
	  //exact matching betweek muon track and pv track.
	  if (tkRef_tk == tkRef_mu) muIndex = trk;
	}

      // check if the track is a muon track
      for ( unsigned int ele = 0; ele < eleH->size(); ++ele )
	{
	  reco::GsfElectron electron = eleH->at(ele);
	  reco::TrackRef tkRef_ele = electron.closestCtfTrackRef();
	  
	  if(electron.pt() < ptMin_){continue;}
	  
	  //matching betweek electron track and pv track.                                                                                                                                                                    
	  if (tkRef_tk == tkRef_ele) eleIndex = trk;
	}

      if (trk != muIndex && trk != eleIndex) { 
	LeptonLess->push_back( (*tkH)[trk]);
      }
      
    }// end loop over tracks
  
  iEvent.put(LeptonLess);  
}

// ------------ method called once each job just before starting event loop  ------------
void 
NoLeptonTrackProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NoLeptonTrackProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(NoLeptonTrackProducer);

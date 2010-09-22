#include "HiggsAnalysis/littleH/interface/Onia2MuMuPAT.h"

//Headers for the data items
#include <DataFormats/TrackReco/interface/TrackFwd.h>
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/Common/interface/View.h>
#include <DataFormats/HepMCCandidate/interface/GenParticle.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/VertexReco/interface/VertexFwd.h>

//Headers for services and tools
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "TMath.h"
#include "Math/VectorUtil.h"
#include "TVector3.h"
#include "HiggsAnalysis/littleH/interface/VertexReProducer.h"


Onia2MuMuPAT::Onia2MuMuPAT(const edm::ParameterSet& iConfig):
  muons_(iConfig.getParameter<edm::InputTag>("muons")),
  higherPuritySelection_(iConfig.getParameter<std::string>("higherPuritySelection")),
  lowerPuritySelection_(iConfig.getParameter<std::string>("lowerPuritySelection")),
  dimuonSelection_(iConfig.existsAs<std::string>("dimuonSelection") ? iConfig.getParameter<std::string>("dimuonSelection") : ""),
  addCommonVertex_(iConfig.getParameter<bool>("addCommonVertex")),
  addMCTruth_(iConfig.getParameter<bool>("addMCTruth"))
{  
 produces<pat::CompositeCandidateCollection>();  
}


Onia2MuMuPAT::~Onia2MuMuPAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
  Onia2MuMuPAT::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
 using namespace edm;
 using namespace std;
 using namespace reco;
 typedef Candidate::LorentzVector LorentzVector;

 std::auto_ptr<pat::CompositeCandidateCollection> oniaOutput(new pat::CompositeCandidateCollection);

 Handle< View<pat::Muon> > muons;
 iEvent.getByLabel(muons_,muons);

 edm::ESHandle<TransientTrackBuilder> theTTBuilder;
 iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);
 KalmanVertexFitter vtxFitter;

  // onia candidates only from muons
 for(View<pat::Muon>::const_iterator it = muons->begin(), itend = muons->end(); it != itend; ++it){
    // both must pass low quality
  if(!lowerPuritySelection_(*it)) continue; 
  for(View<pat::Muon>::const_iterator it2 = it+1; it2 != itend;++it2){
      // both must pass low quality
   if(!lowerPuritySelection_(*it2)) continue; 
      // one must pass tight quality
   if (!(higherPuritySelection_(*it) || higherPuritySelection_(*it2))) continue;

   pat::CompositeCandidate myCand;
   vector<TransientVertex> pvs;

      // ---- no explicit order defined ----
   myCand.addDaughter(*it, "muon1");
   myCand.addDaughter(*it2,"muon2");	

      // ---- define and set candidate's 4momentum  ----  
   LorentzVector onia = it->p4() + it2->p4();
   myCand.setP4(onia);
   myCand.setCharge(it->charge()+it2->charge());

      // ---- apply the dimuon cut ----
   if(!dimuonSelection_(myCand)) continue;

      // ---- fit vertex using Tracker tracks (if they have tracks) ----
   if (it->track().isNonnull() && it2->track().isNonnull()) {
	
    vector<TransientTrack> t_tks;
    t_tks.push_back(theTTBuilder->build(*it->track()));  // pass the reco::Track, not  the reco::TrackRef (which can be transient)
    t_tks.push_back(theTTBuilder->build(*it2->track())); // otherwise the vertex will have transient refs inside.
    TransientVertex myVertex = vtxFitter.vertex(t_tks);
    if (myVertex.isValid()) {
     float vChi2 = myVertex.totalChiSquared();
     float vNDF  = myVertex.degreesOfFreedom();
     float vProb(TMath::Prob(vChi2,(int)vNDF));
	  
     myCand.addUserFloat("vNChi2",vChi2/vNDF);
     myCand.addUserFloat("vProb",vProb);
	   	  
     if (addCommonVertex_) {
      myCand.addUserData("commonVertex",Vertex(myVertex));
     }
    } else {
     myCand.addUserFloat("vNChi2",-1);
     myCand.addUserFloat("vProb", -1);
     if (addCommonVertex_) {
      myCand.addUserData("commonVertex",Vertex());
     }

    }
	
   }
 
      // ---- MC Truth, if enabled ----
   if (addMCTruth_) {
	
    reco::GenParticleRef genMu1 = it->genParticleRef();
    reco::GenParticleRef genMu2 = it2->genParticleRef();
    if (genMu1.isNonnull() && genMu2.isNonnull()) {
     reco::GenParticleRef mom1 = genMu1->motherRef();
     reco::GenParticleRef mom2 = genMu2->motherRef();

     //search for the real mother, i.e. avoid taking as mother the same particle 
     //in a different status (egs. a lepton before/after bremsstrahlung)
     while ( genMu1->pdgId() == mom1->pdgId() ) mom1 = mom1->motherRef();
     while ( genMu2->pdgId() == mom2->pdgId() ) mom2 = mom2->motherRef();

     if (mom1.isNonnull() && (mom1 == mom2)) {
      myCand.setGenParticleRef(mom1); // set
      myCand.embedGenParticle();      // and embed

      myCand.addUserInt("momPDGId",mom1->pdgId());

     } else {
      myCand.addUserInt("momPDGId",0);
     }
    } else {
     myCand.addUserInt("momPDGId",0);
    }
   }

      // ---- Push back output ----  
   oniaOutput->push_back(myCand);
  }
 }

 std::sort(oniaOutput->begin(),oniaOutput->end(),pTComparator_);

 iEvent.put(oniaOutput);

}

// ------------ method called once each job just before starting event loop  ------------
void 
  Onia2MuMuPAT::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
  Onia2MuMuPAT::endJob() {
  }

//define this as a plug-in
  DEFINE_FWK_MODULE(Onia2MuMuPAT);

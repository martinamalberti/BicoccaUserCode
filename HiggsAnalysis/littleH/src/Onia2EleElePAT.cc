#include "HiggsAnalysis/littleH/interface/Onia2EleElePAT.h"

//Headers for the data items
#include <DataFormats/TrackReco/interface/TrackFwd.h>
#include <DataFormats/TrackReco/interface/Track.h>

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include <DataFormats/Common/interface/View.h>

#include <DataFormats/HepMCCandidate/interface/GenParticle.h>
#include <DataFormats/PatCandidates/interface/Electron.h>
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


Onia2EleElePAT::Onia2EleElePAT(const edm::ParameterSet& iConfig):
  electrons_(iConfig.getParameter<edm::InputTag>("electrons")),
  higherPuritySelection_(iConfig.getParameter<std::string>("higherPuritySelection")),
  lowerPuritySelection_(iConfig.getParameter<std::string>("lowerPuritySelection")),
  dieleSelection_(iConfig.existsAs<std::string>("dieleSelection") ? iConfig.getParameter<std::string>("dieleSelection") : ""),
  addCommonVertex_(iConfig.getParameter<bool>("addCommonVertex")),
  addMCTruth_(iConfig.getParameter<bool>("addMCTruth"))
{  
    produces<pat::CompositeCandidateCollection>();  
}


Onia2EleElePAT::~Onia2EleElePAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
Onia2EleElePAT::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  using namespace edm;
  using namespace std;
  using namespace reco;
  typedef Candidate::LorentzVector LorentzVector;

  std::auto_ptr<pat::CompositeCandidateCollection> oniaOutput(new pat::CompositeCandidateCollection);

  Handle< View<pat::Electron> > electrons;
  iEvent.getByLabel(electrons_,electrons);

  edm::ESHandle<TransientTrackBuilder> theTTBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);
  KalmanVertexFitter vtxFitter;

  // onia candidates only from electrons
  for(View<pat::Electron>::const_iterator it = electrons->begin(), itend = electrons->end(); it != itend; ++it){
    // both must pass low quality
    if(!lowerPuritySelection_(*it)) continue; 
    for(View<pat::Electron>::const_iterator it2 = it+1; it2 != itend;++it2){
      // both must pass low quality
      if(!lowerPuritySelection_(*it2)) continue; 
      // one must pass tight quality
      if (!(higherPuritySelection_(*it) || higherPuritySelection_(*it2))) continue;

      pat::CompositeCandidate myCand;
      vector<TransientVertex> pvs;

      // ---- no explicit order defined ----
      myCand.addDaughter(*it, "elec1");
      myCand.addDaughter(*it2,"elec2");	

      // ---- define and set candidate's 4momentum  ----  
      LorentzVector onia = it->p4() + it2->p4();
      myCand.setP4(onia);
      myCand.setCharge(it->charge()+it2->charge());

      // ---- apply the diele cut ----
      if(!dieleSelection_(myCand)) continue;

      // ---- fit vertex using Tracker tracks (if they have tracks) ----
      if (it->gsfTrack().isNonnull() && it2->gsfTrack().isNonnull()) {
	
	vector<TransientTrack> t_tks;
        t_tks.push_back(theTTBuilder->build(*it->gsfTrack()));  // pass the reco::Track, not  the reco::TrackRef (which can be transient)
	t_tks.push_back(theTTBuilder->build(*it2->gsfTrack())); // otherwise the vertex will have transient refs inside.
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
	
	reco::GenParticleRef genEle1 = it->genParticleRef();
	reco::GenParticleRef genEle2 = it2->genParticleRef();
	if (genEle1.isNonnull() && genEle2.isNonnull()) {
	  reco::GenParticleRef mom1 = genEle1->motherRef();
	  reco::GenParticleRef mom2 = genEle2->motherRef();
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
Onia2EleElePAT::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Onia2EleElePAT::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(Onia2EleElePAT);

#define DEBUG    0 // 0=false
#define C_DEBUG  0 // currently debuging

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMuonProducer.h"


using namespace std;
using namespace edm;
using namespace reco;
using namespace vbfhww2l;



VBFMuonProducer::VBFMuonProducer(const edm::ParameterSet& iConfig){

  produces<MuonCollection>("mergedMuons");
  produces<VBFParticleCollection>("muons");

  g_muon_producer = iConfig.getParameter<InputTag>("g_muonProducer");
  t_muon_producer = iConfig.getParameter<InputTag>("t_muonProducer");
//  m_muon_producer = iConfig.getParameter<InputTag>("m_muonProducer");
}


VBFMuonProducer::~VBFMuonProducer(){

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// 
// member functions
//

// ------------ method called to produce the data  ------------
void
VBFMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  //globalMuons --------------------------------------------
  Handle<MuonCollection> gmuHa;
  iEvent.getByLabel(g_muon_producer, gmuHa);
   

  //trackerMuons -------------------------------------------
  Handle<MuonCollection> tmuHa;
  iEvent.getByLabel(t_muon_producer, tmuHa);

   
  //mergedMuons --------------------------------------------
  //merging :/CMSSW/RecoMuon/MuonIdentification/plugins/MuonMerger.cc---
  auto_ptr<MuonCollection> mergedMuons(new MuonCollection);
  // global muons first
  for(MuonCollection::const_iterator muon = gmuHa->begin();
      muon !=  gmuHa->end(); ++muon )
    mergedMuons->push_back(*muon);

         
  // tracker muons next
  for(MuonCollection::const_iterator trackerMuon = tmuHa->begin();
      trackerMuon !=  tmuHa->end(); ++trackerMuon ){
  
    // check if this muon is already in the list
    bool newMuon = true;
    for(MuonCollection::iterator muon = mergedMuons->begin();
 	muon !=  mergedMuons->end(); ++muon )
      if(muon->track().get() == trackerMuon->track().get() ) {
 	newMuon = false;
//    muon->setMatches( trackerMuon->getMatches() );
//    muon->setCalEnergy( trackerMuon->getCalEnergy() );
	break;
	}
    if(newMuon) mergedMuons->push_back( *trackerMuon );
  }
   
  //Handle<MuonCollection> mmuHa;
  //iEvent.getByLabel(m_muon_producer, mmuHa);
  auto_ptr<VBFParticleCollection> muons(new VBFParticleCollection);
  n_muons =0;
  
  for (MuonCollection::const_iterator mu = mergedMuons->begin(); 
       mu != mergedMuons->end(); 
       ++mu){

    
    
#if C_DEBUG
    cout << "muon pT = " << mu->pt() 
	 << "\t muon eta = " << mu->eta() 
	 << endl;
#endif
    
//      if(mu->pt() < muon_cut_pt 
//         || fabs(mu->eta()) > muon_cut_eta)
//        continue;
    
    Particle_* lep = new Particle_();
    lep->v4 = mu->p4();
    lep->vertex = mu->vertex();
    lep->pdgId = mu->pdgId();
    
    lep->AddIntProp("Charge", mu->charge());
    lep->AddIntProp("trackerMuon", mu->isTrackerMuon());
    lep->AddIntProp("globalMuon", mu->isGlobalMuon());

    lep->AddDoubleProp("caloComp", mu->isCaloCompatibilityValid());
//    lep->AddDoubleProp("caloCompVal", mu->getCaloCompatibility());
//    lep->AddDoubleProp("EnDep", mu->isEnergyValid());
//    lep->AddDoubleProp("EnDep_em", mu->getCalEnergy().em);
//    lep->AddDoubleProp("EnDep_ems9", mu->getCalEnergy().emS9);
//    lep->AddDoubleProp("EnDep_ho", mu->getCalEnergy().ho);
//    lep->AddDoubleProp("EnDep_hos9", mu->getCalEnergy().hoS9);
//    lep->AddDoubleProp("EnDep_had", mu->getCalEnergy().had);
//    lep->AddDoubleProp("EnDep_hads9", mu->getCalEnergy().hadS9);
//      
//    lep->AddDoubleProp("i3_sumPt", mu->getIsolationR03().sumPt);
//    lep->AddDoubleProp("i3_emEt", mu->getIsolationR03().emEt);
//    lep->AddDoubleProp("i3_hadEt", mu->getIsolationR03().hadEt);
//    lep->AddDoubleProp("i3_hoEt", mu->getIsolationR03().hoEt);
//    lep->AddDoubleProp("i3_nTracks", mu->getIsolationR03().nTracks);
//    lep->AddDoubleProp("i3_nJets", mu->getIsolationR03().nJets);
//    lep->AddDoubleProp("i5_sumPt", mu->getIsolationR05().sumPt);
//    lep->AddDoubleProp("i5_emEt", mu->getIsolationR05().emEt);
//    lep->AddDoubleProp("i5_hadEt", mu->getIsolationR05().hadEt);
//    lep->AddDoubleProp("i5_hoEt", mu->getIsolationR05().hoEt);
//    lep->AddDoubleProp("i5_nTracks", mu->getIsolationR05().nTracks);
//    lep->AddDoubleProp("i5_nJets", mu->getIsolationR05().nJets);
    
    
    lep->AddDoubleProp("numberOfChambers", mu->numberOfChambers());
    lep->AddDoubleProp("numberOfMatches0",
 		       mu->numberOfMatches(mu->NoArbitration));
    lep->AddDoubleProp("numberOfMatches1", 
 		       mu->numberOfMatches(mu->SegmentArbitration));
    lep->AddDoubleProp("numberOfMatches2", 
 		       mu->numberOfMatches(mu->SegmentAndTrackArbitration));

    //tracker-track
    lep->AddDoubleProp("trk_chi2", mu->track()->chi2());
    lep->AddDoubleProp("trk_ndof", mu->track()->ndof());
    lep->AddDoubleProp("trk_d0", mu->track()->d0());
    lep->AddDoubleProp("trk_dz", mu->track()->dz());
    lep->AddDoubleProp("trk_qoverp", mu->track()->qoverp());

    lep->AddIntProp("trk_charge", mu->track()->charge());
    lep->AddIntProp("trk_nHits", mu->track()->recHitsSize());
    lep->AddIntProp("trk_lostHits", mu->track()->numberOfLostHits());
    lep->AddIntProp("trk_validHits", mu->track()->numberOfValidHits());
    lep->AddIntProp("trk_lost", mu->track()->lost());

    //standalone-muon-track
    if(mu->isStandAloneMuon()){

      lep->AddDoubleProp("std_chi2", mu->standAloneMuon()->chi2());
      lep->AddDoubleProp("std_ndof", mu->standAloneMuon()->ndof());
      lep->AddDoubleProp("std_d0", mu->standAloneMuon()->d0());
      lep->AddDoubleProp("std_dz", mu->standAloneMuon()->dz());
      lep->AddDoubleProp("std_qoverp", mu->standAloneMuon()->qoverp());

      lep->AddIntProp("std_charge", mu->standAloneMuon()->charge());
      lep->AddIntProp("std_nHits", mu->standAloneMuon()->recHitsSize());
      lep->AddIntProp("std_lostHits", mu->standAloneMuon()->numberOfLostHits());
      lep->AddIntProp("std_validHits", mu->standAloneMuon()->numberOfValidHits());
      lep->AddIntProp("std_lost", mu->standAloneMuon()->lost());
    }

    //combined-track
    if (mu->isGlobalMuon()){

      lep->AddDoubleProp("comb_chi2", mu->combinedMuon()->chi2());
      lep->AddDoubleProp("comb_ndof", mu->combinedMuon()->ndof());
      lep->AddDoubleProp("comb_d0", mu->combinedMuon()->d0());
      lep->AddDoubleProp("comb_dz", mu->combinedMuon()->dz());
      lep->AddDoubleProp("comb_qoverp", mu->combinedMuon()->qoverp());

      lep->AddIntProp("comb_charge", mu->combinedMuon()->charge());
      lep->AddIntProp("comb_nHits", mu->combinedMuon()->recHitsSize());
      lep->AddIntProp("comb_lostHits", mu->combinedMuon()->numberOfLostHits());
      lep->AddIntProp("comb_validHits", mu->combinedMuon()->numberOfValidHits());
      lep->AddIntProp("comb_lost", mu->combinedMuon()->lost());
    }

//    lep->AddDoubleProp("TMLastStationLoose",
//  		       muonid::isGoodMuon(*mu,
//  					  muonid::TMLastStationLoose));
//    lep->AddDoubleProp("TMLastStationTight",
//  		       muonid::isGoodMuon(*mu,
//  					  muonid::TMLastStationTight));
    
    n_muons++;

    muons->push_back(*lep);
    
#if C_DEBUG
    cout << "number of muons = " << n_muons << endl
	 << "pt = " << lep->v4.pt() << endl;
#endif
  }
  

#if DEBUG
  cout << "number of muons = " << n_muons << endl;
#endif

  
  iEvent.put(mergedMuons, "mergedMuons");
  iEvent.put(muons, "muons");
}

// ------------ method called once each job just before starting event loop  ------------
void VBFMuonProducer::beginJob(const edm::EventSetup&){
}

// ------------ method called once each job just after ending the event loop  ------------
void VBFMuonProducer::endJob(){
}


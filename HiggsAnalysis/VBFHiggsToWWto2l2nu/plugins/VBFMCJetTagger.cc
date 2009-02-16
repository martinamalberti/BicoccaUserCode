#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCJetTagger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <iostream>
#include <algorithm>

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

VBFMCJetTagger::VBFMCJetTagger (const edm::ParameterSet& iConfig) :
  m_genParticles (iConfig.getParameter<edm::InputTag> ("MCParticlesInputTag")) ,
  m_MCtagJetsName(iConfig.getParameter<std::string> ("MCtagJetsName")) 
{
  // produces<LorentzVectorCollection> (m_MCtagJetsName) ;
  produces<reco::RecoChargedCandidateCollection> (m_MCtagJetsName) ;
}  


// ----------------------------------------------------------------------------

  
VBFMCJetTagger::~VBFMCJetTagger ()
{
}


// ----------------------------------------------------------------------------

  
void 
VBFMCJetTagger::produce (edm::Event& iEvent, const edm::EventSetup& iEventSetup)
{
  
  using namespace reco;
  edm::Handle<GenParticleCollection> genParticles; 
  try {iEvent.getByLabel (m_genParticles,genParticles) ;}
  catch(...){std::cout<<"AAAAB genParticles handle is not valid!!!"<<std::endl;return;}
  if(!genParticles.isValid()){std::cout<<"AAAA genParticles handle is not valid!!!"<<std::endl;return;}
  LorentzVector MCjetTagF;
  LorentzVector MCjetTagB;
  int idF =0, idB=0;
  int chargeF=99,chargeB=99;
  bool foundDaughther = false;
 for (GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
    {
        int mumPDG = p->pdgId();
        int mumSTATUS = p->status();

        ///////////////////////////////////////////////// tag quark /////////////////////////////////////////////////
        //misteriosamente i tag sono i fratelli dell'higgs
        //quindi parto dall'higgs e ne prendo le mamme e quindi riguardo i figli
	
        if ((abs(mumPDG)==25) && (mumSTATUS ==3))
            {
	      if(p->numberOfMothers() < 1){std::cout<<"AAAAA nMoth < 1 !!!! "<<std::endl;continue;}
	      const Candidate * interact0 = p->mother(0);
	      if(interact0->numberOfDaughters() < 2){std::cout<<"AAAAA nDaugh < 2 !!!! "<<std::endl;continue;}
	      foundDaughther = true;
	      if ((interact0->daughter(1)->eta()) > (interact0->daughter(0)->eta())) {
		setMomentum (MCjetTagF, *(interact0->daughter(1)));
		setMomentum (MCjetTagB, *(interact0->daughter(0)));
		chargeF=interact0->daughter(1)->charge();
		chargeB=interact0->daughter(0)->charge();
		idF=interact0->daughter(1)->pdgId();
		idB=interact0->daughter(0)->pdgId();

	      }
	      else {
		setMomentum (MCjetTagB, *(interact0->daughter(1)));
		setMomentum (MCjetTagF, *(interact0->daughter(0)));
		chargeB=interact0->daughter(1)->charge();
		chargeF=interact0->daughter(0)->charge();
		idB=interact0->daughter(1)->pdgId();
		idF=interact0->daughter(0)->pdgId();
	      }
	    }
    }

 
  //PG create and fill the collection to be added to the event
 //std::auto_ptr<LorentzVectorCollection> MCtagJets (new LorentzVectorCollection) ;
 std::auto_ptr<reco::RecoChargedCandidateCollection> MCtagJets (new reco::RecoChargedCandidateCollection) ;

 if(foundDaughther){
   // FIX-ME aggiungere la pId nelle versioni successive
   Particle::Point vtx( 0, 0, 0 );
   reco::RecoChargedCandidate FTagCandidate (chargeF,MCjetTagF,vtx,idF) ;
   reco::RecoChargedCandidate BTagCandidate (chargeB,MCjetTagB,vtx,idB) ;

   MCtagJets->push_back (FTagCandidate) ;  
   MCtagJets->push_back (BTagCandidate) ;  
 }
  //PG insert the collection into the event
  iEvent.put (MCtagJets, m_MCtagJetsName) ;
 
}

void VBFMCJetTagger::setMomentum (LorentzVector & myvector, const reco::GenParticle & gen)
{
    myvector.SetPx (gen.px());
    myvector.SetPy (gen.py());
    myvector.SetPz (gen.pz());
    myvector.SetE (gen.energy());
}

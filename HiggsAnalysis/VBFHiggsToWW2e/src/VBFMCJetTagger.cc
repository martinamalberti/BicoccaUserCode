#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCJetTagger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include <iostream>
#include <algorithm>

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

VBFMCJetTagger::VBFMCJetTagger (const edm::ParameterSet& iConfig) :
  m_genParticles (iConfig.getParameter<edm::InputTag> ("MCParticlesInputTag")) ,
  m_MCtagJetsName(iConfig.getParameter<std::string> ("MCtagJetsName")) 
{
  produces<LorentzVectorCollection> (m_MCtagJetsName) ;
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
  edm::Handle<CandidateCollection> genParticles; 
  iEvent.getByLabel (m_genParticles,genParticles) ;

  LorentzVector MCjetTagF;
  LorentzVector MCjetTagB;

 for (CandidateCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
    {
        int mumPDG = p->pdgId();
        int mumSTATUS = p->status();

        ///////////////////////////////////////////////// tag quark /////////////////////////////////////////////////
        //misteriosamente i tag sono i fratelli dell'higgs
        //quindi parto dall'higgs e ne prendo le mamme e quindi riguardo i figli

        if ((abs(mumPDG)==25) && (mumSTATUS ==3))
            {
	      const Candidate * interact0 = p->mother(0);
	      if ((interact0->daughter(1)->eta()) > (interact0->daughter(0)->eta())) {
		setMomentum (MCjetTagF, *(interact0->daughter(1)));
		setMomentum (MCjetTagB, *(interact0->daughter(0)));

	      }
	      else {
		setMomentum (MCjetTagB, *(interact0->daughter(1)));
		setMomentum (MCjetTagF, *(interact0->daughter(0)));
	      }
	    }
    }
  //PG create and fill the collection to be added to the event
  std::auto_ptr<LorentzVectorCollection> MCtagJets (new LorentzVectorCollection) ;
  
  MCtagJets->push_back (MCjetTagF) ;  
  MCtagJets->push_back (MCjetTagB) ;  
    
  //PG insert the collection into the event
  iEvent.put (MCtagJets, m_MCtagJetsName) ;
}

void VBFMCJetTagger::setMomentum (LorentzVector & myvector, const reco::Candidate & gen)
{
    myvector.SetPx (gen.px());
    myvector.SetPy (gen.py());
    myvector.SetPz (gen.pz());
    myvector.SetE (gen.energy());
}

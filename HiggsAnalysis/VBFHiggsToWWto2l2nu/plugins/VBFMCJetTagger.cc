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
             
          foundDaughther = true;
          if (((p-1)->eta()) > ((p-2)->eta())) {
           setMomentum (MCjetTagF, *((p-1)));
           setMomentum (MCjetTagB, *((p-2)));
           chargeF=(p-1)->charge();
           chargeB=(p-2)->charge();
           idF=(p-1)->pdgId();
           idB=(p-2)->pdgId();

          }
          else {
           setMomentum (MCjetTagB, *((p-1)));
           setMomentum (MCjetTagF, *((p-2)));
           chargeB=(p-1)->charge();
           chargeF=(p-2)->charge();
           idB=(p-1)->pdgId();
           idF=(p-2)->pdgId();
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

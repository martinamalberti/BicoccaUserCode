// $Id: VBFMCChannelFilter.cc,v 1.1 2009/02/03 13:50:43 abenagli Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCChannelFilter.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <iostream>


//! ctor
VBFMCChannelFilter::VBFMCChannelFilter (const edm::ParameterSet& iConfig) :
  m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")) ,
  m_channel (iConfig.getParameter<int> ("channel"))
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFMCChannelFilter::~VBFMCChannelFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFMCChannelFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG MC thruth candidates collection  
  edm::Handle<reco::GenParticleCollection> genParticles ;
  iEvent.getByLabel (m_MCtruthInputTag, genParticles) ;

  int numEle = 0 ; 
  int numMu = 0 ; //PG this is maybe useless

  
  //--------------------------
  //---- select VBF event ----  
  edm::Handle<edm::HepMCProduct> evtMC;
  iEvent.getByLabel("source", evtMC);
  
  const HepMC::GenEvent * mcEv = evtMC->GetEvent();

  if(mcEv->signal_process_id() != 123 && mcEv->signal_process_id() != 124) return false;
  //---- end select VBF event ----  
  //------------------------------  
  
  
  //PG loop over generated particles
  for (reco::GenParticleCollection::const_iterator particle = genParticles->begin () ; 
       particle != genParticles->end () ; 
       ++particle) 
    {
        int mumPDG = particle->pdgId () ;
        int mumSTATUS = particle->status () ;
        if (abs (mumPDG) == 24 && mumSTATUS == 3) //W+-
          {
            for (int i = 0; i < particle->numberOfDaughters () ; ++i) 
              {
                int PDG = particle->daughter (i)->pdgId () ;    
                if (abs (PDG) == 11) ++numEle ;
                else if (abs (PDG) == 13) ++numMu ;
              }
          }
     } //PG loop over generated particles

  if (m_channel == 1 && numMu == 2) return true ;                //PG double u
  if (m_channel == 2 && numEle == 2) return true ;               //PG double e
  if (m_channel == 3 && numMu == 1 && numEle == 1) return true ; //PG cross ue
  
  if (m_channel == 0 && (numMu == 2 || numEle == 2 || (numMu == 1 && numEle == 1))) return true ;                //AM whatever channel
  
  return false ;
}


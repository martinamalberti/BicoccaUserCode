#include "PhysicsTools/MiBiCommonPAT/interface/MCDumper.h"






// -----------------------------------------------------------------
// ctor
// -----------------------------------------------------------------

MCDumper::MCDumper(edm::Handle<reco::GenParticleCollection>& genParticles,
                   const bool& verbosity):
 verbosity_p(verbosity),
 isValid_p(true)
{
  if(verbosity_p)
    std::cerr << "*** MCDumper::ctor ***" << std::endl;
  
  Analyze(genParticles);
}






// -----------------------------------------------------------------
// dtor
// -----------------------------------------------------------------

MCDumper::~MCDumper()
{
  if(verbosity_p)
    std::cerr << "*** MCDumper::dtor ***" << std::endl;
}






// -----------------------------------------------------------------
// Analyze
// -----------------------------------------------------------------

void MCDumper::Analyze(edm::Handle<reco::GenParticleCollection>& genParticles)
{
  if(verbosity_p)
    std::cerr << "*** MCDumper::Analyze ***" << std::endl;
  
  
  
  // loop over gen particles
  int counter = 0;
  for(reco::GenParticleCollection::const_iterator p = genParticles -> begin();
      p != genParticles -> end(); ++p)
  {
    const reco::Candidate* pCurrent = &(*p);
    const reco::Candidate* pMother = 0;
    if(pCurrent -> mother())
      pMother = pCurrent -> mother();
    
    int pdgId  = p -> pdgId();
    int status = p -> status();
    int charge = p -> charge();
    int motherPdgId = 0;
    if(pCurrent -> mother())
      motherPdgId = pMother -> pdgId();
    
        
    // charged
    if( (abs(charge) != 0) && (status == 1) )
      mcCharged_p.push_back(pCurrent);

    // electrons
    if( (abs(pdgId) == 11) && (status == 1) )
      mcE_p.push_back(pCurrent);
    
    // muons
    if( (abs(pdgId) == 13) && (status == 1) )
      mcMu_p.push_back(pCurrent);
    
    // taus and tauJ
    if( (abs(pdgId) == 16) && (status == 1) && (abs(motherPdgId) == 15))
    {
      mcTau_p.push_back(pMother);
      
      std::vector<const reco::Candidate*> tauJBuffer;
      std::vector<const reco::Candidate*> nuBuffer;
      FindTauDecayParticles(tauJBuffer, nuBuffer, pMother, verbosity_p);
      
      mcTauJ_p.push_back(tauJBuffer);
    }
    
    ++counter;
    
  } // loop over gen particles
  
  
  
  return;
}

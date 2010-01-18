#include "HiggsAnalysis/TTBarAnalysis/interface/MCDumperWJets.h"


// -----------------------------------------------------------------
// ctor
// -----------------------------------------------------------------

MCDumperWJets::MCDumperWJets(edm::Handle<reco::GenParticleCollection>& genParticles,
                   const int& eventType,
                   const bool& verbosity):
 verbosity_p(verbosity),
 eventType_p(eventType),
 isValid_p(true),
 mcVDecayMode_p(""),
 mcV_p(0),
 mcF1_fromV_p(0),
 mcF2_fromV_p(0)
 {
  if(verbosity_p)
    std::cerr << "*** MCDumperWJets::ctor ***" << std::endl;
  
  Analyze(genParticles);
}






// -----------------------------------------------------------------
// dtor
// -----------------------------------------------------------------

MCDumperWJets::~MCDumperWJets()
{
  if(verbosity_p)
    std::cerr << "*** MCDumperWJets::dtor ***" << std::endl;
}






// -----------------------------------------------------------------
// Analyze
// -----------------------------------------------------------------

void MCDumperWJets::Analyze(edm::Handle<reco::GenParticleCollection>& genParticles)
{
  if(verbosity_p)
    std::cerr << "*** MCDumperWJets::Analyze ***" << std::endl;
  
  
  
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
    
//     // W
    if( (abs(pdgId) == 24) && (status == 3) ) //---- W
    {
     if (!mcV_p) mcV_p = pCurrent;
    }        
    ++counter;
  } // loop over gen particles
    

  // return if background
  if(eventType_p != 0) return;
 
  
  
  if(!mcV_p)
  {
   std::cerr << ">>> MCDumper::Analyze::Warning: no W in the event" << std::endl;    
   isValid_p = false;
   return;
  }

  
  //---- W decay ----
  //---- find fermions from vector boson decay
  std::vector<const reco::Candidate*> fFromVBuffer;
  FindDaughterParticles(&mcV_p, verbosity_p, 1, &fFromVBuffer);
  
  if(fFromVBuffer.size() != 2)
  {
     std::cerr << "MCDumperWJets::Analyze::Warning: not a V -> ff' decay" << std::endl;
     isValid_p = false;
     return;
  }
  
  mcF1_fromV_p = fFromVBuffer.at(0);
  mcF2_fromV_p = fFromVBuffer.at(1);
  
  // If leptons, see if there is a photon emission
  if(abs(mcF1_fromV_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF1_fromV_p, verbosity_p, 1);
  if(abs(mcF2_fromV_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF2_fromV_p, verbosity_p, 1);
  
  if(mcF2_fromV_p -> pt() > mcF1_fromV_p -> pt())
  {
    const reco::Candidate* buffer = mcF1_fromV_p;
    mcF1_fromV_p = mcF2_fromV_p;
    mcF2_fromV_p = buffer;
  }
  
  return;
}



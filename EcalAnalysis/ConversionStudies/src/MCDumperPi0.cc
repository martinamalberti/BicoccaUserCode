#include "EcalAnalysis/ConversionStudies/interface/MCDumperPi0.h"






// -----------------------------------------------------------------
// ctor
// -----------------------------------------------------------------

MCDumperPi0::MCDumperPi0(edm::Handle<reco::GenParticleCollection>& genParticles,
                             const int& eventType,
                             const bool& verbosity):
 verbosity_p(verbosity),
 eventType_p(eventType),
 isValid_p(true),
 mcVDecayMode_p(""),
 mcV_p(0),
 mcQ1_tag_p(0),
 mcQ2_tag_p(0),
 mcF1_fromV_p(0),
 mcF2_fromV_p(0)
{
  if(verbosity_p)
    std::cerr << "*** MCDumperPi0::ctor ***" << std::endl;
  
  Analyze(genParticles);
}






// -----------------------------------------------------------------
// dtor
// -----------------------------------------------------------------

MCDumperPi0::~MCDumperPi0()
{
  if(verbosity_p)
    std::cerr << "*** MCDumperPi0::dtor ***" << std::endl;
}






// -----------------------------------------------------------------
// Analyze
// -----------------------------------------------------------------

void MCDumperPi0::Analyze(edm::Handle<reco::GenParticleCollection>& genParticles)
{
  if(verbosity_p)
    std::cerr << "*** MCDumperPi0::Analyze ***" << std::endl;
  
  
  
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

    // gamma
    if( (abs(pdgId) == 22) && (status == 1) )
      mcGamma_p.push_back(pCurrent);
    
    // taus and tauJ
    if( (abs(pdgId) == 16) && (status == 1) && (abs(motherPdgId) == 15))
    {
      mcTau_p.push_back(pMother);
      
      std::vector<const reco::Candidate*> tauJBuffer;
      std::vector<const reco::Candidate*> nuBuffer;
      FindTauDecayParticles(tauJBuffer, nuBuffer, pMother, verbosity_p);
      
      mcTauJ_p.push_back(tauJBuffer);
    }
    
    // Pi0
    if( (pdgId == 111) && (status == 3) )
    {
      mcV_p = pCurrent;
    }
    
    // tag quarks
    if(counter == 6)
      mcQ1_tag_p = pCurrent;

    if(counter == 7)
      mcQ2_tag_p = pCurrent;

    //if(counter < 50)
    //if(abs(pdgId) == 13)
    //{
    //  std::cout << "n: " << counter;
    //  PrintParticleInfo(pCurrent);
    //}
    
    ++counter;
    
  } // loop over gen particles
  
  
  
  // return if background
  if(eventType_p != 0) return;
  
  
  
  
  
  
  // check if it is signal
  if(!mcV_p)
  {
    std::cerr << ">>> MCDumperPi0::Analyze::Warning: no Pi0 in the event" << std::endl;
    
    isValid_p = false;
    return;
  }
  
  //if(verbosity_p)
  //PrintDecayTree(mcV_p);
  // check if it is signal
  
  
  
  
  // find fermions from vector boson decay
  std::vector<const reco::Candidate*> fFromVBuffer;
  FindDaughterParticles(&mcV_p, verbosity_p, 1, &fFromVBuffer);
  
  if(fFromVBuffer.size() != 2)
  {
     std::cerr << "MCDumperPi0::Analyze::Warning: not a V -> ff decay" << std::endl;

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
  
  if(mcV_p -> pdgId() == 23)
    {
      if(mcF2_fromV_p -> pt() > mcF1_fromV_p -> pt())
	{
	  const reco::Candidate* buffer = mcF1_fromV_p;
	  mcF1_fromV_p = mcF2_fromV_p;
	  mcF2_fromV_p = buffer;
	}
    }
  else if(abs(mcV_p -> pdgId()) == 24)
    {
      if( (abs(mcF1_fromV_p -> pdgId()))%2 == 0)
	{
	  const reco::Candidate* buffer = mcF1_fromV_p;
	  mcF1_fromV_p = mcF2_fromV_p;
	  mcF2_fromV_p = buffer;
	}
    }
  
  
  GetVDecayMode();
  
  
  
  
  // Print event summary
  if(verbosity_p)
    PrintEventInfo();
  
  
  
  return;
}






// -----------------------------------------------------------------
// Print Event Info
// -----------------------------------------------------------------

void MCDumperPi0::PrintEventInfo()
{
  std::cout << "\n>>> MCDumperPi0::PrintZWInfo \n" << std::endl;
  
  
    
  std::cout << "Vector bosons:   V -> " << mcVDecayMode_p << std::endl;
  if(mcV_p)
    PrintParticleInfo(mcV_p);

  std::cout << std::endl;
  
  
  
  std::cout << "Fermions from vector bosons: " << std::endl;
  if(mcF1_fromV_p)
    PrintParticleInfo(mcF1_fromV_p);
  if(mcF2_fromV_p)
    PrintParticleInfo(mcF2_fromV_p);
  
  std::cout << std::endl;
}



// -----------------------------------------------------------------
// Get Vector Boson decay mode
// -----------------------------------------------------------------

void MCDumperPi0::GetVDecayMode()
{
  std::vector<const reco::Candidate*> buffer;
  std::string stringBuffer;

  buffer.push_back(mcF1_fromV_p);
  buffer.push_back(mcF2_fromV_p);

  // sort: first e/mu/tau - d/s/b 
  // sort: then nue/numu/nutau - u/c/t
  if( ((buffer.at(0) -> pdgId())%2) == 0 )
  {
    const reco::Candidate* temp = buffer.at(0);
    buffer.at(0) = buffer.at(1);
    buffer.at(1) = temp;
  }
  
  
  
  for(unsigned int i = 0; i < buffer.size(); ++i)
  {
    if( abs(buffer.at(i) -> pdgId()) == 11 )
      stringBuffer += "e";
    if( abs(buffer.at(i) -> pdgId()) == 12 )
      stringBuffer += "nue";
    
    if( abs(buffer.at(i) -> pdgId()) == 13 )
      stringBuffer += "mu";
    if( abs(buffer.at(i) -> pdgId()) == 14 )
      stringBuffer += "numu";
    
    if( abs(buffer.at(i) -> pdgId()) == 15 )
      stringBuffer += "tau";
    if( abs(buffer.at(i) -> pdgId()) == 16 )
      stringBuffer += "nutau";
    
    
    
    if( abs(buffer.at(i) -> pdgId()) == 1 )
      stringBuffer += "d";
    if( abs(buffer.at(i) -> pdgId()) == 2 )
      stringBuffer += "u";
    
    if( abs(buffer.at(i) -> pdgId()) == 3 )
      stringBuffer += "s";
    if( abs(buffer.at(i) -> pdgId()) == 4 )
      stringBuffer += "c";
    
    if( abs(buffer.at(i) -> pdgId()) == 5 )
      stringBuffer += "b";
    if( abs(buffer.at(i) -> pdgId()) == 6 )
      stringBuffer += "t";
    
    
    
    if(i == 0)  stringBuffer += "_";
  }
  
  
  mcVDecayMode_p = stringBuffer;
}

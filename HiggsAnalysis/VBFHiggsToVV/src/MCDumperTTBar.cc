#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperTTBar.h"


// -----------------------------------------------------------------
// ctor
// -----------------------------------------------------------------

MCDumperTTBar::MCDumperTTBar(edm::Handle<reco::GenParticleCollection>& genParticles,
                   const int& eventType,
                   const bool& verbosity):
 verbosity_p(verbosity),
 eventType_p(eventType),
 isValid_p(true),
 mcV1DecayMode_p(""),
 mcV2DecayMode_p(""),
 mcT1_p(0),
 mcT2_p(0),
 mcB1_p(0),
 mcB2_p(0),
 mcV1_p(0),
 mcV2_p(0),
 mcF1_fromV1_p(0),
 mcF2_fromV1_p(0),
 mcF1_fromV2_p(0),
 mcF2_fromV2_p(0)
 {
  if(verbosity_p)
    std::cerr << "*** MCDumperTTBar::ctor ***" << std::endl;
  
  Analyze(genParticles);
}






// -----------------------------------------------------------------
// dtor
// -----------------------------------------------------------------

MCDumperTTBar::~MCDumperTTBar()
{
  if(verbosity_p)
    std::cerr << "*** MCDumperTTBar::dtor ***" << std::endl;
}






// -----------------------------------------------------------------
// Analyze
// -----------------------------------------------------------------

void MCDumperTTBar::Analyze(edm::Handle<reco::GenParticleCollection>& genParticles)
{
  if(verbosity_p)
    std::cerr << "*** MCDumperTTBar::Analyze ***" << std::endl;
  
  
  
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
    
//     // top
    if( (abs(pdgId) == 6) && (status == 3) ) //---- top quark
    {
     if (!mcT1_p) mcT1_p = pCurrent;
     else mcT2_p = pCurrent;
    }
        
    ++counter;
    
  } // loop over gen particles
    

  // return if background
  if(eventType_p != 0) return;
 
  
  // check if it is signal
//   if(!mcH_p)
//   {
//     std::cerr << ">>> MCDumperTTBar::Analyze::Warning: no H in the event" << std::endl;
//     
//     isValid_p = false;
//     return;
//   }
  
  
  if(!mcT1_p || !mcT2_p)
  {
   std::cerr << ">>> MCDumper::Analyze::Warning: no 2 top in the event" << std::endl;
    
   isValid_p = false;
   return;
  }

  
  //---- first top decay ----
  std::vector<const reco::Candidate*> VBuffer_1;
  FindDaughterParticles(&mcT1_p, verbosity_p, 1, &VBuffer_1);
  
  if (verbosity_p) std::cerr << "MCDumperTTBar::VBuffer_1.size() = " << VBuffer_1.size() << std::endl;

  
  if(VBuffer_1.size() < 2) //---- even if some gluons are produced
  {
     std::cerr << "MCDumperTTBar::Analyze::Warning: not a top -> bW decay" << std::endl;

     isValid_p = false;
     return;
  }
  
  if (abs(VBuffer_1.at(0)->pdgId()) == 5){//---- bottom quark
   mcB1_p = VBuffer_1.at(0);
   mcV1_p = VBuffer_1.at(1);   
  }
  else {
   mcB1_p = VBuffer_1.at(1);
   mcV1_p = VBuffer_1.at(0);   
  }

   //---- second top decay ----
  std::vector<const reco::Candidate*> VBuffer_2;
  FindDaughterParticles(&mcT2_p, verbosity_p, 1, &VBuffer_2);
  
  if (verbosity_p) std::cerr << "MCDumperTTBar::VBuffer_2.size() = " << VBuffer_2.size() << std::endl;
  
  if(VBuffer_2.size() < 2) //---- even if some gluons are produced
  {
   std::cerr << "MCDumperTTBar::Analyze::Warning: not a top -> bW decay" << std::endl;

   isValid_p = false;
   return;
  }
  
  if (abs(VBuffer_2.at(0)->pdgId()) == 5){//---- bottom quark
   mcB2_p = VBuffer_2.at(0);
   mcV2_p = VBuffer_2.at(1);   
  }
  else {
   mcB2_p = VBuffer_2.at(1);
   mcV2_p = VBuffer_2.at(0);   
  }
  
  

  //---- find fermions from vector boson decay
  std::vector<const reco::Candidate*> fFromV1Buffer;
  FindDaughterParticles(&mcV1_p, verbosity_p, 1, &fFromV1Buffer);
  
  if(fFromV1Buffer.size() != 2)
  {
     std::cerr << "MCDumperTTBar::Analyze::Warning: not a V1 -> ff decay" << std::endl;

     isValid_p = false;
     return;
  }
  
  mcF1_fromV1_p = fFromV1Buffer.at(0);
  mcF2_fromV1_p = fFromV1Buffer.at(1);
  
  // If leptons, see if there is a photon emission
  if(abs(mcF1_fromV1_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF1_fromV1_p, verbosity_p, 1);
  if(abs(mcF2_fromV1_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF2_fromV1_p, verbosity_p, 1);
  
  if(mcF2_fromV1_p -> pt() > mcF1_fromV1_p -> pt())
  {
    const reco::Candidate* buffer = mcF1_fromV1_p;
    mcF1_fromV1_p = mcF2_fromV1_p;
    mcF2_fromV1_p = buffer;
  }
  
  //---- check if tau ---
  if (abs(mcF1_fromV1_p -> pdgId()) == 15 || abs(mcF2_fromV1_p -> pdgId()) == 15){
   if (abs(mcF1_fromV1_p -> pdgId()) == 15){
//     std::vector<const reco::Candidate*> tauJBuffer;
    std::vector<const reco::Candidate*> nuBuffer;
    FindTauDecayParticles(mcFX_fromV1_TauParticles_p, nuBuffer, mcF1_fromV1_p, verbosity_p);
//     mcFX_fromV1_TauParticles_p.push_back(tauJBuffer);
   }
   else if (abs(mcF2_fromV1_p -> pdgId()) == 15){
//     std::vector<const reco::Candidate*> tauJBuffer;
    std::vector<const reco::Candidate*> nuBuffer;
    FindTauDecayParticles(mcFX_fromV1_TauParticles_p, nuBuffer, mcF2_fromV1_p, verbosity_p);
//     mcFX_fromV1_TauParticles_p.push_back(tauJBuffer);
   }
  }
   
  
  
  std::vector<const reco::Candidate*> fFromV2Buffer;
  FindDaughterParticles(&mcV2_p, verbosity_p, 1, &fFromV2Buffer);

  if(fFromV2Buffer.size() != 2)
  {
     std::cerr << "MCDumperTTBar::Analyze::Warning: not a V2 -> ff decay" << std::endl;

     isValid_p = false;
     return;
  }
  
  mcF1_fromV2_p    = fFromV2Buffer.at(0);
  mcF2_fromV2_p    = fFromV2Buffer.at(1);
  
  // If leptons, see if there is a photon emission
  if(abs(mcF1_fromV2_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF1_fromV2_p, verbosity_p, 1);
  if(abs(mcF2_fromV2_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF2_fromV2_p, verbosity_p, 1);
  
  if(mcF2_fromV2_p -> pt() > mcF1_fromV2_p -> pt())
  {
    const reco::Candidate* buffer = mcF1_fromV2_p;
    mcF1_fromV2_p = mcF2_fromV2_p;
    mcF2_fromV2_p = buffer;
  }
  
  if (abs(mcF1_fromV2_p -> pdgId()) == 15 || abs(mcF2_fromV2_p -> pdgId()) == 15){
   if (abs(mcF1_fromV2_p -> pdgId()) == 15){
//     std::vector<const reco::Candidate*> tauJBuffer;
    std::vector<const reco::Candidate*> nuBuffer;
    FindTauDecayParticles(mcFX_fromV2_TauParticles_p, nuBuffer, mcF1_fromV2_p, verbosity_p);
//     mcFX_fromV2_TauParticles_p = tauJBuffer;
   }
   else if (abs(mcF2_fromV2_p -> pdgId()) == 15){
//     std::vector<const reco::Candidate*> tauJBuffer;
    std::vector<const reco::Candidate*> nuBuffer;
    FindTauDecayParticles(mcFX_fromV2_TauParticles_p, nuBuffer, mcF2_fromV2_p, verbosity_p);
//     mcFX_fromV2_TauParticles_p = tauJBuffer;
   }
  }

  
  
  
  
  GetVDecayMode(1);
  GetVDecayMode(2);
  
  // Print event summary
  if(verbosity_p)
    PrintEventInfo();

  return;
}






// -----------------------------------------------------------------
// Print Event Info
// -----------------------------------------------------------------

void MCDumperTTBar::PrintEventInfo()
{
  std::cout << "\n>>> MCDumperTTBar::PrintHInfo \n" << std::endl;
  
  
  
//   std::cout << "Higgs:           H  -> " << mcHDecayMode_p << std::endl;
//   if(mcH_p)
//     PrintParticleInfo(mcH_p);
  
  std::cout << std::endl;
  
  
  
  std::cout << "Vector bosons:   V1 -> " << mcV1DecayMode_p << std::endl;
  if(mcV1_p)
    PrintParticleInfo(mcV1_p);
  
  std::cout << "Vector bosons:   V2 -> " << mcV2DecayMode_p << std::endl;  
  if(mcV2_p)
    PrintParticleInfo(mcV2_p);
  
  std::cout << std::endl;
  
  
  
  
  std::cout << "Fermions from vector bosons: " << std::endl;
  if(mcF1_fromV1_p)
    PrintParticleInfo(mcF1_fromV1_p);
  if(mcF2_fromV1_p)
    PrintParticleInfo(mcF2_fromV1_p);
  if(mcF1_fromV2_p)
    PrintParticleInfo(mcF1_fromV2_p);
  if(mcF2_fromV2_p)
    PrintParticleInfo(mcF2_fromV2_p);
  
  std::cout << std::endl;
}







// -----------------------------------------------------------------
// Get Vector Boson decay mode
// -----------------------------------------------------------------

void MCDumperTTBar::GetVDecayMode(const int& Vindex)
{
  std::vector<const reco::Candidate*> buffer;
  std::string stringBuffer;
  if(Vindex == 1)
  {
    buffer.push_back(mcF1_fromV1_p);
    buffer.push_back(mcF2_fromV1_p);
  }
  if(Vindex == 2)
  {
    buffer.push_back(mcF1_fromV2_p);
    buffer.push_back(mcF2_fromV2_p);
  }
  
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
  
  
  
 if(Vindex == 1)
   mcV1DecayMode_p = stringBuffer;
 if(Vindex == 2)
   mcV2DecayMode_p = stringBuffer;
}

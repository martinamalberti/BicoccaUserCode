#include "PhysicsTools/MiBiCommonPAT/plugins/MCDecayModeFilterZW.h"



//! ctor
MCDecayModeFilterZW::MCDecayModeFilterZW(const edm::ParameterSet& iConfig): 
 m_srcGenParticles(iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType",0)),
 m_verbosity(iConfig.getUntrackedParameter<bool>("verbosity",false)),
 m_W_e_nue      (iConfig.getParameter<bool>("W_e_nue")),
 m_W_mu_numu    (iConfig.getParameter<bool>("W_mu_numu")),
 m_W_tau_nutau  (iConfig.getParameter<bool>("W_tau_nutau")),
 m_Z_e_e        (iConfig.getParameter<bool>("Z_e_e")),
 m_Z_mu_mu      (iConfig.getParameter<bool>("Z_mu_mu")),
 m_Z_tau_tau    (iConfig.getParameter<bool>("Z_tau_tau")),
 m_Z_nue_nue    (iConfig.getParameter<bool>("Z_nue_nue")),
 m_Z_numu_numu  (iConfig.getParameter<bool>("Z_numu_numu")),
 m_Z_nutau_nutau(iConfig.getParameter<bool>("Z_nutau_nutau"))
{}

// ----------------------------------------------------------------



//! dtor
MCDecayModeFilterZW::~MCDecayModeFilterZW()
{}

// ----------------------------------------------------------------




//! loop over the reco particles and count leptons
bool MCDecayModeFilterZW::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  // -------------------
  // Get the collections

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(m_srcGenParticles, genParticles);
  if( !(genParticles.isValid ()) )
  {
    std::cerr << ">>>>>> MCDecayModeFilterZW::Warning: " << m_srcGenParticles << " not available" << std::endl;
    return false;
  }
  
  
  
  //-----------
  // MCDumperZW 
  double keepEvent = true;
  
  // if it is signal
  if(m_eventType == 0)
  {
    MCDumperZW mcAnalysis(genParticles,m_eventType,m_verbosity);
    bool isValid = mcAnalysis.isValid();
    if(!isValid) keepEvent = false;
    
    
    
    // --------------
    // W boson decays
    
    if( (mcAnalysis.mcVDecayMode() == "e_nue")     && (m_W_e_nue     == false) ) keepEvent = false;
    if( (mcAnalysis.mcVDecayMode() == "mu_numu")   && (m_W_mu_numu   == false) ) keepEvent = false;
    if( (mcAnalysis.mcVDecayMode() == "tau_nutau") && (m_W_tau_nutau == false) ) keepEvent = false;
    
    // --------------
    // Z boson decays
    
    if( (mcAnalysis.mcVDecayMode() == "e_e")     && (m_Z_e_e     == false) ) keepEvent = false;
    if( (mcAnalysis.mcVDecayMode() == "mu_mu")   && (m_Z_mu_mu   == false) ) keepEvent = false;
    if( (mcAnalysis.mcVDecayMode() == "tau_tau") && (m_Z_tau_tau == false) ) keepEvent = false;
    if( (mcAnalysis.mcVDecayMode() == "nue_nue")     && (m_Z_nue_nue     == false) ) keepEvent = false;
    if( (mcAnalysis.mcVDecayMode() == "numu_numu")   && (m_Z_numu_numu   == false) ) keepEvent = false;
    if( (mcAnalysis.mcVDecayMode() == "nutau_nutau") && (m_Z_nutau_nutau == false) ) keepEvent = false;
  } // if it is signal
  
  
  
  return keepEvent;  
}

DEFINE_FWK_MODULE(MCDecayModeFilterZW);

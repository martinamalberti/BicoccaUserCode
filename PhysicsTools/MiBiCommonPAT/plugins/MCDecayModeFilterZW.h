#ifndef MCDecayModeFilterZW_h
#define MCDecayModeFilterZW_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include <string>






class MCDecayModeFilterZW : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit MCDecayModeFilterZW(const edm::ParameterSet&);
  
  //! dtor 
  ~MCDecayModeFilterZW();
  
  
  
 private:
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcGenParticles;
  int m_eventType;
  bool m_verbosity;
  
  
  bool m_Z_e_e;
  bool m_Z_mu_mu;
  bool m_Z_tau_tau;
  bool m_Z_nue_nue;
  bool m_Z_numu_numu;
  bool m_Z_nutau_nutau;
 };

#endif

#ifndef VBFMCJetTagger_h
#define VBFMCJetTagger_h

/** \class VBFMCJetTagger
 *
 * Object selector perform electron track isolation selection
 *
 */  
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
  
//#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <string>


class VBFMCJetTagger: public edm::EDProducer
{

 public:
 typedef math::XYZTLorentzVector LorentzVector ;
  typedef std::vector<LorentzVector> LorentzVectorCollection ;
 
  //! ctor  
  VBFMCJetTagger (const edm::ParameterSet& conf) ;  
  //! dtor
  virtual ~VBFMCJetTagger () ;

  void beginJob ( const edm::EventSetup & ) {} ;
  //! actual produce method
  void produce (edm::Event &, const edm::EventSetup&) ;
  //! end job
  void endJob () {} ;

 private:
 
  //! input collection
  edm::InputTag m_genParticles ;
  //! output collection
  std::string m_MCtagJetsName ;
  void setMomentum (LorentzVector & myvector, const reco::GenParticle & gen);

};
  
#endif
 



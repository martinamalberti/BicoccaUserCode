#ifndef VBFJetTagger_h
#define VBFJetTagger_h

/** \class VBFJetTagger
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

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"

#include <string>

class VBFJetTagger: public edm::EDProducer
{

 public:

  //! ctor  
  VBFJetTagger (const edm::ParameterSet& conf) ;  
  //! dtor
  virtual ~VBFJetTagger () ;
  
  typedef reco::CaloJetCollection collection ;
  typedef reco::CaloJetRef jet ;
//  typedef std::vector<const reco::PixelMatchGsfElectron *> container;
  typedef std::vector<reco::CaloJetRef> container ;
  typedef container::const_iterator const_iterator ;

  //! begin job
  void beginJob ( const edm::EventSetup & ) {} ;
  //! actual produce method
  void produce (edm::Event &, const edm::EventSetup&) ;
  //! end job
  void endJob () {} ;

 private:
 
  //! input collection
  edm::InputTag m_jetInputTag ;
  //! output collection
  std::string m_tagJetsName ;

  //! |eta| threshold below which look for jets
  double m_jetEtaMax ;
  //! jet pt minimum threshold above which look for jets
  double m_jetPtMin ;
  //! size of the cone inside which other jets are summed 
  //! to the leading tag ones
  double m_gatherConeSize ;
  


};
  
#endif
 



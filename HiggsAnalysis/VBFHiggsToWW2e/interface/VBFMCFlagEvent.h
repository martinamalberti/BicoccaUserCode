#ifndef VBFMCFlagEvent_h
#define VBFMCFlagEvent_h

/** \class VBFMCFlagEvent
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
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include <string>


class VBFMCFlagEvent: public edm::EDProducer
{

  public : 
  //! ctor  
  VBFMCFlagEvent (const edm::ParameterSet& conf) ;  
  //! dtor
  virtual ~VBFMCFlagEvent () ;

  void beginJob ( const edm::EventSetup & ) {} ;
  //! actual produce method
  void produce (edm::Event &, const edm::EventSetup&) ;
  //! end job
  void endJob () {} ;

 private:
 
  //! input collection
  std::string m_label ;
  //! output collection
  std::string m_eventFlagName ;

};
  
#endif
 



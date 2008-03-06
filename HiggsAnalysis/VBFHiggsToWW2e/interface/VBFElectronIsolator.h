#ifndef VBFElectronIsolator_h
#define VBFElectronIsolator_h

#include <memory>
#include <vector>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"


class VBFElectronIsolator{
 public:
   explicit VBFElectronIsolator (const edm::ParameterSet&) ;
   ~VBFElectronIsolator();

   // Collections to be selected
   typedef reco::PixelMatchGsfElectronCollection collection;
   typedef std::vector<reco::PixelMatchGsfElectronRef> ::const_iterator const_iterator;

   //define iterators with above typedef
   const_iterator begin () const { return selected_.begin () ; }
   const_iterator end () const { return  selected_.end () ; }

   void select (edm::Handle<reco::PixelMatchGsfElectronCollection>, const edm::Event&) ;
	
 private:	
   std::vector<reco::PixelMatchGsfElectronRef> selected_;
   edm::InputTag tracksLabel_;
   edm::InputTag electronsLabel_;
   edm::InputTag trckIsolationProducer_;
   bool doRefCheck_;
   edm::InputTag selectedElectronsRefLabel_;
   double theTrackIsolCut_; 
};


#endif

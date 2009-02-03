// $Id: VBFElectronHadIsolator.h,v 1.3 2008/12/31 16:39:31 govoni Exp $
#ifndef VBFElectronHadIsolator_h
#define VBFElectronHadIsolator_h

#include <memory>
#include <vector>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleHadIsolationAlgo.h"

#include <TH1F.h>

class VBFElectronHadIsolator{

 public:

   explicit VBFElectronHadIsolator (const edm::ParameterSet&) ;
   ~VBFElectronHadIsolator () ;

   // Collections to be selected
//   typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
   typedef edm::View<reco::PixelMatchGsfElectron> collection ;
   typedef reco::PixelMatchGsfElectronRef electronRef ;
   typedef edm::View<reco::Track> trackCollection ; 

//   typedef reco::PixelMatchGsfElectronCollection collection ;
   typedef std::vector<reco::PixelMatchGsfElectronRef> ::const_iterator const_iterator ;

   //define iterators with above typedef
   const_iterator begin () const { return m_selected.begin () ; }
   const_iterator end () const { return  m_selected.end () ; }

   void select (const edm::Handle<collection>, const edm::Event&, const edm::EventSetup&) ;
   void newEvent (const edm::Event & evt, const edm::EventSetup& es) ;
	
 private:	
 
   //! all the tracks
   edm::InputTag m_hcalrhitsInputTag ;

   //! the isolation algorithm
   VBFEleHadIsolationAlgo m_hadIsolationAlgo ;

   //! the isolation threshold
   double m_hadIsolationCut ; 

   //! isolated electrons   
   std::vector<reco::PixelMatchGsfElectronRef> m_selected ;

   //! the calorimeter geometry
   edm::ESHandle<CaloGeometry> m_caloGeomHandle ;

};

#include "PhysicsTools/UtilAlgos/interface/EventSetupInitTrait.h"

EVENTSETUP_STD_INIT (VBFElectronHadIsolator) ; //PG no ";" for this release


#endif

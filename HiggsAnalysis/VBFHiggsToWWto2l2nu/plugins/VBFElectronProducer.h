#ifndef __VBFElectronProducer_H__
#define __VBFElectronProducer_H__


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
  
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
//#include "DataFormats/TrackReco/interface/Track.h"

//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/classes.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleTrackerIsolationAlgo.h"


class VBFElectronProducer : public edm::EDProducer {

 public:
 
     typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
     typedef reco::PixelMatchGsfElectronRef electronRef ;

      explicit VBFElectronProducer (const edm::ParameterSet&) ;
      ~VBFElectronProducer () ;

   private:
   
      virtual void beginJob (const edm::EventSetup&) ;
      virtual void produce (edm::Event&, const edm::EventSetup&) ;
      virtual void endJob () ;

//      const reco::ClusterShapeRef& getClusterShape (const reco::PixelMatchGsfElectronRef electron, 
//                                                    const edm::Event& e) ;

      //! isolation algorithm
      VBFEleTrackerIsolationAlgo m_tkIsolationAlgo ;
    
      edm::InputTag m_eleInputTag ;
      edm::InputTag m_EBClusterShapesInputTag ;
      edm::InputTag m_EEClusterShapesInputTag ;

      edm::InputTag m_eleIDPTDRLooseInputTag ; 
      edm::InputTag m_eleIDPTDRMediumInputTag ; 
      edm::InputTag m_eleIDPTDRTightInputTag ; 
      edm::InputTag m_TracksInputTag ;
      edm::InputTag m_IterativeTracksInputTag ;

      int m_electronsNum ;
} ;
  
#endif
 



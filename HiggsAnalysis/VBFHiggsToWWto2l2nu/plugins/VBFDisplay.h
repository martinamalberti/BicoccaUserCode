// -*- C++ -*-
//
// Package:    VBFDisplay
// Class:      VBFDisplay
// 
/**\class VBFDisplay VBFDisplay.cc HiggsAnalysis/VBFDisplay/src/VBFDisplay.cc

 Description: <one line class summary>
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFDisplay.h,v 1.5 2008/12/31 16:39:30 govoni Exp $
//
//

#ifndef VBFDisplay_H
#define VBFDisplay_H

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "HepMC/GenEvent.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

////#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaHcalIsolation.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"

//root include
#include <TCanvas.h>
#include <TLegend.h>
#include <TH2.h>
#include <TTree.h>
#include <TFile.h>
#include <TGraph.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TVector3.h>


class VBFDisplay : public edm::EDAnalyzer {

  public:
    
      typedef math::XYZTLorentzVector LorentzVector ;
      typedef std::vector<LorentzVector> LorentzVectorCollection ;
      typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
      typedef edm::View<reco::Muon> muonCollection ;
      typedef edm::View<reco::Track> trackCollection ;

      //! ctor
      explicit VBFDisplay (const edm::ParameterSet&);
      //! dtor
      ~VBFDisplay ();

   private:

      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();

   private:

      edm::InputTag m_jetTagsInputTag ;
      edm::InputTag m_jetInputTag ;
      edm::InputTag m_tracksInputTag ;
      edm::InputTag m_GSFInputTag ;
      edm::InputTag m_electronIDInputTag ;
      edm::InputTag m_muInputTag ;
      edm::InputTag m_metInputTag ;
      edm::InputTag m_MCtruthInputTag ;

      //!PG choose whether to plot the MCtruth
      bool m_drawMC ;

      //!PG common graphic objects
      TCanvas * m_c1 ;
      TH2F * m_bkg ;
      TLegend * m_legend ;

      double * m_tagjet_eta ;
      double * m_tagjet_phi ;
      TGraph * m_tagjet_graph ;

      double * m_jet_eta ;
      double * m_jet_phi ;
      TGraph * m_jet_graph ;

      double * m_otherjet_eta ;
      double * m_otherjet_phi ;
      TGraph * m_otherjet_graph ;

      double * m_elecs_eta ;
      double * m_elecs_phi ;
      TGraph * m_elecs_graph ;

      double * m_tracks_eta ;
      double * m_tracks_phi ;
      TGraph * m_tracks_graph ;

      double * m_muons_eta ;
      double * m_muons_phi ;
      TGraph * m_muons_graph ;

      double * m_MET_eta ;
      double * m_MET_phi ;
      TGraph * m_MET_graph ;

      TH2F * m_MCtruth ;

//PG does not work
     template <class handle>
     void fillgraph (double * eta, double * phi, TGraph * map,
                     handle & inputColl,
                     int marker, int color)
       {
         if (inputColl->size () == 0) return ;
         eta = new double [inputColl->size ()] ;
         phi = new double [inputColl->size ()] ;
       
         for (int it = 0 ; it < inputColl->size () ; ++it)
           {
             eta[it] = (*inputColl)[it].eta () ;
             phi[it] = (*inputColl)[it].phi () ;
           }
         map = new TGraph (inputColl->size (), eta, phi) ;
         map->SetMarkerStyle (marker) ;
         map->SetMarkerColor (color) ;
       }                     



};

#endif

// -*- C++ -*-
//
// Package:    VBFLeptPlots
// Class:      VBFLeptPlots
// 
/**\class VBFLeptPlots VBFLeptPlots.cc HiggsAnalysis/VBFLeptPlots/src/VBFLeptPlots.cc

 Description: <one line class summary>
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFLeptPlots.h,v 1.2 2008/02/08 13:35:16 govoni Exp $
//
//

#ifndef VBFLeptPlots_H
#define VBFLeptPlots_H

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "CLHEP/HepMC/GenEvent.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"


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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"

//root include
#include <TTree.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TNtuple.h>


struct VBFLeptHistos 
{
  TH1F * m_deltaEta ;
  TH1F * m_deltaPhi ;
  TH1F * m_averageEta ;
  TH1F * m_averagePhi ;
  TH1F * m_invMass ;
  TH1F * m_eta ;
  TH1F * m_phi ;
  TH1F * m_pt ;
  TH1F * m_ptMax ;
  TH1F * m_ptMin ;
  void init (std::string tag, 
             edm::Service<TFileService> & fs) ;
} ;


// --------------------------------------------------------


class VBFLeptPlots : public edm::EDAnalyzer 
{
  public:
    
      typedef math::XYZTLorentzVector LorentzVector ;
      typedef std::vector<LorentzVector> LorentzVectorCollection ;
      typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
      typedef edm::View<reco::Muon> muonCollection ;

      //! ctor
      explicit VBFLeptPlots (const edm::ParameterSet&);
      //! dtor
      ~VBFLeptPlots ();

   private:

      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();

      template <class firstIt, class secondIt>
      void fillHistos (firstIt first, secondIt second, VBFLeptHistos & histos)
        {
          histos.m_eta->Fill (first->eta ()) ;
          histos.m_eta->Fill (second->eta ()) ;
          histos.m_phi->Fill (first->phi ()) ;
          histos.m_phi->Fill (second->phi ()) ;
          histos.m_pt->Fill (first->energy ()) ;
          histos.m_pt->Fill (second->energy ()) ;
          histos.m_deltaEta->Fill (fabs (first->eta () - second->eta ())) ;
          double dphi = vbfhww2l::deltaPhi (first->phi (), second->phi ()) ;
          histos.m_deltaPhi->Fill (dphi) ;
          histos.m_averageEta->Fill (0.5 * (first->eta () + second->eta ())) ;
          double phi1 = first->phi () ; 
          if (phi1 < 0) phi1 += 6.28 ;
          double phi2 = second->phi () ; 
          if (phi2 < 0) phi2 += 6.28 ;
          histos.m_averagePhi->Fill (0.5 * (phi1 + phi2)) ;
          LorentzVector sum = first->p4 () + second->p4 () ;
          histos.m_invMass->Fill (sum.M ()) ;
          histos.m_ptMax->Fill (first->pt ()) ;
          histos.m_ptMin->Fill (second->pt ()) ;
          return ;
        }


   private:

      edm::InputTag m_GSFInputTag ;
      edm::InputTag m_muInputTag ;

      TNtuple * m_ntuple ;
      VBFLeptHistos m_GSFhistos ;
      VBFLeptHistos m_MUhistos ;
      VBFLeptHistos m_Xhistos ;

} ;

#endif

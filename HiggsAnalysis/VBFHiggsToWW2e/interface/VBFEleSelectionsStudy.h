// -*- C++ -*-
//
// Package:    VBFEleSelectionsStudy
// Class:      VBFEleSelectionsStudy
// 
/**\class VBFEleSelectionsStudy VBFEleSelectionsStudy.cc HiggsAnalysis/VBFEleSelectionsStudy/src/VBFEleSelectionsStudy.cc

 Description: <one line class summary>
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFEleSelectionsStudy.h,v 1.1 2008/02/10 18:13:00 govoni Exp $
//
//

#ifndef VBFEleSelectionsStudy_H
#define VBFEleSelectionsStudy_H

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

//root include
#include <TTree.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TNtuple.h>


class VBFEleSelectionsStudy : public edm::EDAnalyzer 
{
  public:
    
      typedef math::XYZTLorentzVector LorentzVector ;
      typedef std::vector<LorentzVector> LorentzVectorCollection ;
      typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
      typedef edm::View<reco::Muon> muonCollection ;

      //! ctor
      explicit VBFEleSelectionsStudy (const edm::ParameterSet&);
      //! dtor
      ~VBFEleSelectionsStudy ();

   private:

      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();
      void findFirstTwo (
        edm::Handle<electronCollection> & EleHandle ,
        electronCollection::const_iterator & firstEle ,
        electronCollection::const_iterator & secondEle) ;
      void findFirstTwoMC (
        edm::Handle<reco::CandidateCollection> & EleHandle ,
        const reco::Candidate ** firstEle ,
        const reco::Candidate ** secondEle) ;

   private:

      edm::InputTag m_MCtruthInputTag ;
      edm::InputTag m_GSFInputTag ;
      edm::InputTag m_ARInputTag ;

      TH2F * m_ARvsGSF ;
      TH1F * m_deltaEta ;
      TH1F * m_deltaPhi ;


} ;

#endif

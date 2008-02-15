// -*- C++ -*-
//
// Package:    VBFplots
// Class:      VBFplots
// 
/**\class VBFplots VBFplots.cc HiggsAnalysis/VBFplots/src/VBFplots.cc

 Description: <one line class summary>
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFplots.h,v 1.9 2008/02/13 15:47:06 tancini Exp $
//
//

#ifndef VBFplots_H
#define VBFplots_H

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

class VBFplots : public edm::EDAnalyzer {

  public:
    
      typedef math::XYZTLorentzVector LorentzVector ;
      typedef std::vector<LorentzVector> LorentzVectorCollection ;

      //! ctor
      explicit VBFplots (const edm::ParameterSet&);
      //! dtor
      ~VBFplots ();

   private:

      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();

   private:

      edm::InputTag m_jetTagsInputTag ;
      edm::InputTag m_jetOthersInputTag ;
      edm::InputTag m_GSFInputTag ;
      edm::InputTag m_electronIDInputTag ;
      edm::InputTag m_muInputTag ;
      edm::InputTag m_metInputTag ;

      TH1F *m_met;
      TH1F *m_firstEnergyTagEnergy;
      TH1F *m_secondEnergyTagEnergy;
      TH2F *m_deltaPhiVsMinEnergyTag;
      TH1F *m_deltaEta;
      TH1F *m_deltaPhi;
      TH1F *m_sumEta;
      TH1F *m_invMass;
      TH1F *m_eta;
      TH1F *m_energy;
      TH1F *m_pt;
      TH1F *m_numOthers15;
      TH1F *m_numOthers20;
      TH1F *m_numOthers30;
      TH1F *m_etaOthers;
      TH1F *m_energyOthers;
      TH1F *m_ptOthers;
      TH1F *m_etaOthersSummed;
      TH1F *m_energyOthersSummed;
      TH1F *m_ptOthersSummed;
      TH1F *m_invMassOtherSummed;
      TH1F *m_deltaROthers;
      TH1F *m_deltaEtaOthers;

      TH1F *m_etaOthers30;
      TH1F *m_energyOthers30;
      TH1F *m_ptOthers30;
      TH1F *m_etaOthersSummed30;
      TH1F *m_energyOthersSummed30;
      TH1F *m_ptOthersSummed30;
      TH1F *m_invMassOtherSummed30;
      TH1F *m_deltaROthers30;
      TH1F *m_deltaEtaOthers30;

      TH2F *m_etaSummedTagVsEtaSummedOthers;
      TNtuple *m_ntuple;

      int m_evAnalyzed ;
      int m_evWithTags ;
      int m_evWithOthers ;
};

#endif

// -*- C++ -*-
//
// Package:    VBFReadEvent
// Class:      VBFReadEvent
// 
/**\class VBFReadEvent VBFReadEvent.cc HiggsAnalysis/VBFReadEvent/src/VBFReadEvent.cc

 Description: <one line class summary>
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFReadEvent.h,v 1.25 2007/12/05 13:27:11 tancini Exp $
//
//

#ifndef VBFReadEvent_H
#define VBFReadEvent_H

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


class VBFReadEvent : public edm::EDAnalyzer {

  public:
    
      //! ctor
      explicit VBFReadEvent (const edm::ParameterSet&);
      //! dtor
      ~VBFReadEvent ();

   private:

      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();
      void setMomentum (TLorentzVector & vector, const reco::Candidate & gen);
      TLorentzVector get4momentum (const Candidate & gen);
	  TVector3 getVtx (const Candidate & gen);
    
      TVector3 getTrackMomentumAtVtx (const reco::PixelMatchGsfElectron & ele);
      TVector3 getTrackPositionAtVtx (const reco::PixelMatchGsfElectron & ele);
      void findGenParticles (edm::Handle<CandidateCollection> &genParticles,
                                         TLorentzVector &m_genHiggs,
                                         TLorentzVector &m_genWm,
                                         TLorentzVector &m_genWp,
                                         TLorentzVector &m_genLepPlus,
                                         TLorentzVector &m_genLepMinus,
                                         TLorentzVector &m_genMetPlus,
                                         TLorentzVector &m_genMetMinus,
                                         TLorentzVector &m_genqTagF,
                                         TLorentzVector &m_genqTagB,
					 int & m_genqTagF_Flavour,
				         int & m_genqTagB_Flavour);

      void setZero (TLorentzVector &m_genHiggs,
	 	   TLorentzVector &m_genWm,
		   TLorentzVector &m_genWp,
		   TLorentzVector &m_genLepPlus,
		   TLorentzVector &m_genLepMinus,
		   TLorentzVector &m_genMetPlus,
		   TLorentzVector &m_genMetMinus,
		   TLorentzVector &m_genqTagF,
		   TLorentzVector &m_genqTagB,
		   int & m_genqTagF_Flavour,
		   int & m_genqTagB_Flavour);
    

   private:

      edm::InputTag m_metInputTag;
      edm::InputTag m_genMetInputTag;
      edm::InputTag m_jetInputTag;
      edm::InputTag m_genJetInputTag;
      edm::InputTag m_GSFInputTag;
      edm::InputTag m_electronIDInputTag;
	  //std::string m_siElectronProducer;
      //std::string m_siElectronCollection;
	  //std::string m_electronProducer;
	  //std::string m_electronCollection;
      edm::InputTag m_MCtruthInputTag;
      edm::InputTag m_MC;
      edm::InputTag m_muInputTag;
      edm::InputTag m_trackInputTag;
	  
	  double m_ptMin;
      double m_intRadius;
      double m_extRadius;
      double m_maxVtxDist;
	  
      edm::InputTag m_hcalRecHitProducer;
      edm::InputTag m_emObjectProducer;
	  double m_egHcalIsoPtMin;
	  double m_egHcalIsoConeSizeIn; 
      double m_egHcalIsoConeSizeOut;
	  
	  //Jet Flavour Identifier from BTau
      JetFlavourIdentifier m_jfi;  
    
      TFile *m_outfile;
      TTree* m_genTree;
      std::string m_rootfile;

      int m_evtFlag;
      TLorentzVector *m_genHiggs;
      TLorentzVector *m_genWm;
      TLorentzVector *m_genWp;
      TLorentzVector *m_genLepPlus;
      TLorentzVector *m_genLepMinus;
      TLorentzVector *m_genMetPlus;
      TLorentzVector *m_genMetMinus;
      TLorentzVector *m_genqTagF;
      TLorentzVector *m_genqTagB;
      int m_LepPlusFlavour;
      int m_LepMinusFlavour; 
      int m_genqTagF_Flavour;
      int m_genqTagB_Flavour;
    
      int m_numberGSF;
      TClonesArray *m_recoEle4Momentum;
      TClonesArray *m_recoEleTrkMomentumAtVtx;
      TClonesArray *m_recoEleTrkPositionAtVtx;
      std::vector<double> *m_recoEleEcalEnergy;
      std::vector<double> *m_recoEleTrkIsoVal;
      std::vector<double> *m_recoEleCalIsoVal;
      std::vector<int> *m_recoEleClass;
      std::vector<int> *m_recoEleCutBasedID;
	  std::vector<int> *m_recoEleCharge;
	  
	  int m_numberGlobMuons;
      TClonesArray *m_recoMuon4Momentum;
	  TClonesArray *m_recoMuonTrkPositionAtVtx;
	  std::vector<int> *m_recoMuonCharge;
	  std::vector<double> *m_recoMuonR03_sumPt;
      std::vector<double> *m_recoMuonR03_emEt;
      std::vector<double> *m_recoMuonR03_hadEt;
      std::vector<double> *m_recoMuonR03_hoEt;
      std::vector<int> *m_recoMuonR03_nTracks;
      std::vector<int> *m_recoMuonR03_nJets;
    
      int m_numberGenJet;
      TClonesArray *m_genJet4Momentum;
      std::vector<int> *m_genJetFlavour;    
    
      int m_numberJet;
      TClonesArray *m_recoJet4Momentum;
      std::vector<int> *m_recoJetFlavour;     
    
      TLorentzVector *m_genMet4Momentum;    
    
      TLorentzVector *m_recoMet4Momentum; 
    
};

#endif

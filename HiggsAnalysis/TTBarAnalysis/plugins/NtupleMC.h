// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include "TParticle.h"


#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"
#include "PhysicsTools/NtupleUtils/interface/MCDumper.h"

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"
#include "Math/GenVector/VectorUtil.h"


//---------------------------
//---- class declaration ----
//---------------------------

class NtupleMC : public edm::EDAnalyzer {
 public:
  typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::GsfElectron>, float> > eleMap;
  typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::Muon>, float> > muMap;

  explicit NtupleMC(const edm::ParameterSet&);
  ~NtupleMC();
  
 private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
    
  void fillGenJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;
    
  
  ///---- input tag ----

  edm::InputTag MCtruthTag_;
  edm::InputTag genJetTag_; 
  
  bool verbosity_; //---- true = loquacious     false = silence  
};


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//--- objects ----
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/GaussianSumVertexFit/interface/GsfVertexFitter.h"

#include "DataFormats/Common/interface/ValueMap.h"

//---- utilities ----
#include "HiggsAnalysis/littleH/interface/MCDumper.h"
#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

#include <TTree.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TParticle.h>

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"

using namespace edm;
using namespace std;

//---------------------------
//---- class declaration ----
//---------------------------

class SimpleNtple : public EDAnalyzer {
  public:
    explicit SimpleNtple(const ParameterSet&);
    ~SimpleNtple();
  
  private:
    virtual void beginJob(const EventSetup&) ;
    virtual void analyze(const Event&, const EventSetup&);
    virtual void endJob() ;
    
    void fillVtxInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillMuInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillTrackInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillEleInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillSCInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillMCInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillTriggerInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillBeamSpotInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void findOniaCategories (const Event & iEvent, const EventSetup & iESetup) ;
    void fillOnia2MuMuTracks(reco::TrackRef lep1, int l1, reco::TrackRef lep2, int l2, TVector3 vperp2, int oniacato);
    void fillOnia2EleEleTracks(reco::GsfTrackRef lep1, int l1, reco::GsfTrackRef lep2, int l2, TVector3 vperp2, int oniacato);
    double PhiInRange(const double& phi) const;

    template <class T> math::XYZTLorentzVector lorentzMomentum(const T & muon) const;
    std::vector<unsigned int> trackHits(const reco::Track& tr);
    template <class T, class U> double deltaR(const T & t, const U & u) const;
    double GetTheta( math::XYZTLorentzVector & a,  math::XYZTLorentzVector & b) const; 
 
    TTree* outTree_;
    NtupleFactory* NtupleFactory_;
    double oniaMass;
    double branch_ratio;
    
    ParameterSet gsfPSet;

    InputTag TracksTag_;
    InputTag EleTag_;
    InputTag MuTag_;
    InputTag PrimaryVertexTag_;        

    InputTag MCtruthTag_;
  
    InputTag m_eleIDCut_LooseInputTag ;
    InputTag m_eleIDCut_RLooseInputTag ;
    InputTag m_eleIDCut_TightInputTag ;
    InputTag m_eleIDCut_RTightInputTag ;

    InputTag barrelClusterCollection_;
    InputTag endcapClusterCollection_;
    
    InputTag beamSpotTag_;
  
    string thetriggerEventTag_;
    string theHLTriggerResults_;     // HLT trigger results
    string the8e29ProcName_;
    string the1e31ProcName_;
  
    static const int Max_HLT_size = 7; //why 10 in Onia2MuMu? ask to Mario
    static const int NHLTTRIGGERS = 7;
    static const int Max_L1T_size = 6; 
    static const int NL1TTRIGGERS = 6;
  
    int hltBits[NHLTTRIGGERS];
    int l1tBits[NL1TTRIGGERS];
  
    HLTConfigProvider hltConfig;
    ESHandle<TransientTrackBuilder> theB;

    bool saveVtx_ ;
    bool saveMu_ ;
    bool saveTracks_ ;
    bool saveEle_ ;
    bool saveMC_ ;
    bool saveSC_ ;
    bool saveTrigger_ ;
    bool saveBeamSpot_ ;
    bool saveOniaCand_ ;
    
    bool theStoreWSOnia; // Yes or No to store wrong-sign mu-mu combinations
    bool theBeamSpotFlag; 
    
    int eventType_; //---- 0 = signal      1 = background 
    bool verbosity_; //---- true = loquacious     false = silence  

  //used to save all tracks BUT muons
    std::vector<int> theMuonTrkIndexes_ ;
    
    //RECO Objects    
    reco::MuonCollection theTrkMuons;
    reco::MuonCollection theGlobalMuons;
    
    reco::GsfElectronCollection theElectrons;

    int Reco_mu_glb_size;
    int Reco_mu_trk_size;  
    
    int QQ_size;
    int theOniaType;
    int theOniaMaxCat;
    // Onia category:
    // 0 = EleEle 
    // 1 = goldenMuMu (2 global muons)
    // 2 = silverMuMu (1 global - 1 tracker muon)
    // 3 = bronzeMuMu (2 tracker muons)

};

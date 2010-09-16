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
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//--- objects ----
#include <DataFormats/PatCandidates/interface/Muon.h>
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include <DataFormats/PatCandidates/interface/Electron.h>
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/GaussianSumVertexFit/interface/GsfVertexFitter.h"
#include "RecoVertex/GaussianSumVertexFit/interface/AdaptiveGsfVertexFitter.h"

#include "DataFormats/Common/interface/ValueMap.h"

//---- utilities ----
#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

#include <TTree.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TParticle.h>

//#include "Math/PtEtaPhiE4D.h"
//#include "Math/PtEtaPhiM4D.h"
//#include "Math/LorentzVector.h"
//#include "Math/Vector3D.h"

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
    virtual void beginJob() ;
    virtual void analyze(const Event&, const EventSetup&);
    virtual void endJob() ;
    
    void buildLepCollections (const Event & iEvent, const EventSetup & iESetup) ;
    void fillEvtInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillVtxInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillMuInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillTrackInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillEleInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillMCInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillBeamSpotInfo (const Event & iEvent, const EventSetup & iESetup) ;
    void fillOniaInfo (const Event & iEvent, const EventSetup & iESetup) ;

    template <class T> TLorentzVector lorentzMomentumMu(const T & muon) const;
    
    std::vector<unsigned int> trackHits(const reco::Track & tr);
 
    TTree* outTree_;
    NtupleFactory* NtupleFactory_;
    float branch_ratio;
    
    InputTag Onia2MuMuTag_;
    InputTag Onia2EleEleTag_;    
    InputTag TracksTag_;
    InputTag EleTag_;
    InputTag MuTag_;
    InputTag PrimaryVertexTag_;        
    InputTag MCtruthTag_;
    string m_eleIDCut_LooseInputTag ;
    string m_eleIDCut_RLooseInputTag ;
    string m_eleIDCut_TightInputTag ;
    string m_eleIDCut_RTightInputTag ;
    InputTag beamSpotTag_;

    bool saveEvt_ ;
    bool saveVtx_ ;
    bool saveMu_ ;
    bool saveTracks_ ;
    bool saveEle_ ;
    bool saveMC_ ;
    bool saveBeamSpot_ ;
    bool saveOniaCand_ ;
        
    int eventType_; //---- 0 = signal      1 = background 
    bool verbosity_; //---- true = loquacious     false = silence  

    //used to save all tracks BUT muons
    std::vector<int> theMuonTrkIndexes_ ;
    
    //RECO Objects    
    pat::MuonCollection theTrkMuons;
    pat::MuonCollection theGlobalMuons;
    pat::ElectronCollection theElectrons;

    int Reco_mu_glb_size;
    int Reco_mu_trk_size;  
    
    //Index for DATA
    int nEvent ;
    int nRun ;
    int nLumi ;
    
};
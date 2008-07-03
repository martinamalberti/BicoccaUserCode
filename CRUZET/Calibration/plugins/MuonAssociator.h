/**
 * module  for displaying unpacked DCCHeader information
 *   
 * \author
 *
 */

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Framework/interface/MakerMacros.h>
#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TNtuple.h"


#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"


class MuonAssociator: public edm::EDAnalyzer {
    public:
     explicit MuonAssociator(const edm::ParameterSet&);
     virtual ~MuonAssociator(){}

     virtual void beginJob(const edm::EventSetup&) ;
     virtual void endJob() ;
     virtual void analyze (const edm::Event&, const edm::EventSetup&);
  
    private:
     edm::InputTag m_EBRecHitsInputTag ;
     edm::InputTag m_MuonsInputTag ; 
     edm::InputTag m_TracksInputTag ; 
     edm::InputTag barrelClusterCollection_;
     edm::InputTag endcapClusterCollection_;

     std::vector<int> maskedChannels_;
          
     TrackDetectorAssociator trackAssociator_;
     TrackAssociatorParameters parameters_; 

     int deltaPhi(int phi1, int phi2);

     double deta1,dphi1,R1,deta2,dphi2,R2,twopi;
     double detaDetID1,dphiDetID1,RDetID1,detaDetID2,dphiDetID2,RDetID2;

     double ECitPos, ECitNeg;
          double ECitPosUP, ECitNegUP;
     double GPointPos, GPointNeg;



     std::vector<EBDetId> findCosmicsInEvent (const edm::Event& , const edm::EventSetup& ) ;

     double minCosmicE1_ ; //PG GeV
     double minCosmicE2_ ; //PG GeV
     double minNoiseThr_ ; //PG GeV

  TH2F* allOccupancy_;
  TH2F* allOccupancyMatch_;
  TH2F* allOccupancyNoMatch_;
  TH1F* energyMatch_;
  TH1F* energyNoMatch_;
  TH2F* GPOccupancy_;
  TH1F* FirstCluster_;
  TH1F* nTracks_;
  TH1F* nMuons_;
  TH2F* MagVSPhiInner_;
  TH2F* MagVSPhiOuter_;
  TH2F* yVSxOuter_;
  TH2F* yVSxInner_;
  TH1F* phiMu_;
  //------------
  TH1F* RSeedVSDetID1;
  TH1F* RSeedVSDetID2;

  TH2F* EVSR1;
  TH2F* EVSR2;

  TH1F* GPointVSDetID1;
  TH1F* GPointVSDetID2;
  TH1F* GPointEner1;
  TH1F* GPointEner2;
  TH2F* DEtaVSDPhi1;
  TH2F* DEtaVSDPhi2;
  TH1F* diffHisto;

  TH1F* clusterSize;
  TH1F* nCluster_;
  TH1F* numberOfCosmics_;
  TH2F* ECitOccupancy;
  
};


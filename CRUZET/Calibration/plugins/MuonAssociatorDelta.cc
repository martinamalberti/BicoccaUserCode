
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include <vector>


#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "Calibration/Tools/interface/TrackDetMatchInfo.h"
//----------------
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
//----------------
#include "TrackingTools/TrackAssociator/interface/TrackDetMatchInfo.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "CRUZET/Calibration/plugins/MuonAssociatorDelta.h"

#include <vector>

using namespace cms;
using namespace edm;
using namespace std;



MuonAssociatorDelta::MuonAssociatorDelta (const edm::ParameterSet& iConfig):
  m_EBRecHitsInputTag (iConfig.getParameter<edm::InputTag> ("EBRecHitsInputTag")),
  m_MuonsInputTag (iConfig.getParameter<edm::InputTag> ("MuonsInputTag")),
  m_TracksInputTag (iConfig.getParameter<edm::InputTag> ("TracksInputTag")),
  barrelClusterCollection_ (iConfig.getParameter<edm::InputTag>("barrelClusterCollection")),
  endcapClusterCollection_ (iConfig.getParameter<edm::InputTag>("endcapClusterCollection")),
  minCosmicE1_ (iConfig.getUntrackedParameter<double>("minCosmicE1", 15.0)),
  minCosmicE2_ (iConfig.getUntrackedParameter<double>("minCosmicE2", 5.0)),
  minNoiseThr_ (iConfig.getUntrackedParameter<double>("minNoiseThr_", 0.027)) 
{
  twopi  = 2*acos(-1.);
  
  //MaskedCannels
  std::vector<int> listDefaults;
  listDefaults.push_back(-1);
  maskedChannels_ = iConfig.getUntrackedParameter <std::vector<int> >("maskedChannels", listDefaults);


  // TrackAssociator parameters
  edm::ParameterSet parameters = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
  parameters_.loadParameters( parameters );
  trackAssociator_.useDefaultPropagator();
}



void MuonAssociatorDelta::analyze ( const edm::Event & iEvent, const  edm::EventSetup& iSetup)
{


  //-------------------------------------RecHits----------------------
  //get the EB RecHits collection
  edm::Handle<EcalRecHitCollection> EBRecHitsHandle ;
  iEvent.getByLabel (m_EBRecHitsInputTag, EBRecHitsHandle) ;
  //  std::cerr << "found " << EBRecHitsHandle->size () << " EBRecHits" << std::endl ;
  if (!EBRecHitsHandle->size ()) return ;

  std::vector<EBDetId> hitCrystals ;
  std::vector<EcalRecHit> hitCrystalsRecHit;
  // loop over ECAL xtals
  for (EcalRecHitCollection::const_iterator EBCRHit = EBRecHitsHandle->begin () ;
       EBCRHit != EBRecHitsHandle->end () ;
       ++EBCRHit)
    {
      //-----------skipping masked Channel-------------------------------
      EBDetId ebDet = EBCRHit->id();
      int hashedIndex = ebDet.hashedIndex();

      std::vector<int>::iterator result;
      result = find(maskedChannels_.begin(), maskedChannels_.end(), hashedIndex);
      if  (result != maskedChannels_.end())
	{
	  //std::cerr << "skipping masked uncalRecHit: " << *result << std::endl;
	  continue;
	}
      //----------------filling hitCrystals-------------------------------
      
      hitCrystals.push_back(ebDet);
      hitCrystalsRecHit.push_back(*EBCRHit);
    }  // loop over ECAL xtals
  
  
  //--------------------------------------Tracks------------------------
  //get reco tracks 
  edm::Handle<reco::TrackCollection> recoTracks;
  iEvent.getByLabel(m_TracksInputTag, recoTracks);
  //  std::cerr << "found " << recoTracks->size () << " Tracks" << std::endl ;
  nTracks_->Fill(recoTracks->size());
  if (!recoTracks->size ()) return ;

  //  TrajectoryStateTransform tsTransform;    //serve per inner e outer state
  edm::ESHandle<MagneticField> bField;
  iSetup.get<IdealMagneticFieldRecord>().get(bField);

  //-----------------i più energetici-------------------
  std::vector<EBDetId> ecalCosmics = findCosmicsInEvent(iEvent,iSetup);
  //if (ecalCosmics.size () != 2) return; // troppo restrittivo!!!???

  
  //--------------loop sulle tracce----------------------
  for(reco::TrackCollection::const_iterator recoTrack = recoTracks->begin();
      recoTrack != recoTracks->end(); ++recoTrack)
    { 
      //----------------indicati dal TrackAss---------------
      TrackDetMatchInfo info = trackAssociator_.associate(iEvent, iSetup, *recoTrack, parameters_);
      if(info.crossedEcalIds.size()==0) continue;
      
      double nearest = 1000;
      double nearestDEta = 1000;
      double nearestDPhi = 1000;

      double deta = 1000;
      double dphi = 1000;
      double deltaR = 1000;

      int ii = 0;
      int firstPhi = 0;
      int diffPhi = 1000;
      for(std::vector<DetId>::const_iterator CEDId = info.crossedEcalIds.begin(); CEDId != info.crossedEcalIds.end(); ++CEDId)
	{
	  if (info.crossedEcalIds.at(ii).det() != DetId::Ecal || info.crossedEcalIds.at(ii).subdetId() != 1) continue;  //FIXME non dovrebbe essere necessario!
	  EBDetId dummy (*CEDId);
	  
	  //controllo sull'associazione delle tracce???  solo sopra/sotto
	  if(ii == 0) firstPhi = dummy.iphi();
	  else diffPhi = deltaPhi(firstPhi,dummy.iphi());
	  if(diffPhi != 1000) diffPhi_->Fill(diffPhi);

	  ii++;
	  
	  for(std::vector<EBDetId>::const_iterator ECit = ecalCosmics.begin () ;
	      ECit != ecalCosmics.end () ;
	      ++ECit )
	    {
	      deta = ECit->ieta() - dummy.ieta(); 
	      dphi = deltaPhi(ECit->iphi(),dummy.iphi());
	      deltaR = sqrt(deta*deta + dphi*dphi);
	      
	      if(deltaR < nearest) 
		{
		  nearest = deltaR; 
		  nearestDEta = deta;
		  nearestDPhi = dphi;
		}
	    }
	}
      if(deta != 1000 && dphi != 1000) DEtaVSDPhi1->Fill(nearestDEta,nearestDPhi);
    }
}
    


// ------------ method called once each job just before starting event loop  ------------
void 
MuonAssociatorDelta::beginJob(const edm::EventSetup&)
{
  
  edm::Service<TFileService> fs ;
  allOccupancy_  = fs->make <TH2F> ("OccupancyAllEvents" ,"OccupancyAllEvents",36,0.,6.28,172,-86,86) ;
  allOccupancyMatch_  = fs->make <TH2F> ("OccupancyAllEventsMatch_" ,"OccupancyAllEventsMatch_",36,0.,6.28,172,-86,86) ;
  allOccupancyNoMatch_  = fs->make <TH2F> ("OccupancyAllEventsNoMatch_" ,"OccupancyAllEventsNoMatch_",36,0.,6.28,172,-86,86) ;
  energyMatch_  = fs->make <TH1F> ("energyMatch_" ,"energyMatch_",500,0.,250.) ;
  energyNoMatch_ = fs->make <TH1F> ("energyNoMatch_" ,"energyNoMatch_",500,0.,250.) ;
  GPOccupancy_  = fs->make <TH2F> ("GPOccupancy_" ,"GPOccupancy_",36,-3.14,3.14,800,-1.479,1.479) ;

  FirstCluster_  = fs->make <TH1F> ("FirstCluster" ,"FirstCluster",10,0.,10.) ;
  nTracks_  = fs->make <TH1F> ("nTracks" ,"nTracks",10,0.,10.) ;
  nMuons_  = fs->make <TH1F> ("nMuons" ,"nMuons",10,0.,10.) ;

  MagVSPhiOuter_  = fs->make <TH2F> ("TransverseMagVSPhiOuter" ,"TransverseMagVSPhiOuter",800,-4.,4.,1200,0.,1200.) ;
  MagVSPhiInner_  = fs->make <TH2F> ("TransverseMagVSPhiInner" ,"TransverseMagVSPhiInner",800,-4.,4.,1200,0.,1200.) ;

  yVSxOuter_  = fs->make <TH2F> ("yVSxOuter_" ,"yVSxOuter_",2000,-1000.,1000.,2000,-1000.,1000.) ;
  yVSxInner_  = fs->make <TH2F> ("yVSxInner_" ,"yVSxInner_",2000,-1000.,1000.,2000,-1000.,1000.) ;


  phiMu_  = fs->make <TH1F> ("phiMu_" ,"phiMu_",36,-3.14,3.14) ;

  RSeedVSDetID1 = fs->make <TH1F> ("RSeedVSDetID1" ,"RSeedVSDetID1",200,0.,1.) ;
  RSeedVSDetID2 = fs->make <TH1F> ("RSeedVSDetID2" ,"RSeedVSDetID2",200,0.,1.) ;
  
  EVSR1 = fs->make <TH2F> ("EVSR1" ,"EVSR1",200,0.,1.,250,0.,250.) ;
  EVSR2 = fs->make <TH2F> ("EVSR2" ,"EVSR2",200,0.,1.,250,0.,250.) ;

  GPointVSDetID1 = fs->make <TH1F> ("GPointVSDetID1" ,"GPointVSDetID1",100,0.,1.) ;
  GPointVSDetID2 = fs->make <TH1F> ("GPointVSDetID2" ,"GPointVSDetID2",100,0.,1.) ;

  GPointEner1 = fs->make <TH1F> ("GPointEner1" ,"GPointEner1",100,0.,1.) ;
  GPointEner2 = fs->make <TH1F> ("GPointEner2" ,"GPointEner2",100,0.,1.) ;
  DEtaVSDPhi1 = fs->make <TH2F> ("DEtaVSDPhi1" ,"DEtaVSDPhi1",360,-180.,180.,345,-172.,173.) ;
  DEtaVSDPhi2 = fs->make <TH2F> ("DEtaVSDPhi2" ,"DEtaVSDPhi2",600,-0.3,0.3,600,-0.3,0.3) ;
  diffPhi_ = fs->make <TH1F> ("diffPhi" ,"diffPhi",360,-180.,180.) ;

  diffHisto = fs->make <TH1F> ("diffHisto" ,"diffHisto",1000,0.,1.) ;

  nCluster_  = fs->make <TH1F> ("nCluster_" ,"nCluster_",10,0.,10.) ;
  clusterSize  = fs->make <TH1F> ("clusterSize" ,"clusterSize",25,0.,25.) ;
  numberOfCosmics_ =  fs->make <TH1F> ("numberOfCosmics_" ,"numberOfCosmics_",10,0.,10.) ;
  ECitOccupancy = fs->make <TH2F> ("ECitOccupancy" ,"ECitOccupancy",360,0.,360.,172,-86.,86.) ;


  ECitPosUP=0;
  ECitNegUP=0;
  ECitPos=0;
  ECitNeg=0;
  GPointPos=0;
  GPointNeg=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuonAssociatorDelta::endJob()
{
//   std::cerr << "GPointPos = " << GPointPos << std::endl;
//   std::cerr << "GPointNeg = " << GPointNeg << std::endl;

//   std::cerr << "ECitPos = " << ECitPos << std::endl;
//   std::cerr << "ECitNeg = " << ECitNeg << std::endl;

//   std::cerr << "ECitPosUP = " << ECitPosUP << std::endl;
//   std::cerr << "ECitNegUP = " << ECitNegUP << std::endl;
}


//-------------deltaPhi--------------------
int MuonAssociatorDelta::deltaPhi (int phi1, int phi2)
{

  int deltaphi=(phi1-phi2);
  //  if (deltaphi > 360) deltaphi -= 360;
  if (abs(deltaphi) > 180)
    if(deltaphi >= 0) deltaphi = 360 - deltaphi;
    else  deltaphi = -360 - deltaphi;
  return deltaphi;
}


//!PG find a cosmic in the event
//!PG from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CCEcal/EcalCosmicRuns/CRUZET/Calibration/plugins/EcalCosmicsHists.cc?revision=1.5&view=markup
std::vector<EBDetId>
MuonAssociatorDelta::findCosmicsInEvent (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  int ievt = iEvent.id().event();
  edm::Handle<reco::BasicClusterCollection> bccHandle;  
  edm::Handle<reco::BasicClusterCollection> eccHandle;  
  std::vector<EBDetId> cosmicCrystals ;


  iEvent.getByLabel(barrelClusterCollection_, bccHandle);
  if (!(bccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << barrelClusterCollection_ << " not available";
      return cosmicCrystals;
    }
  LogDebug("EcalCosmicsHists") << "event " << ievt;
  
  iEvent.getByLabel(endcapClusterCollection_, eccHandle);
  if (!(eccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << endcapClusterCollection_ << " not available";
      return cosmicCrystals;
    }
  
  Handle<EcalRecHitCollection> hits;
  iEvent.getByLabel(m_EBRecHitsInputTag, hits);
  if (!(eccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << m_EBRecHitsInputTag << " not available";
      return cosmicCrystals;
    }

  int  numberOfCosmics = 0;
  int nCluster = 0;
  //int eventnum = iEvent.id().event();
  
  const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();
  //PG loop on basic clusters
  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); 
       clus != clusterCollection_p->end(); 
       ++clus)
    {
 
      double time = -1000.0;
      double ampli = 0.;
      double secondMin = 0.;
      double secondTime = -1000.;
      int numXtalsinE9 = 0;
	
      EBDetId maxDet;
      EBDetId secDet;

      std::vector<DetId> clusterDetIds = clus->getHitsByDetId();//get these from the cluster
      //PG loop on detIds in the cluster
      //std::cerr << "AAAAAAAAAAAAAAAAAAA      clusterDetIds.size()" << clusterDetIds.size() << std::endl;
      clusterSize->Fill(clusterDetIds.size());

      for(std::vector<DetId>::const_iterator detitr = clusterDetIds.begin(); detitr != clusterDetIds.end(); ++detitr)
	{
	  //Here I use the "find" on a digi collection... I have been warned...
	  if ((*detitr).det() != DetId::Ecal) { continue; }
	  if ((*detitr).subdetId() != EcalBarrel) { continue; }
	  EcalRecHitCollection::const_iterator thishit = hits->find((*detitr));
	  if (thishit == hits->end()) { continue; }
	  //The checking above should no longer be needed...... as only those in the cluster would already have rechits..
		
	  EcalRecHit myhit = (*thishit);
	    
	  double thisamp = myhit.energy () ;

	  //	  std::cerr << "BBBBBBBB ThisAmp" << thisamp << std::endl;
	  

	  if (thisamp > minNoiseThr_) { numXtalsinE9++; } //PG 3 ADC counts
	  if (thisamp > secondMin) {secondMin = thisamp; secondTime = myhit.time(); secDet = (EBDetId)(*detitr);}
	  if (secondMin > ampli) {std::swap(ampli,secondMin); std::swap(time,secondTime); std::swap(maxDet,secDet);}
	} //PG loop on detIds in the cluster

      numberOfCosmics++;
      numberOfCosmics_->Fill(numberOfCosmics);

      //if ((ampli < minCosmicE1_) && (secondMin < minCosmicE2_)) continue; //Posibly add a third && (numXtalsinenergy<3) TEST IT FIRST
      //      std::cout << "---------------> MAXDET = " << maxDet << std::endl;
      cosmicCrystals.push_back (maxDet) ;     //è l'hot channel all'interno dei supercluster

      nCluster++;
    } //PG loop on basic clusters
  nCluster_->Fill(nCluster);
  return cosmicCrystals ;
}

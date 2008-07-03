
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

#include "CRUZET/Calibration/plugins/MuonAssociator.h"

#include <vector>

using namespace cms;
using namespace edm;
using namespace std;



MuonAssociator::MuonAssociator (const edm::ParameterSet& iConfig):
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



void MuonAssociator::analyze ( const edm::Event & iEvent, const  edm::EventSetup& iSetup)
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
  
  
  //--------------------------------------Muons------------------------
  //PG get the muons collection
  edm::Handle<reco::MuonCollection> MuonsHandle ;
  iEvent.getByLabel (m_MuonsInputTag, MuonsHandle) ;     
  //  std::cerr << "found " << MuonsHandle->size () << " Muons" << std::endl ;
  nMuons_->Fill(MuonsHandle->size ());
  if (!MuonsHandle->size ()) return ;
  
  
  //   //loop over muons
  //   for (reco::MuonCollection::const_iterator muIt = MuonsHandle->begin () ;
  //        muIt != MuonsHandle->end (); ++muIt)
  //     {}


  //--------------------------------------Tracks------------------------
  //get reco tracks 
  edm::Handle<reco::TrackCollection> recoTracks;
  iEvent.getByLabel(m_TracksInputTag, recoTracks);
  //  std::cerr << "found " << recoTracks->size () << " Tracks" << std::endl ;
  nTracks_->Fill(recoTracks->size());
  if (!recoTracks->size ()) return ;

  TrajectoryStateTransform tsTransform;
  edm::ESHandle<MagneticField> bField;
  iSetup.get<IdealMagneticFieldRecord>().get(bField);

  //------------test-----------
  std::vector<EBDetId> ecalCosmics = findCosmicsInEvent(iEvent,iSetup);
  if (ecalCosmics.size () != 2) return; // troppo restrittivo!!!???
  for(std::vector<EBDetId>::const_iterator ECit = ecalCosmics.begin () ;
      ECit != ecalCosmics.end () ;
      ++ECit )
    {
      ECitOccupancy->Fill(ECit->iphi(),ECit->ieta());
      if(ECit->ieta() > 0) ECitPosUP++;
      else if(ECit->ieta() < 0) ECitNegUP++;
      
    }
  //------------test-----------

  for(reco::TrackCollection::const_iterator recoTrack = recoTracks->begin();
      recoTrack != recoTracks->end(); ++recoTrack)
    { 

      FreeTrajectoryState innerState = tsTransform.innerFreeState(*recoTrack,&*bField);
      FreeTrajectoryState outerState = tsTransform.outerFreeState(*recoTrack,&*bField);

      double ECALrad = 131. ;  //131 - 22 (altezza Xtals)

      double x1 = innerState.position() . x () ;
      double y1 = innerState.position() . y () ;
      double z1 = innerState.position() . z () ;
      double mag1 = innerState.position() . mag();

      double x2 = outerState.position() . x () ;
      double y2 = outerState.position() . y () ;
      double z2 = outerState.position() . z () ;
      double mag2 = outerState.position() . mag();

      //---------solo tracce sotto-----------------
      //if(innerState.position().phi() > 0 || outerState.position().phi() > 0) continue;
      //---------solo tracce sotto-----------------

      if(mag1 == mag2) continue;                                        //è lo stesso punto!!! non so tirare una retta.
      
      double dx = x2 - x1 ;
      double dy = y2 - y1 ;
      
      double rad = x1 * dx + y1 * dy ;
      rad = rad * rad - (dx*dx + dy*dy) * (x1*x1 + y1*y1 - ECALrad*ECALrad) ;
      if (rad >= 0) rad = sqrt (rad);
      else continue; /* non incontro il cilindro (non c'è soluzione) */
      
      double alpha1 =  (- x1*dx - y1*dy + rad) / (dx*dx + dy*dy) ;
      double alpha2 =  (- x1*dx - y1*dy - rad) / (dx*dx + dy*dy) ;

      //      std::cerr << "-------------->>>>>>>>> alfa1 = " << alpha1  << "   -------------->>>>>>>>> alfa2 = " << alpha2 << std::endl;
      //      std::cerr << " x1 = " << x1 << "; y1 = " << y1 << "; x2 = " << x2 << "; y2 = " << y2 << "; dx = " << dx << "; dy = " << dy << "; rad = " << rad << std::endl;
      //      std::cerr << " z1 = " << z1 << "; z2 = " << z2 << std::endl;

      double xi1 = x1 + alpha1 * (x2-x1);
      double yi1 = y1 + alpha1 * (y2-y1);
      double zi1 = z1 + alpha1 * (z2-z1);

      double xi2 = x1 + alpha2 * (x2-x1);
      double yi2 = y1 + alpha2 * (y2-y1);
      double zi2 = z1 + alpha2 * (z2-z1);

      GlobalPoint p1(xi1,yi1,zi1);   //GlobalPoints on ECAL surface
      GlobalPoint p2(xi2,yi2,zi2);

      if(p1.eta() > 1.479 || p1.eta() < -1.479 || p2.eta() > 1.479 || p2.eta() < -1.479) continue;  //controllo la z (eta) entrambi i punti dentro!
      //      std::cerr << "r = xi1 + yi1 = " << (sqrt(xi1*xi1 + yi1*yi1)) << ";     r = xi2 + yi2 = " << (sqrt(xi2*xi2 + yi2*yi2)) << std::endl;
      
      if(p1.eta() > 0) GPointPos++;
      else if (p1.eta() < 0) GPointNeg++;

      if(p2.eta() > 0) GPointPos++;
      else if (p2.eta() < 0) GPointNeg++;

      //----------------------findCosmicsInEvent()----------------------
      //std::vector<EBDetId> ecalCosmics = findCosmicsInEvent(iEvent,iSetup);
      //if (ecalCosmics.size () != 2) continue ; // troppo restrittivo!!!
      //loop ecal cosmics 
      double nearest1 = 0.9;
      double nearest2 = 0.9;
      double nearestDEta1 = -1, nearestDEta2 = -1;
      double nearestDPhi1 = -1, nearestDPhi2 = -1;
      for(std::vector<EBDetId>::const_iterator ECit = ecalCosmics.begin () ;
          ECit != ecalCosmics.end () ;
          ++ECit )
        {
	  if(ECit->ieta() > 0) ECitPos++;
	  else if(ECit->ieta() < 0) ECitNeg++;

	  double dphi1 = 0;
          double deta1 = ((ECit->ieta() - 0.5) * 0.01745 - p1.eta());     //non è necessaria la correzione!!!
	  double fabsDeta1 = fabs(deta1);
	  if((ECit->iphi() - 0.5) * 0.01745 > twopi/2) dphi1 = ((ECit->iphi() - 0.5) * 0.01745 - twopi - p1.phi());
	  else dphi1 = ((ECit->iphi() - 0.5) * 0.01745 - p1.phi());
	  double fabsDphi1 = fabs(dphi1);
	  if (fabsDphi1>(twopi-fabsDphi1)) {fabsDphi1 = twopi-fabsDphi1;}
	  double deltaR_1 = sqrt(fabsDphi1 * fabsDphi1 + fabsDeta1 * fabsDeta1);
	  
	  double dphi2 = 0;
          double deta2 = ((ECit->ieta () - 0.5) * 0.0175 - p2.eta()); 
	  double fabsDeta2 = fabs(deta2);
	  if((ECit->iphi() - 0.5) * 0.01745 > twopi/2) dphi2 = ((ECit->iphi() - 0.5) * 0.01745 - twopi - p2.phi());
	  else dphi2 = ((ECit->iphi() - 0.5) * 0.01745 - p2.phi());
	  double fabsDphi2 = fabs(dphi2);
	  if (fabsDphi2>(twopi-fabsDphi2)) {fabsDphi2 = twopi-fabsDphi2;}
	  double deltaR_2 = sqrt(fabsDphi2 * fabsDphi2 + fabsDeta2 * fabsDeta2);

	  if(deltaR_1 < 0.3 && deltaR_1 < nearest1) 
	    {
	      nearest1 = deltaR_1; 
	      nearestDEta1 = deta1;
	      nearestDPhi1 = dphi1;
	    }
	  if(deltaR_2 < 0.3 && deltaR_2 < nearest2) 
	    {
	      nearest2 = deltaR_2;
	      nearestDEta2 = deta2;
	      nearestDPhi2 = dphi2;
	    }
	}// loop ecal cosmics
            
      GPointEner1->Fill(nearest1);   //sommo tutto sullo stesso histo
      GPointEner1->Fill(nearest2);
      DEtaVSDPhi1->Fill(nearestDEta1,nearestDPhi1);
      DEtaVSDPhi1->Fill(nearestDEta2,nearestDPhi2);


      //------Globstd::vector<EBDetID>alPoints Occupancy----------
      GPOccupancy_->Fill(p1.phi(), p1.eta() );
      GPOccupancy_->Fill(p2.phi(), p2.eta() );

      MagVSPhiInner_->Fill(innerState.position().phi(),sqrt(x1*x1 + y1*y1));
      MagVSPhiOuter_->Fill(outerState.position().phi(),sqrt(x2*x2 + y2*y2));

      yVSxInner_->Fill(x1,y1);
      yVSxOuter_->Fill(x2,y2);

      //      std::cerr << "GlobalPoint---------------->>>>>>>> ieta = " << p1.eta() << std::endl;

      //check with RecHits
      double max_E1 = 0.09, max_E2 = 0.09;    //GeV  (10 ADC channels)
      double R1_Seed = 100., R2_Seed = 100.;
      double E1=-10,E2=-10;
      int count1=0,count2=0;
      for(int ii = 0; ii < (int)hitCrystals.size(); ++ii )
	{
	  deta1 = fabs( (hitCrystals.at(ii).ieta()-0.5)*0.01745 - p1.eta());     
	  dphi1 = fabs( (hitCrystals.at(ii).iphi()-0.5)*0.01745 - p1.phi());  //  - twopi/2
	  if (dphi1>(twopi-dphi1)) {dphi1 = twopi-dphi1;}

	  deta2 = fabs( (hitCrystals.at(ii).ieta()-0.5)*0.01745 - p2.eta());         
	  dphi2 = fabs( (hitCrystals.at(ii).iphi()-0.5)*0.01745 - p2.phi());  //  - twopi/2
	  if (dphi2>(twopi-dphi2)) {dphi2 = twopi-dphi2;}

	  R1 = sqrt(deta1*deta1 + dphi1*dphi1);
	  R2 = sqrt(deta2*deta2 + dphi2*dphi2);

	  //std::cerr << " ___________________________deta1 = " << deta1 << "; dphi1 = " << dphi1 << "; R1 = " << R1 << std::endl;
	  //std::cerr << " ___________________________deta2 = " << deta2 << "; dphi2 = " << dphi2 << "; R2 = " << R2 << std::endl;
	  
 	  if(R1 < 0.2) 
 	    if(hitCrystalsRecHit.at(ii).energy() > max_E1)
	      if(R1 < R1_Seed)
		{
		  //max_E1 = hitCrystalsRecHit.at(ii).energy();
		  R1_Seed = R1;	
		  E1 = hitCrystalsRecHit.at(ii).energy();
		  ++count1;
		}
	  
	  
	  if(R2 < 0.2) 
	    if(hitCrystalsRecHit.at(ii).energy() > max_E2)
	      if(R2 < R2_Seed)
		{
		  //max_E2 = hitCrystalsRecHit.at(ii).energy();
		  R2_Seed = R2;	
		  E2 = hitCrystalsRecHit.at(ii).energy();
		  ++count2;
		}
// 	  RSeedVSDetID1->Fill(R1_Seed);
// 	  RSeedVSDetID2->Fill(R2_Seed);
	}
      
      RSeedVSDetID1->Fill(R1_Seed);
      RSeedVSDetID2->Fill(R2_Seed);

      EVSR1->Fill(R1_Seed,E1);
      EVSR2->Fill(R2_Seed,E2);
      //      std::cerr << "count1 = " << count1 << "; count2 = " << count2 << std::endl;
      
      

      //---------------------------------------     PROVO AD INVERTIRE!!!


      //std::cout << "innerState.position().mag() = " << innerState.position().mag() << "; outerState.position().mag() = " << outerState.position().mag() << std::endl;

//       TrackDetMatchInfo info;
//       if((innerState.position().mag()) < (outerState.position().mag()))
// 	{
// 	  info = trackAssociator_.associate(iEvent, iSetup, parameters_, &innerState, &outerState);
// 	}
//       else
// 	{
// 	  info = trackAssociator_.associate(iEvent, iSetup, parameters_, &outerState, &innerState);
// 	}
      
      //--------------------------------------
      TrackDetMatchInfo info = trackAssociator_.associate(iEvent, iSetup, *recoTrack, parameters_);
      if(info.crossedEcalIds.size()==0) continue;
      
      //phiMu_->Fill(recoTrack->innerState.position().phi());
      
      //      std::cout << "Track (pt,eta,phi): " << recoTrack->pt() << " , " << recoTrack->eta() << " , " << recoTrack->phi() << std::endl;
      //      std::cout << "Ecal energy in crossed crystals based on RecHits: " << info.crossedEnergy(TrackDetMatchInfo::EcalRecHits) << std::endl;
      //      std::cout << "Number of muon segment matches: " << info.numberOfSegments() << std::endl;
      
//       std::cout << "-----------------------------DetID through RecHits----------------" << std::endl;
//       for(std::vector<const EcalRecHit*>::const_iterator CERHit = info.crossedEcalRecHits.begin();
//        	  CERHit != info.crossedEcalRecHits.end(); ++CERHit)
//        	{
// 	  EBDetId dummyRecHit (*CERHit);
	  
// 	  // 	  //std::cout << "crossedEcalRecHits[] = " << dummyRecHit->id() << std::endl;
// 	  // 	  //std::cout << "EBDetId ieta,iphi"<< dummyRecHit.ieta() << " , " << dummyRecHit.iphi() << std::endl;
	  
//        	}
//      std::cout << "------------------------------DetID---------------------" << std::endl;
      std::vector<EBDetId> FirstCluster;
      std::vector<EBDetId> SecondCluster;
      std::vector<DetId>::const_iterator CEDId = info.crossedEcalIds.begin();
      if (info.crossedEcalIds.at(0).det() != DetId::Ecal || info.crossedEcalIds.at(0).subdetId() != 1) continue;  //FIXME non dovrebbe essere necessario!
      EBDetId dummy (*CEDId);
      FirstCluster.push_back (dummy) ;
      int iphi1 = dummy.iphi ();
      
      if(find(hitCrystals.begin(),hitCrystals.end(),dummy) != hitCrystals.end()) 
	{
	  allOccupancyMatch_->Fill(dummy.iphi()*0.01745,dummy.ieta());
	  //energyMatch_->Fill(hitCrystals.energy());     //dovrei usare gli hitCrystalsRecHit
	}
      else 
	{
	  allOccupancyNoMatch_->Fill(dummy.iphi()*0.01745,dummy.ieta());
	  //energyNoMatch_->Fill(hitCrystals.energy());
	}
      
      allOccupancy_->Fill(dummy.iphi()*0.01744,dummy.ieta());

      int ii = 1;
      int deltaPhiMaxFirst = -1;

      double near_DetID1 = 0.9;
      double near_DetID2 = 0.9;
      for(++CEDId; CEDId != info.crossedEcalIds.end(); ++CEDId)
	{
	  if (info.crossedEcalIds.at(ii).det() != DetId::Ecal || info.crossedEcalIds.at(ii).subdetId() != 1) continue;  //FIXME non dovrebbe essere necessario!

	  EBDetId dummy (*CEDId);
	  //	  std::cout << "crossedEcalIds[] = " << dummy.rawId() << "; ieta = " << dummy.ieta() << "; iphi = " << dummy.iphi() << std::endl;

	  //-------------match DetID from RecHits VS DetID from recoTracks----------
	  if(find(hitCrystals.begin(),hitCrystals.end(),dummy) != hitCrystals.end()) 
	    {
	      allOccupancyMatch_->Fill(dummy.iphi()*0.01744,dummy.ieta());
	      //energyMatch_->Fill(hitCrystals.energy());
	    }
	  else 
	    {
	      allOccupancyNoMatch_->Fill(dummy.iphi()*0.01744,dummy.ieta());
	      //energyNoMatch_->Fill(hitCrystals.energy());
	    }

	  
	  //-------------deltaphi and containers-----------------------------
	  int deltaphi = deltaPhi (dummy.iphi(),iphi1);
	  if (deltaphi < 90)
	    {
	      FirstCluster.push_back(dummy);
	      if(deltaPhiMaxFirst < deltaphi) deltaPhiMaxFirst = deltaphi;
	    }
	  else
 	    SecondCluster.push_back(dummy);
	  ++ii;

	  allOccupancy_->Fill(dummy.iphi()*0.01744,dummy.ieta());

	  //------------- distanza DetID-----------------------
	  detaDetID1 = fabs( (dummy.ieta() - 0.5) * 0.01745 - p1.eta());       //non è necessaria la correzione!!
	  if((dummy.iphi() - 0.5) * 0.01745 > twopi/2) dphiDetID1 = fabs((dummy.iphi() - 0.5) * 0.01745 - twopi - p1.phi());
	  else dphiDetID1 = fabs( (dummy.iphi() - 0.5) * 0.01745 - p1.phi());
	  if (dphiDetID1 > (twopi-dphiDetID1)) {dphiDetID1 = twopi-dphiDetID1;}
	  
	  detaDetID2 = fabs( (dummy.ieta() - 0.5) * 0.01745 - p2.eta()); 
	  if((dummy.iphi() - 0.5) * 0.01745 > twopi/2) dphiDetID2 = fabs((dummy.iphi() - 0.5) * 0.01745 - twopi - p2.phi());
	  else dphiDetID2 = fabs( (dummy.iphi() - 0.5) * 0.01745 - p2.phi());
	  if (dphiDetID2 > (twopi-dphiDetID2)) {dphiDetID2 = twopi-dphiDetID2;}

	  RDetID1 = sqrt(detaDetID1*detaDetID1 + dphiDetID1*dphiDetID1);
	  RDetID2 = sqrt(detaDetID2*detaDetID2 + dphiDetID2*dphiDetID2);

 	  if(RDetID1 < 0.2 && RDetID1 < near_DetID1) 
	    {
	      near_DetID1 = RDetID1;
	     
	    }
	  
	  
	  if(RDetID2 < 0.2 && RDetID2 < near_DetID2) 
	    {
	      near_DetID2 = RDetID1;
			
	    }
	}

      GPointVSDetID1->Fill(near_DetID1);
      GPointVSDetID2->Fill(near_DetID2);
      



//       for(int kk=0; kk!=FirstCluster.size(); ++kk)
// 	{
	  
// 	}
	
      FirstCluster_->Fill(FirstCluster.size());

//       std::cout << "n crossedEcalIds = " << info.crossedEcalIds.size() << std::endl;
//       std::cout << "FirstCluster = " << FirstCluster.size() << std::endl;
//       std::cout << "SecondCluster = " << SecondCluster.size() << std::endl;

      //      std::cerr << "deltaPhiMaxFirst = " << deltaPhiMaxFirst << std::endl;
      //      std::cout << "------------------------------END------------------------" << std::endl;

      
      
      
    }
} 

// ------------ method called once each job just before starting event loop  ------------
void 
MuonAssociator::beginJob(const edm::EventSetup&)
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
  DEtaVSDPhi1 = fs->make <TH2F> ("DEtaVSDPhi1" ,"DEtaVSDPhi1",600,-0.3,0.3,600,-0.3,0.3) ;
  DEtaVSDPhi2 = fs->make <TH2F> ("DEtaVSDPhi2" ,"DEtaVSDPhi2",600,-0.3,0.3,600,-0.3,0.3) ;

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
MuonAssociator::endJob()
{
//   std::cerr << "GPointPos = " << GPointPos << std::endl;
//   std::cerr << "GPointNeg = " << GPointNeg << std::endl;

//   std::cerr << "ECitPos = " << ECitPos << std::endl;
//   std::cerr << "ECitNeg = " << ECitNeg << std::endl;

//   std::cerr << "ECitPosUP = " << ECitPosUP << std::endl;
//   std::cerr << "ECitNegUP = " << ECitNegUP << std::endl;
}


//-------------deltaPhi--------------------
int MuonAssociator::deltaPhi (int phi1, int phi2)
{

  int deltaphi=abs(phi1-phi2);
  if (deltaphi > 360) deltaphi -= 360;
  if (deltaphi > 180) deltaphi = 360-deltaphi;
  return deltaphi;
}


//!PG find a cosmic in the event
//!PG from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CCEcal/EcalCosmicRuns/CRUZET/Calibration/plugins/EcalCosmicsHists.cc?revision=1.5&view=markup
std::vector<EBDetId>
MuonAssociator::findCosmicsInEvent (const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
 

      //      std::cerr << "------->>> clus->energy(), clus->phi(), clus->eta() =  " << energy << " " << phi <<" " << eta << std::endl;
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


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

#include "CaloOnlineTools/EcalTools/plugins/MuonAssociatorEcalIP.h"

#include <vector>

using namespace cms;
using namespace edm;
using namespace std;



MuonAssociatorEcalIP::MuonAssociatorEcalIP (const edm::ParameterSet& iConfig):
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
  hXtal = 23.;
  
  //MaskedCannels
  std::vector<int> listDefaults;
  listDefaults.push_back(-1);
  maskedChannels_ = iConfig.getUntrackedParameter <std::vector<int> >("maskedChannels", listDefaults);


  // TrackAssociator parameters
  edm::ParameterSet parameters = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
  parameters_.loadParameters( parameters );
  trackAssociator_.useDefaultPropagator();
}



void MuonAssociatorEcalIP::analyze ( const edm::Event & iEvent, const  edm::EventSetup& iSetup)
{
  
 //--------------------------------------Muons------------------------
  //PG get the muons collection
  edm::Handle<reco::MuonCollection> MuonsHandle ;
  iEvent.getByLabel (m_MuonsInputTag, MuonsHandle) ;     
  //std::cerr << "found " << MuonsHandle->size () << " Muons" << std::endl ;
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
  //std::cerr << "found " << recoTracks->size () << " Tracks" << std::endl ;
  nTracks_->Fill(recoTracks->size());
  if (recoTracks->size() == 2 )                    //chiedo eventi a 2 tracce
    {
      TrajectoryStateTransform tsTransform;
      edm::ESHandle<MagneticField> bField;
      iSetup.get<IdealMagneticFieldRecord>().get(bField);
      
      double x1 = 0;
      double y1 = 0;
      double z1 = 0;
      double mag1 = 0;
      
      double x2 = 0;
      double y2 = 0;
      double z2 = 0;
      double mag2 = 0;
      
      double ECALinRad = 131. ;
      double ECALoutRad = 154. ;
      
      bool sopra = false, sotto = false;
      for(reco::TrackCollection::const_iterator recoTrack = recoTracks->begin();
	  recoTrack != recoTracks->end(); ++recoTrack)
	{ 
	  
	  FreeTrajectoryState innerState = tsTransform.innerFreeState(*recoTrack,&*bField);
	  FreeTrajectoryState outerState = tsTransform.outerFreeState(*recoTrack,&*bField);
	  
	  //---------chiedo innerState alla traccia sotto, outerState alla sopra-----------------
	  InnerStateOccupancy_->Fill(innerState.position().phi(),innerState.position().eta());
	  OuterStateOccupancy_->Fill(outerState.position().phi(),outerState.position().eta());
	  
	  if(innerState.position().y() > 0)
	    {
	      sopra = true;
	      
	      IPdistTopTracks_->Fill(fabs(recoTrack->dxy()));
	      
	      x1 = innerState.position() . x () ;
	      y1 = innerState.position() . y () ;
	      z1 = innerState.position() . z () ;
	      mag1 = innerState.position() . mag();
	    }
	  else if(innerState.position().y() < 0)
	    {
	      sotto = true;
	      
	      IPdistBottomTracks_->Fill(fabs(recoTrack->dxy()));
	    
	      x2 = outerState.position() . x () ;
	      y2 = outerState.position() . y () ;
	      z2 = outerState.position() . z () ;
	      mag2 = outerState.position() . mag();
	    }
	}
      if(sopra == true && sotto == true)
	{
	  GlobalPoint point1(x1,y1,z1);
	  GlobalPoint point2(x2,y2,z2);
	  
	  //------dentro e fuori ECAL lavorando sugli stessi GPoint
	  std::pair <GlobalPoint,GlobalPoint> CrossIn = getEcalCross(point1,point2,ECALinRad);
	  std::pair <GlobalPoint,GlobalPoint> CrossOut = getEcalCross(point1,point2,ECALoutRad);
	  
	  //1 = EcalTop; 2 = EcalBottom
	  GlobalPoint pIn1 = CrossIn.first;
	  GlobalPoint pIn2 = CrossIn.second;
	  
	  GlobalPoint pOut1 = CrossOut.first;
	  GlobalPoint pOut2 = CrossOut.second;
	  
	  /*----------------------------------------------------
	    1 poss: la retta non iterseca ECAL                -> nessuna soluzione
	    2 poss: la retta tange ECAL sulla sup esterna     -> 2 soluz coincidenti e nessun segmento interno
	    3 poss: la retta interseca la sup esterna di ECAL -> 1cluster e 1segmento
	    4 poss: la retta interseca la sup interna di ECAL -> 2clusters e 2 segmenti
	    -----------------------------------------------------*/
	  
	  //if(pOut1.mag() > 0.01 && pOut2.mag() > 0.01)
	  if(pIn1.mag() > 0.01 && pIn2.mag() > 0.01)
	    {
	      posiXYtracks_->Fill(pIn1.x(),pIn1.y());
	      posiXYtracks_->Fill(pIn2.x(),pIn2.y());
	      posiZtracks_->Fill(pIn1.z());
	      posiZtracks_->Fill(pIn2.z());
	      GPOccupancyz_->Fill(pIn1.phi(),pIn1.z());
	      GPOccupancyz_->Fill(pIn2.phi(),pIn2.z());
 	      GPOccupancy_->Fill(pIn1.phi(),pIn1.eta());
 	      GPOccupancy_->Fill(pIn2.phi(),pIn2.eta());
	      double alfaTrack = getImpactParameter(pIn1,pIn2);
	      GlobalPoint IPTrack = pIn1 + alfaTrack*(pIn2 - pIn1);
	      IPdistGPoints_->Fill(IPTrack.mag());
	    }
	  
	  // 	double lunghTop = getEcalXLenght(pIn1,pOut1);
	  // 	double lunghBot = getEcalXLenght(pIn2,pOut2);
	  
	  
	}
    }
  //------------------------------------------------------------------
  //-------------------------------------RecHits----------------------
  //get the EB RecHits collection
  edm::Handle<EcalRecHitCollection> EBRecHitsHandle ;
  iEvent.getByLabel (m_EBRecHitsInputTag, EBRecHitsHandle) ;
  //std::cerr << "found " << EBRecHitsHandle->size () << " EBRecHits" << std::endl ;
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
  
  //--------position and energy of BasicClusters-----------------
  
  std::vector<GlobalPoint> position;
  std::vector<double> energy;
  
  int ievt = iEvent.id().event();
  edm::Handle<reco::BasicClusterCollection> bccHandle;  
  edm::Handle<reco::BasicClusterCollection> eccHandle;  
 

  iEvent.getByLabel(barrelClusterCollection_, bccHandle);
  if (!(bccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << barrelClusterCollection_ << " not available";
      return;
    }
  LogDebug("EcalCosmicsHists") << "event " << ievt;
  
  iEvent.getByLabel(endcapClusterCollection_, eccHandle);
  if (!(eccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << endcapClusterCollection_ << " not available";
      return;
    }
  
  Handle<EcalRecHitCollection> hits;
  iEvent.getByLabel(m_EBRecHitsInputTag, hits);
  if (!(eccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << m_EBRecHitsInputTag << " not available";
      return;
    }
  
  //--------------------loop on basic clusters-----------------------
  double nCluster = 0;
  std::vector<int> clustSize;
  const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();
  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); 
       clus != clusterCollection_p->end(); 
       ++clus)
    {
      GlobalPoint posCluster(clus->x(),clus->y(),clus->z());
      position.push_back(posCluster);

      energy.push_back(clus->energy());
      ClusEner_->Fill(clus->energy());

      std::vector<DetId> clusterDetIds = clus->getHitsByDetId();//get these from the cluster
      clusterSize->Fill(clusterDetIds.size());
      clustSize.push_back(clusterDetIds.size());

      nCluster++;
    }


  nCluster_->Fill(nCluster);

  //-------------only 2cluster events------
  if(energy.size() != 2 || position.size() != 2) return;    
  //if((position.at(0).y() > 0 && position.at(1).y() > 0) || (position.at(0).y() < 0 && position.at(1).y() < 0)) return;   //no doppi fisici nel buco di ECAL
  if(fabs(deltaPhi(position.at(0).phi(),position.at(1).phi()) - twopi/2.) > 0.6) return;                                   //più raffinato
  deltaPhi_->Fill(fabs(deltaPhi(position.at(0).phi(),position.at(1).phi()) - twopi/2.));
  //deltaPhi_->Fill(deltaPhi(position.at(0).phi(),position.at(1).phi()));


  posiXY_->Fill(position.at(0).x(),position.at(0).y());
  posiXY_->Fill(position.at(1).x(),position.at(1).y());
  posiZ_->Fill(position.at(0).z());
  posiZ_->Fill(position.at(1).z());


  double alfa = getImpactParameter(position.at(0),position.at(1));
  GlobalPoint IP = position.at(0) + alfa*(position.at(1) - position.at(0));

  double magIP = IP.mag();
  //double IPxy = sqrt(IP.x()*IP.x() + IP.y()*IP.y());      //WRONG

  //--------------FillingHisto-----------
  IPVSdeltaPhi_->Fill(deltaPhi(position.at(0).phi(),position.at(1).phi()),magIP);

  IPdist_->Fill(magIP);
  //IPxydist_->Fill(IPxy);                                  //WRONG

  clusterSizeVSIP_->Fill(magIP,clustSize.at(0));    //volendo (clustSize.at(0) + clustSize.at(1))/2
  clusterSizeVSIP_->Fill(magIP,clustSize.at(1));

  dEondXVSIP_->Fill(magIP,energy.at(0)/hXtal);      //volendo anche qui media
  dEondXVSIP_->Fill(magIP,energy.at(1)/hXtal);

  if(position.at(0).y() > 0 && position.at(1).y() < 0) 
    {
      DiffClusVSIP_->Fill(magIP,energy.at(1) - energy.at(0));
      RatioClusVSIP_->Fill(magIP,energy.at(1)/energy.at(0));
      IPplane_->Fill(position.at(0).phi(),position.at(0).eta(),magIP);
    }
  else if(position.at(0).y() < 0 && position.at(1).y() > 0) 
    {
      DiffClusVSIP_->Fill(magIP,energy.at(0) - energy.at(1));
      RatioClusVSIP_->Fill(magIP,energy.at(0)/energy.at(1));
      IPplane_->Fill(position.at(1).phi(),position.at(1).eta(),magIP);
    }

  //----------------TAGLIO---------------

  if(magIP > 10) return;                //voglio la direzione puntante; 10cm
  
  if(position.at(0).y() > 0) dEondX_Up_->Fill(energy.at(0)/hXtal);
  else if(position.at(0).y() < 0) dEondX_Down_->Fill(energy.at(0)/hXtal);

  if(position.at(1).y() > 0) dEondX_Up_->Fill(energy.at(1)/hXtal);
  else if(position.at(1).y() < 0) dEondX_Down_->Fill(energy.at(1)/hXtal);

  if(position.at(0).z() > 0) dEondX_Plus_->Fill(energy.at(0)/hXtal);
  else if(position.at(0).z() < 0) dEondX_Minus_->Fill(energy.at(0)/hXtal);

  if(position.at(1).z() > 0) dEondX_Plus_->Fill(energy.at(1)/hXtal);
  else if(position.at(1).z() < 0) dEondX_Minus_->Fill(energy.at(1)/hXtal);

  dEondX_->Fill(energy.at(0)/hXtal);
  dEondX_->Fill(energy.at(1)/hXtal);

  if(position.at(0).y() > 0 && position.at(1).y() < 0) 
    DiffClus_->Fill(energy.at(1) - energy.at(0));
  else if(position.at(0).y() < 0 && position.at(1).y() > 0) 
    DiffClus_->Fill(energy.at(0) - energy.at(1));
}





  
//   std::vector<EBDetId> ecalCosmics = findCosmicsInEvent(iEvent,iSetup);
//   std::vector<double> clusterEnergy = getClusterEnergy(iEvent,iSetup);
//   if (ecalCosmics.size() != 2 || clusterEnergy.size() != 2) return; //chiedo eventi con 2 BasicClusters in ECAL

//   double AvEnergy = (clusterEnergy.at(0) + clusterEnergy.at(1)) / 2;

//   int cont = 0;
//   for(std::vector<EBDetId>::const_iterator ECit = ecalCosmics.begin () ;
//       ECit != ecalCosmics.end () ;
//       ++ECit )
//     {
//       ECitOccupancy->Fill(ECit->iphi(),ECit->ieta());
//       if(ECit->ieta() > 0) ECitPosUP++;
//       else if(ECit->ieta() < 0) ECitNegUP++;

//       //       if(cont == 0) GlobalPoint p1 (ECit->x(),ECit->y(),ECit->z());
//       //       if(cont == 1) GlobalPoint p2 (ECit->x(),ECit->y(),ECit->z());
      
//       cont++;
//     }
  
  
  
//   //  double path = getImpactParameter();
  

//   double x1 = 0;
//   double y1 = 0;
//   double z1 = 0;
//   double mag1 = 0;

//   double x2 = 0;
//   double y2 = 0;
//   double z2 = 0;
//   double mag2 = 0;

//   double ECALinRad = 131. ;
//   double ECALoutRad = 154. ;




//   LvsPhiTop_->Fill(pOut1.phi(),lunghTop);
//   LvsPhiBot_->Fill(pOut2.phi(),lunghBot);
//   AvEnergyVsL_->Fill(lunghTop,AvEnergy);
//   //AvEnergyVsL_->Fill(lunghBot,AvEnergy);
//   dEondX_->Fill(AvEnergy/lunghTop);
      

  
//   //----------------------findCosmicsInEvent()----------------------
//   double nearest1 = 0.9;
//   double nearest2 = 0.9;
//   double nearestDEta1 = -1, nearestDEta2 = -1;
//   double nearestDPhi1 = -1, nearestDPhi2 = -1;
//   for(std::vector<EBDetId>::const_iterator ECit = ecalCosmics.begin () ;
//       ECit != ecalCosmics.end () ;
//       ++ECit )
//     {
//       if(ECit->ieta() > 0) ECitPos++;
//       else if(ECit->ieta() < 0) ECitNeg++;
      
//       double dphi1 = 0;
//       double deta1 = ((ECit->ieta() - 0.5) * 0.01745 - pIn1.eta());     //non è necessaria la correzione!!!
//       double fabsDeta1 = fabs(deta1);
//       if((ECit->iphi() - 0.5) * 0.01745 > twopi/2) dphi1 = ((ECit->iphi() - 0.5) * 0.01745 - twopi - pIn1.phi());
//       else dphi1 = ((ECit->iphi() - 0.5) * 0.01745 - pIn1.phi());
//       double fabsDphi1 = fabs(dphi1);
//       if (fabsDphi1>(twopi-fabsDphi1)) {fabsDphi1 = twopi-fabsDphi1;}
//       double deltaR_1 = sqrt(fabsDphi1 * fabsDphi1 + fabsDeta1 * fabsDeta1);
      
//       double dphi2 = 0;
//       double deta2 = ((ECit->ieta () - 0.5) * 0.0175 - pIn2.eta()); 
//       double fabsDeta2 = fabs(deta2);
//       if((ECit->iphi() - 0.5) * 0.01745 > twopi/2) dphi2 = ((ECit->iphi() - 0.5) * 0.01745 - twopi - pIn2.phi());
//       else dphi2 = ((ECit->iphi() - 0.5) * 0.01745 - pIn2.phi());
//       double fabsDphi2 = fabs(dphi2);
//       if (fabsDphi2>(twopi-fabsDphi2)) {fabsDphi2 = twopi-fabsDphi2;}
//       double deltaR_2 = sqrt(fabsDphi2 * fabsDphi2 + fabsDeta2 * fabsDeta2);
      
//       if(deltaR_1 < 0.3 && deltaR_1 < nearest1) 
// 	{
// 	  nearest1 = deltaR_1; 
// 	  nearestDEta1 = deta1;
// 	  nearestDPhi1 = dphi1;
// 	}
//       if(deltaR_2 < 0.3 && deltaR_2 < nearest2) 
// 	{
// 	  nearest2 = deltaR_2;
// 	  nearestDEta2 = deta2;
// 	  nearestDPhi2 = dphi2;
// 	}
//     }// loop ecal cosmics
  
//   GPointEner1->Fill(nearest1);   //sommo tutto sullo stesso histo
//   GPointEner1->Fill(nearest2);
//   DEtaVSDPhi1->Fill(nearestDEta1,nearestDPhi1);
//   DEtaVSDPhi1->Fill(nearestDEta2,nearestDPhi2);
  
  
//   //------Globstd::vector<EBDetID>alPoints Occupancy----------
//   GPOccupancy_->Fill(pIn1.phi(), pIn1.eta() );
//   GPOccupancy_->Fill(pIn2.phi(), pIn2.eta() );
  
//   // MagVSPhiInner_->Fill(innerState.position().phi(),sqrt(x1*x1 + y1*y1));
//   // MagVSPhiOuter_->Fill(outerState.position().phi(),sqrt(x2*x2 + y2*y2));
  
//   yVSxInner_->Fill(x1,y1);
//   yVSxOuter_->Fill(x2,y2);

//   std::cerr << "GlobalPoint---------------->>>>>>>> ieta = " << p1.eta() << std::endl;

//       //check with RecHits
//       double max_E1 = 0.09, max_E2 = 0.09;    //GeV  (10 ADC channels)
//       double R1_Seed = 100., R2_Seed = 100.;
//       double E1=-10,E2=-10;
//       int count1=0,count2=0;
//       for(int ii = 0; ii < (int)hitCrystals.size(); ++ii )
// 	{
// 	  deta1 = fabs( (hitCrystals.at(ii).ieta()-0.5)*0.01745 - p1.eta());     
// 	  dphi1 = fabs( (hitCrystals.at(ii).iphi()-0.5)*0.01745 - p1.phi());  //  - twopi/2
// 	  if (dphi1>(twopi-dphi1)) {dphi1 = twopi-dphi1;}

// 	  deta2 = fabs( (hitCrystals.at(ii).ieta()-0.5)*0.01745 - p2.eta());         
// 	  dphi2 = fabs( (hitCrystals.at(ii).iphi()-0.5)*0.01745 - p2.phi());  //  - twopi/2
// 	  if (dphi2>(twopi-dphi2)) {dphi2 = twopi-dphi2;}

// 	  R1 = sqrt(deta1*deta1 + dphi1*dphi1);
// 	  R2 = sqrt(deta2*deta2 + dphi2*dphi2);

// 	  //std::cerr << " ___________________________deta1 = " << deta1 << "; dphi1 = " << dphi1 << "; R1 = " << R1 << std::endl;
// 	  //std::cerr << " ___________________________deta2 = " << deta2 << "; dphi2 = " << dphi2 << "; R2 = " << R2 << std::endl;
	  
//  	  if(R1 < 0.2) 
//  	    if(hitCrystalsRecHit.at(ii).energy() > max_E1)
// 	      if(R1 < R1_Seed)
// 		{
// 		  //max_E1 = hitCrystalsRecHit.at(ii).energy();
// 		  R1_Seed = R1;	
// 		  E1 = hitCrystalsRecHit.at(ii).energy();
// 		  ++count1;
// 		}
	  
	  
// 	  if(R2 < 0.2) 
// 	    if(hitCrystalsRecHit.at(ii).energy() > max_E2)
// 	      if(R2 < R2_Seed)
// 		{
// 		  //max_E2 = hitCrystalsRecHit.at(ii).energy();
// 		  R2_Seed = R2;	
// 		  E2 = hitCrystalsRecHit.at(ii).energy();
// 		  ++count2;
// 		}
// // 	  RSeedVSDetID1->Fill(R1_Seed);
// // 	  RSeedVSDetID2->Fill(R2_Seed);
// 	}
      
//       RSeedVSDetID1->Fill(R1_Seed);
//       RSeedVSDetID2->Fill(R2_Seed);

//       EVSR1->Fill(R1_Seed,E1);
//       EVSR2->Fill(R2_Seed,E2);
//       std::cerr << "count1 = " << count1 << "; count2 = " << count2 << std::endl;
      
      

//       //---------------------------------------     PROVO AD INVERTIRE!!!


//       //std::cout << "innerState.position().mag() = " << innerState.position().mag() << "; outerState.position().mag() = " << outerState.position().mag() << std::endl;

// //       TrackDetMatchInfo info;
// //       if((innerState.position().mag()) < (outerState.position().mag()))
// // 	{
// // 	  info = trackAssociator_.associate(iEvent, iSetup, parameters_, &innerState, &outerState);
// // 	}
// //       else
// // 	{
// // 	  info = trackAssociator_.associate(iEvent, iSetup, parameters_, &outerState, &innerState);
// // 	}
      
//       //--------------------------------------
//       TrackDetMatchInfo info = trackAssociator_.associate(iEvent, iSetup, *recoTrack, parameters_);
//       if(info.crossedEcalIds.size()==0) continue;
      
//       //phiMu_->Fill(recoTrack->innerState.position().phi());
      
//       std::cout << "Track (pt,eta,phi): " << recoTrack->pt() << " , " << recoTrack->eta() << " , " << recoTrack->phi() << std::endl;
//       std::cout << "Ecal energy in crossed crystals based on RecHits: " << info.crossedEnergy(TrackDetMatchInfo::EcalRecHits) << std::endl;
//       std::cout << "Number of muon segment matches: " << info.numberOfSegments() << std::endl;
      
// //       std::cout << "-----------------------------DetID through RecHits----------------" << std::endl;
// //       for(std::vector<const EcalRecHit*>::const_iterator CERHit = info.crossedEcalRecHits.begin();
// //        	  CERHit != info.crossedEcalRecHits.end(); ++CERHit)
// //        	{
// // 	  EBDetId dummyRecHit (*CERHit);
	  
// // 	  // 	  //std::cout << "crossedEcalRecHits[] = " << dummyRecHit->id() << std::endl;
// // 	  // 	  //std::cout << "EBDetId ieta,iphi"<< dummyRecHit.ieta() << " , " << dummyRecHit.iphi() << std::endl;
	  
// //        	}
//       std::cout << "------------------------------DetID---------------------" << std::endl;
//       std::vector<EBDetId> FirstCluster;
//       std::vector<EBDetId> SecondCluster;
//       std::vector<DetId>::const_iterator CEDId = info.crossedEcalIds.begin();
//       if (info.crossedEcalIds.at(0).det() != DetId::Ecal || info.crossedEcalIds.at(0).subdetId() != 1) continue;  //FIXME non dovrebbe essere necessario!
//       EBDetId dummy (*CEDId);
//       FirstCluster.push_back (dummy) ;
//       int iphi1 = dummy.iphi ();
      
//       if(find(hitCrystals.begin(),hitCrystals.end(),dummy) != hitCrystals.end()) 
// 	{
// 	  allOccupancyMatch_->Fill(dummy.iphi()*0.01745,dummy.ieta());
// 	  //energyMatch_->Fill(hitCrystals.energy());     //dovrei usare gli hitCrystalsRecHit
// 	}
//       else 
// 	{
// 	  allOccupancyNoMatch_->Fill(dummy.iphi()*0.01745,dummy.ieta());
// 	  //energyNoMatch_->Fill(hitCrystals.energy());
// 	}
      
//       allOccupancy_->Fill(dummy.iphi()*0.01744,dummy.ieta());

//       int ii = 1;
//       int deltaPhiMaxFirst = -1;

//       double near_DetID1 = 0.9;
//       double near_DetID2 = 0.9;
//       for(++CEDId; CEDId != info.crossedEcalIds.end(); ++CEDId)
// 	{
// 	  if (info.crossedEcalIds.at(ii).det() != DetId::Ecal || info.crossedEcalIds.at(ii).subdetId() != 1) continue;  //FIXME non dovrebbe essere necessario!

// 	  EBDetId dummy (*CEDId);
// 	  std::cout << "crossedEcalIds[] = " << dummy.rawId() << "; ieta = " << dummy.ieta() << "; iphi = " << dummy.iphi() << std::endl;

// 	  //-------------match DetID from RecHits VS DetID from recoTracks----------
// 	  if(find(hitCrystals.begin(),hitCrystals.end(),dummy) != hitCrystals.end()) 
// 	    {
// 	      allOccupancyMatch_->Fill(dummy.iphi()*0.01744,dummy.ieta());
// 	      //energyMatch_->Fill(hitCrystals.energy());
// 	    }
// 	  else 
// 	    {
// 	      allOccupancyNoMatch_->Fill(dummy.iphi()*0.01744,dummy.ieta());
// 	      //energyNoMatch_->Fill(hitCrystals.energy());
// 	    }

	  
// 	  //-------------deltaphi and containers-----------------------------
// 	  int deltaphi = deltaPhi (dummy.iphi(),iphi1);
// 	  if (deltaphi < 90)
// 	    {
// 	      FirstCluster.push_back(dummy);
// 	      if(deltaPhiMaxFirst < deltaphi) deltaPhiMaxFirst = deltaphi;
// 	    }
// 	  else
//  	    SecondCluster.push_back(dummy);
// 	  ++ii;

// 	  allOccupancy_->Fill(dummy.iphi()*0.01744,dummy.ieta());

// 	  //------------- distanza DetID-----------------------
// 	  detaDetID1 = fabs( (dummy.ieta() - 0.5) * 0.01745 - p1.eta());       //non è necessaria la correzione!!
// 	  if((dummy.iphi() - 0.5) * 0.01745 > twopi/2) dphiDetID1 = fabs((dummy.iphi() - 0.5) * 0.01745 - twopi - p1.phi());
// 	  else dphiDetID1 = fabs( (dummy.iphi() - 0.5) * 0.01745 - p1.phi());
// 	  if (dphiDetID1 > (twopi-dphiDetID1)) {dphiDetID1 = twopi-dphiDetID1;}
	  
// 	  detaDetID2 = fabs( (dummy.ieta() - 0.5) * 0.01745 - p2.eta()); 
// 	  if((dummy.iphi() - 0.5) * 0.01745 > twopi/2) dphiDetID2 = fabs((dummy.iphi() - 0.5) * 0.01745 - twopi - p2.phi());
// 	  else dphiDetID2 = fabs( (dummy.iphi() - 0.5) * 0.01745 - p2.phi());
// 	  if (dphiDetID2 > (twopi-dphiDetID2)) {dphiDetID2 = twopi-dphiDetID2;}

// 	  RDetID1 = sqrt(detaDetID1*detaDetID1 + dphiDetID1*dphiDetID1);
// 	  RDetID2 = sqrt(detaDetID2*detaDetID2 + dphiDetID2*dphiDetID2);

//  	  if(RDetID1 < 0.2 && RDetID1 < near_DetID1) 
// 	    {
// 	      near_DetID1 = RDetID1;
	     
// 	    }
	  
	  
// 	  if(RDetID2 < 0.2 && RDetID2 < near_DetID2) 
// 	    {
// 	      near_DetID2 = RDetID1;
			
// 	    }
// 	}

//       GPointVSDetID1->Fill(near_DetID1);
//       GPointVSDetID2->Fill(near_DetID2);
      



// //       for(int kk=0; kk!=FirstCluster.size(); ++kk)
// // 	{
	  
// // 	}
	
//       FirstCluster_->Fill(FirstCluster.size());

//       std::cout << "n crossedEcalIds = " << info.crossedEcalIds.size() << std::endl;
//       std::cout << "FirstCluster = " << FirstCluster.size() << std::endl;
//       std::cout << "SecondCluster = " << SecondCluster.size() << std::endl;

//       std::cerr << "deltaPhiMaxFirst = " << deltaPhiMaxFirst << std::endl;
//       std::cout << "------------------------------END------------------------" << std::endl;

 

// ------------ method called once each job just before starting event loop  ------------
void 
MuonAssociatorEcalIP::beginJob(const edm::EventSetup&)
{
  
  edm::Service<TFileService> fs ;
  allOccupancy_  = fs->make <TH2F> ("OccupancyAllEvents" ,"OccupancyAllEvents",36,0.,6.28,172,-86,86) ;
  allOccupancyMatch_  = fs->make <TH2F> ("OccupancyAllEventsMatch_" ,"OccupancyAllEventsMatch_",36,0.,6.28,172,-86,86) ;
  allOccupancyNoMatch_  = fs->make <TH2F> ("OccupancyAllEventsNoMatch_" ,"OccupancyAllEventsNoMatch_",36,0.,6.28,172,-86,86) ;
  energyMatch_  = fs->make <TH1F> ("energyMatch_" ,"energyMatch_",500,0.,250.) ;
  energyNoMatch_ = fs->make <TH1F> ("energyNoMatch_" ,"energyNoMatch_",500,0.,250.) ;
  GPOccupancy_  = fs->make <TH2F> ("GPOccupancy_" ,"GPOccupancy_",36,-3.14,3.14,800,-1.5,1.5) ;
  GPOccupancyz_  = fs->make <TH2F> ("GPOccupancyz_" ,"GPOccupancyz_",36,-3.14,3.14,700,-350.,350.) ;

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
  ClusEner_  = fs->make <TH1F> ("ClusEner" ,"ClusEner",25,0.,1.) ;
  numberOfCosmics_ =  fs->make <TH1F> ("numberOfCosmics_" ,"numberOfCosmics_",10,0.,10.) ;
  ECitOccupancy = fs->make <TH2F> ("ECitOccupancy" ,"ECitOccupancy",360,0.,360.,172,-86.,86.) ;

  DiffClus_  = fs->make <TH1F> ("DiffClus_" ,"DiffClus_",400,-10.,10.) ;
  DiffClusProfile_  = fs->make <TProfile> ("DiffClusProfile_" ,"DiffClusProfile_",400,-10.,10.) ;
  diffnXtalVSdiffE_ = fs->make <TH2F> ("diffnXtalVSdiffE" ,"diffnXtalVSdiffE",400,-10.,10.,30,-15.,15.) ;

  InnerStateOccupancy_ = fs->make <TH2F> ("InnerStateOccupancy" ,"InnerStateOccupancy",360,-3.14,3.14,800,-1.479,1.479) ;
  OuterStateOccupancy_ = fs->make <TH2F> ("OuterStateOccupancy" ,"OuterStateOccupancy",360,-3.14,3.14,800,-1.479,1.479) ;
  //------lunghezza segmento---------
  LvsPhiTop_ = fs->make <TH2F> ("LvsPhiTop","LvsPhiTop",360,-3.14,3.14,10000,0.,100.) ;
  LvsPhiBot_ = fs->make <TH2F> ("LvsPhiBot","LvsPhiBot",360,-3.14,3.14,10000,0.,100.) ;
  AvEnergyVsL_ =  fs->make <TH2F> ("AvEnergyVsL","AvEnergyVsL",10000,0.,100.,500,0.,5.) ;
  dEondX_ =  fs->make <TH1F> ("dEondX_","dEondX_",1000,0.,0.5) ;
  dEondX_Up_ =  fs->make <TH1F> ("dEondX_Up","dEondX_Up",1000,0.,0.5) ;
  dEondX_Down_ =  fs->make <TH1F> ("dEondX_Down","dEondX_Down",1000,0.,0.5) ;
  dEondX_Plus_ =  fs->make <TH1F> ("dEondX_Plus","dEondX_Plus",1000,0.,0.5) ;
  dEondX_Minus_ =  fs->make <TH1F> ("dEondX_Minus","dEondX_Minus",1000,0.,0.5) ;
  IPplane_ =  fs->make <TH2F> ("IPplane","IPplane",360,-3.14,3.14,800,-1.479,1.479) ;
  IPdist_ =  fs->make <TH1F> ("IPdist","IPdist",350,0.,350.) ;
  IPxydist_ =  fs->make <TH1F> ("IPXYdist","IPXYdist",170,0.,170.) ;
  IPdistBottomTracks_ =  fs->make <TH1F> ("IPdistBottomTracks_","IPdistBottomTracks_",1200,0.,1200.) ;
  IPdistTopTracks_ =  fs->make <TH1F> ("IPdistTopTracks_","IPdistTopTracks_",1200,0.,1200.) ;
  IPdistGPoints_ =  fs->make <TH1F> ("IPdistGPoints","IPdistGPoints",5000,0.,5000.) ;
  
  DiffClusVSIP_ =  fs->make <TH2F> ("DiffClusVSIP","DiffClusVSIP",350,0.,350.,400,-10.,10.) ;
  RatioClusVSIP_ =  fs->make <TH2F> ("RatioClusVSIP","RatioClusVSIP",350,0.,350.,300,0.,3.) ;
  dEondXVSIP_ =  fs->make <TH2F> ("dEondXVSIP","dEondXVSIP",350,0.,350.,1000,0.,0.5) ;
  clusterSizeVSIP_ =  fs->make <TH2F> ("clusterSizeVSIP","clusterSizeVSIP",350,0.,350.,25,0.,25.) ;

  posiXY_ = fs->make <TH2F> ("posiXY","posiXY",300,-150.,150.,300,-150.,150.) ;
  posiXYtracks_ = fs->make <TH2F> ("posiXYtracks","posiXYtracks",300,-150.,150.,300,-150.,150.) ;
  posiZ_ =  fs->make <TH1F> ("posiZ","posiZ",700,-350.,350.) ;
  posiZtracks_ =  fs->make <TH1F> ("posiZtracks","posiZtracks",700,-350.,350.) ;
  IPVSdeltaPhi_ = fs->make <TH2F> ("IPVSdeltaPhi","IPVSdeltaPhi",400,0.,4.,350,0.,350.) ;
  deltaPhi_ =  fs->make <TH1F> ("deltaPhi","deltaPhi",400,0.,4.) ;

  ECitPosUP=0;
  ECitNegUP=0;
  ECitPos=0;
  ECitNeg=0;
  GPointPos=0;
  GPointNeg=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuonAssociatorEcalIP::endJob()
{

}


//-------------deltaiPhi--------------------
int MuonAssociatorEcalIP::deltaiPhi (int phi1, int phi2)
{

  int deltaphi=abs(phi1-phi2);
  if (deltaphi > 360) deltaphi -= 360;
  if (deltaphi > 180) deltaphi = 360-deltaphi;
  return deltaphi;
}

//-------------deltaPhi--------------------
double MuonAssociatorEcalIP::deltaPhi (double phi1, double phi2)
{
  double deltaphi=abs(phi1-phi2);
  if (deltaphi > twopi) deltaphi -= twopi;
  if (deltaphi > twopi/2) deltaphi = twopi-deltaphi;
  return deltaphi;
}



//!PG find a cosmic in the event
//!PG from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CCEcal/EcalCosmicRuns/CaloOnlineTools/EcalTools/plugins/EcalCosmicsHists.cc?revision=1.5&view=markup
std::vector<EBDetId>
MuonAssociatorEcalIP::findCosmicsInEvent (const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

  int nCluster = 0;
  int cont = 0;
  double clusEnergy1 = 0,clusEnergy2 = 0;
  double clusPhi1 = 0, clusPhi2 = 0;
  double clusEta1 = 0, clusEta2 = 0;
  double clusSize1 = 0, clusSize2 = 0;
  //int eventnum = iEvent.id().event();
  
  const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();
  //PG loop on basic clusters
  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); 
       clus != clusterCollection_p->end(); 
       ++clus)
    {
      double energy = clus->energy();
      double phi    = clus->phi();
      double eta    = clus->eta();
      
      double time = -1000.0;
      cont++;
      
      //std::cerr << "------->>> clus->energy(), clus->phi(), clus->eta() =  " << energy << " " << phi <<" " << eta << std::endl;
      double ampli = 0.;
      double secondMin = 0.;
      double secondTime = -1000.;
      int numXtalsinE9 = 0;
      
      EBDetId maxDet;
      EBDetId secDet;
      
      std::vector<DetId> clusterDetIds = clus->getHitsByDetId();//get these from the cluster
      //PG loop on detIds in the cluster
       if(cont == 1) 
	{
	  clusEnergy1 = energy;
	  clusPhi1 = phi;
	  clusEta1 = eta;
	  clusSize1 = clusterDetIds.size();
	}
      if(cont == 2) 
	{
	  clusEnergy2 = energy;
	  clusPhi2 = phi;
	  clusEta2 = eta;
	  clusSize2 = clusterDetIds.size();
	}
      
      
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
	  if (thisamp > minNoiseThr_) { numXtalsinE9++; } //PG 3 ADC counts
	  if (thisamp > secondMin) {secondMin = thisamp; secondTime = myhit.time(); secDet = (EBDetId)(*detitr);}
	  if (secondMin > ampli) {std::swap(ampli,secondMin); std::swap(time,secondTime); std::swap(maxDet,secDet);}
	  
	  
	  
	} //PG loop on detIds in the cluster
      
      //std::cout << "---------------> MAXDET = " << maxDet << std::endl;
      cosmicCrystals.push_back (maxDet) ;     //è l'hot channel all'interno dei supercluster
      
      nCluster++;
    } //PG loop on basic clusters
  nCluster_->Fill(nCluster);
  
//   bool up = false;
//   bool down = false;
//   if(nCluster == 2) 
//     {    // faccio sempre Esotto - Esopra, 0.175 = 10Xtals (è un cono!)
//       if(((fabs(clusPhi1) + fabs(clusPhi2)) > (twopi/2 - 0.175)) && ((fabs(clusPhi1) + fabs(clusPhi2)) < (twopi/2 + 0.175)))
// 	if((fabs(clusEta1) - fabs(clusEta2) >  -0.175) && (fabs(clusEta1) - fabs(clusEta2) <  0.175))
// 	  if(clusPhi1 > 0 && clusPhi2 < 0) 
// 	    {
// 	      DiffClus_->Fill(clusEnergy2 - clusEnergy1);
// 	      DiffClusProfile_->Fill(clusEta1,clusEnergy2 - clusEnergy1);  //eta di quello sopra
// 	      diffnXtalVSdiffE_->Fill(clusEnergy2 - clusEnergy1,clusSize2 - clusSize1);
// 	      up = true;
// 	    }
// 	  else if(clusPhi1 < 0 && clusPhi2 > 0) 
// 	    {
// 	      DiffClus_->Fill(clusEnergy1 - clusEnergy2);
// 	      DiffClusProfile_->Fill(clusEta2,clusEnergy1 - clusEnergy2);  //eta di quello sopra
// 	      diffnXtalVSdiffE_->Fill(clusEnergy1 - clusEnergy2,clusSize1 - clusSize2);
// 	      down = true;
// 	    }
//     }
//   if(up == false && down == false) DiffClus_->Fill(-100.);
  return cosmicCrystals ;
}


//------------------------------------------------------------
std::vector<double>
MuonAssociatorEcalIP::getClusterEnergy (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  int ievt = iEvent.id().event();
  edm::Handle<reco::BasicClusterCollection> bccHandle;  
  edm::Handle<reco::BasicClusterCollection> eccHandle;  
  std::vector<double> clusterEnergy ;


  iEvent.getByLabel(barrelClusterCollection_, bccHandle);
  if (!(bccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << barrelClusterCollection_ << " not available";
      return clusterEnergy;
    }
  LogDebug("EcalCosmicsHists") << "event " << ievt;
  
  iEvent.getByLabel(endcapClusterCollection_, eccHandle);
  if (!(eccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << endcapClusterCollection_ << " not available";
      return clusterEnergy;
    }
  
  Handle<EcalRecHitCollection> hits;
  iEvent.getByLabel(m_EBRecHitsInputTag, hits);
  if (!(eccHandle.isValid())) 
    {
      edm::LogWarning("EcalCosmicsHists") << m_EBRecHitsInputTag << " not available";
      return clusterEnergy;
    }

  const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();
  //PG loop on basic clusters
  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); 
       clus != clusterCollection_p->end(); 
       ++clus)
    {
      double energy = clus->energy();
      //double phi    = clus->phi();
      //double eta    = clus->eta();
      
      clusterEnergy.push_back(energy);
    }
  return clusterEnergy;
}



std::pair<GlobalPoint,GlobalPoint> 
MuonAssociatorEcalIP::getEcalCross(const GlobalPoint& G1,const GlobalPoint& G2,const double& ECALrad)
{
  GlobalPoint nonsense (0,0,0) ;
  std::pair<GlobalPoint,GlobalPoint> Pair (nonsense,nonsense);
  double dx = G2.x() - G1.x();
  double dy = G2.y() - G1.y();
  
  double rad = G1.x() * dx + G1.y() * dy ;
  rad = rad * rad - (dx*dx + dy*dy) * (G1.x()*G1.x() + G1.y()*G1.y() - ECALrad*ECALrad) ;
  if (rad >= 0)
  {      
    rad = sqrt (rad);

    double alpha1 =  (- G1.x()*dx - G1.y()*dy + rad) / (dx*dx + dy*dy) ;
    double alpha2 =  (- G1.x()*dx - G1.y()*dy - rad) / (dx*dx + dy*dy) ;
    
    //std::cerr << "-------------->>>>>>>>> alfa1 = " << alpha1  << "   -------------->>>>>>>>> alfa2 = " << alpha2 << std::endl;
    //std::cerr << " x1 = " << x1 << "; y1 = " << y1 << "; x2 = " << x2 << "; y2 = " << y2 << "; dx = " << dx << "; dy = " << dy << "; rad = " << rad << std::endl;
    //std::cerr << " z1 = " << z1 << "; z2 = " << z2 << std::endl;
    
    double xi1 = G1.x() + alpha1 * (G2.x() - G1.x());
    double yi1 = G1.y() + alpha1 * (G2.y() - G1.y());
    double zi1 = G1.z() + alpha1 * (G2.z() - G1.z());
    
    double xi2 = G1.x() + alpha2 * (G2.x() - G1.x());
    double yi2 = G1.y() + alpha2 * (G2.y() - G1.y());
    double zi2 = G1.z() + alpha2 * (G2.z() - G1.z());
    
    GlobalPoint p1(xi1,yi1,zi1);   //GlobalPoints on ECAL surface
    GlobalPoint p2(xi2,yi2,zi2);
    
    //if(p1.eta() < 1.479 && p1.eta() > -1.479 && p2.eta() < 1.479 && p2.eta() > -1.479)  //controllo la z (eta) entrambi i punti dentro l'accett del barrel!
    if(p1.z() < 300. && p1.z() > -300. && p2.z() < 300. && p2.z() > -300.)
      {
	if(p1.phi() > 0 && p2.phi() < 0)
	  Pair = std::pair<GlobalPoint,GlobalPoint> (p1,p2);
	else if(p1.phi() < 0 && p2.phi() > 0)
	  Pair = std::pair<GlobalPoint,GlobalPoint> (p2,p1);
      }
  }
  return(Pair);
}


double 
MuonAssociatorEcalIP::getEcalXLenght (const GlobalPoint& G1,const GlobalPoint& G2)
{
  double deltaX = fabs(G1.x() - G2.x());
  double deltaY = fabs(G1.y() - G2.y());
  double deltaZ = fabs(G1.z() - G2.z());
  double lungh = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ); 
  return(lungh);
}

double 
MuonAssociatorEcalIP::getImpactParameter (const GlobalPoint& G1,const GlobalPoint& G2)
{
  double vx = G2.x() - G1.x();
  double vy = G2.y() - G1.y();
  double vz = G2.z() - G1.z();

  double alfa = - G1.x()*vx - G1.y()*vy - G1.z()*vz;
  alfa = alfa / (vx*vx + vy*vy + vz*vz);

  return(alfa);
}

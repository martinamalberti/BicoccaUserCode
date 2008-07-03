// -*- C++ -*-
//
// Package:   EcalCosmicsTree 
// Class:     EcalCosmicsTree  
// 
/**\class EcalCosmicsTree EcalCosmicsTree.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/

 
#include "CRUZET/Calibration/plugins/EcalCosmicsTree.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include <vector>
 
using namespace cms;
using namespace edm;
using namespace std;

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
EcalCosmicsTree::EcalCosmicsTree(const edm::ParameterSet& iConfig) :
  ecalRecHitCollection_ (iConfig.getParameter<edm::InputTag>("ecalRecHitCollection")),
  barrelClusterCollection_ (iConfig.getParameter<edm::InputTag>("barrelClusterCollection")),
  endcapClusterCollection_ (iConfig.getParameter<edm::InputTag>("endcapClusterCollection")),
  l1GTReadoutRecTag_ (iConfig.getUntrackedParameter<std::string>("L1GlobalReadoutRecord","gtDigis")),
  muonTracksCollection_ (iConfig.getParameter<edm::InputTag>("muonTracksCollection")),
  runNum_(-1),
  fileName_ (iConfig.getUntrackedParameter<std::string>("fileName", std::string("ecalCosmicHists")))
{
  naiveEvtNum_ = 0;
  // TrackAssociator parameters
  edm::ParameterSet trkParameters = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
  trackParameters_.loadParameters( trkParameters );
  trackAssociator_.useDefaultPropagator();

  //Initialize the tree
  //fileName_ += "-"+intToString(runNum_)+".tree.root";

  file_ = new TFile(fileName_.c_str() , "RECREATE");
  file_->cd();
  tree_ = new TTree( "EcalCosmicsAnalysis","EcalCosmicsAnalysis" );
  tree_->Branch("runId",&myTreeVariables_.runId,"runId/i"); //
  tree_->Branch("eventId",&myTreeVariables_.eventId,"eventId/i"); //
  tree_->Branch("timeStampLow",&myTreeVariables_.timeStampLow,"timeStampLow/i"); //
  tree_->Branch("timeStampHigh",&myTreeVariables_.timeStampHigh,"timeStampHigh/i"); //
  tree_->Branch("isECALL1",&myTreeVariables_.isECALL1,"isECALL1/b"); //
  tree_->Branch("isHCALL1",&myTreeVariables_.isHCALL1,"isHCALL1/b"); //
  tree_->Branch("isDTL1",&myTreeVariables_.isDTL1,"isDTL1/b"); //
  tree_->Branch("isRPCL1",&myTreeVariables_.isRPCL1,"isRPCL1/b"); //
  tree_->Branch("isCSCL1",&myTreeVariables_.isCSCL1,"isCSCL1/b"); //
  tree_->Branch("nCosmicsCluster",&myTreeVariables_.nCosmicsCluster,"nCosmicsCluster/I"); //
  tree_->Branch("cosmicClusterXtals",myTreeVariables_.cosmicClusterXtals,"cosmicClusterXtals[nCosmicsCluster]/I"); //
  tree_->Branch("cosmicClusterXtalsAbove3Sigma",myTreeVariables_.cosmicClusterXtalsAbove3Sigma,"cosmicClusterXtalsAbove3Sigma[nCosmicsCluster]/I"); //
  tree_->Branch("cosmicClusterEnergy",myTreeVariables_.cosmicClusterEnergy,"cosmicClusterEnergy[nCosmicsCluster]/F"); //
  tree_->Branch("cosmicClusterEnergyXtals",myTreeVariables_.cosmicClusterEnergyXtals,"cosmicClusterEnergyXtals[nCosmicsCluster][25]/F"); //
  tree_->Branch("cosmicClusterLengthXtals",myTreeVariables_.cosmicClusterLengthXtals,"cosmicClusterLengthXtals[nCosmicsCluster][25]/F"); //
  tree_->Branch("cosmicClusterE1",myTreeVariables_.cosmicClusterE1,"cosmicClusterE1[nCosmicsCluster]/F"); //
  tree_->Branch("cosmicClusterE2",myTreeVariables_.cosmicClusterE2,"cosmicClusterE2[nCosmicsCluster]/F"); //
  tree_->Branch("cosmicClusterE9",myTreeVariables_.cosmicClusterE9,"cosmicClusterE9[nCosmicsCluster]/F"); //
  tree_->Branch("cosmicClusterE25",myTreeVariables_.cosmicClusterE25,"cosmicClusterE25[nCosmicsCluster]/F"); //
  tree_->Branch("cosmicClusterTime",myTreeVariables_.cosmicClusterTime,"cosmicClusterTime[nCosmicsCluster]/F"); //
  tree_->Branch("cosmicClusterEta",myTreeVariables_.cosmicClusterEta,"cosmicClusterEta[nCosmicsCluster]/F"); //
  tree_->Branch("cosmicClusterPhi",myTreeVariables_.cosmicClusterPhi,"cosmicClusterPhi[nCosmicsCluster]/F"); //

  tree_->Branch("cosmicClusterMaxId",myTreeVariables_.cosmicClusterMaxId,"cosmicClusterMaxId[nCosmicsCluster]/i"); //
  tree_->Branch("cosmicCluster2ndId",myTreeVariables_.cosmicCluster2ndId,"cosmicCluster2ndId[nCosmicsCluster]/i"); //
  tree_->Branch("nRecoMuons",&myTreeVariables_.nRecoMuons,"nRecoMuons/I"); //
  tree_->Branch("muonPt",myTreeVariables_.muonPt,"muonPt[nRecoMuons]/F"); //
  tree_->Branch("muonEta",myTreeVariables_.muonEta,"muonEta[nRecoMuons]/F"); //
  tree_->Branch("muonPhi",myTreeVariables_.muonPhi,"muonPhi[nRecoMuons]/F"); //
  tree_->Branch("muonNChi2",myTreeVariables_.muonNChi2,"muonNChi2[nRecoMuons]/F"); //
  tree_->Branch("muonNDof",myTreeVariables_.muonNDof,"muonNDof[nRecoMuons]/F"); //
  tree_->Branch("muonNHits",myTreeVariables_.muonNHits,"muonNHits[nRecoMuons]/F"); //
  tree_->Branch("muonCharge",myTreeVariables_.muonCharge,"muonCharge[nRecoMuons]/F"); //
  tree_->Branch("muonQOverP",myTreeVariables_.muonQOverP,"muonQOverP[nRecoMuons]/F"); //
  tree_->Branch("muond0",myTreeVariables_.muond0,"muond0[nRecoMuons]/F"); //
  tree_->Branch("muondz",myTreeVariables_.muondz,"muondz[nRecoMuons]/F"); //
  tree_->Branch("muonTkAtEcalEta",myTreeVariables_.muonTkAtEcalEta,"muonTkAtEcalEta[nRecoMuons]/F"); //
  tree_->Branch("muonTkAtEcalPhi",myTreeVariables_.muonTkAtEcalPhi,"muonTkAtEcalPhi[nRecoMuons]/F"); //
  tree_->Branch("muonTkAtHcalEta",myTreeVariables_.muonTkAtHcalEta,"muonTkAtHcalEta[nRecoMuons]/F"); //
  tree_->Branch("muonTkAtHcalPhi",myTreeVariables_.muonTkAtHcalPhi,"muonTkAtHcalPhi[nRecoMuons]/F"); //
  tree_->Branch("muonEcalEnergy3x3",myTreeVariables_.muonEcalEnergy3x3,"muonEcalEnergy3x3[nRecoMuons]/F"); //
  tree_->Branch("muonEcalEnergy5x5",myTreeVariables_.muonEcalEnergy5x5,"muonEcalEnergy5x5[nRecoMuons]/F"); //
  tree_->Branch("muonEcalEnergyCrossed",myTreeVariables_.muonEcalEnergyCrossed,"muonEcalEnergyCrossed[nRecoMuons]/F"); //
  tree_->Branch("muonHcalEnergy3x3",myTreeVariables_.muonHcalEnergy3x3,"muonHcalEnergy3x3[nRecoMuons]/F"); //
  tree_->Branch("muonHcalEnergyCrossed",myTreeVariables_.muonHcalEnergyCrossed,"muonHcalEnergyCrossed[nRecoMuons]/F"); //
  tree_->Branch("muonNCrossedEcalDetId",myTreeVariables_.muonNCrossedEcalDetId,"muonNCrossedEcalDetId[nRecoMuons]/I"); //
  tree_->Branch("muonMaxEneEcalDetIdCrossed",myTreeVariables_.muonMaxEneEcalDetIdCrossed,"muonMaxEneEcalDetIdCrossed[nRecoMuons]/i"); //

}


EcalCosmicsTree::~EcalCosmicsTree()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
EcalCosmicsTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //-----------test inizializzazione matrici (obbligatorio)-------------
  for(int ii = 0; ii < 100; ++ii)
    for(int kk = 0; kk < 25; ++kk)
      {
	myTreeVariables_.cosmicClusterEnergyXtals[ii][kk] = 0.;
	myTreeVariables_.cosmicClusterLengthXtals[ii][kk] = 0.;
      }
  //----------------------inizializzazione------------------------------



  int ievt = iEvent.id().event();
  
  edm::Handle<reco::BasicClusterCollection> bccHandle;  
  edm::Handle<reco::BasicClusterCollection> eccHandle;  
  
  naiveEvtNum_++;
  
  //  cout << "Timestamp: " << iEvent.id().run() << " " << iEvent.id().event() << " " << iEvent.time().value() << endl;
  
  iEvent.getByLabel(barrelClusterCollection_, bccHandle);
  if (!(bccHandle.isValid())) 
    {
      LogWarning("EcalCosmicsTree") << barrelClusterCollection_ << " not available";
      return; 
    }
  LogDebug("EcalCosmicsTree") << "event " << ievt;
  
  iEvent.getByLabel(endcapClusterCollection_, eccHandle);
  if (!(eccHandle.isValid())) 
    {
      LogWarning("EcalCosmicsTree") << endcapClusterCollection_ << " not available";
      //return;
    }
  
  Handle<EcalRecHitCollection> hits;
  iEvent.getByLabel(ecalRecHitCollection_, hits);
  if (!(eccHandle.isValid())) 
    {
      LogWarning("EcalCosmicsTree") << ecalRecHitCollection_ << " not available";
      //return;
    }
  
  
  // get the GMTReadoutCollection
  Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
  iEvent.getByLabel(l1GTReadoutRecTag_,gmtrc_handle);
  L1MuGMTReadoutCollection const* gmtrc = gmtrc_handle.product();
  if (!(gmtrc_handle.isValid())) 
    {
	  LogWarning("EcalCosmicsTree") << "l1MuGMTReadoutCollection" << " not available";
      //return;
    }
 
  
  // get hold of L1GlobalReadoutRecord
  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  iEvent.getByLabel(l1GTReadoutRecTag_,L1GTRR);
  bool isEcalL1 = false;
  const unsigned int sizeOfDecisionWord(L1GTRR->decisionWord().size());
  if (!(L1GTRR.isValid()))
    {
      LogWarning("EcalCosmicsTree") << l1GTReadoutRecTag_ << " not available";
      //return;
    }
  else if(sizeOfDecisionWord<128)
    {
      LogWarning("EcalCosmicsTree") << "size of L1 decisionword is " << sizeOfDecisionWord
				    << "; L1 Ecal triggering bits not available";
    } 
  else
    {
      l1Names_.resize(sizeOfDecisionWord);
      l1Accepts_.resize(sizeOfDecisionWord);
      for (unsigned int i=0; i!=sizeOfDecisionWord; ++i) {
        l1Accepts_[i]=0;
        l1Names_[i]="NameNotAvailable";
      }
      for (unsigned int i=0; i!=sizeOfDecisionWord; ++i) {
        if (L1GTRR->decisionWord()[i])
          {
            l1Accepts_[i]++;
            //cout << "L1A bit: " << i << endl;
          }
      }
     
      if(l1Accepts_[14] || l1Accepts_[15] || l1Accepts_[16] || l1Accepts_[17]
	 || l1Accepts_[18] || l1Accepts_[19] || l1Accepts_[20])
        isEcalL1 = true;
      if(l1Accepts_[73] || l1Accepts_[74] || l1Accepts_[75] || l1Accepts_[76]
	 || l1Accepts_[77] || l1Accepts_[78])
        isEcalL1 = true;
    } 
  
  bool isRPCL1 = false;
  bool isDTL1 = false;
  bool isCSCL1 = false;
  bool isHCALL1 = false;
  
  std::vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
  std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
  
  for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {
    
    std::vector<L1MuRegionalCand>::const_iterator iter1;
    std::vector<L1MuRegionalCand> rmc;

    //DT triggers
    int idt = 0;
    rmc = igmtrr->getDTBXCands();
    for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
      if ( !(*iter1).empty() ) {
        idt++;
      }
    }
    
    //if(idt>0) std::cout << "Found " << idt << " valid DT candidates in bx wrt. L1A = " 
    //  << igmtrr->getBxInEvent() << std::endl;
    if(igmtrr->getBxInEvent()==0 && idt>0) isDTL1 = true;

    //RPC triggers
    int irpcb = 0;
    rmc = igmtrr->getBrlRPCCands();
    for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
      if ( !(*iter1).empty() ) {
        irpcb++;
      }
    }
    
    //if(irpcb>0) std::cout << "Found " << irpcb << " valid RPC candidates in bx wrt. L1A = " 
    //  << igmtrr->getBxInEvent() << std::endl;
    if(igmtrr->getBxInEvent()==0 && irpcb>0) isRPCL1 = true;

    //CSC Triggers
    int icsc = 0;
    rmc = igmtrr->getCSCCands();
    for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
      if ( !(*iter1).empty() ) {
        icsc++;
      }
    }
    
    //if(icsc>0) std::cout << "Found " << icsc << " valid CSC candidates in bx wrt. L1A = " 
    //  << igmtrr->getBxInEvent() << std::endl;
    if(igmtrr->getBxInEvent()==0 && icsc>0) isCSCL1 = true;
  }
  
  
  L1GlobalTriggerReadoutRecord const* gtrr = L1GTRR.product();
  
  for(int ibx=-1; ibx<=1; ibx++) {
    bool hcal_top = false;
    bool hcal_bot = false;
    const L1GtPsbWord psb = gtrr->gtPsbWord(0xbb0d,ibx);
    std::vector<int> valid_phi;
    if((psb.aData(4)&0x3f) >= 1) {valid_phi.push_back( (psb.aData(4)>>10)&0x1f ); }
    if((psb.bData(4)&0x3f) >= 1) {valid_phi.push_back( (psb.bData(4)>>10)&0x1f ); }
    if((psb.aData(5)&0x3f) >= 1) {valid_phi.push_back( (psb.aData(5)>>10)&0x1f ); }
    if((psb.bData(5)&0x3f) >= 1) {valid_phi.push_back( (psb.bData(5)>>10)&0x1f ); }
    std::vector<int>::const_iterator iphi;
    for(iphi=valid_phi.begin(); iphi!=valid_phi.end(); iphi++) {
      //std::cout << "Found HCAL mip with phi=" << *iphi << " in bx wrt. L1A = " << ibx << std::endl;
      if(*iphi<9) hcal_top=true;
      if(*iphi>8) hcal_bot=true;
    }
    if(ibx==0 && hcal_top && hcal_bot) isHCALL1=true;
  }
  
  /*
  std::cout << "**** Trigger Source ****" << std::endl;
  if(isDTL1) std::cout << "DT" << std::endl;
  if(isRPCL1) std::cout << "RPC" << std::endl;
  if(isCSCL1) cout << "CSC" << endl;
  if(isHCALL1) std::cout << "HCAL" << std::endl;
  if(isEcalL1) std::cout << "ECAL" << std::endl;
  std::cout << "************************" << std::endl;
  */
  
  //L1 Trigger informations
  myTreeVariables_.isDTL1= isDTL1;
  myTreeVariables_.isECALL1= isEcalL1;
  myTreeVariables_.isHCALL1= isHCALL1;
  myTreeVariables_.isRPCL1= isRPCL1;
  myTreeVariables_.isCSCL1= isCSCL1;
  
  //Event Id
  myTreeVariables_.runId = iEvent.id().run();
  myTreeVariables_.eventId = iEvent.id().event();
  myTreeVariables_.timeStampLow = ( 0xFFFFFFFF & iEvent.time().value() );
  myTreeVariables_.timeStampHigh = ( iEvent.time().value() >> 32 );
  
  //Get geometry
  edm::ESHandle<CaloGeometry> pGeometry;
  iSetup.get<IdealGeometryRecord>().get(pGeometry);
  const CaloGeometry *geometry = pGeometry.product();
  
  const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();
  //only 2Cluster events
  if (clusterCollection_p->size() != 2) return;
  myTreeVariables_.nCosmicsCluster = clusterCollection_p->size();  //filter check
  std::vector<GlobalPoint> position;
  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); clus != clusterCollection_p->end(); ++clus)
   {
     GlobalPoint posCluster(clus->x(),clus->y(),clus->z());
     position.push_back(posCluster);
   }
  // define trajectory ----> check sulla direzione corretta
  std::vector<GlobalPoint> trajectoryUP;
  std::vector<GlobalPoint> trajectoryDOWN;
  bool zeroSopra = false;
  bool zeroSotto = false;
  if(position.at(0).y() > position.at(1).y()) //FIXME controllo come si sposta alpha!!! sopra e sotto assegnati giusti?
    {
      trajectoryUP   = getTrajectoryPoints(position.at(0),position.at(1),-0.0001);
      trajectoryDOWN = getTrajectoryPoints(position.at(0),position.at(1),0.0001);
      zeroSopra = true;
    }
  else if(position.at(0).y() < position.at(1).y()) 
    {
      trajectoryUP   = getTrajectoryPoints(position.at(1),position.at(0),-0.0001);
      trajectoryDOWN = getTrajectoryPoints(position.at(1),position.at(0),0.0001);
      zeroSotto = true;
    }
  else return; //se clusters @ stessa altezza -> butto

  int  numberOfCosmics = 0;
  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); clus != clusterCollection_p->end(); ++clus)
   {
     double energy = clus->energy();
     double phi    = clus->phi();
     double eta    = clus->eta();
     
     double time = -1000.0;
     double ampli = 0.;
     double secondMin = 0.;
     double secondTime = -1000.;
     int numXtalsinCluster = 0;
     
     EBDetId maxDet;
     EBDetId secDet;
     
     std::vector<DetId> clusterDetIds = clus->getHitsByDetId();//get these from the cluster

     int nXtallo = 0;
     for(std::vector<DetId>::const_iterator detitr = clusterDetIds.begin(); detitr != clusterDetIds.end(); ++detitr)
       {
	 //Here I use the "find" on a digi collection... I have been warned...
	 //	 if ((*detitr).det() != DetId::Ecal) { std::cout << " det is " <<(*detitr).det() << std::endl;continue;}
	 //	 if ((*detitr).subdetId() != EcalBarrel) {std::cout << " subdet is " <<(*detitr).subdetId() << std::endl; continue; }
	 EcalRecHitCollection::const_iterator thishit = hits->find((*detitr));
	 if (thishit == hits->end()) 
	   {
	     continue;
	   }
	 //The checking above should no longer be needed...... as only those in the cluster would already have rechits..
	 
	 EcalRecHit myhit = (*thishit);
	 
	 float thisamp = myhit.energy();
	 if (thisamp > 0.027) {numXtalsinCluster++; }
	 if (thisamp > secondMin) {secondMin = thisamp; secondTime = myhit.time(); secDet = (EBDetId)(*detitr);}
	 if (secondMin > ampli) {std::swap(ampli,secondMin); std::swap(time,secondTime); std::swap(maxDet,secDet);}
	 
	 //-----------
	 //std::cout << "-------------------------->> numberOfCosmics = " << numberOfCosmics << std::endl;
	 //std::cout << "-------------------------->> nXtallo = " << nXtallo << std::endl;
	 //-----------
	 // find track length in DetId
	 float lengthInsideTop = 0.;
	 float lengthInsideBot = 0.;
	 if((numberOfCosmics == 0 && zeroSopra == true) || (numberOfCosmics == 1 && zeroSotto == true))
	   {
	     for(std::vector<GlobalPoint>::const_iterator it = trajectoryUP.begin(); it != trajectoryUP.end(); ++it) 
	       {
		 //std::cout << "-------------------------->> point UP = " << std::endl;
		 std::vector<GlobalPoint>::const_iterator jt = it + 1;
		 if ( geometry->getSubdetectorGeometry( *detitr )->getGeometry( *detitr )->inside( *it ) && 
		      geometry->getSubdetectorGeometry( *detitr )->getGeometry( *detitr )->inside( *jt ) )  
		   {
		     lengthInsideTop = lengthInsideTop + (*it - *jt).mag();
		     //std::cout << "Another step inside UP = " << (*it - *jt).mag() << std::endl;
		     //std::cout << "lengthInsideTop        = " << lengthInsideTop << std::endl;
		   }
	       }
	   }
	 else
	   {
	     for(std::vector<GlobalPoint>::const_iterator it = trajectoryDOWN.begin(); it != trajectoryDOWN.end(); ++it) 
	       {
		 //std::cout << "-------------------------->> point DOWN = " << std::endl;
		 std::vector<GlobalPoint>::const_iterator jt = it + 1;
		 if ( geometry->getSubdetectorGeometry( *detitr )->getGeometry( *detitr )->inside( *it ) && 
		      geometry->getSubdetectorGeometry( *detitr )->getGeometry( *detitr )->inside( *jt ) )  
		   {
		     lengthInsideBot = lengthInsideBot + (*it - *jt).mag();
		     //std::cout << "Another step inside DOWN = " << (*it - *jt).mag() << std::endl;
		     //std::cout << "lengthInsideBot          = " << lengthInsideBot << std::endl;
		   }
	       }
	   }
	 
	 //std::cout << "riempio vettore. nXtallo = " << nXtallo << "; numberOfCosmics =  " << numberOfCosmics << std::endl;
	 std::cout << "numberOfCosmics = " << numberOfCosmics << ";  lengthInsideTop = " << lengthInsideTop << ";  lengthInsideBot = " << lengthInsideBot << std::endl;
	 //std::cout << "thisamp = " << thisamp << std::endl;
	 myTreeVariables_.cosmicClusterLengthXtals[numberOfCosmics][nXtallo] = lengthInsideTop + lengthInsideBot;  //FIXME distinguere sopra/sotto magari con due matrici?
	 myTreeVariables_.cosmicClusterEnergyXtals[numberOfCosmics][nXtallo] = thisamp;
	 nXtallo++;
       }
     
     float E2 = ampli + secondMin;
     
     //Set some more values
     myTreeVariables_.cosmicClusterEnergy[numberOfCosmics] = energy;
     myTreeVariables_.cosmicClusterTime[numberOfCosmics] = time;
     myTreeVariables_.cosmicClusterEta[numberOfCosmics] = eta;
     myTreeVariables_.cosmicClusterPhi[numberOfCosmics] = phi;
     myTreeVariables_.cosmicClusterE1[numberOfCosmics] = ampli;
     myTreeVariables_.cosmicClusterE2[numberOfCosmics] = E2;
     myTreeVariables_.cosmicClusterXtalsAbove3Sigma[numberOfCosmics] = numXtalsinCluster;
     myTreeVariables_.cosmicClusterXtals[numberOfCosmics] = clusterDetIds.size();
     myTreeVariables_.cosmicClusterMaxId[numberOfCosmics] =  maxDet.rawId();
     myTreeVariables_.cosmicClusterMaxId[numberOfCosmics] =  secDet.rawId();
     numberOfCosmics++;
     
   }
  
  
  // TrackAssociator
  //std::cout << "---------------> TrackAssociator" << std::endl;
  // get reco tracks 
  edm::Handle<reco::TrackCollection> recoTracks;
  iEvent.getByLabel(muonTracksCollection_, recoTracks);  

  if ( !recoTracks.isValid() ) {
    LogWarning("EcalCosmicsTree") << "... No Valid Muon recoTracks !!! " << recoTracks.product()->size();
  }
  
  myTreeVariables_.nRecoMuons=recoTracks.product()->size();
  int numberOfRecoMuons= 0;
  for(reco::TrackCollection::const_iterator recoTrack = recoTracks->begin(); recoTrack != recoTracks->end(); ++recoTrack)
    {
    
      TrackDetMatchInfo info = trackAssociator_.associate(iEvent, iSetup, *recoTrack, trackParameters_);
      
      myTreeVariables_.muonPt[numberOfRecoMuons] =  recoTrack->pt() ;
      myTreeVariables_.muonEta[numberOfRecoMuons] =  recoTrack->eta() ;
      myTreeVariables_.muonPhi[numberOfRecoMuons] =  recoTrack->phi() ;
      myTreeVariables_.muonNChi2[numberOfRecoMuons] =  recoTrack->normalizedChi2();
      myTreeVariables_.muonNDof[numberOfRecoMuons] =  recoTrack->ndof();
      myTreeVariables_.muonQOverP[numberOfRecoMuons] =  recoTrack->qoverp();
      myTreeVariables_.muonCharge[numberOfRecoMuons] =  recoTrack->charge();
      myTreeVariables_.muond0[numberOfRecoMuons] =  recoTrack->d0();
      myTreeVariables_.muondz[numberOfRecoMuons] =  recoTrack->dz();
      myTreeVariables_.muonNHits[numberOfRecoMuons] =  recoTrack->numberOfValidHits();
      myTreeVariables_.muonTkAtEcalEta[numberOfRecoMuons] = info.trkGlobPosAtEcal.eta();
      myTreeVariables_.muonTkAtEcalPhi[numberOfRecoMuons] = info.trkGlobPosAtEcal.phi();
      myTreeVariables_.muonTkAtHcalEta[numberOfRecoMuons] = info.trkGlobPosAtHcal.eta();
      myTreeVariables_.muonTkAtHcalPhi[numberOfRecoMuons] = info.trkGlobPosAtHcal.phi();
      double ampli = -9999.;
      EBDetId maxDet(0);
      
      for (unsigned int i=0; i<info.crossedEcalIds.size(); i++) 
	{	 
	  if (! (info.crossedEcalIds[i].det() == DetId::Ecal && info.crossedEcalIds[i].subdetId() == 1) ) 
	    continue;
	  EcalRecHitCollection::const_iterator thishit = hits->find(info.crossedEcalIds[i]);
	  if (thishit == hits->end()) 
	    continue;
	  EcalRecHit myhit = (*thishit);
	  double thisamp = myhit.energy();
	  if (thisamp > ampli) { ampli = thisamp; maxDet = (EBDetId)(info.crossedEcalIds[i].det()); }
	}
      myTreeVariables_.muonNCrossedEcalDetId[numberOfRecoMuons] = info.crossedEcalIds.size();
      myTreeVariables_.muonEcalEnergyCrossed[numberOfRecoMuons] = info.crossedEnergy(TrackDetMatchInfo::EcalRecHits);
      myTreeVariables_.muonHcalEnergyCrossed[numberOfRecoMuons] = info.crossedEnergy(TrackDetMatchInfo::HcalRecHits);
      myTreeVariables_.muonHcalEnergy3x3[numberOfRecoMuons] = info.nXnEnergy(TrackDetMatchInfo::HcalRecHits, 1);
      
      if (!maxDet.null())
	{
	  myTreeVariables_.muonEcalEnergy3x3[numberOfRecoMuons] = info.nXnEnergy(maxDet, TrackDetMatchInfo::EcalRecHits, 1);
	  myTreeVariables_.muonEcalEnergy5x5[numberOfRecoMuons] = info.nXnEnergy(maxDet, TrackDetMatchInfo::EcalRecHits, 2);
	  myTreeVariables_.muonMaxEneEcalDetIdCrossed[numberOfRecoMuons] = maxDet;
	}
      
      ++numberOfRecoMuons;    
    }    
  tree_->Fill();
}

//mi sposto lungo la direzione: G1 = sopra; G2 = sotto.
std::vector<GlobalPoint> EcalCosmicsTree::getTrajectoryPoints( const GlobalPoint& G1, const GlobalPoint& G2,const float& dt  )
{
  //parto dal centro! average alpha = 0.5: sicuramente dentro ECAL
  std::vector<GlobalPoint> v;
  GlobalPoint p;

  float alpha = 0.5;

  while ( fabs(alpha) < 3. ) {                     //FIXME sara' sufficiente 3 per essere fuori da ECAL? intervalli non simm!
    double x = G1.x() + alpha * (G2.x() - G1.x());
    double y = G1.y() + alpha * (G2.y() - G1.y());
    double z = G1.z() + alpha * (G2.z() - G1.z());
    GlobalPoint p( x, y, z );
    v.push_back( p );
    alpha += dt;
    
    std::cout << "alpha = " << alpha << "; R = " << sqrt(x*x + y*y) << "; y = " << y << std::endl;
  }

  return v;
}


// ------------ method called once each job just after ending the event loop  ------------
void 
EcalCosmicsTree::endJob()
{
  using namespace std;
  file_->cd();
  tree_->Write();
  file_->Close();

}


std::string EcalCosmicsTree::intToString(int num)
{
    using namespace std;
    ostringstream myStream;
    myStream << num << flush;
    return(myStream.str()); //returns the string form of the stringstream object
}


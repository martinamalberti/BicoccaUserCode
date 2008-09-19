#include "EcalValidation/EtaValidation/plugins/EtaAnalyzerWithMCTreeGenerator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Common/interface/Handle.h"
//
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
//
#include "Math/GenVector/LorentzVector.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "TFile.h"

#define PI 3.141592654
#define TWOPI 6.283185308



//========================================================================
EtaAnalyzerWithMCTreeGenerator::EtaAnalyzerWithMCTreeGenerator( const edm::ParameterSet& ps )
//========================================================================
{
 photonCollectionProducer_ = ps.getParameter<std::string>("phoProducer");
 photonCollection_ = ps.getParameter<std::string>("photonCollection");

 barrelEcalHits_   = ps.getParameter<edm::InputTag>("barrelEcalHits");
 endcapEcalHits_   = ps.getParameter<edm::InputTag>("endcapEcalHits");

//  islandBarrelBasicClusters_ = ps.getParameter<edm::InputTag>("islandBarrelBasicClusters");
//  islandBarrelShapeAssoc_ = ps.getParameter<edm::InputTag>("islandBarrelShapeAssoc");
//  islandEndcapBasicClusters_= ps.getParameter<edm::InputTag>("islandEndcapBasicClusters");
//  islandEndcapShapeAssoc_= ps.getParameter<edm::InputTag>("islandEndcapShapeAssoc");

 islandBarrelBasicClustersProducer_ = ps.getParameter<std::string>("islandBarrelBasicClustersProducer");
 islandBarrelBasicClusters_ = ps.getParameter<std::string>("islandBarrelBasicClusters");
 
 islandBarrelShapeAssocProducer_ = ps.getParameter<std::string>("islandBarrelShapeAssocProducer");
 islandBarrelShapeAssoc_ = ps.getParameter<std::string>("islandBarrelShapeAssoc");
 
 islandEndcapBasicClustersProducer_ = ps.getParameter<std::string>("islandEndcapBasicClustersProducer");
 islandEndcapBasicClusters_ = ps.getParameter<std::string>("islandEndcapBasicClusters");
 
 islandEndcapShapeAssocProducer_ = ps.getParameter<std::string>("islandEndcapShapeAssocProducer");
 islandEndcapShapeAssoc_ = ps.getParameter<std::string>("islandEndcapShapeAssoc");
 
 mcProducer_ = ps.getParameter<std::string>("mcProducer");
  //mcCollection_ = ps.getParameter<std::string>("mcCollection");
 vertexProducer_ = ps.getParameter<std::string>("primaryVertexProducer");
  
 IsolationProducer_       = ps.getParameter<edm::InputTag>("IsolationProducer");

 MinClusterEt_   = ps.getParameter<double> ("MinClusterEt");
 MinPtGamma_     = ps.getParameter<double> ("MinPtGamma");
 MinPtGammaPair_ = ps.getParameter<double> ("MinPtGammaPair");
 IsolationCut_   = ps.getParameter<double> ("IsolationCut");
 MinE4oE9_       = ps.getParameter<double> ("MinE4oE9");
 RMax_           = ps.getParameter<double> ("RMax");
 DeltaEtaMax_    = ps.getParameter<double> ("DeltaEtaMax");
}


//========================================================================
EtaAnalyzerWithMCTreeGenerator::~EtaAnalyzerWithMCTreeGenerator()
//========================================================================
{


}

//========================================================================
void
  EtaAnalyzerWithMCTreeGenerator::beginJob(edm::EventSetup const&) {
//========================================================================

 edm::Service<TFileService> fs;

 //---- Tree creation ----
 tTreeUtilities_ = fs->make<TTree>("tTreeUtilities","tTreeUtilities");
 
 etaC_ = new std::vector<double>;
 thetaC_ = new std::vector<double>;
 phiC_ = new std::vector<double>;
 S4oS9C_ = new std::vector<double>;
 S4C_ = new std::vector<double>;
 S9C_ = new std::vector<double>;
 S16C_ = new std::vector<double>;
 S25C_ = new std::vector<double>;
 pxC_ = new std::vector<double>;
 pyC_ = new std::vector<double>;
 pzC_ = new std::vector<double>;
 etC_ = new std::vector<double>;
 HitsC_ = new std::vector<int>;
 HitsEnergyC_ = new std::vector<double>;
 
 numPh_ = new std::vector<int>;
 thetaPh_ = new std::vector<double>;
 etaPh_ = new std::vector<double>;
 phiPh_ = new std::vector<double>;
 pxPh_ = new std::vector<double>;
 pyPh_ = new std::vector<double>;
 pzPh_ = new std::vector<double>;
 
 //---- ---- Clusters ---- C = cluster ----
 tTreeUtilities_->Branch("numC_",&numC_,"numC_/I");
 tTreeUtilities_->Branch("numCB_",&numCB_,"numCB_/I");
 tTreeUtilities_->Branch("numCE_",&numCE_,"numCE_/I");
 tTreeUtilities_->Branch("etaC_","std::vector<double>",&etaC_);
 tTreeUtilities_->Branch("thetaC_","std::vector<double>",&thetaC_);
 tTreeUtilities_->Branch("phiC_","std::vector<double>",&phiC_);
 tTreeUtilities_->Branch("S4oS9C_","std::vector<double>",&S4oS9C_);
 tTreeUtilities_->Branch("S4C_","std::vector<double>",&S4C_);
 tTreeUtilities_->Branch("S9C_","std::vector<double>",&S9C_);
 tTreeUtilities_->Branch("S16C_","std::vector<double>",&S16C_);
 tTreeUtilities_->Branch("S25C_","std::vector<double>",&S25C_);
 tTreeUtilities_->Branch("pxC_","std::vector<double>",&pxC_);
 tTreeUtilities_->Branch("pyC_","std::vector<double>",&pyC_);
 tTreeUtilities_->Branch("pzC_","std::vector<double>",&pzC_);
 tTreeUtilities_->Branch("etC_","std::vector<double>",&etC_);
 tTreeUtilities_->Branch("HitsC_","std::vector<int>",&HitsC_);
 tTreeUtilities_->Branch("HitsEnergyC_","std::vector<double>",&HitsEnergyC_);
 
 //---- ---- Photons from Eta ---- 
 tTreeUtilities_->Branch("numEta_",&numEta_,"numEta_/I");
 tTreeUtilities_->Branch("numPh_","std::vector<int>",&numPh_);
 tTreeUtilities_->Branch("thetaPh_","std::vector<double>",&thetaPh_);
 tTreeUtilities_->Branch("etaPh_","std::vector<double>",&etaPh_);
 tTreeUtilities_->Branch("phiPh_","std::vector<double>",&phiPh_);
 tTreeUtilities_->Branch("pxPh_","std::vector<double>",&pxPh_);
 tTreeUtilities_->Branch("pyPh_","std::vector<double>",&pyPh_);
 tTreeUtilities_->Branch("pzPh_","std::vector<double>",&pzPh_);
  
 
  }


//========================================================================
  void
    EtaAnalyzerWithMCTreeGenerator::analyze( const edm::Event& evt, const edm::EventSetup& es ) {
//========================================================================

   using namespace edm; // needed for all fwk related classes
   edm::LogInfo("PhotonAnalyzer") << "Analyzing event number: " << evt.id() << "\n";


     //---- clear the vectors ----
   
   etaC_->clear();
   thetaC_->clear();
   phiC_->clear();
   S4oS9C_->clear();
   S4C_->clear();
   S9C_->clear();
   S16C_->clear();
   S25C_->clear();
   pxC_->clear();
   pyC_->clear();
   pzC_->clear();
   etC_->clear();
   HitsC_->clear();
   HitsEnergyC_->clear();
   
   numPh_->clear();
   thetaPh_->clear();
   etaPh_->clear();
   phiPh_->clear();
   pxPh_->clear();
   pyPh_->clear();
   pzPh_->clear();
 
  /// Get the MC truth
   Handle< HepMCProduct > hepProd ;
   evt.getByLabel( "source",  hepProd ) ;
   const HepMC::GenEvent * myGenEvent = hepProd->GetEvent();


   //---- Eta selection ----

   numEta_ = 0;
   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {
    if ( !( (*p)->pdg_id() == 221 && (*p)->status() == 2 )  )  continue;
    numEta_++;
    //---- photons from eta 
    //---- loop over daughters
    int nGammaFromEta = 0;
    for (HepMC::GenVertex::particle_iterator d = (*p)->end_vertex()->particles_begin(HepMC::children); d != (*p)->end_vertex()->particles_end(HepMC::children); ++d){
     if ((abs((*d)->pdg_id()) == 22) && (*d)->status()==1)  {
      
      //---- fill the TTree ----
      thetaPh_->push_back((*d)->momentum().theta());
      etaPh_->push_back((*d)->momentum().eta());
      phiPh_->push_back((*d)->momentum().phi());
      pxPh_->push_back((*d)->momentum().x());
      pyPh_->push_back((*d)->momentum().y());
      pzPh_->push_back((*d)->momentum().z());
      nGammaFromEta++;
     }
    }
    numPh_->push_back(nGammaFromEta);
   }

  // USO I BASIC CLUSTERS invece DEI FOTONI
  // loop over basic clusters
   
    
  //---- basic cluster Barrel ----   
   Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters;
  //evt.getByLabel(islandBCProd_, islandBCColl_, pIslandBarrelBasicClusters);
   evt.getByLabel(islandBarrelBasicClustersProducer_,islandBarrelBasicClusters_,pIslandBarrelBasicClusters);
//    evt.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters);
   const reco::BasicClusterCollection* islandBarrelBasicClusters = pIslandBarrelBasicClusters.product();


  //---- Get association maps linking BasicClusters to ClusterShape
   edm::Handle<reco::BasicClusterShapeAssociationCollection> barrelClShpHandle;
   evt.getByLabel(islandBarrelShapeAssocProducer_,islandBarrelShapeAssoc_, barrelClShpHandle);
//    evt.getByLabel("islandBasicClusters","islandBarrelShapeAssoc", barrelClShpHandle);
   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;

   
   
   
   //---- basic cluster Endcap ----   
   
   Handle<reco::BasicClusterCollection> pIslandEndcapBasicClusters;
   evt.getByLabel(islandEndcapBasicClustersProducer_,islandEndcapBasicClusters_,pIslandEndcapBasicClusters);
//    evt.getByLabel("islandBasicClusters","islandEndcapBasicClusters",pIslandEndcapBasicClusters);
   const reco::BasicClusterCollection* islandEndcapBasicClusters = pIslandEndcapBasicClusters.product();


  //Get association maps linking BasicClusters to ClusterShape
   edm::Handle<reco::BasicClusterShapeAssociationCollection> endcapClShHandle;
   evt.getByLabel(islandEndcapShapeAssocProducer_,islandEndcapShapeAssoc_, endcapClShHandle);
//    evt.getByLabel("islandBasicClusters","islandEndcapShapeAssoc", endcapClShHandle);
   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItrEE;

   
   //---- TTree Analysis ----
 
   const EBRecHitCollection* barrelHitsCollection = 0;
   edm::Handle<EBRecHitCollection> barrelRecHitsHandle ;
   evt.getByLabel (barrelEcalHits_,barrelRecHitsHandle) ;
   barrelHitsCollection = barrelRecHitsHandle.product () ;
   
   const EERecHitCollection* endcapHitsCollection = 0;
   edm::Handle<EERecHitCollection> endcapRecHitsHandle ;
   evt.getByLabel (endcapEcalHits_,endcapRecHitsHandle) ;
   endcapHitsCollection = endcapRecHitsHandle.product () ;
   
    
   numC_ = 0;
   int iClus = 0;
  //---- Barrel ----
   numCB_ = 0;
   for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters->begin(); aClus != islandBarrelBasicClusters->end(); aClus++) {
    numC_++;
    numCB_++;
    reco::BasicCluster bc = reco::BasicCluster(*aClus);
    reco::BasicClusterRef clusterRef( pIslandBarrelBasicClusters,iClus);
    iClus++;
    
    float theta = 2. * atan(exp(-aClus->position().eta()));
    float p0x = aClus->energy() * sin(theta) * cos(aClus->position().phi());
    float p0y = aClus->energy() * sin(theta) * sin(aClus->position().phi());
    float p0z = aClus->energy() * cos(theta);
    double et = sqrt( p0x*p0x + p0y*p0y);
    
    thetaC_->push_back(theta);
    etaC_->push_back(aClus->position().eta());
    phiC_->push_back(aClus->position().phi());
    pxC_->push_back(p0x);
    pyC_->push_back(p0y);
    pzC_->push_back(p0z);
    etC_->push_back(et);
    HitsC_->push_back(bc.getHitsByDetId().size());
    std::vector<DetId> vector_DetId_temp = bc.getHitsByDetId();
    for (std::vector<DetId>::const_iterator iteratorDetId = vector_DetId_temp.begin(); iteratorDetId != vector_DetId_temp.end(); iteratorDetId++){
     EBRecHitCollection::const_iterator itrechit;
     itrechit = barrelHitsCollection->find(*iteratorDetId);
     if (itrechit == barrelHitsCollection->end()) continue;
     double dummy = 0;
     dummy = itrechit->energy () ;
     HitsEnergyC_->push_back(dummy);
    }
    seedShpItr = barrelClShpHandle->find(clusterRef);
    reco::ClusterShapeRef seedShapeRef = (*seedShpItr).val;
    
    S4C_->push_back(seedShapeRef->e2x2());
    S9C_->push_back(seedShapeRef->e3x3());
    S16C_->push_back(seedShapeRef->e4x4());
    S25C_->push_back(seedShapeRef->e5x5());
    S4oS9C_->push_back(seedShapeRef->e2x2()/seedShapeRef->e3x3());
   }
   
      
   int jClus = 0;
   //---- Endcap reduction ----
   numCE_ = 0;
   for(reco::BasicClusterCollection::const_iterator aClus = islandEndcapBasicClusters->begin(); aClus != islandEndcapBasicClusters->end(); aClus++) {
    numC_++;
    numCE_++;
    reco::BasicCluster bc = reco::BasicCluster(*aClus);
    reco::BasicClusterRef clusterRefEE( pIslandEndcapBasicClusters,jClus);
    jClus++;

    float theta = 2. * atan(exp(-aClus->position().eta()));
    float p0x = aClus->energy() * sin(theta) * cos(aClus->position().phi());
    float p0y = aClus->energy() * sin(theta) * sin(aClus->position().phi());
    float p0z = aClus->energy() * cos(theta);
    float et = sqrt( p0x*p0x + p0y*p0y);

    thetaC_->push_back(theta);
    etaC_->push_back(aClus->position().eta());
    phiC_->push_back(aClus->position().phi());
    pxC_->push_back(p0x);
    pyC_->push_back(p0y);
    pzC_->push_back(p0z);
    etC_->push_back(sqrt( p0x*p0x + p0y*p0y));
    HitsC_->push_back(bc.getHitsByDetId().size());
    
    std::vector<DetId> vector_DetId_temp = bc.getHitsByDetId();
    for (std::vector<DetId>::const_iterator iteratorDetId = vector_DetId_temp.begin(); iteratorDetId != vector_DetId_temp.end(); iteratorDetId++){
     EERecHitCollection::const_iterator itrechit;
     itrechit = endcapHitsCollection->find(*iteratorDetId);
     if (itrechit == barrelHitsCollection->end()) continue;
     double dummy = 0;
     dummy = itrechit->energy () ;
     HitsEnergyC_->push_back(dummy);
    }

    seedShpItrEE = endcapClShHandle->find(clusterRefEE);
    reco::ClusterShapeRef seedShapeRef = (*seedShpItrEE).val;
    
    S4C_->push_back(seedShapeRef->e2x2());
    S9C_->push_back(seedShapeRef->e3x3());
    S16C_->push_back(seedShapeRef->e4x4());
    S25C_->push_back(seedShapeRef->e5x5());
    S4oS9C_->push_back(seedShapeRef->e2x2()/seedShapeRef->e3x3());
   }

   tTreeUtilities_->Fill();
   
  }

    
    
    
  float EtaAnalyzerWithMCTreeGenerator::etaTransformation(  float EtaParticle , float Zvertex)  {


  //---Definitions for ECAL
     const float R_ECAL           = 136.5;
     const float Z_Endcap         = 328.0;
     const float etaBarrelEndcap  = 1.479; 
   
  //---ETA correction

     float Theta = 0.0  ; 
     float ZEcal = R_ECAL*sinh(EtaParticle)+Zvertex;

     if(ZEcal != 0.0) Theta = atan(R_ECAL/ZEcal);
     if(Theta<0.0) Theta = Theta+PI ;
     float ETA = - log(tan(0.5*Theta));
         
     if( fabs(ETA) > etaBarrelEndcap )
     {
      float Zend = Z_Endcap ;
      if(EtaParticle<0.0 )  Zend = -Zend ;
      float Zlen = Zend - Zvertex ;
      float RR = Zlen/sinh(EtaParticle); 
      Theta = atan(RR/Zend);
      if(Theta<0.0) Theta = Theta+PI ;
      ETA = - log(tan(0.5*Theta));            
     } 
  //---Return the result
     return ETA;
  //---end
    }

    

    
    
    
//========================================================================
    double
       EtaAnalyzerWithMCTreeGenerator::deltaPhi(double phi1,double phi2) {
//========================================================================
     double deltaphi = fabs(phi1-phi2);  
     if (deltaphi > TWOPI) deltaphi -= TWOPI;  
     if (deltaphi > PI ) deltaphi = TWOPI - deltaphi;  
     return deltaphi; 
       }

 
 
    
    

//========================================================================
       void
         EtaAnalyzerWithMCTreeGenerator::endJob() {
//========================================================================

        delete etaC_;
        delete thetaC_;
        delete phiC_;
        delete S4oS9C_;
        delete S4C_;
        delete S9C_;
        delete S16C_;
        delete S25C_;
        delete pxC_;
        delete pyC_;
        delete pzC_;
        delete etC_;
        delete HitsC_;
        delete HitsEnergyC_;
        
        delete numPh_;
        delete thetaPh_;
        delete etaPh_;
        delete phiPh_;
        delete pxPh_;
        delete pyPh_;
        delete pzPh_;
        
        

         }

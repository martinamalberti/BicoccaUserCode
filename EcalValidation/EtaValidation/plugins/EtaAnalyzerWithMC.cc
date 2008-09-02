
#include "EcalValidation/EtaValidation/plugins/EtaAnalyzerWithMC.h"
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
EtaAnalyzerWithMC::EtaAnalyzerWithMC( const edm::ParameterSet& ps )
//========================================================================
{
 photonCollectionProducer_ = ps.getParameter<std::string>("phoProducer");
 photonCollection_ = ps.getParameter<std::string>("photonCollection");

 barrelEcalHits_   = ps.getParameter<edm::InputTag>("barrelEcalHits");
 endcapEcalHits_   = ps.getParameter<edm::InputTag>("endcapEcalHits");



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
EtaAnalyzerWithMC::~EtaAnalyzerWithMC()
//========================================================================
{


}

//========================================================================
void
  EtaAnalyzerWithMC::beginJob(edm::EventSetup const&) {
//========================================================================

 edm::Service<TFileService> fs;

 hNphotonsFromEta_ = fs->make<TH1F>("hNphotonsFromEta"," Number of photons from Eta",10,0, 10);
 hNphotonsInEvent_ = fs->make<TH1F>("hNphotonsInEvent"," Number of photons in an Event",10,0, 10);
 hNEtaInEvent_ = fs->make<TH1F>("hNEtaInEvent"," Number of Eta in an Event",10,0, 10);

 hSCe1_    = fs->make<TH1F>("hSCe1"," SC Energy ",200,0., 100.);

 hSCet1_    = fs->make<TH1F>("hSCet1"," SC Et ",200,0., 100.);
 hSCet2_    = fs->make<TH1F>("hSCet2"," SC Et ",200,0., 100.);
  
 hInvMass_  = fs->make<TH1F>("hInvMass","Invariant mass",1000, 0., 1.);
 
 hInvMassEtaPh_  = fs->make<TH1F>("hInvMassEtaPh","Invariant mass Eta Photon",1000, 0., 1.);
 hInvMassEtaPhCMatch_  = fs->make<TH1F>("hInvMassEtaPhCMatch","Invariant mass Eta Photon match C",1000, 0., 1.);
 
 hInvMassEta_  = fs->make<TH1F>("hInvMassEta","Invariant mass Eta",1000, 0., 1.);
 hInvMassEtaBB_  = fs->make<TH1F>("hInvMassEtaBB","Invariant mass Eta Barrel Barrel",1000, 0., 1.);
 hInvMassEtaEE_  = fs->make<TH1F>("hInvMassEtaEE","Invariant mass Eta Endcap Endcap",1000, 0., 1.);
 hInvMassEtaEB_  = fs->make<TH1F>("hInvMassEtaEB","Invariant mass Eta Endcap Barrel",1000, 0., 1.);
 hInvMassNoEta_  = fs->make<TH1F>("hInvMassNoEta","Invariant mass No Eta",1000, 0., 1.);
 hInvMassNoEtaBarrel_  = fs->make<TH1F>("hInvMassNoEtaBarrel","Invariant mass No Eta Barrel",1000, 0., 1.);
 hInvMassNoEtaEndcap_  = fs->make<TH1F>("hInvMassNoEtaEndcap","Invariant mass No Eta Endcap",1000, 0., 1.);
 
 hInvMassPi0_  = fs->make<TH1F>("hInvMassPi0","Invariant mass Pi0",1000, 0., 1.);
 hInvMassPi0BB_  = fs->make<TH1F>("hInvMassPi0BB","Invariant mass Pi0 Barrel Barrel",1000, 0., 1.);
 hInvMassPi0EE_  = fs->make<TH1F>("hInvMassPi0EE","Invariant mass Pi0 Endcap Endcap",1000, 0., 1.);
 hInvMassPi0EB_  = fs->make<TH1F>("hInvMassPi0EB","Invariant mass Pi0 Endcap Barrel",1000, 0., 1.);
 hInvMassNoPi0_  = fs->make<TH1F>("hInvMassNoPi0","Invariant mass No Pi0",1000, 0., 1.);

 hEtaPhEnergy_  = fs->make<TH1F>("hEtaPhEnergy","Eta Photon Energy",1000, 0., 1.); 
 hKindOfParticle_ = fs->make<TH2F>("hKindOfParticle","Kind of Generated Particles",10000,0,10000,10,0,10);
 hKindOfGeneratedParticle_ = fs->make<TH2F>("hKindOfGeneratedParticle","Kind of Generated Particles from an Eta",10000,0,10000,10,0,10);
    
 hPtgg_     = fs->make<TH1F>("hPtgg","Pt_{#gamma#gamma}",100, 0., 100.);
  
 hS4oS9_    = fs->make<TH1F>("hS4oS9","S4/S9",500, 0., 5.);
  
 hR_        = fs->make<TH1F>("hR","R(gamma-gamma)",1000, 0., 10.);
  
 hIsolation_ = fs->make<TH1F>("hIsolation","Isolation",1000, 0., 10.);

 hInvMassMCAndC_ = fs->make<TH2F>("hInvMassMCAndC","Invariant Mass MC Truth and Cluster",1000, 0., 1.,1000, 0., 1.);
 
 
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
  
 
 //---- name of axis ----
 
 hInvMassMCAndC_->GetXaxis()->SetTitle("Energy MC");
 hInvMassMCAndC_->GetYaxis()->SetTitle("Energy Cluster");
 
 
  }


//========================================================================
  void
    EtaAnalyzerWithMC::analyze( const edm::Event& evt, const edm::EventSetup& es ) {
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


   std::vector<HepMC::GenParticle*> genPhotonsEta;
   std::vector<HepMC::GenParticle*> genPhotonsPi0;
   

   
   //---- Eta selection ----

   numEta_ = 0;
   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {
    hKindOfParticle_->Fill((*p)->pdg_id(),(*p)->status());
    if ( !( (*p)->pdg_id() == 221 && (*p)->status() == 2 )  )  continue;
    numEta_++;
    //---- photons from eta 
    //---- loop over daughters
    
    math::XYZTLorentzVector pPh(0,0,0,0);
    
    
    int nGammaFromEta = 0;
    
    for (HepMC::GenVertex::particle_iterator d = (*p)->end_vertex()->particles_begin(HepMC::children); d != (*p)->end_vertex()->particles_end(HepMC::children); ++d){
     hKindOfGeneratedParticle_->Fill((*d)->pdg_id(),(*d)->status());
     if ((abs((*d)->pdg_id()) == 22) && (*d)->status()==1)  {
      
      //---- fill the TTree ----
      thetaPh_->push_back((*d)->momentum().theta());
      etaPh_->push_back((*d)->momentum().eta());
      phiPh_->push_back((*d)->momentum().phi());
      pxPh_->push_back((*d)->momentum().x());
      pyPh_->push_back((*d)->momentum().y());
      pzPh_->push_back((*d)->momentum().z());
      
      
      //---- Invariant Mass ----
      math::XYZTLorentzVector p_temp((*d)->momentum().x(),(*d)->momentum().y(),(*d)->momentum().z(),(*d)->momentum().t());
      pPh = pPh + p_temp;
      
      genPhotonsEta.push_back((*d));
      nGammaFromEta++;
      hEtaPhEnergy_->Fill(1);
     }
    }
    
    float InvMass = pPh.mag();
    hInvMassEtaPh_->Fill(InvMass);
    numPh_->push_back(nGammaFromEta);
    hNphotonsFromEta_->Fill(nGammaFromEta);

   }

   hNphotonsInEvent_->Fill(genPhotonsEta.size());
   hNEtaInEvent_->Fill(numEta_);


   
   //---- Pi0 selection ----   
   
   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {
    hKindOfParticle_->Fill((*p)->pdg_id(),(*p)->status());
    if ( !( (*p)->pdg_id() == 111 && (*p)->status() == 2 )  )  continue;
     //---- photons from pi0 
     //---- loop over daughters
    for (HepMC::GenVertex::particle_iterator d = (*p)->end_vertex()->particles_begin(HepMC::children); d != (*p)->end_vertex()->particles_end(HepMC::children); ++d){
     hKindOfGeneratedParticle_->Fill((*d)->pdg_id(),(*d)->status());
     if ((abs((*d)->pdg_id()) == 22) && (*d)->status()==1)  {
      genPhotonsPi0.push_back((*d));
     }
    }
   }




  // USO I BASIC CLUSTERS invece DEI FOTONI
  // loop over basic clusters
  //---- basic cluster Barrel ----   
   Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters;
  //evt.getByLabel(islandBCProd_, islandBCColl_, pIslandBarrelBasicClusters);
   evt.getByLabel("islandBasicClusters", "islandBarrelBasicClusters",pIslandBarrelBasicClusters);
   const reco::BasicClusterCollection* islandBarrelBasicClusters = pIslandBarrelBasicClusters.product();


  //Get association maps linking BasicClusters to ClusterShape
   edm::Handle<reco::BasicClusterShapeAssociationCollection> barrelClShpHandle;
   evt.getByLabel("islandBasicClusters", "islandBarrelShapeAssoc", barrelClShpHandle);
   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;

   
   
   
   //---- basic cluster Endcap ----   
   
   Handle<reco::BasicClusterCollection> pIslandEndcapBasicClusters;
   evt.getByLabel("islandBasicClusters", "islandEndcapBasicClusters",pIslandEndcapBasicClusters);
   const reco::BasicClusterCollection* islandEndcapBasicClusters = pIslandEndcapBasicClusters.product();


  //Get association maps linking BasicClusters to ClusterShape
   edm::Handle<reco::BasicClusterShapeAssociationCollection> endcapClShHandle;
   evt.getByLabel("islandBasicClusters", "islandEndcapShapeAssoc", endcapClShHandle);
   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItrEE;

   
   //---- reduce basic cluster and join Endcap and Barrel ----
   
   std::vector<reco::BasicCluster> localClusters;
   std::vector<double> S4oS9;

   int iClus = 0;

   
   //---- Barrel reduction ----
   for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters->begin(); aClus != islandBarrelBasicClusters->end(); aClus++) {
 
    reco::BasicCluster bc = reco::BasicCluster(*aClus);
    reco::BasicClusterRef clusterRef( pIslandBarrelBasicClusters,iClus);
    iClus++;

    float theta = 2. * atan(exp(-aClus->position().eta()));
    float p0x = aClus->energy() * sin(theta) * cos(aClus->position().phi());
    float p0y = aClus->energy() * sin(theta) * sin(aClus->position().phi());
    float p0z = aClus->energy() * cos(theta);
    float et = sqrt( p0x*p0x + p0y*p0y);

    seedShpItr = barrelClShpHandle->find(clusterRef);
    reco::ClusterShapeRef seedShapeRef = (*seedShpItr).val;
    if ( bc.getHitsByDetId().size() > 5) hS4oS9_->Fill( seedShapeRef->e2x2()/seedShapeRef->e3x3() );

    // qualche selezione 
    if (et < MinClusterEt_  ) continue; // photon pt


    //  std::cout<<"E="<<bc.energy()<<" S4 = "<< seedShapeRef->e2x2() << "  S9 = "<<seedShapeRef->e3x3()<< "  S4oS9 = "<< seedShapeRef->e2x2()/seedShapeRef->e3x3()<< " #xtals = "<< bc.getHitsByDetId().size() <<std::endl;
    localClusters.push_back(bc);
    S4oS9.push_back((seedShapeRef->e2x2()/seedShapeRef->e3x3()));
   }

   
   int jClus = 0;
   //---- Endcap reduction ----
   for(reco::BasicClusterCollection::const_iterator aClus = islandEndcapBasicClusters->begin(); aClus != islandEndcapBasicClusters->end(); aClus++) {
 
    reco::BasicCluster bc = reco::BasicCluster(*aClus);
    reco::BasicClusterRef clusterRefEE( pIslandEndcapBasicClusters,jClus);
    jClus++;

    float theta = 2. * atan(exp(-aClus->position().eta()));
    float p0x = aClus->energy() * sin(theta) * cos(aClus->position().phi());
    float p0y = aClus->energy() * sin(theta) * sin(aClus->position().phi());
    float p0z = aClus->energy() * cos(theta);
    float et = sqrt( p0x*p0x + p0y*p0y);

    seedShpItrEE = endcapClShHandle->find(clusterRefEE);
    reco::ClusterShapeRef seedShapeRef = (*seedShpItrEE).val;
    if ( bc.getHitsByDetId().size() > 5) hS4oS9_->Fill( seedShapeRef->e2x2()/seedShapeRef->e3x3() );

    // qualche selezione 
    if (et < MinClusterEt_  ) continue; // photon pt
    localClusters.push_back(bc);
    S4oS9.push_back((seedShapeRef->e2x2()/seedShapeRef->e3x3()));
   }



   //----- start Analysis ----
   
   
   for (int i = 0; i < localClusters.size(); i++){

    reco::BasicCluster bc1 = localClusters.at(i);
   
    float theta1 = 2. * atan(exp(-bc1.position().eta()));
    float p1x = bc1.energy() * sin(theta1) * cos(bc1.position().phi());
    float p1y = bc1.energy() * sin(theta1) * sin(bc1.position().phi());
    float p1z = bc1.energy() * cos(theta1);
    math::XYZTLorentzVector p1(p1x, p1y, p1z,  bc1.energy());
   
    hS4oS9_->Fill(S4oS9[i]);
    
    if (S4oS9[i] < MinE4oE9_ ) continue;
    if (bc1.energy() * sin(theta1) < MinPtGamma_ ) continue;


    for (int j = i+1; j < localClusters.size(); j++){
     reco::BasicCluster bc2 = localClusters.at(j);
     float theta2 = 2. * atan(exp(-bc2.position().eta()));
     float p2x = bc2.energy() * sin(theta2) * cos(bc2.position().phi());
     float p2y = bc2.energy() * sin(theta2) * sin(bc2.position().phi());
     float p2z = bc2.energy() * cos(theta2);
     math::XYZTLorentzVector p2(p2x, p2y, p2z,  bc2.energy());

     if (S4oS9[j] < MinE4oE9_ ) continue;
     if (bc2.energy() * sin(theta2) < MinPtGamma_ ) continue;

      // PT(gammagamma)
     math::XYZTLorentzVector p = p1 +  p2 ;
     float PtPair = sqrt(pow(p1x+p2x,2) + pow(p1y+p2y,2));
     hPtgg_ ->Fill(PtPair); 
      
      // Isolation       
     float iso = 0;
     for(Int_t k = 0 ; k < localClusters.size() ; k++)
     {
      if( (k != i) && (k != j) ) 
      {
       reco::BasicCluster bc = localClusters.at(k);
       float R = sqrt( pow(bc.position().eta()-p.eta(),2) + pow(bc.position().phi()-p.phi(),2));
       float deta = fabs(bc.position().eta() - p.eta());
       float theta = 2. * atan(exp(-bc.position().eta()));
       float et =  bc.energy() * sin(theta);
       if ( (R < RMax_)  && (deta < DeltaEtaMax_) && (et > 1.)  ) iso = iso + et ;
      }
     }
     hIsolation_->Fill(iso/PtPair);     
     
     if (PtPair < MinPtGammaPair_ ) continue;
     if (iso > IsolationCut_) continue; 
 
      //float InvMass = sqrt ( (bc1.energy()+bc2.energy())*(bc1.energy()+bc2.energy() ) - (p2x+p1x)*(p2x+p1x) - (p2y+p1y)*(p2y+p1y) - (p2z+p1z)*(p2z+p1z) ); 

     float InvMass = p.mag();

     hInvMass_->Fill(InvMass);
     
     //---- check if there is a matching between a photon coming from eta and a basiccluster ----
     float dEta = 0.01;
     float dPhi = 0.1;
     float R = 0.1;
     bool flagOK = false;
     for (std::vector<HepMC::GenParticle*>::iterator iterator_genPhotonsEta = genPhotonsEta.begin(); iterator_genPhotonsEta != genPhotonsEta.end(); iterator_genPhotonsEta++){
      float etaPh = (*iterator_genPhotonsEta)->momentum().eta();
      float phiPh = (*iterator_genPhotonsEta)->momentum().phi();
  
      float etaBC2 = bc2.position().eta();
      float phiBC2 = bc2.position().phi();
      float etaBC1 = bc1.position().eta();
      float phiBC1 = bc1.position().phi();
           
      if (((etaPh-etaBC1)*(etaPh-etaBC1) + deltaPhi(phiPh,phiBC1)* deltaPhi(phiPh,phiBC1)) < R*R){
       for (std::vector<HepMC::GenParticle*>::iterator iterator_genPhotonsEtaIn = genPhotonsEta.begin();
            iterator_genPhotonsEtaIn != genPhotonsEta.end(); iterator_genPhotonsEtaIn++){
             float etaPhIn = (*iterator_genPhotonsEtaIn)->momentum().eta();
             float phiPhIn = (*iterator_genPhotonsEtaIn)->momentum().phi();
             if (((etaPhIn-etaBC2)*(etaPhIn-etaBC2) + deltaPhi(phiPhIn,phiBC2)* deltaPhi(phiPhIn,phiBC2)) < R*R){
              
              math::XYZTLorentzVector p_temp1((*iterator_genPhotonsEta)->momentum().x(),(*iterator_genPhotonsEta)->momentum().y(),(*iterator_genPhotonsEta)->momentum().z(),(*iterator_genPhotonsEta)->momentum().t());
              math::XYZTLorentzVector p_temp2((*iterator_genPhotonsEtaIn)->momentum().x(),(*iterator_genPhotonsEtaIn)->momentum().y(),(*iterator_genPhotonsEtaIn)->momentum().z(),(*iterator_genPhotonsEtaIn)->momentum().t());
              math::XYZTLorentzVector pPh_Tot = p_temp1 + p_temp2;
              float InvMassMCTruth = pPh_Tot.mag();
              hInvMassEtaPhCMatch_->Fill(InvMassMCTruth);
              hInvMassEta_->Fill(InvMass);
              hInvMassMCAndC_->Fill(InvMassMCTruth,InvMass);
              if ((etaBC2*etaBC2 > 1.49*1.49) && (etaBC1*etaBC1 > 1.49*1.49)) hInvMassEtaEE_->Fill(InvMass);
              if ((etaBC2*etaBC2 > 1.49*1.49) && (etaBC1*etaBC1 < 1.49*1.49)) hInvMassEtaEB_->Fill(InvMass);
              if ((etaBC2*etaBC2 < 1.49*1.49) && (etaBC1*etaBC1 < 1.49*1.49)) hInvMassEtaBB_->Fill(InvMass);
              flagOK = true;
             }
            }
      }  
     }
     if (!flagOK) {
      hInvMassNoEta_->Fill(InvMass); //---- two photons that are not due to an eta ----
      if (bc1.position().eta() * bc1.position().eta() > 1.49*1.49) hInvMassNoEtaEndcap_->Fill(InvMass);
      else hInvMassNoEtaBarrel_->Fill(InvMass);
     }
          
          
          //---- check if there is a matching between a photon coming from Pi0 and a basiccluster ----
     flagOK = false;
     for (std::vector<HepMC::GenParticle*>::iterator iterator_genPhotonsPi0 = genPhotonsPi0.begin(); iterator_genPhotonsPi0 != genPhotonsPi0.end(); iterator_genPhotonsPi0++){
      float etaPh = (*iterator_genPhotonsPi0)->momentum().eta();
      float phiPh = (*iterator_genPhotonsPi0)->momentum().phi();
  
      float etaBC2 = bc2.position().eta();
      float phiBC2 = bc2.position().phi();
      float etaBC1 = bc1.position().eta();
      float phiBC1 = bc1.position().phi();
           
      if (((etaPh-etaBC1)*(etaPh-etaBC1) + deltaPhi(phiPh,phiBC1)* deltaPhi(phiPh,phiBC1)) < R*R){
       for (std::vector<HepMC::GenParticle*>::iterator iterator_genPhotonsPi0In = genPhotonsPi0.begin();
            iterator_genPhotonsPi0In != genPhotonsPi0.end(); iterator_genPhotonsPi0In++){
             float etaPhIn = (*iterator_genPhotonsPi0In)->momentum().eta();
             float phiPhIn = (*iterator_genPhotonsPi0In)->momentum().phi();
             if (((etaPhIn-etaBC2)*(etaPhIn-etaBC2) + deltaPhi(phiPhIn,phiBC2)* deltaPhi(phiPhIn,phiBC2)) < R*R){
              hInvMassPi0_->Fill(InvMass);
              if ((etaBC2*etaBC2 > 1.49*1.49) && (etaBC1*etaBC1 > 1.49*1.49)) hInvMassPi0EE_->Fill(InvMass);
              if ((etaBC2*etaBC2 > 1.49*1.49) && (etaBC1*etaBC1 < 1.49*1.49)) hInvMassPi0EB_->Fill(InvMass);
              if ((etaBC2*etaBC2 < 1.49*1.49) && (etaBC1*etaBC1 < 1.49*1.49)) hInvMassPi0BB_->Fill(InvMass);
              flagOK = true;
             }
            }
      }  
     }
     if (!flagOK) {
      hInvMassNoPi0_->Fill(InvMass); //---- two photons that are not due to Pi0 ----
     }
          
          
          
          
    }
   }
   
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
   iClus = 0;
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
    float et = sqrt( p0x*p0x + p0y*p0y);
    
    thetaC_->push_back(theta);
    etaC_->push_back(aClus->position().eta());
    phiC_->push_back(aClus->position().phi());
    pxC_->push_back(p0x);
    pyC_->push_back(p0y);
    pzC_->push_back(p0z);
    etC_->push_back(sqrt( p0x*p0x + p0y*p0y));
    HitsC_->push_back(bc.getHitsByDetId().size());

//     std::cerr << std::endl << "Entro" << std::endl;
    std::vector<DetId> vector_DetId_temp = bc.getHitsByDetId();
    for (std::vector<DetId>::const_iterator iteratorDetId = vector_DetId_temp.begin(); iteratorDetId != vector_DetId_temp.end(); iteratorDetId++){
//      std::cerr << "  Appena Entrato    " << std::endl;
     EBRecHitCollection::const_iterator itrechit;
     itrechit = barrelHitsCollection->find(*iteratorDetId);
//      std::cerr << "  Entrato    " << std::endl;
     if (itrechit == barrelHitsCollection->end()) continue;
     double dummy = 0;
     dummy = itrechit->energy () ;
//      std::cerr << std::endl << "energy = " << dummy << std::endl;
     HitsEnergyC_->push_back(dummy);
//      std::cerr << "    buttato" << std::endl;
    }
    
//     std::cerr << " Uscito" << std::endl;
    
    seedShpItr = barrelClShpHandle->find(clusterRef);
    reco::ClusterShapeRef seedShapeRef = (*seedShpItr).val;
    
    S4C_->push_back(seedShapeRef->e2x2());
    S9C_->push_back(seedShapeRef->e3x3());
    S16C_->push_back(seedShapeRef->e4x4());
    S25C_->push_back(seedShapeRef->e5x5());
    S4oS9C_->push_back(seedShapeRef->e2x2()/seedShapeRef->e3x3());
   }

   
   
   
   
   
   
   
   
      
   jClus = 0;
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

    
    
    
    float EtaAnalyzerWithMC::etaTransformation(  float EtaParticle , float Zvertex)  {


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
       EtaAnalyzerWithMC::deltaPhi(double phi1,double phi2) {
//========================================================================
     double deltaphi = fabs(phi1-phi2);  
     if (deltaphi > TWOPI) deltaphi -= TWOPI;  
     if (deltaphi > PI ) deltaphi = TWOPI - deltaphi;  
     return deltaphi; 
       }

 
 
    
    

//========================================================================
       void
         EtaAnalyzerWithMC::endJob() {
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

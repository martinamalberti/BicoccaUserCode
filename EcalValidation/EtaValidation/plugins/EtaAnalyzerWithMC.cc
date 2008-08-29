
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

 hNphotons_ = fs->make<TH1F>("hNphotons"," Number of photons",10,0, 10);

 hSCe1_    = fs->make<TH1F>("hSCe1"," SC Energy ",200,0., 100.);

 hSCet1_    = fs->make<TH1F>("hSCet1"," SC Et ",200,0., 100.);
 hSCet2_    = fs->make<TH1F>("hSCet2"," SC Et ",200,0., 100.);
  
 hInvMass_  = fs->make<TH1F>("hInvMass","Invariant mass",1000, 0., 1.);
 hInvMassEta_  = fs->make<TH1F>("hInvMassEta","Invariant mass Eta",1000, 0., 1.);
 hInvMassEtaBarrel_  = fs->make<TH1F>("hInvMassEtaBarrel","Invariant mass Eta Barrel",1000, 0., 1.);
 hInvMassEtaEndcap_  = fs->make<TH1F>("hInvMassEtaEndcap","Invariant mass Eta Endcap",1000, 0., 1.);
 hInvMassNoEta_  = fs->make<TH1F>("hInvMassNoEta","Invariant mass No Eta",1000, 0., 1.);
 hInvMassNoEtaBarrel_  = fs->make<TH1F>("hInvMassNoEtaBarrel","Invariant mass No Eta Barrel",1000, 0., 1.);
 hInvMassNoEtaEndcap_  = fs->make<TH1F>("hInvMassNoEtaEndcap","Invariant mass No Eta Endcap",1000, 0., 1.);
 
 hEtaPhEnergy_  = fs->make<TH1F>("hEtaPhEnergy","Eta Photon Energy",1000, 0., 1.); 
 hKindOfParticle_ = fs->make<TH2F>("hKindOfParticle","Kind of Generated Particles",10000,0,10000,10,0,10);
 hKindOfGeneratedParticle_ = fs->make<TH2F>("hKindOfGeneratedParticle","Kind of Generated Particles from an Eta",10000,0,10000,10,0,10);
    
 hPtgg_     = fs->make<TH1F>("hPtgg","Pt_{#gamma#gamma}",100, 0., 100.);
  
 hS4oS9_    = fs->make<TH1F>("hS4oS9","S4/S9",500, 0., 5.);
  
 hR_        = fs->make<TH1F>("hR","R(gamma-gamma)",1000, 0., 10.);
  
 hIsolation_ = fs->make<TH1F>("hIsolation","Isolation",1000, 0., 10.);

  }


//========================================================================
  void
    EtaAnalyzerWithMC::analyze( const edm::Event& evt, const edm::EventSetup& es ) {
//========================================================================

   using namespace edm; // needed for all fwk related classes
   edm::LogInfo("PhotonAnalyzer") << "Analyzing event number: " << evt.id() << "\n";


 
  /// Get the MC truth
   Handle< HepMCProduct > hepProd ;
   evt.getByLabel( "source",  hepProd ) ;
   const HepMC::GenEvent * myGenEvent = hepProd->GetEvent();


   std::vector<HepMC::GenParticle*> genPhotons;

   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {
   
//     genPhotons.push_back((*p));
    hKindOfParticle_->Fill((*p)->pdg_id(),(*p)->status());
    if ( !( (*p)->pdg_id() == 221 && (*p)->status() == 2 )  )  continue;
//     if ( !( (*p)->pdg_id() == 221 && (*p)->status() == 1 )  )  continue;
    
    // photons from eta 
      //loop over daughters
    
    
    for (HepMC::GenVertex::particle_iterator d = (*p)->end_vertex()->particles_begin(HepMC::children); d != (*p)->end_vertex()->particles_end(HepMC::children); ++d){
     
     hKindOfGeneratedParticle_->Fill((*d)->pdg_id(),(*d)->status());
    
//     for (HepMC::GenVertex::particle_iterator d = (*p)->end_vertex()->particles_begin(HepMC::children); d != (*p)->end_vertex()->particles_end(HepMC::children); ++d){
     if ((abs((*d)->pdg_id()) == 22) && (*d)->status()==1)  {
      genPhotons.push_back((*d));
      hEtaPhEnergy_->Fill(1);
     }
    }
    hNphotons_->Fill(genPhotons.size());
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

   
   
   //---- Endcap reduction ----
   for(reco::BasicClusterCollection::const_iterator aClus = islandEndcapBasicClusters->begin(); aClus != islandEndcapBasicClusters->end(); aClus++) {
 
    reco::BasicCluster bc = reco::BasicCluster(*aClus);
    reco::BasicClusterRef clusterRef( pIslandEndcapBasicClusters,iClus);
    iClus++;

    float theta = 2. * atan(exp(-aClus->position().eta()));
    float p0x = aClus->energy() * sin(theta) * cos(aClus->position().phi());
    float p0y = aClus->energy() * sin(theta) * sin(aClus->position().phi());
    float p0z = aClus->energy() * cos(theta);
    float et = sqrt( p0x*p0x + p0y*p0y);

    seedShpItrEE = endcapClShHandle->find(clusterRef);
    reco::ClusterShapeRef seedShapeRef = (*seedShpItrEE).val;
//     if ( bc.getHitsByDetId().size() > 5) hS4oS9_->Fill( seedShapeRef->e2x2()/seedShapeRef->e3x3() );
// 
//     // qualche selezione 
//     if (et < MinClusterEt_  ) continue; // photon pt
//     localClusters.push_back(bc);
//     S4oS9.push_back((seedShapeRef->e2x2()/seedShapeRef->e3x3()));
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
     for (std::vector<HepMC::GenParticle*>::iterator iterator_genPhotons = genPhotons.begin();
          iterator_genPhotons != genPhotons.end(); iterator_genPhotons++){
//            HepMC::GenParticle phot(*iterator_genPhotons);
           
//            std::cerr << "Eta Cerr = " << (*iterator_genPhotons)->momentum().eta() << std::endl;
           
           float etaPh = (*iterator_genPhotons)->momentum().eta();
           float phiPh = (*iterator_genPhotons)->momentum().phi();
  
           float etaBC2 = bc2.position().eta();
           float phiBC2 = bc2.position().phi();
           float etaBC1 = bc1.position().eta();
           float phiBC1 = bc1.position().phi();
           
           if (((etaPh-etaBC1)*(etaPh-etaBC1) + (phiPh-phiBC1)*(phiPh-phiBC1)) < R*R){
            for (std::vector<HepMC::GenParticle*>::iterator iterator_genPhotonsIn = genPhotons.begin();
                 iterator_genPhotonsIn != genPhotons.end(); iterator_genPhotonsIn++){
                  float etaPhIn = (*iterator_genPhotonsIn)->momentum().eta();
                  float phiPhIn = (*iterator_genPhotonsIn)->momentum().phi();
                  if (((etaPhIn-etaBC2)*(etaPhIn-etaBC2) + (phiPhIn-phiBC2)*(phiPhIn-phiBC2)) < R*R){
                   hInvMassEta_->Fill(InvMass);
                   if (etaBC2*etaBC2 > 1.49*1.49) hInvMassEtaEndcap_->Fill(InvMass);
                   else hInvMassEtaBarrel_->Fill(InvMass);
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
          
    }
  
   }
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
    void
      EtaAnalyzerWithMC::endJob() {
//========================================================================



      }

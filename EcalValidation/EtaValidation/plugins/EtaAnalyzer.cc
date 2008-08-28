/**\class PhotonSimpleAnalyzer
 **
 ** $Date: 2008/08/28 09:54:42 $ 
 ** $Revision: 1.1 $
 ** \author Nancy Marinelli, U. of Notre Dame, US
*/

#include "EcalValidation/EtaValidation/plugins/EtaAnalyzer.h"
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
EtaAnalyzer::EtaAnalyzer( const edm::ParameterSet& ps )
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
EtaAnalyzer::~EtaAnalyzer()
//========================================================================
{


}

//========================================================================
void
EtaAnalyzer::beginJob(edm::EventSetup const&) {
//========================================================================

  edm::Service<TFileService> fs;

  hNphotons_ = fs->make<TH1F>("hNphotons"," Number of photons",10,0, 10);

  hSCe1_    = fs->make<TH1F>("hSCe1"," SC Energy ",200,0., 100.);

  hSCet1_    = fs->make<TH1F>("hSCet1"," SC Et ",200,0., 100.);
  hSCet2_    = fs->make<TH1F>("hSCet2"," SC Et ",200,0., 100.);
  
  hInvMass_  = fs->make<TH1F>("hInvMass","Invariant mass",1000, 0., 1.);
  
  hPtgg_     = fs->make<TH1F>("hPtgg","Pt_{#gamma#gamma}",100, 0., 100.);
  
  hS4oS9_    = fs->make<TH1F>("hS4oS9","S4/S9",500, 0., 5.);
  
  hR_        = fs->make<TH1F>("hR","R(gamma-gamma)",1000, 0., 10.);
  
  hIsolation_ = fs->make<TH1F>("hIsolation","Isolation",1000, 0., 10.);

}


//========================================================================
void
EtaAnalyzer::analyze( const edm::Event& evt, const edm::EventSetup& es ) {
//========================================================================

  using namespace edm; // needed for all fwk related classes
  edm::LogInfo("PhotonAnalyzer") << "Analyzing event number: " << evt.id() << "\n";


  /*
  // Get the  corrected  photon collection (set in the configuration) which also contains infos about conversions 
  Handle<reco::PhotonCollection> photonHandle; 
  evt.getByLabel(photonCollectionProducer_, photonCollection_ , photonHandle);
  const reco::PhotonCollection photonCollection = *(photonHandle.product());
 
  std::vector<reco::Photon> localPhotons;
  
  // loop over corrected  Photon candidates 
  for( reco::PhotonCollection::const_iterator  iPho = photonCollection.begin(); iPho != photonCollection.end(); iPho++) {
       
    reco::Photon photon = reco::Photon(*iPho);

    //Get the ClusterShapeRef corresponding to the BasicCluster
    reco::ClusterShapeRef seedShapeRef = iPho -> seedClusterShape();

    
    // supercluster ET 
    float et = photon.superCluster()->energy()/cosh(photon.superCluster()->position().eta()) ;
    
    //std::cout<<"   pt = "<<photon.pt()<<"     SC et = "<<et<<  "   e3x3 = "<< <<std::endl;
    std::cout<<" SC e = "<< photon.superCluster()->energy() <<  "   e3x3 = "<< seedShapeRef->e3x3() <<std::endl;
    
    float S4oS9 = seedShapeRef->e2x2()/seedShapeRef->e3x3();
    
    
    hS4oS9_->Fill(S4oS9);
 
    // qualche selezione 
    if (photon.pt() < MinPtGamma_  ) continue; // photon pt
    if ( S4oS9 < MinE4oE9_ ) continue; // S4/S9
    
    localPhotons.push_back(photon);
    
  } // End loop over uncorrected photons


  // massa invariante delle coppie di fotoni selezionati 

  Handle< edm::View<reco::Photon> > emObjectHandle;
  evt.getByLabel(photonCollectionProducer_, photonCollection_ , emObjectHandle);

  // Isolation
  edm::Handle< reco::CandViewDoubleAssociations > IsolationHandle;
  evt.getByLabel(IsolationProducer_,IsolationHandle);


  hNphotons_->Fill(localPhotons.size());

  int k = 0;
  for (int i = 0; i < localPhotons.size(); i++){

    reco::Photon photon1 = localPhotons.at(i);
    if (i==0) hSCet1_ ->Fill(photon1.pt());
    if (i==0) hSCe1_ ->Fill(photon1.superCluster()->energy());
 
    for (int j = i+1; j < localPhotons.size(); j++){
      
      reco::Photon photon2 = localPhotons.at(j);

      reco::Particle::LorentzVector p = photon1.p4() +  photon2.p4() ;

      std::cout<<" photon1.p4().mag() = "<<photon1.p4().mag() <<std::endl;
      std::cout<<" Invariant mass = "<< p.mag() <<"   p.e() = "<< p.e()<< " p.p() ="<<p.p()<<"  InvMass = "<< InvMass <<std::endl;
 


      //isolation
      edm::Ref< edm::View<reco::Photon> > photonRef(emObjectHandle,k);
      k++;
      edm::RefToBase<reco::Candidate> candRef = edm::RefToBase<reco::Candidate>(photonRef);
      double iso = (*IsolationHandle)[ candRef ];
      hIsolation_->Fill(iso/p.pt());
      
      hPtgg_ ->Fill(p.pt()); 
      
      if ( iso/p.pt() > IsolationCut_ ) continue;
      if ( p.pt() < MinPtGammaPair_ )      continue; 

     
     
      // photon separation
      double deta =  fabs(photon1.eta() - photon2.eta());
      double dphi =  fabs(photon1.phi() - photon2.phi());
      if (dphi > TWOPI) dphi = dphi-TWOPI;
      double R = sqrt(deta*deta+dphi*dphi); 
      hR_ ->Fill(R);


      if (R < RMax_  && deta < DeltaEtaMax_ )  hInvMass_->Fill(p.mag());


    }// end loop over 2nd photon
  }
  */

  // USO I BASIC CLUSTERS invece DEI FOTONI
  // loop over basic clusters
  Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters;
  //evt.getByLabel(islandBCProd_, islandBCColl_, pIslandBarrelBasicClusters);
  evt.getByLabel("islandBasicClusters", "islandBarrelBasicClusters",pIslandBarrelBasicClusters);
  const reco::BasicClusterCollection* islandBarrelBasicClusters = pIslandBarrelBasicClusters.product();


  //Get association maps linking BasicClusters to ClusterShape
  edm::Handle<reco::BasicClusterShapeAssociationCollection> barrelClShpHandle;
  evt.getByLabel("islandBasicClusters", "islandBarrelShapeAssoc", barrelClShpHandle);
  reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;

  std::vector<reco::BasicCluster> localClusters;
  std::vector<double> S4oS9;

  int iClus = 0;

  for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters->begin();
      aClus != islandBarrelBasicClusters->end(); aClus++) {
 
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

    }
  }
  
}


float EtaAnalyzer::etaTransformation(  float EtaParticle , float Zvertex)  {


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
EtaAnalyzer::endJob() {
//========================================================================



}

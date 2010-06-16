/**\class EtaAnalyzerRealData
 **
 ** $Date: 2009/12/07 12:52:42 $ 
 ** $Revision: 1.1 $
 ** \author Andrea Massironi
 */

#include "EcalValidation/EtaValidation/plugins/EtaAnalyzerRealData.h"
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

#include "DataFormats/EcalDetId/interface/EBDetId.h"



#include "RecoEcal/EgammaCoreTools/interface/EcalRecHitLess.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

//
#include "Math/GenVector/LorentzVector.h"
#include "CLHEP/Units/PhysicalConstants.h"



#define PI 3.141592654
#define TWOPI 6.283185308


//========================================================================
EtaAnalyzerRealData::EtaAnalyzerRealData( const edm::ParameterSet& iConfig )
//========================================================================
{
 isMC_ = iConfig.getUntrackedParameter<bool> ("isMC","False");
 if  (isMC_){
  HepMCProducer_ = iConfig.getParameter< std::string > ("HepMCProducer");
 }
 
 debug_ = iConfig.getUntrackedParameter<bool> ("debug","False");
 
 ecalHitsProducer_ = iConfig.getParameter< std::string > ("ecalRecHitsProducer");
 barrelHits_ = iConfig.getParameter< std::string > ("barrelHitCollection");
 dummy_ = iConfig.getParameter< edm::InputTag > ("dummy");
 
 
 
 seleXtalMinEnergy_ = iConfig.getParameter< double > ("seleXtalMinEnergy");
 clusSeedThr_ = iConfig.getParameter< double > ("clusSeedThr");
  
 
 ParameterLogWeighted_ = iConfig.getParameter<bool> ("ParameterLogWeighted");
 ParameterX0_ = iConfig.getParameter<double> ("ParameterX0");
 ParameterT0_barl_ = iConfig.getParameter<double> ("ParameterT0_barl");
 ParameterT0_endc_ = iConfig.getParameter<double> ("ParameterT0_endc");
 ParameterT0_endcPresh_ = iConfig.getParameter<double> ("ParameterT0_endcPresh");
 ParameterW0_ = iConfig.getParameter<double> ("ParameterW0");
 ParameterE0_ = iConfig.getUntrackedParameter<double> ("ParameterE0",0.);
 
 providedParameters.insert(std::make_pair("LogWeighted",ParameterLogWeighted_));
 providedParameters.insert(std::make_pair("X0",ParameterX0_));
 providedParameters.insert(std::make_pair("T0_barl",ParameterT0_barl_));
 providedParameters.insert(std::make_pair("T0_endc",ParameterT0_endc_));
 providedParameters.insert(std::make_pair("T0_endcPresh",ParameterT0_endcPresh_));
 providedParameters.insert(std::make_pair("W0",ParameterW0_));
 providedParameters.insert(std::make_pair("E0",ParameterE0_));

 posCalculator_ = PositionCalc(providedParameters);
 
}


//========================================================================
EtaAnalyzerRealData::~EtaAnalyzerRealData()
//========================================================================
{


}

//========================================================================
void
  EtaAnalyzerRealData::beginJob(edm::EventSetup const&) {
//========================================================================

  //---- output ----
 edm::Service<TFileService> fs;

 hEnergy_ = fs->make<TH1F>("hEnergy"," Crystals energy",1000,0, 100);

 mytree_  = fs->make <TTree>("PhotonsTree","PhotonsTree"); 
 
 E1_ = new std::vector<float> ;
 E9_ = new std::vector<float> ;
 E25_ = new std::vector<float> ;
 E4o9_ = new std::vector<float> ;
 

 mytree_->Branch("E1","std::vector<float>",&E1_);
 mytree_->Branch("E9","std::vector<float>",&E9_);
 mytree_->Branch("E25","std::vector<float>",&E25_);
 mytree_->Branch("E4o9","std::vector<float>",&E4o9_);
 photons_ = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("photons", "TClonesArray", &photons_, 256000,0);
 
 mc_photons_ = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("mc_photons", "TClonesArray", &mc_photons_, 256000,0);
 mc_photons_3pi0_ = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("mc_photons_3pi0", "TClonesArray", &mc_photons_3pi0_, 256000,0);
 mc_eta_ = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("mc_eta", "TClonesArray", &mc_eta_, 256000,0);
 
 
  }


//========================================================================
  void
    EtaAnalyzerRealData::analyze( const edm::Event& event, const edm::EventSetup& iSetup ) {
//========================================================================

   //---- initialize for tree ----
   E1_ -> clear () ;
   E9_ -> clear () ;
   E25_ -> clear () ;
   E4o9_ -> clear () ;
   photons_ -> Clear ();
   mc_photons_ -> Clear ();
   mc_photons_3pi0_ -> Clear ();
   mc_eta_ -> Clear ();
   
//    using namespace edm; // needed for all fwk related classes
   edm::LogInfo("EtaAnalyzerRealData") << "Analyzing event number: " << event.id() << "\n";


  // get the ecal geometry:
   edm::ESHandle<CaloGeometry> geoHandle;
   iSetup.get<CaloGeometryRecord>().get(geoHandle); 
   const CaloSubdetectorGeometry *geometry_eb = geoHandle->getSubdetectorGeometry(DetId::Ecal,EcalBarrel);
   const CaloSubdetectorGeometry *geometry_ee = geoHandle->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);
   const CaloSubdetectorGeometry *geometry_es = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  
   edm::ESHandle<CaloTopology> pTopology;
   iSetup.get<CaloTopologyRecord>().get(pTopology);
   const CaloSubdetectorTopology *topology_eb = pTopology->getSubdetectorTopology(DetId::Ecal,EcalBarrel);
   const CaloSubdetectorTopology *topology_ee = pTopology->getSubdetectorTopology(DetId::Ecal,EcalEndcap);

   
   int nRecHitsEB=0;
   edm::Handle<EcalRecHitCollection> pEcalRecHitBarrelCollection;
   event.getByLabel(ecalHitsProducer_, barrelHits_, pEcalRecHitBarrelCollection);
   
//    event.getByLabel(dummy_, pEcalRecHitBarrelCollection);
   
   
   const EcalRecHitCollection *ecalRecHitBarrelCollection = pEcalRecHitBarrelCollection.product();
   std::cout << " ECAL Barrel RecHits # "<< ecalRecHitBarrelCollection->size() <<std::endl;
   
   
   
//    for(EcalRecHitCollection::const_iterator aRecHitEB = ecalRecHitBarrelCollection->begin(); aRecHitEB != ecalRecHitBarrelCollection->end(); aRecHitEB++) {
   //     
//     std::cout << " ECAL Barrel RecHit #,E,time,det,subdetid: "<<nRecHitsEB<<" "<<aRecHitEB->energy()<<" "<<aRecHitEB->time()<<" "<<aRecHitEB->detid().det()<<" "<<aRecHitEB->detid().subdetId()<<std::endl;
   // 
//     EBDetId ebrhdetid = aRecHitEB->detid();
//     std::cout << " EBDETID: z,ieta,iphi "<<ebrhdetid.zside()<<" "<<ebrhdetid.ieta()<<" "<<ebrhdetid.iphi()<<std::endl;
//     std::cout << " EBDETID: tower_ieta,tower_iphi "<<ebrhdetid.tower_ieta()<<" "<<ebrhdetid.tower_iphi()<<std::endl;
//     std::cout << " EBDETID: iSM, ic "<<ebrhdetid.ism()<<" "<<ebrhdetid.ic()<<std::endl;
//     std::cout << " EBDETID: energy "<<aRecHitEB->energy()<<std::endl;
   // 
//     nRecHitsEB++;
   // 
//    }

  
  
   ///---- make seeds ----
   
   std::vector<EcalRecHit> seeds;
   seeds.clear();

   std::vector<EcalRecHit> xtals;
   xtals.clear();
   std::vector<DetId> xtalsDetId;
   xtalsDetId.clear();

 
   std::vector<DetId> xtals_used;
   
   
   for(EcalRecHitCollection::const_iterator aRecHitEB = ecalRecHitBarrelCollection->begin(); aRecHitEB != ecalRecHitBarrelCollection->end(); aRecHitEB++){
    double energy = aRecHitEB->energy();
    if( energy >= seleXtalMinEnergy_) {
     xtals.push_back(*aRecHitEB);
     DetId det = aRecHitEB->id();
     xtalsDetId.push_back(det);
     if (energy > clusSeedThr_) {
      seeds.push_back(*aRecHitEB);
      xtals_used.push_back(det); //---- a seed must NOT be merged ----
//       std::cout << " Found a seed!" << " energy = " << energy << std::endl;
     }
    }
   }
   
   
   ///---- order seeds ----
   sort(seeds.begin(), seeds.end(), EcalRecHitLess());
   
  
   TClonesArray &photons = *photons_;
   int counter = 0;
   
   for(std::vector<EcalRecHit>::const_iterator itSeed = seeds.begin(); itSeed != seeds.end(); itSeed++){
    E1_->push_back(itSeed->energy());
    
    ///---- look around the seed ----
    EBDetId seed_id = itSeed->id();
    int seed_ieta = seed_id.ieta();
    int seed_iphi = seed_id.iphi();
    float s4s9_tmp[4];
    for(int i=0;i<4;i++)s4s9_tmp[i]= 0;


    //     std::vector<DetId> clus_v = topology_eb->getWindow(seed_id,clusEtaSize_,clusPhiSize_);  
    std::vector<DetId> clus_v = topology_eb->getWindow(seed_id,5,5);  
    std::vector<std::pair<DetId, float> > clus_used;
    
    int num_xtals = 0;
    
    double e3x3 = 0;
    double e5x5 = 0;
    for (std::vector<DetId>::iterator det=clus_v.begin(); det!=clus_v.end(); det++) {
     EBDetId EBdet = *det;
     std::vector<DetId>::iterator itdet = find( xtalsDetId.begin(),xtalsDetId.end(),EBdet);
     if(itdet != xtalsDetId.end()){
      std::vector<DetId>::iterator itdet_already_used = find( xtals_used.begin(),xtals_used.end(),EBdet);
      if (itdet_already_used != xtals_used.end() || (xtals_used.empty())){
       xtals_used.push_back(EBdet);
       num_xtals++;
       clus_used.push_back(std::pair<DetId, float>(*det, 1) );
       int nn = int(itdet - xtalsDetId.begin());
       double energy = xtals[nn].energy();
       int ieta = ((EBDetId) *itdet).ieta();
       int iphi = ((EBDetId) *itdet).iphi();
      
       e5x5 += energy;
       int dx = diff_neta_s(seed_ieta,ieta);
       int dy = diff_nphi_s(seed_iphi,iphi);

       if(abs(dx)<=1 && abs(dy)<=1) {
        e3x3 += energy; 
        if(dx <= 0 && dy <=0) s4s9_tmp[0] += energy; 
        if(dx >= 0 && dy <=0) s4s9_tmp[1] += energy; 
        if(dx <= 0 && dy >=0) s4s9_tmp[2] += energy; 
        if(dx >= 0 && dy >=0) s4s9_tmp[3] += energy; 
       }
      } 
     }
    }
    
//     std::cout << "num_xtals = " << num_xtals << std::endl;
    
    float s4s9_max = s4s9_tmp[0];
    for (int i=1; i<4; i++) if (s4s9_tmp[i]>s4s9_max) s4s9_max = s4s9_tmp[i] ; 
    E9_->push_back(e3x3);
    E25_->push_back(e5x5);
    E4o9_->push_back(s4s9_max/e3x3);
    
    math::XYZPoint photon_position = posCalculator_.Calculate_Location(clus_used,ecalRecHitBarrelCollection,geometry_eb,geometry_es);
    TLorentzVector photon_position_v;
    photon_position_v.SetPx (e3x3 * sin(photon_position.Theta()) * cos(photon_position.Phi()));
    photon_position_v.SetPy (e3x3 * sin(photon_position.Theta()) * sin(photon_position.Phi()));
    photon_position_v.SetPz (e3x3 * cos(photon_position.Theta()));
    photon_position_v.SetE (e3x3);
    new (photons[counter]) TLorentzVector (photon_position_v);
    counter++;
   }
   
   
   
   
   
   
   
   
   
   
   ///---- MC analysis ----
   if (isMC_){
    TClonesArray &mc_eta = *mc_eta_;
    TClonesArray &mc_photons = *mc_photons_;
    TClonesArray &mc_photons_3pi0 = *mc_photons_3pi0_;
    int mc_counter_eta = 0;
    int mc_counter_photons = 0;
    int mc_counter_photons_3pi0 = 0;
    
    edm::Handle< edm::HepMCProduct > hepProd ;
    event.getByLabel(HepMCProducer_,  hepProd ) ;
    const HepMC::GenEvent * myGenEvent = hepProd->GetEvent();

  ///---- Eta selection ----

    int numEta_ = 0;
    for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {
     if ( (*p)->pdg_id() == 221 && (*p)->status() == 2 ) {
      if (debug_) std::cout << "found an eta" << std::endl;
            
      int num = 0;
      for (HepMC::GenVertex::particle_iterator d = (*p)->end_vertex()->particles_begin(HepMC::children); d != (*p)->end_vertex()->particles_end(HepMC::children); ++d){
       if (debug_) std::cout << " part = " << (*d)->pdg_id() << " status = " << (*d)->status() << std::endl;
       if ((abs((*d)->pdg_id()) == 22) && (*d)->status()==1)  { //---- eta->gamma gamma
        TLorentzVector mc_photon_position_v;
        mc_photon_position_v.SetPx ((*d)->momentum().x());
        mc_photon_position_v.SetPy ((*d)->momentum().y());
        mc_photon_position_v.SetPz ((*d)->momentum().z());
        mc_photon_position_v.SetE ((*d)->momentum().mag());
        new (mc_photons[mc_counter_photons]) TLorentzVector (mc_photon_position_v);
        mc_counter_photons++;
        num++;
        if (debug_) std::cerr << "    mc_counter_photons = " << mc_counter_photons << std::endl;
       }
       else {
        if ((abs((*d)->pdg_id()) == 111) && ((*d)->status() == 2)){ //--- a pi0
         for (HepMC::GenVertex::particle_iterator d2 = (*d)->end_vertex()->particles_begin(HepMC::children); d2 != (*d)->end_vertex()->particles_end(HepMC::children); ++d2){
          if (debug_) std::cout << " ---> part = " << (*d2)->pdg_id() << " status = " << (*d2)->status() << std::endl;
          if ((abs((*d2)->pdg_id()) == 22) && (*d2)->status()==1)  {
           TLorentzVector mc_photon_position_v;
           mc_photon_position_v.SetPx ((*d2)->momentum().x());
           mc_photon_position_v.SetPy ((*d2)->momentum().y());
           mc_photon_position_v.SetPz ((*d2)->momentum().z());
           mc_photon_position_v.SetE ((*d2)->momentum().mag());
           new (mc_photons_3pi0[mc_counter_photons_3pi0]) TLorentzVector (mc_photon_position_v);
           mc_counter_photons_3pi0++;
           num++;          
          }
         }
        }//---- end a pi0
       }
      }
      if (num==2 || num==6){ ///---- only 2 photons decay of eta or 3 pi0 (6 photons) decay ----
       if (debug_) if (num==2) std::cout << "   -> gamma gamma" << std::endl;
       if (debug_) if (num==6) std::cout << "   -> pi0 pi0 pi0 -> 6 x gamma" << std::endl;
       TLorentzVector mc_eta_position_v;
       mc_eta_position_v.SetPx ((*p)->momentum().x());
       mc_eta_position_v.SetPy ((*p)->momentum().y());
       mc_eta_position_v.SetPz ((*p)->momentum().z());
       mc_eta_position_v.SetE ((*p)->momentum().mag());
       new (mc_eta[mc_counter_eta]) TLorentzVector (mc_eta_position_v);
       mc_counter_eta++;
      }
     }
    }
   
   }
   
   mytree_->Fill();
 
   
    }



//========================================================================
    void
      EtaAnalyzerRealData::endJob() 
    {
//========================================================================

     delete E1_;
     delete E9_;
     delete E25_;
     delete E4o9_;
     delete photons_;
     delete mc_photons_;
     delete mc_photons_3pi0_;
     delete mc_eta_;
    }

      
      
//========================================================================

    int 
      EtaAnalyzerRealData::diff_neta_s(int neta1, int neta2)
    {
     int mdiff;
     mdiff=(neta1-neta2);
     return mdiff;
    }

//========================================================================

// Calculate the distance in xtals taking into account the periodicity of the Barrel
    int 
      EtaAnalyzerRealData::diff_nphi_s(int nphi1,int nphi2) 
    {
     int mdiff;
     if(abs(nphi1-nphi2) < (360-abs(nphi1-nphi2))) {
      mdiff=nphi1-nphi2;
     }
     else {
      mdiff=360-abs(nphi1-nphi2);
      if(nphi1>nphi2) mdiff=-mdiff;
     }
     return mdiff;
    }
//========================================================================




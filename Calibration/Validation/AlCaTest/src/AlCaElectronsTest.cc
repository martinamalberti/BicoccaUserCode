// system include files
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "Validation/AlCaTest/interface/AlCaElectronsTest.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsProducer.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

//DS momentum
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/EcalRecHitRecalib.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include <math.h>
#include "TFile.h"
#include <Math/VectorUtil.h>


using namespace reco ;
using namespace edm ;


// ----------------------------------------------------------------


AlCaElectronsTest::AlCaElectronsTest (const edm::ParameterSet& iConfig) :
  m_ElectronLabel (iConfig.getParameter<edm::InputTag> ("electronLabel")),
  m_barrelAlCa (iConfig.getParameter<edm::InputTag> ("alcaBarrelHitCollection")) ,
  m_endcapAlCa (iConfig.getParameter<edm::InputTag> ("alcaEndcapHitCollection")), 
  m_outputFileName (iConfig.getUntrackedParameter<std::string>
                      ("HistOutFile",std::string ("AlCaElectronsTest.root"))) 
{}


// ----------------------------------------------------------------


void 
AlCaElectronsTest::beginJob ()
{
  //Maps of th electrons hits
  m_barrelGlobalCrystalsMap = new TH2F ("m_barrelGlobalCrystalsMap","m_barrelGlobalCrystalsMap",170,-85,85,360,0,360) ;
  m_barrelGlobalCrystalsMap ->SetDirectory(0);
  m_endcapGlobalCrystalsMap = new TH2F ("m_endcapGlobalCrystalsMap","m_endcapGlobalCrystalsMap",100,0,100,100,0,100) ;
  m_endcapGlobalCrystalsMap ->SetDirectory(0);

  //Energy around the MOX 
  m_barrelLocalCrystalsEnergy = new TH2F ("m_barrelLocalCrystalsEnergy","m_barrelLocalCrystalsEnergy",20,-10,10,20,-10,10) ;
  m_barrelLocalCrystalsEnergy ->SetDirectory(0);
  m_endcapLocalCrystalsEnergy = new TH2F ("m_endcapLocalCrystalsEnergy","m_endcapLocalCrystalsEnergy",20,-10,10,20,-10,10) ;
  m_endcapLocalCrystalsEnergy ->SetDirectory(0);
 Zee = new TH1F ("Zee","Zee",100,0,200);
 Zee->SetDirectory(0);
  //The tree to save the datas
  m_tree = new TTree ("m_tree","m_tree");
  m_tree->SetDirectory(0);
  m_tree->Branch("eta",&m_eta,"eta/F");
  m_tree->Branch("phi",&m_phi,"phi/F");
  m_tree->Branch("pTk",&m_pTk,"pTk/F");
  m_tree->Branch("MaxEnergy",&m_MaxEnergy,"MaxEnergy/F");
  m_tree->Branch("energy",&m_energy,"energy/F");
  m_tree->Branch("Energy25",&m_Energy25,"Energy25/F");
  m_tree->Branch("ESCoP",&m_ESCoP,"ESCoP/F");
  m_tree->Branch("eSeedOverPout",&m_eSeedOverPout,"eSeedOverPout/F");
  m_tree->Branch("pOut",&m_pOut,"pOut/F");
  m_tree->Branch("Energy9",&m_Energy9,"Energy9/F");
  m_tree->Branch("Energy49",&m_Energy49,"Energy49/F");
  m_tree->Branch("Presh",&m_preshower,"Presh/F");
  m_tree->Branch("pErr",&m_pErr,"pErr/F");
  m_tree->Branch("recHits",&m_recHits,"recHits/F");
  m_tree->Branch("class",&m_class,"class/I");
  m_tree->Branch("chis",&m_chis,"chis/D");
  m_tree->Branch("lost",&m_lost,"lost/I");
  m_tree->Branch("found",&m_found,"found/I");
  m_tree->Branch("ndof",&m_ndof,"ndof/D");
  m_tree->Branch("momentum",&m_momentum,"m_momentum/D");
  m_tree->Branch("chisN",&m_chisN,"chisN/D");

 return ;
}


// ----------------------------------------------------------------


void 
AlCaElectronsTest::endJob ()
{      
   m_File = new TFile (m_outputFileName.c_str (),"recreate") ;
  edm::LogWarning ("loop") << "PIETRO endjob" ;
  m_barrelGlobalCrystalsMap->Write(); 
  m_endcapGlobalCrystalsMap->Write();  
  m_barrelLocalCrystalsEnergy->Write();
  m_endcapLocalCrystalsEnergy->Write();
  m_tree->Write();
  Zee->Write();
  m_File->Close();
  return ;
}


// ----------------------------------------------------------------


void 
AlCaElectronsTest::analyze (const edm::Event& iEvent, 
                            const edm::EventSetup& iSetup)
{
  double energia5 = 0 ;
  m_pTk = 0. ;
  math::XYZVector Zmoment(0.,0.,0.);
  double  Zenergy=0;
  //get the EB rechit collection
  Handle<EBRecHitCollection> barrelRecHitsHandle ;
  const EBRecHitCollection*  barrelHitsCollection = 0 ;
  iEvent.getByLabel (m_barrelAlCa, barrelRecHitsHandle) ;
  barrelHitsCollection = barrelRecHitsHandle.product () ; // get a ptr to the product

  Handle<EERecHitCollection> endcapRecHitsHandle ;
  const EERecHitCollection*  endcapHitsCollection = 0 ; 
  iEvent.getByLabel (m_endcapAlCa,endcapRecHitsHandle) ;
  endcapHitsCollection = endcapRecHitsHandle.product () ; // get a ptr to the product
//  try {
//      iEvent.getByLabel (m_barrelAlCa, barrelRecHitsHandle) ;
//      barrelHitsCollection = barrelRecHitsHandle.product () ; // get a ptr to the product
//      iEvent.getByLabel (m_endcapAlCa,endcapRecHitsHandle) ;
//      endcapHitsCollection = endcapRecHitsHandle.product () ; // get a ptr to the product
//    }
//  catch ( std::exception& ex ) {
//       LogDebug ("") << "EcalREcHitMiscalib: Error! can't get product! caught"<<ex.what () << std::endl ;
//    }
  
  edm::Handle<reco::GsfElectronCollection> pElectrons ;
  iEvent.getByLabel (m_ElectronLabel, pElectrons) ;
//  try {
//       iEvent.getByLabel (m_ElectronLabel, pElectrons) ;
//      }
//  catch (...) {
//       edm::LogError ("reading") << m_ElectronLabel << "NotFound" ;
//       return ;
//     }
  
  //PG loop on the electrons
  int i=0;
  bool EE=0;
  for (reco::GsfElectronCollection::const_iterator eleIt = pElectrons->begin () ;

       eleIt != pElectrons->end () ;
       ++eleIt) 
    {
//    std::cerr<<"\n\n\n inizio un elettrone\n";
     m_eta=eleIt->eta () ;
     m_recHits=0;
     m_pErr=eleIt->trackMomentumError();
     m_chis=eleIt->gsfTrack()->chi2();
     m_lost=eleIt->gsfTrack()->lost();
     m_found=eleIt->gsfTrack()->found();
     m_ndof=eleIt->gsfTrack()->ndof();
     m_chisN=eleIt->gsfTrack()->normalizedChi2();
     m_class=eleIt->classification();
     m_preshower=eleIt->superCluster()->preshowerEnergy();
     m_phi=eleIt->phi () ;
     m_pTk = eleIt->trackMomentumAtVtx ().R () ; 
     m_ESCoP= eleIt->eSuperClusterOverP();
     m_energy=eleIt->caloEnergy();
     m_eSeedOverPout=eleIt->eSeedClusterOverPout();
     m_pOut = eleIt->trackMomentumOut().R();	
     Zmoment+= eleIt->trackMomentumAtVtx();
     m_momentum = eleIt->p();
//     std::cerr<<"sto per farmi dare le hits\n";
     const std::vector<std::pair<DetId,float> > & hits= eleIt->superCluster()->hitsAndFractions();
//     std::cerr<<"me le ha date\n";
     for (std::vector<std::pair<DetId,float> > ::const_iterator rh = hits.begin();
            rh!=hits.end(); ++rh){
//	     std::cerr<<"una hit e' passata\n";
//	     std::cerr<<"hitsAndFraction vector size= "<<eleIt->superCluster()->hitsAndFractions().size()<<"\n";
             if ((*rh).first.subdetId()== EcalBarrel){

         	  EBRecHitCollection::const_iterator itrechit = barrelHitsCollection->find((*rh).first);
		  if (itrechit==barrelHitsCollection->end()) continue;
	          m_recHits+=itrechit->energy();
	     }
	     
             if ((*rh).first.subdetId()== EcalEndcap){
         	  EERecHitCollection::const_iterator itrechit = endcapHitsCollection->find((*rh).first);
		  if (itrechit==endcapHitsCollection->end()) continue;
	          m_recHits+=itrechit->energy();
	     }
	     else
	     { 
//		     std::cerr<<"something is wrong about where the hit is\n";
//	             std::cerr<<"subDetID= "<<(*rh).first.subdetId()<<"\n";
	     }
//	     std::cerr<<"finito il primo loop\n";

	}
//     std::cerr<<"ho fatto il loop\n";
     if (m_energy>40) ++i;
     //PG look for the max detid in the cluster relative to the electron
     DetId Max = 0;
     if ((fabs(eleIt->eta())<1.49)){
	     DetId temp;
	     temp = EcalClusterTools::getMaximum(eleIt->superCluster()->hitsAndFractions(),barrelHitsCollection).first;
	     Max=temp;
	     }
     else 
     	{
//	     std::cerr<<"else\n";
	     DetId temp;
	     temp = EcalClusterTools::getMaximum(eleIt->superCluster()->hitsAndFractions(),endcapHitsCollection).first;
//	     std::cerr<<"hitsAndFraction vector size= "<<eleIt->superCluster()->hitsAndFractions().size()<<"\n";
	     Max=temp;
	}
     if (Max.det () == 0){ continue;}  
     if ( Max.subdetId () == EcalBarrel  ) //PG in the barrel
       {
         EBDetId EBMax (Max) ;    
         EBRecHitCollection::const_iterator itrechit ;
         energia5 = Energy25Barrel (EBMax.ieta (), EBMax.iphi (), 5,barrelHitsCollection) ;
         itrechit = barrelHitsCollection->find (Max) ;
         m_MaxEnergy=itrechit->energy () ;
//	 std::cerr<<"qua ci sei?"<<"\n";
         m_barrelGlobalCrystalsMap->Fill (
                EBMax.ieta () ,
                EBMax.iphi () 
              ) ;
//	 std::cerr<<"e qua?\n";
         m_Energy25=energia5 ;
	 m_Energy49= Energy25Barrel (EBMax.ieta(),EBMax.iphi(),7,barrelHitsCollection);
	 m_Energy9=Energy25Barrel (EBMax.ieta(),EBMax.iphi(),3,barrelHitsCollection);
	 Zenergy+=energia5;
         fillAroundBarrel (
             barrelHitsCollection, 
             EBMax.ieta (), 
             EBMax.iphi (),
             m_pTk 
            ) ;
       } //PG in the barrel
     
     else //PG in the endcap
       {      
	 EE=1;
         EEDetId EEMax (Max) ;  
         EERecHitCollection::const_iterator itrechit ;
         itrechit = endcapHitsCollection->find (Max) ;
         m_MaxEnergy=itrechit->energy () ;
         energia5 = Energy25Endcap (EEMax.ix (), EEMax.iy (), EEMax.zside(),5, endcapHitsCollection) ;
//	 std::cerr<<"qua ci sono\n";
         m_endcapGlobalCrystalsMap->Fill (
                EEMax.ix () ,
                EEMax.iy () 
              ) ;
//	 std::cerr<<"e qua?\n";
         m_Energy25 = energia5 ;
         m_Energy49 = Energy25Endcap (EEMax.ix (), EEMax.iy (), EEMax.zside(),7, endcapHitsCollection) ;
         m_Energy9 = Energy25Endcap (EEMax.ix (), EEMax.iy (), EEMax.zside(),3, endcapHitsCollection) ;
	 Zenergy+=energia5+m_preshower;
         fillAroundEndcap (
            endcapHitsCollection, 
            EEMax.ix (), 
            EEMax.iy (),
            m_pTk
            ) ;
//	 std::cerr<<"e in fondo\n";
       } //PG in the endcap
//   std::cerr<<"prima del fill\n";
   m_tree->Fill();
//   std::cerr<<"fill del tree?\n";

   } //PG loop over electrons
   double MZ;
   MZ = sqrt( Zenergy*Zenergy - Zmoment.R()*Zmoment.R());
   if (i==2)
	   Zee->Fill(MZ);

}


// ----------------------------------------------------------------


void
AlCaElectronsTest::fillAroundBarrel (const EcalRecHitCollection * recHits, int eta, int phi, double momentum)
{

  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      EBDetId elementId = elem->id () ; 
      m_barrelLocalCrystalsEnergy->Fill (
        elementId.ieta () - eta ,
        elementId.iphi () - phi ,
        elem->energy ()
      ) ;
    }   
  return ;
}


// ----------------------------------------------------------------


void
AlCaElectronsTest::fillAroundEndcap (const EcalRecHitCollection * recHits, int ics, int ips, double momentum)
{
  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      EEDetId elementId = elem->id () ; 
      m_endcapLocalCrystalsEnergy->Fill (
        elementId.ix () - ics ,
        elementId.iy () - ips ,
        elem->energy ()
      ) ;
    }   
  return ;
}


// ----------------------------------------------------------------


//! MF energy in the 5x5 neighbourhood around MaxId
//! in the barrel
double AlCaElectronsTest::Energy25Barrel (int eta, int phi,int side,
                const EcalRecHitCollection * barrelHitsCollection)
{
  int curr_eta = 0 ;
  int curr_phi = 0 ;
  double dummy = 0. ;
 //PG questo puo' essere in futuro specificato separatamente per eta,phi
     //PG loop on the energy reconstruction window
  for (int ii = 0 ; ii < side ; ++ii)
  for (int ij = 0 ; ij < side ; ++ij)
    {
      curr_eta = eta - (side/2) + ii ;
      curr_phi = phi - (side/2) + ij ;
      if (abs (curr_eta) > 85) continue ;
      if (curr_eta * eta <= 0) 
        {
          if (eta > 0) --curr_eta ; 
          else curr_eta++ ; 
        }  // JUMP over 0
      if (curr_phi < 1) curr_phi += 360 ;
      if (curr_phi > 360) curr_phi -= 360 ;
      if (EBDetId::validDetId (curr_eta,curr_phi))
       {
         EBDetId det = EBDetId (curr_eta,curr_phi,EBDetId::ETAPHIMODE) ;
         EcalRecHitCollection::const_iterator curr_recHit = barrelHitsCollection->find (det) ;
         if (isnan(curr_recHit->energy())) continue;
         if (curr_recHit->energy()<0) continue;
         if (curr_recHit->energy()>1000) continue;
         dummy  = dummy + curr_recHit->energy () ;
        }
    }
 return dummy ;
}


// ----------------------------------------------------------------


//! MF energy in the 5x5 neighbourhood around MaxId
//! in the endcap   
double AlCaElectronsTest::Energy25Endcap (int ics, int ips, int z,int side,
                  const EcalRecHitCollection * endcapHitsCollection)
{
  int curr_x = 0 ;
  int curr_y = 0 ;
  int local_x = 0 ;
  int local_y = 0 ;
  double dummy = 0. ;
  //PG loop on the energy reconstruction window
  for (int ii = 0 ; ii < side ; ++ii)
  for (int ij = 0 ; ij < side ; ++ij)
   {
    curr_x = ics - (side/2) + ii ;
    curr_y = ips - (side/2) + ij ;
    local_x = curr_x - 1 ;
    local_y = curr_y - 1 ;
    if (curr_x>100 || curr_x<0) continue ; //PG prob qs ctrl nn serve visto qllo dopo
    if (curr_y>100 || curr_y<0) continue ; //PG prob qs ctrl nn serve visto qllo dopo
    if (EEDetId::validDetId (curr_x,curr_y,z))
     {
      EEDetId det = EEDetId (curr_x,curr_y,z,EEDetId::XYMODE) ;
      EcalRecHitCollection::const_iterator curr_recHit = endcapHitsCollection->find (det) ;
      if (isnan(curr_recHit->energy())) continue;
      if (curr_recHit->energy()<0) continue;
      if (curr_recHit->energy()>1000) continue;
      dummy  = dummy + curr_recHit->energy () ;
     } 
   } 
  return dummy ;
} 


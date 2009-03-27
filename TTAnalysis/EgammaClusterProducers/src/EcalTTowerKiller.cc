// -*- C++ -*-
//
// Package:    EcalTTowerKiller
// Class:      EcalTTowerKiller
// 
/**\class EcalTTowerKiller EcalTTowerKiller.cc RecoEcal/src/EcalTTowerKiller.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Georgios Daskalakis
//         Created:  Tue Apr 24 17:21:31 CEST 2007
// $Id: EcalChannelKiller.cc,v 1.2 2007/05/09 14:21:16 beaucero Exp $
//
//




// Geometry
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelHardcodedTopology.h"

// Reconstruction Classes
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"


//TP
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include <TH1F.h>


#include "TTAnalysis/EgammaClusterProducers/interface/EcalTTowerKiller.h"




#include <fstream>

#include<string>
using namespace cms;
using namespace std;


//
// constructors and destructor
//
EcalTTowerKiller::EcalTTowerKiller(const edm::ParameterSet& ps)
{

  ECollection_ = ps.getParameter<std::vector<edm::InputTag> >("ECollection");

  reducedHitCollectionEB_ = ps.getParameter<std::string>("reducedHitCollectionEB");
  rejectedHitCollectionEB_ = ps.getParameter<std::string>("rejectedHitCollectionEB");
  reducedHitCollectionEE_ = ps.getParameter<std::string>("reducedHitCollectionEE");
  rejectedHitCollectionEE_ = ps.getParameter<std::string>("rejectedHitCollectionEE");
  recoveredHitCollectionEB_ = ps.getParameter<std::string>("recoveredHitCollectionEB");
  recoveredHitCollectionEE_ = ps.getParameter<std::string>("recoveredHitCollectionEE");

  DeadChannelFileName_  = ps.getParameter<std::string>("DeadChannelsFile");
  
  produces< EcalRecHitCollection >(reducedHitCollectionEB_);
  produces< EcalRecHitCollection >(rejectedHitCollectionEB_);
  produces< EcalRecHitCollection >(reducedHitCollectionEE_);
  produces< EcalRecHitCollection >(rejectedHitCollectionEE_);

  produces< EcalRecHitCollection >(recoveredHitCollectionEB_);
  produces< EcalRecHitCollection >(recoveredHitCollectionEE_);

  histofile_ =  ps.getParameter<std::string>("histofile");

}


EcalTTowerKiller::~EcalTTowerKiller()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EcalTTowerKiller::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   // create an auto_ptr to a EcalRecHitCollection, copy the RecHits into it and put in the Event:
   std::auto_ptr< EcalRecHitCollection > redCollectionEB(new EcalRecHitCollection);
   std::auto_ptr< EcalRecHitCollection > rejCollectionEB(new EcalRecHitCollection);
   std::auto_ptr< EcalRecHitCollection > redCollectionEE(new EcalRecHitCollection);
   std::auto_ptr< EcalRecHitCollection > rejCollectionEE(new EcalRecHitCollection);

   std::auto_ptr< EcalRecHitCollection > recoveredCollectionEB(new EcalRecHitCollection);
   std::auto_ptr< EcalRecHitCollection > recoveredCollectionEE(new EcalRecHitCollection);



   //Trigger Primitives
   // edm::ESHandle<CaloGeometry> cGeom; 
   //iSetup.get<IdealGeometryRecord>().get(cGeom);    

   /*  673         //CaloTowerDetId  towerDetId = CaloTowerDetId( eieta, teiphi); 
     674         //CaloTowerDetId  towerDetId2 = CaloTowerDetId( eieta, eiphi); 
     675         //const GlobalPoint gP1 = cGeom->getPosition(towerDetId);
     676         //const GlobalPoint gP12 = cGeom->getPosition(towerDetId2);
     677         //double eta = gP1.eta();  
   */
   ESHandle<CaloGeometry> theGeometry;
   ESHandle<CaloSubdetectorGeometry> theEndcapGeometry_handle, theBarrelGeometry_handle;
   iSetup.get<CaloGeometryRecord>().get( theGeometry );
   iSetup.get<EcalEndcapGeometryRecord>().get("EcalEndcap",theEndcapGeometry_handle);
   iSetup.get<EcalBarrelGeometryRecord>().get("EcalBarrel",theBarrelGeometry_handle);
   const CaloSubdetectorGeometry * theBarrelGeometry_ ;
   
   theBarrelGeometry_ = &(*theBarrelGeometry_handle);
   
   std::vector < std::pair< std::pair<int, int>, float > >TP_map_EB, TP_map_EE;
   std::vector < std::pair< std::pair<int, int>, float > >TT_map_EB, TT_map_EE;

   
   Handle<EcalTrigPrimDigiCollection> emulDigis;
   iEvent.getByLabel("simEcalTriggerPrimitiveDigis", emulDigis);
   for ( EcalTrigPrimDigiCollection::const_iterator tpdigiItr = emulDigis->begin();tpdigiItr != emulDigis->end(); ++tpdigiItr ) {
     EcalTriggerPrimitiveDigi data = (*tpdigiItr);
     EcalTrigTowerDetId idt = data.id();
     
     //const GlobalPoint gP1 = cGeom->getPosition(idt);

     if(idt.subDet()== EcalBarrel){
       EBDetId id( idt.ieta(), idt.iphi() ) ;
       //std::cout <<"ieta:" << idt.ieta() <<" iphi:" << idt.iphi() << " " <<  theBarrelGeometry_->getGeometry(id)->getPosition().eta() << " " << theBarrelGeometry_->getGeometry(id)->getPosition().phi() << std::endl;

       float theta =  theBarrelGeometry_->getGeometry(id)->getPosition().theta();

       int iFED = 600 + idt.iDCC();
       int iTT =   idt.iTT();
       
       if(tpdigiItr->compressedEt() > 0){
	 TP_map_EB.push_back( make_pair( make_pair( iFED, iTT ), tpdigiItr->compressedEt()/( sin(theta)*sin(theta) ) ) );
	 cout << "- Et: "<< tpdigiItr->compressedEt() << " E:" << tpdigiItr->compressedEt()/( sin(theta)*sin(theta) )  << " theta:" << theta <<  "  "  << idt.iTT() <<endl;
       }
       //if(  tpdigiItr->compressedEt()>0)
	 //cout << "- Et: "<< tpdigiItr->compressedEt() << " E:" << tpdigiItr->compressedEt()/( sin(theta)*sin(theta) )  << " theta:" << theta <<  "  "  << idt.iTT() <<endl;
     }
   }


   // get the hit collection from the event:
   std::vector<edm::InputTag>::const_iterator i;
   for (i=ECollection_.begin(); i!=ECollection_.end(); i++) {
     
     edm::Handle<EcalRecHitCollection> ec;
     iEvent.getByLabel(*i,ec);
     
     if (!( ec.isValid())) {
       std::cout << "could not get a handle on the EcalRecHitCollection! "<< *i << std::endl;
       return;
     }
     
     const EcalRecHitCollection* hit_collectionEB = ec.product();
     
     float totalE=0;
       for(EcalRecHitCollection::const_iterator it = hit_collectionEB->begin(); it != hit_collectionEB->end(); ++it) {
       double NewEnergy = it->energy();
       bool ItIsDead = false;
       bool isBarrel = false;

       ESHandle< EcalElectronicsMapping > ecalmapping;
       iSetup.get< EcalMappingRcd >().get(ecalmapping);
       const EcalElectronicsMapping * theMapping_ = ecalmapping.product();
       
       if( it->detid().subdetId() == 1) isBarrel = true;

       EcalElectronicsId elecId = theMapping_->getElectronicsId( it->id() );
       int iFED = 600 + elecId.dccId();
       int iTT =   elecId.towerId();

       std::pair<int,int> TTid = make_pair(iFED, iTT);
       
       if(isBarrel){
	 // iterator to vector element:
	 bool Found=false;
	 for(vector< pair<pair<int,int>, float> >::iterator it = TT_map_EB.begin(); it!= TT_map_EB.end(); it++)
	   if(it->first.first == iFED && it->first.second == iTT ) {it->second += NewEnergy; Found=true; break; }
	 
	 if(!Found) TT_map_EB.push_back( make_pair( make_pair(iFED, iTT), NewEnergy  ) );
	 

       }       
       for( std::vector<pair<int, int> >::const_iterator DeadCell = ChannelsDeadID.begin();
	    DeadCell < ChannelsDeadID.end(); DeadCell++ ){
	 
	 if(iFED == DeadCell->first && iTT == DeadCell->second){
	   DetId itID = it->id();
	   ItIsDead = true;
	   totalE += NewEnergy;
	   //cout << "KILLED! E:" << NewEnergy << " " << iFED << " " << iTT <<endl;
	   
	 }
       }//End looping on vector of Dead Cells
       
       if(!ItIsDead){
	 //cout << "RH energy:" << NewEnergy << endl;
	 EcalRecHit NewHit(it->id(), NewEnergy, it->time());
	 if(isBarrel) {redCollectionEB->push_back( NewHit ); recoveredCollectionEB->push_back( NewHit );}
	 else {redCollectionEE->push_back( NewHit ); recoveredCollectionEE->push_back( NewHit );}
       }
       
       else{
	 EcalRecHit NewHit(it->id(), NewEnergy, it->time());
	 if(isBarrel) rejCollectionEB->push_back( NewHit );
	 else rejCollectionEE->push_back( NewHit );
	 
	 if(isBarrel)
           for( std::vector < std::pair < pair<int,int>, float > >::const_iterator TP=TP_map_EB.begin(); TP!=TP_map_EB.end(); TP++ ){
             if(TP->first.first == iFED && TP->first.second==iTT){
	       
	       if( recoveredHitCollectionEB_ == "EcalDRRecHitsEB"){
		 cout << "E: " << NewEnergy << " DR_rE:"<< TP->second /25. << " " << TP->second << endl;
                 EcalRecHit NewHitR(it->id(), (float) TP->second /25. , it->time());
		 recoveredCollectionEB->push_back( NewHitR );
	       }
	     }
           }
	 
	 else
	   for( std::vector < std::pair< pair<int,int>, float > >::const_iterator TP=TP_map_EE.begin(); TP!=TP_map_EE.end(); TP++ ){
             if(TP->first.first == iFED && TP->first.second==iTT){
               if( recoveredHitCollectionEE_ == "EcalDRRecHitsEE"){
                 EcalRecHit NewHitR(it->id(), (float) TP->second /25. , it->time());
                 recoveredCollectionEE->push_back( NewHitR );
               }
             }
           }
	 
	 
       }
       
       
       }
       
       cout << "Total E in TT dead:"<< totalE << endl;
       
       for(vector< pair<pair<int,int>, float> >::iterator it = TT_map_EB.begin(); it!= TT_map_EB.end(); it++){
	 for(vector< pair<pair<int,int>, float> >::iterator it2 = TP_map_EB.begin(); it2!= TP_map_EB.end(); it2++){
	   if(it2->first.second == it->first.second && it2->first.first == it->first.first ){
	     cout << it->second << " " << it2->second << endl;
	     histos["TowerRes"]->Fill( (it2->second - it->second )/it->second );
	 }
	 }
       }
   }
   iEvent.put(redCollectionEB, reducedHitCollectionEB_);
   iEvent.put(rejCollectionEB, rejectedHitCollectionEB_);
   iEvent.put(redCollectionEE, reducedHitCollectionEE_);
   iEvent.put(rejCollectionEE, rejectedHitCollectionEE_);
   iEvent.put(recoveredCollectionEB, recoveredHitCollectionEB_);
   iEvent.put(recoveredCollectionEE, recoveredHitCollectionEE_);


//   std::cout << "============ quanti???? " << nRed << std::endl; 
}




// ------------ method called once each job just before starting event loop  ------------
void 
EcalTTowerKiller::beginJob(const edm::EventSetup&)
{

  nReds = 0;


  //Open the DeadChannel file, read it.
  edm::FileInPath path_deadFile(DeadChannelFileName_.c_str() );
  std::ifstream inFile ( path_deadFile.fullPath().c_str(), std::ios::in);
  
  int SMid = -10000;
  int TTid = -10000;


  while(!inFile.eof())
    {
      
      inFile >> SMid >> TTid;
      if( TTid == -10000  || SMid == -10000) 
	{std::cout << "Problem reading Dead Channels file "<< std::endl; break;}

      std::cout <<" +++++++++++ SM "<< SMid << "  e TT " << TTid << std::endl;
      
      pair<int, int> torre(SMid, TTid);
      ChannelsDeadID.push_back(torre);
    } //end while	    
  std::cout << " Read " << ChannelsDeadID.size() <<" dead channels " << std::endl;
  inFile.close();

  m_file=new TFile(histofile_.c_str(),"RECREATE");
  string hLabel="";
  hLabel = "TowerRes"; histos[hLabel] = new TH1F(hLabel.c_str(), hLabel.c_str(),1000,-10,10);


}



// ------------ method called once each job just after ending the event loop  ------------
void 
EcalTTowerKiller::endJob() {
  // histos["TowerRes"]->Write();
  m_file->Write();
  m_file->Close();

  //std::cout << "++++ elliminate Hit numero " << nReds << std::endl;
}

////define this as a plug-in
//DEFINE_FWK_MODULE(EcalTTowerKiller);

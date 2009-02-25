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
  DeadChannelFileName_  = ps.getParameter<std::string>("DeadChannelsFile");
  
  produces< EcalRecHitCollection >(reducedHitCollectionEB_);
  produces< EcalRecHitCollection >(rejectedHitCollectionEB_);
  produces< EcalRecHitCollection >(reducedHitCollectionEE_);
  produces< EcalRecHitCollection >(rejectedHitCollectionEE_);
  
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


   // get the hit collection from the event:
   std::vector<edm::InputTag>::const_iterator i;
   for (i=ECollection_.begin(); i!=ECollection_.end(); i++) {
     
     edm::Handle<EcalRecHitCollection> ec;
     iEvent.getByLabel(*i,ec);
     
     if (!( ec.isValid())) {
       std::cout << "could not get a handle on the EcalRecHitCollection!" << std::endl;
       return;
     }
     
     const EcalRecHitCollection* hit_collectionEB = ec.product();
     
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
       
       for( std::vector<pair<int, int> >::const_iterator DeadCell = ChannelsDeadID.begin();
	    DeadCell < ChannelsDeadID.end(); DeadCell++ ){
	 
	 if(iFED == DeadCell->first && iTT == DeadCell->second){
	   DetId itID = it->id();
	   ItIsDead = true;
	 }
       }//End looping on vector of Dead Cells

       if(!ItIsDead){
	 EcalRecHit NewHit(it->id(), NewEnergy, it->time());
	 if(isBarrel) redCollectionEB->push_back( NewHit );
	 else redCollectionEE->push_back( NewHit );
       }

       else{
	 EcalRecHit NewHit(it->id(), NewEnergy, it->time());
	  if(isBarrel) rejCollectionEB->push_back( NewHit );
	  else rejCollectionEE->push_back( NewHit );
       }
       
       
     }
   }

   iEvent.put(redCollectionEB, reducedHitCollectionEB_);
   iEvent.put(rejCollectionEB, rejectedHitCollectionEB_);
   iEvent.put(redCollectionEE, reducedHitCollectionEE_);
   iEvent.put(rejCollectionEE, rejectedHitCollectionEE_);

//   std::cout << "============ quanti???? " << nRed << std::endl; 
}




// ------------ method called once each job just before starting event loop  ------------
void 
EcalTTowerKiller::beginJob(const edm::EventSetup&)
{

  nReds = 0;


  //Open the DeadChannel file, read it.
  std::ifstream inFile (DeadChannelFileName_.c_str(),std::ios::in);
  
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
}



// ------------ method called once each job just after ending the event loop  ------------
void 
EcalTTowerKiller::endJob() {

  //std::cout << "++++ elliminate Hit numero " << nReds << std::endl;
}

////define this as a plug-in
//DEFINE_FWK_MODULE(EcalTTowerKiller);

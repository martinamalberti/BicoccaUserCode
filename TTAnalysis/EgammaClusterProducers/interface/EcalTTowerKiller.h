#ifndef RecoEcal_EcalTTowerKiller_HH
#define RecoEcal_EcalTTowerKiller_HH
 
/** \class EcalChannelKiller
  *
  *  $Date: 2007/05/03 12:33:39 $
  *  $Revision: 1.1 $
  */
 
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

#include <string>
using namespace cms;
using namespace std;
//
// class decleration
//

class EcalTTowerKiller : public edm::EDProducer {



   public:
      explicit EcalTTowerKiller(const edm::ParameterSet&);
      ~EcalTTowerKiller();

      int nReds;
      


   private:


      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
     string hitProducer_;
     string hitCollection_;
     string reducedHitCollectionEB_;
     string rejectedHitCollectionEB_;
     string reducedHitCollectionEE_;
     string rejectedHitCollectionEE_;
     
     string DeadChannelFileName_;

     std::vector<edm::InputTag> ECollection_;
     edm::InputTag EBCollection_, EECollection_;
     std::vector<pair<int, int> > ChannelsDeadID;

};


#endif

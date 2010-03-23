#ifndef AlCaSuperClustersTest_H
#define AlCaSuperClustersTest_H 

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"


class AlCaSuperClustersTest : public edm::EDAnalyzer {
  public:
    explicit AlCaSuperClustersTest (const edm::ParameterSet&) ;
    ~AlCaSuperClustersTest () {}
     virtual void analyze (const edm::Event& iEvent, 
                           const edm::EventSetup& iSetup) ;
     virtual void beginJob () ;
     virtual void endJob () ;

  private:

    int m_nSCColl ;
    edm::InputTag m_EB_SC ;         // barrel superclusters (SC)
    edm::InputTag m_EB_SC_corr ;    // corrected barrel SC
    edm::InputTag m_EE_SC ;         // endcap SC
    edm::InputTag m_EE_SC_corr ;    // corrected endcap SC
    edm::InputTag m_EE_SC_corr_ES ; // corr. endcap SC + preshower energy
    edm::InputTag m_ES_SC ;         // preshower SC
    edm::InputTag m_HF_SC ;         // fwd calorimeter elctro magnetic SC
    edm::InputTag m_PF_SC ;         // particle flow SC
    edm::InputTag m_merge_SC ;      // merged SC

    edm::InputTag m_ElectronLabel ;
    std::string m_outputFileName ;

    std::vector<std::vector<double> > m_getCollStatus ;
    std::vector<TH1F*> m_energies ;

  private:

    template <typename TColl>
    int getCollection (
      edm::Handle<TColl> & handle, 
      const edm::InputTag & inputTag,
      const edm::Event& iEvent 
      ) 
    {
      iEvent.getByLabel (inputTag, handle) ;
      if (! (handle.isValid ()) )
        {
          edm::LogWarning ("AlCaSuperClustersTest") << inputTag 
                                               << " not available" ;
          return 1 ;
        }
      return 0 ;
    }
  
    template <typename TColl>
    void fillHisto (
      TH1F & energy_h1,
      edm::Handle<TColl> & handle 
      ) 
    {
      //PG loop over the collection
      for (typename TColl::const_iterator iSC = handle->begin () ;
           iSC != handle->end () ;
           ++iSC) 
        {
          energy_h1.Fill (iSC->Energy ()) ;
        } //PG loop over the collection      
    }
  
     
} ;
#endif

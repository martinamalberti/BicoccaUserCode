#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFSkimEfficiencyNtuple.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


#include <Math/VectorUtil.h>

      VBFSkimEfficiencyNtuple::VBFSkimEfficiencyNtuple (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_elePtMin (iConfig.getParameter<double> ("elePtMin")) ,
  m_muPtMin (iConfig.getParameter<double> ("muPtMin")) ,
  m_minLeptonsNum (iConfig.getParameter<int> ("minLeptonsNum"))
{
}


// --------------------------------------------------------------------


      VBFSkimEfficiencyNtuple::~VBFSkimEfficiencyNtuple ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


// --------------------------------------------------------------------


      void
        VBFSkimEfficiencyNtuple::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 numEvents_analyzed++;
 
 
  //AM get the muons collection
 edm::Handle<reco::MuonCollection> MuonHandle;
 iEvent.getByLabel (m_muInputTag,MuonHandle); 
 int mu_counter = 0;
 for(reco::MuonCollection::const_iterator muonIt = MuonHandle->begin(); muonIt != MuonHandle->end(); muonIt++)
 {
  if (muonIt -> pt()> m_muPtMin) mu_counter++;
 }
   
    
   //AM get the GSF electrons collection
 edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle;
 iEvent.getByLabel (m_GSFInputTag,GSFHandle); 
 int ele_counter = 0;
 for(reco::PixelMatchGsfElectronCollection::const_iterator electronIt = GSFHandle->begin(); electronIt != GSFHandle->end(); electronIt++)
 {
  if (electronIt -> pt()> m_elePtMin) ele_counter++;
 }
   
 if ((mu_counter + ele_counter) >= m_minLeptonsNum) numEvents_selected++ ;
}
// --------------------------------------------------------------------


      void 
        VBFSkimEfficiencyNtuple::beginJob (const edm::EventSetup&)
{
 numEvents_analyzed = 0;
 numEvents_selected = 0;
 
 edm::Service<TFileService> fs ;

 m_ntuple = fs->make <TNtuple> ("ntuple","number of events","numEvents_analyzed:numEvents_selected");

}


// --------------------------------------------------------------------


      void 
        VBFSkimEfficiencyNtuple::endJob () 
{
 m_ntuple -> Fill (numEvents_analyzed,numEvents_selected) ;
}

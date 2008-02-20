// $Id: VBFDiffTagFinderComparison.cc,v 1.2 2008/02/15 16:58:48 tancini Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFDiffTagFinderComparison.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"
#include <Math/VectorUtil.h>

VBFDiffTagFinderComparison::VBFDiffTagFinderComparison (const edm::ParameterSet& iConfig) :
      m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")),
      m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")),
      m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax"))
 {}


// --------------------------------------------------------------------


VBFDiffTagFinderComparison::~VBFDiffTagFinderComparison ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


// --------------------------------------------------------------------


void
VBFDiffTagFinderComparison::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle);
 
  // get quarkk tag


  std::pair<VBFjetIt,VBFjetIt> tagJetCandsMaxMinv = findTagJets (jetCollectionHandle->begin (),
                                                          jetCollectionHandle->end (),
                                                          m_jetPtMin, m_jetEtaMax) ;

  reco::CaloJetCollection TheJets = *jetCollectionHandle;

  std::pair<VBFjetIt,VBFjetIt> tagJetCandsMaxPt = findMaxPtJetsPair (TheJets,
                                                          m_jetPtMin, m_jetEtaMax) ;


   
}
// --------------------------------------------------------------------


void 
VBFDiffTagFinderComparison::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_deltaR = fs->make<TH1F> ("m_deltaR", "dR between jet and ele", 100, 0, 10);
  m_HEoverEmE = fs->make<TH1F> ("m_HEoverEmE", "H/E", 100, -0.2,0.2) ;
  m_EleOJetEratio = fs->make<TH1F> ("m_EleOJetEratio", "ratio between ele and jet energies", 100, 0, 50);

}


// --------------------------------------------------------------------


void 
VBFDiffTagFinderComparison::endJob () 
{
}


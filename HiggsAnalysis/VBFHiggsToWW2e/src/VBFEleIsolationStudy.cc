// $Id: VBFEleIsolationStudy.cc,v 1.4 2008/01/15 20:47:17 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleIsolationStudy.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

VBFEleIsolationStudy::VBFEleIsolationStudy (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_OLDIsoInputTag (iConfig.getParameter<edm::InputTag> ("OLDIsoInputTag")) ,
  m_NEWIsoInputTag (iConfig.getParameter<edm::InputTag> ("NEWIsoInputTag")) 
{}


// --------------------------------------------------------------------


VBFEleIsolationStudy::~VBFEleIsolationStudy ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFEleIsolationStudy::analyze (const edm::Event& iEvent, 
                               const edm::EventSetup& iSetup)
{

  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle) ; 

  std::cout << "[VBFEleIsolationStudy][analyze] number of GSF electrons : "
            << GSFHandle->size () 
            << std::endl ;

  //PG get the OLDIso electrons collection
  edm::Handle<electronCollection> OLDIsoHandle ;
  iEvent.getByLabel (m_OLDIsoInputTag,OLDIsoHandle) ; 

  std::cout << "[VBFEleIsolationStudy][analyze] number of OLDIso electrons : "
            << OLDIsoHandle->size () 
            << std::endl ;

  //PG get the NEWIso electrons collection
  edm::Handle<electronCollection> NEWIsoHandle ;
  iEvent.getByLabel (m_NEWIsoInputTag,NEWIsoHandle) ; 

  std::cout << "[VBFEleIsolationStudy][analyze] number of NEWIso electrons : "
            << NEWIsoHandle->size () 
            << std::endl ;

}


// --------------------------------------------------------------------


void 
VBFEleIsolationStudy::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFEleIsolationStudy::endJob () 
{
}


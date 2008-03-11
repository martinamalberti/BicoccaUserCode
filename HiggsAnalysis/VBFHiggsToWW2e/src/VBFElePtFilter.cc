// $Id: VBFElePtFilter.cc,v 1.1 2008/03/11 09:45:38 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFElePtFilter.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
//for TH1F
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include <Math/VectorUtil.h>
#include "TLorentzVector.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"


VBFElePtFilter::VBFElePtFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_firstPtMin (iConfig.getParameter<double> ("firstPtMin")) ,
  m_secondPtMin (iConfig.getParameter<double> ("secondPtMin")) ,
  m_testCharge (iConfig.getParameter<bool> ("testCharge"))
{}


// --------------------------------------------------------------------


VBFElePtFilter::~VBFElePtFilter ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


bool
VBFElePtFilter::filter (edm::Event& iEvent, 
                       const edm::EventSetup& iSetup)
{
  // Get the electrons
  edm::Handle<electronCollection> EleHandle ;
  iEvent.getByLabel (m_GSFInputTag, EleHandle) ;

  if (EleHandle->size () < 2) return false ;
  
  //PG crea due iteratori per i due elettroni scelti
  electronCollection::const_iterator firstEle ;
  electronCollection::const_iterator secondEle ;
  findLeptons (firstEle, secondEle, EleHandle) ;

  if (firstEle->pt () < m_firstPtMin) return false ;
  if (secondEle->pt () < m_secondPtMin) return false ;
  
  if (m_testCharge && firstEle->charge () * secondEle->charge () >= 0) return false ;

  return true ;
}


// --------------------------------------------------------------------


void 
VBFElePtFilter::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFElePtFilter::endJob () 
{
}


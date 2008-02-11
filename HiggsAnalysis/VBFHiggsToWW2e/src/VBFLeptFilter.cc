// $Id: VBFLeptPLots.cc,v 1.2 2008/02/08 13:35:16 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFLeptFilter.h"
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


VBFLeptFilter::VBFLeptFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muonsInputTag"))
{}


// --------------------------------------------------------------------


VBFLeptFilter::~VBFLeptFilter ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


bool
VBFLeptFilter::filter (edm::Event& iEvent, 
                       const edm::EventSetup& iSetup)
{

  // Get the electrons
  edm::Handle<electronCollection> EleHandle ;
  iEvent.getByLabel (m_GSFInputTag, EleHandle) ;

  //PG crea due iteratori per i due elettroni scelti
  electronCollection::const_iterator firstEle ;
  electronCollection::const_iterator secondEle ;
  findLeptons (firstEle, secondEle, EleHandle) ;

  //VT get the Global muons collection
  edm::Handle<muonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 
    
  //PG crea due iteratori per i due elettroni scelti
  muonCollection::const_iterator firstMu ;
  muonCollection::const_iterator secondMu ;
  findLeptons (firstMu, secondMu, MuonHandle) ;

  std::vector <const reco::Particle *> leptons ;
  if (firstEle != EleHandle->end ()) leptons.push_back (&*firstEle) ;
  if (secondEle != EleHandle->end ()) leptons.push_back (&*secondEle) ;
  if (firstMu != MuonHandle->end ()) leptons.push_back (&*firstMu) ;
  if (secondMu != MuonHandle->end ()) leptons.push_back (&*secondMu) ;

  if (leptons.size () < 2) return false ;
  sort (leptons.begin (), leptons.end (), PtSorting ()) ;

  double invMass = (leptons[0]->p4 () + leptons[1]->p4 ()).M () ;
  double deltaphi = deltaPhi (leptons[0]->phi (), leptons[1]->phi ()) ;

  return true ;

}


// --------------------------------------------------------------------


void 
VBFLeptFilter::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFLeptFilter::endJob () 
{
}


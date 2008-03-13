// $Id: VBFLeptFilter.cc,v 1.3 2008/02/12 12:06:02 govoni Exp $
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
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muonsInputTag")) ,
  m_invMassMin (iConfig.getParameter<double> ("invMassMin")) ,
  m_invMassMax (iConfig.getParameter<double> ("invMassMax")) ,
  m_deltaPhiMax (iConfig.getParameter<double> ("deltaPhiMax"))
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

  std::vector <std::pair<int, const reco::Particle *> > leptons ;
  leptons.reserve (EleHandle->size () + MuonHandle->size ()) ;
  if (firstEle != EleHandle->end ()) leptons.push_back (
    std::pair<int, const reco::Particle *> (1,&*firstEle)) ;
  if (secondEle != EleHandle->end ()) leptons.push_back (
    std::pair<int, const reco::Particle *> (1,&*secondEle)) ;
  if (firstMu != MuonHandle->end ()) leptons.push_back (
    std::pair<int, const reco::Particle *> (2,&*firstMu)) ;
  if (secondMu != MuonHandle->end ()) leptons.push_back (
    std::pair<int, const reco::Particle *> (2,&*secondMu)) ;

  if (leptons.size () < 2) return false ;

  sort (leptons.begin (), leptons.end (), PtSorting ()) ;

  if (leptons[0].first = leptons[1].first)
    {
      double invMass = (leptons[0].second->p4 () + leptons[1].second->p4 ()).M () ;
      if (invMass < m_invMassMin || invMass > m_invMassMax) return false ;
    }
  double deltaphi = vbfhww2l::deltaPhi (leptons[0].second->phi (), leptons[1].second->phi ()) ;
  if (deltaphi > m_deltaPhiMax) return false ;

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


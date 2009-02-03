// $Id: VBFEleSelectionsStudy.cc,v 1.1 2008/03/17 17:01:16 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleSelectionsStudy.h"
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
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"


VBFEleSelectionsStudy::VBFEleSelectionsStudy (const edm::ParameterSet& iConfig) :
  m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")) ,
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_ARInputTag (iConfig.getParameter<edm::InputTag> ("ARInputTag"))
{}


// --------------------------------------------------------------------


VBFEleSelectionsStudy::~VBFEleSelectionsStudy ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFEleSelectionsStudy::analyze (const edm::Event& iEvent, 
                                const edm::EventSetup& iSetup)
{
  //PG MC thruth candidates collection  
  //PG NB this works only with two MC electrons
  //PG so far not used nor tested the MC part
  edm::Handle<reco::CandidateCollection> genParticles ;
  iEvent.getByLabel (m_MCtruthInputTag, genParticles) ;

  const reco::Candidate * firstMCEle ;
  const reco::Candidate * secondMCEle ;

  findFirstTwoMC (genParticles, &firstMCEle, &secondMCEle) ;

  // Get the electrons
  edm::Handle<electronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag, GSFHandle) ;

  edm::Handle<electronCollection> ARHandle ;
  iEvent.getByLabel (m_ARInputTag, ARHandle) ;

  m_ARvsGSF->Fill (GSFHandle->size (), ARHandle->size ()) ;

  electronCollection::const_iterator firstGSFEle ;
  electronCollection::const_iterator secondGSFEle ;
  findFirstTwo (GSFHandle, firstGSFEle, secondGSFEle) ;
  //PG if not enough electrons
  if (firstGSFEle == secondGSFEle) return ; 

  electronCollection::const_iterator firstAREle ;
  electronCollection::const_iterator secondAREle ;
  findFirstTwo (ARHandle, firstAREle, secondAREle) ;
  //PG if not enough electrons
  if (firstAREle == secondAREle) return ; 

//  m_deltaEta->Fill (firstAREle->eta () - firstGSFEle->eta ()) ;
//  m_deltaEta->Fill (secondAREle->eta () - secondGSFEle->eta ()) ;
//  m_deltaPhi->Fill (deltaPhi (firstAREle->phi (), firstGSFEle->phi ())) ;
//  m_deltaPhi->Fill (deltaPhi (secondAREle->phi (), secondGSFEle->phi ())) ;
  m_deltaEta->Fill (firstAREle->eta () - firstMCEle->eta ()) ;
  m_deltaEta->Fill (secondAREle->eta () - secondMCEle->eta ()) ;
  m_deltaPhi->Fill (vbfhww2l::deltaPhi (firstAREle->phi (), firstMCEle->phi ())) ;
  m_deltaPhi->Fill (vbfhww2l::deltaPhi (secondAREle->phi (), secondMCEle->phi ())) ;
    
}


// --------------------------------------------------------------------


void 
VBFEleSelectionsStudy::findFirstTwo (
  edm::Handle<electronCollection> & EleHandle ,
  electronCollection::const_iterator & firstEle ,
  electronCollection::const_iterator & secondEle) 
{
  double firstEleMaxPt = 0 ;
  double secondEleMaxPt = 0 ;

  //PG switch for electrons
  switch (EleHandle->size ())
    {
      case 0 :
        firstEle = secondEle = EleHandle->end () ;
        return ;
      case 1 :
//        firstEleMaxPt == GSFHandle->begin ()->pt () ;
        firstEle = EleHandle->begin () ;
        secondEle = firstEle ;
        return ;
      default :
        //PG loop over electrons
        for (electronCollection::const_iterator eleIt = 
             EleHandle->begin () ; 
             eleIt != EleHandle->end () ; 
             ++eleIt) 
          {
            if (firstEleMaxPt < eleIt->pt ())
              {
                secondEleMaxPt = firstEleMaxPt ;
                secondEle = firstEle ;
                firstEleMaxPt = eleIt->pt () ;
                firstEle = eleIt ;
              }
            else if (secondEleMaxPt < eleIt->pt ())
              {
                secondEleMaxPt = eleIt->pt () ;
                secondEle = eleIt ;
              }  
          } //PG loop over electrons 
    } //PG switch for electrons
}


// --------------------------------------------------------------------


void 
VBFEleSelectionsStudy::findFirstTwoMC (
  edm::Handle<reco::CandidateCollection> & EleHandle ,
  const reco::Candidate ** firstEle ,
  const reco::Candidate ** secondEle) 
{
  double firstEleMaxPt = 0 ;
  double secondEleMaxPt = 0 ;

  //PG loop over generated particles
  for (reco::CandidateCollection::const_iterator particle = EleHandle->begin () ; 
       particle != EleHandle->end () ; 
       ++particle) 
    {
        int mumPDG = particle->pdgId () ;
        int mumSTATUS = particle->status () ;
        if (abs (mumPDG) == 24 && mumSTATUS == 3) //W+-
          {
            for (int i = 0; i < particle->numberOfDaughters () ; ++i) 
              {
                int PDG = particle->daughter (i)->pdgId () ;    
                if (abs (PDG) != 11) continue ;
    
                if (firstEleMaxPt < particle->daughter (i)->pt ())
                  {
                    secondEleMaxPt = firstEleMaxPt ;
                    *secondEle = *firstEle ;
                    firstEleMaxPt = particle->daughter (i)->pt () ;
                    *firstEle = particle->daughter (i) ;
                    std::cerr << "[PIETRO] new first " << firstEle << std::endl ;
                  }
                else if (secondEleMaxPt < particle->daughter (i)->pt ())
                  {
                    secondEleMaxPt = particle->daughter (i)->pt () ;
                    *secondEle = particle->daughter (i) ;
                    std::cerr << "[PIETRO] new second " << secondEle << std::endl ;
                  } 
              } //W+-
          }
     } //PG loop over generated particles

  return ;
}

// --------------------------------------------------------------------


void 
VBFEleSelectionsStudy::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_ARvsGSF = fs->make<TH2F> ("m_ARvsGSF","ambiguity resolved vs GSF",6,0,6,6,0,6) ;
  m_deltaEta = fs->make<TH1F> ("m_deltaEta","#eta_{res} - #eta_{GSF}",200,-0.1,0.1) ;
  m_deltaPhi = fs->make<TH1F> ("m_deltaPhi","#phi_{res} - #phi_{GSF}",200,0,0.1) ;
}


// --------------------------------------------------------------------


void 
VBFEleSelectionsStudy::endJob () 
{
}


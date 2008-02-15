// $Id: VBFTestTagJets.cc,v 1.3 2008/01/29 11:12:46 tancini Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFTestTagJets.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include <Math/VectorUtil.h>


VBFTestTagJets::VBFTestTagJets (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag")) ,
  m_MCjetTagsInputTag (iConfig.getParameter<edm::InputTag> ("MCjetTagsInputTag"))
{}


// --------------------------------------------------------------------


VBFTestTagJets::~VBFTestTagJets ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFTestTagJets::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{
  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;

  std::cerr << "[VBFTestTagJets][analyze] number of jet Tags : "
            << jetTagsHandle->size () 
            << std::endl ;

  if (jetTagsHandle->size () < 2) return ;

  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> MCjetTagsHandle ;
  iEvent.getByLabel (m_MCjetTagsInputTag, MCjetTagsHandle) ;

  std::cerr << "[VBFTestTagJets][analyze] number of jet Tags : "
            << MCjetTagsHandle->size () 
            << std::endl ;
            
  if (MCjetTagsHandle->size () < 2) return ;

  int maxIndex = (*MCjetTagsHandle)[0].p4 ().E () < 
                 (*MCjetTagsHandle)[1].p4 ().E () ;

  if (fabs ((*jetTagsHandle)[0].momentum ().Eta () -
            (*MCjetTagsHandle)[0].momentum ().Eta ()) <   
      fabs ((*jetTagsHandle)[0].momentum ().Eta () - 
            (*MCjetTagsHandle)[1].momentum ().Eta ()) )
    {
      m_deltaEta->Fill ((*jetTagsHandle)[0].momentum ().Eta () -
                        (*MCjetTagsHandle)[0].momentum ().Eta ()) ;
      m_deltaPhi->Fill ((*jetTagsHandle)[0].momentum ().Phi () -
                        (*MCjetTagsHandle)[0].momentum ().Phi ()) ;
      m_deltaR->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[0].momentum (),
                                                      (*MCjetTagsHandle)[0].momentum ())) ;
      m_Ejet_o_EMC->Fill ((*jetTagsHandle)[0].p4 ().E () /
                          (*MCjetTagsHandle)[0].p4 ().E ()) ;

      m_deltaEta->Fill ((*jetTagsHandle)[1].momentum ().Eta () -
                        (*MCjetTagsHandle)[1].momentum ().Eta ()) ;
      m_deltaPhi->Fill ((*jetTagsHandle)[1].momentum ().Phi () -
                        (*MCjetTagsHandle)[1].momentum ().Phi ()) ;
      m_deltaR->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[1].momentum (),
                                                      (*MCjetTagsHandle)[1].momentum ())) ;
      m_Ejet_o_EMC->Fill ((*jetTagsHandle)[1].p4 ().E () /
                          (*MCjetTagsHandle)[1].p4 ().E ()) ;

      m_deltaEtaMaxE->Fill ((*jetTagsHandle)[maxIndex].momentum ().Eta () -
                        (*MCjetTagsHandle)[maxIndex].momentum ().Eta ()) ;
      m_deltaPhiMaxE->Fill ((*jetTagsHandle)[maxIndex].momentum ().Phi () -
                        (*MCjetTagsHandle)[maxIndex].momentum ().Phi ()) ;
      m_deltaRMaxE->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[maxIndex].momentum (),
                                                          (*MCjetTagsHandle)[maxIndex].momentum ())) ;
      m_Ejet_o_EMCMaxE->Fill ((*jetTagsHandle)[maxIndex].p4 ().E () /
                          (*MCjetTagsHandle)[maxIndex].p4 ().E ()) ;

      m_deltaEtaMinE->Fill ((*jetTagsHandle)[!maxIndex].momentum ().Eta () -
                        (*MCjetTagsHandle)[!maxIndex].momentum ().Eta ()) ;
      m_deltaPhiMinE->Fill ((*jetTagsHandle)[!maxIndex].momentum ().Phi () -
                        (*MCjetTagsHandle)[!maxIndex].momentum ().Phi ()) ;
      m_deltaRMinE->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[!maxIndex].momentum (),
                                                          (*MCjetTagsHandle)[!maxIndex].momentum ())) ;
      m_Ejet_o_EMCMinE->Fill ((*jetTagsHandle)[!maxIndex].p4 ().E () /
                          (*MCjetTagsHandle)[!maxIndex].p4 ().E ()) ;
    }
  else 
    {
      m_deltaEta->Fill ((*jetTagsHandle)[0].momentum ().Eta () -
                        (*MCjetTagsHandle)[1].momentum ().Eta ()) ;
      m_deltaPhi->Fill ((*jetTagsHandle)[0].momentum ().Phi () -
                        (*MCjetTagsHandle)[1].momentum ().Phi ()) ;
      m_deltaR->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[0].momentum (),
                                                      (*MCjetTagsHandle)[1].momentum ())) ;
      m_Ejet_o_EMC->Fill ((*jetTagsHandle)[0].p4 ().E () /
                          (*MCjetTagsHandle)[1].p4 ().E ()) ;

      m_deltaEta->Fill ((*jetTagsHandle)[1].momentum ().Eta () -
                        (*MCjetTagsHandle)[0].momentum ().Eta ()) ;
      m_deltaPhi->Fill ((*jetTagsHandle)[1].momentum ().Phi () -
                        (*MCjetTagsHandle)[0].momentum ().Phi ()) ;
      m_deltaR->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[1].momentum (),
                                                      (*MCjetTagsHandle)[0].momentum ())) ;
      m_Ejet_o_EMC->Fill ((*jetTagsHandle)[1].p4 ().E () /
                          (*MCjetTagsHandle)[0].p4 ().E ()) ;

      m_deltaEtaMaxE->Fill ((*jetTagsHandle)[!maxIndex].momentum ().Eta () -
                        (*MCjetTagsHandle)[maxIndex].momentum ().Eta ()) ;
      m_deltaPhiMaxE->Fill ((*jetTagsHandle)[!maxIndex].momentum ().Phi () -
                        (*MCjetTagsHandle)[maxIndex].momentum ().Phi ()) ;
      m_deltaRMaxE->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[!maxIndex].momentum (),
                                                          (*MCjetTagsHandle)[maxIndex].momentum ())) ;
      m_Ejet_o_EMCMaxE->Fill ((*jetTagsHandle)[!maxIndex].p4 ().E () /
                              (*MCjetTagsHandle)[maxIndex].p4 ().E ()) ;
   
      m_deltaEtaMinE->Fill ((*jetTagsHandle)[maxIndex].momentum ().Eta () -
                        (*MCjetTagsHandle)[!maxIndex].momentum ().Eta ()) ;
      m_deltaPhiMinE->Fill ((*jetTagsHandle)[maxIndex].momentum ().Phi () -
                        (*MCjetTagsHandle)[!maxIndex].momentum ().Phi ()) ;
      m_deltaRMinE->Fill (ROOT::Math::VectorUtil::DeltaR ((*jetTagsHandle)[maxIndex].momentum (),
                                                          (*MCjetTagsHandle)[!maxIndex].momentum ())) ;
      m_Ejet_o_EMCMinE->Fill ((*jetTagsHandle)[maxIndex].p4 ().E () /
                          (*MCjetTagsHandle)[!maxIndex].p4 ().E ()) ;
    }  

}


// --------------------------------------------------------------------


void 
VBFTestTagJets::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;

  m_deltaEta   = fs->make<TH1F> ("deltaEta","deltaEta",100,-0.2,0.2) ;
  m_deltaPhi   = fs->make<TH1F> ("deltaPhi","deltaPhi",100,-0.2,0.2) ;
  m_deltaR     = fs->make<TH1F> ("deltaR","deltaR",150,0,0.3) ;
  m_Ejet_o_EMC = fs->make<TH1F> ("Ejet_o_EMC","Ejet_o_EMC",300,0,10) ; 
  
  m_deltaEtaMaxE   = fs->make<TH1F> ("deltaEtaMaxE","deltaEtaMaxE",100,-0.2,0.2) ;
  m_deltaPhiMaxE   = fs->make<TH1F> ("deltaPhiMaxE","deltaPhiMaxE",100,-0.2,0.2) ;
  m_deltaRMaxE     = fs->make<TH1F> ("deltaRMaxE","deltaRMaxE",150,0,0.3) ;
  m_Ejet_o_EMCMaxE = fs->make<TH1F> ("Ejet_o_EMCMaxE","Ejet_o_EMCMaxE",300,0,10) ; 
  
  m_deltaEtaMinE   = fs->make<TH1F> ("deltaEtaMinE","deltaEtaMinE",100,-0.2,0.2) ;
  m_deltaPhiMinE   = fs->make<TH1F> ("deltaPhiMinE","deltaPhiMinE",100,-0.2,0.2) ;
  m_deltaRMinE     = fs->make<TH1F> ("deltaRMinE","deltaRMinE",150,0,0.3) ;
  m_Ejet_o_EMCMinE = fs->make<TH1F> ("Ejet_o_EMCMinE","Ejet_o_EMCMinE",300,0,10) ; 

}


// --------------------------------------------------------------------


void 
VBFTestTagJets::endJob () 
{
}


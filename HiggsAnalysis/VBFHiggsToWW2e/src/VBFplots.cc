// $Id: VBFplots.cc,v 1.1 2008/01/29 11:12:46 tancini Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFplots.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
//for TH1F
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

VBFplots::VBFplots (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag")) ,
  m_jetOthersInputTag (iConfig.getParameter<edm::InputTag> ("jetOthersInputTag")) ,
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_metInputTag (iConfig.getParameter<edm::InputTag> ("metInputTag")) 
{}


// --------------------------------------------------------------------


VBFplots::~VBFplots ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFplots::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{

  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;

  if (jetTagsHandle->size () < 2) return ;
  if ((*jetTagsHandle)[0].p4 () == (*jetTagsHandle)[1].p4 ()) return;

  // Get remaining jets
  edm::Handle<reco::CaloJetCollection> jetOthersHandle ;
  iEvent.getByLabel (m_jetOthersInputTag, jetOthersHandle) ;


  // Tag jets plots
  m_deltaEta -> Fill (fabs ((*jetTagsHandle)[0].p4 ().Eta () - (*jetTagsHandle)[1].p4 ().Eta ()) ) ;
  LorentzVector summedTags = (*jetTagsHandle)[0].p4 () + (*jetTagsHandle)[1].p4 ();
  m_sumEta -> Fill (summedTags.Eta ()) ;
  m_invMass -> Fill (summedTags.M ()) ;

  m_eta -> Fill ((*jetTagsHandle)[0].p4 ().Eta () ) ;
  m_eta -> Fill ((*jetTagsHandle)[1].p4 ().Eta () ) ;
  m_energy -> Fill ((*jetTagsHandle)[0].p4 ().E () );
  m_energy -> Fill ((*jetTagsHandle)[1].p4 ().E () );
  m_pt -> Fill ((*jetTagsHandle)[0].p4 ().Pt () );
  m_pt -> Fill ((*jetTagsHandle)[1].p4 ().Pt () );

  LorentzVector summedOthers(0.0, 0.0, 0.0, 0.0) ;
  // Other jets plots
  for (reco::CaloJetCollection::const_iterator jetIt = jetOthersHandle->begin () ; 
       jetIt != jetOthersHandle->end () ; 
       ++jetIt) 
    {

      m_etaOthers -> Fill (jetIt->p4 ().Eta () ) ;
      m_energyOthers -> Fill (jetIt->p4 ().E () ) ;
      m_ptOthers -> Fill (jetIt->p4 ().Pt () ) ;
      summedOthers += (jetIt->p4 ());
    }

  m_etaOthersSummed -> Fill (summedOthers.Eta ()) ;
  m_energyOthersSummed -> Fill (summedOthers.E ()) ;
  m_ptOthersSummed -> Fill (summedOthers.Pt ()) ;
  m_invMassOtherSummed -> Fill (summedOthers.M ()) ;
}


// --------------------------------------------------------------------


void 
VBFplots::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;

  m_deltaEta = fs->make<TH1F> ("m_deltaEtaTags","#Delta#eta between tag jets",100,0,10) ;
  m_sumEta = fs->make<TH1F> ("m_sumEtaTags","#Sigma#eta of tag jets",100,-12,12) ;
  m_invMass = fs->make<TH1F> ("m_invMassTags","invariant mass of tag jets",100, 0, 4500) ;

  m_eta = fs->make<TH1F> ("m_etaTags","#eta of tag jets",50,-6,6) ;
  m_energy = fs->make<TH1F> ("m_energyTags","energy of tag jets",100, 0, 1200) ;
  m_pt = fs->make<TH1F> ("m_ptTags","pt of tag jets",100, 0, 600) ;

  m_etaOthers = fs->make<TH1F> ("m_etaOthers","#eta of tag jets",50,-6,6) ;
  m_energyOthers = fs->make<TH1F> ("m_energyOthers","energy of tag jets",100, 0, 1200) ;
  m_ptOthers = fs->make<TH1F> ("m_ptOthers","pt of tag jets",100, 0, 600) ;

  m_etaOthersSummed = fs->make<TH1F> ("m_etaOthersSummed","#eta of summed jets",50,-6,6) ;
  m_energyOthersSummed = fs->make<TH1F> ("m_energyOthersSummed","energy of summed jets",100, 0, 1200) ;
  m_ptOthersSummed = fs->make<TH1F> ("m_ptOthersSummed","pt of summed jets",100, 0, 600) ;
  m_invMassOtherSummed = fs->make<TH1F> ("m_invMassOthersSummed","invariant mass of summed jets",100, 0, 4500) ;
}


// --------------------------------------------------------------------


void 
VBFplots::endJob () 
{
}


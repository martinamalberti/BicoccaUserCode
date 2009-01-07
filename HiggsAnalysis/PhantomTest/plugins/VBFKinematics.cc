// $Id: VBFKinematics.cc,v 1.6 2009/01/06 15:27:19 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/PhantomTest/plugins/VBFKinematics.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <algorithm>

VBFKinematics::VBFKinematics (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag")) ,
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_metInputTag (iConfig.getParameter<edm::InputTag> ("metInputTag")) 
{
  edm::Service<TFileService> fileService ;
  m_jet_eta = fileService->make<TH1F> ("m_jet_eta","m_jet_eta",400,-3,3) ;
  m_ele_eta = fileService->make<TH1F> ("m_ele_eta","m_ele_eta",100,-3,3) ;
  m_lep_eta = fileService->make<TH1F> ("m_lep_eta","m_lep_eta",100,-3,3) ;
  m_mu_eta = fileService->make<TH1F> ("m_mu_eta","m_mu_eta",100,-3,3) ;
  m_met_phi = fileService->make<TH1F> ("m_met_phi","m_met_phi",200,-6.28,6.28) ;
  m_ele_pt = fileService->make<TH1F> ("m_ele_pt","m_ele_pt",100,0,200) ;
  m_mu_pt = fileService->make<TH1F> ("m_mu_pt","m_mu_pt",100,0,200) ;
  m_met_energy = fileService->make<TH1F> ("m_met_energy","m_met_energy",30,0,200) ;
  m_met_maxEInEmTowers = fileService->make<TH1F> ("m_met_maxEInEmTowers","m_met_maxEInEmTowers",200,0,100) ;
  m_met_maxEInHadTowers = fileService->make<TH1F> ("m_met_maxEInHadTowers","m_met_maxEInHadTowers",200,0,100) ;
  m_jet_maxEInEmTowers = fileService->make<TH1F> ("m_jet_maxEInEmTowers","m_jet_maxEInEmTowers",200,0,100) ;
  m_jet_maxEInHadTowers = fileService->make<TH1F> ("m_jet_maxEInHadTowers","m_jet_maxEInHadTowers",200,0,100) ;
  m_jet_multiplicity = fileService->make<TH1F> ("m_jet_multiplicity","m_jet_multiplicity",20,0,20) ;
  m_alljets_multiplicity = fileService->make<TH1F> ("m_alljets_multiplicity","m_alljets_multiplicity",10,0,10) ;
  m_sel_maxPtLep_pt = fileService->make<TH1F> ("m_sel_maxPtLep_pt","m_sel_maxPtLep_pt",150,0,200) ;
  m_sel_minPtLep_pt = fileService->make<TH1F> ("m_sel_minPtLep_pt","m_sel_minPtLep_pt",150,0,200) ;
  m_sel_mu_multiplicity = fileService->make<TH1F> ("m_sel_mu_multiplicity","m_sel_mu_multiplicity",10,0,10) ;
  m_sel_ele_multiplicity = fileService->make<TH1F> ("m_sel_ele_multiplicity","m_sel_ele_multiplicity",10,0,10) ;
  m_sel_lept_invmass = fileService->make<TH1F> ("m_sel_lept_invmass","m_sel_lept_invmass",150,0,200) ;
}


// --------------------------------------------------------------------


VBFKinematics::~VBFKinematics ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFKinematics::analyze (const edm::Event& iEvent, 
                        const edm::EventSetup& iSetup)
{
  // Get the tag jets
//  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  edm::Handle<reco::CaloJetCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;
  m_alljets_multiplicity->Fill (jetTagsHandle->size ()) ;
  int count = 0 ;
  for (int index = 0; index < jetTagsHandle->size () ; ++index)
    {
      m_jet_eta->Fill ((*jetTagsHandle)[index].p4().eta ()) ;
      m_jet_maxEInEmTowers->Fill ((*jetTagsHandle)[index].maxEInEmTowers()) ;
      m_jet_maxEInHadTowers->Fill ((*jetTagsHandle)[index].maxEInHadTowers()) ;
      if ((*jetTagsHandle)[index].p4().energy () > 15 /*GeV*/ &&
          fabs ((*jetTagsHandle)[index].p4().eta ()) < 5) ++count ;
    }
  m_jet_multiplicity->Fill (count) ;

  std::vector<const reco::RecoCandidate *> leptons ;
  //PG get the GSF electrons collection
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag, GSFHandle) ; 
  for (int index = 0; index < GSFHandle->size () ; ++index)
    {
      m_ele_eta->Fill ((*GSFHandle)[index].p4().eta ()) ;
      m_ele_pt->Fill ((*GSFHandle)[index].pt ()) ;
      leptons.push_back (&((*GSFHandle)[index])) ;
    }

  //PG get the electron ID collection
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle ;
  iEvent.getByLabel (m_electronIDInputTag, electronIDAssocHandle) ;

  //VT get the Global muons collection
  edm::Handle<reco::MuonCollection> MuonHandle ;
  iEvent.getByLabel (m_muInputTag, MuonHandle) ; 
  for (int index = 0; index < MuonHandle->size () ; ++index)
    {
      m_mu_eta->Fill ((*MuonHandle)[index].p4().eta ()) ;
      m_mu_pt->Fill ((*MuonHandle)[index].pt ()) ;
      leptons.push_back (&((*MuonHandle)[index])) ;
    }
    
  //PG get the calo MET
  edm::Handle<reco::CaloMETCollection> metCollectionHandle ;
  iEvent.getByLabel (m_metInputTag, metCollectionHandle) ;
  const CaloMETCollection *calometcol = metCollectionHandle.product () ;
  const CaloMET *calomet = &(calometcol->front ()) ;   
  m_met_phi->Fill (calomet->p4 ().phi ()) ;
  m_met_energy->Fill (calomet->p4 ().energy ()) ;
  m_met_maxEInEmTowers->Fill(calomet->maxEtInEmTowers());
  m_met_maxEInHadTowers->Fill(calomet->maxEtInHadTowers());

  sort (leptons.rbegin (), leptons.rend (), ptSorting ()) ;  
  for (int iLepton = 0 ; iLepton < min (2,(int)leptons.size ()) ; ++iLepton)
    {
      m_lep_eta->Fill (leptons.at (iLepton)->p4 ().eta ()) ;
    }

  //PG SELECTION
  //PG ---------
  
  if (leptons.size () != 2) return ;
  m_sel_maxPtLep_pt->Fill (leptons.at (0)->p4 ().Et ()) ;
  m_sel_minPtLep_pt->Fill (leptons.at (1)->p4 ().Et ()) ;
  m_sel_mu_multiplicity->Fill (MuonHandle->size ()) ;
  m_sel_ele_multiplicity->Fill (GSFHandle->size ()) ;
  LorentzVector sum = leptons.at (0)->p4 () + leptons.at (1)->p4 () ;
  m_sel_lept_invmass->Fill (sum.M ()) ;

}


// --------------------------------------------------------------------


void 
VBFKinematics::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
VBFKinematics::endJob () 
{
}


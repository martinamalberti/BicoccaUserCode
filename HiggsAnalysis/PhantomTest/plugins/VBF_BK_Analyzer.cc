// $Id: VBF_BK_Analyzer.cc,v 1.6 2009/01/06 15:27:19 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/PhantomTest/plugins/VBF_BK_Analyzer.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <algorithm>

VBF_BK_Analyzer::VBF_BK_Analyzer (const edm::ParameterSet& iConfig) :
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
 
 m_jet_eta_n90 = fileService->make<TH1F> ("m_jet_eta_n90","m_jet_eta_n90",400,-3,3) ;
 m_jet_eta_vs_n90 = fileService->make<TH2F> ("m_jet_eta_vs_n90","m_jet_eta_vs_n90",400,-3,3,100,0,100) ;
 m_jet_n90 = fileService->make<TH1F> ("n90","n90",100,0,20) ;
 m_jet_n60 = fileService->make<TH1F> ("n60","n60",100,0,20) ;
 m_sel_jet_eta = fileService->make<TH1F> ("m_sel_jet_eta","m_sel_jet_eta",400,-3,3) ;
 
 m_sel_number_events = fileService->make<TTree>("m_sel_number_events","m_sel_number_events");
//  m_sel_number_events->Branch("m_CJV",&m_CJV,"m_CJV/O");
//  m_sel_number_events->Branch("m_MET",&m_MET,"m_MET/O");
//  m_sel_number_events->Branch("m_pTLMax",&m_pTLMax,"m_pTLMax/O");
//  m_sel_number_events->Branch("m_pTLMin",&m_pTLMin,"m_pTLMin/O");
//  m_sel_number_events->Branch("m_mll",&m_mll,"m_mll/O");
//  m_sel_number_events->Branch("m_DPhi",&m_DPhi,"m_DPhi/O");

 m_sel_number_events->Branch("m_CJV_value",&m_CJV_value,"m_CJV_value/I");
 m_sel_number_events->Branch("m_MET_value",&m_MET_value,"m_MET_value/D");
 m_sel_number_events->Branch("m_MET_corrected_value",&m_MET_corrected_value,"m_MET_corrected_value/D");
 m_sel_number_events->Branch("m_pTLMax_value",&m_pTLMax_value,"m_pTLMax_value/D");
 m_sel_number_events->Branch("m_pTLMin_value",&m_pTLMin_value,"m_pTLMin_value/D");
 m_sel_number_events->Branch("m_mll_value",&m_mll_value,"m_mll_value/D");
 m_sel_number_events->Branch("m_DPhi_value",&m_DPhi_value,"m_DPhi_value/D");
 m_sel_number_events->Branch("m_charge_1_value",&m_charge_1_value,"m_charge_1_value/I");
 m_sel_number_events->Branch("m_charge_2_value",&m_charge_2_value,"m_charge_2_value/I");
 m_sel_number_events->Branch("m_eta_1_value",&m_eta_1_value,"m_eta_1_value/D");
 m_sel_number_events->Branch("m_eta_2_value",&m_eta_2_value,"m_eta_2_value/D");

 
 m_CJV_cut = iConfig.getParameter<double> ("CJV_cut");
 m_MET_cut = iConfig.getParameter<double> ("MET_cut");
 m_pTLMax_cut = iConfig.getParameter<double> ("pTLMax_cut");
 m_pTLMin_cut = iConfig.getParameter<double> ("pTLMin_cut");
 m_mll_cut = iConfig.getParameter<double> ("mll_cut");
 m_DPhi_cut = iConfig.getParameter<double> ("DPhi_cut");
 
 m_jetDeltaEta = iConfig.getParameter<double> ("jetDeltaEta");
 m_jetPtMin = iConfig.getParameter<double> ("jetPtMin");
 m_maxJetsNum = iConfig.getParameter<int> ("maxJetsNum");
 m_jetEtaMax = iConfig.getParameter<double> ("jetEtaMax");
 
}


// --------------------------------------------------------------------


VBF_BK_Analyzer::~VBF_BK_Analyzer ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
  VBF_BK_Analyzer::analyze (const edm::Event& iEvent, 
                          const edm::EventSetup& iSetup)
{
 
 m_CJV = false;
 m_MET = false;
 m_pTLMax = false;
 m_pTLMin = false;
 m_mll = false;
 m_DPhi = false;
 
 m_CJV_value = 0;
 m_MET_value = 0;
 m_MET_corrected_value= 0;
 m_pTLMax_value = 0;
 m_pTLMin_value = 0;
 m_mll_value = 0;
 m_DPhi_value = 0;
 m_eta_1_value = 0;
 m_eta_2_value = 0;
 
  // Get the tag jets
//  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
 edm::Handle<reco::CaloJetCollection> jetTagsHandle ;
 iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;
 m_alljets_multiplicity->Fill (jetTagsHandle->size ()) ;
 int count = 0 ;
 for (int index = 0; index < jetTagsHandle->size () ; ++index)
 {
  if (((*jetTagsHandle)[index]).n90()>=3) m_jet_eta_n90->Fill ((*jetTagsHandle)[index].p4().eta ()) ;
  m_jet_eta_vs_n90->Fill((*jetTagsHandle)[index].p4().eta (),(*jetTagsHandle)[index].n90());
  m_jet_n90->Fill ((*jetTagsHandle)[index].n90 ()) ;
  m_jet_n60->Fill ((*jetTagsHandle)[index].n60 ()) ;
  m_jet_eta->Fill ((*jetTagsHandle)[index].p4().eta ()) ;
  m_jet_maxEInEmTowers->Fill ((*jetTagsHandle)[index].maxEInEmTowers()) ;
  m_jet_maxEInHadTowers->Fill ((*jetTagsHandle)[index].maxEInHadTowers()) ;
  if ((*jetTagsHandle)[index].p4().energy () > 15 /*GeV*/ &&
        fabs ((*jetTagsHandle)[index].p4().eta ()) < 2.5) {
   m_sel_jet_eta->Fill ((*jetTagsHandle)[index].p4().eta ()) ;
   ++count ;
        }
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
 
 double EnergyT_muon = 0;
 for (int index = 0; index < MuonHandle->size () ; ++index)
 {
  m_mu_eta->Fill ((*MuonHandle)[index].p4().eta ()) ;
  m_mu_pt->Fill ((*MuonHandle)[index].pt ()) ;
  leptons.push_back (&((*MuonHandle)[index])) ;
  EnergyT_muon = EnergyT_muon + (*MuonHandle)[index].et ();
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

 
//  if (leptons.at (0)->p4 ().Et () < m_pTLMax_cut) m_pTLMax = true;
//  if (leptons.at (1)->p4 ().Et () > m_pTLMin_cut) m_pTLMin = true;
 
//  m_CJV = VBF_BK_Analyzer::VBFCentralJetVetoFilter(jetTagsHandle);
  
 
//  if(sum.M () < 20.) m_mll = true;
//  if(calomet->p4 ().energy () < 20.) m_MET = true;
//  if ((leptons.at (0)->momentum ().Phi () - leptons.at (1)->momentum ().Phi ()) < 100) m_DPhi = true;
  
  
 
//   m_CJV_value = VBF_BK_Analyzer::VBFCentralJetVetoFilterValue(jetTagsHandle);
  
  m_CJV_value = VBF_BK_Analyzer::VBFJetVetoFilterValue(jetTagsHandle); 
  m_MET_value = calomet->p4 ().energy ();
//   m_MET_corrected_value = calomet->p4 ().energy () + EnergyT_muon;
  m_pTLMax_value = leptons.at (0)->p4 ().Et ();
  m_pTLMin_value = leptons.at (1)->p4 ().Et ();
  m_mll_value = sum.M();
  m_DPhi_value = deltaPhi(leptons.at (0)->momentum ().Phi (),leptons.at (1)->momentum ().Phi ());
  m_charge_1_value = leptons.at (0)-> charge();
  m_charge_2_value = leptons.at (1)-> charge();
  m_eta_1_value = leptons.at (0)-> p4().eta ();
  m_eta_2_value = leptons.at (1)-> p4().eta ();
  
  m_sel_number_events->Fill();

}


// --------------------------------------------------------------------


void 
  VBF_BK_Analyzer::beginJob (const edm::EventSetup&)
{
}


// --------------------------------------------------------------------


void 
  VBF_BK_Analyzer::endJob () 
{
}



// --------------------------------------------------------------------

bool VBF_BK_Analyzer::VBFCentralJetVetoFilter (edm::Handle<reco::CaloJetCollection>& jetTagsHandle){
 
 LorentzVector firstTag = (*jetTagsHandle)[0].p4 () ;
 LorentzVector secondTag = (*jetTagsHandle)[1].p4 () ;

 double etaMax = firstTag.eta () ;
 double etaMin = secondTag.eta () ;
 if (etaMin > etaMax) 
{
 double tempo = etaMin ;
 etaMin = etaMax ; 
 etaMax = tempo ;
}

  etaMin -= m_jetDeltaEta ;
  etaMax += m_jetDeltaEta ;

  int counter = 0 ;

  typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
  for (VBFjetIt jetIt = jetTagsHandle->begin () ; 
       jetIt != jetTagsHandle->end () ; 
       ++jetIt )
{
 if (jetIt->eta () > etaMax ||
     jetIt->eta () < etaMin ||
     jetIt->et () < m_jetPtMin) continue ;
 ++counter ; 
}

  if (counter > m_maxJetsNum) return false ;
  return true ;

}



// --------------------------------------------------------------------

int VBF_BK_Analyzer::VBFCentralJetVetoFilterValue (edm::Handle<reco::CaloJetCollection>& jetTagsHandle){
 
 //---- basato su: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFCentralJetVetoFilter.cc?revision=1.3&view=markup ----//
 
 LorentzVector firstTag = (*jetTagsHandle)[0].p4 () ;
 LorentzVector secondTag = (*jetTagsHandle)[1].p4 () ;

 double etaMax = firstTag.eta () ;
 double etaMin = secondTag.eta () ;
 if (etaMin > etaMax) 
 {
  double tempo = etaMin ;
  etaMin = etaMax ; 
  etaMax = tempo ;
 }

 etaMin -= m_jetDeltaEta ;
 etaMax += m_jetDeltaEta ;

 int counter = 0 ;

 typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
 for (VBFjetIt jetIt = jetTagsHandle->begin () ; 
      jetIt != jetTagsHandle->end () ; 
      ++jetIt )
 {
  if (jetIt->eta () > etaMax ||
      jetIt->eta () < etaMin ||
      jetIt->et () < m_jetPtMin) continue ;
  ++counter ; 
 }

 return counter;
}




// --------------------------------------------------------------------

int VBF_BK_Analyzer::VBFJetVetoFilterValue (edm::Handle<reco::CaloJetCollection>& jetTagsHandle){
 
 //---- basato su: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Bicocca/HiggsAnalysis/VBFHiggsToWW2e/src/VBFJetVetoFilter.cc?revision=1.1&view=markup ----//

 int counter = 0 ;

 typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
 for (VBFjetIt jetIt = jetTagsHandle->begin () ; 
      jetIt != jetTagsHandle->end () ; 
      ++jetIt )
 {
  if (fabs (jetIt->eta ()) > m_jetEtaMax || 
      jetIt->et () < m_jetPtMin) continue ;
//   if (jetIt->et () < m_jetPtMin) continue ;
  
  ++counter ; 
 }

 return counter;
}






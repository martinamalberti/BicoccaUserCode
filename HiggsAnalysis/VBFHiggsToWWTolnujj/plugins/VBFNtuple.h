#ifndef VBFNtuple_h
#define VBFNtuple_h

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"
#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumper.h"
#include "HiggsAnalysis/VBFHiggsToVV/interface/MCUtils.h"
#include "HiggsAnalysis/VBFHiggsToVV/interface/VBFUtils.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"

#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"

#define MCEMAX 25
#define MCMUMAX 25
#define GENJETMAX 100

#define RECOEMAX 15
#define RECOMUMAX 15
#define RECOJETMAX 50






template <class TCollection>
class VBFNtuple : public edm::EDAnalyzer
{
 public:
  
  typedef reco::GsfElectronRef electron;
  typedef reco::MuonRef muon;
  typedef edm::Ref<TCollection> jet;
  
  
  
 public:
 
  //!ctor
  VBFNtuple(const edm::ParameterSet& iConfig);
  
  //!dtor
  ~VBFNtuple();
  
  void beginJob(const edm::EventSetup&);
  void endJob();
  
  void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcGenParticles;
  edm::InputTag m_srcGenJets;
  edm::InputTag m_srcRecoJets;
  edm::InputTag m_srcElectrons;
  edm::InputTag m_srcElectronTkIsoValues;
  edm::InputTag m_srcElectronEmIsoValues;
  edm::InputTag m_srcElectronHadIsoValues;
  edm::InputTag m_srcElectronIdLooseValues;
  edm::InputTag m_srcElectronIdTightValues;
  edm::InputTag m_srcElectronIdRobustLooseValues;
  edm::InputTag m_srcElectronIdRobustTightValues;
  edm::InputTag m_srcMuons;
  edm::InputTag m_srcGenMetNoNuBSM;
  edm::InputTag m_srcCaloMet;
  edm::InputTag m_srcMuonCorrectedCaloMet;
  edm::InputTag m_srcPFMet;
  
  std::string m_fileName;
  bool m_verbosity;
  
  int m_eventType;
  int m_eventId;

  bool m_doElectronRefCheck;
  edm::InputTag m_srcElectronsRef;
  bool m_doMuonRefCheck;
  edm::InputTag m_srcMuonsRef;
  bool m_doRecoJetRefCheck;
  edm::InputTag m_srcRecoJetsRef;
  
  
  TFile* m_outFile;
  TTree* m_tree;
  
  
  VBFNtupleContent m_treeVars;
  
};






template <class TCollection>
VBFNtuple<TCollection>::VBFNtuple(const edm::ParameterSet& iConfig):
 m_srcGenParticles               (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 m_srcGenJets                    (iConfig.getParameter<edm::InputTag>("srcGenJets")),
 m_srcRecoJets                   (iConfig.getParameter<edm::InputTag>("srcRecoJets")),
 m_srcElectrons                  (iConfig.getParameter<edm::InputTag>("srcElectrons")),
 m_srcElectronTkIsoValues        (iConfig.getParameter<edm::InputTag>("srcElectronTkIsoValues")),
 m_srcElectronEmIsoValues        (iConfig.getParameter<edm::InputTag>("srcElectronEmIsoValues")),
 m_srcElectronHadIsoValues       (iConfig.getParameter<edm::InputTag>("srcElectronHadIsoValues")),
 m_srcElectronIdLooseValues      (iConfig.getParameter<edm::InputTag>("srcElectronIdLooseValues")),
 m_srcElectronIdTightValues      (iConfig.getParameter<edm::InputTag>("srcElectronIdTightValues")),
 m_srcElectronIdRobustLooseValues(iConfig.getParameter<edm::InputTag>("srcElectronIdRobustLooseValues")),
 m_srcElectronIdRobustTightValues(iConfig.getParameter<edm::InputTag>("srcElectronIdRobustTightValues")),
 m_srcMuons                      (iConfig.getParameter<edm::InputTag>("srcMuons")),
 m_srcGenMetNoNuBSM              (iConfig.getParameter<edm::InputTag>("srcGenMetNoNuBSM")),
 m_srcCaloMet                    (iConfig.getParameter<edm::InputTag>("srcCaloMet")),
 m_srcMuonCorrectedCaloMet       (iConfig.getParameter<edm::InputTag>("srcMuonCorrectedCaloMet")),
 m_srcPFMet                      (iConfig.getParameter<edm::InputTag>("srcPFMet")),
 m_fileName (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFNtuple"))),
 m_verbosity(iConfig.getUntrackedParameter<bool>("verbosity", true)),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType", 0)),
 m_eventId  (0),
 m_doElectronRefCheck(iConfig.getParameter<bool>("doElectronRefCheck")),
 m_srcElectronsRef   (iConfig.getParameter<edm::InputTag>("srcElectronsRef")),
 m_doMuonRefCheck    (iConfig.getParameter<bool>("doMuonRefCheck")),
 m_srcMuonsRef       (iConfig.getParameter<edm::InputTag>("srcMuonsRef")),
 m_doRecoJetRefCheck (iConfig.getParameter<bool>("doRecoJetRefCheck")),
 m_srcRecoJetsRef    (iConfig.getParameter<edm::InputTag>("srcRecoJetsRef"))

{
  // Create File
  m_outFile = new TFile(m_fileName.c_str() , "RECREATE");
  m_outFile -> cd();
  
  
  // Initialize Tree
  m_tree = new TTree("ntu", "ntu");
  SetBranches(m_tree, m_treeVars);
}

// ----------------------------------------------------------------------------






template <class TCollection>
VBFNtuple<TCollection>::~VBFNtuple()
{}

// ----------------------------------------------------------------------------






template <class TCollection>
void VBFNtuple<TCollection>::beginJob(const edm::EventSetup& iSetup)
{
  if(m_verbosity)
    std::cerr << "*** VBFNtuple::beginJob ***" << std::endl;
}

// ----------------------------------------------------------------------------






template <class TCollection>
void VBFNtuple<TCollection>::endJob()
{
  if(m_verbosity)
    std::cerr << "*** VBFNtuple::endJob ***" << std::endl;
  
  m_outFile -> cd();
  m_tree -> Write();
  m_outFile -> Close();
}

// ----------------------------------------------------------------------------






template <class TCollection>
void VBFNtuple<TCollection>::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++m_eventId;
  
  if( (m_eventId%1000 == 0) && (m_verbosity == true) )
    std::cout << ">>>>>> VBFNtuple::Event number -----> " << std::fixed << std::setw(10) << m_eventId << std::endl;
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(m_srcGenParticles, genParticles);
  if( !(genParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcGenParticles << " not available" << std::endl;
    return;
  }
  
  
  
  
  
  
  edm::Handle<reco::GenJetCollection> genJets;
  iEvent.getByLabel(m_srcGenJets, genJets);
  if( !(genJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcGenJets << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<TCollection> recoJets;
  iEvent.getByLabel(m_srcRecoJets, recoJets);
  if( !(recoJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcRecoJets << " not available" << std::endl;
    return;
  }  
  
  edm::Handle< edm::RefVector<TCollection> > recoJetsRef;
  if(m_doRecoJetRefCheck)
    iEvent.getByLabel(m_srcRecoJetsRef, recoJetsRef);  
  
  
  
  
  
  
  edm::Handle<reco::PixelMatchGsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  if( !(electrons.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectrons << " not available" << std::endl;
    return;
  }
  
  edm::Handle< edm::RefVector<reco::PixelMatchGsfElectronCollection> > electronsRef;
  if(m_doElectronRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);
  
  
  
  edm::Handle<edm::ValueMap<double> > electronTkIsoValues;
  iEvent.getByLabel(m_srcElectronTkIsoValues, electronTkIsoValues);
  if( !(electronTkIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectronTkIsoValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > electronEmIsoValues;
  iEvent.getByLabel(m_srcElectronEmIsoValues, electronEmIsoValues);
  if( !(electronEmIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectronEmIsoValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > electronHadIsoValues;
  iEvent.getByLabel(m_srcElectronHadIsoValues, electronHadIsoValues);
  if( !(electronHadIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectronHadIsoValues << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<edm::ValueMap<float> > electronIdLooseValues;
  iEvent.getByLabel(m_srcElectronIdLooseValues, electronIdLooseValues);
  if( !(electronIdLooseValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectronIdLooseValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdTightValues;
  iEvent.getByLabel(m_srcElectronIdTightValues, electronIdTightValues);
  if( !(electronIdTightValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectronIdTightValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdRobustLooseValues;
  iEvent.getByLabel(m_srcElectronIdRobustLooseValues, electronIdRobustLooseValues);
  if( !(electronIdRobustLooseValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectronIdRobustLooseValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdRobustTightValues;
  iEvent.getByLabel(m_srcElectronIdRobustTightValues, electronIdRobustTightValues);
  if( !(electronIdRobustTightValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcElectronIdRobustTightValues << " not available" << std::endl;
    return;
  }
  
  
  
  
  
  
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel (m_srcMuons, muons);
  if( !(muons.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcMuons << " not available" << std::endl;
    return;
  } 
  
  edm::Handle< edm::RefVector<reco::MuonCollection> > muonsRef;
  if(m_doMuonRefCheck)
    iEvent.getByLabel(m_srcMuonsRef, muonsRef); 
  
  
  
  
  
  
  edm::Handle<reco::GenMETCollection> genMetNoNuBSM;
  iEvent.getByLabel(m_srcGenMetNoNuBSM, genMetNoNuBSM);
  if( !(genMetNoNuBSM.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcGenMetNoNuBSM << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::CaloMETCollection> caloMet;
  iEvent.getByLabel(m_srcCaloMet, caloMet);
  if( !(caloMet.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcCaloMet << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::CaloMETCollection> muonCorrectedCaloMet;
  iEvent.getByLabel(m_srcMuonCorrectedCaloMet, muonCorrectedCaloMet);
  if( !(muonCorrectedCaloMet.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcMuonCorrectedCaloMet << " not available" << std::endl;
    return;
  }
  
  //edm::Handle<reco::PFMETCollection> PFMet;
  //iEvent.getByLabel(m_srcPFMet, PFMet);
  //if( !(PFMet.isValid ()) )
  //{
  //  std::cerr << ">>>>>> VBFNtuple::Warning: " << m_srcPFMet << " not available" << std::endl;
  //  return;
  //}
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the tree
  // -----------------------------------------------------------------  

  // Initialize tree branches
  InitializeBranches(m_tree, m_treeVars);
  
  
  
  m_treeVars.eventType = m_eventType;
  m_treeVars.eventId   = m_eventId;
  
  
    
  // MCDumper
  MCDumper mcAnalysis(genParticles, m_eventType, m_verbosity);
  bool isValid = mcAnalysis.isValid();
  
  if( (m_eventType == 0) && (isValid == true) )
  {
    const reco::Candidate* mcH = mcAnalysis.mcH();
    
    std::vector<const reco::Candidate*> mcW(2);
    mcW.at(0) = mcAnalysis.mcV1();
    mcW.at(1) = mcAnalysis.mcV2();
    
    std::vector<const reco::Candidate*> mcL_fromW1(2);
    mcL_fromW1.at(0) = mcAnalysis.mcF1_fromV1();
    mcL_fromW1.at(1) = mcAnalysis.mcF2_fromV1();
    
    std::vector<const reco::Candidate*> mcQ_fromW2(2);
    mcQ_fromW2.at(0) = mcAnalysis.mcF1_fromV2();
    mcQ_fromW2.at(1) = mcAnalysis.mcF2_fromV2();
    
    std::vector<const reco::Candidate*> mcQ_tag(2);
    mcQ_tag.at(0) = mcAnalysis.mcQ1_tag();
    mcQ_tag.at(1) = mcAnalysis.mcQ2_tag();
    
    
    
    
    m_treeVars.mcH_energy = mcH -> energy();
    m_treeVars.mcH_px     = mcH -> px();
    m_treeVars.mcH_py     = mcH -> py();
    m_treeVars.mcH_pz     = mcH -> pz();
    m_treeVars.mcH_p      = mcH -> p();
    m_treeVars.mcH_pt     = mcH -> pt();
    m_treeVars.mcH_et     = mcH -> et();
    m_treeVars.mcH_mass   = mcH -> mass();
    m_treeVars.mcH_phi    = mcH -> phi();
    m_treeVars.mcH_eta    = mcH -> eta();
    m_treeVars.mcH_charge = mcH -> charge();
    
    
    
    for(unsigned int i = 0; i < 2; ++i)
    {
      m_treeVars.mcW_energy[i] = mcW.at(i) -> energy();
      m_treeVars.mcW_px[i]     = mcW.at(i) -> px();
      m_treeVars.mcW_py[i]     = mcW.at(i) -> py();
      m_treeVars.mcW_pz[i]     = mcW.at(i) -> pz();
      m_treeVars.mcW_p[i]      = mcW.at(i) -> p();
      m_treeVars.mcW_pt[i]     = mcW.at(i) -> pt();
      m_treeVars.mcW_et[i]     = mcW.at(i) -> et();
      m_treeVars.mcW_mass[i]   = mcW.at(i) -> mass();
      m_treeVars.mcW_phi[i]    = mcW.at(i) -> phi();
      m_treeVars.mcW_eta[i]    = mcW.at(i) -> eta();
      m_treeVars.mcW_charge[i] = mcW.at(i) -> charge();
      
      m_treeVars.mcF_fromW1_energy[i] = mcL_fromW1.at(i) -> energy();
      m_treeVars.mcF_fromW1_px[i]     = mcL_fromW1.at(i) -> px();
      m_treeVars.mcF_fromW1_py[i]     = mcL_fromW1.at(i) -> py();
      m_treeVars.mcF_fromW1_pz[i]     = mcL_fromW1.at(i) -> pz();
      m_treeVars.mcF_fromW1_p[i]      = mcL_fromW1.at(i) -> p();
      m_treeVars.mcF_fromW1_pt[i]     = mcL_fromW1.at(i) -> pt();
      m_treeVars.mcF_fromW1_et[i]     = mcL_fromW1.at(i) -> et();
      m_treeVars.mcF_fromW1_mass[i]   = mcL_fromW1.at(i) -> mass();
      m_treeVars.mcF_fromW1_phi[i]    = mcL_fromW1.at(i) -> phi();
      m_treeVars.mcF_fromW1_eta[i]    = mcL_fromW1.at(i) -> eta();
      m_treeVars.mcF_fromW1_charge[i] = mcL_fromW1.at(i) -> charge();
      m_treeVars.mcF_fromW1_pdgId[i]  = mcL_fromW1.at(i) -> pdgId();
      
      m_treeVars.mcQ_fromW2_energy[i] = mcQ_fromW2.at(i) -> energy();
      m_treeVars.mcQ_fromW2_px[i]     = mcQ_fromW2.at(i) -> px();
      m_treeVars.mcQ_fromW2_py[i]     = mcQ_fromW2.at(i) -> py();
      m_treeVars.mcQ_fromW2_pz[i]     = mcQ_fromW2.at(i) -> pz();
      m_treeVars.mcQ_fromW2_p[i]      = mcQ_fromW2.at(i) -> p();
      m_treeVars.mcQ_fromW2_pt[i]     = mcQ_fromW2.at(i) -> pt();
      m_treeVars.mcQ_fromW2_et[i]     = mcQ_fromW2.at(i) -> et();
      m_treeVars.mcQ_fromW2_mass[i]   = mcQ_fromW2.at(i) -> mass();
      m_treeVars.mcQ_fromW2_phi[i]    = mcQ_fromW2.at(i) -> phi();
      m_treeVars.mcQ_fromW2_eta[i]    = mcQ_fromW2.at(i) -> eta();
      m_treeVars.mcQ_fromW2_charge[i] = mcQ_fromW2.at(i) -> charge();
      m_treeVars.mcQ_fromW2_pdgId[i]  = mcQ_fromW2.at(i) -> pdgId();
      
      m_treeVars.mcQ_tag_energy[i] = mcQ_tag.at(i) -> energy();
      m_treeVars.mcQ_tag_px[i]     = mcQ_tag.at(i) -> px();
      m_treeVars.mcQ_tag_py[i]     = mcQ_tag.at(i) -> py();
      m_treeVars.mcQ_tag_pz[i]     = mcQ_tag.at(i) -> pz();
      m_treeVars.mcQ_tag_p[i]      = mcQ_tag.at(i) -> p();
      m_treeVars.mcQ_tag_pt[i]     = mcQ_tag.at(i) -> pt();
      m_treeVars.mcQ_tag_et[i]     = mcQ_tag.at(i) -> et();
      m_treeVars.mcQ_tag_mass[i]   = mcQ_tag.at(i) -> mass();
      m_treeVars.mcQ_tag_phi[i]    = mcQ_tag.at(i) -> phi();
      m_treeVars.mcQ_tag_eta[i]    = mcQ_tag.at(i) -> eta();
      m_treeVars.mcQ_tag_charge[i] = mcQ_tag.at(i) -> charge();
      m_treeVars.mcQ_tag_pdgId[i]  = mcQ_tag.at(i) -> pdgId();
    }  
    
    
        
    // Matching between genJets and quarks
    std::vector<reco::GenJetCollection::const_iterator> genJetsIt;
    for(reco::GenJetCollection::const_iterator jetIt = genJets -> begin();
        jetIt != genJets -> end(); ++jetIt)
      genJetsIt.push_back(jetIt);   
    
    std::vector<const reco::Candidate*> mcQuarksIt;
    mcQuarksIt.push_back(mcQ_tag.at(0));
    mcQuarksIt.push_back(mcQ_tag.at(1));
    mcQuarksIt.push_back(mcQ_fromW2.at(0));
    mcQuarksIt.push_back(mcQ_fromW2.at(1));
    
    std::vector<int> matchGenJetIt;
    GetMatching<reco::GenJetCollection::const_iterator, const reco::Candidate*>(genJetsIt, mcQuarksIt, 0.3, false, 0., 100., &matchGenJetIt);
    
    
    if(matchGenJetIt.at(0) != -1) 
    {
      //std::cout << "TagQ1:   jetIt = " << matchGenJetIt.at(0) << std::endl;
      m_treeVars.genJet_tag_isMatching[0] = 1;
      m_treeVars.genJet_tag_it[0] = matchGenJetIt.at(0);
    }
    
    if(matchGenJetIt.at(1) != -1) 
    {
      //std::cout << "TagQ2:   jetIt = " << matchGenJetIt.at(1) << std::endl;
      m_treeVars.genJet_tag_isMatching[1] = 1;
      m_treeVars.genJet_tag_it[1] = matchGenJetIt.at(1);
    }
    
    if(matchGenJetIt.at(2) != -1) 
    {
      //std::cout << "Q1_fromW2:   jetIt = " << matchGenJetIt.at(2) << std::endl;
      m_treeVars.genJet_fromW2_isMatching[0] = 1;
      m_treeVars.genJet_fromW2_it[0] = matchGenJetIt.at(2);
    }
    
    if(matchGenJetIt.at(3) != -1) 
    {
      //std::cout << "Q2_fromW2:   jetIt = " << matchGenJetIt.at(3) << std::endl;
      m_treeVars.genJet_fromW2_isMatching[1] = 1;
      m_treeVars.genJet_fromW2_it[1] = matchGenJetIt.at(3);
    }
    
  }
  
  
  
  
    
  
  // mc electrons
  m_treeVars.nMcE = 0;
  for(unsigned eleIt = 0; eleIt < (mcAnalysis.GetMcE()).size(); ++eleIt)
  {
    if(m_treeVars.nMcE >= MCEMAX) break;
    
    m_treeVars.mcE_energy[m_treeVars.nMcE] = ((mcAnalysis.GetMcE()).at(eleIt)) -> energy();
    m_treeVars.mcE_px[m_treeVars.nMcE]     = ((mcAnalysis.GetMcE()).at(eleIt)) -> px();
    m_treeVars.mcE_py[m_treeVars.nMcE]     = ((mcAnalysis.GetMcE()).at(eleIt)) -> py();
    m_treeVars.mcE_pz[m_treeVars.nMcE]     = ((mcAnalysis.GetMcE()).at(eleIt)) -> pz();
    m_treeVars.mcE_p[m_treeVars.nMcE]      = ((mcAnalysis.GetMcE()).at(eleIt)) -> p();
    m_treeVars.mcE_pt[m_treeVars.nMcE]     = ((mcAnalysis.GetMcE()).at(eleIt)) -> pt();
    m_treeVars.mcE_et[m_treeVars.nMcE]     = ((mcAnalysis.GetMcE()).at(eleIt)) -> et();
    m_treeVars.mcE_mass[m_treeVars.nMcE]   = ((mcAnalysis.GetMcE()).at(eleIt)) -> mass();
    m_treeVars.mcE_phi[m_treeVars.nMcE]    = ((mcAnalysis.GetMcE()).at(eleIt)) -> phi();
    m_treeVars.mcE_eta[m_treeVars.nMcE]    = ((mcAnalysis.GetMcE()).at(eleIt)) -> eta();
    m_treeVars.mcE_charge[m_treeVars.nMcE] = ((mcAnalysis.GetMcE()).at(eleIt)) -> charge();
    
    ++m_treeVars.nMcE;
  }  
  
  // reco electrons
  m_treeVars.nRecoE = 0;
  for(unsigned int i = 0; i < electrons -> size(); ++i)
  {
    // do the reference check 
    bool isElectronRefCheckOk = true;
    electron electronRef(electrons, i);
    if(m_doElectronRefCheck)
      if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
        isElectronRefCheckOk = false;
    
    if(!isElectronRefCheckOk) continue;
    
    if(m_treeVars.nRecoE >= RECOEMAX) break;
    
    m_treeVars.recoE_energy[m_treeVars.nRecoE] = electronRef -> energy();
    m_treeVars.recoE_px[m_treeVars.nRecoE]     = electronRef -> px();
    m_treeVars.recoE_py[m_treeVars.nRecoE]     = electronRef -> py();
    m_treeVars.recoE_pz[m_treeVars.nRecoE]     = electronRef -> pz();
    m_treeVars.recoE_p[m_treeVars.nRecoE]      = electronRef -> p();
    m_treeVars.recoE_pt[m_treeVars.nRecoE]     = electronRef -> pt();
    m_treeVars.recoE_et[m_treeVars.nRecoE]     = electronRef -> et();
    m_treeVars.recoE_mass[m_treeVars.nRecoE]   = electronRef -> mass();
    m_treeVars.recoE_phi[m_treeVars.nRecoE]    = electronRef -> phi();
    m_treeVars.recoE_eta[m_treeVars.nRecoE]    = electronRef -> eta();
    m_treeVars.recoE_charge[m_treeVars.nRecoE] = electronRef -> charge();
    
    m_treeVars.recoE_tkIso[m_treeVars.nRecoE]  =  (*electronTkIsoValues)[electronRef];
    m_treeVars.recoE_emIso[m_treeVars.nRecoE]  =  (*electronEmIsoValues)[electronRef];
    m_treeVars.recoE_hadIso[m_treeVars.nRecoE] = (*electronHadIsoValues)[electronRef];
    
    m_treeVars.recoE_eleIdLoose[m_treeVars.nRecoE]       =       (*electronIdLooseValues)[electronRef];
    m_treeVars.recoE_eleIdTight[m_treeVars.nRecoE]       =       (*electronIdTightValues)[electronRef];
    m_treeVars.recoE_eleIdRobustLoose[m_treeVars.nRecoE] = (*electronIdRobustLooseValues)[electronRef];
    m_treeVars.recoE_eleIdRobustTight[m_treeVars.nRecoE] = (*electronIdRobustTightValues)[electronRef];
    
    ++m_treeVars.nRecoE;
  }
  
    
  
  
    
  // mc muons
  m_treeVars.nMcMu = 0;
  for(unsigned muIt = 0; muIt < (mcAnalysis.GetMcMu()).size(); ++muIt)
  {
    if(m_treeVars.nMcMu >= MCMUMAX) break;
    
    m_treeVars.mcMu_energy[m_treeVars.nMcMu] = ((mcAnalysis.GetMcMu()).at(muIt)) -> energy();
    m_treeVars.mcMu_px[m_treeVars.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> px();
    m_treeVars.mcMu_py[m_treeVars.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> py();
    m_treeVars.mcMu_pz[m_treeVars.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> pz();
    m_treeVars.mcMu_p[m_treeVars.nMcMu]      = ((mcAnalysis.GetMcMu()).at(muIt)) -> p();
    m_treeVars.mcMu_pt[m_treeVars.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> pt();
    m_treeVars.mcMu_et[m_treeVars.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> et();
    m_treeVars.mcMu_mass[m_treeVars.nMcMu]   = ((mcAnalysis.GetMcMu()).at(muIt)) -> mass();
    m_treeVars.mcMu_phi[m_treeVars.nMcMu]    = ((mcAnalysis.GetMcMu()).at(muIt)) -> phi();
    m_treeVars.mcMu_eta[m_treeVars.nMcMu]    = ((mcAnalysis.GetMcMu()).at(muIt)) -> eta();
    m_treeVars.mcMu_charge[m_treeVars.nMcMu] = ((mcAnalysis.GetMcMu()).at(muIt)) -> charge();
    
    ++m_treeVars.nMcMu;
  }  
  
  // reco muons
  m_treeVars.nRecoMu = 0;
  for(unsigned int i = 0; i < muons -> size(); ++i)
  {
    // do the reference check 
    bool isMuonRefCheckOk = true;
    muon muonRef(muons, i);
    if(m_doMuonRefCheck)
      if(find(muonsRef -> begin(), muonsRef -> end(), muonRef) == muonsRef -> end())
        isMuonRefCheckOk = false;
    
    if(!isMuonRefCheckOk) continue;

    if(!muonRef -> isGlobalMuon()) continue;
    if(m_treeVars.nRecoMu >= RECOMUMAX) break;
    
    m_treeVars.recoMu_energy[m_treeVars.nRecoMu] = muonRef -> energy();
    m_treeVars.recoMu_px[m_treeVars.nRecoMu]     = muonRef -> px();
    m_treeVars.recoMu_py[m_treeVars.nRecoMu]     = muonRef -> py();
    m_treeVars.recoMu_pz[m_treeVars.nRecoMu]     = muonRef -> pz();
    m_treeVars.recoMu_p[m_treeVars.nRecoMu]      = muonRef -> p();
    m_treeVars.recoMu_pt[m_treeVars.nRecoMu]     = muonRef -> pt();
    m_treeVars.recoMu_et[m_treeVars.nRecoMu]     = muonRef -> et();
    m_treeVars.recoMu_mass[m_treeVars.nRecoMu]   = muonRef -> mass();
    m_treeVars.recoMu_phi[m_treeVars.nRecoMu]    = muonRef -> phi();
    m_treeVars.recoMu_eta[m_treeVars.nRecoMu]    = muonRef -> eta();
    m_treeVars.recoMu_charge[m_treeVars.nRecoMu] = muonRef -> charge();
    
    m_treeVars.recoMu_tkIsoR03[m_treeVars.nRecoMu]      = (muonRef -> isolationR03()).sumPt;
    m_treeVars.recoMu_nTkIsoR03[m_treeVars.nRecoMu]     = (muonRef -> isolationR03()).nTracks;    
    m_treeVars.recoMu_emIsoR03[m_treeVars.nRecoMu]      = (muonRef -> isolationR03()).emEt;
    m_treeVars.recoMu_hadIsoR03[m_treeVars.nRecoMu]     = (muonRef -> isolationR03()).hadEt;
    
    m_treeVars.recoMu_tkIsoR05[m_treeVars.nRecoMu]      = (muonRef -> isolationR05()).sumPt;
    m_treeVars.recoMu_nTkIsoR05[m_treeVars.nRecoMu]     = (muonRef -> isolationR05()).nTracks;    
    m_treeVars.recoMu_emIsoR05[m_treeVars.nRecoMu]      = (muonRef -> isolationR05()).emEt;
    m_treeVars.recoMu_hadIsoR05[m_treeVars.nRecoMu]     = (muonRef -> isolationR05()).hadEt;

    ++m_treeVars.nRecoMu;
  }
  
  
  
  // mc jets
  m_treeVars.nGenJet = 0;
  for(reco::GenJetCollection::const_iterator jetIt = genJets -> begin();
      jetIt != genJets -> end(); ++jetIt)
  {
    if(m_treeVars.nGenJet >= GENJETMAX) break;
    
    m_treeVars.genJet_energy[m_treeVars.nGenJet] = jetIt -> energy();
    m_treeVars.genJet_px[m_treeVars.nGenJet]     = jetIt -> px();
    m_treeVars.genJet_py[m_treeVars.nGenJet]     = jetIt -> py();
    m_treeVars.genJet_pz[m_treeVars.nGenJet]     = jetIt -> pz();
    m_treeVars.genJet_p[m_treeVars.nGenJet]      = jetIt -> p();
    m_treeVars.genJet_pt[m_treeVars.nGenJet]     = jetIt -> pt();
    m_treeVars.genJet_et[m_treeVars.nGenJet]     = jetIt -> et();
    m_treeVars.genJet_mass[m_treeVars.nGenJet]   = jetIt -> mass();
    m_treeVars.genJet_phi[m_treeVars.nGenJet]    = jetIt -> phi();
    m_treeVars.genJet_eta[m_treeVars.nGenJet]    = jetIt -> eta();
    m_treeVars.genJet_charge[m_treeVars.nGenJet] = jetIt -> charge();
    
    ++m_treeVars.nGenJet;
  }
  
  
  // reco jets
  m_treeVars.nRecoJet = 0;
  for(unsigned int i = 0; i < recoJets -> size(); ++i)
  {
    // do the reference check 
    bool isJetRefCheckOk = true;
    jet recoJetRef(recoJets, i);
    if(m_doRecoJetRefCheck)
      if(find(recoJetsRef -> begin(), recoJetsRef -> end(), recoJetRef) == recoJetsRef -> end())
        isJetRefCheckOk = false;
    
    if(!isJetRefCheckOk) continue;
    
    if(m_treeVars.nRecoJet >= RECOJETMAX) break;
    
    m_treeVars.recoJet_energy[m_treeVars.nRecoJet] = recoJetRef -> energy();
    m_treeVars.recoJet_px[m_treeVars.nRecoJet]     = recoJetRef -> px();
    m_treeVars.recoJet_py[m_treeVars.nRecoJet]     = recoJetRef -> py();
    m_treeVars.recoJet_pz[m_treeVars.nRecoJet]     = recoJetRef -> pz();
    m_treeVars.recoJet_p[m_treeVars.nRecoJet]      = recoJetRef -> p();
    m_treeVars.recoJet_pt[m_treeVars.nRecoJet]     = recoJetRef -> pt();
    m_treeVars.recoJet_et[m_treeVars.nRecoJet]     = recoJetRef -> et();
    m_treeVars.recoJet_mass[m_treeVars.nRecoJet]   = recoJetRef -> mass();
    m_treeVars.recoJet_phi[m_treeVars.nRecoJet]    = recoJetRef -> phi();
    m_treeVars.recoJet_eta[m_treeVars.nRecoJet]    = recoJetRef -> eta();
    m_treeVars.recoJet_charge[m_treeVars.nRecoJet] = recoJetRef -> charge();
    
    ++m_treeVars.nRecoJet;
  }
  
    
  
  
  
  
  // gen met
  reco::GenMETCollection::const_iterator genMetNoNuBSMIt = genMetNoNuBSM -> begin();
  m_treeVars.genMetNoNuBSM   = genMetNoNuBSMIt -> pt();
  m_treeVars.genMetNoNuBSM_x = genMetNoNuBSMIt -> px();
  m_treeVars.genMetNoNuBSM_y = genMetNoNuBSMIt -> py();
  
  // calo met
  reco::CaloMETCollection::const_iterator caloMetIt = caloMet -> begin();
  m_treeVars.caloMet   = caloMetIt -> pt();
  m_treeVars.caloMet_x = caloMetIt -> px();
  m_treeVars.caloMet_y = caloMetIt -> py();
  
  reco::CaloMETCollection::const_iterator muonCorrectedCaloMetIt = muonCorrectedCaloMet -> begin();
  m_treeVars.muonCorrectedCaloMet   = muonCorrectedCaloMetIt -> pt();
  m_treeVars.muonCorrectedCaloMet_x = muonCorrectedCaloMetIt -> px();
  m_treeVars.muonCorrectedCaloMet_y = muonCorrectedCaloMetIt -> py();
  
  //reco::PFMETCollection::const_iterator PFMetIt = PFMet -> begin();
  //m_treeVars.PFMet   = PFMetIt -> pt();
  //m_treeVars.PFMet_x = PFMetIt -> px();
  //m_treeVars.PFMet_y = PFMetIt -> py();
  
  
  
  m_tree -> Fill();
}


#endif

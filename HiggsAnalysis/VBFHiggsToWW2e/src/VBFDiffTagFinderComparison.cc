// $Id: VBFDiffTagFinderComparison.cc,v 1.5 2008/02/25 17:03:58 tancini Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFDiffTagFinderComparison.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"
#include <Math/VectorUtil.h>

VBFDiffTagFinderComparison::VBFDiffTagFinderComparison (const edm::ParameterSet& iConfig) :
      m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")),
      m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")) ,
      m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")),
      m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax"))
 {}


// --------------------------------------------------------------------


VBFDiffTagFinderComparison::~VBFDiffTagFinderComparison ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


// --------------------------------------------------------------------


void
VBFDiffTagFinderComparison::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle);
  if (jetCollectionHandle->size () < 2) return ;

  edm::Handle<CandidateCollection> genParticles;
  iEvent.getByLabel(m_MCtruthInputTag, genParticles);
       
  findGenParticles (genParticles, *m_genqTagF, *m_genqTagB);

  double deltaRF;
  double deltaRB;
  double threshold = 0.3;


  std::pair<VBFjetIt,VBFjetIt> tagJetCandsMaxMinv = findTagJets (jetCollectionHandle->begin (), jetCollectionHandle->end (), m_jetPtMin, m_jetEtaMax) ;
  if (tagJetCandsMaxMinv.first->p4().Eta() > tagJetCandsMaxMinv.second->p4().Eta())
    {
      ///////////////// ************ resolution ***********************************************                                                                       
      deltaRF = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.first->momentum(), m_genqTagF->Vect()) ;
      m_deltaR_CandsMaxMinv -> Fill (deltaRF) ; 
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.first->p4().E()/m_genqTagF->E()) ;

      deltaRB = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.second->momentum(), m_genqTagB->Vect()) ;
      m_deltaR_CandsMaxMinv -> Fill (deltaRB) ;
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.second->p4().E()/m_genqTagB->E()) ;
	  
	  ///////////////// ************ purity with inv mass selection ***********************************************

      if (deltaRF < threshold && deltaRB > threshold) m_purityHisto_mInv-> Fill (1) ; // 1 matching
      else if (deltaRB < threshold && deltaRF > threshold) m_purityHisto_mInv-> Fill (1) ; // 1 matching       
      else if (deltaRB < threshold && deltaRF < threshold) m_purityHisto_mInv -> Fill (2) ; // 2 matching
      else if (deltaRB > threshold && deltaRF > threshold) m_purityHisto_mInv-> Fill (0) ; // 0 matching


    }
  else if (tagJetCandsMaxMinv.first->p4().Eta() < tagJetCandsMaxMinv.second->p4().Eta()) 
    {
      ///////////////// ************ resolution ***********************************************                                                                      

      deltaRB = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.first->momentum(), m_genqTagB->Vect()) ;
      m_deltaR_CandsMaxMinv -> Fill (deltaRB) ;
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.first->p4().E()/m_genqTagB->E()) ;

      deltaRF = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.second->momentum(), m_genqTagF->Vect()) ;
      m_deltaR_CandsMaxMinv -> Fill (deltaRF) ;
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.second->p4().E()/m_genqTagF->E()) ;
	  
	  ///////////////// ************ purity with inv mass selection ***********************************************

      if (deltaRF < threshold && deltaRB > threshold) m_purityHisto_mInv-> Fill (1) ; // 1 matching
      else if (deltaRB < threshold && deltaRF > threshold) m_purityHisto_mInv-> Fill (1) ; // 1 matching       
      else if (deltaRB < threshold && deltaRF < threshold) m_purityHisto_mInv -> Fill (2) ; // 2 matching
      else if (deltaRB > threshold && deltaRF > threshold) m_purityHisto_mInv-> Fill (0) ; // 0 matching
    }

///////////////////////////////////////////////////////////////// pt based selection

  reco::CaloJetCollection TheJets = *jetCollectionHandle;
  std::pair<VBFjetIt,VBFjetIt> tagJetCandsMaxPt = findMaxPtJetsPair (TheJets, m_jetPtMin, m_jetEtaMax) ;


  if (tagJetCandsMaxPt.first->p4().Eta() > tagJetCandsMaxPt.second->p4().Eta())
    {
      ///////////////// ************ resolution ***********************************************
      
      deltaRF = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.first->momentum(), m_genqTagF->Vect()) ;
      m_deltaR_CandsMaxPt -> Fill (deltaRF) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.first->p4().E()/m_genqTagF->E()) ;

      deltaRB = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.second->momentum(), m_genqTagB->Vect()) ;
      m_deltaR_CandsMaxPt -> Fill (deltaRB ) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.second->p4().E()/m_genqTagB->E()) ;

      ///////////////// ************ purity with inv mass selection ***********************************************

      if (deltaRF < threshold && deltaRB > threshold) m_purityHisto_pt-> Fill (1) ; // 1 matching
      else if (deltaRB < threshold && deltaRF > threshold) m_purityHisto_pt-> Fill (1) ; // 1 matching       
      else if (deltaRB < threshold && deltaRF < threshold) m_purityHisto_pt -> Fill (2) ; // 2 matching
      else if (deltaRB > threshold && deltaRF > threshold) m_purityHisto_pt-> Fill (0) ; // 0 matching
    }

  else if (tagJetCandsMaxPt.first->p4().Eta() < tagJetCandsMaxPt.second->p4().Eta())
    {
	  ///////////////// ************ resolution ***********************************************

      deltaRB = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.first->momentum(), m_genqTagB->Vect()) ;
      m_deltaR_CandsMaxPt -> Fill (deltaRB) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.first->p4().E()/m_genqTagB->E()) ;

      deltaRF = ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.second->momentum(), m_genqTagF->Vect()) ;
      m_deltaR_CandsMaxPt -> Fill (deltaRF) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.second->p4().E()/m_genqTagF->E()) ;
	  
	  
      ///////////////// ************ purity with inv mass selection ***********************************************

      if (deltaRF < threshold && deltaRB > threshold) m_purityHisto_pt-> Fill (1) ; // 1 matching
      else if (deltaRB < threshold && deltaRF > threshold) m_purityHisto_pt-> Fill (1) ; // 1 matching       
      else if (deltaRB < threshold && deltaRF < threshold) m_purityHisto_pt -> Fill (2) ; // 2 matching
      else if (deltaRB > threshold && deltaRF > threshold) m_purityHisto_pt-> Fill (0) ; // 0 matching

    }

}
// --------------------------------------------------------------------


void 
VBFDiffTagFinderComparison::beginJob (const edm::EventSetup&)
{
  m_genqTagF = new TLorentzVector (0.0,0.0,0.0,0.0);
  m_genqTagB = new TLorentzVector (0.0,0.0,0.0,0.0);

  edm::Service<TFileService> fs ;

  m_deltaR_CandsMaxMinv = fs->make<TH1F> ("m_deltaR_CandsMaxMinv", "dR between jet and quark", 10000, 0, 50);
  m_Eratio_CandsMaxMinv = fs->make<TH1F> ("m_Eratio_CandsMaxMinv", "ratio between quark and jet energies", 10000, 0, 150);

  m_deltaR_CandsMaxPt = fs->make<TH1F> ("m_deltaR_CandsMaxPt", "dR between jet and quark", 10000, 0, 50);
  m_Eratio_CandsMaxPt = fs->make<TH1F> ("m_Eratio_CandsMaxPt", "ratio between quark and jet energies", 10000, 0, 150);
  
  m_purityHisto_mInv = fs->make<TH1F> ("m_purityHisto_mInv", "purity with max inv mass method", 3, 0, 3);
  m_purityHisto_pt = fs->make<TH1F> ("m_purityHisto_pt", "purity with max pt method", 3, 0, 3);

}


// --------------------------------------------------------------------


void 
VBFDiffTagFinderComparison::endJob () 
{
}

// --------------------------------------------------------------------

void VBFDiffTagFinderComparison::findGenParticles (edm::Handle<CandidateCollection> &genParticles,
				     TLorentzVector &m_genqTagF,
				     TLorentzVector &m_genqTagB)
{
  for (CandidateCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
    {
      int mumPDG = p->pdgId();
      int mumSTATUS = p->status();

      ///////////////////////////////////////////////// tag quark /////////////////////////////////////////////////
      //misteriosamente i tag sono i fratelli dell'higgs
      //quindi parto dall'higgs e ne prendo le mamme e quindi riguardo i figli
      if ((abs(mumPDG)==25) && (mumSTATUS ==3))
	{
       	  const Candidate * interact0 = p->mother(0);
	  if ((interact0->daughter(1)->eta()) > (interact0->daughter(0)->eta())) 
            {
	    setMomentum (m_genqTagF, *(interact0->daughter(1)));
	    setMomentum (m_genqTagB, *(interact0->daughter(0)));
	    }
       	  else 
            {
	    setMomentum (m_genqTagB, *(interact0->daughter(1)));
	    setMomentum (m_genqTagF, *(interact0->daughter(0)));
	    }
	}
        
    }
}

// --------------------------------------------------------------------

void VBFDiffTagFinderComparison::setMomentum (TLorentzVector & myvector, const Candidate & gen)
{
  myvector.SetPx (gen.px());
  myvector.SetPy (gen.py());
  myvector.SetPz (gen.pz());
  myvector.SetE (gen.energy());
}

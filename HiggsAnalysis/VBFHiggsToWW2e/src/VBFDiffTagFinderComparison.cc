// $Id: VBFDiffTagFinderComparison.cc,v 1.2 2008/02/20 14:59:58 tancini Exp $
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
  std::cout << "size " << jetCollectionHandle->size ()  << std::endl; 
  if (jetCollectionHandle->size () < 2) return ;

  edm::Handle<CandidateCollection> genParticles;
  iEvent.getByLabel(m_MCtruthInputTag, genParticles);
       
  findGenParticles (genParticles, *m_genqTagF, *m_genqTagB);

  std::cout << "fatto  findGenParticles" << std::endl;
  std::pair<VBFjetIt,VBFjetIt> tagJetCandsMaxMinv = findTagJets (jetCollectionHandle->begin (),
                                                          jetCollectionHandle->end (),
                                                          m_jetPtMin, m_jetEtaMax) ;

  std::cout << "A ************** eta 1 " << tagJetCandsMaxMinv.first->p4().Eta() << " eta 2 "<<  tagJetCandsMaxMinv.second->p4().Eta() << std::endl;

  if (tagJetCandsMaxMinv.first->p4().Eta() > tagJetCandsMaxMinv.second->p4().Eta())
    {
      m_deltaR_CandsMaxMinv -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.first->momentum(), m_genqTagF->Vect()) ) ; 
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.first->p4().E()/m_genqTagF->E()) ;
      std::cout << "ratioA " << (tagJetCandsMaxMinv.first->p4().E()/m_genqTagF->E())  << std::endl;
      m_deltaR_CandsMaxMinv -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.second->momentum(), m_genqTagB->Vect()) ) ;
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.second->p4().E()/m_genqTagB->E()) ;
      std::cout << "ratioA " << (tagJetCandsMaxMinv.second->p4().E()/m_genqTagB->E())  << std::endl;
    }
  else if (tagJetCandsMaxMinv.first->p4().Eta() < tagJetCandsMaxMinv.second->p4().Eta()) 
    {
      m_deltaR_CandsMaxMinv -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.first->momentum(), m_genqTagB->Vect()) ) ;
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.first->p4().E()/m_genqTagB->E()) ;
      std::cout << "ratioA " << (tagJetCandsMaxMinv.first->p4().E()/m_genqTagB->E())  << std::endl;
      m_deltaR_CandsMaxMinv -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxMinv.second->momentum(), m_genqTagF->Vect()) ) ;
      m_Eratio_CandsMaxMinv -> Fill (tagJetCandsMaxMinv.second->p4().E()/m_genqTagF->E()) ;
      std::cout << "ratioA " << (tagJetCandsMaxMinv.second->p4().E()/m_genqTagF->E())  << std::endl;
    }

  reco::CaloJetCollection TheJets = *jetCollectionHandle;
  std::pair<VBFjetIt,VBFjetIt> tagJetCandsMaxPt = findMaxPtJetsPair (TheJets, m_jetPtMin, m_jetEtaMax) ;

  std::cout << "B ************** eta 1 " << tagJetCandsMaxPt.first->p4().Eta() << " eta 2 "<<  tagJetCandsMaxPt.second->p4().Eta() << std::endl;
  std::cout << "B ************** E 1 " << tagJetCandsMaxPt.first->p4().E() << " E 2 "<<  tagJetCandsMaxPt.second->p4().E() << std::endl;
  std::cout << "B ************** Pt 1 " << tagJetCandsMaxPt.first->p4().Pt() << " Pt 2 "<<  tagJetCandsMaxPt.second->p4().Pt() << std::endl;

  if (tagJetCandsMaxPt.first->p4().Eta() > tagJetCandsMaxPt.second->p4().Eta())
    {
      m_deltaR_CandsMaxPt -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.first->momentum(), m_genqTagF->Vect()) ) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.first->p4().E()/m_genqTagF->E()) ;
      std::cout << "ratioB " << (tagJetCandsMaxPt.first->p4().E()/m_genqTagF->E())  << std::endl;
      m_deltaR_CandsMaxPt -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.second->momentum(), m_genqTagB->Vect()) ) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.second->p4().E()/m_genqTagB->E()) ;
      std::cout << "ratioB " << (tagJetCandsMaxPt.second->p4().E()/m_genqTagB->E())  << std::endl;
    }
  else if (tagJetCandsMaxPt.first->p4().Eta() < tagJetCandsMaxPt.second->p4().Eta())
    {
      m_deltaR_CandsMaxPt -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.first->momentum(), m_genqTagB->Vect()) ) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.first->p4().E()/m_genqTagB->E()) ;
      std::cout << "ratioB " << (tagJetCandsMaxPt.first->p4().E()/m_genqTagB->E())  << std::endl;
      m_deltaR_CandsMaxPt -> Fill (ROOT::Math::VectorUtil::DeltaR (tagJetCandsMaxPt.second->momentum(), m_genqTagF->Vect()) ) ;
      m_Eratio_CandsMaxPt -> Fill (tagJetCandsMaxPt.second->p4().E()/m_genqTagF->E()) ;
      std::cout << "ratioB " << (tagJetCandsMaxPt.second->p4().E()/m_genqTagF->E())  << std::endl;
    }

   
}
// --------------------------------------------------------------------


void 
VBFDiffTagFinderComparison::beginJob (const edm::EventSetup&)
{
  m_genqTagF = new TLorentzVector (0.0,0.0,0.0,0.0);
  m_genqTagB = new TLorentzVector (0.0,0.0,0.0,0.0);

  edm::Service<TFileService> fs ;

  m_deltaR_CandsMaxMinv = fs->make<TH1F> ("m_deltaR_CandsMaxMinv", "dR between jet and quark", 10000, 0, 150);
  m_Eratio_CandsMaxMinv = fs->make<TH1F> ("m_Eratio_CandsMaxMinv", "ratio between quark and jet energies", 10000, 0, 150);

  m_deltaR_CandsMaxPt = fs->make<TH1F> ("m_deltaR_CandsMaxPt", "dR between jet and quark", 10000, 0, 50);
  m_Eratio_CandsMaxPt = fs->make<TH1F> ("m_Eratio_CandsMaxPt", "ratio between quark and jet energies", 10000, 0, 50);
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
  std::cout << "settato" << std::endl;
}


void VBFDiffTagFinderComparison::setMomentum (TLorentzVector & myvector, const Candidate & gen)
{
  myvector.SetPx (gen.px());
  myvector.SetPy (gen.py());
  myvector.SetPz (gen.pz());
  myvector.SetE (gen.energy());
}

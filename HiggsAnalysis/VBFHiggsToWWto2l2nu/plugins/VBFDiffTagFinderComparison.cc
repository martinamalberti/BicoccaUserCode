// $Id: VBFDiffTagFinderComparison.cc,v 1.1 2009/02/03 13:50:42 abenagli Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFDiffTagFinderComparison.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include <Math/VectorUtil.h>

VBFDiffTagFinderComparison::VBFDiffTagFinderComparison (const edm::ParameterSet& iConfig) :
      m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")),
      m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")),
	  m_algo (iConfig.getParameter<int> ("algoType"))

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
  edm::Handle<edm::HepMCProduct> evtMC;
  bool sig = true;
  try {
  iEvent.getByLabel("source",evtMC);}
  catch (...) {
  sig = false;}
  
  //get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle);
  if (jetCollectionHandle->size () < 2) return ;
  //get the gen particles collection
  edm::Handle<GenParticleCollection> genParticles;
  iEvent.getByLabel(m_MCtruthInputTag, genParticles);

  std::pair<VBFjetIt,VBFjetIt> tagJetCands ;
  if (m_algo == 0) tagJetCands = 
    vbfhww2l::findTagJets (jetCollectionHandle->begin (), jetCollectionHandle->end (), 15.0, 5.0) ;
  else if (m_algo == 1) tagJetCands = 
    vbfhww2l::findMaxPtJetsPair (jetCollectionHandle->begin (), jetCollectionHandle->end (), 15.0, 5.0) ;

  if (tagJetCands.first->p4() == tagJetCands.second->p4()) return;

  double threshold = 0.3;
  double m_ptMax, m_energyMax;
  int m_purity = 99;
  double m_deltaRF = 99.9;
  double m_EratioF = 99.9;
  double m_deltaRB = 99.9;
  double m_EratioB = 99.9;
  
  double m_deltaEta = fabs (tagJetCands.first->p4().Eta() - tagJetCands.second->p4().Eta() );
  double m_prdEta = (tagJetCands.first->p4().Eta() * tagJetCands.second->p4().Eta() );
  
  math::XYZTLorentzVector summedTags = (tagJetCands.first->p4() + tagJetCands.second->p4());  
  double m_massInv = summedTags.M ();
  
  if (tagJetCands.first->p4().Pt() > tagJetCands.second->p4().Pt()) m_ptMax = tagJetCands.first->p4().Pt();
  else m_ptMax = tagJetCands.second->p4().Pt();
  
  if (tagJetCands.first->p4().E() > tagJetCands.second->p4().E()) m_energyMax = tagJetCands.first->p4().E();
  else m_energyMax = tagJetCands.second->p4().E();
  
  if (sig)
  {
	  const HepMC::GenEvent * Evt = evtMC->GetEvent();
	  int processID = Evt->signal_process_id() ;
	  if (processID == 123 || processID == 124)
	  {
		  findGenParticles (genParticles, *m_genqTagF, *m_genqTagB);
		  if (tagJetCands.first->p4().Eta() > tagJetCands.second->p4().Eta())
			{
			  ///////////////// ************ resolution ***********************************************                                                                       
			  m_deltaRF = ROOT::Math::VectorUtil::DeltaR (tagJetCands.first->momentum(), m_genqTagF->Vect()) ;
			  m_EratioF = tagJetCands.first->p4().E()/m_genqTagF->E() ;

			  m_deltaRB = ROOT::Math::VectorUtil::DeltaR (tagJetCands.second->momentum(), m_genqTagB->Vect()) ;
			  m_EratioB = tagJetCands.second->p4().E()/m_genqTagB->E() ;
			  
			  ///////////////// ************ purity with inv mass selection ***********************************************

			  if (m_deltaRF < threshold && m_deltaRB > threshold) m_purity = 1 ; // 1 matching
			  else if (m_deltaRB < threshold && m_deltaRF > threshold) m_purity = 1 ; // 1 matching       
			  else if (m_deltaRB < threshold && m_deltaRF < threshold) m_purity = 2 ; // 2 matching
			  else if (m_deltaRB > threshold && m_deltaRF > threshold) m_purity = 0 ; // 0 matching
			}
		  else if (tagJetCands.first->p4().Eta() < tagJetCands.second->p4().Eta()) 
			{
			  ///////////////// ************ resolution ***********************************************                                                                      

			  m_deltaRB = ROOT::Math::VectorUtil::DeltaR (tagJetCands.first->momentum(), m_genqTagB->Vect()) ;
			  m_EratioB  = tagJetCands.first->p4().E()/m_genqTagB->E() ;

			  m_deltaRF = ROOT::Math::VectorUtil::DeltaR (tagJetCands.second->momentum(), m_genqTagF->Vect()) ;
			  m_EratioF = tagJetCands.second->p4().E()/m_genqTagF->E() ;
			  
			  ///////////////// ************ purity with inv mass selection ***********************************************

			  if (m_deltaRF < threshold && m_deltaRB > threshold) m_purity = 1 ; // 1 matching
			  else if (m_deltaRB < threshold && m_deltaRF > threshold) m_purity = 1 ; // 1 matching       
			  else if (m_deltaRB < threshold && m_deltaRF < threshold) m_purity = 2 ; // 2 matching
			  else if (m_deltaRB > threshold && m_deltaRF > threshold) m_purity = 0 ; // 0 matching
			}

		}
	}
  
m_ntuple -> Fill (m_deltaEta, m_prdEta, m_massInv, m_energyMax, m_ptMax, m_purity, m_deltaRF, m_deltaRB, m_EratioF, m_EratioB, m_algo) ;

}
// --------------------------------------------------------------------


void 
VBFDiffTagFinderComparison::beginJob (const edm::EventSetup&)
{
  m_genqTagF = new TLorentzVector (0.0,0.0,0.0,0.0);
  m_genqTagB = new TLorentzVector (0.0,0.0,0.0,0.0);

  edm::Service<TFileService> fs ;

  m_ntuple = fs->make <TNtuple> ("ntuple","Some variables","deltaEta:prodEta:mInv:Emax:Ptmax:purity:dR1:dR2:rE1:rE2:algo");

}


// --------------------------------------------------------------------


void 
VBFDiffTagFinderComparison::endJob () 
{
}

// --------------------------------------------------------------------

void VBFDiffTagFinderComparison::findGenParticles (edm::Handle<GenParticleCollection> &genParticles,
				     TLorentzVector &m_genqTagF,
				     TLorentzVector &m_genqTagB)
{
 for (GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
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

void VBFDiffTagFinderComparison::setMomentum (TLorentzVector & myvector, const GenParticle & gen)
{
  myvector.SetPx (gen.px());
  myvector.SetPy (gen.py());
  myvector.SetPz (gen.pz());
  myvector.SetE (gen.energy());
}

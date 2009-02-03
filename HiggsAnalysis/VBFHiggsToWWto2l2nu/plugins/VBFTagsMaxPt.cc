// $Id: VBFTagsMaxPt.cc,v 1.2 2008/04/07 12:50:20 tancini Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTagsMaxPt.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include <Math/VectorUtil.h>

VBFTagsMaxPt::VBFTagsMaxPt (const edm::ParameterSet& iConfig) :
      m_tagJetInputTag (iConfig.getParameter<edm::InputTag> ("tagJetInputTag")),
      m_otherJetInputTag (iConfig.getParameter<edm::InputTag> ("otherJetInputTag")),
	  m_tracksInputTag (iConfig.getParameter<edm::InputTag> ("tracksInputTag"))


 {}


// --------------------------------------------------------------------


VBFTagsMaxPt::~VBFTagsMaxPt ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


// --------------------------------------------------------------------


void
VBFTagsMaxPt::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //get the tag jet collection

  edm::Handle<RecoChargedCandidateCollection> jetCollectionHandle;
  iEvent.getByLabel (m_tagJetInputTag, jetCollectionHandle);
  
  TLorentzVector myvector;
  
  TClonesArray &jetTag = *m_tagJets;
  int counter = 0;
  for (RecoChargedCandidateCollection::const_iterator jet = jetCollectionHandle->begin (); jet != jetCollectionHandle->end (); ++jet )
       { 
        setMomentum (myvector, jet->p4());
		new (jetTag[counter]) TLorentzVector (myvector);
		counter++;
        }
    
  //get the other tag jet collection

  edm::Handle<reco::CaloJetCollection> jetCollectionHandle2;
  iEvent.getByLabel (m_otherJetInputTag, jetCollectionHandle2);
  
  TClonesArray &jetOther = *m_otherJets;
  counter = 0;
  for (CaloJetCollection::const_iterator jet2 = jetCollectionHandle2->begin (); jet2 != jetCollectionHandle2->end (); ++jet2 ) 
       { 
        setMomentum (myvector, jet2->p4());		
		new (jetOther[counter]) TLorentzVector (myvector);
		counter++;
        }

  
  //get the tracks collection
  edm::Handle<trackCollection> TrackHandle ;
  iEvent.getByLabel (m_tracksInputTag, TrackHandle) ;
   
  double etaF = 0.0;	 
  double etaB = 0.0;	 

  if (((*jetCollectionHandle)[0].eta ()) > ((*jetCollectionHandle)[1].eta ()))
  {
  etaF = ((*jetCollectionHandle)[0].eta ());
  etaB = ((*jetCollectionHandle)[1].eta ());
  }
  else
  {
  etaF = ((*jetCollectionHandle)[1].eta ());
  etaB = ((*jetCollectionHandle)[2].eta ());
  }

  m_nTranks = 0 ;  
  m_nTranks_between = 0;
  
  for (trackCollection::const_iterator trackIt = TrackHandle->begin () ;trackIt != TrackHandle->end () ; ++trackIt)
  {
	  ++m_nTranks ;
	  if (trackIt->innerMomentum ().eta() > etaB && trackIt->innerMomentum ().eta() < etaF) 
	  ++m_nTranks_between ;
  }
  
  m_tree->Fill () ;

  m_tagJets -> Clear () ;
  m_otherJets -> Clear ();  
}
// --------------------------------------------------------------------


void 
VBFTagsMaxPt::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;

  m_tree = fs->make <TTree> ("tree","Some variables for tag jets and other jets");
    
  // vector with the 2 tag TLorentzVectors
  m_tagJets = new TClonesArray ("TLorentzVector");
  m_tree->Branch ("tagJets", "TClonesArray", &m_tagJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
  m_otherJets = new TClonesArray ("TLorentzVector");
  m_tree->Branch ("otherJets", "TClonesArray", &m_otherJets, 256000,0);

  //numero tracce in mezzo
  m_nTranks = 0;
  m_tree->Branch ("nTranks", &m_nTranks, "m_nTranks/I");
  m_nTranks_between = 0;
  m_tree->Branch ("nTranks_between", &m_nTranks_between, "m_nTranks_between/I");

}


// --------------------------------------------------------------------


void 
VBFTagsMaxPt::endJob () 
{
}

// --------------------------------------------------------------------

void
VBFTagsMaxPt::setMomentum (TLorentzVector &myvector, const LorentzVector & mom)
{
    myvector.SetPx (mom.Px());
    myvector.SetPy (mom.Py());
    myvector.SetPz (mom.Pz());
    myvector.SetE (mom.E());
}



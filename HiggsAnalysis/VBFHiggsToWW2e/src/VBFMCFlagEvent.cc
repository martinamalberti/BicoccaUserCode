#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCFlagEvent.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

//#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
//#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"

#include <iostream>
#include <algorithm>

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

VBFMCFlagEvent::VBFMCFlagEvent (const edm::ParameterSet& iConfig) :
  m_label (iConfig.getUntrackedParameter<std::string> ("moduleLabel",std::string ("source"))),
  m_eventFlagName (iConfig.getUntrackedParameter<std::string> ("eventFlagName",std::string ("eventFlag"))) 
{
  produces<std::vector<int> > (m_eventFlagName += "Mass").setBranchAlias (m_eventFlagName) ;
}  


// ----------------------------------------------------------------------------

  
VBFMCFlagEvent::~VBFMCFlagEvent ()
{
}


// ----------------------------------------------------------------------------

  
void 
VBFMCFlagEvent::produce (edm::Event& iEvent, const edm::EventSetup& iEventSetup)
{
  
  edm::Handle<edm::HepMCProduct> evtMC;
  iEvent.getByLabel(m_label,evtMC);

  const HepMC::GenEvent * Evt = evtMC->GetEvent();

  std::auto_ptr<std::vector<int> > eventFlag (new std::vector<int>) ;
  eventFlag->push_back (Evt->signal_process_id ()) ;

  //PG insert the collection into the event
  iEvent.put (eventFlag, m_eventFlagName) ;
 
}

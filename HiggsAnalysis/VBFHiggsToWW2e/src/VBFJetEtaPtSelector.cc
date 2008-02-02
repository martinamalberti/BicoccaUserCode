#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetEtaPtSelector.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <Math/VectorUtil.h>

#include <iostream>
#include <algorithm>


VBFJetEtaPtSelector::VBFJetEtaPtSelector (const edm::ParameterSet& iConfig) :
  m_minPt (iConfig.getParameter<double> ("minPt")) ,
  m_maxEta (iConfig.getParameter<double> ("maxEta"))
{
}  


// ----------------------------------------------------------------------------

  
VBFJetEtaPtSelector::~VBFJetEtaPtSelector ()
{
}


// ----------------------------------------------------------------------------

  
void 
VBFJetEtaPtSelector::select (edm::Handle<VBFJetEtaPtSelector::collection> jetCollectionHandle, 
                       const edm::Event& iEvent)
{
  m_selected.clear () ;

  //VT loop over jets
  for (collection::const_iterator jetIt = jetCollectionHandle->begin () ;
       jetIt != jetCollectionHandle->end () ;
       ++jetIt)
    {
      if (jetIt->p4().Pt() < m_minPt || jetIt->p4().Eta() > m_maxEta) continue;
      m_selected.push_back (jet (jetCollectionHandle, jetIt - jetCollectionHandle->begin ()) );
    } 
}

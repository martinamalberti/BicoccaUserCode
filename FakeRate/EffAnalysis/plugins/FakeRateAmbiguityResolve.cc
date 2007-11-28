// my includes
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraBase.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FakeRate/EffAnalysis/plugins/FakeRateAmbiguityResolve.h"

//CLHEP
#include <CLHEP/Vector/LorentzVector.h>

#include <iostream>

FakeRateAmbiguityResolve::FakeRateAmbiguityResolve (const edm::ParameterSet& conf)
{}


// ------------------------------------------------------------


void 
//FakeRateAmbiguityResolve::select (edm::Handle<collection> inputHandle, 
//                         const edm::Event& evt, const edm::EventSetup& evtStp ) 
FakeRateAmbiguityResolve::select (edm::Handle<collection> inputHandle, 
                        const edm::Event& evt) 
{
  m_selected.clear () ;
  //PG get the actual product

  dump (&m_selected, inputHandle) ;

  container::iterator ShorterEnd;

  sort (m_selected.begin (), m_selected.end (), TKeEoPSorting ()) ; 
  ShorterEnd = unique (m_selected.begin (), m_selected.end (), equalTk ()) ; 
  m_selected.erase(ShorterEnd, m_selected.end());
  sort (m_selected.begin (), m_selected.end (), SCEeEoPSorting ()) ; 
  ShorterEnd = unique (m_selected.begin (), m_selected.end (), equalSC ()) ; 
  m_selected.erase (ShorterEnd, m_selected.end());

  return ;
}       


// ------------------------------------------------------------


FakeRateAmbiguityResolve::~FakeRateAmbiguityResolve()
{}


// ------------------------------------------------------------


void
FakeRateAmbiguityResolve::dump (container * output,
                                const edm::Handle<collection> & input) {
  for (size_t i = 0; i < input->size(); ++i )
    output->push_back (electron (input, i)) ;
}

//void
//FakeRateAmbiguityResolve::dump (container * output,
//                                const edm::Handle<collection> & input) {
//  for (size_t i = 0; i < input->size(); ++i )
//    output->push_back (reco::PixelMatchGsfElectronRef (input, i)) ;
//}

//PG old, before Luca suggestions
//void 
//FakeRateAmbiguityResolve::dump (container * output, 
//                              const edm::Handle<collection> & input) 
//{
//  for (collection::const_iterator it = input->begin () ;
//       it != input->end () ;
//       ++it)
//    { output->push_back (&(*it)) ; }   
//  return ;
//}


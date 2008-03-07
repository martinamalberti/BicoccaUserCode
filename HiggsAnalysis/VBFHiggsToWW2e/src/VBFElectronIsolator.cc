// $Id: VBFElectronIsolator.cc,v 1.5 2008/03/07 13:53:10 govoni Exp $
//#include "DataFormats/EgammaCandidates/interface/PMGsfElectronIsoCollection.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFElectronIsolator.h"


VBFElectronIsolator::VBFElectronIsolator (const edm::ParameterSet& iConfig) :
  m_TrackInputTag (iConfig.getParameter<edm::InputTag> ("trackInputTag")) ,
  m_tkIsolationAlgo (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      iConfig.getParameter<double> ("otherVetoRadius") ,
      iConfig.getParameter<double> ("ptMin") ,
      iConfig.getParameter<double> ("lipMax") 
    ) ,
  m_trackIsolationCut (iConfig.getParameter<double> ("tkIsoCut"))
{}


// ------------------------------------------------------------------------------------------------


VBFElectronIsolator::~VBFElectronIsolator ()
{
}


// ------------------------------------------------------------------------------------------------


void VBFElectronIsolator::select (const edm::Handle<VBFElectronIsolator::collection> electrons, 
                                  const edm::Event& iEvent)
{
// Get the electrons
//  edm::Handle<electronCollection> GSFHandle ;
//  iEvent.getByLabel (m_GSFInputTag, GSFHandle) ;

  // Get the tracks
  edm::Handle<trackCollection> TrackHandle ;
  iEvent.getByLabel (m_TrackInputTag, TrackHandle) ;

  m_selected.clear () ;

  //PG loop over electrons
  for (unsigned i = 0 ; i < electrons->size () ; ++i)
    {
      electronRef electronReference = electrons->refAt (i).castTo<electronRef> () ;
      double isolationValue = 
        m_tkIsolationAlgo.calcIsolationValue (electrons, TrackHandle, electronReference) ;
      if (isolationValue < m_trackIsolationCut)
        {
          m_selected.push_back (electronReference) ;
        }
    } //PG loop over electrons
      
}






#define DEBUG    0 // 0=false
#define C_DEBUG  10 // currently debuging

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTrackProducer.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/IdealGeometryRecord.h"


using namespace std ;
using namespace edm ;
using namespace reco ;
using namespace vbfhww2l ;


VBFTrackProducer::VBFTrackProducer (const edm::ParameterSet& iConfig) :
  m_trackInputTag (iConfig.getParameter<InputTag> ("trackInputTag"))
{

  produces<VBFParticleCollection> () ;

}


// --------------------------------------------------------------------


VBFTrackProducer::~VBFTrackProducer ()
{}


// --------------------------------------------------------------------


void
VBFTrackProducer::produce (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  //PG get the electrons collection 
  Handle<trackCollection> tracksHandle ;
  iEvent.getByLabel (m_trackInputTag, tracksHandle) ;

  //PG the output collection
  auto_ptr<VBFParticleCollection> tracks (new VBFParticleCollection) ;

 //PG loop over GSF electrons
  for (trackCollection::const_iterator trackIt = tracksHandle->begin () ; 
       trackIt != tracksHandle->end () ; 
       ++trackIt)
    {
      vbfhww2l::VBFParticle * track = new vbfhww2l::VBFParticle () ;
      track->v4 = LorentzVector (trackIt->px (), 
                                 trackIt->py (), 
                                 trackIt->pz (),
                                 trackIt->p ()) ; //PG FIXME metto questo, o quello al vtx?
      track->vertex = trackIt->vertex () ;
//      track->pdgId = trackIt->pdgId () ;

//PG FIXME track properties missing
//PG FIXME maybe also inner, outer momentum and position
  // Extract track properties
      track->AddDoubleProp ("d0", fabs (trackIt->d0 ())) ; 
      track->AddDoubleProp ("d0Error", trackIt->d0Error ()) ;
      track->AddDoubleProp ("dz", fabs (trackIt->dz ())) ;
      track->AddDoubleProp ("dzError", trackIt->dzError ()) ;
      track->AddIntProp ("nHits", trackIt->recHitsSize ()) ;
 
      tracks->push_back (*track) ;
    }  
#if DEBUG
  cout << "number of tracks = " << tracks->size () << endl ;
#endif

  
  iEvent.put (tracks) ;
}


// --------------------------------------------------------------------


void 
VBFTrackProducer::beginJob (const edm::EventSetup&)
{}


// --------------------------------------------------------------------


void 
VBFTrackProducer::endJob ()
{}


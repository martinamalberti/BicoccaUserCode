#define DEBUG    0 // 0=false
#define C_DEBUG  10 // currently debuging

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetProducer.h"

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


VBFJetProducer::VBFJetProducer (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<InputTag> ("jetInputTag"))
{

  produces<VBFParticleCollection> () ;

}


// --------------------------------------------------------------------


VBFJetProducer::~VBFJetProducer ()
{}


// --------------------------------------------------------------------


void
VBFJetProducer::produce (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  //PG get the electrons collection 
  Handle<JetCollection> jetsHandle ;
  try{  
    iEvent.getByLabel (m_jetInputTag, jetsHandle) ;
  }
  catch(cms::Exception& e){
    //create empty collection
    auto_ptr<VBFParticleCollection> jets (new VBFParticleCollection);
    iEvent.put(jets);
    return;
  }

  //PG the output collection
  auto_ptr<VBFParticleCollection> jets (new VBFParticleCollection) ;

 //PG loop over GSF electrons
  for (JetCollection::const_iterator jetIt = jetsHandle->begin () ; 
       jetIt != jetsHandle->end () ; 
       ++jetIt)
    {
      vbfhww2l::VBFParticle * jet = new vbfhww2l::VBFParticle () ;
      jet->v4 = jetIt->p4 () ; //PG FIXME metto questo, o quello al vtx?
      jet->vertex = jetIt->vertex () ;
      jet->pdgId = jetIt->pdgId () ;
 
      jets->push_back (*jet) ;
    }  
#if DEBUG
  cout << "number of jets = " << jets->size () << endl ;
#endif

  
  iEvent.put (jets) ;
}


// --------------------------------------------------------------------


void 
VBFJetProducer::beginJob (const edm::EventSetup&)
{}


// --------------------------------------------------------------------


void 
VBFJetProducer::endJob ()
{}


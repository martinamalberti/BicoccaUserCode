// $Id: VBFElectronHadIsolator.cc,v 1.2 2008/07/09 14:13:35 govoni Exp $
//#include "DataFormats/EgammaCandidates/interface/PMGsfElectronIsoCollection.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElectronHadIsolator.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"


VBFElectronHadIsolator::VBFElectronHadIsolator (const edm::ParameterSet& iConfig) :
  m_hcalrhitsInputTag (iConfig.getParameter<edm::InputTag> ("hcalrhitsInputTag")) ,
  m_hadIsolationAlgo (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      iConfig.getParameter<double> ("etMin")
    ) ,
  m_hadIsolationCut (iConfig.getParameter<double> ("hadIsoCut"))
{}


// ------------------------------------------------------------------------------------------------


VBFElectronHadIsolator::~VBFElectronHadIsolator ()
{
}


// ------------------------------------------------------------------------------------------------


void
VBFElectronHadIsolator::newEvent (const edm::Event & evt, const edm::EventSetup& es) 
{
  //get calo geometry
  es.get<IdealGeometryRecord> ().get (m_caloGeomHandle) ;
  m_caloGeomHandle.product () ;
}


// ------------------------------------------------------------------------------------------------


void 
VBFElectronHadIsolator::select (const edm::Handle<VBFElectronHadIsolator::collection> electrons, 
                                const edm::Event& iEvent,
                                const edm::EventSetup& es)
{
  //get the HCAL rechit collection
  edm::Handle<HBHERecHitCollection> hbhe ;
  iEvent.getByLabel(m_hcalrhitsInputTag, hbhe); 
  HBHERecHitMetaCollection mhbhe = HBHERecHitMetaCollection (*hbhe) ;  

  m_selected.clear () ;

  //PG loop over electrons
  for (unsigned i = 0 ; i < electrons->size () ; ++i)
    {
      electronRef electronReference = electrons->refAt (i).castTo<electronRef> () ;
      double isolationValue = 
        m_hadIsolationAlgo.calcIsolationValue (m_caloGeomHandle, mhbhe, electronReference, electrons) ;
      if (isolationValue < m_hadIsolationCut)
        {
          m_selected.push_back (electronReference) ;
        }
    } //PG loop over electrons
      
}






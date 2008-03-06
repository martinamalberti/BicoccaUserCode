#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleTrackerIsolationAlgo.h"
#include "DataFormats/EgammaCandidates/interface/PMGsfElectronIsoCollection.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFElectronIsolator.h"


VBFElectronIsolator::VBFElectronIsolator(const edm::ParameterSet& iConfig)
{
  selectedElectronsRefLabel_ = iConfig.getParameter<InputTag>("SelectedElectronRefCollectionLabel");
  trckIsolationProducer_     = iConfig.getParameter<InputTag>("TrckIsolationProducerLabel");
  doRefCheck_		     = iConfig.getParameter<bool>("doRefCheck");
  theTrackIsolCut_	     = iConfig.getParameter<double>("TrackIsolCut");
}


VBFElectronIsolator::~VBFElectronIsolator()
{
}

void VBFElectronIsolator::select(edm::Handle<reco::PixelMatchGsfElectronCollection> electrons, const edm::Event& iEvent)
{
  using namespace edm;
  using namespace reco;
  using namespace std;

  selected_.clear();
  Handle<RefVector<PixelMatchGsfElectronCollection> >electronsRef;

  edm::Handle< reco::PMGsfElectronIsoCollection > tkIsolationHandle;
  try {
    iEvent.getByLabel(trckIsolationProducer_, tkIsolationHandle);
  }
  catch ( cms::Exception& ex ) {
    printf("Can't get tracker isolation product\n");
  }

  if(doRefCheck_==true)
    iEvent.getByLabel(selectedElectronsRefLabel_,electronsRef);

  for(unsigned i =0; i<electrons->size(); i++)
  {
     double sumPtOverEt = (*tkIsolationHandle)[i].second; 

     Ref<reco::PixelMatchGsfElectronCollection> electronRAWRef(electrons,i);
     
     bool selected=true;
     if(doRefCheck_==true)
       if (find(electronsRef->begin(), electronsRef->end(),electronRAWRef)==electronsRef->end())
       {
         selected=false;
       }

     if (sumPtOverEt < theTrackIsolCut_ && selected==true)
     {
       selected_.push_back(electronRAWRef);
     }
  }
}






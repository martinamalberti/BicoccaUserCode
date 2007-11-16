#include "HiggsAnalysis/VBFReadEvent/interface/VBFReadEvent.h"


VBFReadEvent::VBFReadEvent (const edm::ParameterSet& iConfig) :
      m_metColletion (iConfig.getParameter<edm::InputTag> ("metCollection")) ,
      m_genMetColletion (iConfig.getParameter<edm::InputTag> ("genMetCollection")) ,
      m_jetColletion (iConfig.getParameter<edm::InputTag> ("jetCollection")) ,
      m_genJetColletion (iConfig.getParameter<edm::InputTag> ("genJetCollection")) ,
      m_eleColletion (iConfig.getParameter<edm::InputTag> ("eleCollection")) ,
      m_MCtruthColletion (iConfig.getParameter<edm::InputTag> ("MCtruthCollection"))
      m_muColletion (iConfig.getParameter<edm::InputTag> ("muCollection"))
// il resto del MC
// il trigger
// gli elettroni, guarda il codice ftto con roberto

{
   //now do what ever initialization is needed

}


// --------------------------------------------------------------------


VBFReadEvent::~VBFReadEvent ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFReadEvent::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

//   edm::Handle<ExampleData> pIn ;
//   iEvent.getByLabel ("example",pIn) ;

   
}


// --------------------------------------------------------------------


void 
VBFReadEvent::beginJob (const edm::EventSetup&)
{}


// --------------------------------------------------------------------


void 
VBFReadEvent::endJob () 
{}


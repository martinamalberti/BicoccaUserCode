#define DEBUG    0 // 0=false
#define C_DEBUG  0 // currently debuging


#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
//#include "RecoMuon/MuonIdentification/interface/IdGlobalFunctions.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"



#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTreeProducer.h"


using namespace std;
using namespace edm;
using namespace reco;
using namespace vbfhww2l;



VBFTreeProducer::VBFTreeProducer(const ParameterSet& iConfig) :
 m_particlesList (iConfig.getParameter<std::vector<edm::ParameterSet> > 
   ("particlesList"))
{

#if DEBUG
 cerr << "\n\tVBFTreeProducer::VBFTreeProducer" << endl ;
#endif

  s_info = new SampleInfo_();
  s_info->name = iConfig.getParameter<string>("SampleName");
  s_info->cross_section = iConfig.getParameter<double>("SampleCrossSection");
  
  nEv = 0;

  result_dir = iConfig.getParameter<string>("ResultDir");
  mini_tree_file_name = iConfig.getParameter<string>("MiniTreeFilename");

  mini_tree_file =0;  	
  mini_tree      =0;

//PG FIXME  ev    = new Event_();
//PG FIXME  mc_ev = new MCEvent_();

  //PG loop over list of particle types to be saved
  for (std::vector<edm::ParameterSet>::const_iterator partIt = m_particlesList.begin () ;
       partIt != m_particlesList.end () ;
       ++partIt) 
    {
      VBFParticleCollection * dummy = new VBFParticleCollection ;
      std::string branchName = partIt->getParameter<std::string>("branchName") ;
      m_particleCollections[branchName] = dummy ;    
    } //PG loop over list of particle types to be saved

  use_mcInfo = iConfig.getParameter<string>("use_mcInfo");
 
  
#if DEBUG
  cerr << "\n\tVBFTreeProducer::VBFTreeProducer...\t\tDONE\n" << endl ;
#endif
}


// ------------------------------------------------------------------------


VBFTreeProducer::~VBFTreeProducer(){

#if DEBUG
  cerr << "\n\tVBFTreeProducer::~VBFTreeProducer" << endl ;
#endif

  //delete mini_tree_file;  	
  //delete mini_tree;

//   delete ev;
//   delete lep;
//   delete jet;
//   delete mc_ev;
   delete s_info;

//PG FIXME should put the delete of the collections here
  //m_particleCollections.clear();
  for(map<string, VBFParticleCollection *>::iterator i = m_particleCollections.begin();
      i != m_particleCollections.end(); i++){

    delete i->second;
  }

#if DEBUG
  cerr << "\n\tVBFTreeProducer::~VBFTreeProducer...\t\tDONE\n" << endl ;
#endif
}


// ------------------------------------------------------------------------


void VBFTreeProducer::beginJob(const EventSetup&){

#if DEBUG
  cerr << "\n\tVBFTreeProducer::beginJob" << endl ;
#endif
  
  string file_name = result_dir + mini_tree_file_name + ".root"; 

  mini_tree_file = new TFile(file_name.c_str(), "recreate");

  if (mini_tree_file){

    mini_tree = new TTree("Event", "A tree with preselected Objects.",3);

    if(use_mcInfo == "yes")
      mini_tree->Branch("MCInfo", "vbfhww2l::VBFMCEvent", &mc_ev, 32000, 3);

    mini_tree->Branch("EventInfo", "vbfhww2l::VBFEventInfo", &ev_i, 32000,3); 

//PG FIXME    if(s_info->name.find("qqH") != string::npos)
//PG FIXME      mini_tree->Branch("VBFMCEvent", "vbfhww2l::VBFMCEvent", &mc_ev, 32000, 3);
    
    //PG loop over particle containers map
    for (std::map<std::string, VBFParticleCollection *>::iterator mapIt = m_particleCollections.begin () ;
         mapIt != m_particleCollections.end () ;
         ++mapIt)
      {
        mini_tree->Branch (mapIt->first.c_str (), 
                           "std::vector<vbfhww2l::VBFParticle>", 
                           &(mapIt->second)) ; 
      } //PG loop over particle containers map
  }

#if DEBUG
  cerr << "\n\tVBFTreeProducer::beginJob...\t\tDONE\n" << endl ;
#endif
}


// ------------------------------------------------------------------------


void 
VBFTreeProducer::analyze (const Event& iEvent, const EventSetup& iSetup)
{

#if DEBUG
  cerr << "\n\tVBFTreeProducer::analyze" << endl ;
#endif

  nEv++;

  //PG reset the containers

//PG FIXME  ev->Reset () ;
  //PG loop over particle containers map
  for (std::map<std::string, VBFParticleCollection *>::iterator mapIt = m_particleCollections.begin () ;
       mapIt != m_particleCollections.end () ;
       ++mapIt)
    {
      mapIt->second->clear () ;
    } //PG loop over particle containers map


  //general infos
//PG FIXME  ev->runId = iEvent.id().run();
//PG FIXME  ev->eventId = iEvent.id().event();

  //fill mc info vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//PG FIXME  if (s_info->name.find("qqH") != string::npos)
//PG FIXME    {
      Handle<MCEvent_> mc_Handle;
      try {
	iEvent.getByLabel("MCEventProducer", mc_Handle);
	mc_ev = mc_Handle.product();
      }
      catch(cms::Exception& e) {}

      Handle<VBFEventInfo>infoha;
      try {
	iEvent.getByLabel("EvInfoProducer", infoha);
	ev_i = infoha.product();
      }
      catch(cms::Exception& e)  
	{
	  ev_i = new VBFEventInfo;
	  cout << " module EvInfoProducer hasn't been executed!!! " <<endl;
	}

//PG FIXME    }


#if DEBUG
//PG FIXME  cerr << "\nRunId " << ev->runId << "\tEventId " << ev->eventId << endl ;
#endif



  //PG save all particles collections 
  //PG ------------------------------

  
  //PG loop over list of particle types to be saved
  for (std::vector<edm::ParameterSet>::const_iterator partIt = m_particlesList.begin () ;
       partIt != m_particlesList.end () ;
       ++partIt)
    {
//      VBFParticleCollection * dummy = new VBFParticleCollection ;
      std::string branchName = partIt->getParameter<std::string>("branchName") ;
      edm::InputTag partInputTag = partIt->getParameter<edm::InputTag>("src") ;
      Handle<VBFParticleCollection> vbfparticleHandle ;
      iEvent.getByLabel (partInputTag, vbfparticleHandle) ;

      //PG loop over content
      for (VBFParticleCollection::const_iterator it = vbfparticleHandle->begin () ; 
           it != vbfparticleHandle->end () ; 
           ++it)
        {    
          m_particleCollections[branchName]->push_back (*it) ;
        } //PG loop over content

    } //PG loop over list of particle types to be saved
  
   mini_tree->Fill () ;


#if DEBUG
  cout << "\n\tVBFTreeProducer::analyze...\t\tDONE\n\n";
#endif
}


// ------------------------------------------------------------------------


void VBFTreeProducer::endJob()
{

#if DEBUG
  cout << "\n\tVBFTreeProducer::endJob\n";
#endif

  s_info->total_events = nEv;
   

  if (mini_tree_file){

 	mini_tree_file->Write(0,TObject::kOverwrite);
	mini_tree_file->WriteObjectAny(s_info, "vbfhww2l::VBFSampleInfo", "SampleInfo");
	mini_tree_file->Close();
  }

  cout << "\nUsed settings: \n";
//PG FIXME  s_info->PrintCuts();

#if DEBUG
  cout << "\n\tVBFTreeProducer::endJob...\t\tDONE\t\n";
#endif
}


#ifndef __VBFTreeProducer_H__
#define __VBFTreeProducer_H__


//-------------------- system include files
#include <memory>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>
#include <vector>


//-------------------- user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"


//-------------------- ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TChain.h"


//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/classes.h"
//PG FIXME#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFFillMCEvent.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFParticle.h"


typedef vbfhww2l::VBFEventInfo  EvInfo_;
typedef vbfhww2l::VBFMCEvent    MCEvent_;
typedef vbfhww2l::VBFSampleInfo SampleInfo_;


//
// class declaration
//
class VBFTreeProducer : public edm::EDAnalyzer {

 public:
  explicit VBFTreeProducer(const edm::ParameterSet&);
  ~VBFTreeProducer();
  
  
 private:

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

//  virtual void saveCollection (edm::InputTag & collectionInputTag, 
//                               ) ;

  std::vector<edm::ParameterSet> m_particlesList ;

  TFile*  mini_tree_file;  	
  TTree*  mini_tree;

  std::string mini_tree_file_name;
  std::string result_dir;

  std::string use_mcInfo;
    
  int nEv;


//PG FIXME  Event_*      ev;
  const MCEvent_*    mc_ev;
  const EvInfo_* ev_i;
  SampleInfo_* s_info;

//  VBFFillMCEvent fill_mc;

  //------------------ for input from .cfg file
  double	electron_cut_pt;
  double	electron_cut_eta;
  
  //to remove jets from pileup
  edm::InputTag vertex_producer;
  edm::InputTag track_producer;
  
  //PG test the particle collections
  std::map<std::string, VBFParticleCollection *> m_particleCollections ;

};

#endif

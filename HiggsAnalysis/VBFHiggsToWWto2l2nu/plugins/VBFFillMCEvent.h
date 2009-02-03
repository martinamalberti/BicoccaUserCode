#ifndef __VBFFillMCEvent_H__
#define __VBFFillMCEvent_H__


//-------------------- system include files
#include <iostream>
#include <iomanip>
#include <string>


//-------------------- user include files
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidate.h"



//-------------------- ROOT includes


//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFMCEvent.h"


typedef vbfhww2l::VBFMCEvent MCEvent_;



//
// class declaration
//
class VBFFillMCEvent{

 public:

  VBFFillMCEvent();
  ~VBFFillMCEvent();

  void FillMC(const HepMC::GenEvent* genEvent, MCEvent_* mcEvent,
	      edm::Handle<reco::CandidateCollection> &genParticles);
  void Print(const HepMC::GenEvent* genEvent,
	     edm::Handle<reco::CandidateCollection> &genParticles,
	     int nParticles =0);
  
 private:

};

#endif

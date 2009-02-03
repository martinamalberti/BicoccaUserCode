#define DEBUG 0 // 0=false


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFFillMCEvent.h"


using namespace std;
using namespace reco;
//using namespace vbfhww2l;


VBFFillMCEvent::VBFFillMCEvent(){}


VBFFillMCEvent::~VBFFillMCEvent(){}


void VBFFillMCEvent::FillMC(const HepMC::GenEvent* genEvent, MCEvent_* mcEvent,
			    edm::Handle<reco::CandidateCollection> &genParticles){

#if DEBUG
  cout << "\n\tVBFFillMCEvent::FillMC\n";
#endif

  mcEvent->Reset();

  const Candidate* p4Parton1_gen =0; 
  const Candidate* p4Parton2_gen =0; 
  const Candidate* p4Higgs_gen =0;
  const Candidate* p4W1_gen =0;
  const Candidate* p4W2_gen =0;      
  const Candidate* p4Lep1_gen =0;
  const Candidate* p4Lep2_gen =0;
  const Candidate* p4Nu1_gen =0; 
  const Candidate* p4Nu2_gen =0;

  const Candidate* h_tmp =0;  //particles from hard process
  const Candidate* w1_tmp =0;
  const Candidate* w2_tmp =0;
  const Candidate* l1_tmp =0;
  const Candidate* l2_tmp =0;
  const Candidate* n1_tmp =0;
  const Candidate* n2_tmp =0;
  

  //find higgs from hard process
  for(CandidateCollection::const_iterator paIt = genParticles->begin();
      paIt != genParticles->end();
      ++ paIt)
    if(paIt->pdgId() == 25){

      h_tmp = paIt->clone();
      break;
    }

  //get higs parents
  p4Parton1_gen = h_tmp->mother(0);
  p4Parton2_gen = h_tmp->mother(1);


  //get higgs(shower) and W+, W- from hard process
  for(unsigned int i=0; i<h_tmp->numberOfDaughters(); i++){

    int pdg_id = h_tmp->daughter(i)->pdgId();

    if(pdg_id == 25)
      p4Higgs_gen = h_tmp->daughter(i);

    else if(pdg_id == 24)
      w1_tmp = h_tmp->daughter(i);

    else if(pdg_id == -24)
      w2_tmp = h_tmp->daughter(i);

    else
      cerr << "\tVBFFillMCEvent::FillMC\n"
	   << "\tERROR trying h_tmp->daughter(" << i << ")\n\n";
  }


  //get W+(shower) and lepton, neutrino from hard process
  for(unsigned int i=0; i<w1_tmp->numberOfDaughters(); i++){

    int pdg_id = w1_tmp->daughter(i)->pdgId();

    if(pdg_id == 24)
      p4W1_gen = w1_tmp->daughter(i);
      
    else if(pdg_id == -11 
	    || pdg_id == -13 
	    || pdg_id == -15)
      l1_tmp = w1_tmp->daughter(i);

    else if(pdg_id == 12 
	    || pdg_id == 14 
	    || pdg_id == 16)
      n1_tmp = w1_tmp->daughter(i);

    else
      cerr << "\tVBFFillMCEvent::FillMC\n"
	   << "\tERROR trying w1_tmp->daughter(" << i << ")\n\n";
  }


  //get W-(shower) and lepton, neutrino from hard process
  for(unsigned int i=0; i<w2_tmp->numberOfDaughters(); i++){

    int pdg_id = w2_tmp->daughter(i)->pdgId();

    if(pdg_id == -24)
      p4W2_gen = w2_tmp->daughter(i);
      
    else if(pdg_id == 11
	    || pdg_id == 13
	    || pdg_id == 15)
      l2_tmp = w2_tmp->daughter(i);

    else if(pdg_id == -12
	    || pdg_id == -14
	    || pdg_id == -16)
      n2_tmp = w2_tmp->daughter(i);

    else
      cerr << "\tVBFFillMCEvent::FillMC\n"
	   << "\tERROR trying w2_tmp->daughter(" << i << ")\n\n";
  }


  //get lepton(shower)
  for(unsigned int i=0; i<l1_tmp->numberOfDaughters(); i++){

    int pdg_id = l1_tmp->daughter(i)->pdgId();

    if(pdg_id == -11 
       || pdg_id == -13 
       || pdg_id == -15)
      p4Lep1_gen = l1_tmp->daughter(i);

    else if(pdg_id == 22) //photon radiation
      ;
    else
      cerr << "\tVBFFillMCEvent::FillMC\n"
	   << "\tERROR trying l1_tmp->daughter(" << i << ")\n\n";
  }


  //get lepton(shower)
  for(unsigned int i=0; i<l2_tmp->numberOfDaughters(); i++){

    int pdg_id = l2_tmp->daughter(i)->pdgId();

    if(pdg_id == 11
       || pdg_id == 13
       || pdg_id == 15)
      p4Lep2_gen = l2_tmp->daughter(i);

    else if(pdg_id == 22) //photon radiation
      ;
    else
      cerr << "\tVBFFillMCEvent::FillMC\n"
	   << "\tERROR trying l2_tmp->daughter(" << i << ")\n\n";
  }

  
  //get neutrinos
  p4Nu1_gen  = n1_tmp->daughter(0);
  p4Nu2_gen  = n2_tmp->daughter(0);
  

#if DEBUG

  //print properties of all particles of interest
  cout << "\nFill\n"

       << setw(5) << "Id" 
       << setw(5) << "KS" 
       << setw(5) << "#mo"
       << setw(7) << "mo1Id"
       << setw(7) << "mo2ID" 
       << setw(5) << "#da"
       << "\n"

       << setw(5) << p4Parton1_gen->pdgId()
       << setw(5) << p4Parton1_gen->status()
       << setw(5) << p4Parton1_gen->numberOfMothers()
       << setw(7) << p4Parton1_gen->mother(0)->pdgId()
       << setw(7) << p4Parton1_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4Parton1_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4Parton2_gen->pdgId()
       << setw(5) << p4Parton2_gen->status()
       << setw(5) << p4Parton2_gen->numberOfMothers()
       << setw(7) << p4Parton2_gen->mother(0)->pdgId()
       << setw(7) << p4Parton2_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4Parton2_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4Higgs_gen->pdgId()
       << setw(5) << p4Higgs_gen->status()
       << setw(5) << p4Higgs_gen->numberOfMothers()
       << setw(7) << p4Higgs_gen->mother(0)->pdgId()
       << setw(7) << p4Higgs_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4Higgs_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4W1_gen->pdgId()
       << setw(5) << p4W1_gen->status()
       << setw(5) << p4W1_gen->numberOfMothers()
       << setw(7) << p4W1_gen->mother(0)->pdgId()
       << setw(7) << p4W1_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4W1_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4W2_gen->pdgId()
       << setw(5) << p4W2_gen->status()
       << setw(5) << p4W2_gen->numberOfMothers()
       << setw(7) << p4W2_gen->mother(0)->pdgId()
       << setw(7) << p4W2_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4W2_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4Lep1_gen->pdgId()
       << setw(5) << p4Lep1_gen->status()
       << setw(5) << p4Lep1_gen->numberOfMothers()
       << setw(7) << p4Lep1_gen->mother(0)->pdgId()
       << setw(7) << p4Lep1_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4Lep1_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4Lep2_gen->pdgId()
       << setw(5) << p4Lep2_gen->status()
       << setw(5) << p4Lep2_gen->numberOfMothers()
       << setw(7) << p4Lep2_gen->mother(0)->pdgId()
       << setw(7) << p4Lep2_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4Lep2_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4Nu1_gen->pdgId()
       << setw(5) << p4Nu1_gen->status()
       << setw(5) << p4Nu1_gen->numberOfMothers()
       << setw(7) << p4Nu1_gen->mother(0)->pdgId()
       << setw(7) << p4Nu1_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4Nu1_gen->numberOfDaughters()
       << "\n"

       << setw(5) << p4Nu2_gen->pdgId()
       << setw(5) << p4Nu2_gen->status()
       << setw(5) << p4Nu2_gen->numberOfMothers()
       << setw(7) << p4Nu2_gen->mother(0)->pdgId()
       << setw(7) << p4Nu2_gen->mother(0)->mother(0)->pdgId()
       << setw(5) << p4Nu2_gen->numberOfDaughters()
       << "\n";
#endif


  //fill mcEvent
  mcEvent->p4Parton1_gen = p4Parton1_gen->daughter(0)->p4();
  mcEvent->p4Parton2_gen = p4Parton2_gen->daughter(1)->p4();
  mcEvent->p4Higgs_gen = p4Higgs_gen->p4();
  mcEvent->p4W1_gen = p4W1_gen->p4();
  mcEvent->p4W2_gen = p4W2_gen->p4();
  mcEvent->p4Lep1_gen = p4Lep1_gen->p4();
  mcEvent->p4Lep2_gen = p4Lep2_gen->p4();
  mcEvent->p4Nu1_gen = p4Nu1_gen->p4();
  mcEvent->p4Nu2_gen = p4Nu2_gen->p4();

  
  mcEvent->QLep1_gen = p4Lep1_gen->charge();
  mcEvent->QLep2_gen = p4Lep2_gen->charge();
  mcEvent->pdgId_Lep1_gen = p4Lep1_gen->pdgId();
  mcEvent->pdgId_Lep2_gen = p4Lep2_gen->pdgId();

  mcEvent->procId = genEvent->signal_process_id();

  mcEvent->mcPrimV = p4Higgs_gen->vertex();

  //mcEvent->DecChan;                         
  // =0 for 2e
  // =1 for 2mu
  // =2 for e + mu
  // =3 for mu + e
  // =4 tau(s)
  if(abs(p4Lep1_gen->pdgId()) == abs(p4Lep2_gen->pdgId())){

    if(abs(p4Lep1_gen->pdgId()) == 11)
      mcEvent->DecChan = 0;

    else if(abs(p4Lep1_gen->pdgId()) == 13)
      mcEvent->DecChan = 1;

    else
      mcEvent->DecChan = 4;
  }
  else if(abs(p4Lep1_gen->pdgId()) == 15 
	  || abs(p4Lep2_gen->pdgId()) == 15)
    mcEvent->DecChan = 4;

  else if (((abs(p4Lep1_gen->pdgId()) == 11)
	    &&(p4Lep1_gen->pt()>p4Lep2_gen->pt())) 

	   || ((abs(p4Lep2_gen->pdgId()) == 11)
	       &&(p4Lep2_gen->pt()>p4Lep1_gen->pt())))
    mcEvent->DecChan = 2;

  else
     mcEvent->DecChan = 3;


  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!to be filled
  //mcEvent->weight;


#if DEBUG
  cout << "\n\tVBFFillMCEvent::FillMC...\t\tDONE\n\n";
#endif

}


//print properties of the first particles
void VBFFillMCEvent::Print(const HepMC::GenEvent* genEvent,
			   edm::Handle<reco::CandidateCollection> &genParticles,
			   int nParticles){

#if DEBUG
  cout << "\n\tVBFFillMCEvent::Print\n";
#endif

  cout << setw(7) << "No" 
       << setw(7) << "Id" 
       << setw(7) << "KS"
       << setw(7) << "#mo"
       << setw(7) << "mo1Id"
       << setw(7) << "mo2Id"
       << setw(7) << "#dau"
       << setw(7) << "dau1id"
       << setw(7) << "dau2id"
       << setw(7) << "dau3id"

       << "\n";

  if(nParticles == 0)
    nParticles = genParticles->size();

  int i =1;
  for(reco::CandidateCollection::const_iterator paIt = genParticles->begin(); 
      //paIt != genParticles->end();
      i <= nParticles;
      ++paIt){ //Loop over particles

    cout << setw(7) << i
	 << setw(7) << paIt->pdgId()
	 << setw(7) << paIt->status()
	 << setw(7) << paIt->numberOfMothers();
      
    if(paIt->mother(0))
      cout << setw(7) << paIt->mother(0)->pdgId();
    else
      cout << setw(7) << "--";

    if(paIt->mother(1))
      cout << setw(7) << paIt->mother(1)->pdgId();
    else
      cout << setw(7) << "--";

    cout << setw(7) << paIt->numberOfDaughters();

    if(paIt->daughter(0))
      cout << setw(7) << paIt->daughter(0)->pdgId();
    else
      cout << setw(7) << "--";

    if(paIt->daughter(1))
      cout << setw(7) << paIt->daughter(1)->pdgId();
    else
      cout << setw(7) << "--";

    if(paIt->daughter(2))
      cout << setw(7) << paIt->daughter(2)->pdgId();
    else
      cout << setw(7) << "--";

    cout << "\n";

    i++;
  }


#if DEBUG
  cout << "\n\tVBFFillMCEvent::Print...\t\tDONE\n\n";
#endif

}

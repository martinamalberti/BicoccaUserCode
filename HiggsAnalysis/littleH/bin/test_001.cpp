/*
Test per Onia2EE!!!

compiling:
c++ -o test_001 `root-config --cflags --glibs` -lGenVector test_001.cpp \
   PhysicsTools/NtupleUtils/bin/dict.cpp PhysicsTools/NtupleUtils/bin/treeReader.cc

running example:
./testReader ../Ntuple/900GeV/Upsilon1StoEE/SimpleTree_Upsilon_900GeV.root SimpleNtple/SimpleTree

PIETRO WAS HERE

*/

#include "PhysicsTools/NtupleUtils/bin/treeReader.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TROOT.h"

using namespace std;

int main (int argc, char ** argv)
{
  TFile f (argv[1]) ;
  TTree * tr = (TTree*) f.Get (argv[2]) ;

  treeReader reader (tr) ;
  
  TH1F triggerBit ("triggerBit", "triggerBit", 5, 0, 5) ;
  
  /// SELECTION CUTS ///
  int MIN_nhits_trk = 12;   
  int MIN_nhits_pixel = 2;

  float MAX_normchi2_trk = 5.0; 
  float MAX_normchi2_glb = 20.0;
  float MAX_dz_trk = 20.0;
  float MIN_vtxprob_jpsi = 0.01;

  /// B-meson/baryon masses
  const float MuMuNomMass = 10.0;
  const float B0NomMass = 5.27953;
  const float BpNomMass = 5.27915;
  const float BsNomMass = 5.3663;
  const float LbNomMass = 5.6202;

  int nentries = (int) tr->GetEntries();

  //PG loop over the events
  cout << "Number of entries = " << nentries << endl;

  // counters
  int totalEvents = 0;
  int passedCandidates = 0;

  for (int iEvent = 0 ; iEvent < tr->GetEntries () ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent%100 == 0) cout << ">>> Processing event # " << iEvent << endl;

      totalEvents++;

      /// TRIGGER CUTS 
      //Trigger Leg:     
      //string HLTbitNames[NHLTTRIGGERS] = {"HLT_Mu3", "HLT_Mu5", "HLT_Mu9", "HLT_DoubleMu0", "HLT_DoubleMu3",   "HLT_Ele10_LW_L1R", "HLT_DoubleEle5_SW_L1R"};
      //string L1TbitNames[NL1TTRIGGERS]  = {"HLT_L1MuOpen","HLT_L1Mu", "HLT_L1DoubleMuOpen", "HLT_L1SingleEG5", "HLT_L1SingleEG8", "HLT_L1DoubleEG5"};
      std::vector<int>* HLTBits_accept = reader.GetInt("HLTBits_accept");
      
      triggerBit.Fill (HLTBits_accept->size ()) ;
      if (HLTBits_accept->size () && !HLTBits_accept->at (0)) continue; 



      std::vector<float>* electrons_charge= reader.GetFloat("electrons_charge");
      //       cout << electrons_charge->size() << endl ;
    } //PG loop over the events

  TCanvas c1 ;
  gROOT->SetStyle ("Plain") ;
  triggerBit.Draw () ;
  c1.Print ("triggerBit.gif","gif") ;

  return 0 ;
}
/*
test the histogram tools
c++ -o test_003 `root-config --cflags --glibs` -lGenVector test_003.cpp \
    PhysicsTools/NtupleUtils/bin/hChain.cc \
    PhysicsTools/NtupleUtils/bin/h2Chain.cc \
    PhysicsTools/NtupleUtils/bin/hFactory.cc \
    PhysicsTools/NtupleUtils/bin/h2Factory.cc \
    PhysicsTools/NtupleUtils/bin/dict.cpp \
    PhysicsTools/NtupleUtils/bin/treeReader.cc

running example:
./test_003 data.root tree/name

*/

#include "PhysicsTools/NtupleUtils/bin/hFactory.h"
#include "PhysicsTools/NtupleUtils/bin/treeReader.h"
#include "PhysicsTools/NtupleUtils/bin/hFactory.h"
#include "PhysicsTools/NtupleUtils/bin/hFunctions.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int main (int argc, char ** argv)
{
  //PG prepare hitograms
  hFactory factory ("trivialHistos.root", true) ;
  int numOfPlots = 2 ;
  factory.add_h1 ("ele_eta", "electrons eta", 200, -3, 3, numOfPlots) ;
  factory.add_h1 ("tracks_eta", "tracks eta", 200, -3, 3, numOfPlots) ;
  factory.add_h1 ("objects_num", "number of objects per entry", 50, 0, 50, 5) ;

  //PG read the ntuple file
  TFile f (argv[1]) ;
  TTree * tr = (TTree*) f.Get (argv[2]) ;
  treeReader reader (tr) ;

  //PG loop over the events
  for (int iEvent = 0 ; iEvent < tr->GetEntries () ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;

      factory.Fill ("objects_num", 0, reader.Get4V ("electrons")->size ()) ;    
      factory.Fill ("objects_num", 1, reader.Get4V ("muons")->size ()) ;    
      factory.Fill ("objects_num", 2, reader.Get3V ("tracks_in")->size ()) ;    
      factory.Fill ("objects_num", 3, reader.GetFloat ("SC_Energy")->size ()) ;    
      factory.Fill ("objects_num", 4, reader.GetFloat ("MC_pdgID")->size ()) ;    

      int nEle = reader.Get4V ("electrons")->size () ;
      for (int iEle = 0 ; iEle < nEle ; ++iEle)
        {
          double eta = reader.Get4V ("electrons")->at (iEle).eta () ;     
          factory.Fill ("ele_eta", 0, eta) ;    
          if (nEle == 2) factory.Fill ("ele_eta", 1, eta) ;    
        }

      int nTracks = reader.Get3V ("tracks_in")->size () ;
      for (int iTrack = 0 ; iTrack < nTracks ; ++iTrack)
        {
          double eta = reader.Get3V ("tracks_in")->at (iTrack).eta () ;     
          factory.Fill ("tracks_eta", 0, eta) ;    
          if (nTracks == 2) factory.Fill ("tracks_eta", 1, eta) ;    
        }
    } //PG loop over the events
    

  int colors[5] = {1,2,3,4,6} ;
  factory["objects_num"]->SetColors (std::vector<int> (colors, colors+5)) ;
  factory["objects_num"]->Print () ;  
  return 0 ;

}
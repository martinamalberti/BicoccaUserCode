/*
test the histogram tools
c++ -o test_004 `root-config --cflags --glibs` -lGenVector test_004.cpp \
    PhysicsTools/NtupleUtils/bin/hChain.cc \
    PhysicsTools/NtupleUtils/bin/h2Chain.cc \
    PhysicsTools/NtupleUtils/bin/hFactory.cc \
    PhysicsTools/NtupleUtils/bin/h2Factory.cc \
    PhysicsTools/NtupleUtils/bin/dict.cpp \
    PhysicsTools/NtupleUtils/bin/treeReader.cc

running example:
./test_004 data.root tree/name

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
  int numOfPlots = 1 ;
  factory.add_h1 ("QQ_num", "average number of QQ candidates", 10, 0, 10, numOfPlots) ;
  factory.add_h1 ("QQ_M", "QQ mass", 200, 3, 20, numOfPlots) ;

  //PG read the ntuple file
  TFile f (argv[1]) ;
  TTree * tr = (TTree*) f.Get (argv[2]) ;
  treeReader reader (tr) ;

  //PG loop over the events
  for (int iEvent = 0 ; iEvent < tr->GetEntries () ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      factory.Fill ("QQ_num", 0, reader.GetInt ("QQ_size")->at (0)) ;
      // loop over QQ candidates
      for (int iCand = 0 ; iCand < reader.GetInt ("QQ_size")->at (0) ; ++iCand)
        {        
          factory.Fill ("QQ_M", 0, reader.Get4V ("QQ_4mom")->at (iCand).M ()) ;        
        }
    
    } //PG loop over the events

  factory["QQ_M"]->Print () ;      
  factory["QQ_num"]->Print () ;      
  return 0 ;

}
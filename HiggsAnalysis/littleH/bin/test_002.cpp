/*
test the histogram tools
c++ -o test_002 `root-config --cflags --glibs` -lGenVector test_002.cpp \
    PhysicsTools/NtupleUtils/bin/hChain.cc \
    PhysicsTools/NtupleUtils/bin/h2Chain.cc \
    PhysicsTools/NtupleUtils/bin/hFactory.cc \
    PhysicsTools/NtupleUtils/bin/h2Factory.cc

running example:
./test_002

*/

#include "PhysicsTools/NtupleUtils/bin/hFactory.h"
#include "PhysicsTools/NtupleUtils/bin/h2Factory.h"
#include <cstdlib>

using namespace std;

int main (int argc, char ** argv)
{
  //PG prepare hitograms
  hFactory factory ("testHistos.root", true) ;
  h2Factory factory2D ("testHistos2D.root", true) ;
  int numOfPlots = 2 ;
  factory.add_h1 ("ele_eta", "electrons eta", 200, -3, 3, numOfPlots) ;
  factory2D.add_h2 ("ele_pos", "electrons position", 200, -3, 3, 200, -3.14, 3.14, numOfPlots) ;
  
  for (int iEle = 0 ; iEle < 10000 ; ++iEle)
    {
      double eta = -2.5 + rand () / (1. * RAND_MAX) * 5. ;
      factory.Fill ("ele_eta", 0, eta) ;  
      if (iEle % 2) factory.Fill ("ele_eta", 1, eta) ;  
      double phi =  -3.14 + rand () / (1. * RAND_MAX) * 6.28 ;
      factory2D.Fill ("ele_pos", 0, eta, phi) ;  
      if (iEle % 2) factory2D.Fill ("ele_pos", 1, eta, phi) ;  
    } 
  return 0 ;
}
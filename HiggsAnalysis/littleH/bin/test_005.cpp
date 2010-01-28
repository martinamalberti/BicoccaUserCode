/*
test the histogram tools
c++ -o test_005 `root-config --cflags --glibs` -lGenVector test_005.cpp \
    PhysicsTools/NtupleUtils/bin/dict.cpp \
    PhysicsTools/NtupleUtils/bin/treeReader.cc

running example:
./test_005 data.root tree/name

*/

#include "PhysicsTools/NtupleUtils/bin/hFactory.h"
#include "PhysicsTools/NtupleUtils/bin/treeReader.h"
#include "PhysicsTools/NtupleUtils/bin/hFactory.h"
#include "PhysicsTools/NtupleUtils/bin/hFunctions.h"

#include <cstdlib>
#include <iostream>
#include <TH2.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>

using namespace std;

int main (int argc, char ** argv)
{

  //PG read the ntuple file
  TFile f (argv[1]) ;
  TTree * tr = (TTree*) f.Get (argv[2]) ;
  treeReader reader (tr) ;

  TH2F * guardami = reader.PrepareTest () ;
  //PG loop over the events
  for (int iEvent = 0 ; iEvent < tr->GetEntries () ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      reader.FillTest (guardami) ;    
    } //PG loop over the events

  gROOT->SetStyle ("Plain") ;
  gStyle->SetPalette (1) ;
  TCanvas c1 ;
  c1.SetGridx () ;
  c1.SetGridy () ;
  guardami->SetStats (0) ;
  guardami->Draw ("COLZ") ;
  c1.Print ("guardami.gif", "gif") ;

  TFile g ("output.root", "recreate") ;
  g.cd () ;
  guardami->Write () ;
  g.Close () ;
  
  delete guardami ;
  
  return 0 ;

}
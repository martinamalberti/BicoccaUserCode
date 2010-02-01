
#include "MuMuLooper.hh"
#include <iostream>

int main (int argc, char ** argv) 
{
  
  TFile f (argv[1]) ;
  TTree * albero = (TTree *) f.Get (argv[2]) ;
  std::cout << "al : " << albero->GetEntries () << std::endl ;

  MuMuLooper analyzer (albero) ;
  analyzer.Loop () ;
}

/* to compile:
c++ -o test_looper_01 `root-config --glibs --cflags` \
    looper_01.cc smallHBaseClass.C test_looper_01.cpp
*/

#include "looper_01.h"
#include <iostream>


int main (int argc, char ** argv) 
{
  
  TFile f (argv[1]) ;
  TTree * albero = (TTree *) f.Get (argv[2]) ;
  std::cout << "al : " << albero->GetEntries () << std::endl ;
  looper_01 analyzer (albero) ;
  analyzer.Begin () ;
  analyzer.Loop () ;
  analyzer.Terminate () ;
}

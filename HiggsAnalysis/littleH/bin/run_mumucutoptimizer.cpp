
#include "MuMuCutOptimizer.hh"
#include <iostream>
#include <fstream>

int main (int argc, char ** argv) 
{
  /*
  char inputFileName[150];
  if ( argc < 2 ){
    std::cout << "missing argument: insert inputFile with list of root files" << std::endl; 
    return 1;
  }

  TChain *theChain = new TChain("SimpleTree");

  TList *treelist = new TList();

  for(int i=1;i<argc;i++){

    string filename(argv[i]);
    filename += "/SimpleNtple";

    cout << "Adding " << filename.c_str() << endl;

    theChain->Add(filename.c_str());
  }

  cout << "Total number of entries" << theChain->GetEntries() << endl;
  */
  
  TChain * albero = new TChain ("SimpleNtple/SimpleTree") ;
  string filename(argv[1]);
  filename += "*.root" ;
  albero -> Add(filename.c_str());
  std::cout << "al : " << albero->GetEntries () << std::endl ;

  MuMuCutOptimizer analyzer(albero);
  analyzer.Loop();
  delete albero;

  return 0;
}

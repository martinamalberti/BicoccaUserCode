
#include "MuMuLooper.hh"
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

  TChain *theChain = new TChain("OniaTree");

  TList *treelist = new TList();

  for(int i=1;i<argc;i++){

    string filename(argv[i]);
    filename += "/makeNtple";

    cout << "Adding " << filename.c_str() << endl;

    theChain->Add(filename.c_str());
  }

  cout << "Total number of entries" << theChain->GetEntries() << endl;
  */
  
  TChain * albero = new TChain ("makeNtple/OniaTree") ;
  string filename(argv[1]);
  filename += "*.root" ;
  albero -> Add(filename.c_str());
  std::cout << "al : " << albero->GetEntries () << std::endl ;

  string outputfile(argv[2]);
  outputfile += ".root" ;
  TFile *f1 = new TFile(outputfile.c_str(),"RECREATE");  

  MuMuLooper analyzer(albero);
  analyzer.Loop(filename);
  analyzer.saveHistos(f1);
  delete albero;

  return 0;
}

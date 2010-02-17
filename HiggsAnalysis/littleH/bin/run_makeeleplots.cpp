
#include "MakeElePlots.hh"
#include <iostream>
#include <fstream>


int main (int argc, char ** argv) 
{
  TChain * albero = new TChain ("SimpleNtple/SimpleTree") ;
  string filename(argv[1]);
  filename += "*.root" ;
  albero -> Add(filename.c_str());
  std::cout << "al : " << albero->GetEntries () << std::endl ;

  string outputfile(argv[2]);
  outputfile += ".root" ;
  TFile *f1 = new TFile(outputfile.c_str(),"RECREATE");  
  
  int entries = 1E8 ;
  if ( argc > 3 ) entries = 10000 ; 
  
  std::cout << "ao" << entries << std::endl ;
 
  MakeElePlots analyzer(albero);
  analyzer.Loop(entries);
  analyzer.saveHistos(f1);
  delete albero;

  return 0;
}

#include "looper_01.h"
#include <iostream>


void looper_01::Begin ()
{
  std::cout << "Begin" << std::endl ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void looper_01::Loop ()
{

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast () ;

  Long64_t nbytes = 0, nb = 0;
  //PG loop over events
  for (Long64_t jentry = 0 ; jentry < nentries ; ++jentry) 
    {
      Long64_t ientry = LoadTree (jentry) ;
      if (ientry < 0) break ;
      nb = fChain->GetEntry (jentry) ;   nbytes += nb ;
      
      std::cout << "reading " << ientry << " entry\n" ;
      // if (Cut(ientry) < 0) continue;
    } //PG loop over events

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void looper_01::Terminate ()
{
  std::cout << "Terminate" << std::endl ;
}

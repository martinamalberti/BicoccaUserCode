{
  std::ifstream inFile ("betheBlock_PbWO4.dat", std::ios::in) ;
  

  TGraph betheBlock;
  int entry = 0;

  float dummy, p, dEdX;
  while (!inFile.eof ())
  {
    inFile >> dummy;
    inFile >> p ;  
    inFile >> dummy;
    inFile >> dummy;
    inFile >> dummy;
    inFile >> dummy;
    inFile >> dummy;
    inFile >> dEdX;
    inFile >> dummy;
    inFile >> dummy;
    inFile >> dummy;

    if(p/1000. > 1 && p/1000.<10000.)
{
    betheBlock.SetPoint (entry, p/1000., dEdX) ;
    entry++;
} 

 } 

  
  betheBlock.Draw ("APL") ;
}

{
  std::ifstream inFile ("betheBlock_PbWO4.dat", std::ios::in) ;
  

  TGraph betheBlock;
  TGraph betheBlock_corrected;
  TGraph betheBlock_ion;
  TGraph betheBlock_irr;
  TGraph betheBlock_irr_corrected;
  int entry = 0;

  float dummy, p, dEdX, ion;
  while (!inFile.eof ())
  {
    inFile >> dummy;
    inFile >> p ;  
    inFile >> ion;
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
	betheBlock_ion.SetPoint (entry, p/1000., ion) ;
	betheBlock_irr.SetPoint (entry, p/1000., dEdX-ion) ;
	betheBlock_irr_corrected.SetPoint (entry, p/1000., (dEdX-ion)*0.95 ) ;
	betheBlock_corrected.SetPoint (entry, p/1000., ion + (dEdX-ion)*0.95) ;
	entry++;
      }
  } 

  
  betheBlock.Draw ("APL") ;
  betheBlock_ion.SetLineStyle (2) ;
  betheBlock_ion.SetLineColor (kRed) ;
  betheBlock_ion.Draw ("PL") ;
  betheBlock_irr.SetLineColor (kBlue) ;
  betheBlock_irr.SetLineStyle (2) ;
  betheBlock_irr.Draw ("PL") ;
  //  betheBlock_corrected.SetLineStyle (2) ;
  //  betheBlock_corrected.Draw ("PL") ;
}

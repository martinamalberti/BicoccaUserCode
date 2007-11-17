{
  double meanEBM1[75] ;
  double sigmaEBM1[75] ;
  double chisqEBM1[75] ;

  double meanEBM2[75] ;
  double sigmaEBM2[75] ;
  double chisqEBM2[75] ;

  double meanEBM3[75] ;
  double sigmaEBM3[75] ;
  double chisqEBM3[75] ;

  double meanEBM4[75] ;
  double sigmaEBM4[75] ;
  double chisqEBM4[75] ;

  //PG loop over the files
  for (int i=0 ; i<75 ; ++i)
    {
      TString filename = "SEL_first_results_" ;
      filename += i ;
      filename += "/compareEB.root" ;
      std::cout << "OPENING : " << filename << std::endl ;
      TFile current (filename) ;

      double min = 0 ;
      double max = 2 ;
      TF1 gaussEBM1 ("gaussEBM1","gaus",3) ;
      TH1F * EBM1 = (TH1F *) current.Get ("EBCompareCoeff_M1") ;
      min = EBM1->GetMean () - 2 * EBM1->GetRMS () ;
      max = EBM1->GetMean () + 2 * EBM1->GetRMS () ;
      EBM1->Fit ("gaussEBM1","QILEM","",min,max) ;
      meanEBM1[i] = EBM1->GetFunction ("gaussEBM1")->GetParameter (1) ;
      sigmaEBM1[i] = 100 * EBM1->GetFunction ("gaussEBM1")->GetParameter (2) ;
      chisqEBM1[i] = EBM1->GetFunction ("gaussEBM1")->GetChisquare () ;

      TF1 gaussEBM2 ("gaussEBM2","gaus",3) ;
      TH1F * EBM2 = (TH1F *) current->Get ("EBCompareCoeff_M2") ;
      min = EBM2->GetMean () - 2 * EBM2->GetRMS () ;
      max = EBM2->GetMean () + 2 * EBM2->GetRMS () ;
      EBM2->Fit ("gaussEBM2","QILEM","",min,max) ;
      meanEBM2[i] = EBM2->GetFunction ("gaussEBM2")->GetParameter (1) ;
      sigmaEBM2[i] = 100 * EBM2->GetFunction ("gaussEBM2")->GetParameter (2) ;
      chisqEBM2[i] = EBM2->GetFunction ("gaussEBM2")->GetChisquare () ;

      TF1 gaussEBM3 ("gaussEBM3","gaus",3) ;
      TH1F * EBM3 = (TH1F *) current->Get ("EBCompareCoeff_M3") ;
      min = EBM3->GetMean () - 2 * EBM3->GetRMS () ;
      max = EBM3->GetMean () + 2 * EBM3->GetRMS () ;
      EBM3->Fit ("gaussEBM3","QILEM","",min,max) ;
      meanEBM3[i] = EBM3->GetFunction ("gaussEBM3")->GetParameter (1) ;
      sigmaEBM3[i] = 100 * EBM3->GetFunction ("gaussEBM3")->GetParameter (2) ;
      chisqEBM3[i] = EBM3->GetFunction ("gaussEBM3")->GetChisquare () ;

      TF1 gaussEBM4 ("gaussEBM4","gaus",3) ;
      TH1F * EBM4 = (TH1F *) current->Get ("EBCompareCoeff_M4") ;
      min = EBM4->GetMean () - 2 * EBM4->GetRMS () ;
      max = EBM4->GetMean () + 2 * EBM4->GetRMS () ;
      EBM4->Fit ("gaussEBM4","QILEM","",min,max) ;
      meanEBM4[i] = EBM4->GetFunction ("gaussEBM4")->GetParameter (1) ;
      sigmaEBM4[i] = 100 * EBM4->GetFunction ("gaussEBM4")->GetParameter (2) ;
      chisqEBM4[i] = EBM4->GetFunction ("gaussEBM4")->GetChisquare () ;

      current.Close () ;


    } //PG loop over the files


  std::cout << "\nMODULE ONE\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEBM1 = 1000 ;
  int indexMinEBM1 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEBM1[i] << "\t" << chisqEBM1[i] << "\n" ;
      if (sigmaEBM1[i] < sigmaMinEBM1)
        {
          sigmaMinEBM1 = sigmaEBM1[i] ;
          indexMinEBM1 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEBM1 
            << " FOR THE SELECTION " << indexMinEBM1 << "\n" ;

  std::cout << "\nMODULE TWO\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEBM2 = 1000 ;
  int indexMinEBM2 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEBM2[i] << "\t" << chisqEBM2[i] << "\n" ;
      if (sigmaEBM2[i] < sigmaMinEBM2)
        {
          sigmaMinEBM2 = sigmaEBM2[i] ;
          indexMinEBM2 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEBM2 
            << " FOR THE SELECTION " << indexMinEBM2 << "\n" ;

  std::cout << "\nMODULE THREE\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEBM3 = 1000 ;
  int indexMinEBM3 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEBM3[i] << "\t" << chisqEBM3[i] << "\n" ;
      if (sigmaEBM3[i] < sigmaMinEBM3)
        {
          sigmaMinEBM3 = sigmaEBM3[i] ;
          indexMinEBM3 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEBM3 
            << " FOR THE SELECTION " << indexMinEBM3 << "\n" ;

  std::cout << "\nMODULE FOUR\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEBM4 = 1000 ;
  int indexMinEBM4 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEBM4[i] << "\t" << chisqEBM4[i] << "\n" ;
      if (sigmaEBM4[i] < sigmaMinEBM4)
        {
          sigmaMinEBM4 = sigmaEBM4[i] ;
          indexMinEBM4 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEBM4 
            << " FOR THE SELECTION " << indexMinEBM4 << "\n" ;

  std::cout << "\nSUMMARY \n\n" ;
  
  std::cout << "MINIMUM " << sigmaMinEBM1 
            << " FOR THE SELECTION " << indexMinEBM1 << "\n" ;

  std::cout << "MINIMUM " << sigmaMinEBM2 
            << " FOR THE SELECTION " << indexMinEBM2 << "\n" ;

  std::cout << "MINIMUM " << sigmaMinEBM3 
            << " FOR THE SELECTION " << indexMinEBM3 << "\n" ;

  std::cout << "MINIMUM " << sigmaMinEBM4 
            << " FOR THE SELECTION " << indexMinEBM4 << "\n" ;

  double assex[75] ;
  for (int i=0 ; i<75; ++i) assex[i] = i ;
  TGraph trendEBM1 (75,assex,sigmaEBM1) ;
  trendEBM1->SetMarkerStyle (20) ;
  trendEBM1->SetMarkerColor (8) ;
  TGraph trendEBM2 (75,assex,sigmaEBM2) ;
  trendEBM2->SetMarkerStyle (21) ;
  trendEBM2->SetMarkerColor (9) ;
  TGraph trendEBM3 (75,assex,sigmaEBM3) ;
  trendEBM3->SetMarkerStyle (22) ;
  trendEBM3->SetMarkerColor (28) ;
  TGraph trendEBM4 (75,assex,sigmaEBM4) ;
  trendEBM4->SetMarkerStyle (29) ;
  trendEBM4->SetMarkerColor (50) ;

  TH2F sfondo ("sfondo","resolution",76,0,76,10,0,2) ;
  sfondo->SetStats (0) ;
  gROOT->SetStyle ("Plain") ;
  
  TCanvas c1 ;
  sfondo.Draw () ;
  trendEBM1->Draw ("P") ;
  trendEBM2->Draw ("P") ;
  trendEBM3->Draw ("P") ;
  trendEBM4->Draw ("P") ;
  c1.Print ("findResolution.gif","gif") ;
}

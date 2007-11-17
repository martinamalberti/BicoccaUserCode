{
  double meanEEPR1[75] ;
  double sigmaEEPR1[75] ;
  double chisqEEPR1[75] ;

  double meanEEPR2[75] ;
  double sigmaEEPR2[75] ;
  double chisqEEPR2[75] ;

  double meanEEPR3[75] ;
  double sigmaEEPR3[75] ;
  double chisqEEPR3[75] ;

  double meanEEPR4[75] ;
  double sigmaEEPR4[75] ;
  double chisqEEPR4[75] ;

  double meanEEPR5[75] ;
  double sigmaEEPR5[75] ;
  double chisqEEPR5[75] ;

  //PG loop over the files
  for (int i=0 ; i<75 ; ++i)
    {
      TString filename = "SEL_first_results_" ;
      filename += i ;
      filename += "/compareEE.root" ;
      std::cout << "OPENING : " << filename << std::endl ;
      TFile current (filename) ;

      double min = 0 ;
      double max = 2 ;
      TF1 gaussEEPR1 ("gaussEEPR1","gaus",3) ;
      TH1F * EEPR1 = (TH1F *) current.Get ("EEPCompareCoeff_R1") ;
      min = EEPR1->GetMean () - 2 * EEPR1->GetRMS () ;
      max = EEPR1->GetMean () + 2 * EEPR1->GetRMS () ;
      EEPR1->Fit ("gaussEEPR1","QILEM","",min,max) ;
      meanEEPR1[i] = EEPR1->GetFunction ("gaussEEPR1")->GetParameter (1) ;
      sigmaEEPR1[i] = 100 * EEPR1->GetFunction ("gaussEEPR1")->GetParameter (2) ;
      chisqEEPR1[i] = EEPR1->GetFunction ("gaussEEPR1")->GetChisquare () ;

      TF1 gaussEEPR2 ("gaussEEPR2","gaus",3) ;
      TH1F * EEPR2 = (TH1F *) current->Get ("EEPCompareCoeff_R2") ;
      min = EEPR2->GetMean () - 2 * EEPR2->GetRMS () ;
      max = EEPR2->GetMean () + 2 * EEPR2->GetRMS () ;
      EEPR2->Fit ("gaussEEPR2","QILEM","",min,max) ;
      meanEEPR2[i] = EEPR2->GetFunction ("gaussEEPR2")->GetParameter (1) ;
      sigmaEEPR2[i] = 100 * EEPR2->GetFunction ("gaussEEPR2")->GetParameter (2) ;
      chisqEEPR2[i] = EEPR2->GetFunction ("gaussEEPR2")->GetChisquare () ;

      TF1 gaussEEPR3 ("gaussEEPR3","gaus",3) ;
      TH1F * EEPR3 = (TH1F *) current->Get ("EEPCompareCoeff_R3") ;
      min = EEPR3->GetMean () - 2 * EEPR3->GetRMS () ;
      max = EEPR3->GetMean () + 2 * EEPR3->GetRMS () ;
      EEPR3->Fit ("gaussEEPR3","QILEM","",min,max) ;
      meanEEPR3[i] = EEPR3->GetFunction ("gaussEEPR3")->GetParameter (1) ;
      sigmaEEPR3[i] = 100 * EEPR3->GetFunction ("gaussEEPR3")->GetParameter (2) ;
      chisqEEPR3[i] = EEPR3->GetFunction ("gaussEEPR3")->GetChisquare () ;

      TF1 gaussEEPR4 ("gaussEEPR4","gaus",3) ;
      TH1F * EEPR4 = (TH1F *) current->Get ("EEPCompareCoeff_R4") ;
      min = EEPR4->GetMean () - 2 * EEPR4->GetRMS () ;
      max = EEPR4->GetMean () + 2 * EEPR4->GetRMS () ;
      EEPR4->Fit ("gaussEEPR4","QILEM","",min,max) ;
      meanEEPR4[i] = EEPR4->GetFunction ("gaussEEPR4")->GetParameter (1) ;
      sigmaEEPR4[i] = 100 * EEPR4->GetFunction ("gaussEEPR4")->GetParameter (2) ;
      chisqEEPR4[i] = EEPR4->GetFunction ("gaussEEPR4")->GetChisquare () ;

      TF1 gaussEEPR5 ("gaussEEPR5","gaus",3) ;
      TH1F * EEPR5 = (TH1F *) current->Get ("EEPCompareCoeff_R4") ;
      min = EEPR5->GetMean () - 2 * EEPR5->GetRMS () ;
      max = EEPR5->GetMean () + 2 * EEPR5->GetRMS () ;
      EEPR5->Fit ("gaussEEPR5","QILEM","",min,max) ;
      meanEEPR5[i] = EEPR5->GetFunction ("gaussEEPR5")->GetParameter (1) ;
      sigmaEEPR5[i] = 100 * EEPR5->GetFunction ("gaussEEPR5")->GetParameter (2) ;
      chisqEEPR5[i] = EEPR5->GetFunction ("gaussEEPR5")->GetChisquare () ;

      current.Close () ;


    } //PG loop over the files


  std::cout << "\nRING ONE\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEEPR1 = 1000 ;
  int indexMinEEPR1 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEEPR1[i] << "\t" << chisqEEPR1[i] << "\n" ;
      if (sigmaEEPR1[i] < sigmaMinEEPR1)
        {
          sigmaMinEEPR1 = sigmaEEPR1[i] ;
          indexMinEEPR1 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEEPR1 
            << " FOR THE SELECTION " << indexMinEEPR1 << "\n" ;

  std::cout << "\nRING TWO\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEEPR2 = 1000 ;
  int indexMinEEPR2 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEEPR2[i] << "\t" << chisqEEPR2[i] << "\n" ;
      if (sigmaEEPR2[i] < sigmaMinEEPR2)
        {
          sigmaMinEEPR2 = sigmaEEPR2[i] ;
          indexMinEEPR2 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEEPR2 
            << " FOR THE SELECTION " << indexMinEEPR2 << "\n" ;

  std::cout << "\nRING THREE\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEEPR3 = 1000 ;
  int indexMinEEPR3 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEEPR3[i] << "\t" << chisqEEPR3[i] << "\n" ;
      if (sigmaEEPR3[i] < sigmaMinEEPR3)
        {
          sigmaMinEEPR3 = sigmaEEPR3[i] ;
          indexMinEEPR3 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEEPR3 
            << " FOR THE SELECTION " << indexMinEEPR3 << "\n" ;

  std::cout << "\nRING FOUR\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEEPR4 = 1000 ;
  int indexMinEEPR4 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEEPR4[i] << "\t" << chisqEEPR4[i] << "\n" ;
      if (sigmaEEPR4[i] < sigmaMinEEPR4)
        {
          sigmaMinEEPR4 = sigmaEEPR4[i] ;
          indexMinEEPR4 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEEPR4 
            << " FOR THE SELECTION " << indexMinEEPR4 << "\n" ;

  std::cout << "\nRING FIVE\n" ;
  std::cout << "==========\n\n" ;

  double sigmaMinEEPR5 = 1000 ;
  int indexMinEEPR5 = -1 ;
  //PG print the table
  for (int i=0 ; i<75 ; ++i)
    {
      std::cout << sigmaEEPR5[i] << "\t" << chisqEEPR5[i] << "\n" ;
      if (sigmaEEPR5[i] < sigmaMinEEPR5)
        {
          sigmaMinEEPR5 = sigmaEEPR5[i] ;
          indexMinEEPR5 = i ;
        }    
    }
  std::cout << "MINIMUM " << sigmaMinEEPR5 
            << " FOR THE SELECTION " << indexMinEEPR5 << "\n" ;

  std::cout << "\nSUMMARY \n\n" ;
  
  std::cout << "MINIMUM " << sigmaMinEEPR1 
            << " FOR THE SELECTION " << indexMinEEPR1 << "\n" ;

  std::cout << "MINIMUM " << sigmaMinEEPR2 
            << " FOR THE SELECTION " << indexMinEEPR2 << "\n" ;

  std::cout << "MINIMUM " << sigmaMinEEPR3 
            << " FOR THE SELECTION " << indexMinEEPR3 << "\n" ;

  std::cout << "MINIMUM " << sigmaMinEEPR4 
            << " FOR THE SELECTION " << indexMinEEPR4 << "\n" ;

  std::cout << "MINIMUM " << sigmaMinEEPR5 
            << " FOR THE SELECTION " << indexMinEEPR5 << "\n" ;

  double assex[75] ;
  for (int i=0 ; i<75; ++i) assex[i] = i ;
  TGraph trendEEPR1 (75,assex,sigmaEEPR1) ;
  trendEEPR1->SetMarkerStyle (20) ;
  trendEEPR1->SetMarkerColor (8) ;
  TGraph trendEEPR2 (75,assex,sigmaEEPR2) ;
  trendEEPR2->SetMarkerStyle (21) ;
  trendEEPR2->SetMarkerColor (7) ;
  TGraph trendEEPR3 (75,assex,sigmaEEPR3) ;
  trendEEPR3->SetMarkerStyle (22) ;
  trendEEPR3->SetMarkerColor (28) ;
  TGraph trendEEPR4 (75,assex,sigmaEEPR4) ;
  trendEEPR4->SetMarkerStyle (29) ;
  trendEEPR4->SetMarkerColor (50) ;
  TGraph trendEEPR5 (75,assex,sigmaEEPR5) ;
  trendEEPR5->SetMarkerStyle (23) ;
  trendEEPR5->SetMarkerColor (9) ;

  TH2F sfondo ("sfondo","resolution",76,0,76,10,0,2) ;
  sfondo->SetStats (0) ;
  gROOT->SetStyle ("Plain") ;
  
  TCanvas c1 ;
  sfondo.Draw () ;
  trendEEPR1->Draw ("P") ;
  trendEEPR2->Draw ("P") ;
  trendEEPR3->Draw ("P") ;
  trendEEPR4->Draw ("P") ;
  trendEEPR5->Draw ("P") ;
  c1.Print ("findResolutionEE.gif","gif") ;
}

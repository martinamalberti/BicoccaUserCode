void Kolmogorov () {

  gROOT->ProcessLine(".x Style.C") ;

  const int n_bkg_SM = 2 ;
  const int n_colors = 2 ;

  std::vector<TString> v_bkg_SM_names;

  std::vector<TString> v_leg_bkg_SM_names;

  std::TString s_histo_input ;
  std::TString s_plot_name ;

  v_bkg_SM_names.push_back("MuMuBkg_newCuts_LOOSE") ;
  v_bkg_SM_names.push_back("MuMuBkg_newCuts");
  
  v_leg_bkg_SM_names.push_back("Comb #mu#mu HLT_loose") ;
  v_leg_bkg_SM_names.push_back("Comb #mu#mu HLT") ;
  
  double IntLum = 500. ; // In Inverse pb
  TString Lum = "500";
  TString tanB = "10";
  
  int bkg_SM_colors [n_colors] = { 2, 4 } ; 

  int bkg_SM_markers [n_bkg_SM] = { 20, 21} ;

  TH1F * h_bkg_SM_2mumass [n_bkg_SM] ;

  TCanvas *can = new TCanvas("can", "can",4,48,800,658);
  TLegend *leg = new TLegend(0.2123116,0.6412698,0.5565327,0.9174603,NULL,"brNDC");;
  
  THStack *a_2muMass = new THStack(); ;

  TH1F *samesign = new TH1F();
  TH1F *oppositesign = new TH1F();

  //SM BKG
  for ( int iSM = 0; iSM < n_bkg_SM; iSM++ )	
  {
    TFile *bkg_fin = new TFile ( "../" + v_bkg_SM_names[iSM] + ".root", "READ" ) ;
            
    s_histo_input = "hInvMass" ;
    h_bkg_SM_2mumass [iSM] = (TH1F *) bkg_fin-> Get (s_histo_input) ;
 
   if(iSM==0)
      h_bkg_SM_2mumass [iSM]->Scale(0.135);

    h_bkg_SM_2mumass[iSM]->Rebin(2);
    h_bkg_SM_2mumass [iSM]->Sumw2(); 

      //  ( *h_bkg_SM_2mumass [iSM] ) =  ( *h_bkg_SM_2mumass[iSM] )*0.267;
    //     ( *h_bkg_SM_2mumass [iSM] ) =  ( *h_bkg_SM_2mumass[iSM] )*(1/h_bkg_SM_2mumass[iSM]->GetEntries()) ; //rescaling with the event weight 

    cout << "entries " << h_bkg_SM_2mumass[iSM]->GetEntries() << endl;
    h_bkg_SM_2mumass [iSM] ->SetMarkerStyle(bkg_SM_markers[iSM]) ;
    h_bkg_SM_2mumass [iSM]->SetMarkerColor ( bkg_SM_colors[iSM] ) ;
    h_bkg_SM_2mumass [iSM]->SetMarkerSize(1.6) ;
    h_bkg_SM_2mumass [iSM]->SetLineColor ( bkg_SM_colors[iSM] ) ;
    h_bkg_SM_2mumass [iSM]->SetLineWidth(2.5) ;
    h_bkg_SM_2mumass [iSM] -> SetFillColor (bkg_SM_colors [iSM]) ;
    h_bkg_SM_2mumass [iSM] -> SetFillStyle (3018) ;

//       if(iSM==0){
// 	samesign = *h_bkg_SM_2mumass [iSM];
// 	samesign->Sumw2();          
//       }
//       if(iSM==1){
// 	oppositesign = *h_bkg_SM_2mumass [iSM]; 
// 	oppositesign->Sumw2();
//       }
    leg->AddEntry( h_bkg_SM_2mumass [iSM] ,v_leg_bkg_SM_names [iSM] ,"f");
      
    a_2muMass->Add(h_bkg_SM_2mumass [iSM]);

    if(iSM==1)
      std::cout << h_bkg_SM_2mumass [iSM]->KolmogorovTest(h_bkg_SM_2mumass [iSM-1]) << std::endl;
  }      
  a_2muMass ->Draw("nostack") ;

  a_2muMass ->GetYaxis()->SetTitle("d#sigma/dm (pb/100 MeV/c^{2})") ;
  a_2muMass ->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})") ;
  a_2muMass ->GetXaxis()->SetRangeUser(0,500) ;
  leg->Draw() ;
    
//     samesign->Sumw2();     
//     oppositesign->Sumw2();  
  if(iSM==1)
    std::cout << h_bkg_SM_2mumass [iSM]->KolmogorovTest(h_bkg_SM_2mumass [iSM-1], "UO") << std::endl;
//   std::cout << Lum << endl;
//   s_plot_name = "~/www/little" + "_" + Lum + "pb_" + "tanB"+ tanB + "HLT.gif" ;
//     can-> Print ( s_plot_name, "gif" ) ;
//   s_plot_name = "~/www/little" + "_" + Lum + "pb_" + "tanB"+ tanB +"samesign.eps" ;
//   can-> Print ( s_plot_name, "eps" ) ;

}

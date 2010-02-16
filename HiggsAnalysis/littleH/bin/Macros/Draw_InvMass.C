void Draw_InvMass () {
  
  gROOT->ProcessLine(".x Style.C") ;
  
  const int n_signal = 2 ;
  const int n_bkg_SM = 4 ;
  const int n_colors = 4 ;
  const int n_steps = 5 ;
  
  std::vector<TString> v_signal_names;
  std::vector<TString> v_bkg_SM_names;
  
  std::vector<TString> v_leg_signal_names;
  std::vector<TString> v_leg_bkg_SM_names;
  
  std::TString s_histo_input ;
  std::TString s_plot_name ;
  
  v_signal_names.push_back("Signal_8GeV");
  v_signal_names.push_back("Signal_10GeV") ; 
  
  v_leg_signal_names.push_back("a_8GeV") ; 
  v_leg_signal_names.push_back("a_10GeV") ;
  
  v_bkg_SM_names.push_back("MuMuBkg_HLT") ;
  v_bkg_SM_names.push_back("Upsilon1S_HLT") ;
  v_bkg_SM_names.push_back("Upsilon2S_HLT") ;
  v_bkg_SM_names.push_back("Upsilon3S_HLT") ;
  
  v_leg_bkg_SM_names.push_back("Comb #mu#mu") ;
  v_leg_bkg_SM_names.push_back("Comb #mu#mu + #Upsilon_{1S}") ; 
  v_leg_bkg_SM_names.push_back("Comb #mu#mu + #Upsilon_{1S} + #Upsilon_{2S}") ;
  v_leg_bkg_SM_names.push_back("Comb #mu#mu + #Upsilon_{1S} + #Upsilon_{2S} + #Upsilon_{3S}") ; 
  
  double IntLum = 500. ; // In Inverse pb
  TString Lum = "500";
  TString tanB = "10";
  
  double signal_Xsec [n_signal] = { 6, 15};// <= values for tanBeta = 10,  values for tanBeta = 3 => {94.64 151.25}  ; 
  double bkg_SM_Xsec [n_bkg_SM] = { 85000000., 14000., 5600., 1600.  } ;
  
  double signal_filter_eff [n_signal] = { 0.8, 0.8 } ;
  double bkg_SM_filter_eff [n_bkg_SM] = { 1., 1., 1., 1. } ;
  
  double signal_genMCevts [n_signal] = { 9600., 9600. } ; 
  double bkg_SM_genMCevts [n_bkg_SM] = {  10414205., 10000., 10000., 10000. } ;
  
  double signal_weight [n_signal] ;
  double bkg_SM_weight [n_bkg_SM] ;
  
  int signal_colors [n_colors] = { 1, 1} ; 
  int bkg_SM_colors [n_colors] = { 2, 4, 5, 6 } ; 
  
  int signal_markers [n_signal] = { 25, 25 } ; /*25,*/
  int bkg_SM_markers [n_bkg_SM] = { 20, 21, 22, 23 } ;
  
  TH1F * h_signal_2mumass [n_signal] ;
  TH1F * h_bkg_SM_2mumass [n_signal][n_bkg_SM] ;
  
  TCanvas * can [n_signal] ;
  TCanvas * can2 [n_signal] ;
  TLegend *leg [n_signal] ;
  
  THStack *a_2muMass [n_signal] ;
  TH1F * h_2muMass[n_signal];

  TFile *output = new TFile( "Weighted_evts_HLT.root", "RECREATE");
  
  //Cycle on Signals:  
  for ( int i = 0; i < n_signal; i++ )
    {
      TFile *signal_fin = new TFile( "../" + v_signal_names[i] + ".root", "READ");
      a_2muMass[i] = new THStack("invMass" + v_signal_names[i],"Invariant Mass");
      h_2muMass[i] = new TH1F("hInvMass" + v_signal_names[i],"hInvMass", 100, 3.2, 12.);    
      
      can[i] = new TCanvas ;
      can2[i] = new TCanvas ;
      
      can[i]->cd(0) ;
      
      leg [i] = new TLegend(0.21,0.64,0.56,0.92,NULL,"brNDC");
      leg [i] ->SetFillColor(kWhite);
      leg [i] ->SetTextSize(0.025);
      leg [i] ->SetBorderSize(0);
      
      //SM BKG
      for ( int iSM = 0; iSM < n_bkg_SM; iSM++ )	
	
	{
	  TFile *bkg_fin = new TFile ( "../" + v_bkg_SM_names[iSM] + ".root", "READ" ) ;
	  
	  bkg_SM_weight [iSM] = IntLum * bkg_SM_Xsec [iSM] * bkg_SM_filter_eff [iSM] / bkg_SM_genMCevts [iSM] ;
	  
	  std::cout << bkg_SM_weight [iSM] << " " + v_bkg_SM_names [iSM] + " " << bkg_SM_weight [iSM] * bkg_SM_genMCevts [iSM] << "\n \n"  << std::endl;
	  
	  s_histo_input = "hInvMass" ;
	  h_bkg_SM_2mumass [i][iSM] = (TH1F *) bkg_fin-> Get (s_histo_input) ;
	  //	h_bkg_SM_2mumass [i][iSM]->Rebin(2);
	  //if (i==0) 
	  ( *h_bkg_SM_2mumass [i][iSM] ) = bkg_SM_weight [iSM] * ( *h_bkg_SM_2mumass [i][iSM] ) ; //rescaling with the event weight 
	  
	  h_bkg_SM_2mumass [i][iSM] ->SetMarkerStyle(bkg_SM_markers[iSM]) ;
	  h_bkg_SM_2mumass [i][iSM]->SetMarkerColor ( bkg_SM_colors[iSM] ) ;
	  h_bkg_SM_2mumass [i][iSM]->SetMarkerSize(1.6) ;
	  h_bkg_SM_2mumass [i][iSM]->SetLineColor ( bkg_SM_colors[iSM] ) ;
	  h_bkg_SM_2mumass [i][iSM]->SetLineWidth(2.5) ;
	  h_bkg_SM_2mumass [i][iSM] -> SetFillColor (bkg_SM_colors [iSM]) ;
	  h_bkg_SM_2mumass [i][iSM] -> SetFillStyle (3018) ;
	  
	  leg[i]->AddEntry( h_bkg_SM_2mumass [i][iSM] ,v_leg_bkg_SM_names [iSM] ,"f");
	  
	  a_2muMass[i]->Add(h_bkg_SM_2mumass [i][iSM]);
	  h_2muMass[i]->Add(h_bkg_SM_2mumass [i][iSM]);	  
	}      
      //SIGNALS
      signal_weight [i] = IntLum * signal_Xsec [i] * signal_filter_eff [i] / signal_genMCevts [i] ;
      
      std::cout << signal_weight [i] << " " + v_signal_names [i] + " " << signal_weight [i] * signal_genMCevts [i] << "\n \n"  << std::endl;
      
      s_histo_input = "hInvMass";
      h_signal_2mumass [i] = (TH1F *) signal_fin-> Get (s_histo_input) ;
      
      ( *h_signal_2mumass [i] ) = signal_weight [i] * ( *h_signal_2mumass [i] ) ; //rescaling with the event weight 
      
      h_signal_2mumass [i] ->SetMarkerStyle(signal_markers[i]) ;
      h_signal_2mumass [i]->SetMarkerColor ( signal_colors[i] ) ;
      h_signal_2mumass [i]->SetMarkerSize(1.6) ;
      h_signal_2mumass [i]->SetLineColor ( signal_colors[i] ) ;
      h_signal_2mumass [i]->SetLineWidth(2.5) ;
      h_signal_2mumass [i]->SetFillColor (signal_colors [i]) ;
      
      leg[i]->AddEntry( h_signal_2mumass [i] ,v_leg_signal_names [i] ,"f");
      
      a_2muMass[i]->Add(h_signal_2mumass [i]);
      h_2muMass[i]->Add(h_signal_2mumass [i]);       
      
      a_2muMass[i] ->Draw() ;
      
      a_2muMass[i] ->GetYaxis()->SetTitle("d#sigma/dm (pb/100 MeV/c^{2})") ;
      a_2muMass[i] ->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})") ;
      a_2muMass[i] ->GetXaxis()->SetRangeUser(0,500) ;
      
      leg [i] ->Draw() ;
      s_plot_name = "~/www/little" + v_leg_signal_names [i] +"_" + Lum + "pb_" + "tanB"+ tanB + "_HLT.gif" ;
      can [i] -> Print ( s_plot_name, "gif" ) ;
      //     s_plot_name = "~/www/little" + v_leg_signal_names [i] + "_" + Lum + "pb_" + "tanB"+ tanB +"_noHLTWeighted.eps" ;
      //     can [i] -> Print ( s_plot_name, "eps" ) ;
      output->cd();
      
      can2[i]->cd();
      h_2muMass[i] ->Draw();
      h_2muMass[i] ->Write() ;
      
    }     
}
 

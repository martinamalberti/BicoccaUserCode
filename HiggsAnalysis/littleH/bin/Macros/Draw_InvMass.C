void Draw_InvMass () {
  
  gROOT->ProcessLine(".x Style.C") ;
  
  const int n_signal = 8 ;
  const int n_bkg_SM = 4 ;
  const int n_colors = 8 ;
  
  std::vector<TString> v_signal_names;
  std::vector<TString> v_bkg_SM_names;
  
  std::vector<TString> v_leg_signal_names;
  std::vector<TString> v_leg_bkg_SM_names;
  
  std::TString s_histo_input ;
  std::TString s_plot_name ;
  
  v_signal_names.push_back("Signal_5GeV_glbglb");
  v_signal_names.push_back("Signal_5GeV_glbglb");
  v_signal_names.push_back("Signal_6GeV_glbglb") ; 
  v_signal_names.push_back("Signal_7GeV_glbglb") ; 
  v_signal_names.push_back("Signal_8GeV_glbglb");
  v_signal_names.push_back("Signal_9GeV_glbglb") ; 
  v_signal_names.push_back("Signal_10GeV_glbglb") ; 
  v_signal_names.push_back("Signal_105GeV_glbglb") ; 

  v_leg_signal_names.push_back("a_5GeV") ;
  v_leg_signal_names.push_back("a_5GeV") ; 
  v_leg_signal_names.push_back("a_6GeV") ;  
  v_leg_signal_names.push_back("a_7GeV") ; 
  v_leg_signal_names.push_back("a_8GeV") ; 
  v_leg_signal_names.push_back("a_9GeV") ; 
  v_leg_signal_names.push_back("a_10GeV") ;
  v_leg_signal_names.push_back("a_10.5GeV") ;  

  //v_bkg_SM_names.push_back("MuMuBkg_glbglb");
  v_bkg_SM_names.push_back("ContGenBkg_glbglb") ;
   //  v_bkg_SM_names.push_back("MuMuBkg_P_Loose_Rescaled") ;
  v_bkg_SM_names.push_back("Upsilon1S_glbglb") ;
  v_bkg_SM_names.push_back("Upsilon2S_glbglb") ;
  v_bkg_SM_names.push_back("Upsilon3S_glbglb") ;
  
  v_leg_bkg_SM_names.push_back("Comb #mu#mu") ;
  v_leg_bkg_SM_names.push_back("Comb #mu#mu + #Upsilon_{1S}") ; 
  v_leg_bkg_SM_names.push_back("Comb #mu#mu + #Upsilon_{1S} + #Upsilon_{2S}") ;
  v_leg_bkg_SM_names.push_back("Comb #mu#mu + #Upsilon_{1S} + #Upsilon_{2S} + #Upsilon_{3S}") ; 
  
  double IntLum = 500. ; // In Inverse pb
  TString Lum = "500";
  TString tanB = "10";

  double signal_mass [n_signal] = { 5., 5., 6., 7., 8., 9., 10., 10.5};  
  double signal_Xsec [n_signal] = { 6.6, 6.6, 6.4, 6.4, 6.6, 9., 15, 10.};// <= values for tanBeta = 10
  double bkg_SM_Xsec [n_bkg_SM] = { 85000000., 14000., 5600., 1600.  } ;
  
  double signal_filter_eff [n_signal] = { 0., 1., 1., 1., 1., 1., 1., 1. } ;
  double bkg_SM_filter_eff [n_bkg_SM] = { 1., 1., 1., 1. } ;
  
  double signal_genMCevts [n_signal] = { 19800., 19800., 19600., 18600., 18600., 18800., 17800., 19800. } ; 
  double bkg_SM_genMCevts [n_bkg_SM] = {  10414205., 100000., 100000., 100000. } ;
  
  double signal_weight [n_signal] ;
  double bkg_SM_weight [n_bkg_SM] ;
  
  int signal_colors [n_colors] = { 1, 1, 1, 1, 1, 1, 1, 1} ; 
  int bkg_SM_colors [n_colors] = { 2, 4, 5, 6 } ; 
  
  int signal_markers [n_signal] = { 25, 25, 25, 25, 25, 25,  25, 25} ; /*25,*/
  int bkg_SM_markers [n_bkg_SM] = { 20, 21, 22, 23 } ;
  
  TH1F * h_signal_2mumass [n_signal] ;
  TH1F * h_bkg_SM_2mumass [n_signal][n_bkg_SM] ;
  
  TCanvas * can [n_signal] ;
  TCanvas * can2 [n_signal] ;
  TLegend *leg [n_signal] ;
  
  THStack *a_2muMass [n_signal] ;
  TH1F * h_2muMass[n_signal];
   
  //Cycle on Signals:  
  for ( int i = 0; i < n_signal; i++ )
    {
      TFile *signal_fin = new TFile( "../" + v_signal_names[i] + ".root", "READ");
      a_2muMass[i] = new THStack("invMass" + v_signal_names[i],"Invariant Mass");
      h_2muMass[i] = new TH1F("hInvMass" + v_signal_names[i],"hInvMass", 100, 4., 12.);    
      
      can[i] = new TCanvas ;
      can2[i] = new TCanvas ;
      
      can[i]->cd(0) ;
      //      can[i]->SetLogy();     
      leg [i] = new TLegend(0.21,0.64,0.56,0.92,NULL,"brNDC");
      leg [i] ->SetFillColor(kWhite);
      leg [i] ->SetTextSize(0.025);
      leg [i] ->SetBorderSize(0);

      signal_weight [i] = IntLum * signal_Xsec [i] * signal_filter_eff [i] / signal_genMCevts [i] ;
      s_histo_input = "hInvMass";

      h_signal_2mumass [i] = (TH1F *) signal_fin-> Get (s_histo_input) ;
      h_signal_2mumass [i]->Rebin(2);
      //      h_signal_2mumass [i]->Sumw2();
      ( *h_signal_2mumass [i] ).Scale(signal_weight [i]) ; //rescaling with the event weight 
      cout << signal_weight [i] << " " + v_signal_names [i] +"  " <<  h_signal_2mumass [i]->Integral(h_signal_2mumass [i]->FindBin(signal_mass[i])-1,h_signal_2mumass [i]->FindBin(signal_mass[i])+1 ) << " counts \n \n"  << std::endl;

      //SM BKG
      for ( int iSM = 0; iSM < n_bkg_SM; iSM++ )		
	{
	  TFile *bkg_fin = new TFile ( "../" + v_bkg_SM_names[iSM] + ".root", "READ" ) ;
	  
	  s_histo_input = "hInvMass" ;
	    
	  bkg_SM_weight [iSM] = IntLum * bkg_SM_Xsec [iSM] * bkg_SM_filter_eff [iSM] / bkg_SM_genMCevts [iSM] ;
	  
	  h_bkg_SM_2mumass [i][iSM] = (TH1F *) bkg_fin-> Get (s_histo_input) ;

	  //	  ( *h_bkg_SM_2mumass [i][iSM] ).Sumw2();
	  if(!v_bkg_SM_names[iSM].Contains("Gen")){
	    h_bkg_SM_2mumass [i][iSM]->Rebin(2);
	    ( *h_bkg_SM_2mumass [i][iSM] ).Scale(bkg_SM_weight [iSM]);
	  }

	  std::cout << bkg_SM_weight [iSM] << " " + v_bkg_SM_names [iSM] + " "  << h_bkg_SM_2mumass [i][iSM]->Integral(h_signal_2mumass [i]->FindBin(signal_mass[i])-1,h_signal_2mumass [i]->FindBin(signal_mass[i])+1 )<< " counts \n \n"  << std::endl;	  
	
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
      
      a_2muMass[i] ->GetYaxis()->SetTitle("Events/0.08 GeV/c^{2}") ;
      a_2muMass[i] ->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})") ;
      a_2muMass[i] ->GetXaxis()->SetRangeUser(0,500) ;
      
      leg [i] ->Draw() ;
      s_plot_name = "~/www/little" + v_leg_signal_names [i] +"_" + Lum + "pb" + + v_bkg_SM_names [0]+".gif" ;
      can [i] -> Print ( s_plot_name, "gif" ) ;
      //      s_plot_name = "~/www/little" + v_leg_signal_names [i] + "_" + Lum + "pb_" + "tanB"+ tanB +"_noHLTWeighted.eps" ;
      //     can [i] -> Print ( s_plot_name, "eps" ) ;
      TFile *output = new TFile( "../PseudoData"+ v_bkg_SM_names [0] + "_"+  v_signal_names[i]+"_glbglb.root", "RECREATE");
      if(signal_filter_eff[i]==0) 	  TFile *output = new TFile( "../PseudoData"+ v_bkg_SM_names [0] + "_NoSignal_glbglb.root", "RECREATE");
      
      output->cd();

      can2[i]->cd();
      h_2muMass[i] ->Draw();
      h_2muMass[i] ->Write() ;
      
    }     
}


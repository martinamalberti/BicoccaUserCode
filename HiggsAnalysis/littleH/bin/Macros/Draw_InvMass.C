void Draw_InvMass () {

  gROOT->ProcessLine(".x Style.C") ;

  const int n_bkg_SM = 4 ;
  const int n_colors = 4 ;
  const int n_steps = 5 ;

  std::vector<TString> v_bkg_SM_names;

  std::vector<TString> v_leg_bkg_SM_names;

  std::TString s_histo_input ;
  std::TString s_plot_name ;
  
  v_bkg_SM_names.push_back("ppEleX") ;
  v_bkg_SM_names.push_back("Upsilon1StoEE") ;
  v_bkg_SM_names.push_back("Upsilon2StoEE") ;
  v_bkg_SM_names.push_back("Upsilon3StoEE") ;
  
  v_leg_bkg_SM_names.push_back("Comb ee") ;
  v_leg_bkg_SM_names.push_back("Comb ee + #Upsilon_{1S}") ; 
  v_leg_bkg_SM_names.push_back("Comb ee + #Upsilon_{1S} + #Upsilon_{2S}") ;
  v_leg_bkg_SM_names.push_back("Comb ee + #Upsilon_{1S} + #Upsilon_{2S} + #Upsilon_{3S}") ; 

//   v_leg_bkg_SM_names.push_back("Comb ee") ;
//   v_leg_bkg_SM_names.push_back("#Upsilon_{1S}") ; 
//   v_leg_bkg_SM_names.push_back("#Upsilon_{1S} + #Upsilon_{2S}") ;
//   v_leg_bkg_SM_names.push_back("#Upsilon_{1S} + #Upsilon_{2S} + #Upsilon_{3S}") ; 
  
  double IntLum = 500. ; // In Inverse pb
  TString Lum = "500";
  TString tanB = "10";
  
  TString s_HLT = "" ; 

  double bkg_SM_Xsec [n_bkg_SM] = { 35845600., 12179., 8767., 2323.  } ;
  
  double bkg_SM_filter_eff [n_bkg_SM] = { 1., 1., 1., 1. } ;
                                        
  double bkg_SM_genMCevts [n_bkg_SM] = { 1938668. /* 1438668.*/, 100000., 100000., 96021. } ;

  double bkg_SM_weight [n_bkg_SM] ;

  int bkg_SM_colors [n_colors] = { 2, 4, 5, 6 } ; 

  int bkg_SM_markers [n_bkg_SM] = { 20, 21, 22, 23 } ;

  TH1F * h_bkg_SM_eleelemass [n_bkg_SM] ;
  TH1F * h_TOT ;

  TCanvas * can ;
  TLegend *leg ;
  
  THStack *eleeleMass ;

    eleeleMass = new THStack("invMass","Invariant Mass");
    can = new TCanvas ;
//     can[i] ->SetLogy () ;
/*    can[i]->SetGridx () ;
    can[i]->SetGridy () ;*/
    can->cd(0) ;

    leg  = new TLegend(0.21,0.64,0.56,0.92,NULL,"brNDC");
    leg  ->SetFillColor(kWhite);
    leg  ->SetTextSize(0.025);
    leg  ->SetBorderSize(0);

    //SM BKG
    for ( int iSM = 0; iSM < n_bkg_SM; iSM++ )	
      {
	if(iSM==0) TFile *bkg_fin = new TFile ( "../" + v_bkg_SM_names[iSM] + "_NoHLT.root", "READ" ) ;
	else TFile *bkg_fin = new TFile ( "../" + v_bkg_SM_names[iSM] + ".root", "READ" ) ;
	
	bkg_SM_weight [iSM] = IntLum * bkg_SM_Xsec [iSM] * bkg_SM_filter_eff [iSM] / bkg_SM_genMCevts [iSM] ;
	
	std::cout << bkg_SM_weight [iSM] << " " + v_bkg_SM_names [iSM] + " " << bkg_SM_weight [iSM] * bkg_SM_genMCevts [iSM] << "\n \n"  << std::endl;
	
	s_histo_input = "h_1_InvMass" ;
	h_bkg_SM_eleelemass [iSM] = (TH1F *) bkg_fin-> Get (s_histo_input) ;
	h_bkg_SM_eleelemass [iSM] -> Rebin(2) ;
	
	( *h_bkg_SM_eleelemass [iSM] ) = bkg_SM_weight [iSM] * ( *h_bkg_SM_eleelemass [iSM] ) ; //rescaling with the event weight 

        if ( iSM == 0 ) { 
	  ( *h_bkg_SM_eleelemass [iSM] ) = 0.025393627 * ( *h_bkg_SM_eleelemass [iSM] );
	     h_TOT = (TH1F *) bkg_fin-> Get (s_histo_input) ;
	}
	
	h_TOT->Add(h_bkg_SM_eleelemass [iSM]);

	h_bkg_SM_eleelemass [iSM] ->SetMarkerStyle(bkg_SM_markers[iSM]) ;
	h_bkg_SM_eleelemass [iSM]->SetMarkerColor ( bkg_SM_colors[iSM] ) ;
	h_bkg_SM_eleelemass [iSM]->SetMarkerSize(1.6) ;
	h_bkg_SM_eleelemass [iSM]->SetLineColor ( bkg_SM_colors[iSM] ) ;
	h_bkg_SM_eleelemass [iSM]->SetLineWidth(2.5) ;
	h_bkg_SM_eleelemass [iSM] -> SetFillColor (bkg_SM_colors [iSM]) ;
	h_bkg_SM_eleelemass [iSM] -> SetFillStyle (3018) ;
	
	leg->AddEntry( h_bkg_SM_eleelemass [iSM] ,v_leg_bkg_SM_names [iSM] ,"f");
	
	eleeleMass->Add(h_bkg_SM_eleelemass [iSM]);

      }      
       
    eleeleMass ->Draw() ;

    eleeleMass ->GetYaxis()->SetTitle("d#sigma/dm (pb/100 MeV/c^{2})") ;
    eleeleMass ->GetXaxis()->SetTitle("m_{ee} (GeV/c^{2})") ;
    eleeleMass ->GetXaxis()->SetRangeUser(0,500) ;

    leg -> Draw() ;
    std::cout << Lum << endl;
    s_plot_name = "./Stack_InvMass_" + Lum + "pb_tanB"+ tanB + ".gif" ;
    can  -> Print ( s_plot_name, "gif" ) ;
    s_plot_name = "./Stack_InvMass_" + Lum + "pb_tanB"+ tanB + ".eps" ;
    can  -> Print ( s_plot_name, "eps" ) ;
  


}

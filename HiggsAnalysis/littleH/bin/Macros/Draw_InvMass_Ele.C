void Draw_InvMass_Ele () {
  
  gROOT->ProcessLine(".x Style.C") ;

  const int n_bkg_SM = 5 ;
  const int n_colors = 5 ;

  std::vector<TString> v_bkg_SM_names;

  std::vector<TString> v_leg_bkg_SM_names;

  std::TString s_histo_input ;
  std::TString s_plot_name ;
  
  v_bkg_SM_names.push_back("QCD_EMDoubleEnriched_Pt6to20") ;
  v_bkg_SM_names.push_back("QCD_EMDoubleEnriched_Pt20") ;
  v_bkg_SM_names.push_back("Upsilon1StoEE") ;
  v_bkg_SM_names.push_back("Upsilon2StoEE") ;
  v_bkg_SM_names.push_back("Upsilon3StoEE") ;
  
  v_leg_bkg_SM_names.push_back("QCD_EMDoubleEnriched_Pt6to20") ;
  v_leg_bkg_SM_names.push_back("QCD_EMDoubleEnriched_Pt20") ;
//   v_leg_bkg_SM_names.push_back("ppEleX + #Upsilon_{1S}") ; 
//   v_leg_bkg_SM_names.push_back("ppEleX + #Upsilon_{1S} + #Upsilon_{2S}") ;
//   v_leg_bkg_SM_names.push_back("ppEleX + #Upsilon_{1S} + #Upsilon_{2S} + #Upsilon_{3S}") ; 
  v_leg_bkg_SM_names.push_back("#Upsilon_{1S}") ; 
  v_leg_bkg_SM_names.push_back("#Upsilon_{2S}") ;
  v_leg_bkg_SM_names.push_back("#Upsilon_{3S}") ; 
  
  double IntLum = 2.8 ; // In Inverse pb
  TString Lum = "2.8";
  TString tanB = "10";
  
  double ppEleX_SelEff ;
  
  double bkg_SM_Xsec [n_bkg_SM] = { 478400000., 69795000., 12179., 8767., 2323.  } ;
  
  double bkg_SM_filter_eff [n_bkg_SM] = { 1., 1., 1., 1., 1. } ;

  double bkg_SM_genMCevts [n_bkg_SM] = { 10113679., 10796208., 230304., 217774., 132752. } ;

  double bkg_SM_weight [n_bkg_SM] ;

  int bkg_SM_colors [n_colors] = { 2, 4, 5, 6, 7 } ; 

  int bkg_SM_markers [n_bkg_SM] = { 20, 21, 22, 23, 24 } ;
  
  float N_Upsilon[4];
  float err_Upsilon[4];

  TH1F * h_bkg_SM_eleelemass [n_bkg_SM] ;
  TH1F * h_auxiliary [2] ;
  
  TFile *output = new TFile( "MC_EleEle.root", "RECREATE");

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
    
   TH1F * h_eleeleMass ;
   h_eleeleMass = new TH1F("hInvMass","hInvMass", 100, 4., 12.);    

    //SM BKG
    for ( int iSM = 0; iSM < n_bkg_SM; iSM++ )	
      {
	TFile *bkg_fin = new TFile ( "../ana_" + v_bkg_SM_names[iSM] + ".root", "READ" ) ;
	bkg_SM_weight [iSM] = IntLum * bkg_SM_Xsec [iSM] * bkg_SM_filter_eff [iSM] / bkg_SM_genMCevts [iSM] ;
	
	std::cout << bkg_SM_weight [iSM] << " " + v_bkg_SM_names [iSM] + " " << bkg_SM_weight [iSM] * bkg_SM_genMCevts [iSM] << "\n \n"  << std::endl;
	
	if (iSM == 0 || iSM == 1) {
	  s_histo_input = "h_0_InvMass_OS" ;
	  h_bkg_SM_eleelemass [iSM] = (TH1F *) bkg_fin-> Get (s_histo_input) ;
          h_auxiliary [iSM] = (TH1F *) bkg_fin-> Get ("h_14_InvMass_OS") ;
	  ppEleX_SelEff = (float) h_auxiliary [iSM]->GetEntries()/h_bkg_SM_eleelemass [iSM] -> GetEntries() ;
          h_bkg_SM_eleelemass [iSM] -> Scale (ppEleX_SelEff);
//           h_bkg_SM_eleelemass [iSM] -> Smooth (2);
//           h_bkg_SM_eleelemass [iSM] -> Smooth (20);
	}
	else  s_histo_input = "h_14_InvMass_OS" ;
// 	s_histo_input = "h_14_InvMass_OS" ;
	
	h_bkg_SM_eleelemass [iSM] = (TH1F *) bkg_fin-> Get (s_histo_input) ;
	h_bkg_SM_eleelemass [iSM] -> Rebin (2) ;
	
	//GetNY and Errors
	N_Upsilon[iSM] =  bkg_SM_weight [iSM] * h_bkg_SM_eleelemass [iSM] -> GetEntries() ;
	err_Upsilon[iSM] =  bkg_SM_weight [iSM] * sqrt(h_bkg_SM_eleelemass [iSM] -> GetEntries()) ;
	
// 	h_bkg_SM_eleelemass [iSM] -> Rebin(2) ;
//         h_bkg_SM_eleelemass [iSM] -> Sumw2() ;
        ( *h_bkg_SM_eleelemass [iSM] ) = bkg_SM_weight [iSM] * ( *h_bkg_SM_eleelemass [iSM] ) ; //rescaling with the event weight 
// 	if (iSM == 0) ( *h_bkg_SM_eleelemass [iSM] ) = ppEleX_SelEff * ( *h_bkg_SM_eleelemass [iSM] ) ; //rescaling ppEleX with Selection efficiency
	
	h_bkg_SM_eleelemass [iSM] ->SetMarkerStyle(bkg_SM_markers[iSM]) ;
	h_bkg_SM_eleelemass [iSM]->SetMarkerColor ( bkg_SM_colors[iSM] ) ;
	h_bkg_SM_eleelemass [iSM]->SetMarkerSize(1.6) ;
	h_bkg_SM_eleelemass [iSM]->SetLineColor ( bkg_SM_colors[iSM] ) ;
	h_bkg_SM_eleelemass [iSM]->SetLineWidth(2.5) ;
	h_bkg_SM_eleelemass [iSM] -> SetFillColor (bkg_SM_colors [iSM]) ;
// 	h_bkg_SM_eleelemass [iSM] -> SetFillStyle (3001) ;
	h_bkg_SM_eleelemass [iSM] -> SetFillStyle (3018) ;
	
// 	if (iSM > 0) leg->AddEntry( h_bkg_SM_eleelemass [iSM] ,v_leg_bkg_SM_names [iSM] ,"f");
	leg->AddEntry( h_bkg_SM_eleelemass [iSM] ,v_leg_bkg_SM_names [iSM] ,"f");

	eleeleMass->Add(h_bkg_SM_eleelemass [iSM]);
        h_eleeleMass->Add(h_bkg_SM_eleelemass [iSM]);	  

      }      
       
    eleeleMass ->Draw("nostack") ;
    eleeleMass ->Draw() ;

    eleeleMass ->GetYaxis()->SetTitle("dN/dm (evts/"+Lum+"pb^{-1}/80 MeV/c^{2})") ;
    eleeleMass ->GetXaxis()->SetTitle("m_{ee} (GeV/c^{2})") ;
//     eleeleMass ->GetXaxis()->SetRangeUser(0,500) ;

    leg -> Draw() ;
    std::cout << Lum << endl;
    s_plot_name = "../InvMass/InvMass_MC_" + Lum + "pb.gif" ;
    can  -> Print ( s_plot_name, "gif" ) ;
    s_plot_name = "../InvMass/InvMass_MC_" + Lum + "pb.eps" ;
    can  -> Print ( s_plot_name, "eps" ) ;

//     leg -> Draw() ;
//     std::cout << Lum << endl;
//     s_plot_name = "../InvMass/InvMass_PF_" + Lum + "pb_tanB"+ tanB + ".gif" ;
//     can  -> Print ( s_plot_name, "gif" ) ;
//     s_plot_name = "../InvMass/InvMass_PF_" + Lum + "pb_tanB"+ tanB + ".eps" ;
//     can  -> Print ( s_plot_name, "eps" ) ;

    output->cd();
    h_eleeleMass ->Write() ;
    
    //Cout NY,frac and errors:
    for ( int iY=1; iY < 4; iY++ ) std::cout << " NY" << iY << " = " << N_Upsilon[iY] << " +- " << err_Upsilon[iY] << std::endl ;
    float N_UpsilonTOT = N_Upsilon[1]+N_Upsilon[2] ;
    float err_UpsilonTOT = sqrt(err_Upsilon[1]*err_Upsilon[1]+err_Upsilon[2]*err_Upsilon[2]) ;
    std::cout << "frac1 " <<
    N_Upsilon[1]/N_UpsilonTOT << 
    " +- " << sqrt(err_Upsilon[1]*err_Upsilon[1]/N_Upsilon[1]/N_Upsilon[1] + err_Upsilon[2]*err_Upsilon[2]/N_UpsilonTOT/N_UpsilonTOT)*N_Upsilon[1]/N_UpsilonTOT <<
    std::endl ;
    N_UpsilonTOT = N_Upsilon[1]+N_Upsilon[2]+N_Upsilon[2] ;
    err_UpsilonTOT = sqrt(err_Upsilon[1]*err_Upsilon[1]+err_Upsilon[2]*err_Upsilon[2]+err_Upsilon[3]*err_Upsilon[3]) ;
    std::cout << "frac2 " <<
    1- N_Upsilon[3]/N_UpsilonTOT << 
    " +- " << sqrt(err_Upsilon[3]*err_Upsilon[3]/N_Upsilon[3]/N_Upsilon[3] + err_UpsilonTOT*err_UpsilonTOT/N_UpsilonTOT/N_UpsilonTOT)*(1- N_Upsilon[3]/N_UpsilonTOT) <<
    std::endl ;

}

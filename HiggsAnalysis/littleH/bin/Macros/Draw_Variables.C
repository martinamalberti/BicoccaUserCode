void Draw_Variables () {

  gROOT->ProcessLine(".x Style.C") ;

  const int n_sample = 4 ;
  const int n_colors = 4 ;
  const int n_variables = 12 ; 
  
  std::vector<TString> v_sample_names;
  std::vector<TString> v_leg_sample_names;
  std::vector<TString> v_variable_names;

  std::TString s_histo_input ;
  std::TString s_plot_name ;
  
  v_sample_names.push_back("Upsilon1StoEE") ;
  v_sample_names.push_back("Upsilon2StoEE") ;
  v_sample_names.push_back("Upsilon3StoEE") ;
  v_sample_names.push_back("ppEleX") ;
  
  v_leg_sample_names.push_back("#Upsilon_{1S}#rightarrow ee") ; 
  v_leg_sample_names.push_back("#Upsilon_{2S}#rightarrow ee") ;
  v_leg_sample_names.push_back("#Upsilon_{3S}#rightarrow ee") ; 
  v_leg_sample_names.push_back("ppEleX") ; 

  v_variable_names.push_back("EleLowPt") ; 
  v_variable_names.push_back("EleHighPt") ; 
  v_variable_names.push_back("ElePt") ; 
  v_variable_names.push_back("EleIsoTk") ; 
  v_variable_names.push_back("EleIsoEm") ; 
  v_variable_names.push_back("EleIsoHad") ; 
  v_variable_names.push_back("EleId") ; 
  v_variable_names.push_back("InvMass") ; 
  v_variable_names.push_back("QQprobChi2") ; 
  v_variable_names.push_back("QQS3Dip") ; 
  v_variable_names.push_back("QQSlxy") ; 
  v_variable_names.push_back("QQSTip") ;   

  std::vector<float> h_minX ;
  std::vector<float> h_maxX ;

  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(-1) ; 
  h_minX.push_back(3.2) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 
  h_minX.push_back(0) ; 

  h_maxX.push_back(15) ; 
  h_maxX.push_back(15) ; 
  h_maxX.push_back(15) ; 
  h_maxX.push_back(1) ; 
  h_maxX.push_back(1) ; 
  h_maxX.push_back(1) ; 
  h_maxX.push_back(1) ; 
  h_maxX.push_back(12) ; 
  h_maxX.push_back(1) ; 
  h_maxX.push_back(5) ; 
  h_maxX.push_back(5000) ; 
  h_maxX.push_back(5) ; 
  
  int sample_colors [n_colors] = { 2, 4, 5, 6 } ; 

  int sample_markers [n_sample] = { 20, 21, 22, 23 } ;

  TH1F * h_variable [n_sample][n_variables] ;

  TCanvas *can [n_variables] ;
  TLegend *leg [n_variables];

  for ( int iVar = 0; iVar < n_variables ; iVar++ )
  {
    can[iVar] = new TCanvas ;
//     can[iVar] ->SetLogy () ;
//     can[iVar]->SetGridx () ;
//     can[iVar]->SetGridy () ;
    can[iVar]->cd(0) ;

    leg[iVar]  = new TLegend(0.81,0.72,1.,1.,NULL,"brNDC");
    leg[iVar]  ->SetFillColor(kWhite);
    leg[iVar]  ->SetTextSize(0.025);
    leg[iVar]  ->SetBorderSize(0);

  
    //Draw Histos
  for ( int iSample = 0; iSample < n_sample; iSample++ )	
    {
      TFile *bkg_fin = new TFile ( "../Plots/" + v_sample_names[iSample] + "_Plots.root", "READ" ) ;
      
      s_histo_input = "h_1_" + v_variable_names[iVar] ;
      h_variable [iSample][iVar] = (TH1F *) bkg_fin->Get(s_histo_input) ;
      
      ( *h_variable [iSample][iVar] ) = (1./((float) h_variable [iSample][iVar] -> GetEntries())) * ( *h_variable [iSample][iVar] ) ; //rescaling with the event weight 
            
      h_variable [iSample][iVar] ->SetMarkerStyle(sample_markers[iSample]) ;
      h_variable [iSample][iVar]->SetMarkerColor ( sample_colors[iSample] ) ;
      h_variable [iSample][iVar]->SetMarkerSize(1.6) ;
      h_variable [iSample][iVar]->SetLineColor ( sample_colors[iSample] ) ;
      h_variable [iSample][iVar]->SetLineWidth(2.5) ;
      h_variable [iSample][iVar] -> SetFillColor (sample_colors [iSample]) ;
      h_variable [iSample][iVar] -> SetFillStyle (3018) ;

      h_variable [iSample][iVar] ->GetYaxis()->SetTitle("a.u.") ;
//       h_variable [iSample][iVar] ->GetYaxis()->SetRangeUser(0,1.) ;
      h_variable [iSample][iVar] ->GetXaxis()->SetTitle(v_variable_names[iVar]) ;
      h_variable [iSample][iVar] ->GetXaxis()->SetRangeUser(h_minX[iVar],h_maxX[iVar]) ;

      leg[iVar]->AddEntry( h_variable [iSample][iVar] ,v_leg_sample_names [iSample] ,"f");
            
      if (iSample == 0) h_variable [iSample][iVar] -> Draw() ;
      else h_variable [iSample][iVar] -> Draw("same") ;

    }      

    leg[iVar] -> Draw() ;
    s_plot_name = "../Distributions/" + v_variable_names[iVar] + ".gif" ;
    can[iVar]  -> Print ( s_plot_name, "gif" ) ;
//     s_plot_name = "./Variables_" + Lum + "pb_tanB"+ tanB + ".eps" ;
//     can[iVar]  -> Print ( s_plot_name, "eps" ) ;
  }

}

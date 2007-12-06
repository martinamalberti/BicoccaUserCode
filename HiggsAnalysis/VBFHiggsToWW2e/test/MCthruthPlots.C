void plots ()
{
    gROOT->SetStyle("Plain") ;
	
    TFile *f = new TFile("WWF_HIGGS160/WWf_higgsMass160_tree.root") ;
    TTree *tree = (TTree*)gDirectory->Get("tree");  
        
	int evtFlag;
	
    ///////////// reco GSF electrons
    int numberGSF;
	int numberJet;
    TClonesArray *recoEle4Momentum = new TClonesArray ("TLorentzVector");
    TClonesArray *recoEleTrkMomentumAtVtx = new TClonesArray ("TVector3");
    TClonesArray *recoEleTrkPositionAtVtx = new TClonesArray ("TVector3");
    std::vector<double> *recoEleEcalEnergy = new std::vector<double>(); 
    std::vector<double> *recoEleTrkIsoVal = new std::vector<double>();
    std::vector<double> *recoEleCalIsoVal = new std::vector<double>();
    std::vector<int> *recoEleClass = new std::vector<int>();
    std::vector<int> *recoEleCutBasedID = new std::vector<int>();
	TClonesArray *recoJet4Momentum = new TClonesArray ("TLorentzVector");
    
	tree->SetBranchAddress ("evtFlag", &evtFlag);
    tree->SetBranchAddress ("numberGSF", &numberGSF);
    tree->SetBranchAddress ("recoEle4Momentum",  &recoEle4Momentum); 
    tree->SetBranchAddress ("recoEleTrkMomentumAtVtx", &recoEleTrkMomentumAtVtx); 
    tree->SetBranchAddress ("recoEleTrkPositionAtVtx", &recoEleTrkPositionAtVtx);
    tree->SetBranchAddress ("recoEleEcalEnergy",  &recoEleEcalEnergy); //vettore
    tree->SetBranchAddress ("recoEleTrkIsoVal",  &recoEleTrkIsoVal); //vettore
    tree->SetBranchAddress ("recoEleCalIsoVal",  &recoEleCalIsoVal); //vettore
    tree->SetBranchAddress ("recoEleClass",  &recoEleClass); //vettore
    tree->SetBranchAddress ("recoEleCutBasedID",  &recoEleCutBasedID); //vettore
	
	///////////// reco jets
	tree->SetBranchAddress ("numberJet", &numberJet);
	tree->SetBranchAddress ("genJet4Momentum",  &recoJet4Momentum); 

    //electrons
    TH1F *histo_recLep_eVVF = new TH1F ("histo_recLep_eVVF", "GSF electrons energy (4p)",100,0,500) ;
    histo_recLep_eVVF->GetXaxis ()->SetTitle ("Energy (GeV)") ;
    histo_recLep_eVVF->SetLineColor (2);
    TH1F *histo_recLep_eggF = new TH1F ("histo_recLep_eggF", "GSF electrons energy (4p)",100,0,500) ;
    histo_recLep_eggF->GetXaxis ()->SetTitle ("Energy (GeV)") ;
    histo_recLep_eggF->SetLineColor (3);
    TH1F *histo_recLep_ptVVF = new TH1F ("histo_recLep_ptVVF", "GSF electrons pt (4p)",100,0,200) ;
    histo_recLep_ptVVF->GetXaxis ()->SetTitle ("pt (GeV)") ;
    histo_recLep_ptVVF->SetLineColor (2);
    TH1F *histo_recLep_ptggF = new TH1F ("histo_recLep_ptggF", "GSF electrons pt (4p)",100,0,200) ;
    histo_recLep_ptggF->GetXaxis ()->SetTitle ("pt (GeV)") ;
    histo_recLep_ptggF->SetLineColor (3);
	TH1F *histo_recLep_etaVVF = new TH1F ("histo_recLep_etaVVF", "GSF electrons #eta (4p)",100,-4,4) ;
    histo_recLep_etaVVF->GetXaxis ()->SetTitle ("#eta (GeV)") ;
    histo_recLep_etaVVF->SetLineColor (2);
    TH1F *histo_recLep_etaggF = new TH1F ("histo_recLep_etaggF", "GSF electrons #eta (4p)",100,-4,4) ;
    histo_recLep_etaggF->GetXaxis ()->SetTitle ("#eta (GeV)") ;
    histo_recLep_etaggF->SetLineColor (3);
	
	// jets 
	TH1F *histo_numJetVVF = new TH1F ("histo_numJetVVF", "number of jets",50,0,50) ;
    histo_numJetVVF->GetXaxis ()->SetTitle ("number of jets") ;
    histo_numJetVVF->SetLineColor (2);
    TH1F *histo_numJetggF = new TH1F ("histo_numJetggF", "number of jets",50,0,50) ;
    histo_numJetggF->GetXaxis ()->SetTitle ("number of jets") ;
    histo_numJetggF->SetLineColor (3);	
	TH1F *histo_energyJetVVF = new TH1F ("histo_energyJetVVF", "energy of jets",100,0,500) ;
    histo_energyJetVVF->GetXaxis ()->SetTitle ("energy of jets") ;
    histo_energyJetVVF->SetLineColor (2);
    TH1F *histo_energyJetggF = new TH1F ("histo_energyJetggF", "energy of jets",100,0,500) ;
    histo_energyJetggF->GetXaxis ()->SetTitle ("energy of jets") ;
    histo_energyJetggF->SetLineColor (3);
	TH1F *histo_ptJetVVF = new TH1F ("histo_ptJetVVF", "pt of jets",50,0,200) ;
    histo_ptJetVVF->GetXaxis ()->SetTitle ("pt of jets") ;
    histo_ptJetVVF->SetLineColor (2);
    TH1F *histo_ptJetggF = new TH1F ("histo_ptJetggF", "pt of jets",50,0,200) ;
    histo_ptJetggF->GetXaxis ()->SetTitle ("pt of jets") ;
    histo_ptJetggF->SetLineColor (3);
    TH1F *histo_etaJetVVF = new TH1F ("histo_etaJetVVF", "#eta of jets",100,-10,10) ;
    histo_etaJetVVF->GetXaxis ()->SetTitle ("#eta of jets") ;
    histo_etaJetVVF->SetLineColor (2);
    TH1F *histo_etaJetggF = new TH1F ("histo_etaJetggF", "#eta of jets",100,-10,10) ;
    histo_etaJetggF->GetXaxis ()->SetTitle ("#eta of jets") ;
    histo_etaJetggF->SetLineColor (3);
	
	//2 leading jets
	TH1F *histo_etaLeadingJetVVF = new TH1F ("histo_etaLeadingJetVVF", "#eta of 2 leading jets",100,-10,10) ;
    histo_etaLeadingJetVVF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_etaLeadingJetVVF->SetLineColor (2);
    TH1F *histo_etaLeadingJetggF = new TH1F ("histo_etaLeadingJetggF", "#eta of 2 leading jets",100,-10,10) ;
    histo_etaLeadingJetggF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_etaLeadingJetggF->SetLineColor (3);
    TH1F *histo_energyLeadingJetVVF = new TH1F ("histo_energyLeadingJetVVF", "#energy of 2 leading jets",100,0,5000) ;
    histo_energyLeadingJetVVF->GetXaxis ()->SetTitle ("#energy of leading jets") ;
    histo_energyLeadingJetVVF->SetLineColor (2);
    TH1F *histo_energyLeadingJetggF = new TH1F ("histo_energyLeadingJetggF", "#energy of 2 leading jets",100,0,5000) ;
    histo_energyLeadingJetggF->GetXaxis ()->SetTitle ("#energy of leading jets") ;
    histo_energyLeadingJetggF->SetLineColor (3);
	TH1F *histo_deltaEtaLeadingJetVVF = new TH1F ("histo_deltaEtaLeadingJetVVF", "#Delta#eta between 2 leading jets",100,-0.5,20) ;
    histo_deltaEtaLeadingJetVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaLeadingJetVVF->SetLineColor (2);
    TH1F *histo_deltaEtaLeadingJetggF = new TH1F ("histo_deltaEtaLeadingJetggF", "#Delta#eta of 2 leading jets",100,-0.5,20) ;
    histo_deltaEtaLeadingJetggF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaLeadingJetggF->SetLineColor (3);
	TH1F *histo_ptLeadingJetVVF = new TH1F ("histo_ptLeadingJetVVF", "#pt of 2 leading jets",100,0,200) ;
    histo_ptLeadingJetVVF->GetXaxis ()->SetTitle ("#pt of leading jets") ;
    histo_ptLeadingJetVVF->SetLineColor (2);
    TH1F *histo_ptLeadingJetggF = new TH1F ("histo_ptLeadingJetggF", "#pt of 2 leading jets",100,0,200) ;
    histo_ptLeadingJetggF->GetXaxis ()->SetTitle ("#pt of leading jets") ;
    histo_ptLeadingJetggF->SetLineColor (3);
	TH1F *histo_invMassLeadingJetVVF = new TH1F ("histo_invMassLeadingJetVVF", "#invMass of 2 leading jets",100,0,1000) ;
    histo_invMassLeadingJetVVF->GetXaxis ()->SetTitle ("#invMass of leading jets") ;
    histo_invMassLeadingJetVVF->SetLineColor (2);
    TH1F *histo_invMassLeadingJetggF = new TH1F ("histo_invMassLeadingJetggF", "#invMass of 2 leading jets",100,0,1000) ;
    histo_invMassLeadingJetggF->GetXaxis ()->SetTitle ("#invMass of leading jets") ;
    histo_invMassLeadingJetggF->SetLineColor (3);

	
	
    TLorentzVector *leadingJet1 = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector *leadingJet2 = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector sommaTag;
	
    int nentries = (int) tree->GetEntries();
    for(int i=0; i<nentries; i++)
        {
            tree->GetEntry(i);
			
			/// loop sugli elettroni
			for (int j=0; j<numberGSF ; j++)
			{
			   if (evtFlag == 124 || evtFlag == 125) //VV fusion
			   {
			   histo_recLep_eVVF->Fill (((TLorentzVector*)(recoEle4Momentum->At(j)))->Energy()) ;
			   histo_recLep_ptVVF->Fill (((TLorentzVector*)(recoEle4Momentum->At(j)))->Pt()) ;
			   histo_recLep_etaVVF->Fill (((TLorentzVector*)(recoEle4Momentum->At(j)))->Eta()) ;
			   }
			   if (evtFlag == 102) //gg fusion
			   {
			   histo_recLep_eggF->Fill (((TLorentzVector*)(recoEle4Momentum->At(j)))->Energy()) ;
			   histo_recLep_ptggF->Fill (((TLorentzVector*)(recoEle4Momentum->At(j)))->Pt()) ;
			   histo_recLep_etaggF->Fill (((TLorentzVector*)(recoEle4Momentum->At(j)))->Eta()) ;
			   }
			   //cout<< (((TVector3*)(recoEleTrkMomentumAtVtx->At(j)))->Px()) << endl ;
			   //histo_recLep_eCal->Fill (recoEleEcalEnergy->at(0)) ;
			}
			
			
			// loop sui jet
			for (int j=0; j <numberJet ; j++)
			{
			   if (evtFlag== 124 || evtFlag== 125) //VV fusion
			   {
			   histo_energyJetVVF->Fill (((TLorentzVector*)(recoJet4Momentum->At(j)))->Energy()) ;
			   histo_ptJetVVF->Fill (((TLorentzVector*)(recoJet4Momentum->At(j)))->Pt()) ;
			   histo_etaJetVVF->Fill (((TLorentzVector*)(recoJet4Momentum->At(j)))->Eta()) ;
		       }
			   else if (evtFlag == 102) //gg fusion
			   {
			   histo_energyJetggF->Fill (((TLorentzVector*)(recoJet4Momentum->At(j)))->Energy()) ;
			   histo_ptJetggF->Fill (((TLorentzVector*)(recoJet4Momentum->At(j)))->Pt()) ;
			   histo_etaJetggF->Fill (((TLorentzVector*)(recoJet4Momentum->At(j)))->Eta()) ;
			   }
			}
			
			///// loop sui due jet piu' energetici
    		twoLeadingJets (*recoJet4Momentum, *leadingJet1, *leadingJet2) ;
			sommaTag = *(leadingJet1) + *(leadingJet2);
			
			if (evtFlag == 124 || evtFlag == 125) //VV fusion
			{
				histo_numJetVVF->Fill (numberJet) ;
				histo_energyLeadingJetVVF -> Fill (leadingJet1->Energy ()) ;
				histo_energyLeadingJetVVF -> Fill (leadingJet2->Energy ()) ;
				histo_ptLeadingJetVVF -> Fill (leadingJet1->Pt ()) ;
				histo_ptLeadingJetVVF -> Fill (leadingJet2->Pt ()) ;
				histo_etaLeadingJetVVF -> Fill (leadingJet1->Eta ()) ;
				histo_etaLeadingJetVVF -> Fill (leadingJet2->Eta ()) ;
				histo_deltaEtaLeadingJetVVF -> Fill (fabs (leadingJet1->Eta () - leadingJet2->Eta ()) ) ;
				histo_invMassLeadingJetVVF -> Fill (sommaTag.M()) ;
			}
			else if (evtFlag == 102) //gg fusion
			{
				histo_numJetggF->Fill (numberJet) ;
				histo_energyLeadingJetggF -> Fill (leadingJet1->Energy ()) ;
				histo_energyLeadingJetggF -> Fill (leadingJet2->Energy ()) ;
				histo_ptLeadingJetggF -> Fill (leadingJet1->Pt ()) ;
				histo_ptLeadingJetggF -> Fill (leadingJet2->Pt ()) ;
				histo_etaLeadingJetggF -> Fill (leadingJet1->Eta ()) ;
				histo_etaLeadingJetggF -> Fill (leadingJet2->Eta ()) ;
				histo_deltaEtaLeadingJetggF -> Fill (fabs (leadingJet1->Eta () - leadingJet2->Eta ()) ) ;
				histo_invMassLeadingJetggF -> Fill (sommaTag.M()) ;
			}

        }
 
 
    //normalization 
	histo_recLep_eVVF->Scale (1/(histo_recLep_eVVF->Integral()));
    histo_recLep_ptVVF->Scale (1/(histo_recLep_ptVVF->Integral()));
    histo_recLep_etaVVF->Scale (1/(histo_recLep_etaVVF->Integral()));
	histo_recLep_eggF->Scale (1/(histo_recLep_eggF->Integral()));
    histo_recLep_ptggF->Scale (1/(histo_recLep_ptggF->Integral()));
    histo_recLep_etaggF->Scale (1/(histo_recLep_etaggF->Integral()));
    histo_numJetVVF->Scale (1/(histo_numJetVVF->Integral()));
    histo_numJetggF->Scale (1/(histo_numJetggF->Integral()));
    histo_energyJetVVF->Scale (1/(histo_energyJetVVF->Integral()));
    histo_energyJetggF->Scale (1/(histo_energyJetggF->Integral()));
	histo_ptJetVVF->Scale (1/(histo_ptJetVVF->Integral()));
    histo_ptJetggF->Scale (1/(histo_ptJetggF->Integral()));
	histo_etaJetVVF->Scale (1/(histo_etaJetVVF->Integral()));
    histo_etaJetggF->Scale (1/(histo_etaJetggF->Integral()));
	histo_energyLeadingJetggF->Scale (1/(histo_energyLeadingJetggF->Integral()));
	histo_ptLeadingJetggF->Scale (1/(histo_ptLeadingJetggF->Integral()));
	histo_etaLeadingJetggF->Scale (1/(histo_etaLeadingJetggF->Integral()));
	histo_energyLeadingJetVVF->Scale (1/(histo_energyLeadingJetVVF->Integral()));
	histo_ptLeadingJetVVF->Scale (1/(histo_ptLeadingJetVVF->Integral()));
	histo_etaLeadingJetVVF->Scale (1/(histo_etaLeadingJetVVF->Integral()));
	histo_deltaEtaLeadingJetggF->Scale (1/(histo_deltaEtaLeadingJetggF->Integral()));
	histo_deltaEtaLeadingJetVVF->Scale (1/(histo_deltaEtaLeadingJetVVF->Integral()));
	histo_invMassLeadingJetggF->Scale (1/(histo_invMassLeadingJetggF->Integral()));
	histo_invMassLeadingJetVVF->Scale (1/(histo_invMassLeadingJetVVF->Integral()));
	
			
	TLegend *tleg = new TLegend (0.7, 0.6, 0.9, 0.8);
    tleg -> SetBorderSize (0);
    tleg -> SetFillColor (0);
    tleg-> AddEntry (histo_recLep_eVVF, "VV fusion","l") ;
    tleg-> AddEntry (histo_recLep_eggF, "gg fusion","l") ;

 
    TCanvas *p=new TCanvas () ;
	p->Divide (3,4) ;
	p->cd (1);
    histo_recLep_eVVF->Draw () ;
    histo_recLep_eggF->Draw ("same") ;
	tleg->Draw ("same") ;
	p->cd (2);
    histo_recLep_ptVVF->Draw () ;
    histo_recLep_ptggF->Draw ("same") ;
	tleg->Draw ("same") ;
	p->cd (3);
    histo_recLep_etaVVF->Draw () ;
    histo_recLep_etaggF->Draw ("same") ;
	tleg->Draw ("same") ;
	p->cd (4);
	histo_numJetVVF->Draw () ;
    histo_numJetggF->Draw ("same") ;
	p->cd (5);
	histo_energyJetVVF->Draw () ;
    histo_energyJetggF->Draw ("same") ;
	p->cd (6);
	histo_ptJetVVF->Draw () ;
    histo_ptJetggF->Draw ("same") ;
	p->cd (7);
	histo_etaJetVVF->Draw () ;
    histo_etaJetggF->Draw ("same") ;
	p->cd (8);
	histo_energyLeadingJetVVF->Draw () ;
	histo_energyLeadingJetggF->Draw ("same") ;
    p->cd (9);
	histo_etaLeadingJetggF->Draw () ;
	histo_etaLeadingJetVVF->Draw ("same") ;
	p->cd (10);
	histo_deltaEtaLeadingJetggF->Draw () ;
	histo_deltaEtaLeadingJetVVF->Draw ("same") ;
	p->cd (11);
	histo_ptLeadingJetggF->Draw () ;
	histo_ptLeadingJetVVF->Draw ("same") ;
	p->cd (12);
	histo_invMassLeadingJetggF->Draw () ;
	histo_invMassLeadingJetVVF->Draw ("same") ;

}


void MCtruthPlots ()
{
    gROOT->SetStyle("Plain") ;
    
    TFile *f = new TFile ("WWf_higgsMass160_tree_9.root") ;

    TTree *tree = (TTree*)gDirectory->Get("tree");  
    
    //verita' MC //////////////////////////////////////////////////////////
    TLorentzVector *genLepPlus = new TLorentzVector();
    TLorentzVector *genLepMinus = new TLorentzVector();
    TLorentzVector *genMetPlus = new TLorentzVector();
    TLorentzVector *genMetMinus = new TLorentzVector();
    TLorentzVector *genqTagF = new TLorentzVector();
    TLorentzVector *genqTagB = new TLorentzVector();
    TLorentzVector *genHiggs = new TLorentzVector();
    TLorentzVector *genWp = new TLorentzVector();
    TLorentzVector *genWm = new TLorentzVector();
 
    int LepPlusFlavour, LepMinusFlavour ; //e=11/mu=13
    
    tree->SetBranchAddress("LepPlusFlavour", &LepPlusFlavour);
    tree->SetBranchAddress("LepMinusFlavour", &LepMinusFlavour);
    tree->SetBranchAddress("genLepPlus", &genLepPlus);
    tree->SetBranchAddress("genLepMinus", &genLepMinus);
    tree->SetBranchAddress("genMetPlus", &genMetPlus);
    tree->SetBranchAddress("genMetMinus", &genMetMinus);
    tree->SetBranchAddress("genqTagF", &genqTagF);
    tree->SetBranchAddress("genqTagB", &genqTagB);
    tree->SetBranchAddress("genHiggs", &genHiggs);
    tree->SetBranchAddress("genWp", &genWp);
    tree->SetBranchAddress("genWm", &genWm);
    
    //leptons
    TH1F *histo_genLep_e = new TH1F("histo_genLep_e", "l+ and l- energy",100,0,1000) ;
    histo_genLep_e->GetXaxis ()->SetTitle ("Energy (GeV)") ;
    histo_genLep_e->SetFillColor (94);
    TH1F *histo_genLep_pt = new TH1F("histo_genLep_pt", "l+ and l- pt",100,0,200) ;
    histo_genLep_pt->GetXaxis ()->SetTitle ("Pt (GeV)") ;
    histo_genLep_pt->SetFillColor (94);
    TH1F *histo_genLep_deta_ee = new TH1F("histo_genLep_deta_ee", "#Delta#eta between l+ and l-",50,0,10) ;
    histo_genLep_deta_ee->GetXaxis ()->SetTitle ("#Delta#eta") ;
    histo_genLep_deta_ee->SetLineColor (1);
    TH1F *histo_genLep_dphi_ee = new TH1F("histo_genLep_dphi_ee", "#Delta#phi between l+ and l-",50,0,10) ;
    histo_genLep_dphi_ee->GetXaxis ()->SetTitle ("#Delta#phi") ;
    histo_genLep_dphi_ee->SetLineColor (1);
    TH1F *histo_genLep_deta_mumu = new TH1F("histo_genLep_deta_mumu", "#Delta#eta between l+ and l-",50,0,10) ;
    histo_genLep_deta_mumu->GetXaxis ()->SetTitle ("#Delta#eta") ;
    histo_genLep_deta_mumu->SetLineColor (2);
    TH1F *histo_genLep_dphi_mumu = new TH1F("histo_genLep_dphi_mumu", "#Delta#phi between l+ and l-",50,0,10) ;
    histo_genLep_dphi_mumu->GetXaxis ()->SetTitle ("#Delta#phi") ;
    histo_genLep_dphi_mumu->SetLineColor (2);
    TH1F *histo_genLep_deta_emu = new TH1F("histo_genLep_deta_emu", "#Delta#eta between l+ and l-",50,0,10) ;
    histo_genLep_deta_emu->GetXaxis ()->SetTitle ("#Delta#eta") ;
    histo_genLep_deta_emu->SetLineColor (3);
    TH1F *histo_genLep_dphi_emu = new TH1F("histo_genLep_dphi_emu", "#Delta#phi between l+ and l-",50,0,10) ;
    histo_genLep_dphi_emu->GetXaxis ()->SetTitle ("#Delta#phi") ;
    histo_genLep_dphi_emu->SetLineColor (3);
    TH1F *histo_genMet_e = new TH1F("histo_genMet_e", "neutrinos energy",100,0,1000) ;
    histo_genMet_e->GetXaxis ()->SetTitle ("Energy (GeV)") ;
    histo_genMet_e->SetFillColor (4);
    TH1F *histo_genMet_pt = new TH1F("histo_genMet_pt", "neutrinos pt",100,0,200) ;
    histo_genMet_pt->GetXaxis ()->SetTitle ("Pt (GeV)") ;
    histo_genMet_pt->SetFillColor (4);

    // bosons and higgs
    TH1F *histo_genHiggs_mass = new TH1F("histo_genHiggs_mass", "generated higgs mass",100,100,200) ;
    histo_genHiggs_mass->GetXaxis ()->SetTitle ("Higgs mass") ;
    histo_genHiggs_mass->SetFillColor (2);
    TH1F *histo_genW_mass = new TH1F("histo_genW_mass", "generated W mass",100,20,120) ;
    histo_genW_mass->GetXaxis ()->SetTitle ("W mass") ;
    histo_genW_mass->SetFillColor (2);
    TH1F *histo_genrecoHiggs_mass = new TH1F("histo_genrecoHiggs_mass", "higgs mass from generated Ws",100,100,200) ;
    histo_genrecoHiggs_mass->GetXaxis ()->SetTitle ("Higgs mass") ;
    histo_genrecoHiggs_mass->SetFillColor (5);
    TH1F *histo_genrecoW_mass = new TH1F("histo_genrecoW_mass", "W mass from generated leptons",100,20,120) ;
    histo_genrecoW_mass->GetXaxis ()->SetTitle ("W mass") ;
    histo_genrecoW_mass->SetFillColor (5);
    
    
    

    TLegend *tleg = new TLegend (0.2, 0.2, 0.5, 0.5);
    tleg -> SetBorderSize (0);
    tleg -> SetFillColor (0);
    tleg-> AddEntry (histo_genLep_dphi_ee, "ee","l") ;
    tleg-> AddEntry (histo_genLep_dphi_emu, "e#mu","l") ;
    tleg-> AddEntry (histo_genLep_dphi_mumu, "#mu#mu","l") ;
    
    //tags
    TH1F *histo_genTag_eta = new TH1F("histo_genTag_eta", "tag quarks #eta",100,-10,10) ;
    histo_genTag_eta->GetXaxis ()->SetTitle ("#eta") ;
    histo_genTag_eta->SetFillColor (8);
    TH1F *histo_genTag_e = new TH1F("histo_genTag_e", "tag quarks energy",100,0,5000) ;
    histo_genTag_e->GetXaxis ()->SetTitle ("Energy (GeV)") ;
    histo_genTag_e->SetFillColor (8);
    TH1F *histo_genTag_pt = new TH1F("histo_genTag_pt", "tag quarks pt",100,0,800) ;
    histo_genTag_pt->GetXaxis ()->SetTitle ("Pt (GeV)") ;
    histo_genTag_pt->SetFillColor (8);
    TH1F *histo_genTag_deta = new TH1F("histo_genTag_deta", "#Delta#eta between tag quarks",100,0,10) ;
    histo_genTag_deta->GetXaxis ()->SetTitle ("#Delta#eta") ;
    histo_genTag_deta->SetFillColor (8);
    TH1F *histo_genTag_mass = new TH1F("histo_genTag_mass", "Invariant mass of tag quarks",100,0,5000) ;
    histo_genTag_mass->GetXaxis ()->SetTitle ("invariant mass") ;
    histo_genTag_mass->SetFillColor (8);
    
    TH1F *histo_genTutto_mass = new TH1F("histo_genTutto_mass", "invariant mass of the 6 generated particles",100,20,7000) ;
    histo_genTutto_mass->GetXaxis ()->SetTitle ("mass (leptons + neutrinos + tags)") ;
    histo_genTutto_mass->SetFillColor (6);
 
    TLorentzVector sommaTag;
    TLorentzVector sommaTutto;
    TLorentzVector wp;
    TLorentzVector wm;
    TLorentzVector higgs;
    
    int counter_ee = 0 ;
    int counter_mumu = 0 ;
    int counter_emu = 0 ;    
    
    Int_t nentries = (Int_t) tree->GetEntries();
    
    for(Int_t i=0; i<nentries; i++)
    {
        tree->GetEntry(i);
        //lepton
        histo_genLep_e->Fill (genLepPlus->E()) ;
        histo_genLep_e->Fill (genLepMinus->E()) ;
        histo_genLep_pt->Fill (genLepPlus->Pt()) ;
        histo_genLep_pt->Fill (genLepMinus->Pt()) ;
		/*
        if (LepPlusFlavour == 11 && LepMinusFlavour == 11) histo_genLep_deta_ee->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 11 && LepMinusFlavour == 11) histo_genLep_dphi_ee->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 13) histo_genLep_deta_mumu->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 13) histo_genLep_dphi_mumu->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;
        if (LepPlusFlavour == 11 && LepMinusFlavour == 13) histo_genLep_deta_emu->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 11 && LepMinusFlavour == 13) histo_genLep_dphi_emu->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 11) histo_genLep_deta_emu->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 11) histo_genLep_dphi_emu->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;
        */
         if (LepPlusFlavour == 11 && LepMinusFlavour == 11) counter_ee++;
         if (LepPlusFlavour == 13 && LepMinusFlavour == 13) counter_mumu++;
         if (LepPlusFlavour == 13 && LepMinusFlavour == 11) counter_emu++;
         if (LepPlusFlavour == 11 && LepMinusFlavour == 13) counter_emu++;
        
        // neutrinos    
        histo_genMet_e->Fill (genMetPlus->E()) ;
        histo_genMet_e->Fill (genMetMinus->E()) ;
        histo_genMet_pt->Fill (genMetPlus->Pt()) ;
        histo_genMet_pt->Fill (genMetMinus->Pt()) ;    
            
        //tags
        //histo_genTag_eta->Fill (genqTagF->Eta()) ;
        //histo_genTag_eta->Fill (genqTagB->Eta()) ;
        histo_genTag_e->Fill (genqTagF->E()) ;
        histo_genTag_e->Fill (genqTagB->E()) ;
        histo_genTag_pt->Fill (genqTagF->Pt()) ;
        histo_genTag_pt->Fill (genqTagB->Pt()) ;
        //histo_genTag_deta->Fill (fabs(genqTagF->Eta()-genqTagB->Eta())) ;
        
        // bosons and higgs
        sommaTag = *(genqTagF) + *(genqTagB) ; 
        histo_genTag_mass->Fill (sommaTag.M()) ;
        
        wp = *(genLepPlus)+*(genMetPlus);
        wm = *(genLepMinus)+*(genMetMinus);
        higgs = wp + wm ;
        
        histo_genrecoHiggs_mass->Fill (higgs.M()) ;
        histo_genrecoW_mass->Fill (wp.M()) ;
        histo_genrecoW_mass->Fill (wm.M()) ;
        
        histo_genHiggs_mass->Fill (genHiggs->M()) ;
        histo_genW_mass->Fill (genWp->M()) ;
        histo_genW_mass->Fill (genWm->M()) ;
        
        sommaTutto = higgs + sommaTag;
        histo_genTutto_mass->Fill (sommaTutto.M()) ;
        
}

    histo_genLep_e->Draw ();
     /*
    
    TCanvas *pippo = new TCanvas () ;
    pippo->Divide (3,2) ;
    pippo->cd (1);
    histo_genLep_e->Draw ();
    pippo->cd (2);
    histo_genLep_pt->Draw ();
    pippo->cd (3);
    histo_genLep_deta_emu->Draw () ;
    histo_genLep_deta_ee->Draw ("same") ;
    histo_genLep_deta_mumu->Draw ("same") ;
    tleg->Draw("same");
    pippo->cd (4);
    histo_genLep_dphi_emu->Draw () ;
    histo_genLep_dphi_ee->Draw ("same") ;
    histo_genLep_dphi_mumu->Draw ("same") ;
    tleg->Draw("same");
    pippo->cd (5);
    histo_genMet_e->Draw ();
    pippo->cd (6);
    histo_genMet_pt->Draw ();
    
    TCanvas *pippo3 = new TCanvas () ;
    pippo3->Divide (2,2) ;
    pippo3->cd (1);
    histo_genHiggs_mass->Draw () ;
    pippo3->cd (2);
    histo_genW_mass->Draw () ;
    pippo3->cd (3);
    histo_genrecoHiggs_mass->Draw () ;
    pippo3->cd (4);
    histo_genrecoW_mass->Draw () ;
    
    
    TCanvas *pippo2 = new TCanvas () ;
    pippo2->Divide (2,3) ;
    pippo2->cd (1);
    histo_genTag_eta->Draw ();
    pippo2->cd (2);
    histo_genTag_deta->Draw ();
    pippo2->cd (3);
    histo_genTag_pt->Draw ();
    pippo2->cd (4);
    histo_genTag_e->Draw ();
    pippo2->cd (5);
    histo_genTag_mass->Draw () ;
    pippo2->cd (6);
    histo_genTutto_mass->Draw () ;
    
    TCanvas *pippo4 = new TCanvas () ;
    pippo4->SetLogy() ;
    histo_genTutto_mass->Draw () ;
*/
    /*
    TCanvas *pippo = new TCanvas () ;
    pippo->Divide (1,2) ;
    pippo->SetLogy() ;
    pippo->cd (1);
    histo_genLep_deta_emu->Draw () ;
    histo_genLep_deta_ee->Draw ("same") ;
    histo_genLep_deta_mumu->Draw ("same") ;
    tleg->Draw("same");
    pippo->cd (2);
    histo_genLep_dphi_emu->Draw () ;
    histo_genLep_dphi_ee->Draw ("same") ;
    histo_genLep_dphi_mumu->Draw ("same") ;
    tleg->Draw("same");
    */
    cout << "#ee " << counter_ee << endl ;
    cout << "#mumu " << counter_mumu << endl ;
    cout << "#emu " << counter_emu << endl ;  
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void twoLeadingJets (TClonesArray &recoJet4Momentum , TLorentzVector &leadingJet1, TLorentzVector &leadingJet2)
{
	int numberJet = recoJet4Momentum.GetEntries () ;

	int firstEnergyIndex = 0 ;
	double maxEnergy = 0.0;
	double energy = 0.0;

	/////////// loop per il first leading jet 
	for (int j=0; j <numberJet ; j++)
		{
		energy = (((TLorentzVector*)(recoJet4Momentum.At(j)))->Pt()) ;
		if (energy > maxEnergy) 
			{
			maxEnergy = energy;
			firstEnergyIndex = j;
			}
		}

	leadingJet1.SetPx (((TLorentzVector*)(recoJet4Momentum.At(firstEnergyIndex)))->Px()) ;  
	leadingJet1.SetPy (((TLorentzVector*)(recoJet4Momentum.At(firstEnergyIndex)))->Py()) ;
	leadingJet1.SetPz (((TLorentzVector*)(recoJet4Momentum.At(firstEnergyIndex)))->Pz()) ;
	leadingJet1.SetE (((TLorentzVector*)(recoJet4Momentum.At(firstEnergyIndex)))->Energy()) ;

	/////////// loop per il second leading jet 
	int secondEnergyIndex = 0 ;
	maxEnergy = 0.0;
	
	for (int j=0; j <(numberJet) ; j++)
		{
		if (j==firstEnergyIndex) continue ;
		energy = (((TLorentzVector*)(recoJet4Momentum.At(j)))->Pt()) ;
		if (energy > maxEnergy) 
			{
			maxEnergy = energy;
			secondEnergyIndex = j;
			}
		}

	leadingJet2.SetPx (((TLorentzVector*)(recoJet4Momentum.At(secondEnergyIndex)))->Px()) ;  
	leadingJet2.SetPy (((TLorentzVector*)(recoJet4Momentum.At(secondEnergyIndex)))->Py()) ;
	leadingJet2.SetPz (((TLorentzVector*)(recoJet4Momentum.At(secondEnergyIndex)))->Pz()) ;
	leadingJet2.SetE (((TLorentzVector*)(recoJet4Momentum.At(secondEnergyIndex)))->Energy()) ;
		
}



void tagJetSigVsBkg ()
{
    gROOT->SetStyle("Plain") ;
	
    TFile *f = new TFile("WWF_HIGGS160/WWf_higgsMass160_tree.root") ;
    TTree *tree = (TTree*)f->Get("tree"); 
        
	int evtFlag;
	int numberJet;
	int numberJetAfterCuts;
	TClonesArray *recoJet4Momentum = new TClonesArray ("TLorentzVector");
	
	tree->SetBranchAddress ("evtFlag", &evtFlag);
	tree->SetBranchAddress ("numberJet", &numberJet);
	tree->SetBranchAddress ("recoJet4Momentum", &recoJet4Momentum);
    tree->SetBranchAddress ("numberJet", &numberJet);
	tree->SetBranchAddress ("recoJet4Momentum", &recoJet4Momentum);
	
	//VVF
	TH1F *histo_deltaEtaTag_VVf = new TH1F ("histo_deltaEtaTag_VVf", "#Delta#eta between tag jets", 100, 0, 10) ;
	histo_deltaEtaTag_VVf->SetLineColor (4) ;
	TH1F *histo_invMassTag_VVf = new TH1F ("histo_invMassTag_VVf", "Invariant mass of tag jets", 100, 0, 4000) ;
	histo_invMassTag_VVf->SetLineColor (4) ;
	TH1F *histo_energyTag_VVf = new TH1F ("histo_energyTag_VVf", "Energy of tag jets", 100, 0, 1200) ;
	histo_energyTag_VVf->SetLineColor (4) ;
	TH1F *histo_ptTag_VVf = new TH1F ("histo_ptTag_VVf", "Pt of tag jets", 100, 0, 400) ;
	histo_ptTag_VVf->SetLineColor (4) ;
	TH1F *histo_etaTag_VVf = new TH1F ("histo_etaTag_VVf", "#eta of tag jets", 100, -6, 6) ;
	histo_etaTag_VVf->SetLineColor (4) ;
	TH1F *histo_numJet_VVf = new TH1F ("histo_numJet_VVf", "# of tag jets", 12, 0, 12) ;
	histo_numJet_VVf->SetLineColor (4) ;
    //ggF
    TH1F *histo_deltaEtaTag_ggf = new TH1F ("histo_deltaEtaTag_ggf", "#Delta#eta between tag jets", 100, 0, 10) ;
	histo_deltaEtaTag_ggf->SetLineColor (3) ;
	TH1F *histo_invMassTag_ggf = new TH1F ("histo_invMassTag_ggf", "Invariant mass of tag jets", 100, 0, 4000) ;
	histo_invMassTag_ggf->SetLineColor (3) ;
	TH1F *histo_energyTag_ggf = new TH1F ("histo_energyTag_ggf", "Energy of tag jets", 100, 0, 1200) ;
	histo_energyTag_ggf->SetLineColor (3) ;
	TH1F *histo_ptTag_ggf = new TH1F ("histo_ptTag_ggf", "Pt of tag jets", 100, 0, 400) ;
	histo_ptTag_ggf->SetLineColor (3) ;
	TH1F *histo_etaTag_ggf = new TH1F ("histo_etaTag_ggf", "#eta of tag jets", 100, -6, 6) ;
	histo_etaTag_ggf->SetLineColor (3) ;
	TH1F *histo_numJet_ggf = new TH1F ("histo_numJet_ggf", "# of tag jets", 12, 0, 12) ;
	histo_numJet_ggf->SetLineColor (3) ;
    //ttbar
    TH1F *histo_deltaEtaTag_tt = new TH1F ("histo_deltaEtaTag_tt", "#Delta#eta between tag jets", 100, 0, 10) ;
	histo_deltaEtaTag_tt->SetLineColor (2) ;
	TH1F *histo_invMassTag_tt = new TH1F ("histo_invMassTag_tt", "Invariant mass of tag jets", 100, 0, 4000) ;
	histo_invMassTag_tt->SetLineColor (2) ;
	TH1F *histo_energyTag_tt = new TH1F ("histo_energyTag_tt", "Energy of tag jets", 100, 0, 1200) ;
	histo_energyTag_tt->SetLineColor (2) ;
	TH1F *histo_ptTag_tt = new TH1F ("histo_ptTag_tt", "Pt of tag jets", 100, 0, 400) ;
	histo_ptTag_tt->SetLineColor (2) ;
	TH1F *histo_etaTag_tt = new TH1F ("histo_etaTag_tt", "#eta of tag jets", 100, -6, 6) ;
	histo_etaTag_tt->SetLineColor (2) ;
	TH1F *histo_numJet_tt = new TH1F ("histo_numJet_tt", "# of tag jets", 12, 0, 12) ;
	histo_numJet_tt->SetLineColor (2) ;

	
	TLorentzVector *reco_leadingJetF = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector *reco_leadingJetB = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector recoSommaTag;
	
	
	int nentries = (int) tree->GetEntries();
	TLorentzVector sommaTag;
    for(int i=0; i<nentries; i++)
        {
            tree->GetEntry(i);
			
			twoLargestInvMassJets (*recoJet4Momentum, *reco_leadingJetF, *reco_leadingJetB, numberJet, numberJetAfterCuts ) ; //due recojet con massima massa invariante
			recoSommaTag = *(reco_leadingJetF) + *(reco_leadingJetB);
			
			if (evtFlag== 123 || evtFlag== 124)
			{
			    histo_numJet_VVf -> Fill (numberJetAfterCuts) ;
                if (reco_leadingJetF->E() > 0)
				{ 
				histo_energyTag_VVf -> Fill (reco_leadingJetF->Energy ()) ;
				histo_ptTag_VVf -> Fill (reco_leadingJetF->Pt ()) ;
				histo_etaTag_VVf -> Fill (reco_leadingJetF->Eta ()) ;
                }
				if (reco_leadingJetB->E() > 0)
				{
				histo_energyTag_VVf -> Fill (reco_leadingJetB->Energy ()) ;
				histo_ptTag_VVf -> Fill (reco_leadingJetB->Pt ()) ;
				histo_etaTag_VVf -> Fill (reco_leadingJetB->Eta ()) ;
				}
				if (reco_leadingJetF->E() > 0 && reco_leadingJetB->E())
				{
				histo_invMassTag_VVf -> Fill (recoSommaTag.M()) ;
				histo_deltaEtaTag_VVf -> Fill ((fabs (reco_leadingJetF->Eta () - reco_leadingJetB->Eta ()) )) ;
                }
			}
			else if (evtFlag== 102)
			{
       			histo_numJet_ggf -> Fill (numberJetAfterCuts) ;
			    if (reco_leadingJetF->E() > 0)
				{ 
				histo_energyTag_ggf -> Fill (reco_leadingJetF->Energy ()) ;
				histo_ptTag_ggf -> Fill (reco_leadingJetF->Pt ()) ;
				histo_etaTag_ggf -> Fill (reco_leadingJetF->Eta ()) ;
                }
				if (reco_leadingJetB->E() > 0)
				{
				histo_energyTag_ggf -> Fill (reco_leadingJetB->Energy ()) ;
				histo_ptTag_ggf -> Fill (reco_leadingJetB->Pt ()) ;
				histo_etaTag_ggf -> Fill (reco_leadingJetB->Eta ()) ;
				}
				if (reco_leadingJetF->E() > 0 && reco_leadingJetB->E())
				{
				histo_invMassTag_ggf -> Fill (recoSommaTag.M()) ;
				histo_deltaEtaTag_ggf -> Fill ((fabs (reco_leadingJetF->Eta () - reco_leadingJetB->Eta ()) )) ;
                }

			}
			else
			{
    			histo_numJet_tt -> Fill (numberJetAfterCuts) ;
			    if (reco_leadingJetF->E() > 0)
				{ 
				histo_energyTag_tt -> Fill (reco_leadingJetF->Energy ()) ;
				histo_ptTag_tt -> Fill (reco_leadingJetF->Pt ()) ;
				histo_etaTag_tt -> Fill (reco_leadingJetF->Eta ()) ;
                }
				if (reco_leadingJetB->E() > 0)
				{
				histo_energyTag_tt -> Fill (reco_leadingJetB->Energy ()) ;
				histo_ptTag_tt -> Fill (reco_leadingJetB->Pt ()) ;
				histo_etaTag_tt -> Fill (reco_leadingJetB->Eta ()) ;
				}
				if (reco_leadingJetF->E() > 0 && reco_leadingJetB->E())
				{
				histo_invMassTag_tt -> Fill (recoSommaTag.M()) ;
				histo_deltaEtaTag_tt -> Fill ((fabs (reco_leadingJetF->Eta () - reco_leadingJetB->Eta ()) )) ;
                }

			}
	}		


	histo_deltaEtaTag_VVf -> Scale (1/(histo_deltaEtaTag_VVf->Integral()));
    histo_invMassTag_VVf -> Scale (1/(histo_invMassTag_VVf->Integral()));
	histo_energyTag_VVf -> Scale (1/(histo_energyTag_VVf->Integral()));
	histo_ptTag_VVf -> Scale (1/(histo_ptTag_VVf->Integral()));
	histo_etaTag_VVf -> Scale (1/(histo_etaTag_VVf->Integral()));
	histo_numJet_VVf -> Scale (1/(histo_numJet_VVf->Integral()));	
    histo_deltaEtaTag_ggf -> Scale (1/(histo_deltaEtaTag_ggf->Integral()));
    histo_invMassTag_ggf -> Scale (1/(histo_invMassTag_ggf->Integral()));
	histo_energyTag_ggf -> Scale (1/(histo_energyTag_ggf->Integral()));
	histo_ptTag_ggf -> Scale (1/(histo_ptTag_ggf->Integral()));
	histo_etaTag_ggf -> Scale (1/(histo_etaTag_ggf->Integral()));
	histo_numJet_ggf -> Scale (1/(histo_numJet_ggf->Integral()));
	/*	
	histo_deltaEtaTag_tt -> Scale (1/(histo_deltaEtaTag_tt->Integral()));
    histo_invMassTag_tt -> Scale (1/(histo_invMassTag_tt->Integral()));
	histo_energyTag_tt -> Scale (1/(histo_energyTag_tt->Integral()));
	histo_ptTag_tt -> Scale (1/(histo_ptTag_tt->Integral()));
	histo_etaTag_tt -> Scale (1/(histo_etaTag_tt->Integral()));
	histo_numJet_tt -> Scale (1/(histo_numJet_tt->Integral()));	
	*/
	TLegend *tleg = new TLegend (0.7, 0.6, 0.9, 0.8);
    tleg -> SetBorderSize (0);
    tleg -> SetFillColor (0);
	tleg-> AddEntry (histo_deltaEtaTag_VVf , "VVf","l") ;
	tleg-> AddEntry (histo_deltaEtaTag_ggf , "ggf","l") ;
	tleg-> AddEntry (histo_deltaEtaTag_tt , "tt","l") ;


    TCanvas *pip = new TCanvas () ;
	pip->Divide (3,2);
	pip->cd (1);
    histo_deltaEtaTag_ggf->Draw () ;
	histo_deltaEtaTag_VVf->Draw ("same") ;
	histo_deltaEtaTag_tt->Draw ("same") ;
	tleg-> Draw ("same") ; 
	pip->cd (2);
	histo_invMassTag_ggf->Draw () ;
	histo_invMassTag_VVf->Draw ("same") ;
	histo_invMassTag_tt->Draw ("same") ;
	pip->cd (3);
	histo_energyTag_ggf->Draw () ;
	histo_energyTag_VVf->Draw ("same") ;
	histo_energyTag_tt->Draw ("same") ;
	pip->cd (4);
	histo_ptTag_ggf->Draw () ;
	histo_ptTag_VVf->Draw ("same") ;
	histo_ptTag_tt->Draw ("same") ;
	pip->cd (5);
	histo_etaTag_ggf->Draw () ;
	histo_etaTag_VVf->Draw ("same") ;
	histo_etaTag_tt->Draw ("same") ;
	pip->cd (6);
	histo_numJet_ggf->Draw () ;
	histo_numJet_VVf->Draw ("same") ;
	histo_numJet_tt->Draw ("same") ;
}

///////////////////////////////////////////////////////////////////////////////////////


void jetClonesPlot ()
{
    gROOT->SetStyle("Plain") ;
	
    TFile *f = new TFile("WWF_HIGGS160/WWf_higgsMass160_tree.root") ;
    TTree *tree = (TTree*)gDirectory->Get("tree");  
    
	TClonesArray *genJet4Momentum = new TClonesArray ("TLorentzVector");
	TLorentzVector *genJetCloneTagB = new TLorentzVector();
	TLorentzVector *genJetCloneTagF = new TLorentzVector();
    int numberJet;	    
	int evtFlag;

    ///////////// reco jets
	tree->SetBranchAddress ("numberJet", &numberJet);
	tree->SetBranchAddress ("evtFlag", &evtFlag);
	tree->SetBranchAddress ("genJet4Momentum",  &genJet4Momentum);
	tree->SetBranchAddress ("genJetCloneTagB", &genJetCloneTagB);
	tree->SetBranchAddress ("genJetCloneTagF", &genJetCloneTagF);
	
	TH1F *histo_deltaEtaTag_VVf = new TH1F ("histo_deltaEtaTag_VVf", "#Delta#eta between tag jets (matching with q)", 100, 0, 10) ;
	histo_deltaEtaTag_VVf->SetFillColor (107) ;
	TH1F *histo_invMassTag_VVf = new TH1F ("histo_invMassTag_VVf", "Invariant mass of tag jets (matching with q)", 100, 0, 4000) ;
	histo_invMassTag_VVf->SetFillColor (107) ;
	TH1F *histo_energyTag_VVf = new TH1F ("histo_energyTag_VVf", "Energy of tag jets (matching with q)", 100, 0, 1200) ;
	histo_energyTag_VVf->SetFillColor (107) ;
	TH1F *histo_ptTag_VVf = new TH1F ("histo_ptTag_VVf", "Pt of tag jets (matching with q)", 100, 0, 400) ;
	histo_ptTag_VVf->SetFillColor (107) ;
	TH1F *histo_etaTag_VVf = new TH1F ("histo_etaTag_VVf", "#eta of tag jets (matching with q)", 100, -6, 6) ;
	histo_etaTag_VVf->SetFillColor (107) ;
	
	int nentries = (int) tree->GetEntries();
	TLorentzVector sommaTag;
    for(int i=0; i<nentries; i++)
        {
            tree->GetEntry(i);
			if (evtFlag== 124 || evtFlag== 125)
			{
			    if (genJetCloneTagB->E () != 0) 
				{
				histo_energyTag_VVf -> Fill (genJetCloneTagB->E()) ;
				histo_ptTag_VVf -> Fill (genJetCloneTagB->Pt()) ;
				histo_etaTag_VVf -> Fill (genJetCloneTagB->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (genJetCloneTagF->E () != 0) 
				{
				histo_energyTag_VVf -> Fill (genJetCloneTagF->E()) ;
				histo_ptTag_VVf -> Fill (genJetCloneTagF->Pt()) ;
				histo_etaTag_VVf -> Fill (genJetCloneTagF->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (genJetCloneTagB->E () != 0 && genJetCloneTagF->E () != 0)
				{
				double deltaEtaTag_VVf = fabs (genJetCloneTagB->Eta () - genJetCloneTagF->Eta ()) ; 
				histo_deltaEtaTag_VVf -> Fill (deltaEtaTag_VVf) ;
				sommaTag = *(genJetCloneTagB)+*(genJetCloneTagF);
				histo_invMassTag_VVf -> Fill (sommaTag.M()) ;
				}
			}
       }
	
	 TCanvas *p = new TCanvas ()  ;
	 p->Divide (2, 3);
	 p->cd (1) ;
	 histo_etaTag_VVf->Draw () ;
	 p->cd (2) ;
	 histo_energyTag_VVf->Draw () ;
	 p->cd (3) ;
	 histo_ptTag_VVf->Draw () ;
	 p->cd (4) ; 
	 histo_deltaEtaTag_VVf->Draw () ;
	 p->cd (5) ;
	 histo_invMassTag_VVf -> Draw () ;
	
}

///////////////////////////////////////////////////////////////////////////////////////




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
	TLorentzVector *genJetCloneTagB = new TLorentzVector();
	TLorentzVector *genJetCloneTagF = new TLorentzVector();
    
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
	tree->SetBranchAddress ("genJet4Momentum", &recoJet4Momentum);
	tree->SetBranchAddress ("genJetCloneTagB", &genJetCloneTagB);
	tree->SetBranchAddress ("genJetCloneTagF", &genJetCloneTagF); 

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
	TH1F *histo_etaCloneJetVVF = new TH1F ("histo_etaCloneJetVVF", "#eta of 2 leading jets",100,-10,10) ;
    histo_etaCloneJetVVF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_etaCloneJetVVF->SetLineColor (4);

    TH1F *histo_energyLeadingJetVVF = new TH1F ("histo_energyLeadingJetVVF", "#energy of 2 leading jets",100,0,5000) ;
    histo_energyLeadingJetVVF->GetXaxis ()->SetTitle ("#energy of leading jets") ;
    histo_energyLeadingJetVVF->SetLineColor (2);
    TH1F *histo_energyLeadingJetggF = new TH1F ("histo_energyLeadingJetggF", "#energy of 2 leading jets",100,0,5000) ;
    histo_energyLeadingJetggF->GetXaxis ()->SetTitle ("#energy of leading jets") ;
    histo_energyLeadingJetggF->SetLineColor (3);
    TH1F *histo_energyCloneJetVVF = new TH1F ("histo_energyCloneJetVVF", "#energy of 2 leading jets",100,0,5000) ;
    histo_energyCloneJetVVF->GetXaxis ()->SetTitle ("#energy of leading jets") ;
    histo_energyCloneJetVVF->SetLineColor (4);
	
	TH1F *histo_deltaEtaLeadingJetVVF = new TH1F ("histo_deltaEtaLeadingJetVVF", "#Delta#eta between 2 leading jets",100,-0.5,20) ;
    histo_deltaEtaLeadingJetVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaLeadingJetVVF->SetLineColor (2);
    TH1F *histo_deltaEtaLeadingJetggF = new TH1F ("histo_deltaEtaLeadingJetggF", "#Delta#eta of 2 leading jets",100,-0.5,20) ;
    histo_deltaEtaLeadingJetggF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaLeadingJetggF->SetLineColor (3);
    TH1F *histo_deltaEtaCloneJetVVF = new TH1F ("histo_deltaEtaCloneJetVVF", "#Delta#eta of 2 leading jets",100,-0.5,20) ;
    histo_deltaEtaCloneJetVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaCloneJetVVF->SetLineColor (4);
	
	TH1F *histo_ptLeadingJetVVF = new TH1F ("histo_ptLeadingJetVVF", "#pt of 2 leading jets",100,0,200) ;
    histo_ptLeadingJetVVF->GetXaxis ()->SetTitle ("#pt of leading jets") ;
    histo_ptLeadingJetVVF->SetLineColor (2);
    TH1F *histo_ptLeadingJetggF = new TH1F ("histo_ptLeadingJetggF", "#pt of 2 leading jets",100,0,200) ;
    histo_ptLeadingJetggF->GetXaxis ()->SetTitle ("#pt of leading jets") ;
    histo_ptLeadingJetggF->SetLineColor (3);
	TH1F *histo_ptCloneJetVVF = new TH1F ("histo_ptCloneJetVVF", "#pt of 2 leading jets",100,0,200) ;
    histo_ptCloneJetVVF->GetXaxis ()->SetTitle ("#pt of leading jets") ;
    histo_ptCloneJetVVF->SetLineColor (4);
	
	TH1F *histo_invMassLeadingJetVVF = new TH1F ("histo_invMassLeadingJetVVF", "#invMass of 2 leading jets",100,0,5000) ;
    histo_invMassLeadingJetVVF->GetXaxis ()->SetTitle ("#invMass of leading jets") ;
    histo_invMassLeadingJetVVF->SetLineColor (2);
    TH1F *histo_invMassLeadingJetggF = new TH1F ("histo_invMassLeadingJetggF", "#invMass of 2 leading jets",100,0,5000) ;
    histo_invMassLeadingJetggF->GetXaxis ()->SetTitle ("#invMass of leading jets") ;
    histo_invMassLeadingJetggF->SetLineColor (3);
	TH1F *histo_invMassCloneJetVVF = new TH1F ("histo_invMassCloneJetVVF", "#invMass of 2 leading jets",100,0,5000) ;
    histo_invMassCloneJetVVF->GetXaxis ()->SetTitle ("#invMass of leading jets") ;
    histo_invMassCloneJetVVF->SetLineColor (4);

	
	
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
    		//twoLeadingJets (*recoJet4Momentum, *leadingJet1, *leadingJet2) ; //due jet con maggior pt
			twoLargestInvMassJets (*recoJet4Momentum, *leadingJet1, *leadingJet2) ; //due jet con massima massa invariante
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
				histo_invMassLeadingJetVVF -> Fill (sommaTag.M()) ;
				double deltaEta = (fabs (leadingJet1->Eta () - leadingJet2->Eta ()) ) ;
				//if (deltaEta > 3) histo_deltaEtaLeadingJetVVF -> Fill (deltaEta) ;
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
				histo_invMassLeadingJetggF -> Fill (sommaTag.M()) ;
				double deltaEta2 = (fabs (leadingJet1->Eta () - leadingJet2->Eta ()) ) ;
				//if (deltaEta2 > 3) 
				histo_deltaEtaLeadingJetggF -> Fill (deltaEta2) ;
			}
			
			/////////////////////////////////// clone
			if (evtFlag== 124 || evtFlag== 125)
			{
			    if (genJetCloneTagB->E () != 0) 
				{
				histo_energyCloneJetVVF -> Fill (genJetCloneTagB->E()) ;
				histo_ptCloneJetVVF -> Fill (genJetCloneTagB->Pt()) ;
				histo_etaCloneJetVVF -> Fill (genJetCloneTagB->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (genJetCloneTagF->E () != 0) 
				{
				histo_energyCloneJetVVF -> Fill (genJetCloneTagF->E()) ;
				histo_ptCloneJetVVF -> Fill (genJetCloneTagF->Pt()) ;
				histo_etaCloneJetVVF -> Fill (genJetCloneTagF->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (genJetCloneTagB->E () != 0 && genJetCloneTagF->E () != 0)
				{
				double deltaEtaTag_VVf = fabs (genJetCloneTagB->Eta () - genJetCloneTagF->Eta ()) ; 
				histo_deltaEtaCloneJetVVF -> Fill (deltaEtaTag_VVf) ;
				sommaTag = *(genJetCloneTagB)+*(genJetCloneTagF);
				histo_invMassCloneJetVVF -> Fill (sommaTag.M()) ;
				}
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
	histo_invMassCloneJetVVF -> Scale (1/(histo_invMassCloneJetVVF->Integral()));
	histo_deltaEtaCloneJetVVF -> Scale (1/(histo_deltaEtaCloneJetVVF->Integral()));
	histo_energyCloneJetVVF -> Scale (1/(histo_energyCloneJetVVF->Integral()));
	histo_ptCloneJetVVF -> Scale (1/(histo_ptCloneJetVVF->Integral()));
	histo_etaCloneJetVVF -> Scale (1/(histo_etaCloneJetVVF->Integral()));
	
	cout << "VVf #eventi prima del taglio in deltaEta" << histo_deltaEtaLeadingJetVVF->GetEntries() << endl ;		
	cout << "ggf #eventi prima del taglio in deltaEta" << histo_deltaEtaLeadingJetggF->GetEntries() << endl ;		
			
	TLegend *tleg = new TLegend (0.7, 0.6, 0.9, 0.8);
    tleg -> SetBorderSize (0);
    tleg -> SetFillColor (0);
    tleg-> AddEntry (histo_recLep_eVVF, "VV fusion","l") ;
    tleg-> AddEntry (histo_recLep_eggF, "gg fusion","l") ;
	tleg-> AddEntry (histo_etaCloneJetVVF, "clones","l") ;

 
    TCanvas *p=new TCanvas () ;
	p->Divide (3,2) ;
	/*
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
	*/
	p->cd (1);
	histo_energyLeadingJetVVF->Draw () ;
	histo_energyLeadingJetggF->Draw ("same") ;
	histo_energyCloneJetVVF->Draw ("same") ;
    p->cd (2);
	histo_etaLeadingJetggF->Draw () ;
	histo_etaLeadingJetVVF->Draw ("same") ;
	histo_etaCloneJetVVF->Draw ("same") ; 
	p->cd (3);
	histo_deltaEtaLeadingJetggF->Draw () ;
	histo_deltaEtaLeadingJetVVF->Draw ("same") ;
	histo_deltaEtaCloneJetVVF->Draw ("same") ;
	p->cd (4);
	histo_ptLeadingJetggF->Draw () ;
	histo_ptLeadingJetVVF->Draw ("same") ;
	histo_ptCloneJetVVF->Draw ("same") ; 
	p->cd (5);
	histo_invMassLeadingJetggF->Draw () ;
	histo_invMassLeadingJetVVF->Draw ("same") ;
	histo_invMassCloneJetVVF->Draw ("same") ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void jetPlots ()
{
    gROOT->SetStyle("Plain") ;
	
    TFile *f = new TFile("WWF_HIGGS160/WWf_higgsMass160_tree.root") ;
    TTree *tree = (TTree*)gDirectory->Get("tree");  
        
	int evtFlag;
	int numberJet;
	int numberGenJet;
	
	TClonesArray *genJet4Momentum = new TClonesArray ("TLorentzVector");
	TClonesArray *recoJet4Momentum = new TClonesArray ("TLorentzVector");
	TLorentzVector *genJetCloneTagB = new TLorentzVector();
	TLorentzVector *genJetCloneTagF = new TLorentzVector();
	TLorentzVector *recoJetCloneTagB = new TLorentzVector();
	TLorentzVector *recoJetCloneTagF = new TLorentzVector();
	TLorentzVector *genqTagF = new TLorentzVector();
    TLorentzVector *genqTagB = new TLorentzVector();
    
	tree->SetBranchAddress ("evtFlag", &evtFlag);
	tree->SetBranchAddress ("numberJet", &numberJet);
	tree->SetBranchAddress ("numberGenJet", &numberGenJet);
	tree->SetBranchAddress ("genJet4Momentum", &genJet4Momentum);
	tree->SetBranchAddress ("recoJet4Momentum", &recoJet4Momentum);
	tree->SetBranchAddress ("genJetCloneTagB", &genJetCloneTagB);
	tree->SetBranchAddress ("genJetCloneTagF", &genJetCloneTagF); 
	tree->SetBranchAddress ("recoJetCloneTagB", &recoJetCloneTagB);
	tree->SetBranchAddress ("recoJetCloneTagF", &recoJetCloneTagF); 
    tree->SetBranchAddress ("genqTagB", &genqTagB);
	tree->SetBranchAddress ("genqTagF", &genqTagF); 


	//2 leading jets eta
	TH1F *histo_etaLeadingJetVVF = new TH1F ("histo_etaLeadingJetVVF", "#eta of 2 leading jets",100,-10,10) ;
    histo_etaLeadingJetVVF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_etaLeadingJetVVF->SetLineColor (2);
    TH1F *histo_etaLeadingJetggF = new TH1F ("histo_etaLeadingJetggF", "#eta of 2 leading jets",100,-10,10) ;
    histo_etaLeadingJetggF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_etaLeadingJetggF->SetLineColor (1);
	TH1F *histo_reco_etaLeadingJetVVF = new TH1F ("histo_reco_etaLeadingJetVVF", "#eta of 2 leading jets",100,-10,10) ;
    histo_reco_etaLeadingJetVVF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_reco_etaLeadingJetVVF->SetLineColor (8);
    TH1F *histo_reco_etaLeadingJetggF = new TH1F ("histo_reco_etaLeadingJetggF", "#eta of 2 leading jets",100,-10,10) ;
    histo_reco_etaLeadingJetggF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_reco_etaLeadingJetggF->SetLineColor (6);
	TH1F *histo_etaCloneJetVVF = new TH1F ("histo_etaCloneJetVVF", "#eta of 2 leading jets",100,-10,10) ;
    histo_etaCloneJetVVF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_etaCloneJetVVF->SetLineColor (4);
	TH1F *histo_reco_etaCloneJetVVF = new TH1F ("histo_reco_etaCloneJetVVF", "#eta of 2 leading jets",100,-10,10) ;
    histo_reco_etaCloneJetVVF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_reco_etaCloneJetVVF->SetLineColor (94);
	TH1F *histo_etaqVVF = new TH1F ("histo_etaqVVF", "#eta of 2 leading jets",100,-10,10) ;
    histo_etaqVVF->GetXaxis ()->SetTitle ("#eta of leading jets") ;
    histo_etaqVVF->SetLineColor (7);
	//2 leading jets energy
    TH1F *histo_energyLeadingJetVVF = new TH1F ("histo_energyLeadingJetVVF", "energy of 2 leading jets",100,0,5000) ;
    histo_energyLeadingJetVVF->GetXaxis ()->SetTitle ("energy of leading jets") ;
    histo_energyLeadingJetVVF->SetLineColor (2);
    TH1F *histo_energyLeadingJetggF = new TH1F ("histo_energyLeadingJetggF", "energy of 2 leading jets",100,0,5000) ;
    histo_energyLeadingJetggF->GetXaxis ()->SetTitle ("energy of leading jets") ;
    histo_energyLeadingJetggF->SetLineColor (1);
    TH1F *histo_reco_energyLeadingJetVVF = new TH1F ("histo_reco_energyLeadingJetVVF", "energy of 2 leading jets",100,0,5000) ;
    histo_reco_energyLeadingJetVVF->GetXaxis ()->SetTitle ("energy of leading jets") ;
    histo_reco_energyLeadingJetVVF->SetLineColor (8);
    TH1F *histo_reco_energyLeadingJetggF = new TH1F ("histo_reco_energyLeadingJetggF", "energy of 2 leading jets",100,0,5000) ;
    histo_reco_energyLeadingJetggF->GetXaxis ()->SetTitle ("energy of leading jets") ;
    histo_reco_energyLeadingJetggF->SetLineColor (6);
    TH1F *histo_energyCloneJetVVF = new TH1F ("histo_energyCloneJetVVF", "energy of 2 leading jets",100,0,5000) ;
    histo_energyCloneJetVVF->GetXaxis ()->SetTitle ("energy of leading jets") ;
    histo_energyCloneJetVVF->SetLineColor (4);
    TH1F *histo_reco_energyCloneJetVVF = new TH1F ("histo_reco_energyCloneJetVVF", "energy of 2 leading jets",100,0,5000) ;
    histo_reco_energyCloneJetVVF->GetXaxis ()->SetTitle ("energy of leading jets") ;
    histo_reco_energyCloneJetVVF->SetLineColor (94);
	TH1F *histo_energyqVVF = new TH1F ("histo_energyqVVF", "energy of 2 leading jets",100,0,5000) ;
    histo_energyqVVF->GetXaxis ()->SetTitle ("energy of leading jets") ;
    histo_energyqVVF->SetLineColor (7);
	//2 leading jets deltaEta
	TH1F *histo_deltaEtaLeadingJetVVF = new TH1F ("histo_deltaEtaLeadingJetVVF", "#Delta#eta between 2 leading jets",100,-0.5,12) ;
    histo_deltaEtaLeadingJetVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaLeadingJetVVF->SetLineColor (2);
    TH1F *histo_deltaEtaLeadingJetggF = new TH1F ("histo_deltaEtaLeadingJetggF", "#Delta#eta of 2 leading jets",100,-0.5,12) ;
    histo_deltaEtaLeadingJetggF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaLeadingJetggF->SetLineColor (1);
	TH1F *histo_reco_deltaEtaLeadingJetVVF = new TH1F ("histo_reco_deltaEtaLeadingJetVVF", "#Delta#eta between 2 leading jets",100,-0.5,12) ;
    histo_reco_deltaEtaLeadingJetVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_reco_deltaEtaLeadingJetVVF->SetLineColor (8);
    TH1F *histo_reco_deltaEtaLeadingJetggF = new TH1F ("histo_reco_deltaEtaLeadingJetggF", "#Delta#eta of 2 leading jets",100,-0.5,12) ;
    histo_reco_deltaEtaLeadingJetggF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_reco_deltaEtaLeadingJetggF->SetLineColor (6);
    TH1F *histo_deltaEtaCloneJetVVF = new TH1F ("histo_deltaEtaCloneJetVVF", "#Delta#eta of 2 leading jets",100,-0.5,12) ;
    histo_deltaEtaCloneJetVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaCloneJetVVF->SetLineColor (4);
	TH1F *histo_reco_deltaEtaCloneJetVVF = new TH1F ("histo_reco_deltaEtaCloneJetVVF", "#Delta#eta of 2 leading jets",100,-0.5,12) ;
    histo_reco_deltaEtaCloneJetVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_reco_deltaEtaCloneJetVVF->SetLineColor (94);
	TH1F *histo_deltaEtaqVVF = new TH1F ("histo_deltaEtaqVVF", "#Delta#eta of 2 leading jets",100,-0.5,12) ;
    histo_deltaEtaqVVF->GetXaxis ()->SetTitle ("#Delta#eta between leading jets") ;
    histo_deltaEtaqVVF->SetLineColor (7);
    //2 leading jets deltaPhi
	TH1F *histo_deltaPhiLeadingJetVVF = new TH1F ("histo_deltaPhiLeadingJetVVF", "#Delta#phi between 2 leading jets",50,0,7) ;
    histo_deltaPhiLeadingJetVVF->GetXaxis ()->SetTitle ("#Delta#phi between leading jets") ;
    histo_deltaPhiLeadingJetVVF->SetLineColor (2);
    TH1F *histo_deltaPhiLeadingJetggF = new TH1F ("histo_deltaPhiLeadingJetggF", "#Delta#phi of 2 leading jets",50,0, 7) ;
    histo_deltaPhiLeadingJetggF->GetXaxis ()->SetTitle ("#Delta#phi between leading jets") ;
    histo_deltaPhiLeadingJetggF->SetLineColor (1);
	TH1F *histo_reco_deltaPhiLeadingJetVVF = new TH1F ("histo_reco_deltaPhiLeadingJetVVF", "#Delta#phi between 2 leading jets",50,0, 7) ;
    histo_reco_deltaPhiLeadingJetVVF->GetXaxis ()->SetTitle ("#Delta#phi between leading jets") ;
    histo_reco_deltaPhiLeadingJetVVF->SetLineColor (8);
    TH1F *histo_reco_deltaPhiLeadingJetggF = new TH1F ("histo_reco_deltaPhiLeadingJetggF", "#Delta#phi of 2 leading jets",50,0, 7) ;
    histo_reco_deltaPhiLeadingJetggF->GetXaxis ()->SetTitle ("#Delta#phi between leading jets") ;
    histo_reco_deltaPhiLeadingJetggF->SetLineColor (6);
    TH1F *histo_deltaPhiCloneJetVVF = new TH1F ("histo_deltaPhiCloneJetVVF", "#Delta#phi of 2 leading jets",50,0, 7) ;
    histo_deltaPhiCloneJetVVF->GetXaxis ()->SetTitle ("#Delta#phi between leading jets") ;
    histo_deltaPhiCloneJetVVF->SetLineColor (4);
    TH1F *histo_reco_deltaPhiCloneJetVVF = new TH1F ("histo_reco_deltaPhiCloneJetVVF", "#Delta#phi of 2 leading jets",50,0, 7) ;
    histo_reco_deltaPhiCloneJetVVF->GetXaxis ()->SetTitle ("#Delta#phi between leading jets") ;
    histo_reco_deltaPhiCloneJetVVF->SetLineColor (94);
	TH1F *histo_deltaPhiqVVF = new TH1F ("histo_deltaPhiqVVF", "#Delta#phi of 2 leading jets",50,0, 7) ;
    histo_deltaPhiqVVF->GetXaxis ()->SetTitle ("#Delta#phi between leading jets") ;
    histo_deltaPhiqVVF->SetLineColor (7);
    //2 leading jets deltaR
	TH1F *histo_deltaRLeadingJetVVF = new TH1F ("histo_deltaRLeadingJetVVF", "#DeltaR between 2 leading jets",100,0, 10) ;
    histo_deltaRLeadingJetVVF->GetXaxis ()->SetTitle ("#DeltaR between leading jets") ;
    histo_deltaRLeadingJetVVF->SetLineColor (2);
    TH1F *histo_deltaRLeadingJetggF = new TH1F ("histo_deltaRLeadingJetggF", "#DeltaR of 2 leading jets",100,0, 10) ;
    histo_deltaRLeadingJetggF->GetXaxis ()->SetTitle ("#DeltaR between leading jets") ;
    histo_deltaRLeadingJetggF->SetLineColor (1);
	TH1F *histo_reco_deltaRLeadingJetVVF = new TH1F ("histo_reco_deltaRLeadingJetVVF", "#DeltaR between 2 leading jets",100,0, 10) ;
    histo_reco_deltaRLeadingJetVVF->GetXaxis ()->SetTitle ("#DeltaR between leading jets") ;
    histo_reco_deltaRLeadingJetVVF->SetLineColor (8);
    TH1F *histo_reco_deltaRLeadingJetggF = new TH1F ("histo_reco_deltaRLeadingJetggF", "#DeltaR of 2 leading jets",100,0, 10) ;
    histo_reco_deltaRLeadingJetggF->GetXaxis ()->SetTitle ("#DeltaR between leading jets") ;
    histo_reco_deltaRLeadingJetggF->SetLineColor (6);
    TH1F *histo_deltaRCloneJetVVF = new TH1F ("histo_deltaRCloneJetVVF", "#DeltaR of 2 leading jets",100,0, 10) ;
    histo_deltaRCloneJetVVF->GetXaxis ()->SetTitle ("#DeltaR between leading jets") ;
    histo_deltaRCloneJetVVF->SetLineColor (4);
	TH1F *histo_reco_deltaRCloneJetVVF = new TH1F ("histo_reco_deltaRCloneJetVVF", "#DeltaR of 2 leading jets",100,0, 10) ;
    histo_reco_deltaRCloneJetVVF->GetXaxis ()->SetTitle ("#DeltaR between leading jets") ;
    histo_reco_deltaRCloneJetVVF->SetLineColor (94);
	TH1F *histo_deltaRqVVF = new TH1F ("histo_deltaRqVVF", "#DeltaR of 2 leading jets",100,0, 10) ;
    histo_deltaRqVVF->GetXaxis ()->SetTitle ("#DeltaR between leading jets") ;
    histo_deltaRqVVF->SetLineColor (7);
	//2 leading jets pt
	TH1F *histo_ptLeadingJetVVF = new TH1F ("histo_ptLeadingJetVVF", "pt of 2 leading jets",100,0,200) ;
    histo_ptLeadingJetVVF->GetXaxis ()->SetTitle ("pt of leading jets") ;
    histo_ptLeadingJetVVF->SetLineColor (2);
    TH1F *histo_ptLeadingJetggF = new TH1F ("histo_ptLeadingJetggF", "pt of 2 leading jets",100,0,200) ;
    histo_ptLeadingJetggF->GetXaxis ()->SetTitle ("pt of leading jets") ;
    histo_ptLeadingJetggF->SetLineColor (1);
    TH1F *histo_reco_ptLeadingJetVVF = new TH1F ("histo_reco_ptLeadingJetVVF", "pt of 2 leading jets",100,0,200) ;
    histo_reco_ptLeadingJetVVF->GetXaxis ()->SetTitle ("pt of leading jets") ;
    histo_reco_ptLeadingJetVVF->SetLineColor (8);
    TH1F *histo_reco_ptLeadingJetggF = new TH1F ("histo_reco_ptLeadingJetggF", "pt of 2 leading jets",100,0,200) ;
    histo_reco_ptLeadingJetggF->GetXaxis ()->SetTitle ("pt of leading jets") ;
    histo_reco_ptLeadingJetggF->SetLineColor (6);
	TH1F *histo_ptCloneJetVVF = new TH1F ("histo_ptCloneJetVVF", "pt of 2 leading jets",100,0,200) ;
    histo_ptCloneJetVVF->GetXaxis ()->SetTitle ("pt of leading jets") ;
    histo_ptCloneJetVVF->SetLineColor (4);
	TH1F *histo_reco_ptCloneJetVVF = new TH1F ("histo_reco_ptCloneJetVVF", "pt of 2 leading jets",100,0,200) ;
    histo_reco_ptCloneJetVVF->GetXaxis ()->SetTitle ("pt of leading jets") ;
    histo_reco_ptCloneJetVVF->SetLineColor (94);
	TH1F *histo_ptqVVF = new TH1F ("histo_ptqVVF", "pt of 2 leading jets",100,0,200) ;
    histo_ptqVVF->GetXaxis ()->SetTitle ("pt of leading jets") ;
    histo_ptqVVF->SetLineColor (7);
	//2 leading jets invariant mass
	TH1F *histo_invMassLeadingJetVVF = new TH1F ("histo_invMassLeadingJetVVF", "Invariant mass of 2 leading jets",100,0,5000) ;
    histo_invMassLeadingJetVVF->GetXaxis ()->SetTitle ("invMass of leading jets") ;
    histo_invMassLeadingJetVVF->SetLineColor (2);
    TH1F *histo_invMassLeadingJetggF = new TH1F ("histo_invMassLeadingJetggF", "Invariant mass of 2 leading jets",100,0,5000) ;
    histo_invMassLeadingJetggF->GetXaxis ()->SetTitle ("invMass of leading jets") ;
    histo_invMassLeadingJetggF->SetLineColor (1);
	TH1F *histo_reco_invMassLeadingJetVVF = new TH1F ("histo_reco_invMassLeadingJetVVF", "Invariant mass of 2 leading jets",100,0,5000) ;
    histo_reco_invMassLeadingJetVVF->GetXaxis ()->SetTitle ("invMass of leading jets") ;
    histo_reco_invMassLeadingJetVVF->SetLineColor (8);
    TH1F *histo_reco_invMassLeadingJetggF = new TH1F ("histo_reco_invMassLeadingJetggF", "Invariant mass of 2 leading jets",100,0,5000) ;
    histo_reco_invMassLeadingJetggF->GetXaxis ()->SetTitle ("invMass of leading jets") ;
    histo_reco_invMassLeadingJetggF->SetLineColor (6);
	TH1F *histo_invMassCloneJetVVF = new TH1F ("histo_invMassCloneJetVVF", "Invariant mass of 2 leading jets",100,0,5000) ;
    histo_invMassCloneJetVVF->GetXaxis ()->SetTitle ("invMass of leading jets") ;
    histo_invMassCloneJetVVF->SetLineColor (4);
    TH1F *histo_reco_invMassCloneJetVVF = new TH1F ("histo_reco_invMassCloneJetVVF", "Invariant mass of 2 leading jets",100,0,5000) ;
    histo_reco_invMassCloneJetVVF->GetXaxis ()->SetTitle ("invMass of leading jets") ;
    histo_reco_invMassCloneJetVVF->SetLineColor (94);
	TH1F *histo_invMassqVVF = new TH1F ("histo_invMassqVVF", "Invariant mass of 2 leading jets",100,0,5000) ;
    histo_invMassqVVF->GetXaxis ()->SetTitle ("invMass of leading jets") ;
    histo_invMassqVVF->SetLineColor (7);
	
	
	//resolution plot between cloneJet and selected genJet
	TH1F *histo_energyResoCloneSelected = new TH1F ("histo_energyResoCloneSelected", "Energy difference between clone and selected tag jet",100,-300,300) ;
    histo_energyResoCloneSelected ->GetXaxis ()->SetTitle ("Energy difference") ;
    histo_energyResoCloneSelected ->SetFillColor (34);
    TH1F *histo_deltaRCloneSelected = new TH1F ("histo_deltaRCloneSelected", "dR between clone and selected tag jet",100,-10,10) ;
    histo_deltaRCloneSelected ->GetXaxis ()->SetTitle ("dR") ;
    histo_deltaRCloneSelected ->SetFillColor (34);
	
    TLorentzVector *leadingJetF = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector *leadingJetB = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector *reco_leadingJetF = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector *reco_leadingJetB = new TLorentzVector (0.0, 0.0, 0.0, 0.0);
	TLorentzVector genSommaTag;
	TLorentzVector recoSommaTag;
	TLorentzVector genCloneSommaTag;
	TLorentzVector recoCloneSommaTag;
	TLorentzVector qSommaTag;
	
    int nentries = (int) tree->GetEntries();
	
	int purezzaF = 0;
	int purezzaB = 0;
	int purezzaBoth = 0;
	int reco_purezzaF = 0;
	int reco_purezzaB = 0;
	int reco_purezzaBoth = 0;
	int nvff = 0;
	int nClonesF = 0;
	int nClonesB = 0;
	int nClonesBoth = 0;
	int reco_nClonesF = 0;
	int reco_nClonesB = 0;
	int reco_nClonesBoth = 0;
	
	//nentries
    for(int i=0; i<nentries; i++)
        {
            tree->GetEntry(i);

			/*
			if (genJetCloneTagB->E () != 0 && genJetCloneTagF->E () == 0) 
				nClonesB++;
			if (genJetCloneTagF->E () != 0 && genJetCloneTagB->E () == 0) 
				nClonesF++;
			if (genJetCloneTagF->E () != 0 && genJetCloneTagB->E () != 0) 
				nClonesBoth++;
			}
			
		}
	cout << "nClonesF = " << nClonesF << endl;
	cout << "nClonesB = " << nClonesB << endl;
	cout << "nClonesBoth = " << nClonesBoth << endl;
	*/
			twoLargestInvMassJets (*genJet4Momentum, *leadingJetF, *leadingJetB, numberGenJet ) ; //due genjet con massima massa invariante
			twoLargestInvMassJets (*recoJet4Momentum, *reco_leadingJetF, *reco_leadingJetB, numberJet ) ; //due recojet con massima massa invariante
			genSommaTag = *(leadingJetF) + *(leadingJetB);
			recoSommaTag = *(reco_leadingJetF) + *(reco_leadingJetB);
				
			if (evtFlag == 123 || evtFlag == 124) //VV fusion
			{

			    nvff++;
				// generated tag quarks
				histo_deltaEtaqVVF -> Fill (fabs (genqTagB->Eta () - genqTagF->Eta ())) ;
				histo_deltaPhiqVVF -> Fill (fabs (genqTagB->Phi () - genqTagF->Phi ())) ;
				histo_deltaRqVVF -> Fill (genqTagB->DeltaR (*genqTagF));
				qSommaTag = *(genqTagB)+*(genqTagF);
				histo_invMassqVVF -> Fill (qSommaTag.M()) ;
				histo_energyqVVF -> Fill (genqTagB->E()) ;
				histo_ptqVVF -> Fill (genqTagB->Pt()) ;
				histo_etaqVVF -> Fill (genqTagB->Eta()) ;
				histo_energyqVVF -> Fill (genqTagF->E()) ;
				histo_ptqVVF -> Fill (genqTagF->Pt()) ;
				histo_etaqVVF -> Fill (genqTagF->Eta()) ;

                // leading jet tra i genJet
				if (leadingJetF->E() > 0)
				{ 
				histo_energyLeadingJetVVF -> Fill (leadingJetF->Energy ()) ;
				histo_ptLeadingJetVVF -> Fill (leadingJetF->Pt ()) ;
				histo_etaLeadingJetVVF -> Fill (leadingJetF->Eta ()) ;
                }
				if (leadingJetB->E() > 0)
				{
				histo_energyLeadingJetVVF -> Fill (leadingJetB->Energy ()) ;
				histo_ptLeadingJetVVF -> Fill (leadingJetB->Pt ()) ;
				histo_etaLeadingJetVVF -> Fill (leadingJetB->Eta ()) ;
				}
				if (leadingJetF->E() > 0 && leadingJetB->E())
				{
				histo_invMassLeadingJetVVF -> Fill (recoSommaTag.M()) ;
				histo_deltaEtaLeadingJetVVF -> Fill ((fabs (leadingJetF->Eta () - leadingJetB->Eta ()) )) ;
				histo_deltaPhiLeadingJetVVF -> Fill ((fabs (leadingJetF->Phi () - leadingJetB->Phi ()) )) ;
				histo_deltaRLeadingJetVVF -> Fill (leadingJetF->DeltaR (*leadingJetB)) ;
                }

                // leading jet tra i recoJet
                if (reco_leadingJetF->E() > 0)
				{ 
				histo_reco_energyLeadingJetVVF -> Fill (reco_leadingJetF->Energy ()) ;
				histo_reco_ptLeadingJetVVF -> Fill (reco_leadingJetF->Pt ()) ;
				histo_reco_etaLeadingJetVVF -> Fill (reco_leadingJetF->Eta ()) ;
                }
				if (reco_leadingJetB->E() > 0)
				{
				histo_reco_energyLeadingJetVVF -> Fill (reco_leadingJetB->Energy ()) ;
				histo_reco_ptLeadingJetVVF -> Fill (reco_leadingJetB->Pt ()) ;
				histo_reco_etaLeadingJetVVF -> Fill (reco_leadingJetB->Eta ()) ;
				}
				if (reco_leadingJetF->E() > 0 && reco_leadingJetB->E())
				{
				histo_reco_invMassLeadingJetVVF -> Fill (recoSommaTag.M()) ;
				histo_reco_deltaEtaLeadingJetVVF -> Fill ((fabs (reco_leadingJetF->Eta () - reco_leadingJetB->Eta ()) )) ;
				histo_reco_deltaPhiLeadingJetVVF -> Fill ((fabs (reco_leadingJetF->Phi () - reco_leadingJetB->Phi ()) )) ;
				histo_reco_deltaRLeadingJetVVF -> Fill (reco_leadingJetF->DeltaR (*reco_leadingJetB)) ;
                }
				
				/////////////////// gen clones /////////////////////////////////
				if (genJetCloneTagB->E () != 0) 
				{
				nClonesB++;
				histo_energyResoCloneSelected -> Fill (leadingJetB->E()- genJetCloneTagB->E()) ;
				histo_deltaRCloneSelected -> Fill (leadingJetB->DeltaR(*genJetCloneTagB)) ;
				if (((*leadingJetB) == (*genJetCloneTagB)) && ((*leadingJetF) != (*genJetCloneTagF))) purezzaB++ ;
				histo_energyCloneJetVVF -> Fill (genJetCloneTagB->E()) ;
				histo_ptCloneJetVVF -> Fill (genJetCloneTagB->Pt()) ;
				histo_etaCloneJetVVF -> Fill (genJetCloneTagB->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (genJetCloneTagF->E () != 0) 
				{
				nClonesF++;
				histo_energyResoCloneSelected -> Fill (leadingJetF->E()- genJetCloneTagF->E()) ;
				histo_deltaRCloneSelected -> Fill (leadingJetF->DeltaR(*genJetCloneTagF)) ;
				if (((*leadingJetF) == (*genJetCloneTagF)) && ((*leadingJetB) != (*genJetCloneTagB))) purezzaF++ ;
				histo_energyCloneJetVVF -> Fill (genJetCloneTagF->E()) ;
				histo_ptCloneJetVVF -> Fill (genJetCloneTagF->Pt()) ;
				histo_etaCloneJetVVF -> Fill (genJetCloneTagF->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (genJetCloneTagB->E () != 0 && genJetCloneTagF->E () != 0)
				{
				nClonesBoth++;
				if (((*leadingJetF) == (*genJetCloneTagF)) && ((*leadingJetB) == (*genJetCloneTagB))) purezzaBoth++ ;
				genCloneSommaTag = *(genJetCloneTagB)+*(genJetCloneTagF);
				histo_invMassCloneJetVVF -> Fill (genCloneSommaTag.M()) ;
				histo_deltaEtaCloneJetVVF -> Fill (fabs (genJetCloneTagB->Eta () - genJetCloneTagF->Eta ())) ;
                histo_deltaPhiCloneJetVVF -> Fill (fabs (genJetCloneTagB->Phi () - genJetCloneTagF->Phi ())) ;
                histo_deltaRCloneJetVVF -> Fill (genJetCloneTagB->DeltaR (*genJetCloneTagF)) ;
				}

                /////////////////// reco clones /////////////////////////////////
				if (recoJetCloneTagB->E () != 0) 
				{
				reco_nClonesB++;
				
				//histo_reco_energyResoCloneSelected -> Fill (reco_leadingJetB->E()- recoJetCloneTagB->E()) ;
				//histo_reco_deltaRCloneSelected -> Fill (reco_leadingJetB->DeltaR(*recoJetCloneTagB)) ;
				if (((*reco_leadingJetB) == (*recoJetCloneTagB)) && ((*reco_leadingJetF) != (*recoJetCloneTagF))) reco_purezzaB++ ;
				histo_reco_energyCloneJetVVF -> Fill (recoJetCloneTagB->E()) ;
				histo_reco_ptCloneJetVVF -> Fill (recoJetCloneTagB->Pt()) ;
				histo_reco_etaCloneJetVVF -> Fill (recoJetCloneTagB->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (recoJetCloneTagF->E () != 0) 
				{
				reco_nClonesF++;
				//histo_reco_energyResoCloneSelected -> Fill (reco_leadingJetF->E()- recoJetCloneTagF->E()) ;
				//histo_reco_deltaRCloneSelected -> Fill (reco_leadingJetF->DeltaR(*recoJetCloneTagF)) ;
				if (((*reco_leadingJetF) == (*recoJetCloneTagF)) && ((*reco_leadingJetB) != (*recoJetCloneTagB))) reco_purezzaF++ ;
				histo_reco_energyCloneJetVVF -> Fill (recoJetCloneTagF->E()) ;
				histo_reco_ptCloneJetVVF -> Fill (recoJetCloneTagF->Pt()) ;
				histo_reco_etaCloneJetVVF -> Fill (recoJetCloneTagF->Eta()) ;
				}
				/////////////////////////////////////////////////////////////
				if (recoJetCloneTagB->E () != 0 && recoJetCloneTagF->E () != 0)
				{
				reco_nClonesBoth++;
				if (((*reco_leadingJetF) == (*recoJetCloneTagF)) && ((*reco_leadingJetB) == (*recoJetCloneTagB))) reco_purezzaBoth++ ;
				recoCloneSommaTag = *(recoJetCloneTagB)+*(recoJetCloneTagF);
				histo_reco_invMassCloneJetVVF -> Fill (recoCloneSommaTag.M()) ;
				histo_reco_deltaEtaCloneJetVVF -> Fill (fabs (recoJetCloneTagB->Eta () - recoJetCloneTagF->Eta ())) ;
                histo_reco_deltaPhiCloneJetVVF -> Fill (fabs (recoJetCloneTagB->Phi () - recoJetCloneTagF->Phi ())) ;
                histo_reco_deltaRCloneJetVVF -> Fill (recoJetCloneTagB->DeltaR (*recoJetCloneTagF)) ;
				}



			}
            /////////////////////////////////////////// ggF ////////////////////////////////////////////////////////////////////////

			else if (evtFlag == 102) //gg fusion
			{
                // leading jet tra i clones
				if (leadingJetF->E() > 0)
				{ 
				histo_energyLeadingJetggF -> Fill (leadingJetF->Energy ()) ;
				histo_ptLeadingJetggF -> Fill (leadingJetF->Pt ()) ;
				histo_etaLeadingJetggF -> Fill (leadingJetF->Eta ()) ;
                }
				if (leadingJetB->E() > 0)
				{
				histo_energyLeadingJetggF -> Fill (leadingJetB->Energy ()) ;
				histo_ptLeadingJetggF -> Fill (leadingJetB->Pt ()) ;
				histo_etaLeadingJetggF -> Fill (leadingJetB->Eta ()) ;
				}
				if (leadingJetF->E() > 0 && leadingJetB->E())
				{
				histo_invMassLeadingJetggF -> Fill (recoSommaTag.M()) ;
				histo_deltaEtaLeadingJetggF -> Fill ((fabs (leadingJetF->Eta () - leadingJetB->Eta ()) )) ;
				histo_deltaPhiLeadingJetggF -> Fill ((fabs (leadingJetF->Phi () - leadingJetB->Phi ()) )) ;
				histo_deltaRLeadingJetggF -> Fill (leadingJetF->DeltaR (*leadingJetB)) ;
                }

                //lreading jet tra i reco
				if (reco_leadingJetF->E() > 0)
				{ 
				histo_reco_energyLeadingJetggF -> Fill (reco_leadingJetF->Energy ()) ;
				histo_reco_ptLeadingJetggF -> Fill (reco_leadingJetF->Pt ()) ;
				histo_reco_etaLeadingJetggF -> Fill (reco_leadingJetF->Eta ()) ;
                }
				if (reco_leadingJetB->E() > 0)
				{
				histo_reco_energyLeadingJetggF -> Fill (reco_leadingJetB->Energy ()) ;
				histo_reco_ptLeadingJetggF -> Fill (reco_leadingJetB->Pt ()) ;
				histo_reco_etaLeadingJetggF -> Fill (reco_leadingJetB->Eta ()) ;
				}
				if (reco_leadingJetF->E() > 0 && reco_leadingJetB->E())
				{
				histo_reco_invMassLeadingJetggF -> Fill (recoSommaTag.M()) ;
				histo_reco_deltaEtaLeadingJetggF -> Fill ((fabs (reco_leadingJetF->Eta () - reco_leadingJetB->Eta ()) )) ;
				histo_reco_deltaPhiLeadingJetggF -> Fill ((fabs (reco_leadingJetF->Phi () - reco_leadingJetB->Phi ()) )) ;
				histo_reco_deltaRLeadingJetggF -> Fill (reco_leadingJetF->DeltaR (*reco_leadingJetB)) ;
                }
			}
        }
 
    cout << "eventi VVF = " << nvff << endl;
	cout << "nClonesF = " << nClonesF << endl;
	cout << "nClonesB = " << nClonesB << endl;
	cout << "nClonesBoth = " << nClonesBoth << endl;
	
 	cout << "azzeccatiF = " << purezzaF << endl; // purezze sui genJet
	cout << "azzeccatiB = " << purezzaB << endl;
	cout << "azzeccatiBoth = " << purezzaBoth << endl;
	
	cout << "reco_nClonesF = " << reco_nClonesF << endl;
	cout << "reco_nClonesB = " << reco_nClonesB << endl;
	cout << "reco_nClonesBoth = " << reco_nClonesBoth << endl;

	
	cout << "reco_azzeccatiF = " << reco_purezzaF << endl; // purezze sui recoJet
	cout << "reco_azzeccatiB = " << reco_purezzaB << endl;
	cout << "reco_azzeccatiBoth = " << reco_purezzaBoth << endl;
 
    
    //normalization 
	//genJet
	histo_energyLeadingJetggF->Scale (1/(histo_energyLeadingJetggF->Integral()));
	histo_ptLeadingJetggF->Scale (1/(histo_ptLeadingJetggF->Integral()));
	histo_etaLeadingJetggF->Scale (1/(histo_etaLeadingJetggF->Integral()));
	histo_energyLeadingJetVVF->Scale (1/(histo_energyLeadingJetVVF->Integral()));
	histo_ptLeadingJetVVF->Scale (1/(histo_ptLeadingJetVVF->Integral()));
	histo_etaLeadingJetVVF->Scale (1/(histo_etaLeadingJetVVF->Integral()));
	histo_deltaEtaLeadingJetggF->Scale (1/(histo_deltaEtaLeadingJetggF->Integral()));
	histo_deltaEtaLeadingJetVVF->Scale (1/(histo_deltaEtaLeadingJetVVF->Integral()));
	histo_deltaPhiLeadingJetggF->Scale (1/(histo_deltaPhiLeadingJetggF->Integral()));
	histo_deltaPhiLeadingJetVVF->Scale (1/(histo_deltaPhiLeadingJetVVF->Integral()));
	histo_deltaRLeadingJetggF->Scale (1/(histo_deltaRLeadingJetggF->Integral()));
	histo_deltaRLeadingJetVVF->Scale (1/(histo_deltaRLeadingJetVVF->Integral()));
	histo_invMassLeadingJetggF->Scale (1/(histo_invMassLeadingJetggF->Integral()));
	histo_invMassLeadingJetVVF->Scale (1/(histo_invMassLeadingJetVVF->Integral()));
    //recoJet
	histo_reco_energyLeadingJetggF->Scale (1/(histo_reco_energyLeadingJetggF->Integral()));
	histo_reco_ptLeadingJetggF->Scale (1/(histo_reco_ptLeadingJetggF->Integral()));
	histo_reco_etaLeadingJetggF->Scale (1/(histo_reco_etaLeadingJetggF->Integral()));
	histo_reco_energyLeadingJetVVF->Scale (1/(histo_reco_energyLeadingJetVVF->Integral()));
	histo_reco_ptLeadingJetVVF->Scale (1/(histo_reco_ptLeadingJetVVF->Integral()));
	histo_reco_etaLeadingJetVVF->Scale (1/(histo_reco_etaLeadingJetVVF->Integral()));
	histo_reco_deltaEtaLeadingJetggF->Scale (1/(histo_reco_deltaEtaLeadingJetggF->Integral()));
	histo_reco_deltaEtaLeadingJetVVF->Scale (1/(histo_reco_deltaEtaLeadingJetVVF->Integral()));
	histo_reco_deltaPhiLeadingJetggF->Scale (1/(histo_reco_deltaPhiLeadingJetggF->Integral()));
	histo_reco_deltaPhiLeadingJetVVF->Scale (1/(histo_reco_deltaPhiLeadingJetVVF->Integral()));
	histo_reco_deltaRLeadingJetggF->Scale (1/(histo_reco_deltaRLeadingJetggF->Integral()));
	histo_reco_deltaRLeadingJetVVF->Scale (1/(histo_reco_deltaRLeadingJetVVF->Integral()));
	histo_reco_invMassLeadingJetggF->Scale (1/(histo_reco_invMassLeadingJetggF->Integral()));
	histo_reco_invMassLeadingJetVVF->Scale (1/(histo_reco_invMassLeadingJetVVF->Integral()));
	//genJet cloni
	histo_invMassCloneJetVVF -> Scale (1/(histo_invMassCloneJetVVF->Integral()));
	histo_deltaEtaCloneJetVVF -> Scale (1/(histo_deltaEtaCloneJetVVF->Integral()));
	histo_deltaPhiCloneJetVVF -> Scale (1/(histo_deltaPhiCloneJetVVF->Integral()));
	histo_deltaRCloneJetVVF -> Scale (1/(histo_deltaRCloneJetVVF->Integral()));
	histo_energyCloneJetVVF -> Scale (1/(histo_energyCloneJetVVF->Integral()));
	histo_ptCloneJetVVF -> Scale (1/(histo_ptCloneJetVVF->Integral()));
	histo_etaCloneJetVVF -> Scale (1/(histo_etaCloneJetVVF->Integral()));
	//recoJet cloni
	histo_reco_invMassCloneJetVVF -> Scale (1/(histo_reco_invMassCloneJetVVF->Integral()));
	histo_reco_deltaEtaCloneJetVVF -> Scale (1/(histo_reco_deltaEtaCloneJetVVF->Integral()));
	histo_reco_deltaPhiCloneJetVVF -> Scale (1/(histo_reco_deltaPhiCloneJetVVF->Integral()));
	histo_reco_deltaRCloneJetVVF -> Scale (1/(histo_reco_deltaRCloneJetVVF->Integral()));
	histo_reco_energyCloneJetVVF -> Scale (1/(histo_reco_energyCloneJetVVF->Integral()));
	histo_reco_ptCloneJetVVF -> Scale (1/(histo_reco_ptCloneJetVVF->Integral()));
	histo_reco_etaCloneJetVVF -> Scale (1/(histo_reco_etaCloneJetVVF->Integral()));
	//quark
	histo_invMassqVVF -> Scale (1/(histo_invMassqVVF->Integral()));
	histo_deltaEtaqVVF -> Scale (1/(histo_deltaEtaqVVF->Integral()));
	histo_deltaPhiqVVF -> Scale (1/(histo_deltaPhiqVVF->Integral()));
	histo_deltaRqVVF -> Scale (1/(histo_deltaRqVVF->Integral()));
	histo_energyqVVF -> Scale (1/(histo_energyqVVF->Integral()));
	histo_ptqVVF -> Scale (1/(histo_ptqVVF->Integral()));
	histo_etaqVVF -> Scale (1/(histo_etaqVVF->Integral()));

			
	TLegend *tleg = new TLegend (0.7, 0.6, 0.9, 0.8);
    tleg -> SetBorderSize (0);
    tleg -> SetFillColor (0);
	tleg-> AddEntry (histo_etaqVVF, "VVf quarks","l") ;
	tleg-> AddEntry (histo_etaCloneJetVVF, "VVf genJet matching quarks","l") ;
	tleg-> AddEntry (histo_reco_etaCloneJetVVF, "VVf recoJet matching quarks","l") ;
	tleg-> AddEntry (histo_energyLeadingJetVVF, "VVf selected genJet","l") ;
    tleg-> AddEntry (histo_energyLeadingJetggF, "ggf selected genJet","l") ;
	tleg-> AddEntry (histo_reco_energyLeadingJetVVF, "VVf selected recoJet","l") ;
    tleg-> AddEntry (histo_reco_energyLeadingJetggF, "ggf selected recoJet","l") ;

 
    TCanvas *p=new TCanvas () ;
	p->Divide (3,3) ;
	p->cd (1);
	histo_energyLeadingJetVVF->Draw ("C") ;
	histo_energyLeadingJetggF->Draw ("Csame") ;
	histo_reco_energyLeadingJetVVF->Draw ("Csame") ;
	histo_reco_energyLeadingJetggF->Draw ("Csame") ;
	histo_energyCloneJetVVF->Draw ("Csame") ;
	histo_reco_energyCloneJetVVF->Draw ("Csame") ;
	histo_energyqVVF->Draw ("Csame") ;
	tleg->Draw ("Csame") ; 
    p->cd (2);
	histo_etaLeadingJetggF->Draw ("C") ;
	histo_etaLeadingJetVVF->Draw ("Csame") ;
    histo_reco_etaLeadingJetggF->Draw ("Csame") ;
	histo_reco_etaLeadingJetVVF->Draw ("Csame") ;
	histo_etaCloneJetVVF->Draw ("Csame") ; 
	histo_reco_etaCloneJetVVF->Draw ("Csame") ; 
	histo_etaqVVF->Draw ("Csame") ; 
	p->cd (3);
	histo_deltaEtaLeadingJetggF->Draw ("C") ;
	histo_deltaEtaLeadingJetVVF->Draw ("Csame") ;
	histo_reco_deltaEtaLeadingJetggF->Draw ("Csame") ;
	histo_reco_deltaEtaLeadingJetVVF->Draw ("Csame") ;
	histo_deltaEtaCloneJetVVF->Draw ("Csame") ;
	histo_reco_deltaEtaCloneJetVVF->Draw ("Csame") ;
	histo_deltaEtaqVVF->Draw ("Csame") ;
	p->cd (4);
	histo_ptLeadingJetggF->Draw ("C") ;
	histo_ptLeadingJetVVF->Draw ("Csame") ;
	histo_reco_ptLeadingJetggF->Draw ("Csame") ;
	histo_reco_ptLeadingJetVVF->Draw ("Csame") ;
	histo_ptCloneJetVVF->Draw ("Csame") ;
	histo_reco_ptCloneJetVVF->Draw ("Csame") ;
	histo_ptqVVF->Draw ("Csame") ; 
	p->cd (5);
	histo_invMassLeadingJetggF->Draw ("C") ;
	histo_invMassLeadingJetVVF->Draw ("Csame") ;
	histo_reco_invMassLeadingJetggF->Draw ("Csame") ;
	histo_reco_invMassLeadingJetVVF->Draw ("Csame") ;
	histo_invMassCloneJetVVF->Draw ("Csame") ;
	histo_reco_invMassCloneJetVVF->Draw ("Csame") ;
	histo_invMassqVVF->Draw ("Csame") ;
	p->cd (6);
	histo_deltaPhiLeadingJetggF->Draw ("C") ;
	histo_deltaPhiLeadingJetVVF->Draw ("Csame") ;
	histo_reco_deltaPhiLeadingJetggF->Draw ("Csame") ;
	histo_reco_deltaPhiLeadingJetVVF->Draw ("Csame") ;
	histo_deltaPhiCloneJetVVF->Draw ("Csame") ;
	histo_reco_deltaPhiCloneJetVVF->Draw ("Csame") ;
	histo_deltaPhiqVVF->Draw ("Csame") ;
	p->cd (7);
	histo_deltaRLeadingJetggF->Draw ("C") ;
	histo_deltaRLeadingJetVVF->Draw ("Csame") ;
	histo_reco_deltaRLeadingJetggF->Draw ("Csame") ;
	histo_reco_deltaRLeadingJetVVF->Draw ("Csame") ;
	histo_deltaRCloneJetVVF->Draw ("Csame") ;
	histo_reco_deltaRCloneJetVVF->Draw ("Csame") ;
	histo_deltaRqVVF->Draw ("Csame") ;
	/*
	TCanvas *p2=new TCanvas () ;
	p2->Divide (1,2) ;
	p2->cd (1);
    histo_energyResoCloneSelected -> Draw () ;
	p2->cd (2);
    histo_deltaRCloneSelected -> Draw () ;
	*/
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void twoLargestInvMassJets (TClonesArray &recoJet4Momentum , TLorentzVector &leadingJetF, 
                           TLorentzVector &leadingJetB, int & numberJet, int &numberJetAfterCuts = 0)
{

    leadingJetF.SetPxPyPzE (0.0, 0.0, 0.0, 0.0) ;  
	leadingJetB.SetPxPyPzE (0.0, 0.0, 0.0, 0.0) ;  
	
	int A = 0 ;
	int B = 0 ;
	double maxInvMass = 0.0;
	double invMass = 0.0;
	TLorentzVector sum;
	TLorentzVector leadingJet1;
	TLorentzVector leadingJet2;
	int dopoA = 0;
	int dopoB = 0;

	/////////// loop per i leading jet 
	for (int a=0; a <numberJet; a++)
	{
	if (fabs(((TLorentzVector*)(recoJet4Momentum.At(a)))->Eta()) > 5) continue ;
	if (fabs(((TLorentzVector*)(recoJet4Momentum.At(a)))->Pt()) < 15) continue ;
	dopoA++;

	for (int b=0; b <numberJet; b++)
	{
	if (*((TLorentzVector*)(recoJet4Momentum.At(a)))==*((TLorentzVector*)(recoJet4Momentum.At(b)))) continue;
	
	if (fabs(((TLorentzVector*)(recoJet4Momentum.At(b)))->Eta()) > 5) continue ;
	if (fabs(((TLorentzVector*)(recoJet4Momentum.At(b)))->Pt()) < 15) continue ;
	
	dopoB++;
	sum = *((TLorentzVector*)(recoJet4Momentum.At(a)))+*((TLorentzVector*)(recoJet4Momentum.At(b)));
	invMass = sum.M() ;
	if (invMass > maxInvMass)
		{ 
		maxInvMass = invMass ;
		A = a;
		B = b; 
		} 
	}
	}
	numberJetAfterCuts = dopoA;
	
	if (dopoA > 0)
	{
	leadingJet1.SetPx (((TLorentzVector*)(recoJet4Momentum.At(A)))->Px()) ;  
	leadingJet1.SetPy (((TLorentzVector*)(recoJet4Momentum.At(A)))->Py()) ;
	leadingJet1.SetPz (((TLorentzVector*)(recoJet4Momentum.At(A)))->Pz()) ;
	leadingJet1.SetE (((TLorentzVector*)(recoJet4Momentum.At(A)))->Energy()) ;
	}
	if (dopoB > 0)
	{
	leadingJet2.SetPx (((TLorentzVector*)(recoJet4Momentum.At(B)))->Px()) ;  
	leadingJet2.SetPy (((TLorentzVector*)(recoJet4Momentum.At(B)))->Py()) ;
	leadingJet2.SetPz (((TLorentzVector*)(recoJet4Momentum.At(B)))->Pz()) ;
	leadingJet2.SetE (((TLorentzVector*)(recoJet4Momentum.At(B)))->Energy()) ;
	}
		
	if (leadingJet1.Eta () > leadingJet2.Eta ())
	{ 
	leadingJetF = leadingJet1;
	leadingJetB = leadingJet2;
	}
	else
	{ 
	leadingJetB = leadingJet1;
	leadingJetF = leadingJet2;
	}
	
}	




///////////////////////////////////////////////////////////////////////////////////////////////

void debug () // per treovare i genJetCloneTag tra i genJet4Momentum
{
    TFile *f = new TFile("WWF_HIGGS160/WWf_higgsMass160_tree.root") ;
    TTree *tree = (TTree*)gDirectory->Get("tree");  
        
	TClonesArray *genJet4Momentum = new TClonesArray ("TLorentzVector");
	TLorentzVector *genJetCloneTagB = new TLorentzVector();
	TLorentzVector *genJetCloneTagF = new TLorentzVector();
	TLorentzVector *genqTagF = new TLorentzVector();
    TLorentzVector *genqTagB = new TLorentzVector();
	int evtFlag ;
	
    tree->SetBranchAddress ("evtFlag", &evtFlag);
	tree->SetBranchAddress ("genJet4Momentum", &genJet4Momentum);
	tree->SetBranchAddress ("genJetCloneTagB", &genJetCloneTagB);
	tree->SetBranchAddress ("genJetCloneTagF", &genJetCloneTagF); 
    tree->SetBranchAddress ("genqTagB", &genqTagB);
	tree->SetBranchAddress ("genqTagF", &genqTagF); 

	int nentries = (int) tree->GetEntries();

	for(int i=0; i<20; i++)
        {
            tree->GetEntry(i);
			cout << "entry " << i << endl ;					
			if (evtFlag == 123 || evtFlag == 124) //VV fusion
			{
	         cout << i << ": energia del clone F " << genJetCloneTagF->Energy () << endl;
			 cout << i << ": energia del clone B " << genJetCloneTagB->Energy () << endl;
			 
			 cout << i << ": energia del q F " << genqTagF->Energy () << endl;
			 cout << i << ": energia del q B " << genqTagB->Energy () << endl;
			 
			 cout << "deltaR F " << genJetCloneTagF->DeltaR (*genqTagF) << endl ;
			 cout << "deltaR B " << genJetCloneTagF->DeltaR (*genqTagB) << endl ;
			 
			 //for (int b=0; b <genJet4Momentum->GetEntries(); b++)
	         //cout << i << ": genJet "<< (((TLorentzVector*)(genJet4Momentum->At(b)))->Energy()) << endl;
				
			}
		}		

}

void elePlots ()
{
    gROOT->SetStyle("Plain") ;
    TFile *f = new TFile("/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_0/src/HiggsAnalysis/VBFHiggsToWW2e/prova.root") ;
    
    TTree *tree = (TTree*)gDirectory->Get("genTree");  
    
    //GSF //////////////////////////////////////////////////////////
    
    //reco electrons
    int numberGSF;
    TClonesArray *recoEle4Momentum = new TClonesArray ("TLorentzVector");
    TClonesArray *recoEleTrkMomentumAtVtx = new TClonesArray ("TVector3");
    TClonesArray *recoEleTrkPositionAtVtx = new TClonesArray ("TVector3");
    std::vector<double> *recoEleEcalEnergy = new std::vector<double>(); 
    std::vector<double> *recoEleTrkIsoVal = new std::vector<double>();
    std::vector<double> *recoEleCalIsoVal = new std::vector<double>();
    std::vector<int> *recoEleClass = new std::vector<int>();
    std::vector<int> *recoEleCutBasedID = new std::vector<int>();
    
    tree->SetBranchAddress("numberGSF", &numberGSF);
    tree->SetBranchAddress("recoEle4Momentum",  &recoEle4Momentum); 
    tree->SetBranchAddress("recoEleTrkMomentumAtVtx", &recoEleTrkMomentumAtVtx); 
    tree->SetBranchAddress("recoEleTrkPositionAtVtx", &recoEleTrkPositionAtVtx);
    tree->SetBranchAddress("recoEleEcalEnergy",  &recoEleEcalEnergy); //vettore
    tree->SetBranchAddress("recoEleTrkIsoVal",  &recoEleTrkIsoVal); //vettore
    tree->SetBranchAddress("recoEleCalIsoVal",  &recoEleCalIsoVal); //vettore
    //tree->SetBranchAddress("recoEleClass",  &recoEleClass);//vettore
    //tree->SetBranchAddress("recoEleCutBasedID",  &recoEleCutBasedID);//vettore

    TH1F *histo_recLep_e = new TH1F("histo_recLep_e", "GSF electrons energy",100,0,500) ;
    histo_recLep_e->GetXaxis ()->SetTitle ("Energy (GeV)") ;
    histo_recLep_e->SetFillColor (94);
    
    TH1F *histo_trkIso = new TH1F("histo_trkIso", "GSF electrons trk isolation value",100,0,500) ;
    histo_trkIso->GetXaxis ()->SetTitle ("Trk Isolation") ;
    histo_trkIso->SetFillColor (94);
    
    TH1F *histo_calIso = new TH1F("histo_trkIso", "GSF electrons cal isolation value",100,0,50) ;
    histo_calIso->GetXaxis ()->SetTitle ("Cal Isolation") ;
    histo_calIso->SetFillColor (94);
    

    int nentries = (int) tree->GetEntries();
    for(int i=0; i<nentries; i++)
        {
            tree->GetEntry(i);
            //cout << "numberGSF="<< numberGSF << endl ;
            for(int j=0; j<numberGSF ; j++)
            {
                histo_recLep_e -> Fill (((TLorentzVector*)(recoEle4Momentum->At(j)))->Energy()) ;
		histo_trkIso -> Fill (recoEleTrkIsoVal->at(j)) ;
		histo_calIso -> Fill (recoEleCalIsoVal->at(j)) ;
               //cout<< "Px "<< (((TLorentzVector*)(recoEle4Momentum->At(j)))->Px()) << endl ;
               //cout<< (((TVector3*)(recoEleTrkMomentumAtVtx->At(j)))->Px()) << endl ;
               //cout << "**recoEleEcalEnergy " << j << " " << (recoEleEcalEnergy->at(j)) << endl ;
	       //histo_recLep_e->Fill ((recoEleEcalEnergy->at(j))) ;
            }   
        }
 
    TCanvas *p=new TCanvas () ;
    p->Divide (2,2);
    p->cd (1);
    histo_recLep_e->Draw () ;
    p->cd (2);
    histo_trkIso->Draw () ;
    p->cd (3);
    histo_calIso->Draw () ;  
}


void MCtruthPlots ()
{
    gROOT->SetStyle("Plain") ;
    
    TFile f ("prova.root") ;

    TTree *tree = (TTree*)gDirectory->Get("genTree");  
    
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
        if (LepPlusFlavour == 11 && LepMinusFlavour == 11) histo_genLep_deta_ee->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 11 && LepMinusFlavour == 11) histo_genLep_dphi_ee->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 13) histo_genLep_deta_mumu->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 13) histo_genLep_dphi_mumu->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;
        if (LepPlusFlavour == 11 && LepMinusFlavour == 13) histo_genLep_deta_emu->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 11 && LepMinusFlavour == 13) histo_genLep_dphi_emu->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 11) histo_genLep_deta_emu->Fill (fabs(genLepMinus->Eta()-genLepPlus->Eta())) ;
        if (LepPlusFlavour == 13 && LepMinusFlavour == 11) histo_genLep_dphi_emu->Fill (fabs(genLepMinus->Phi()-genLepPlus->Phi())) ;

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
        histo_genTag_eta->Fill (genqTagF->Eta()) ;
        histo_genTag_eta->Fill (genqTagB->Eta()) ;
        histo_genTag_e->Fill (genqTagF->E()) ;
        histo_genTag_e->Fill (genqTagB->E()) ;
        histo_genTag_pt->Fill (genqTagF->Pt()) ;
        histo_genTag_pt->Fill (genqTagB->Pt()) ;
        histo_genTag_deta->Fill (fabs(genqTagF->Eta()-genqTagB->Eta())) ;
        
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
    /*
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
    
    cout << "#ee " << counter_ee << endl ;
    cout << "#mumu " << counter_mumu << endl ;
    cout << "#emu " << counter_emu << endl ;  
    
}

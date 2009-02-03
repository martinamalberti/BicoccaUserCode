void plot () 
{
gROOT->SetStyle ("Plain");
gStyle->SetOptStat (0000000) ;

TH2F *histo2_deltaEta = new TH2F("histo2_deltaEta", "#Delta#eta between tag jets", 100, 0, 10, 100, 0, 0.05) ;
TH2F *histo2_deltaPhi = new TH2F("histo2_deltaPhiTags","#Delta#phi between tag jets",100,0,3.5, 100, 0, 0.08) ;
TH2F *histo2_sumEta = new TH2F("histo2_sumEtaTags","#Sigma#eta of tag jets",50,-10,10, 100, 0, 0.09) ;
TH2F *histo2_invMass = new TH2F("histo2_invMassTags","invariant mass of tag jets",100, 0, 2000, 100, 0, 0.1) ;
TH2F *histo2_eta = new TH2F("histo2_etaTags","#eta of tag jets",50,-6,6, 100, 0, 0.04) ;
TH2F *histo2_energy = new TH2F("histo2_energyTags","energy of tag jets",100, 0, 1000, 100, 0, 0.08) ;
TH2F *histo2_pt = new TH2F("histo2_ptTags","pt of tag jets",100, 0, 250, 100, 0, 0.16) ;
TH2F *histo2_firstEnergyTagEnergy = new TH2F("m_firstEnergyTagEnergy","energy of leading tag jets",100, 0, 1200,100, 0, 0.03) ;
TH2F *histo2_secondEnergyTagEnergy = new TH2F("m_secondEnergyTagEnergy","energy of softer tag jets",100, 0, 1200,100, 0, 0.13) ;

TH2F *histo2_numOthers = new TH2F("histo2_numOthers","# of other jets",15,0,15,100, 0, 0.45) ;
TH2F *histo2_etaOthers = new TH2F("histo2_etaOthers","#eta of other jets",50,-6,6, 100, 0, 0.08) ;
TH2F *histo2_energyOthers = new TH2F("histo2_energyOthers","energy of other jets",100, 0, 400, 100, 0, 0.07) ;
TH2F *histo2_ptOthers = new TH2F("histo2_ptOthers","pt of other jets",100, 0, 200, 100, 0, 0.15) ;
TH2F *histo2_deltaROthers = new TH2F("histo2_deltaROthers","#DeltaR between tag jets and other jets",50,0,9, 100, 0, 0.08) ;
TH2F *histo2_deltaEtaOthers = new TH2F("histo2_deltaEtaOthers","#Delta#eta between tag jets and other jets",50,0,9, 100, 0, 0.12) ;

TH2F *histo2_etaOthersSummed = new TH2F("histo2_etaOthersSummed","#eta of summed jets",50,-6,6, 100, 0, 0.05) ;
TH2F *histo2_energyOthersSummed = new TH2F("histo2_energyOthersSummed","energy of summed jets",100, 0, 1500, 100, 0, 0.15) ;
TH2F *histo2_ptOthersSummed = new TH2F("histo2_ptOthersSummed","pt of summed jets",100, 0, 200, 100, 0, 0.25) ;
TH2F *histo2_invMassOtherSummed = new TH2F("histo2_invMassOthersSummed","invariant mass of summed jets",100, 0, 800, 100, 0, 0.3) ;

TCanvas *c1 = new TCanvas () ;
c1 -> Divide (3, 3) ;
c1 -> cd (1);
histo2_deltaEta->Draw ();
c1 -> cd (2);
histo2_sumEta -> Draw  ();
c1 -> cd (3);
histo2_invMass -> Draw  ();
c1 -> cd (4);
histo2_eta -> Draw  ();
c1 -> cd (5);
histo2_energy -> Draw  ();
c1 -> cd (6);
histo2_pt -> Draw  ();
c1 -> cd (7);
histo2_deltaPhi -> Draw  ();
c1 -> cd (8);
m_firstEnergyTagEnergy -> Draw  ();
c1 -> cd (9);
m_secondEnergyTagEnergy -> Draw  ();

TCanvas *c2 = new TCanvas () ;
c2 -> Divide (3, 2) ;
c2 -> cd (1);
histo2_etaOthers -> Draw  ();
c2 -> cd (2);
histo2_energyOthers -> Draw  ();
c2 -> cd (3);
histo2_ptOthers -> Draw  ();
c2 -> cd (4);
histo2_deltaROthers -> Draw  ();
c2 -> cd (5);
histo2_deltaEtaOthers -> Draw  ();
c2 -> cd (6);
histo2_numOthers -> Draw  ();

TCanvas *c3 = new TCanvas () ;
c3 -> Divide (3, 2) ;
c3 -> cd (1);
histo2_etaOthersSummed -> Draw  ();
c3 -> cd (2);
histo2_energyOthersSummed -> Draw  ();
c3 -> cd (3);
histo2_ptOthersSummed -> Draw  ();
c3 -> cd (4);
histo2_invMassOthersSummed -> Draw  ();

TCanvas *g1 = new TCanvas () ;
histo2_deltaEta->Draw ();
TCanvas *g2 = new TCanvas () ;
histo2_sumEta -> Draw  ();
TCanvas *g3 = new TCanvas () ;
histo2_invMass -> Draw  ();
TCanvas *g4 = new TCanvas () ;
histo2_eta -> Draw  ();
TCanvas *g5 = new TCanvas () ;
histo2_energy -> Draw  ();
TCanvas *g6 = new TCanvas () ;
histo2_pt -> Draw  ();
TCanvas *g7 = new TCanvas () ;
histo2_deltaPhi -> Draw  ();
TCanvas *g8 = new TCanvas () ;
histo2_etaOthers -> Draw  ();
TCanvas *g9 = new TCanvas () ;
histo2_energyOthers -> Draw  ();
TCanvas *g10 = new TCanvas () ;
histo2_ptOthers -> Draw  ();
TCanvas *g11 = new TCanvas () ;
histo2_deltaROthers -> Draw  ();
TCanvas *g12 = new TCanvas () ;
histo2_deltaEtaOthers -> Draw  ();
TCanvas *g13 = new TCanvas () ;
histo2_numOthers -> Draw  ();
TCanvas *g14 = new TCanvas () ;
histo2_etaOthersSummed -> Draw  ();
TCanvas *g15 = new TCanvas () ;
histo2_energyOthersSummed -> Draw  ();
TCanvas *g16 = new TCanvas () ;
histo2_ptOthersSummed -> Draw  ();
TCanvas *g17 = new TCanvas () ;
histo2_invMassOthersSummed -> Draw  ();

TLegend *tleg = new TLegend (0.5, 0.3, 0.7, 0.8);
tleg -> SetBorderSize (0);
tleg -> SetFillColor (0);

otherFiles ("CMSSW_histos/H120_WWF_histos.root", "H120 WWf", 30, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
otherFiles ("CMSSW_histos/H160_WWF_histos.root", "H160 WWf", 38, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
otherFiles ("CMSSW_histos/H180_WWF_histos.root", "H180 WWf", 50, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);

//otherFiles ("CMSSW_histos/ggF_histos.root", "ggf", 2, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Wt_histos.root", "Wt", 3, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/WW_histos.root", "WW", 50, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/WZ_histos.root", "WZ", 5, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/WZ_incl_histos.root", "WZ_incl", 6, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/ZZ_incl_histos.root", "ZZ_incl", 7, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/ttnjWeighted_histos.root", "ttj", 51, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/tt0j_histos.root", "tt0j", 53, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/tt1j_histos.root", "tt1j", 53, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/tt2j_histos.root", "tt2j", 55, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/tt3j_histos.root", "tt3j", 57, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/tt4j_histos.root", "tt4j", 59, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/ZeejWeighted_histos.root", "Zeej", 70, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_0_15_histos.root", "Zeej_0_15", 70, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_15_20_histos.root", "Zeej_15_20", 73, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_20_30_histos.root", "Zeej_20_30", 76, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_30_50_histos.root", "Zeej_30_50", 79, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_50_80_histos.root", "Zeej_50_80", 82, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_80_120_histos.root", "Zeej_80_120", 85, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_120_170_histos.root", "Zeej_120_170", 87, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);
//otherFiles ("CMSSW_histos/Zeej_170_230_histos.root", "Zeej_170_230", 89, c1, c2, c3, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15, g16, g17, tleg);


c1 -> cd (5);
tleg -> Draw ("same") ;
c2 -> cd (2);
tleg -> Draw ("same") ;
c3 -> cd (4);
tleg -> Draw ("same") ;
g7 -> cd ();
tleg -> Draw ("same") ;
g6 -> cd ();
tleg -> Draw ("same") ;
g9 -> cd ();
tleg -> Draw ("same") ;
g10 -> cd ();
tleg -> Draw ("same") ;
g11 -> cd ();
tleg -> Draw ("same") ;
g17 -> cd ();
tleg -> Draw ("same") ;

}

void otherFiles (TString nomeFile, TString tag, int color, TCanvas *c1, TCanvas *c2, TCanvas *c3, 
TCanvas *g1, TCanvas *g2, TCanvas *g3, TCanvas *g4, TCanvas *g5, TCanvas *g6, TCanvas *g7, TCanvas *g8, TCanvas *g9, TCanvas *g10,
TCanvas *g11, TCanvas *g12, TCanvas *g13, TCanvas *g14, TCanvas *g15, TCanvas *g16, TCanvas *g17, TLegend *tleg)
{

std::cout << "reading " << nomeFile << std::endl;
TFile *f = new TFile (nomeFile) ;
TH1F *histo_deltaEta = (TH1F*)f->Get ("trivialReader/m_deltaEtaTags");
TH1F *histo_deltaPhi = (TH1F*)f->Get ("trivialReader/m_deltaPhiTags");
TH1F *histo_sumEta = (TH1F*)f->Get ("trivialReader/m_sumEtaTags");
TH1F *histo_invMass = (TH1F*)f->Get ("trivialReader/m_invMassTags");
TH1F *histo_eta = (TH1F*)f->Get ("trivialReader/m_etaTags");
TH1F *histo_energy = (TH1F*)f->Get ("trivialReader/m_energyTags");
TH1F *histo_pt = (TH1F*)f->Get ("trivialReader/m_ptTags");
TH1F *histo_firstEnergyTagEnergy = (TH1F*)f->Get ("trivialReader/m_firstEnergyTagEnergy");
TH1F *histo_secondEnergyTagEnergy = (TH1F*)f->Get ("trivialReader/m_secondEnergyTagEnergy");

TH1F *histo_numOthers = (TH1F*)f->Get ("trivialReader/m_numOthers");
TH1F *histo_etaOthers = (TH1F*)f->Get ("trivialReader/m_etaOthers");
TH1F *histo_energyOthers = (TH1F*)f->Get ("trivialReader/m_energyOthers");
TH1F *histo_ptOthers = (TH1F*)f->Get ("trivialReader/m_ptOthers");
TH1F *histo_deltaROthers = (TH1F*)f->Get ("trivialReader/m_deltaROthers");
TH1F *histo_deltaEtaOthers = (TH1F*)f->Get ("trivialReader/m_deltaEtaOthers");
TH1F *histo_etaOthersSummed = (TH1F*)f->Get ("trivialReader/m_etaOthersSummed");
TH1F *histo_energyOthersSummed = (TH1F*)f->Get ("trivialReader/m_energyOthersSummed");
TH1F *histo_ptOthersSummed = (TH1F*)f->Get ("trivialReader/m_ptOthersSummed");
TH1F *histo_invMassOthersSummed = (TH1F*)f->Get ("trivialReader/m_invMassOthersSummed");

histo_deltaEta -> SetLineColor (color);
histo_deltaPhi -> SetLineColor (color);
histo_sumEta -> SetLineColor (color);
histo_invMass -> SetLineColor (color);
histo_eta -> SetLineColor (color);
histo_energy -> SetLineColor (color);
histo_pt -> SetLineColor (color);
histo_firstEnergyTagEnergy -> SetLineColor (color);
histo_secondEnergyTagEnergy -> SetLineColor (color);
histo_numOthers -> SetLineColor (color);
histo_etaOthers -> SetLineColor (color);
histo_energyOthers -> SetLineColor (color);
histo_ptOthers -> SetLineColor (color);
histo_deltaROthers -> SetLineColor (color);
histo_deltaEtaOthers -> SetLineColor (color);
histo_etaOthersSummed -> SetLineColor (color);
histo_energyOthersSummed -> SetLineColor (color);
histo_ptOthersSummed -> SetLineColor (color);
histo_invMassOthersSummed -> SetLineColor (color);

histo_deltaEta -> SetLineWidth (2);
histo_deltaPhi -> SetLineWidth (2);
histo_sumEta -> SetLineWidth (2);
histo_invMass -> SetLineWidth (2);
histo_eta -> SetLineWidth (2);
histo_energy -> SetLineWidth (2);
histo_pt -> SetLineWidth (2);
histo_firstEnergyTagEnergy -> SetLineWidth (2);
histo_secondEnergyTagEnergy -> SetLineWidth (2);
histo_numOthers -> SetLineWidth (2);
histo_etaOthers -> SetLineWidth (2);
histo_energyOthers -> SetLineWidth (2);
histo_ptOthers -> SetLineWidth (2);
histo_deltaROthers -> SetLineWidth (2);
histo_deltaEtaOthers -> SetLineWidth (2);
histo_etaOthersSummed -> SetLineWidth (2);
histo_energyOthersSummed -> SetLineWidth (2);
histo_ptOthersSummed -> SetLineWidth (2);
histo_invMassOthersSummed -> SetLineWidth (2);


if (color == 1) 
{
histo_deltaEta -> SetFillColor (color);
histo_deltaPhi -> SetFillColor (color);
histo_sumEta -> SetFillColor (color);
histo_invMass -> SetFillColor (color);
histo_eta -> SetFillColor (color);
histo_energy -> SetFillColor (color);
histo_pt -> SetFillColor (color);
histo_firstEnergyTagEnergy -> SetFillColor (color);
histo_secondEnergyTagEnergy -> SetFillColor (color);
histo_numOthers -> SetFillColor (color);
histo_etaOthers -> SetFillColor (color);
histo_energyOthers -> SetFillColor (color);
histo_ptOthers -> SetFillColor (color);
histo_deltaROthers -> SetFillColor (color);
histo_deltaEtaOthers -> SetFillColor (color);
histo_etaOthersSummed -> SetFillColor (color);
histo_energyOthersSummed -> SetFillColor (color);
histo_ptOthersSummed -> SetFillColor (color);
histo_invMassOthersSummed -> SetFillColor (color);

histo_deltaEta -> SetFillStyle (3002);
histo_deltaPhi -> SetFillStyle (3002);
histo_sumEta -> SetFillStyle (3002);
histo_invMass -> SetFillStyle (3002);
histo_eta -> SetFillStyle (3002);
histo_energy -> SetFillStyle (3002);
histo_pt -> SetFillStyle (3002);
histo_firstEnergyTagEnergy -> SetFillStyle (3002);
histo_secondEnergyTagEnergy -> SetFillStyle (3002);
histo_numOthers -> SetFillStyle (3002);
histo_etaOthers -> SetFillStyle (3002);
histo_energyOthers -> SetFillStyle (3002);
histo_ptOthers -> SetFillStyle (3002);
histo_deltaROthers -> SetFillStyle (3002);
histo_deltaEtaOthers -> SetFillStyle (3002);
histo_etaOthersSummed -> SetFillStyle (3002);
histo_energyOthersSummed -> SetFillStyle (3002);
histo_ptOthersSummed -> SetFillStyle (3002);
histo_invMassOthersSummed -> SetFillStyle (3002);
}


c1 -> cd (1);
histo_deltaEta -> DrawNormalized  ("same");
c1 -> cd (2);
histo_sumEta -> DrawNormalized  ("same");
c1 -> cd (3);
histo_invMass -> DrawNormalized  ("same");
c1 -> cd (4);
histo_eta -> DrawNormalized  ("same");
c1 -> cd (5);
histo_energy -> DrawNormalized  ("same");
c1 -> cd (6);
histo_pt -> DrawNormalized  ("same");
c1 -> cd (7);
histo_deltaPhi -> DrawNormalized  ("same");
c1 -> cd (8);
histo_firstEnergyTagEnergy -> DrawNormalized  ("same");
c1 -> cd (9);
histo_secondEnergyTagEnergy -> DrawNormalized  ("same");

c2 -> cd (1);
histo_etaOthers -> DrawNormalized  ("same");
c2 -> cd (2);
histo_energyOthers -> DrawNormalized  ("same");
c2 -> cd (3);
histo_ptOthers -> DrawNormalized  ("same");
c2 -> cd (4);
histo_deltaROthers -> DrawNormalized  ("same");
c2 -> cd (5);
histo_deltaEtaOthers -> DrawNormalized  ("same");
c2 -> cd (6);
histo_numOthers -> DrawNormalized  ("same");

c3 -> cd (1);
histo_etaOthersSummed -> DrawNormalized  ("same");
c3 -> cd (2);
histo_energyOthersSummed -> DrawNormalized  ("same");
c3 -> cd (3);
histo_ptOthersSummed -> DrawNormalized  ("same");
c3 -> cd (4);
histo_invMassOthersSummed -> DrawNormalized  ("same");


g1-> cd () ;
histo_deltaEta->DrawNormalized  ("same");
g2-> cd () ;
histo_sumEta -> DrawNormalized  ("same");
g3-> cd () ;
histo_invMass -> DrawNormalized  ("same");
g4-> cd () ;
histo_eta -> DrawNormalized  ("same");
g5-> cd () ;
histo_energy -> DrawNormalized  ("same");
g6-> cd () ;
histo_pt -> DrawNormalized  ("same");
g7-> cd () ;
histo_deltaPhi -> DrawNormalized  ("same");
g8-> cd () ;
histo_etaOthers -> DrawNormalized  ("same");
g9-> cd () ;
histo_energyOthers -> DrawNormalized  ("same");
g10-> cd () ;
histo_ptOthers -> DrawNormalized  ("same");
g11-> cd () ;
histo_deltaROthers -> DrawNormalized  ("same");
g12-> cd () ;
histo_deltaEtaOthers -> DrawNormalized  ("same");
g13-> cd () ;
histo_numOthers -> DrawNormalized  ("same");
g14-> cd () ;
histo_etaOthersSummed -> DrawNormalized  ("same");
g15-> cd () ;
histo_energyOthersSummed -> DrawNormalized  ("same");
g16-> cd () ;
histo_ptOthersSummed -> DrawNormalized  ("same");
g17-> cd () ;
histo_invMassOthersSummed -> DrawNormalized  ("same");



tleg-> AddEntry (histo_invMassOthersSummed, tag,"l") ;




}

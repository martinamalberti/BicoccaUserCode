void plot () 
{
gROOT->SetStyle ("Plain");

TFile *f = new TFile ("tt0j_histos.root") ;
TH1F *histo_deltaEta = (TH1F*)f->Get ("trivialReader/m_deltaEtaTags");
TH1F *histo_deltaPhi = (TH1F*)f->Get ("trivialReader/m_deltaPhiTags");
TH1F *histo_sumEta = (TH1F*)f->Get ("trivialReader/m_sumEtaTags");
TH1F *histo_invMass = (TH1F*)f->Get ("trivialReader/m_invMassTags");
TH1F *histo_eta = (TH1F*)f->Get ("trivialReader/m_etaTags");
TH1F *histo_energy = (TH1F*)f->Get ("trivialReader/m_energyTags");
TH1F *histo_pt = (TH1F*)f->Get ("trivialReader/m_ptTags");
TH1F *histo_etaOthers = (TH1F*)f->Get ("trivialReader/m_etaOthers");
TH1F *histo_energyOthers = (TH1F*)f->Get ("trivialReader/m_energyOthers");
TH1F *histo_ptOthers = (TH1F*)f->Get ("trivialReader/m_ptOthers");
TH1F *histo_deltaROthers = (TH1F*)f->Get ("trivialReader/m_deltaROthers");
TH1F *histo_deltaEtaOthers = (TH1F*)f->Get ("trivialReader/m_deltaEtaOthers");
TH1F *histo_etaOthersSummed = (TH1F*)f->Get ("trivialReader/m_etaOthersSummed");
TH1F *histo_energyOthersSummed = (TH1F*)f->Get ("trivialReader/m_energyOthersSummed");
TH1F *histo_ptOthersSummed = (TH1F*)f->Get ("trivialReader/m_ptOthersSummed");
TH1F *histo_invMassOthersSummed = (TH1F*)f->Get ("trivialReader/m_invMassOthersSummed");

histo_deltaEta -> GetYaxis () -> SetRangeUser (0, 0.3) ;

TCanvas *c1 = new TCanvas () ;
c1 -> Divide (3, 3) ;
c1 -> cd (1);
histo_deltaEta -> DrawNormalized  ();
c1 -> cd (2);
histo_sumEta -> DrawNormalized  ();
c1 -> cd (3);
histo_invMass -> DrawNormalized  ();
c1 -> cd (4);
histo_eta -> DrawNormalized  ();
c1 -> cd (5);
histo_energy -> DrawNormalized  ();
c1 -> cd (6);
histo_pt -> DrawNormalized  ();
c1 -> cd (7);
histo_deltaPhi -> DrawNormalized  ();

TCanvas *c2 = new TCanvas () ;
c2 -> Divide (3, 2) ;
c2 -> cd (1);
histo_etaOthers -> DrawNormalized  ();
c2 -> cd (2);
histo_energyOthers -> DrawNormalized  ();
c2 -> cd (3);
histo_ptOthers -> DrawNormalized  ();
c2 -> cd (4);
histo_deltaROthers -> DrawNormalized  ();
c2 -> cd (5);
histo_deltaEtaOthers -> DrawNormalized  ();

TCanvas *c3 = new TCanvas () ;
c3 -> Divide (3, 2) ;
c3 -> cd (1);
histo_etaOthersSummed -> DrawNormalized  ();
c3 -> cd (2);
histo_energyOthersSummed -> DrawNormalized  ();
c3 -> cd (3);
histo_ptOthersSummed -> DrawNormalized  ();
c3 -> cd (4);
histo_invMassOthersSummed -> DrawNormalized  ();

TLegend *tleg = new TLegend (0.5, 0.3, 0.9, 0.8);
tleg -> SetBorderSize (0);
tleg -> SetFillColor (0);

otherFiles ("H160_histos_WWF.root", "WWf", 1, c1, c2, c3, tleg);
otherFiles ("H160_histos_ggF.root", "ggf", 2, c1, c2, c3, tleg);
otherFiles ("Wt_histos.root", "Wt", 3, c1, c2, c3, tleg);
otherFiles ("WW_histos.root", "WW", 4, c1, c2, c3, tleg);
otherFiles ("WZ_histos.root", "WZ", 5, c1, c2, c3, tleg);
otherFiles ("WZ_incl_histos.root", "WZ_incl", 6, c1, c2, c3, tleg);
otherFiles ("ZZ_incl_histos.root", "ZZ_incl", 7, c1, c2, c3, tleg);
otherFiles ("tt1j_histos.root", "tt0j", 8, c1, c2, c3, tleg);
otherFiles ("tt1j_histos.root", "tt1j", 9, c1, c2, c3, tleg);
otherFiles ("tt2j_histos.root", "tt2j", 11, c1, c2, c3, tleg);
otherFiles ("tt3j_histos.root", "tt3j", 12, c1, c2, c3, tleg);
otherFiles ("tt4j_histos.root", "tt4j", 13, c1, c2, c3, tleg);

c1 -> cd (3);
tleg -> Draw ("same") ;
c2 -> cd (4);
tleg -> Draw ("same") ;
c3 -> cd (4);
tleg -> Draw ("same") ;
}

void otherFiles (TString nomeFile, TString tag, int color, TCanvas *c1, TCanvas *c2, TCanvas *c3, TLegend *tleg)
{

TFile *f = new TFile (nomeFile) ;
TH1F *histo_deltaEta = (TH1F*)f->Get ("trivialReader/m_deltaEtaTags");
TH1F *histo_deltaPhi = (TH1F*)f->Get ("trivialReader/m_deltaPhiTags");
TH1F *histo_sumEta = (TH1F*)f->Get ("trivialReader/m_sumEtaTags");
TH1F *histo_invMass = (TH1F*)f->Get ("trivialReader/m_invMassTags");
TH1F *histo_eta = (TH1F*)f->Get ("trivialReader/m_etaTags");
TH1F *histo_energy = (TH1F*)f->Get ("trivialReader/m_energyTags");
TH1F *histo_pt = (TH1F*)f->Get ("trivialReader/m_ptTags");
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
histo_etaOthers -> SetLineColor (color);
histo_energyOthers -> SetLineColor (color);
histo_ptOthers -> SetLineColor (color);
histo_deltaROthers -> SetLineColor (color);
histo_deltaEtaOthers -> SetLineColor (color);
histo_etaOthersSummed -> SetLineColor (color);
histo_energyOthersSummed -> SetLineColor (color);
histo_ptOthersSummed -> SetLineColor (color);
histo_invMassOthersSummed -> SetLineColor (color);

if (color == 1) 
{
histo_deltaEta -> SetFillColor (color);
histo_deltaPhi -> SetFillColor (color);
histo_sumEta -> SetFillColor (color);
histo_invMass -> SetFillColor (color);
histo_eta -> SetFillColor (color);
histo_energy -> SetFillColor (color);
histo_pt -> SetFillColor (color);
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
histo_deltaEta -> GetYaxis () -> SetRangeUser (0, 0.3) ;
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

c3 -> cd (1);
histo_etaOthersSummed -> DrawNormalized  ("same");
c3 -> cd (2);
histo_energyOthersSummed -> DrawNormalized  ("same");
c3 -> cd (3);
histo_ptOthersSummed -> DrawNormalized  ("same");
c3 -> cd (4);
histo_invMassOthersSummed -> DrawNormalized  ("same");

tleg-> AddEntry (histo_invMassOthersSummed, tag,"l") ;

}

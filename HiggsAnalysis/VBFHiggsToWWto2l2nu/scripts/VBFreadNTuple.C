{                            
TFile *f0 = new TFile ("CMSSW_histos/tt0j_histos.root") ;
TNtuple *t0 = (TNtuple*)f0->Get ("trivialReader/ntuple");
TFile *f1 = new TFile ("CMSSW_histos/tt1j_histos.root") ;
TNtuple *t1 = (TNtuple*)f1->Get ("trivialReader/ntuple");
TFile *f2 = new TFile ("CMSSW_histos/tt2j_histos.root") ;
TNtuple *t2 = (TNtuple*)f2->Get ("trivialReader/ntuple");
TFile *f3 = new TFile ("CMSSW_histos/tt3j_histos.root") ;
TNtuple *t3 = (TNtuple*)f3->Get ("trivialReader/ntuple");
TFile *f4 = new TFile ("CMSSW_histos/tt4j_histos.root") ;
TNtuple *t4 = (TNtuple*)f4->Get ("trivialReader/ntuple");

TString what = "nJet30>>my_histo";
TString cut ="deltaEta>2 && mInv>300 && Ptmax < 200 && nJet30 < 3";
double min = 0;
double max = 15;
int bin = 15;
TH1F *my_histo0 = new TH1F ("my_histo0","titolo",bin,min,max) ;						
t0->Draw (what+"0",cut);
TH1F *my_histo1 = new TH1F ("my_histo1","titolo",bin,min,max) ;						
t1->Draw (what+"1",cut);
TH1F *my_histo2 = new TH1F ("my_histo2","titolo",bin,min,max) ;						
t2->Draw (what+"2",cut);
TH1F *my_histo3 = new TH1F ("my_histo3","titolo",bin,min,max) ;						
t3->Draw (what+"3",cut);
TH1F *my_histo4 = new TH1F ("my_histo4","titolo",bin,min,max) ;						
t4->Draw (what+"4",cut);

TH1F *my_histo = new TH1F ("my_histo","titolo",bin,min,max) ;						

double crossSections[5]= {334.51/42000. // tt0j 
						  90.23/26082., // tt1j
						  8.75/20000.,  // tt2j
						  3.16/12000.,   // tt3j
						  0.82/2959.};  // tt4j
							
my_histo0->Scale (crossSections[0]) ;
my_histo->Add (my_histo0) ;
my_histo1->Scale (crossSections[1]) ;
my_histo->Add (my_histo1) ;
my_histo2->Scale (crossSections[2]) ;
my_histo->Add (my_histo2) ;
my_histo3->Scale (crossSections[3]) ;
my_histo->Add (my_histo3) ;
my_histo4->Scale (crossSections[4]) ;
my_histo->Add (my_histo4) ;

my_histo->DrawCopy () ;
cout << my_histo->Integral () << std::endl;

}
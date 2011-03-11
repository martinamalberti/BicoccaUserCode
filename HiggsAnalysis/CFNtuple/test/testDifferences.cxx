void testDifferences()
{

   int NBIN = 100;
//   double MIN = 25;
   double MIN = -200;
   double MAX = 800;
    
  cout << "================================= FILLING HISTOGRAMS =================================" << endl;
 

  TFile FileMC_My("~/Dropbox/QCD_CF/MyHerwig/HerwigJimmy15.root","READ"); 
  TFile FileMC_Bo("~/Dropbox/QCD_CF/NtupleBo/cut_35_35/qcd_15_herwigjimmy.root","READ");
    
  TTree* TreeMC_Bo = (TTree*) FileMC_Bo.Get("AnaHiggs");
  TTree* TreeMC_My = (TTree*) FileMC_My.Get("AnaHiggs");
  
  double xsec1H = 1.;		//cross section HERWIG
  double xsec1 = 1.;		//cross section HERWIG

  Int_t S_EventNb; //~~~~ event number


  Float_t G_FJet_Pt; //~~~~ had
  Float_t S_FJet_Pt; //~~~~ reco

  Float_t G_CJet_Pt; //~~~~ had
  Float_t S_CJet_Pt; //~~~~ reco
 
  Double_t G_FJet_Pt_D; //~~~~ had
  Double_t S_FJet_Pt_D; //~~~~ reco

  Double_t G_CJet_Pt_D; //~~~~ had
  Double_t S_CJet_Pt_D; //~~~~ reco

  Double_t G_FJet_Eta_D; //~~~~ had
  Double_t S_FJet_Eta_D; //~~~~ reco

  Double_t G_CJet_Eta_D; //~~~~ had
  Double_t S_CJet_Eta_D; //~~~~ reco
 
 

 TreeMC_Bo->SetBranchAddress("S_EventNb",&S_EventNb); 
 TreeMC_Bo->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeMC_Bo->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeMC_Bo->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeMC_Bo->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeMC_My->SetBranchAddress("S_EventNb",&S_EventNb); 
 TreeMC_My->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt_D);
 TreeMC_My->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt_D);
 TreeMC_My->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt_D);
 TreeMC_My->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt_D);

 TreeMC_My->SetBranchAddress("G_FJet_Eta",&G_FJet_Eta_D);
 TreeMC_My->SetBranchAddress("S_FJet_Eta",&S_FJet_Eta_D);
 TreeMC_My->SetBranchAddress("G_CJet_Eta",&G_CJet_Eta_D);
 TreeMC_My->SetBranchAddress("S_CJet_Eta",&S_CJet_Eta_D);

 TH1D* MyHerwigC15 = new TH1D ("MyHerwigC15", "MyHerwig Central 15 HAD",    NBIN, MIN, MAX);
 TH1D* MyHerwigF15 = new TH1D ("MyHerwigF15", "MyHerwig Forward 15", NBIN, MIN, MAX);
  
 TH1D* HerwigC15= new TH1D ("HerwigC15", "Herwig Central 15 HAD",    NBIN, MIN, MAX);
 TH1D* HerwigF15 = new TH1D ("HerwigF15", "Herwig Forward 15", NBIN, MIN, MAX);
  
 TH1D* MyHerwigC15_reco = new TH1D ("MyHerwigC15_reco", "MyHerwig Central_15_reco",    NBIN, MIN, MAX);
 TH1D* MyHerwigF15_reco = new TH1D ("MyHerwigF15_reco", "MyHerwig Forward 15_reco", NBIN, MIN, MAX);
  
 TH1D* HerwigC15_reco= new TH1D ("HerwigC15_reco", "Herwig Central 15_reco",    NBIN, MIN, MAX);
 TH1D* HerwigF15_reco = new TH1D ("HerwigF15_reco", "Herwig Forward 15_reco", NBIN, MIN, MAX);
  
std::cerr << "S_EventNb ::: G_FJet_Pt ::: G_CJet_Pt ::: S_FJet_Pt :::S_CJet_Pt " << std::endl;
 for (Int_t iEvt= 0; iEvt<TreeMC_Bo->GetEntries(); iEvt++) {
   TreeMC_Bo->GetEntry(iEvt);
   if (G_CJet_Pt>35 && G_FJet_Pt>35){ // && (S_CJet_Pt>15 && S_FJet_Pt>15)) {
   //if (((G_CJet_Pt>35 && G_FJet_Pt>35) || (S_CJet_Pt>35 && S_FJet_Pt>35))){ // && (S_CJet_Pt>15 && S_FJet_Pt>15)) {
//       std::cerr << " " << S_EventNb << " ::: " << G_FJet_Pt << " ::: " << G_CJet_Pt << " ::: " << S_FJet_Pt << " ::: " << S_CJet_Pt << " " << std::endl;
     HerwigF15->Fill(G_FJet_Pt,xsec1H);
     HerwigC15->Fill(G_CJet_Pt,xsec1H);
   }       
   if (S_CJet_Pt>35 && S_FJet_Pt>35){ // && (S_CJet_Pt>15 && S_FJet_Pt>15)) {
     HerwigF15_reco->Fill(S_FJet_Pt,xsec1H);
     HerwigC15_reco->Fill(S_CJet_Pt,xsec1H);     
   }
 }

std::cerr << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~ our ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
std::cerr << "S_EventNb ::: G_FJet_Pt_D ::: G_CJet_Pt_D ::: S_FJet_Pt_D ::: S_CJet_Pt_D ::: G_FJet_Eta_D ::: G_CJet_Eta_D ::: S_FJet_Eta_D :::S_CJet_Eta_D " << std::endl;


 std::map<int,int> BoEvents;
 std::map<int,int> MyEvents;
 
 for (Int_t iEvt= 0; iEvt<TreeMC_Bo->GetEntries(); iEvt++) {
   TreeMC_Bo->GetEntry(iEvt);
    if (G_CJet_Pt_D>35 && G_FJet_Pt_D>35){ // && (S_CJet_Pt_D>15 && S_FJet_Pt_D>15)){
     BoEvents[S_EventNb] = 1;
  }
 }   
 
 for (Int_t iEvt= 0; iEvt<TreeMC_My->GetEntries(); iEvt++) {
   TreeMC_My->GetEntry(iEvt);
    if (G_CJet_Pt_D>35 && G_FJet_Pt_D>35){ // && (S_CJet_Pt_D>15 && S_FJet_Pt_D>15)){
     MyEvents[S_EventNb] = 1;
     if (! BoEvents.find(S_EventNb) ) std::cerr << " perbacco ... Bo non vede l'evento : " << S_EventNb << std::endl;
  }
 }   

 for (Int_t iEvt= 0; iEvt<TreeMC_Bo->GetEntries(); iEvt++) {
   TreeMC_Bo->GetEntry(iEvt);
    if (G_CJet_Pt_D>35 && G_FJet_Pt_D>35){ // && (S_CJet_Pt_D>15 && S_FJet_Pt_D>15)){
     if (! MyEvents.find(S_EventNb) ) std::cerr << " perbacco ... Noi non vediamo l'evento : " << S_EventNb << std::endl;
  }
 }   
 
  //riempio con MyHerwig ptHat 15
 for (Int_t iEvt= 0; iEvt<TreeMC_My->GetEntries(); iEvt++) {
   TreeMC_My->GetEntry(iEvt);
    if (G_CJet_Pt_D>35 && G_FJet_Pt_D>35){ // && (S_CJet_Pt_D>15 && S_FJet_Pt_D>15)){
//    if (((G_CJet_Pt_D>35 && G_FJet_Pt_D>35) || (S_CJet_Pt_D>35 && S_FJet_Pt_D>35))){ // && (S_CJet_Pt_D>15 && S_FJet_Pt_D>15)){
        MyHerwigF15->Fill(G_FJet_Pt_D,xsec1);
        MyHerwigC15->Fill(G_CJet_Pt_D,xsec1);
        //std::cerr << " " << S_EventNb << " ::: " << G_FJet_Pt_D << " ::: " << G_CJet_Pt_D << " ::: " << S_FJet_Pt_D << " ::: " << S_CJet_Pt_D << " ::: " << G_FJet_Eta_D << " ::: " << G_CJet_Eta_D << " ::: " << S_FJet_Eta_D << " ::: " << S_CJet_Eta_D << std::endl;
	}
    if (S_CJet_Pt_D>35 && S_FJet_Pt_D>35){
//        std::cerr << " CCCCCCCCCCCCCCCCCIAO !!!! " << std::endl;
	MyHerwigF15_reco->Fill(S_FJet_Pt_D,xsec1);
        MyHerwigC15_reco->Fill(S_CJet_Pt_D,xsec1);
    }
   }
 
  cout << "===================================== DRAW ====================================" << endl;

  
  TCanvas* MyHerwigCentral = new TCanvas("MyHerwigCentral","MyHerwig Central HAD level",600,600);  
//  MyHerwigC15->SetStats(0);
  MyHerwigC15->SetFillColor(kRed);  
  MyHerwigC15->SetLineColor(kRed);
  MyHerwigC15->SetFillStyle(3005);
  MyHerwigC15->SetMarkerStyle(20);
  MyHerwigC15->SetMarkerColor(kRed);
  MyHerwigC15->SetMarkerSize(1);
  MyHerwigC15->SetLineWidth(2);
  MyHerwigC15->GetXaxis()->SetTitle("Central p_{T}");

  HerwigC15->SetFillColor(kGray);  
  HerwigC15->SetFillStyle(3002);
  HerwigC15->SetMarkerStyle(20);
  HerwigC15->SetMarkerSize(1);
  HerwigC15->SetLineWidth(2);
  HerwigC15->SetLineStyle(9);
  HerwigC15->GetXaxis()->SetTitle("Central p_{T}");

   if (MyHerwigC15->GetMaximum() > HerwigC15->GetMaximum()) {
     MyHerwigC15->DrawCopy();
     HerwigC15->DrawCopy("SAME");
   } else {
     HerwigC15->DrawCopy();
     MyHerwigC15->DrawCopy("SAME");
   }

   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
  leg->AddEntry("MyHerwigC15","Herwig 15_MyNtuple", "lpf");
  leg->AddEntry("HerwigC15","Herwig 15_BoNtuple", "lpf");
  leg->Draw();    
  for (int iBin = 0; iBin<NBIN; iBin ++){
   std::cerr<<"Red/Black = ["<< iBin << "] " << MyHerwigC15->GetBinContent(iBin+1) << " / " << HerwigC15->GetBinContent(iBin+1) << " :: " ;
   if (HerwigC15->GetBinContent(iBin+1) != 0) std::cout << MyHerwigC15->GetBinContent(iBin+1)/HerwigC15->GetBinContent(iBin+1);
   std::cout <<std::endl;
  }
   
   std::cerr<<"My/Bo Red/Black tot (integral) = " << MyHerwigC15->Integral() << " / " << HerwigC15->Integral() <<std::endl;
   std::cerr<<"My/Bo Red/Black tot (entries) = " << MyHerwigC15->GetEntries() << " / " << HerwigC15->GetEntries() <<std::endl;


std::cerr << "  ~~~~~~~~~~~~~~~ " << std::endl;
std::cerr << "  ~~~~~~~~~~~~~~~ " << std::endl;
std::cerr << "  ~~~~ Reco ~~~~~ " << std::endl;

for (int iBin = 0; iBin<NBIN; iBin ++){

   std::cerr<<"Red/Black = ["<< iBin << "] " << MyHerwigC15_reco->GetBinContent(iBin+1) << " / " << HerwigC15_reco->GetBinContent(iBin+1) << " :: " ;
   if (HerwigC15_reco->GetBinContent(iBin+1) != 0) std::cout << MyHerwigC15_reco->GetBinContent(iBin+1)/HerwigC15_reco->GetBinContent(iBin+1);
   std::cout <<std::endl;
  }
   
   std::cerr<<"My/Bo Red/Black tot (integral) = " << MyHerwigC15_reco->Integral() << " / " << HerwigC15_reco->Integral() <<std::endl;
   std::cerr<<"My/Bo Red/Black tot (entries) = " << MyHerwigC15_reco->GetEntries() << " / " << HerwigC15_reco->GetEntries() <<std::endl;
   
  TCanvas* MyHerwigCentral_reco = new TCanvas("MyHerwigCentral_reco","MyHerwig Central DET level",600,600);  
  MyHerwigCentral_reco->cd();
//  MyHerwigC15_reco->SetStats(0);
  MyHerwigC15_reco->SetFillColor(kRed);  
  MyHerwigC15_reco->SetLineColor(kRed);
  MyHerwigC15_reco->SetFillStyle(3005);
  MyHerwigC15_reco->SetMarkerStyle(20);
  MyHerwigC15_reco->SetMarkerColor(kRed);
  MyHerwigC15_reco->SetMarkerSize(1);
  MyHerwigC15_reco->SetLineWidth(2);
  MyHerwigC15_reco->GetXaxis()->SetTitle("Central p_{T}");

  HerwigC15_reco->SetFillColor(kGray);  
  HerwigC15_reco->SetFillStyle(3002);
  HerwigC15_reco->SetMarkerStyle(20);
  HerwigC15_reco->SetMarkerSize(1);
  HerwigC15_reco->SetLineWidth(2);
  HerwigC15_reco->SetLineStyle(9);
  HerwigC15_reco->GetXaxis()->SetTitle("Central p_{T}");

   if (MyHerwigC15_reco->GetMaximum() > HerwigC15_reco->GetMaximum()) {
     MyHerwigC15_reco->DrawCopy();
     HerwigC15_reco->DrawCopy("SAME");
   } else {
     HerwigC15_reco->DrawCopy();
     MyHerwigC15_reco->DrawCopy("SAME");
   }
  leg->Draw();    
  
}
#ifndef __CINT__
int main () { ptHat(); return 0; }  // Main program when run stand-alone
#endif

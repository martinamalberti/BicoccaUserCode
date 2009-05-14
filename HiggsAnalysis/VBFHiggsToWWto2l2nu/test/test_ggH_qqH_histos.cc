{
 
//  char name[100] = {"mcH_p"};
 char name[100] = {"mcH_pL"};
//  char name[100] = {"mcH_pT"};

 TFile *fileGG = TFile::Open("/tmp/amassiro/VBFMCDumper_ggH_H130.root");
 TFile *fileQQ = TFile::Open("/tmp/amassiro/VBFMCDumper_qqH_H130.root");
 
 fileGG->cd("mcH");
 TH1F *ggH_p = (TH1F *) gROOT->FindObject(name);
 char ggH_name[100];
 sprintf(ggH_name,"ggH_%s",name);
 ggH_p->SetName(ggH_name);
 ggH_p->SetLineColor(kRed);
 ggH_p->Rebin(20);
 ggH_p->DrawNormalized();

 fileQQ->cd("mcH");
 TH1F *qqH_p = (TH1F *) gROOT->FindObject(name);
 char qqH_name[100];
 sprintf(qqH_name,"qqH_%s",name);
 qqH_p->SetName(qqH_name);
 qqH_p->SetLineColor(kBlue);
 qqH_p->Rebin(20);
 qqH_p->DrawNormalized("same");

 
 
 
 TLegend *leg = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
 leg->AddEntry(ggH_name,ggH_name,"lpf");
 leg->AddEntry(qqH_name,qqH_name,"lpf");
 leg->Draw();

    
}
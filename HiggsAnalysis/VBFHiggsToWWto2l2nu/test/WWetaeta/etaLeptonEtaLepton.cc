{

//---------------------- eta Lepton vs eta Lepton -------------

gStyle->SetOptStat(0);
TCanvas c1("c1","c1",10,10,700,700);
_file0->cd(0);
mcL1L2_eta->SetFillStyle(3001);
mcL1L2_eta->SetFillColor(kRed);
mcL1L2_eta->Draw("BOX");
mcL1L2_eta->RebinX(20);
mcL1L2_eta->RebinY(20);
mcL1L2_eta->GetXaxis()->SetTitle("#eta Lepton_{1}");
mcL1L2_eta->GetYaxis()->SetTitle("#eta Lepton_{2}");

leg = new TLegend(0.1,0.7,0.48,0.9);
leg->AddEntry(&mcL1L2_eta,"HWW","f");


_file1->cd(0);
mcL1L2_eta->SetFillColor(kBlue);
// mcL1L2_eta->SetFillStyle(3000);
mcL1L2_eta->RebinX(20);
mcL1L2_eta->RebinY(20);
mcL1L2_eta->GetXaxis()->SetTitle("#eta Lepton_{1}");
mcL1L2_eta->GetYaxis()->SetTitle("#eta Lepton_{2}");
mcL1L2_eta->Draw("BOXsame");
leg->AddEntry(&mcL1L2_eta,"ttbar","f");


_file0->cd(0);
mcL1L2_eta->Draw("BOXsame");

TLine LVert(0,-8,0,8);
LVert.SetLineWidth(2);
LVert.SetLineColor(kGreen);
LVert.Draw();
TLine LHor(-8,0,8,0);
LHor.SetLineWidth(2);
LHor.SetLineColor(kGreen);
LHor.Draw();

leg->Draw();


TCanvas c2("c2","c2",10,10,1000,500);
c2.Divide(2,1);

c2.cd(1);
_file0->cd(0);
mcL1L2_eta->Draw("BOX");

c2.cd(2);
_file1->cd(0);
mcL1L2_eta->Draw("BOX");


TString name_file;
name_file = _file0->GetName();
std::cerr << name_file << std::endl;
name_file.Remove(0,20);
std::cerr << name_file << std::endl;
name_file.Remove(18,10);
std::cerr << name_file << std::endl;
c1.SaveAs("etaLeptonEtaLepton_bis_" + name_file + ".png");
c2.SaveAs("etaLeptonEtaLepton" + name_file + ".png");


}
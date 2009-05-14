{

//---- Delta Eta ----


gStyle->SetOptStat(0);
TCanvas c1("c1","c1",10,10,700,700);

_file0->cd(0);
mcL1L2_DiffEta->SetFillStyle(3001);
mcL1L2_DiffEta->SetFillColor(kBlue);
mcL1L2_DiffEta->Rebin(10);
mcL1L2_DiffEta->DrawNormalized();
mcL1L2_DiffEta->GetXaxis()->SetTitle("#Delta#eta Lepton");
mcL1L2_DiffEta->GetYaxis()->SetTitle("Normalized");
mcL1L2_DiffEta->DrawNormalized();
leg = new TLegend(0.1,0.7,0.48,0.9);
leg->AddEntry(&mcL1L2_DiffEta,"HWW","f");

_file1->cd(0);
mcL1L2_DiffEta->SetFillColor(kRed);
mcL1L2_DiffEta->SetFillStyle(3002);
mcL1L2_DiffEta->Rebin(10);
mcL1L2_DiffEta->DrawNormalized("same");
leg->AddEntry(&mcL1L2_DiffEta,"ttbar","f");

leg->Draw();


TString name_file;
name_file = _file0->GetName();
std::cerr << name_file << std::endl;
name_file.Remove(0,20);
std::cerr << name_file << std::endl;
name_file.Remove(18,10);
std::cerr << name_file << std::endl;
c1.SaveAs("deltaEtaLeptonLepton_" + name_file + ".png");



}
{


//---- Sum Eta ----


gStyle->SetOptStat(0);
TCanvas c1("c1","c1",10,10,700,700);

_file1->cd(0);
mcV1V2_SumEta->SetFillStyle(3001);
mcV1V2_SumEta->SetFillColor(kRed);
mcV1V2_SumEta->Rebin(10);
mcV1V2_SumEta->DrawNormalized();
mcV1V2_SumEta->GetXaxis()->SetTitle("#Sigma#eta W");
mcV1V2_SumEta->GetYaxis()->SetTitle("Normalized");
mcV1V2_SumEta->DrawNormalized();
leg = new TLegend(0.1,0.7,0.48,0.9);
leg->AddEntry(&mcV1V2_SumEta,"ttbar","f");
std::cerr << "tt -> GetKurtosis = " << mcV1V2_SumEta->GetKurtosis() << std::endl;

_file0->cd(0);
mcV1V2_SumEta->SetFillColor(kBlue);
mcV1V2_SumEta->SetFillStyle(3002);
mcV1V2_SumEta->Rebin(10);
mcV1V2_SumEta->GetXaxis()->SetTitle("#Sigma#eta W");
mcV1V2_SumEta->GetYaxis()->SetTitle("Normalized");
mcV1V2_SumEta->DrawNormalized("same");
leg->AddEntry(&mcV1V2_SumEta,"HWW","f");
std::cerr << "HWW -> GetKurtosis = " << mcV1V2_SumEta->GetKurtosis() << std::endl;

leg->Draw();


TString name_file;
name_file = _file0->GetName();
std::cerr << name_file << std::endl;
name_file.Remove(0,20);
std::cerr << name_file << std::endl;
name_file.Remove(18,10);
std::cerr << name_file << std::endl;
c1.SaveAs("sumEtaWW_" + name_file + ".png");





}
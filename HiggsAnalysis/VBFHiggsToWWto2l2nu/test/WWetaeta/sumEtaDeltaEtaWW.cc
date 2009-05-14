{


//---- Sum Eta vs Delta Eta ----


gStyle->SetOptStat(0);
TCanvas c1("c1","c1",10,10,700,700);

_file0->cd(0);
mcV1V2_SumDiffEta->SetFillStyle(3001);
mcV1V2_SumDiffEta->SetFillColor(kRed);
mcV1V2_SumDiffEta->RebinX(20);
mcV1V2_SumDiffEta->RebinY(20);
mcV1V2_SumDiffEta->DrawNormalized("BOX");
mcV1V2_SumDiffEta->GetXaxis()->SetTitle("#Delta#eta W");
mcV1V2_SumDiffEta->GetYaxis()->SetTitle("#Sigma#eta W");
mcV1V2_SumDiffEta->DrawNormalized("BOX");

leg = new TLegend(0.1,0.7,0.48,0.9);
leg->AddEntry(&mcV1V2_SumDiffEta,"HWW","f");


_file1->cd(0);
mcV1V2_SumDiffEta->SetFillColor(kBlue);
mcV1V2_SumDiffEta->SetFillStyle(3001);
mcV1V2_SumDiffEta->RebinX(20);
mcV1V2_SumDiffEta->RebinY(20);
mcV1V2_SumDiffEta->GetXaxis()->SetTitle("#Delta#eta W");
mcV1V2_SumDiffEta->GetYaxis()->SetTitle("#Sigma#eta W");
mcV1V2_SumDiffEta->DrawNormalized("BOXsame");
leg->AddEntry(&mcV1V2_SumDiffEta,"ttbar","f");

_file0->cd(0);
mcV1V2_SumDiffEta->DrawNormalized("BOXsame");

gPad->SetGrid();
leg->Draw();


TCanvas c2("c2","c2",10,10,1000,500);
c2.Divide(2,1);

c2.cd(1);
_file0->cd(0);
mcV1V2_SumDiffEta->Draw("BOX");
gPad->SetGrid();

c2.cd(2);
_file1->cd(0);
mcV1V2_SumDiffEta->Draw("BOX");
gPad->SetGrid();

TString name_file;
name_file = _file0->GetName();
std::cerr << name_file << std::endl;
name_file.Remove(0,20);
std::cerr << name_file << std::endl;
name_file.Remove(18,10);
std::cerr << name_file << std::endl;
c1.SaveAs("sumEtaDeltaEtaWW_bis_" + name_file + ".png");
c2.SaveAs("sumEtaDeltaEtaWW_" + name_file + ".png");


}
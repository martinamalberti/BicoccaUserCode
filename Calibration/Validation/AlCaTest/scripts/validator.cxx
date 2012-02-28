{
  TH1F EoP ("EoP","EoP",200,0,1.2);
  TH1F EoPZ ("EoPZ","EoPZ",80,0.9,1.1);
  TH1F etaDist ("etaDist","etaDist",200,-3,3);
  TH1F energy ("energy","energy",300,0,800);
  TH1F ESCoP ("ESCoP","ESCoP",200,0,20);
  TH1F HitsOe ("HitsOe","HitsOe",200,0,1.02);
  m_tree->Draw("energy/momentum>>EoP");
  m_tree->Draw("energy/momentum>>EoPZ");
  m_tree->Draw("eta>>etaDist");
  m_tree->Draw("energy>>energy");
  m_tree->Draw("ESCoP>>ESCoP");
  m_tree->Draw("recHits/energy>>HitsOe");
  TFile file ("validazione_TTbar_3_3_3.root","RECREATE");
  EoP.Write();
  EoPZ.Write();
  etaDist.Write();
  energy.Write();
  ESCoP.Write();
  HitsOe.Write();
  file.Close();

  gApplication->Terminate(0);
}

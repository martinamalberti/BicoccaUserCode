std::string path = "/gwpool/users/benaglia/VBF/CMSSW_2_2_13/src/HiggsAnalysis/VBFHiggsToWWTolnujj/data/";






//-------------------------------------
// draw efficiencies after preselection
//-------------------------------------

void drawEfficiencies()
{
  std::ifstream inFile((path+"list.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  std::cout << files.at(0) << std::endl;  
  TFile* inRootFile = new TFile((path+"VBFPreselectionPlots_"+files.at(0)+".root").c_str(), "READ");
  TH1F* efficiencies_sisCone5CaloJets = static_cast<TH1F*>(inRootFile -> Get("efficiencies_sisCone5CaloJets"));
  
  
  
  TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", "efficiencies_sisCone5CaloJets");
  c1 -> SetLogy();
  c1 -> SetGridy();
  efficiencies_sisCone5CaloJets -> SetLineColor(kBlack);
  efficiencies_sisCone5CaloJets -> SetLineWidth(3);
  efficiencies_sisCone5CaloJets -> GetYaxis() -> SetTitle("#varepsilon");
  efficiencies_sisCone5CaloJets -> GetXaxis() -> SetLabelSize(0.04);
  efficiencies_sisCone5CaloJets -> GetYaxis() -> SetLabelSize(0.04);
  efficiencies_sisCone5CaloJets -> GetYaxis() -> SetTitleOffset(1.10);
  efficiencies_sisCone5CaloJets -> GetYaxis() -> SetRangeUser(0.0001, 2.);
  efficiencies_sisCone5CaloJets -> Draw();
  legend -> AddEntry(efficiencies_sisCone5CaloJets, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
  
  for(int i = 1; i < files.size(); ++i)
  {
    std::cout << files.at(i) << std::endl;
    inRootFile = new TFile((path+"VBFPreselectionPlots_"+files.at(i)+".root").c_str(), "READ");
    efficiencies_sisCone5CaloJets = static_cast<TH1F*>(inRootFile -> Get("efficiencies_sisCone5CaloJets"));
    
    efficiencies_sisCone5CaloJets -> SetLineColor(i+1);
    efficiencies_sisCone5CaloJets -> SetLineWidth(2);
    efficiencies_sisCone5CaloJets -> Draw("same");
    legend -> AddEntry(efficiencies_sisCone5CaloJets, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
  }
  
}






//-----------------------------------------
// draw number of events after preselection
//-----------------------------------------

void drawEventsScaled(double lumi = 1000.)
{
  std::ifstream inFile((path+"list.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  TFile* inRootFile = new TFile((path+"VBFPreselectionPlots_"+files.at(0)+".root").c_str(), "READ");
  TH1F* eventsScaled_sisCone5CaloJets = static_cast<TH1F*>(inRootFile -> Get("eventsScaled_sisCone5CaloJets"));
  
  
  
  TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", "events_sisCone5CaloJets");
  c1 -> SetLogy();
  c1 -> SetGridy();
  eventsScaled_sisCone5CaloJets -> SetLineColor(kBlack);
  eventsScaled_sisCone5CaloJets -> SetLineWidth(3);
  eventsScaled_sisCone5CaloJets -> Scale(lumi);
  char title[50];
  sprintf(title, "events / %d pb^{-1}", lumi);
  eventsScaled_sisCone5CaloJets -> GetYaxis() -> SetTitle(title);
  eventsScaled_sisCone5CaloJets -> GetXaxis() -> SetLabelSize(0.04);
  eventsScaled_sisCone5CaloJets -> GetYaxis() -> SetLabelSize(0.04);
  eventsScaled_sisCone5CaloJets -> GetYaxis() -> SetTitleOffset(1.10);
  eventsScaled_sisCone5CaloJets -> GetYaxis() -> SetRangeUser(1., 10000000000000.);
  eventsScaled_sisCone5CaloJets -> Draw();
  legend -> AddEntry(eventsScaled_sisCone5CaloJets, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
  
  for(int i = 1; i < files.size(); ++i)
  {
    inRootFile = new TFile((path+"VBFPreselectionPlots_"+files.at(i)+".root").c_str(), "READ");
    eventsScaled_sisCone5CaloJets = static_cast<TH1F*>(inRootFile -> Get("eventsScaled_sisCone5CaloJets"));
    
    eventsScaled_sisCone5CaloJets -> SetLineColor(i+1);
    eventsScaled_sisCone5CaloJets -> SetLineWidth(2);
    eventsScaled_sisCone5CaloJets -> Scale(lumi);
    eventsScaled_sisCone5CaloJets -> Draw("same");
    legend -> AddEntry(eventsScaled_sisCone5CaloJets, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
  }
  
}

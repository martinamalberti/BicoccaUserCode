std::string path = "/gwpool/users/benaglia/VBF/CMSSW_2_2_13/src/HiggsAnalysis/VBFHiggsToWWTolnujj/data/";

int* colors = new int[19];
colors[0]  = kBlack;
colors[1]  = kRed;
colors[2]  = kGreen;
colors[3]  = kBlue;
colors[4]  = kYellow;
colors[5]  = kMagenta;
colors[6]  = kCyan;
colors[7]  = kRed+1;
colors[8]  = kGreen+1;
colors[9]  = kBlue+1;
colors[10] = kYellow+1;
colors[11] = kMagenta+1;
colors[12] = kCyan+1;
colors[13]  = kRed+2;
colors[14]  = kGreen+2;
colors[15]  = kBlue+2;
colors[16] = kYellow+2;
colors[17] = kMagenta+2;
colors[18] = kCyan+2;







//-------------------------------------
// drawPlot 
//-------------------------------------

void drawPlot(const std::string& histoName, int step = 1, int rebin = 5)
{
  std::ifstream inFile((path+"listAll.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  std::cout << files.at(0) << std::endl;  
  TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
  string string1 = "RecoHistograms_step";
  char char1[50];
  sprintf(char1, "%d/%d_", step, step);
  std::string fullHistoName = string1 + std::string(char1) + histoName;
  
  TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(fullHistoName.c_str()));
  
  
  TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", histoName.c_str());
  c1 -> SetLogy();
  c1 -> SetGridy();
  histo -> SetLineColor(kBlack);
  histo -> SetLineWidth(3);
  histo -> GetXaxis() -> SetTitle(histoName.c_str());
  histo -> GetYaxis() -> SetTitle("event fraction");
  histo -> GetXaxis() -> SetLabelSize(0.04);
  histo -> GetYaxis() -> SetLabelSize(0.04);
  histo -> GetYaxis() -> SetTitleOffset(1.10);
  histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
  histo -> Rebin(rebin);
  histo -> Draw();
  legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
 
  for(int i = 1; i < files.size(); ++i)
  {
    std::cout << files.at(i) << std::endl;
    inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
    histo = static_cast<TH1F*>(inRootFile -> Get(fullHistoName.c_str()));
    
    histo -> SetLineColor(colors[i]);
    histo -> SetLineWidth(2);
    histo -> Rebin(rebin);
    histo -> Draw("same");
    legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
    }
}






//----------------------
// draw number of events
//----------------------

void drawEvents()
{
  std::ifstream inFile((path+"listAll.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  TH1F* events = static_cast<TH1F*>(inRootFile -> Get("events"));
  
  
  
  TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", "events");
  c1 -> SetLogy();
  c1 -> SetGridy();
  events -> SetLineColor(kBlack);
  events -> SetLineWidth(3);
  char title[50];
  sprintf(title, "events");
  events -> GetYaxis() -> SetTitle(title);
  events -> GetXaxis() -> SetLabelSize(0.04);
  events -> GetYaxis() -> SetLabelSize(0.04);
  events -> GetYaxis() -> SetTitleOffset(1.10);
  events -> GetYaxis() -> SetRangeUser(1., 10000000000000.);
  events -> Draw();
  legend -> AddEntry(events, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
  
  for(int i = 1; i < files.size(); ++i)
  {
    inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
    events = static_cast<TH1F*>(inRootFile -> Get("events"));
    
    events -> SetLineColor(colors[i]);
    events -> SetLineWidth(2);
    events -> Draw("same");
    legend -> AddEntry(events, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
    
    
  }
  
}






//------------------
// draw efficiencies
//------------------

void drawEfficiencies()
{
  std::ifstream inFile((path+"listAll.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  TH1F* efficiencies = static_cast<TH1F*>(inRootFile -> Get("efficiencies"));
  
  
  
  TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", "events");
  c1 -> SetLogy();
  c1 -> SetGridy();
  efficiencies -> SetLineColor(kBlack);
  efficiencies -> SetLineWidth(3);
  char title[50];
  sprintf(title, "events");
  efficiencies -> GetYaxis() -> SetTitle(title);
  efficiencies -> GetXaxis() -> SetLabelSize(0.04);
  efficiencies -> GetYaxis() -> SetLabelSize(0.04);
  efficiencies -> GetYaxis() -> SetTitleOffset(1.10);
  efficiencies -> GetYaxis() -> SetRangeUser(0.000001., 2.);
  efficiencies -> Draw();
  legend -> AddEntry(efficiencies, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
  
  for(int i = 1; i < files.size(); ++i)
  {
    inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
    efficiencies = static_cast<TH1F*>(inRootFile -> Get("efficiencies"));
    
    efficiencies -> SetLineColor(colors[i]);
    efficiencies -> SetLineWidth(2);
    efficiencies -> Draw("same");
    legend -> AddEntry(efficiencies, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
    
    
  }
  
}






//---------------------------
// draw relative efficiencies
//---------------------------

void drawRelativeEfficiencies()
{
  std::ifstream inFile((path+"listAll.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  TH1F* events = static_cast<TH1F*>(inRootFile -> Get("events"));
  int nBins = events -> GetNbinsX();
  
  TH1F* relativeEfficiencies = new TH1F("relativeEfficiencies", "relativeEfficiencies", nBins, 0., nBins);
  relativeEfficiencies -> SetBinContent( 1, 1.);
  for(int bin = 2; bin <= nBins; ++ bin)
    relativeEfficiencies -> SetBinContent( bin, 1. * events -> GetBinContent(bin) / events -> GetBinContent(bin-1) );
    
  
  
  TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", "events");
  c1 -> SetLogy();
  c1 -> SetGridy();
  relativeEfficiencies -> SetLineColor(kBlack);
  relativeEfficiencies -> SetLineWidth(3);
  char title[50];
  sprintf(title, "events");
  relativeEfficiencies -> GetYaxis() -> SetTitle(title);
  relativeEfficiencies -> GetXaxis() -> SetLabelSize(0.04);
  relativeEfficiencies -> GetYaxis() -> SetLabelSize(0.04);
  relativeEfficiencies -> GetYaxis() -> SetTitleOffset(1.10);
  relativeEfficiencies -> GetYaxis() -> SetRangeUser(0.001., 2.);
  relativeEfficiencies -> Draw();
  legend -> AddEntry(relativeEfficiencies, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
  
  for(int i = 1; i < files.size(); ++i)
  {
    inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
    
    events = static_cast<TH1F*>(inRootFile -> Get("events"));
    
    char histoName[50];
    sprintf(histoName, "relativeEfficiencies_%d", i);
    relativeEfficiencies = new TH1F(histoName, histoName, nBins, 0., nBins);
    relativeEfficiencies -> SetBinContent( 1, 1.);
    for(int bin = 2; bin <= nBins; ++ bin)
    {
      if(events -> GetBinContent(bin -1) == 0) relativeEfficiencies -> SetBinContent( bin, 0. );
      relativeEfficiencies -> SetBinContent( bin, 1. * events -> GetBinContent(bin) / events -> GetBinContent(bin-1) );
    }    
    
    relativeEfficiencies -> SetLineColor(colors[i]);
    relativeEfficiencies -> SetLineWidth(2);
    relativeEfficiencies -> Draw("same");
    legend -> AddEntry(relativeEfficiencies, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
    
    
  }
  
}






//------------------------------------
// draw number of events lumi-rescaled
//------------------------------------

void drawEventsScaled(double lumi = 1000.)
{
  std::ifstream inFile((path+"listAll.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  TH1F* eventsScaled = static_cast<TH1F*>(inRootFile -> Get("eventsScaled"));
  
  
  
  TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", "events");
  c1 -> SetLogy();
  c1 -> SetGridy();
  eventsScaled -> SetLineColor(kBlack);
  eventsScaled -> SetLineWidth(3);
  eventsScaled -> Scale(lumi);
  char title[50];
  sprintf(title, "events / %d pb^{-1}", lumi);
  eventsScaled -> GetYaxis() -> SetTitle(title);
  eventsScaled -> GetXaxis() -> SetLabelSize(0.04);
  eventsScaled -> GetYaxis() -> SetLabelSize(0.04);
  eventsScaled -> GetYaxis() -> SetTitleOffset(1.10);
  eventsScaled -> GetYaxis() -> SetRangeUser(1., 10000000000000.);
  eventsScaled -> Draw();
  legend -> AddEntry(eventsScaled, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
  
  for(int i = 1; i < files.size(); ++i)
  {
    inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
    eventsScaled = static_cast<TH1F*>(inRootFile -> Get("eventsScaled"));
    
    eventsScaled -> SetLineColor(colors[i]);
    eventsScaled -> SetLineWidth(2);
    eventsScaled -> Scale(lumi);
    eventsScaled -> Draw("same");
    legend -> AddEntry(eventsScaled, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
    
    
  }
  
}






//------------------------------------------------
// draw number of events over signal lumi-rescaled
//------------------------------------------------

void drawEventsOverSignalScaled()
{
  std::ifstream inFile((path+"listAll.txt").c_str());
 
  std::vector<std::string> files;
  while(!inFile.eof())
  {
    std::string buffer;
    inFile >> buffer;
    files.push_back(buffer);
  }  
  
  
  
  TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  TH1F* eventsSignalScaled = static_cast<TH1F*>(inRootFile -> Get("eventsScaled"));
  int nBins = eventsSignalScaled -> GetNbinsX();
  
  TH1F* eventsOverSignalScaled=  new TH1F("eventsOverSignalScaled", "eventsOverSignalScaled", nBins, 0., nBins);
  for(int bin = 1; bin <= nBins; ++ bin)
    eventsOverSignalScaled -> SetBinContent( bin, 1. * eventsSignalScaled -> GetBinContent(bin) / eventsSignalScaled -> GetBinContent(bin) );
  
  
  
  TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
  legend -> SetFillColor(kWhite);
  
  TCanvas* c1 = new TCanvas("c1", "events over signal");
  c1 -> SetLogy();
  c1 -> SetGridy();
  eventsOverSignalScaled -> SetLineColor(kBlack);
  eventsOverSignalScaled -> SetLineWidth(3);
  char title[50];
  sprintf(title, "eventsOverSignal");
  eventsOverSignalScaled -> GetYaxis() -> SetTitle(title);
  eventsOverSignalScaled -> GetXaxis() -> SetLabelSize(0.04);
  eventsOverSignalScaled -> GetYaxis() -> SetLabelSize(0.04);
  eventsOverSignalScaled -> GetYaxis() -> SetTitleOffset(1.10);
  eventsOverSignalScaled -> GetYaxis() -> SetRangeUser(0.1, 100000.);
  eventsOverSignalScaled -> Draw();
  legend -> AddEntry(eventsOverSignalScaled, (files.at(0)).c_str(), "L");
  legend -> Draw("same");
  
  
  
  for(int i = 1; i < files.size(); ++i)
  {
    inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
    TH1F* eventsScaled = static_cast<TH1F*>(inRootFile -> Get("eventsScaled"));

    char histoName[50];
    sprintf(histoName, "eventsOverSignalScaled_%d", i);
    TH1F* eventsOverSignalScaled=  new TH1F(histoName, histoName, nBins, 0., nBins);

    for(int bin = 1; bin <= nBins; ++ bin)
      eventsOverSignalScaled -> SetBinContent( bin, 1. * eventsScaled -> GetBinContent(bin) / eventsSignalScaled -> GetBinContent(bin) );
    
    eventsOverSignalScaled -> SetLineColor(colors[i]);
    eventsOverSignalScaled -> SetLineWidth(2);
    eventsOverSignalScaled -> Draw("same");
    legend -> AddEntry(eventsOverSignalScaled, (files.at(i)).c_str(), "L");
    legend -> Draw("same");
    
    
  }
  
}






// //-------------------------------------
// // draw recoJ_central_n
// //-------------------------------------

// void drawRecoJ_central_n(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJ_central_n", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJ_central_n");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJ_tag_absEta
// //-------------------------------------

// void drawRecoJ_tag_absEta(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJ_tag_absEta", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJ_tag_absEta");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_tag_mass
// //-------------------------------------

// void drawRecoJJ_tag_mass(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_tag_mass", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_tag_mass");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_tag_Deta
// //-------------------------------------

// void drawRecoJJ_tag_Deta(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_tag_Deta", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_tag_Deta");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
  
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_tag_DR
// //-------------------------------------

// void drawRecoJJ_tag_DR(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_tag_DR", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_tag_DR");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_W_mass
// //-------------------------------------

// void drawRecoJJ_W_mass(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_W_mass", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_W_mass");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_W_pt
// //-------------------------------------

// void drawRecoJJ_W_pt(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_W_pt", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_W_pt");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_W_absEta
// //-------------------------------------

// void drawRecoJJ_W_absEta(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_W_absEta", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_W_absEta");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_W_Deta
// //-------------------------------------

// void drawRecoJJ_W_Deta(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_W_Deta", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_W_Deta");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_W_Dphi
// //-------------------------------------

// void drawRecoJJ_W_Dphi(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_W_Dphi", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_W_Dphi");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoJJ_W_etRatio
// //-------------------------------------

// void drawRecoJJ_W_etRatio(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoJJ_W_etRatio", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoJJ_W_etRatio");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
			         
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }







// //------------------------------------
// // draw number of events lumi-rescaled
// //------------------------------------

// void drawEventsScaled(double lumi = 1000.)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
//   TH1F* eventsScaled = static_cast<TH1F*>(inRootFile -> Get("eventsScaled"));
  
  
  
//   TLegend* legend = new TLegend(0.15, 0.20, 0.40, 0.60);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "events");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   eventsScaled -> SetLineColor(kBlack);
//   eventsScaled -> SetLineWidth(3);
//   eventsScaled -> Scale(lumi);
//   char title[50];
//   sprintf(title, "events / %d pb^{-1}", lumi);
//   eventsScaled -> GetYaxis() -> SetTitle(title);
//   eventsScaled -> GetXaxis() -> SetLabelSize(0.04);
//   eventsScaled -> GetYaxis() -> SetLabelSize(0.04);
//   eventsScaled -> GetYaxis() -> SetTitleOffset(1.10);
//   eventsScaled -> GetYaxis() -> SetRangeUser(1., 10000000000000.);
//   eventsScaled -> Draw();
//   legend -> AddEntry(eventsScaled, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
  
//   for(int i = 1; i < files.size(); ++i)
//   {
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     eventsScaled = static_cast<TH1F*>(inRootFile -> Get("eventsScaled"));
    
//     eventsScaled -> SetLineColor(colors[i]);
//     eventsScaled -> SetLineWidth(2);
//     eventsScaled -> Scale(lumi);
//     eventsScaled -> Draw("same");
//     legend -> AddEntry(eventsScaled, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
    
    
//   }
  
// }













// //-------------------------------------
// // draw recoL_W_pt
// //-------------------------------------

// void drawRecoL_W_pt(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoL_W_pt", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoL_W_pt");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(5);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(5);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }











// //-------------------------------------
// // draw recoLJJ_DR
// //-------------------------------------

// void drawRecoLJJ_DR(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoLJJ_DR", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoLJJ_DR");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(50);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(50);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoLJJ_Deta
// //-------------------------------------

// void drawRecoLJJ_Deta(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoLJJ_Deta", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoLJJ_Deta");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(50);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(50);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoLJJ_Dphi
// //-------------------------------------

// void drawRecoLJJ_Dphi(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoLJJ_Dphi", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoLJJ_Dphi");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(50);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(50);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }






// //-------------------------------------
// // draw recoLJJ_mass
// //-------------------------------------

// void drawRecoLJJ_mass(int step = 1)
// {
//   std::ifstream inFile((path+"listAll.txt").c_str());
 
//   std::vector<std::string> files;
//   while(!inFile.eof())
//   {
//     std::string buffer;
//     inFile >> buffer;
//     files.push_back(buffer);
//   }  
  
  
  
//   std::cout << files.at(0) << std::endl;  
//   TFile* inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(0)+".root").c_str(), "READ");
  
  
  
//   char histoName[50];
//   sprintf(histoName, "RecoHistograms_step%d/%d_recoLJJ_mass", step, step);
//   TH1F* histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
  
  
//   TLegend* legend = new TLegend(0.65, 0.50, 0.90, 0.90);
//   legend -> SetFillColor(kWhite);
  
//   TCanvas* c1 = new TCanvas("c1", "recoLJJ_mass");
//   c1 -> SetLogy();
//   c1 -> SetGridy();
//   histo -> SetLineColor(kBlack);
//   histo -> SetLineWidth(3);
//   histo -> GetYaxis() -> SetTitle("event fraction");
//   histo -> GetXaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetLabelSize(0.04);
//   histo -> GetYaxis() -> SetTitleOffset(1.10);
//   histo -> GetYaxis() -> SetRangeUser(0.0001, 1.);
//   histo -> Rebin(50);
//   histo -> Draw();
//   legend -> AddEntry(histo, (files.at(0)).c_str(), "L");
//   legend -> Draw("same");
  
  
 
//   for(int i = 1; i < files.size(); ++i)
//   {
//     std::cout << files.at(i) << std::endl;
//     inRootFile = new TFile((path+"VBFSimpleAnalysis_"+files.at(i)+".root").c_str(), "READ");
//     histo = static_cast<TH1F*>(inRootFile -> Get(histoName));
    
//     histo -> SetLineColor(colors[i]);
//     histo -> SetLineWidth(2);
//     histo -> Rebin(50);
//     histo -> Draw("same");
//     legend -> AddEntry(histo, (files.at(i)).c_str(), "L");
//     legend -> Draw("same");
//     }
// }

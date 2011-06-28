/*
ECALAlignmentFramework
*/

#include "ECALAlignmentFramework.h"


//  ------------------------------------------------------------

int parseConfigFile (const TString& config)
{
 std::cout << ">>> Parsing " << config << " file" << std::endl ;
 
 if (gConfigParser) return 1 ;
 gConfigParser = new ConfigParser();
 
 if( !(gConfigParser -> init(config)) )
 {
  std::cout << ">>> parseConfigFile::Could not open configuration file "
	       << config << std::endl;
	       return -1;
 }
 
 gConfigParser -> print();
 
 return 0 ;
}

//  ------------------------------------------------------------



double deltaPhi(const double& phi1, const double& phi2)
{ 
 double deltaphi = fabs(phi1 - phi2);
 if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
 if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
 return deltaphi;
}

//  ------------------------------------------------------------

double deltaEta(const double& eta1, const double& eta2)
{ 
 double deltaeta = fabs(eta1 - eta2);
 return deltaeta;
}








///==== Plot tools ====
///
///    _ \   |         |   
///   |   |  |   _ \   __| 
///   ___/   |  (   |  |   
///  _|     _| \___/  \__| 
///                        






///==== GetTrendInfo ====
///==== Transform TH1 with "trace" information to TH1 ====

TH1F* GetTrendInfo(TH1F* hTrend, double min, double max){
 int nbin = hTrend->GetNbinsX();
 //  double maxX = hTrend->GetYaxis()->GetXmax();
 //  double minX = hTrend->GetYaxis()->GetXmin();
 //  std::cout << "minX = " << minX << " ::: " << maxX << std::endl;
 //  std::cout << "min  = " << min  << " ::: " << max  << std::endl;
 //  maxX = std::min(max,maxX);
 //  minX = std::max(min,minX);
 //  std::cout << "minX = " << minX << " ::: " << maxX << std::endl;
 double maxX = max;
 double minX = min;
 
 std::string name = hTrend->GetName(); 
 std::string nameNew;
 nameNew = name + "_Info";
 
 TH1F* hTrendInfo = new TH1F (nameNew.c_str(),nameNew.c_str(),nbin,minX,maxX);
 for (int iBin = 0; iBin<nbin; iBin++){
  hTrendInfo->Fill(hTrend->GetBinContent(iBin+1));
 }
 
 hTrendInfo->SetLineColor(hTrend->GetLineColor());
 hTrendInfo->SetLineWidth(2);
 hTrendInfo->SetMarkerColor(hTrend->GetMarkerColor());
 hTrendInfo->SetMarkerStyle(20);
 hTrendInfo->SetMarkerSize(1);
 hTrendInfo->SetFillStyle(1001);
 hTrendInfo->GetXaxis()->SetTitle(hTrend->GetYaxis()->GetTitle()); 
 return hTrendInfo;
}



TH1F* PullPlot(TH1F* hDATA, TH1F* hMC){
 int nbin = hDATA->GetNbinsX();
 double max = hDATA->GetXaxis()->GetXmax();
 double min = hDATA->GetXaxis()->GetXmin();
 std::string name1 = hDATA->GetName();
 std::string name2 = hMC->GetName(); 
 std::string nameNew = name1 + name2; 
 TH1F* hPool = new TH1F (nameNew.c_str(),nameNew.c_str(),nbin,min,max);
 for (int iBin = 0; iBin<nbin; iBin++){
  double A = hDATA->GetBinContent(iBin);
  double B = hMC->GetBinContent(iBin);
  if (A+B != 0) {
   hPool->SetBinContent(iBin,(A-B)/(A+B)*2.);
   //    hPool->SetBinError(iBin,4. * A / (A+B) / (A+B) * sqrt(A+B));
   hPool->SetBinError(iBin,4. * B / (A+B) / (A+B) * sqrt(A));
  }
 }
 hPool->SetLineColor(kRed);
 hPool->SetLineWidth(2);
 hPool->SetMarkerColor(kRed);
 hPool->SetMarkerStyle(20);
 hPool->SetMarkerSize(1);
 hPool->GetXaxis()->SetTitle(hMC->GetXaxis()->GetTitle()); 
 hPool->GetYaxis()->SetTitle("2(DATA-MC)/(DATA+MC)"); 
 hPool->GetYaxis()->SetRangeUser(-2.,2.); 
 return hPool;
}


///==== Pull Plot: drawing utility ====
void PullPlot(TCanvas* canvas, TH1* hDATA, TH1* hMC){
 canvas->Divide(1,2);
 int nbin = hDATA->GetNbinsX();
 double max = hDATA->GetXaxis()->GetXmax();
 double min = hDATA->GetXaxis()->GetXmin();
 std::string name1 = hDATA->GetName();
 std::string name2 = hMC->GetName();
 
 std::string nameNew = name1 + name2;
 
 TH1F* hPool = new TH1F (nameNew.c_str(),nameNew.c_str(),nbin,min,max);
 for (int iBin = 0; iBin<nbin; iBin++){
  double A = hDATA->GetBinContent(iBin);
  double B = hMC->GetBinContent(iBin);
  if (A+B != 0) {
   hPool->SetBinContent(iBin,(A-B)/(A+B)*2.);
   hPool->SetBinError(iBin,4. * B / (A+B) / (A+B) * sqrt(A));
  }
 }
 
 double maxY_DATA = hDATA->GetMaximum();
 double maxY_MC = hMC->GetMaximum();
 
 double minY_DATA = hDATA->GetMinimum();
 double minY_MC = hMC->GetMinimum();
 
 canvas->cd(1); 
 if (maxY_MC > maxY_DATA) {
  hMC->Draw();
  hDATA->Draw("EsameP");
 }
 else {
  hDATA->Draw("EP");
  hMC->Draw("same");  
 }
 hDATA->GetYaxis()->SetRangeUser(std::min(minY_MC,minY_DATA),std::max(maxY_MC,maxY_DATA) * 1.1);
 hMC  ->GetYaxis()->SetRangeUser(std::min(minY_MC,minY_DATA),std::max(maxY_MC,maxY_DATA) * 1.1);
 
 gPad->SetGrid();
 
 canvas->cd(2); 
 hPool->SetLineColor(kRed);
 hPool->SetLineWidth(2);
 hPool->SetMarkerColor(kRed);
 hPool->SetMarkerStyle(20);
 hPool->SetMarkerSize(1);
 hPool->Draw("EP");
 hPool->GetXaxis()->SetTitle(hMC->GetXaxis()->GetTitle()); 
 hPool->GetYaxis()->SetTitle("2(DATA-MC)/(DATA+MC)"); 
 hPool->Draw("EP");
 gPad->SetGrid();
}


void PullPlot(TCanvas* canvas, TH1* hDATA, THStack* hsMC){
 canvas->Divide(1,2);
 int nbin = hDATA->GetNbinsX();
 double max = hDATA->GetXaxis()->GetXmax();
 double min = hDATA->GetXaxis()->GetXmin();
 std::string name1 = hDATA->GetName();
 std::string name2 = hsMC->GetName();
 
 std::string nameNew = name1 + name2;
 
 TH1F* hPool = new TH1F (nameNew.c_str(),nameNew.c_str(),nbin,min,max);
 for (int iBin = 0; iBin<nbin; iBin++){
  double A = hDATA->GetBinContent(iBin);
  double B = ((TH1*)(hsMC->GetStack()->Last()))->GetBinContent(iBin);
  if (A+B != 0) {
   hPool->SetBinContent(iBin,(A-B)/(A+B)*2.);
   hPool->SetBinError(iBin,4. * B / (A+B) / (A+B) * sqrt(A));
  }
 }
 
 double maxY_DATA = hDATA->GetMaximum();
 double maxY_MC = ((TH1*)(hsMC->GetStack()->Last()))->GetMaximum();
 
 double minY_DATA = hDATA->GetMinimum();
 double minY_MC = ((TH1*)(hsMC->GetStack()->Last()))->GetMinimum();
 
 canvas->cd(1); 
 hDATA->Draw("EP");
 DrawStack(hsMC);
 hDATA->Draw("EsameP");
 gPad->SetGrid();
 
 canvas->cd(2); 
 hPool->SetLineColor(kRed);
 hPool->SetLineWidth(2);
 hPool->SetMarkerColor(kRed);
 hPool->SetMarkerStyle(20);
 hPool->SetMarkerSize(1);
 hPool->Draw("EP");
 hPool->GetXaxis()->SetTitle(hDATA->GetXaxis()->GetTitle()); 
 hPool->GetYaxis()->SetTitle("2(DATA-MC)/(DATA+MC)"); 
 hPool->Draw("EP");
 gPad->SetGrid();
}

///==== Draw Stack ====
///---- if error == 1 than consider systematic error
void DrawStack(THStack* hs, int error, double syst){ 
 if (error == 1) {
  DrawStackError(hs, syst);
 }
 else {
  TObjArray* histos = hs->GetStack () ;
  Int_t number = histos->GetEntries();
  TH1F* last = (TH1F*) histos->At (number-1) ;
  last->Draw ("hist") ;
  for (int i = number-2 ; i >=0 ; --i) 
  {
   TH1F * histo = (TH1F*) histos->At (i) ;
   Style_t origStyle = histo->GetFillStyle ();
   Color_t origColor = histo->GetFillColor ();
   TH1F* dummy = (TH1F*) histo->Clone () ;
   dummy->SetFillStyle (1001) ; 
   dummy->SetFillColor (10) ;        
   dummy->Draw ("same hist") ;
   histo->Draw ("same hist") ;
  }
 }
}


///==== Draw Stack ====
void DrawStackError(THStack* hs, double syst){ 
 TObjArray* histos = hs->GetStack () ;
 Int_t number = histos->GetEntries();
 TH1F* last = (TH1F*) histos->At (number-1) ;
 last->DrawClone ("hist") ;
 for (int i = number-2 ; i >=0 ; --i) 
 {
  TH1F * histo = (TH1F*) histos->At (i) ;
  Style_t origStyle = histo->GetFillStyle ();
  Color_t origColor = histo->GetFillColor ();
  TH1F* dummy = (TH1F*) histo->Clone () ;
  dummy->SetFillStyle (1001) ; 
  dummy->SetFillColor (10) ;        
  dummy->Draw ("same hist") ;
  histo->Draw ("same hist") ;
 }
 Style_t origStyleLast = last->GetFillStyle ();
 Color_t origColorLast = last->GetFillColor ();
 last->SetFillStyle(3335);
 last->SetFillColor(kBlack);
 last->SetMarkerSize(0);
 
 std::vector <double> vErr ;
 for (int iBin = 0 ; iBin < last->GetNbinsX(); iBin++) {
  double additionalError = last->GetBinContent(iBin+1) * syst;
  vErr.push_back(last->GetBinError(iBin+1));
  last->SetBinError(iBin+1,sqrt(additionalError*additionalError + last->GetBinError(iBin+1) * last->GetBinError(iBin+1)) );
 }
 last->DrawClone ("sameE2") ;
 //---- restore hist ----
 last->SetFillStyle(origStyleLast);
 last->SetFillColor(origColorLast);
 for (int iBin = 0 ; iBin < last->GetNbinsX(); iBin++) {
  last->SetBinError(iBin+1, vErr.at(iBin));
 }
}

///==== Add systrematic error ====
void AddError(THStack* hs, double syst){ 
 TObjArray* histos = hs->GetStack () ;
 Int_t number = histos->GetEntries();
 TH1F* last = (TH1F*) histos->At (number-1) ;
 for (int iBin = 0 ; iBin < last->GetNbinsX(); iBin++) {
  double additionalError = last->GetBinContent(iBin+1) * syst;
  last->SetBinError(iBin+1,sqrt(additionalError*additionalError + last->GetBinError(iBin+1) * last->GetBinError(iBin+1)) );
 }
}











///==== read list of systematics file ====
void ReadFileSystematics(std::string CutSystematicFile, std::vector< std::pair< int, std::pair<std::string, double> > >& listSystematics){
 std::ifstream inFile(CutSystematicFile.c_str());
 std::string buffer; 
 std::string tempVar;
 double tempSyst;
 int tempKind;
 while(!inFile.eof()){
  getline(inFile,buffer);
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer.at(0) != '#'){     
    std::stringstream line( buffer );      
    line >> tempVar; 
    line >> tempSyst;
    line >> tempKind;
    std::cout << " Syst = " << tempVar << " ~ " << tempSyst << " ~ " << tempKind << std::endl;
    std::pair<std::string, double> tempPair(tempVar,tempSyst);
    std::pair< int, std::pair<std::string, double> > tempPairPair(tempKind, tempPair);
    listSystematics.push_back (tempPairPair);
   } 
  }
 }
}

///==== std::string replace ====
void repl(std::string& s, const std::string& from, const std::string& to){
 std::string::size_type cnt(std::string::npos);
 if(from != to && !from.empty())
 {
  std::string::size_type pos1(0);
  std::string::size_type pos2(0);
  const std::string::size_type from_len(from.size());
  const std::string::size_type to_len(to.size());
  cnt = 0;
  
  while((pos1 = s.find(from, pos2)) != std::string::npos)
  {
   s.replace(pos1, from_len, to);
   pos2 = pos1 + to_len;
   ++cnt;
  }
 }
}

///==== modify list of cuts to include systematics ====
void ModifyCut(std::vector <std::string> & vCut, const std::vector< std::pair< int, std::pair<std::string, double> > >& listSystematics){
 std::cout << std::endl;
 for (int iSyst = 0; iSyst < listSystematics.size(); iSyst++) {
  std::string var = listSystematics.at(iSyst).second.first;
  double syst = listSystematics.at(iSyst).second.second;
  int kind = listSystematics.at(iSyst).first;
  char systChar[128];
  sprintf(systChar,"%0.2f",syst); 
  std::string newvar = "(";
  if (kind == 1){ //---- dilation
   newvar += var;
   newvar += "*(1+(";
   newvar += systChar;
   newvar += ")))";
  }
  else { //---- shift
   newvar += var;
   newvar += "+(";
   newvar += syst;
   newvar += "))";
  }
  std::cout << " newvar = " << newvar << std::endl;
  std::cout << "    var = " <<    var << std::endl;
  std::cout << "   syst = " <<   syst << std::endl;
  for (int iCut = 0; iCut < vCut.size(); iCut++) {
   repl(vCut.at(iCut),var,newvar);
  }
 } 
 for (int iCut = 0; iCut < vCut.size(); iCut++) {
  std::cout << " newCut = " << vCut.at(iCut) << std::endl;
 }
 std::cout << std::endl;
}


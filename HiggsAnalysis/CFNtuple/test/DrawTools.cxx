TGraphErrors* XBinCenterCorrectedHisto(const TH1F& thLargeBin, const TH1F& thSlimBin){
 int nBin = thLargeBin.GetNbinsX();

 TGraphErrors* newGraph = new TGraphErrors();
 int iPoint = 0;
 
 int nBinSlim = thSlimBin.GetNbinsX();
 int ratioNBin = nBinSlim / nBin;
 
 for (int iBin = 0; iBin < nBin; iBin++){
  double Delta = (thLargeBin.GetBinLowEdge(iBin+2) - thLargeBin.GetBinLowEdge(iBin+1));
  double LittleDelta = Delta / ratioNBin;
  double Y = thLargeBin.GetBinContent(iBin+1) / Delta;
  double X = 0;
  for (int iInside = 0; iInside < ratioNBin; iInside++){
   if (thSlimBin.GetBinContent(iBin*ratioNBin + iInside + 1) / LittleDelta) {
    X = thSlimBin.GetBinCenter(iBin*ratioNBin + iInside + 1);
    break;
   }
  }
  newGraph->SetPoint(iPoint,X,Y);
  newGraph->SetPointError(iPoint,0,thLargeBin.GetBinError(iBin+1));
  iPoint++;
 }
 return newGraph;
}

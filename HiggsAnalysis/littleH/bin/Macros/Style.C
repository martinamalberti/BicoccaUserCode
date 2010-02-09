{
  gROOT->Reset();

  // style:
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  //For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(800); //600
  //Height of canvas
  tdrStyle->SetCanvasDefW(800);//800 //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  //For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // For the histo:
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(2);
  tdrStyle->SetEndErrorSize(2);
//  tdrStyle->SetErrorMarker(20);
  //tdrStyle->SetErrorX(0.);
  tdrStyle->SetMarkerStyle(8);


  // For the statistics box:
  tdrStyle->SetOptFile(0);
//  tdrStyle->SetOptStat(1);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
//   tdrStyle->SetStatBorderSize(0);
  tdrStyle->SetStatH(.1);
  tdrStyle->SetStatW(.15);

  //  tdrStyle->SetStatX(.9);
  // tdrStyle->SetStatY(.9);

  // For the Global title:
  tdrStyle->SetOptTitle(0);
//   tdrStyle->SetOptTitle(1);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);

  // For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  //  tdrStyle->SetTitleSize(0.06, "XYZ");
  tdrStyle->SetTitleSize(0.04, "XYZ");
  tdrStyle->SetTitleXOffset(0.9);
  // tdrStyle->SetTitleYOffset(1.25);
  //tdrStyle->SetTitleXOffset(0.5);
  tdrStyle->SetTitleYOffset(1.0);

  // For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");

  //tdrStyle->SetLabelSize(0.05, "XYZ");
  tdrStyle->SetLabelSize(0.04, "XYZ");

  // For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(0);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);
  // // Postscript options:
  //   //tdrStyle->SetPaperSize(20.,20.);

  // CC style  
  tdrStyle->SetTitleXOffset(0.8);
//   tdrStyle->SetTitleYOffset(0.8);
  //Inv Mass Presel
    tdrStyle->SetTitleYOffset(1.0);

  tdrStyle->SetLabelOffset(0.005, "XYZ");
  tdrStyle->SetTitleSize(0.07, "XYZ");
  tdrStyle->SetTitleFont(22,"X");
  tdrStyle->SetTitleFont(22,"Y");
/*  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadTopMargin(0.23);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.21);*/
/*  //PlotPt4Lep=====================
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);*/
/*  //PlotPrePreSelInvMass=====================
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.10);*/
/*  //PlotPrePreSelEff=====================
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadTopMargin(0.15);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.15);*/
  //PlotInvMass=====================
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);
/*  //PlotEffvsEff=====================
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);*/
/*  //JetMET=====================
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);*/
//   //Met2dEff=====================
//   tdrStyle->SetPadBottomMargin(0.15);
//   tdrStyle->SetPadTopMargin(0.05);
//   tdrStyle->SetPadLeftMargin(0.15);
//   tdrStyle->SetPadRightMargin(0.15);
//   //Dalitz+PtVsMET=====================
//   tdrStyle->SetPadBottomMargin(0.15);
//   tdrStyle->SetPadTopMargin(0.10);
//   tdrStyle->SetPadLeftMargin(0.15);
//   tdrStyle->SetPadRightMargin(0.15);

/*  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.15);*/
//  tdrStyle->SetHistLineWidth(3);
  tdrStyle->SetHistLineWidth(2);
  tdrStyle->SetLineStyleString(5,"20 12 4 12");
  tdrStyle->SetLineStyleString(6,"20 12 4 12 4 12 4 12");
  tdrStyle->SetLineStyleString(7,"20 20");
  tdrStyle->SetLineStyleString(8,"20 12 4 12 4 12");
  tdrStyle->SetLineStyleString(9,"80 20");

  tdrStyle->cd(); 

  gROOT->ForceStyle();
}  
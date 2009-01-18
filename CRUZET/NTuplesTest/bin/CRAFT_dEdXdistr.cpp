#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CRUZET/NTuplesTest/test/Langaus.cc"
#include "CRUZET/Calibration/interface/CRUtils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "CLHEP/Units/PhysicalConstants.h"

#define PHI_MIN -3.1416
#define PHI_MAX +3.1416
#define PHI_BIN 360

#define ETA_MIN -1.5
#define ETA_MAX +1.5
#define ETA_BIN 170

#define IPHI_MIN 0.
#define IPHI_MAX +361
#define IPHI_BIN 361

#define IETA_MIN -80.
#define IETA_MAX +80.
#define IETA_BIN 361

#define X_BIN 500
#define X_MIN -800
#define X_MAX +800

#define Y_BIN 500
#define Y_MIN -800
#define Y_MAX +800

#define P_BIN 50
#define P_MIN 0
#define P_MAX 3



void BinLogX (TH1F& h) ;
void BinLogX (TProfile& h) ;
void BinLogX (TH2F& h) ;
void LangausFit (TF1** langaus, TH1D* histo) ;



int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  bool muonLegUpOK = subPSetSelections.getParameter<bool> ("muonLegUpOK") ;
  bool muonLegDownOK = subPSetSelections.getParameter<bool> ("muonLegDownOK") ;
  double muond0MAX = subPSetSelections.getParameter<double> ("muond0MAX") ;
  double muondzMAX = subPSetSelections.getParameter<double> ("muondzMAX") ;
  double muonPMAX = subPSetSelections.getParameter<double> ("muonPMAX") ;
  double muonPMIN = subPSetSelections.getParameter<double> ("muonPMIN") ;
  double muonEoverPMAX = subPSetSelections.getParameter<double> ("muonEoverPMAX") ;
  double muonTkLengthInEcalMIN = subPSetSelections.getParameter<double> ("muonTkLengthInEcalMIN") ;
  double muonTkLengthInEcalMAX = subPSetSelections.getParameter<double> ("muonTkLengthInEcalMAX") ;
  
  edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  
  
  
  // load ntuple
  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;
  
  
  
  // input files
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ; ++listIt)
  {
    std::cout << *listIt << " " << std::endl ;
    chain->Add (listIt->c_str ()) ;
  }
  
  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;
  
  
  
  // Output file
  std::string outputRootName = "CRAFT_dEdXdistr.root" ;
  TFile outputRootFile (outputRootName.c_str (), "recreate") ;
  outputRootFile.cd () ;
  
  
  
  // output distributions
  TH2F BetheBloch ("BetheBloch", "BetheBloch", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch) ;
  std::vector<TH1D*> BetheBloch_pBin ;
  TProfile BetheBloch_profile ("BetheBloch_profile", "BetheBloch_profile", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile) ;
  TGraphAsymmErrors BetheBloch_graph ;
  TGraphAsymmErrors BetheBloch_graph_mean ;
  
  TH2F BetheBloch_curved ("BetheBloch_curved", "BetheBloch_curved", P_BIN, P_MIN, P_MAX, 5000, 0., 1000.);
  BinLogX (BetheBloch_curved) ;
  std::vector<TH1D*> BetheBloch_pBin_curved ;
  TProfile BetheBloch_profile_curved ("BetheBloch_profile_curved", "BetheBloch_profile_curved", P_BIN, P_MIN, P_MAX);
  BinLogX (BetheBloch_profile_curved) ;
  TGraphAsymmErrors BetheBloch_graph_curved ;
  TGraphAsymmErrors BetheBloch_graph_mean_curved ;
  
  TH2F muonTkLength_straight_curved_pT ("muonTkLength_straight_curved_pT", "muonTkLength_straight_curved_pT", P_BIN, P_MIN, P_MAX, 600, -0.2, 0.2) ;
  BinLogX (muonTkLength_straight_curved_pT) ;
  
  
  std::map<int,float> pMap ;
  pMap. clear() ;
  std::map<int,float> pMapCurved ;
  pMapCurved. clear() ;
  std::map<int, float> BetheBloch_mean ;
  BetheBloch_mean.clear () ;
  std::map<int, float> BetheBloch_mean_curved ;
  BetheBloch_mean_curved.clear () ;
  std::map<int, float> muonTkLengthMap_straight_curved_RMS ;
  muonTkLengthMap_straight_curved_RMS.clear () ;


//  
//  TH2F BetheBloch_E5x5 ("BetheBloch_E5x5", "BetheBloch_E5x5", P_BIN, P_MIN, P_MAX, 1000, 0., 100.);
//  BinLogX (BetheBloch_E5x5) ;
//  TProfile BetheBloch_profile_E5x5 ("BetheBloch_profile_E5x5", "BetheBloch_profile_E5x5", P_BIN, P_MIN, P_MAX);
//  BinLogX (BetheBloch_profile_E5x5) ;
//  TGraphAsymmErrors BetheBloch_graph_E5x5 ;
//  
//  TProfile BetheBloch_energy ("BetheBloch_energy", "BetheBloch_energy", P_BIN, P_MIN, P_MAX);
//  BinLogX(BetheBloch_energy);
//  TProfile BetheBloch_corrected ("BetheBloch_corrected", "BetheBloch_corrected", P_BIN, P_MIN, P_MAX);
//  BinLogX(BetheBloch_corrected);
//  TGraphErrors BB_energy ;
//  TH1F numAssociations ("numAssociations", "numAssociations", 20, 0., 20.);
//  
//  TH1F Radius_d ("Radius_d", "Radius_d", 500, 0., 1000.);
//  TH1F Curved ("Curved", "Curved", 500, -10., 10.);
//  TH1F Straight ("Straight", "Straight", 500, 0., 1000.);
//  
//  TH2F DEta_DPhi ("DEta_DPhi", "DEta_DPhi", 400, -2.5, 2.5, 400, -3.16, 3.16);
//  TH2F DPhi_Pt ("DPhi_Pt", "DPhi_Pt", 1000, 0., 1000., 400, -3.16, 3.16);
//  TH2F DR_Pt ("DR_Pt", "DR_Pt", 1000, 0., 1000., 400, -5., 5.);
//  
//  TH2F DPhi_leg ("DPhi_leg", "DPhi_leg", 100, -10., 10., 400, -3.16, 3.16);
  
  
  
  
  
  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100000) == 0)
      std::cout << "Reading entry " << entry << std::endl;
    chain->GetEntry (entry) ;
    // if (entry == 100000) break ;
    
    // association MU-SC
    std::vector<ect::association> associations ;
    ect::fillAssocVector (associations, treeVars) ;
    ect::selectOnDR (associations, treeVars, 0.1) ;
    
    // numAssociations.Fill(associations.size()) ;
    
    
    
    //loop on associations vector
    for (unsigned int i = 0 ; i < associations.size () ; ++i)
    {
      int MUindex = associations.at (i).first  ;
      int SCindex = associations.at (i).second ;
      
      
      
      int muonLeg = treeVars.muonLeg[MUindex];
      float muonP = 0.;
      float muonPt = 0.;
      float muond0 = treeVars.muond0[MUindex];
      float muondz = treeVars.muondz[MUindex];
      float muonPhi = 0.;
      float muonEta = 0.;
      float muonTkLengthInEcal = treeVars.muonTkLengthInEcalDetail[MUindex] ;
      float muonTkLengthInEcalCurved = treeVars.muonTkLengthInEcalDetailCurved[MUindex] ;
      float superClusterRawEnergy = treeVars.superClusterRawEnergy[SCindex]/4.*0.9 ;
      int pBin = -1;
      int pBinCurved = -1;
      
      if (muonLeg == 1)
      {
        muonP = treeVars.muonInnTkInnerHitP[MUindex];
        muonPt = treeVars.muonInnTkInnerHitPt[MUindex];
        muonPhi = treeVars.muonInnTkInnerHitPhi[MUindex];
        muonEta = treeVars.muonInnTkInnerHitEta[MUindex];
      }
      
      else if (muonLeg == -1)
      {
        muonP = treeVars.muonInnTkOuterHitP[MUindex];
        muonPt = treeVars.muonInnTkOuterHitPt[MUindex];
        muonPhi = treeVars.muonInnTkOuterHitPhi[MUindex];
        muonEta = treeVars.muonInnTkOuterHitEta[MUindex];
      }
      
      else continue ;
      
      float Radius = muonPt / 0.3 /3.8 * 100.;     //pT (GeV) = 0.3 B(T) R(m)
      float correction =  muonTkLengthInEcal*muonTkLengthInEcal /Radius /Radius / 6.;
      float muonEoverP = superClusterRawEnergy/muonP ;
      
      
      // Cut event
      if ( (fabs(muond0) > muond0MAX) || (fabs(muondz) > muondzMAX) ) continue ;
      if ( (muonP < muonPMIN) || (muonP > muonPMAX) ) continue ;
      //if ( (muonLegUpOK == true && muonLeg != 1) ||
      //     (muonLegDownOK == true && muonLeg != -1) ) continue ;
      if (muonEoverP > muonEoverPMAX) continue ; 
      if ( (muonTkLengthInEcal < muonTkLengthInEcalMIN) ||
           (muonTkLengthInEcal > muonTkLengthInEcalMAX) ) continue ;
      
      
      
      // Fill distribution
      if(muonTkLengthInEcal > 0.)
      {
        float dEdX = superClusterRawEnergy / muonTkLengthInEcal / 8.28 * 1000. ;
        BetheBloch.Fill (muonP, dEdX) ;
        pBin = BetheBloch_profile.Fill (muonP, dEdX) ;
        pMap[pBin] += muonP;
        
        if (dEdX > 500.)
        {
          std::cout << "   muonP = " << muonP
                    << "   SC = " << superClusterRawEnergy
                    << "   LENGTH = " << muonTkLengthInEcal
                    << "   dEdX = " << dEdX
                    << std::endl ;
        }
      }
      
      if(muonTkLengthInEcalCurved > 0.)
      {
        float dEdXCurved = superClusterRawEnergy / muonTkLengthInEcalCurved / 8.28 * 1000. ;
        BetheBloch_curved.Fill (muonP, dEdXCurved);
        pBinCurved = BetheBloch_profile_curved.Fill (muonP, dEdXCurved);
        pMapCurved[pBinCurved] += muonP;
        
        muonTkLength_straight_curved_pT.Fill (muonPt, (muonTkLengthInEcal - muonTkLengthInEcalCurved) );
      }
      
      
      
      
      // BetheBloch_corrected.Fill( muonP, superClusterRawEnergy/(muonTkLengthInEcalDetail+correction)/8.28*250. * 0.9);
      
      //loop su cluster del Supercluster Associato
      //for (int CLUSTERindex = treeVars.clusterIndexInSuperCluster[SCindex] ;
      //     CLUSTERindex < treeVars.clusterIndexInSuperCluster[SCindex] + treeVars.nClustersInSuperCluster[SCindex] ; ++CLUSTERindex)
      //{
      //  BetheBloch_energy.Fill (muonP, treeVars.clusterEnergy[CLUSTERindex]/muonTkLengthInEcalCurved/8.28*250. * 0.9);
      //  BetheBloch_E5x5.Fill (muonP, treeVars.clusterE5x5[CLUSTERindex]/muonTkLengthInEcalCurved/8.28*250. * 0.9);
      //}
      // Curved_Straight_pt.Fill(muonPt, (Length_straight-Length_curved)/Length_curved);
      //Curved.Fill (muonTkLengthInEcalCurved);
      //Straight.Fill (muonTkLengthInEcal);
      //Radius_d.Fill (Radius)
      /////////////////////////////////////////////////////
      //float dphi = muonPhi - treeVars.superClusterPhi[SCindex];
      //if      (dphi>CLHEP::pi) dphi-= 2*CLHEP::pi ;
      //else if(dphi<-CLHEP::pi) dphi+= 2*CLHEP::pi;
      //
      //float deta = muonEta - treeVars.superClusterEta[SCindex];
      //float deltaR = sqrt(pow(deta,2) + pow(dphi,2));
      //
      //
      //DEta_DPhi.Fill(deta, dphi);
      //DPhi_Pt.Fill(muonPt, dphi);
      //DR_Pt.Fill(muonPt, deltaR);
      //
      //DPhi_leg.Fill(treeVars.muonLeg[MUindex], dphi);
    }
  } // loop over entries
  
  
  
  for (int bin = 1 ; bin <= muonTkLength_straight_curved_pT.GetNbinsX () ; ++bin)
  {
    char histoName[50] ;
    sprintf (histoName, "muonTkLength_straight_curved_pTbin_%d", bin) ;
    TH1D* histo = muonTkLength_straight_curved_pT.ProjectionY (histoName, bin, bin) ;
    muonTkLengthMap_straight_curved_RMS[bin] = histo -> GetRMS() ;
  }
  
  
  
  for (int bin = 1 ; bin <= BetheBloch_profile.GetNbinsX () ; ++bin)
  {
    if (pMap[bin] > 0)
      pMap[bin] /= BetheBloch_profile.GetBinEntries (bin) ;
    
    char histoName[50] ;
    sprintf (histoName, "BetheBloch_pBin_%d", bin) ;
    TH1D* histo = BetheBloch.ProjectionY (histoName, bin, bin) ;
    BetheBloch_pBin.push_back (histo);
    
    TF1* langaus = new TF1 ;
    LangausFit (&langaus, histo) ;
    histo -> Write () ;
    
    if (pMap[bin] > 0)
      BetheBloch_mean[bin] = langaus -> Mean (0., 1000.) ;
  }
  
  
  
  for (int bin = 1 ; bin <= BetheBloch_profile_curved.GetNbinsX () ; ++bin)
  {
    if (pMapCurved[bin] >0)
      pMapCurved[bin] /= BetheBloch_profile_curved.GetBinEntries (bin) ;
    
    char histoName[50] ;
    sprintf (histoName, "BetheBloch_pBin_curved_%d", bin) ;
    TH1D* histo = BetheBloch_curved.ProjectionY (histoName, bin, bin) ;
    BetheBloch_pBin_curved.push_back (histo);
    
    TF1* langaus = new TF1 ;
    LangausFit (&langaus, histo) ;
    histo -> Write () ;
    
    if (pMapCurved[bin] >0)
      BetheBloch_mean_curved[bin] = langaus -> Mean (0., 1000.) ;
  }
  
  
  
  
  
  
  double errX_low = -1.;
  double errX_hig = -1.;
  double errY = -1.;
  
  for(int bin = 1; bin <= BetheBloch_profile.GetNbinsX(); ++bin)
  {
    errX_low = pMap[bin] - BetheBloch_profile.GetBinLowEdge (bin) ;
    errX_hig = BetheBloch_profile.GetBinLowEdge (bin) + BetheBloch_profile.GetBinWidth (bin) - pMap[bin] ;
    errY = sqrt ( pow (muonTkLengthMap_straight_curved_RMS[bin], 2) + pow (BetheBloch_profile.GetBinError (bin), 2 ) ) ;
    
    BetheBloch_graph.SetPoint (bin-1, pMap[bin], BetheBloch_profile.GetBinContent(bin) );
    BetheBloch_graph.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    
    BetheBloch_graph_mean.SetPoint (bin-1, pMap[bin], BetheBloch_mean[bin]) ;
    BetheBloch_graph_mean.SetPointError (bin-1, errX_low, errX_hig, errY, errY) ;
    
    // BB_energy.SetPoint(bin-1, pMap[bin-1]/BetheBloch_profile.GetBinEntries(bin), BetheBloch_energy.GetBinContent(bin));
    // BB_energy.SetPointError(bin-1, 0., BetheBloch_energy.GetBinError(bin));
    // BB_25x25.SetPoint(bin+1, pMap[bin]/BetheBloch_profile.GetBinEntries(bin), BetheBloch_E5x5.GetBinContent(bin));
    // BB_E5x5.SetPointError(bin+1, 0., BetheBloch_E5x5.GetBinError(bin));
  }
  
  
  
  for(int bin = 1; bin <= BetheBloch_profile_curved.GetNbinsX(); ++bin)
  {
    errX_low = pMapCurved[bin] - BetheBloch_profile_curved.GetBinLowEdge (bin) ;
    errX_hig = BetheBloch_profile_curved.GetBinLowEdge (bin) + BetheBloch_profile_curved.GetBinWidth (bin) - pMapCurved[bin] ;
    errY = sqrt ( pow (muonTkLengthMap_straight_curved_RMS[bin], 2) + pow (BetheBloch_profile_curved.GetBinError (bin), 2 ) ) ;
    
    BetheBloch_graph_curved.SetPoint (bin-1, pMapCurved[bin], BetheBloch_profile_curved.GetBinContent(bin) );
    BetheBloch_graph_curved.SetPointError (bin-1, errX_low, errX_hig, errY, errY);
    
    BetheBloch_graph_mean_curved.SetPoint (bin-1, pMapCurved[bin], BetheBloch_mean_curved[bin]) ;
    BetheBloch_graph_mean_curved.SetPointError (bin-1, errX_low, errX_hig, errY, errY) ;
    
    // BB_energy.SetPoint(bin-1, pMap[bin-1]/BetheBloch_profile_curved.GetBinEntries(bin), BetheBloch_energy.GetBinContent(bin));
    // BB_energy.SetPointError(bin-1, 0., BetheBloch_energy.GetBinError(bin));
    // BB_25x25.SetPoint(bin+1, pMap[bin]/BetheBloch_profile_curved.GetBinEntries(bin), BetheBloch_E5x5.GetBinContent(bin));
    // BB_E5x5.SetPointError(bin+1, 0., BetheBloch_E5x5.GetBinError(bin));
  }
  
  
  
  
  
  
  
  // Save histograms
  DrawBetheBloch () ;
  BetheBloch.Write() ;
  BetheBloch_profile.Write () ;
  BetheBloch_graph.Write ("BetheBloch_graph") ;
  BetheBloch_graph_mean.Write ("BetheBloch_graph_mean") ;
  BetheBloch_curved.Write() ;
  BetheBloch_profile_curved.Write() ;
  BetheBloch_graph_curved.Write ("BetheBloch_graph_curved") ;
  BetheBloch_graph_mean.Write ("BetheBloch_graph_mean_curved") ;
  // BetheBloch_energy.Write();
  // BetheBloch_25x25.Write();
  // BetheBloch_corrected.Write();
  // numAssociations.Write();
  // Curved_Straight_pt.Write();
  // Radius_d.Write();
  // Curved.Write();
  // Straight.Write();
  // DEta_DPhi.Write();
  // DPhi_Pt.Write();
  // DR_Pt.Write();
  // DPhi_leg.Write();
  //BB_curva.Write("BB_curva");
  //BB_energy.Write("BB_energy");
  //BB_25x25.Write("BB_25x25");
  
  outputRootFile.Close () ;
  
  
  return 0 ;
}






void BinLogX(TProfile& h)
{
  TAxis* axis = h.GetXaxis () ;
  int bins = axis -> GetNbins () ;
  
  Axis_t from = axis -> GetXmin () ;
  Axis_t to = axis -> GetXmax () ;
  Axis_t width = (to - from) / bins ;
  Axis_t* new_bins = new Axis_t[bins + 1] ;
  
  for (int i = 0; i <= bins; i++)
    new_bins[i] = pow(10, from + i * width) ;
  
  axis -> Set (bins, new_bins) ;
  delete new_bins ;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}



void BinLogX(TH2F& h)
{
  TAxis* axis = h.GetXaxis();
  int bins = axis->GetNbins();
  
  Axis_t from = axis->GetXmin();
  Axis_t to = axis->GetXmax();
  Axis_t width = (to - from) / bins;
  Axis_t* new_bins = new Axis_t[bins + 1];
  
  for (int i = 0; i <= bins; i++) 
    new_bins[i] = pow(10, from + i * width);
  
  axis->Set(bins, new_bins);
  delete new_bins;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}



void BinLogX (TH1F& h)
{
  
  TAxis* axis = h.GetXaxis();
  int bins = axis->GetNbins();

  Axis_t from = axis->GetXmin();
  Axis_t to = axis->GetXmax();
  Axis_t width = (to - from) / bins;
  Axis_t* new_bins = new Axis_t[bins + 1];
  
  for (int i = 0; i <= bins; i++) 
    new_bins[i] = pow(10, from + i * width);
  
  axis->Set(bins, new_bins);
  delete new_bins;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}



void LangausFit (TF1** langaus, TH1D* histo)
{
  
  
  // Langaus Fit
  double fitRange[2] ;
  fitRange[0] = 0.; 
  fitRange[1] = 1000. ;
  
  double startValues[4] ;
  startValues[0] = 0.1 ;
  startValues[1] = histo -> GetMean () ;
  startValues[2] = histo -> GetEntries () / 10. ;
  startValues[3] = 0.5 ;
  
  double parLimitsLow[4] ;
  parLimitsLow[0] = 0.0001 ;
  parLimitsLow[1] = 0.0001 ;
  parLimitsLow[2] = 0.0001 ;
  parLimitsLow[3] = 0.0001 ;
  
  double parLimitsHigh[4] ;
  parLimitsHigh[0] = 1000000 ;
  parLimitsHigh[1] = 10 ;
  parLimitsHigh[2] = 100000 ;
  parLimitsHigh[3] = 10 ;
  
  double parameters[4], parErrors[4] ;
  double chi2 ;
  int ndf ;
  
  *langaus = langaufit ((TH1F*)histo, fitRange, startValues, parLimitsLow, parLimitsHigh, parameters, parErrors, &chi2, &ndf) ;
}

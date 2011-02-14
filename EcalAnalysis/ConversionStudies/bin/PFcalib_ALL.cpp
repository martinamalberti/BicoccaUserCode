/*
out of CMSSW:
source ~/Desktop/setupRoot5.25.sh
rootcint -f dict.cpp -c LinkDef.h
c++ -o testReader `root-config --cflags --glibs` -lGenVector testReader.cpp dict.cpp treeReader.cc
*/

//usage: PFcalib conversionTree_collisions900.root conversionTree/SimpleTree


#include "EcalAnalysis/ConversionStudies/interface/PFcalib_functions.h"
#include "PhysicsTools/NtupleUtils/interface/treeReader.h"
#include "PhysicsTools/NtupleUtils/interface/hFactory.h"
#include "PhysicsTools/NtupleUtils/interface/hChain.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>

#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#define mElec 0.000511
#define mPi0  0.1349766
#define PI    3.141592654

int main (int argc, char ** argv)
{
  //==============================
  //==== get input files list ====
  //=============================
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfig(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet();
  
  edm::ParameterSet subPSetSelections =  parameterSet -> getParameter<edm::ParameterSet>("selections");
  int nEvents_ = subPSetSelections.getUntrackedParameter<int>("nEvents", -1);
  bool MCpresent_ =  subPSetSelections.getUntrackedParameter<bool>("MCpresent", false);
  bool MCpho_ =  subPSetSelections.getUntrackedParameter<bool>("MCpho_", false);
  bool MCpair_ =  subPSetSelections.getUntrackedParameter<bool>("MCpair_", false);
  bool MCphoEne_ =  subPSetSelections.getUntrackedParameter<bool>("MCphoEne_", false);
  bool MCphoTheta_ =  subPSetSelections.getUntrackedParameter<bool>("MCphoTheta_", false);

  int nEnergyBins_ = subPSetSelections.getUntrackedParameter<int>("nEnergyBins", 4);
  int nEtaBins_ = subPSetSelections.getUntrackedParameter<int>("nEtaBins", 4);
  double etaMin_ = subPSetSelections.getUntrackedParameter<double>("etaMin", -1.5);
  double etaMax_ = subPSetSelections.getUntrackedParameter<double>("etaMax", 1.5);

  bool reclustering_ =  subPSetSelections.getUntrackedParameter<bool>("reclustering", false);
  double doubleSeedThr_ = subPSetSelections.getUntrackedParameter<double>("doubleSeedThr", 0.);
  double singleSeedThr_ = subPSetSelections.getUntrackedParameter<double>("singleSeedThr", 0.);
  double secondThr_ = subPSetSelections.getUntrackedParameter<double>("secondThr", 0.);

  bool checkComplete3x3_ =  subPSetSelections.getUntrackedParameter<bool>("checkComplete3x3", false);

  bool evaluateEneSpectrum_ =  subPSetSelections.getUntrackedParameter<bool>("evaluateEneSpectrum", true);
  std::string eneSpectrum_ = subPSetSelections.getParameter<std::string> ("eneSpectrum");
  bool fixedBinning_ =  subPSetSelections.getUntrackedParameter<bool>("fixedBinning", false);
  std::vector<double> forcedEneBins_ = subPSetSelections.getParameter<std::vector<double> > ("forcedEneBins");

  std::string outFile_ = subPSetSelections.getParameter<std::string> ("outFile");
 
  edm::ParameterSet subPSetInput = parameterSet -> getParameter<edm::ParameterSet>("inputNtuples");
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles");

  //==========================
  //==== set up the chain ====
  //==========================
  TChain *chain = new TChain ("conversionTree/SimpleTree") ;

  // Input files
  for(std::vector<std::string>::const_iterator listIt = inputFiles.begin();
      listIt != inputFiles.end(); ++listIt)
    {
      chain -> Add (listIt -> c_str());
      std::cout << *listIt << std::endl;
    }

  treeReader reader (chain) ;
  
  //===========================
  //==== define hfactory ======
  //===========================
  int nStep = 3;
  hFactory* histograms = new hFactory(outFile_);

  histograms -> add_h1("seedEnergy",    "",  1000, 0.,  10., nStep);
  histograms -> add_h1("secondEnergy",   "",  1000, 0.,  10., nStep);
  histograms -> add_h1("clusterEnergy", "",  1000, 0.,  50., nStep);
  histograms -> add_h1("photonEnergy",  "",  1000, 0.,  50., nStep);
  histograms -> add_h1("photonEt",      "",  1000, 0.,  50., nStep);
  histograms -> add_h1("photonEta",     "",  1000,-10., 10., nStep);
  histograms -> add_h1("E1onE9",        "",  100, 0.,  2., nStep);
  histograms -> add_h1("nXtalsInCluster","",  10, 0.,  10., nStep);


  //===========================
  //==== define histograms ====
  //===========================
  TH1F* eneBins_histo = new TH1F("eneBins_histo","eneBins_histo",100,0.,100.);
  TH1F* etaBins_histo = new TH1F("etaBins_histo","etaBins_histo",100,0.,100.);
  
  //conversions plots
  TH1F* distOfMinimumApproach = new TH1F("distOfMinimumApproach","distOfMinimumApproach",1000,0.,10.);
  TH1F* dPhiTracksAtVtx = new TH1F("dPhiTracksAtVtx","dPhiTracksAtVtx",1000,0.,3.14);
  TH1F* zOfPrimaryVertexFromTracks = new TH1F("zOfPrimaryVertexFromTracks","zOfPrimaryVertexFromTracks",1000,0.,100.);
  TH1F* conversionMass = new TH1F("conversionMass","conversionMass",2000,0.,1.);
  TH1F* fbrem_distr = new TH1F("fbrem_distr","fbrem_distr",1000,0.,1.);
  TH1F* firstHitPosition = new TH1F("firstHitPosition","firstHitPosition",6500,0.,130.);

  //PFPhotons plots
  TH1F* nConvPerEvent = new TH1F("nConvPerEvent","nConvPerEvent",100,0.,100.);
  TH1F* nPhotonsPerEvent = new TH1F("nPhotonsPerEvent","nPhotonsPerEvent",100,0.,100.);
  TH1F* nGoodPhotonsPerEvent = new TH1F("nGoodPhotonsPerEvent","nGoodPhotonsPerEvent",100,0.,100.);
  //Clusters plots
  TH1F* sumXtals_minusClusterEnergy = new TH1F("sumXtals_minusClusterEnergy","sumXtals_minusClusterEnergy",1000,-3.,3.);
  TH2F* sumXtals_minusClusterEnergy_vsEta = new TH2F("sumXtals_minusClusterEnergy_vsEta","sumXtals_minusClusterEnergy_vsEta",100,-4.,4.,1000,-3.,3.);
  TH1F* photonClusterDeltaR = new TH1F("photonClusterDeltaR","photonClusterDeltaR",1000,0.,1.);

  TH1F* xtalEnergy = new TH1F("xtalEnergy","xtalEnergy",1000,0.,100.);
  TH1F* xtalChi2 = new TH1F("xtalChi2","xtalChi2",600,0.,60.);
  TH1F* xtalTime = new TH1F("xtalTime","xtalTime",2000,10.,-10.);

  //Pi0 distributions
  TH1F* pi0mass_PFcandidate = new TH1F("pi0mass_PFcandidate","pi0mass_PFcandidate",5000,0.,1.);
  TH1F* pi0mass_PFcandidate_cut = new TH1F("pi0mass_PFcandidate_cut","pi0mass_PFcandidate_cut",100,0.,1.);
  TH1F* cosTheta = new TH1F("cosTheta","cosTheta",200,-1.,1.);
  TH1F* pi0pt_PFcandidate = new TH1F("pi0pt_PFcandidate","pi0pt_PFcandidate",1000,0.,10.);
  TH1F* pi0p_PFcandidate = new TH1F("pi0p_PFcandidate","pi0p_PFcandidate",1000,0.,10.);

  TProfile* pi0mass_vsEta = new TProfile("pi0mass_vsEta","pi0mass_vsEta",2000,-2.,2.);
  TH2F* dR_vsPi0mass = new TH2F("dR_vsPi0mass","dR_vsPi0mass",1000,0.,10.,1000,0.,10.);

  TH2F* dEta_vsDPhi = new TH2F("dEta_vsDPhi","dEta_vsDPhi",1000,0.,3.2,1000,0.,10.);
  TH1F* dR_0_025 = new TH1F("dR_0_025","dR_0_025",1000,0.,10.);
  TH1F* dR_025_end = new TH1F("dR_025_end","dR_025_end",1000,0.,10.);
  TH1F* dR_min_dist = new TH1F("dR_min_dist","dR_min_dist",10000,0.,10.);
  TH1F* dR_min_dist_conv = new TH1F("dR_min_dist_conv","dR_min_dist_conv",10000,0.,10.);
  TH1F* pairRECOp_onPairMCp = new TH1F("pairRECOp_onPairMCp","pairRECOp_onPairMCp",2400,-0.2,2.2);
  TH2F* fbrem_vsPairRECOp_onPairMCp = new TH2F("fbrem_vsPairRECOp_onPairMCp","fbrem_vsPairRECOp_onPairMCp",2400,-0.2,2.2,1000,0.,1.);
  TH2F* firstHitPosition_vsPairRECOp_onPairMCp = new TH2F("firstHitPosition_vsPairRECOp_onPairMCp","firstHitPosition_vsPairRECOp_onPairMCp",2400,-0.2,2.2,6500,0.,130.);

  TH1F* diPhotonInvMass = new TH1F("diPhotonInvMass","diPhotonInvMass",2000,0.,1.);

  //calibration distributions
  TH1F* deltaE = new TH1F("deltaE","deltaE",1000, -10., 10.);

  TH2F* deltaE_vsE = new TH2F("deltaE_vsE","deltaE_vsE",1000,0.,100.,1000, -10., 10.);
  TH2F* deltaE_vsEta = new TH2F("deltaE_vsEta","deltaE_vsEta",100,-5.,5.,1000, -10., 10.);

  TH2F* deltaE_vsE_prev = new TH2F("deltaE_vsE_prev","deltaE_vsE_prev",1000,0.,100.,1000, -10., 10.);
  TH2F* deltaE_vsEta_prev = new TH2F("deltaE_vsEta_prev","deltaE_vsEta_prev",100,-5.,5.,1000, -10., 10.);

  TH2F* clusterNXtals_vsPhoEneRatio = new TH2F("clusterNXtals_vsPhoEneRatio","clusterNXtals_vsPhoEneRatio", 2500, 0., 10.,100,0.,100.);

  TH2F* Eexp_vsEmeas = new TH2F("Eexp_vsEmeas","Eexp_vsEmeas",3000,0.,20.,2000,-10.,20.);
  TProfile* Eexp_vsEmeas_profile = new TProfile("Eexp_vsEmeas_profile","Eexp_vsEmeas_profile",2000,0.,20.);
  TH1F* Eexp_onEmeas = new TH1F("Eexp_onEmeas","Eexp_onEmeas",1000,0.,5.);

  
  //MAPS
  TH2F* pi0peak_vsEnergy = new TH2F("pi0peak_vsEnergy","pi0peak_vsEnergy", 1000, 0., 50., 4000, 0., 1.);
  TH2F* pi0peak_vsEta = new TH2F("pi0peak_vsEta","pi0peak_vsEta", 1000, -1.5, 1.5, 4000, 0., 1.);


  std::map<int,TH1F*> deltaE_energyBin;
  std::map<int,TH1F*> deltaE_etaBin;
  std::map<int,TH1F*> pi0peak_energyBin;
  std::map<int,TH1F*> pi0peak_etaBin;
  std::map<int,TH1F*> energyBin;
  std::map<int,TH1F*> etaBin;
  std::map<int,TH1F*> EcosTheta_energyBin;
  std::map<int,TH1F*> EcosTheta_etaBin;
  std::map<int,TH1F*> phoEneRatio_energyBin;
  std::map<int,TH1F*> phoEneRatio_etaBin;


  //=====================ene spectrum=================
  TFile* eneFile = new TFile(eneSpectrum_.c_str(),"READ");
  TH1F* enespectrum;
  //=====================ene spectrum=================

  //==========================
  //==== global variables ====
  //==========================

  int nevents = 0;
  if (nEvents_ == -1) nevents = chain->GetEntries ();
  else nevents = nEvents_;
  
  //==============================================================filling ene spectrum============================================
  //==============================
  //==== loop over the events ====
  //==============================
  if(evaluateEneSpectrum_ == false) 
    enespectrum = (TH1F*)eneFile->Get("Plots/enespectrum");
  else
    {
      enespectrum = new TH1F("enespectrum","enespectrum",50000,0.,50.);
      for (int iEvent = 0 ; iEvent < nevents ; ++iEvent)
	{
	  reader.GetEntry (iEvent) ;
	  
	  if(iEvent%100000 == 0) std::cout << "FIRST CYCLE - reading entry: " << iEvent << std::endl;
	  
	  //=========================
	  //==== set up branches ====
	  //=========================
	  //conversions 
	  std::vector<float> pairIsConverted = *(reader.GetFloat("pairIsConverted"));
	  std::vector<float> pairNTracks = *(reader.GetFloat("pairNTracks"));
	  std::vector<float> pairInvariantMass = *(reader.GetFloat("pairInvariantMass"));
	  std::vector<ROOT::Math::XYZVector> pairMomentum = *(reader.Get3V("pairMomentum"));
	  std::vector<float> pairZOfPrimaryVertexFromTracks = *(reader.GetFloat("pairZOfPrimaryVertexFromTracks"));
	  std::vector<float> pairDistOfMinimumApproach = *(reader.GetFloat("pairDistOfMinimumApproach"));
	  std::vector<float> pairDPhiTracksAtVertex = *(reader.GetFloat("pairDPhiTracksAtVertex"));
	  std::vector<ROOT::Math::XYZVector> pairFirstTrackPIn = *(reader.Get3V("pairFirstTrackPIn"));
	  std::vector<ROOT::Math::XYZVector> pairSecondTrackPIn = *(reader.Get3V("pairSecondTrackPIn"));
	  std::vector<ROOT::Math::XYZVector> pairFirstTrackPOut = *(reader.Get3V("pairFirstTrackPOut"));
	  std::vector<ROOT::Math::XYZVector> pairSecondTrackPOut = *(reader.Get3V("pairSecondTrackPOut"));
	  std::vector<ROOT::Math::XYZVector> pairFirstTrackInnerPosition = *(reader.Get3V("pairFirstTrackInnerPosition"));
	  std::vector<ROOT::Math::XYZVector> pairSecondTrackInnerPosition = *(reader.Get3V("pairSecondTrackInnerPosition"));
	  //PFPhotons
	  std::vector<ROOT::Math::XYZTVector> PFPhoton = *(reader.Get4V("PFPhoton"));
	  std::vector<float> PFPhotonClusterEnergy = *(reader.GetFloat("PFPhotonClusterEnergy"));
	  std::vector<ROOT::Math::XYZVector> PFPhotonClusterPosition = *(reader.Get3V("PFPhotonClusterPosition"));
	  std::vector<ROOT::Math::XYZTVector> PFPhotonCluster = *(reader.Get4V("PFPhotonCluster"));
	  std::vector<float> PFPhotonType = *(reader.GetFloat("PFPhotonType"));      
	  
	  std::vector<float> PFPhotonClusterE3x3 = *(reader.GetFloat("PFPhotonClusterE3x3"));
	  std::vector<float> PFPhotonClusterE5x5 = *(reader.GetFloat("PFPhotonClusterE5x5"));
	  std::vector<float> PFPhotonClusterEMax = *(reader.GetFloat("PFPhotonClusterEMax"));
	  std::vector<float> PFPhotonClusterE2nd = *(reader.GetFloat("PFPhotonClusterE2nd"));
	  std::vector<float> PFPhotonClusterRawIdMax = *(reader.GetFloat("PFPhotonClusterRawIdMax"));
	  std::vector<float> PFPhotonCluster3x3Complete = *(reader.GetFloat("PFPhotonCluster3x3Complete"));      
	  
	  //xtals
	  std::vector<float> PFPhotonClusterXtalEnergy = *(reader.GetFloat("PFPhotonClusterXtalEnergy"));
	  std::vector<float> PFPhotonClusterXtalTime = *(reader.GetFloat("PFPhotonClusterXtalTime"));
	  std::vector<float> PFPhotonClusterNXtals = *(reader.GetFloat("PFPhotonClusterNXtals"));
	  std::vector<float> PFPhotonClusterXtalIndex = *(reader.GetFloat("PFPhotonClusterXtalIndex"));
	  std::vector<float> PFPhotonClusterXtalRawId = *(reader.GetFloat("PFPhotonClusterXtalRawId"));
	  //std::vector<float> PFPhotonClusterXtalChi2 = *(reader.GetFloat("PFPhotonClusterXtalChi2"));
	  
	  
	  //MCPhotons
	  std::vector<float> MCPhotonMother;
	  std::vector<ROOT::Math::XYZTVector> MCPhoton;
	  if(MCpresent_ == true)
	    {
	      MCPhotonMother = *(reader.GetFloat("MCPhotonMother"));
	      MCPhoton = *(reader.Get4V("MCPhoton"));
	    }
	  
	  
	  //===================================
	  //==== loop over the conversions ====
	  //===================================
	  for(unsigned int convItr = 0; convItr < pairMomentum.size(); ++convItr)
	    {
	      if(pairIsConverted[convItr] == 0) continue;
	      if(pairNTracks[convItr] != 2) continue;
	      
	      if(fabs(pairDistOfMinimumApproach[convItr]) > 0.05) continue;
	      if(fabs(pairDPhiTracksAtVertex[convItr]) > 0.05) continue;
	      //if(fabs(zOfPrimaryVertexFromTracks[convItr]) > 0.02) continue;

	      if(fabs(pairMomentum[convItr].eta()) > 1.2 ) continue;	      
	      //===========================================
	      //==== loop over PF candidate collection ====
	      //===========================================
	      
	      
	      for(unsigned int phoItr = 0; phoItr < PFPhoton.size(); ++phoItr) //ogni fotone ha un cluster
		{
		  if(PFPhotonType[phoItr] != 0) continue; //only barrel
		  if(fabs(PFPhoton[phoItr].eta()) > 1.) continue;

		  //RECLUSTERING

		  //3X3COMPLETE

		  //matching cluster-MC
		  int matchedPho = -1;
		  int matchedConv = -1;
		  if(MCpresent_ == true)
		    {
		      std::pair<int,double> dRpho = min_dR(PFPhoton[phoItr], MCPhoton, MCPhotonMother);

		      //std::cout << "progr: MCPhoton.size() = " << MCPhoton.size() << std::endl;
		      //std::cout << "progr: dRpho.second = " << dRpho.second << std::endl;

		      if (dRpho.second > 0.04) continue;  //FIXME MINIMO DA FILE
		      matchedPho = dRpho.first;
		  
		      //matching conv-MC
		      std::pair<int,double> dRconv = min_dR(pairMomentum[convItr], MCPhoton, MCPhotonMother);
		      if (dRconv.second > 0.04) continue;  //FIXME MINIMO DA FILE
		      matchedConv = dRconv.first;
		    }

		  // -->> condition for MC variables or not
		  double pho_eta = PFPhoton[phoItr].eta();
		  double pho_phi = PFPhoton[phoItr].phi();	      
		  double pho_px = PFPhoton[phoItr].px();
  		  double pho_py = PFPhoton[phoItr].py();
  		  double pho_pz = PFPhoton[phoItr].pz();
		  double pho_ene = PFPhoton[phoItr].energy();

		  double conv_px = pairFirstTrackPIn[convItr].x() + pairSecondTrackPIn[convItr].x();
		  double conv_py = pairFirstTrackPIn[convItr].y() + pairSecondTrackPIn[convItr].y();
		  double conv_pz = pairFirstTrackPIn[convItr].z() + pairSecondTrackPIn[convItr].z();

		  if(MCpresent_ == true && MCpho_ == true)
		    {
		      if(MCphoTheta_ == true)
			{
			  pho_eta = MCPhoton[matchedPho].eta();
			  pho_phi = MCPhoton[matchedPho].phi();	      
			  pho_px = MCPhoton[matchedPho].px();
			  pho_py = MCPhoton[matchedPho].py();
			  pho_pz = MCPhoton[matchedPho].pz();
			}
		      if(MCphoEne_ == true)
			pho_ene = MCPhoton[matchedPho].energy();	
		    }
		  if(MCpresent_ == true && MCpair_ == true)
		    {
		      conv_px = MCPhoton[matchedConv].px();
		      conv_py = MCPhoton[matchedConv].py();
		      conv_pz = MCPhoton[matchedConv].pz();
		    }

		  double px = conv_px + pho_px;
		  double py = conv_py + pho_py;
		  double pz = conv_pz + pho_pz;
		  
		  if(sqrt(pho_px*pho_px + pho_py*pho_py) < 0.25 || sqrt(px*px + py*py) < 0.9)  //taglio su pt pi0
		    continue;
		  
		  
		  
		  enespectrum->Fill(pho_ene);
		  
		} //loop photons
	    } //loop conversions
	}//loop entries
    }
  //==============================================================choosing the ene bins============================================
  std::vector<double> energybins;

  if(fixedBinning_ == false)
    {
      int nbins = enespectrum->GetNbinsX();
      float counts = 0;
      energybins.push_back(0.);
      for(int ii = 0; ii < nbins; ++ii)
	{
	  counts += enespectrum->GetBinContent(ii);
	  if (counts >  (enespectrum->GetEntries() / nEnergyBins_))
	    {
	      energybins.push_back(enespectrum->GetBinCenter(ii));
	      counts = 0;
	    }
	}
    }
  else
    for(unsigned int ii = 0; ii < forcedEneBins_.size(); ++ii)
      energybins.push_back(forcedEneBins_.at(ii));
    
  std::cout << "EneBins -> ";
  for (unsigned int oo = 0; oo < energybins.size(); ++oo)
    std::cout << energybins.at(oo) << ", " ;
  std::cout << std::endl;

  //==============================================================END filling ene spectrum============================================  
  //==============================
  //==== loop over the events ====
  //==============================
  int passed = 0;
  for (int iEvent = 0 ; iEvent < nevents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;

      if(iEvent%100000 == 0) std::cout << "SECOND CYCLE - reading entry: " << iEvent << ", passed = " << passed << std::endl;


      //=========================
      //==== set up branches ====
      //=========================
      //conversions 
      std::vector<float> pairIsConverted = *(reader.GetFloat("pairIsConverted"));
      std::vector<float> pairNTracks = *(reader.GetFloat("pairNTracks"));
      std::vector<float> pairInvariantMass = *(reader.GetFloat("pairInvariantMass"));
      std::vector<ROOT::Math::XYZVector> pairMomentum = *(reader.Get3V("pairMomentum"));
      std::vector<float> pairZOfPrimaryVertexFromTracks = *(reader.GetFloat("pairZOfPrimaryVertexFromTracks"));
      std::vector<float> pairDistOfMinimumApproach = *(reader.GetFloat("pairDistOfMinimumApproach"));
      std::vector<float> pairDPhiTracksAtVertex = *(reader.GetFloat("pairDPhiTracksAtVertex"));
      std::vector<ROOT::Math::XYZVector> pairFirstTrackPIn = *(reader.Get3V("pairFirstTrackPIn"));
      std::vector<ROOT::Math::XYZVector> pairSecondTrackPIn = *(reader.Get3V("pairSecondTrackPIn"));
      std::vector<ROOT::Math::XYZVector> pairFirstTrackPOut = *(reader.Get3V("pairFirstTrackPOut"));
      std::vector<ROOT::Math::XYZVector> pairSecondTrackPOut = *(reader.Get3V("pairSecondTrackPOut"));
      std::vector<ROOT::Math::XYZVector> pairFirstTrackInnerPosition = *(reader.Get3V("pairFirstTrackInnerPosition"));
      std::vector<ROOT::Math::XYZVector> pairSecondTrackInnerPosition = *(reader.Get3V("pairSecondTrackInnerPosition"));

      //PFPhotons
      std::vector<ROOT::Math::XYZTVector> PFPhoton = *(reader.Get4V("PFPhoton"));
      std::vector<float> PFPhotonClusterEnergy = *(reader.GetFloat("PFPhotonClusterEnergy"));
      std::vector<ROOT::Math::XYZVector> PFPhotonClusterPosition = *(reader.Get3V("PFPhotonClusterPosition"));
      std::vector<ROOT::Math::XYZTVector> PFPhotonCluster = *(reader.Get4V("PFPhotonCluster"));
      std::vector<float> PFPhotonType = *(reader.GetFloat("PFPhotonType"));      

      std::vector<float> PFPhotonClusterE3x3 = *(reader.GetFloat("PFPhotonClusterE3x3"));
      std::vector<float> PFPhotonClusterE5x5 = *(reader.GetFloat("PFPhotonClusterE5x5"));
      std::vector<float> PFPhotonClusterEMax = *(reader.GetFloat("PFPhotonClusterEMax"));
      std::vector<float> PFPhotonClusterE2nd = *(reader.GetFloat("PFPhotonClusterE2nd"));
      std::vector<float> PFPhotonClusterRawIdMax = *(reader.GetFloat("PFPhotonClusterRawIdMax"));
      std::vector<float> PFPhotonCluster3x3Complete = *(reader.GetFloat("PFPhotonCluster3x3Complete"));      
      
      //xtals
      std::vector<float> PFPhotonClusterXtalEnergy = *(reader.GetFloat("PFPhotonClusterXtalEnergy"));
      std::vector<float> PFPhotonClusterXtalTime = *(reader.GetFloat("PFPhotonClusterXtalTime"));
      std::vector<float> PFPhotonClusterNXtals = *(reader.GetFloat("PFPhotonClusterNXtals"));
      std::vector<float> PFPhotonClusterXtalIndex = *(reader.GetFloat("PFPhotonClusterXtalIndex"));
      std::vector<float> PFPhotonClusterXtalRawId = *(reader.GetFloat("PFPhotonClusterXtalRawId"));
      //std::vector<float> PFPhotonClusterXtalChi2 = *(reader.GetFloat("PFPhotonClusterXtalChi2"));

      //PFClusters
      std::vector<float> PFClusterEnergy = *(reader.GetFloat("PFClusterEnergy"));
      std::vector<ROOT::Math::XYZVector> PFClusterPosition = *(reader.Get3V("PFClusterPosition"));
      std::vector<ROOT::Math::XYZTVector> PFCluster = *(reader.Get4V("PFCluster"));

      //MCPhotons
      std::vector<float> MCPhotonMother;
      std::vector<ROOT::Math::XYZTVector> MCPhoton;
      if(MCpresent_ == true)
	{
	  MCPhotonMother = *(reader.GetFloat("MCPhotonMother"));
	  MCPhoton = *(reader.Get4V("MCPhoton"));
	}
      
      //==== loop over the PFPhotons per test massa inv doppio Pi0 ====
      for(unsigned int phoItr = 0; phoItr < PFPhoton.size(); ++phoItr) //ogni fotone ha un cluster
	{
	  if(PFPhotonType[phoItr] != 0) continue; //only barrel
	  if(fabs(PFPhoton[phoItr].eta()) > 1.) continue;
	  //massa inv 2 fotoni naive
	  for(unsigned int phoItr2 = phoItr+1; phoItr2 < PFPhoton.size(); ++phoItr2) //ogni fotone ha un cluster
	    {
	      if(PFPhotonType[phoItr2] != 0) continue; //only barrel
	      if(fabs(PFPhoton[phoItr2].eta()) > 1.) continue;
	      
	      if(PFPhoton[phoItr].E() < 0.4 || PFPhoton[phoItr2].E() < 0.4) continue;
	      if((PFPhoton[phoItr]+PFPhoton[phoItr2]).E() < 1.5) continue;
	      
	      diPhotonInvMass->Fill((PFPhoton[phoItr]+PFPhoton[phoItr2]).M());
	      
	      
	    }
	  //end massa inv 2 fotoni 
	}
      
      //===================================
      //==== loop over the conversions ====
      //===================================
      int PFGoodPhotons = 0;
      int goodConv = 0;
      for(unsigned int convItr = 0; convItr < pairMomentum.size(); ++convItr)
	{
	  if(pairIsConverted[convItr] == 0) continue;
	  if(pairNTracks[convItr] != 2) continue;


	  //=========================
	  //=== conversions plots ===
	  //=========================
	  distOfMinimumApproach->Fill(fabs(pairDistOfMinimumApproach[convItr]));
	  dPhiTracksAtVtx->Fill(fabs(pairDPhiTracksAtVertex[convItr]));
	  zOfPrimaryVertexFromTracks->Fill(fabs(pairZOfPrimaryVertexFromTracks[convItr]));
	  conversionMass->Fill(pairInvariantMass[convItr]); //only for double track conversions
	  	  
	  if(fabs(pairDistOfMinimumApproach[convItr]) > 0.05) continue;
	  if(fabs(pairDPhiTracksAtVertex[convItr]) > 0.05) continue;
	  //if(fabs(zOfPrimaryVertexFromTracks[convItr]) > 0.02) continue;

	  if(fabs(pairMomentum[convItr].eta()) > 1.2 ) continue;

	  double fbremFirstTrack = (sqrt(pairFirstTrackPIn[convItr].Mag2()) - sqrt(pairFirstTrackPOut[convItr].Mag2())) / sqrt(pairFirstTrackPIn[convItr].Mag2());
	  double fbremSecondTrack = (sqrt(pairSecondTrackPIn[convItr].Mag2()) - sqrt(pairSecondTrackPOut[convItr].Mag2())) / sqrt(pairSecondTrackPIn[convItr].Mag2());
	  fbrem_distr->Fill(fbremFirstTrack);
	  fbrem_distr->Fill(fbremSecondTrack);

	  firstHitPosition->Fill(pairFirstTrackInnerPosition[convItr].R());
	  firstHitPosition->Fill(pairSecondTrackInnerPosition[convItr].R());
	  //===========================================
	  //==== loop over PF candidate collection ====
	  //===========================================
	  PFGoodPhotons = 0;
	  for(unsigned int phoItr = 0; phoItr < PFPhoton.size(); ++phoItr) //ogni fotone ha un cluster
	    {
	      if(PFPhotonType[phoItr] != 0) continue; //only barrel
	      if(fabs(PFPhoton[phoItr].eta()) > 1.) continue;

	      //====================================
	      //=== PFPhotonClusters plots STEP1 ===
	      //====================================
	      int step = 0;
	      histograms -> Fill("seedEnergy",       step, PFPhotonClusterEMax[phoItr]);
	      histograms -> Fill("secondEnergy",      step, PFPhotonClusterE2nd[phoItr]);
	      histograms -> Fill("clusterEnergy",    step, PFPhotonClusterEnergy[phoItr]);
	      histograms -> Fill("photonEnergy",     step, PFPhoton[phoItr].energy());
	      histograms -> Fill("photonEt",         step, PFPhoton[phoItr].Et());
	      histograms -> Fill("photonEta",        step, PFPhoton[phoItr].eta());
	      histograms -> Fill("E1onE9",           step, PFPhotonClusterEMax[phoItr] / PFPhotonClusterE3x3[phoItr]);
	      histograms -> Fill("nXtalsInCluster",  step, PFPhotonClusterNXtals[phoItr]);
	      
	      double eneTmp = 0;

	      //RECLUSTERING

	      //3X3 COMPLETE

	      //====================================
	      //=== PFPhotonClusters plots STEP2 ===
	      //====================================
	      step = 1;
	      histograms -> Fill("seedEnergy",       step, PFPhotonClusterEMax[phoItr]);
	      histograms -> Fill("secondEnergy",      step, PFPhotonClusterE2nd[phoItr]);
	      histograms -> Fill("clusterEnergy",    step, PFPhotonClusterEnergy[phoItr]);
	      histograms -> Fill("photonEnergy",     step, PFPhoton[phoItr].energy());
	      histograms -> Fill("photonEt",         step, PFPhoton[phoItr].Et());
	      histograms -> Fill("photonEta",        step, PFPhoton[phoItr].eta());
	      histograms -> Fill("E1onE9",           step, PFPhotonClusterEMax[phoItr] / PFPhotonClusterE3x3[phoItr]);
	      histograms -> Fill("nXtalsInCluster",  step, PFPhotonClusterNXtals[phoItr]);
	      //histograms -> Fill("nPhotonsPerEvent",  step, PFPhoton.size());
	     

	      //==========================
	      //=== plots di controllo ===
	      //==========================
	      sumXtals_minusClusterEnergy->Fill(eneTmp - PFPhotonClusterEnergy[phoItr]);
	      sumXtals_minusClusterEnergy_vsEta->Fill(PFPhotonCluster[phoItr].eta(), eneTmp - PFPhotonClusterEnergy[phoItr]);


	      //controllo matching PFPhoton, PFCluster
	      double dEtaPC = fabs(PFPhoton[phoItr].eta() - PFPhotonCluster[phoItr].eta());
	      double dPhiPC = fabs(PFPhoton[phoItr].phi() - PFPhotonCluster[phoItr].phi());
	      if (dPhiPC > 2*PI) dPhiPC -= 2*PI;
	      if (dPhiPC > PI ) dPhiPC = 2*PI - dPhiPC;
	      photonClusterDeltaR->Fill(sqrt(dPhiPC*dPhiPC + dEtaPC*dEtaPC));
	      //controllo matching PFPhoton, PFCluster

	      //----------------------------------------------------------------------
	      //----------------------------------------------------------------------
	      int matchedPho = -1;
	      int matchedConv = -1;
	      double phoEneRatio = -9999.;
	      if(MCpresent_ == true)
		{
		  //matching cluster-MC
		  std::pair<int,double> dRpho = min_dR(PFPhoton[phoItr], MCPhoton, MCPhotonMother);
		  if (dRpho.second > 0.04) continue;  //FIXME MINIMO DA FILE
		  matchedPho = dRpho.first;
		  
		  //matching conv-MC
		  std::pair<int,double> dRconv = min_dR(pairMomentum[convItr], MCPhoton, MCPhotonMother);
		  if (dRconv.second > 0.04) continue;  //FIXME MINIMO DA FILE
		  matchedConv = dRconv.first;
		  
		  
		  //check conversione reco / conversione MC
		  pairRECOp_onPairMCp->Fill(sqrt(pairMomentum[convItr].Mag2()) / MCPhoton[matchedConv].P());
		  
		  fbrem_vsPairRECOp_onPairMCp->Fill(sqrt(pairMomentum[convItr].Mag2()) / MCPhoton[matchedConv].P(),fbremFirstTrack);
		  fbrem_vsPairRECOp_onPairMCp->Fill(sqrt(pairMomentum[convItr].Mag2()) / MCPhoton[matchedConv].P(),fbremSecondTrack);
		  
		  firstHitPosition_vsPairRECOp_onPairMCp->Fill(sqrt(pairMomentum[convItr].Mag2()) / MCPhoton[matchedConv].P(),pairFirstTrackInnerPosition[convItr].R());
		  firstHitPosition_vsPairRECOp_onPairMCp->Fill(sqrt(pairMomentum[convItr].Mag2()) / MCPhoton[matchedConv].P(),pairSecondTrackInnerPosition[convItr].R());

		  phoEneRatio = PFPhoton[phoItr].energy() / MCPhoton[matchedPho].energy();
		  clusterNXtals_vsPhoEneRatio->Fill(PFPhoton[phoItr].energy() / MCPhoton[matchedPho].energy(),PFPhotonClusterNXtals[phoItr]);
		}


	      //----------------------------------------------------------------------
	      //----------------------------------------------------------------------


	      double pho_eta = PFPhoton[phoItr].eta();
	      double pho_phi = PFPhoton[phoItr].phi();	      
	      double pho_px = PFPhoton[phoItr].px();
	      double pho_py = PFPhoton[phoItr].py();
	      double pho_pz = PFPhoton[phoItr].pz();
	      double pho_ene = PFPhoton[phoItr].energy();
	      
	      double conv_px = pairFirstTrackPIn[convItr].x() + pairSecondTrackPIn[convItr].x();
	      double conv_py = pairFirstTrackPIn[convItr].y() + pairSecondTrackPIn[convItr].y();
	      double conv_pz = pairFirstTrackPIn[convItr].z() + pairSecondTrackPIn[convItr].z();
	      double conv_eta = pairMomentum[convItr].eta();
	      double conv_phi = pairMomentum[convItr].phi();	
	      double conv_energy = (sqrt(pairFirstTrackPIn[convItr].Mag2() + mElec*mElec) + sqrt(pairSecondTrackPIn[convItr].Mag2() + mElec*mElec));


	      if(MCpresent_ == true && MCpho_ == true)
		{
		  if(MCphoTheta_ == true)
		    {
		      pho_eta = MCPhoton[matchedPho].eta();
		      pho_phi = MCPhoton[matchedPho].phi();	      
		      pho_px = MCPhoton[matchedPho].px();
		      pho_py = MCPhoton[matchedPho].py();
		      pho_pz = MCPhoton[matchedPho].pz();
		    }
		  if(MCphoEne_ == true)
		    pho_ene = MCPhoton[matchedPho].energy();		      
		}
	      if(MCpresent_ == true && MCpair_ == true)
		{
		  conv_px = MCPhoton[matchedConv].px();
		  conv_py = MCPhoton[matchedConv].py();
		  conv_pz = MCPhoton[matchedConv].pz();
		  conv_eta = MCPhoton[matchedConv].eta();
		  conv_phi = MCPhoton[matchedConv].phi();
		  conv_energy = MCPhoton[matchedConv].energy();
		}
	      
	      double px = conv_px + pho_px;
	      double py = conv_py + pho_py;
	      double pz = conv_pz + pho_pz;
	      
	  
	      TLorentzVector pair(conv_px, conv_py, conv_pz, conv_energy);
	      TLorentzVector pho(pho_px, pho_py, pho_pz, pho_ene);
	      double Etot = pair.Energy() + pho.Energy();

	      double theta = pair.Angle(pho.Vect());
	      
	      double dEta = fabs(pho_eta - conv_eta);
	      double dPhi = fabs(pho_phi - conv_phi);
	      if (dPhi > 2*PI) dPhi -= 2*PI;
	      if (dPhi > PI ) dPhi = 2*PI - dPhi;
 	      double dR = sqrt(dPhi*dPhi + dEta*dEta);
 	      dEta_vsDPhi->Fill(dPhi,dEta);

	      //==================================
	      //==== Pi0 selections and plots ====
	      //==================================
	      pi0pt_PFcandidate->Fill(sqrt(px*px + py*py));
	      pi0p_PFcandidate->Fill(sqrt(px*px + py*py + pz*pz));

	      if(sqrt(pho_px*pho_px + pho_py*pho_py) < 0.25 || sqrt(px*px + py*py) < 0.9)  //taglio su pt pi0
		continue;

	      //====================================
	      //=== PFPhotonClusters plots STEP3 ===
	      //====================================
	      step = 2;
	      histograms -> Fill("seedEnergy",       step, PFPhotonClusterEMax[phoItr]);
	      histograms -> Fill("secondEnergy",      step, PFPhotonClusterE2nd[phoItr]);
	      histograms -> Fill("clusterEnergy",    step, PFPhotonClusterEnergy[phoItr]);
	      histograms -> Fill("photonEnergy",     step, PFPhoton[phoItr].energy());
	      histograms -> Fill("photonEt",         step, PFPhoton[phoItr].Et());
	      histograms -> Fill("photonEta",        step, PFPhoton[phoItr].eta());
	      histograms -> Fill("E1onE9",           step, PFPhotonClusterEMax[phoItr] / PFPhotonClusterE3x3[phoItr]);
	      histograms -> Fill("nXtalsInCluster",  step, PFPhotonClusterNXtals[phoItr]);
	      //histograms -> Fill("nPhotonsPerEvent",  step, PFPhoton.size());

	      double ePairCosTheta = 2*pair.Energy()*(1 - cos(theta));
	      double invMass = 2*pair.Energy()*pho.Energy()*(1 - cos(theta));
	      if(invMass > 0) invMass = sqrt(invMass);
	      else invMass = -1.;
	      
	      pi0mass_PFcandidate->Fill(invMass);
	      cosTheta->Fill(cos(theta));

	      //fixme
	      //if (invMass < 0.100 || invMass > 0.160) continue;

	      deltaE->Fill(pho.Energy() - (mPi0*mPi0 / (2*pair.Energy()*(1 - cos(theta)))));
	      
	      deltaE_vsE->Fill(pho.Energy(), pho.Energy() - (mPi0*mPi0 / (2*pair.Energy()*(1 - cos(theta)))));
	      deltaE_vsE_prev->Fill((mPi0*mPi0 / (2*pair.Energy()*(1 - cos(theta)))), pho.Energy() - (mPi0*mPi0 / (2*pair.Energy()*(1 - cos(theta)))));
	      deltaE_vsEta->Fill(pho_eta, pho.Energy() - (mPi0*mPi0 / (2*pair.Energy()*(1 - cos(theta)))));
	      

	      //=====================
	      //==== energy bins ====  
	      //=====================
	      //riempio le MAPS
	      pi0peak_vsEnergy -> Fill(pho_ene, invMass);
	      pi0peak_vsEta    -> Fill(pho_eta, invMass);




	      //=== scelgo il bin dove cade il fotone ===

	      int eneIndex = -1;
	      if(pho.Energy() > energybins.at(energybins.size() - 1)) eneIndex = energybins.size() - 1;
	      else for(unsigned int kk = 1; kk < energybins.size(); ++kk)
		if(pho.Energy() <= energybins.at(kk) && pho.Energy() > energybins.at(kk-1)) 
		  {
		    eneIndex = kk-1;
		    break;
		  }
	      
	      std::string name1 = ("deltaE_energyBin");
	      checkBin(eneIndex,deltaE_energyBin, name1, 5000, -5., 5.);
	      deltaE_energyBin[eneIndex]->Fill(pho.Energy() - (mPi0*mPi0 / (2*pair.Energy()*(1 - cos(theta)))));

	      std::string name2 = ("pi0peak_energyBin");
	      checkBin(eneIndex,pi0peak_energyBin, name2, 4000, 0., 1.);
	      pi0peak_energyBin[eneIndex]->Fill(invMass);

	      std::string name3 = ("energyBin");
	      checkBin(eneIndex,energyBin, name3, 5000, 0., 50.);
	      energyBin[eneIndex]->Fill(pho.Energy());

	      std::string name4 = ("EcosTheta_energyBin");
	      checkBin(eneIndex,EcosTheta_energyBin, name4, 5000, 0., 100.);
	      EcosTheta_energyBin[eneIndex]->Fill(ePairCosTheta);

	      std::string name5 = ("phoEneRatio_energyBin");
	      checkBin(eneIndex,phoEneRatio_energyBin, name5, 5000, 0., 10.);
	      phoEneRatio_energyBin[eneIndex]->Fill(phoEneRatio);

	      
	      //==================
	      //==== eta bins ====  
	      //==================
	      //=== scelgo il bin dove cade il fotone ===

	      double etaWidth = (etaMax_ - etaMin_)/nEtaBins_;
	      int etaIndex = (int)fabs((pho_eta - etaMin_) / etaWidth);

	      std::string name11 = ("deltaE_etaBin");
	      checkBin(etaIndex,deltaE_etaBin, name11, 5000, -5., 5.);
	      deltaE_etaBin[etaIndex]->Fill(pho.Energy() - (mPi0*mPi0 / (2*pair.Energy()*(1 - cos(theta)))));

	      std::string name22 = ("pi0peak_etaBin");
	      checkBin(etaIndex,pi0peak_etaBin, name22, 4000, 0., 1.);
	      pi0peak_etaBin[etaIndex]->Fill(invMass);

	      std::string name33 = ("etaBin");
	      checkBin(etaIndex,etaBin, name33, 5000, -1.5, 1.5);
	      etaBin[etaIndex]->Fill(pho.Eta());

	      std::string name44 = ("EcosTheta_etaBin");
	      checkBin(etaIndex,EcosTheta_etaBin, name44, 5000, 0., 100.);
	      EcosTheta_etaBin[etaIndex]->Fill(ePairCosTheta);

	      std::string name55 = ("phoEneRatio_etaBin");
	      checkBin(etaIndex,phoEneRatio_etaBin, name55, 5000, 0., 10.);
	      phoEneRatio_etaBin[etaIndex]->Fill(phoEneRatio);



	      ++PFGoodPhotons;
	      ++goodConv;
	      
	    } //loop over PFcandidate collection
	} //loop over conversions

      nPhotonsPerEvent -> Fill(PFPhoton.size());
      nGoodPhotonsPerEvent->Fill(PFGoodPhotons);
      nConvPerEvent -> Fill(goodConv);

      ++passed;

    } //loop over the events
  
  //======================================
  //==== faccio scrivere all'Hfactory ====
  //======================================

  if (fixedBinning_ == true) eneBins_histo->Fill(forcedEneBins_.size());
  else eneBins_histo->Fill(energyBin.size());

  etaBins_histo->Fill(etaBin.size());

  delete histograms;

  //=====================
  //==== output file ====
  //=====================
  TFile* file_ = new TFile(outFile_.c_str(),"UPDATE");
  file_->cd();
  file_->mkdir("Plots");
  file_->cd("Plots");
  
  //============================
  //==== writing histograms ====
  //============================
  eneBins_histo->Write();
  etaBins_histo->Write();
  enespectrum->Write();

  distOfMinimumApproach->Write();
  dPhiTracksAtVtx->Write();
  zOfPrimaryVertexFromTracks->Write();

  pi0mass_PFcandidate->Write();
  pi0mass_PFcandidate_cut->Write();
  pi0pt_PFcandidate->Write();
  pi0p_PFcandidate->Write();
  cosTheta->Write();
  deltaE->Write();

  sumXtals_minusClusterEnergy->Write();
  sumXtals_minusClusterEnergy_vsEta->Write();
  photonClusterDeltaR->Write();
  xtalEnergy->Write();
  //xtalChi2->Write();
  xtalTime->Write();

  deltaE_vsE->Write();
  deltaE_vsE_prev->Write();  
  deltaE_vsEta->Write();

  clusterNXtals_vsPhoEneRatio->Write();

  pi0mass_vsEta->Write();
  dR_vsPi0mass->Write();

  nConvPerEvent->Write();
  nPhotonsPerEvent->Write();
  nGoodPhotonsPerEvent->Write();

  conversionMass->Write();

  dEta_vsDPhi->Write();
  dR_0_025->Write();
  dR_025_end->Write();
  dR_min_dist->Write();
  dR_min_dist_conv->Write();
  pairRECOp_onPairMCp->Write();
  fbrem_vsPairRECOp_onPairMCp->Write();
  firstHitPosition_vsPairRECOp_onPairMCp->Write();

  fbrem_distr->Write();
  firstHitPosition->Write();

  diPhotonInvMass->Write();

  Eexp_vsEmeas->Write();
  Eexp_vsEmeas_profile->Write();
  Eexp_onEmeas->Write();

  //MAPS
  pi0peak_vsEnergy -> Write();
  pi0peak_vsEta -> Write();

  file_->mkdir("deltaE_");
  file_->cd ("deltaE_");
  for(std::map<int,TH1F*>::const_iterator energyItr = deltaE_energyBin.begin(); energyItr != deltaE_energyBin.end(); ++energyItr)
    (energyItr->second) -> Write();
  for(std::map<int,TH1F*>::const_iterator etaItr = deltaE_etaBin.begin(); etaItr != deltaE_etaBin.end(); ++etaItr)
    (etaItr->second) -> Write();
  file_->cd("Plots");

  file_->mkdir("pi0peak_");
  file_->cd ("pi0peak_");
  for(std::map<int,TH1F*>::const_iterator energyItr = pi0peak_energyBin.begin(); energyItr != pi0peak_energyBin.end(); ++energyItr)
    (energyItr->second) -> Write();
  for(std::map<int,TH1F*>::const_iterator etaItr = pi0peak_etaBin.begin(); etaItr != pi0peak_etaBin.end(); ++etaItr)
    (etaItr->second) -> Write();

  file_->mkdir("Bins_");
  file_->cd ("Bins_");
  for(std::map<int,TH1F*>::const_iterator energyItr = energyBin.begin(); energyItr != energyBin.end(); ++energyItr)
    (energyItr->second) -> Write();
  for(std::map<int,TH1F*>::const_iterator etaItr = etaBin.begin(); etaItr != etaBin.end(); ++etaItr)
    (etaItr->second) -> Write();

  file_->mkdir("EcosTheta_");
  file_->cd ("EcosTheta_");
  for(std::map<int,TH1F*>::const_iterator energyItr = EcosTheta_energyBin.begin(); energyItr != EcosTheta_energyBin.end(); ++energyItr)
    (energyItr->second) -> Write();
  for(std::map<int,TH1F*>::const_iterator etaItr = EcosTheta_etaBin.begin(); etaItr != EcosTheta_etaBin.end(); ++etaItr)
    (etaItr->second) -> Write();

  file_->mkdir("phoEneRatio_");
  file_->cd ("phoEneRatio_");
  for(std::map<int,TH1F*>::const_iterator energyItr = phoEneRatio_energyBin.begin(); energyItr != phoEneRatio_energyBin.end(); ++energyItr)
    (energyItr->second) -> Write();
  for(std::map<int,TH1F*>::const_iterator etaItr = phoEneRatio_etaBin.begin(); etaItr != phoEneRatio_etaBin.end(); ++etaItr)
    (etaItr->second) -> Write();


  file_->cd();
  file_->Close();
  eneFile->Close();


  return 0 ;
}

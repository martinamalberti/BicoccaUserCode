#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCDumper.h"

#define PI 3.141592654

using namespace vbfhww2l;



VBFMCDumper::VBFMCDumper(const edm::ParameterSet& iConfig):
 srcHepMCProduct_p          (iConfig.getParameter<edm::InputTag>("srcHepMCProduct")),
 srcGenParticles_p          (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 srcGenMet_p                (iConfig.getParameter<edm::InputTag>("srcGenMet")),
 srcIC5GenJets_p            (iConfig.getParameter<edm::InputTag>("srcIC5GenJets")),
 srcIC5CaloJets_p           (iConfig.getParameter<edm::InputTag>("srcIC5CaloJets")),
 fileName_p                 (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFMCDumper"))),
 mass_p                     (iConfig.getParameter<std::string>("mass")),
 verbosity_p                (iConfig.getUntrackedParameter<bool>("verbosity", false)),
 eventsToPrint_p            (iConfig.getUntrackedParameter<int>("eventsToPrint", 0)),
 eventId_p(0),
 naiveId_p(0)
{}



VBFMCDumper::~VBFMCDumper()
{}






void VBFMCDumper::beginJob(const edm::EventSetup& iSetup)
{
  if(verbosity_p)
    std::cerr << "*** VBFMCDumper::beginJob ***" << std::endl;


  // Create File
  std::string path = "/afs/cern.ch/user/a/abenagli/scratch0/VBF/CMSSW_2_2_7/src/HiggsAnalysis/VBFHiggsToWWto2l2nu/test/";
  std::string outFileName = path+fileName_p+"_H"+mass_p+".root";
  outFile_p = new TFile(outFileName.c_str(), "RECREATE");
  
  
  
  // -----------------------------------------------------------------
  // define histograms
  // -----------------------------------------------------------------

  // decay modes
  mcHDecayId = new TH1F("mcHDecayId", "mcHDecayId", 2, 225, 227.);
  mcV1DecayId = new TH1F("mcV1DecayId", "mcV1DecayId", 14, 174., 188.);
  mcV2DecayId = new TH1F("mcV2DecayId", "mcV2DecayId", 14, 174., 188.);
  
  
  
  // higgs histograms
  mcH_charge = new TH1F("mcH_charge", "mcH_charge", 5, -2., 3.);
  mcH_mass = new TH1F("mcH_mass", "mcH_mass", 1000, 0., 1000.);
  mcH_energy = new TH1F("mcH_energy", "mcH_energy", 1000, 0., 1000.);
  mcH_p = new TH1F("mcH_p", "mcH_p", 1000, 0., 1000.);
  mcH_pT = new TH1F("mcH_pT", "mcH_pT", 1000, 0., 1000.);
  mcH_pL = new TH1F("mcH_pL", "mcH_pL", 1000, 0., 1000.);
  mcH_eta = new TH1F("mcH_eta", "mcH_eta", 1000, -5., 5.);
  mcH_phi = new TH1F("mcH_phi", "mcH_phi", 1000, -5., 5.);

  mcZZ_charge = new TH1F("mcZZ_charge", "mcZZ_charge", 5, -2., 3.);
  mcZZ_mass = new TH1F("mcZZ_mass", "mcZZ_mass", 1000, 0., 1000.);
  mcZZ_energy = new TH1F("mcZZ_energy", "mcZZ_energy", 1000, 0., 1000.);
  mcZZ_p = new TH1F("mcZZ_p", "mcZZ_p", 1000, 0., 1000.);
  mcZZ_pT = new TH1F("mcZZ_pT", "mcZZ_pT", 1000, 0., 1000.);
  mcZZ_pL = new TH1F("mcZZ_pL", "mcZZ_pL", 1000, 0., 1000.);
  mcZZ_eta = new TH1F("mcZZ_eta", "mcZZ_eta", 1000, -5., 5.);
  mcZZ_phi = new TH1F("mcZZ_phi", "mcZZ_phi", 1000, -5., 5.);

  mc4l_charge = new TH1F("mc4l_charge", "mc4l_charge", 5, -2., 3.);
  mc4l_mass = new TH1F("mc4l_mass", "mc4l_mass", 1000, 0., 1000.);
  mc4l_energy = new TH1F("mc4l_energy", "mc4l_energy", 1000, 0., 1000.);
  mc4l_p = new TH1F("mc4l_p", "mc4l_p", 1000, 0., 1000.);
  mc4l_pT = new TH1F("mc4l_pT", "mc4l_pT", 1000, 0., 1000.);
  mc4l_pL = new TH1F("mc4l_pL", "mc4l_pL", 1000, 0., 1000.);
  mc4l_eta = new TH1F("mc4l_eta", "mc4l_eta", 1000, -5., 5.);
  mc4l_phi = new TH1F("mc4l_phi", "mc4l_phi", 1000, -5., 5.);
  
  
  // vector boson histogram
  mcV1_charge = new TH1F("mcV1_charge", "mcV1_charge", 5, -2., 3.);
  mcV1_mass = new TH1F("mcV1_mass", "mcV1_mass", 1000, 0., 1000.);
  mcV1_energy = new TH1F("mcV1_energy", "mcV1_energy", 1000, 0., 1000.);
  mcV1_p = new TH1F("mcV1_p", "mcV1_p", 1000, 0., 1000.);
  mcV1_pT = new TH1F("mcV1_pT", "mcV1_pT", 1000, 0., 1000.);
  mcV1_pL = new TH1F("mcV1_pL", "mcV1_pL", 1000, 0., 1000.);
  mcV1_eta = new TH1F("mcV1_eta", "mcV1_eta", 1000, -5., 5.);
  mcV1_phi = new TH1F("mcV1_phi", "mcV1_phi", 1000, -5., 5.);

  mcFF_fromV1_charge = new TH1F("mcFF_fromV1_charge", "mcFF_fromV1_charge", 5, -2., 3.);
  mcFF_fromV1_mass = new TH1F("mcFF_fromV1_mass", "mcFF_fromV1_mass", 1000, 0., 1000.);
  mcFF_fromV1_energy = new TH1F("mcFF_fromV1_energy", "mcFF_fromV1_energy", 1000, 0., 1000.);
  mcFF_fromV1_p = new TH1F("mcFF_fromV1_p", "mcFF_fromV1_p", 1000, 0., 1000.);
  mcFF_fromV1_pT = new TH1F("mcFF_fromV1_pT", "mcFF_fromV1_pT", 1000, 0., 1000.);
  mcFF_fromV1_pL = new TH1F("mcFF_fromV1_pL", "mcFF_fromV1_pL", 1000, 0., 1000.);
  mcFF_fromV1_eta = new TH1F("mcFF_fromV1_eta", "mcFF_fromV1_eta", 1000, -5., 5.);
  mcFF_fromV1_phi = new TH1F("mcFF_fromV1_phi", "mcFF_fromV1_phi", 1000, -5., 5.);
  
  mcV2_charge = new TH1F("mcV2_charge", "mcV2_charge", 5, -2., 3.);
  mcV2_mass = new TH1F("mcV2_mass", "mcV2_mass", 1000, 0., 1000.);
  mcV2_energy = new TH1F("mcV2_energy", "mcV2_energy", 1000, 0., 1000.);
  mcV2_p = new TH1F("mcV2_p", "mcV2_p", 1000, 0., 1000.);
  mcV2_pT = new TH1F("mcV2_pT", "mcV2_pT", 1000, 0., 1000.);
  mcV2_pL = new TH1F("mcV2_pL", "mcV2_pL", 1000, 0., 1000.);
  mcV2_eta = new TH1F("mcV2_eta", "mcV2_eta", 1000, -5., 5.);
  mcV2_phi = new TH1F("mcV2_phi", "mcV2_phi", 1000, -5., 5.);

  mcFF_fromV2_charge = new TH1F("mcFF_fromV2_charge", "mcFF_fromV2_charge", 5, -2., 3.);
  mcFF_fromV2_mass = new TH1F("mcFF_fromV2_mass", "mcFF_fromV2_mass", 1000, 0., 1000.);
  mcFF_fromV2_energy = new TH1F("mcFF_fromV2_energy", "mcFF_fromV2_energy", 1000, 0., 1000.);
  mcFF_fromV2_p = new TH1F("mcFF_fromV2_p", "mcFF_fromV2_p", 1000, 0., 1000.);
  mcFF_fromV2_pT = new TH1F("mcFF_fromV2_pT", "mcFF_fromV2_pT", 1000, 0., 1000.);
  mcFF_fromV2_pL = new TH1F("mcFF_fromV2_pL", "mcFF_fromV2_pL", 1000, 0., 1000.);
  mcFF_fromV2_eta = new TH1F("mcFF_fromV2_eta", "mcFF_fromV2_eta", 1000, -5., 5.);
  mcFF_fromV2_phi = new TH1F("mcFF_fromV2_phi", "mcFF_fromV2_phi", 1000, -5., 5.);

  mcV_deltaPhi = new TH1F("mcV_deltaPhi", "mcV_deltaPhi", 1000, 0., PI);
  mcV_deltaEta = new TH1F("mcV_deltaEta", "mcV_deltaEta", 1000, 0., 10.);

  mcFF_deltaPhi = new TH1F("mcFF_deltaPhi", "mcFF_deltaPhi", 1000, 0., PI);
  mcFF_deltaEta = new TH1F("mcFF_deltaEta", "mcFF_deltaEta", 1000, 0., 10.);

  
  // fermion histograms
  mcF_fromV1_charge = new TH1F("mcF_fromV1_charge", "mcF_fromV1_charge", 5, -2., 3.);
  mcF_fromV1_mass = new TH1F("mcF_fromV1_mass", "mcF_fromV1_mass", 1000, -10., 10.);
  mcF_fromV1_energy = new TH1F("mcF_fromV1_energy", "mcF_fromV1_energy", 1000, 0., 1000.);
  mcF_fromV1_p = new TH1F("mcF_fromV1_p", "mcF_fromV1_p", 1000, 0., 1000.);
  mcF_fromV1_pT = new TH1F("mcF_fromV1_pT", "mcF_fromV1_pT", 1000, 0., 1000.);
  mcF_fromV1_pL = new TH1F("mcF_fromV1_pL", "mcF_fromV1_pL", 1000, 0., 1000.);
  mcF_fromV1_eta = new TH1F("mcF_fromV1_eta", "mcF_fromV1_eta", 1000, -5., 5.);
  mcF_fromV1_phi = new TH1F("mcF_fromV1_phi", "mcF_fromV1_phi", 1000, -5., 5.);

  mcF_fromV1_deltaPhi = new TH1F("mcF_fromV1_deltaPhi", "mcF_fromV1_deltaPhi", 1000, 0., PI);
  mcF_fromV1_deltaEta = new TH1F("mcF_fromV1_deltaEta", "mcF_fromV1_deltaEta", 1000, 0., 10.);

  mcF_fromV2_charge = new TH1F("mcF_fromV2_charge", "mcF_fromV2_charge", 5, -2., 3.);
  mcF_fromV2_mass = new TH1F("mcF_fromV2_mass", "mcF_fromV2_mass", 1000, -10., 10.);
  mcF_fromV2_energy = new TH1F("mcF_fromV2_energy", "mcF_fromV2_energy", 1000, 0., 1000.);
  mcF_fromV2_p = new TH1F("mcF_fromV2_p", "mcF_fromV2_p", 1000, 0., 1000.);
  mcF_fromV2_pT = new TH1F("mcF_fromV2_pT", "mcF_fromV2_pT", 1000, 0., 1000.);
  mcF_fromV2_pL = new TH1F("mcF_fromV2_pL", "mcF_fromV2_pL", 1000, 0., 1000.);
  mcF_fromV2_eta = new TH1F("mcF_fromV2_eta", "mcF_fromV2_eta", 1000, -5., 5.);
  mcF_fromV2_phi = new TH1F("mcF_fromV2_phi", "mcF_fromV2_phi", 1000, -5., 5.);

  mcF_fromV2_deltaPhi = new TH1F("mcF_fromV2_deltaPhi", "mcF_fromV2_deltaPhi", 1000, 0., PI);
  mcF_fromV2_deltaEta = new TH1F("mcF_fromV2_deltaEta", "mcF_fromV2_deltaEta", 1000, 0., 10.);


  // iC5GenJets histograms
  iC5GenJets_n = new TH1F("iC5GenJets_n", "iC5GenJets_n", 100, 0., 100.);
  iC5GenJets_charge = new TH1F("iC5GenJets_charge", "iC5GenJets_charge", 5, -2., 3.);
  iC5GenJets_mass = new TH1F("iC5GenJets_mass", "iC5GenJets_mass", 1000, 0., 500.);
  iC5GenJets_energy = new TH1F("iC5GenJets_energy", "iC5GenJets_energy", 1000, 0., 1000.);
  iC5GenJets_p = new TH1F("iC5GenJets_p", "iC5GenJets_p", 1000, 0., 1000.);
  iC5GenJets_pT = new TH1F("iC5GenJets_pT", "iC5GenJets_pT", 1000, 0., 1000.);
  iC5GenJets_pL = new TH1F("iC5GenJets_pL", "iC5GenJets_pL", 1000, 0., 1000.);
  iC5GenJets_phi = new TH1F("iC5GenJets_phi", "iC5GenJets_phi", 1000, -5., 5.);
  iC5GenJets_eta = new TH1F("iC5GenJets_eta", "iC5GenJets_eta", 1000, -5., 5.);

  iC5GenJets_mcF_fromV2_DR = new TH1F("iC5GenJets_mcF_fromV2_DR", "iC5GenJets_mcF_fromV2_DR", 10000, 0., 10.);
  
  
  // matched genJets histograms
  iC5GenJets_mcF_fromV2_matchEff = new TProfile("iC5GenJets_mcF_fromV2_matchEff", "iC5GenJets_mcF_fromV2_matchEff", 3, 0., 3.);
  iC5GenJets_mcF_fromV2_matchEff_vsPt = new TProfile("iC5GenJets_mcF_fromV2_matchEff_vsPt", "iC5GenJets_mcF_fromV2_matchEff_vsPt", 200, 0., 1000.);
  iC5GenJets_mcF_fromV2_matchEff_vsEta = new TProfile("iC5GenJets_mcF_fromV2_matchEff_vsEta", "iC5GenJets_mcF_fromV2_matchEff_vsEta", 200, 0., 5.);

  mcJJ_fromV2_charge = new TH1F("mcJJ_fromV2_charge", "mcJJ_fromV2_charge", 5, -2., 3.);
  mcJJ_fromV2_mass = new TH1F("mcJJ_fromV2_mass", "mcJJ_fromV2_mass", 1000, 0., 500.);
  mcJJ_fromV2_energy = new TH1F("mcJJ_fromV2_energy", "mcJJ_fromV2_energy", 1000, 0., 1000.);
  mcJJ_fromV2_p = new TH1F("mcJJ_fromV2_p", "mcJJ_fromV2_p", 1000, 0., 1000.);
  mcJJ_fromV2_pT = new TH1F("mcJJ_fromV2_pT", "mcJJ_fromV2_pT", 1000, 0., 1000.);
  mcJJ_fromV2_pL = new TH1F("mcJJ_fromV2_pL", "mcJJ_fromV2_pL", 1000, 0., 1000.);
  mcJJ_fromV2_phi = new TH1F("mcJJ_fromV2_phi", "mcJJ_fromV2_phi", 1000, -5., 5.);
  mcJJ_fromV2_eta = new TH1F("mcJJ_fromV2_eta", "mcJJ_fromV2_eta", 1000, -5., 5.);

  mcJ_fromV2_charge = new TH1F("mcJ_fromV2_charge", "mcJ_fromV2_charge", 5, -2., 3.);
  mcJ_fromV2_mass = new TH1F("mcJ_fromV2_mass", "mcJ_fromV2_mass", 1000, 0., 500.);
  mcJ_fromV2_energy = new TH1F("mcJ_fromV2_energy", "mcJ_fromV2_energy", 1000, 0., 1000.);
  mcJ_fromV2_p = new TH1F("mcJ_fromV2_p", "mcJ_fromV2_p", 1000, 0., 1000.);
  mcJ_fromV2_pT = new TH1F("mcJ_fromV2_pT", "mcJ_fromV2_pT", 1000, 0., 1000.);
  mcJ_fromV2_pL = new TH1F("mcJ_fromV2_pL", "mcJ_fromV2_pL", 1000, 0., 1000.);
  mcJ_fromV2_phi = new TH1F("mcJ_fromV2_phi", "mcJ_fromV2_phi", 1000, -5., 5.);
  mcJ_fromV2_eta = new TH1F("mcJ_fromV2_eta", "mcJ_fromV2_eta", 1000, -5., 5.);
  
  
  // iC5CaloJets histograms
  iC5CaloJets_n = new TH1F("iC5CaloJets_n", "iC5CaloJets_n", 100, 0., 100.);
  iC5CaloJets_charge = new TH1F("iC5CaloJets_charge", "iC5CaloJets_charge", 5, -2., 3.);
  iC5CaloJets_mass = new TH1F("iC5CaloJets_mass", "iC5CaloJets_mass", 1000, 0., 500.);
  iC5CaloJets_energy = new TH1F("iC5CaloJets_energy", "iC5CaloJets_energy", 1000, 0., 1000.);
  iC5CaloJets_p = new TH1F("iC5CaloJets_p", "iC5CaloJets_p", 1000, 0., 1000.);
  iC5CaloJets_pT = new TH1F("iC5CaloJets_pT", "iC5CaloJets_pT", 1000, 0., 1000.);
  iC5CaloJets_pL = new TH1F("iC5CaloJets_pL", "iC5CaloJets_pL", 1000, 0., 1000.);
  iC5CaloJets_phi = new TH1F("iC5CaloJets_phi", "iC5CaloJets_phi", 1000, -5., 5.);
  iC5CaloJets_eta = new TH1F("iC5CaloJets_eta", "iC5CaloJets_eta", 1000, -5., 5.);



}



void VBFMCDumper::endJob()
{
  if(verbosity_p)
    std::cerr << "*** VBFMCDumper::endJob ***" << std::endl;

  // Save histograms
  outFile_p -> cd();
  //TDirectory* sd1;
  
  
  
  // decay modes
  mcHDecayId -> Write();
  mcV1DecayId -> Write();
  mcV2DecayId -> Write();
  
  
  // higgs histograms
  outFile_p -> mkdir("mcH");
  outFile_p -> cd("mcH");

  mcH_charge-> Write();
  mcH_mass -> Write();
  mcH_energy -> Write();
  mcH_p -> Write();
  mcH_pT -> Write();
  mcH_pL -> Write();
  mcH_phi -> Write();
  mcH_eta -> Write();

  mcZZ_charge -> Write();
  mcZZ_mass -> Write();
  mcZZ_energy -> Write();
  mcZZ_p -> Write();
  mcZZ_pT -> Write();
  mcZZ_pL -> Write();
  mcZZ_phi -> Write();
  mcZZ_eta -> Write();

  mc4l_charge -> Write();
  mc4l_mass -> Write();
  mc4l_energy -> Write();
  mc4l_p -> Write();
  mc4l_pT -> Write();
  mc4l_pL -> Write();
  mc4l_phi -> Write();
  mc4l_eta -> Write();

  outFile_p -> cd();
  

  // Z boson histograms
  outFile_p -> mkdir("mcV");
  outFile_p -> cd("mcV");

  mcV1_charge -> Write();
  mcV1_mass -> Write();
  mcV1_energy -> Write();
  mcV1_p -> Write();
  mcV1_pT -> Write();
  mcV1_pL -> Write();
  mcV1_phi -> Write();
  mcV1_eta -> Write();

  mcFF_fromV1_charge -> Write();
  mcFF_fromV1_mass -> Write();
  mcFF_fromV1_energy -> Write();
  mcFF_fromV1_p -> Write();
  mcFF_fromV1_pT -> Write();
  mcFF_fromV1_pL -> Write();
  mcFF_fromV1_phi -> Write();
  mcFF_fromV1_eta -> Write();
  
  mcV2_charge -> Write();
  mcV2_mass -> Write();
  mcV2_energy -> Write();
  mcV2_p -> Write();
  mcV2_pT -> Write();
  mcV2_pL -> Write();
  mcV2_phi -> Write();
  mcV2_eta -> Write();
  
  mcFF_fromV2_charge -> Write();
  mcFF_fromV2_mass -> Write();
  mcFF_fromV2_energy -> Write();
  mcFF_fromV2_p -> Write();
  mcFF_fromV2_pT -> Write();
  mcFF_fromV2_pL -> Write();
  mcFF_fromV2_phi -> Write();
  mcFF_fromV2_eta -> Write();

  mcV_deltaPhi -> Write();
  mcV_deltaEta -> Write();

  mcFF_deltaPhi -> Write();
  mcFF_deltaEta -> Write();

  outFile_p -> cd ();
  
  
  // fermion histograms
  outFile_p -> mkdir("mcF_fromV1");
  outFile_p -> cd("mcF_fromV1");

  mcF_fromV1_charge -> Write();
  mcF_fromV1_mass -> Write();
  mcF_fromV1_energy -> Write();
  mcF_fromV1_p -> Write();
  mcF_fromV1_pT -> Write();
  mcF_fromV1_pL -> Write();
  mcF_fromV1_phi -> Write();
  mcF_fromV1_eta -> Write();

  mcF_fromV1_deltaPhi -> Write();
  mcF_fromV1_deltaEta -> Write();

  outFile_p -> cd ();


  outFile_p -> mkdir("mcF_fromV2");
  outFile_p -> cd("mcF_fromV2");

  mcF_fromV2_charge -> Write();
  mcF_fromV2_mass -> Write();
  mcF_fromV2_energy -> Write();
  mcF_fromV2_p -> Write();
  mcF_fromV2_pT -> Write();
  mcF_fromV2_pL -> Write();
  mcF_fromV2_phi -> Write();
  mcF_fromV2_eta -> Write();

  mcF_fromV2_deltaPhi -> Write();
  mcF_fromV2_deltaEta -> Write();

  outFile_p -> cd ();


  // iC5GenJets histograms
  outFile_p -> mkdir("iC5GenJets");
  outFile_p -> cd("iC5GenJets");

  iC5GenJets_n -> Write();
  iC5GenJets_charge -> Write();  
  iC5GenJets_mass -> Write();  
  iC5GenJets_energy -> Write();  
  iC5GenJets_p -> Write();  
  iC5GenJets_pT -> Write();  
  iC5GenJets_pL -> Write();  
  iC5GenJets_eta -> Write();  
  iC5GenJets_phi -> Write();

  iC5GenJets_mcF_fromV2_DR -> Write();

  iC5GenJets_mcF_fromV2_matchEff -> Write();
  iC5GenJets_mcF_fromV2_matchEff_vsPt -> Write();
  iC5GenJets_mcF_fromV2_matchEff_vsEta -> Write();

  outFile_p -> cd();
  
  
  // iC5GenJets histograms
  outFile_p -> mkdir("matchedGenJet");
  outFile_p -> cd("matchedGenJet");

  mcJJ_fromV2_charge -> Write();  
  mcJJ_fromV2_mass -> Write();  
  mcJJ_fromV2_energy -> Write();  
  mcJJ_fromV2_p -> Write();  
  mcJJ_fromV2_pT -> Write();  
  mcJJ_fromV2_pL -> Write();  
  mcJJ_fromV2_eta -> Write();  
  mcJJ_fromV2_phi -> Write();

  mcJ_fromV2_charge -> Write();  
  mcJ_fromV2_mass -> Write();  
  mcJ_fromV2_energy -> Write();  
  mcJ_fromV2_p -> Write();  
  mcJ_fromV2_pT -> Write();  
  mcJ_fromV2_pL -> Write();  
  mcJ_fromV2_eta -> Write();  
  mcJ_fromV2_phi -> Write();


  // iC5CaloJets histograms
  outFile_p -> mkdir("iC5CaloJets");
  outFile_p -> cd("iC5CaloJets");

  iC5CaloJets_n -> Write();
  iC5CaloJets_charge -> Write();  
  iC5CaloJets_mass -> Write();  
  iC5CaloJets_energy -> Write();  
  iC5CaloJets_p -> Write();  
  iC5CaloJets_pT -> Write();  
  iC5CaloJets_pL -> Write();  
  iC5CaloJets_eta -> Write();  
  iC5CaloJets_phi -> Write();

  outFile_p -> cd();  
  


  outFile_p -> Close();
}






void VBFMCDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if(verbosity_p)
    std::cerr << "*** VBFMCDumper::analyze ***" << std::endl;
  
  
  // set event id
  eventId_p = (iEvent.id()).event();
  ++naiveId_p;
  
  if( (naiveId_p%100 == 0) && (verbosity_p == true) )
    std::cout << ">>>>>> VBFMCDumper   Analyze event number: " << naiveId_p << std::endl;






  // Get the collections
  //edm::Handle<edm::HepMCProduct> hHepMCProduct;
  //iEvent.getByLabel(srcHepMCProduct_p, hHepMCProduct);
  //if(! (hHepMCProduct.isValid ()) )
  //{
  //  std::cerr << ">>>>>> VBFMCDumper::Warning: " << srcHepMCProduct_p << " not available" << std::endl;
  //  return;    
  //}

  edm::Handle<reco::GenParticleCollection> hGenParticles;
  iEvent.getByLabel(srcGenParticles_p, hGenParticles);
  if( !(hGenParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMCDumper::Warning: " << srcGenParticles_p << " not available" << std::endl;
    return;
  }

  edm::ESHandle<ParticleDataTable> hPdt;
  iSetup.getData(hPdt);

  edm::Handle<reco::GenMETCollection> hGenMet;
  iEvent.getByLabel(srcGenMet_p, hGenMet);
  if( !(hGenMet.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMCDumper::Warning: " << srcGenMet_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::GenJetCollection> hIC5GenJets;
  iEvent.getByLabel(srcIC5GenJets_p, hIC5GenJets);
  if( !(hIC5GenJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMCDumper::Warning: " << srcIC5GenJets_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::CaloJetCollection> hIC5CaloJets;
  iEvent.getByLabel (srcIC5CaloJets_p, hIC5CaloJets) ;
  if( !(hIC5CaloJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMCDumper::Warning: " << srcIC5CaloJets_p << " not available" << std::endl;
    return;
  }


  
  
  
  
  // -----------------------------------------------------------------
  // mc analysis
  // -----------------------------------------------------------------  

  MCAnalysis mcAnalysis(hGenParticles, hPdt, false);
  bool isMCAnalysisValid = mcAnalysis.isValid();
  if(!isMCAnalysisValid) return;

  const reco::Candidate* mcH = mcAnalysis.mcH();
  const reco::Candidate* mcV1 = mcAnalysis.mcV1();
  const reco::Candidate* mcV2 = mcAnalysis.mcV2();
  const reco::Candidate* mcF_fromV1 = mcAnalysis.mcF_fromV1();
  const reco::Candidate* mcFbar_fromV1 = mcAnalysis.mcFbar_fromV1();
  const reco::Candidate* mcF_fromV2 = mcAnalysis.mcF_fromV2();
  const reco::Candidate* mcFbar_fromV2 = mcAnalysis.mcFbar_fromV2();
 
  const reco::Particle * mcZZ = new reco::Particle(0, mcV1 -> p4() + mcV2 -> p4(), mcH -> vertex(), 25);
  const reco::Particle * mc4l = new reco::Particle(0, mcF_fromV1 -> p4() + mcFbar_fromV1 -> p4() +
						      mcF_fromV2 -> p4() + mcFbar_fromV2 -> p4(),
                                                      mcH -> vertex(), 25);
  const reco::Particle * mcFF_fromV1 = new reco::Particle(0, mcF_fromV1 -> p4() + mcFbar_fromV1 -> p4(),
                                                          mcH -> vertex(), 23);
  const reco::Particle * mcFF_fromV2 = new reco::Particle(0, mcF_fromV2 -> p4() + mcFbar_fromV2 -> p4(),
                                                          mcH -> vertex(), 23);


 

  if(false)
  {
    //mcAnalysis.PrintHDecayTree(hPdt);

    std::cout << "******************* SUMMARY *******************\n" << std::endl;
    
    std::cout << "Higgs:            H -> " << mcAnalysis.mcHDecayMode() << std::endl;
    PrintParticleInfo(mcAnalysis.mcH(), hPdt);
    std::cout << std::endl;
    
    std::cout << "Vector bosons:   V1 -> " << mcAnalysis.mcV1DecayMode() << std::endl;
    PrintParticleInfo(mcAnalysis.mcV1(), hPdt);

    std::cout << "Vector bosons:   V2 -> " << mcAnalysis.mcV2DecayMode() << std::endl;
    PrintParticleInfo(mcAnalysis.mcV2(), hPdt);
    std::cout << std::endl;
    
    std::cout << "Fermions from vector bosons: " << std::endl;
    PrintParticleInfo(mcAnalysis.mcF_fromV1(), hPdt);
    PrintParticleInfo(mcAnalysis.mcFbar_fromV1(), hPdt);
    PrintParticleInfo(mcAnalysis.mcF_fromV2(), hPdt);
    PrintParticleInfo(mcAnalysis.mcFbar_fromV2(), hPdt);
    std::cout << "******************* SUMMARY *******************\n" << std::endl;
  }


  
  
  
  
  // -----------------------------------------------------------------
  // fill histograms
  // -----------------------------------------------------------------  

  // decay mode histograms
  mcHDecayId -> Fill(mcAnalysis.mcHDecayId());
  mcV1DecayId -> Fill(mcAnalysis.mcV1DecayId());
  mcV2DecayId -> Fill(mcAnalysis.mcV2DecayId());


  // higgs histograms
  mcH_charge -> Fill(mcH -> charge());
  mcH_mass -> Fill(mcH -> mass());
  mcH_energy -> Fill(mcH -> energy());
  mcH_p -> Fill(mcH -> p());
  mcH_pT -> Fill(mcH -> pt());
  mcH_pL -> Fill(fabs(mcH -> pz()));
  mcH_phi -> Fill(mcH -> phi());
  mcH_eta -> Fill(mcH -> eta());

  mcZZ_charge -> Fill(mcZZ -> charge());
  mcZZ_mass -> Fill(mcZZ -> mass());
  mcZZ_energy -> Fill(mcZZ -> energy());
  mcZZ_p -> Fill(mcZZ -> p());
  mcZZ_pT -> Fill(mcZZ -> pt());
  mcZZ_pL -> Fill(fabs(mcZZ -> pz()));
  mcZZ_phi -> Fill(mcZZ -> phi());
  mcZZ_eta -> Fill(mcZZ -> eta());

  mc4l_charge -> Fill(mc4l -> charge());
  mc4l_mass -> Fill(mc4l -> mass());
  mc4l_energy -> Fill(mc4l -> energy());
  mc4l_p -> Fill(mc4l -> p());
  mc4l_pT -> Fill(mc4l -> pt());
  mc4l_pL -> Fill(fabs(mc4l -> pz()));
  mc4l_phi -> Fill(mc4l -> phi());
  mc4l_eta -> Fill(mc4l -> eta());
  
  
  // vector boson histograms
  mcV1_charge -> Fill(mcV1 -> charge());
  mcV1_mass -> Fill(mcV1 -> mass());
  mcV1_energy -> Fill(mcV1 -> energy());
  mcV1_p -> Fill(mcV1 -> p());
  mcV1_pT -> Fill(mcV1 -> pt());
  mcV1_pL -> Fill(fabs(mcV1 -> pz()));
  mcV1_phi -> Fill(mcV1 -> phi());
  mcV1_eta -> Fill(mcV1 -> eta());

  mcFF_fromV1_charge -> Fill(mcFF_fromV1 -> charge());
  mcFF_fromV1_mass -> Fill(mcFF_fromV1 -> mass());
  mcFF_fromV1_energy -> Fill(mcFF_fromV1 -> energy());
  mcFF_fromV1_p -> Fill(mcFF_fromV1 -> p());
  mcFF_fromV1_pT -> Fill(mcFF_fromV1 -> pt());
  mcFF_fromV1_pL -> Fill(fabs(mcFF_fromV1 -> pz()));
  mcFF_fromV1_phi -> Fill(mcFF_fromV1 -> phi());
  mcFF_fromV1_eta -> Fill(mcFF_fromV1 -> eta());

  mcV2_charge -> Fill(mcV2 -> charge());
  mcV2_mass -> Fill(mcV2 -> mass());
  mcV2_energy -> Fill(mcV2 -> energy());
  mcV2_p -> Fill(mcV2 -> p());
  mcV2_pT -> Fill(mcV2 -> pt());
  mcV2_pL -> Fill(fabs(mcV2 -> pz()));
  mcV2_phi -> Fill(mcV2 -> phi());
  mcV2_eta -> Fill(mcV2 -> eta());

  mcFF_fromV2_charge -> Fill(mcFF_fromV2 -> charge());
  mcFF_fromV2_mass -> Fill(mcFF_fromV2 -> mass());
  mcFF_fromV2_energy -> Fill(mcFF_fromV2 -> energy());
  mcFF_fromV2_p -> Fill(mcFF_fromV2 -> p());
  mcFF_fromV2_pT -> Fill(mcFF_fromV2 -> pt());
  mcFF_fromV2_pL -> Fill(fabs(mcFF_fromV2 -> pz()));
  mcFF_fromV2_phi -> Fill(mcFF_fromV2 -> phi());
  mcFF_fromV2_eta -> Fill(mcFF_fromV2 -> eta());

  float dphi = deltaPhi(mcV1 -> phi(), mcV2 -> phi());
  float deta = fabs(mcV1 -> eta() - mcV2 -> eta());
  mcV_deltaPhi -> Fill(dphi);
  mcV_deltaEta -> Fill(deta);

  dphi = deltaPhi(mcFF_fromV1 -> phi(), mcFF_fromV2 -> phi());
  deta = fabs(mcFF_fromV1 -> eta() - mcFF_fromV2 -> eta());
  mcFF_deltaPhi -> Fill(dphi);
  mcFF_deltaEta -> Fill(deta);


  // fermion histograms
  mcF_fromV1_charge -> Fill(mcF_fromV1    -> charge());
  mcF_fromV1_charge -> Fill(mcFbar_fromV1 -> charge());
  mcF_fromV1_mass -> Fill(mcF_fromV1    -> mass());
  mcF_fromV1_mass -> Fill(mcFbar_fromV1 -> mass());
  mcF_fromV1_energy -> Fill(mcF_fromV1    -> energy());
  mcF_fromV1_energy -> Fill(mcFbar_fromV1 -> energy());
  mcF_fromV1_p -> Fill(mcF_fromV1    -> p());
  mcF_fromV1_p -> Fill(mcFbar_fromV1 -> p());
  mcF_fromV1_pT -> Fill(mcF_fromV1    -> pt());
  mcF_fromV1_pT -> Fill(mcFbar_fromV1 -> pt());
  mcF_fromV1_pL -> Fill(fabs(mcF_fromV1    -> pz()));
  mcF_fromV1_pL -> Fill(fabs(mcFbar_fromV1 -> pz()));
  mcF_fromV1_phi -> Fill(mcF_fromV1    -> phi());
  mcF_fromV1_phi -> Fill(mcFbar_fromV1 -> phi());
  mcF_fromV1_eta -> Fill(mcF_fromV1    -> eta());  
  mcF_fromV1_eta -> Fill(mcFbar_fromV1 -> eta());  
  
  dphi = deltaPhi(mcF_fromV1 -> phi(), mcFbar_fromV1 -> phi());
  deta = fabs(mcF_fromV1 -> eta() - mcFbar_fromV1 -> eta());
  mcF_fromV1_deltaPhi -> Fill(dphi);
  mcF_fromV1_deltaEta -> Fill(deta);


  mcF_fromV2_charge -> Fill(mcF_fromV2    -> charge());
  mcF_fromV2_charge -> Fill(mcFbar_fromV2 -> charge());
  mcF_fromV2_mass -> Fill(mcF_fromV2    -> mass());
  mcF_fromV2_mass -> Fill(mcFbar_fromV2 -> mass());
  mcF_fromV2_energy -> Fill(mcF_fromV2    -> energy());
  mcF_fromV2_energy -> Fill(mcFbar_fromV2 -> energy());
  mcF_fromV2_p -> Fill(mcF_fromV2    -> p());
  mcF_fromV2_p -> Fill(mcFbar_fromV2 -> p());
  mcF_fromV2_pT -> Fill(mcF_fromV2    -> pt());
  mcF_fromV2_pT -> Fill(mcFbar_fromV2 -> pt());
  mcF_fromV2_pL -> Fill(fabs(mcF_fromV2    -> pz()));
  mcF_fromV2_pL -> Fill(fabs(mcFbar_fromV2 -> pz()));
  mcF_fromV2_phi -> Fill(mcF_fromV2    -> phi());
  mcF_fromV2_phi -> Fill(mcFbar_fromV2 -> phi());
  mcF_fromV2_eta -> Fill(mcF_fromV2    -> eta());  
  mcF_fromV2_eta -> Fill(mcFbar_fromV2 -> eta());  
  
  dphi = deltaPhi(mcF_fromV2 -> phi(), mcFbar_fromV2 -> phi());
  deta = fabs(mcF_fromV2 -> eta() - mcFbar_fromV2 -> eta());
  mcF_fromV2_deltaPhi -> Fill(dphi);
  mcF_fromV2_deltaEta -> Fill(deta);
  
  
  // iC5GenJets histograms
  iC5GenJets_n -> Fill(hIC5GenJets -> size());
  for(reco::GenJetCollection::const_iterator it = hIC5GenJets -> begin();
      it != hIC5GenJets -> end(); ++it)
  {
    std::vector<const GenParticle*> iC5GenJetConstituents = it -> getGenConstituents();
    int charge = 0;
    for(std::vector<const GenParticle*>::const_iterator it2 = iC5GenJetConstituents.begin();
       	it2 != iC5GenJetConstituents.end(); ++it2)
      charge += (*it2) -> charge();

    iC5GenJets_charge -> Fill(charge);  
    iC5GenJets_mass -> Fill(it -> mass());  
    iC5GenJets_energy -> Fill(it -> energy());  
    iC5GenJets_p -> Fill(it -> p());  
    iC5GenJets_pT -> Fill(it -> pt());  
    iC5GenJets_pL -> Fill(it -> pz());  
    iC5GenJets_phi -> Fill(it -> phi());    
    iC5GenJets_eta -> Fill(it -> eta());  
  }






  // match 
  bool isMcF_fromV2_matched = false;
  float DRmax_mcF = 0.3;
  reco::GenJetCollection::const_iterator iC5GenJet_mcF_fromV2_matchIt = hIC5GenJets -> end();

  bool isMcFbar_fromV2_matched = false;
  float DRmax_mcFbar = 0.3;
  reco::GenJetCollection::const_iterator iC5GenJet_mcFbar_fromV2_matchIt = hIC5GenJets -> end();

  mcAnalysis.GetMatchJets_fromV2(iC5GenJet_mcF_fromV2_matchIt,
                                 iC5GenJet_mcFbar_fromV2_matchIt,
                                 DRmax_mcF,
                                 DRmax_mcFbar,
                                 hIC5GenJets);
  
  
  
  
  if( (iC5GenJet_mcF_fromV2_matchIt != hIC5GenJets -> end()) &&
      (iC5GenJet_mcFbar_fromV2_matchIt != hIC5GenJets -> end()) && 
      (iC5GenJet_mcF_fromV2_matchIt != iC5GenJet_mcFbar_fromV2_matchIt) )
  {
    isMcF_fromV2_matched = true;
    isMcFbar_fromV2_matched = true;

    const reco::Particle* mcJJ_fromV2 = new reco::Particle(0, iC5GenJet_mcF_fromV2_matchIt -> p4() +
                                                              iC5GenJet_mcFbar_fromV2_matchIt -> p4(),
                                                           mcV2 -> vertex(), 23);
    
    mcJJ_fromV2_charge -> Fill(mcJJ_fromV2 -> charge());
    mcJJ_fromV2_mass -> Fill(mcJJ_fromV2 -> mass());
    mcJJ_fromV2_energy -> Fill(mcJJ_fromV2 -> energy());
    mcJJ_fromV2_p -> Fill(mcJJ_fromV2 -> p());
    mcJJ_fromV2_pT -> Fill(mcJJ_fromV2 -> pt());
    mcJJ_fromV2_pL -> Fill(fabs(mcJJ_fromV2 -> pz()));
    mcJJ_fromV2_phi -> Fill(mcJJ_fromV2 -> phi());
    mcJJ_fromV2_eta -> Fill(mcJJ_fromV2 -> eta());

    mcJ_fromV2_charge -> Fill(iC5GenJet_mcF_fromV2_matchIt -> charge());
    mcJ_fromV2_charge -> Fill(iC5GenJet_mcFbar_fromV2_matchIt -> charge());
    mcJ_fromV2_mass -> Fill(iC5GenJet_mcF_fromV2_matchIt -> mass());
    mcJ_fromV2_mass -> Fill(iC5GenJet_mcFbar_fromV2_matchIt -> mass());
    mcJ_fromV2_energy -> Fill(iC5GenJet_mcF_fromV2_matchIt -> energy());
    mcJ_fromV2_energy -> Fill(iC5GenJet_mcFbar_fromV2_matchIt -> energy());
    mcJ_fromV2_p -> Fill(iC5GenJet_mcF_fromV2_matchIt -> p());
    mcJ_fromV2_p -> Fill(iC5GenJet_mcFbar_fromV2_matchIt -> p());
    mcJ_fromV2_pT -> Fill(iC5GenJet_mcF_fromV2_matchIt -> pt());
    mcJ_fromV2_pT -> Fill(iC5GenJet_mcFbar_fromV2_matchIt -> pt());
    mcJ_fromV2_pL -> Fill(fabs(iC5GenJet_mcF_fromV2_matchIt -> pz()));
    mcJ_fromV2_pL -> Fill(fabs(iC5GenJet_mcFbar_fromV2_matchIt -> pz()));
    mcJ_fromV2_phi -> Fill(iC5GenJet_mcF_fromV2_matchIt -> phi());
    mcJ_fromV2_phi -> Fill(iC5GenJet_mcFbar_fromV2_matchIt -> phi());
    mcJ_fromV2_eta -> Fill(iC5GenJet_mcF_fromV2_matchIt -> eta());
    mcJ_fromV2_eta -> Fill(iC5GenJet_mcFbar_fromV2_matchIt -> eta());

    //std::cout << ">>>>> jet_mcF:    DR = " << DRmin_mcF << "   ";
    //PrintParticleInfo(&(*iC5GenJet_mcF_fromV2_matchIt), hPdt);
    //std::cout << ">>>>> jet_mcFbar: DR = " << DRmin_mcFbar << "   ";
    //PrintParticleInfo(&(*iC5GenJet_mcFbar_fromV2_matchIt), hPdt);

  } 

  if( (iC5GenJet_mcF_fromV2_matchIt != hIC5GenJets -> end()) &&
      (iC5GenJet_mcFbar_fromV2_matchIt == hIC5GenJets -> end()) )
  {
    isMcF_fromV2_matched = true;

    //std::cout << ">>>>> jet_mcF:    DR = " << DRmin_mcF << "   ";
    //PrintParticleInfo(&(*iC5GenJet_mcF_fromV2_matchIt), hPdt);
  }
  
  if( (iC5GenJet_mcF_fromV2_matchIt == hIC5GenJets -> end()) &&
      (iC5GenJet_mcFbar_fromV2_matchIt != hIC5GenJets -> end()) )
  {
    isMcFbar_fromV2_matched = true; 
    
    //std::cout << ">>>>> jet_mcFbar: DR = " << DRmin_mcFbar << "   ";
    //PrintParticleInfo(&(*iC5GenJet_mcFbar_fromV2_matchIt), hPdt);
  }
  
  
  
  
  if( (isMcF_fromV2_matched == true) && (isMcFbar_fromV2_matched == true) )
  {
    iC5GenJets_mcF_fromV2_matchEff -> Fill(0., 0.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(1., 0.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(2., 1.);

    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcF_fromV2 -> pt(), 1.);
    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcFbar_fromV2 -> pt(), 1.);

    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcF_fromV2 -> eta()), 1.);
    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcFbar_fromV2 -> eta()), 1.);
  }

  else if( (isMcF_fromV2_matched == true) && (isMcFbar_fromV2_matched == false) )  
  {
    iC5GenJets_mcF_fromV2_matchEff -> Fill(0., 0.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(1., 1.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(2., 0.);

    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcF_fromV2 -> pt(), 1.);
    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcFbar_fromV2 -> pt(), 0.);

    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcF_fromV2 -> eta()), 1.);
    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcFbar_fromV2 -> eta()), 0.);
  }

  else if( (isMcF_fromV2_matched == false) && (isMcFbar_fromV2_matched == true) )  
  {
    iC5GenJets_mcF_fromV2_matchEff -> Fill(0., 0.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(1., 1.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(2., 0.);

    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcF_fromV2 -> pt(), 0.);
    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcFbar_fromV2 -> pt(), 1.);

    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcF_fromV2 -> eta()), 0.);
    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcFbar_fromV2 -> eta()), 1.);
  }

  else
  {
    iC5GenJets_mcF_fromV2_matchEff -> Fill(0., 1.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(1., 0.);
    iC5GenJets_mcF_fromV2_matchEff -> Fill(2., 0.);

    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcF_fromV2 -> pt(), 0.);
    iC5GenJets_mcF_fromV2_matchEff_vsPt -> Fill(mcFbar_fromV2 -> pt(), 0.);

    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcF_fromV2 -> eta()), 0.);
    iC5GenJets_mcF_fromV2_matchEff_vsEta -> Fill(fabs(mcFbar_fromV2 -> eta()), 0.);
  }
  
  
  
  
  
  
  
  
  // genMet
//   CLHEP::Hep3Vector genMetDummy;
//   for(reco::GenMETCollection::const_iterator it = hGenMet -> begin(); it != hGenMet -> end(); ++it)
//   { 
//     CLHEP::Hep3Vector momentumDummy((*it).px(), (*it).py(), (*it).pz());
//     genMetDummy += momentumDummy;
//   }  
  
//   genMet_pT -> Fill(genMetDummy.mag());
  
  
  
  
  // iC5CaloJets histograms
  iC5CaloJets_n -> Fill(hIC5CaloJets -> size());
  for(reco::CaloJetCollection::const_iterator it = hIC5CaloJets -> begin();
      it != hIC5CaloJets -> end(); ++it)
  {
    iC5CaloJets_charge -> Fill(it -> charge());  
    iC5CaloJets_mass -> Fill(it -> mass());  
    iC5CaloJets_energy -> Fill(it -> energy());  
    iC5CaloJets_p -> Fill(it -> p());  
    iC5CaloJets_pT -> Fill(it -> pt());  
    iC5CaloJets_pL -> Fill(it -> pz());  
    iC5CaloJets_phi -> Fill(it -> phi());    
    iC5CaloJets_eta -> Fill(it -> eta());  
  }  
  
  

}


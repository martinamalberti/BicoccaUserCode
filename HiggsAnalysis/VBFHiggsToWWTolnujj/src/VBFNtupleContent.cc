#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"



void SetBranchAddresses(TTree* tree, VBFNtupleContent& treeVars)
{
  tree -> SetBranchAddress("eventType", &treeVars.eventType);
  tree -> SetBranchAddress("eventId",   &treeVars.eventId);
  
  
  
  // mc variables
  tree -> SetBranchAddress("mcH_energy", &treeVars.mcH_energy);
  tree -> SetBranchAddress("mcH_px",     &treeVars.mcH_px);
  tree -> SetBranchAddress("mcH_py",     &treeVars.mcH_py);
  tree -> SetBranchAddress("mcH_pz",     &treeVars.mcH_pz);
  tree -> SetBranchAddress("mcH_p",      &treeVars.mcH_p);
  tree -> SetBranchAddress("mcH_pt",     &treeVars.mcH_pt);
  tree -> SetBranchAddress("mcH_et",     &treeVars.mcH_et);
  tree -> SetBranchAddress("mcH_mass",   &treeVars.mcH_mass);
  tree -> SetBranchAddress("mcH_phi",    &treeVars.mcH_phi);
  tree -> SetBranchAddress("mcH_eta",    &treeVars.mcH_eta);
  tree -> SetBranchAddress("mcH_charge", &treeVars.mcH_charge);
  
  tree -> SetBranchAddress("mcW_energy", treeVars.mcW_energy);
  tree -> SetBranchAddress("mcW_px",     treeVars.mcW_px);
  tree -> SetBranchAddress("mcW_py",     treeVars.mcW_py);
  tree -> SetBranchAddress("mcW_pz",     treeVars.mcW_pz);
  tree -> SetBranchAddress("mcW_p",      treeVars.mcW_p);
  tree -> SetBranchAddress("mcW_pt",     treeVars.mcW_pt);
  tree -> SetBranchAddress("mcW_et",     treeVars.mcW_et);
  tree -> SetBranchAddress("mcW_mass",   treeVars.mcW_mass);
  tree -> SetBranchAddress("mcW_phi",    treeVars.mcW_phi);
  tree -> SetBranchAddress("mcW_eta",    treeVars.mcW_eta);
  tree -> SetBranchAddress("mcW_charge", treeVars.mcW_charge);
  
  tree -> SetBranchAddress("mcF_fromW1_energy", treeVars.mcF_fromW1_energy);
  tree -> SetBranchAddress("mcF_fromW1_px",     treeVars.mcF_fromW1_px);
  tree -> SetBranchAddress("mcF_fromW1_py",     treeVars.mcF_fromW1_py);
  tree -> SetBranchAddress("mcF_fromW1_pz",     treeVars.mcF_fromW1_pz);
  tree -> SetBranchAddress("mcF_fromW1_p",      treeVars.mcF_fromW1_p);
  tree -> SetBranchAddress("mcF_fromW1_pt",     treeVars.mcF_fromW1_pt);
  tree -> SetBranchAddress("mcF_fromW1_et",     treeVars.mcF_fromW1_et);
  tree -> SetBranchAddress("mcF_fromW1_mass",   treeVars.mcF_fromW1_mass);
  tree -> SetBranchAddress("mcF_fromW1_phi",    treeVars.mcF_fromW1_phi);
  tree -> SetBranchAddress("mcF_fromW1_eta",    treeVars.mcF_fromW1_eta);
  tree -> SetBranchAddress("mcF_fromW1_charge", treeVars.mcF_fromW1_charge);
  tree -> SetBranchAddress("mcF_fromW1_pdgId",  treeVars.mcF_fromW1_pdgId);
  
  tree -> SetBranchAddress("mcQ_fromW2_energy", treeVars.mcQ_fromW2_energy);
  tree -> SetBranchAddress("mcQ_fromW2_px",     treeVars.mcQ_fromW2_px);
  tree -> SetBranchAddress("mcQ_fromW2_py",     treeVars.mcQ_fromW2_py);
  tree -> SetBranchAddress("mcQ_fromW2_pz",     treeVars.mcQ_fromW2_pz);
  tree -> SetBranchAddress("mcQ_fromW2_p",      treeVars.mcQ_fromW2_p);
  tree -> SetBranchAddress("mcQ_fromW2_pt",     treeVars.mcQ_fromW2_pt);
  tree -> SetBranchAddress("mcQ_fromW2_et",     treeVars.mcQ_fromW2_et);
  tree -> SetBranchAddress("mcQ_fromW2_mass",   treeVars.mcQ_fromW2_mass);
  tree -> SetBranchAddress("mcQ_fromW2_phi",    treeVars.mcQ_fromW2_phi);
  tree -> SetBranchAddress("mcQ_fromW2_eta",    treeVars.mcQ_fromW2_eta);
  tree -> SetBranchAddress("mcQ_fromW2_charge", treeVars.mcQ_fromW2_charge);
  tree -> SetBranchAddress("mcQ_fromW2_pdgId",  treeVars.mcQ_fromW2_pdgId);
  
  tree -> SetBranchAddress("genJet_fromW2_isMatching", treeVars.genJet_fromW2_isMatching);
  tree -> SetBranchAddress("genJet_fromW2_it",         treeVars.genJet_fromW2_it);
  
  tree -> SetBranchAddress("mcQ_tag_energy", treeVars.mcQ_tag_energy);
  tree -> SetBranchAddress("mcQ_tag_px",     treeVars.mcQ_tag_px);
  tree -> SetBranchAddress("mcQ_tag_py",     treeVars.mcQ_tag_py);
  tree -> SetBranchAddress("mcQ_tag_pz",     treeVars.mcQ_tag_pz);
  tree -> SetBranchAddress("mcQ_tag_p",      treeVars.mcQ_tag_p);
  tree -> SetBranchAddress("mcQ_tag_pt",     treeVars.mcQ_tag_pt);
  tree -> SetBranchAddress("mcQ_tag_et",     treeVars.mcQ_tag_et);
  tree -> SetBranchAddress("mcQ_tag_mass",   treeVars.mcQ_tag_mass);
  tree -> SetBranchAddress("mcQ_tag_phi",    treeVars.mcQ_tag_phi);
  tree -> SetBranchAddress("mcQ_tag_eta",    treeVars.mcQ_tag_eta);
  tree -> SetBranchAddress("mcQ_tag_charge", treeVars.mcQ_tag_charge);
  tree -> SetBranchAddress("mcQ_tag_pdgId",  treeVars.mcQ_tag_pdgId);
  
  tree -> SetBranchAddress("genJet_tag_isMatching", treeVars.genJet_tag_isMatching);
  tree -> SetBranchAddress("genJet_tag_it",         treeVars.genJet_tag_it);
  
  
  
  
  
  
  // mc electrons
  tree -> SetBranchAddress("nMcE",      &treeVars.nMcE);
  tree -> SetBranchAddress("mcE_energy", treeVars.mcE_energy);
  tree -> SetBranchAddress("mcE_px",     treeVars.mcE_px);
  tree -> SetBranchAddress("mcE_py",     treeVars.mcE_py);
  tree -> SetBranchAddress("mcE_pz",     treeVars.mcE_pz);
  tree -> SetBranchAddress("mcE_p",      treeVars.mcE_p);
  tree -> SetBranchAddress("mcE_pt",     treeVars.mcE_pt);
  tree -> SetBranchAddress("mcE_et",     treeVars.mcE_et);
  tree -> SetBranchAddress("mcE_mass",   treeVars.mcE_mass);
  tree -> SetBranchAddress("mcE_phi",    treeVars.mcE_phi);
  tree -> SetBranchAddress("mcE_eta",    treeVars.mcE_eta);
  tree -> SetBranchAddress("mcE_charge", treeVars.mcE_charge);

  // reco electrons
  tree -> SetBranchAddress("nRecoE",      &treeVars.nRecoE);
  tree -> SetBranchAddress("recoE_energy", treeVars.recoE_energy);
  tree -> SetBranchAddress("recoE_px",     treeVars.recoE_px);
  tree -> SetBranchAddress("recoE_py",     treeVars.recoE_py);
  tree -> SetBranchAddress("recoE_pz",     treeVars.recoE_pz);
  tree -> SetBranchAddress("recoE_p",      treeVars.recoE_p);
  tree -> SetBranchAddress("recoE_pt",     treeVars.recoE_pt);
  tree -> SetBranchAddress("recoE_et",     treeVars.recoE_et);
  tree -> SetBranchAddress("recoE_mass",   treeVars.recoE_mass);
  tree -> SetBranchAddress("recoE_phi",    treeVars.recoE_phi);
  tree -> SetBranchAddress("recoE_eta",    treeVars.recoE_eta);
  tree -> SetBranchAddress("recoE_charge", treeVars.recoE_charge);
  
  tree -> SetBranchAddress("recoE_tkIso",  treeVars.recoE_tkIso);
  tree -> SetBranchAddress("recoE_emIso",  treeVars.recoE_emIso);
  tree -> SetBranchAddress("recoE_hadIso", treeVars.recoE_hadIso);
  
  tree -> SetBranchAddress("recoE_eleIdLoose",       treeVars.recoE_eleIdLoose);
  tree -> SetBranchAddress("recoE_eleIdTight",       treeVars.recoE_eleIdTight);
  tree -> SetBranchAddress("recoE_eleIdRobustLoose", treeVars.recoE_eleIdRobustLoose);
  tree -> SetBranchAddress("recoE_eleIdRobustTight", treeVars.recoE_eleIdRobustTight);
  
  
  
  
  
  
  // mc muons
  tree -> SetBranchAddress("nMcMu",      &treeVars.nMcMu);
  tree -> SetBranchAddress("mcMu_energy", treeVars.mcMu_energy);
  tree -> SetBranchAddress("mcMu_px",     treeVars.mcMu_px);
  tree -> SetBranchAddress("mcMu_py",     treeVars.mcMu_py);
  tree -> SetBranchAddress("mcMu_pz",     treeVars.mcMu_pz);
  tree -> SetBranchAddress("mcMu_p",      treeVars.mcMu_p);
  tree -> SetBranchAddress("mcMu_pt",     treeVars.mcMu_pt);
  tree -> SetBranchAddress("mcMu_et",     treeVars.mcMu_et);
  tree -> SetBranchAddress("mcMu_mass",   treeVars.mcMu_mass);
  tree -> SetBranchAddress("mcMu_phi",    treeVars.mcMu_phi);
  tree -> SetBranchAddress("mcMu_eta",    treeVars.mcMu_eta);
  tree -> SetBranchAddress("mcMu_charge", treeVars.mcMu_charge);  

  // reco muons
  tree -> SetBranchAddress("nRecoMu",      &treeVars.nRecoMu);
  tree -> SetBranchAddress("recoMu_energy", treeVars.recoMu_energy);
  tree -> SetBranchAddress("recoMu_px",     treeVars.recoMu_px);
  tree -> SetBranchAddress("recoMu_py",     treeVars.recoMu_py);
  tree -> SetBranchAddress("recoMu_pz",     treeVars.recoMu_pz);
  tree -> SetBranchAddress("recoMu_p",      treeVars.recoMu_p);
  tree -> SetBranchAddress("recoMu_pt",     treeVars.recoMu_pt);
  tree -> SetBranchAddress("recoMu_et",     treeVars.recoMu_et);
  tree -> SetBranchAddress("recoMu_mass",   treeVars.recoMu_mass);
  tree -> SetBranchAddress("recoMu_phi",    treeVars.recoMu_phi);
  tree -> SetBranchAddress("recoMu_eta",    treeVars.recoMu_eta);
  tree -> SetBranchAddress("recoMu_charge", treeVars.recoMu_charge);
  
  tree -> SetBranchAddress("recoMu_tkIsoR03",      treeVars.recoMu_tkIsoR03);
  tree -> SetBranchAddress("recoMu_nTkIsoR03",     treeVars.recoMu_nTkIsoR03);
  tree -> SetBranchAddress("recoMu_emIsoR03",      treeVars.recoMu_emIsoR03);
  tree -> SetBranchAddress("recoMu_hadIsoR03",     treeVars.recoMu_hadIsoR03);
  
  tree -> SetBranchAddress("recoMu_tkIsoR05",      treeVars.recoMu_tkIsoR05);
  tree -> SetBranchAddress("recoMu_nTkIsoR05",     treeVars.recoMu_nTkIsoR05);
  tree -> SetBranchAddress("recoMu_emIsoR05",      treeVars.recoMu_emIsoR05);
  tree -> SetBranchAddress("recoMu_hadIsoR05",     treeVars.recoMu_hadIsoR05);
  
  
  
  
  
  
  // gen jets
  tree -> SetBranchAddress("nGenJet",      &treeVars.nGenJet);
  tree -> SetBranchAddress("genJet_energy", treeVars.genJet_energy);
  tree -> SetBranchAddress("genJet_px",     treeVars.genJet_px);
  tree -> SetBranchAddress("genJet_py",     treeVars.genJet_py);
  tree -> SetBranchAddress("genJet_pz",     treeVars.genJet_pz);
  tree -> SetBranchAddress("genJet_p",      treeVars.genJet_p);
  tree -> SetBranchAddress("genJet_pt",     treeVars.genJet_pt);
  tree -> SetBranchAddress("genJet_et",     treeVars.genJet_et);
  tree -> SetBranchAddress("genJet_mass",   treeVars.genJet_mass);
  tree -> SetBranchAddress("genJet_phi",    treeVars.genJet_phi);
  tree -> SetBranchAddress("genJet_eta",    treeVars.genJet_eta);
  tree -> SetBranchAddress("genJet_charge", treeVars.genJet_charge);
  
  // reco jets
  tree -> SetBranchAddress("nRecoJet",      &treeVars.nRecoJet);
  tree -> SetBranchAddress("recoJet_energy", treeVars.recoJet_energy);
  tree -> SetBranchAddress("recoJet_px",     treeVars.recoJet_px);
  tree -> SetBranchAddress("recoJet_py",     treeVars.recoJet_py);
  tree -> SetBranchAddress("recoJet_pz",     treeVars.recoJet_pz);
  tree -> SetBranchAddress("recoJet_p",      treeVars.recoJet_p);
  tree -> SetBranchAddress("recoJet_pt",     treeVars.recoJet_pt);
  tree -> SetBranchAddress("recoJet_et",     treeVars.recoJet_et);
  tree -> SetBranchAddress("recoJet_mass",   treeVars.recoJet_mass);
  tree -> SetBranchAddress("recoJet_phi",    treeVars.recoJet_phi);
  tree -> SetBranchAddress("recoJet_eta",    treeVars.recoJet_eta);
  tree -> SetBranchAddress("recoJet_charge", treeVars.recoJet_charge);
  
  
  
  
  
  
  // gen MET
  tree -> SetBranchAddress("genMetNoNuBSM",   &treeVars.genMetNoNuBSM);
  tree -> SetBranchAddress("genMetNoNuBSM_x", &treeVars.genMetNoNuBSM_x);
  tree -> SetBranchAddress("genMetNoNuBSM_y", &treeVars.genMetNoNuBSM_y);
  
  // reco MET
  tree -> SetBranchAddress("caloMet",   &treeVars.caloMet);
  tree -> SetBranchAddress("caloMet_x", &treeVars.caloMet_x);
  tree -> SetBranchAddress("caloMet_y", &treeVars.caloMet_y);
  
  tree -> SetBranchAddress("muonCorrectedCaloMet",   &treeVars.muonCorrectedCaloMet);
  tree -> SetBranchAddress("muonCorrectedCaloMet_x", &treeVars.muonCorrectedCaloMet_x);
  tree -> SetBranchAddress("muonCorrectedCaloMet_y", &treeVars.muonCorrectedCaloMet_y);
  
  tree -> SetBranchAddress("PFMet",   &treeVars.PFMet);
  tree -> SetBranchAddress("PFMet_x", &treeVars.PFMet_x);
  tree -> SetBranchAddress("PFMet_y", &treeVars.PFMet_y);
  
}


 



void SetBranches(TTree* tree, VBFNtupleContent& treeVars)
{
  tree -> Branch("eventType", &treeVars.eventType, "eventType/i");
  tree -> Branch("eventId",   &treeVars.eventId,     "eventId/i");
  
  
  
  // mc variables
  tree -> Branch("mcH_energy", &treeVars.mcH_energy, "mcH_energy/F");
  tree -> Branch("mcH_px",     &treeVars.mcH_px,         "mcH_px/F");
  tree -> Branch("mcH_py",     &treeVars.mcH_py,         "mcH_py/F");
  tree -> Branch("mcH_pz",     &treeVars.mcH_pz,         "mcH_pz/F");
  tree -> Branch("mcH_p",      &treeVars.mcH_p,           "mcH_p/F");
  tree -> Branch("mcH_pt",     &treeVars.mcH_pt,         "mcH_pt/F");
  tree -> Branch("mcH_et",     &treeVars.mcH_et,         "mcH_et/F");
  tree -> Branch("mcH_mass",   &treeVars.mcH_mass,     "mcH_mass/F");
  tree -> Branch("mcH_phi",    &treeVars.mcH_phi,       "mcH_phi/F");
  tree -> Branch("mcH_eta",    &treeVars.mcH_eta,       "mcH_eta/F");
  tree -> Branch("mcH_charge", &treeVars.mcH_charge, "mcH_charge/I");
  
  tree -> Branch("mcW_energy", treeVars.mcW_energy, "mcW_energy[2]/F");
  tree -> Branch("mcW_px",     treeVars.mcW_px,         "mcW_px[2]/F");
  tree -> Branch("mcW_py",     treeVars.mcW_py,         "mcW_py[2]/F");
  tree -> Branch("mcW_pz",     treeVars.mcW_pz,         "mcW_pz[2]/F");
  tree -> Branch("mcW_p",      treeVars.mcW_p,           "mcW_p[2]/F");
  tree -> Branch("mcW_pt",     treeVars.mcW_pt,         "mcW_pt[2]/F");
  tree -> Branch("mcW_et",     treeVars.mcW_et,         "mcW_et[2]/F");
  tree -> Branch("mcW_mass",   treeVars.mcW_mass,     "mcW_mass[2]/F");
  tree -> Branch("mcW_phi",    treeVars.mcW_phi,       "mcW_phi[2]/F");
  tree -> Branch("mcW_eta",    treeVars.mcW_eta,       "mcW_eta[2]/F");
  tree -> Branch("mcW_charge", treeVars.mcW_charge, "mcW_charge[2]/I");
  
  tree -> Branch("mcF_fromW1_energy", treeVars.mcF_fromW1_energy, "mcF_fromW1_energy[2]/F");
  tree -> Branch("mcF_fromW1_px",     treeVars.mcF_fromW1_px,         "mcF_fromW1_px[2]/F");
  tree -> Branch("mcF_fromW1_py",     treeVars.mcF_fromW1_py,         "mcF_fromW1_py[2]/F");
  tree -> Branch("mcF_fromW1_pz",     treeVars.mcF_fromW1_pz,         "mcF_fromW1_pz[2]/F");
  tree -> Branch("mcF_fromW1_p",      treeVars.mcF_fromW1_p,           "mcF_fromW1_p[2]/F");
  tree -> Branch("mcF_fromW1_pt",     treeVars.mcF_fromW1_pt,         "mcF_fromW1_pt[2]/F");
  tree -> Branch("mcF_fromW1_et",     treeVars.mcF_fromW1_et,         "mcF_fromW1_et[2]/F");
  tree -> Branch("mcF_fromW1_mass",   treeVars.mcF_fromW1_mass,     "mcF_fromW1_mass[2]/F");
  tree -> Branch("mcF_fromW1_phi",    treeVars.mcF_fromW1_phi,       "mcF_fromW1_phi[2]/F");
  tree -> Branch("mcF_fromW1_eta",    treeVars.mcF_fromW1_eta,       "mcF_fromW1_eta[2]/F");
  tree -> Branch("mcF_fromW1_charge", treeVars.mcF_fromW1_charge, "mcF_fromW1_charge[2]/I");
  tree -> Branch("mcF_fromW1_pdgId",  treeVars.mcF_fromW1_pdgId,   "mcF_fromW1_pdgId[2]/I");
  
  tree -> Branch("mcQ_fromW2_energy", treeVars.mcQ_fromW2_energy, "mcQ_fromW2_energy[2]/F");
  tree -> Branch("mcQ_fromW2_px",     treeVars.mcQ_fromW2_px,         "mcQ_fromW2_px[2]/F");
  tree -> Branch("mcQ_fromW2_py",     treeVars.mcQ_fromW2_py,         "mcQ_fromW2_py[2]/F");
  tree -> Branch("mcQ_fromW2_pz",     treeVars.mcQ_fromW2_pz,         "mcQ_fromW2_pz[2]/F");
  tree -> Branch("mcQ_fromW2_p",      treeVars.mcQ_fromW2_p,           "mcQ_fromW2_p[2]/F");
  tree -> Branch("mcQ_fromW2_pt",     treeVars.mcQ_fromW2_pt,         "mcQ_fromW2_pt[2]/F");
  tree -> Branch("mcQ_fromW2_et",     treeVars.mcQ_fromW2_et,         "mcQ_fromW2_et[2]/F");
  tree -> Branch("mcQ_fromW2_mass",   treeVars.mcQ_fromW2_mass,     "mcQ_fromW2_mass[2]/F");
  tree -> Branch("mcQ_fromW2_phi",    treeVars.mcQ_fromW2_phi,       "mcQ_fromW2_phi[2]/F");
  tree -> Branch("mcQ_fromW2_eta",    treeVars.mcQ_fromW2_eta,       "mcQ_fromW2_eta[2]/F");
  tree -> Branch("mcQ_fromW2_charge", treeVars.mcQ_fromW2_charge, "mcQ_fromW2_charge[2]/I");
  tree -> Branch("mcQ_fromW2_pdgId",  treeVars.mcQ_fromW2_pdgId,   "mcQ_fromW2_pdgId[2]/I");
  
  tree -> Branch("genJet_fromW2_isMatching", treeVars.genJet_fromW2_isMatching, "genJet_fromW2_isMatching[2]/I");
  tree -> Branch("genJet_fromW2_it",         treeVars.genJet_fromW2_it,                 "genJet_fromW2_it[2]/I");
  
  tree -> Branch("mcQ_tag_energy", treeVars.mcQ_tag_energy, "mcQ_tag_energy[2]/F");
  tree -> Branch("mcQ_tag_px",     treeVars.mcQ_tag_px,         "mcQ_tag_px[2]/F");
  tree -> Branch("mcQ_tag_py",     treeVars.mcQ_tag_py,         "mcQ_tag_py[2]/F");
  tree -> Branch("mcQ_tag_pz",     treeVars.mcQ_tag_pz,         "mcQ_tag_pz[2]/F");
  tree -> Branch("mcQ_tag_p",      treeVars.mcQ_tag_p,           "mcQ_tag_p[2]/F");
  tree -> Branch("mcQ_tag_pt",     treeVars.mcQ_tag_pt,         "mcQ_tag_pt[2]/F");
  tree -> Branch("mcQ_tag_et",     treeVars.mcQ_tag_et,         "mcQ_tag_et[2]/F");
  tree -> Branch("mcQ_tag_mass",   treeVars.mcQ_tag_mass,     "mcQ_tag_mass[2]/F");
  tree -> Branch("mcQ_tag_phi",    treeVars.mcQ_tag_phi,       "mcQ_tag_phi[2]/F");
  tree -> Branch("mcQ_tag_eta",    treeVars.mcQ_tag_eta,       "mcQ_tag_eta[2]/F");
  tree -> Branch("mcQ_tag_charge", treeVars.mcQ_tag_charge, "mcQ_tag_charge[2]/I");
  tree -> Branch("mcQ_tag_pdgId",  treeVars.mcQ_tag_pdgId,   "mcQ_tag_pdgId[2]/I");
  
  tree -> Branch("genJet_tag_isMatching", treeVars.genJet_tag_isMatching,   "genJet_tag_isMatching[2]/I");
  tree -> Branch("genJet_tag_it",         treeVars.genJet_tag_it,                   "genJet_tag_it[2]/I");
  
  
  
  
  
  
  // mc electrons
  tree -> Branch("nMcE",      &treeVars.nMcE,                 "nMcE/i");
  tree -> Branch("mcE_energy", treeVars.mcE_energy, "mcE_energy[25]/F");
  tree -> Branch("mcE_px",     treeVars.mcE_px,         "mcE_px[25]/F");
  tree -> Branch("mcE_py",     treeVars.mcE_py,         "mcE_py[25]/F");
  tree -> Branch("mcE_pz",     treeVars.mcE_pz,         "mcE_pz[25]/F");
  tree -> Branch("mcE_p",      treeVars.mcE_p,           "mcE_p[25]/F");
  tree -> Branch("mcE_pt",     treeVars.mcE_pt,         "mcE_pt[25]/F");
  tree -> Branch("mcE_et",     treeVars.mcE_et,         "mcE_et[25]/F");
  tree -> Branch("mcE_mass",   treeVars.mcE_mass,     "mcE_mass[25]/F");
  tree -> Branch("mcE_phi",    treeVars.mcE_phi,       "mcE_phi[25]/F");
  tree -> Branch("mcE_eta",    treeVars.mcE_eta,       "mcE_eta[25]/F");
  tree -> Branch("mcE_charge", treeVars.mcE_charge, "mcE_charge[25]/I");  
  
  // reco electrons
  tree -> Branch("nRecoE",      &treeVars.nRecoE,                 "nRecoE/i");
  tree -> Branch("recoE_energy", treeVars.recoE_energy, "recoE_energy[15]/F");
  tree -> Branch("recoE_px",     treeVars.recoE_px,         "recoE_px[15]/F");
  tree -> Branch("recoE_py",     treeVars.recoE_py,         "recoE_py[15]/F");
  tree -> Branch("recoE_pz",     treeVars.recoE_pz,         "recoE_pz[15]/F");
  tree -> Branch("recoE_p",      treeVars.recoE_p,           "recoE_p[15]/F");
  tree -> Branch("recoE_pt",     treeVars.recoE_pt,         "recoE_pt[15]/F");
  tree -> Branch("recoE_et",     treeVars.recoE_et,         "recoE_et[15]/F");
  tree -> Branch("recoE_mass",   treeVars.recoE_mass,     "recoE_mass[15]/F");
  tree -> Branch("recoE_phi",    treeVars.recoE_phi,       "recoE_phi[15]/F");
  tree -> Branch("recoE_eta",    treeVars.recoE_eta,       "recoE_eta[15]/F");
  tree -> Branch("recoE_charge", treeVars.recoE_charge, "recoE_charge[15]/I");
  
  tree -> Branch("recoE_tkIso",      treeVars.recoE_tkIso,           "recoE_tkIso[15]/F");
  tree -> Branch("recoE_emIso",      treeVars.recoE_emIso,           "recoE_emIso[15]/F");
  tree -> Branch("recoE_hadIso",     treeVars.recoE_hadIso,         "recoE_hadIso[15]/F");
  
  tree -> Branch("recoE_eleIdLoose",       treeVars.recoE_eleIdLoose,       "recoE_eleIdLoose[15]/F");
  tree -> Branch("recoE_eleIdTight",       treeVars.recoE_eleIdTight,       "recoE_eleIdTight[15]/F");
  tree -> Branch("recoE_eleIdRobustLoose", treeVars.recoE_eleIdRobustLoose, "recoE_eleIdRobustLoose[15]/F");
  tree -> Branch("recoE_eleIdRobustTight", treeVars.recoE_eleIdRobustTight, "recoE_eleIdRobustTight[15]/F");
  
  
  
  
  
  
  // mc muons
  tree -> Branch("nMcMu",      &treeVars.nMcMu,                 "nMcMu/i");
  tree -> Branch("mcMu_energy", treeVars.mcMu_energy, "mcMu_energy[25]/F");
  tree -> Branch("mcMu_px",     treeVars.mcMu_px,         "mcMu_px[25]/F");
  tree -> Branch("mcMu_py",     treeVars.mcMu_py,         "mcMu_py[25]/F");
  tree -> Branch("mcMu_pz",     treeVars.mcMu_pz,         "mcMu_pz[25]/F");
  tree -> Branch("mcMu_p",      treeVars.mcMu_p,           "mcMu_p[25]/F");
  tree -> Branch("mcMu_pt",     treeVars.mcMu_pt,         "mcMu_pt[25]/F");
  tree -> Branch("mcMu_et",     treeVars.mcMu_et,         "mcMu_et[25]/F");
  tree -> Branch("mcMu_mass",   treeVars.mcMu_mass,     "mcMu_mass[25]/F");
  tree -> Branch("mcMu_phi",    treeVars.mcMu_phi,       "mcMu_phi[25]/F");
  tree -> Branch("mcMu_eta",    treeVars.mcMu_eta,       "mcMu_eta[25]/F");
  tree -> Branch("mcMu_charge", treeVars.mcMu_charge, "mcMu_charge[25]/I");  
  
  // reco muons
  tree -> Branch("nRecoMu",      &treeVars.nRecoMu,                 "nRecoMu/i");
  tree -> Branch("recoMu_energy", treeVars.recoMu_energy, "recoMu_energy[15]/F");
  tree -> Branch("recoMu_px",     treeVars.recoMu_px,         "recoMu_px[15]/F");
  tree -> Branch("recoMu_py",     treeVars.recoMu_py,         "recoMu_py[15]/F");
  tree -> Branch("recoMu_pz",     treeVars.recoMu_pz,         "recoMu_pz[15]/F");
  tree -> Branch("recoMu_p",      treeVars.recoMu_p,           "recoMu_p[15]/F");
  tree -> Branch("recoMu_pt",     treeVars.recoMu_pt,         "recoMu_pt[15]/F");
  tree -> Branch("recoMu_et",     treeVars.recoMu_et,         "recoMu_et[15]/F");
  tree -> Branch("recoMu_mass",   treeVars.recoMu_mass,     "recoMu_mass[15]/F");
  tree -> Branch("recoMu_phi",    treeVars.recoMu_phi,       "recoMu_phi[15]/F");
  tree -> Branch("recoMu_eta",    treeVars.recoMu_eta,       "recoMu_eta[15]/F");
  tree -> Branch("recoMu_charge", treeVars.recoMu_charge, "recoMu_charge[15]/I");
  
  tree -> Branch("recoMu_tkIsoR03",      treeVars.recoMu_tkIsoR03,           "recoMu_tkIsoR03[15]/F");
  tree -> Branch("recoMu_nTkIsoR03",     treeVars.recoMu_nTkIsoR03,         "recoMu_nTkIsoR03[15]/I");
  tree -> Branch("recoMu_emIsoR03",      treeVars.recoMu_emIsoR03,           "recoMu_emIsoR03[15]/F");
  tree -> Branch("recoMu_hadIsoR03",     treeVars.recoMu_hadIsoR03,         "recoMu_hadIsoR03[15]/F");
  
  tree -> Branch("recoMu_tkIsoR05",      treeVars.recoMu_tkIsoR05,           "recoMu_tkIsoR05[15]/F");
  tree -> Branch("recoMu_nTkIsoR05",     treeVars.recoMu_nTkIsoR05,         "recoMu_nTkIsoR05[15]/I");
  tree -> Branch("recoMu_emIsoR05",      treeVars.recoMu_emIsoR05,           "recoMu_emIsoR05[15]/F");
  tree -> Branch("recoMu_hadIsoR05",     treeVars.recoMu_hadIsoR05,         "recoMu_hadIsoR05[15]/F");
  
  
  
  
  
  
  // gen jets
  tree -> Branch("nGenJet",      &treeVars.nGenJet,                  "nGenJet/i");
  tree -> Branch("genJet_energy", treeVars.genJet_energy, "genJet_energy[100]/F");
  tree -> Branch("genJet_px",     treeVars.genJet_px,         "genJet_px[100]/F");
  tree -> Branch("genJet_py",     treeVars.genJet_py,         "genJet_py[100]/F");
  tree -> Branch("genJet_pz",     treeVars.genJet_pz,         "genJet_pz[100]/F");
  tree -> Branch("genJet_p",      treeVars.genJet_p,           "genJet_p[100]/F");
  tree -> Branch("genJet_pt",     treeVars.genJet_pt,         "genJet_pt[100]/F");
  tree -> Branch("genJet_et",     treeVars.genJet_et,         "genJet_et[100]/F");
  tree -> Branch("genJet_mass",   treeVars.genJet_mass,     "genJet_mass[100]/F");
  tree -> Branch("genJet_phi",    treeVars.genJet_phi,       "genJet_phi[100]/F");
  tree -> Branch("genJet_eta",    treeVars.genJet_eta,       "genJet_eta[100]/F");
  tree -> Branch("genJet_charge", treeVars.genJet_charge, "genJet_charge[100]/I");
  
  // reco jets
  tree -> Branch("nRecoJet",      &treeVars.nRecoJet,                 "nRecoJet/i");
  tree -> Branch("recoJet_energy", treeVars.recoJet_energy, "recoJet_energy[50]/F");
  tree -> Branch("recoJet_px",     treeVars.recoJet_px,         "recoJet_px[50]/F");
  tree -> Branch("recoJet_py",     treeVars.recoJet_py,         "recoJet_py[50]/F");
  tree -> Branch("recoJet_pz",     treeVars.recoJet_pz,         "recoJet_pz[50]/F");
  tree -> Branch("recoJet_p",      treeVars.recoJet_p,           "recoJet_p[50]/F");
  tree -> Branch("recoJet_pt",     treeVars.recoJet_pt,         "recoJet_pt[50]/F");
  tree -> Branch("recoJet_et",     treeVars.recoJet_et,         "recoJet_et[50]/F");
  tree -> Branch("recoJet_mass",   treeVars.recoJet_mass,     "recoJet_mass[50]/F");
  tree -> Branch("recoJet_phi",    treeVars.recoJet_phi,       "recoJet_phi[50]/F");
  tree -> Branch("recoJet_eta",    treeVars.recoJet_eta,       "recoJet_eta[50]/F");
  tree -> Branch("recoJet_charge", treeVars.recoJet_charge, "recoJet_charge[50]/I");
  
  
  
  
  
  
  
  // gen met
  tree -> Branch("genMetNoNuBSM",   &treeVars.genMetNoNuBSM,     "genMetNoNuBSM/F");
  tree -> Branch("genMetNoNuBSM_x", &treeVars.genMetNoNuBSM_x, "genMetNoNuBSM_x/F");
  tree -> Branch("genMetNoNuBSM_y", &treeVars.genMetNoNuBSM_y, "genMetNoNuBSM_y/F");
  
  // reco met
  tree -> Branch("caloMet",   &treeVars.caloMet,     "caloMet/F");
  tree -> Branch("caloMet_x", &treeVars.caloMet_x, "caloMet_x/F");
  tree -> Branch("caloMet_y", &treeVars.caloMet_y, "caloMet_y/F");
  
  tree -> Branch("muonCorrectedCaloMet",   &treeVars.muonCorrectedCaloMet,     "muonCorrectedCaloMet/F");
  tree -> Branch("muonCorrectedCaloMet_x", &treeVars.muonCorrectedCaloMet_x, "muonCorrectedCaloMet_x/F");
  tree -> Branch("muonCorrectedCaloMet_y", &treeVars.muonCorrectedCaloMet_y, "muonCorrectedCaloMet_y/F");
  
  tree -> Branch("PFMet",   &treeVars.PFMet,     "PFMet/F");
  tree -> Branch("PFMet_x", &treeVars.PFMet_x, "PFMet_x/F");
  tree -> Branch("PFMet_y", &treeVars.PFMet_y, "PFMet_y/F");
  
}






void InitializeBranches(TTree* tree, VBFNtupleContent& treeVars)
{
  treeVars.eventType = 0;
  treeVars.eventId   = 0; 
  
  
  
  treeVars.genJet_fromW2_isMatching[0] = 0;
  treeVars.genJet_fromW2_isMatching[1] = 0;
  treeVars.genJet_fromW2_it[0] = -1;
  treeVars.genJet_fromW2_it[1] = -1;
  
  treeVars.genJet_tag_isMatching[0] = 0;
  treeVars.genJet_tag_isMatching[1] = 0;
  treeVars.genJet_tag_it[0] = -1;
  treeVars.genJet_tag_it[1] = -1;
  
  
  
  // mc electrons
  treeVars.nMcE = 0;
  for(unsigned int i = 0; i < 25; ++i)
  {
    treeVars.mcE_energy[i] = -1.;
    treeVars.mcE_px[i] = 0.;
    treeVars.mcE_py[i] = 0.;
    treeVars.mcE_pz[i] = 0.;
    treeVars.mcE_p[i] = -1.;
    treeVars.mcE_pt[i] = -1.;
    treeVars.mcE_et[i] = -1.;
    treeVars.mcE_mass[i] = -1.;
    treeVars.mcE_phi[i] = 0.;
    treeVars.mcE_eta[i] = 0.;
    treeVars.mcE_charge[i] = 0;
  }
  
  // reco electrons
  treeVars.nRecoE = 0;
  for(unsigned int i = 0; i < 15; ++i)
  {
    treeVars.recoE_energy[i] = -1.;
    treeVars.recoE_px[i] = 0.;
    treeVars.recoE_py[i] = 0.;
    treeVars.recoE_pz[i] = 0.;
    treeVars.recoE_p[i] = -1.;
    treeVars.recoE_pt[i] = -1.;
    treeVars.recoE_et[i] = -1.;
    treeVars.recoE_mass[i] = -1.;
    treeVars.recoE_phi[i] = 0.;
    treeVars.recoE_eta[i] = 0.;
    treeVars.recoE_charge[i] = 0;
    
    treeVars.recoE_tkIso[i] = -1.;
    treeVars.recoE_emIso[i] = -1.;
    treeVars.recoE_hadIso[i] = -1.;
    
    treeVars.recoE_eleIdLoose[i] = -1.;
    treeVars.recoE_eleIdTight[i] = -1.;
    treeVars.recoE_eleIdRobustLoose[i] = -1.;
    treeVars.recoE_eleIdRobustTight[i] = -1.;
  }
  
  
  
  
  
  
  // mc muons
  treeVars.nMcMu = 0;
  for(unsigned int i = 0; i < 25; ++i)
  {
    treeVars.mcMu_energy[i] = -1.;
    treeVars.mcMu_px[i] = 0.;
    treeVars.mcMu_py[i] = 0.;
    treeVars.mcMu_pz[i] = 0.;
    treeVars.mcMu_p[i] = -1.;
    treeVars.mcMu_pt[i] = -1.;
    treeVars.mcMu_et[i] = -1.;
    treeVars.mcMu_mass[i] = -1.;
    treeVars.mcMu_phi[i] = 0.;
    treeVars.mcMu_eta[i] = 0.;
    treeVars.mcMu_charge[i] = 0;
  }  
  
  // reco muons
  treeVars.nRecoMu = 0;
  for(unsigned int i = 0; i < 15; ++i)
  {
    treeVars.recoMu_energy[i] = -1.;
    treeVars.recoMu_px[i] = 0.;
    treeVars.recoMu_py[i] = 0.;
    treeVars.recoMu_pz[i] = 0.;
    treeVars.recoMu_p[i] = -1.;
    treeVars.recoMu_pt[i] = -1.;
    treeVars.recoMu_et[i] = -1.;
    treeVars.recoMu_mass[i] = -1.;
    treeVars.recoMu_phi[i] = 0.;
    treeVars.recoMu_eta[i] = 0.;
    treeVars.recoMu_charge[i] = 0;

    treeVars.recoMu_tkIsoR03[i] = -1.;
    treeVars.recoMu_nTkIsoR03[i] = -1;
    treeVars.recoMu_emIsoR03[i] = -1.;
    treeVars.recoMu_hadIsoR03[i] = -1.;
    
    treeVars.recoMu_tkIsoR05[i] = -1.;
    treeVars.recoMu_nTkIsoR05[i] = -1;
    treeVars.recoMu_emIsoR05[i] = -1.;
    treeVars.recoMu_hadIsoR05[i] = -1.;
  }
  
  
  
  
  
  
  // gen jets
  treeVars.nGenJet = 0;
  for(unsigned int i = 0; i < 100; ++i)
  {
    treeVars.genJet_energy[i] = -1.;
    treeVars.genJet_px[i] = 0.;
    treeVars.genJet_py[i] = 0.;
    treeVars.genJet_pz[i] = 0.;
    treeVars.genJet_p[i] = -1.;
    treeVars.genJet_pt[i] = -1.;
    treeVars.genJet_et[i] = -1.;
    treeVars.genJet_mass[i] = -1.;
    treeVars.genJet_phi[i] = 0.;
    treeVars.genJet_eta[i] = 0.;
    treeVars.genJet_charge[i] = 0;
  }
  
  // reco jets
  treeVars.nRecoJet = 0;
  for(unsigned int i = 0; i < 50; ++i)
  {
    treeVars.recoJet_energy[i] = -1.;
    treeVars.recoJet_px[i] = 0.;
    treeVars.recoJet_py[i] = 0.;
    treeVars.recoJet_pz[i] = 0.;
    treeVars.recoJet_p[i] = -1.;
    treeVars.recoJet_pt[i] = -1.;
    treeVars.recoJet_et[i] = -1.;
    treeVars.recoJet_mass[i] = -1.;
    treeVars.recoJet_phi[i] = 0.;
    treeVars.recoJet_eta[i] = 0.;
    treeVars.recoJet_charge[i] = 0;
  }
  
  
  
  
  
  
  // gen met
  treeVars.genMetNoNuBSM   = -1.;
  treeVars.genMetNoNuBSM_x = -1.;
  treeVars.genMetNoNuBSM_y = -1.;
  
  // reco met
  treeVars.caloMet   = -1.;
  treeVars.caloMet_x = -1.;
  treeVars.caloMet_y = -1.;
  
  treeVars.muonCorrectedCaloMet   = -1.;
  treeVars.muonCorrectedCaloMet_x = -1.;
  treeVars.muonCorrectedCaloMet_y = -1.;
  
  treeVars.PFMet   = -1.;
  treeVars.PFMet_x = -1.;
  treeVars.PFMet_y = -1.;
  
}

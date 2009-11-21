#ifndef VBFNtupleContent_h
#define VBFNtupleContent_h

#include "TChain.h" 

#define MCEMAX 25
#define MCMUMAX 25
#define GENJETMAX 100

#define RECOEMAX 15
#define RECOMUMAX 15
#define RECOJETMAX 50






// ------------------------------------------------------------------------
//! Define variables

struct VBFNtupleContent
{
  unsigned int eventType;
  unsigned int eventId;
  
  
  
  // mc variables
  float mcH_energy;
  float mcH_px;
  float mcH_py;
  float mcH_pz;
  float mcH_p;
  float mcH_pt;
  float mcH_et;
  float mcH_mass;
  float mcH_phi;
  float mcH_eta;
  int mcH_charge;  
  
  float mcW_energy[2];
  float mcW_px[2];
  float mcW_py[2];
  float mcW_pz[2];
  float mcW_p[2];
  float mcW_pt[2];
  float mcW_et[2];
  float mcW_mass[2];
  float mcW_phi[2];
  float mcW_eta[2];
  int mcW_charge[2];
  
  float mcF_fromW1_energy[2];
  float mcF_fromW1_px[2];
  float mcF_fromW1_py[2];
  float mcF_fromW1_pz[2];
  float mcF_fromW1_p[2];
  float mcF_fromW1_pt[2];
  float mcF_fromW1_et[2];
  float mcF_fromW1_mass[2];
  float mcF_fromW1_phi[2];
  float mcF_fromW1_eta[2];
  int mcF_fromW1_charge[2];
  int mcF_fromW1_pdgId[2];
  
  float mcQ_fromW2_energy[2];
  float mcQ_fromW2_px[2];
  float mcQ_fromW2_py[2];
  float mcQ_fromW2_pz[2];
  float mcQ_fromW2_p[2];
  float mcQ_fromW2_pt[2];
  float mcQ_fromW2_et[2];
  float mcQ_fromW2_mass[2];
  float mcQ_fromW2_phi[2];
  float mcQ_fromW2_eta[2];
  int mcQ_fromW2_charge[2];
  int mcQ_fromW2_pdgId[2];
  
  int genJet_fromW2_isMatching[2];
  int genJet_fromW2_it[2];
  
  float mcQ_tag_energy[2];
  float mcQ_tag_px[2];
  float mcQ_tag_py[2];
  float mcQ_tag_pz[2];
  float mcQ_tag_p[2];
  float mcQ_tag_pt[2];
  float mcQ_tag_et[2];
  float mcQ_tag_mass[2];
  float mcQ_tag_phi[2];
  float mcQ_tag_eta[2];
  int mcQ_tag_charge[2];
  int mcQ_tag_pdgId[2];
  
  int genJet_tag_isMatching[2];
  int genJet_tag_it[2];
  
  
  
  
  
  
  // mc electrons
  unsigned int nMcE;
  float mcE_energy[MCEMAX];
  float mcE_px[MCEMAX];
  float mcE_py[MCEMAX];
  float mcE_pz[MCEMAX];
  float mcE_p[MCEMAX];
  float mcE_pt[MCEMAX];
  float mcE_et[MCEMAX];
  float mcE_mass[MCEMAX];
  float mcE_phi[MCEMAX];
  float mcE_eta[MCEMAX];
  int mcE_charge[MCEMAX];  

  // reco electrons
  unsigned int nRecoE;
  float recoE_energy[RECOEMAX];
  float recoE_px[RECOEMAX];
  float recoE_py[RECOEMAX];
  float recoE_pz[RECOEMAX];
  float recoE_p[RECOEMAX];
  float recoE_pt[RECOEMAX];
  float recoE_et[RECOEMAX];
  float recoE_mass[RECOEMAX];
  float recoE_phi[RECOEMAX];
  float recoE_eta[RECOEMAX];
  int recoE_charge[RECOEMAX];
  
  float recoE_tkIso[RECOEMAX];
  float recoE_emIso[RECOEMAX];
  float recoE_hadIso[RECOEMAX];
    
  float recoE_eleIdLoose[RECOEMAX];
  float recoE_eleIdTight[RECOEMAX];
  float recoE_eleIdRobustLoose[RECOEMAX];
  float recoE_eleIdRobustTight[RECOEMAX];
  
  
  
  
  
  // mc muons
  unsigned int nMcMu;
  float mcMu_energy[MCMUMAX];
  float mcMu_px[MCMUMAX];
  float mcMu_py[MCMUMAX];
  float mcMu_pz[MCMUMAX];
  float mcMu_p[MCMUMAX];
  float mcMu_pt[MCMUMAX];
  float mcMu_et[MCMUMAX];
  float mcMu_mass[MCMUMAX];
  float mcMu_phi[MCMUMAX];
  float mcMu_eta[MCMUMAX];
  int mcMu_charge[MCMUMAX];
  
  // reco muons
  unsigned int nRecoMu;
  float recoMu_energy[RECOMUMAX];
  float recoMu_px[RECOMUMAX];
  float recoMu_py[RECOMUMAX];
  float recoMu_pz[RECOMUMAX];
  float recoMu_p[RECOMUMAX];
  float recoMu_pt[RECOMUMAX];
  float recoMu_et[RECOMUMAX];
  float recoMu_mass[RECOMUMAX];
  float recoMu_phi[RECOMUMAX];
  float recoMu_eta[RECOMUMAX];
  int recoMu_charge[RECOMUMAX];
  
  float recoMu_tkIsoR03[RECOMUMAX];
  int recoMu_nTkIsoR03[RECOMUMAX];
  float recoMu_emIsoR03[RECOMUMAX];
  float recoMu_hadIsoR03[RECOMUMAX];
  
  float recoMu_tkIsoR05[RECOMUMAX];
  int recoMu_nTkIsoR05[RECOMUMAX];
  float recoMu_emIsoR05[RECOMUMAX];
  float recoMu_hadIsoR05[RECOMUMAX];
  
  
  
  // mc jets
  unsigned int nGenJet;
  float genJet_energy[GENJETMAX];
  float genJet_px[GENJETMAX];
  float genJet_py[GENJETMAX];
  float genJet_pz[GENJETMAX];
  float genJet_p[GENJETMAX];
  float genJet_pt[GENJETMAX];
  float genJet_et[GENJETMAX];
  float genJet_mass[GENJETMAX];
  float genJet_phi[GENJETMAX];
  float genJet_eta[GENJETMAX];
  int genJet_charge[GENJETMAX];
  
  // reco jets
  unsigned int nRecoJet;
  float recoJet_energy[RECOJETMAX];
  float recoJet_px[RECOJETMAX];
  float recoJet_py[RECOJETMAX];
  float recoJet_pz[RECOJETMAX];
  float recoJet_p[RECOJETMAX];
  float recoJet_pt[RECOJETMAX];
  float recoJet_et[RECOJETMAX];
  float recoJet_mass[RECOJETMAX];
  float recoJet_phi[RECOJETMAX];
  float recoJet_eta[RECOJETMAX];
  int recoJet_charge[RECOJETMAX];
  
  
  
  // mc MET
  float genMetNoNuBSM;
  float genMetNoNuBSM_x;
  float genMetNoNuBSM_y;
  
  // reco MET
  float caloMet;
  float caloMet_x;
  float caloMet_y;
  
  float muonCorrectedCaloMet;
  float muonCorrectedCaloMet_x;
  float muonCorrectedCaloMet_y;
  
  float PFMet;
  float PFMet_x;
  float PFMet_y;
  
};







// ------------------------------------------------------------------------
//! Branch addresses settings

void SetBranchAddresses(TTree* tree, VBFNtupleContent& treeVars);






// ------------------------------------------------------------------------
//! Create branches for the ntuple

void SetBranches(TTree* tree, VBFNtupleContent& treeVars);






// ------------------------------------------------------------------------
//! Initialize branches

void InitializeBranches(TTree* tree, VBFNtupleContent& treeVars);



#endif

//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Sep 23 19:15:30 2010 by ROOT version 5.22/00d
// from TTree OniaTree/OniaTree
// found on file: SimpleNtple.root
//////////////////////////////////////////////////////////

#ifndef smallHBaseClass_h
#define smallHBaseClass_h

#include "TClonesArray.h" //PG added

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <vector>

using namespace std;

class smallHBaseClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   TClonesArray    *muons_glb_4mom;
   TClonesArray    *muons_glb_track4mom;
   vector<int>     *muons_glb_charge;
   vector<float>   *muons_glb_tkIsoR03;
   vector<float>   *muons_glb_nTkIsoR03;
   vector<float>   *muons_glb_emIsoR03;
   vector<float>   *muons_glb_hadIsoR03;
   vector<float>   *muons_glb_tkIsoR05;
   vector<float>   *muons_glb_nTkIsoR05;
   vector<float>   *muons_glb_emIsoR05;
   vector<float>   *muons_glb_hadIsoR05;
   vector<float>   *muons_glb_ptErr;
   vector<float>   *muons_glb_phiErr;
   vector<float>   *muons_glb_etaErr;
   vector<float>   *muons_glb_d0;
   vector<float>   *muons_glb_d0err;
   vector<float>   *muons_glb_dz;
   vector<float>   *muons_glb_dzerr;
   vector<float>   *muons_glb_normChi2;
   vector<float>   *muons_glb_nhitstrack;
   vector<float>   *muons_glb_nhitsDT;
   vector<float>   *muons_glb_nhitsCSC;
   vector<float>   *muons_glb_nhitsStrip;
   vector<float>   *muons_glb_nhitsPixB;
   vector<float>   *muons_glb_nhitsPixE;
   vector<float>   *muons_glb_nhitsPix1Hit;
   vector<float>   *muons_glb_nhitsPix1HitBE;
   TClonesArray    *muons_trk_4mom;
   vector<int>     *muons_trk_charge;
   vector<float>   *muons_trk_tkIsoR03;
   vector<float>   *muons_trk_nTkIsoR03;
   vector<float>   *muons_trk_emIsoR03;
   vector<float>   *muons_trk_hadIsoR03;
   vector<float>   *muons_trk_ptErr;
   vector<float>   *muons_trk_phiErr;
   vector<float>   *muons_trk_etaErr;
   vector<float>   *muons_trk_d0;
   vector<float>   *muons_trk_d0err;
   vector<float>   *muons_trk_dz;
   vector<float>   *muons_trk_dzerr;
   vector<float>   *muons_trk_normChi2;
   vector<float>   *muons_trk_nhitstrack;
   vector<float>   *muons_trk_nhitsStrip;
   vector<float>   *muons_trk_nhitsPixB;
   vector<float>   *muons_trk_nhitsPixE;
   vector<float>   *muons_trk_nhitsPix1Hit;
   vector<float>   *muons_trk_nhitsPix1HitBE;
   vector<float>   *muons_trk_PIDmask;
   TClonesArray    *electrons;
   vector<int>     *electrons_charge;
   vector<float>   *electrons_tkIso;
   vector<float>   *electrons_emIso;
   vector<float>   *electrons_hadIso;
   vector<float>   *electrons_IdBDT;
   vector<float>   *electrons_IdLoose;
   vector<float>   *electrons_IdTight;
   vector<float>   *electrons_IdRobustLoose;
   vector<float>   *electrons_IdRobustTight;
   vector<float>   *electrons_HoE;
   vector<float>   *electrons_dEtaSc;
   vector<float>   *electrons_dPhiSc;
   vector<float>   *electrons_SigiEtaiEtaSc;
   vector<float>   *electrons_EoP;
   vector<float>   *electrons_Et;
   vector<float>   *electrons_pAtVtx;
   vector<float>   *electrons_ptAtVtx;
   vector<float>   *electrons_nLostHits;
   vector<float>   *electrons_fbrem;
   vector<float>   *electrons_EseedOP;
   vector<int>     *electrons_class;
   vector<float>   *electrons_track_d0;
   vector<float>   *electrons_track_dz;
   vector<float>   *electrons_track_d0err;
   vector<float>   *electrons_track_dzerr;
   vector<float>   *electrons_flag_mask;
   TClonesArray    *priVtx_3vec;
   vector<float>   *priVtx_xxE;
   vector<float>   *priVtx_yyE;
   vector<float>   *priVtx_zzE;
   vector<float>   *priVtx_yxE;
   vector<float>   *priVtx_zyE;
   vector<float>   *priVtx_zxE;
   vector<float>   *priVtx_chi2;
   vector<float>   *priVtx_ndof;
   vector<int>     *nEvent;
   vector<int>     *nRun;
   vector<int>     *nLumi;
   TClonesArray    *beamSpot_3vec;
   vector<float>   *beamSpot_xxE;
   vector<float>   *beamSpot_yyE;
   vector<float>   *beamSpot_zzE;
   vector<float>   *beamSpot_yxE;
   vector<float>   *beamSpot_zyE;
   vector<float>   *beamSpot_zxE;
   vector<int>     *QQ_size;
   vector<int>     *QQ_sign;
   vector<int>     *QQ_type;
   TClonesArray    *QQ_4mom;
   vector<int>     *QQ_lepone;
   vector<int>     *QQ_leptwo;
   vector<float>   *QQ_Tip;
   vector<float>   *QQ_Lip;
   vector<float>   *QQ_3Dip;
   vector<float>   *QQ_errTip;
   vector<float>   *QQ_errLip;
   vector<float>   *QQ_err3Dip;
   vector<float>   *QQ_STip;
   vector<float>   *QQ_SLip;
   vector<float>   *QQ_S3Dip;
   vector<float>   *QQ_Tip_BS;
   vector<float>   *QQ_Lip_BS;
   vector<float>   *QQ_3Dip_BS;
   vector<float>   *QQ_errTip_BS;
   vector<float>   *QQ_errLip_BS;
   vector<float>   *QQ_err3Dip_BS;
   vector<float>   *QQ_STip_BS;
   vector<float>   *QQ_SLip_BS;
   vector<float>   *QQ_S3Dip_BS;
   vector<float>   *QQ_normChi2;
   vector<float>   *QQ_probChi2;

   // List of branches
   TBranch        *b_muons_glb_4mom;   //!
   TBranch        *b_muons_glb_track4mom;   //!
   TBranch        *b_muons_glb_charge;   //!
   TBranch        *b_muons_glb_tkIsoR03;   //!
   TBranch        *b_muons_glb_nTkIsoR03;   //!
   TBranch        *b_muons_glb_emIsoR03;   //!
   TBranch        *b_muons_glb_hadIsoR03;   //!
   TBranch        *b_muons_glb_tkIsoR05;   //!
   TBranch        *b_muons_glb_nTkIsoR05;   //!
   TBranch        *b_muons_glb_emIsoR05;   //!
   TBranch        *b_muons_glb_hadIsoR05;   //!
   TBranch        *b_muons_glb_ptErr;   //!
   TBranch        *b_muons_glb_phiErr;   //!
   TBranch        *b_muons_glb_etaErr;   //!
   TBranch        *b_muons_glb_d0;   //!
   TBranch        *b_muons_glb_d0err;   //!
   TBranch        *b_muons_glb_dz;   //!
   TBranch        *b_muons_glb_dzerr;   //!
   TBranch        *b_muons_glb_normChi2;   //!
   TBranch        *b_muons_glb_nhitstrack;   //!
   TBranch        *b_muons_glb_nhitsDT;   //!
   TBranch        *b_muons_glb_nhitsCSC;   //!
   TBranch        *b_muons_glb_nhitsStrip;   //!
   TBranch        *b_muons_glb_nhitsPixB;   //!
   TBranch        *b_muons_glb_nhitsPixE;   //!
   TBranch        *b_muons_glb_nhitsPix1Hit;   //!
   TBranch        *b_muons_glb_nhitsPix1HitBE;   //!
   TBranch        *b_muons_trk_4mom;   //!
   TBranch        *b_muons_trk_charge;   //!
   TBranch        *b_muons_trk_tkIsoR03;   //!
   TBranch        *b_muons_trk_nTkIsoR03;   //!
   TBranch        *b_muons_trk_emIsoR03;   //!
   TBranch        *b_muons_trk_hadIsoR03;   //!
   TBranch        *b_muons_trk_ptErr;   //!
   TBranch        *b_muons_trk_phiErr;   //!
   TBranch        *b_muons_trk_etaErr;   //!
   TBranch        *b_muons_trk_d0;   //!
   TBranch        *b_muons_trk_d0err;   //!
   TBranch        *b_muons_trk_dz;   //!
   TBranch        *b_muons_trk_dzerr;   //!
   TBranch        *b_muons_trk_normChi2;   //!
   TBranch        *b_muons_trk_nhitstrack;   //!
   TBranch        *b_muons_trk_nhitsStrip;   //!
   TBranch        *b_muons_trk_nhitsPixB;   //!
   TBranch        *b_muons_trk_nhitsPixE;   //!
   TBranch        *b_muons_trk_nhitsPix1Hit;   //!
   TBranch        *b_muons_trk_nhitsPix1HitBE;   //!
   TBranch        *b_muons_trk_PIDmask;   //!
   TBranch        *b_electrons;   //!
   TBranch        *b_electrons_charge;   //!
   TBranch        *b_electrons_tkIso;   //!
   TBranch        *b_electrons_emIso;   //!
   TBranch        *b_electrons_hadIso;   //!
   TBranch        *b_electrons_IdBDT;   //!
   TBranch        *b_electrons_IdLoose;   //!
   TBranch        *b_electrons_IdTight;   //!
   TBranch        *b_electrons_IdRobustLoose;   //!
   TBranch        *b_electrons_IdRobustTight;   //!
   TBranch        *b_electrons_HoE;   //!
   TBranch        *b_electrons_dEtaSc;   //!
   TBranch        *b_electrons_dPhiSc;   //!
   TBranch        *b_electrons_SigiEtaiEtaSc;   //!
   TBranch        *b_electrons_EoP;   //!
   TBranch        *b_electrons_Et;   //!
   TBranch        *b_electrons_pAtVtx;   //!
   TBranch        *b_electrons_ptAtVtx;   //!
   TBranch        *b_electrons_nLostHits;   //!
   TBranch        *b_electrons_fbrem;   //!
   TBranch        *b_electrons_EseedOP;   //!
   TBranch        *b_electrons_class;   //!
   TBranch        *b_electrons_track_d0;   //!
   TBranch        *b_electrons_track_dz;   //!
   TBranch        *b_electrons_track_d0err;   //!
   TBranch        *b_electrons_track_dzerr;   //!
   TBranch        *b_electrons_flag_mask;   //!
   TBranch        *b_priVtx_3vec;   //!
   TBranch        *b_priVtx_xxE;   //!
   TBranch        *b_priVtx_yyE;   //!
   TBranch        *b_priVtx_zzE;   //!
   TBranch        *b_priVtx_yxE;   //!
   TBranch        *b_priVtx_zyE;   //!
   TBranch        *b_priVtx_zxE;   //!
   TBranch        *b_priVtx_chi2;   //!
   TBranch        *b_priVtx_ndof;   //!
   TBranch        *b_nEvent;   //!
   TBranch        *b_nRun;   //!
   TBranch        *b_nLumi;   //!
   TBranch        *b_beamSpot_3vec;   //!
   TBranch        *b_beamSpot_xxE;   //!
   TBranch        *b_beamSpot_yyE;   //!
   TBranch        *b_beamSpot_zzE;   //!
   TBranch        *b_beamSpot_yxE;   //!
   TBranch        *b_beamSpot_zyE;   //!
   TBranch        *b_beamSpot_zxE;   //!
   TBranch        *b_QQ_size;   //!
   TBranch        *b_QQ_sign;   //!
   TBranch        *b_QQ_type;   //!
   TBranch        *b_QQ_4mom;   //!
   TBranch        *b_QQ_lepone;   //!
   TBranch        *b_QQ_leptwo;   //!
   TBranch        *b_QQ_Tip;   //!
   TBranch        *b_QQ_Lip;   //!
   TBranch        *b_QQ_3Dip;   //!
   TBranch        *b_QQ_errTip;   //!
   TBranch        *b_QQ_errLip;   //!
   TBranch        *b_QQ_err3Dip;   //!
   TBranch        *b_QQ_STip;   //!
   TBranch        *b_QQ_SLip;   //!
   TBranch        *b_QQ_S3Dip;   //!
   TBranch        *b_QQ_Tip_BS;   //!
   TBranch        *b_QQ_Lip_BS;   //!
   TBranch        *b_QQ_3Dip_BS;   //!
   TBranch        *b_QQ_errTip_BS;   //!
   TBranch        *b_QQ_errLip_BS;   //!
   TBranch        *b_QQ_err3Dip_BS;   //!
   TBranch        *b_QQ_STip_BS;   //!
   TBranch        *b_QQ_SLip_BS;   //!
   TBranch        *b_QQ_S3Dip_BS;   //!
   TBranch        *b_QQ_normChi2;   //!
   TBranch        *b_QQ_probChi2;   //!

   smallHBaseClass(TTree *tree=0);
   virtual ~smallHBaseClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef smallHBaseClass_cxx
smallHBaseClass::smallHBaseClass(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("SimpleNtple.root");
      if (!f) {
         f = new TFile("SimpleNtple.root");
      }
      tree = (TTree*)gDirectory->Get("OniaTree");

   }
   Init(tree);
}

smallHBaseClass::~smallHBaseClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t smallHBaseClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t smallHBaseClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void smallHBaseClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   muons_glb_4mom = 0;
   muons_glb_track4mom = 0;
   muons_glb_charge = 0;
   muons_glb_tkIsoR03 = 0;
   muons_glb_nTkIsoR03 = 0;
   muons_glb_emIsoR03 = 0;
   muons_glb_hadIsoR03 = 0;
   muons_glb_tkIsoR05 = 0;
   muons_glb_nTkIsoR05 = 0;
   muons_glb_emIsoR05 = 0;
   muons_glb_hadIsoR05 = 0;
   muons_glb_ptErr = 0;
   muons_glb_phiErr = 0;
   muons_glb_etaErr = 0;
   muons_glb_d0 = 0;
   muons_glb_d0err = 0;
   muons_glb_dz = 0;
   muons_glb_dzerr = 0;
   muons_glb_normChi2 = 0;
   muons_glb_nhitstrack = 0;
   muons_glb_nhitsDT = 0;
   muons_glb_nhitsCSC = 0;
   muons_glb_nhitsStrip = 0;
   muons_glb_nhitsPixB = 0;
   muons_glb_nhitsPixE = 0;
   muons_glb_nhitsPix1Hit = 0;
   muons_glb_nhitsPix1HitBE = 0;
   muons_trk_4mom = 0;
   muons_trk_charge = 0;
   muons_trk_tkIsoR03 = 0;
   muons_trk_nTkIsoR03 = 0;
   muons_trk_emIsoR03 = 0;
   muons_trk_hadIsoR03 = 0;
   muons_trk_ptErr = 0;
   muons_trk_phiErr = 0;
   muons_trk_etaErr = 0;
   muons_trk_d0 = 0;
   muons_trk_d0err = 0;
   muons_trk_dz = 0;
   muons_trk_dzerr = 0;
   muons_trk_normChi2 = 0;
   muons_trk_nhitstrack = 0;
   muons_trk_nhitsStrip = 0;
   muons_trk_nhitsPixB = 0;
   muons_trk_nhitsPixE = 0;
   muons_trk_nhitsPix1Hit = 0;
   muons_trk_nhitsPix1HitBE = 0;
   muons_trk_PIDmask = 0;
   electrons = 0;
   electrons_charge = 0;
   electrons_tkIso = 0;
   electrons_emIso = 0;
   electrons_hadIso = 0;
   electrons_IdBDT = 0;
   electrons_IdLoose = 0;
   electrons_IdTight = 0;
   electrons_IdRobustLoose = 0;
   electrons_IdRobustTight = 0;
   electrons_HoE = 0;
   electrons_dEtaSc = 0;
   electrons_dPhiSc = 0;
   electrons_SigiEtaiEtaSc = 0;
   electrons_EoP = 0;
   electrons_Et = 0;
   electrons_pAtVtx = 0;
   electrons_ptAtVtx = 0;
   electrons_nLostHits = 0;
   electrons_fbrem = 0;
   electrons_EseedOP = 0;
   electrons_class = 0;
   electrons_track_d0 = 0;
   electrons_track_dz = 0;
   electrons_track_d0err = 0;
   electrons_track_dzerr = 0;
   electrons_flag_mask = 0;
   priVtx_3vec = 0;
   priVtx_xxE = 0;
   priVtx_yyE = 0;
   priVtx_zzE = 0;
   priVtx_yxE = 0;
   priVtx_zyE = 0;
   priVtx_zxE = 0;
   priVtx_chi2 = 0;
   priVtx_ndof = 0;
   nEvent = 0;
   nRun = 0;
   nLumi = 0;
   beamSpot_3vec = 0;
   beamSpot_xxE = 0;
   beamSpot_yyE = 0;
   beamSpot_zzE = 0;
   beamSpot_yxE = 0;
   beamSpot_zyE = 0;
   beamSpot_zxE = 0;
   QQ_size = 0;
   QQ_sign = 0;
   QQ_type = 0;
   QQ_4mom = 0;
   QQ_lepone = 0;
   QQ_leptwo = 0;
   QQ_Tip = 0;
   QQ_Lip = 0;
   QQ_3Dip = 0;
   QQ_errTip = 0;
   QQ_errLip = 0;
   QQ_err3Dip = 0;
   QQ_STip = 0;
   QQ_SLip = 0;
   QQ_S3Dip = 0;
   QQ_Tip_BS = 0;
   QQ_Lip_BS = 0;
   QQ_3Dip_BS = 0;
   QQ_errTip_BS = 0;
   QQ_errLip_BS = 0;
   QQ_err3Dip_BS = 0;
   QQ_STip_BS = 0;
   QQ_SLip_BS = 0;
   QQ_S3Dip_BS = 0;
   QQ_normChi2 = 0;
   QQ_probChi2 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("muons_glb_4mom", &muons_glb_4mom, &b_muons_glb_4mom);
   fChain->SetBranchAddress("muons_glb_track4mom", &muons_glb_track4mom, &b_muons_glb_track4mom);
   fChain->SetBranchAddress("muons_glb_charge", &muons_glb_charge, &b_muons_glb_charge);
   fChain->SetBranchAddress("muons_glb_tkIsoR03", &muons_glb_tkIsoR03, &b_muons_glb_tkIsoR03);
   fChain->SetBranchAddress("muons_glb_nTkIsoR03", &muons_glb_nTkIsoR03, &b_muons_glb_nTkIsoR03);
   fChain->SetBranchAddress("muons_glb_emIsoR03", &muons_glb_emIsoR03, &b_muons_glb_emIsoR03);
   fChain->SetBranchAddress("muons_glb_hadIsoR03", &muons_glb_hadIsoR03, &b_muons_glb_hadIsoR03);
   fChain->SetBranchAddress("muons_glb_tkIsoR05", &muons_glb_tkIsoR05, &b_muons_glb_tkIsoR05);
   fChain->SetBranchAddress("muons_glb_nTkIsoR05", &muons_glb_nTkIsoR05, &b_muons_glb_nTkIsoR05);
   fChain->SetBranchAddress("muons_glb_emIsoR05", &muons_glb_emIsoR05, &b_muons_glb_emIsoR05);
   fChain->SetBranchAddress("muons_glb_hadIsoR05", &muons_glb_hadIsoR05, &b_muons_glb_hadIsoR05);
   fChain->SetBranchAddress("muons_glb_ptErr", &muons_glb_ptErr, &b_muons_glb_ptErr);
   fChain->SetBranchAddress("muons_glb_phiErr", &muons_glb_phiErr, &b_muons_glb_phiErr);
   fChain->SetBranchAddress("muons_glb_etaErr", &muons_glb_etaErr, &b_muons_glb_etaErr);
   fChain->SetBranchAddress("muons_glb_d0", &muons_glb_d0, &b_muons_glb_d0);
   fChain->SetBranchAddress("muons_glb_d0err", &muons_glb_d0err, &b_muons_glb_d0err);
   fChain->SetBranchAddress("muons_glb_dz", &muons_glb_dz, &b_muons_glb_dz);
   fChain->SetBranchAddress("muons_glb_dzerr", &muons_glb_dzerr, &b_muons_glb_dzerr);
   fChain->SetBranchAddress("muons_glb_normChi2", &muons_glb_normChi2, &b_muons_glb_normChi2);
   fChain->SetBranchAddress("muons_glb_nhitstrack", &muons_glb_nhitstrack, &b_muons_glb_nhitstrack);
   fChain->SetBranchAddress("muons_glb_nhitsDT", &muons_glb_nhitsDT, &b_muons_glb_nhitsDT);
   fChain->SetBranchAddress("muons_glb_nhitsCSC", &muons_glb_nhitsCSC, &b_muons_glb_nhitsCSC);
   fChain->SetBranchAddress("muons_glb_nhitsStrip", &muons_glb_nhitsStrip, &b_muons_glb_nhitsStrip);
   fChain->SetBranchAddress("muons_glb_nhitsPixB", &muons_glb_nhitsPixB, &b_muons_glb_nhitsPixB);
   fChain->SetBranchAddress("muons_glb_nhitsPixE", &muons_glb_nhitsPixE, &b_muons_glb_nhitsPixE);
   fChain->SetBranchAddress("muons_glb_nhitsPix1Hit", &muons_glb_nhitsPix1Hit, &b_muons_glb_nhitsPix1Hit);
   fChain->SetBranchAddress("muons_glb_nhitsPix1HitBE", &muons_glb_nhitsPix1HitBE, &b_muons_glb_nhitsPix1HitBE);
   fChain->SetBranchAddress("muons_trk_4mom", &muons_trk_4mom, &b_muons_trk_4mom);
   fChain->SetBranchAddress("muons_trk_charge", &muons_trk_charge, &b_muons_trk_charge);
   fChain->SetBranchAddress("muons_trk_tkIsoR03", &muons_trk_tkIsoR03, &b_muons_trk_tkIsoR03);
   fChain->SetBranchAddress("muons_trk_nTkIsoR03", &muons_trk_nTkIsoR03, &b_muons_trk_nTkIsoR03);
   fChain->SetBranchAddress("muons_trk_emIsoR03", &muons_trk_emIsoR03, &b_muons_trk_emIsoR03);
   fChain->SetBranchAddress("muons_trk_hadIsoR03", &muons_trk_hadIsoR03, &b_muons_trk_hadIsoR03);
   fChain->SetBranchAddress("muons_trk_ptErr", &muons_trk_ptErr, &b_muons_trk_ptErr);
   fChain->SetBranchAddress("muons_trk_phiErr", &muons_trk_phiErr, &b_muons_trk_phiErr);
   fChain->SetBranchAddress("muons_trk_etaErr", &muons_trk_etaErr, &b_muons_trk_etaErr);
   fChain->SetBranchAddress("muons_trk_d0", &muons_trk_d0, &b_muons_trk_d0);
   fChain->SetBranchAddress("muons_trk_d0err", &muons_trk_d0err, &b_muons_trk_d0err);
   fChain->SetBranchAddress("muons_trk_dz", &muons_trk_dz, &b_muons_trk_dz);
   fChain->SetBranchAddress("muons_trk_dzerr", &muons_trk_dzerr, &b_muons_trk_dzerr);
   fChain->SetBranchAddress("muons_trk_normChi2", &muons_trk_normChi2, &b_muons_trk_normChi2);
   fChain->SetBranchAddress("muons_trk_nhitstrack", &muons_trk_nhitstrack, &b_muons_trk_nhitstrack);
   fChain->SetBranchAddress("muons_trk_nhitsStrip", &muons_trk_nhitsStrip, &b_muons_trk_nhitsStrip);
   fChain->SetBranchAddress("muons_trk_nhitsPixB", &muons_trk_nhitsPixB, &b_muons_trk_nhitsPixB);
   fChain->SetBranchAddress("muons_trk_nhitsPixE", &muons_trk_nhitsPixE, &b_muons_trk_nhitsPixE);
   fChain->SetBranchAddress("muons_trk_nhitsPix1Hit", &muons_trk_nhitsPix1Hit, &b_muons_trk_nhitsPix1Hit);
   fChain->SetBranchAddress("muons_trk_nhitsPix1HitBE", &muons_trk_nhitsPix1HitBE, &b_muons_trk_nhitsPix1HitBE);
   fChain->SetBranchAddress("muons_trk_PIDmask", &muons_trk_PIDmask, &b_muons_trk_PIDmask);
   fChain->SetBranchAddress("electrons", &electrons, &b_electrons);
   fChain->SetBranchAddress("electrons_charge", &electrons_charge, &b_electrons_charge);
   fChain->SetBranchAddress("electrons_tkIso", &electrons_tkIso, &b_electrons_tkIso);
   fChain->SetBranchAddress("electrons_emIso", &electrons_emIso, &b_electrons_emIso);
   fChain->SetBranchAddress("electrons_hadIso", &electrons_hadIso, &b_electrons_hadIso);
   fChain->SetBranchAddress("electrons_IdBDT", &electrons_IdBDT, &b_electrons_IdBDT);
   fChain->SetBranchAddress("electrons_IdLoose", &electrons_IdLoose, &b_electrons_IdLoose);
   fChain->SetBranchAddress("electrons_IdTight", &electrons_IdTight, &b_electrons_IdTight);
   fChain->SetBranchAddress("electrons_IdRobustLoose", &electrons_IdRobustLoose, &b_electrons_IdRobustLoose);
   fChain->SetBranchAddress("electrons_IdRobustTight", &electrons_IdRobustTight, &b_electrons_IdRobustTight);
   fChain->SetBranchAddress("electrons_HoE", &electrons_HoE, &b_electrons_HoE);
   fChain->SetBranchAddress("electrons_dEtaSc", &electrons_dEtaSc, &b_electrons_dEtaSc);
   fChain->SetBranchAddress("electrons_dPhiSc", &electrons_dPhiSc, &b_electrons_dPhiSc);
   fChain->SetBranchAddress("electrons_SigiEtaiEtaSc", &electrons_SigiEtaiEtaSc, &b_electrons_SigiEtaiEtaSc);
   fChain->SetBranchAddress("electrons_EoP", &electrons_EoP, &b_electrons_EoP);
   fChain->SetBranchAddress("electrons_Et", &electrons_Et, &b_electrons_Et);
   fChain->SetBranchAddress("electrons_pAtVtx", &electrons_pAtVtx, &b_electrons_pAtVtx);
   fChain->SetBranchAddress("electrons_ptAtVtx", &electrons_ptAtVtx, &b_electrons_ptAtVtx);
   fChain->SetBranchAddress("electrons_#LostHits", &electrons_nLostHits, &b_electrons_nLostHits);
   fChain->SetBranchAddress("electrons_fbrem", &electrons_fbrem, &b_electrons_fbrem);
   fChain->SetBranchAddress("electrons_EseedOP", &electrons_EseedOP, &b_electrons_EseedOP);
   fChain->SetBranchAddress("electrons_class", &electrons_class, &b_electrons_class);
   fChain->SetBranchAddress("electrons_track_d0", &electrons_track_d0, &b_electrons_track_d0);
   fChain->SetBranchAddress("electrons_track_dz", &electrons_track_dz, &b_electrons_track_dz);
   fChain->SetBranchAddress("electrons_track_d0err", &electrons_track_d0err, &b_electrons_track_d0err);
   fChain->SetBranchAddress("electrons_track_dzerr", &electrons_track_dzerr, &b_electrons_track_dzerr);
   fChain->SetBranchAddress("electrons_flag_mask", &electrons_flag_mask, &b_electrons_flag_mask);
   fChain->SetBranchAddress("priVtx_3vec", &priVtx_3vec, &b_priVtx_3vec);
   fChain->SetBranchAddress("priVtx_xxE", &priVtx_xxE, &b_priVtx_xxE);
   fChain->SetBranchAddress("priVtx_yyE", &priVtx_yyE, &b_priVtx_yyE);
   fChain->SetBranchAddress("priVtx_zzE", &priVtx_zzE, &b_priVtx_zzE);
   fChain->SetBranchAddress("priVtx_yxE", &priVtx_yxE, &b_priVtx_yxE);
   fChain->SetBranchAddress("priVtx_zyE", &priVtx_zyE, &b_priVtx_zyE);
   fChain->SetBranchAddress("priVtx_zxE", &priVtx_zxE, &b_priVtx_zxE);
   fChain->SetBranchAddress("priVtx_chi2", &priVtx_chi2, &b_priVtx_chi2);
   fChain->SetBranchAddress("priVtx_ndof", &priVtx_ndof, &b_priVtx_ndof);
   fChain->SetBranchAddress("nEvent", &nEvent, &b_nEvent);
   fChain->SetBranchAddress("nRun", &nRun, &b_nRun);
   fChain->SetBranchAddress("nLumi", &nLumi, &b_nLumi);
   fChain->SetBranchAddress("beamSpot_3vec", &beamSpot_3vec, &b_beamSpot_3vec);
   fChain->SetBranchAddress("beamSpot_xxE", &beamSpot_xxE, &b_beamSpot_xxE);
   fChain->SetBranchAddress("beamSpot_yyE", &beamSpot_yyE, &b_beamSpot_yyE);
   fChain->SetBranchAddress("beamSpot_zzE", &beamSpot_zzE, &b_beamSpot_zzE);
   fChain->SetBranchAddress("beamSpot_yxE", &beamSpot_yxE, &b_beamSpot_yxE);
   fChain->SetBranchAddress("beamSpot_zyE", &beamSpot_zyE, &b_beamSpot_zyE);
   fChain->SetBranchAddress("beamSpot_zxE", &beamSpot_zxE, &b_beamSpot_zxE);
   fChain->SetBranchAddress("QQ_size", &QQ_size, &b_QQ_size);
   fChain->SetBranchAddress("QQ_sign", &QQ_sign, &b_QQ_sign);
   fChain->SetBranchAddress("QQ_type", &QQ_type, &b_QQ_type);
   fChain->SetBranchAddress("QQ_4mom", &QQ_4mom, &b_QQ_4mom);
   fChain->SetBranchAddress("QQ_lepone", &QQ_lepone, &b_QQ_lepone);
   fChain->SetBranchAddress("QQ_leptwo", &QQ_leptwo, &b_QQ_leptwo);
   fChain->SetBranchAddress("QQ_Tip", &QQ_Tip, &b_QQ_Tip);
   fChain->SetBranchAddress("QQ_Lip", &QQ_Lip, &b_QQ_Lip);
   fChain->SetBranchAddress("QQ_3Dip", &QQ_3Dip, &b_QQ_3Dip);
   fChain->SetBranchAddress("QQ_errTip", &QQ_errTip, &b_QQ_errTip);
   fChain->SetBranchAddress("QQ_errLip", &QQ_errLip, &b_QQ_errLip);
   fChain->SetBranchAddress("QQ_err3Dip", &QQ_err3Dip, &b_QQ_err3Dip);
   fChain->SetBranchAddress("QQ_STip", &QQ_STip, &b_QQ_STip);
   fChain->SetBranchAddress("QQ_SLip", &QQ_SLip, &b_QQ_SLip);
   fChain->SetBranchAddress("QQ_S3Dip", &QQ_S3Dip, &b_QQ_S3Dip);
   fChain->SetBranchAddress("QQ_Tip_BS", &QQ_Tip_BS, &b_QQ_Tip_BS);
   fChain->SetBranchAddress("QQ_Lip_BS", &QQ_Lip_BS, &b_QQ_Lip_BS);
   fChain->SetBranchAddress("QQ_3Dip_BS", &QQ_3Dip_BS, &b_QQ_3Dip_BS);
   fChain->SetBranchAddress("QQ_errTip_BS", &QQ_errTip_BS, &b_QQ_errTip_BS);
   fChain->SetBranchAddress("QQ_errLip_BS", &QQ_errLip_BS, &b_QQ_errLip_BS);
   fChain->SetBranchAddress("QQ_err3Dip_BS", &QQ_err3Dip_BS, &b_QQ_err3Dip_BS);
   fChain->SetBranchAddress("QQ_STip_BS", &QQ_STip_BS, &b_QQ_STip_BS);
   fChain->SetBranchAddress("QQ_SLip_BS", &QQ_SLip_BS, &b_QQ_SLip_BS);
   fChain->SetBranchAddress("QQ_S3Dip_BS", &QQ_S3Dip_BS, &b_QQ_S3Dip_BS);
   fChain->SetBranchAddress("QQ_normChi2", &QQ_normChi2, &b_QQ_normChi2);
   fChain->SetBranchAddress("QQ_probChi2", &QQ_probChi2, &b_QQ_probChi2);
   Notify();
}

Bool_t smallHBaseClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void smallHBaseClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t smallHBaseClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef smallHBaseClass_cxx

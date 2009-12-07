#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/MCHistograms.h"





//! ctor ------------------------------------------------------------------------

MCHistograms::MCHistograms(const int& step):
 m_step(step)
{
  char histoName[50];
  
  
  
  // ----------
  // tag quarks
  // ----------
  
  sprintf(histoName, "%d_mcQ_tag_pt", m_step);
  mcQ_tag_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);

  sprintf(histoName, "%d_mcQ_tag_mass", m_step);
  mcQ_tag_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);

  sprintf(histoName, "%d_mcQ_tag_eta", m_step);
  mcQ_tag_eta = new TH1F(histoName, histoName, 2000, -10., 10.);
  
  sprintf(histoName, "%d_mcQ_tag_absEta", m_step);
  mcQ_tag_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  
  
  sprintf(histoName, "%d_mcQQ_tag_pt", m_step);
  mcQQ_tag_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcQQ_tag_mass", m_step);
  mcQQ_tag_mass = new TH1F(histoName, histoName, 2500, 0., 5000.);
  
  sprintf(histoName, "%d_mcQQ_tag_eta", m_step);
  mcQQ_tag_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_mcQQ_tag_absEta", m_step);
  mcQQ_tag_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);

  sprintf(histoName, "%d_mcQQ_tag_Dphi", m_step);
  mcQQ_tag_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcQQ_tag_Deta", m_step);
  mcQQ_tag_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcQQ_tag_DR", m_step);
  mcQQ_tag_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcQQ_tag_angle", m_step);
  mcQQ_tag_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcQQ_tag_etaProd", m_step);
  mcQQ_tag_etaProd = new TH1F(histoName, histoName, 3, -1., 2.);
  
  
  
  
  
  
  // --------------
  // quarks from W2
  // --------------
  
  sprintf(histoName, "%d_mcQ_W_pt", m_step);
  mcQ_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcQ_W_mass", m_step);
  mcQ_W_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcQ_W_eta", m_step);
  mcQ_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);
  
  sprintf(histoName, "%d_mcQ_W_absEta", m_step);
  mcQ_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcQ_W_zepp", m_step);
  mcQ_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  sprintf(histoName, "%d_mcQ_W_pdgId", m_step);
  mcQ_W_pdgId = new TH1F(histoName, histoName, 40, -20., 20.);
  
  
  
  sprintf(histoName, "%d_mcQQ_W_pt", m_step);
  mcQQ_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcQQ_W_mass", m_step);
  mcQQ_W_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcQQ_W_eta", m_step);
  mcQQ_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_mcQQ_W_absEta", m_step);
  mcQQ_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcQQ_W_Dphi", m_step);
  mcQQ_W_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcQQ_W_Deta", m_step);
  mcQQ_W_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcQQ_W_DR", m_step);
  mcQQ_W_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcQQ_W_angle", m_step);
  mcQQ_W_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcQQ_W_zepp", m_step);
  mcQQ_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  
  
  
  
  
  // ---------------
  // leptons from W1
  // ---------------
  
  sprintf(histoName, "%d_mcL_W_pt", m_step);
  mcL_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcL_W_mass", m_step);
  mcL_W_mass = new TH1F(histoName, histoName, 2000, 0., 10.);
  
  sprintf(histoName, "%d_mcL_W_eta", m_step);
  mcL_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);
  
  sprintf(histoName, "%d_mcL_W_absEta", m_step);
  mcL_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcL_W_zepp", m_step);
  mcL_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  sprintf(histoName, "%d_mcL_W_pdgId", m_step);
  mcL_W_pdgId = new TH1F(histoName, histoName, 40, -20., 20.);
  
  
  
  sprintf(histoName, "%d_mcNu_W_pt", m_step);
  mcNu_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcNu_W_mass", m_step);
  mcNu_W_mass = new TH1F(histoName, histoName, 2000, 0., 10.);
  
  sprintf(histoName, "%d_mcNu_W_eta", m_step);
  mcNu_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);
  
  sprintf(histoName, "%d_mcNu_W_absEta", m_step);
  mcNu_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcNu_W_zepp", m_step);
  mcNu_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  sprintf(histoName, "%d_mcNu_W_pdgId", m_step);
  mcNu_W_pdgId = new TH1F(histoName, histoName, 40, -20., 20.);
  
  
  
  sprintf(histoName, "%d_mcLNu_W_pt", m_step);
  mcLNu_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcLNu_W_mass", m_step);
  mcLNu_W_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcLNu_W_eta", m_step);
  mcLNu_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_mcLNu_W_absEta", m_step);
  mcLNu_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLNu_W_Dphi", m_step);
  mcLNu_W_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcLNu_W_Deta", m_step);
  mcLNu_W_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLNu_W_DR", m_step);
  mcLNu_W_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLNu_W_angle", m_step);
  mcLNu_W_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcLNu_W_zepp", m_step);
  mcLNu_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  
  
  
  
  
  // -----
  // higgs
  // -----
  
  sprintf(histoName, "%d_mcLQQ_pt", m_step);
  mcLQQ_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcLQQ_mass", m_step);
  mcLQQ_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcLQQ_eta", m_step);
  mcLQQ_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_mcLQQ_absEta", m_step);
  mcLQQ_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLQQ_Dphi", m_step);
  mcLQQ_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcLQQ_Deta", m_step);
  mcLQQ_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLQQ_DR", m_step);
  mcLQQ_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLQQ_angle", m_step);
  mcLQQ_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcLQQ_zepp", m_step);
  mcLQQ_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  
  
  sprintf(histoName, "%d_mcLNuQQ_pt", m_step);
  mcLNuQQ_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcLNuQQ_mass", m_step);
  mcLNuQQ_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_mcLNuQQ_eta", m_step);
  mcLNuQQ_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_mcLNuQQ_absEta", m_step);
  mcLNuQQ_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLNuQQ_Dphi", m_step);
  mcLNuQQ_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcLNuQQ_Deta", m_step);
  mcLNuQQ_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLNuQQ_DR", m_step);
  mcLNuQQ_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_mcLNuQQ_angle", m_step);
  mcLNuQQ_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_mcLNuQQ_zepp", m_step);
  mcLNuQQ_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
}

//-------------------------------------------------------------------------------






//! dtor ------------------------------------------------------------------------
//
//MCHistograms::~MCHistograms()
//
//-------------------------------------------------------------------------------






//! Method ------------------------------------------------------------------------

void MCHistograms::FillMCHistograms(const VBFNtupleContent& treeVars)
{
  // ----------
  // tag quarks
  // ----------
  
  TLorentzVector mcQ_tag1(treeVars.mcQ_tag_px[0],
                          treeVars.mcQ_tag_py[0],
                          treeVars.mcQ_tag_pz[0],
                          treeVars.mcQ_tag_energy[0]);
  
  TLorentzVector mcQ_tag2(treeVars.mcQ_tag_px[1],
                          treeVars.mcQ_tag_py[1],
                          treeVars.mcQ_tag_pz[1],
                          treeVars.mcQ_tag_energy[1]);
  
  TLorentzVector mcQQ_tag = mcQ_tag1 + mcQ_tag2;
  
  float tagQQ_avgEta = 0.5 * (mcQ_tag1.Eta() + mcQ_tag2.Eta());
  float tagQQ_Deta   =   fabs(mcQ_tag1.Eta() - mcQ_tag2.Eta());
  
  
  
  mcQ_tag_pt     -> Fill( mcQ_tag1.Perp() );
  mcQ_tag_pt     -> Fill( mcQ_tag2.Perp() );
  mcQ_tag_mass   -> Fill( mcQ_tag1.M() );
  mcQ_tag_mass   -> Fill( mcQ_tag2.M() );
  mcQ_tag_eta    -> Fill( mcQ_tag1.Eta() );
  mcQ_tag_eta    -> Fill( mcQ_tag2.Eta() );
  mcQ_tag_absEta -> Fill( fabs(mcQ_tag1.Eta()) );
  mcQ_tag_absEta -> Fill( fabs(mcQ_tag2.Eta()) );
  
  mcQQ_tag_pt      -> Fill( mcQQ_tag.Perp() );
  mcQQ_tag_mass    -> Fill( mcQQ_tag.M() );
  mcQQ_tag_eta     -> Fill( mcQQ_tag.Eta() );
  mcQQ_tag_absEta  -> Fill( fabs(mcQQ_tag.Eta()) );
  mcQQ_tag_Dphi    -> Fill( fabs(deltaPhi(mcQ_tag1.Phi(), mcQ_tag2.Phi())) );
  mcQQ_tag_Deta    -> Fill( fabs(mcQ_tag1.Eta() -  mcQ_tag2.Eta()) );
  mcQQ_tag_DR      -> Fill( deltaR(mcQ_tag1.Eta(), mcQ_tag1.Phi(), mcQ_tag2.Eta(), mcQ_tag2.Phi()) );
  mcQQ_tag_angle   -> Fill( (mcQ_tag1.Vect()).Angle(mcQ_tag2.Vect()) );
  mcQQ_tag_etaProd -> Fill( mcQ_tag1.Eta()*mcQ_tag2.Eta() / fabs(mcQ_tag1.Eta() * mcQ_tag2.Eta()) );
  
  
  
  
  
  
  // --------------
  // quarks from W2
  // --------------
  
  TLorentzVector mcQ_W_1(treeVars.mcQ_fromW2_px[0],
                         treeVars.mcQ_fromW2_py[0],
                         treeVars.mcQ_fromW2_pz[0],
                         treeVars.mcQ_fromW2_energy[0]);
  
  TLorentzVector mcQ_W_2(treeVars.mcQ_fromW2_px[1],
                         treeVars.mcQ_fromW2_py[1],
                         treeVars.mcQ_fromW2_pz[1],
                         treeVars.mcQ_fromW2_energy[1]);
  
  TLorentzVector mcQQ_W = mcQ_W_1 + mcQ_W_2;
  
  
  
  mcQ_W_pt     -> Fill( mcQ_W_1.Perp() );
  mcQ_W_pt     -> Fill( mcQ_W_2.Perp() );
  mcQ_W_mass   -> Fill( mcQ_W_1.M() );
  mcQ_W_mass   -> Fill( mcQ_W_2.M() );
  mcQ_W_eta    -> Fill( mcQ_W_1.Eta() );
  mcQ_W_eta    -> Fill( mcQ_W_2.Eta() );
  mcQ_W_absEta -> Fill( fabs(mcQ_W_1.Eta()) );
  mcQ_W_absEta -> Fill( fabs(mcQ_W_2.Eta()) );
  mcQ_W_zepp   -> Fill( fabs((mcQ_W_1.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
  mcQ_W_zepp   -> Fill( fabs((mcQ_W_2.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
  mcQ_W_pdgId  -> Fill( treeVars.mcQ_fromW2_pdgId[0] );
  mcQ_W_pdgId  -> Fill( treeVars.mcQ_fromW2_pdgId[1] );
  
  mcQQ_W_pt     -> Fill( mcQQ_W.Perp() );
  mcQQ_W_mass   -> Fill( mcQQ_W.M() );
  mcQQ_W_eta    -> Fill( mcQQ_W.Eta() );
  mcQQ_W_absEta -> Fill( fabs(mcQQ_W.Eta()) );
  mcQQ_W_Dphi   -> Fill( fabs(deltaPhi(mcQ_W_1.Phi(), mcQ_W_2.Phi())) );
  mcQQ_W_Deta   -> Fill( fabs(mcQ_W_1.Eta() -  mcQ_W_2.Eta()) );
  mcQQ_W_DR     -> Fill( deltaR(mcQ_W_1.Eta(), mcQ_W_1.Phi(), mcQ_W_2.Eta(), mcQ_W_2.Phi()) );
  mcQQ_W_angle  -> Fill( (mcQ_W_1.Vect()).Angle(mcQ_W_2.Vect()) );
  mcQQ_W_zepp   -> Fill( fabs((mcQQ_W.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
  
  
  
  
  
  
  // ---------------
  // leptons from W1
  // ---------------
  
  TLorentzVector mcL_W(treeVars.mcF_fromW1_px[0],
                       treeVars.mcF_fromW1_py[0],
                       treeVars.mcF_fromW1_pz[0],
                       treeVars.mcF_fromW1_energy[0]);
  int pdgId1 = treeVars.mcF_fromW1_pdgId[0];
  
  TLorentzVector mcNu_W(treeVars.mcF_fromW1_px[1],
                        treeVars.mcF_fromW1_py[1],
                        treeVars.mcF_fromW1_pz[1],
                        treeVars.mcF_fromW1_energy[1]);
  int pdgId2 = treeVars.mcF_fromW1_pdgId[1];
  
  if(treeVars.mcF_fromW1_pdgId[0]%2 == 0)
  {
    TLorentzVector buffer = mcL_W;
    mcL_W = mcNu_W;
    mcNu_W = buffer;
    
    pdgId1 = treeVars.mcF_fromW1_pdgId[1];
    pdgId2 = treeVars.mcF_fromW1_pdgId[0];
  }
  
  TLorentzVector mcLNu_W = mcL_W + mcNu_W;
  
  
  
  mcL_W_pt     -> Fill( mcL_W.Perp() );
  mcL_W_mass   -> Fill( mcL_W.M() );
  mcL_W_eta    -> Fill( mcL_W.Eta() );
  mcL_W_absEta -> Fill( fabs(mcL_W.Eta()) );
  mcL_W_zepp   -> Fill( fabs((mcL_W.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
  mcL_W_pdgId  -> Fill( pdgId1 );
  
  mcNu_W_pt     -> Fill( mcNu_W.Perp() );
  mcNu_W_mass   -> Fill( mcNu_W.M() );
  mcNu_W_eta    -> Fill( mcNu_W.Eta() );
  mcNu_W_absEta -> Fill( fabs(mcNu_W.Eta()) );
  mcNu_W_zepp   -> Fill( fabs((mcNu_W.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
  mcNu_W_pdgId  -> Fill( pdgId2 ); 
  
  mcLNu_W_pt     -> Fill( mcLNu_W.Perp() );
  mcLNu_W_mass   -> Fill( mcLNu_W.M() );
  mcLNu_W_eta    -> Fill( mcLNu_W.Eta() );
  mcLNu_W_absEta -> Fill( fabs(mcLNu_W.Eta()) );
  mcLNu_W_Dphi   -> Fill( fabs(deltaPhi(mcL_W.Phi(), mcNu_W.Phi())) );
  mcLNu_W_Deta   -> Fill( fabs(mcL_W.Eta() -  mcNu_W.Eta()) );
  mcLNu_W_DR     -> Fill( deltaR(mcL_W.Eta(), mcL_W.Phi(), mcNu_W.Eta(), mcNu_W.Phi()) );
  mcLNu_W_angle  -> Fill( (mcL_W.Vect()).Angle(mcNu_W.Vect()) );
  mcLNu_W_zepp   -> Fill( fabs((mcLNu_W.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
  
  
  
  // -----
  // higgs
  // -----
  
  TLorentzVector mcLQQ = mcL_W + mcQQ_W;  
  TLorentzVector mcLNuQQ = mcLNu_W + mcQQ_W;  
  
  mcLQQ_pt     -> Fill( mcLQQ.Perp() );
  mcLQQ_mass   -> Fill( mcLQQ.M() );
  mcLQQ_eta    -> Fill( mcLQQ.Eta() );
  mcLQQ_absEta -> Fill( fabs(mcLQQ.Eta()) );
  mcLQQ_Dphi   -> Fill( fabs(deltaPhi(mcL_W.Phi(), mcQQ_W.Phi())) );
  mcLQQ_Deta   -> Fill( fabs(mcL_W.Eta() -  mcQQ_W.Eta()) );
  mcLQQ_DR     -> Fill( deltaR(mcL_W.Eta(), mcL_W.Phi(), mcQQ_W.Eta(), mcQQ_W.Phi()) );
  mcLQQ_angle  -> Fill( (mcL_W.Vect()).Angle(mcQQ_W.Vect()) );
  mcLQQ_zepp   -> Fill( fabs((mcLQQ.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
  
  mcLNuQQ_pt     -> Fill( mcLNuQQ.Perp() );
  mcLNuQQ_mass   -> Fill( mcLNuQQ.M() );
  mcLNuQQ_eta    -> Fill( mcLNuQQ.Eta() );
  mcLNuQQ_absEta -> Fill( fabs(mcLNuQQ.Eta()) );
  mcLNuQQ_Dphi   -> Fill( fabs(deltaPhi(mcLNu_W.Phi(), mcQQ_W.Phi())) );
  mcLNuQQ_Deta   -> Fill( fabs(mcLNu_W.Eta() -  mcQQ_W.Eta()) );
  mcLNuQQ_DR     -> Fill( deltaR(mcLNu_W.Eta(), mcLNu_W.Phi(), mcQQ_W.Eta(), mcQQ_W.Phi()) );
  mcLNuQQ_angle  -> Fill( (mcLNu_W.Vect()).Angle(mcQQ_W.Vect()) );
  mcLNuQQ_zepp   -> Fill( fabs((mcLNuQQ.Eta() - tagQQ_avgEta) / tagQQ_Deta) );
}

//-------------------------------------------------------------------------------






//! Method ------------------------------------------------------------------------

void MCHistograms::WriteMCHistograms(TFile* outputRootFile)
{
  char dirName[50];
  TDirectory* sd1;
  
  
  
  sprintf(dirName, "MCHistograms_step%d", m_step);
  sd1 = outputRootFile -> mkdir(dirName);
  sd1 -> cd();
  
  WriteNormalized(mcQ_tag_pt);
  WriteNormalized(mcQ_tag_mass);
  WriteNormalized(mcQ_tag_eta);
  WriteNormalized(mcQ_tag_absEta);

  WriteNormalized(mcQQ_tag_pt);
  WriteNormalized(mcQQ_tag_mass);
  WriteNormalized(mcQQ_tag_eta);
  WriteNormalized(mcQQ_tag_absEta);
  WriteNormalized(mcQQ_tag_Dphi);
  WriteNormalized(mcQQ_tag_Deta);
  WriteNormalized(mcQQ_tag_DR);
  WriteNormalized(mcQQ_tag_angle);
  WriteNormalized(mcQQ_tag_etaProd);
  
  
  
  WriteNormalized(mcQ_W_pt);
  WriteNormalized(mcQ_W_mass);
  WriteNormalized(mcQ_W_eta);
  WriteNormalized(mcQ_W_absEta);
  WriteNormalized(mcQ_W_zepp);
  WriteNormalized(mcQ_W_pdgId);
  
  WriteNormalized(mcQQ_W_pt);
  WriteNormalized(mcQQ_W_mass);
  WriteNormalized(mcQQ_W_eta);
  WriteNormalized(mcQQ_W_absEta);
  WriteNormalized(mcQQ_W_Dphi);
  WriteNormalized(mcQQ_W_Deta);
  WriteNormalized(mcQQ_W_DR);
  WriteNormalized(mcQQ_W_angle);
  WriteNormalized(mcQQ_W_zepp);
  
  
  
  WriteNormalized(mcL_W_pt);
  WriteNormalized(mcL_W_mass);
  WriteNormalized(mcL_W_eta);
  WriteNormalized(mcL_W_absEta);
  WriteNormalized(mcL_W_zepp);
  WriteNormalized(mcL_W_pdgId);
  
  WriteNormalized(mcNu_W_pt);
  WriteNormalized(mcNu_W_mass);
  WriteNormalized(mcNu_W_eta);
  WriteNormalized(mcNu_W_absEta);
  WriteNormalized(mcNu_W_zepp);
  WriteNormalized(mcNu_W_pdgId);
  
  WriteNormalized(mcLNu_W_pt);
  WriteNormalized(mcLNu_W_mass);
  WriteNormalized(mcLNu_W_eta);
  WriteNormalized(mcLNu_W_absEta);
  WriteNormalized(mcLNu_W_Dphi);
  WriteNormalized(mcLNu_W_Deta);
  WriteNormalized(mcLNu_W_DR);
  WriteNormalized(mcLNu_W_angle);
  WriteNormalized(mcLNu_W_zepp);
  
  
  
  WriteNormalized(mcLQQ_pt);
  WriteNormalized(mcLQQ_mass);
  WriteNormalized(mcLQQ_eta);
  WriteNormalized(mcLQQ_absEta);
  WriteNormalized(mcLQQ_Dphi);
  WriteNormalized(mcLQQ_Deta);
  WriteNormalized(mcLQQ_DR);
  WriteNormalized(mcLQQ_angle);
  WriteNormalized(mcLQQ_zepp);
  
  WriteNormalized(mcLNuQQ_pt);
  WriteNormalized(mcLNuQQ_mass);
  WriteNormalized(mcLNuQQ_eta);
  WriteNormalized(mcLNuQQ_absEta);
  WriteNormalized(mcLNuQQ_Dphi);
  WriteNormalized(mcLNuQQ_Deta);
  WriteNormalized(mcLNuQQ_DR);
  WriteNormalized(mcLNuQQ_angle);
  WriteNormalized(mcLNuQQ_zepp);
  
  outputRootFile -> cd();
}

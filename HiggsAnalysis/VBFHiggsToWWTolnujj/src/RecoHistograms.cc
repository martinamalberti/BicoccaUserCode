#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/RecoHistograms.h"





//! ctor ------------------------------------------------------------------------

RecoHistograms::RecoHistograms(const int& step):
 m_step(step)
{
  char histoName[50];
  
  
  
  // ---
  // met
  // ---
  
  sprintf(histoName, "%d_recoCaloMet_pt", m_step);
  recoCaloMet_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoMuonCorrectedCaloMet_pt", m_step);
  recoMuonCorrectedCaloMet_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  
  
  
  
  
  // ----------
  // tag jets
  // ----------
  
  sprintf(histoName, "%d_recoJ_tag_pt", m_step);
  recoJ_tag_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoJ_tag_pt1", m_step);
  recoJ_tag_pt1 = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoJ_tag_pt2", m_step);
  recoJ_tag_pt2 = new TH1F(histoName, histoName, 2000, 0., 1000.);

  sprintf(histoName, "%d_recoJ_tag_mass", m_step);
  recoJ_tag_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);

  sprintf(histoName, "%d_recoJ_tag_eta", m_step);
  recoJ_tag_eta = new TH1F(histoName, histoName, 2000, -10., 10.);
  
  sprintf(histoName, "%d_recoJ_tag_absEta", m_step);
  recoJ_tag_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  
  
  sprintf(histoName, "%d_recoJJ_tag_pt", m_step);
  recoJJ_tag_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoJJ_tag_mass", m_step);
  recoJJ_tag_mass = new TH1F(histoName, histoName, 2500, 0., 5000.);
  
  sprintf(histoName, "%d_recoJJ_tag_eta", m_step);
  recoJJ_tag_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_recoJJ_tag_absEta", m_step);
  recoJJ_tag_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);

  sprintf(histoName, "%d_recoJJ_tag_Dphi", m_step);
  recoJJ_tag_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_recoJJ_tag_Deta", m_step);
  recoJJ_tag_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoJJ_tag_DR", m_step);
  recoJJ_tag_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoJJ_tag_angle", m_step);
  recoJJ_tag_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_recoJJ_tag_etaProd", m_step);
  recoJJ_tag_etaProd = new TH1F(histoName, histoName, 3, -1., 2.);
  
  
  
  
  
  
  // --------------
  // jets from W2
  // --------------
  
  sprintf(histoName, "%d_recoJ_W_pt", m_step);
  recoJ_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoJ_W_mass", m_step);
  recoJ_W_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoJ_W_eta", m_step);
  recoJ_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);
  
  sprintf(histoName, "%d_recoJ_W_absEta", m_step);
  recoJ_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoJ_W_zepp", m_step);
  recoJ_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  
  
  sprintf(histoName, "%d_recoJJ_W_pt", m_step);
  recoJJ_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoJJ_W_mass", m_step);
  recoJJ_W_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoJJ_W_eta", m_step);
  recoJJ_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_recoJJ_W_absEta", m_step);
  recoJJ_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoJJ_W_Dphi", m_step);
  recoJJ_W_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_recoJJ_W_Deta", m_step);
  recoJJ_W_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoJJ_W_DR", m_step);
  recoJJ_W_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoJJ_W_angle", m_step);
  recoJJ_W_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_recoJJ_W_etRatio", m_step);
  recoJJ_W_etRatio = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoJJ_W_zepp", m_step);
  recoJJ_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  
  
  
  
  
  // ------------
  // central jets
  // ------------
  
  sprintf(histoName, "%d_recoJ_central_n", m_step);
  recoJ_central_n = new TH1F(histoName, histoName, 10, 0., 10.);
  
  
  
  
  
  
  // --------------
  // lepton from W1
  // --------------
  
  sprintf(histoName, "%d_recoL_W_pt", m_step);
  recoL_W_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoL_W_mass", m_step);
  recoL_W_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoL_W_eta", m_step);
  recoL_W_eta = new TH1F(histoName, histoName, 2000, -10., 10.);
  
  sprintf(histoName, "%d_recoL_W_absEta", m_step);
  recoL_W_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoL_W_zepp", m_step);
  recoL_W_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
  
  sprintf(histoName, "%d_recoL_W_tkIsoOverPt", m_step);
  recoL_W_tkIsoOverPt = new TH1F(histoName, histoName, 1000, 0., 1.);
  
  sprintf(histoName, "%d_recoL_W_emIsoOverPt", m_step);
  recoL_W_emIsoOverPt = new TH1F(histoName, histoName, 1000, 0., 1.);
  
  sprintf(histoName, "%d_recoL_W_hadIsoOverPt", m_step);
  recoL_W_hadIsoOverPt = new TH1F(histoName, histoName, 1000, 0., 1.);
  
  
  
  
  
  // ----------
  // reco higgs
  // ----------
  
  sprintf(histoName, "%d_recoLJJ_pt", m_step);
  recoLJJ_pt = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoLJJ_mass", m_step);
  recoLJJ_mass = new TH1F(histoName, histoName, 2000, 0., 1000.);
  
  sprintf(histoName, "%d_recoLJJ_eta", m_step);
  recoLJJ_eta = new TH1F(histoName, histoName, 2000, -10., 10.);

  sprintf(histoName, "%d_recoLJJ_absEta", m_step);
  recoLJJ_absEta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoLJJ_Dphi", m_step);
  recoLJJ_Dphi = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_recoLJJ_Deta", m_step);
  recoLJJ_Deta = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoLJJ_DR", m_step);
  recoLJJ_DR = new TH1F(histoName, histoName, 1000, 0., 10.);
  
  sprintf(histoName, "%d_recoLJJ_angle", m_step);
  recoLJJ_angle = new TH1F(histoName, histoName, 500, 0., 5.);
  
  sprintf(histoName, "%d_recoLJJ_zepp", m_step);
  recoLJJ_zepp = new TH1F(histoName, histoName, 100, 0., 1.);
}

//-------------------------------------------------------------------------------






//! dtor ------------------------------------------------------------------------
//
//RecoHistograms::~RecoHistograms()
//
//-------------------------------------------------------------------------------






//! Method ------------------------------------------------------------------------

void RecoHistograms::FillRecoHistograms(const VBFNtupleContent& treeVars,
                                        const std::vector<int>* recoJet_tag_it,
                                        const std::vector<int>* recoJet_W_it,
                                        const std::vector<int>* recoL_W_it)
{
  TLorentzVector recoJ_tag1;
  TLorentzVector recoJ_tag2;
  TLorentzVector recoJJ_tag;
  
  double tagJJ_avgEta = 0.;
  double tagJJ_Deta = 0.;
  
  TLorentzVector recoJ_W_1;
  TLorentzVector recoJ_W_2;
  TLorentzVector recoJJ_W;
  
  TLorentzVector recoL_W;  
  
  TLorentzVector recoLJJ;  
  
  
  
  
  
  
  // ---
  // met
  // ---
  
  recoCaloMet_pt -> Fill(treeVars.caloMet);
  recoMuonCorrectedCaloMet_pt -> Fill(treeVars.muonCorrectedCaloMet);
  
  
  
  
  
  
  // --------
  // tag jets
  // --------
  
  if(recoJet_tag_it)
  {
    int it1 = recoJet_tag_it -> at(0);
    int it2 = recoJet_tag_it -> at(1);
    
    
    
   recoJ_tag1.SetXYZT(treeVars.recoJet_px[it1],
                      treeVars.recoJet_py[it1],
                      treeVars.recoJet_pz[it1],
                      treeVars.recoJet_energy[it1]);
    
   recoJ_tag2.SetXYZT(treeVars.recoJet_px[it2],
                      treeVars.recoJet_py[it2],
                      treeVars.recoJet_pz[it2],
                      treeVars.recoJet_energy[it2]);
    
    recoJJ_tag = recoJ_tag1 + recoJ_tag2;
    
    tagJJ_avgEta = 0.5 * (recoJ_tag1.Eta() + recoJ_tag2.Eta());
    tagJJ_Deta   =   fabs(recoJ_tag1.Eta() - recoJ_tag2.Eta());
    
    
    
    recoJ_tag_pt     -> Fill( recoJ_tag1.Perp() );
    recoJ_tag_pt     -> Fill( recoJ_tag2.Perp() );
    recoJ_tag_pt1    -> Fill( std::max(recoJ_tag1.Perp(), recoJ_tag2.Perp()) );
    recoJ_tag_pt2    -> Fill( std::min(recoJ_tag1.Perp(), recoJ_tag2.Perp()) );
    recoJ_tag_mass   -> Fill( recoJ_tag1.M() );
    recoJ_tag_mass   -> Fill( recoJ_tag2.M() );
    recoJ_tag_eta    -> Fill( recoJ_tag1.Eta() );
    recoJ_tag_eta    -> Fill( recoJ_tag2.Eta() );
    recoJ_tag_absEta -> Fill( fabs(recoJ_tag1.Eta()) );
    recoJ_tag_absEta -> Fill( fabs(recoJ_tag2.Eta()) );
    
    recoJJ_tag_pt      -> Fill( recoJJ_tag.Perp() );
    recoJJ_tag_mass    -> Fill( recoJJ_tag.M() );
    recoJJ_tag_eta     -> Fill( recoJJ_tag.Eta() );
    recoJJ_tag_absEta  -> Fill( fabs(recoJJ_tag.Eta()) );
    recoJJ_tag_Dphi    -> Fill( fabs(deltaPhi(recoJ_tag1.Phi(), recoJ_tag2.Phi())) );
    recoJJ_tag_Deta    -> Fill( fabs(recoJ_tag1.Eta() -  recoJ_tag2.Eta()) );
    recoJJ_tag_DR      -> Fill( deltaR(recoJ_tag1.Eta(), recoJ_tag1.Phi(), recoJ_tag2.Eta(), recoJ_tag2.Phi()) );
    recoJJ_tag_angle   -> Fill( (recoJ_tag1.Vect()).Angle(recoJ_tag2.Vect()) );
    recoJJ_tag_etaProd -> Fill( recoJ_tag1.Eta()*recoJ_tag2.Eta() / fabs(recoJ_tag1.Eta() * recoJ_tag2.Eta()) );
  }
  
  
  
  
  
  
  // --------------
  // jets from W2
  // --------------
  
  if(recoJet_W_it)
  {
    int it1 = recoJet_W_it -> at(0);
    int it2 = recoJet_W_it -> at(1);
    
    
    
    recoJ_W_1.SetXYZT(treeVars.recoJet_px[it1],
                      treeVars.recoJet_py[it1],
                      treeVars.recoJet_pz[it1],
                      treeVars.recoJet_energy[it1]);
    
    recoJ_W_2.SetXYZT(treeVars.recoJet_px[it2],
                      treeVars.recoJet_py[it2],
                      treeVars.recoJet_pz[it2],
                      treeVars.recoJet_energy[it2]);
    
    recoJJ_W = recoJ_W_1 + recoJ_W_2;
    
    
    
    recoJ_W_pt     -> Fill( recoJ_W_1.Perp() );
    recoJ_W_pt     -> Fill( recoJ_W_2.Perp() );
    recoJ_W_mass   -> Fill( recoJ_W_1.M() );
    recoJ_W_mass   -> Fill( recoJ_W_2.M() );
    recoJ_W_eta    -> Fill( recoJ_W_1.Eta() );
    recoJ_W_eta    -> Fill( recoJ_W_2.Eta() );
    recoJ_W_absEta -> Fill( fabs(recoJ_W_1.Eta()) );
    recoJ_W_absEta -> Fill( fabs(recoJ_W_2.Eta()) );
    recoJ_W_zepp   -> Fill( fabs((recoJ_W_1.Eta() - tagJJ_avgEta) / tagJJ_Deta) );
    recoJ_W_zepp   -> Fill( fabs((recoJ_W_2.Eta() - tagJJ_avgEta) / tagJJ_Deta) );
    
    recoJJ_W_pt      -> Fill( recoJJ_W.Perp() );
    recoJJ_W_mass    -> Fill( recoJJ_W.M() );
    recoJJ_W_eta     -> Fill( recoJJ_W.Eta() );
    recoJJ_W_absEta  -> Fill( fabs(recoJJ_W.Eta()) );
    recoJJ_W_Dphi    -> Fill( fabs(deltaPhi(recoJ_W_1.Phi(), recoJ_W_2.Phi())) );
    recoJJ_W_Deta    -> Fill( fabs(recoJ_W_1.Eta() -  recoJ_W_2.Eta()) );
    recoJJ_W_DR      -> Fill( deltaR(recoJ_W_1.Eta(), recoJ_W_1.Phi(), recoJ_W_2.Eta(), recoJ_W_2.Phi()) );
    recoJJ_W_angle   -> Fill( (recoJ_W_1.Vect()).Angle(recoJ_W_2.Vect()) );
    recoJJ_W_etRatio -> Fill( treeVars.recoJet_et[it1] / treeVars.recoJet_et[it2] );
    recoJJ_W_zepp    -> Fill( fabs((recoJJ_W.Eta() - tagJJ_avgEta) / tagJJ_Deta) );
  }
  
  
  
  
  
  
  // ------------
  // central jets
  // ------------
  
  int nRecoJets_central = 0;
  
  for(unsigned int jetIt = 0; jetIt < treeVars.nRecoJet; ++jetIt)
  {
    bool skipJet = false;
    
    if(recoJet_tag_it)
      for(unsigned int kk = 0; kk < recoJet_tag_it -> size(); ++kk)
        if(recoJet_tag_it -> at(kk) == static_cast<int>(jetIt)) skipJet = true;
    
    if(recoJet_W_it)
      for(unsigned int kk = 0; kk < recoJet_W_it -> size(); ++kk)
        if(recoJet_W_it -> at(kk) == static_cast<int>(jetIt)) skipJet = true;
    
    if(skipJet) continue;
    
    if( fabs(treeVars.recoJet_eta[jetIt]) < 3. ) ++ nRecoJets_central;
  }  
  
  recoJ_central_n -> Fill(nRecoJets_central);
  
  
  
  
  
  
  // ----------------------
  // lepton from W1 - higgs
  // ----------------------
  
  if(recoL_W_it)
  {
    int it = recoL_W_it -> at(1);
    
    
    if(recoL_W_it -> at(0) == 0)
      recoL_W.SetXYZT(treeVars.recoE_px[it],
                      treeVars.recoE_py[it],
                      treeVars.recoE_pz[it],
                      treeVars.recoE_energy[it]);
    
    if(recoL_W_it -> at(0) == 1)
      recoL_W.SetXYZT(treeVars.recoMu_px[it],
                      treeVars.recoMu_py[it],
                      treeVars.recoMu_pz[it],
                      treeVars.recoMu_energy[it]);
    
    recoL_W_pt     -> Fill( recoL_W.Perp() );
    recoL_W_mass   -> Fill( recoL_W.M() );
    recoL_W_eta    -> Fill( recoL_W.Eta() );
    recoL_W_absEta -> Fill( fabs(recoL_W.Eta()) );
    recoL_W_zepp   -> Fill( fabs((recoL_W.Eta() - tagJJ_avgEta) / tagJJ_Deta) ); 
    
    if(recoL_W_it -> at(0) == 0)
    {
      recoL_W_tkIsoOverPt  -> Fill( treeVars.recoE_tkIso[it]  / recoL_W.Perp() );
      recoL_W_emIsoOverPt  -> Fill( treeVars.recoE_emIso[it]  / recoL_W.Perp() );
      recoL_W_hadIsoOverPt -> Fill( treeVars.recoE_hadIso[it] / recoL_W.Perp() );
    }
    
    if(recoL_W_it -> at(0) == 1)
    {
      recoL_W_tkIsoOverPt  -> Fill( treeVars.recoMu_tkIsoR03[it]  / recoL_W.Perp() );
      recoL_W_emIsoOverPt  -> Fill( treeVars.recoMu_emIsoR03[it]  / recoL_W.Perp() );
      recoL_W_hadIsoOverPt -> Fill( treeVars.recoMu_hadIsoR03[it] / recoL_W.Perp() );
    }
    
    
    
    
    
    recoLJJ = recoL_W + recoJJ_W;
    
    recoLJJ_pt      -> Fill( recoLJJ.Perp() );
    recoLJJ_mass    -> Fill( recoLJJ.M() );
    recoLJJ_eta     -> Fill( recoLJJ.Eta() );
    recoLJJ_absEta  -> Fill( fabs(recoLJJ.Eta()) );
    recoLJJ_Dphi    -> Fill( fabs(deltaPhi(recoL_W.Phi(), recoJJ_W.Phi())) );
    recoLJJ_Deta    -> Fill( fabs(recoL_W.Eta() -  recoJJ_W.Eta()) );
    recoLJJ_DR      -> Fill( deltaR(recoL_W.Eta(), recoL_W.Phi(), recoJJ_W.Eta(), recoJJ_W.Phi()) );
    recoLJJ_angle   -> Fill( (recoL_W.Vect()).Angle(recoJJ_W.Vect()) );
    recoLJJ_zepp    -> Fill( fabs((recoLJJ.Eta() - tagJJ_avgEta) / tagJJ_Deta) );
  }
  
  
  
}

//-------------------------------------------------------------------------------






//! Method ------------------------------------------------------------------------

void RecoHistograms::WriteRecoHistograms(TFile* outputRootFile)
{
  char dirName[50];
  TDirectory* sd1;
  
  
  
  sprintf(dirName, "RecoHistograms_step%d", m_step);
  sd1 = outputRootFile -> mkdir(dirName);
  sd1 -> cd();
  
  WriteNormalized(recoCaloMet_pt);
  WriteNormalized(recoMuonCorrectedCaloMet_pt);
  
  WriteNormalized(recoJ_tag_pt);
  WriteNormalized(recoJ_tag_pt1);
  WriteNormalized(recoJ_tag_pt2);
  WriteNormalized(recoJ_tag_mass);
  WriteNormalized(recoJ_tag_eta);
  WriteNormalized(recoJ_tag_absEta);
  
  WriteNormalized(recoJJ_tag_pt);
  WriteNormalized(recoJJ_tag_mass);
  WriteNormalized(recoJJ_tag_eta);
  WriteNormalized(recoJJ_tag_absEta);
  WriteNormalized(recoJJ_tag_Dphi);
  WriteNormalized(recoJJ_tag_Deta);
  WriteNormalized(recoJJ_tag_DR);
  WriteNormalized(recoJJ_tag_angle);
  WriteNormalized(recoJJ_tag_etaProd);
  
  
  
  
  WriteNormalized(recoJ_W_pt);
  WriteNormalized(recoJ_W_mass);
  WriteNormalized(recoJ_W_eta);
  WriteNormalized(recoJ_W_absEta);
  WriteNormalized(recoJ_W_zepp);
  
  WriteNormalized(recoJJ_W_pt);
  WriteNormalized(recoJJ_W_mass);
  WriteNormalized(recoJJ_W_eta);
  WriteNormalized(recoJJ_W_absEta);
  WriteNormalized(recoJJ_W_Dphi);
  WriteNormalized(recoJJ_W_Deta);
  WriteNormalized(recoJJ_W_DR);
  WriteNormalized(recoJJ_W_angle);
  WriteNormalized(recoJJ_W_etRatio);
  WriteNormalized(recoJJ_W_zepp);
  
  
  
  WriteNormalized(recoJ_central_n);
  
  
  
  WriteNormalized(recoL_W_pt);
  WriteNormalized(recoL_W_mass);
  WriteNormalized(recoL_W_eta);
  WriteNormalized(recoL_W_absEta);
  WriteNormalized(recoL_W_zepp);
  WriteNormalized(recoL_W_tkIsoOverPt);
  WriteNormalized(recoL_W_emIsoOverPt);
  WriteNormalized(recoL_W_hadIsoOverPt);
  
  
  
  WriteNormalized(recoLJJ_pt);
  WriteNormalized(recoLJJ_mass);
  WriteNormalized(recoLJJ_eta);
  WriteNormalized(recoLJJ_absEta);
  WriteNormalized(recoLJJ_Dphi);
  WriteNormalized(recoLJJ_Deta);
  WriteNormalized(recoLJJ_DR);
  WriteNormalized(recoLJJ_angle);
  WriteNormalized(recoLJJ_zepp);
  
  outputRootFile -> cd();
}

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleUtils.h"





double SelectJets(std::vector<int>& it, VBFNtupleContent& treeVars, const std::string& method,
                  const double& etMin, const std::vector<int>* blacklist)
{
  // initialize vector with result
  it.clear();
  it.push_back(-1);
  it.push_back(-1);
  
  
  
  // initialize the selection variable
  double maxMJJ = -999999.;
  double tempMJJ = 0.;
  
  double maxPt = -999999.;
  double tempPt = 0.;
  
  double maxSumPt = -999999.;
  double tempSumPt = 0.;
  
  
  
  // loop over 1st jet
  for(unsigned int i = 0; i < treeVars.nRecoJet; ++i)
  {
    if(treeVars.recoJet_et[i] < etMin) continue;
    
    bool skipJet1 = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipJet1 = true;
    if(skipJet1) continue;
    
    
    TLorentzVector tempJ1(treeVars.recoJet_px[i],
                          treeVars.recoJet_py[i],
                          treeVars.recoJet_pz[i],
                          treeVars.recoJet_energy[i]);
    
    
    // loop over 2nd jet
    for(unsigned int j = i+1; j < treeVars.nRecoJet; ++j)
    {
      if(treeVars.recoJet_et[j] < etMin) continue;
      
      bool skipJet2 = false;
      if(blacklist)
        for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
          if(blacklist -> at(kk) == static_cast<int>(j)) skipJet2 = true;
      if(skipJet2) continue;
      
      
      TLorentzVector tempJ2(treeVars.recoJet_px[j],
                            treeVars.recoJet_py[j],
                            treeVars.recoJet_pz[j],
                            treeVars.recoJet_energy[j]);
      
      
      
      // -------------------------------------
      // select jets with different techniques
      // -------------------------------------
      
      // -------------------------------------
      if(method == "maxMJJ")
      {
        tempMJJ = (tempJ1 + tempJ2).M();
        if(tempMJJ > maxMJJ)
        {
          maxMJJ = tempMJJ;
          
	  it.at(0) = i;
	  it.at(1) = j;
        }
      }
      
      // -------------------------------------
      else if(method == "maxPt")
      {
        tempPt = (tempJ1 + tempJ2).Perp();
        if(tempPt > maxPt)
        {
          maxPt = tempPt;
          
	  it.at(0) = i;
	  it.at(1) = j;
        }
      }
      
      // -------------------------------------
      else if(method == "maxSumPt")
      {
        tempSumPt = tempJ1.Perp() + tempJ2.Perp();
        if(tempSumPt > maxSumPt)
        {
          maxSumPt = tempSumPt;
          
	  it.at(0) = i;
	  it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      
     
    } // loop over 2nd jet
  } // loop over 1st jet
  
  
  
  if(method == "maxMJJ")
    return maxMJJ;
  
  else if(method == "maxPt")
    return maxPt;
  
  else if(method == "maxSumPt")
    return maxSumPt;
  
  else return -1.;
}






double Select4Jets(std::vector<int>& tagIt, std::vector<int>& WIt, VBFNtupleContent& treeVars,
                   const double& etMin)
{
  // initialize vector with result
  tagIt.clear();
  tagIt.push_back(-1);
  tagIt.push_back(-1);
  
  WIt.clear();
  WIt.push_back(-1);
  WIt.push_back(-1);
  
  
  
  // initialize the selection variable
  double maxMJJ = -999999.;
  double tempMJJ = 0.;
  double tempDeta = 0.;
  
  double maxSumPt = -999999.;
  double tempSumPt = 0.;
  
  
  
  // loop over 1st tag jet
  for(unsigned int i1 = 0; i1 < treeVars.nRecoJet; ++i1)
  {
    if(treeVars.recoJet_et[i1] < etMin) continue;
    
    TLorentzVector tempJ1(treeVars.recoJet_px[i1],
                          treeVars.recoJet_py[i1],
                          treeVars.recoJet_pz[i1],
                          treeVars.recoJet_energy[i1]);
    
    
    // loop over 2nd tag jet
    for(unsigned int i2 = i1+1; i2 < treeVars.nRecoJet; ++i2)
    {
      if(treeVars.recoJet_et[i2] < etMin) continue;
      
      TLorentzVector tempJ2(treeVars.recoJet_px[i2],
                            treeVars.recoJet_py[i2],
                            treeVars.recoJet_pz[i2],
                            treeVars.recoJet_energy[i2]);
      
      
      
      // --------------
      // apply VBF cuts
      // --------------
      
      tempMJJ = (tempJ1 + tempJ2).M();
      tempDeta = fabs(tempJ1.Eta() - tempJ2.Eta());
      
      if(tempMJJ < mJJ_tag_MIN) continue;
      if(tempDeta < Deta_tag_MIN) continue;





      if(tempMJJ > maxMJJ)
      {
        maxMJJ = tempMJJ;
        
        tagIt.at(0) = i1;
	tagIt.at(1) = i2;
        }
      }
      
}

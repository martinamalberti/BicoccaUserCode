#include "ntpleUtils.h"



std::map<int, int> GetTotalEvents(const std::string& histoName, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;
  std::map<int, int> totalEvents;
  
  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return totalEvents;
  }
  
  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;

    TFile* f = new TFile(buffer.c_str(), "READ");
    TH1F* histo = (TH1F*)(f -> Get(histoName.c_str()));
    
    for(int bin = 1; bin <= histo -> GetNbinsX(); ++bin)
      totalEvents[bin] += int(histo -> GetBinContent(bin));
    
    f -> Close();
    
    delete f;
  }

  return totalEvents;
}

//  ------------------------------------------------------------


bool FillChain(TChain& chain, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;

  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }
  
  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;
    chain.Add(buffer.c_str());
  }

  return true;
}

//  ------------------------------------------------------------

int parseConfigFile (const TString& config)
{
  std::cout << ">>> Parsing " << config << " file" << std::endl ;
  
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser();
  
  if( !(gConfigParser -> init(config)) )
  {
    std::cout << ">>> parseConfigFile::Could not open configuration file "
              << config << std::endl;
     return -1;
  }
  
  gConfigParser -> print();
  
  return 0 ;
}

//  ------------------------------------------------------------






double deltaPhi(const double& phi1, const double& phi2)
{ 
  double deltaphi = fabs(phi1 - phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
  return deltaphi;
}

//  ------------------------------------------------------------

double deltaEta(const double& eta1, const double& eta2)
{ 
  double deltaeta = fabs(eta1 - eta2);
  return deltaeta;
}

//  ------------------------------------------------------------

double deltaR(const double& eta1, const double& phi1,
              const double& eta2, const double& phi2)
{ 
 double deltaphi = deltaPhi(phi1, phi2);
 double deltaeta = deltaEta(eta1, eta2);
 double deltar = sqrt(deltaphi*deltaphi + deltaeta*deltaeta);
 return deltar;
}

//  ------------------------------------------------------------



int getCJV(std::vector<ROOT::Math::XYZTVector>& jets,
	      int q1,
	      int q2,
	      const double& EtMin,
	      const std::vector<int>* blacklist){
 
 int CJV = 0;
 double etaMin = jets.at(q1).Eta();
 double etaMax = jets.at(q2).Eta();
 
 if (etaMax < etaMin) std::swap(etaMin,etaMax);
 
 for(unsigned int i = 0; i < jets.size(); ++i)
 {
  if (i==q1 || i==q2) continue;
  if (jets.at(i).Et() < EtMin) continue;
  
  bool skipJet = false;
  if(blacklist){
   for(unsigned int kk = 0; kk < blacklist -> size(); ++kk) {
    if(blacklist -> at(kk) == static_cast<int>(i)) skipJet = true;
   }
  }
  
  if (skipJet) continue;
   
  if(jets.at(i).Eta() > etaMax || jets.at(i).Eta() < etaMin) continue;
  
  CJV++;
 } 
 
 return CJV;
 
}


//  ------------------------------------------------------------


int getJV(std::vector<ROOT::Math::XYZTVector>& jets,
	      const double& EtMin,
	      const std::vector<int>* blacklist){
 
 int JV = 0;
  
 for(unsigned int i = 0; i < jets.size(); ++i)
 {
  if (jets.at(i).Et() < EtMin) continue;
  
  bool skipJet = false;
  if(blacklist){
   for(unsigned int kk = 0; kk < blacklist -> size(); ++kk) {
    if(blacklist -> at(kk) == static_cast<int>(i)) skipJet = true;
   }
  }
  
  if (skipJet) continue;
  JV++;  
 } 
 
 return JV;
 
}


//  ------------------------------------------------------------



double SelectJets(std::vector<int>& it, std::vector<ROOT::Math::XYZTVector>& jets,
                  const std::string& method,
                  const double& etMin,
                  const std::vector<int>* blacklist)
{
  // initialize vector with result
  it.clear();
  it.push_back(-1);
  it.push_back(-1);
  
  
  
  // initialize the selection variable
  double maxDeta = -999999.;
  double tempDeta = 0.;
  
  double maxMJJ = -999999.;
  double tempMJJ = 0.;
  
  double maxPt = -999999.;
  double tempPt = 0.;
  
  double maxSumPt = -999999.;
  double tempSumPt = 0.;
  
  
  
  // loop over 1st jet
  for(unsigned int i = 0; i < jets.size(); ++i)
  {
    if(jets.at(i).Et() < etMin) continue;
    
    bool skipJet1 = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipJet1 = true;
    if(skipJet1) continue;
    
    
    
    // loop over 2nd jet
    for(unsigned int j = i+1; j < jets.size(); ++j)
    {
      if(jets.at(j).Et() < etMin) continue;
      
      bool skipJet2 = false;
      if(blacklist)
        for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
          if(blacklist -> at(kk) == static_cast<int>(j)) skipJet2 = true;
      if(skipJet2) continue;
      
      
      
      // -------------------------------------
      // select jets with different techniques
      // -------------------------------------
      
      if(method == "maxDeta")
      {
        tempDeta = deltaEta(jets.at(i).Eta(), jets.at(j).Eta());
        if(tempDeta > maxDeta)
        {
          maxDeta = tempDeta;
          
    	    it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      if(method == "maxMJJ")
      {
        tempMJJ = (jets.at(i) + jets.at(j)).mass();
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
        tempPt = sqrt( (jets.at(i) + jets.at(j)).perp2() );
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
        tempSumPt = sqrt(jets.at(i).perp2()) + sqrt(jets.at(j).perp2());
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

//  ------------------------------------------------------------

int SelectLepton(std::vector<ROOT::Math::XYZTVector>& leptons,
                 const std::string& method,
                 const double& ptMin,
                 const std::vector<int>* blacklist)
{
  // initialize variable with result
  int it = -1;
  
  
  
  // initialize the selection variable
  double maxPt = -999999.;
  double tempPt = 0.;
  
  
  
  // loop over leptons
  for(unsigned int i = 0; i < leptons.size(); ++i)
  {
    if( sqrt(leptons.at(i).Perp2()) < ptMin ) continue;
    
    bool skipLep = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipLep = true;
    if(skipLep) continue;
    
    
    
    // -------------------------------------
    // select jets with different techniques
    // -------------------------------------
    
    if(method == "maxPt")
    {
      tempPt = sqrt(leptons.at(i).perp2());
      if(tempPt > maxPt)
      {
        maxPt = tempPt;
        
        it = i;
      }
    }
    
    // -------------------------------------
    
    
    
  } // loop over leptons
  
  
  
  if(method == "maxPt")
    return it;
  
  else return -1;
}

//  ------------------------------------------------------------






int Build4JetCombinations(std::vector<std::vector<int> >& combinations, const int& nJets)
{
  combinations.clear();
  
  std::vector<int> vi;
  for(int i = 0; i < nJets; ++i)
 	  vi.push_back(i);
  
  std::vector<int> buffer;
  buffer.push_back(0);
  buffer.push_back(1);
  buffer.push_back(2);
  buffer.push_back(3);
  
  combinations.push_back(buffer);
  

  std::vector<int> oldCombination = buffer;
  while( next_permutation(vi.begin(), vi.end()) )      
  {
    if( (vi.at(0) < vi.at(1)) && (vi.at(2) < vi.at(3)) )
    {
      buffer.at(0) = vi.at(0);
      buffer.at(1) = vi.at(1);
      buffer.at(2) = vi.at(2);
      buffer.at(3) = vi.at(3);                  
      
      if(buffer == oldCombination) continue;
      
      combinations.push_back(buffer);
      oldCombination = buffer;
    }  
  }
  
  return combinations.size();
}

//  ------------------------------------------------------------

void Print4JetCombination(const std::vector<int>& combination)
{
  std::cout << "(" << combination.at(0) << "," << combination.at(1) << ")";
  std::cout << "   ---   ";
  std::cout << "(" << combination.at(2) << "," << combination.at(3) << ")";  
  std::cout << std::endl;
}

//  ------------------------------------------------------------



int Build2JetCombinations(std::vector<std::vector<int> >& combinations, const int& nJets)
{
 combinations.clear();
 
 std::vector<int> vi;
 for(int i = 0; i < nJets; ++i)
  vi.push_back(i);
 
 std::vector<int> buffer;
 buffer.push_back(0);
 buffer.push_back(1);
 
 combinations.push_back(buffer);
 
 std::vector<int> oldCombination = buffer;
 while( next_permutation(vi.begin(), vi.end()) )      
 {
  if(vi.at(0) < vi.at(1))
  {
   buffer.at(0) = vi.at(0);
   buffer.at(1) = vi.at(1);

   if(buffer == oldCombination) continue;
   
   combinations.push_back(buffer);
   oldCombination = buffer;
  }  
 }
 
 return combinations.size();
}


//  ------------------------------------------------------------


double SelectResonance(std::vector<int>& it, std::vector<ROOT::Math::XYZTVector>& objects,
		       const double& mass,
		       const double& ptMin,
		       const std::vector<int>* blacklist){
 // initialize vector with result
 it.clear();
 it.push_back(-1);
 it.push_back(-1);
 
 double minDMass = 999999.;
 double tempDMass = 0;
 
 // loop over 1st object
 for(unsigned int i = 0; i < objects.size(); ++i){
  
  if(objects.at(i).Pt() < ptMin) continue;
  
  bool skipObj1 = false;
  if(blacklist)
   for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
    if(blacklist -> at(kk) == static_cast<int>(i)) skipObj1 = true;
    if(skipObj1) continue;
    
    // loop over 2nd object
    for(unsigned int j = i+1; j < objects.size(); ++j)
    {
     if(objects.at(j).Pt() < ptMin) continue;
     
     bool skipObj2 = false;
     if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
       if(blacklist -> at(kk) == static_cast<int>(j)) skipObj2 = true;
       if(skipObj2) continue;
       
       tempDMass = fabs((objects.at(i) + objects.at(j)).mass() - mass);
      if(tempDMass < minDMass)
      {
       minDMass = tempDMass;
       it.at(0) = i;
       it.at(1) = j;
      }
    } // loop over 2nd object
 } // loop over 1st object
 return minDMass;
}
//  ------------------------------------------------------------


double SelectResonanceOppositeCharge(std::vector<int>& it,
		       std::vector<ROOT::Math::XYZTVector>& objects,
		       std::vector<float>& charge,	     
		       const double& mass,
		       const double& ptMin,
		       const std::vector<int>* blacklist){
 // initialize vector with result
 it.clear();
 it.push_back(-1);
 it.push_back(-1);
 
 double minDMass = 999999.;
 double tempDMass = 0;
 
 // loop over 1st object
 for(unsigned int i = 0; i < objects.size(); ++i){
  if(objects.at(i).Pt() < ptMin) continue;
  double charge1 = charge.at(i);
  
  bool skipObj1 = false;
  if(blacklist)
   for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
    if(blacklist -> at(kk) == static_cast<int>(i)) skipObj1 = true;
    if(skipObj1) continue;
    
    // loop over 2nd object
    for(unsigned int j = i+1; j < objects.size(); ++j)
    {
     if(objects.at(j).Pt() < ptMin) continue;
     double charge2 = charge.at(j);     
     if(charge1 * charge2 > 0) continue;
     
     bool skipObj2 = false;
     
     if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
       if(blacklist -> at(kk) == static_cast<int>(j)) skipObj2 = true;
       if(skipObj2) continue;
       
       tempDMass = fabs((objects.at(i) + objects.at(j)).mass() - mass);
      if(tempDMass < minDMass)
      {
       minDMass = tempDMass;
       it.at(0) = i;
       it.at(1) = j;
      }
    } // loop over 2nd object
 } // loop over 1st object
 return minDMass;
}
//  ------------------------------------------------------------

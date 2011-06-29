#include "ClusterCalibTools.h"


//! ctor
EBregionBuilder::EBregionBuilder(int etaStart,int etaEnd,int etaWidth,int phiStart,int phiEnd,int phiWidth,const std::string& histoName):
 m_etaStart(etaShifter(etaStart)),  
 m_etaEnd  (etaShifter(etaEnd)),
 m_etaWidth(etaWidth),  
 m_phiStart(phiStart),
 m_phiEnd  (phiEnd),
 m_phiWidth(phiWidth),
 m_EBRegionHistoName(histoName)
{
  EBRegionDefinition();
  
  m_EBRegionHisto = new TH2F(m_EBRegionHistoName.c_str(),"EB region",171,-85,86,360,1,361);  
  for(int eta = -85; eta < 86; ++eta)
    for (int phi = 1; phi < 361; ++phi)
    {
      if( EBRegionId(eta,phi) < 0 ) continue;
      m_EBRegionHisto -> Fill(eta,phi,EBRegionId(eta,phi));
    }
  m_EBRegionHisto -> GetXaxis() -> SetTitle("i#eta");
  m_EBRegionHisto -> GetYaxis() -> SetTitle("i#phi");
}

//----------------------------------------



//!Shifts eta in other coordinates (from 0 to 170)
int EBregionBuilder::etaShifter(int etaOld) const
{
//  if (etaOld < 0) return etaOld + 85;
//   else if (etaOld > 0) return etaOld + 84;
//   else return etaOld;
  return etaOld;
}

//--------------------------------------------



//! Tells if you are in the region to be calibrated
int EBregionBuilder::EBregionCheck(int eta,int phi) const
{
  int etaLocal = etaShifter(eta);
  
  if( etaLocal < m_etaStart ) return 1;
  if( etaLocal >= m_etaEnd )  return 2;
  if( phi < m_phiStart ) return 3;
  if( phi >= m_phiEnd )  return 4;
  if( eta == 0) return 5;
  
  return 0;
}

//--------------------------------------------



//! number of regions in EB
int EBregionBuilder::EBregionsNum() const
{
  int nEtaEBP = 0;
  int nEtaEBM = 0;
  int dEtaEBP = 0;
  int dEtaEBM = 0;


  if (m_etaEnd > 0 && m_etaStart > 0) {
    dEtaEBP = m_etaEnd - m_etaStart ;
    dEtaEBM = 0;
  }
  
  if (m_etaEnd > 0 && m_etaStart < 0) {
    dEtaEBP = (m_etaEnd - 1) ;
    dEtaEBM = (0 - m_etaStart);
  }

  if (m_etaEnd < 0 && m_etaStart < 0) {
    dEtaEBP = 0 ;
    dEtaEBM = m_etaEnd - m_etaStart;
  }

  nEtaEBP=dEtaEBP/m_etaWidth;
  nEtaEBM=dEtaEBM/m_etaWidth;

  if ( (dEtaEBP%m_etaWidth)!=0 ) nEtaEBP+=1;
  if ( (dEtaEBM%m_etaWidth)!=0 ) nEtaEBM+=1;
  int neta    = nEtaEBP+nEtaEBM;

  int nphi = (m_phiEnd - m_phiStart) / m_phiWidth;
  if ( ((m_phiEnd - m_phiStart)%m_phiWidth)!=0 ) nphi+=1; 

  return ( neta*nphi ); 

}

//----------------------------------------



//! id of regions in EB
int EBregionBuilder::EBRegionId(int etaXtl,int phiXtl) const
{
  if( EBregionCheck(etaXtl,phiXtl) ) return -1;

  int Nphi = (m_phiEnd - m_phiStart) / m_phiWidth;
  int etaI = (etaXtl-m_etaStart)/m_etaWidth;  
  int phiI = (phiXtl-m_phiStart)/m_phiWidth; 
  int regionNumEB = 0;
  
  if ( m_etaStart > 0 && m_etaEnd > 0 ) 
    regionNumEB = phiI + Nphi * etaI;
 
  if ( m_etaStart < 0 && m_etaEnd < 0 ) {
    etaI = (abs(etaXtl)-abs(m_etaEnd))/m_etaWidth;
    regionNumEB = phiI + Nphi * etaI;
  }

  if ( m_etaStart < 0 && m_etaEnd > 0) {
    if ( etaXtl > 0 ) {
      etaI = (etaXtl-1)/m_etaWidth;
      regionNumEB = phiI + Nphi * etaI;
    }    
    if ( etaXtl < 0 ) {
      etaI = (abs(etaXtl)-1)/m_etaWidth; 
      int nEtaEBP = (m_etaEnd-1)/m_etaWidth;
      regionNumEB = Nphi * nEtaEBP + phiI + Nphi* etaI;
      if ( (m_etaEnd-1)%m_etaWidth !=0  ) regionNumEB += Nphi;
  
    }
  }

  if (regionNumEB >= EBregionsNum()) std::cout << "regionNumEB = " << regionNumEB << std::endl;

  return regionNumEB;
}

//----------------------------------------



//! EB average phi of region id
double EBregionBuilder::EBRegionPhi(int id) const
{  

  double avephi = 0;
  int nphi = 0;
  
  for(int eta = -85; eta < 86; ++eta){
    if (eta==0) continue;
    for (int phi = 0; phi < 360; ++phi){
      if (id == EBRegionId(eta,phi+1)){
	avephi+= (phi+1);
	nphi++;
      }
    }
  }
   
  return (avephi/nphi);
  
}
//----------------------------------------



//! EB average eta of region id
double EBregionBuilder::EBRegionEta(int id) const
{  

  double aveeta = 0;
  int neta = 0;
  
  for(int eta = -85; eta < 86; ++eta){
    if (eta==0) continue;
    for (int phi = 0; phi < 360; ++phi){
      if (id == EBRegionId(eta,phi+1)){
	aveeta+= eta;
	neta++;
      }
    }
  }
   
  return (aveeta/neta);
  
}


//----------------------------------------



//! EB Region Definition
void EBregionBuilder::EBRegionDefinition()
{
  for(int it = 0; it < EBregionsNum(); ++it)
    m_xtalNumInRegion.push_back(0);
  
  for(int eta = -85; eta < 86; ++eta)
    for (int phi = 0; phi < 360; ++phi)
     {
       if(eta == 0) continue;
       
       int reg = EBRegionId(eta,phi+1);
       int hashedIndex = -1;
       if( eta < 0 ) hashedIndex = 30600 + (eta)   * 360 + phi;
       if( eta > 0 ) hashedIndex = 30600 + (eta-1) * 360 + phi;
       
       m_xtalRegionId[hashedIndex] = reg;
       if (reg == -1) continue;
       m_xtalPositionInRegion[hashedIndex] = m_xtalNumInRegion.at(reg);
       ++m_xtalNumInRegion.at(reg);
     }
  
}

//----------------------------------------



//! Build a EB region reading from a cfg file
EBregionBuilder* BuildEBRegion(const std::string& inFileName, const std::string& histoName)
{
  std::ifstream inFile;
  inFile.open(inFileName.c_str());
  
  int etaStart = -1;
  int etaEnd   = -1;
  int etaWidth = -1;
  int phiStart = -1;
  int phiEnd   = -1;
  int phiWidth = -1;
  
  while(1)
  {
    if( inFile.eof() ) break;
    
    std::string name;
    int val;
    
    inFile >> name >> val;
    
    if( name == "etaStart" ) etaStart = val;
    if( name == "etaEnd" )   etaEnd   = val;
    if( name == "etaWidth" ) etaWidth = val;
    if( name == "phiStart" ) phiStart = val;
    if( name == "phiEnd" )   phiEnd   = val;
    if( name == "phiWidth" ) phiWidth = val;
  }
 
  inFile.close();
 
  EBregionBuilder* dummy = new EBregionBuilder(etaStart,etaEnd,etaWidth,phiStart,phiEnd,phiWidth,histoName);
  
  std::cout << ">>> EB region definition:" << std::endl;
  std::cout << ">>>>>> eta: [" << std::setw(3) << etaStart << "," << std::setw(3) << etaEnd << ")   -   etaWidth: " << etaWidth << std::endl;
  std::cout << ">>>>>> phi: [" << std::setw(3) << phiStart << "," << std::setw(3) << phiEnd << ")   -   phiWidth: " << phiWidth << std::endl;
  std::cout << ">>>>>> num of regions: " << dummy->EBregionsNum() << std::endl;
  return dummy;
}

//----------------------------------------

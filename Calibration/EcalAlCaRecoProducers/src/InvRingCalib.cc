#include <memory>
#include <math.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Calibration/EcalAlCaRecoProducers/interface/InvRingCalib.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "Calibration/Tools/interface/calibXMLwriter.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/CaloMiscalibTools.h"
#include "TH2.h"
#include "TFile.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
//----------------------------------------------------------------
//LP ctor

InvRingCalib::InvRingCalib (const edm::ParameterSet& iConfig) :
  m_barrelAlCa (iConfig.getParameter<edm::InputTag>("barrelAlca")),
  m_endcapAlCa (iConfig.getParameter<edm::InputTag>("endcapAlca")) ,
  m_ElectronLabel (iConfig.getParameter<edm::InputTag>("ElectronLabel")),
  m_recoWindowSide (iConfig.getParameter<int>("recoWindowSide")),
  m_minEnergyPerCrystal (iConfig.getParameter<double>("minEnergyPerCrystal")) ,
  m_maxEnergyPerCrystal (iConfig.getParameter<double>("maxEnergyPerCrystal")) ,
  m_etaStart (etaShifter(iConfig.getParameter<int>("etaStart"))),
  m_etaEnd (etaShifter(iConfig.getParameter<int>("etaEnd"))),
  m_etaWidth (iConfig.getParameter<int>("etaWidth")),
 // m_ADCtoGeV (iConfig.getParameter<double>("ADCtoGeV")),//FIXME
  m_minCoeff (iConfig.getParameter<double>("minCoeff")),
  m_maxCoeff (iConfig.getParameter<double>("maxCoeff")),
  m_usingBlockSolver(iConfig.getParameter<int>("usingBlockSolver")),
  m_loops (iConfig.getParameter<int>("loops")),
  m_startRing (iConfig.getParameter<int>("startRing")),
  m_endRing (iConfig.getParameter<int>("endRing"))
{
 assert (m_etaStart >=0 && m_etaStart <= 170);
 assert (m_etaEnd >= m_etaStart && m_etaEnd <= 170);
 assert (m_startRing>-1 && m_startRing< 40);
 assert (m_endRing>=m_startRing && m_endRing<40);
 assert(!((m_endRing - m_startRing+1)%m_etaWidth));
 assert ((m_etaEnd-m_etaStart+1)%m_etaWidth == 0);
 edm::LogInfo ("IML") << "[InvRingCalib][ctor] entering " ;
 
 //LP CalibBlock vector instantiation
 for (int i =0; i< EBRegionNum(); ++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth)); 
 for (int i=0; i<EERegionNum();++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth));
 for (int i=0; i<EERegionNum();++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth));

}

//---------------------------------------------------------------------------end ctor



//------------------
//LP destructor

InvRingCalib::~InvRingCalib ()
{
}
//----------------


//---------------------------------------------------
//BeginOfJob
void 
InvRingCalib::beginOfJob (const edm::EventSetup& iSetup) 
{
  edm::ESHandle<CaloGeometry> geoHandle;
  iSetup.get<IdealGeometryRecord>().get(geoHandle);
  const CaloGeometry& geometry = *geoHandle;
  m_barrelCells = geometry.getValidDetIds(DetId::Ecal, EcalBarrel);
  edm::LogInfo ("IML") << "[InvRingCalib][beginOfJob]" ;
  m_endcapCells = geometry.getValidDetIds(DetId::Ecal, EcalEndcap);
 //Defines the EB regions
  EBRegionDef();
//Defines what is a ring in the EE
  EERingDef(iSetup);
//Defines the regions in the EB
  EERegionDef();
/*  TH2F * EBRegion = new TH2F ("EBRegion","EBRegion",171,0,171,361,0,361) ;
   for (int eta = 0; eta<170; ++eta)
      for (int phi = 0; phi <360; ++phi){
	int zona= -10;
	if (m_EBXtlReg[eta][phi]%2) zona = 10 ;
	EBRegion->Fill(eta, phi, zona);
      }
   TH2F * EERegion = new TH2F ("EERegion", "EERegion",101,0,100,101,0,100);
   for (int x = 0; x<100; ++x)
      for (int y = 0; y<100;++y){
	    int zona = -10;
	    if (m_EEXtlReg[x][y]%2) zona = 10;
	    if (m_EEXtlReg[x][y]==-1) zona = 0; 
	    EERegion->Fill(x,y,zona);
      }
   TFile out ("EBZone.root", "recreate");
     EBRegion->Write();
     EERegion->Write();
     out.Close();
     delete EBRegion;
     delete EERegion;
*/
 for (int i=0; i<170;++i) 
   for (int j=0; j<360; ++j) m_EBRingRecalibFactor[i][j] = 1.;

 for (int i=0; i<100;++i) 
  for (int j=0; j<100;++j){
      m_EEPRingRecalibFactor[i][j] =-1.;//FIXME
      m_EEMRingRecalibFactor[i][j] =-1.;//FIXME
 }
//fills a map with al the Recalib Coefficients. 
 EcalIntercalibConstants::EcalIntercalibConstantMap imap;
 edm::ESHandle<EcalIntercalibConstants> pIcal;
   try {
      iSetup.get<EcalIntercalibConstantsRcd>().get(pIcal);
      edm::LogInfo ("IML")  << "Taken EcalIntercalibConstants" << std::endl;
      imap = pIcal.product()->getMap();
      edm::LogInfo ("IML") << "imap.size() = " << imap.size() << std::endl;
     } 
   catch ( std::exception& ex ) {     
      std::cerr << "Error! can't get EcalIntercalibConstants " << std::endl;
      }
//Fills the vectors of the coefficients from the map in the barrel.
 
  for (std::vector<DetId>::const_iterator barrelIt =m_barrelCells.begin(); 
       barrelIt!=m_barrelCells.end(); 
       ++barrelIt) {
            EBDetId eb(*barrelIt);
	    int i=etaShifter(eb.ieta());
	    int j=eb.iphi();
	    m_EBRingRecalibFactor[i][j] = (imap.find(eb.rawId()))->second;
         }


//Fills the vectors of the coefficients in the endcap.
  for (std::vector<DetId>::const_iterator endcapIt=m_endcapCells.begin(); 
        endcapIt!=m_endcapCells.end(); 
	++endcapIt) {
    EEDetId ee(*endcapIt);
    int ix=ee.ix()-1;
    int iy=ee.iy()-1;
      if (ee.zside()>0)
            m_EEPRingRecalibFactor[ix][iy] = (imap.find(ee.rawId()))->second;
      if (ee.zside()<0)
            m_EEMRingRecalibFactor[ix][iy] =  (imap.find(ee.rawId()))->second;
    }
//To see the initial coefficients
/*  TH1F * EBcoeff = new TH1F ("EBRegion","EBRegion",200,0.8,1.2) ;
   for (int eta = 0; eta<170; ++eta)
      for (int phi = 0; phi <360; ++phi){
	EBcoeff->Fill(m_EBRingRecalibFactor[eta][phi]);
      }
     TH1F * EEcoeff = new TH1F ("EERegion", "EERegion",200,0.8,1.2);
   for (int x = 0; x<100; ++x)
      for (int y = 0; y<100;++y){
	    EEcoeff->Fill(m_EEPRingRecalibFactor[x][y]);
      }
   TFile zout ("InitCoeff.root", "recreate");
     EBcoeff->Write();
     EEcoeff->Write();
     zout.Close();
     std::cout<<"Uè, guarda che ho scritto il file! si chiama EBZone.root!"<<std::endl;
*/
 }

//--------------------------------------------------------
//startingNewLoop
void InvRingCalib::startingNewLoop (unsigned int ciclo) 
{
    edm::LogInfo ("IML") << "[InvMatrixCalibLooper][Start] entering loop " << ciclo;
    for (std::vector<EcalCalibBlock>::iterator calibBlock = m_ecalCalibBlocks.begin () ;
         calibBlock != m_ecalCalibBlocks.end () ;
         ++calibBlock)
      {
	//LP empties the energies vector, to fill DuringLoop.
        calibBlock->reset () ;
      }
   return ;
}

//--------------------------------------------------------
//duringLoop
edm::EDLooper::Status
InvRingCalib::duringLoop (const edm::Event& iEvent,
                             const edm::EventSetup&) 
{
//DS Barrel collections
  const EBRecHitCollection* barrelHitsCollection = 0;
  //DS get the EB collection
  try {
      //DS get Barrel RecHits
      edm::Handle<EBRecHitCollection> barrelRecHitsHandle ;
      iEvent.getByLabel (m_barrelAlCa, barrelRecHitsHandle) ;
      barrelHitsCollection = barrelRecHitsHandle.product () ;
     }
    
    catch (std::exception& ce) {
      edm::LogError("reading") << "[InvMatrixLooper] caught std::exception "
                << " in retrieving " << m_barrelAlCa << "\t" 
                << ce.what () << std::endl ;
      return  kContinue ;
    }

 //DS Momentum
   HandleBasicCSAC barrelClShpHandle ;
   iEvent.getByLabel("hybridSuperClusters","hybridShapeAssoc", barrelClShpHandle) ;

   edm::Handle<reco::PixelMatchGsfElectronCollection> pElectrons;
   try {
   iEvent.getByLabel(m_ElectronLabel,pElectrons);
    }
    catch (std::exception& ce) {
	edm::LogError("reading")<<m_ElectronLabel<<"NotFound"<<"\t"<<ce.what()<<std::endl;
	return kContinue;
    }

//DS EE collections
  const EERecHitCollection* endcapHitsCollection = 0;
  edm::Handle<reco::BasicClusterShapeAssociationCollection> endcapClShpHandle ; 
  try{
  iEvent.getByLabel("islandBasicClusters", "islandEndcapShapeAssoc", endcapClShpHandle) ;
    }
  catch(std::exception& ce){
    edm::LogError("reading")<<m_ElectronLabel<<"notFound";
  }
  double pSubtract = 0.;
  double pTk = 0.;

  //DS get the EE collection
  try {
      //DS get Endcap RecHits
      edm::Handle<EERecHitCollection> endcapRecHitsHandle ;
      iEvent.getByLabel (m_endcapAlCa, endcapRecHitsHandle) ;
      endcapHitsCollection = endcapRecHitsHandle.product () ;
    }

    catch (std::exception& ce) {
      edm::LogError("reading") << "[InvMatrixLooper] caught std::exception " 
                << " in retrieving " << m_endcapAlCa << "\t"
                << ce.what () << std::endl ;

      return kContinue;
    }

///////////////////////////////////////


//DS loop over electrons 

int index =0;

//  const reco::PixelMatchGsfElectronCollection * electronCollection = pElectrons.product();
  for (eleIterator eleIt = pElectrons->begin();
       eleIt != pElectrons->end();
       ++eleIt )
   {
 edm::LogInfo ("IML") <<"Riempio l'eb";
    DetId Max = getMaxId(eleIt, barrelClShpHandle, endcapClShpHandle);
    pTk = eleIt->trackMomentumAtVtx().R();
    if  ( Max.subdetId() == EcalBarrel  )
    {
      EBDetId EBmax = Max;
     //PG region of interest cut
     if (EBRegionCheck (etaShifter(EBmax.ieta()), EBmax.iphi())) continue;
     std::map<int , double> EBxtlMap;

     fillEBMap (EBmax, barrelHitsCollection, EBxtlMap,
                EBRegId (etaShifter(EBmax.ieta())), pSubtract ) ;

     int blockIndex =  EBRegId (etaShifter(EBmax.ieta())) ;
     m_ecalCalibBlocks.at(blockIndex).Fill (EBxtlMap.begin(), EBxtlMap.end(),pTk,pSubtract) ;

     ++index;
   
    } //DS chiude l'if sulla regione
    else 
    {
edm::LogInfo ("IML")<<"Riempio EE";
      EEDetId EEmax = Max;
     //PG region of interest cut
     if (EERegionCheck (EEmax.ix(), EEmax.iy())) continue ; 
     
     std::map<int , double> EExtlMap;

      if (EEmax.zside()==1) 
      {
        fillEEMap (EEmax, endcapHitsCollection, EExtlMap,
                    EERegId (EEmax.ix(),EEmax.iy()), pSubtract ) ;
        int blockIndex = EBRegionNum()+EERegId (EEmax.ix(),EEmax.iy());
        pSubtract += eleIt->superCluster()->preshowerEnergy() ;
        m_ecalCalibBlocks.at(blockIndex).Fill (EExtlMap.begin(), EExtlMap.end(),pTk,pSubtract) ;
      }
      else  
      {
       fillEEMap (EEmax, endcapHitsCollection, EExtlMap,
                  EERegId (EEmax.ix(),EEmax.iy()), pSubtract ) ;
       int blockIndex = EBRegionNum()+EERegionNum()+EERegId (EEmax.ix(),EEmax.iy());
       pSubtract += eleIt->superCluster()->preshowerEnergy() ;
       m_ecalCalibBlocks.at(blockIndex).Fill (EExtlMap.begin(), EExtlMap.end(),pTk,pSubtract) ;
      }
    } //DS chiude else sulla regione
   } //DS chiude loop sugli elettroni  



 edm::LogInfo ("IML") << "[InvMatrixCalibLooper][duringLoop] End of during loop" ; 
 return  kContinue;
}

//-----------------------------------
//end of duringLoop

//-------------------------------------
//LP EndOfLoop
edm::EDLooper::Status InvRingCalib::endOfLoop (const edm::EventSetup& dumb,unsigned int iCounter)
{
for (std::vector<EcalCalibBlock>::iterator calibBlock=m_ecalCalibBlocks.begin();
	calibBlock!=m_ecalCalibBlocks.end();
	++calibBlock)
		calibBlock->solve(m_usingBlockSolver,m_minCoeff,m_maxCoeff);
edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfLoop] End of endOfLoop" ;
//Reads back the InterCalibration between the coefficients
for (int eta = m_etaStart; eta <=m_etaEnd; ++eta)
      m_InterRings[eta]= m_ecalCalibBlocks[m_EBXtlReg[eta][0]].at(m_EBRingNum[eta]); 
return kStop;
for (int ring = m_startRing; ring<=m_endRing;++ring){
      m_InterRings[ring+170]=m_ecalCalibBlocks[EBRegionNum()+ring-m_startRing].at(ring);
      m_InterRings[ring+210]=m_ecalCalibBlocks[EBRegionNum()+EERegionNum()+ring-m_startRing].at(ring);
}

if (iCounter < m_loops ) return kContinue ;
else return kStop; 
}


//---------------------------------------
//LP endOfJob
void 
InvRingCalib::endOfJob ()
{

  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfJob] saving calib coeffs" ;

calibXMLwriter barrelWriter(EcalBarrel);
calibXMLwriter endcapWriter(EcalEndcap);
  for (std::vector<DetId>::const_iterator barrelIt =m_barrelCells.begin(); 
       barrelIt!=m_barrelCells.end(); 
       ++barrelIt) {
            EBDetId eb(*barrelIt);
	    int leta = etaShifter(eb.ieta());
	    int lphi = eb.iphi();
            double coeff = m_InterRings[leta]*m_EBRingRecalibFactor[leta][lphi] ; 
	    barrelWriter.writeLine(eb,coeff);
          }
  for (std::vector<DetId>::const_iterator endcapIt = m_endcapCells.begin();
     endcapIt!=m_endcapCells.end();
     ++endcapIt) {
            EEDetId ee(*endcapIt);
	    int ix = ee.ix()-1;
	    int iy = ee.iy()-1;
            double coeff;
            if (ee.zside()>0) coeff = m_EEPRingRecalibFactor[ix][iy]*m_InterRings[m_EEXtlRing[ix][iy]+170];
            else coeff  = m_EEMRingRecalibFactor[ix][iy] *m_InterRings[m_EEXtlRing[ix][iy]+170+40]; 
	  endcapWriter.writeLine(ee,coeff);
	}
    
/*  TH1F * EBcoeff = new TH1F ("EBRegion","EBRegion",200,0.8,1.2) ;
   for (int eta = 0; eta<170; ++eta)
      for (int phi = 0; phi <360; ++phi){
	EBcoeff->Fill(m_EBRingRecalibFactor[eta][phi]);
      }
     TH1F * EEcoeff = new TH1F ("EERegion", "EERegion",200,0.8,1.2);
   for (int x = 0; x<100; ++x)
      for (int y = 0; y<100;++y){
	    EEcoeff->Fill(m_EEPRingRecalibFactor[x][y]);
      }
   TFile zout ("EndCoeff.root", "recreate");
     EBcoeff->Write();
     EEcoeff->Write();
     zout.Close();
*/
}



//------------------------------------//
//      definition of functions       //
//------------------------------------//

//-------------------------
int 
InvRingCalib::EBRegionCheck (const int eta, const int phi) const
  {
    if (eta < m_etaStart) return 1 ;
    if (eta >= m_etaEnd)   return 2 ;

    return 0 ;
  }

int InvRingCalib::EERegionCheck (const int x, const int y) const
 {
    if (m_EEXtlRing[x][y] < m_startRing) return 1;
    if (m_EEXtlRing[x][y] > m_endRing) return 2;
     return 0;
 }
//-------------------------

void InvRingCalib::fillEBMap (EBDetId EBmax,
				 const EcalRecHitCollection * barrelHitsCollection,
				 std::map<int, double> & EBRegionMap,
				 int EBNumberOfRegion, double & pSubtract)
  {
   unsigned int row;
   unsigned int column;
   int curr_eta;
   int curr_phi;
   for (unsigned int icry = 0 ; icry< m_etaWidth * 360 ; ++icry)
    {
     row = icry / m_etaWidth;
     column= icry % m_etaWidth;
     curr_eta=EBmax.ieta() + column - (m_etaWidth/2);
     if (abs(curr_eta)>85) continue;
     curr_phi=EBmax.iphi() + row - (180);

     // in ECAL reference system
     if (curr_eta * EBmax.ieta() <= 0) {if (EBmax.ieta() > 0) curr_eta--; else curr_eta++; }  // JUMP over 0

     if (curr_phi < 1) curr_phi += 360;
     if (curr_phi > 360) curr_phi -= 360;
     if(EBDetId::validDetId(curr_eta,curr_phi))
      {
       EBDetId det = EBDetId(curr_eta,curr_phi,EBDetId::ETAPHIMODE);
       EcalRecHitCollection::const_iterator curr_recHit = barrelHitsCollection->find(det) ;
    int local_eta = etaShifter(det.ieta());
       double dummy = 0;
      dummy = curr_recHit->energy () ;
    if (abs(curr_eta - EBmax.ieta())<m_recoWindowSide/2 || 
           abs(curr_phi - EBmax.iphi())<m_recoWindowSide/2)
       { 
         if ( dummy > m_minEnergyPerCrystal || dummy < m_maxEnergyPerCrystal)
	 {
           dummy *= m_EBRingRecalibFactor[local_eta][curr_phi]; 
           dummy *= m_InterRings[local_eta];
           if (isMyRegion(local_eta,curr_phi, EBNumberOfRegion, 0)) 
            {
	     EBRegionMap[m_EBRingNum[local_eta]]+= dummy;
	    }
          else pSubtract +=dummy; 
	}
      }
    }
   }
 }
//----------------------------
//! Filss the EE map to be given to the calibBlock

void InvRingCalib::fillEEMap (EEDetId EEmax,
                const EcalRecHitCollection * endcapHitsCollection,
                std::map<int,double> & EExtlMap,
                int EENumberOfRegion, double & pSubtract )
   {


    int curr_x = EEmax.ix();
    int curr_y = EEmax.iy();

     if(EEDetId::validDetId(curr_x,curr_y,EEmax.zside()))
     {
      EEDetId det = EEDetId(curr_x,curr_y,EEmax.zside(),EEDetId::XYMODE);

      EcalRecHitCollection::const_iterator curr_recHit = endcapHitsCollection->find(det) ;
      double dummy = curr_recHit->energy () ;
      int icsX =  det.ix();
      int ipsX =  det.iy();

      if (abs(icsX-curr_x)>(m_recoWindowSide/2) || 
        abs(ipsX-curr_y)>(m_recoWindowSide/2)) return;
      if ( dummy < m_minEnergyPerCrystal ) return; // selection
      if ( dummy > m_maxEnergyPerCrystal ) return; // selection

      int ecalZone = det.zside();
      int RingId;
      if (ecalZone==-1) RingId = 170+40+m_EEXtlRing[icsX][ipsX];
      if (ecalZone==1) RingId = 170+m_EEXtlRing[icsX][ipsX];
     dummy *= m_EEPRingRecalibFactor[icsX][ipsX]; 
     dummy *= m_InterRings[RingId];

      if (isMyRegion(icsX, ipsX, EENumberOfRegion, ecalZone)){
          EExtlMap[m_EERingNum[icsX][ipsX]] += dummy;
      }
      else pSubtract +=dummy; 

     } //DS chiude l'if sul valid det Id
 
 }



//----------------------------------------
//!Reg Id generator EB ----- for the barrel
int InvRingCalib::EBRegId(const int etaXtl) const
{
  //LP checks if the width is compatible.
  //LP index of the ring
  int etaI = (etaXtl-m_etaStart)/m_etaWidth ;


  return  etaI;
}




//!EE ring definition

void InvRingCalib::EERingDef(const edm::EventSetup& iSetup)  
{
  for (int x = 0; x<100; ++x)
     for (int y = 0; y<100; ++y)
           m_EEXtlRing[x][y] = -1;
  edm::ESHandle<CaloGeometry> geoHandle;
  iSetup.get<IdealGeometryRecord>().get(geoHandle);
  const CaloGeometry& geometry = *geoHandle;
  std::cout<<"vettore di detid di dimensione"<<m_endcapCells.size()<<std::endl;
  const CaloSubdetectorGeometry *endcapGeometry = geometry.getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
  for (std::vector<DetId>::const_iterator endcapIt = m_endcapCells.begin();
     endcapIt!=m_endcapCells.end();
     ++endcapIt) {
       const CaloCellGeometry *cellGeometry = endcapGeometry->getGeometry(*endcapIt);
       EEDetId ee(*endcapIt);
       int ics=ee.ix()-1;
       int ips=ee.iy()-1;
       m_cellPos[ics][ips] = cellGeometry->getPosition();
       m_cellPhi[ics][ips] = cellGeometry->getPosition().phi();
   }
  float eta_ring[39];
  for (int ring=0; ring<39; ring++) {
    eta_ring[ring]=m_cellPos[ring][50].eta();
  }
 double etaBonduary[40];
 etaBonduary[0]=1.49;
 etaBonduary[39]=4.0;
 for (int ring=1; ring<39; ++ring)
 {
   etaBonduary[ring]=(eta_ring[ring]+eta_ring[ring-1])/2.;
 }
  for (int ring=0; ring<39; ring++) {
    for (int ix=0; ix<100; ix++) {
      for (int iy=0; iy<100; iy++) {
	if (fabs(m_cellPos[ix][iy].eta())>etaBonduary[ring] &&
	    fabs(m_cellPos[ix][iy].eta())<etaBonduary[ring+1]) {
	  m_EEXtlRing[ix][iy]=ring;
	}
      }
    }
   }
 std::cout<<"sto uscendo"<<std::endl;
 return;
}

//
int InvRingCalib::EERegId(const int ics, const int ips) const 
{
   int reg;
   if (m_EEXtlRing[ics][ips] == -1) return -1;
   else
     if(m_EEXtlRing[ics][ips] >=m_endRing) return -1;
    else reg = (m_EEXtlRing[ics][ips]-m_startRing) / m_etaWidth;
   return reg;
}
//----------------------------------------
//Endcap Region Definition 
void InvRingCalib::EERegionDef ()
{ 
  std::vector<int> XtlRing(m_endRing-m_startRing+1);
  for (int i=m_startRing ; i<=m_endRing;++i) XtlRing.at(i)= i - (i/m_etaWidth)*m_etaWidth; 
  for (int i = 0; i<EERegionNum(); ++i) XtlRing.at(i) = 0;
     for (int ics=0; ics<100;++ics)
       for (int ips=0; ips<100;++ips){
	    int reg = EERegId(ics,ips);
	    m_EEXtlReg[ics][ips]=reg; 
	    if (reg ==-1) continue;
	    m_EERingNum[ics][ips]= XtlRing.at(m_EEXtlRing[ics][ips]);
       }
}

//----------------------------------------
//DS Number of Regions in EE 
inline int InvRingCalib::EERegionNum () const 
  {
    return ((m_endRing - m_startRing+1)/m_etaWidth);
  }


//DS number of Ring in EB
inline int InvRingCalib::EBRegionNum () const 
  {
    return ((m_etaEnd - m_etaStart + 1)/m_etaWidth); 
  }

//----------------------------------------
//DS EB Region Definition
void InvRingCalib::EBRegionDef()
  {
     for (int eta=m_etaStart; eta<=m_etaEnd;++eta){
       for (int phi=0; phi<360;++phi){
	    int reg = EBRegId(eta);
	    m_EBXtlReg[eta+m_etaStart][phi]=reg; 
       }
        m_EBRingNum[eta+m_etaStart]= eta - (eta/m_etaWidth)*m_etaWidth;
     }
  }
//DS Shifts eta in other coordinates (from 0 to 170)
inline int InvRingCalib::etaShifter (const int etaOld) 
   {
     if (etaOld < 0) return etaOld + 85;
     else return etaOld + 84;
   }

//--------------------------------------------
//!LP checks if the ring you asks for is the correct one
int InvRingCalib::isMyRegion (const int coordOne, const int coordTwo, const int regionId, const int ecalZone) 
{
  if (ecalZone==0)  return (m_EBXtlReg[coordOne][coordTwo] == regionId) ;
  return (m_EEXtlReg[coordOne][coordTwo] == regionId) ; 
}

//! gives back the Id of the Most Energetic Xtal
DetId InvRingCalib::getMaxId(eleIterator eleIt,
                                HandleBasicCSAC  & barrelClShpHandle,
                                HandleBasicCSAC  & endcapClShpHandle)
{
   const reco::SuperCluster* mySC =
          & (*((eleIt->superCluster ()))) ;

   reco::AlgoId algoId = mySC->seed()->algo() ;
   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr ;

    if (algoId == 1) seedShpItr = barrelClShpHandle->find(mySC->seed());
    
    else seedShpItr = endcapClShpHandle->find(mySC->seed());

    return seedShpItr->val->eMaxId();

} 


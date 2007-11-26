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
//ctor

InvRingCalib::InvRingCalib (const edm::ParameterSet& iConfig) :
  m_barrelAlCa (iConfig.getParameter<edm::InputTag>("barrelAlca")),
  m_endcapAlCa (iConfig.getParameter<edm::InputTag>("endcapAlca")) ,
  m_ElectronLabel (iConfig.getParameter<edm::InputTag>("ElectronLabel")),
  m_recoWindowSide (iConfig.getParameter<int>("recoWindowSide")),
  m_minEnergyPerCrystal (iConfig.getParameter<double>("minEnergyPerCrystal")) ,
  m_maxEnergyPerCrystal (iConfig.getParameter<double>("maxEnergyPerCrystal")) ,
  m_etaStart (iConfig.getParameter<int>("etaStart")),
  m_etaEnd (iConfig.getParameter<int>("etaEnd")),
  m_etaWidth (iConfig.getParameter<int>("etaWidth")),
  m_minCoeff (iConfig.getParameter<double>("minCoeff")),
  m_maxCoeff (iConfig.getParameter<double>("maxCoeff")),
  m_usingBlockSolver(iConfig.getParameter<int>("usingBlockSolver")),
  m_loops (iConfig.getParameter<int>("loops")),
  m_startRing (iConfig.getParameter<int>("startRing")),
  m_endRing (iConfig.getParameter<int>("endRing"))
{
 //controls if the parameters inputed are correct
 assert (m_etaStart >=-85 && m_etaStart <= 85);
 assert (m_etaEnd >= m_etaStart && m_etaEnd <= 85);
 assert (m_startRing>-1 && m_startRing<= 40);
 assert (m_endRing>=m_startRing && m_endRing<=40);
 assert(!((m_endRing - m_startRing)%m_etaWidth));
 assert ((m_etaEnd-m_etaStart)%m_etaWidth == 0);
 edm::LogInfo ("IML") << "[InvRingCalib][ctor] entering " ;
 
 //LP CalibBlock vector instantiation
 for (int i =0; i< EBRegionNum(); ++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth)); 
 for (int i=0; i<EERegionNum();++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth));
 for (int i=0; i<EERegionNum();++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth));
 for (int i=0; i<(EBRegionNum()+2*EERegionNum());++i)
	for(int j=0;j<m_etaWidth;++j)
  	m_InterRings.push_back(1);
}

//---------------------------------------------------------------------------end ctor
//!destructor

InvRingCalib::~InvRingCalib ()
{
}
//----------------

//---------------------------------------------------
//!BeginOfJob
void 
InvRingCalib::beginOfJob (const edm::EventSetup& iSetup) 
{
 edm::LogInfo ("IML") << "[InvRingCalib][beginOfJob]" ;
 //gets the geometry from the event setup
 edm::ESHandle<CaloGeometry> geoHandle;
 iSetup.get<IdealGeometryRecord>().get(geoHandle);
 const CaloGeometry& geometry = *geoHandle;
 //fills a vector with all the cells
 m_barrelCells = geometry.getValidDetIds(DetId::Ecal, EcalBarrel);
 m_endcapCells = geometry.getValidDetIds(DetId::Ecal, EcalEndcap);
 //Defines the EB regions
 EBRegionDef();
 //Defines what is a ring in the EE
 EERingDef(iSetup);
 //Defines the regions in the EE
 EERegionDef();

//Graphs to check ring, regions and so on, not needed in the final version
  TH2F EBRegion ("EBRegion","EBRegion",171,-85,86,360,1,361);
  TH2F EBRing ("EBRing","EBRing",171,-85,86,360,1,361);
  for (std::vector<DetId>::const_iterator it= m_barrelCells.begin();
        it!= m_barrelCells.end(); ++it ){
	EBDetId eb (*it);
        EBRing.Fill(eb.ieta(),eb.iphi(),m_RinginRegion[it->rawId()]);
	EBRegion.Fill(eb.ieta(),eb.iphi(),m_xtalRegionId[it->rawId()]);
	m_fakeCoeffs[it->rawId()]=1;
      }
 TH2F EEPRegion ("EEPRegion", "EEPRegion",100,1,101,100,1,101);
 TH2F EEPRing ("EEPRing", "EEPRing",100,1,101,100,1,101);
 TH2F EEPRingReg ("EEPRingReg", "EEPRingReg",100,1,101,100,1,101);
 TH2F EEMRegion ("EEMRegion", "EEMRegion",100,1,101,100,1,101);
 TH2F EEMRing ("EEMRing", "EEMRing",100,1,101,100,1,101);
 TH2F EEMRingReg ("EEMRingReg", "EEMRingReg",100,1,101,100,1,101);
 for (std::vector<DetId>::const_iterator it = m_endcapCells.begin();
      it!= m_endcapCells.end();++it)
    {
     EEDetId ee (*it);
     if (ee.zside()>0){
       EEPRegion.Fill(ee.ix(),ee.iy(),m_xtalRegionId[ee.rawId()]);
       EEPRing.Fill(ee.ix(),ee.iy(),m_xtalRing[ee.rawId()]);
       EEPRingReg.Fill(ee.ix(),ee.iy(),m_RinginRegion[ee.rawId()]);
       m_fakeCoeffs[it->rawId()]=1;
     }
     if (ee.zside()<0){
       EEMRegion.Fill(ee.ix(),ee.iy(),m_xtalRegionId[ee.rawId()]);
       EEMRing.Fill(ee.ix(),ee.iy(),m_xtalRing[ee.rawId()]);
       m_fakeCoeffs[it->rawId()]=1; EEMRingReg.Fill(ee.ix(),ee.iy(),m_RinginRegion[ee.rawId()]);
   }    
  } 
 TFile out ("EBZone.root", "recreate");
 EBRegion.Write();
 EBRing.Write();
 EEPRegion.Write();
 EEPRing.Write();
 EEPRingReg.Write();
 EEMRegion.Write();
 EEMRing.Write();
 EEMRingReg.Write();
 out.Close();
}

//--------------------------------------------------------
//!startingNewLoop
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
//!duringLoop
edm::EDLooper::Status
InvRingCalib::duringLoop (const edm::Event& iEvent,
                          const edm::EventSetup&) 
{
 //gets the barrel recHits
 double pSubtract = 0.;
 double pTk = 0.;
 const EBRecHitCollection* barrelHitsCollection = 0;
 try {
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

 //gets the endcap recHits
 const EERecHitCollection* endcapHitsCollection = 0;
 try {
      edm::Handle<EERecHitCollection> endcapRecHitsHandle ;
      iEvent.getByLabel (m_endcapAlCa, endcapRecHitsHandle) ;
      endcapHitsCollection = endcapRecHitsHandle.product () ;
    }
 catch (std::exception& ce) {
    edm::LogError("reading") << "[InvRingLooper] caught std::exception " 
              << " in retrieving " << m_endcapAlCa << "\t"
              << ce.what () << std::endl ;
    return kContinue;
  }

 //gets the electrons
 edm::Handle<reco::PixelMatchGsfElectronCollection> pElectrons;
 try {
      iEvent.getByLabel(m_ElectronLabel,pElectrons);
     }
 catch (std::exception& ce) {
	edm::LogError("reading")<<m_ElectronLabel<<"NotFound"
			<<"\t"<<ce.what()<<std::endl;
       return kContinue;
    }
 //loops over the electrons in the event
 const reco::PixelMatchGsfElectronCollection * electronCollection = pElectrons.product();
 for (eleIterator eleIt = electronCollection->begin();
      eleIt != electronCollection->end();
      ++eleIt )
   {
    pSubtract =0;
    pTk=0;
    //find the most energetic xtal
    DetId Max = findMaxHit (eleIt->superCluster ()->getHitsByDetId (), 
                            barrelHitsCollection, 
			    endcapHitsCollection) ;
    
    //Skips if findMaxHit failed 
    if (Max.det()==0) continue; 
    //Skips if the Max is in a region we don't want to calibrate
    if (m_xtalRegionId[Max.rawId()]==-1) continue;
//    if (m_maxSelectedNumPerXtal > 0 &&  //FIXME
//        m_xtalNumOfHits[Max.rawId ()] > m_maxSelectedNumPerXtal ) continue ;
//    ++m_xtalNumOfHits[Max.rawId()];
    //declares a map to be filled with the hits of the xtals around the MOX
    std::map<int , double> xtlMap;
    //Gets the momentum of the track
    pTk = eleIt->trackMomentumAtVtx().R();
    if  ( Max.subdetId() == EcalBarrel  )
    {
     EBDetId EBmax = Max;
     //fills the map of the hits 
     fillEBMap (EBmax, barrelHitsCollection, xtlMap,
                m_xtalRegionId[Max.rawId()], pSubtract ) ;
    }
    else 
    {
     EEDetId EEmax = Max;
     fillEEMap (EEmax, endcapHitsCollection, xtlMap,
                    m_xtalRegionId[Max.rawId()],pSubtract ) ;
     //subtracts the preshower Energy deposit
     pSubtract += eleIt->superCluster()->preshowerEnergy() ;
    }
    //fills the calibBlock 
    m_ecalCalibBlocks.at(m_xtalRegionId[Max.rawId()]).Fill (xtlMap.begin(), xtlMap.end(),pTk,pSubtract) ;
  }
 return  kContinue;
}
//-----------------------------------
//end of duringLoop


//-------------------------------------
//EndOfLoop
edm::EDLooper::Status InvRingCalib::endOfLoop (const edm::EventSetup& dumb,unsigned int iCounter)
{
edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfLoop] Start to invert the matrixes" ;
//call the autoexplaining "solve" method for every calibBlock
for (std::vector<EcalCalibBlock>::iterator calibBlock=m_ecalCalibBlocks.begin();
	calibBlock!=m_ecalCalibBlocks.end();
	++calibBlock)
		calibBlock->solve(m_usingBlockSolver,m_minCoeff,m_maxCoeff);
//Reads back the InterCalibration between the rings
edm::LogInfo("IML") << "[InvRingLooper][endOfLoop] Starting to write the coeffs";
int Numbers= EBRegionNum()+2*EERegionNum();
Numbers *= m_etaWidth;
TH1F * coeffMap= new TH1F ("coeffMap","coeffMap",Numbers ,0,Numbers-1);
TH1F coeffDistr("coeffdistr","coeffdistr",100, 0.7,1.4);
for (int reg = 0; reg< EBRegionNum()+ 2 * EERegionNum();++reg)
  for (int i = 0; i < m_etaWidth; ++i){ 
      m_InterRings[reg+i]= m_ecalCalibBlocks.at(reg).at(i);
      coeffDistr.Fill(m_InterRings[reg+i]);
      coeffMap->Fill(reg+i, m_InterRings[reg+i]);
    }
TFile out("coeffs.root","recreate");    
coeffDistr.Write();
coeffMap->Write();
out.Close();
delete coeffMap;
if (iCounter < m_loops-1 ) return kContinue ;
else return kStop; 
}


//---------------------------------------
//LP endOfJob
void 
InvRingCalib::endOfJob ()
{

 edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfJob] saving calib coeffs" ;
/* calibXMLwriter barrelWriter(EcalBarrel);
 calibXMLwriter endcapWriter(EcalEndcap);
 for (std::vector<DetId>::const_iterator barrelIt =m_barrelCells.begin(); 
       barrelIt!=m_barrelCells.end(); 
       ++barrelIt) {
            EBDetId eb(*barrelIt);
	    int leta = etaShifter(eb.ieta());
	    int lphi = eb.iphi();
//            double coeff = m_InterRings[leta]*m_EBRingRecalibFactor[leta][lphi] ; 
	    barrelWriter.writeLine(eb,coeff);
          }
 for (std::vector<DetId>::const_iterator endcapIt = m_endcapCells.begin();
     endcapIt!=m_endcapCells.end();
     ++endcapIt) {
            EEDetId ee(*endcapIt);
	    int ix = ee.ix()-1;
	    int iy = ee.iy()-1;
            double coeff;
//            coeff = m_EEPRingRecalibFactor[ix][iy]*m_InterRings[m_xtalRegionId[ee.rawId()]];
	  endcapWriter.writeLine(ee,coeff);
	}
*/
}


//------------------------------------//
//      definition of functions       //
//------------------------------------//
//-------------------------

void InvRingCalib::fillEBMap (EBDetId EBmax,
	 const EcalRecHitCollection * barrelHitsCollection,
	 std::map<int, double> & EBRegionMap,
	 int EBNumberOfRegion, double & pSubtract)
{
 int curr_eta;
 int curr_phi;
 //reads the hits in a recoWindowSide^2 wide region around the MOX
 for (int ii = 0 ; ii< m_recoWindowSide ; ++ii)
  for (int ij =0 ; ij< m_recoWindowSide ; ++ij) 
  {
   curr_eta=EBmax.ieta() + ii - (m_recoWindowSide/2);
   curr_phi=EBmax.iphi() + ij - (m_recoWindowSide/2);
   //The followind 2 lines couples with the zero gap in the barrel
   if (abs(curr_eta)>85) continue;
   if (curr_eta * EBmax.ieta() <= 0) {if (EBmax.ieta() > 0) curr_eta--; else curr_eta++; }  // JUMP over 0
   //The following 2 couples with the ciclicity of the phiIndex
   if (curr_phi < 1) curr_phi += 360;
   if (curr_phi > 360) curr_phi -= 360;
   //checks if the detId is valid
   if(EBDetId::validDetId(curr_eta,curr_phi))
    {
     EBDetId det = EBDetId(curr_eta,curr_phi,EBDetId::ETAPHIMODE);
     //finds the hit corresponding to the cell
     EcalRecHitCollection::const_iterator curr_recHit = barrelHitsCollection->find(det) ;
     double dummy = 0;
     dummy = curr_recHit->energy () ;
     //checks if the reading of the xtal is in a sensible range
     if ( dummy < m_minEnergyPerCrystal || dummy > m_maxEnergyPerCrystal) continue;
    //corrects the energy with the calibration coeff of the ring
     dummy *= m_fakeCoeffs[det.rawId()]; 
     //sums the energy of the xtal to the appropiate ring
     if (m_xtalRegionId[det.rawId()]==EBNumberOfRegion)
       EBRegionMap[m_RinginRegion[det.rawId()]]+= dummy;
     //adds the reading to pSubtract when part of the matrix is outside the region
     else pSubtract +=dummy; 
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
 int curr_x;
 int curr_y;
 for (int ii = 0 ; ii< m_recoWindowSide ; ++ii)
  for (int ij =0 ; ij< m_recoWindowSide ; ++ij) 
 {
  curr_x = EEmax.ix() - m_recoWindowSide/2 +ii;
  curr_y = EEmax.iy() - m_recoWindowSide /2 +ij;
  if(EEDetId::validDetId(curr_x,curr_y,EEmax.zside()))
  {
   EEDetId det = EEDetId(curr_x,curr_y,EEmax.zside(),EEDetId::XYMODE);
   EcalRecHitCollection::const_iterator curr_recHit = endcapHitsCollection->find(det) ;
   double dummy = curr_recHit->energy () ;
   if ( dummy < m_minEnergyPerCrystal ) continue; // selection
   if ( dummy > m_maxEnergyPerCrystal ) continue; // selection
//   std::cerr<<"x "<<curr_x<<" y "<<curr_y<<"  ";
//   std::cerr<<"RingId "<<RingId<<"  ";
//   std::cerr<<"energy = "<<dummy<<"   ";
//   std::cerr<<"coeff = "<<m_InterRings[RingId]<<std::endl;
//    dummy *= m_InterRings[RingId];FIXME
    dummy *= m_fakeCoeffs[det.rawId()];
   if (m_xtalRegionId[det.rawId()]==EENumberOfRegion)
      EExtlMap[m_RinginRegion[det.rawId()]] += dummy;
   else pSubtract +=dummy; 
  }
 }
}

//!EE ring definition
void InvRingCalib::EERingDef(const edm::EventSetup& iSetup)  
{
 edm::ESHandle<CaloGeometry> geoHandle;
 iSetup.get<IdealGeometryRecord>().get(geoHandle);
 const CaloGeometry& geometry = *geoHandle;
 const CaloSubdetectorGeometry *endcapGeometry = geometry.getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
 for (std::vector<DetId>::const_iterator endcapIt = m_endcapCells.begin();
    endcapIt!=m_endcapCells.end();
    ++endcapIt) {
     const CaloCellGeometry *cellGeometry = endcapGeometry->getGeometry(*endcapIt);
     m_cellPos[endcapIt->rawId()] = cellGeometry->getPosition();
     m_cellPhi[endcapIt->rawId()] = cellGeometry->getPosition().phi();
  }
 float eta_ring[39];
 for (int ring=0; ring<39; ring++) 
	if (EEDetId::validDetId(ring,50,1)){  
	  EEDetId det = EEDetId (ring,50,1,EEDetId::XYMODE);
          eta_ring[ring]=m_cellPos[det.rawId()].eta();
	  }
 double etaBonduary[40];
 etaBonduary[0]=1.49;
 etaBonduary[39]=4.0;
 for (int ring=1; ring<39; ++ring)
       etaBonduary[ring]=(eta_ring[ring]+eta_ring[ring-1])/2.;
 for (int ring=0; ring<39; ring++) 
   for (std::vector<DetId>::const_iterator endcapIt=m_endcapCells.begin();
        endcapIt!=m_endcapCells.end();++endcapIt){
     if (fabs(m_cellPos[endcapIt->rawId()].eta())>etaBonduary[ring] &&
         fabs(m_cellPos[endcapIt->rawId()].eta())<etaBonduary[ring+1]) 
              m_xtalRing[endcapIt->rawId()]=ring;
      }
 return;
}

//
int InvRingCalib::EERegId( int id) 
{
   int reg;
   if (m_xtalRing[id] == -1) return -1;
   else
   if(m_xtalRing[id] >=m_endRing) return -1;
   if (m_xtalRing[id] <m_startRing) return -1;
   else reg = (m_xtalRing[id]-m_startRing) / m_etaWidth;
   return reg;
}
//----------------------------------------
//Endcap Region Definition 
void InvRingCalib::EERegionDef ()
{ 
int reg;
for (std::vector<DetId>::const_iterator endcapIt=m_endcapCells.begin();
     endcapIt!=m_endcapCells.end();++endcapIt){
      EEDetId ee(*endcapIt);
      reg = EERegId(endcapIt->rawId());
      if(reg==-1) 
         m_xtalRegionId[endcapIt->rawId()]=reg;
      else {
      if (ee.zside()>0)reg += EBRegionNum();
      else reg += EBRegionNum()+EERegionNum();
      m_xtalRegionId[endcapIt->rawId()]=reg;
      m_RinginRegion[endcapIt->rawId()]=(m_xtalRing[endcapIt->rawId()]-m_startRing)%m_etaWidth;
   }
  }
}

//----------------------------------------
//DS Number of Regions in EE 
inline int InvRingCalib::EERegionNum () const 
{
  return ((m_endRing - m_startRing)/m_etaWidth);
}


//DS number of Ring in EB
inline int InvRingCalib::EBRegionNum () const 
{
  return ((m_etaEnd - m_etaStart )/m_etaWidth); 
}
//EB Region Id
int InvRingCalib::EBRegId(const int ieta) const
{
 int reg;
 if (ieta<m_etaStart || ieta>=m_etaEnd) return -1;
 else reg = (ieta - m_etaStart)/m_etaWidth;
 return reg;
}

//----------------------------------------
//EB Region Definition
void InvRingCalib::EBRegionDef()
{
  for (std::vector<DetId>::const_iterator it=m_barrelCells.begin();
  	it!=m_barrelCells.end();++it)
  {
    EBDetId eb (it->rawId());
    m_xtalRing[eb.rawId()] = eb.ieta() ;
    m_xtalRegionId[eb.rawId()] = EBRegId (eb.ieta()); 
    if (m_xtalRegionId[eb.rawId()]==-1) continue;
    m_RinginRegion[eb.rawId()] = (eb.ieta() - m_etaStart)% m_etaWidth; 
  }
}


//--------------------------------------------
//!LP checks if the ring you asks for is the correct one

//---------------------Find the most energetic Xtal
DetId  InvRingCalib::findMaxHit (const std::vector<DetId> & v1,
				    const EBRecHitCollection* EBhits,
				    const EERecHitCollection* EEhits) 
{
 double currEnergy = 0.;
 DetId maxHit;
 for (std::vector<DetId>::const_iterator idsIt = v1.begin () ; 
      idsIt != v1.end () ; ++idsIt)
   {
    if (idsIt->subdetId () == EcalBarrel) 
       {              
         EBRecHitCollection::const_iterator itrechit;
         itrechit = EBhits->find (*idsIt) ;
         if (itrechit == EBhits->end () )
           {
            edm::LogWarning("IML") <<"max hit not found";
            continue;
           }
         if (itrechit->energy () > currEnergy)
           {
             currEnergy = itrechit->energy () ;
             maxHit= *idsIt ;
           }
       } //barrel part ends
    else 
       {     
         EERecHitCollection::const_iterator itrechit;
         itrechit = EEhits->find (*idsIt) ;
         if (itrechit == EEhits->end () )
           {
             edm::LogWarning("IML") <<"max hit not found";
             continue;
           }
              
         if (itrechit->energy () > currEnergy)
           {
            currEnergy=itrechit->energy ();
            maxHit= *idsIt;
           }
       } //ends the barrel part
    } //end of the loop over the detId
 return maxHit;
}
//TODOS
//Sistemare i vari RegDef
//Ring struct
//Ridefinire i coefficienti...le coordinate

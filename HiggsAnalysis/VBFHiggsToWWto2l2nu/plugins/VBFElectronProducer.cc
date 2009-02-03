#define DEBUG    0 // 0=false
#define C_DEBUG  0 // currently debuging

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElectronProducer.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/IdealGeometryRecord.h"


using namespace std ;
using namespace edm ;
using namespace reco ;
using namespace vbfhww2l ;


VBFElectronProducer::VBFElectronProducer (const edm::ParameterSet& iConfig) :
  m_tkIsolationAlgo (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      iConfig.getParameter<double> ("otherVetoRadius") ,
      iConfig.getParameter<double> ("ptMin") ,
      iConfig.getParameter<double> ("lipMax") ,
      iConfig.getUntrackedParameter<bool> ("useTkQuality",true)
    ) ,
  m_eleInputTag (iConfig.getParameter<InputTag> ("eleInputTag")),
  m_EBClusterShapesInputTag (iConfig.getParameter<InputTag> ("EBClusterShapesInputTag")),
  m_EEClusterShapesInputTag (iConfig.getParameter<InputTag> ("EEClusterShapesInputTag")),
  m_eleIDPTDRLooseInputTag (iConfig.getParameter<InputTag> ("eleIDPTDRLooseInputTag")), 
  m_eleIDPTDRMediumInputTag (iConfig.getParameter<InputTag> ("eleIDPTDRMediumInputTag")), 
  m_eleIDPTDRTightInputTag (iConfig.getParameter<InputTag> ("eleIDPTDRTightInputTag")), 
  m_TracksInputTag (iConfig.getParameter<InputTag> ("TracksInputTag")),
  m_IterativeTracksInputTag (iConfig.getParameter<InputTag> ("IterativeTracksInputTag")),
  m_electronsNum (-1)
{

  produces<VBFParticleCollection> () ;

}


// --------------------------------------------------------------------


VBFElectronProducer::~VBFElectronProducer ()
{}


// --------------------------------------------------------------------


void
VBFElectronProducer::produce (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  //PG get the electrons collection 
  Handle<electronCollection> electronsHandle ;
  iEvent.getByLabel (m_eleInputTag, electronsHandle) ;

  Handle<BasicClusterShapeAssociationCollection> barrelClShpHandle ;
  iEvent.getByLabel (m_EBClusterShapesInputTag, barrelClShpHandle) ;

  Handle<BasicClusterShapeAssociationCollection> endcapClShpHandle ;
  iEvent.getByLabel (m_EEClusterShapesInputTag, endcapClShpHandle) ;

  std::vector<edm::Handle<reco::ElectronIDAssociationCollection> > eleIdHandles ;
  for (int i=0 ; i<3 ; ++i)
    {
      edm::Handle<reco::ElectronIDAssociationCollection> dummy ;
      eleIdHandles.push_back (dummy) ;
    }
  iEvent.getByLabel (m_eleIDPTDRLooseInputTag, eleIdHandles[0]);
  iEvent.getByLabel (m_eleIDPTDRMediumInputTag, eleIdHandles[1]);
  iEvent.getByLabel (m_eleIDPTDRTightInputTag, eleIdHandles[2]);

  edm::Handle<edm::View<reco::Track> > TracksHandle ;
  iEvent.getByLabel (m_TracksInputTag, TracksHandle) ;

  edm::Handle<edm::View<reco::Track> > IterativeTracksHandle ;
  iEvent.getByLabel (m_IterativeTracksInputTag, IterativeTracksHandle) ;

  //PG the output collection
  auto_ptr<VBFParticleCollection> electrons (new VBFParticleCollection) ;

 //PG loop over GSF electrons
  for (electronCollection::const_iterator eleIt = electronsHandle->begin () ; 
       eleIt != electronsHandle->end () ; 
       ++eleIt)
    {

#if DEBUG
      cout << "electron pT = " << eleIt->pt () 
           << "\t electron eta = " << eleIt->eta () 
  	       << endl ;
#endif

      vbfhww2l::VBFParticle * lep = new vbfhww2l::VBFParticle () ;
      lep->v4 = eleIt->p4 () ; //PG FIXME metto questo, o quello al vtx?
      lep->vertex = eleIt->vertex () ;
      lep->pdgId = eleIt->pdgId () ;
 
      lep->AddIntProp ("Charge", eleIt->charge ()) ;
 
      lep->AddDoubleProp ("Pvtx", eleIt->trackMomentumAtVtx ().r ()) ; //PG FIXME metto anche eta, phi?
      lep->AddDoubleProp ("caloCorrEnergy", eleIt->caloEnergy ()) ;
      lep->AddDoubleProp ("fullCorrEnergy", eleIt->energy ()) ;
 
      lep->AddDoubleProp ("rawEnergy", eleIt->superCluster ()->rawEnergy ()) ;
      lep->AddDoubleProp ("SCEnergy", eleIt->superCluster ()->energy ()) ;
      lep->AddDoubleProp ("BCenergy", eleIt->superCluster ()->seed ()->energy ()) ;
      lep->AddIntProp ("classification", eleIt->classification ()) ;
 
      GsfTrackRef trRef = eleIt->gsfTrack () ;
      float myTip = sqrt ((trRef->vertex ().x ()) * (trRef->vertex ().x ()) + 
                          (trRef->vertex ().y ()) * (trRef->vertex ().y ())) ;
 
      lep->AddDoubleProp ("TIP", myTip) ;

      lep->AddDoubleProp ("HoE", eleIt->hadronicOverEm ()) ;
      lep->AddDoubleProp ("ESCoPin", eleIt->eSuperClusterOverP ()) ; //PG FIXME ctrl if it is in
 
      lep->AddDoubleProp ("EBCoPout", eleIt->eSeedClusterOverPout ()) ;
      lep->AddDoubleProp ("dEtaSCPin", eleIt->deltaEtaSuperClusterTrackAtVtx ()) ;
      lep->AddDoubleProp ("dPhiSCPin", eleIt->deltaPhiSuperClusterTrackAtVtx ()) ;
      lep->AddDoubleProp ("dEtaBCPout", eleIt->deltaEtaSeedClusterTrackAtCalo ()) ;
      lep->AddDoubleProp ("dPhiBCPout", eleIt->deltaPhiSeedClusterTrackAtCalo ()) ;

//PG questo e' usato per escludere elettroni che non abbiano cluster in EE ed EB, e' possibile che capiti?
 
      reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr ;
      seedShpItr = barrelClShpHandle->find (eleIt->superCluster ()->seed ()) ;
 
      bool hasBarrel = true ;
      bool hasEndcap = true ;
 
      if (seedShpItr == barrelClShpHandle->end ()) 
        {
          hasBarrel = false ;
          seedShpItr = endcapClShpHandle->find (eleIt->superCluster ()->seed ()) ;
          if (seedShpItr == endcapClShpHandle->end ()) hasEndcap = false ;
        }

//PG chiude il questo

     reco::PixelMatchGsfElectronRef electronReference = 
       electronsHandle->refAt (eleIt - electronsHandle->begin ()).castTo<reco::PixelMatchGsfElectronRef> () ;

 //    const reco::ClusterShapeRef& shapeRef = getClusterShape (electronReference,iEvent) ;

//      lep->AddDoubleProp ("eMax",shapeRef->eMax ()) ;         
// //     lep->AddDoubleProp ("e2nd",shapeRef->e2nd ()) ;         
// //     lep->AddDoubleProp ("e2x2",shapeRef->e2x2 ()) ;         
// //     lep->AddDoubleProp ("e3x2",shapeRef->e3x2 ()) ;         
//      lep->AddDoubleProp ("e3x3",shapeRef->e3x3 ()) ;         
// //     lep->AddDoubleProp ("e4x4",shapeRef->e4x4 ()) ;         
//      lep->AddDoubleProp ("e5x5",shapeRef->e5x5 ()) ;         
// //     lep->AddDoubleProp ("e2x5Right",shapeRef->e2x5Right ()) ;    
// //     lep->AddDoubleProp ("e2x5Left",shapeRef->e2x5Left ()) ;     
// //     lep->AddDoubleProp ("e2x5Top",shapeRef->e2x5Top ()) ;      
// //     lep->AddDoubleProp ("e2x5Bottom",shapeRef->e2x5Bottom ()) ;
// //     lep->AddDoubleProp ("e3x2Ratio",shapeRef->e3x2Ratio ()) ;    
//      lep->AddDoubleProp ("covEtaEta",shapeRef->covEtaEta ()) ;    
//      lep->AddDoubleProp ("covEtaPhi",shapeRef->covEtaPhi ()) ;    
//      lep->AddDoubleProp ("covPhiPhi",shapeRef->covPhiPhi ()) ;    

     //PG track iso info

     lep->AddDoubleProp ("sumOfTkPtInCone", m_tkIsolationAlgo.calcSumOfPt (electronsHandle, TracksHandle, electronReference)) ;
     lep->AddDoubleProp ("numOfTkInCone", m_tkIsolationAlgo.countNumOfTracks (electronsHandle, TracksHandle, electronReference)) ;
     lep->AddDoubleProp ("sumOfIterTkPtInCone", m_tkIsolationAlgo.calcSumOfPt (electronsHandle, IterativeTracksHandle, electronReference)) ;
     lep->AddDoubleProp ("numOfIterTkInCone", m_tkIsolationAlgo.countNumOfTracks (electronsHandle, IterativeTracksHandle, electronReference)) ;

     //PG ECAL iso info
     
     //PG HCAL iso info

     //PG electron ID

     int eleId[3] = {0,0,0} ;
     reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr ;
     //PG loop over the eleID
     for (int i=0 ; i<3 ; ++i)
       {
         electronIDAssocItr = eleIdHandles[i]->find (electronReference) ;
         if (electronIDAssocItr != eleIdHandles[i]->end ())
           {
             const reco::ElectronIDRef & id = electronIDAssocItr->val ;
             eleId[i] = id->cutBasedDecision () ;
           }
       } //PG loop over the eleID
     lep->AddIntProp ("loosePTDREleId", eleId[0]) ;
     lep->AddIntProp ("mediumPTDREleId", eleId[1]) ;
     lep->AddIntProp ("tightPTDREleId", eleId[2]) ;


     electrons->push_back (*lep) ;
//  EcalClusterLazyTools lazyTools = getClusterShape(e,es);
//  std::vector<float> vCov = lazyTools.covariances(*(electron->superCluster()->seed())) ;
//  double sigmaee = sqrt(vCov[0]);

     
//PG Emanuele's code from here -------------------------->
/*
void CmsEleIDTreeFiller::writeEleInfo(const PixelMatchGsfElectronRef electronRef,
				      const edm::Event& iEvent, const edm::EventSetup& iSetup,
				      const reco::BasicClusterShapeAssociationCollection& barrelClShpMap, 
				      const reco::BasicClusterShapeAssociationCollection& endcapClShpMap,
				      const reco::ElectronIDAssociationCollection& eleIdAssoc) { 

  SuperClusterRef sclusRef = electronRef->superCluster();
  // ele corr - notcorr energy
  float myEleTrackerP   = electronRef->trackMomentumAtVtx().r();
  float myEleFullCorrE  = electronRef->energy();
  float myEleCaloCorrE  = electronRef->caloEnergy();
  float myEleNxtalCorrE = 0.;
  float myEleRawE       = 0.;
  float mySeedE         = 0.;
  float mySeedCorrE     = 0.;            
  if(&sclusRef) {
    myEleNxtalCorrE = sclusRef->energy();
    myEleRawE       = sclusRef->rawEnergy();
    mySeedE         = sclusRef->seed()->energy();
    if ((int(electronRef->classification()/10) == 3) || (int(electronRef->classification()/10) == 13) ){
      double Ebrem = 0.;  
      basicCluster_iterator bc;
      for(bc = sclusRef->clustersBegin(); bc!=sclusRef->clustersEnd(); bc++) {
	Ebrem = Ebrem +(*bc)->energy();
      }
      Ebrem = Ebrem - mySeedE;
      mySeedCorrE = myEleNxtalCorrE - Ebrem;
    }
    else {mySeedCorrE = myEleFullCorrE;}
  }

  // transverse impact parameter
  GsfTrackRef trRef = electronRef->gsfTrack();
  float myTip = sqrt((trRef->vertex().x())*(trRef->vertex().x()) + (trRef->vertex().y())*(trRef->vertex().y()));

  // eleID
  privateData_->eleFullCorrE     ->push_back(myEleFullCorrE);  
  privateData_->eleCaloCorrE     ->push_back(myEleCaloCorrE);  
  privateData_->eleNxtalCorrE    ->push_back(myEleNxtalCorrE); 
  privateData_->eleRawE          ->push_back(myEleRawE); 
  privateData_->eleTrackerP      ->push_back(myEleTrackerP);
  privateData_->eleClass         ->push_back(electronRef->classification());
  privateData_->eleHoE           ->push_back(electronRef->hadronicOverEm());
  privateData_->eleCorrEoP       ->push_back(electronRef->eSuperClusterOverP());
  privateData_->eleNotCorrEoP    ->push_back(myEleRawE/myEleTrackerP);
  privateData_->eleCorrEoPout    ->push_back(electronRef->eSeedClusterOverPout());
  privateData_->eleNotCorrEoPout ->push_back(electronRef->eSeedClusterOverPout()*(mySeedCorrE/mySeedE));
  privateData_->eleDeltaEtaAtVtx ->push_back(electronRef->deltaEtaSuperClusterTrackAtVtx());
  privateData_->eleDeltaPhiAtVtx ->push_back(electronRef->deltaPhiSuperClusterTrackAtVtx());
  privateData_->eleDeltaEtaAtCalo->push_back(electronRef->deltaEtaSeedClusterTrackAtCalo());
  privateData_->eleDeltaPhiAtCalo->push_back(electronRef->deltaPhiSeedClusterTrackAtCalo());
  privateData_->eleTip           ->push_back(myTip);

  // electron ID (cut-based and likelihood)
  // for the cut-based, store the decision
  // for the likelihood, store the output of the algorithm
  
  bool hasBarrel=true;
  bool hasEndcap=true;
  reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;
  seedShpItr = barrelClShpMap.find(sclusRef->seed());

  if(seedShpItr==barrelClShpMap.end()) {
    hasBarrel=false;
    seedShpItr=endcapClShpMap.find(sclusRef->seed());
    if(seedShpItr==endcapClShpMap.end()) hasEndcap=false;
  }

  if(hasBarrel || hasEndcap) {

    reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr;

    // electron ID
    electronIDAssocItr = eleIdAssoc.find( electronRef );
    
    if ( electronIDAssocItr==eleIdAssoc.end() ) edm::LogWarning("CmsEleIDTreeFiller") << "cannot find the electron id associated with electron";
    
    const reco::ElectronIDRef& id = electronIDAssocItr->val;

    privateData_->eleIdCutBasedDecision->push_back( id->cutBasedDecision() );
    privateData_->eleLik->push_back( id->likelihood() );

  }
  else {
    edm::LogWarning("CmsEleIDTreeFiller") << "cannot find cluster shapes in ECAL barrel or endcap "
					  << " setting electron ID results to the default";
    privateData_->eleIdCutBasedDecision->push_back( false );
    privateData_->eleLik->push_back( -1.);
  }


  // --- isolations ---
  
  // in the egamma style: use private one for now, to study
  // this retrieves the index in the original collection associated to the reference to electron
  //  int index = electronRef.key();

  //   double sumPt = (*tkIsolationHandle_)[index].second;
  //   double sumEt = (*towerIsolationHandle_)[index].second;
  //   privateData_->eleTrackerIso_sumPt->push_back( sumPt );
  //   privateData_->eleCaloIso_sumPt->push_back( sumEt );


  // for tracker isolation studies
  const TrackCollection tracksC = *(m_tracks.product());
  hwwEleTrackerIsolation trackIsolation(&(*electronRef), tracksC);
  trackIsolation.setExtRadius(0.3);    
  trackIsolation.setIntRadius(0.02);    
  float minDR03     = trackIsolation.minDeltaR(0.15);  
  float minDRveto03 = trackIsolation.minDeltaR_withVeto(0.15);  
  float sumPt03  = trackIsolation.getPtTracks(true,false);
  float sumPtSquared03  = trackIsolation.getPtTracks(true,true);
  float sumN03 = trackIsolation.getNTracks(1.0);

  trackIsolation.setExtRadius(0.4);    
  float sumPt04     = trackIsolation.getPtTracks();  

  trackIsolation.setExtRadius(0.5);
  float sumPt05     = trackIsolation.getPtTracks();  

  trackIsolation.setIntRadius(0.015);
  trackIsolation.setExtRadius(0.2);
  float sumPtPreselection =  trackIsolation.getPtTracks();

  privateData_->minDR03->push_back(minDR03);
  privateData_->minDRveto03->push_back(minDRveto03);
  privateData_->sumPt03->push_back(sumPt03);
  privateData_->sumPtSquared03->push_back(sumPtSquared03);
  privateData_->sumN03->push_back(sumN03);
  privateData_->sumPt04->push_back(sumPt04);
  privateData_->sumPt05->push_back(sumPt05);
  privateData_->sumPtPreselection->push_back(sumPtPreselection);

  // calo isolation - rechit based: cannot be used on AOD
//   const HBHERecHitCollection hcalRecHits = *(hcalrhits.product());
//   hwwEleCaloIsolation caloIsolation(electron, hcalRecHits, caloGeo);
//   //float minDR_calo = caloIsolation.minDeltaR(0.15);  
//   float minDR_calo = -1000.;
//   caloIsolation.setExtRadius(0.2);    
//   float sumEt_calo = caloIsolation.getEtHcal();  

  // calo isolation - calotower based: can be used on both RECO and AOD
  const CaloTowerCollection *calotowersC = m_calotowers.product();
  hwwEleCalotowerIsolation calotowerIsolation(&(*electronRef), calotowersC);
  calotowerIsolation.setIntRadius(0.1);
  calotowerIsolation.setExtRadius(0.40);
  
  bool relative = true;
  float sumHadEt04 = calotowerIsolation.getEtHcal(relative);
  float sumEmEt04 = calotowerIsolation.getEtEcal(relative);

  calotowerIsolation.setExtRadius(0.50);
  float sumHadEt05 = calotowerIsolation.getEtHcal(relative);
  float sumEmEt05 = calotowerIsolation.getEtEcal(relative);

  privateData_->sumHadEt04->push_back(sumHadEt04);
  privateData_->sumEmEt04->push_back(sumEmEt04);
  privateData_->sumHadEt05->push_back(sumHadEt05);
  privateData_->sumEmEt05->push_back(sumEmEt05);

//   privateData_->eleCaloIso_minDR->push_back(minDR_calo);
//   privateData_->eleCaloIso_sumPt->push_back(sumEt_calo);


*/
//PG --------------------------------------------> to here

   } //PG loop over GSF electrons
  
#if C_DEBUG
  //PG FIXME questo numero per ora non ha senso
  cout << "number of electrons = " << electrons->size () << endl ;
#endif

  
  iEvent.put (electrons) ;
}


// --------------------------------------------------------------------


void 
VBFElectronProducer::beginJob (const edm::EventSetup&)
{}


// --------------------------------------------------------------------


void 
VBFElectronProducer::endJob ()
{}


// --------------------------------------------------------------------


// //PG from CMSSW/EgammaAnalysis/ElectronIDAlgos/src/ElectronIDAlgo.cc
// const reco::ClusterShapeRef& 
// VBFElectronProducer::getClusterShape (const reco::PixelMatchGsfElectronRef electron, const edm::Event& e) 
// {
//   // Get association maps linking BasicClusters to ClusterShape.
//   edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeHandle ;
//   if (electron->classification () < 100) {
//     e.getByLabel (m_EBClusterShapesInputTag, clusterShapeHandle) ;
//   } else {
//     e.getByLabel (m_EEClusterShapesInputTag, clusterShapeHandle) ;
//   }
// 
//   // Find entry in map corresponding to seed BasicCluster of SuperCluster
//   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr ;
//   seedShpItr = clusterShapeHandle->find (electron->superCluster ()->seed ()) ;
//   return seedShpItr->val ;
// }

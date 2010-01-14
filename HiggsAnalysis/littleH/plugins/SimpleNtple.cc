// -*- C++ -*-
//
// Package:    SimpleNtple
// Class:      SimpleNtple
// 
/**\class SimpleNtple SimpleNtple.cc Analysis/SimpleNtple/src/SimpleNtple.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
 */
//
// Original Author:  Andrea Massironi
//         Created:  Fri Jan  5 17:34:31 CEST 2010
// $Id: SimpleNtple.cc,v 1.4 2010/01/14 09:13:29 dimatteo Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "HiggsAnalysis/littleH/plugins/SimpleNtple.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//--- objects ----
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"

//---- utilities ----
#include "HiggsAnalysis/littleH/interface/MCDumper.h"


SimpleNtple::SimpleNtple(const edm::ParameterSet& iConfig) :
 m_eleIDCut_LooseInputTag (iConfig.getParameter<edm::InputTag> ("eleIDCut_LooseInputTag")) ,
 m_eleIDCut_RLooseInputTag (iConfig.getParameter<edm::InputTag> ("eleIDCut_RLooseInputTag")) ,
 m_eleIDCut_TightInputTag (iConfig.getParameter<edm::InputTag> ("eleIDCut_TightInputTag")) ,
 m_eleIDCut_RTightInputTag (iConfig.getParameter<edm::InputTag> ("eleIDCut_RTightInputTag"))
{
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 
 TracksTag_ = iConfig.getParameter<edm::InputTag>("TracksTag");
 EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
 MuTag_ = iConfig.getParameter<edm::InputTag>("MuTag");
 PrimaryVertexTag_ = iConfig.getParameter<edm::InputTag>("PrimaryVertexTag");


 //  MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");

 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 
//  thetriggerEventLabel       = iConfig.getParameter<string>("triggerEventLabel");
//  theHLTriggerResults        = iConfig.getParameter<string>("triggerResultsLabel");
//  the8e29ProcName            = iConfig.getParameter<string>("HLTprocessName8e29");
//  the1e31ProcName            = iConfig.getParameter<string>("HLTprocessName1e31");

 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


SimpleNtple::~SimpleNtple()
{
 delete NtupleFactory_;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
  SimpleNtple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 
  ///---- save HLT report ----
//   using namespace trigger;
//   Handle<TriggerResults> HLTR;
//   iEvent.getByLabel(InputTag(theHLTriggerResults,"",the8e29ProcName), HLTR);
//   if (HLTR.isValid()) {
//    HLTGlobal_wasrun=HLTR->wasrun();
//    HLTGlobal_Decision=HLTR->accept();
//    HLTGlobal_error=HLTR->error();
//     
//     // HLTBits_size=HLTR->size();
//    for (int i=0; i<HLTBits_size && i<(int)Max_trig_size; i++) {
//     HLTBits_wasrun[i]=HLTR->wasrun(hltBits[i]);
//     HLTBits_accept[i]=HLTR->accept(hltBits[i]);
//     HLTBits_error[i]=HLTR->error(hltBits[i]);
//    }
//    
//    HLT1Mu3_L3_size=0;
//    HLT1Mu5_L3_size=0;
//    HLT1Mu9_L3_size=0;
//    HLT1Mu11_L3_size=0;
//    HLT2IsoMu3_L3_size=0;
// 
//    HLT2Mu0_L3_size=0;
//    HLT2Mu3_L3_size=0;
//    HLTJpsi2Mu_L3_size=0;
//    HLTUpsilon2Mu_L3_size=0;
// 
//    Handle<TriggerEvent> trgEvent;
//    bool hltF = true;
//    try {
//     iEvent.getByLabel(InputTag(thetriggerEventLabel,"",the8e29ProcName), trgEvent);
//    }
//    catch (const cms::Exception& e) {
//     hltF = false;
//     cout<<"Error!! No TriggerEvent with label " << thetriggerEventLabel << endl;
//    }
//    if ( hltF ) {
//     const TriggerObjectCollection& TOC(trgEvent->getObjects());
//  
//     for ( int lvl = 1; lvl<2; lvl++ ) { 
//      for ( int ipath = 0; ipath < HLTBits_size; ipath++) {
//       int muonsize = 0;
//       const InputTag trigName = hltModules[lvl][ipath];
//       size_type index = trgEvent->filterIndex(trigName);
//       if ( index < trgEvent->sizeFilters() ) {
//        const Keys& KEYS( trgEvent->filterKeys(index) );
//        muonsize = KEYS.size();
//        int minNMuons = 1; if ( ipath>=3 ) minNMuons = 2;
//        if (  muonsize < minNMuons && HLTR->accept(hltBits[ipath]) ) {  
//         cout<<"Error!! Not enough HLT muons for "<<trigName.label()<<", but decision = "<<HLTR->accept(hltBits[ipath])<<endl;
//        }
//        for ( int hltm = 0; hltm < muonsize; hltm++ ) {
//         size_type hltf = KEYS[hltm];
//         const TriggerObject& TO(TOC[hltf]);
//         TLorentzVector a = lorentzTriObj(TO);
// 
//         if ( lvl==1 ) {
//          if ( ipath==0 ) {
//           new((*HLT1Mu3_L3_4mom)[HLT1Mu3_L3_size])TLorentzVector(a);
//           HLT1Mu3_L3_id[HLT1Mu3_L3_size]=TO.id();
//           HLT1Mu3_L3_size++;          
//          }
//          if ( ipath==1 ) {
//           new((*HLT1Mu5_L3_4mom)[HLT1Mu5_L3_size])TLorentzVector(a);
//           HLT1Mu5_L3_id[HLT1Mu5_L3_size]=TO.id();
//           HLT1Mu5_L3_size++;      
//          }
//          if ( ipath==2 ) {
//           new((*HLT1Mu9_L3_4mom)[HLT1Mu9_L3_size])TLorentzVector(a);
//           HLT1Mu9_L3_id[HLT1Mu9_L3_size]=TO.id();
//           HLT1Mu9_L3_size++;      
//          }
//          if ( ipath==3 ) {
//           new((*HLT2Mu0_L3_4mom)[HLT2Mu0_L3_size])TLorentzVector(a);
//           HLT2Mu0_L3_id[HLT2Mu0_L3_size]=TO.id();
//           HLT2Mu0_L3_size++;
//          }
//          if ( ipath==4 ) {
//           new((*HLT2Mu3_L3_4mom)[HLT2Mu3_L3_size])TLorentzVector(a);
//           HLT2Mu3_L3_id[HLT2Mu3_L3_size]=TO.id();
//           HLT2Mu3_L3_size++;
//          }
//         }
//        } 
//       } 
//      }
//     }
//    }
//   }
 
 
 ///---- fill muons ----
 edm::Handle<edm::View<reco::Muon> > MuHandle ;
 iEvent.getByLabel (MuTag_,MuHandle);
 int nMu;
 if(MuHandle->size() < 30 ){ nMu = MuHandle->size(); }
 else {nMu = 30;}
 for(int i=0; i< nMu; i++){
  math::XYZTLorentzVector* myvect_XYZT = new math::XYZTLorentzVector((*MuHandle)[i].p4().Px(),(*MuHandle)[i].p4().Py(),(*MuHandle)[i].p4().Pz(),(*MuHandle)[i].p4().E());
  NtupleFactory_->FillStdXYZTLorentzVector("muons",myvect_XYZT);
  
  NtupleFactory_->FillFloat("muons_charge",((*MuHandle)[i].charge()));
  NtupleFactory_->FillFloat("muons_tkIsoR03",((*MuHandle)[i].isolationR03()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR03",((*MuHandle)[i].isolationR03()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR03",((*MuHandle)[i].isolationR03()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR03",((*MuHandle)[i].isolationR03()).hadEt);
    
  NtupleFactory_->FillFloat("muons_tkIsoR05",((*MuHandle)[i].isolationR05()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR05",((*MuHandle)[i].isolationR05()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR05",((*MuHandle)[i].isolationR05()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR05",((*MuHandle)[i].isolationR05()).hadEt);

  //Get Global Muon Track
  TrackRef glbTrack = (*MuHandle)[i].globalTrack();
  
  NtupleFactory_->FillFloat("muons_track_d0", glbTrack->d0 ());
  NtupleFactory_->FillFloat("muons_track_dz", glbTrack->dz ());
  NtupleFactory_->FillFloat("muons_track_d0err", glbTrack->d0Error ());
  NtupleFactory_->FillFloat("muons_track_dzerr", glbTrack->dzError ());
}

 ///---- fill electrons ----
 edm::Handle<edm::View<reco::GsfElectron> > EleHandle ;
 iEvent.getByLabel (EleTag_,EleHandle);
 int nEle;
 if(EleHandle->size() < 30 ){ nEle = EleHandle->size(); }
 else {nEle = 30;}
 
 //PG get the electron ID collections
 std::vector<edm::Handle<edm::ValueMap<float> > > eleIdCutHandles(4) ;
 iEvent.getByLabel (m_eleIDCut_LooseInputTag, eleIdCutHandles[0]) ;
 iEvent.getByLabel (m_eleIDCut_RLooseInputTag, eleIdCutHandles[1]) ;
 iEvent.getByLabel (m_eleIDCut_TightInputTag, eleIdCutHandles[2]) ;
 iEvent.getByLabel (m_eleIDCut_RTightInputTag, eleIdCutHandles[3]) ;

 
 for(int i=0; i< nEle; i++){
  
  //Get Ele Ref
  edm::Ref<edm::View<reco::GsfElectron> > electronEdmRef(EleHandle,i);
  
  math::XYZTLorentzVector* myvect_XYZT = new math::XYZTLorentzVector((*EleHandle)[i].p4().Px(),(*EleHandle)[i].p4().Py(),(*EleHandle)[i].p4().Pz(),(*EleHandle)[i].p4().E());
  NtupleFactory_->FillStdXYZTLorentzVector("electrons",myvect_XYZT);
  NtupleFactory_->FillFloat("electrons_charge",((*EleHandle)[i].charge()));
  NtupleFactory_->FillFloat("electrons_tkIso", ((*EleHandle)[i].dr03TkSumPt()));
  NtupleFactory_->FillFloat("electrons_emIso", ((*EleHandle)[i].dr03EcalRecHitSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));   
  
  //ELE ID
  NtupleFactory_->FillFloat("electrons_IdLoose",(*(eleIdCutHandles[0]))[electronEdmRef]);
  NtupleFactory_->FillFloat("electrons_IdTight",(*(eleIdCutHandles[1]))[electronEdmRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustLoose",(*(eleIdCutHandles[2]))[electronEdmRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustTight",(*(eleIdCutHandles[3]))[electronEdmRef]);
  
  //Get Ele Track
  reco::GsfTrackRef eleTrack  = (*EleHandle)[i].gsfTrack () ; 
  
  NtupleFactory_->FillFloat("electrons_track_d0", eleTrack->d0 ());
  NtupleFactory_->FillFloat("electrons_track_dz", eleTrack->dz ());
  NtupleFactory_->FillFloat("electrons_track_d0err", eleTrack->d0Error ());
  NtupleFactory_->FillFloat("electrons_track_dzerr", eleTrack->dzError ());
}

 ///---- fill tracks ----
 edm::Handle<edm::View<reco::Track> > TracksHandle ;
 iEvent.getByLabel (TracksTag_, TracksHandle) ;
 for (edm::View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); tkIt != TracksHandle->end (); ++tkIt ) 
{ 
  math::XYZVector vector_in = (*tkIt).innerMomentum () ; 
  NtupleFactory_->FillStdXYZVector("tracks_in",&vector_in);
  
  math::XYZVector vector_out = (*tkIt).outerMomentum () ; 
  NtupleFactory_->FillStdXYZVector("tracks_out",&vector_out);
  
  NtupleFactory_->FillFloat("tracks_d0",(*tkIt).d0());
  NtupleFactory_->FillFloat("tracks_dz",(*tkIt).dz());
  NtupleFactory_->FillFloat("tracks_d0err",(*tkIt).d0Error());
  NtupleFactory_->FillFloat("tracks_dzerr",(*tkIt).dzError());
}
 
 ///---- fill Primary Vertex
  edm::Handle<reco::VertexCollection> privtxs;
  iEvent.getByLabel(PrimaryVertexTag_, privtxs);
  
  int nPriVtx;
  if(privtxs->size() < 100 ){ nPriVtx = privtxs->size(); }
  else {nPriVtx = 100;}

  for(int i=0; i< nPriVtx; i++)
   {     
    math::XYZVector* myvect_XYZ = new math::XYZVector((*privtxs)[i].position().x(),(*privtxs)[i].position().y(),(*privtxs)[i].position().z());
    NtupleFactory_->FillStdXYZVector("priVtx_3vec",myvect_XYZ);
    NtupleFactory_->FillFloat("priVtx_xxE",(*privtxs)[i].covariance(0,0));
    NtupleFactory_->FillFloat("priVtx_yyE",(*privtxs)[i].covariance(1,1));
    NtupleFactory_->FillFloat("priVtx_zzE",(*privtxs)[i].covariance(2,2));
    NtupleFactory_->FillFloat("priVtx_yxE",(*privtxs)[i].covariance(1,0));
    NtupleFactory_->FillFloat("priVtx_zyE",(*privtxs)[i].covariance(2,1));
    NtupleFactory_->FillFloat("priVtx_zxE",(*privtxs)[i].covariance(2,0));
    NtupleFactory_->FillFloat("priVtx_chi2",(*privtxs)[i].chi2());
    NtupleFactory_->FillFloat("priVtx_ndof",(*privtxs)[i].ndof());
}      
  
 ///---- fill MCParticle ---- 
//  edm::Handle<reco::GenParticleCollection> genParticles;
//  iEvent.getByLabel(MCtruthTag_, genParticles);
// 
// //  int eventType_ = 1; //---- 0 = signal      1 = background 
// //  bool verbosity_ = true; //---- true = loquacious     false = silence
//  MCDumper mcAnalysis(genParticles, eventType_, verbosity_); //---- i "tau" mi fanno scrivere a schermo anche se NON è segnale
//  bool isValid = mcAnalysis.isValid();
//   
//  if( (eventType_ == 0) && (isValid == true) )
// {
//   math::XYZTLorentzVector* myvect_XYZT_mcH = new math::XYZTLorentzVector(mcAnalysis.mcH()->p4().Px(),mcAnalysis.mcH()->p4().Py(),mcAnalysis.mcH()->p4().Pz(),mcAnalysis.mcH()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mc_H",myvect_XYZT_mcH);
//   NtupleFactory_->FillFloat("mc_H_charge",mcAnalysis.mcH()->charge());
//   
//   math::XYZTLorentzVector* myvect_XYZT_mcV1 = new math::XYZTLorentzVector(mcAnalysis.mcV1()->p4().Px(),mcAnalysis.mcV1()->p4().Py(),mcAnalysis.mcV1()->p4().Pz(),mcAnalysis.mcV1()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcV1",myvect_XYZT_mcV1);
//   NtupleFactory_->FillFloat("mcV1_charge",mcAnalysis.mcV1()->charge());
//   NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysis.mcV1()->pdgId());
//   
//   math::XYZTLorentzVector* myvect_XYZT_mcV2 = new math::XYZTLorentzVector(mcAnalysis.mcV2()->p4().Px(),mcAnalysis.mcV2()->p4().Py(),mcAnalysis.mcV2()->p4().Pz(),mcAnalysis.mcV2()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcV2",myvect_XYZT_mcV2);
//   NtupleFactory_->FillFloat("mcV2_charge",mcAnalysis.mcV2()->charge());
//   NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysis.mcV2()->pdgId());
//      
//   math::XYZTLorentzVector* myvect_XYZT_mcF1_fromV1 = new math::XYZTLorentzVector(mcAnalysis.mcF1_fromV1()->p4().Px(),mcAnalysis.mcF1_fromV1()->p4().Py(),mcAnalysis.mcF1_fromV1()->p4().Pz(),mcAnalysis.mcF1_fromV1()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcF1_fromV1",myvect_XYZT_mcF1_fromV1);
//   NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysis.mcF1_fromV1()->charge());
//   NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysis.mcF1_fromV1()->pdgId());
// 
//   math::XYZTLorentzVector* myvect_XYZT_mcF2_fromV1 = new math::XYZTLorentzVector(mcAnalysis.mcF2_fromV1()->p4().Px(),mcAnalysis.mcF2_fromV1()->p4().Py(),mcAnalysis.mcF2_fromV1()->p4().Pz(),mcAnalysis.mcF2_fromV1()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcF2_fromV1",myvect_XYZT_mcF2_fromV1);
//   NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysis.mcF2_fromV1()->charge());
//   NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysis.mcF2_fromV1()->pdgId());
// 
//   math::XYZTLorentzVector* myvect_XYZT_mcF1_fromV2 = new math::XYZTLorentzVector(mcAnalysis.mcF1_fromV2()->p4().Px(),mcAnalysis.mcF1_fromV2()->p4().Py(),mcAnalysis.mcF1_fromV2()->p4().Pz(),mcAnalysis.mcF1_fromV2()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcF1_fromV2",myvect_XYZT_mcF1_fromV2);
//   NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysis.mcF1_fromV2()->charge());
//   NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysis.mcF1_fromV2()->pdgId());
// 
//   math::XYZTLorentzVector* myvect_XYZT_mcF2_fromV2 = new math::XYZTLorentzVector(mcAnalysis.mcF2_fromV2()->p4().Px(),mcAnalysis.mcF2_fromV2()->p4().Py(),mcAnalysis.mcF2_fromV2()->p4().Pz(),mcAnalysis.mcF2_fromV2()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcF2_fromV2",myvect_XYZT_mcF2_fromV2);
//   NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysis.mcF2_fromV2()->charge());
//   NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysis.mcF2_fromV2()->pdgId());
//     
//   math::XYZTLorentzVector* myvect_XYZT_mcQ1_tag = new math::XYZTLorentzVector(mcAnalysis.mcQ1_tag()->p4().Px(),mcAnalysis.mcQ1_tag()->p4().Py(),mcAnalysis.mcQ1_tag()->p4().Pz(),mcAnalysis.mcQ1_tag()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcQ1_tag",myvect_XYZT_mcQ1_tag);
//   NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysis.mcQ1_tag()->charge());
//   NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysis.mcQ1_tag()->pdgId());
// 
//   math::XYZTLorentzVector* myvect_XYZT_mcQ2_tag = new math::XYZTLorentzVector(mcAnalysis.mcQ2_tag()->p4().Px(),mcAnalysis.mcQ2_tag()->p4().Py(),mcAnalysis.mcQ2_tag()->p4().Pz(),mcAnalysis.mcQ2_tag()->p4().E());
//   NtupleFactory_->FillStdXYZTLorentzVector("mcQ2_tag",myvect_XYZT_mcQ2_tag);
//   NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysis.mcQ2_tag()->charge());
//   NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysis.mcQ2_tag()->pdgId());
//       
// }
  
 
  ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}

    
// ------------ method called once each job just before starting event loop  ------------
  
  void 
    SimpleNtple::beginJob(const edm::EventSetup& iSetup)
{
   
 NtupleFactory_->AddStdXYZTLorentzVector("muons");
 NtupleFactory_->AddFloat("muons_charge"); 
 NtupleFactory_->AddFloat("muons_tkIsoR03"); 
 NtupleFactory_->AddFloat("muons_nTkIsoR03"); 
 NtupleFactory_->AddFloat("muons_emIsoR03"); 
 NtupleFactory_->AddFloat("muons_hadIsoR03"); 
 NtupleFactory_->AddFloat("muons_tkIsoR05"); 
 NtupleFactory_->AddFloat("muons_nTkIsoR05"); 
 NtupleFactory_->AddFloat("muons_emIsoR05"); 
 NtupleFactory_->AddFloat("muons_hadIsoR05"); 
 NtupleFactory_->AddFloat("muons_track_d0"); 
 NtupleFactory_->AddFloat("muons_track_dz"); 
 NtupleFactory_->AddFloat("muons_track_d0err"); 
 NtupleFactory_->AddFloat("muons_track_dzerr"); 
 
   
 NtupleFactory_->AddStdXYZTLorentzVector("electrons");
 NtupleFactory_->AddFloat("electrons_charge"); 
 NtupleFactory_->AddFloat("electrons_tkIso"); 
 NtupleFactory_->AddFloat("electrons_emIso"); 
 NtupleFactory_->AddFloat("electrons_hadIso"); 
 NtupleFactory_->AddFloat("electrons_IdLoose"); 
 NtupleFactory_->AddFloat("electrons_IdTight"); 
 NtupleFactory_->AddFloat("electrons_IdRobustLoose"); 
 NtupleFactory_->AddFloat("electrons_IdRobustTight"); 
 NtupleFactory_->AddFloat("electrons_track_d0");
 NtupleFactory_->AddFloat("electrons_track_dz");
 NtupleFactory_->AddFloat("electrons_track_d0err");
 NtupleFactory_->AddFloat("electrons_track_dzerr");
  
 NtupleFactory_->AddStdXYZVector("tracks_in");
 NtupleFactory_->AddStdXYZVector("tracks_out");   
 NtupleFactory_->AddFloat("tracks_d0");
 NtupleFactory_->AddFloat("tracks_dz");   
 NtupleFactory_->AddFloat("tracks_d0err");
 NtupleFactory_->AddFloat("tracks_dzerr");   

 NtupleFactory_->AddStdXYZVector("priVtx_3vec");
 NtupleFactory_->AddFloat("priVtx_xxE");
 NtupleFactory_->AddFloat("priVtx_yyE");
 NtupleFactory_->AddFloat("priVtx_zzE");
 NtupleFactory_->AddFloat("priVtx_yxE");
 NtupleFactory_->AddFloat("priVtx_zyE");
 NtupleFactory_->AddFloat("priVtx_zxE");
 NtupleFactory_->AddFloat("priVtx_chi2");
 NtupleFactory_->AddFloat("priVtx_ndof");

//  NtupleFactory_->AddStdXYZTLorentzVector("mc_H");    
//  NtupleFactory_->AddFloat("mc_H_charge");    
//       
//  NtupleFactory_->AddStdXYZTLorentzVector("mcV1");         
//  NtupleFactory_->AddFloat("mcV1_charge");    
//  NtupleFactory_->AddFloat("mcV1_pdgId");    
//  
//  NtupleFactory_->AddStdXYZTLorentzVector("mcV2");         
//  NtupleFactory_->AddFloat("mcV2_charge");    
//  NtupleFactory_->AddFloat("mcV2_pdgId");  
//   
//  NtupleFactory_->AddStdXYZTLorentzVector("mcF1_fromV1");   
//  NtupleFactory_->AddFloat("mcF1_fromV1_charge");    
//  NtupleFactory_->AddFloat("mcF1_fromV1_pdgId");  
//        
//  NtupleFactory_->AddStdXYZTLorentzVector("mcF2_fromV1");         
//  NtupleFactory_->AddFloat("mcF2_fromV1_charge");    
//  NtupleFactory_->AddFloat("mcF2_fromV1_pdgId");  
//  
//  NtupleFactory_->AddStdXYZTLorentzVector("mcF1_fromV2");         
//  NtupleFactory_->AddFloat("mcF1_fromV2_charge");    
//  NtupleFactory_->AddFloat("mcF1_fromV2_pdgId");  
//  
//  NtupleFactory_->AddStdXYZTLorentzVector("mcF2_fromV2");         
//  NtupleFactory_->AddFloat("mcF2_fromV2_charge");    
//  NtupleFactory_->AddFloat("mcF2_fromV2_pdgId");  
//  
//  NtupleFactory_->AddStdXYZTLorentzVector("mcQ1_tag");    
//  NtupleFactory_->AddFloat("mcQ1_tag_charge");    
//  NtupleFactory_->AddFloat("mcQ1_tag_pdgId");  
//       
//  NtupleFactory_->AddStdXYZTLorentzVector("mcQ2_tag");         
//  NtupleFactory_->AddFloat("mcQ2_tag_charge");    
//  NtupleFactory_->AddFloat("mcQ2_tag_pdgId");  
 
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
  void 
    SimpleNtple::endJob() {
   
   NtupleFactory_->WriteNtuple();

}


// $Id: VBFReadEvent.cc,v 1.20 2007/11/26 12:39:19 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFReadEvent.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaHcalIsolation.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"


VBFReadEvent::VBFReadEvent (const edm::ParameterSet& iConfig) :
      m_metInputTag (iConfig.getParameter<edm::InputTag> ("metInputTag")) ,
      m_genMetInputTag (iConfig.getParameter<edm::InputTag> ("genMetInputTag")) ,
      m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
      m_genJetInputTag (iConfig.getParameter<edm::InputTag> ("genJetInputTag")) ,
      m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
      m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
      m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")) ,
      m_MC (iConfig.getParameter<edm::InputTag> ("MC")) ,
      m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")),
      m_trackInputTag (iConfig.getParameter<edm::InputTag> ("trackInputTag")),
      // trak isolation 
      m_ptMin  (iConfig.getParameter<double>("ptMin")),
      m_intRadius  (iConfig.getParameter<double>("intRadius")),
      m_extRadius  (iConfig.getParameter<double>("extRadius")),
      m_maxVtxDist  (iConfig.getParameter<double>("maxVtxDist")),
      //cal isolation
      m_hcalRecHitProducer (iConfig.getParameter<edm::InputTag>("hcalRecHitProducer")),
      m_emObjectProducer (iConfig.getParameter<edm::InputTag>("emObjectProducer")),
      m_egHcalIsoPtMin  (iConfig.getParameter<double>("etMinHI")),
      m_egHcalIsoConeSizeIn  (iConfig.getParameter<double>("intRadiusHI")), 
      m_egHcalIsoConeSizeOut  (iConfig.getParameter<double>("extRadiusHI")) 

// il resto del MC
// il trigger
// gli elettroni, guarda il codice ftto con roberto
{}


// --------------------------------------------------------------------


VBFReadEvent::~VBFReadEvent ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFReadEvent::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the calo MET
  edm::Handle<reco::CaloMETCollection> metCollectionHandle ;
  iEvent.getByLabel (m_metInputTag, metCollectionHandle) ;
    
  //PG get the gen MET
  edm::Handle<reco::GenMETCollection> genMetCollectionHandle ;
  iEvent.getByLabel (m_genMetInputTag, genMetCollectionHandle) ;

  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;
 
  //PG get the gen jet collection
  edm::Handle<reco::GenJetCollection> genJetCollectionHandle ;
  iEvent.getByLabel (m_genJetInputTag, genJetCollectionHandle) ;

  //PG get the GSF electrons collection
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle) ; 
    
  // VT get the traks   
  edm::Handle<TrackCollection> tracksCollectionHandle;
  iEvent.getByLabel(m_trackInputTag, tracksCollectionHandle); 
  const TrackCollection * trackCollection = tracksCollectionHandle.product () ;
    
  // Get Calo Geometry
  edm::ESHandle<CaloGeometry> caloGeometryHandle ;
  iSetup.get<IdealGeometryRecord>().get(caloGeometryHandle); 
  const CaloGeometry* caloGeom = caloGeometryHandle.product();  
    
  // Get the barrel hcal hits
  edm::Handle<HBHERecHitCollection> hcalRecHitHandle;
  iEvent.getByLabel(m_hcalRecHitProducer, hcalRecHitHandle);
    
  HBHERecHitMetaCollection mhbhe =  HBHERecHitMetaCollection (*hcalRecHitHandle) ;      
    
  // Get the  filtered objects
  edm::Handle<edm::View<reco::Candidate> > emObjectHandle ;
  iEvent.getByLabel (m_emObjectProducer, emObjectHandle) ;   
  
  //PG MC thruth collection  
  edm::Handle<edm::HepMCProduct> evtMC;
  iEvent.getByLabel(m_MC, evtMC);
  
  //PG MC thruth candidates collection  
  edm::Handle<CandidateCollection> genParticles;
  iEvent.getByLabel(m_MCtruthInputTag, genParticles);
       
  //PG get the electron ID collection
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle;
  iEvent.getByLabel (m_electronIDInputTag, electronIDAssocHandle);
    
//  PG check the result of the electron ID on a given ref
  reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr ;
    
   m_numberGSF = GSFHandle->size () ; 
  //PG loop over GSF electrons
  for (int i = 0 ; i < GSFHandle->size () ; ++i) 
    {
      //PG get the track
      const reco::GsfTrack* gsfTrack = 
                    & (*((*GSFHandle)[i].gsfTrack ())) ;
      //PG select according the to the hits number       
      //for the bug not fixed in CMSSW < 15X       
      if (gsfTrack->numberOfValidHits () < 5 && i > 9)  continue ; 
      //PG get some electron kinmatic vars
      //double  elePT  =  gsfTrack->pt () ; 
      //double  eleEta =  (*GSFHandle)[i].eta () ;
      //double  elePhi =  (*GSFHandle)[i].phi () ;
      reco::PixelMatchGsfElectronRef GSFref (GSFHandle, i) ;
      electronIDAssocItr = electronIDAssocHandle->find (GSFref) ;
      if (electronIDAssocItr == electronIDAssocHandle->end ()) continue ;
      const reco::ElectronIDRef& electronIDref = electronIDAssocItr->val ;
      bool cutBasedID = electronIDref->cutBasedDecision () ;
      std::cout << cutBasedID << std::endl; 
      m_recoEleCutBasedID -> push_back (int(cutBasedID)) ;

        
    } //PG loop over GSF electrons

  //PG fetch the MC information
  const HepMC::GenEvent * Evt = evtMC->GetEvent();
  
  findGenParticles (genParticles, *m_genHiggs, *m_genWm, *m_genWp, *m_genLepPlus, *m_genLepMinus,
                    *m_genMetPlus, *m_genMetMinus, *m_genqTagF, *m_genqTagB) ;
  
  TClonesArray &elePartMom = *m_recoEleTrkMomentumAtVtx;
  TClonesArray &elePartPos = *m_recoEleTrkPositionAtVtx;

  //PG get the isolation  
  ElectronTkIsolation myTkIsolation (m_extRadius, m_intRadius, m_ptMin, m_maxVtxDist, trackCollection) ; 

  int counter = 0; 
  for (PixelMatchGsfElectronCollection::const_iterator ele = GSFHandle->begin () ; 
       ele != GSFHandle->end () ; 
       ++ele ) 
      {
          new(elePartMom[counter]) TVector3 (getTrackMomentumAtVtx(*ele)); 
          new(elePartPos[counter]) TVector3 (getTrackPositionAtVtx(*ele)); 
          m_recoEleTrkIsoVal -> push_back (myTkIsolation.getPtTracks (&(*ele)));
          m_recoEleEcalEnergy -> push_back (ele->caloEnergy()) ;
          m_recoEleClass -> push_back (ele->classification()) ;
          counter++;
               
       } // end loop over PixelMatchGsfElectronCollection
    
  EgammaHcalIsolation myHadIsolation (m_egHcalIsoConeSizeOut, m_egHcalIsoConeSizeIn, m_egHcalIsoPtMin, caloGeom, &mhbhe) ;        
  
  for( size_t i = 0 ; i < emObjectHandle->size(); ++i) 
      {
          m_recoEleCalIsoVal->push_back (myHadIsolation.getHcalEtSum(&(emObjectHandle->at(i))));     

      }
      
   /*// looking for jets   
    for (PixelMatchGsfElectronCollection::const_iterator jet = jetCollectionHandle->begin () ; 
         jet != jetCollectionHandle->end () ; 
         ++jet ) 
        {
        }   
      */      
  
   m_genTree->Fill () ;
   
   m_recoEleEcalEnergy->clear ();
   m_recoEleTrkIsoVal->clear ();
   m_recoEleCalIsoVal->clear ();
   m_recoEleClass->clear ();
   m_recoEleCutBasedID->clear ();         
}
// --------------------------------------------------------------------


void 
VBFReadEvent::beginJob (const edm::EventSetup&)
{

    m_outfile  = new TFile ("prova.root", "RECREATE");
    m_genTree = new TTree ("genTree","generatedParticles") ;
    
    //generated particles 
    m_genHiggs = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genWp = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genWm = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genLepPlus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genLepMinus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genMetPlus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genMetMinus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genqTagF = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genqTagB = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_LepPlusFlavour = 0 ;
    m_LepMinusFlavour = 0 ;
    
    //reco electrons
    m_numberGSF = 0;
    m_recoEleTrkMomentumAtVtx = new TClonesArray ("TVector3");
    m_recoEleTrkPositionAtVtx = new TClonesArray ("TVector3");
    m_recoEleEcalEnergy = new std::vector<double>; 
    m_recoEleTrkIsoVal = new std::vector<double>;
    m_recoEleCalIsoVal = new std::vector<double>;
    m_recoEleClass = new std::vector<int>;
    m_recoEleCutBasedID = new std::vector<int>;
    
    m_genTree->Branch ("LepPlusFlavour", &m_LepPlusFlavour, "m_LepPlusFlavour/I");
    m_genTree->Branch ("LepMinusFlavour", &m_LepMinusFlavour, "m_LepMinusFlavour/I");
    m_genTree->Branch ("genHiggs","TLorentzVector",&m_genHiggs,6400,99) ;
    m_genTree->Branch ("genWp","TLorentzVector",&m_genWp,6400,99) ;
    m_genTree->Branch ("genWm","TLorentzVector",&m_genWm,6400,99) ;
    m_genTree->Branch ("genLepPlus","TLorentzVector",&m_genLepPlus,6400,99) ;
    m_genTree->Branch ("genLepMinus","TLorentzVector",&m_genLepMinus,6400,99) ;
    m_genTree->Branch ("genMetPlus","TLorentzVector",&m_genMetPlus,6400,99) ;
    m_genTree->Branch ("genMetMinus","TLorentzVector",&m_genMetMinus,6400,99) ;
    m_genTree->Branch ("genqTagF","TLorentzVector",&m_genqTagF,6400,99) ;
    m_genTree->Branch ("genqTagB","TLorentzVector",&m_genqTagB,6400,99) ;

    m_genTree->Branch ("numberGSF", &m_numberGSF, "m_numberGSF/I");
    m_genTree->Branch("recoEleTrkMomentumAtVtx", "TClonesArray", &m_recoEleTrkMomentumAtVtx, 256000,0); 
    m_genTree->Branch("recoEleTrkPositionAtVtx", "TClonesArray", &m_recoEleTrkPositionAtVtx, 256000,0);
    m_genTree->Branch("recoEleEcalEnergy",  &m_recoEleEcalEnergy); 
    m_genTree->Branch("recoEleTrkIsoVal",  &m_recoEleTrkIsoVal); 
    m_genTree->Branch("recoEleCalIsoVal",  &m_recoEleCalIsoVal); 
    m_genTree->Branch("recoEleClass",  &m_recoEleClass);
    m_genTree->Branch("recoEleCutBasedID",  &m_recoEleCutBasedID);

}


// --------------------------------------------------------------------


void 
VBFReadEvent::endJob () 
{
    m_genTree->Write () ;
    m_outfile->Write () ; 
    m_outfile->Close () ;
}

// --------------------------------------------------------------------

void VBFReadEvent::setMomentum (TLorentzVector & myvector, const Candidate & gen)
{
    myvector.SetPx (gen.px());
    myvector.SetPy (gen.py());
    myvector.SetPz (gen.pz());
    myvector.SetE (gen.energy());
}

// --------------------------------------------------------------------

TVector3 VBFReadEvent::getTrackMomentumAtVtx (const PixelMatchGsfElectron & ele)
{
    TVector3 myVect;
    myVect.	SetXYZ ((ele.trackMomentumAtVtx()).x(), (ele.trackMomentumAtVtx()).y(), (ele.trackMomentumAtVtx()).z()) ;
    return myVect;
}

// --------------------------------------------------------------------

TVector3 VBFReadEvent::getTrackPositionAtVtx (const PixelMatchGsfElectron & ele)
{
    TVector3 myVect;
    myVect.	SetXYZ ((ele.TrackPositionAtVtx()).x(), (ele.TrackPositionAtVtx()).y(), (ele.TrackPositionAtVtx()).z()) ;
    return myVect;
}


//11 ele-
//12 nu_ele
//13 mu-
//14 nu_mu
//1-6 quarks
//W+ 24
//W- -24
//Z 23
//h 25
//g 21 ... nella WW fusion i vertici coinvolgono solo q...    
void VBFReadEvent::findGenParticles (edm::Handle<CandidateCollection> &genParticles,
                                                             TLorentzVector &m_genHiggs,
                                                             TLorentzVector &m_genWm,
                                                             TLorentzVector &m_genWp,
                                                             TLorentzVector &m_genLepPlus,
                                                             TLorentzVector &m_genLepMinus,
                                                             TLorentzVector &m_genMetPlus,
                                                             TLorentzVector &m_genMetMinus,
                                                             TLorentzVector &m_genqTagF,
                                                             TLorentzVector &m_genqTagB)
{
    for (CandidateCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
    {
        int mumPDG = p->pdgId();
        int mumSTATUS = p->status() ;

        ///////////////////////////////////////////////// tag quark /////////////////////////////////////////////////
        //misteriosamente i tag sono i fratelli dell'higgs
        //quindi parto dall'higgs e ne prendo le mamme e quindi riguardo i figli
        if ((abs(mumPDG)==25) && (mumSTATUS ==3))
            {
                setMomentum (m_genHiggs, *p) ; 
                const Candidate * interact0 = p->mother(0) ;
                if ((interact0->daughter(1)->eta()) > (interact0->daughter(0)->eta())) {
                    setMomentum (m_genqTagF, *(interact0->daughter(1))) ;
                setMomentum (m_genqTagB, *(interact0->daughter(0))) ;}
                else {
                    setMomentum (m_genqTagB, *(interact0->daughter(1))) ;
                setMomentum (m_genqTagF, *(interact0->daughter(0))) ;}
            }
        
        ///////////////////////////////////////////////// W- /////////////////////////////////////////////////
        
        else if (mumPDG == -24 &&  mumSTATUS ==3) //W-
            {
                setMomentum (m_genWm, *p) ; 
                for ( size_t i = 0; i < p->numberOfDaughters(); ++ i ) 
                    {
                        const Candidate * daughter = p->daughter ( i );
                        int PDG = daughter -> pdgId() ;    
                        if (PDG==11) { // e-
                            setMomentum (m_genLepMinus, *daughter) ;
                        m_LepMinusFlavour = 11 ;}
                        else if (PDG==-12) {//nu_e_bar
                        setMomentum (m_genMetMinus, *daughter) ;}
                        else if (PDG==13) {//mu-
                            setMomentum (m_genLepMinus, *daughter) ;
                        m_LepMinusFlavour = 13 ;}
                        else if (PDG==-14) {//nu_mu_bar
                        setMomentum (m_genMetMinus, *daughter) ;}
                    }
            }
        
        ///////////////////////////////////////////////// W+ /////////////////////////////////////////////////
        
        else if (mumPDG == 24 &&  mumSTATUS ==3) //W+
            {  
                setMomentum (m_genWp, *p) ; 
                for ( size_t i = 0; i < p->numberOfDaughters(); ++ i ) 
                    {
                        const Candidate *daughter = p->daughter ( i );
                        int PDG = daughter-> pdgId() ;    
                        if (PDG==-11) {//e+
                            setMomentum (m_genLepPlus, *daughter) ;
                        m_LepPlusFlavour = 11 ;}
                        else if (PDG==12) {//nu_e
                        setMomentum (m_genMetPlus, *daughter) ;}
                        else if (PDG==-13) {//mu+
                            setMomentum (m_genLepPlus, *daughter) ;
                        m_LepPlusFlavour = 13 ;}
                        else if (PDG==14) {//nu_mu
                        setMomentum (m_genMetPlus, *daughter) ;}
                    }
            }
        
    }
}

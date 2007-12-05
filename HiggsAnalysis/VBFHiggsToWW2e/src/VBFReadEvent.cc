// $Id: VBFReadEvent.cc,v 1.34 2007/12/05 10:58:13 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFReadEvent.h"

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
      m_egHcalIsoConeSizeOut  (iConfig.getParameter<double>("extRadiusHI")),
      // get flavour
      m_jfi (iConfig.getParameter<edm::ParameterSet>("jetIdParameters")),
      //rootfile for the tree
      m_rootfile (iConfig.getUntrackedParameter<std::string> ("rootfile")) 
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
  edm::Handle<reco::CaloMETCollection> metCollectionHandle;
  iEvent.getByLabel (m_metInputTag, metCollectionHandle);
  const CaloMETCollection *calometcol = metCollectionHandle.product();
  const CaloMET *calomet = &(calometcol->front());  
    
  //PG get the gen MET
  edm::Handle<reco::GenMETCollection> genMetCollectionHandle;
  iEvent.getByLabel (m_genMetInputTag, genMetCollectionHandle);
  const GenMETCollection *genmetcol = genMetCollectionHandle.product();
  const GenMET *genmet = &(genmetcol->front());

  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle);
 
  //PG get the gen jet collection
  edm::Handle<reco::GenJetCollection> genJetCollectionHandle;
  iEvent.getByLabel (m_genJetInputTag, genJetCollectionHandle);

  //PG get the GSF electrons collection
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 
  
  //VT get the Global muons collection
  edm::Handle<reco::MuonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 
    
  //VT get the traks   
  edm::Handle<TrackCollection> tracksCollectionHandle;
  iEvent.getByLabel(m_trackInputTag, tracksCollectionHandle); 
  const TrackCollection * trackCollection = tracksCollectionHandle.product ();
    
  // Get Calo Geometry
  edm::ESHandle<CaloGeometry> caloGeometryHandle;
  iSetup.get<IdealGeometryRecord>().get(caloGeometryHandle); 
  const CaloGeometry* caloGeom = caloGeometryHandle.product();  
    
  // Get the barrel hcal hits
  edm::Handle<HBHERecHitCollection> hcalRecHitHandle;
  iEvent.getByLabel(m_hcalRecHitProducer, hcalRecHitHandle);
    
  HBHERecHitMetaCollection mhbhe =  HBHERecHitMetaCollection (*hcalRecHitHandle);      
    
  // Get the  filtered objects
  edm::Handle<edm::View<reco::Candidate> > emObjectHandle;
  iEvent.getByLabel (m_emObjectProducer, emObjectHandle);   
  
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
  reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr;
    
  //PG fetch the MC information
  const HepMC::GenEvent * Evt = evtMC->GetEvent();
  m_evtFlag = Evt->signal_process_id();
  if (m_evtFlag == 123 || m_evtFlag == 124)
    {
       findGenParticles (genParticles, *m_genHiggs, *m_genWm, *m_genWp, *m_genLepPlus, *m_genLepMinus,
                                                *m_genMetPlus, *m_genMetMinus, *m_genqTagF, *m_genqTagB,
                                              	 m_genqTagF_Flavour, m_genqTagB_Flavour);
    }
  else setZero (*m_genHiggs, *m_genWm, *m_genWp, *m_genLepPlus, *m_genLepMinus,
		*m_genMetPlus, *m_genMetMinus, *m_genqTagF, *m_genqTagB,
	         m_genqTagF_Flavour, m_genqTagB_Flavour);
			 
			 
  //////////////////////////////////////// reco muons	
  		 
  m_numberGlobMuons = MuonHandle->size (); 
  TClonesArray &muPart4Mom = *m_recoMuon4Momentum;
  TClonesArray &muPartPos = *m_recoMuonTrkPositionAtVtx;			 

  int counter = 0; 
  for (MuonCollection::const_iterator mu = MuonHandle->begin (); 
       mu != MuonHandle->end (); 
       ++mu ) 
      {
          new (muPart4Mom[counter]) TLorentzVector (get4momentum (*mu));
		  new (muPartPos[counter]) TVector3 (getVtx (*mu));
		  m_recoMuonCharge -> push_back (mu->charge());
		  m_recoMuonR03_sumPt -> push_back (mu->getIsolationR03().sumPt);
	      m_recoMuonR03_emEt -> push_back (mu->getIsolationR03().emEt);
	      m_recoMuonR03_hadEt -> push_back (mu->getIsolationR03().hadEt);
	      m_recoMuonR03_hoEt -> push_back (mu->getIsolationR03().hoEt);
	      m_recoMuonR03_nTracks -> push_back (mu->getIsolationR03().nTracks);
	      m_recoMuonR03_nJets -> push_back (mu->getIsolationR03().nJets);

	  }  
	  
  //////////////////////////////////////// reco electrons			 

  TClonesArray &elePart4Mom = *m_recoEle4Momentum;
  TClonesArray &elePartMom = *m_recoEleTrkMomentumAtVtx;
  TClonesArray &elePartPos = *m_recoEleTrkPositionAtVtx;

  ElectronTkIsolation myTkIsolation (m_extRadius, m_intRadius, m_ptMin, m_maxVtxDist, trackCollection); 

  m_numberGSF = GSFHandle->size ();   
  counter = 0; 
  for (PixelMatchGsfElectronCollection::const_iterator ele = GSFHandle->begin (); 
       ele != GSFHandle->end (); 
       ++ele ) 
      {
          new (elePart4Mom[counter]) TLorentzVector (get4momentum (*ele));
          new (elePartMom[counter]) TVector3 (getTrackMomentumAtVtx (*ele)); 
          //new (elePartPos[counter]) TVector3 (getTrackPositionAtVtx (*ele));
		  new (elePartPos[counter]) TVector3 (getVtx (*ele));
          m_recoEleTrkIsoVal -> push_back (myTkIsolation.getPtTracks (&(*ele)));
          m_recoEleEcalEnergy -> push_back (ele->caloEnergy());
          m_recoEleClass -> push_back (ele->classification());
		  m_recoEleCharge -> push_back (ele->charge());

          const reco::GsfTrack* gsfTrack =  & (*((*GSFHandle)[counter].gsfTrack ()));
          //PG select according the to the hits number       
          //for the bug not fixed in CMSSW < 15X       
          //if (gsfTrack->numberOfValidHits () < 5); 
          //PG get some electron kinmatic vars
          //double  elePT  =  gsfTrack->pt (); 
          //double  eleEta =  (*GSFHandle)[i].eta ();
          //double  elePhi =  (*GSFHandle)[i].phi ();
          bool cutBasedID = 0;
          reco::PixelMatchGsfElectronRef GSFref (GSFHandle, counter);
          electronIDAssocItr = electronIDAssocHandle->find (GSFref);
          if (electronIDAssocItr != electronIDAssocHandle->end ())
              {
              const reco::ElectronIDRef& electronIDref = electronIDAssocItr->val;
              cutBasedID = electronIDref->cutBasedDecision ();
              }
          m_recoEleCutBasedID -> push_back (int(cutBasedID));
          counter++;               
       } // end loop over PixelMatchGsfElectronCollection
    
  EgammaHcalIsolation myHadIsolation (m_egHcalIsoConeSizeOut, m_egHcalIsoConeSizeIn, m_egHcalIsoPtMin, caloGeom, &mhbhe);        
 
  for( size_t i = 0; i < emObjectHandle->size(); ++i) 
      {
          m_recoEleCalIsoVal->push_back (myHadIsolation.getHcalEtSum(&(emObjectHandle->at(i))));     
      }
    
    //////////////////////////////////////// looking for jets
    m_jfi.readEvent (iEvent); // for the flavour
    TClonesArray &jetPart4Mom = *m_recoJet4Momentum;
    counter = 0;
    m_numberJet =  jetCollectionHandle->size ();
    for (CaloJetCollection::const_iterator jet = jetCollectionHandle->begin (); 
         jet != jetCollectionHandle->end (); 
         ++jet ) 
        {
            new (jetPart4Mom[counter]) TLorentzVector (get4momentum (*jet));
            counter++;
	    JetFlavour jetFlavour = m_jfi.identifyBasedOnPartons(*(jet));
	    int myflav = jetFlavour.flavour ();
	    m_recoJetFlavour -> push_back (myflav);
        }   
    
    
    //////////////////////////////////////// looking for genjets
    TClonesArray &genJetPart4Mom = *m_genJet4Momentum;
    counter = 0;
    m_numberGenJet =  genJetCollectionHandle->size ();
    for (GenJetCollection::const_iterator jet = genJetCollectionHandle->begin (); 
         jet != genJetCollectionHandle->end (); 
         ++jet ) 
        {
            new (genJetPart4Mom[counter]) TLorentzVector (get4momentum (*jet));
            counter++;
	    JetFlavour jetFlavour = m_jfi.identifyBasedOnPartons(*(jet));
	    int myflav = jetFlavour.flavour ();
	    m_genJetFlavour -> push_back (myflav);
        }   
    
    //////////////////////////////////////// looking for met
    setMomentum (*m_recoMet4Momentum, *calomet);
    
    //////////////////////////////////////// looking for gen met
    setMomentum (*m_genMet4Momentum, *genmet);
    
   m_genTree->Fill ();
   
   m_recoEleEcalEnergy->clear ();
   m_recoEleTrkIsoVal->clear ();
   m_recoEleCalIsoVal->clear ();
   m_recoEleClass->clear ();
   m_recoEleCutBasedID->clear ();
   m_recoEleCharge->clear ();         
   m_genJetFlavour->clear ();
   m_recoJetFlavour->clear ();
   m_recoMuonCharge->clear ();
   m_recoMuonR03_sumPt->clear ();
   m_recoMuonR03_emEt->clear ();
   m_recoMuonR03_hadEt->clear ();
   m_recoMuonR03_hoEt->clear ();
   m_recoMuonR03_nTracks->clear ();
   m_recoMuonR03_nJets->clear ();
}
// --------------------------------------------------------------------


void 
VBFReadEvent::beginJob (const edm::EventSetup&)
{

    m_outfile  = new TFile (m_rootfile.c_str(), "RECREATE");
    m_genTree = new TTree ("tree","generatedParticles");
    
    m_evtFlag = 0; // MC process ID

    //generated particles 
    m_genHiggs = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genWp = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genWm = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genLepPlus = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genLepMinus = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genMetPlus = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genMetMinus = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genqTagF = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_genqTagB = new TLorentzVector (0.0,0.0,0.0,0.0);
    m_LepPlusFlavour = 0;
    m_LepMinusFlavour = 0;
    m_genqTagF_Flavour = 0;
    m_genqTagB_Flavour = 0;
	
	//reco muons
	m_numberGlobMuons = 0;
    m_recoMuon4Momentum = new TClonesArray ("TLorentzVector");
	m_recoMuonTrkPositionAtVtx = new TClonesArray ("TVector3");
	m_recoMuonCharge = new std::vector<int>;
	m_recoMuonR03_sumPt = new std::vector<double>;
    m_recoMuonR03_emEt = new std::vector<double>;
    m_recoMuonR03_hadEt = new std::vector<double>;
    m_recoMuonR03_hoEt = new std::vector<double>;
    m_recoMuonR03_nTracks = new std::vector<int>;
    m_recoMuonR03_nJets = new std::vector<int>;
    
    //reco electrons
    m_numberGSF = 0;
    m_recoEle4Momentum = new TClonesArray ("TLorentzVector");
    m_recoEleTrkMomentumAtVtx = new TClonesArray ("TVector3");
    m_recoEleTrkPositionAtVtx = new TClonesArray ("TVector3");
    m_recoEleEcalEnergy = new std::vector<double>; 
    m_recoEleTrkIsoVal = new std::vector<double>;
    m_recoEleCalIsoVal = new std::vector<double>;
    m_recoEleClass = new std::vector<int>;
    m_recoEleCutBasedID = new std::vector<int>;
	m_recoEleCharge = new std::vector<int>;

  
    // gen jets
    m_numberGenJet = 0;
    m_genJet4Momentum = new TClonesArray ("TLorentzVector");
    m_genJetFlavour = new std::vector<int>;    
    
    // reco jets
    m_numberJet = 0;
    m_recoJet4Momentum = new TClonesArray ("TLorentzVector");    
    m_recoJetFlavour = new std::vector<int>;  
    
    // gen met
    m_genMet4Momentum= new TLorentzVector (0.0,0.0,0.0,0.0);   
    
    // reco met
    m_recoMet4Momentum = new TLorentzVector (0.0,0.0,0.0,0.0); 
    
    m_genTree->Branch ("evtFlag", &m_evtFlag, "m_evtFlag/I");
    m_genTree->Branch ("LepPlusFlavour", &m_LepPlusFlavour, "m_LepPlusFlavour/I");
    m_genTree->Branch ("LepMinusFlavour", &m_LepMinusFlavour, "m_LepMinusFlavour/I");
    m_genTree->Branch ("genHiggs","TLorentzVector",&m_genHiggs,6400,99);
    m_genTree->Branch ("genWp","TLorentzVector",&m_genWp,6400,99);
    m_genTree->Branch ("genWm","TLorentzVector",&m_genWm,6400,99);
    m_genTree->Branch ("genLepPlus","TLorentzVector",&m_genLepPlus,6400,99);
    m_genTree->Branch ("genLepMinus","TLorentzVector",&m_genLepMinus,6400,99);
    m_genTree->Branch ("genMetPlus","TLorentzVector",&m_genMetPlus,6400,99);
    m_genTree->Branch ("genMetMinus","TLorentzVector",&m_genMetMinus,6400,99);
    m_genTree->Branch ("genqTagF","TLorentzVector",&m_genqTagF,6400,99);
    m_genTree->Branch ("genqTagB","TLorentzVector",&m_genqTagB,6400,99);
    m_genTree->Branch ("genqTagF_Flavour", &m_genqTagF_Flavour, "m_genqTagF_Flavour/I");
    m_genTree->Branch ("genqTagB_Flavour", &m_genqTagB_Flavour, "m_genqTagB_Flavour/I");
	
	m_genTree->Branch ("numberGlobMuons", &m_numberGlobMuons, "m_numberGlobMuons/I");
	m_genTree->Branch ("recoMuonR03_sumPt", &m_recoMuonR03_sumPt);
    m_genTree->Branch ("recoMuonR03_emEt", &m_recoMuonR03_emEt);
    m_genTree->Branch ("recoMuonR03_hadEt", &m_recoMuonR03_hadEt);
    m_genTree->Branch ("recoMuonR03_hoEt", &m_recoMuonR03_hoEt);
    m_genTree->Branch ("recoMuonR03_nTracks", &m_recoMuonR03_nTracks);
    m_genTree->Branch ("recoMuonR03_nJets", &m_recoMuonR03_nJets);
    m_genTree->Branch ("recoMuon4Momentum", "TClonesArray", &m_recoMuon4Momentum, 256000,0);
	m_genTree->Branch ("recoMuonTrkPositionAtVtx", "TClonesArray", &m_recoMuonTrkPositionAtVtx, 256000,0);
	m_genTree->Branch ("recoMuonCharge", &m_recoMuonCharge);
	
    m_genTree->Branch ("numberGSF", &m_numberGSF, "m_numberGSF/I");
    m_genTree->Branch ("recoEle4Momentum", "TClonesArray", &m_recoEle4Momentum, 256000,0); 
    m_genTree->Branch ("recoEleTrkMomentumAtVtx", "TClonesArray", &m_recoEleTrkMomentumAtVtx, 256000,0); 
    m_genTree->Branch ("recoEleTrkPositionAtVtx", "TClonesArray", &m_recoEleTrkPositionAtVtx, 256000,0);
    m_genTree->Branch ("recoEleEcalEnergy",  &m_recoEleEcalEnergy); 
    m_genTree->Branch ("recoEleTrkIsoVal",  &m_recoEleTrkIsoVal); 
    m_genTree->Branch ("recoEleCalIsoVal",  &m_recoEleCalIsoVal); 
    m_genTree->Branch ("recoEleClass",  &m_recoEleClass);
    m_genTree->Branch ("recoEleCutBasedID",  &m_recoEleCutBasedID);
	m_genTree->Branch ("recoEleCharge", &m_recoEleCharge);
    
    m_genTree->Branch ("numberGenJet", &m_numberGenJet, "m_numberGenJet/I");
    m_genTree->Branch ("genJet4Momentum", "TClonesArray", &m_genJet4Momentum, 256000,0); 
    m_genTree->Branch ("genJetFlavour",  &m_genJetFlavour);
    
    m_genTree->Branch ("numberJet", &m_numberJet, "m_numberJet/I");
    m_genTree->Branch ("recoJet4Momentum", "TClonesArray", &m_recoJet4Momentum, 256000,0); 
    m_genTree->Branch ("recoJetFlavour",  &m_recoJetFlavour);
    
    m_genTree->Branch ("genMet4Momentum", "TLorentzVector", &m_genMet4Momentum, 6400,99); 
    
    m_genTree->Branch ("recoMet4Momentum", "TLorentzVector", &m_recoMet4Momentum, 6400,99); 
    

}


// --------------------------------------------------------------------


void 
VBFReadEvent::endJob () 
{
    m_genTree->Write ();
    m_outfile->Write (); 
    m_outfile->Close ();
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
    myVect.	SetXYZ ((ele.trackMomentumAtVtx()).x(), (ele.trackMomentumAtVtx()).y(), (ele.trackMomentumAtVtx()).z());
    return myVect;
}

// --------------------------------------------------------------------

TVector3 VBFReadEvent::getTrackPositionAtVtx (const PixelMatchGsfElectron & ele)
{
    TVector3 myVect;
    myVect.SetXYZ ((ele.TrackPositionAtVtx()).x(), (ele.TrackPositionAtVtx()).y(), (ele.TrackPositionAtVtx()).z());
    return myVect;
}

// --------------------------------------------------------------------

TLorentzVector VBFReadEvent::get4momentum (const Candidate & gen)
{
    TLorentzVector myvector;
    myvector.SetPxPyPzE (gen.px(), gen.py(), gen.pz(), gen.energy());
    return myvector;
}

// --------------------------------------------------------------------

TVector3 VBFReadEvent::getVtx (const Candidate & gen)
{
    TVector3 myvector;
    myvector.SetXYZ (gen.vx(), gen.vy(), gen.vz());
	return myvector;
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

void VBFReadEvent::setZero (TLorentzVector &m_genHiggs,
				     TLorentzVector &m_genWm,
				     TLorentzVector &m_genWp,
				     TLorentzVector &m_genLepPlus,
				     TLorentzVector &m_genLepMinus,
				     TLorentzVector &m_genMetPlus,
				     TLorentzVector &m_genMetMinus,
				     TLorentzVector &m_genqTagF,
				     TLorentzVector &m_genqTagB,
				     int & m_genqTagF_Flavour,
				     int & m_genqTagB_Flavour)
{
  m_genHiggs.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genWm.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genWp.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genLepPlus.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genLepMinus.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genMetPlus.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genMetMinus.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genqTagF.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genqTagB.SetPxPyPzE (0.0,0.0,0.0,0.0);
  m_genqTagF_Flavour = 0;
  m_genqTagB_Flavour = 0;
}


void VBFReadEvent::findGenParticles (edm::Handle<CandidateCollection> &genParticles,
                                                             TLorentzVector &m_genHiggs,
                                                             TLorentzVector &m_genWm,
                                                             TLorentzVector &m_genWp,
                                                             TLorentzVector &m_genLepPlus,
                                                             TLorentzVector &m_genLepMinus,
                                                             TLorentzVector &m_genMetPlus,
                                                             TLorentzVector &m_genMetMinus,
                                                             TLorentzVector &m_genqTagF,
                                                             TLorentzVector &m_genqTagB,
							     int & m_genqTagF_Flavour,
							     int & m_genqTagB_Flavour)
{
    for (CandidateCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
    {
        int mumPDG = p->pdgId();
        int mumSTATUS = p->status();

        ///////////////////////////////////////////////// tag quark /////////////////////////////////////////////////
        //misteriosamente i tag sono i fratelli dell'higgs
        //quindi parto dall'higgs e ne prendo le mamme e quindi riguardo i figli
        if ((abs(mumPDG)==25) && (mumSTATUS ==3))
            {
                setMomentum (m_genHiggs, *p); 
                const Candidate * interact0 = p->mother(0);
                if ((interact0->daughter(1)->eta()) > (interact0->daughter(0)->eta())) {
                    setMomentum (m_genqTagF, *(interact0->daughter(1)));
                    setMomentum (m_genqTagB, *(interact0->daughter(0)));
		    m_genqTagF_Flavour = interact0->daughter (1)->pdgId ();
                    m_genqTagB_Flavour = interact0->daughter (0)->pdgId ();}
                else {
                    setMomentum (m_genqTagB, *(interact0->daughter(1)));
                    setMomentum (m_genqTagF, *(interact0->daughter(0)));
		    m_genqTagF_Flavour = interact0->daughter (0)->pdgId ();
                    m_genqTagB_Flavour = interact0->daughter (1)->pdgId ();}
            }
        
        ///////////////////////////////////////////////// W- /////////////////////////////////////////////////
        
        else if (mumPDG == -24 &&  mumSTATUS ==3) //W-
            {
                setMomentum (m_genWm, *p); 
                for ( size_t i = 0; i < p->numberOfDaughters(); ++ i ) 
                    {
                        const Candidate * daughter = p->daughter ( i );
                        int PDG = daughter -> pdgId();    
                        if (PDG==11) { // e-
                            setMomentum (m_genLepMinus, *daughter);
                        m_LepMinusFlavour = 11;}
                        else if (PDG==-12) {//nu_e_bar
                        setMomentum (m_genMetMinus, *daughter);}
                        else if (PDG==13) {//mu-
                            setMomentum (m_genLepMinus, *daughter);
                        m_LepMinusFlavour = 13;}
                        else if (PDG==-14) {//nu_mu_bar
                        setMomentum (m_genMetMinus, *daughter);}
                    }
            }
        
        ///////////////////////////////////////////////// W+ /////////////////////////////////////////////////
        
        else if (mumPDG == 24 &&  mumSTATUS ==3) //W+
            {  
                setMomentum (m_genWp, *p); 
                for ( size_t i = 0; i < p->numberOfDaughters(); ++ i ) 
                    {
                        const Candidate *daughter = p->daughter ( i );
                        int PDG = daughter-> pdgId();    
                        if (PDG==-11) {//e+
                            setMomentum (m_genLepPlus, *daughter);
                        m_LepPlusFlavour = 11;}
                        else if (PDG==12) {//nu_e
                        setMomentum (m_genMetPlus, *daughter);}
                        else if (PDG==-13) {//mu+
                            setMomentum (m_genLepPlus, *daughter);
                        m_LepPlusFlavour = 13;}
                        else if (PDG==14) {//nu_mu
                        setMomentum (m_genMetPlus, *daughter);}
                    }
            }
        
    }
}

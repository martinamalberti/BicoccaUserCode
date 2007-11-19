// $Id: VBFReadEvent.cc,v 1.8 2007/11/19 13:24:17 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFReadEvent.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "CLHEP/HepMC/GenEvent.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"


VBFReadEvent::VBFReadEvent (const edm::ParameterSet& iConfig) :
      m_metInputTag (iConfig.getParameter<edm::InputTag> ("metInputTag")) ,
      m_genMetInputTag (iConfig.getParameter<edm::InputTag> ("genMetInputTag")) ,
      m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
      m_genJetInputTag (iConfig.getParameter<edm::InputTag> ("genJetInputTag")) ,
      m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
      //m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
      m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")) ,
      m_MC (iConfig.getParameter<edm::InputTag> ("MC")) ,
      m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag"))
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

    //PG MC thruth collection  
    edm::Handle<edm::HepMCProduct> evtMC;
    iEvent.getByLabel(m_MC, evtMC);
  
    //PG MC thruth candidates collection  
    edm::Handle<CandidateCollection> genParticles;
    iEvent.getByLabel(m_MCtruthInputTag, genParticles);
    
   
    //PG get the electron ID collection
    //edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle;
    //iEvent.getByLabel (m_electronIDInputTag, electronIDAssocHandle);
    

//  PG check the result of the electron ID on a given ref
//  reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr ;
//  electronIDAssocItr = electronIDTightAssocHandle->find (ref) ;
//  if (electronIDAssocItr == electronIDAssocHandle->end ()) continue ;
//  const reco::ElectronIDRef& electronIDref = electronIDAssocItr->val ;
//  bool cutBasedID = electronIDref->cutBasedDecision () ;
    
    //11 ele-
    //12 nu_ele
    //13 mu-
    //14 nu_mu
    //1-6 quarks
    //W+ 24
    //W- -24
    //h 25
    //g 21 ... nella WW fusion i vertici coinvolgono solo q...    
 
    const HepMC::GenEvent * Evt = evtMC->GetEvent();
    
    std::vector<const Candidate *> tags;
    
    for (CandidateCollection::const_iterator p = genParticles->begin(); 
        p != genParticles->end(); 
        ++ p) 
        {
        int mumPDG = (p)->pdgId();
        int mumSTATUS = (p)->status() ;

///////////////////////////////////////////////// tag quark /////////////////////////////////////////////////
            
        if (abs(mumPDG) > 0 && abs(mumPDG) < 6 && mumSTATUS ==3)
            {
            int haUnFiglioW = 0;
            int haUnFiglioQ = 0 ;
            const Candidate *myTag = (p)->daughter (0) ; // tanto per inizializzare senno si incazza
            for ( size_t i = 0; i < (p)->numberOfDaughters(); ++ i ) 
                {
                    const Candidate * daughter = (p)->daughter( i );
                    int PDG = daughter-> pdgId() ;
                    if (abs(PDG) == 24 || abs(PDG)==25) haUnFiglioW = 1;
                    if (abs(PDG) > 0 && abs(PDG) < 6) 
                        {
                        haUnFiglioQ = 1 ;
                        myTag = daughter ;
                        }    
                }
            if (haUnFiglioW==1 &&  haUnFiglioQ==1) tags.push_back (myTag) ;       
            }
            
            if (tags.at(0)->theta () > tags.at(1)->theta () ){
                setMomentum (*m_genqTagF, *tags.at(0)) ;
                setMomentum (*m_genqTagB, *tags.at(1)) ;}
            else {
                setMomentum (*m_genqTagB, *tags.at(0)) ;
                setMomentum (*m_genqTagF, *tags.at(1)) ; }
           
///////////////////////////////////////////////// W- /////////////////////////////////////////////////
             /*           
             if (mumPDG == -24 &&  mumSTATUS ==3) //W-
                {                    
                    for ( size_t i = 0; i < (p)->numberOfDaughters(); ++ i ) 
                        {
                        const Candidate * daughter = (p)->daughter ( i );
                        int PDG = daughter -> pdgId() ;    
                        if (PDG==11) { // e-
                                setMomentum (*m_genLepMinus, *daughter) ;
                                m_LepMinusFlavour = 11 ;}
                         else if (PDG==-12) {//nu_e_bar
                                setMomentum (*m_genMetMinus, *daughter) ;}
                         else if (PDG==13) {//mu-
                                setMomentum (*m_genLepMinus, *daughter) ;
                                m_LepMinusFlavour = 13 ;}
                        else if (PDG==-14) {//nu_mu_bar
                                setMomentum (*m_genMetMinus, *daughter) ;}
                        }
                }
            
///////////////////////////////////////////////// W+ /////////////////////////////////////////////////

             else if (mumPDG == 24 &&  mumSTATUS ==3) //W+
                {                    
                    for ( size_t i = 0; i < (p)->numberOfDaughters(); ++ i ) 
                        {
                            const Candidate *daughter = (p)->daughter ( i );
                            int PDG = daughter-> pdgId() ;    
                            if (PDG==-11) {//e+
                                    setMomentum (*m_genLepPlus, *daughter) ;
                                    m_LepPlusFlavour = 11 ;}
                            else if (PDG==12) {//nu_e
                                    setMomentum (*m_genMetPlus, *daughter) ;}
                            else if (PDG==-13) {//mu+
                                    setMomentum (*m_genLepPlus, *daughter) ;
                                    m_LepPlusFlavour = 13 ;}
                            else if (PDG==14) {//nu_mu
                                   setMomentum (*m_genMetPlus, *daughter) ;}
                        }
                }
           */
            } // loop sulle particelle generate
           
     m_genTree->Fill () ;
}
// --------------------------------------------------------------------


void 
VBFReadEvent::beginJob (const edm::EventSetup&)
{
    m_outfile  = new TFile ("prova.root", "RECREATE");
    m_genTree = new TTree ("genTree","generatedParticles") ;
    
    m_genLepPlus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genLepMinus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genMetPlus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genMetMinus = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genqTagF = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_genqTagB = new TLorentzVector (0.0,0.0,0.0,0.0) ;
    m_LepPlusFlavour = 0 ;
    m_LepMinusFlavour = 0 ;
    
    m_genTree->Branch ("LepPlusFlavour", &m_LepPlusFlavour, "m_LepPlusFlavour/I");
    m_genTree->Branch ("LepMinusFlavour", &m_LepMinusFlavour, "m_LepMinusFlavour/I");
    m_genTree->Branch ("genLepPlus","TLorentzVector",&m_genLepPlus,6400,99) ;
    m_genTree->Branch ("genLepMinus","TLorentzVector",&m_genLepMinus,6400,99) ;
    m_genTree->Branch ("genMetPlus","TLorentzVector",&m_genMetPlus,6400,99) ;
    m_genTree->Branch ("genMetMinus","TLorentzVector",&m_genMetMinus,6400,99) ;
    m_genTree->Branch ("genqTagF","TLorentzVector",&m_genqTagF,6400,99) ;
    m_genTree->Branch ("genqTagB","TLorentzVector",&m_genqTagB,6400,99) ;    
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

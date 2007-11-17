// $Id: VBFReadEvent.cc,v 1.5 2007/11/17 16:14:24 tancini Exp $

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

{
   //now do what ever initialization is needed
}


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
    

  //PG check the result of the electron ID on a given ref
//  reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr ;
//  electronIDAssocItr = electronIDTightAssocHandle->find (ref) ;
//  if (electronIDAssocItr == electronIDAssocHandle->end ()) continue ;
//  const reco::ElectronIDRef& electronIDref = electronIDAssocItr->val ;
//  bool cutBasedID = electronIDref->cutBasedDecision () ;
 
    const HepMC::GenEvent * Evt = evtMC->GetEvent();
    std::cout << "process id ------>"<< Evt->signal_process_id() << std::endl;
    /*
    std::vector<const reco::Candidate*>::const_iterator iGenPart;
    for (iGenPart = mcCollection_->begin(); 
           iGenPart != mcCollection_->end(); 
           iGenPart ++){    
        
        if ((*iGenPart)->numberOfMothers() > 0 ) 
            {
            if ( (*iGenPart)->mother()->status()==3 &&  (*iGenPart)->mother()->pdgId()==-24) //W-
                {
                if ( (*iGenPart)->status()==1 && abs((*iGenPart)->pdgId())==11) //e-
                {
                    std::cout << "px particle = " << (*iGenPart)->px () << std::endl ;
                    (*iGenPart); 
                }
            }
        }
    }
    */
    
    
    for (CandidateCollection::const_iterator p = genParticles->begin(); 
        p != genParticles->end(); 
        ++ p) 
        {
        int mumPDG = p -> pdgId();
        int mumSTATUS = p->status() ;    
            if (mumPDG == -24 &&  mumSTATUS ==3) 
                {
                    std::cout << " ecco un W con mumPDG="<< mumPDG << " con # figlie " << p->numberOfDaughters() << std::endl ; 
                    
                    for( size_t i = 0; i < p->numberOfDaughters(); ++ i ) 
                        {
                        const Candidate * daughter = p->daughter( i );
                        int PDG = daughter-> pdgId() ;    
                        if (PDG==11) 
                            {
                                std::cout << "ecco un elettrone"<< std::endl ; 
                                m_genLepMinus->SetPx (daughter->px());
                                m_genLepMinus->SetPy (daughter->py());
                                m_genLepMinus->SetPz (daughter->pz());
                                m_genLepMinus->SetE (daughter->energy());
                                }
                         else if (PDG==-12) 
                                {
                                std::cout << "ecco un neutrino"<< std::endl ; 
                                m_genMetMinus->SetPx (daughter->px());
                                m_genMetMinus->SetPy (daughter->py());
                                m_genMetMinus->SetPz (daughter->pz());
                                m_genMetMinus->SetE (daughter->energy());
                              }
                        }
                }
            }
            
            //11 ele-
            //12 nu_ele
            //13 mu-
            //14 nu_mu
            //1-6 quarks
            //W+ 24
            //W- -24
            //h 25

     m_genTree->Fill () ;
}
// --------------------------------------------------------------------


void 
VBFReadEvent::beginJob (const edm::EventSetup&)
{
    m_outfile  = new TFile ("prova.root", "RECREATE");
    m_genTree = new TTree("genTree","generatedParticles") ;
    std::cout << "files ok" << std::endl ;
    
    m_genLepPlus = new TLorentzVector (0.,0.,0.,0.) ;
    m_genLepMinus = new TLorentzVector (0.,0.,0.,0.) ;
    m_genMetPlus = new TLorentzVector (0.,0.,0.,0.) ;
    m_genMetMinus = new TLorentzVector (0.,0.,0.,0.) ;
    m_genqTagF = new TLorentzVector (0.,0.,0.,0.) ;
    m_genqTagF = new TLorentzVector (0.,0.,0.,0.) ;
    
    m_genTree->Branch ("genLepPlus","TLorentzVector",&m_genLepPlus) ;
    m_genTree->Branch ("genLepMinus","TLorentzVector",&m_genLepMinus) ;
    m_genTree->Branch ("genMetPlus","TLorentzVector",&m_genMetPlus) ;
    m_genTree->Branch ("genMetMinus","TLorentzVector",&m_genMetMinus) ;
    m_genTree->Branch ("genqTagF","TLorentzVector",&m_genqTagF) ;
    m_genTree->Branch ("genqTagB","TLorentzVector",&m_genqTagB) ;
    std::cout << "beginned job" << std::endl ;
    
}


// --------------------------------------------------------------------


void 
VBFReadEvent::endJob () 
{
    m_genTree->Write () ;
}


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFFirstFilterTreeProducer.h"

#define PI 3.14159265



VBFFirstFilterTreeProducer::VBFFirstFilterTreeProducer(const edm::ParameterSet& conf)
{
 verbosity_       = conf.getParameter<bool>         ("verbosity");
 verbosity_mc_    = conf.getParameter<bool>         ("verbosity_mc");
 
 srcElectrons_        = conf.getParameter<edm::InputTag>("srcElectrons");
 srcMuons_            = conf.getParameter<edm::InputTag>("srcMuons");
 srcJets_             = conf.getParameter<edm::InputTag>("srcJets");
   
 eleTkIso_ = conf.getParameter<edm::InputTag>("eleTkIso");
 eleEcalIso_ = conf.getParameter<edm::InputTag>("eleEcalIso");
 eleHcalIso_ = conf.getParameter<edm::InputTag>("eleHcalIso");
 
}  
  


VBFFirstFilterTreeProducer::~VBFFirstFilterTreeProducer()
{
}



void VBFFirstFilterTreeProducer::beginJob(edm::EventSetup const&iSetup)
{
 
 edm::Service<TFileService> fs;

 
 if(verbosity_) 
  std::cerr << "*** TreeProducer beginjob:: Tree created ***" << std::endl;
  
 //---- Tree creation ----
 outTree_ = fs->make<TTree>("outTree","VBFH2l2nu Analyzer Tree");

 if(verbosity_) 
    std::cerr << "*** TreeProducer beginjob:: begin ***" << std::endl;
  
  
 //---- define tree branches ----
 DefineBranches();
  std::cout << ">>> Tree branches defined" << std::endl;
  

 if(verbosity_) 
    std::cerr << "*** TreeProducer beginjob:: end ***" << std::endl;
 
 
 //---- create vectors ----
 if(verbosity_) 
  std::cerr << "*** Vectors creation beginjob:: begin ***" << std::endl;

 std::cout << ">>> Vectors creation" << std::endl;
  
 mcMu_px_ = new std::vector<double>;
 mcMu_py_ = new std::vector<double>;
 mcMu_pz_ = new std::vector<double>;
 mcMu_e_ = new std::vector<double>;
 mcMu_eta_ = new std::vector<double>;
 mcMu_phi_ = new std::vector<double>;
 mcMu_pT_ = new std::vector<double>;
 mcMu_charge_ = new std::vector<int>;
 
 mcEle_px_ = new std::vector<double>;
 mcEle_py_ = new std::vector<double>;
 mcEle_pz_ = new std::vector<double>;
 mcEle_e_ = new std::vector<double>;
 mcEle_eta_ = new std::vector<double>;
 mcEle_phi_ = new std::vector<double>;
 mcEle_pT_ = new std::vector<double>;
 mcEle_charge_ = new std::vector<int>;

 recoMu_px_ = new std::vector<double>;
 recoMu_py_ = new std::vector<double>;
 recoMu_pz_ = new std::vector<double>;
 recoMu_e_ = new std::vector<double>;
 recoMu_eta_ = new std::vector<double>;
 recoMu_phi_ = new std::vector<double>;
 recoMu_pT_ = new std::vector<double>;
 recoMu_charge_ = new std::vector<int>;
 recoMu_isolationR03_sumPt = new std::vector<double>;
 recoMu_isolationR03_emEt = new std::vector<double>;
 recoMu_isolationR03_hadEt = new std::vector<double>;
 recoMu_isolationR05_sumPt = new std::vector<double>;
 recoMu_isolationR05_emEt = new std::vector<double>;
 recoMu_isolationR05_hadEt = new std::vector<double>;

  
 recoEle_px_ = new std::vector<double>;
 recoEle_py_ = new std::vector<double>;
 recoEle_pz_ = new std::vector<double>;
 recoEle_e_ = new std::vector<double>;
 recoEle_eta_ = new std::vector<double>;
 recoEle_phi_ = new std::vector<double>;
 recoEle_pT_ = new std::vector<double>;
 recoEle_charge_ = new std::vector<int>;
 recoEle_IsoTk_ = new std::vector<double>;
 recoEle_IsoEcal_ = new std::vector<double>;
 recoEle_IsoHcal_ = new std::vector<double>;
 recoEle_eId_ = new std::vector<double>;
 recoEle_eIdLikelihood_ = new std::vector<double>;
 
 recoJet_px_ = new std::vector<double>;
 recoJet_py_ = new std::vector<double>;
 recoJet_pz_ = new std::vector<double>;
 recoJet_e_ = new std::vector<double>;
 recoJet_eta_ = new std::vector<double>;
 recoJet_phi_ = new std::vector<double>;
 recoJet_pT_ = new std::vector<double>;
 recoJet_et_ = new std::vector<double>;
 
 if(verbosity_) 
  std::cerr << "*** Vectors creation beginjob:: end ***" << std::endl;
 
}     



void VBFFirstFilterTreeProducer::endJob()
{
  if(verbosity_) 
    std::cerr << "*** TreeProducer endjob ***" << std::endl;
}



void VBFFirstFilterTreeProducer::analyze(const edm::Event& e, const edm::EventSetup& iSetup)
{
  
  if(nAnalyzed_%100 == 0) 
    {
      std::cout << ">>> Analyze event number: " << nAnalyzed_ << std::endl;
    }
  
    flagVBFH_ = 0;
    flagVBFHWW_ = 0;
   
    mcMu_px_ ->clear();
    mcMu_py_ ->clear();
    mcMu_pz_ ->clear();
    mcMu_e_ ->clear();
    mcMu_eta_ ->clear();
    mcMu_phi_ ->clear();
    mcMu_pT_ ->clear();
    mcMu_charge_ ->clear();
 
    mcEle_px_ ->clear();
    mcEle_py_ ->clear();
    mcEle_pz_ ->clear();
    mcEle_e_ ->clear();
    mcEle_eta_ ->clear();
    mcEle_phi_ ->clear();
    mcEle_pT_ ->clear();
    mcEle_charge_ ->clear();

    recoMu_px_ ->clear();
    recoMu_py_ ->clear();
    recoMu_pz_ ->clear();
    recoMu_e_ ->clear();
    recoMu_eta_ ->clear();
    recoMu_phi_ ->clear();
    recoMu_pT_ ->clear();
    recoMu_charge_ ->clear();
    recoMu_isolationR03_sumPt ->clear();
    recoMu_isolationR03_emEt ->clear();
    recoMu_isolationR03_hadEt ->clear();
    recoMu_isolationR05_sumPt ->clear();
    recoMu_isolationR05_emEt ->clear();
    recoMu_isolationR05_hadEt ->clear();

  
    recoEle_px_ ->clear();
    recoEle_py_ ->clear();
    recoEle_pz_ ->clear();
    recoEle_e_ ->clear();
    recoEle_eta_ ->clear();
    recoEle_phi_ ->clear();
    recoEle_pT_ ->clear();
    recoEle_charge_ ->clear();
    recoEle_IsoTk_ ->clear();
    recoEle_IsoEcal_ ->clear();
    recoEle_IsoHcal_ ->clear();
    recoEle_eId_ ->clear();
    recoEle_eIdLikelihood_ ->clear();

 
    recoJet_px_ ->clear();
    recoJet_py_ ->clear();
    recoJet_pz_ ->clear();
    recoJet_e_ ->clear();
    recoJet_eta_ ->clear();
    recoJet_phi_ ->clear();
    recoJet_pT_ ->clear();
    recoJet_et_ ->clear();
  
  
  // *** montecarlo analysis ***
      
    //---- check if VBF H is produced ----//   
    edm::Handle<edm::HepMCProduct> evtMC;
    e.getByLabel("source",evtMC);
    const HepMC::GenEvent * Evt = evtMC->GetEvent();
    int processID = Evt->signal_process_id() ;
    if (processID == 123 || processID == 124) flagVBFH_=1 ;
    else flagVBFH_=0;
// // //     'MSUB(123) = 1           !ZZ fusion to H', 
// // //     'MSUB(124) = 1           !WW fusion to H', 
  
    //---- end check if VBF H is produced ----//   
    

 
  
  // *** reco analysis ***
  
  //---- reco Electron ----
  //---- get electrons ----
  edm::Handle<reco::PixelMatchGsfElectronCollection> electronCollection;
  e.getByLabel(srcElectrons_, electronCollection);
  recoEle_n_ = electronCollection -> size();
  
  //---- for isolation ----
  std::vector<edm::Handle<edm::ValueMap<double> > > eleIsoValueMap(3);
     
  e.getByLabel( eleTkIso_ , eleIsoValueMap[0] );
  e.getByLabel( eleEcalIso_ , eleIsoValueMap[1] );
  e.getByLabel( eleHcalIso_ , eleIsoValueMap[2] );

  const edm::ValueMap<double> & eleIsoTk = * eleIsoValueMap[0] ;
  const edm::ValueMap<double> & eleIsoEcal = * eleIsoValueMap[1] ;
  const edm::ValueMap<double> & eleIsoHcal = * eleIsoValueMap[2] ;
  //---- end for isolation ----
  
  
  //--- for electron Id ----
  std::vector<edm::Handle<edm::ValueMap<float> > > eIDValueMap(2);
  e.getByLabel( "egammaIDCutsLoose" , eIDValueMap[0]);
  e.getByLabel( "egammaIDLikelihood" , eIDValueMap[1]);
  const edm::ValueMap<float> & eIdmapCuts = * eIDValueMap[0] ;
  const edm::ValueMap<float> & eIdmapLikelihood = * eIDValueMap[1] ;
  //--- end for electron Id ----
  
  
  unsigned int num = 0;
  for(reco::PixelMatchGsfElectronCollection::const_iterator electronIt = electronCollection->begin(); electronIt != electronCollection->end(); electronIt++)
    {
//      std::cerr << "   SON QUI !!! " << std::endl;
     recoEle_px_->push_back(electronIt -> px());
     recoEle_py_->push_back(electronIt -> py()); 
     recoEle_pz_->push_back(electronIt -> pz());
     recoEle_e_->push_back(electronIt -> energy());
     recoEle_eta_->push_back(electronIt -> eta()); 
     recoEle_phi_->push_back(electronIt -> phi()); 
     recoEle_pT_->push_back(electronIt -> pt());
     recoEle_charge_->push_back(electronIt -> charge());

         
     edm::Ref<reco::GsfElectronCollection> electronRef(electronCollection,num); //---- ma funziona così ???
     num++;
     
     recoEle_IsoTk_->push_back(eleIsoTk[electronRef]);
     recoEle_IsoEcal_->push_back(eleIsoEcal[electronRef]);
     recoEle_IsoHcal_->push_back(eleIsoHcal[electronRef]);

     recoEle_eId_->push_back(eIdmapCuts[electronRef]);
     recoEle_eIdLikelihood_->push_back(eIdmapLikelihood[electronRef]);
     
     }
//      std::cerr << "   ORA I MU " << std::endl;
     
  //---- reco Muon ----
     
     edm::Handle<reco::MuonCollection> muonCollection;
     e.getByLabel(srcMuons_, muonCollection);
     recoMu_n_ = muonCollection -> size();
     
     
  for(reco::MuonCollection::const_iterator muonIt = muonCollection->begin();
      muonIt != muonCollection->end(); muonIt++)
    {
     recoMu_px_->push_back(muonIt -> px());
     recoMu_py_->push_back(muonIt -> py()); 
     recoMu_pz_->push_back(muonIt -> pz());
     recoMu_e_->push_back(muonIt -> energy());
     recoMu_eta_->push_back(muonIt -> eta()); 
     recoMu_phi_->push_back(muonIt -> phi()); 
     recoMu_pT_->push_back(muonIt -> pt());
     recoMu_charge_->push_back(muonIt -> charge());

     recoMu_isolationR03_sumPt->push_back(muonIt ->isolationR03().sumPt);
     recoMu_isolationR03_emEt->push_back(muonIt ->isolationR03().emEt);
     recoMu_isolationR03_hadEt->push_back(muonIt ->isolationR03().hadEt);

     recoMu_isolationR05_sumPt->push_back(muonIt ->isolationR05().sumPt);
     recoMu_isolationR05_emEt->push_back(muonIt ->isolationR05().emEt);
     recoMu_isolationR05_hadEt->push_back(muonIt ->isolationR05().hadEt);
 
    }

//     std::cerr << "   ORA I JET " << std::endl;

  
    //---- reco Jet ----
  // reco jets
  edm::Handle<reco::CaloJetCollection> jetCollection_;
  e.getByLabel(srcJets_, jetCollection_); 
  recoJet_n_ = jetCollection_ -> size();
  
  for(reco::CaloJetCollection::const_iterator jetIt = jetCollection_->begin();
      jetIt != jetCollection_->end(); jetIt++)
    {
     recoJet_eta_->push_back(jetIt -> eta());
     recoJet_phi_->push_back(jetIt -> phi());
     recoJet_et_->push_back(jetIt -> et());
    }

    
    
    
//     std::cerr << "   ORA FILL " << std::endl;
  outTree_->Fill();
  nAnalyzed_ ++;
}



void VBFFirstFilterTreeProducer::DefineBranches()
{
  // set number of analyzed event to 0
  nAnalyzed_ = 0;

  outTree_->Branch("flagVBFH_",&flagVBFH_,"flagVBFH_/I");
  outTree_->Branch("flagVBFHWW_",&flagVBFHWW_,"flagVBFHWW_/I");
  
  outTree_->Branch("mcMu_n_",&mcMu_n_,"mcMu_n_/I");
  outTree_->Branch("mcMu_px_","std::vector<double>",&mcMu_px_);
  outTree_->Branch("mcMu_py_","std::vector<double>",&mcMu_py_);
  outTree_->Branch("mcMu_pz_","std::vector<double>",&mcMu_pz_);
  outTree_->Branch("mcMu_e_","std::vector<double>",&mcMu_e_);
  outTree_->Branch("mcMu_eta_","std::vector<double>",&mcMu_eta_);
  outTree_->Branch("mcMu_phi_","std::vector<double>",&mcMu_phi_);
  outTree_->Branch("mcMu_pT_","std::vector<double>",&mcMu_pT_);
  outTree_->Branch("mcMu_charge_","std::vector<int>",&mcMu_charge_);
  
  outTree_->Branch("mcEle_n_",&mcEle_n_,"mcEle_n_/I");
  outTree_->Branch("mcEle_px_","std::vector<double>",&mcEle_px_);
  outTree_->Branch("mcEle_py_","std::vector<double>",&mcEle_py_);
  outTree_->Branch("mcEle_pz_","std::vector<double>",&mcEle_pz_);
  outTree_->Branch("mcEle_e_","std::vector<double>",&mcEle_e_);
  outTree_->Branch("mcEle_eta_","std::vector<double>",&mcEle_eta_);
  outTree_->Branch("mcEle_phi_","std::vector<double>",&mcEle_phi_);
  outTree_->Branch("mcEle_pT_","std::vector<double>",&mcEle_pT_);
  outTree_->Branch("mcEle_charge_","std::vector<int>",&mcEle_charge_);

  outTree_->Branch("recoMu_n_",&recoMu_n_,"recoMu_n_/I");
  outTree_->Branch("recoMu_px_","std::vector<double>",&recoMu_px_);
  outTree_->Branch("recoMu_py_","std::vector<double>",&recoMu_py_);
  outTree_->Branch("recoMu_pz_","std::vector<double>",&recoMu_pz_);
  outTree_->Branch("recoMu_e_","std::vector<double>",&recoMu_e_);
  outTree_->Branch("recoMu_eta_","std::vector<double>",&recoMu_eta_);
  outTree_->Branch("recoMu_phi_","std::vector<double>",&recoMu_phi_);
  outTree_->Branch("recoMu_pT_","std::vector<double>",&recoMu_pT_);
  outTree_->Branch("recoMu_charge_","std::vector<int>",&recoMu_charge_);
  outTree_->Branch("recoMu_isolationR03_sumPt","std::vector<double>",&recoMu_isolationR03_sumPt);
  outTree_->Branch("recoMu_isolationR03_emEt","std::vector<double>",&recoMu_isolationR03_emEt);
  outTree_->Branch("recoMu_isolationR03_hadEt","std::vector<double>",&recoMu_isolationR03_hadEt);
  outTree_->Branch("recoMu_isolationR05_sumPt","std::vector<double>",&recoMu_isolationR05_sumPt);
  outTree_->Branch("recoMu_isolationR05_emEt","std::vector<double>",&recoMu_isolationR05_emEt);
  outTree_->Branch("recoMu_isolationR05_hadEt","std::vector<double>",&recoMu_isolationR05_hadEt);
  
  outTree_->Branch("recoEle_n_",&recoEle_n_,"recoEle_n_/I");
  outTree_->Branch("recoEle_px_","std::vector<double>",&recoEle_px_);
  outTree_->Branch("recoEle_py_","std::vector<double>",&recoEle_py_);
  outTree_->Branch("recoEle_pz_","std::vector<double>",&recoEle_pz_);
  outTree_->Branch("recoEle_e_","std::vector<double>",&recoEle_e_);
  outTree_->Branch("recoEle_eta_","std::vector<double>",&recoEle_eta_);
  outTree_->Branch("recoEle_phi_","std::vector<double>",&recoEle_phi_);
  outTree_->Branch("recoEle_pT_","std::vector<double>",&recoEle_pT_);
  outTree_->Branch("recoEle_charge_","std::vector<int>",&recoEle_charge_);
  outTree_->Branch("recoEle_IsoTk_","std::vector<double>",&recoEle_IsoTk_);
  outTree_->Branch("recoEle_IsoEcal_","std::vector<double>",&recoEle_IsoEcal_);
  outTree_->Branch("recoEle_IsoHcal_","std::vector<double>",&recoEle_IsoHcal_);
  outTree_->Branch("recoEle_eId_","std::vector<double>",&recoEle_eId_);
  outTree_->Branch("recoEle_eIdLikelihood_","std::vector<double>",&recoEle_eIdLikelihood_);
  
  outTree_->Branch("recoJet_n_",&recoJet_n_,"recoJet_n_/I");
  outTree_->Branch("recoJet_px_","std::vector<double>",&recoJet_px_);
  outTree_->Branch("recoJet_py_","std::vector<double>",&recoJet_py_);
  outTree_->Branch("recoJet_pz_","std::vector<double>",&recoJet_pz_);
  outTree_->Branch("recoJet_e_","std::vector<double>",&recoJet_e_);
  outTree_->Branch("recoJet_eta_","std::vector<double>",&recoJet_eta_);
  outTree_->Branch("recoJet_phi_","std::vector<double>",&recoJet_phi_);
  outTree_->Branch("recoJet_pT_","std::vector<double>",&recoJet_pT_);
  outTree_->Branch("recoJet_et_","std::vector<double>",&recoJet_et_);
  


}



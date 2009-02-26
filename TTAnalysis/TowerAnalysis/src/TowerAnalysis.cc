// -*- C++ -*-
//
// Package:    TowerAnalysis
// Class:      TowerAnalysis
// 
/**\class TowerAnalysis TowerAnalysis.cc TTAnalysis/TowerAnalysis/src/TowerAnalysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Authors:  Leonardo Sala 
//         Created:  Tue Feb 24 11:40:58 CET 2009
// $Id: TowerAnalysis.cc,v 1.2 2009/02/26 17:58:56 amartell Exp $
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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <map>
#include <string>
#include <vector>

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include "TProfile.h"
#include "TProfile2D.h"

//
// class decleration
//

class TowerAnalysis : public edm::EDAnalyzer {
public:
  explicit TowerAnalysis(const edm::ParameterSet&);
  ~TowerAnalysis();
  
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  //  TFile *histfile_;
  //  std::string outputFile_;

  edm::Service<TFileService> fs;
  std::map<std::string, TH1F*> histos;
  std::map<std::string, TH2F*> histos2d;

  edm::InputTag rechEB_;
  edm::InputTag rechEE_;

  edm::InputTag rejhEB_;
  edm::InputTag rejhEE_;

  edm::InputTag electronCollection_;
  edm::InputTag electronReducedCollection_;

  edm::InputTag mcTruthCollection_;


  ////////////////

  int  nEvent_;

  double maxPt_;
  double maxAbsEta_;
  double deltaR_;
  
  double etamin;
  double etamax;
  int nbineta;

  double phimin;
  double phimax;
  int nbinphi;


  double ptmin;
  double ptmax;
  int nbinpt;
  int nbinpteff;

  double pmin;
  double pmax;
  int nbinp;

  int nbinxyz;

  /////////////// histos

  TH2F* h_simEtavsPhi;
  TH2F* h_simXvsYinPE;
  TH2F* h_simXvsYinNE;

  TH2F* h_ele_simEtavsPhi_matched;
  TH2F* h_ele_simXvsYinPE_matched;
  TH2F* h_ele_simXvsYinNE_matched;

  TProfile2D*  h_ele_simE_matched;
  TProfile2D*  h_ele_simEinPE_matched;
  TProfile2D*  h_ele_simEinNE_matched;

  TH2F* h_eleRed_simEtavsPhi_matched;
  TH2F* h_eleRed_simXvsYinPE_matched;
  TH2F* h_eleRed_simXvsYinNE_matched;

  TProfile2D*  h_eleRed_simE_matched;
  TProfile2D*  h_eleRed_simEinPE_matched;
  TProfile2D*  h_eleRed_simEinNE_matched;

  ///////////////////////

  TH2F* h_ele_EtavsPhiEff;
  TH2F* h_ele_XvsYEffinPE;
  TH2F* h_ele_XvsYEffinNE;

  TH2F* h_eleRed_EtavsPhiEff;
  TH2F* h_eleRed_XvsYEffinPE;
  TH2F* h_eleRed_XvsYEffinNE;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TowerAnalysis::TowerAnalysis(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed

  //  histfile_ = new TFile(outputFile_.c_str(),"RECREATE");
  //  outputFile_ = iConfig.getParameter<std::string>("outputFile");
  
  rechEB_ = iConfig.getParameter<edm::InputTag>("ecalRecH_EB");
  rechEE_ = iConfig.getParameter<edm::InputTag>("ecalRecH_EE");
  
  rejhEB_ = iConfig.getParameter<edm::InputTag>("ecalRejH_EB");
  rejhEE_ = iConfig.getParameter<edm::InputTag>("ecalRejH_EE");
    
  electronCollection_ = iConfig.getParameter<edm::InputTag>("electronCollection");
  electronReducedCollection_ = iConfig.getParameter<edm::InputTag>("electronReducedCollection");

  mcTruthCollection_ = iConfig.getParameter<edm::InputTag>("mcTruthCollection");

  maxPt_ = iConfig.getParameter<double>("MaxPt");
  maxAbsEta_ = iConfig.getParameter<double>("MaxAbsEta");
  deltaR_ = iConfig.getParameter<double>("DeltaR");

  etamin = iConfig.getParameter<double>("Etamin");
  etamax = iConfig.getParameter<double>("Etamax");
  nbineta = iConfig.getParameter<int>("Nbineta");

  phimin = iConfig.getParameter<double>("Phimin");
  phimax = iConfig.getParameter<double>("Phimax");
  nbinphi = iConfig.getParameter<int>("Nbinphi");


  ptmin = iConfig.getParameter<double>("Ptmin");
  ptmax = iConfig.getParameter<double>("Ptmax");
  nbinpt = iConfig.getParameter<int>("Nbinpt");
  nbinpteff = iConfig.getParameter<int>("Nbinpteff");

  pmin = iConfig.getParameter<double>("Pmin");
  pmax = iConfig.getParameter<double>("Pmax");
  nbinp = iConfig.getParameter<int>("Nbinp");
  
  nbinxyz = iConfig.getParameter<int>("Nbinxyz");
}


TowerAnalysis::~TowerAnalysis()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
  //  histfile_->Write();
  //  histfile_->Close(); 
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TowerAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if(nEvent_ % 1000 == 0)  std::cout << ">>>>>>>>>>>>> analyzing event " << nEvent_ << std::endl;
  ++nEvent_;
  //  histfile_->cd();

  using namespace edm;

   // get the hit collection from the event:
   edm::Handle<EcalRecHitCollection> hit_collectionEB;
   iEvent.getByLabel( rechEB_, hit_collectionEB);
   if (!(hit_collectionEB.isValid()))
     {
       std::cout << "[Analysis] could not get a handle on the EcalRecHitCollection EB!" << std::endl;
       return;
     }
   
   for(EcalRecHitCollection::const_iterator it = hit_collectionEB->begin(); it != hit_collectionEB->end(); ++it) {
     EBDetId singleHit = it->detid();
     histos2d["deadTowers_etaPhi"]->Fill(singleHit.iphi(), singleHit.ieta());
     //std::cout << it->energy() << std::endl;
   }
   
   edm::Handle<EcalRecHitCollection> hit_collectionEE;
   iEvent.getByLabel( rechEE_, hit_collectionEE);
   if (!(hit_collectionEE.isValid()))
     {
       std::cout << "[Analysis] could not get a handle on the EcalRecHitCollection EE!" << std::endl;
       return;
     }
   
   for(EcalRecHitCollection::const_iterator it = hit_collectionEE->begin(); it != hit_collectionEE->end(); ++it) {
     EEDetId singleHit = it->detid();
     if( singleHit.zside() >0)
       histos2d["deadTowersEE+_etaPhi"]->Fill(singleHit.ix(), singleHit.iy());
     else
       histos2d["deadTowersEE-_etaPhi"]->Fill(singleHit.ix(), singleHit.iy());
     //std::cout << it->energy() << std::endl;
   }
   
   // get electrons 
   edm::Handle<reco::GsfElectronCollection> gsfElectrons;
   iEvent.getByLabel(electronCollection_,gsfElectrons);
   edm::LogInfo("")<<"\n\n =================> Treating event "<<iEvent.id()<<" Number of electrons "<<gsfElectrons.product()->size();

   edm::Handle<reco::GsfElectronCollection> gsfElectronsRed;
   iEvent.getByLabel(electronReducedCollection_,gsfElectronsRed);
   edm::LogInfo("")<<"\n\n =================> Treating event "<<iEvent.id()<<" Number of electrons "<<gsfElectronsRed.product()->size();
   

   //get mc events
   edm::Handle<edm::HepMCProduct> hepMC;
   iEvent.getByLabel(mcTruthCollection_,hepMC);

   
   // association mc-reco          

   HepMC::GenParticle* genPc=0;
   const HepMC::GenEvent *myGenEvent = hepMC->GetEvent();
   //   int mcNum=0, gamNum=0, eleNum=0;
   HepMC::FourVector pAssSim;
   
   for ( HepMC::GenEvent::particle_const_iterator mcIter=myGenEvent->particles_begin(); mcIter != myGenEvent->particles_end(); mcIter++ ) {
     // number of mc particles
     // counts photons 
     //  if ((*mcIter)->pdg_id() == 22 ){ gamNum++; }

     // select electrons      
     if ( (*mcIter)->pdg_id() == 11 || (*mcIter)->pdg_id() == -11 ){
       
       // single primary electrons or electrons from Zs or Ws   
       HepMC::GenParticle* mother = 0;
       if ( (*mcIter)->production_vertex() ) {
	 if ( (*mcIter)->production_vertex()->particles_begin(HepMC::parents) !=
	      (*mcIter)->production_vertex()->particles_end(HepMC::parents))
	   mother = *((*mcIter)->production_vertex()->particles_begin(HepMC::parents));
       }
       if ( ((mother == 0) || ((mother != 0) && (mother->pdg_id() == 23))
	     || ((mother != 0) && (mother->pdg_id() == 32))
	     || ((mother != 0) && (fabs(mother->pdg_id()) == 24)))) {
	 
	 genPc=(*mcIter);
	 pAssSim = genPc->momentum();
	 
	 //	 if (pAssSim.perp()> maxPt_ || fabs(pAssSim.eta())> maxAbsEta_) continue;
	 

	 // looking for the best matching gsf electron 
	 bool okGsfFound = false;
	 double gsfOkRatio = 999999.;
	 reco::SuperClusterRef 	SCok;

	 // find best matched electron

	 reco::GsfElectron bestGsfElectron;
	 for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin();
	      gsfIter!=gsfElectrons->end(); gsfIter++){
	   
	   double deltaR = sqrt(pow((gsfIter->eta()-pAssSim.eta()),2) + pow((gsfIter->phi()-pAssSim.phi()),2));
	   if ( deltaR < deltaR_ ){
	     if ( (genPc->pdg_id() == 11) && (gsfIter->charge() < 0.) || (genPc->pdg_id() == -11) &&
		  (gsfIter->charge() > 0.) ){
	       double tmpGsfRatio = gsfIter->p()/pAssSim.t();
	       if ( fabs(tmpGsfRatio-1) < fabs(gsfOkRatio-1) ) {
		 gsfOkRatio = tmpGsfRatio;
		 bestGsfElectron=*gsfIter;
		 okGsfFound = true;
		 SCok = bestGsfElectron.superCluster();
	       }
	     }
	   }
	 } // loop over rec ele to look for the best one
	 


	 //////////////////////////////////////////////   
	 // looking for the best matching gsf electron
 
	 bool okGsfRedFound = false;
	 double gsfRedOkRatio = 999999.;
	 reco::SuperClusterRef 	SCRedok;
	 // find best matched electron 

	 reco::GsfElectron bestGsfElectronRed;
	 for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectronsRed->begin();
	      gsfIter!=gsfElectronsRed->end(); gsfIter++){
	   
	   double deltaR = sqrt(pow((gsfIter->eta()-pAssSim.eta()),2) + pow((gsfIter->phi()-pAssSim.phi()),2));
	   if ( deltaR < deltaR_ ){
	     if ( (genPc->pdg_id() == 11) && (gsfIter->charge() < 0.) || (genPc->pdg_id() == -11) &&
		  (gsfIter->charge() > 0.) ){
	       double tmpGsfRatio = gsfIter->p()/pAssSim.t();
	       if ( fabs(tmpGsfRatio-1) < fabs(gsfRedOkRatio-1) ) {
		 gsfRedOkRatio = tmpGsfRatio;
		 bestGsfElectronRed = *gsfIter;
		 okGsfRedFound = true;
		 SCRedok = bestGsfElectronRed.superCluster();
	       }
	     }
	   }
	 } // loop over rec ele to look for the best one  

	 ///////////////////////////////////////

	 float etaSC = SCok->eta();
	 float phiSC = SCok->phi();
	 float xSC = SCok->position().x();
	 float ySC = SCok->position().y();
	 float zSC = SCok->position().z();

	 float etaSCRed = SCRedok->eta();
	 float phiSCRed = SCRedok->phi();
	 float xSCRed = SCRedok->position().x();
	 float ySCRed = SCRedok->position().y();
	 float zSCRed = SCRedok->position().z();


	 if(etaSC < 1.5 && etaSC > -1.5) h_simEtavsPhi -> Fill(phiSC, etaSC) ;
	 if(etaSC > 1.5) h_simXvsYinPE -> Fill(xSC, ySC) ;
	 if(etaSC < -1.5) h_simXvsYinNE -> Fill(xSC, ySC) ;


	 /////////////////////////////////////  

	 // analysis when the mc track is found 

	 if (okGsfFound){
	   // generated distributions for matched electrons
	   
	   if(etaSC < 1.5 && etaSC > -1.5) h_ele_simEtavsPhi_matched -> Fill(phiSC, etaSC);
	   if(etaSC > 1.5) h_ele_simXvsYinPE_matched -> Fill(xSC, ySC);
	   if(etaSC < -1.5) h_ele_simXvsYinNE_matched -> Fill(xSC, ySC);

	   if(etaSC < 1.5 && etaSC > -1.5) h_ele_simE_matched -> Fill(phiSC, etaSC, (pAssSim.t() - bestGsfElectron.energy()) / pAssSim.t());	 
	   if(etaSC > 1.5) h_ele_simEinPE_matched -> Fill(xSC, ySC,  (pAssSim.t() - bestGsfElectron.energy()) / pAssSim.t());	 
	    if(etaSC < -1.5)h_ele_simEinNE_matched -> Fill(xSC, ySC,  (pAssSim.t() - bestGsfElectron.energy()) / pAssSim.t());	 
	 } // gsf electron found  


	 if(okGsfRedFound){
	   if(etaSC < 1.5 && etaSC > -1.5) h_eleRed_simEtavsPhi_matched -> Fill(phiSC, etaSC);
	   if(etaSC > 1.5) h_eleRed_simXvsYinPE_matched -> Fill(xSC, ySC);
	   if(etaSC < -1.5) h_eleRed_simXvsYinNE_matched -> Fill(xSC, ySC);

	   if(etaSC < 1.5 && etaSC > -1.5) h_eleRed_simE_matched -> Fill(phiSC, etaSC, (pAssSim.t() - bestGsfElectronRed.energy()) / pAssSim.t());	 
	   if(etaSC > 1.5) h_eleRed_simEinPE_matched -> Fill(xSC, ySC, (pAssSim.t() - bestGsfElectronRed.energy()) / pAssSim.t());	 
	   if(etaSC < -1.5) h_eleRed_simEinNE_matched -> Fill(xSC, ySC, (pAssSim.t() - bestGsfElectronRed.energy()) / pAssSim.t());	 

	 }
	 
       } // mc particle found

     }
   } // loop over mc particle
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
TowerAnalysis::beginJob(const edm::EventSetup&)
{
  nEvent_ = 0;
  //  histfile_->cd();

  histos2d["deadTowers_etaPhi"] = fs->make<TH2F>("deadTowers_etaPhi","dead towers (i#phi,i#eta);i#phi;i#eta",365,0,365,200,-100,100);
  
  histos2d["deadTowersEE+_etaPhi"] = fs->make<TH2F>("deadTowersEE+_etaPhi","dead towers (i#phi,i#eta);i#phi;i#eta",100,0,100,100,0,100);
  histos2d["deadTowersEE-_etaPhi"] = fs->make<TH2F>("deadTowersEE-_etaPhi","dead towers (i#phi,i#eta);i#phi;i#eta",100,0,100,100,0,100);
  
  // mc
 h_simEtavsPhi = fs->make<TH2F>("h_simEtavsPhi", "h_simEtavsPhi", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
 h_simXvsYinPE = fs->make<TH2F>("h_simXvsYinPE", "h_simXvsYinPE", 100, 0., 100., 100, 0., 100.);
 h_simXvsYinNE = fs->make<TH2F>("h_simXvsYinNE", "h_simXvsYinNE", 100, 0., 100., 100, 0., 100.);


 h_ele_simEtavsPhi_matched = fs->make<TH2F>("h_ele_simEtavsPhi_matched", "h_ele_simEtavsPhi_matched", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
 h_ele_simXvsYinPE_matched = fs->make<TH2F>("h_ele_simXvsYinPE_matched", "h_ele_simXvsYinPE_matched", 100, 0., 100., 100, 0., 100.); 
 h_ele_simXvsYinNE_matched = fs->make<TH2F>("h_ele_simXvsYinNE_matched", "h_ele_simXvsYinNE_matched", 100, 0., 100., 100, 0., 100.);

 h_ele_simE_matched = fs->make<TProfile2D>("h_ele_simE_matched", "h_ele_simE_matched", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
 h_ele_simEinPE_matched = fs->make<TProfile2D>("h_ele_simEinPE_matched", "h_ele_simEinPE_matched", 100, 0., 100., 100, 0., 100.);
 h_ele_simEinNE_matched = fs->make<TProfile2D>("h_ele_simEinNE_matched", "h_ele_simEinNE_matched", 100, 0., 100., 100, 0., 100.);

 h_eleRed_simEtavsPhi_matched = fs->make<TH2F>("h_eleRed_simEtavsPhi_matched", "h_eleRed_simEtavsPhi_matched", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
 h_eleRed_simXvsYinPE_matched = fs->make<TH2F>("h_eleRed_simXvsYinPE_matched", "h_eleRed_simXvsYinPE_matched", 100, 0., 100., 100, 0., 100.); 
 h_eleRed_simXvsYinNE_matched = fs->make<TH2F>("h_eleRed_simXvsYinNE_matched", "h_eleRed_simXvsYinNE_matched", 100, 0., 100., 100, 0., 100.);

 h_eleRed_simE_matched = fs->make<TProfile2D>("h_eleRed_simE_matched", "h_eleRed_simE_matched", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
 h_eleRed_simEinPE_matched = fs->make<TProfile2D>("h_eleRed_simEinPE_matched", "h_eleRed_simEinPE_matched", 100, 0., 100., 100, 0., 100.);
 h_eleRed_simEinNE_matched = fs->make<TProfile2D>("h_eleRed_simEinNE_matched", "h_eleRed_simEinNE_matched", 100, 0., 100., 100, 0., 100.);


 h_ele_EtavsPhiEff = fs->make<TH2F>("h_ele_EtavsPhiEff", "h_ele_EtavsPhiEff", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
 h_ele_XvsYEffinPE = fs->make<TH2F>("h_ele_XvsYEffinPE", "h_ele_XvsYEffinPE", 100, 0., 100., 100, 0., 100.);
 h_ele_XvsYEffinNE = fs->make<TH2F>("h_ele_XvsYEffinNE", "h_ele_XvsYEffinNE", 100, 0., 100., 100, 0., 100.);

 h_eleRed_EtavsPhiEff = fs->make<TH2F>("h_eleRed_EtavsPhiEff", "h_eleRed_EtavsPhiEff", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
 h_eleRed_XvsYEffinPE = fs->make<TH2F>("h_eleRed_XvsYEffinPE", "h_eleRed_XvsYEffinPE", 100, 0., 100., 100, 0., 100.);
 h_eleRed_XvsYEffinNE = fs->make<TH2F>("h_eleRed_XvsYEffinNE", "h_eleRed_XvsYEffinNE", 100, 0., 100., 100, 0., 100.);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TowerAnalysis::endJob() {

  //  histfile_->cd();
  std::cout << "efficiency calculation " << std::endl;

  h_ele_EtavsPhiEff->Reset();
  h_ele_EtavsPhiEff->Divide(h_ele_simEtavsPhi_matched,h_simEtavsPhi,1,1);
  h_ele_EtavsPhiEff->Print();

  h_ele_XvsYEffinPE->Reset();
  h_ele_XvsYEffinPE->Divide(h_ele_simXvsYinPE_matched,h_simXvsYinPE,1,1);
  h_ele_XvsYEffinPE->Print();

  h_ele_XvsYEffinNE->Reset();
  h_ele_XvsYEffinNE->Divide(h_ele_simXvsYinNE_matched,h_simXvsYinNE,1,1);
  h_ele_XvsYEffinNE->Print();


  h_eleRed_EtavsPhiEff->Reset();
  h_eleRed_EtavsPhiEff->Divide(h_eleRed_simEtavsPhi_matched,h_simEtavsPhi,1,1);
  h_eleRed_EtavsPhiEff->Print();

  h_eleRed_XvsYEffinPE->Reset();
  h_eleRed_XvsYEffinPE->Divide(h_eleRed_simXvsYinPE_matched,h_simXvsYinPE,1,1);
  h_eleRed_XvsYEffinPE->Print();

  h_eleRed_XvsYEffinNE->Reset();
  h_eleRed_XvsYEffinNE->Divide(h_eleRed_simXvsYinNE_matched,h_simXvsYinNE,1,1);
  h_eleRed_XvsYEffinNE->Print();

}

//define this as a plug-in
DEFINE_FWK_MODULE(TowerAnalysis);

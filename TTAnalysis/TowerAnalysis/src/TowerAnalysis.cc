// -*- C++ -*-
//
// Package:    TowerAnalysis
// Class:      TowerAnalysis
// 
/**\class TowerAnalysis TowerAnalysis.cc Analysis/TowerAnalysis/src/TowerAnalysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Leonardo Sala
//         Created:  Fri Jan 16 11:52:03 CET 2009
// $Id: TowerAnalysis.cc,v 1.12 2009/03/05 13:29:47 leo Exp $
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

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"


#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include <Math/GenVector/VectorUtil.h>
#include "DataFormats/GeometryVector/interface/VectorUtil.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"
#include <CLHEP/Vector/LorentzVector.h>

#include "FWCore/Framework/interface/ESHandle.h"

#include "TH1D.h"
#include <map>

#include "DataFormats/Common/interface/View.h"
#include <string>
#include <vector>

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <cmath>
#include <TH1F.h>
//#include <Math/VectorUtil.h>
#include <TTree.h>
#include <TH1.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TH2F.h>


#define MAXJETS 200
#define TIVMAX 700
#define MAXMC 200
#define MAXELE 1000

using namespace std;
using namespace reco;
using namespace edm;

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
  float delta_phi(float phi1, float phi2);

      // ----------member data ---------------------------
  
  std::vector<edm::InputTag> jetLabel_;
  edm::InputTag genJetLabel_;
  std::vector<edm::InputTag> metLabel_;
  InputTag genMetLabel_;

  std::vector<edm::InputTag> eleLabel_;
  edm::InputTag mcLabel_;

  float Jet_threshold_;

  //////General
  TTree * mtree;
  int DEBUG;
  edm::Service<TFileService> fs;
  float deltaR_;

  ////PROCESS QUANTITIES
  int Event_number;
  int Run_number;

  /////JET 
  vector<string> JET_VAR; //Name of Jet variables
  map<string,float*> varFloatArr_Jets; //map of jet variables

  ///MET 
  vector<string> MET_CORR;  //Name of met corrections
  vector<string> MET_VAR; //Name of  met variables
  map<string,float> varFloatArr_Met; //map of met variables

  ///MC
  vector<string> MC_VAR;
  map<string,float*> varFloatArr_MC;

  //Generic Int arrays
  map<string,int*> varIntArr_MC;
  map<string,int> varInt;

  //Muons
  vector<string> ELE_VAR; //Name of Jet variables
  map<string,float*> varFloatArr_Ele; //map of jet variables

  //Histos
  map<string, TH1F*> histos;
  map<string, TH2F*> histos2D;
  map<string, TProfile *> profiles;
  map<string, TProfile2D *> histos2P;

  //Labels
  string sLABEL[2];

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
  //edm::Service<TFileService> fs;

  jetLabel_ = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("jetLabel");
  genJetLabel_ = iConfig.getUntrackedParameter<edm::InputTag>("genJetLabel");

  metLabel_ = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("metLabel");
  genMetLabel_ = iConfig.getUntrackedParameter<edm::InputTag>("genMetLabel");

  eleLabel_ = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("eleLabel");
  mcLabel_ = iConfig.getUntrackedParameter<edm::InputTag>("mcLabel");

  deltaR_ = iConfig.getParameter<double>("DeltaR");
 

  //---- Tree creation ----
  mtree = fs->make<TTree>("Analysis","Analysis");
  

  DEBUG = iConfig.getUntrackedParameter<int>("DEBUG");
  
  //JET VARIABLES
  JET_VAR.push_back("px");  JET_VAR.push_back("py");  JET_VAR.push_back("pz");  JET_VAR.push_back("pt");
  JET_VAR.push_back("E"); JET_VAR.push_back("EmFrac"); JET_VAR.push_back("HadFrac");
  JET_VAR.push_back("eta"); JET_VAR.push_back("phi");
  JET_VAR.push_back("Matching");

  //this creates and fills the jet var map
  varInt["Jets_n"] = 0;
  for(vector<string>::iterator var = JET_VAR.begin(); var!=JET_VAR.end(); ++var)
    varFloatArr_Jets["Jets_"+*var] = new float[MAXJETS];
  
  varInt["killedJets_n"] = 0;
  for(vector<string>::iterator var = JET_VAR.begin(); var!=JET_VAR.end(); ++var)
    varFloatArr_Jets["killedJets_"+*var] = new float[MAXJETS];

  varInt["genJets_n"] = 0;
  for(vector<string>::iterator var = JET_VAR.begin(); var!=JET_VAR.end(); ++var)
    varFloatArr_Jets["Jets_GEN_"+*var] = new float[MAXJETS];


  /////Insert here the Met corrections wanted
  /// see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATLayer1#MET
  
  MET_VAR.push_back("pt"); MET_VAR.push_back("phi");MET_VAR.push_back("py");MET_VAR.push_back("px");MET_VAR.push_back("sumEt");

  for(vector<string>::iterator var = MET_VAR.begin(); var!=MET_VAR.end(); ++var){
	varFloatArr_Met["Met_"+*var] = -1000;
	varFloatArr_Met["killedMet_"+*var] = -1000;
	varFloatArr_Met["Met_GEN_"+*var] = -1000;
  }

  ////Electrons
  ELE_VAR.push_back("E"); ELE_VAR.push_back("pt"); ELE_VAR.push_back("phi");  ELE_VAR.push_back("eta");  ELE_VAR.push_back("px"); ELE_VAR.push_back("py");
  ELE_VAR.push_back("outerPhi");  ELE_VAR.push_back("outerEta");
  ELE_VAR.push_back("ScPhi");  ELE_VAR.push_back("ScEta");
  ELE_VAR.push_back("pz"); ELE_VAR.push_back("charge");
  

  varInt["Ele_n"] = 0;
  varInt["killedEle_n"] = 0;

  for(vector<string>::iterator var = ELE_VAR.begin(); var!=ELE_VAR.end(); ++var){
    varFloatArr_Ele["Ele_"+*var] = new float[MAXELE];
    varFloatArr_Ele["killedEle_"+*var] = new float[MAXELE];
  }
  //MC
  MC_VAR.push_back("E"); MC_VAR.push_back("px");MC_VAR.push_back("py");MC_VAR.push_back("pz");MC_VAR.push_back("pt");
  MC_VAR.push_back("eta"); MC_VAR.push_back("phi");
  
  varIntArr_MC["MC_id"] = new int[MAXMC];
  varIntArr_MC["MC_status"] = new int[MAXMC];
  varIntArr_MC["MC_matches"] = new int[MAXMC];
  varIntArr_MC["killedMC_matches"] = new int[MAXMC];
  varInt["MC_n"] = 0;

  for(vector<string>::iterator var = MC_VAR.begin(); var!=MC_VAR.end(); ++var){
    varFloatArr_MC["MC_"+*var] = new float[MAXMC];
  }


  sLABEL[0] = "";
  sLABEL[1] = "killed";
}


TowerAnalysis::~TowerAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TowerAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Event_number =  iEvent.id().event();
   Run_number = iEvent.id().run();

   if (DEBUG==1) std::cout << "------- event " << Event_number << " ---------"<< std::endl;

   ///////// JETS
   Handle<GenJetCollection> genJets;
   iEvent.getByLabel(genJetLabel_,genJets);

   std::vector<edm::InputTag>::const_iterator i;
   int counter  = 0;
   string JET_LABEL = "";
   for (i = jetLabel_.begin(); i!=jetLabel_.end(); i++) {
     
     Handle<CaloJetCollection> jets;
     iEvent.getByLabel(*i,jets);
     
     if(counter==1) JET_LABEL = "killed"; 
     
     varInt[JET_LABEL+"Jets_n"]=0;
     
     
     int totJets = 0;
     for (CaloJetCollection::const_iterator jet = jets->begin(); jet != jets->end(); jet++) 
       {
	 float jetEta = jet->eta();
	 float jetPhi = jet->phi();
	 float maxR = 1.;
	 int matchedId = -1;
	 int genJets_n = 0;

	 //GenJets
	 for (GenJetCollection::const_iterator genJet = genJets->begin(); genJet != genJets->end(); genJet++)
	   {
	     if(totJets==0){
	       varFloatArr_Jets["Jets_GEN_pt"][genJets_n] = genJet->pt();
	       varFloatArr_Jets["Jets_GEN_px"][genJets_n] = genJet->px();
	       varFloatArr_Jets["Jets_GEN_py"][genJets_n] = genJet->py();
	       varFloatArr_Jets["Jets_GEN_pz"][genJets_n] = genJet->pz();
	       varFloatArr_Jets["Jets_GEN_E"][genJets_n] = genJet->energy();
	       
	       varFloatArr_Jets["Jets_GEN_phi"][genJets_n] = genJet->phi();
	       varFloatArr_Jets["Jets_GEN_eta"][genJets_n] = genJet->eta();
	       
	       varFloatArr_Jets["Jets_GEN_EmFrac"][genJets_n] = genJet->emEnergy()/genJet->energy();
	       varFloatArr_Jets["Jets_GEN_HadFrac"][genJets_n] = genJet->hadEnergy()/genJet->energy();
	     }
	     
	     float dPhi = delta_phi(jetPhi, genJet->phi());
	     float dEta = jetEta - genJet->eta();
	     float dR = sqrt(dPhi*dPhi + dEta*dEta);
	     if( dR < maxR )  { matchedId = genJets_n; maxR=dR; }
	     
	     genJets_n++;
	   }

	 if(totJets==0) varInt["genJets_n"] = genJets_n;
	 
	 int njets = varInt[JET_LABEL+"Jets_n"];
	 if(matchedId != -1 ){
	   varFloatArr_Jets[JET_LABEL+"Jets_Matching"][njets] = matchedId;
	   float res = ( varFloatArr_Jets[JET_LABEL+"Jets_pt"][njets] - varFloatArr_Jets["Jets_GEN_pt"][matchedId] ) / varFloatArr_Jets["Jets_GEN_pt"][matchedId];
	   histos[JET_LABEL+"Jets_EtRes"]->Fill(res);
	 }	 

	 varFloatArr_Jets[JET_LABEL+"Jets_pt"][njets] = jet->pt();
	 varFloatArr_Jets[JET_LABEL+"Jets_px"][njets] = jet->px();
	 varFloatArr_Jets[JET_LABEL+"Jets_py"][njets] = jet->py();
	 varFloatArr_Jets[JET_LABEL+"Jets_pz"][njets] = jet->pz();
	 varFloatArr_Jets[JET_LABEL+"Jets_E"][njets] = jet->energy();
	 
	 varFloatArr_Jets[JET_LABEL+"Jets_phi"][njets] = jet->phi();
	 varFloatArr_Jets[JET_LABEL+"Jets_eta"][njets] = jet->eta();
	 
	 varFloatArr_Jets[JET_LABEL+"Jets_EmFrac"][njets] = jet->emEnergyFraction();
	 varFloatArr_Jets[JET_LABEL+"Jets_HadFrac"][njets] = jet->energyFractionHadronic ();
	
	 varInt[JET_LABEL+"Jets_n"]++;
	 totJets++;
       }
     
     counter++;
   }

   counter  = 0;
   string MET_LABEL = "";

   const GenMET *genmet;
   edm::Handle<GenMETCollection> gMET;
   iEvent.getByLabel(genMetLabel_, gMET);
   const GenMETCollection* gmets = gMET.product();
   genmet = &(gmets->front());

   varFloatArr_Met["Met_GEN_pt"] = genmet->et();
   varFloatArr_Met["Met_GEN_phi"] = genmet->phi();
   varFloatArr_Met["Met_GEN_px"] = genmet->px();
   varFloatArr_Met["Met_GEN_py"] = genmet->py();
   varFloatArr_Met["Met_GEN_sumEt"] = genmet->sumEt();

   for (std::vector<edm::InputTag>::const_iterator i = metLabel_.begin(); i!=metLabel_.end(); i++) {

     if(counter==1) MET_LABEL = "killed";

     edm::Handle<reco::CaloMETCollection> pMET;
     iEvent.getByLabel(*i, pMET);
     const reco::CaloMETCollection* mets = pMET.product();
     
     for ( CaloMETCollection::const_iterator met=mets->begin(); met!=mets->end(); met++) {

       varFloatArr_Met[MET_LABEL+"Met_pt"] = met->et();
       varFloatArr_Met[MET_LABEL+"Met_phi"] = met->phi();
       varFloatArr_Met[MET_LABEL+"Met_px"] = met->px();
       varFloatArr_Met[MET_LABEL+"Met_py"] = met->py();
       varFloatArr_Met[MET_LABEL+"Met_sumEt"] = met->sumEt();
     }

     float res = ( varFloatArr_Met[MET_LABEL+"Met_pt"] - varFloatArr_Met["Met_GEN_pt"] )/varFloatArr_Met["Met_GEN_pt"] ;
     histos[MET_LABEL+"Met_EtRes"]->Fill(res);

     counter++;
   }
   

   //Electrons

   //get mc events
   edm::Handle<edm::HepMCProduct> hepMC;
   iEvent.getByLabel(mcLabel_,hepMC);
   // association mc-reco          
   HepMC::GenParticle* genPc=0;
   const HepMC::GenEvent *myGenEvent = hepMC->GetEvent();
   HepMC::FourVector pAssSim;

   vector<pair<int,int> > matches, matchesKilled; //pair of (reco, mc) matches
   int ele_MCid=0;

   varInt["MC_n"] = 0;
   for ( HepMC::GenEvent::particle_const_iterator mcIter=myGenEvent->particles_begin(); mcIter != myGenEvent->particles_end(); mcIter++ ){
     
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
	 
	 genPc= (*mcIter);
	 pAssSim = genPc->momentum();

         int MC_n = varInt["MC_n"];
	 
	 // looking for the best matching gsf electron 
	 bool okGsfFound = false;
	 double gsfOkRatio = 999999.;
	 reco::SuperClusterRef SCok;
	 
	 // find best matched electron

	 counter  = 0;
	 string LABEL = "";
	 for (std::vector<edm::InputTag>::const_iterator i = eleLabel_.begin(); i!=eleLabel_.end(); i++) {
	   // get electrons
	   edm::Handle<reco::GsfElectronCollection> electrons;
	   iEvent.getByLabel(*i,electrons);
	   
	   if(counter==1) LABEL = "killed";


	   // looking for the best matching gsf electron
	   bool okGsfFound = false;
	   double gsfOkRatio = 999999.;
	   reco::SuperClusterRef SCok;

	   reco::GsfElectron bestGsfElectron;
	   int ele_id=0;
	   int bestEle=-1;

	   for (reco::GsfElectronCollection::const_iterator gsfIter=electrons->begin();
		gsfIter!=electrons->end(); gsfIter++){
	     
	     //changed in dPhi!
	     double deltaR = sqrt(pow((gsfIter->eta()-pAssSim.eta()),2) + pow( delta_phi(gsfIter->phi(), pAssSim.phi() ),2));
	     if ( deltaR < deltaR_ ){
	       if ( (genPc->pdg_id() == 11) && (gsfIter->charge() < 0.) || (genPc->pdg_id() == -11) &&
		    (gsfIter->charge() > 0.) ){
		 double tmpGsfRatio = gsfIter->p()/pAssSim.t();
		 if ( fabs(tmpGsfRatio-1) < fabs(gsfOkRatio-1) ) {
		   gsfOkRatio = tmpGsfRatio;
		   bestGsfElectron=*gsfIter;
		   okGsfFound = true;
		   SCok = bestGsfElectron.superCluster();
		   bestEle = ele_id;

		   //cout << "matched " << gsfIter->eta() << " " << gsfIter->phi() 
		   //<< "with " << pAssSim.eta() << " " << pAssSim.phi() << endl;
 		 }
	       }
	     }
	     ele_id++;
	   } // loop over rec ele to look for the best one
	   if(counter==0) matches.push_back( make_pair(ele_MCid, bestEle) );
	   else matchesKilled.push_back( make_pair(ele_MCid, bestEle) );
	   varIntArr_MC[LABEL+"MC_matches"][MC_n] = bestEle;

	   counter++;
	 }// end of eleLabel loop
	 

         varIntArr_MC["MC_id"][MC_n] = genPc->pdg_id();
         varFloatArr_MC["MC_px"][MC_n] = pAssSim.px();
         varFloatArr_MC["MC_py"][MC_n] = pAssSim.py();
         varFloatArr_MC["MC_pz"][MC_n] = pAssSim.pz();
         varFloatArr_MC["MC_E"][MC_n] = pAssSim.e();
         varFloatArr_MC["MC_eta"][MC_n] = pAssSim.eta();
         varFloatArr_MC["MC_phi"][MC_n] = pAssSim.phi();
         varFloatArr_MC["MC_pt"][MC_n] = pAssSim.perp();
         varIntArr_MC["MC_status"][MC_n] = genPc->status();

	 varInt["MC_n"] = ele_MCid++;
	 //ele_MCid++;
       }//end of Z/W check
     }//end of ifEle mc check
   }//3nd of mc loop
   

   ////Eles
   counter  = 0;
   string LABEL = "";
   edm::Handle<reco::GsfElectronCollection> okElectrons;
   iEvent.getByLabel(eleLabel_[0],okElectrons);
   edm::Handle<reco::GsfElectronCollection> killedElectrons;
   iEvent.getByLabel(eleLabel_[1],killedElectrons);
   
   for (std::vector<edm::InputTag>::const_iterator i = eleLabel_.begin(); i!=eleLabel_.end(); i++) {
     //get electrons
     edm::Handle<reco::GsfElectronCollection> electrons;
     if(counter == 1) electrons = killedElectrons;
     else electrons = okElectrons;
     
     if(counter==1) LABEL = "killed";
     int ele_n=0;
     int matchedMC=-1;
	for (reco::GsfElectronCollection::const_iterator gsfIter=electrons->begin();gsfIter!=electrons->end(); gsfIter++){
	  
	  for(vector< pair<int,int> >::const_iterator p = matches.begin(); p != matches.end(); p++ ){
	    if(p->first == ele_n ) matchedMC = p->second;
	    //cout << p->first << " " << p->second << endl;
	  }
	  float res = -1000;
       if( matchedMC!=-1){
	 res = (gsfIter->energy() -  varFloatArr_MC["MC_E"][matchedMC] ) / varFloatArr_MC["MC_E"][matchedMC];
	 histos[LABEL+"Ele_ERes"]->Fill(res );
	 cout << LABEL << " " << ele_n << " " << matchedMC << " MC_E:" << varFloatArr_MC["MC_E"][matchedMC]  << " reco_E:" << gsfIter->energy() << " " << res << endl;
	 profiles["p"+LABEL+"Ele_ERes"]->Fill(gsfIter->pt(), res  ); 
	 
	 float outerEta = gsfIter->trackMomentumOut().eta(); float outerPhi = gsfIter->trackMomentumOut().phi();
	 float xTk = gsfIter->trackMomentumOut().x(); float yTk = gsfIter->trackMomentumOut().y();
	 
	 varFloatArr_Ele[LABEL+"Ele_E"][ele_n] = gsfIter->energy(); varFloatArr_Ele[LABEL+"Ele_eta"][ele_n] = gsfIter->eta();
	 varFloatArr_Ele[LABEL+"Ele_phi"][ele_n] = gsfIter->phi(); varFloatArr_Ele[LABEL+"Ele_px"][ele_n] = gsfIter->px();
	 varFloatArr_Ele[LABEL+"Ele_py"][ele_n] = gsfIter->py(); varFloatArr_Ele[LABEL+"Ele_pz"][ele_n] = gsfIter->pz();
	 varFloatArr_Ele[LABEL+"Ele_pt"][ele_n] = gsfIter->pt(); varFloatArr_Ele[LABEL+"Ele_outerEta"][ele_n] = outerEta;
	 varFloatArr_Ele[LABEL+"Ele_outerPhi"][ele_n] = outerPhi;
	 
         varFloatArr_Ele[LABEL+"Ele_ScEta"][ele_n] = gsfIter->superCluster()->eta(); varFloatArr_Ele[LABEL+"Ele_ScPhi"][ele_n] = gsfIter->superCluster()->phi();

	 if(outerEta < 1.5 && outerEta > -1.5) histos2D[LABEL+"Ele_EtaPhi_EB"]-> Fill(outerPhi, outerEta);
	 if(outerEta > 1.5) histos2D[LABEL+"Ele_XY_EE+"]-> Fill(xTk, yTk);
	 if(outerEta < -1.5) histos2D[LABEL+"Ele_XY_EE-"] -> Fill(xTk, yTk);
	 
	 if(outerEta < 1.5 && outerEta > -1.5) histos2P[LABEL+"Ele_EnergyRes_EB"] -> Fill(outerPhi, outerEta, res); 
	 if(outerEta > 1.5) histos2P[LABEL+"Ele_EnergyRes_EE+"] -> Fill(xTk, yTk,  res); 
	 if(outerEta < -1.5) histos2P[LABEL+"Ele_EnergyRes_EE-"] -> Fill(xTk, yTk,  res);
	 
	 if(outerEta < 1.5 && outerEta > -1.5) histos2P[LABEL+"Ele_Energy_EB"] -> Fill(outerPhi, outerEta,gsfIter->energy() );
         if(outerEta > 1.5) histos2P[LABEL+"Ele_Energy_EE+"] -> Fill(xTk, yTk,  gsfIter->energy());
         if(outerEta < -1.5) histos2P[LABEL+"Ele_Energy_EE-"] -> Fill(xTk, yTk, gsfIter->energy());

	 ele_n++;
	 varInt[LABEL+"Ele_n"] = ele_n;
       }
	}//endl ele loop
	//counter++;
	//}
	counter++;
   }
   
   
   if( varInt["Ele_n"] != varInt["killedEle_n"])
     cout << varInt["Ele_n"] << " " << varInt["killedEle_n"] << endl;
   
   mtree->Fill();
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
TowerAnalysis::beginJob(const edm::EventSetup&)
{
  mtree->Branch("Event_number",&Event_number,"Event_number/I");
  mtree->Branch("Run_number",&Run_number,"Run_number/I");
  
  //Jet branches creation
  mtree->Branch( "Jets_n",&(varInt["Jets_n"]),"Jets_n/I" );
  mtree->Branch( "killedJets_n",&(varInt["killedJets_n"]),"killedJets_n/I" );
  mtree->Branch( "genJets_n",&(varInt["genJets_n"]),"genJets_n/I" );

  
  for(vector<string>::iterator var = JET_VAR.begin(); var!=JET_VAR.end(); ++var){
    mtree->Branch( ("Jets_"+*var).c_str(), varFloatArr_Jets["Jets_"+*var], ("Jets_"+*var+"[Jets_n]/F").c_str() );
    mtree->Branch( ("killedJets_"+*var).c_str(), varFloatArr_Jets["killedJets_"+*var], ("killedJets_"+*var+"[killedJets_n]/F").c_str() );
    mtree->Branch( ("Jets_GEN_"+*var).c_str(), varFloatArr_Jets["Jets_GEN_"+*var], ("Jets_GEN_"+*var+"[genJets_n]/F").c_str() );
  }  

  //Met branches creation
  for(vector<string>::iterator var = MET_VAR.begin(); var!=MET_VAR.end(); ++var){
    mtree->Branch( ("Met_"+*var).c_str(), &(varFloatArr_Met["Met_"+*var]), ("Met_"+*var+"/F").c_str() );
    mtree->Branch( ("killedMet_"+*var).c_str(), &(varFloatArr_Met["killedMet_"+*var]), ("killedMet_"+*var+"/F").c_str() );
    mtree->Branch( ("Met_GEN_"+*var).c_str(), &(varFloatArr_Met["Met_GEN_"+*var]), ("Met_GEN_"+*var+"/F").c_str() );
  }
  //Electrons
  mtree->Branch( "Ele_n",&(varInt["Ele_n"]),"Ele_n/I" );
  mtree->Branch( "killedEle_n",&(varInt["killedEle_n"]),"killedEle_n/I" );

  for(vector<string>::iterator var = ELE_VAR.begin(); var!=ELE_VAR.end(); ++var){
    mtree->Branch( ("Ele_"+*var).c_str(), varFloatArr_Ele["Ele_"+*var], ("Ele_"+*var+"[Ele_n]/F").c_str() );
    mtree->Branch( ("killedEle_"+*var).c_str(), varFloatArr_Ele["killedEle_"+*var], ("killedEle_"+*var+"[killedEle_n]/F").c_str() );
  }

  mtree->Branch( "MC_n", &varInt["MC_n"],"MC_n/I" );
  for(vector<string>::iterator var = MC_VAR.begin(); var!=MC_VAR.end(); ++var)
    mtree->Branch( ("MC_"+*var).c_str(), varFloatArr_MC["MC_"+*var], ("MC_"+*var+"[MC_n]/F").c_str() );
  
  mtree->Branch( "MC_id", varIntArr_MC["MC_id"], "MC_id[MC_n]/I");
  mtree->Branch( "MC_status", varIntArr_MC["MC_status"], "MC_status[MC_n]/I");
  mtree->Branch( "MC_matches", varIntArr_MC["MC_matches"], "MC_matches[MC_n]/I");
  mtree->Branch( "killedMC_matches", varIntArr_MC["killedMC_matches"], "killedMC_matches[MC_n]/I");


  for(int i=0;i<2;i++){
    string label = sLABEL[i];
    histos[label+"Ele_ERes"] = fs->make<TH1F>( (label+"Ele_ERes").c_str(),"electron resolution;E^{reco}-E^{true}/E^{true}",100,-0.2,0.2);
    profiles["p"+label+"Ele_ERes"] = fs->make<TProfile>( ("p"+label+"Ele_ERes").c_str(),"electron resolution;E^{reco}-E^{true}/E^{true}",100,-0.2,0.2);

    histos[label+"Met_EtRes"] = fs->make<TH1F>( (label+"Met_EtRes").c_str(),"met resolution;",1000,-1000,1000);
    histos[label+"Met_PhiRes"] = fs->make<TH1F>( (label+"Met_PhiRes").c_str(),"met #phi resolution;",1000,-1000,1000);

    histos[label+"Jets_EtRes"] = fs->make<TH1F>( (label+"Jets_EtRes").c_str(),"jet resolution;",100,-100,100);
    histos[label+"Jets_PhiRes"] = fs->make<TH1F>( (label+"Jets_PhiRes").c_str(),"jet #phi resolution;",100,-100,100);


    histos2D[label + "Ele_EtaPhi_EB"] = fs->make<TH2F>( (label + "Ele_EtaPhi_EB").c_str(), (label+"Ele_simEtavsPhi").c_str(), 360, -3.14159, 3.14159, 172, -1.5, 1.5);
    histos2D[label + "Ele_XY_EE+"] = fs->make<TH2F>( (label+"Ele_XY_EE+").c_str(), (label + "Ele_simXvsYinPE_matched").c_str(), 100, -150., 150., 100, -150., 150.); 
    histos2D[label + "Ele_XY_EE-"] = fs->make<TH2F>( (label + "Ele_XY_EE-").c_str(), (label + "Ele_simXvsYinNE_matched").c_str(), 100, -150., 150., 100, -150., 150.);


    histos2P[label + "Ele_EnergyRes_EB"] = fs->make<TProfile2D>((label + "Ele_EnergyRes_EB").c_str(), (label + "Ele_EnergyRes_EB").c_str(), 360, -3.14159, 3.14159, 172, -1.5, 1.5);
    histos2P[label + "Ele_EnergyRes_EE+"] = fs->make<TProfile2D>( (label + "Ele_EnergyRes_EE+").c_str(), (label + "Ele_EnergyRes_EE+").c_str(), 100, -150., 150., 100, -150., 150.);
    histos2P[label + "Ele_EnergyRes_EE-"] = fs->make<TProfile2D>( (label + "Ele_EnergyRes_EE-").c_str(), (label + "Ele_EnergyRes_EE-").c_str(), 100, -150., 150., 100, -150., 150.);

    
    histos2P[label + "Ele_Energy_EB"] = fs->make<TProfile2D>((label + "Ele_Energy_EB").c_str(), (label + "Ele_Energy_EB").c_str(), 360, -3.14159, 3.14159, 172, -1.5, 1.5);
    histos2P[label + "Ele_Energy_EE+"] = fs->make<TProfile2D>( (label + "Ele_Energy_EE+").c_str(), (label + "Ele_Energy_EE+").c_str(), 100, -150., 150., 100, -150., 150.);
    histos2P[label + "Ele_Energy_EE-"] = fs->make<TProfile2D>( (label + "Ele_Energy_EE-").c_str(), (label + "Ele_Energy_EE-").c_str(), 100, -150., 150., 100, -150., 150.);
  }  
  
  histos2D["Ele_EtaPhiEff_EB"] = fs->make<TH2F>( "Ele_EtaPhiEff_EB", "Ele_simEtavsPhiEff_matched", 360,-3.14159, 3.14159, 172, -1.5, 1.5);
  histos2D["Ele_XYEff_EE+"] = fs->make<TH2F>( "Ele_XYEff_EE+", "Ele_simXvsYEffinPE_matched", 100, -150., 150., 100, -150., 150.);
  histos2D["Ele_XYEff_EE-"] = fs->make<TH2F>( "Ele_XYEff_EE-", "Ele_simXvsYEffinNE_matched", 100, -150., 150., 100, -150., 150.);

  histos2P["globalEle_EnergyRes_EB"] = fs->make<TProfile2D>("globalEle_EnergyRes_EB", "globalEle_EnergyRes_EB", 360, -3.14159, 3.14159, 172, -1.5, 1.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TowerAnalysis::endJob() {
  //  m_file->Write();
  //m_file->Close();

  std::cout << "efficiency calculation " << std::endl;

    histos2D["Ele_EtaPhiEff_EB"]->Reset();
    histos2D["Ele_EtaPhiEff_EB"]->Divide( histos2D["killedEle_EtaPhi_EB"],histos2D["Ele_EtaPhi_EB"],1,1);
    histos2D["Ele_EtaPhiEff_EB"]->Print();
    
    histos2D["Ele_XYEff_EE+"]->Reset();
    histos2D["Ele_XYEff_EE+"]->Divide( histos2D["killedEle_XY_EE+"],histos2D["Ele_XY_EE+"],1,1);
    histos2D["Ele_XYEff_EE+"]->Print();
    
    histos2D["Ele_XYEff_EE-"]->Reset();
    histos2D["Ele_XYEff_EE-"]->Divide( histos2D["killedEle_XY_EE-"],histos2D["Ele_XY_EE-"],1,1);
    histos2D["Ele_XYEff_EE-"]->Print();

    histos2P["globalEle_EnergyRes_EB"]->Reset();
    histos2P["globalEle_EnergyRes_EB"]->Divide( histos2P["killedEle_EnergyRes_EB"],histos2P["Ele_EnergyRes_EB"],1,1);
    histos2P["globalEle_EnergyRes_EB"]->Print();

}

//simple dPhi calculator
float TowerAnalysis::delta_phi(float phi1, float phi2)
{
  
  Float_t pi_greco = 3.1415;
  Float_t adphi = fabs(phi1 -phi2);
  Float_t dphi = (phi1 -phi2);
  if (adphi > fabs(2*pi_greco-adphi)){
    if(dphi > 0){ dphi -= 2*pi_greco;}
    if(dphi < 0){ dphi += 2*pi_greco;}
    dphi = fabs(2*pi_greco-adphi);
  }
  
  return fabs(dphi);
}


//define this as a plug-in
DEFINE_FWK_MODULE(TowerAnalysis);

//  LocalWords:  str

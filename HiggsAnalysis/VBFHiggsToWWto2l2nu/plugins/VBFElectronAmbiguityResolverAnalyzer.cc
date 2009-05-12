#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElectronAmbiguityResolverAnalyzer.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <Math/VectorUtil.h>

#include <iostream>
#include <algorithm>

#include "TTree.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


VBFElectronAmbiguityResolverAnalyzer::VBFElectronAmbiguityResolverAnalyzer (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag"))
{
 edm::Service<TFileService> fs ;
 mytree_        = fs->make <TTree>("VBFSimpleTree","VBFSimpleTree"); 
 mytree_event_  = fs->make <TTree>("mytree_event","mytree_event"); 
 mytree_ele_    = fs->make <TTree>("mytree_ele","mytree_ele");
 
}  


// ----------------------------------------------------------------------------

  
VBFElectronAmbiguityResolverAnalyzer::~VBFElectronAmbiguityResolverAnalyzer ()
{
}


// ----------------------------------------------------------------------------


void 
  VBFElectronAmbiguityResolverAnalyzer::beginJob(const edm::EventSetup& iSetup)
{

 mytree_->Branch("DR_",&DR_,"DR_/D");
 mytree_->Branch("minMC_EnEn_1_",&minMC_EnEn_1_,"minMC_EnEn_1_/D");
 mytree_->Branch("minMC_EnEn_2_",&minMC_EnEn_2_,"minMC_EnEn_2_/D");
 mytree_->Branch("minMC_DR_1_",&minMC_DR_1_,"minMC_DR_1_/D");
 mytree_->Branch("minMC_DR_2_",&minMC_DR_2_,"minMC_DR_2_/D");
 mytree_->Branch("EoP_1_",&EoP_1_,"EoP_1_/D");
 mytree_->Branch("EoP_2_",&EoP_2_,"EoP_2_/D");
 mytree_->Branch("pt_1_",&pt_1_,"pt_1_/D");
 mytree_->Branch("pt_2_",&pt_2_,"pt_2_/D");
 mytree_->Branch("numAmb_SC_",&numAmb_SC_,"numAmb_SC_/I");
 mytree_->Branch("numAmb_Tk_",&numAmb_Tk_,"numAmb_Tk_/I");
 
 mytree_event_->Branch("numAmb_2_",&numAmb_2_,"numAmb_2_/I");
 mytree_event_->Branch("numAmb_3_",&numAmb_3_,"numAmb_3_/I");
 mytree_event_->Branch("numAmb_4_",&numAmb_4_,"numAmb_4_/I");
 mytree_event_->Branch("numAmb_5_",&numAmb_5_,"numAmb_5_/I");
 
 mytree_ele_->Branch("numAmb_",&numAmb_,"numAmb_/I");
 mytree_ele_->Branch("numAmb_SC_",&numAmb_SC_,"numAmb_SC_/I");
 mytree_ele_->Branch("numAmb_Tk_",&numAmb_Tk_,"numAmb_Tk_/I");
 mytree_ele_->Branch("numAmb_SC_pt_",&numAmb_SC_pt_,"numAmb_SC_pt_/I");
 mytree_ele_->Branch("numAmb_Tk_pt_",&numAmb_Tk_pt_,"numAmb_Tk_pt_/I");
 mytree_ele_->Branch("pt_",&pt_,"pt_/D");
 

 
 
}
 
 
// ----------------------------------------------------------------------------

 
 void 
   VBFElectronAmbiguityResolverAnalyzer::endJob() {
   }

 
 // ----------------------------------------------------------------------------
  
  
   void 
     VBFElectronAmbiguityResolverAnalyzer::analyze (const edm::Event& iEvent,
     const edm::EventSetup& iEs)
     {
  //---- AM get the GSF electrons collection ----
      edm::Handle<electronCollection> GSFHandle;
//       iEvent.getByLabel ("pixelMatchGsfElectrons",GSFHandle) ; 
      iEvent.getByLabel (m_GSFInputTag,GSFHandle) ; 

  //---- AM get GenParticles ----
      edm::Handle<reco::GenParticleCollection> genParticles; 
      iEvent.getByLabel ("genParticles",genParticles);
      if(!genParticles.isValid()){
       std::cout<<"AAAA genParticles handle is not valid!!!"<<std::endl;return;
      }
 
//   //---- AM loop over tracks
//   edm::Handle<edm::View<reco::Track> > TracksHandle ;
//   iEvent.getByLabel (m_TracksInputTag,TracksHandle) ; 
  
 
      numAmb_2_ = 0;
      numAmb_3_ = 0;
      numAmb_4_ = 0;
      numAmb_5_ = 0; 
 
      std::vector<int> excludedEle(static_cast<int> (GSFHandle->size ()),1);
 
      std::vector<int> excludedEle_SC(static_cast<int> (GSFHandle->size ()),1);
      std::vector<int> excludedEle_Tk(static_cast<int> (GSFHandle->size ()),1);
 
      std::vector<int> excludedEle_SC_pt(static_cast<int> (GSFHandle->size ()),1);
      std::vector<int> excludedEle_Tk_pt(static_cast<int> (GSFHandle->size ()),1);
      
      
  //---- AM loop over electrons
      for (electronCollection::const_iterator eleIt_1 = GSFHandle->begin () ;
           eleIt_1 != GSFHandle->end () ;
           ++eleIt_1)
      {
       numAmb_ = 0;
       numAmb_SC_ = 0;
       numAmb_Tk_ = 0;
  
       numAmb_SC_pt_ = 0;
       numAmb_Tk_pt_ = 0;
  
       reco::GsfTrackRef thisTrack1  = eleIt_1->gsfTrack();
       reco::SuperClusterRef thisSc1 = eleIt_1->superCluster();

       pt_ = eleIt_1->pt();
         
     //---- AM loop over electrons
       for (electronCollection::const_iterator eleIt_2 = eleIt_1+1 ;
            eleIt_2 != GSFHandle->end () ;
            ++eleIt_2)
       {      
        math::XYZPoint direction_1 = eleIt_1->caloPosition () ; 
        math::XYZPoint direction_2 = eleIt_2->caloPosition () ; 
        double DR = ROOT::Math::VectorUtil::DeltaR(direction_1,direction_2);
        DR_ = DR;
   
        if (DR_ < 0.02) {
         int num_eleIt_1 = eleIt_1 - GSFHandle->begin ();
         if (excludedEle.at(num_eleIt_1)){
           if (eleIt_1->pt() > 10 && eleIt_2->pt() > 10){
           numAmb_++;
           int num_eleIt_2 = eleIt_2 - GSFHandle->begin ();
           excludedEle.at(num_eleIt_2) = 0;
          }
         }
        }
   
   
        reco::GsfTrackRef thisTrack2  = eleIt_2->gsfTrack();
        reco::SuperClusterRef thisSc2 = eleIt_2->superCluster();

        if (thisSc1 == thisSc2){
         int num_eleIt_1 = eleIt_1 - GSFHandle->begin ();
         if (excludedEle_SC.at(num_eleIt_1)){
          numAmb_SC_++;
          int num_eleIt_2 = eleIt_2 - GSFHandle->begin ();
          excludedEle_SC.at(num_eleIt_2) = 0;
         }
        }
   
        if (thisTrack1 == thisTrack2){
         int num_eleIt_1 = eleIt_1 - GSFHandle->begin ();
         if (excludedEle_Tk.at(num_eleIt_1)){
          numAmb_Tk_++;
          int num_eleIt_2 = eleIt_2 - GSFHandle->begin ();
          excludedEle_Tk.at(num_eleIt_2) = 0;
         }
        }
   
        
        if (thisSc1 == thisSc2){
         int num_eleIt_1 = eleIt_1 - GSFHandle->begin ();
         if (excludedEle_SC_pt.at(num_eleIt_1)){
          if (eleIt_1->pt() > 10 && eleIt_2->pt() > 10){
           numAmb_SC_pt_++;
           int num_eleIt_2 = eleIt_2 - GSFHandle->begin ();
           excludedEle_SC_pt.at(num_eleIt_2) = 0;
          }
         }
        }
   
        if (thisTrack1 == thisTrack2){
         int num_eleIt_1 = eleIt_1 - GSFHandle->begin ();
         if (excludedEle_Tk_pt.at(num_eleIt_1)){
          if (eleIt_1->pt() > 10 && eleIt_2->pt() > 10){
           numAmb_Tk_pt_++;
           int num_eleIt_2 = eleIt_2 - GSFHandle->begin ();
           excludedEle_Tk_pt.at(num_eleIt_2) = 0;
          }
         }
        }
 
   
        EoP_1_ = eleIt_1->eSuperClusterOverP ();
        EoP_2_ = eleIt_2->eSuperClusterOverP ();
   
        pt_1_ = eleIt_1->pt ();
        pt_2_ = eleIt_2->pt ();
   
   
   
   //---- loop over GenParticles ----
  
        minMC_EnEn_1_ = 1000;
        minMC_EnEn_2_ = 1000;
  
        minMC_DR_1_ = 1000;
        minMC_DR_2_ = 1000;
   
//    math::XYZVector posVtx_1  = eleIt_1->TrackPositionAtVtx();
//    math::XYZVector posVtx_2  = eleIt_2->TrackPositionAtVtx();
   
        math::XYZVector posVtx_1  = eleIt_1->trackMomentumAtVtx();
        math::XYZVector posVtx_2  = eleIt_2->trackMomentumAtVtx();

        double Eta_1 =   posVtx_1.eta();
        double Eta_2 =   posVtx_2.eta();

        double Phi_1 =   posVtx_1.phi();
        double Phi_2 =   posVtx_2.phi();
   
        double En_1 = eleIt_1->energy () ; 
        double En_2 = eleIt_2->energy () ; 
        for (reco::GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
        {
         int numPDG = p->pdgId();
         if (abs(numPDG) == 11) {
          double energy_MC = p->energy () ; 
          double minMC_EnEn_temp_1 = fabs(energy_MC - En_1) / En_1; 
          double minMC_EnEn_temp_2 = fabs(energy_MC - En_2) / En_2; 
          if (minMC_EnEn_temp_1 < minMC_EnEn_1_) minMC_EnEn_1_ = minMC_EnEn_temp_1;
          if (minMC_EnEn_temp_2 < minMC_EnEn_2_) minMC_EnEn_2_ = minMC_EnEn_temp_2;
     
          double posVtx_MC_eta  = p->eta();
          double posVtx_MC_phi  = p->phi();
          double DPhi1 = sqrt((posVtx_MC_phi - Phi_1) * (posVtx_MC_phi - Phi_1));
          if (DPhi1>2*3.14159265) DPhi1 -= 2*3.14159265;
          if (DPhi1>3.14159265) DPhi1 -= 3.14159265;
          double DPhi2 = sqrt((posVtx_MC_phi - Phi_2) * (posVtx_MC_phi - Phi_2));
          if (DPhi2>2*3.14159265) DPhi2 -= 2*3.14159265;
          if (DPhi2>3.14159265) DPhi2 -= 3.14159265;
     
          double DR_MC_temp_1 = sqrt((posVtx_MC_eta - Eta_1)*(posVtx_MC_eta - Eta_1) + DPhi1 * DPhi1);
          double DR_MC_temp_2 = sqrt((posVtx_MC_eta - Eta_2)*(posVtx_MC_eta - Eta_2) + DPhi2 * DPhi2);
     
//      double DR_MC_temp_1 = sqrt(DPhi1 * DPhi1);
//      double DR_MC_temp_2 = sqrt(DPhi2 * DPhi2);

          if (DR_MC_temp_1 < minMC_DR_1_) minMC_DR_1_ = DR_MC_temp_1;  
          if (DR_MC_temp_2 < minMC_DR_2_) minMC_DR_2_ = DR_MC_temp_2;  
         }
        }
   
        mytree_->Fill();
       }
  //---- AM end loop over electrons
       mytree_ele_->Fill();
      }
 //---- AM end loop over electrons

      mytree_event_->Fill();   

     }

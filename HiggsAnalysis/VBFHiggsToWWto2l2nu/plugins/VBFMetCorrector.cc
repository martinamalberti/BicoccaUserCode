#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMetCorrector.h"
//#include <Math/VectorUtil.h>

// Lorentz vector
typedef math::XYZTLorentzVector LorentzVector ;

VBFMetCorrector::VBFMetCorrector (const edm::ParameterSet& iConfig)
{
  caloMetLabel_    = iConfig.getParameter<edm::InputTag> ("CaloMetLabel") ;
  muonLabel_       = iConfig.getParameter<edm::InputTag> ("MuonLabel") ;

  corrmetMin_      = iConfig.getParameter<double> ("CorrMetMin") ;   
  muonPtMin_       = iConfig.getParameter<double> ("MuonPtMin") ; 
  muonEtaMax_      = iConfig.getParameter<double> ("MuonEtaMax") ; 
  muonTrackD0Max_  = iConfig.getParameter<double> ("MuonTrackD0Max") ; 
  muonTrackDzMax_  = iConfig.getParameter<double> ("MuonTrackDzMax") ; 
  muonNHitsMin_    = iConfig.getParameter<double> ("MuonNHitsMin") ; 
  muonDPtMax_      = iConfig.getParameter<double> ("MuonDPtMax") ;
  muonChiSqMax_    = iConfig.getParameter<double> ("MuonChiSqMax") ;

  muonCorrection_  = iConfig.getParameter<bool> ("MuonCorrection") ;
  muonDepositCor_  = iConfig.getParameter<bool> ("MuonDepositCor") ;

  //register your products
  produces<reco::CaloMETCollection> ("") ;
}


VBFMetCorrector::~VBFMetCorrector ()
{
}

// ------------ method called to produce the data  ------------
void
VBFMetCorrector::produce (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm ;
  using namespace std ;
  using namespace reco ;
 
   // get met from CMSSW
   Handle<reco::CaloMETCollection> calomet ;
   iEvent.getByLabel (caloMetLabel_,calomet) ;
 
   // For muon corrections get muons 
   Handle<reco::MuonCollection> muons ;
   iEvent.getByLabel (muonLabel_,muons) ; 
 
   // define ethz collections to be stored 
   auto_ptr<reco::CaloMETCollection> pOutMets (new reco::CaloMETCollection) ;
 
   double DeltaPx = 0.0 ;
   double DeltaPy = 0.0 ;
   double DeltaSumET = 0.0 ;
 
   double DeltaExDep = 0.0 ;
   double DeltaEyDep = 0.0 ;
   double DeltaSumETDep = 0.0 ;
 
   //PG loop over MET objects
   for (reco::CaloMETCollection::const_iterator met=calomet->begin () ; met != calomet->end () ; ++met) 
     {
 
     // first we check if we have to correct for muons
     if (muonCorrection_) 
       //PG loop over muons
       for (reco::MuonCollection::const_iterator muon=muons->begin () ; 
            muon != muons->end () ; 
            ++muon) 
         {
           //PG first quality check (reduced wrt official package because of skimming) 
           if (muon->pt () > muonPtMin_ && //PG come mai qui e' usato il muone combinato?
               fabs (muon->eta ()) < muonEtaMax_) //PG come mai qui e' usato il muone combinato?
             {
               DeltaPx +=  muon->px () ;
               DeltaPy +=  muon->py () ;
               DeltaSumET += muon->et () ;
             } //PG first quality check 
         } //PG loop over muons
 
                                       
 //----------------- Calculate and set deltas for new MET correction
      CorrMETData delta ;
      delta.mex     = - DeltaPx ;  //correction to MET (from muons) is negative,    
      delta.mey     = - DeltaPy ;    //since MET points in direction of muons
      delta.sumet   = DeltaSumET ; 
      delta.mex    += DeltaExDep ;    //correction to MET (from muon depositions) is positive,    
      delta.mey    += DeltaEyDep ;    //since MET points in opposite direction of muons
      delta.sumet  -= DeltaSumETDep ;    
      
 //----------------- Fill holder with corrected MET (= uncorrected + delta) values
 
      double CorrPx = met->px () + delta.mex ;
      double CorrPy = met->py () + delta.mey ;
      double CorrSumEt = met->sumEt () + delta.sumet ;
 
      double CorrPt=sqrt (CorrPx * CorrPx + CorrPy * CorrPy) ;
   
      //     if (CorrPt < corrmetMin_) continue ;
      
      LorentzVector p4 (CorrPx, CorrPy, 0, sqrt (CorrPt * CorrPt)) ;
 
 //----------------- get previous corrections and push into new corrections 
 
      std::vector<CorrMETData> corrections = met->mEtCorr () ;
      corrections.push_back (delta) ;
 
 //----------------- Push onto MET Collection
 
      reco::CaloMET* MET = new reco::CaloMET (met->getSpecific (), 
                                              CorrSumEt,corrections, p4, met->vertex ()) ;
 
      pOutMets->push_back (*MET) ;
 
      iEvent.put (pOutMets,"") ;
 
      delete MET ;
    } //PG loop over MET objects
}

// ------------ method called once each job just before starting event loop  ------------
void 
VBFMetCorrector::beginJob (const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VBFMetCorrector::endJob () {
}

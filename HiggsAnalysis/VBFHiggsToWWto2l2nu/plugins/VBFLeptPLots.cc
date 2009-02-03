// $Id: VBFLeptPLots.cc,v 1.1 2008/03/17 17:01:17 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptPlots.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
//for TH1F
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include <Math/VectorUtil.h>
#include "TLorentzVector.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"


VBFLeptPlots::VBFLeptPlots (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muonsInputTag"))
{}


// --------------------------------------------------------------------


VBFLeptPlots::~VBFLeptPlots ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFLeptPlots::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{

  // Get the electrons
  edm::Handle<electronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag, GSFHandle) ;

  //PG crea due iteratori per i due elettroni scelti
  electronCollection::const_iterator firstEle ;
  electronCollection::const_iterator secondEle ;
  double firstEleMaxPt = 0 ;
  double secondEleMaxPt = 0 ;

  //PG switch for electrons
  switch (GSFHandle->size ())
    {
      case 0 :
        break ;
      case 1 :
        firstEleMaxPt == GSFHandle->begin ()->pt () ;
        firstEle = GSFHandle->begin () ;
        break ;
      default :
        //PG loop over electrons
        for (electronCollection::const_iterator eleIt = 
               GSFHandle->begin () ; 
             eleIt != GSFHandle->end () ; 
             ++eleIt) 
          {
            if (firstEleMaxPt < eleIt->pt ())
              {
                secondEleMaxPt = firstEleMaxPt ;
                secondEle = firstEle ;
                firstEleMaxPt = eleIt->pt () ;
                firstEle = eleIt ;
              }
            else if (secondEleMaxPt < eleIt->pt ())
              {
                secondEleMaxPt = eleIt->pt () ;
                secondEle = eleIt ;
              }  
          } //PG loop over electrons 
    } //PG switch for electrons

  if (firstEleMaxPt * secondEleMaxPt != 0)  
    {
      fillHistos (firstEle, secondEle, m_GSFhistos) ;
      LorentzVector sum = firstEle->p4 () + secondEle->p4 () ;
      m_ntuple->Fill (
                      fabs (firstEle->eta () - secondEle->eta ()),
                      vbfhww2l::deltaPhi (firstEle->phi (), secondEle->phi ()),
                      sum.M (),
                      firstEle->pt (),
                      firstEle->eta (),
                      2
	              );
      return ;
    }

/* PG FIXME

  - per ora non basta, bisogna controllare nel caso in cui ci siano
    tanti elettroni e tanti muoni

*/    
    
  //VT get the Global muons collection
  edm::Handle<muonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 
    
  //PG crea due iteratori per i due elettroni scelti
  muonCollection::const_iterator firstMu ;
  muonCollection::const_iterator secondMu ;
  double firstMuMaxPt = 0 ;
  double secondMuMaxPt = 0 ;
  
  //PG switch for muons
  switch (MuonHandle->size ())
    {
      case 0 :
        break ;
      case 1 :
        firstMuMaxPt == MuonHandle->begin ()->pt () ;
        firstMu = MuonHandle->begin () ;
        break ;
      default :
        //PG loop over muons
        for (muonCollection::const_iterator muIt = MuonHandle->begin () ; 
             muIt != MuonHandle->end () ; 
             ++muIt) 
          {
            if (firstMuMaxPt < muIt->pt ())
              {
                secondMuMaxPt = firstMuMaxPt ;
                secondMu = firstMu ;
                firstMuMaxPt = muIt->pt () ;
                firstMu = muIt ;
              }
            else if (secondMuMaxPt < muIt->pt ())
              {
                secondMuMaxPt = muIt->pt () ;
                secondMu = muIt ;
              }  
          } //PG loop over muons 
    } //PG switch for muons
    

  if (firstMuMaxPt * secondMuMaxPt != 0)
    {
      fillHistos (firstMu, secondMu, m_MUhistos) ;
      LorentzVector sum = firstMu->p4 () + secondMu->p4 () ;
      m_ntuple->Fill (
		      fabs (firstMu->eta () - secondMu->eta ()),
		      vbfhww2l::deltaPhi (firstMu->phi (), secondMu->phi ()),
		      sum.M (),
		      firstMu->pt (),
		      firstMu->eta (),
	              1//mumu
		      ) ;
      return ;
    }

  fillHistos (firstMu, firstEle, m_Xhistos) ;
  LorentzVector sum = firstMu->p4 () + firstEle->p4 () ;
  double ptM=-1,etaM=8;
  if (firstMu->pt() > firstEle->pt()) 
    {
      ptM = firstMu->pt () ;
      etaM = firstMu->eta ();
    }
  else
    {
      ptM = firstEle->pt () ;
      etaM = firstEle->eta ();
    }
  m_ntuple->Fill (
      fabs (firstMu->eta () - firstEle->eta ()),
      vbfhww2l::deltaPhi (firstMu->phi (), firstEle->phi ()),
      sum.M (),
      ptM,
      etaM,
      3//emu
    ) ;

}


// --------------------------------------------------------------------


void 
VBFLeptPlots::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_ntuple = fs->make <TNtuple> ("ntuple","Some variables","deltaEta:deltaPhi:mInv:Ptmax:etaMax:sample") ;
  m_GSFhistos.init ("GSF",fs) ;
  m_MUhistos.init ("MU",fs) ;
  m_Xhistos.init ("X",fs) ;
}


// --------------------------------------------------------------------


void 
VBFLeptPlots::endJob () 
{
}


// --------------------------------------------------------------------


void VBFLeptHistos::init (std::string tag, 
                         edm::Service<TFileService> & fs) 
{
  m_deltaEta   = fs->make<TH1F> (TString (tag.c_str ()) + "_deltaEta" ,"#Delta#eta between leptons",50,0,6) ;
  m_deltaPhi   = fs->make<TH1F> (TString (tag.c_str ()) + "_deltaPhi" ,"#Delta#phi between leptons",50,0,3.15) ;
  m_averageEta = fs->make<TH1F> (TString (tag.c_str ()) + "_averageEta" ,"average #eta of leptons",100,-3,3) ;
  m_averagePhi = fs->make<TH1F> (TString (tag.c_str ()) + "_averagePhi" ,"average #phi of leptons",100,0,6.28) ;
  m_invMass    = fs->make<TH1F> (TString (tag.c_str ()) + "_invMass" ,"M_{inv} of leptons",90,0,300) ;
  m_eta        = fs->make<TH1F> (TString (tag.c_str ()) + "_eta" ,"#eta of leptons",100,-3,3) ;
  m_phi        = fs->make<TH1F> (TString (tag.c_str ()) + "_phi" ,"#phi of leptons",100,-3.15,3.15) ;
  m_pt         = fs->make<TH1F> (TString (tag.c_str ()) + "_pt" ,"p_{T} of leptons",100,0,500) ;
  m_ptMax      = fs->make<TH1F> (TString (tag.c_str ()) + "_ptMax" ,"p_{T} of the max p_{T} lepton",50,0,300) ;
  m_ptMin      = fs->make<TH1F> (TString (tag.c_str ()) + "_ptMin" ,"p_{T} of the min p_{T} lepton",50,0,300) ;
}                         


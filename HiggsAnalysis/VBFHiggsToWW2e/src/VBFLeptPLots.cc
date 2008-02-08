// $Id: VBFLeptPlots.cc,v 1.6 2008/02/06 17:57:45 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFLeptPlots.h"
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
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"


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
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag, GSFHandle) ;
  if (GSFHandle->size () < 2) return ;
  
/* PG FIXME

  - togli il controllo sul numero di elettroni, metti il modulo
    sul counting dei leptoni
  - riempi le distro giuste in funzione del numero di oggetti che trova!
  - plot per soli ele
  - plot per soli mu
  - plot per cross channel
    - ne faccio due? 
  - plot per tutti (?)
  - quante ntuple faccio?
  
  - come faccio a salvare il massimo generico e il secondo
    massimo generico, fra elettroni e muoni?

*/    
    
  //PG crea due iteratori per i due elettroni scelti
  reco::PixelMatchGsfElectronCollection::const_iterator firstEle ;
  reco::PixelMatchGsfElectronCollection::const_iterator secondEle ;
  double firstEleMaxPt = 0 ;
  double secondEleMaxPt = 0 ;
  
  //PG loop over electrons
  for (reco::PixelMatchGsfElectronCollection::const_iterator eleIt = GSFHandle->begin () ; 
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

  fillHistos (firstEle, secondEle, m_GSFhistos) ;

  //VT get the Global muons collection
  edm::Handle<reco::MuonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 
    
  //PG crea due iteratori per i due elettroni scelti
  reco::MuonCollection::const_iterator firstMu ;
  reco::MuonCollection::const_iterator secondMu ;
  double firstMuMaxPt = 0 ;
  double secondMuMaxPt = 0 ;
  
  //PG loop over muons
  for (reco::MuonCollection::const_iterator muIt = MuonHandle->begin () ; 
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

  fillHistos (firstMu, secondMu, m_MUhistos) ;


}


// --------------------------------------------------------------------


void 
VBFLeptPlots::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_ntuple = fs->make <TNtuple> ("ntuple","Some variables",
                                "deltaEta:deltaPhi:mInv:Ptmax:etaMax") ;
  m_GSFhistos.init ("GSF",fs) ;
  m_MUhistos.init ("MU",fs) ;
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
  m_deltaEta   = fs->make<TH1F> (TString (tag.c_str ()) + "_deltaEta" ,"#Delta#eta between electrons",50,0,6) ;
  m_deltaPhi   = fs->make<TH1F> (TString (tag.c_str ()) + "_deltaPhi" ,"#Delta#phi between electrons",50,0,3.15) ;
  m_averageEta = fs->make<TH1F> (TString (tag.c_str ()) + "_averageEta" ,"average #eta of electrons",100,-3,3) ;
  m_averagePhi = fs->make<TH1F> (TString (tag.c_str ()) + "_averagePhi" ,"average #phi of electrons",100,0,6.28) ;
  m_invMass    = fs->make<TH1F> (TString (tag.c_str ()) + "_invMass" ,"M_{inv} of electrons",90,0,300) ;
  m_eta        = fs->make<TH1F> (TString (tag.c_str ()) + "_eta" ,"#eta of electrons",100,-3,3) ;
  m_phi        = fs->make<TH1F> (TString (tag.c_str ()) + "_phi" ,"#phi of electrons",100,-3.15,3.15) ;
  m_pt         = fs->make<TH1F> (TString (tag.c_str ()) + "_pt" ,"p_{T} of electrons",100,0,500) ;
  m_ptMax      = fs->make<TH1F> (TString (tag.c_str ()) + "_ptMax" ,"p_{T} of the max p_{T} electron",50,0,300) ;
  m_ptMin      = fs->make<TH1F> (TString (tag.c_str ()) + "_ptMin" ,"p_{T} of the min p_{T} electron",50,0,300) ;
}                         


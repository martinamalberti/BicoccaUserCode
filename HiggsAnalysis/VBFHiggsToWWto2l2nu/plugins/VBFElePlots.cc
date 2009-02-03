// $Id: VBFElePlots.cc,v 1.1 2008/03/17 17:01:16 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElePlots.h"
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


VBFElePlots::VBFElePlots (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag"))
{}


// --------------------------------------------------------------------


VBFElePlots::~VBFElePlots ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFElePlots::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{

  // Get the electrons
  edm::Handle<electronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag, GSFHandle) ;
  if (GSFHandle->size () == 0) return ;
  
  // Get the electron ID
  edm::Handle<reco::ElectronIDAssociationCollection> eleIdHandle ;
  iEvent.getByLabel (m_electronIDInputTag, eleIdHandle) ;

  /* FIXME 
   - come scelgo la coppia di elettroni se ce n'e' piu' d'uno?
     - i piu' energetici
     - quelli con pT piu' elevato
   - ctrl isolamento e leggilo
   - ambiguity resolving e leggilo
   - che cosa faccio con quelli rimanenti?
  */
  
  //PG crea due iteratori per i due elettroni scelti
  electronCollection::const_iterator first ;
  electronCollection::const_iterator second ;
  double firstMaxPt = 0 ;
  double secondMaxPt = 0 ;
  
  electronCollection::const_iterator eleID_first ;
  electronCollection::const_iterator eleID_second ;
  double eleID_firstMaxPt = 0 ;
  double eleID_secondMaxPt = 0 ;
  
  //PG loop over electrons
  for (electronCollection::const_iterator eleIt = GSFHandle->begin () ; 
       eleIt != GSFHandle->end () ; 
       ++eleIt) 
    {
//      reco::PixelMatchGsfElectronRef ref (GSFHandle, eleIt - GSFHandle->begin ()) ;
//      reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr = 
//        eleIdHandle->find (ref) ;

      m_GSFhistos.m_eta->Fill (eleIt->eta ()) ;
      m_GSFhistos.m_phi->Fill (eleIt->phi ()) ;
      m_GSFhistos.m_pt->Fill (eleIt->pt ()) ;

//      //PG electron ID passed
//      if (electronIDAssocItr != eleIdHandle->end ())
//        {
//          const reco::ElectronIDRef & id = electronIDAssocItr->val ;
//          bool cutBasedID = id->cutBasedDecision () ;
//          if (cutBasedID == 0) return ;
//          m_eleIDhistos.m_eta->Fill (eleIt->eta ()) ;
//          m_eleIDhistos.m_phi->Fill (eleIt->phi ()) ;
//          m_eleIDhistos.m_pt->Fill (eleIt->pt ()) ;
//
//          if (eleID_firstMaxPt < eleIt->pt ())
//            {
//              eleID_secondMaxPt = eleID_firstMaxPt ;
//              eleID_second = eleID_first ;
//              eleID_firstMaxPt = eleIt->pt () ;
//              eleID_first = eleIt ;
//            }
//          else if (eleID_secondMaxPt < eleIt->pt ())
//            {
//              eleID_secondMaxPt = eleIt->pt () ;
//              eleID_second = eleIt ;
//            }  
//        } //PG electron ID passed
      
      if (firstMaxPt < eleIt->pt ())
        {
          secondMaxPt = firstMaxPt ;
          second = first ;
          firstMaxPt = eleIt->pt () ;
          first = eleIt ;
        }
      else if (secondMaxPt < eleIt->pt ())
        {
          secondMaxPt = eleIt->pt () ;
          second = eleIt ;
        }  
    } //PG loop over electrons 

  if (secondMaxPt * firstMaxPt == 0 || 
      GSFHandle->size () < 2) return ;

  //PG fill the GSF histos
  m_GSFhistos.m_deltaEta->Fill (fabs (first->eta () - second->eta ())) ;
//  double dphi = first->phi () - second->phi () ;
//  if (dphi > 3.14) dphi -= 6.28 ; //PG FIXME be more precise :)
//  if (dphi < -3.14) dphi += 6.28 ;
  double dphi = vbfhww2l::deltaPhi (first->phi (), second->phi ()) ;
//  std::cerr << "[PIETRO] " << dphi << " " << first->phi () << " " << second->phi () << std::endl ;
  m_GSFhistos.m_deltaPhi->Fill (dphi) ;
  m_GSFhistos.m_averageEta->Fill (0.5 * (first->eta () + second->eta ())) ;
  double phi1 = first->phi () ; 
  if (phi1 < 0) phi1 += 6.28 ;
  double phi2 = second->phi () ; 
  if (phi2 < 0) phi2 += 6.28 ;
  m_GSFhistos.m_averagePhi->Fill (0.5 * (phi1 + phi2)) ;
  LorentzVector sum = first->p4 () + second->p4 () ;
  m_GSFhistos.m_invMass->Fill (sum.M ()) ;
  m_GSFhistos.m_ptMax->Fill (first->pt ()) ;
  m_GSFhistos.m_ptMin->Fill (second->pt ()) ;

  m_ntuple->Fill (
      fabs (first->eta () - second->eta ()),
      dphi,
      sum.M (),
      first->pt (),
      first->eta ()
    ) ;

  if (eleID_secondMaxPt * eleID_firstMaxPt == 0) return ;

  //PG fill the eleID histos
  m_eleIDhistos.m_deltaEta->Fill (fabs (first->eta () - second->eta ())) ;
//  dphi = eleID_first->phi () - eleID_second->phi () ;
//  if (dphi > 3.14) dphi -= 6.28 ; //PG FIXME be more precise :)
//  if (dphi < -3.14) dphi += 6.28 ;
  dphi = vbfhww2l::deltaPhi (eleID_first->phi (), eleID_second->phi ()) ;
  m_eleIDhistos.m_deltaPhi->Fill (dphi) ;
  m_eleIDhistos.m_averageEta->Fill (0.5 * (eleID_first->eta () + eleID_second->eta ())) ;
  phi1 = eleID_first->phi () ; 
  if (phi1 < 0) phi1 += 6.28 ;
  phi2 = eleID_second->phi () ; 
  if (phi2 < 0) phi2 += 6.28 ;
  m_eleIDhistos.m_averagePhi->Fill (0.5 * (phi1 + phi2)) ;
  sum = eleID_first->p4 () + eleID_second->p4 () ;
  m_eleIDhistos.m_invMass->Fill (sum.M ()) ;
  m_eleIDhistos.m_ptMax->Fill (eleID_first->pt ()) ;
  m_eleIDhistos.m_ptMin->Fill (eleID_second->pt ()) ;

}


// --------------------------------------------------------------------


void 
VBFElePlots::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_ntuple = fs->make <TNtuple> ("ntuple","Some variables",
                                "deltaEta:deltaPhi:mInv:Ptmax:etaMax") ;
  m_GSFhistos.init ("GSF",fs) ;
  m_eleIDhistos.init ("eleID",fs) ;
}


// --------------------------------------------------------------------


void 
VBFElePlots::endJob () 
{
}


// --------------------------------------------------------------------


void VBFeleHistos::init (std::string tag, 
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

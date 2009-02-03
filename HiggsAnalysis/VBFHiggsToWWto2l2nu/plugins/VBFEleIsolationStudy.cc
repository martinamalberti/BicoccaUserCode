// $Id: VBFEleIsolationStudy.cc,v 1.1 2008/03/17 17:01:15 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleIsolationStudy.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>


VBFEleIsolationStudy::VBFEleIsolationStudy (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_AmbRefInputTag (iConfig.getParameter<edm::InputTag> ("AmbRefInputTag")) ,
  m_OLDIsoInputTag (iConfig.getParameter<edm::InputTag> ("OLDIsoInputTag")) ,
  m_NEWIsoInputTag (iConfig.getParameter<edm::InputTag> ("NEWIsoInputTag")) , 
  m_TrackInputTag (iConfig.getParameter<edm::InputTag> ("trackInputTag")) ,
  m_tkIsolationAlgoWithOtherCones (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      iConfig.getParameter<double> ("otherVetoRadius") ,
      iConfig.getParameter<double> ("ptMin") ,
      iConfig.getParameter<double> ("lipMax") ,
      iConfig.getUntrackedParameter<bool> ("useTkQuality",true) 
    ) ,
  m_tkIsolationAlgoNoOtherCones (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      0. ,
      iConfig.getParameter<double> ("ptMin") ,
      iConfig.getParameter<double> ("lipMax") ,
      iConfig.getUntrackedParameter<bool> ("useTkQuality",true) 
    ) 
{}


// --------------------------------------------------------------------


VBFEleIsolationStudy::~VBFEleIsolationStudy ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFEleIsolationStudy::analyze (const edm::Event& iEvent, 
                               const edm::EventSetup& iSetup)
{

  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle) ; 

  //PG get the AmbRef electrons collection
  edm::Handle<electronCollection> AmbRefHandle ;
  iEvent.getByLabel (m_AmbRefInputTag,AmbRefHandle) ; 

  //PG get the OLDIso electrons collection
  edm::Handle<electronCollection> OLDIsoHandle ;
  iEvent.getByLabel (m_OLDIsoInputTag,OLDIsoHandle) ; 

  //PG get the NEWIso electrons collection
  edm::Handle<electronCollection> NEWIsoHandle ;
  iEvent.getByLabel (m_NEWIsoInputTag,NEWIsoHandle) ; 

  //PG Get the tracks
  edm::Handle<trackCollection> TrackHandle ;
  iEvent.getByLabel (m_TrackInputTag, TrackHandle) ;

//  std::cout << "[VBFEleIsolationStudy][analyze] number of NEWIso electrons : "
//            << NEWIsoHandle->size () 
//            << std::endl ;

  //PG if not enough electrons
  if (AmbRefHandle->size () < 2) return ;
//  if (AmbRefHandle->size () > 2) return ;

  electronRef firstAmbRefEle ;
  electronRef secondAmbRefEle ;
  findFirstTwo (AmbRefHandle, firstAmbRefEle, secondAmbRefEle) ;
  if (firstAmbRefEle == secondAmbRefEle) return ; 

  int OLDcounter = 0 ;
  int NEWcounter = 0 ;

  //PG loop over GSF electrons
  for (unsigned GSFit = 0 ; GSFit < GSFHandle->size () ; ++GSFit)
    {
      electronBaseRef electronBaseReference = GSFHandle->refAt (GSFit) ;
      if (vbfhww2l::findInView (AmbRefHandle, electronBaseReference) == AmbRefHandle->end ()) continue ;
      electronRef electronReference = electronBaseReference.castTo<electronRef> () ;
      double isolationValueWithOtherCones = 
        m_tkIsolationAlgoWithOtherCones.calcIsolationValue (GSFHandle, TrackHandle, electronReference) ;
      std::cerr << "[VBFEleIsolationStudy][analyze] WITH "
                << electronReference->pt () 
                << " " << electronReference->eta () 
                << " " << electronReference->phi () 
                << " " << TrackHandle->size ()
                << " " << isolationValueWithOtherCones << std::endl ;
      m_NEWIsoValueW->Fill (isolationValueWithOtherCones) ;

      double isolationValueNoOtherCones = 
        m_tkIsolationAlgoNoOtherCones.calcIsolationValue (GSFHandle, TrackHandle, electronReference) ;
      std::cerr << "[VBFEleIsolationStudy][analyze] WITHOUT "
                << electronReference->pt () 
                << " " << electronReference->eta () 
                << " " << electronReference->phi () 
                << " " << TrackHandle->size ()
                << " " << isolationValueWithOtherCones << std::endl ;
      m_NEWIsoValueWO->Fill (isolationValueNoOtherCones) ;
    }

  //PG loop over GSF electrons
  for (unsigned GSFit = 0 ; GSFit < GSFHandle->size () ; ++GSFit)
    {
      electronRef electronReference = GSFHandle->refAt (GSFit).castTo<electronRef> () ;
      electronBaseRef electronBaseReference = GSFHandle->refAt (GSFit) ;
      
      //PG FIXME this I hope is working properly
      if (electronReference != firstAmbRefEle && electronReference != secondAmbRefEle) continue ;

      //PG check if isolated OLD style
      if (vbfhww2l::findInView (OLDIsoHandle, electronBaseReference) != OLDIsoHandle->end ()) ++OLDcounter ;
       
      //PG check if isolated NEW style
      if (vbfhww2l::findInView (NEWIsoHandle, electronBaseReference) != NEWIsoHandle->end ()) ++NEWcounter ;

      std::cerr << "[VBFEleIsolationStudy][analyze]     electron " << GSFit
                << " OLD " << OLDcounter 
                << " NEW " << NEWcounter << std::endl ;

    } //PG loop over electrons

  std::cerr << "[VBFEleIsolationStudy][analyze] summary GSF " << GSFHandle->size ()
            << " AmbRef " << AmbRefHandle->size ()
            << " OLD " << OLDcounter 
            << " NEW " << NEWcounter << std::endl ;

  float Dphi = vbfhww2l::deltaPhi (firstAmbRefEle->phi (), secondAmbRefEle->phi ()) ;
  m_OLDIsoEffvsDPhi->Fill (Dphi, OLDcounter) ;
  m_NEWIsoEffvsDPhi->Fill (Dphi, NEWcounter) ;

}


// --------------------------------------------------------------------


void 
VBFEleIsolationStudy::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_OLDIsoEffvsDPhi = fs->make<TH2F> ("OLDIsoEffvsDPhi","OLDIsoEffvsPhi",180,0,3.15,300,0,3) ;
  m_NEWIsoEffvsDPhi = fs->make<TH2F> ("NEWIsoEffvsDPhi","NEWIsoEffvsPhi",180,0,3.15,300,0,3) ;
  m_NEWIsoValueW = fs->make<TH1F> ("NEWIsoValueW","NEWIsoValueW",100,0,5) ;
  m_NEWIsoValueWO = fs->make<TH1F> ("NEWIsoValueWO","NEWIsoValueWO",100,0,5) ;
}


// --------------------------------------------------------------------


void 
VBFEleIsolationStudy::endJob () 
{
}


// --------------------------------------------------------------------


void 
VBFEleIsolationStudy::findFirstTwo (
    edm::Handle<electronCollection> & EleHandle ,
    electronRef & firstEle ,
    electronRef & secondEle) 
{
  double firstEleMaxPt = 0 ;
  double secondEleMaxPt = 0 ;

  //PG switch for electrons
  switch (EleHandle->size ())
    {
      case 0 :
        return ;
      case 1 :
//        firstEleMaxPt == GSFHandle->begin ()->pt () ;
        firstEle = EleHandle->refAt (0).castTo<electronRef> () ;
        secondEle = firstEle ;
        return ;
      default :
        //PG loop over electrons
        for (unsigned int eleIt = 0 ; eleIt < EleHandle->size () ; ++eleIt) 
          {          
            double pt = (*EleHandle)[eleIt].pt () ;
            if (firstEleMaxPt < pt)
              {
                secondEleMaxPt = firstEleMaxPt ;
                secondEle = firstEle ;
                firstEleMaxPt = pt ;
                firstEle = EleHandle->refAt (eleIt).castTo<electronRef> () ;
              }
            else if (secondEleMaxPt < pt)
              {
                secondEleMaxPt = pt ;
                secondEle = EleHandle->refAt (eleIt).castTo<electronRef> () ;
              }  
          } //PG loop over electrons 
    } //PG switch for electrons
}

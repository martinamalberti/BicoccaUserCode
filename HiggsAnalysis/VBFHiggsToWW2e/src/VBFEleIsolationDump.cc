// $Id: VBFEleIsolationDump.cc,v 1.1 2008/03/11 13:47:29 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleIsolationDump.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>


VBFEleIsolationDump::VBFEleIsolationDump (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_AmbRefInputTag (iConfig.getParameter<edm::InputTag> ("AmbRefInputTag")) ,
  m_TrackInputTag (iConfig.getParameter<edm::InputTag> ("trackInputTag")) ,
  m_tkIsolationAlgoWithOtherCones (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      iConfig.getParameter<double> ("otherVetoRadius") ,
      iConfig.getParameter<double> ("ptMin") ,
      iConfig.getParameter<double> ("lipMax") 
    ) ,
  m_tkIsolationAlgoNoOtherCones (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      0. ,
      iConfig.getParameter<double> ("ptMin") ,
      iConfig.getParameter<double> ("lipMax") 
    ) 
{}


// --------------------------------------------------------------------


VBFEleIsolationDump::~VBFEleIsolationDump ()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFEleIsolationDump::analyze (const edm::Event& iEvent, 
                               const edm::EventSetup& iSetup)
{

  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle ;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle) ; 

  //PG get the AmbRef electrons collection
  edm::Handle<electronCollection> AmbRefHandle ;
  iEvent.getByLabel (m_AmbRefInputTag,AmbRefHandle) ; 

  //PG Get the tracks
  edm::Handle<trackCollection> TrackHandle ;
  iEvent.getByLabel (m_TrackInputTag, TrackHandle) ;

//  std::cout << "[VBFEleIsolationDump][analyze] number of NEWIso electrons : "
//            << NEWIsoHandle->size () 
//            << std::endl ;

  //PG if not enough electrons
  if (AmbRefHandle->size () < 2) return ;
//  if (AmbRefHandle->size () > 2) return ;

  electronRef firstAmbRefEle ;
  electronRef secondAmbRefEle ;
  findFirstTwo (AmbRefHandle, firstAmbRefEle, secondAmbRefEle) ;
  if (firstAmbRefEle == secondAmbRefEle) return ; 

  //PG loop over GSF electrons
  for (unsigned ARit = 0 ; ARit < AmbRefHandle->size () ; ++ARit)
    {
      electronBaseRef electronBaseReference = AmbRefHandle->refAt (ARit) ;
      electronRef electronReference = electronBaseReference.castTo<electronRef> () ;

      //PG FIXME this I hope is working properly
      if (electronReference != firstAmbRefEle && electronReference != secondAmbRefEle) continue ;

      m_dumpIsoW->Fill (m_tkIsolationAlgoWithOtherCones.calcSumOfPt (GSFHandle, TrackHandle, electronReference), 
                        m_tkIsolationAlgoWithOtherCones.countNumOfTracks (GSFHandle, TrackHandle, electronReference),
                        electronReference->pt (),
                        electronReference->eta (),
                        electronReference->phi (),
                        electronReference->charge ()) ;
      m_dumpIsoWO->Fill (m_tkIsolationAlgoNoOtherCones.calcSumOfPt (GSFHandle, TrackHandle, electronReference), 
                         m_tkIsolationAlgoNoOtherCones.countNumOfTracks (GSFHandle, TrackHandle, electronReference),
                         electronReference->pt (),
                         electronReference->eta (),
                         electronReference->phi (),
                         electronReference->charge ()) ;
    }

}


// --------------------------------------------------------------------


void 
VBFEleIsolationDump::beginJob (const edm::EventSetup&)
{
  edm::Service<TFileService> fs ;
  m_dumpIsoW = fs->make <TNtuple> ("dumpIsoW","isol variables W cones",
                                   "sumTk:tkNum:elePt:eleEta:elePhi:eleQ") ;
  m_dumpIsoWO = fs->make <TNtuple> ("dumpIsoWO","isol variables WO cones",
                                    "sumTk:tkNum:elePt:eleEta:elePhi:eleQ") ;
}


// --------------------------------------------------------------------


void 
VBFEleIsolationDump::endJob () 
{
}


// --------------------------------------------------------------------


void 
VBFEleIsolationDump::findFirstTwo (
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

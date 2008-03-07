// $Id: VBFEleIsolationStudy.cc,v 1.1 2008/03/07 14:48:27 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleIsolationStudy.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

VBFEleIsolationStudy::VBFEleIsolationStudy (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_AmbRefInputTag (iConfig.getParameter<edm::InputTag> ("AmbRefInputTag")) ,
  m_OLDIsoInputTag (iConfig.getParameter<edm::InputTag> ("OLDIsoInputTag")) ,
  m_NEWIsoInputTag (iConfig.getParameter<edm::InputTag> ("NEWIsoInputTag")) 
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

  std::cout << "[VBFEleIsolationStudy][analyze] number of GSF electrons : "
            << GSFHandle->size () 
            << std::endl ;

  //PG get the AmbRef electrons collection
  edm::Handle<electronCollection> AmbRefHandle ;
  iEvent.getByLabel (m_AmbRefInputTag,AmbRefHandle) ; 

  std::cout << "[VBFEleIsolationStudy][analyze] number of AmbRef electrons : "
            << AmbRefHandle->size () 
            << std::endl ;

  //PG get the OLDIso electrons collection
  edm::Handle<electronCollection> OLDIsoHandle ;
  iEvent.getByLabel (m_OLDIsoInputTag,OLDIsoHandle) ; 

  std::cout << "[VBFEleIsolationStudy][analyze] number of OLDIso electrons : "
            << OLDIsoHandle->size () 
            << std::endl ;

  //PG get the NEWIso electrons collection
  edm::Handle<electronCollection> NEWIsoHandle ;
  iEvent.getByLabel (m_NEWIsoInputTag,NEWIsoHandle) ; 

  std::cout << "[VBFEleIsolationStudy][analyze] number of NEWIso electrons : "
            << NEWIsoHandle->size () 
            << std::endl ;

  int OLDcounter = 0 ;
  int NEWcounter = 0 ;


  electronRef firstAmbRefEle ;
  electronRef secondAmbRefEle ;
  findFirstTwo (AmbRefHandle, firstAmbRefEle, secondAmbRefEle) ;
  //PG if not enough electrons
  if (firstAmbRefEle == secondAmbRefEle) return ; 



  //PG loop over GSF electrons
  for (unsigned GSFit = 0 ; GSFit < GSFHandle->size () ; ++GSFit)
    {
      electronRef electronReference = GSFHandle->refAt (GSFit).castTo<electronRef> () ;
      electronBaseRef electronBaseReference = GSFHandle->refAt (GSFit) ;
      //PG match ambiguity resolved electrons
      if (findInView (AmbRefHandle, electronBaseReference) == AmbRefHandle->end ()) continue ;

      //PG check if isolated OLD style
      if (findInView (OLDIsoHandle, electronBaseReference) != OLDIsoHandle->end ()) ++OLDcounter ;
       
      //PG check if isolated NEW style
      if (findInView (NEWIsoHandle, electronBaseReference) == NEWIsoHandle->end ()) ++NEWcounter ;

    } //PG loop over electrons


}


// --------------------------------------------------------------------


void 
VBFEleIsolationStudy::beginJob (const edm::EventSetup&)
{
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

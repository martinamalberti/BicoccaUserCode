// user include files
#include "FakeRate/EffAnalysis/interface/testReferences.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "CLHEP/Vector/LorentzVector.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace reco;
using namespace std;
using namespace edm;
using namespace HepMC;

testReferences::testReferences(const edm::ParameterSet& conf) :
   m_genMetInputTag (conf.getParameter<edm::InputTag> ("genMetInputTag")) ,
   m_metInputTag (conf.getParameter<edm::InputTag> ("metInputTag")) ,
   m_rawGSFInputTag (conf.getParameter<edm::InputTag>("rawGSF")) ,
   m_ambiguityInputTag (conf.getParameter<edm::InputTag>("ambiguity")) ,
   m_tkIsoInputTag (conf.getParameter<edm::InputTag>("tkIso")) ,
   m_hadIsoInputTag (conf.getParameter<edm::InputTag>("hadIso")) ,
   m_eleIdInputTag (conf.getParameter<edm::InputTag>("eleId")) ,
   m_eleIdLooseInputTag (conf.getParameter<edm::InputTag>("eleIdLoose")) ,
   m_eleIdTightInputTag (conf.getParameter<edm::InputTag>("eleIdTight")) ,
   m_eleIdRobustInputTag (conf.getParameter<edm::InputTag>("eleIdRobust")) ,
   m_jetInputTag (conf.getParameter<edm::InputTag>("jet")) ,
   m_evtInputTag (conf.getParameter<edm::InputTag>("evt")) ,
   m_superClusterEBInputTag (conf.getParameter<edm::InputTag> ("EBsuperClusters")) ,
   m_superClusterEEInputTag (conf.getParameter<edm::InputTag> ("EEsuperClusters")) ,
   m_rawCounter (0) ,
   m_ambiguityCounter (0),
   m_tkIsoCounter (0),
   m_hadIsoCounter (0),
   m_eleIdCounter (0),
   m_eleIdLooseCounter (0),   
   m_eleIdTightCounter (0),   
   m_eventsCounter (0),
   m_deltaCone (conf.getUntrackedParameter<double> ("deltaCone",0.2)),
   m_diagCutParam (conf.getUntrackedParameter<double> ("diagCutParam",0.2)),
   m_jfi (conf.getParameter<edm::ParameterSet>("jetIdParameters")),
   m_rootfile(conf.getUntrackedParameter <std::string> ("rootfile")) 
{
}  


// ----------------------------------------------------------------------------


testReferences::~testReferences()
{
}


// ----------------------------------------------------------------------------


void testReferences::beginJob(edm::EventSetup const&iSetup)
{
    m_outfile  = new TFile(m_rootfile.c_str(), "RECREATE");
    m_minitree = new TTree("elminitree","elminitree");

    m_genMet4Momentum = new TLorentzVector (0.0,0.0,0.0,0.0) ;   
    m_recoMet4Momentum = new TLorentzVector (0.0,0.0,0.0,0.0) ; 
    
    m_minitree->Branch ("genMet4Momentum", "TLorentzVector", &m_genMet4Momentum, 6400,99); 
    m_minitree->Branch ("recoMet4Momentum", "TLorentzVector", &m_recoMet4Momentum, 6400,99); 
    m_minitree->Branch("elePT" ,m_elePT  ,"elePT[10]/D" ); 
    m_minitree->Branch("eleEta",m_eleEta ,"eleEta[10]/D");
    m_minitree->Branch("elePhi",m_elePhi ,"elePhi[10]/D");
    m_minitree->Branch("SCE" ,m_SCE ,"SCE[30]/D" ); 
    m_minitree->Branch("SCEta",m_SCEta ,"SCEta[30]/D");
    m_minitree->Branch("SCPhi",m_SCPhi ,"SCPhi[30]/D");
    m_minitree->Branch("eleCharge",m_eleCharge ,"eleCharge[10]/I");
    m_minitree->Branch("jetPT" ,m_jetPT  ,"jetPT[10]/D" ); 
    m_minitree->Branch("jetEta",m_jetEta ,"jetEta[10]/D");
    m_minitree->Branch("jetPhi",m_jetPhi ,"jetPhi[10]/D");
    m_minitree->Branch("jetFlav",m_jetFlav ,"eleFlav[30]/I");
    m_minitree->Branch("jetPTMatch" ,m_jetPTMatch  ,"jetPTMatch[10]/D" ); 
    m_minitree->Branch("jetEtaMatch",m_jetEtaMatch ,"jetEtaMatch[10]/D");
    m_minitree->Branch("jetPhiMatch",m_jetPhiMatch ,"jetPhiMatch[10]/D");
    m_minitree->Branch("SCEMatch" ,m_SCEMatch ,"SCEMatch[10]/D" ); 
    m_minitree->Branch("SCEtaMatch",m_SCEtaMatch ,"SCEtaMatch[10]/D");
    m_minitree->Branch("SCPhiMatch",m_SCPhiMatch ,"SCPhiMatch[10]/D");
    m_minitree->Branch("jetmaxPT" ,m_jetmaxPT  ,"jetmaxPT[10]/D" ); 
    m_minitree->Branch("jetmaxEta",m_jetmaxEta ,"jetmaxEta[10]/D");
    m_minitree->Branch("jetmaxPhi",m_jetmaxPhi ,"jetmaxPhi[10]/D");
    m_minitree->Branch("EMjetCompon",m_EMjetCompon ,"EMjetCompon[10]/D");
    m_minitree->Branch("bremFraction",m_bremFraction ,"bremFraction[10]/D");
    m_minitree->Branch("jetFlavour"    ,m_jetFlavour,   "jetFlavour[10]/I") ;   
    m_minitree->Branch("jetmaxFlavour" ,m_jetmaxFlavour,"jetmaxFlavour[10]/I") ;   
    m_minitree->Branch("rawBit"        ,m_rawBit,       "rawBit[10]/I") ;    
    m_minitree->Branch("ambiguityBit"  ,m_ambiguityBit, "ambiguityBit[10]/I") ; 
    m_minitree->Branch("tkIsoBit"      ,m_tkIsoBit,     "tkIsoBit[10]/I") ;      
    m_minitree->Branch("hadIsoBit"     ,m_hadIsoBit,    "hadIsoBit[10]/I") ;    
    m_minitree->Branch("eleIdBit"      ,m_eleIdBit,     "eleIdBit[10]/I") ;      
    m_minitree->Branch("eleIdLooseBit" ,m_eleIdLooseBit,"eleIdLooseBit[10]/I") ;
    m_minitree->Branch("eleIdTightBit" ,m_eleIdTightBit,"eleIdTightBit[10]/I") ;
    m_minitree->Branch("eleClass" ,m_eleClass,"eleClass[10]/I") ;
    m_minitree->Branch("ptHat" ,&m_ptHat,"ptHat/I") ;
    m_minitree->Branch("eleNum" ,&m_eleNum,"eleNum/I") ;
}     


// ----------------------------------------------------------------------------


void testReferences::endJob()
{
  m_outfile->Write();
  m_outfile->Close();
  
  std::cerr << "[DEBUG] ENDJOB RAW : " << m_rawCounter << " (" << m_rawCounter/static_cast<double> (2*m_eventsCounter) << ")"
            << " AMB : " << m_ambiguityCounter << " (" << m_ambiguityCounter/static_cast<double> (2*m_eventsCounter) << ")"
            << " TKI : " << m_tkIsoCounter << " (" << m_tkIsoCounter/static_cast<double> (2*m_eventsCounter) << ")"
            << " HAI : " << m_hadIsoCounter << " (" << m_hadIsoCounter/static_cast<double> (2*m_eventsCounter) << ")"
            << " EID : " << m_eleIdCounter << " (" << m_eleIdCounter/static_cast<double> (2*m_eventsCounter) << ")"
            << " EIDLoose : " << m_eleIdLooseCounter << " (" << m_eleIdLooseCounter/static_cast<double> (2*m_eventsCounter) << ")"
            << " EIDTight : " << m_eleIdTightCounter << " (" << m_eleIdTightCounter/static_cast<double> (2*m_eventsCounter) << ")"
            << std::endl ;
}


// ----------------------------------------------------------------------------

   
void testReferences::analyze (const edm::Event& iEvent, 
                              const edm::EventSetup& iSetup)
{


    *m_genMet4Momentum = TLorentzVector (0.0,0.0,0.0,0.0) ;   
    *m_recoMet4Momentum = TLorentzVector (0.0,0.0,0.0,0.0) ; 
   //PG reset the variables
   for (int ii = 0 ; ii < 30 ; ++ii)
     { 
        m_jetPT[ii] = 0 ;  
        m_jetEta[ii] = 0 ; 
        m_jetPhi[ii] = 0 ; 
        m_jetFlav[ii] = 0 ; 
        m_SCE[ii] = 0 ;  
        m_SCEta[ii] = 0 ; 
        m_SCPhi[ii] = 0 ; 
     }
   for (int ii = 0 ; ii < 10 ; ++ii)
     { 
        m_jetPTMatch[ii] = 0 ;  
        m_jetEtaMatch[ii] = 0 ; 
        m_jetPhiMatch[ii] = 0 ; 
        m_SCEMatch[ii] = 0 ;  
        m_SCEtaMatch[ii] = 0 ; 
        m_SCPhiMatch[ii] = 0 ; 
        m_rawBit[ii] = 0;
        m_elePT[ii] = 0 ;  
        m_eleEta[ii] = 0 ; 
        m_elePhi[ii] = 0 ; 
        m_eleCharge[ii] = 0 ; 
        m_jetmaxPT[ii] = 0 ;  
        m_jetmaxEta[ii] = 0 ; 
        m_jetmaxPhi[ii] = 0 ; 
        m_EMjetCompon[ii] = 0 ; 
        m_bremFraction [ii] = 0 ;
        m_jetFlavour[ii] = 0 ;  
        m_jetmaxFlavour[ii] = 0 ;  
        m_rawBit[ii] = 0 ;  
        m_ambiguityBit[ii] = 0 ;  
        m_tkIsoBit[ii] = 0 ;  
        m_hadIsoBit[ii] = 0 ; 
        m_eleIdBit[ii] = 0 ;  
        m_eleIdLooseBit[ii] = 0 ;  
        m_eleIdTightBit[ii] = 0 ;  
        m_eleClass[ii] = 0 ; 
        m_ptHat = -1 ;  
        m_eleNum = -1 ;
     }

  //take the collections
  typedef edm::RefVector<reco::PixelMatchGsfElectronCollection> GSFRefColl ;
  edm::Handle<PixelMatchGsfElectronCollection> rawGSFHandle ;
  iEvent.getByLabel (m_rawGSFInputTag,rawGSFHandle) ; 
  edm::Handle<GSFRefColl> ambiguityHandle ;
  iEvent.getByLabel (m_ambiguityInputTag,ambiguityHandle) ; 
  edm::Handle<GSFRefColl> tkIsoHandle ;
  iEvent.getByLabel (m_tkIsoInputTag,tkIsoHandle) ; 
  edm::Handle<GSFRefColl> hadIsoHandle ;
  iEvent.getByLabel (m_hadIsoInputTag,hadIsoHandle) ; 
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle;
  iEvent.getByLabel (m_eleIdInputTag, electronIDAssocHandle);
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDLooseAssocHandle;
  iEvent.getByLabel (m_eleIdLooseInputTag, electronIDLooseAssocHandle);
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDTightAssocHandle;
  iEvent.getByLabel (m_eleIdTightInputTag, electronIDTightAssocHandle);
  edm::Handle<reco::CaloJetCollection> jetHandle;
  iEvent.getByLabel (m_jetInputTag, jetHandle);
  edm::Handle<HepMCProduct> evtHandle;
  iEvent.getByLabel (m_evtInputTag, evtHandle);
  edm::Handle<reco::SuperClusterCollection> SCEBHandle;
  iEvent.getByLabel (m_superClusterEBInputTag, SCEBHandle);
  edm::Handle<reco::SuperClusterCollection> SCEEHandle;
  iEvent.getByLabel (m_superClusterEEInputTag, SCEEHandle);

  edm::Handle<reco::CaloMETCollection> metCollectionHandle ;
  iEvent.getByLabel (m_metInputTag, metCollectionHandle) ;
  const CaloMETCollection *calometcol = metCollectionHandle.product();
  const CaloMET *calomet = &(calometcol->front());  
  m_recoMet4Momentum->SetPx (calomet->px ()) ;
  m_recoMet4Momentum->SetPy (calomet->py ()) ;
  m_recoMet4Momentum->SetPz (calomet->pz ()) ;
  m_recoMet4Momentum->SetE  (calomet->energy ()) ;

  edm::Handle<reco::GenMETCollection> genMetCollectionHandle ;
  iEvent.getByLabel (m_genMetInputTag, genMetCollectionHandle) ;
  const GenMETCollection *genmetcol = genMetCollectionHandle.product () ;
  const GenMET *genmet = &(genmetcol->front ()) ;
  m_genMet4Momentum->SetPx (genmet->px ()) ;
  m_genMet4Momentum->SetPy (genmet->py ()) ;
  m_genMet4Momentum->SetPz (genmet->pz ()) ;
  m_genMet4Momentum->SetE  (genmet->energy ()) ;

   typedef reco::PixelMatchGsfElectron Object ;
   typedef reco::PixelMatchGsfElectronRef Ref ;
  
   m_jfi.readEvent (iEvent) ;  

   //Define the jet kind light: uds , heavy: bc , gluon  and the pT hat of the event
   HepMC::GenEvent * generated_event = new HepMC::GenEvent (*(evtHandle->GetEvent ())) ;
   //int ff[7] ;
   //fillComponentsVector (generated_event,ff) ;

   //Loop over jet collection
   int ii = 0;
   for (reco::CaloJetCollection::const_iterator iterJet = jetHandle->begin () ;
                                                iterJet!= jetHandle->end () ; 
                                                ++iterJet)
     {      
       if (ii < 30)
        {
          m_jetPT[ii]  = iterJet->pt () ;
          m_jetEta[ii] = iterJet->eta () ;
          m_jetPhi[ii] = iterJet->phi () ;
          JetFlavour jetFlavour = m_jfi.identifyBasedOnPartons(*(iterJet));
          m_jetFlav[ii] =  jetFlavour.flavour () ;
          ++ii ;
        }
     } //Loop over jet collection
   
   //Loop over EB SC collection
   ii = 0;
   for (reco::SuperClusterCollection::const_iterator iterSCEB = SCEBHandle->begin () ;
                                                     iterSCEB!= SCEBHandle->end () ; 
                                                     ++iterSCEB)
     {      
       if (ii < 30)
        {
          if (fabs (iterSCEB->eta ()) > 2.5) continue ;
          m_SCE[ii]  = iterSCEB->energy () ;
          m_SCEta[ii] = iterSCEB->eta () ;
          m_SCPhi[ii] = iterSCEB->phi () ;
          ++ii ;
        }
     } //Loop over EB SC collection

   m_ptHat = generated_event->event_scale();
   m_eleNum = rawGSFHandle->size () ;
   //PG loop on the raw collection
   for (int i = 0; i < rawGSFHandle->size () ; ++i) 
     {
      const reco::GsfTrack* gsfTrack = 
                    & (*((*rawGSFHandle)[i].gsfTrack ())) ;
      if (gsfTrack->numberOfValidHits () < 5 && i > 9) 
        {
          m_rawBit[i] = 0 ;
          continue ; //for the bug not fixed in CMSSW < 15X
        }
      m_rawBit[i] = 1 ;
      
      double  elePT  =  gsfTrack->pt () ; 
      double  eleEta =  (*rawGSFHandle)[i].eta () ;
      double  elePhi =  (*rawGSFHandle)[i].phi () ;
      m_elePT[i]  = elePT  ; 
      m_eleEta[i] = eleEta ;
      m_elePhi[i] = elePhi ;
      m_eleClass[i] = (*rawGSFHandle)[i].classification () ;
      m_eleCharge[i] = (*rawGSFHandle)[i].charge () ;
      double pin  = (*rawGSFHandle)[i].trackMomentumAtVtx().R();
      double pout = (*rawGSFHandle)[i].trackMomentumOut().R();
      m_bremFraction[i] = (pin - pout)/pin ;
      //double eleDiagCut = gsfTrack->EoP

      reco::CaloJetCollection::const_iterator closestJet ;
      reco::CaloJetCollection::const_iterator highestJet ;
      //match with the closest jet and take the phi eta pt of the jet
      double deltaRMin = 99999. ;
      const double pi = 3.14159 ;
      double  jetPT  = -1 ;
      double  jetEta = -99 ;
      double  jetPhi = -99 ; 
      double  jetmaxPT  = -1 ;
      double  jetmaxEta = -99 ;
      double  jetmaxPhi = -99 ; 
      for (reco::CaloJetCollection::const_iterator iterJet = jetHandle->begin () ;
                                                   iterJet!= jetHandle->end () ; 
                                                   ++iterJet)
          {      
             double deltaEta = iterJet->eta () - eleEta ;
             double deltaPhi = iterJet->phi () - elePhi ;
             if (deltaPhi > pi) deltaPhi -= 2.*pi ;
             if (deltaPhi < -pi) deltaPhi += 2.*pi ;         
             double deltaR = sqrt (deltaEta*deltaEta + deltaPhi*deltaPhi) ;
             if(deltaR < deltaRMin) 
              {
                deltaRMin = deltaR ;
                jetPT  = iterJet->pt () ;
                jetEta = iterJet->eta () ;
                jetPhi = iterJet->phi () ;
                closestJet = iterJet ;
              }
             if(iterJet->pt () > jetmaxPT) 
              {
                jetmaxPT  = iterJet->pt () ;
                jetmaxEta = iterJet->eta () ;
                jetmaxPhi = iterJet->phi () ;
                highestJet = iterJet ;
              }
          
      
      } //end fo the match

     


//     m_jetPT[i]  = jetPT ;
//     m_jetEta[i] = jetEta ;
//     m_jetPhi[i] = jetPhi ;
     m_jetPTMatch[i]  = jetPT ;
     m_jetEtaMatch[i] = jetEta ;
     m_jetPhiMatch[i] = jetPhi ;
     m_jetmaxPT[i]  = jetmaxPT ;
     m_jetmaxEta[i] = jetmaxEta ;
     m_jetmaxPhi[i] = jetmaxPhi ;
     //take the jet flavour
     JetFlavour jetFlavour = m_jfi.identifyBasedOnPartons(*(closestJet));
     m_jetFlavour[i] = jetFlavour.flavour () ;
     jetFlavour = m_jfi.identifyBasedOnPartons(*(highestJet));
     m_jetmaxFlavour[i] = jetFlavour.flavour () ;

     //Compute the EM component
     double jetEMMCComponent = getEMMCComponent (closestJet,generated_event,m_deltaCone) ;
     m_EMjetCompon[i] = jetEMMCComponent ;
     //int isResolved = 0 ;    
     //int isPassed = 0 ;    
     Ref ref (rawGSFHandle, i) ;
     
     if (find (ambiguityHandle->begin (), ambiguityHandle->end (), ref) != ambiguityHandle->end ())
       m_ambiguityBit[i] =  1 ;
     else
       m_ambiguityBit[i] = 0 ;

     if (find (tkIsoHandle->begin (), tkIsoHandle->end (), ref) != tkIsoHandle->end ())
       m_tkIsoBit[i] = 1 ;
     else
       m_tkIsoBit[i] = 0 ;

     if (find (hadIsoHandle->begin (), hadIsoHandle->end (), ref) != hadIsoHandle->end ())
       m_hadIsoBit[i] = 1 ;
     else
       m_hadIsoBit[i] = 0 ;
     
     reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr ;
     electronIDAssocItr = electronIDAssocHandle->find (ref) ;
     if (electronIDAssocItr == electronIDAssocHandle->end ()) continue ;
     const reco::ElectronIDRef& id = electronIDAssocItr->val ;
     bool cutBasedID = id->cutBasedDecision () ;
     if (cutBasedID == 1) 
       m_eleIdBit[i] = 1 ;
     else
       m_eleIdBit[i] = 0 ;

     reco::ElectronIDAssociationCollection::const_iterator electronIDLooseAssocItr ;
     electronIDLooseAssocItr = electronIDLooseAssocHandle->find (ref) ;
     if (electronIDLooseAssocItr == electronIDLooseAssocHandle->end ()) continue ;
     const reco::ElectronIDRef& idLoose = electronIDLooseAssocItr->val ;
     bool cutBasedIDLoose = idLoose->cutBasedDecision () ;
     if (cutBasedIDLoose == 1) 
       m_eleIdLooseBit[i] = 1 ;
     else
       m_eleIdLooseBit[i] = 0 ;

     reco::ElectronIDAssociationCollection::const_iterator electronIDTightAssocItr ;
     electronIDTightAssocItr = electronIDTightAssocHandle->find (ref) ;
     if (electronIDTightAssocItr == electronIDTightAssocHandle->end ()) continue ;
     const reco::ElectronIDRef& idTight = electronIDTightAssocItr->val ;
     bool cutBasedIDTight = idTight->cutBasedDecision () ;
     if (cutBasedIDTight == 1) 
       m_eleIdTightBit[i] = 1 ;
     else
       m_eleIdTightBit[i] = 0 ;

     } //PG loop on the raw collection

     m_minitree->Fill();
}


// ----------------------------------------------------------------------------


HepMC::GenParticle * testReferences::findParticle(const GenPartVect genPartVect,
                                              const int requested_id)
{
  for (GenPartVectIt p = genPartVect.begin(); p != genPartVect.end(); p++)
    {
      if (requested_id == (*p)->pdg_id()) return *p;
    }
  return 0;
}


// ----------------------------------------------------------------------------



//! cerco un vertice q-qbar per capire il flavour del getto
void testReferences::fillComponentsVector (HepMC::GenEvent * generated_event, int ff[7])
{
      vector<int> foundQ;
      HepMC::GenEvent::particle_iterator p;
       // loop over generated particles of the event
      for (p = generated_event->particles_begin() ; 
           p != generated_event->particles_end() ; 
           ++p) 
         {
       if (((*p)->pdg_id() < 1) && ((*p)->pdg_id() > -10) ) 
            { 
          // We have an anti-quark
              vector< GenParticle * > parents;
              HepMC::GenVertex* inVertex = (*p)->production_vertex();
              // build the list of parents frfo the vertex
              for(std::set<GenParticle*>::const_iterator iter = inVertex->particles_in_const_begin() ;
                                                         iter != inVertex->particles_in_const_end() ; 
                                                         ++iter)
                parents.push_back(*iter);
                // loop over parents
                for (GenPartVectIt z = parents.begin() ; 
                                   z != parents.end() ; 
                                  ++z)
                  {
                     vector< GenParticle * > child;
                     HepMC::GenVertex* outVertex = (*z)->end_vertex();
                     // fill childs vector
                     for(std::set<GenParticle*>::const_iterator iter = outVertex->particles_in_const_begin();
                         iter != outVertex->particles_in_const_end();iter++)
                       child.push_back(*iter);
                     // search childs vector  
                     if (findParticle(child, -(*p)->pdg_id())) foundQ.push_back(-(*p)->pdg_id());
                  }
            } //if (((*p)->pdg_id() < 1) && ((*p)->pdg_id() > -10)) 
        } //for (p = generated_event->particles_begin() ; 


  ff[0]=0; ff[1]=0; ff[2]=0; ff[3]=0; ff[4]=0; ff[5]=0; ff[6]=0; 
  for (vector<int>::iterator i = foundQ.begin(); i != foundQ.end(); i++)
  {
    ++ff[(*i)];
    if ((*i)>ff[0]) ff[0] = (*i);
  }
  return ;

}


// ----------------------------------------------------------------------------



//! Calcolo la componente EM di un jet a partire dal Montecarlo
double testReferences::getEMMCComponent (reco::CaloJetCollection::const_iterator iterJet ,  
                                         HepMC::GenEvent * myGenEvent, double deltaCone)
{
//    std::cerr << "[pietro] getEMcomponent " << deltaCone << std::endl ;
    double ptEMMC = 0 ;
    
    double etaJet = iterJet->eta();
    double phiJet = iterJet->phi();
    const double pi = 3.14159 ;
    
    std::vector<const HepMC::GenParticle*> takenEM ; 
    std::vector<const HepMC::GenParticle*>::const_iterator itPart ;

    double total_momentum[4] = {0,0,0,0} ;
    for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();   
                                                   p != myGenEvent->particles_end(); 
                           ++p ) {
      if (((*p)->status() == 1 && (*p)->pdg_id() == 22) ||       // gamma
          ((*p)->status() == 1 && abs((*p)->pdg_id()) == 11) ||  // electron
          (*p)->pdg_id() == 111 ||                 // pi0
          abs((*p)->pdg_id()) == 221 ||             // eta
          abs((*p)->pdg_id()) == 331 ||             // eta prime
          abs((*p)->pdg_id()) == 113 ||             // rho0
          abs((*p)->pdg_id()) == 223  )             // omega
        {
             // check if found is daughter of one already taken
             bool isUsed = false ;
             const HepMC::GenParticle * mother = (*p)->production_vertex() ?
                            *((*p)->production_vertex()->particles_in_const_begin()) : 0 ;
             const HepMC::GenParticle* motherMother = (mother != 0  && mother->production_vertex()) ?
                            *(mother->production_vertex()->particles_in_const_begin()) : 0 ;
             const HepMC::GenParticle* motherMotherMother = (motherMother != 0 && motherMother->production_vertex()) ?
                            *(motherMother->production_vertex()->particles_in_const_begin()) : 0 ;
             for(itPart = takenEM.begin(); itPart != takenEM.end(); ++itPart) {
             if ((*itPart) == mother ||
                 (*itPart) == motherMother ||
                 (*itPart) == motherMotherMother) 
                 {
                   isUsed = true ;
                   break ;     
                 }
              }
        
             if (!isUsed)
               {
                 takenEM.push_back(*p) ;
                 HepMC::FourVector dummy = (*p)->momentum () ;
                 double pt  = (*p)->momentum().perp();
                 double eta = (*p)->momentum().eta() ;
                 double phi = (*p)->momentum().phi() ;
            
                 double deltaEta =  eta - etaJet ; 
                 double deltaPhi =  phi - phiJet ;
                 if(deltaPhi > pi) deltaPhi -= 2.*pi ;
                 if(deltaPhi < -pi) deltaPhi += 2.*pi ;         
                 double deltaR = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
                 if(deltaR < deltaCone) 
                   {             
                     ptEMMC  += pt ;
                     total_momentum[0] += dummy.x () ;
                     total_momentum[1] += dummy.y () ;
                     total_momentum[2] += dummy.z () ;
                     total_momentum[3] += dummy.t () ;
                   }
               } //if !isUsed
         } //if
     } //for
  HepMC::FourVector dummy (total_momentum[0],total_momentum[1],total_momentum[2],total_momentum[3]) ;
  return dummy.perp () ;
}

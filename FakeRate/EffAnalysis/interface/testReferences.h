#ifndef testReferences_h
#define testReferences_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"

#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

//LM aggiunte per i weight
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "TBranch.h"
#include <string>

class testReferences : public edm::EDAnalyzer
{


  typedef struct {
    Int_t procId;
    Float_t ptHat;
    Float_t filterEff;
    Float_t weight;
    Int_t trigBits[90];
  } CSA07Info;

 public:
 
   explicit testReferences(const edm::ParameterSet& conf);
   
   virtual ~testReferences();
   virtual void beginJob(edm::EventSetup const& iSetup);
   virtual void endJob();
   virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
 
 private:

    bool runOnChowder_;
    std::string rootFileName_;
    TFile * rootFile_;
    TBranch * csa07B_;
    CSA07Info csa07Info_;
 
   typedef std::vector< HepMC::GenParticle * > GenPartVect;
   typedef std::vector< HepMC::GenParticle * >::const_iterator GenPartVectIt;
   HepMC::GenParticle * findParticle(const GenPartVect genPartVect, const int requested_id);
   void fillComponentsVector (HepMC::GenEvent * generated_event, int ff[7]) ;
   double getEMMCComponent (reco::CaloJetCollection::const_iterator iterJet,
                            HepMC::GenEvent * generated_event, double deltaCone = 0.2);
  
   edm::InputTag m_genMetInputTag ;
   edm::InputTag m_metInputTag ;
   edm::InputTag m_rawGSFInputTag ;
   edm::InputTag m_ambiguityInputTag ;
   edm::InputTag m_tkIsoInputTag ; 
//   edm::InputTag m_hadIsoInputTag ; 
   edm::InputTag m_EcalIsolationProducer_ ; 
   edm::InputTag m_HcalIsolationProducer_ ; 
   edm::InputTag m_eleIdInputTag ; 
   edm::InputTag m_eleIdLooseInputTag ; 
   edm::InputTag m_eleIdTightInputTag ; 
   edm::InputTag m_eleIdRobustInputTag ; 
   edm::InputTag m_jetInputTag ;
   edm::InputTag m_evtInputTag ;
   edm::InputTag m_barrelClusterShapeAssocTag ;
   edm::InputTag m_endcapClusterShapeAssocTag ;
   edm::InputTag m_superClusterEBInputTag ;
   edm::InputTag m_superClusterEEInputTag ;
//MCTruth
   edm::InputTag matchMap_ ;
   int m_rawCounter ;
   int m_ambiguityCounter ;
   int m_tkIsoCounter ;
//   int m_hadIsoCounter ;
   int m_ecalIsoCounter ;
   int m_hcalIsoCounter ;
   int m_eleIdCounter ;   
   int m_eleIdLooseCounter ;   
   int m_eleIdTightCounter ;   
   int m_eventsCounter ;   
   double m_deltaCone ;
   double m_diagCutParam ;

   //Jet Flavour Identifier from BTau
   JetFlavourIdentifier m_jfi ;
   
   TFile * m_outfile;
   TTree * m_minitree;
   std::string m_rootfile;
   TLorentzVector * m_genMet4Momentum ; 
   TLorentzVector * m_recoMet4Momentum ; 
   double m_elePT[10] ;  
   double m_eleEta[10] ; 
   double m_elePhi[10] ;
   double m_eleHE[10] ;
   double m_eleDeltaEta[10] ;
   double m_eleDeltaPhi[10] ;    
   double m_eleSigmaEtaEta[10] ;
   int    m_eleIsBarrel[10];
   double m_SCE[30] ;  
   double m_SCEta[30] ; 
   double m_SCPhi[30] ; 
   int    m_eleCharge[10] ; 
   double m_jetPT[30] ;  
   double m_jetEta[30] ; 
   double m_jetPhi[30] ;    
   int    m_jetFlav[30] ;    
   double m_jetPTMatch[10] ;  
   double m_jetEtaMatch[10] ; 
   double m_jetPhiMatch[10] ; 
   double m_SCEMatch[10] ;  
   double m_SCEtaMatch[10] ; 
   double m_SCPhiMatch[10] ; 
   double m_jetmaxPT[10] ;  
   double m_jetmaxEta[10] ; 
   double m_jetmaxPhi[10] ; 
   double m_EMjetCompon[10] ; 
   double m_bremFraction[10] ; 
   int    m_jetFlavour[10] ;  
   int    m_jetmaxFlavour[10] ;  
   int    m_rawBit[10] ;  
   int    m_ambiguityBit[10] ;  
   int    m_tkIsoBit[10] ;  
//   int    m_hadIsoBit[10] ; 
   int    m_ecalIsoBit[10] ;
   int    m_hcalIsoBit[10] ;
   int    m_eleIdBit[10] ;  
   int    m_eleIdLooseBit[10] ;  
   int    m_eleIdTightBit[10] ;  
   int    m_eleClass[10] ;  
   double    m_ptHat ;
   int    m_eleNum ; 
   int    m_jetNum ;
   int    m_SCNum ; 
   
//MCTruth
  double  m_eleTruthEta[10] ;
  int     m_dimensioneMappa ;
} ;

#endif

#ifndef VBFElectronAmbiguityResolverAnalyzer_h
#define VBFElectronAmbiguityResolverAnalyzer_h

/** \class VBFElectronAmbiguityResolverAnalyzer
 *
 * Object selector perform electron track isolation selection
 *
 */  

#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"



#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"

#include "TTree.h"

class VBFElectronAmbiguityResolverAnalyzer : public edm::EDAnalyzer {
 public:
  
  explicit VBFElectronAmbiguityResolverAnalyzer (const edm::ParameterSet&);
  ~VBFElectronAmbiguityResolverAnalyzer () ;
  
  typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
 
  
 private:  
  
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  TTree *mytree_;
  double DR_;
  double minMC_EnEn_1_;
  double minMC_EnEn_2_;

  double EoP_1_;
  double EoP_2_;

  double pt_1_;
  double pt_2_;
  
  double minMC_DR_1_;
  double minMC_DR_2_;
  
  TTree *mytree_event_;
  int numAmb_2_;
  int numAmb_3_;
  int numAmb_4_;
  int numAmb_5_;
  
  
  TTree *mytree_ele_;
  int numAmb_;
  int numAmb_SC_;
  int numAmb_Tk_;

  int numAmb_SC_pt_;
  int numAmb_Tk_pt_;
  
  double pt_;
  
  
  //---- input tags ----
  
  edm::InputTag m_GSFInputTag ;
  
};
  
#endif
 



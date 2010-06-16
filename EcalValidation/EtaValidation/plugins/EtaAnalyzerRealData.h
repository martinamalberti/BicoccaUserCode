#ifndef EcalValidation_EtaValidation_EtaAnalyzerRealData_h
#define EcalValidation_EtaValidation_EtaAnalyzerRealData_h
/**\class EtaAnalyzerRealData
 **
 ** Description: Get Photon collection from the event and make very basic histos
 ** $Date: 2009/12/07 12:52:42 $ 
 ** $Revision: 1.1 $
 ** \author Andrea Massironi
 **
 **/
//
//


// system include files
#include <memory>

// Geometry
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include <string>
#include "TH1.h"
#include "TProfile.h"

#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

class EtaAnalyzerRealData : public edm::EDAnalyzer {
 public:
  explicit EtaAnalyzerRealData( const edm::ParameterSet& );
  ~EtaAnalyzerRealData();


  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
 
  int diff_neta_s(int,int);
  int diff_nphi_s(int,int);

  
 private:

  std::map<std::string,double> providedParameters;  
  bool ParameterLogWeighted_;
  double ParameterX0_;
  double ParameterT0_barl_;
  double ParameterT0_endc_;
  double ParameterT0_endcPresh_;
  double ParameterW0_;
  double ParameterE0_;
  
  PositionCalc posCalculator_;
  
  
  std::string ecalHitsProducer_;
  std::string barrelHits_;
      
  edm::InputTag dummy_;
  
  double seleXtalMinEnergy_;
  double clusSeedThr_;
  
 
 
  bool isMC_;
  std::string HepMCProducer_;
    
  bool debug_;
  
  TTree *mytree_;
  std::vector<float> * E1_ ;
  std::vector<float> * E9_ ;
  std::vector<float> * E25_ ;
  std::vector<float> * E4o9_ ;
  
  TClonesArray * photons_ ;
  
  TClonesArray * mc_photons_ ;
  TClonesArray * mc_photons_3pi0_ ;
  TClonesArray * mc_eta_ ;
  
  TH1F* hEnergy_;



};
#endif

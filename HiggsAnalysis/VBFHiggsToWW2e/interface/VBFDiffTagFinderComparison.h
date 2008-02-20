// -*- C++ -*-
//
// Package:    VBFDiffTagFinderComparison
// Class:      VBFDiffTagFinderComparison
// 
/**\class VBFDiffTagFinderComparison VBFDiffTagFinderComparison.cc HiggsAnalysis/VBFDiffTagFinderComparison/src/VBFDiffTagFinderComparison.cc

 Description: <one line class summary>
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Valentina Tancini
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFDiffTagFinderComparison.h,v 1.2 2008/02/15 15:52:19 tancini Exp $
//
//

#ifndef VBFDiffTagFinderComparison_H
#define VBFDiffTagFinderComparison_H

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"
#include "SimDataFormats/HepMCProduct/interface/GenInfoProduct.h"

//root include
#include <TH1F.h>

class VBFDiffTagFinderComparison : public edm::EDAnalyzer {

  typedef reco::CaloJetCollection::const_iterator VBFjetIt ;

  public:
    
      //! ctor
      explicit VBFDiffTagFinderComparison (const edm::ParameterSet&);
      //! dtor
      ~VBFDiffTagFinderComparison ();

   private:

      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();

   private:

      edm::InputTag m_jetInputTag;
      double m_jetPtMin;
      double m_jetEtaMax;
      TH1F *m_deltaR ;
      TH1F *m_HEoverEmE ;
      TH1F *m_EleOJetEratio ;
};

#endif

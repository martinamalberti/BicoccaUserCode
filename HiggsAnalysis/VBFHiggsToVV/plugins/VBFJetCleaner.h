#ifndef VBFJetCleaner_h
#define VBFJetCleaner_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include <iostream>
#include <algorithm>
#include <Math/VectorUtil.h>






template <class TCollection>
class VBFJetCleaner
{
 public:

  typedef reco::GsfElectronCollection electronCollection;
  typedef reco::GsfElectronRef electron;
  
  typedef TCollection collection;
  typedef edm::Ref<collection> jet;
  typedef edm::RefVector<collection> container;
  typedef typename edm::RefVector<collection>::const_iterator const_iterator;  
  
  
 public:
  
  //! ctor
  VBFJetCleaner(const edm::ParameterSet& conf);
  
  //! dtor
  ~VBFJetCleaner();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }
  
  //! iterator to the end of the selected collection
  const_iterator end () const { return m_selected.end(); }
  
  //! the actual selector method 
  void select (edm::Handle<collection>, const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;
  
  edm::InputTag m_srcElectrons;
  edm::InputTag m_srcElectronIdValues;
  
  double m_tkIsoCut;
  double m_maxDeltaR;
  double m_minEleOverJetEratio;
  
  bool m_doJetRefCheck;
  edm::InputTag m_srcJetsRef;
  bool m_doElectronRefCheck;
  edm::InputTag m_srcElectronsRef;
};
  





template <class TCollection>
VBFJetCleaner<TCollection>::VBFJetCleaner(const edm::ParameterSet& iConfig):
  m_srcElectrons          (iConfig.getParameter<edm::InputTag>("srcElectrons")),
  m_srcElectronIdValues   (iConfig.getParameter<edm::InputTag>("srcElectronIdValues")),
  m_tkIsoCut           (iConfig.getParameter<double>("tkIsoCut")), 
  m_maxDeltaR          (iConfig.getParameter<double>("maxDeltaR")),
  m_minEleOverJetEratio(iConfig.getParameter<double>("minEleOJetEratio")),
  m_doJetRefCheck     (iConfig.getParameter<bool>("doJetRefCheck")),
  m_srcJetsRef        (iConfig.getParameter<edm::InputTag>("srcJetsRef")),
  m_doElectronRefCheck(iConfig.getParameter<bool>("doElectronRefCheck")),
  m_srcElectronsRef   (iConfig.getParameter<edm::InputTag>("srcElectronsRef"))
{}  

// ----------------------------------------------------------------------------






template <class TCollection>
VBFJetCleaner<TCollection>::~VBFJetCleaner()
{}

// ----------------------------------------------------------------------------





  
template <class TCollection> 
void VBFJetCleaner<TCollection>::select(edm::Handle<VBFJetCleaner<TCollection>::collection> jets, 
                                        const edm::Event& iEvent,
                                        const edm::EventSetup& iSetup)
{
  m_selected.clear ();
  
  
  
  //Get the electrons collection
  edm::Handle<electronCollection> electrons; 
  iEvent.getByLabel(m_srcElectrons, electrons); 
  
  edm::Handle< edm::RefVector<collection> > jetsRef;
  if(m_doJetRefCheck)
    iEvent.getByLabel(m_srcJetsRef, jetsRef);  

  edm::Handle< edm::RefVector<electronCollection> > electronsRef;
  if(m_doElectronRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);  
  
  // Get the electronid maps
  edm::Handle<edm::ValueMap<float> > electronIdValues;
  iEvent.getByLabel(m_srcElectronIdValues, electronIdValues);
  
  
  
  
  
  
  //PG loop over jets
  for(unsigned int j = 0; j < jets -> size(); ++j)
  {
    // do the reference check 
    bool isJetRefCheckOk = true;
    jet jetRef(jets, j);
    if(m_doJetRefCheck)
      if(find(jetsRef -> begin(), jetsRef -> end(), jetRef) == jetsRef -> end())
        isJetRefCheckOk = false;
    
    if(!isJetRefCheckOk) continue;
    
    
    
    //PG loop over electrons    
    double electronsEnergy = 0.;
    int counter = 0;
    for(unsigned int i = 0; i < electrons -> size(); ++i)
    {
      // do the reference check 
      bool isElectronRefCheckOk = true;
      electron electronRef(electrons, i);
      if(m_doElectronRefCheck)
        if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
          isElectronRefCheckOk = false;
      
      if(!isElectronRefCheckOk) continue;
      
      
      
      //This is not an electron - ISOLATION
          
      // get the isolation deposits
      double tkIso  = (electrons -> at(i)).dr03TkSumPt();
      tkIso  /= electronRef -> pt();
      
      // do the actual cut
      if(tkIso  > m_tkIsoCut) continue;
      
      
      
      //This is not an electron - ELECTRON ID

      // get the electron id values
      double eleId  = (*electronIdValues)[electronRef];
        
      // do the actual cut
      if(eleId < 1) continue;
      
      
      
      
      
      
      double dR = deltaR(electronRef -> eta(), electronRef -> phi(),
                         jetRef -> eta(), jetRef -> phi());
      
      if (dR < m_maxDeltaR)
      {
        ++counter;
        electronsEnergy += electronRef -> caloEnergy();
      }          
      
    } //PG loop over electrons
    
    
    if ( (counter) && ((electronsEnergy / jetRef -> energy()) > m_minEleOverJetEratio) )
      continue;
    
    
    m_selected.push_back(jetRef);
 } //PG loop over jets   

}

// ----------------------------------------------------------------------------

#endif
 



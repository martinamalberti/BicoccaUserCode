#ifndef VBFEleAmbiguityResolver_h
#define VBFEleAmbiguityResolver_h

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtraFwd.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <functional>
#include <vector>
#include <map>


class VBFEleAmbiguityResolver{

 public:
  
  typedef edm::Ref<std::vector<reco::GsfElectron> > electron ;
  typedef std::vector<reco::GsfElectron> collection ;
  typedef std::vector<electron>  container ;
  typedef container::const_iterator const_iterator ;

  
  //! ctor
  VBFEleAmbiguityResolver (const edm::ParameterSet& conf) ;
  //!dtor
  ~VBFEleAmbiguityResolver () ;

  //! iterator to the begin of the selected collection
  const_iterator begin () const { return m_selected.begin () ; }
  
  //! iterator to the end of the selected collection
  const_iterator end () const { return m_selected.end () ; }
  //! the actual selector method 
  void select (edm::Handle<collection>, 
               const edm::Event&, const edm::EventSetup& ) ;
  
  //! to sort the GSF electrons std::vector according to the E/p ratio and E
  //! if the superclusters are different, the sorting is performed by the supercluster raw energy
  //! if the superclusters are the same, the sorting is performed by using the |E/P - 1| function
  struct SCEeEoPSorting : public std::binary_function<electron, electron, bool> 
  {
   bool operator() (electron ele1, electron ele2) 
   { 
    if ( &(*(ele1->superCluster())) == &(*(ele2->superCluster()))  )
     return fabs(ele1->eSuperClusterOverP ()-1) < 
       fabs(ele2->eSuperClusterOverP ()-1) ;
    return (ele1->superCluster ())->rawEnergy () < 
      (ele2->superCluster ())->rawEnergy () ;
   }
  };
        

  //! to sort the GSF electrons std::vector according to the E/p ratio and TK
  //! if the tracks are different, the sorting is performed by the p() of the tk
  //! if the tracks are the same, the sorting is performed by using the |E/P - 1| function
  struct TKeEoPSorting : public std::binary_function<electron, electron, bool> 
  {
   bool operator() (electron ele1, electron ele2) 
   { 
    if (ele1->gsfTrack () == ele2->gsfTrack ())         
     return fabs(ele1->eSuperClusterOverP ()-1) < 
       fabs(ele2->eSuperClusterOverP ()-1) ;
    return (ele1->gsfTrack ())->p () < (ele2->gsfTrack ())->p () ;
   }
  };
      

  //! check if two GSF electrons refer to the same track
  struct equalTk : public std::binary_function<electron, electron, bool> 
  {
   bool operator() (electron ele1, electron ele2) 
   { return ele1->gsfTrack () == ele2->gsfTrack () ; }
  };


  //! check if two GSF electrons contain the same supercluster
  struct equalSC : public std::binary_function<electron, electron, bool> 
  {      bool operator() (electron ele1, electron ele2) 
  { return ( &(*(ele1->superCluster())) == &(*(ele2->superCluster()))  ); }
  };
  

 private:
  //! the selected collection
  container m_selected ;
  //! fill the ouptut with the pointers to the elements of the input
  void dump (container * output, const edm::Handle<collection> & input) ;
 
};  

#endif

/* FIXME

nn mi e' chiara la funzione (o l'uso) del m_selected, perche'
c'e' il container di puntatori? come funziona il selector dell'object?
e' solo un trucco per non perdere tempo a copiare?
NON riesco a capire esattamente che cosa dovrebbe fare! 
se poi ritorna iteratori, a chi puntano questi?

*/

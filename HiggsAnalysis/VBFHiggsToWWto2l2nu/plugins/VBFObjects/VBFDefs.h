#ifndef __VBFDEFS_H__
#define __VBFDEFS_H__


#include "Math/Point3D.h"
#include "Math/GenVector/CoordinateSystemTags.h"

typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double> > Point;


#include "Math/PxPyPzE4D.h"
#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;


namespace vbfhww2l{

  enum LeptonID{
    ELECTRON,
    MUON,
    TAU
  };

}
#endif

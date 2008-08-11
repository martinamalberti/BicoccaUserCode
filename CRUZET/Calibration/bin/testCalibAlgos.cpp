#include <vector>
#include <string>
#include <sstream>
#include <fstream>

//PG for the cluster-based calibration 
#include "Calibration/EcalCalibAlgos/interface/VEcalCalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/IMACalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/L3CalibBlock.h"



int main (int argc, char** argv)
{
  std::vector<VEcalCalibBlock*> calibBlocks ;
  calibBlocks.push_back (new IMACalibBlock (10)) ;
  calibBlocks.push_back (new L3CalibBlock (10, 1.)) ;

//  new IMACalibBlock (m_regions.at (region))
//  new L3CalibBlock (m_regions.at (region), eventWeight)

  return 0 ;

}

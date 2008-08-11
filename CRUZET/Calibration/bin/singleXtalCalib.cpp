#include <map>
#include <string>
#include <sstream>
#include <fstream>

#include "TH1.h"

//PG for the cluster-based calibration 
#include "Calibration/EcalCalibAlgos/interface/VEcalCalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/IMACalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/L3CalibBlock.h"



int main (int argc, char** argv)
{
  std::map<int, TH1F *> dEdx_Histos ;

  int nEntries = -1 ;

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      //PG loop over crystals <-- in case the calib is based on single
      //PG                        xtal deposit
    
      //PG find a unique index for each xtal
      
      //PG verify if the xtal index is already present in the dEdx_Histos map
      //PG    (if not, add it)                   
      
      //PG fill the histo with the E/p for the xtal 
    
    } //PG loop over entries

  //PG loop over dEdx_Histos
  for (std::map<int, TH1F*>::iterator mapIt = dEdx_Histos.begin () ;
       mapIt != dEdx_Histos.end () ;
       ++mapIt)
    {
      mapIt->second->Fit ("gaus") ;
    } //PG loop over dEdx_Histos

  return 0 ;

}

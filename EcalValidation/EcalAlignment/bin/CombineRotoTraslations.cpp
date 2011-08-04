///===================================
///==== c++ -o CombineRotoTraslations.exe CombineRotoTraslations.cpp `root-config --cflags --glibs` -lMinuit `clhep-config --libs`
///===================================

#include "TString.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TEntryList.h"
#include "TMinuit.h"
#include "TF2.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "DataFormats/Math/interface/deltaPhi.h"

//==== parameter include ====
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//---- std include ----
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>


///==== CLHEP ====
#include "CLHEP/Vector/EulerAngles.h"
#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Transform3D.h"





int main(int argc, char** argv)
{

 std::cout << "                                                                                                                                                                   " << std::endl;
 std::cout << "                                                                                                                                                                   " << std::endl;
 std::cout << "      ___|                    |     _)                    _ \\          |           |                      |         |   _)                                        " << std::endl;           
 std::cout << "     |       _ \\   __ `__ \\   __ \\   |  __ \\    _ \\      |   |   _ \\   __|   _ \\   __|   __|  _` |   __|  |   _` |  __|  |   _ \\   __ \\    __|            " << std::endl;
 std::cout << "     |      (   |  |   |   |  |   |  |  |   |   __/      __ <   (   |  |    (   |  |    |    (   | \\__ \\  |  (   |  |    |  (   |  |   | \\__ \\                 " << std::endl;
 std::cout << "    \\____| \\___/  _|  _|  _| _.__/  _| _|  _| \\___|     _| \\_\\ \\___/  \\__| \\___/  \\__| _|   \\__,_| ____/ _| \\__,_| \\__| _| \\___/  _|  _| ____/        " << std::endl;
 std::cout << "                                                                                                                                                                   " << std::endl;
 std::cout << "                                                                                                                                                                   " << std::endl;
 
 if (argc < 4) {
  std::cout << " Error: Code usage: " << std::endl;
  std::cout << "     CombineRotoTraslations  InputPositionsFile  AdditionalPositionsFile  OutputPositionsFile " << std::endl;
  return false;
 }

 std::string filePositionsInput     (argv[1]) ;
 std::string filePositionsAddition  (argv[2]) ;
 std::string filePositionsOutput    (argv[3]) ;

 std::cout << " InputPositionsFile      = " << argv[1] << std::endl;
 std::cout << " AdditionalPositionsFile = " << argv[2] << std::endl;
 std::cout << " OutputPositionsFile     = " << argv[3] << std::endl;

 std::ifstream* fileIn;
 fileIn = new std::ifstream(filePositionsInput.c_str());
 if(!fileIn->is_open()){
  return false;
 }
 
 std::ifstream* fileInAddition;
 fileInAddition = new std::ifstream(filePositionsAddition.c_str());
 if(!fileInAddition->is_open()){
  return false;
 }
 
 double inputDX = 0;
 double inputDY = 0;
 double inputDZ = 0;
 double inputDPHIe = 0;
 double inputDTHETAe = 0;
 double inputDPSIe = 0; 
 
 double inputDX_addition = 0;
 double inputDY_addition = 0;
 double inputDZ_addition = 0;
 double inputDPHIe_addition = 0;
 double inputDTHETAe_addition = 0;
 double inputDPSIe_addition = 0; 
 

 ofstream outFile;
 outFile.open (filePositionsOutput.c_str());

 std::string buffer;

 while(!fileInAddition->eof()){
  getline(*fileIn,buffer);
  if (buffer == "") break; ///---> save from empty line at the end!

  std::stringstream line( buffer );

  line >> inputDPHIe;
  line >> inputDTHETAe;
  line >> inputDPSIe;
  line >> inputDX;  inputDX /= 100.;
  line >> inputDY;  inputDY /= 100.;
  line >> inputDZ;  inputDZ /= 100.; 

  std::cout << " " << inputDPHIe << " " << inputDTHETAe << " " << inputDPSIe ;
  std::cout << " " << inputDX << " " << inputDY << " " << inputDZ << " ===> " ;
  
  getline(*fileInAddition,buffer);
  std::stringstream lineAddition( buffer );

  lineAddition >> inputDPHIe_addition;
  lineAddition >> inputDTHETAe_addition;
  lineAddition >> inputDPSIe_addition;
  lineAddition >> inputDX_addition;    inputDX_addition /= 100.;
  lineAddition >> inputDY_addition;    inputDY_addition /= 100.; 
  lineAddition >> inputDZ_addition;    inputDZ_addition /= 100.; 
  
  std::cout << " " << inputDPHIe_addition << " " << inputDTHETAe_addition << " " << inputDPSIe_addition ;
  std::cout << " " << inputDX_addition << " " << inputDY_addition << " " << inputDZ_addition << std::endl;

  CLHEP::Hep3Vector translation(inputDX,inputDY,inputDZ);
  CLHEP::HepEulerAngles euler(inputDPHIe, inputDTHETAe, inputDPSIe);
  CLHEP::HepRotation rotation(euler);
  HepGeom::Transform3D transform( rotation, translation );
 
  CLHEP::Hep3Vector translation_addition(inputDX_addition,inputDY_addition,inputDZ_addition);
  CLHEP::HepEulerAngles euler_addition(inputDPHIe_addition, inputDTHETAe_addition, inputDPSIe_addition);
  CLHEP::HepRotation rotation_addition(euler_addition);
  HepGeom::Transform3D transform_addition( rotation_addition, translation_addition );
 
  HepGeom::Transform3D Combination = transform_addition * transform;
  CLHEP::Hep3Vector translation_Combination = Combination.getTranslation();
  CLHEP::HepRotation rotation_Combination = Combination.getRotation();

  outFile << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << rotation_Combination.getPhi() << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << rotation_Combination.getTheta() << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << rotation_Combination.getPsi() << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100 * translation_Combination.getX() << " "
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100 * translation_Combination.getY() << " "
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100 * translation_Combination.getZ() << " "
  << std::endl;

 }
 
 outFile.close(); 
 
 std::cout << std::endl << " --- end --- " << std::endl << std::endl;
}




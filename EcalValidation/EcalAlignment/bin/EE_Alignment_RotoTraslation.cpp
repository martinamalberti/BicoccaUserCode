///===================================
///==== c++ -o EE_Alignment_RotoTralsation.exe EE_Alignment_RotoTralsation.cpp `root-config --cflags --glibs` -lMinuit `clhep-config --libs`
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

TF2* FunctionDeta;
TF2* FunctionDphi;

TChain* myTree;
TString globalCut;
double Z = 3.205;

bool even = true;
bool odd = true;

///====================================================
///================== FUNCTIONS =======================

double Chi2Func(const double *xx ){
 const Double_t DX = xx[0];
 const Double_t DY = xx[1];
 const Double_t DZ = xx[2];
 
 const Double_t DPHIEuler = xx[3]; ///==== Euler angles
 const Double_t DTHETAEuler = xx[4];
 const Double_t DPSIEuler = xx[5];
 
///  The transform rotates first, then translates.
///  The Euler angles are gotten from HepRotation::eulerAngles()
///  (a HepTransform3D is a HepRotation and a HepTranslation).
 
 CLHEP::HepEulerAngles RotMat(DPHIEuler,DTHETAEuler,DPSIEuler);
 
 HepGeom::Transform3D RotoTrasl( RotMat,
		                 CLHEP::Hep3Vector(DX,DY,DZ));

 myTree->SetEntryList(0); 
 myTree->Draw(">> myList",globalCut.Data(),"entrylist");
 TEntryList *myList = (TEntryList*)gDirectory->Get("myList");

 //==== bux fix in ROOT see https://savannah.cern.ch/bugs/?60569 ====
 TIter next( myList->GetLists() ); 
 if (myList->GetLists() != 0){
// if ((int) myList->GetLists() != 0){
  TEntryList *ilist;
  while( (ilist = (TEntryList*) *next ) ) {
   ilist->SetTreeName(myTree->GetName()); 
   next();
   } 
  TEntryList *duplist = (TEntryList*) myList->Clone(); 
  delete myList; myList = duplist; 
 }
 else {
  myList->SetTreeName(myTree->GetName());
 }
  //==== end bux fix in ROOT see https://savannah.cern.ch/bugs/?60569 ==== 
 myTree->SetEntryList(myList); 
 
 myTree->Draw("DeltaEtaIn:DeltaPhiIn:etaSC:phiSC","","para goff");
 int nEntries = myList->GetN();
 Double_t *vTemp = myTree->GetV1();
 Double_t *vDEta = new Double_t[nEntries];
 for (int iEntry = 0; iEntry<nEntries; iEntry++){
  vDEta[iEntry] = vTemp[iEntry];
 }
 Double_t *vTemp2 = myTree->GetV2();
 Double_t *vDPhi = new Double_t[nEntries];
 for (int iEntry = 0; iEntry<nEntries; iEntry++){
  vDPhi[iEntry] = vTemp2[iEntry];
 }
 Double_t *vEta = myTree->GetV3();
 Double_t *vPhi = myTree->GetV4();
 myTree->Draw("E5x5:eleCharge","","para goff");
 Double_t *vEnergy = myTree->GetV1();
 Double_t *vCharge = myTree->GetV2();

  Double_t vErrDEta;
  Double_t vErrDPhi;
  Double_t vErrEta;
  Double_t vErrPhi;
  
  double Chi2 = 0;
  
  int counter = 0;
  for (int iEntry = 0; iEntry < nEntries; iEntry++){
   if ((even && !(iEntry%2)) || (odd && (iEntry%2))) {
//    if (!(iEntry%10)) {
    counter++;
    
//   for (int iEntry = 0; iEntry<nEntries; iEntry++){
//     if (!(iEntry/100)) std::cout << " iEntry = " << iEntry << " : " << nEntries << std::endl;
//     std::cerr << " " << vDEta[iEntry] << std::endl;
//     std::cerr << " " << vDPhi[iEntry] << std::endl;
//     std::cerr << " " << vEta[iEntry] << std::endl;
//     std::cerr << " " << vPhi[iEntry] << std::endl;
//     std::cerr << " " << vEnergy[iEntry] << " " << std::endl;
    
    vErrDEta = fabs((sqrt(3.6 / sqrt(vEnergy[iEntry]) * 3.6 / sqrt(vEnergy[iEntry]) + 12. / vEnergy[iEntry] * 12. / vEnergy[iEntry] + 0.54*0.54)) / 1000. / (Z * tan(2*atan(exp(-vEta[iEntry])))) * fabs(sin(2*atan(exp(-vEta[iEntry]))))); ///===> /1000 perchè è in "mm" -> "m"
  
    vErrDPhi = 1.3 * fabs((sqrt(3.6 / sqrt(vEnergy[iEntry]) * 3.6 / sqrt(vEnergy[iEntry]) + 12. / vEnergy[iEntry] * 12. / vEnergy[iEntry] + 0.54*0.54)) / 1000. / (Z * tan(2*atan(exp(-vEta[iEntry]))))); ///===> /1000 perchè è in "mm" -> "m"
    
    vErrEta = 0.0;
    vErrPhi = 0.0;


    ///==== (x,y,z) 
     HepGeom::Point3D<double> xyz( Z / cos(2*atan(exp(-vEta[iEntry]))) * sin(2*atan(exp(-vEta[iEntry]))) * cos (vPhi[iEntry]),
				  Z / cos(2*atan(exp(-vEta[iEntry]))) * sin(2*atan(exp(-vEta[iEntry]))) * sin (vPhi[iEntry]),
				  Z );

			   
    ///==== (x',y',z') = RotoTrasl(x,y,z) 
    HepGeom::Point3D<double> xyz_prime = RotoTrasl * xyz;
    ///==== deta / dphi [(x',y',z'),(x,y,z)]
    

    double deta = xyz.pseudoRapidity() - xyz_prime.pseudoRapidity(); ///==== deta_data = SC - Tracker
    double dphi = deltaPhi(xyz.phi() , xyz_prime.phi()); ///==== check sign convention!!!!!!!!!!!!!!!!!!!!!
    double ddeta = (vDEta[iEntry] - deta);
    double ddphi = (vDPhi[iEntry] - dphi);
    
    ddeta = ddeta - FunctionDeta->Eval(vEta[iEntry],vCharge[iEntry]);
    ddphi = ddphi - FunctionDphi->Eval(vEta[iEntry],vCharge[iEntry]);
         
    Chi2 += (ddeta / vErrDEta * ddeta / vErrDEta + ddphi / vErrDPhi * ddphi / vErrDPhi);
    
   }
  }
  
  std::cout << " Chi2 = " << Chi2 << " / " << counter << " = " << Chi2/counter << " - " << DX*1000 << " mm: " << DY*1000 << " mm: " << DZ*1000 << " mm: "  <<  DPHIEuler << " : "  <<  DTHETAEuler << " : "  <<  DPSIEuler << std::endl;
   return Chi2;
}


///====================================================
///====================================================
 




int main(int argc, char** argv)
{
 //  ROOT::Math::Minimizer* minuit = ROOT::Math::Factory::CreateMinimizer("Genetic");  ---> next ROOT  release!
 ROOT::Math::Minimizer* minuit = ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad2");
 ROOT::Math::Functor functorChi2(&Chi2Func,6); 
 
 TString genCut; //==== (eleFBrem<0.8&&eleCharge>0)

 bool traslationX ;
 bool traslationY ;
 bool traslationZ ;
 bool rotationPhi ;
 bool rotationTheta ;
 bool rotationPsi ;
 
 std::string fileName (argv[1]) ;
 boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfig(fileName) ;
 boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;

 edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputTree") ;
 std::vector<std::string> nameFileIn = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
 std::string nameTree = subPSetInput.getParameter<std::string> ("nameTree") ;
 std::string selection = subPSetInput.getParameter<std::string> ("selection") ;
 genCut = Form("%s",selection.c_str());
 traslationX = subPSetInput.getParameter<bool> ("traslationX") ;
 traslationY = subPSetInput.getParameter<bool> ("traslationY") ;
 traslationZ = subPSetInput.getParameter<bool> ("traslationZ") ;
 rotationPhi   = subPSetInput.getParameter<bool> ("rotationPhi") ;
 rotationTheta = subPSetInput.getParameter<bool> ("rotationTheta") ;
 rotationPsi   = subPSetInput.getParameter<bool> ("rotationPsi") ;
     
 double setRotationPhi     = subPSetInput.getUntrackedParameter<double> ("setRotationPhi",0) ;
 double setRotationTheta   = subPSetInput.getUntrackedParameter<double> ("setRotationTheta",0) ;
 double setRotationPsi     = subPSetInput.getUntrackedParameter<double> ("setRotationPsi",0) ;

 
 even = subPSetInput.getUntrackedParameter<bool> ("even",true) ;
 odd = subPSetInput.getUntrackedParameter<bool> ("odd",true) ;

 edm::ParameterSet subPSetOutput = parameterSet->getParameter<edm::ParameterSet> ("outputTree") ;
 std::string nameFileOut = subPSetOutput.getParameter<std::string> ("outputFile") ;

 //==== plot input/output ====
 std::cout << " nameFileIn = ";
 for (unsigned int i=0; i<nameFileIn.size(); i++ ) std::cout << "    " << nameFileIn.at(i) << std::endl;
 std::cout << " nameFileOut = " << nameFileOut << std::endl;
 std::cout << " genCut = " << genCut.Data() << std::endl;
  
 ///==== input DATA ==== 
 myTree = new TChain(nameTree.c_str());
 int numberInput = 0;
 for (std::vector<std::string>::const_iterator listIt = nameFileIn.begin () ; listIt != nameFileIn.end () ; ++listIt) {
  numberInput++;
  myTree->Add (listIt->c_str ()) ;
  if (numberInput%4 == 0) std::cerr << "Input number " << numberInput << " ... " << listIt->c_str () << std::endl;
 }


 ///==== bias functions ==== 
 
 std::string FunctionDetaName = subPSetInput.getUntrackedParameter<std::string> ("DetaBias","0") ; // x = eta, y = charge
 std::string FunctionDphiName = subPSetInput.getUntrackedParameter<std::string> ("DphiBias","0") ;

 FunctionDeta = new TF2 ("DetaBias",FunctionDetaName.c_str(),-5,5,-2,2);
 FunctionDphi = new TF2 ("DphiBias",FunctionDphiName.c_str(),-5,5,-2,2);

 //==== output DATA ====
 
 ///==== Input ECAL position ====
 std::string inputFilesPosition = subPSetInput.getUntrackedParameter<std::string> ("inputFilesPosition","") ;

 ///==== Build variables ====
 
 double DX_SC_Mean[4];
 double DX_SC_RMS[4];
 double DY_SC_Mean[4];
 double DY_SC_RMS[4];
 double DZ_SC_Mean[4];
 double DZ_SC_RMS[4];
 
 double DTHETAe_SC_Mean[4];
 double DTHETAe_SC_RMS[4];
 double DPSIe_SC_Mean[4];
 double DPSIe_SC_RMS[4];
 double DPHIe_SC_Mean[4];
 double DPHIe_SC_RMS[4];
 
 
 std::ifstream* file;
 if (inputFilesPosition != ""){
  file = new std::ifstream(inputFilesPosition.c_str());
  if(!file->is_open())
  {
   return false;
  }
 }
 
 for (int iSC = 0; iSC<4; iSC++){
  TString cut;
  cut = Form("%s && iDetEB < -10 && iDetEE == %d",genCut.Data(),iSC);
  std::cout << "  cut = " << cut.Data() << std::endl;
  
  ///===========================
  ///==== Chi2 minimization ====
  
  double inputDX = 0;
  double inputDY = 0;
  double inputDZ = 0;
  double inputDPHIe = 0;
  double inputDTHETAe = 0;
  double inputDPSIe = 0;
  if (inputFilesPosition != ""){
   std::string buffer;
   getline(*file,buffer);
   std::stringstream line( buffer );
   line >> inputDPHIe;
   line >> inputDTHETAe;
   line >> inputDPSIe;
   line >> inputDX; inputDX/=100;
   line >> inputDY; inputDY/=100;
   line >> inputDZ; inputDZ/=100; 
  }
  
  std::cerr << " inputDPHIe   = " << inputDPHIe << std::endl;
  std::cerr << " inputDTHETAe = " << inputDTHETAe << std::endl;
  std::cerr << " inputDPSIe   = " << inputDPSIe << std::endl;
  std::cerr << " inputDX      = " << inputDX << std::endl;
  std::cerr << " inputDY      = " << inputDY << std::endl;
  std::cerr << " inputDZ      = " << inputDZ << std::endl;

  
  std::cout << " Chi2 minimization " << std::endl;
  
  globalCut = cut;
  //   unsigned int iNoSteps = 1000;
  //   unsigned int iPar_NoBG = 0;
  minuit->SetFunction(functorChi2);
  minuit->SetMaxFunctionCalls(100000);
  minuit->SetMaxIterations(10000);
  minuit->SetTolerance(0.000002);
  
  if (traslationX)  minuit->SetLimitedVariable(0,"DX",inputDX, 0.00001,-0.050,0.050);
  else  minuit->SetFixedVariable(0,"DX",0);

  if (traslationY)  minuit->SetLimitedVariable(1,"DY",inputDY, 0.00001,-0.050,0.050);
  else  minuit->SetFixedVariable(1,"DY",0);

  if (traslationZ)  minuit->SetLimitedVariable(2,"DZ",inputDZ, 0.00001,-0.050,0.050);
  else  minuit->SetFixedVariable(2,"DZ",0);


  if (rotationPhi)  minuit->SetLimitedVariable(3,"DPHIe",inputDPHIe, 0.001,-0.1,0.1);
  else  minuit->SetFixedVariable(3,"DPHIe",inputDPHIe);

  if (rotationTheta)  minuit->SetLimitedVariable(4,"DTHETAe",inputDTHETAe, 0.001,-0.1,0.1);
  else  minuit->SetFixedVariable(4,"DTHETAe",inputDTHETAe);

  if (rotationPsi)  minuit->SetLimitedVariable(5,"DPSIe",inputDPSIe, 0.001,-0.1,0.1);
  else  minuit->SetFixedVariable(5,"DPSIe",inputDPSIe);

 
  minuit->Minimize();
  minuit->PrintResults();
  
  DX_SC_Mean[iSC] = (minuit->X()[0]);
  DY_SC_Mean[iSC] = (minuit->X()[1]);
  DZ_SC_Mean[iSC] = (minuit->X()[2]);
  
  DX_SC_RMS[iSC] = (minuit->Errors()[0]);
  DY_SC_RMS[iSC] = (minuit->Errors()[1]);
  DZ_SC_RMS[iSC] = (minuit->Errors()[2]);
  
  DPHIe_SC_Mean[iSC] = (minuit->X()[3]);
  DTHETAe_SC_Mean[iSC] = (minuit->X()[4]);
  DPSIe_SC_Mean[iSC] = (minuit->X()[5]);
  
  DPHIe_SC_RMS[iSC] = (minuit->Errors()[3]);
  DTHETAe_SC_RMS[iSC] = (minuit->Errors()[4]);
  DPSIe_SC_RMS[iSC] = (minuit->Errors()[5]);
  
  ///==== end Chi2 minimization ====
  
  std::cout << " iSC = " << iSC << " DX =     " << DX_SC_Mean[iSC]      << " +/- " << DX_SC_RMS[iSC]      << std::endl;
  std::cout << " iSC = " << iSC << " DY =     " << DY_SC_Mean[iSC]      << " +/- " << DY_SC_RMS[iSC]      << std::endl;
  std::cout << " iSC = " << iSC << " DZ =     " << DZ_SC_Mean[iSC]      << " +/- " << DZ_SC_RMS[iSC]      << std::endl;
  std::cout << " iSC = " << iSC << " DPhi =   " << DPHIe_SC_Mean[iSC]   << " +/- " << DPHIe_SC_RMS[iSC]   << std::endl;
  std::cout << " iSC = " << iSC << " DTheta = " << DTHETAe_SC_Mean[iSC] << " +/- " << DTHETAe_SC_RMS[iSC] << std::endl;
  std::cout << " iSC = " << iSC << " DPsi =   " << DPSIe_SC_Mean[iSC]   << " +/- " << DPSIe_SC_RMS[iSC]   << std::endl;
  std::cout << "============================================================================" << std::endl;
  std::cout << "============================================================================" << std::endl;
  std::cout << "============================================================================" << std::endl;
  
 }
 
 ///==== dump results ====
 
 ofstream outFile;
 outFile.open (nameFileOut.c_str());
//  outFile << "#SC  dx   errdx   dy   errdy   dz   errdz  " << std::endl;
  
 for (int iSC = 0; iSC<4; iSC++){
  outFile << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << DPHIe_SC_Mean[iSC] << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << DTHETAe_SC_Mean[iSC] << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << DPSIe_SC_Mean[iSC] << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100*DX_SC_Mean[iSC] << " "
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100*DY_SC_Mean[iSC] << " "
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100*DZ_SC_Mean[iSC] << " "
  << std::endl;
 }
 
 outFile.close(); 
 
}




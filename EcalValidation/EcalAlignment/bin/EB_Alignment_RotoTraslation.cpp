///===================================
///==== c++ -o EB_Alignment_RotoTralsation.exe EB_Alignment_RotoTralsation.cpp `root-config --cflags --glibs` -lMinuit `clhep-config --libs`
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


TChain* myTree;
TString globalCut;
double R = 1.290;

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

 myList->Print(); 
 //==== bux fix in ROOT see https://savannah.cern.ch/bugs/?60569 ====
 TIter next( myList->GetLists() ); 

// std::cerr << " myList->GetLists() = " << (int) myList->GetLists() << std::endl;
 if ((int) myList->GetLists() != 0){
//  std::cerr << " entries = " << myList->GetLists()->GetEntries() << std::endl;
  TEntryList *ilist;
  while( (ilist = (TEntryList*) *next ) ) {
//   std::cerr << "======================================================" << std::endl; 
   ilist->SetTreeName(myTree->GetName()); 
//   std::cerr << " NOME 0 = " << ilist->GetTreeName() << std::endl;
   next();
   } 
//  std::cerr << " NOME 1 = " << myList->GetTreeName() << std::endl;
  TEntryList *duplist = (TEntryList*) myList->Clone(); 
  delete myList; myList = duplist; 
 }
 else {
  myList->SetTreeName(myTree->GetName());
 }
  //==== end bux fix in ROOT see https://savannah.cern.ch/bugs/?60569 ====
 
// std::cerr << " NOME 2 = " << myList->GetTreeName() << std::endl;
 myList->Print();
 myTree->ls();
// std::cout << " myTree->GetName() = " << myTree->GetName() << std::endl;
 
 myTree->SetEntryList(myList); 
 
 myTree->Draw("DeltaEtaIn:DeltaPhiIn:etaSC:phiSC","","para goff");
  Double_t *vDEta = myTree->GetV1();
  Double_t *vDPhi = myTree->GetV2();
  Double_t *vEta = myTree->GetV3();
  Double_t *vPhi = myTree->GetV4();
 myTree->Draw("E5x5","","para goff");
  Double_t *vEnergy = myTree->GetV1();
  
  Double_t vErrDEta;
  Double_t vErrDPhi;
  Double_t vErrEta;
  Double_t vErrPhi;
  
  int nEntries = myList->GetN();
  std::cout << "myList->GetN() = " << myList->GetN() << std::endl;
  
  double Chi2 = 0;
  
  for (int iEntry = 0; iEntry<nEntries; iEntry++){
//     if (!(iEntry/100)) std::cout << " iEntry = " << iEntry << " : " << nEntries << std::endl;
//     std::cerr << " " << vDEta[iEntry] << std::endl;
//     std::cerr << " " << vDPhi[iEntry] << std::endl;
//     std::cerr << " " << vEta[iEntry] << std::endl;
//     std::cerr << " " << vPhi[iEntry] << std::endl;
//     std::cerr << " " << vEnergy[iEntry] << " " << std::endl;
    
    vErrDEta = (sqrt(3.6 / sqrt(vEnergy[iEntry]) * 3.6 / sqrt(vEnergy[iEntry]) + 12. / vEnergy[iEntry] * 12. / vEnergy[iEntry] + 0.54*0.54)) / 1000. / R * fabs(sin(2*atan(exp(-vEta[iEntry])))); ///===> /1000 perchè è in "mm" -> "m"
    
    vErrDPhi = (sqrt(3.6 / sqrt(vEnergy[iEntry]) * 3.6 / sqrt(vEnergy[iEntry]) + 12. / vEnergy[iEntry] * 12. / vEnergy[iEntry] + 0.54*0.54)) / 1000. / R ; ///===> /1000 perchè è in "mm" -> "m"
    
    vErrEta = 0.0;
    vErrPhi = 0.0;

    ///==== (x,y,z) 
    HepGeom::Point3D<double> xyz( R / sin(2*atan(exp(-vEta[iEntry]))) * sin(2*atan(exp(-vEta[iEntry]))) * cos (vPhi[iEntry]),
				  R / sin(2*atan(exp(-vEta[iEntry]))) * sin(2*atan(exp(-vEta[iEntry]))) * sin (vPhi[iEntry]),
				  R  / tan(2*atan(exp(-vEta[iEntry]))));

			   
    ///==== (x',y',z') = RotoTrasl(x,y,z) 
    HepGeom::Point3D<double> xyz_prime = RotoTrasl * xyz;
    ///==== deta / dphi [(x',y',z'),(x,y,z)]
    

    double deta = xyz.pseudoRapidity() - xyz_prime.pseudoRapidity(); ///==== deta_data = SC - Tracker
    double dphi = deltaPhi(xyz.phi() , xyz_prime.phi()); ///==== check sign convention!!!!!!!!!!!!!!!!!!!!!
    double ddeta = (vDEta[iEntry] - deta);
    double ddphi = (vDPhi[iEntry] - dphi);
    Chi2 += (ddeta / vErrDEta * ddeta / vErrDEta + ddphi / vErrDPhi * ddphi / vErrDPhi);
    
   }
  
  std::cout << " Chi2 = " << Chi2 << " / " << nEntries << " = " << Chi2/nEntries << " - " << DX*1000 << " : " << DY*1000 << " : " << DZ*1000 << " : "  <<  DPHIEuler << " : "  <<  DTHETAEuler << " : "  <<  DPSIEuler << std::endl;
 return Chi2;
}


///====================================================
///====================================================
 




int main(int argc, char** argv)
{
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
 boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName) ;
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



 //==== output DATA ====
 
 ///==== Build variables ====
 
 double DX_SM_Mean[36];
 double DX_SM_RMS[36];
 double DY_SM_Mean[36];
 double DY_SM_RMS[36];
 double DZ_SM_Mean[36];
 double DZ_SM_RMS[36];
 
 double DTHETAe_SM_Mean[36];
 double DTHETAe_SM_RMS[36];
 double DPSIe_SM_Mean[36];
 double DPSIe_SM_RMS[36];
 double DPHIe_SM_Mean[36];
 double DPHIe_SM_RMS[36];
 
 
 for (int iSM = 0; iSM<36; iSM++){
  TString cut;
  cut = Form("%s && iDetEE < -10 && iDetEB == %d",genCut.Data(),iSM);
  std::cout << "  cut = " << cut.Data() << std::endl;
  
  ///===========================
  ///==== Chi2 minimization ====
  
  std::cout << " Chi2 minimization " << std::endl;
  
  globalCut = cut;
  //   unsigned int iNoSteps = 1000;
  //   unsigned int iPar_NoBG = 0;
  minuit->SetFunction(functorChi2);
  minuit->SetMaxFunctionCalls(100000);
  minuit->SetMaxIterations(10000);
  minuit->SetTolerance(0.000002);
  
  if (traslationX)  minuit->SetLimitedVariable(0,"DX",0, 0.00001,-0.050,0.050);
  else  minuit->SetFixedVariable(0,"DX",0);

  if (traslationY)  minuit->SetLimitedVariable(1,"DY",0, 0.00001,-0.050,0.050);
  else  minuit->SetFixedVariable(1,"DY",0);

  if (traslationZ)  minuit->SetLimitedVariable(2,"DZ",0, 0.00001,-0.050,0.050);
  else  minuit->SetFixedVariable(2,"DZ",0);


  if (rotationPhi)  minuit->SetLimitedVariable(3,"DPHIe",0, 0.01,-0.1,0.1);
  else  minuit->SetFixedVariable(3,"DPHIe",0);

  if (rotationTheta)  minuit->SetLimitedVariable(4,"DTHETAe",0, 0.01,-0.1,0.1);
  else  minuit->SetFixedVariable(4,"DTHETAe",0);

  if (rotationPsi)  minuit->SetLimitedVariable(5,"DPSIe",0, 0.01,-0.1,0.1);
  else  minuit->SetFixedVariable(5,"DPSIe",0);
 
 
  minuit->Minimize();
  minuit->PrintResults();
  
  DX_SM_Mean[iSM] = (minuit->X()[0]);
  DY_SM_Mean[iSM] = (minuit->X()[1]);
  DZ_SM_Mean[iSM] = (minuit->X()[2]);
  
  DX_SM_RMS[iSM] = (minuit->Errors()[0]);
  DY_SM_RMS[iSM] = (minuit->Errors()[1]);
  DZ_SM_RMS[iSM] = (minuit->Errors()[2]);
  
  DPHIe_SM_Mean[iSM] = (minuit->X()[3]);
  DTHETAe_SM_Mean[iSM] = (minuit->X()[4]);
  DPSIe_SM_Mean[iSM] = (minuit->X()[5]);
  
  DPHIe_SM_RMS[iSM] = (minuit->Errors()[3]);
  DTHETAe_SM_RMS[iSM] = (minuit->Errors()[4]);
  DPSIe_SM_RMS[iSM] = (minuit->Errors()[5]);
  
  ///==== end Chi2 minimization ====
  
  std::cout << " iSM = " << iSM << " DX = " << DX_SM_Mean[iSM]      << " +/- " << DX_SM_RMS[iSM]      << std::endl;
  std::cout << " iSM = " << iSM << " DY = " << DY_SM_Mean[iSM]      << " +/- " << DY_SM_RMS[iSM]      << std::endl;
  std::cout << " iSM = " << iSM << " DZ = " << DZ_SM_Mean[iSM]      << " +/- " << DZ_SM_RMS[iSM]      << std::endl;
  std::cout << " iSM = " << iSM << " DZ = " << DPHIe_SM_Mean[iSM]   << " +/- " << DPHIe_SM_RMS[iSM]   << std::endl;
  std::cout << " iSM = " << iSM << " DZ = " << DTHETAe_SM_Mean[iSM] << " +/- " << DTHETAe_SM_RMS[iSM] << std::endl;
  std::cout << " iSM = " << iSM << " DZ = " << DPSIe_SM_Mean[iSM]   << " +/- " << DPSIe_SM_RMS[iSM]   << std::endl;
  std::cout << "============================================================================" << std::endl;
  std::cout << "============================================================================" << std::endl;
  std::cout << "============================================================================" << std::endl;
  
 }
 
 ///==== dump results ====
 
 ofstream outFile;
 outFile.open (nameFileOut.c_str());
 outFile << "#SM  dx   errdx   dy   errdy   dz   errdz  " << std::endl;
  
 for (int iSM = 0; iSM<36; iSM++){
  outFile << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << DPHIe_SM_Mean[iSM] << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << DTHETAe_SM_Mean[iSM] << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << DPSIe_SM_Mean[iSM] << " " 
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100*DX_SM_Mean[iSM] << " "
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100*DY_SM_Mean[iSM] << " "
  << std::setfill(' ') << std::setw(10) << std::setprecision(5) << 100*DZ_SM_Mean[iSM] << " "
  << std::endl;
 }
 
 outFile.close(); 
 
}




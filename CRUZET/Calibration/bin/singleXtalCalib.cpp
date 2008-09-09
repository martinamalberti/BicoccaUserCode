#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include <strstream>

#include "TH1.h"
#include "TChain.h"

//PG for the cluster-based calibration 
#include "Calibration/EcalCalibAlgos/interface/VEcalCalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/IMACalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/L3CalibBlock.h"


//---- AM added
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "CRUZET/Calibration/interface/CRUtils.h"
#include "TFile.h"
#include <iostream>
#include "TH2.h"
#include "TProfile.h"
#include "TSystem.h"


#define PI 3.14159265

int main (int argc, char** argv)
{
  double EnergyPerCrystal_Min_Cut = 0.05; //---- 0.05 GeV --> 50 MeV
  double EnergyPerCrystal_Max_Cut = 2.0; //---- 2 GeV
 
  //  std::map<int, TH1F *> dEdx_Histos ;
  //  std::map<int, TH1F *> dEdx_Ring_Histos ;
  std::map<int, TH1F *> dEdx_Ring_1M_Up_Histos ;
  std::map<int, TH1F *> dEdx_Ring_1M_Down_Histos ;
  /*  
  std::map<int, TH1F *> dE_Histos ;
  std::map<int, TH1F *> dE_Ring_Histos ;
  std::map<int, TH1F *> dE_Ring_1M_Up_Histos ;
  std::map<int, TH1F *> dE_Ring_1M_Down_Histos ;

  std::map<int, TH1F *> dEdx_Ring_Histos_EnergyCut ;
  std::map<int, TH1F *> dEdx_Ring_1M_Up_Histos_EnergyCut ;
  std::map<int, TH1F *> dEdx_Ring_1M_Down_Histos_EnergyCut ;
  */  
  
  int nEntries = -1 ;
  
  //---- AM Command line input ----
  
  //  for (int i=0; i<argc; i++)
  
  std::string outputRootName = "outputHistos8SepXCastor.root";
  std::string testNameOutput = "-o";
  if (argc>2){
    if (argv[1] == testNameOutput) {
      outputRootName = argv[2];
    }
  }
  
  std::cout << outputRootName << std::endl;
 
 
 
//  std::string testHelp = "--help";
//  if (argc==2){
//   if (argv[1] == testHelp) {
//    std::cout << "Help" << std::endl ;
//    std::cout << " --help : display help" << std::endl ;
//    std::cout << " -o : output root file name (eg histograms.root)" << std::endl ;
//    std::cout << " -i : input root file name (eg histograms.root)" << std::endl ;
//    std::cout << " name of input file : list name of input files ntuples" << std::endl ;     
//    exit(1);
//   }
//  }
 
  
//  if (argc < 2)
//  {
//   std::cerr << "ERROR : ntuple name missing" << std::endl ;
//   exit (1) ;
//  }

  
  

  //---- Tree construction: sum over different trees ----
 std::string testNameInput = "-i";
 TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
 
//  if (argv[1] != testNameInput && argv[3] != testNameInput){
//    if (argv[1] == testNameOutput) {
//      for (int i=3; i< (argc); i++) {
//        chain->Add (argv[i]) ;
//        std::cout << "Open: " << argv[i] << std::endl;
//      }
//    }
//    else{
//      for (int i=1; i< (argc); i++) {
//        chain->Add (argv[i]) ;
//        std::cout << "Open: " << argv[i] << std::endl;
//      }
//    }
//  }
// else {
 char inputRootName[100];
//  for (int i=1; i< 10; i++) {
 
 
 
 
 
  
 
 //---------------------------------------------------------
 //---------------------- Input Files ----------------------
 //---------------------------------------------------------

 
 
 char buffer[1000];
 char inputName[1000];

 
 //--------------------- 
 //---- Cloned Tree ----
 //--------------------- 
 
 /*

//  std::string inputNameDirectory = "/tmp/govoni/50908Cosmic";
 std::string inputNameDirectory = "/data/cosmics";
 
   
 std::string Command2Line = "ls " + inputNameDirectory + " > temp.txt";
 std::cout << Command2Line << std::endl;
 gSystem->Exec(Command2Line.c_str());

 
 ifstream fileC_50908_Cloned("temp.txt");
 std::cerr << " Reading  " << inputNameDirectory << " ... " << std::endl;
 while (!fileC_50908_Cloned.eof()){
  fileC_50908_Cloned.getline (&buffer[0],1000);
  std::istrstream line(buffer);
  std::string uselessString;
  int uselessInt;
  std::string treeName; 
  line >> treeName;
  sprintf(inputName,"%s/%s",inputNameDirectory.c_str(),treeName.c_str());
  std::cerr << "  File ->  " << treeName;
  std::cerr << "  File ->  " << inputName << std::endl;
  chain->Add (inputName);
 }

 
 Command2Line = "rm temp.txt";
 gSystem->Exec(Command2Line.c_str());
 
 */
 //------------------------------------------------------------
 //---------------------- End Input Files ----------------------
 //-------------------------------------------------------------
 
 // da castor pochi eventi Global || Cosmic  
 /*
 
 for (int i=1; i< 46; i++) {
  sprintf (inputRootName,"/castor/cern.ch/user/m/mattia/50908Global/EcalCosmicsTree-50908%i.tree.root",i); 
   //   sprintf (inputRootName,"/castor/cern.ch/user/m/mattia/50908Cosmic/EcalCosmicsTree-50908%i.tree.root",i); 
  chain->Add (inputRootName);
  std::cout << "Open: " << inputRootName << std::endl;
 }
 
 for (int i=1; i< 48; i++) {
  if (i!=28 && i!=35){
   sprintf (inputRootName,"/castor/cern.ch/user/m/mattia/50911Global/50911_%d.tree.root",i); 
   //   sprintf (inputRootName,"/castor/cern.ch/user/m/mattia/50908Cosmic/EcalCosmicsTree-50908%i.tree.root",i); 
   chain->Add (inputRootName);
   std::cout << "Open: " << inputRootName << std::endl;
  }
 }
 
 for (int i=1; i< 39; i++) {
  sprintf (inputRootName,"/castor/cern.ch/user/m/mattia/51503Global/EcalCosmicsTree-51503%d.tree.root",i); 
   //   sprintf (inputRootName,"/castor/cern.ch/user/m/mattia/50908Cosmic/EcalCosmicsTree-50908%i.tree.root",i); 
  chain->Add (inputRootName);
  std::cout << "Open: " << inputRootName << std::endl;
 }
 */
 
 

 // da castor nuove NTuple (cloned)
 
 sprintf(inputName,"/afs/cern.ch/user/a/amartell/public/perAmassi/CRUZET_elenco/listaCosmic.txt");
 //sprintf(inputName,"/afs/cern.ch/user/a/amartell/public/perAmassi/CRUZET_elenco/listaGlobal.txt");
 ifstream fileC_51047(inputName);
 std::cerr << " Reading  " << inputName << " ... " << std::endl;
 while (!fileC_51047.eof()){
  fileC_51047.getline (&buffer[0],1000);
  std::istrstream line(buffer);
  std::string uselessString;
  int uselessInt;
  //---- eg: -rw-r--r--   1 govoni   zh                     305501 Sep 07 18:12 0007034F-6E4F-DD11-BFD3-000423D99160.ntuple.root-50908.tree.root
  line >> uselessString; //---- -rw-r--r--
  std::cerr << "  uselessString ->  " << uselessString << std::endl;
  line >> uselessInt; //---- 1
  std::cerr << "  uselessInt ->  " << uselessInt << std::endl;
  line >> uselessString; //---- govoni
  std::cerr << "  uselessString ->  " << uselessString << std::endl;
  line >> uselessString; //---- zh
  std::cerr << "  uselessString ->  " << uselessString << std::endl;
  line >> uselessInt; //---- 305501
  std::cerr << "  uselessInt ->  " << uselessInt << std::endl;
  line >> uselessString; //---- Sep
  std::cerr << "  uselessString ->  " << uselessString << std::endl;
  line >> uselessInt; //---- 07
  std::cerr << "  uselessInt ->  " << uselessInt << std::endl;
  line >> uselessString; //---- 18:12
  std::cerr << "  uselessString ->  " << uselessString << std::endl;
  std::string treeName; 
  line >> treeName;
  //  sprintf(inputName,"/data/cosmics/%s",treeName.c_str());
  sprintf(inputName,"/data/cosmics/%s",treeName.c_str());
  std::cerr << "  File ->  " << treeName << std::endl;
  std::cerr << "  File ->  " << inputName << std::endl;
  chain->Add (inputName);
 }
 
 
 
std::cerr << " >>>>>>>>>>>>>> Starting analysis ..." << std::endl; 
 
 //---- AM Initiate output file ----
 TFile saving (outputRootName.c_str(),"recreate") ;
 saving.cd ();
  
 EcalCosmicsTreeContent treeVars ; 
 setBranchAddresses (chain, treeVars) ;
 nEntries = chain->GetEntries () ;
   
 TH1F dEdx_Tutti("dEdx_Tutti","dEdx_Tutti",10000,0,2);
 TH1F dE_Tutti("dE_Tutti","dE_Tutti",1000,0,2);
 TH1F dX_Tutti("dX_Tutti","dX_Tutti",1000,0.,25.);  

 TH1F alpha_up("alpha_up", "alpha_up", 180, 0., 180.);
 TH1F alpha_down("alpha_down", "alpha_down", 180, 0., 180.);

 TH1F muon_up("muon_up", "muon_up", 180, 0., 180.);
 TH1F muon_down("muon_down", "muon_down", 180, 0., 180.);

 TH2F EtaPhi_events("EtaPhi_events", "EtaPhi_events", 171, -85., 86, 360, 1., 361.);
 TProfile2D EtaPhi_alpha_xtal("EtaPhi_alpha_xtal", "EtaPhi_alpha_xtal", 171, -85., 86, 360, 1., 361.);
 TProfile2D EtaPhi_alpha_xtalMU("EtaPhi_alpha_xtalMU", "EtaPhi_alpha_xtalMU", 171, -85., 86, 360, 1., 361.);
 TProfile2D EtaPhi_alpha_MU("EtaPhi_alpha_MU", "EtaPhi_alpha_MU", 171, -85., 86, 360, 1., 361.);

 TProfile dEvsAlpha_Tutti("dEvsAlpha_Tutti","dEvsAlpha_Tutti",100, 0., 90.);
 TProfile dXvsAlpha_Tutti("dXvsAlpha_Tutti","dXvsAlpha_Tutti",100, 0., 90.);
 TProfile dEdx_alpha("dEdx_alpha", "dEdx_alpha", 100, 0., 90. );
 
 TH2F dEdxVsEnergy ("dEdxVsEnergy","dE/dx versus Crystal Energy",10000,0,2,1000,0,2);
 TH2F occupancyEnergy ("occupancyEnergy","single crystals counting Energy",180,0,360,171,-85,86) ;
 TH2F occupancy ("occupancy","single crystals counting",360,1,361,171,-85,86) ;
 
 
 
 //PG loop over entries
 for (int entry = 0 ; entry < nEntries ; ++entry)
 {
  chain->GetEntry (entry) ;
  if (entry%10000 == 0)  std::cout << "------> reading entry " << entry << " <------\n" ; 


     //fill association map muon-supercluster
  std::vector<ect::association> associations ;
  ect::fillAssocVector (associations, treeVars) ;
  ect::selectOnDR (associations, treeVars, 0.3) ;

     // find the muon associated to the Xtal-supercluster 
  for (unsigned int i = 0 ; i < associations.size () ; ++i)
  {
   int SCindex = associations.at (i).second ;
   float dummyEmax = 0.;
   float dummyLmax = 0.;
   int numCrystalEMax = -1;
   int numCrystalLMax = -1;
   bool XtalOk = true;
   
   for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
        XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
          treeVars.nXtalsInSuperCluster[SCindex] ; ++XTLindex)
   {
    if(treeVars.xtalTkLength[XTLindex] == -1) continue;
    //check the link Xtal with max energy  == Xtal with max length
    if(treeVars.xtalEnergy[XTLindex] > dummyEmax) 
    {
     numCrystalEMax = XTLindex;   
    }
    if(treeVars.xtalTkLength[XTLindex] > dummyLmax) {
     numCrystalLMax = XTLindex;   
     }
   }
   if (numCrystalEMax == numCrystalLMax) XtalOk = true;
   else XtalOk = false;
   
   
   if(XtalOk == false) continue;
   
   for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
        XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
          treeVars.nXtalsInSuperCluster[SCindex] ; ++XTLindex)
   {
    if(treeVars.xtalTkLength[XTLindex] == -1) continue;
    //---- Cut on Crystal Energy ---- minimum and maximum energy ----
    double dummyEnergy = treeVars.xtalEnergy[XTLindex];
    
    if (treeVars.xtalTkLength[XTLindex] > 0) 
      dEdxVsEnergy.Fill(treeVars.xtalEnergy[XTLindex]/treeVars.xtalTkLength[XTLindex],treeVars.xtalEnergy[XTLindex]);
    
    if ( dummyEnergy < EnergyPerCrystal_Min_Cut || dummyEnergy > EnergyPerCrystal_Max_Cut) continue;
    
    if(3.*treeVars.xtalEnergy[XTLindex] < treeVars.xtalTkLength[XTLindex] * 0.0125) continue;
    
    //---- PG find a unique index for each xtal --> rawId()
    EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;   
   
    //---- get Xtal direction
    TVector3 Xtal_pos (0., 0., 0.) ;
    
    double xtalEta = (dummy.ieta() - 0.5) * 0.0175;
    if(dummy.ieta() < 0.) xtalEta = (dummy.ieta() + 0.5) * 0.0175;
    double xtalPhi = (dummy.iphi() - 10) * 0.0175;
    if(xtalPhi > PI) xtalPhi = - 2*PI + xtalPhi;
    setVectorOnECAL (Xtal_pos, xtalEta, xtalPhi, 131.);
    
    //---- Crystal direction ----
    TVector3 MuonDirf (0., 1., 0.);
    float angle = MuonDirf.Angle( Xtal_pos ) ;
    if(angle > PI ) angle -= PI ;       
    if( angle > PI/2. ) angle = PI - angle; // angle belongs to [0:90]       
              
    EtaPhi_alpha_xtal.Fill(dummy.ieta(), dummy.iphi(), angle*180./PI);

    //---- Muon and Crystal direction ----
    int MUindex = associations.at (i).first  ;
    TVector3 MuonDir ( treeVars.muonMomentumX[MUindex],  treeVars.muonMomentumY[MUindex],  treeVars.muonMomentumZ[MUindex]);
    angle = MuonDir.Angle( Xtal_pos ) ;
    if(angle > PI ) angle -= PI ;       
    if( angle > PI/2. ) angle = PI - angle; // angle belongs to [0:90]       
       
    if(dummy.iphi() < 10 || dummy.iphi() > 190) alpha_down.Fill(angle*180. /PI);       
    else alpha_up.Fill(angle*180. /PI);       
    EtaPhi_alpha_xtalMU.Fill(dummy.ieta(), dummy.iphi(), angle*180./PI);
    dEdx_alpha.Fill(angle*180./PI, treeVars.xtalEnergy[XTLindex] / treeVars.xtalTkLength[XTLindex]);
    EtaPhi_events.Fill(dummy.ieta(), dummy.iphi(), angle*180./PI);

    //---- Muon direction ----
    TVector3 Xtal_posf (0., 1., 0.) ;
    angle = MuonDir.Angle( Xtal_posf ) ;
    if(angle > PI ) angle -= PI ;       
    if( angle > PI/2. ) angle = PI - angle; // angle belongs to [0:90]       
    EtaPhi_alpha_MU.Fill(dummy.ieta(), dummy.iphi(), angle*180./PI);
    if(dummy.iphi() < 10 || dummy.iphi() > 190) muon_down.Fill(angle*180. /PI);       
    else muon_up.Fill(angle*180. /PI);       

    //-------------------------------- define alpha cuts - to be done
    if(angle > 90.) continue;
         
    /* 
    //---- AM Single crystal ----
    std::map<int, TH1F *>::iterator dEdx_Histos_iter = dEdx_Histos.find(dummy.rawId());
    if (dEdx_Histos_iter == dEdx_Histos.end()) {
     std::ostringstream stm;
     stm << dummy.rawId();
     std::string TH1FName = "dEdX_" +  stm.str();
     std::string TH1FNameDescription = "dE over dX -> rawId = " + stm.str();
     dEdx_Histos[dummy.rawId()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    */
    //---- AM Ring ---- dE/dx ----
    //---- up ring ----
    std::map<int, TH1F *>::iterator dEdx_Ring_1M_Up_Histos_iter = dEdx_Ring_1M_Up_Histos.find(dummy.ieta());
    if (dEdx_Ring_1M_Up_Histos_iter == dEdx_Ring_1M_Up_Histos.end()) {
      std::ostringstream stm;
      stm << dummy.ieta();
      std::string TH1FName = "dEdX_Ring_Up" +  stm.str();
      std::string TH1FNameDescription = "dE over dX Ring Up -> ieta = " + stm.str();
      dEdx_Ring_1M_Up_Histos[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    /*
    //---- ---- + Energy Cut ----
    std::map<int, TH1F *>::iterator dEdx_Ring_1M_Up_Histos_EnergyCut_iter = dEdx_Ring_1M_Up_Histos_EnergyCut.find(dummy.ieta());
    if (dEdx_Ring_1M_Up_Histos_EnergyCut_iter == dEdx_Ring_1M_Up_Histos_EnergyCut.end()) {
     std::ostringstream stm;
     stm << dummy.ieta();
     std::string TH1FName = "dEdX_Ring_Up_EnergyCut" +  stm.str();
     std::string TH1FNameDescription = "dE over dX Ring Up EnergyCut -> ieta = " + stm.str();
     dEdx_Ring_1M_Up_Histos_EnergyCut[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    */
    //---- down ring ----
    std::map<int, TH1F *>::iterator dEdx_Ring_1M_Down_Histos_iter = dEdx_Ring_1M_Down_Histos.find(dummy.ieta());
    if (dEdx_Ring_1M_Down_Histos_iter == dEdx_Ring_1M_Down_Histos.end()) {
     std::ostringstream stm;
     stm << dummy.ieta();
     std::string TH1FName = "dEdX_Ring_Down" +  stm.str();
     std::string TH1FNameDescription = "dE over dX Ring Down -> ieta = " + stm.str();
     dEdx_Ring_1M_Down_Histos[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    /*
    //---- ---- + Energy Cut ----
    std::map<int, TH1F *>::iterator dEdx_Ring_1M_Down_Histos_EnergyCut_iter = dEdx_Ring_1M_Down_Histos_EnergyCut.find(dummy.ieta());
    if (dEdx_Ring_1M_Down_Histos_EnergyCut_iter == dEdx_Ring_1M_Down_Histos_EnergyCut.end()) {
     std::ostringstream stm;
     stm << dummy.ieta();
     std::string TH1FName = "dEdX_Ring_Down_EnergyCut" +  stm.str();
     std::string TH1FNameDescription = "dE over dX Ring Down EnergyCut -> ieta = " + stm.str();
     dEdx_Ring_1M_Down_Histos_EnergyCut[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    */
    /*
    //---- all ring ----
    std::map<int, TH1F *>::iterator dEdx_Ring_Histos_iter = dEdx_Ring_Histos.find(dummy.ieta());
    if (dEdx_Ring_Histos_iter == dEdx_Ring_Histos.end()) {
     std::ostringstream stm;
     stm << dummy.ieta();
     std::string TH1FName = "dEdX_Ring" +  stm.str();
     std::string TH1FNameDescription = "dE over dX Ring -> ieta = " + stm.str();
     dEdx_Ring_Histos[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }

    std::map<int, TH1F *>::iterator dEdx_Ring_Histos_EnergyCut_iter = dEdx_Ring_Histos_EnergyCut.find(dummy.ieta());
    if (dEdx_Ring_Histos_EnergyCut_iter == dEdx_Ring_Histos_EnergyCut.end()) {
     std::ostringstream stm;
     stm << dummy.ieta();
     std::string TH1FName = "dEdX_Ring_EnergyCut" +  stm.str();
     std::string TH1FNameDescription = "dE over dX Ring EnergyCut-> ieta = " + stm.str();
     dEdx_Ring_Histos_EnergyCut[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    */
    //---- dE ----

    /*
    std::map<int, TH1F *>::iterator dE_Histos_iter = dE_Histos.find(dummy.rawId());
    if (dE_Histos_iter == dE_Histos.end()) {
     std::ostringstream stm;
     stm << dummy.rawId();
     std::string TH1FName = "dE_" +  stm.str();
     std::string TH1FNameDescription = "dE -> rawId = " + stm.str();
     dE_Histos[dummy.rawId()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    
     
    //---- AM ---- Ring ---- dE ----
    
    //---- up ring ----
    std::map<int, TH1F *>::iterator dE_Ring_1M_Up_Histos_iter = dE_Ring_1M_Up_Histos.find(dummy.ieta());
    if (dE_Ring_1M_Up_Histos_iter == dE_Ring_1M_Up_Histos.end()) {
      std::ostringstream stm;
      stm << dummy.ieta();
      std::string TH1FName = "dE_Ring_Up" +  stm.str();
      std::string TH1FNameDescription = "dE Ring Up -> ieta = " + stm.str();
      dE_Ring_1M_Up_Histos[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    
    //---- down ring ----
    std::map<int, TH1F *>::iterator dE_Ring_1M_Down_Histos_iter = dE_Ring_1M_Down_Histos.find(dummy.ieta());
    if (dE_Ring_1M_Down_Histos_iter == dE_Ring_1M_Down_Histos.end()) {
     std::ostringstream stm;
     stm << dummy.ieta();
     std::string TH1FName = "dE_Ring_Down" +  stm.str();
     std::string TH1FNameDescription = "dE Ring Down -> ieta = " + stm.str();
     dE_Ring_1M_Down_Histos[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    //---- all ring ----
    std::map<int, TH1F *>::iterator dE_Ring_Histos_iter = dE_Ring_Histos.find(dummy.ieta());
    if (dE_Ring_Histos_iter == dE_Ring_Histos.end()) {
     std::ostringstream stm;
     stm << dummy.ieta();
     std::string TH1FName = "dE_Ring" +  stm.str();
     std::string TH1FNameDescription = "dE Ring -> ieta = " + stm.str();
     dE_Ring_Histos[dummy.ieta()] = new TH1F(TH1FName.c_str(),TH1FNameDescription.c_str(),100,0,0.2);
    }
    
    
    */

    //PG fill the histo with the E/L for the xtal
    dEdx_Tutti.Fill(treeVars.xtalEnergy[XTLindex] / treeVars.xtalTkLength[XTLindex]);
    
    //---- AM ---- dE/dx ---- Single crystal ----
    //    dEdx_Histos[dummy.rawId()]->Fill(treeVars.xtalEnergy[XTLindex]/treeVars.xtalTkLength[XTLindex]);
    
    //---- AM ---- dE ---- Single crystal ----
    // dE_Histos[dummy.rawId()]->Fill(treeVars.xtalEnergy[XTLindex]);
      
        
    //---- AM Ring ---- dE/dx ----
    // dEdx_Ring_Histos[dummy.ieta()]->Fill(treeVars.xtalEnergy[XTLindex]/treeVars.xtalTkLength[XTLindex]);
    if (dummy.iphi() < 120 && dummy.iphi() > 80) 
      dEdx_Ring_1M_Up_Histos[dummy.ieta()]->Fill(treeVars.xtalEnergy[XTLindex]/treeVars.xtalTkLength[XTLindex]);
    if (dummy.iphi() < 300 && dummy.iphi() > 260) 
      dEdx_Ring_1M_Down_Histos[dummy.ieta()]->Fill(treeVars.xtalEnergy[XTLindex]/treeVars.xtalTkLength[XTLindex]);

    // dE_Ring_Histos[dummy.ieta()]->Fill(treeVars.xtalEnergy[XTLindex]);
    
    dE_Tutti.Fill(treeVars.xtalEnergy[XTLindex]);
    dX_Tutti.Fill(treeVars.xtalTkLength[XTLindex]);
    occupancy.Fill (dummy.iphi (), dummy.ieta ()) ;
    occupancyEnergy.Fill (dummy.iphi (),dummy.ieta (),treeVars.xtalEnergy[XTLindex]);
   }
  }
 } //PG loop over entries
 

 
 /*
 
 //---- dE/dx map from the fit ---- single crystal ----
 std::map<int, double> XtalCoeff_map ;
 std::map<int, double> XtalCoeffError_map ;
 
 TH2F dEdXEtaPhi ("dEdXEtaPhi","dEdXEtaPhi",360,1,361,171,-85,86) ;
 int MinXtal_cut = 20;
//  for (MinXtal_cut = 10; MinXtal_cut<100; MinXtal_cut++){
//  //AM ---- loop over dEdx_Histos ---- single crystal ----
//   for (std::map<int, TH1F*>::iterator mapIt = dEdx_Histos.begin (); mapIt != dEdx_Histos.end ();++mapIt)
//   {
//      //   mapIt->second->Fit ("gaus") ;
//    if( (mapIt->second->GetEntries()) > MinXtal_cut ){
//     std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
//     mapIt->second->Write();
//     double mean_pair = MeanAndErrorPair.first;
//     double Error_pair = MeanAndErrorPair.second;
//     XtalCoeff_map[mapIt->first] = mean_pair;
//     XtalCoeffError_map[mapIt->first] = Error_pair;
//    //---- fill histogram ----
//     EBDetId dummyMy(mapIt->first);
//     std::cerr << " iphi =  " << dummyMy.iphi() << " ieta = " << dummyMy.ieta() << " -> dEdX = " << mean_pair << std::endl;
//     dEdXEtaPhi.SetBinContent(dummyMy.iphi(),dummyMy.ieta(),mean_pair);
//    }
//    else {
//     XtalCoeff_map[mapIt->first] = 0;
//     XtalCoeffError_map[mapIt->first] = 0;
//    }
//   }  //AM ---- end loop over dEdx_Histos single crystal ----
// 
//   char titleGraph[1000];
//   sprintf(titleGraph,"MinXtal_cut=%i",MinXtal_cut);
//   dEdXEtaPhi.SetTitle(titleGraph);
//   dEdXEtaPhi.Draw("colz");
//   gPad->Update();
//   gPad->SaveAs("3D.gif+");
//   gPad->SaveAs("3D.gif+");
//   gPad->SaveAs("3D.gif+");
//   gPad->SaveAs("3D.gif+");
//   gPad->SaveAs("3D.gif+");
//   gPad->SaveAs("3D.gif+");
//   
//   for (int iieta = -85; iieta<86; iieta++)
//    for (int iiphi = 0; iiphi<361; iiphi++)
//     dEdXEtaPhi.SetBinContent(iiphi,iieta,0);
//  }
//  
 
 MinXtal_cut = 20;
 for (std::map<int, TH1F*>::iterator mapIt = dEdx_Histos.begin (); mapIt != dEdx_Histos.end ();++mapIt)
 {
     //   mapIt->second->Fit ("gaus") ;
  if( (mapIt->second->GetEntries()) > MinXtal_cut ){
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   XtalCoeff_map[mapIt->first] = mean_pair;
   XtalCoeffError_map[mapIt->first] = Error_pair;
   //---- fill histogram ----
   EBDetId dummyMy(mapIt->first);
   std::cerr << " iphi =  " << dummyMy.iphi() << " ieta = " << dummyMy.ieta() << " -> dEdX = " << mean_pair << std::endl;
   dEdXEtaPhi.Fill(dummyMy.iphi(),dummyMy.ieta(),mean_pair);
  }
  else {
   XtalCoeff_map[mapIt->first] = 0;
   XtalCoeffError_map[mapIt->first] = 0;
  }
 }  //AM ---- end loop over dEdx_Histos single crystal ----

 */
 
 
 
 //---- dE/dx slice of the Ring ----
 
 std::map<int, double> RingCoeffUp_map ;
 std::map<int, double> RingCoeffUpError_map ;
 
 std::map<int, double> RingCoeffDown_map ;
 std::map<int, double> RingCoeffDownError_map ;
 
 //AM ---- loop over dEdx_Histos ---- Slice Ring ----
 //---- UP ----
 for (std::map<int, TH1F*>::iterator mapIt = dEdx_Ring_1M_Up_Histos.begin (); mapIt != dEdx_Ring_1M_Up_Histos.end ();++mapIt)
 {
  if (mapIt->second->GetEntries() > 15) {
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   RingCoeffUp_map[mapIt->first] = mean_pair;
   RingCoeffUpError_map[mapIt->first] = Error_pair;
  }
  else {
   RingCoeffUp_map[mapIt->first] = 0;
   RingCoeffUpError_map[mapIt->first] = 0;
  }
 }
 //---- DOWN ----  
 for (std::map<int, TH1F*>::iterator mapIt = dEdx_Ring_1M_Down_Histos.begin (); mapIt != dEdx_Ring_1M_Down_Histos.end ();++mapIt)
 {
  if (mapIt->second->GetEntries() > 15) {
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   RingCoeffDown_map[mapIt->first] = mean_pair;
   RingCoeffDownError_map[mapIt->first] = Error_pair;
  }
  else {
   RingCoeffDown_map[mapIt->first] = 0;
   RingCoeffDownError_map[mapIt->first] = 0;
  }
 } //AM ---- end loop over dEdx_Histos ---- Slice Ring ----

 //---- TGraphErrors UP ----
 TGraphErrors CoeffUP;
 CoeffUP.SetName("CoeffUP");
 CoeffUP.SetTitle("CoeffUP");
 int counterCoeffUP = 0;
 for (int kk=0; kk<171;kk++){
  int numberIEta = kk - 85;
  std::map<int, double>::iterator RingCoeffUp_map_iter = RingCoeffUp_map.find(numberIEta);
  std::map<int, double>::iterator RingCoeffUpError_map_iter = RingCoeffUpError_map.find(numberIEta);
  if (RingCoeffUp_map_iter != RingCoeffUp_map.end()){
   CoeffUP.SetPoint(counterCoeffUP,numberIEta,RingCoeffUp_map_iter->second);
   CoeffUP.SetPointError(counterCoeffUP,0,RingCoeffUpError_map_iter->second);
   counterCoeffUP++;
  }
  else {
   CoeffUP.SetPoint(counterCoeffUP,numberIEta,0);
   CoeffUP.SetPointError(counterCoeffUP,0,0);
   counterCoeffUP++;
  }
 }
 
 //---- TGraphErrors DOWN ----
 TGraphErrors CoeffDOWN;
 CoeffDOWN.SetName("CoeffDOWN");
 CoeffDOWN.SetTitle("CoeffDOWN");
 int counterCoeffDOWN = 0;
 for (int kk=0; kk<171;kk++){
  int numberIEta = kk - 85;
  std::map<int, double>::iterator RingCoeffDown_map_iter = RingCoeffDown_map.find(numberIEta);
  std::map<int, double>::iterator RingCoeffDownError_map_iter = RingCoeffDownError_map.find(numberIEta);
  if (RingCoeffDown_map_iter != RingCoeffDown_map.end()){
   CoeffDOWN.SetPoint(counterCoeffDOWN,numberIEta,RingCoeffDown_map_iter->second);
   CoeffDOWN.SetPointError(counterCoeffDOWN,0,RingCoeffDownError_map_iter->second);
   counterCoeffDOWN++;
  }
  else {
   CoeffDOWN.SetPoint(counterCoeffDOWN,numberIEta,0);
   CoeffDOWN.SetPointError(counterCoeffDOWN,0,0);
   counterCoeffDOWN++;
  }
 }
  
 
 
 //------------------------------------------------
 //---------------------- dE ----------------------
 //------------------------------------------------

 /*
  
 //---- dE map from the fit ---- single crystal ----
 std::map<int, double> dEXtalCoeff_map ;
 std::map<int, double> dEXtalCoeffError_map ;

 TH2F dEEtaPhi ("dEEtaPhi","dEEtaPhi",360,1,361,171,-85,86) ;

 MinXtal_cut = 20;
 //AM ---- loop over dE_Histos single crystal ----
 for (std::map<int, TH1F*>::iterator mapIt = dE_Histos.begin (); mapIt != dE_Histos.end ();++mapIt)
 {
  if( (mapIt->second->GetEntries()) > MinXtal_cut ){
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   dEXtalCoeff_map[mapIt->first] = mean_pair;
   dEXtalCoeffError_map[mapIt->first] = Error_pair;
   //---- fill histogram ----
   EBDetId dummyMy(mapIt->first);
   std::cerr << " iphi =  " << dummyMy.iphi() << " ieta = " << dummyMy.ieta() << " -> dE = " << mean_pair << std::endl;
   dEEtaPhi.Fill(dummyMy.iphi(),dummyMy.ieta(),mean_pair);
  }
  else {
   dEXtalCoeff_map[mapIt->first] = 0;
   dEXtalCoeffError_map[mapIt->first] = 0;
  }
 }  //AM ---- end loop over dE_Histos single crystal ----

 
  //---- dE slice of the Ring ----
 
 std::map<int, double> dERingCoeffUp_map ;
 std::map<int, double> dERingCoeffUpError_map ;
 
 std::map<int, double> dERingCoeffDown_map ;
 std::map<int, double> dERingCoeffDownError_map ;
 
 //AM ---- loop over dE_Histos ---- Slice Ring ----
 //---- UP ----
 for (std::map<int, TH1F*>::iterator mapIt = dE_Ring_1M_Up_Histos.begin (); mapIt != dE_Ring_1M_Up_Histos.end ();++mapIt)
 {
  if (mapIt->second->GetEntries() > 15) {
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   dERingCoeffUp_map[mapIt->first] = mean_pair;
   dERingCoeffUpError_map[mapIt->first] = Error_pair;
  }
  else {
   dERingCoeffUp_map[mapIt->first] = 0;
   dERingCoeffUpError_map[mapIt->first] = 0;
  }
 }
 //---- DOWN ----  
 for (std::map<int, TH1F*>::iterator mapIt = dE_Ring_1M_Down_Histos.begin (); mapIt != dE_Ring_1M_Down_Histos.end ();++mapIt)
 {
  if (mapIt->second->GetEntries() > 15) {
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   dERingCoeffDown_map[mapIt->first] = mean_pair;
   dERingCoeffDownError_map[mapIt->first] = Error_pair;
  }
  else {
   dERingCoeffDown_map[mapIt->first] = 0;
   dERingCoeffDownError_map[mapIt->first] = 0;
  }
 } //AM ---- end loop over dE_Histos ---- Slice Ring ----

 //---- TGraphErrors UP ----
 TGraphErrors dECoeffUP;
 dECoeffUP.SetName("dECoeffUP");
 dECoeffUP.SetTitle("dECoeffUP");
 int dECounterCoeffUP = 0;
 for (int kk=0; kk<171;kk++){
  int numberIEta = kk - 85;
  std::map<int, double>::iterator dERingCoeffUp_map_iter = dERingCoeffUp_map.find(numberIEta);
  std::map<int, double>::iterator dERingCoeffUpError_map_iter = dERingCoeffUpError_map.find(numberIEta);
  if (dERingCoeffUp_map_iter != dERingCoeffUp_map.end()){
   dECoeffUP.SetPoint(dECounterCoeffUP,numberIEta,dERingCoeffUp_map_iter->second);
   dECoeffUP.SetPointError(dECounterCoeffUP,0,dERingCoeffUpError_map_iter->second);
   dECounterCoeffUP++;
  }
  else {
   dECoeffUP.SetPoint(dECounterCoeffUP,numberIEta,0);
   dECoeffUP.SetPointError(dECounterCoeffUP,0,0);
   dECounterCoeffUP++;
  }
 }
 
 //---- TGraphErrors DOWN ----
 TGraphErrors dECoeffDOWN;
 dECoeffDOWN.SetName("dECoeffDOWN");
 dECoeffDOWN.SetTitle("dECoeffDOWN");
 int dECounterCoeffDOWN = 0;
 for (int kk=0; kk<171;kk++){
  int numberIEta = kk - 85;
  std::map<int, double>::iterator dERingCoeffDown_map_iter = dERingCoeffDown_map.find(numberIEta);
  std::map<int, double>::iterator dERingCoeffDownError_map_iter = dERingCoeffDownError_map.find(numberIEta);
  if (dERingCoeffDown_map_iter != dERingCoeffDown_map.end()){
   dECoeffDOWN.SetPoint(dECounterCoeffDOWN,numberIEta,dERingCoeffDown_map_iter->second);
   dECoeffDOWN.SetPointError(dECounterCoeffDOWN,0,dERingCoeffDownError_map_iter->second);
   dECounterCoeffDOWN++;
  }
  else {
   dECoeffDOWN.SetPoint(dECounterCoeffDOWN,numberIEta,0);
   dECoeffDOWN.SetPointError(dECounterCoeffDOWN,0,0);
   dECounterCoeffDOWN++;
  }
 }
  
 
 
 
 //---- dE map from the fit ---- All Ring ----
 std::map<int, double> dERingCoeff_map ;
 std::map<int, double> dERingCoeffError_map ;
 
 //AM ---- loop over dE_Histos ---- Ring ----
 for (std::map<int, TH1F*>::iterator mapIt = dE_Ring_Histos.begin (); mapIt != dE_Ring_Histos.end ();++mapIt)
 {
  if (mapIt->second->GetEntries() > 15) {
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   dERingCoeff_map[mapIt->first] = mean_pair;
   dERingCoeffError_map[mapIt->first] = Error_pair;
  }
  else {
   dERingCoeff_map[mapIt->first] = 0;
   dERingCoeffError_map[mapIt->first] = 0;
  }
 }  //AM ---- end loop over dE_Histos Ring ----

 TGraphErrors dECoeff;
 dECoeff.SetName("dECoeff");
 dECoeff.SetTitle("dECoeff");
 int dECounterCoeff = 0;
 for (int kk=0; kk<171;kk++){
  int numberIEta = kk - 85;
  std::map<int, double>::iterator dERingCoeff_map_iter = dERingCoeff_map.find(numberIEta);
  std::map<int, double>::iterator dERingCoeffError_map_iter = dERingCoeffError_map.find(numberIEta);
  if (dERingCoeff_map_iter != dERingCoeff_map.end()){
   dECoeff.SetPoint(dECounterCoeff,numberIEta,dERingCoeff_map_iter->second);
   dECoeff.SetPointError(dECounterCoeff,0,dERingCoeffError_map_iter->second);
   dECounterCoeff++;
  }
  else {
   dECoeff.SetPoint(dECounterCoeff,numberIEta,0);
   dECoeff.SetPointError(dECounterCoeff,0,0);
   dECounterCoeff++;
  }
 }
  
 */
 //----------------------------------------------------
 //---------------------- End dE ----------------------
 //----------------------------------------------------

 /*
 
 //---- dE/dx map from the fit ---- Ring ----
 std::map<int, double> RingCoeff_map ;
 std::map<int, double> RingCoeffError_map ;
 
 //AM ---- loop over dEdx_Histos ---- Ring ----
 for (std::map<int, TH1F*>::iterator mapIt = dEdx_Ring_Histos.begin (); mapIt != dEdx_Ring_Histos.end ();++mapIt)
 {
     //   mapIt->second->Fit ("gaus") ;

     // get the peak of dEondX (da controllare con piu' statistica anche i "puntatori")
  
  if (mapIt->second->GetEntries() > 15) {
   std::pair<double,double> MeanAndErrorPair = fitdEdx(mapIt->second);
   mapIt->second->Write();
   double mean_pair = MeanAndErrorPair.first;
   double Error_pair = MeanAndErrorPair.second;
   
   RingCoeff_map[mapIt->first] = mean_pair;
   RingCoeffError_map[mapIt->first] = Error_pair;
  }
  else {
   RingCoeff_map[mapIt->first] = 0;
   RingCoeffError_map[mapIt->first] = 0;
  }
 }  //AM ---- end loop over dEdx_Histos Ring ----

 
 
 
 //---- transform a map in histogram ----
 
 std::cerr << "     Creo TProfile ...." << std::endl;

 TProfile Coeff("Coeff","Coefficients in the ring",171, -85, 85);
 Double_t Xcoeff[171];
 Double_t errXcoeff[171];
 Double_t Ycoeff[171];
 Double_t errYcoeff[171];
 
 for (int kk=0; kk<171;kk++){
  int numberIEta = kk - 85;
  Xcoeff[kk] = numberIEta;
  errXcoeff[kk] = 0;
  std::map<int, double>::iterator RingCoeff_map_iter = RingCoeff_map.find(numberIEta);
  std::map<int, double>::iterator RingCoeffError_map_iter = RingCoeffError_map.find(numberIEta);
  if (RingCoeff_map_iter != RingCoeff_map.end()){
   std::cerr << "   Sono entrato con ieta = " << numberIEta << std::endl;
   Ycoeff[kk] = RingCoeff_map_iter->second;
   std::cerr << "   Y =  " << Ycoeff[kk];
   errYcoeff[kk] = RingCoeffError_map_iter->second;
  }
  else {
   Ycoeff[kk] = 0;
   errYcoeff[kk] = 0;
  }
 }
 
 std::cerr << "     Creo TGraphErrors ...." << std::endl;
 
 TGraphErrors TGraphErrorsCoefficients;
 TGraphErrorsCoefficients.SetName("TGraphErrorsCoefficients");
 TGraphErrorsCoefficients.SetTitle("TGraphErrorsCoefficients");
 int counterCoeff = 0;
 for (int kk=0; kk<171;kk++){
  TGraphErrorsCoefficients.SetPoint(counterCoeff,Xcoeff[kk],Ycoeff[kk]);
  TGraphErrorsCoefficients.SetPointError(counterCoeff,errXcoeff[kk],errYcoeff[kk]);
  counterCoeff++;
  std::cerr << "     numberIEta = " << Xcoeff[kk] << " Ycoeff[kk] = " << Ycoeff[kk];
  std::cerr << "   errYcoeff[numberIEta] = " << errYcoeff[kk] << std::endl;
 }

 
 
 for (std::map<int, TH1F*>::iterator mapIt = dEdx_Ring_Histos.begin (); mapIt != dEdx_Ring_Histos.end ();++mapIt)
 {
  Coeff.Fill(mapIt->first,RingCoeff_map[mapIt->first]);
 }
 */

 
 //---- Saving histograms ----
 //---- dEdx_Histos.
 
 dEdx_Tutti.GetXaxis()->SetTitle("dE/dX");
 dEdx_Tutti.Write();
 
 dE_Tutti.GetXaxis()->SetTitle("dE");
 dE_Tutti.Write();
 
 dX_Tutti.GetXaxis()->SetTitle("dX");
 dX_Tutti.Write();
 
 occupancyEnergy.GetXaxis()->SetTitle("i#phi");
 occupancyEnergy.GetYaxis()->SetTitle("i#eta");
 occupancyEnergy.Write () ;

 occupancy.GetXaxis()->SetTitle("i#phi");
 occupancy.GetYaxis()->SetTitle("i#eta");
 occupancy.Write ();
 
 /*
 Coeff.GetXaxis()->SetTitle("i#eta");
 Coeff.GetYaxis()->SetTitle("dE/dx");
 Coeff.Write();
 
 TGraphErrorsCoefficients.GetXaxis()->SetTitle("i#eta");
 TGraphErrorsCoefficients.GetYaxis()->SetTitle("dE/dx");
 TGraphErrorsCoefficients.Write();
 */
 /*
 dEdXEtaPhi.GetXaxis()->SetTitle("i#phi");
 dEdXEtaPhi.GetYaxis()->SetTitle("i#eta");
 dEdXEtaPhi.Write();
 */
 CoeffDOWN.GetXaxis()->SetTitle("i#eta");
 CoeffDOWN.GetYaxis()->SetTitle("dE/dx");
 CoeffDOWN.Write();

 CoeffUP.GetXaxis()->SetTitle("i#eta");
 CoeffUP.GetYaxis()->SetTitle("dE/dx");
 CoeffUP.Write();

 dEdxVsEnergy.GetXaxis()->SetTitle("dE/dx");
 dEdxVsEnergy.GetYaxis()->SetTitle("E");
 dEdxVsEnergy.Write();
 
 
 //---- dE Graphs ----  
 /* 
 dECoeff.GetXaxis()->SetTitle("i#eta");
 dECoeff.GetYaxis()->SetTitle("dE");
 dECoeff.Write();

 dECoeffDOWN.GetXaxis()->SetTitle("i#eta");
 dECoeffDOWN.GetYaxis()->SetTitle("dE");
 dECoeffDOWN.Write();
 
 dECoeffUP.GetXaxis()->SetTitle("i#eta");
 dECoeffUP.GetYaxis()->SetTitle("dE");
 dECoeffUP.Write();
 
 dEEtaPhi.GetXaxis()->SetTitle("i#phi");
 dEEtaPhi.GetYaxis()->SetTitle("i#eta");
 dEEtaPhi.Write();

 */
 //---- Alpha Graphs ----
 
 dEdx_alpha.GetXaxis()->SetTitle("#alpha");
 dEdx_alpha.Write();

 alpha_up.GetXaxis()->SetTitle("#alpha");
 alpha_up.Write();

 alpha_down.GetXaxis()->SetTitle("#alpha");
 alpha_down.Write();

 muon_up.GetXaxis()->SetTitle("#alpha");
 muon_up.Write();

 muon_down.GetXaxis()->SetTitle("#alpha");
 muon_down.Write();

 EtaPhi_events.GetXaxis()->SetTitle("i #eta");
 EtaPhi_events.GetYaxis()->SetTitle("i #phi");
 EtaPhi_events.Write();

 EtaPhi_alpha_xtal.GetXaxis()->SetTitle("i #eta");
 EtaPhi_alpha_xtal.GetYaxis()->SetTitle("i #phi");
 EtaPhi_alpha_xtal.Write();

 EtaPhi_alpha_xtalMU.GetXaxis()->SetTitle("i #eta");
 EtaPhi_alpha_xtalMU.GetYaxis()->SetTitle("i #phi");
 EtaPhi_alpha_xtalMU.Write();

 EtaPhi_alpha_MU.GetXaxis()->SetTitle("i #eta");
 EtaPhi_alpha_MU.GetYaxis()->SetTitle("i #phi");
 EtaPhi_alpha_MU.Write();
 
 //---- end Graphs ----
 
 saving.Close () ;
 delete chain ;
 return 0 ;

}



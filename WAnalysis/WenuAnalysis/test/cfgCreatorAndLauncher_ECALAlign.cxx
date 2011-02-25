{
 ///==== cfg creator ====
 
 int WP = 80;
 
 
 #include <iomanip>
 
 char *nameSample[1000];
 
 double xsection[1000];
 
 int numberOfSamples = 0;
 
 std::ifstream inFile("test/samples.txt");
 std::string buffer;
 
 while(!inFile.eof()){
  getline(inFile,buffer);
  std::cout << "buffer = " << buffer << std::endl;
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer[0] != '#'){
    std::stringstream line( buffer );       
    nameSample[numberOfSamples] = new char [10000];
    line >> nameSample[numberOfSamples]; 
    std::cout << nameSample[numberOfSamples] << " ";
    
    line >> xsection[numberOfSamples]; 
    std::cout << xsection[numberOfSamples] << " ";
    std::cout << std::endl;
    numberOfSamples++;
   } 
  }
 }
 
 system ("rm ___dir.txt");
 system("pwd >> ___dir.txt");
 system("mkdir test/dir_cfg_ECALAlign");
 for (int iSample = 0; iSample < numberOfSamples; iSample++){
  system ("rm ___temp.txt");
  char getNameList[1000];
  if (xsection[iSample] != -1) sprintf(getNameList,"rfdir /castor/cern.ch/user/a/amassiro/WeCalib/MC/Winter10/%s/ | awk '{print \"@\"$9\"@\"}' | tr \"@\" \"\\\"\" >> ___temp.txt",nameSample[iSample]);
  else sprintf(getNameList,"rfdir /castor/cern.ch/user/a/amassiro/WeCalib/DATA/%s/ | awk '{print \"@\"$9\"@\"}' | tr \"@\" \"\\\"\" >> ___temp.txt",nameSample[iSample]);
  std::cout << " to do : " << getNameList << std::endl;
  system (getNameList);

  ///==== prepare cfg file ====
  std::ofstream myfile;
  char nameFile[1000];
  sprintf(nameFile,"test/dir_cfg_ECALAlign/ECALAlign_%s_WP%d.py",nameSample[iSample],WP);
  myfile.open (nameFile);
  myfile << std::fixed;
  myfile << std::setprecision(20);
  myfile << "import FWCore.ParameterSet.Config as cms" << std::endl;
  myfile << "process = cms.Process(\"TEST\")" << std::endl;
  myfile << "process.Input = cms.PSet(" << std::endl;
  myfile << "   WP = cms.untracked.int32(" << WP << ")," << std::endl;
  myfile << "   inputFileList = cms.vstring(" << std::endl;

  int numFiles = 0;
  std::ifstream inFileList("___temp.txt");
  while(!inFileList.eof()){
   getline(inFileList,buffer);
   std::cout << "buffer = " << buffer << std::endl;
   if (buffer != ""){ ///---> save from empty line at the end!
    if (numFiles < 100) { ///---- max 100 files ... è rischioso? ...
     if (numFiles != 0) myfile << "," << std::endl;
     myfile << "      " << buffer << " ";
     numFiles++;   
    }   
   }   
  }
  myfile << std::endl;

  myfile << "    )," << std::endl;
  myfile << "    outputRootFileName = cms.string(\"ECALAlignMC_" << nameSample[iSample] << ".root\")" << std::endl;
  myfile << "   ) " << std::endl;
  myfile.close();

  ///==== prepare sh file ====  

  std::string dirString;
  std::ifstream inFileDir("___dir.txt");
  while(!inFileDir.eof()){
   getline(inFileDir,buffer);
   if (buffer != ""){ ///---> save from empty line at the end!
    dirString = buffer;
   }
  }
  std::cout << "dirString = " << dirString << std::endl;

  std::ofstream myfileSh;
  char nameFileSh[1000];
  sprintf(nameFileSh,"%s/test/dir_cfg_ECALAlign/ECALAlign_%s_WP%d.sh",dirString.c_str(),nameSample[iSample],WP);
  myfileSh.open (nameFileSh);

  myfileSh << std::fixed;
  myfileSh << std::setprecision(20);
  myfileSh << "#!/bin/sh" << std::endl;

  myfileSh << " cd " << dirString << std::endl;
  myfileSh << " eval `scramv1 runtime -sh` " << std::endl;
  myfileSh << " cd -" << std::endl;
  myfileSh << " mkdir temp_" << nameSample[iSample] << std::endl;
  myfileSh << " cd temp_" << nameSample[iSample] << std::endl;
  if (xsection[iSample] != -1) {
   myfileSh << " rfdir /castor/cern.ch/user/a/amassiro/WeCalib/MC/Winter10/" << nameSample[iSample] << " | awk '{print \"rfcp /castor/cern.ch/user/a/amassiro/WeCalib/MC/Winter10/" << nameSample[iSample] << "/\"$9\" ./\"}' | /bin/sh &> __time.txt" << std::endl;
   myfileSh << " head -100 __time.txt | awk '{print $1}' | /bin/sh " << std::endl;
  }
  else myfileSh << " rfdir /castor/cern.ch/user/a/amassiro/WeCalib/DATA/" << nameSample[iSample] << " | awk '{print \"rfcp /castor/cern.ch/user/a/amassiro/WeCalib/DATA/" << nameSample[iSample] << "/\"$9\" ./\"}' | /bin/sh" << std::endl;

  myfileSh << " WenuAnalysisNtple_Alignment " << dirString << "/" << nameFile << std::endl;
  myfileSh << " rfcp ECALAlignMC_" << nameSample[iSample] << ".root  /castor/cern.ch/user/a/amassiro/WeCalib/ReReco39X/ECALAlignResult_1BC_WP" << WP << "/" << std::endl;
  myfileSh << " cd -" << std::endl;
  myfileSh << " rm -r temp_" << nameSample[iSample] << std::endl;

  myfileSh.close();

  ///==== Launch sh file ====
  char nameCommand[1000];
  sprintf(nameCommand,"chmod +x %s",nameFileSh);
  std::cout << ">>>> to do : " << nameCommand << std::endl;
  system (nameCommand);

  sprintf(nameCommand,"bsub -q 1nh %s",nameFileSh);
  std::cout << ">>>> to do : " << nameCommand << std::endl;
  system (nameCommand);

 }

 system ("rm ___temp.txt");
 system ("rm ___dir.txt");
}





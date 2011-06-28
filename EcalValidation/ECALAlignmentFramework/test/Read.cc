int ReadFile(char nameFile[1000],char* nameSample[1000], char* nameHumanReadable[1000], char* xsectionName[1000]){
 
 int numberOfSamples = 0;
 std::ifstream inFile(nameFile);
 std::string buffer; 
 while(!inFile.eof()){
  getline(inFile,buffer);
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer.at(0) != '#'){ 
    std::stringstream line( buffer );       
    nameSample[numberOfSamples] = new char [10000];
    line >> nameSample[numberOfSamples]; 
    std::cout << nameSample[numberOfSamples] << " ";
    
    xsectionName[numberOfSamples] = new char [10000];
    line >> xsectionName[numberOfSamples]; 
    std::cout << xsectionName[numberOfSamples] << " ";
    
    nameHumanReadable[numberOfSamples] = new char [10000];
    line >> nameHumanReadable[numberOfSamples]; 
    std::cout << nameHumanReadable[numberOfSamples] << " ";
    
    std::cout << std::endl;
    numberOfSamples++;
   } 
  }
 }
 
 return numberOfSamples;
}


///==== read list of selections file ====

int ReadFileCut(std::string CutFile, std::vector<std::string>& vCut){
 std::ifstream inFile(CutFile.c_str());
 std::string buffer; 
 while(!inFile.eof()){
  getline(inFile,buffer);
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer.at(0) != '#'){ 
    vCut.push_back(buffer);
    std::cout << " cut = " << buffer << std::endl;
   } 
  }
 }
 return vCut.size();
}


int ReadFileCutHR(std::string CutFile, std::vector<std::string>& vCutHR){
 std::ifstream inFile(CutFile.c_str());
 std::string buffer; 
 while(!inFile.eof()){
  getline(inFile,buffer);
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer.at(0) != '#'){ 
    vCutHR.push_back(buffer);
    std::cout << " cut (HR) = " << buffer << std::endl;
   } 
  }
 }
 return vCutHR.size();
}




///==== read list of variables file ====

int ReadFileVar(std::string VarFile, std::vector<double>& vMin, std::vector<double>& vMax, std::vector<int>& vNBin, std::vector<std::string>& vVarName, std::vector<std::string>& vVarNameHR){
 std::ifstream inFile(VarFile.c_str());
 std::string buffer;
 char temp[10000];
 std::string tempString;
 while(!inFile.eof()){
  getline(inFile,buffer);
  if (buffer != ""){ ///---> save from empty line at the end!
   if (buffer.at(0) != '#'){   
    std::stringstream line( buffer );
    line >> temp; tempString = temp; vVarName.push_back(tempString);
    line >> temp; vMin.push_back(atof(temp));
    line >> temp; vMax.push_back(atof(temp));
    line >> temp; vNBin.push_back(atoi(temp));
    line >> temp; tempString = temp; vVarNameHR.push_back(tempString);
    std::cout << " var = " << buffer << std::endl;
   } 
  }
 }
 return vVarName.size();
}
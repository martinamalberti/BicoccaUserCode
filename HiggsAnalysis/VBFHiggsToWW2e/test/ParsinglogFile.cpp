#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "math.h"
#include <fstream>
#include <stdlib.h>
using namespace std;

class logParser {
public:
  logParser();
  logParser(string fileList, string modulesList);
  //explicit logParser(string fileList, string modulesList);
  ~logParser(){;}
  void Print();
  bool InitOk_;
private:
  std::map<string,pair<float,float> > events_;
  std::map<string,bool> moduleFound_;
  //std::map<string,bool> moduleErrors_;
  std::vector<string> files_;
  bool InitFileList(string fileList);
  bool InitModuleList(string moduleList);
  bool parseSingleFile(string FileName);
};

logParser::logParser(string fileList, string modulesList){
  // cout<<"AAAAAAAAAA"<<endl;
  InitOk_ = InitFileList( fileList );
  InitOk_ = InitOk_ && InitModuleList( modulesList );
  if(InitOk_){
    for(int i=0;i<files_.size();i++)
      parseSingleFile(files_[i]);
  }
  Print();
}

bool logParser::InitFileList( string fileList ){ 

  ifstream FileIn(fileList.c_str());
  if( !(FileIn.is_open()) ){cout<<"Error: fileList "<<fileList <<" not found!!"<<endl;return false;}
  char Buffer[5000];
  int howMany = 0;
  char FileName[5000];
  while( 1 ){
    if(FileIn.eof()){break;}
    FileIn.getline(Buffer,5000);
    if (strstr(Buffer,"#") || (strspn(Buffer," ") == strlen(Buffer))){continue;}
    //cout<<Buffer<<endl;
    int num=sscanf(Buffer,"%s",&FileName);
    if(num != 1 ){cout<<"file list Error in line  --> "<<Buffer<<endl; return false;}
    howMany++;
    files_.push_back(string(FileName));
  }//end of file
  FileIn.close();
  cout<<"Found "<<howMany<<" files in fileList"<<endl;
  return true;
}

bool logParser::InitModuleList( string moduleList ){

 ifstream FileIn(moduleList.c_str());
  if( !(FileIn.is_open()) ){cout<<"Error: moduleList "<<moduleList <<" not found!!"<<endl;return false;}
  char Buffer[5000];
  int howMany = 0;
  char ModuleName[5000];
  while( 1 ){
    if(FileIn.eof()){break;}
    FileIn.getline(Buffer,5000);
    if (strstr(Buffer,"#") || (strspn(Buffer," ") == strlen(Buffer))){continue;}
    //cout<<Buffer<<endl;
    int num=sscanf(Buffer,"%s",&ModuleName);
    if(num != 1 ){cout<<"module list Error in line  --> "<<Buffer<<endl; return false;}
    howMany++;
    pair<float,float> zero(0,0);
    events_[string(ModuleName)]=zero;
    moduleFound_[string(ModuleName)]=false;
  }//end of file
  FileIn.close();
  cout<<"Found "<<howMany<<" modules in ModuleList"<<endl;
  return true;}

bool logParser::parseSingleFile(string FileName){
  ifstream FileIn(FileName.c_str());
  if( !(FileIn.is_open()) ){cout<<"Error: moduleList "<< FileName<<" not found!!"<<endl;return false;}
  char Buffer[5000];
  int howMany = 0;
  char ModuleName[5000],Flag[5000],dum[5000],dum2[5000],dum3[5000],dum4[5000];
  int d1,d2,d3,d4,err=0,passed=0,visited=0;
  map<string,bool> check(moduleFound_);
  bool start = false;
  while( 1 ){
    if(FileIn.eof()){break;}
    FileIn.getline(Buffer,5000);
    if (strstr(Buffer,"#") || (strspn(Buffer," ") == strlen(Buffer))){continue;}
    sscanf(Buffer,"%s %s %s %s",&Flag, &dum, &dum2, &dum3, &dum4);
    if(string(Flag) != "TrigReport"){continue;}
    if( string(dum2) == "Module" && string(dum3)=="Summary"){start = true;}
    if(!start){continue;}
    //cout<<Buffer<<endl;
    int num=sscanf(Buffer,"%s %d %d %d %d %d %s",&Flag,&d1,&visited,&passed,&d4,&err,&ModuleName);
    //    if(string(ModuleName)=="hltL1IsoSingleElectronHcalIsolFilter"){cout<<"UUUUUUUUUUUU"<<endl;}
    //cout<<ModuleName<<endl;cout<<endl;
    if(num != 7 ) {continue;}

      //{cout<<"module list Error in line  --> "<<Buffer<<endl; return false;}
    if( events_.find(string(ModuleName)) != events_.end()){
      howMany++;
      events_[string(ModuleName)].first += visited;
      events_[string(ModuleName)].second += passed;
      check[string(ModuleName)] = true;
    }
  }//end of file
  FileIn.close();
  bool ok;
  map<string,bool>::iterator it;
  for(it = check.begin(); it != check.end(); it++){
    ok = ok && it->second;
    if(!it->second){cout<<"Error in file: "<<FileName<<" Module: "<<it->first<<" not found"<<endl;}
  }
  return ok;
}

void logParser::Print(){
  cout<<"@@@@@@@@@@@@@@@@@@@@@"<<endl;
  map<string,pair<float,float> >::iterator it;
  for(it = events_.begin(); it != events_.end(); it++){ 
    cout<<it->first<<" "<<(it->second).second<<" / "<<(it->second).first;
    cout<<" = "<<(it->second).second/(it->second).first<<endl;
  }
  cout<<"#####################"<<endl;
}
int main(int argc, char **argv)
{

  if(argc < 3){cout<<"Usage : executable FileList ModuleList"<<endl;return -3;}
  string a = string(argv[1]);
  string b = string(argv[2]);
  
  logParser p( a , b );
  //  logParser a( string(argv[1]) , string(argv[2]) );
  // a.Print();
  return 0;
}

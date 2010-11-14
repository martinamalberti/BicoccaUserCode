# include "ABCDMultiDimensional.h"
# include <iostream>
# include <cstdlib>
# include <string>
# include <vector>
# include "TH2D.h"
# include "TFile.h"
# include "TTree.h"
# include "ConfigParser.h"
# include "TString.h"
# include "TCanvas.h"
# include "TMinuit.h"
#include "TEntryList.h"

/** STANDARD AND COMPLETE CONSTRUCTOR*/

ABCDMultiDimensional::ABCDMultiDimensional(TTree* tree_in,
     const std::string & output,
     const std::vector<std::string> & varY,
     const std::vector<std::string> & MinMajY,
     const std::vector<double> & cutY,
     const std::vector<std::string> & varX,
     const std::vector<std::string> & MinMajX,
     const std::vector<double> & cutX,
     const std::string &  gen_cut
     ): output_(output),
     varY_(varY),
     MinMajY_(MinMajY),
     OppositeMinMajY_(OppositeSign(MinMajY)),
     cutY_(cutY),
     varX_(varX),
     MinMajX_(MinMajX),
     OppositeMinMajX_(OppositeSign(MinMajX)),
     cutX_(cutX),
     gen_cut_(gen_cut)
{
 tree_in_ = NULL;
 SetTree(tree_in);
 InitializeHisto();

//  for(int iVar=0; iVar<varX_.size(); iVar++){
//   std::cout << "   X: Variable[" << iVar << "] : " << varX_[iVar] << " " <<  MinMajX_[iVar] << " " << cutX_[iVar] << std::endl;
//  }
//  for(int iVar=0; iVar<varY_.size(); iVar++){
//   std::cout << "   Y: Variable[" << iVar << "] : " << varY_[iVar] << " " <<  MinMajY_[iVar] << " " << cutY_[iVar] << std::endl;
//  }
 
}

/** DESTRUCTOR*/

ABCDMultiDimensional::~ABCDMultiDimensional()
{ 
 ResetHisto();
 if(outFile_ != NULL)
 { 
  delete outFile_;
  outFile_ = NULL;
 } 
}

std::vector<std::string> ABCDMultiDimensional::OppositeSign(const std::vector<std::string>& oldVect)
{
 std::vector<std::string> newVect;
 for (std::vector<std::string>::const_iterator it = oldVect.begin(); it!=oldVect.end(); ++it) {
  if ((*it) == "<") newVect.push_back(">=");
  if ((*it) == ">") newVect.push_back("<=");
  if ((*it) == "<=") newVect.push_back(">");
  if ((*it) == ">=") newVect.push_back("<");
  if ((*it) == "==") newVect.push_back("!=");
  if ((*it) == "!=") newVect.push_back("==");
 }
 return newVect;
}


/** Initialization of histos. Called by creator */
void ABCDMultiDimensional::InitializeHisto()
{
 vRegionName.push_back('A');
 vRegionName.push_back('B');
 vRegionName.push_back('C');
 vRegionName.push_back('D');
 
 for(int i=0; i<4; i++) { 
  VetHisto_.push_back(NULL);
//   VetHistoABCD_.push_back(NULL);
 }  
}

/**CREATOR OF ABCDMultiDimensional CONFIGURATION AFTER THE GENERAL CUT IMPOSED */

void ABCDMultiDimensional::CreateHisto()
{   
 ResetHisto();
 
 Htot_ = new TH2D ("Htot_","Htot",2,0,2,2,0,2);

 for(int itRegion = 0; itRegion < 4; itRegion++)
 {  
  TString Variable;
  TString General_Cut;
  std::string histoName = "H" + vRegionName.at(itRegion);
  
  TH2D* Htemp = new TH2D (histoName.c_str(),histoName.c_str(),2,0.0,2.0,2,0.0,2.0);
  
  General_Cut = Form ("1");

  for (int iVar = 0; iVar < varY_.size(); iVar++){
   if (itRegion == 0 || itRegion == 2) General_Cut = Form("%s && %s %s %f ", General_Cut.Data(), varY_.at(iVar).c_str(), MinMajY_.at(iVar).c_str(), cutY_[iVar]);
   else General_Cut = Form("%s && %s %s %f ", General_Cut.Data(), varY_.at(iVar).c_str(), OppositeMinMajY_.at(iVar).c_str(), cutY_[iVar]);
  }
  for (int iVar = 0; iVar < varX_.size(); iVar++){
   if (itRegion == 0 || itRegion == 1) General_Cut = Form("%s && %s %s %f ", General_Cut.Data(), varX_.at(iVar).c_str(), MinMajX_.at(iVar).c_str(), cutX_[iVar]);
   else General_Cut = Form("%s && %s %s %f ", General_Cut.Data(), varX_.at(iVar).c_str(), OppositeMinMajX_.at(iVar).c_str(), cutX_[iVar]);
  }
  
  General_Cut = General_Cut + Form(" && ( %s )",gen_cut_.c_str());
  
//   std::cout << " General_Cut = " << General_Cut.Data() << std::endl;
  tree_in_->SetEntryList(0); 
  tree_in_->Draw(">> myList",General_Cut.Data(),"entrylist");
  TEntryList *myList = (TEntryList*) gDirectory->Get("myList");
  int num = myList->GetN();
//   tree_in_->SetEntryList(myList); 
//   std::cerr << " num[" << vRegionName.at(itRegion) << "] = " << num << std::endl;
  
  if (itRegion == 0) for (int ntr = 0; ntr < num; ntr++) Htemp->Fill(0.5,0.5);
  if (itRegion == 1) for (int ntr = 0; ntr < num; ntr++) Htemp->Fill(0.5,1.5);
  if (itRegion == 2) for (int ntr = 0; ntr < num; ntr++) Htemp->Fill(1.5,0.5);
  if (itRegion == 3) for (int ntr = 0; ntr < num; ntr++) Htemp->Fill(1.5,1.5);
  
  VetHisto_.at(itRegion) = Htemp;
  Htot_->Add(Htemp);
 }
 
}


/** return a pointer to the total ABCDMultiDimensional 2-D space*/

TH2D* ABCDMultiDimensional::GetTotalHisto()
{
 if(Htot_!=NULL)
  return(Htot_);
}   

/** RETURN A POINTER to THE SINGLE ABCDMultiDimensional REGION */

TH2D* ABCDMultiDimensional::GetHisto(char letter)
{
 if(letter=='A')
  return(VetHisto_[0]);
 
 if(letter=='B')
  return(VetHisto_[1]);
 
 
 if(letter=='C')
  return(VetHisto_[2]);
 
 if(letter=='D')
  return(VetHisto_[3]);
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(NULL);
}  

TH2D* ABCDMultiDimensional::GetHisto(int number)
{
 if(number==0)
  return(GetHisto('A'));
 if(number==1)
  return(GetHisto('B'));
 if(number==2)
  return(GetHisto('C'));
 if(number==3)
  return(GetHisto('C'));
 
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(NULL);
}  

/**Return the entries of a fixed region */

double ABCDMultiDimensional::GetEntries(char region)
{
 if(region=='A')
  return(VetHisto_[0]->GetEntries());
 if(region=='B')
  return(VetHisto_[1]->GetEntries());
 if(region=='C')
  return(VetHisto_[2]->GetEntries());
 if(region=='D')
  return(VetHisto_[3]->GetEntries());
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
} 

double ABCDMultiDimensional::GetEntries(int number)
{
 if(number==0)
  return(GetEntries('A'));
 if(number==1)
  return(GetEntries('B'));
 if(number==2)
  return(GetEntries('C'));
 if(number==3)
  return(GetEntries('D'));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
 
}  

/** return the poissonian error on the counts in each region */ 

double ABCDMultiDimensional::GetEntriesError(char region)
{
 if(region=='A')
  return(sqrt(VetHisto_[0]->GetEntries()));
 if(region=='B')
  return(sqrt(VetHisto_[1]->GetEntries()));
 if(region=='C')
  return(sqrt(VetHisto_[2]->GetEntries()));
 if(region=='D')
  return(sqrt(VetHisto_[3]->GetEntries()));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
}

double ABCDMultiDimensional::GetEntriesError(int number)
{
 if(number==0)
  return(sqrt(GetEntries('A')));
 if(number==1)
  return(sqrt(GetEntries('B')));
 if(number==2)
  return(sqrt(GetEntries('C')));
 if(number==3)
  return(sqrt(GetEntries('D')));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
 
}  

/** Effective entries and errors*/

double ABCDMultiDimensional::GetEffectiveEntries(char region)
{
 if(region=='A')
  return(VetHisto_[0]->GetEffectiveEntries());
 if(region=='B')
  return(VetHisto_[1]->GetEffectiveEntries());
 if(region=='C')
  return(VetHisto_[2]->GetEffectiveEntries());
 if(region=='D')
  return(VetHisto_[3]->GetEffectiveEntries());
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
 
}  

double ABCDMultiDimensional::GetEffectiveEntries(int number)
{
 if(number==0)
  return(GetEffectiveEntries('A'));
 if(number==1)
  return(GetEffectiveEntries('B'));
 if(number==2)
  return(GetEffectiveEntries('C'));
 if(number==3)
  return(GetEffectiveEntries('D'));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
 
}  

double ABCDMultiDimensional::GetEffectiveEntriesError(char region)
{
 if(region=='A')
  return(sqrt(VetHisto_[0]->GetEffectiveEntries()));
 if(region=='B')
  return(sqrt(VetHisto_[1]->GetEffectiveEntries()));
 if(region=='C')
  return(sqrt(VetHisto_[2]->GetEffectiveEntries()));
 if(region=='D')
  return(sqrt(VetHisto_[3]->GetEffectiveEntries()));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
}

double ABCDMultiDimensional::GetEffectiveEntriesError(int number)
{
 if(number==0)
  return(sqrt(GetEntries('A')));
 if(number==1)
  return(sqrt(GetEntries('B')));
 if(number==2)
  return(sqrt(GetEntries('C')));
 if(number==3)
  return(sqrt(GetEntries('D')));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
 
}  

/** Ratio and its error obtained through the linear propagation of errors on the poissonian hypothesis*/ 

double ABCDMultiDimensional::GetRatio(char region1,char region2)
{ if((region1!='A'&&region1!='B' &&region1!='C'&&region1!='D')||(region2!='A'&&region2!='B' &&region2!='C'&&region2!='D'))
{   std::cerr<<"non esiste l'istogramma"<<std::endl;
return(-1);}
else{     
 double Entries1=GetEntries(region1);
 double Entries2=GetEntries(region2);
 if(Entries2!=0)
  return(Entries1/Entries2);
 else
  return(-2);  
} 
}  

double ABCDMultiDimensional::GetRatioError(char region1, char region2)
{  if((region1!='A'&&region1!='B' &&region1!='C'&&region1!='D')||(region2!='A'&&region2!='B' &&region2!='C'&&region2!='D'))
{   std::cerr<<"non esiste l'istogramma"<<std::endl;
return(-1);}
else{ 
 double Error1=GetEntriesError(region1);
 double Error2=GetEntriesError(region2);
 double Entries1=GetEntries(region1);
 double Entries2=GetEntries(region2);
 if(Entries2!=0)
  return(sqrt(pow((1/Entries2),2)*pow(Error1,2)+pow(Entries1/(pow(Entries2,2)),2)*pow(Error2,2)));
 else
  return(-2);  
}

}

double ABCDMultiDimensional::GetEffectiveRatio(char region1,char region2)
{ 
 if((region1!='A'&&region1!='B' &&region1!='C'&&region1!='D')||(region2!='A'&&region2!='B' &&region2!='C'&&region2!='D'))
 { std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);}
 
 else{          
  double Entr1=GetEffectiveEntries(region1);
  double Entr2=GetEffectiveEntries(region2);
  if(Entr2!=0)
   return(Entr1/Entr2);
  else
   return(-2);  
 } 
}  


double ABCDMultiDimensional::GetEffectiveRatioError(char region1, char region2)
{  if((region1!='A'&&region1!='B' &&region1!='C'&&region1!='D')||(region2!='A'&&region2!='B' &&region2!='C'&&region2!='D'))
{   std::cerr<<"non esiste l'istogramma"<<std::endl;
return(-1);}
else{ 
 double Error1=GetEffectiveEntriesError(region1);
 double Error2=GetEffectiveEntriesError(region2);
 double Entries1=GetEffectiveEntries(region1);
 double Entries2=GetEffectiveEntries(region2);
 if(Entries2!=0)
  return(sqrt(pow((1/Entries2),2)*pow(Error1,2)+pow(Entries1/(pow(Entries2,2)),2)*pow(Error2,2)));
 else
  return(-2);  
 
}
} 

/** double Ratio value and error in the order given by the entries in the method*/

double ABCDMultiDimensional::GetDoubleRatio(char region1, char region2, char region3, char region4)
{
 if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
  return(-1);
 
 if(GetRatio(region1,region2)!=-2&& GetRatio(region3,region4)!=-2&&GetEntries(region3)!=0)
  return(GetRatio(region1,region2)/GetRatio(region3,region4));
 else
  return(-2);  
}  

double ABCDMultiDimensional::GetDoubleRatioError(char region1, char region2, char region3, char region4)
{   
 if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
  return(-1);
 if(GetRatio(region1,region2)!=-2&& GetRatio(region3,region4)!=-2&&GetEntries(region3)!=0)
  return(GetDoubleRatio(region1,region2,region3,region4)*((GetRatioError(region1,region2)/GetRatio(region1,region2))+(GetRatioError(region3,region4)/(GetRatio(region3,region4)))));
 else
  return(-2);
} 


double ABCDMultiDimensional::GetEffectiveDoubleRatio(char region1, char region2, char region3, char region4)
{    if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
return(-1);

if(GetRatio(region1,region2)!=-2&& GetRatio(region3,region4)!=-2&&GetEntries(region3)!=0)   
 return(GetEffectiveRatio(region1,region2)/GetEffectiveRatio(region3,region4));
else
 return(-2);
}

double ABCDMultiDimensional::GetEffectiveDoubleRatioError(char region1, char region2, char region3, char region4)
{
 if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
  return(-1);
 if(GetRatio(region1,region2)!=-2 && GetRatio(region3,region4)!=-2 && GetEntries(region3)!=0)
  return(GetEffectiveDoubleRatio(region1,region2,region3,region4) * ((GetEffectiveRatioError(region1,region2)/GetEffectiveRatio(region1,region2)) + (GetEffectiveRatioError(region3,region4)/(GetEffectiveRatio(region3,region4)))));
 else
  return(-2);
 
}  

/** return the one dimensional distribution in different regions */

TH1D* ABCDMultiDimensional::GetDistribution(const char& variable,const std::string& region){
 TString nameHisto = Form ("%c_%s",variable,region.c_str());
 TH1D* returnDistribution = NULL;
 
 for (int ic = 0; ic< region.size(); ic++){
  TH1D *hproj;
  TString nameH = Form ("_%s_myproj_%c_%c",(GetHisto(region.at(ic)))->GetName(),variable,region.at(ic));
  if (variable=='X' || variable=='x') hproj = (GetHisto(region.at(ic)))->ProjectionX(nameH, 0, -1 , "");
  else  hproj = (GetHisto(region.at(ic)))->ProjectionY(nameH, 0, -1 , "");
  if (ic == 0) {
   if (variable=='X' || variable=='x') returnDistribution = new TH1D (nameHisto,nameHisto,2,0,2);
   else returnDistribution = new TH1D (nameHisto,nameHisto,2,0,2);
  }
  returnDistribution->Add(hproj);
  delete hproj;
 }
 return returnDistribution;
}
/** information about the ABCDMultiDimensional subdivision */

void ABCDMultiDimensional::Print() 
{
 std::cout<<"#############################################"<<std::endl;
 std::cout<<"                  AREAS OF CUT               "<<std::endl;
 std::cout<<"#############################################"<<std::endl;
 std::cout<<""<<std::endl;
 std::cout<<"----------------------------------------------"<<std::endl;
 std::cout<<"          B           |           D           "<<std::endl;
 std::cout<<"----------------------------------------------"<<std::endl;
 std::cout<<"          A           |           C           "<<std::endl;
 std::cout<<"----------------------------------------------"<<std::endl;
}  

/** Cancel the previous histograms related to the ABCDMultiDimensional analysis*/

void ABCDMultiDimensional::ResetHisto() 
{ 
 for(int i=0;i<VetHisto_.size();i++)
 {
  if(VetHisto_[i]!=NULL)
  {
   delete VetHisto_[i];
   VetHisto_[i] = NULL;
//    delete VetHistoABCD_[i];
//    VetHistoABCD_[i] = NULL;
  }
 }
 if(Htot_!=NULL)
 {
  delete Htot_;
  Htot_ = NULL; 
 }
}

/** Save results of the ABCDMultiDimensional analysis in the TFILE passed as output in the constructor*/

void ABCDMultiDimensional::Save() 
{  
 if(outFile_ == NULL)
 { outFile_ = new TFile(output_.c_str(),"RECREATE");}
 
 outFile_->cd();
 
 for(int i=0;i<4;i++)
 {
  VetHisto_[i]->Write();
//   VetHistoABCD_[i]->Write();
 }
 
 if(Htot_!=NULL) {
  Htot_->Write();
 }
 
 outFile_->Write();
 outFile_->Print();
}

/** Save results in a chosen outfile*/

void ABCDMultiDimensional::Save(std::string& outFile)
{
 TFile* out= new TFile(outFile.c_str(),"RECREATE");
 out->cd();
 
 for(int i=0;i<4;i++) {
  VetHisto_[i]->Write();
//   VetHistoABCD_[i]->Write();
 }
 
 if(Htot_!=NULL) {
  Htot_->Write();
 }
 
 out->Write();
 out->Print();
}

///-----------------------------
void ABCDMultiDimensional::SetTree(TTree* tree_in)
{
 
 if(tree_in_ != NULL)
 {  tree_in_->Reset(); 
 }
 else{
  
  tree_in_ = tree_in->CloneTree(0);
 } 
 tree_in_->CopyEntries(tree_in);
}

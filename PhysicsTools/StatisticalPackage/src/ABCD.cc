# include "ABCD.h"
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


/** DEFAULT CONSTRUCTOR*/

ABCD::ABCD():
tree_in_p(NULL),
outFile_p(NULL),
Htot_p(NULL),
Kolmogorov_pA(NULL),
Kolmogorov_pB(NULL),
Kolmogorov_pC(NULL),
Kolmogorov_pD(NULL),
Kolmogorov_p1(NULL),
Kolmogorov_p2(NULL),
histoX_p(NULL),
histoY_p(NULL),
histoXCut_p(NULL),
histoYCut_p(NULL)
{
 InitializeHisto();
}

/** STANDARD AND COMPLETE CONSTRUCTOR*/

ABCD::ABCD(TTree* tree_in,std::string& output,std::string& varY,double cutY,std::string&  varX,double cutX,std::string&  gen_cut):
outFile_p(NULL),
output_p(output),
varY_p(varY),
cutY_p(cutY),
varX_p(varX),
cutX_p(cutX),
gen_cut_p(gen_cut),
Htot_p(NULL),
Kolmogorov_pA(NULL),
Kolmogorov_pB(NULL),
Kolmogorov_pC(NULL),
Kolmogorov_pD(NULL),
Kolmogorov_p1(NULL),
Kolmogorov_p2(NULL),
histoX_p(NULL),
histoY_p(NULL),
histoXCut_p(NULL),
histoYCut_p(NULL)
{    
 tree_in_p=NULL;
 SetTree(tree_in);
 InitializeHisto();
}

/** CONSTRUCTOR WITH ONLY THE INITIAL TREE*/

ABCD::ABCD(TTree*tree_in):  
outFile_p(NULL),
Htot_p(NULL),
Kolmogorov_pA(NULL),
Kolmogorov_pB(NULL),
Kolmogorov_pC(NULL),
Kolmogorov_pD(NULL),
Kolmogorov_p1(NULL),
Kolmogorov_p2(NULL),
histoX_p(NULL),
histoY_p(NULL),
histoXCut_p(NULL),
histoYCut_p(NULL)
{ 
 tree_in_p=NULL;
 SetTree(tree_in);
 InitializeHisto();
}  



/** DESTRUCTOR*/

ABCD::~ABCD()
{ 
 ResetHisto();
 
 if(tree_in_p!=NULL)
 {
  tree_in_p=NULL;
  delete tree_in_p;
  
 }
 
 if(outFile_p!=NULL)
 { 
  delete outFile_p;
  outFile_p=NULL;
 } 
 
}



/** Initialization of histos. Called by creator */
void ABCD::InitializeHisto()
{
 minEntriesPerBin_ = 10.;
 for(int i=0; i<4; i++) { 
  VetHisto_p.push_back(NULL);
 }  
}

/**CREATOR OF ABCD CONFIGURATION AFTER THE GENERAL CUT IMPOSED */

void ABCD::CreateHisto(const int& nbinX,const double& X_MIN,const double& X_MAX,const int & nbinY,const double& Y_MIN,const double& Y_MAX)
{   
 ResetHisto();
 X_MIN_ = X_MIN;
 X_MAX_ = X_MAX;
 Y_MIN_ = Y_MIN;
 Y_MAX_ = Y_MAX;
 nbinX_p = nbinX;
 nbinY_p = nbinY;
 
 BinCorrectionX(nbinX_p,X_MIN_,X_MAX_);
 BinCorrectionY(nbinY_p,Y_MIN_,Y_MAX_);
 
 TH2D* HA = new TH2D ("HA","HA",nbinX_p,X_MIN_,X_MAX_,nbinY_p,Y_MIN_,Y_MAX_);
 TH2D* HB = new TH2D ("HB","HB",nbinX_p,X_MIN_,X_MAX_,nbinY_p,Y_MIN_,Y_MAX_);
 TH2D* HC = new TH2D ("HC","HC",nbinX_p,X_MIN_,X_MAX_,nbinY_p,Y_MIN_,Y_MAX_);
 TH2D* HD = new TH2D ("HD","HD",nbinX_p,X_MIN_,X_MAX_,nbinY_p,Y_MIN_,Y_MAX_);
 Htot_p = new TH2D ("Htot_p","Htot_p",nbinX_p,X_MIN_,X_MAX_,nbinY_p,Y_MIN_,Y_MAX_);
 
 for(int it=0; it<4; it++)
 {  
  TString Variable;
  TString General_Cut;
  
  if(it==0)
  {
   std::string VarA = varY_p + ":" + varX_p + ">>HA";
   Variable=Form("%s",VarA.c_str());
   General_Cut = Form("%s<%f&&%s<%f&&%s",varY_p.c_str(),cutY_p,varX_p.c_str(),cutX_p,gen_cut_p.c_str());
  }
  if(it==1)
  { 
  std::string VarA = varY_p + ":" + varX_p + ">>HB";
  Variable=Form("%s",VarA.c_str());
  General_Cut = Form("%s>%f&&%s<%f&&%s",varY_p.c_str(),cutY_p,varX_p.c_str(),cutX_p,gen_cut_p.c_str());
  }
  if(it==2)
  {
   std::string VarA = varY_p + ":" + varX_p + ">>HC";
  Variable=Form("%s",VarA.c_str());
  General_Cut = Form("%s<%f&&%s>%f&&%s",varY_p.c_str(),cutY_p,varX_p.c_str(),cutX_p,gen_cut_p.c_str());
  }
  if(it==3)
  {
   std::string VarA = varY_p + ":" + varX_p + ">>HD";
  Variable=Form("%s",VarA.c_str());
  General_Cut = Form("%s>%f&&%s>%f&&%s",varY_p.c_str(),cutY_p,varX_p.c_str(),cutX_p,gen_cut_p.c_str());
  } 
  tree_in_p->Draw(Variable,General_Cut);
 }
 
 VetHisto_p.at(0) = HA;
 VetHisto_p.at(1) = HB;
 VetHisto_p.at(2) = HC;
 VetHisto_p.at(3) = HD;
 
 Htot_p->Add(HA);
 Htot_p->Add(HB);
 Htot_p->Add(HC);
 Htot_p->Add(HD); 
}

/** Create ABCD configuration with automatic sizing of X and Y axis*/   

void ABCD::CreateHisto(int& nbinX,int& nbinY)
{
 
 double MAX_varX = GetMaxVariable(varX_p);
 double MIN_varX = GetMinVariable(varX_p);
 
 double MAX_varY = GetMaxVariable(varY_p); 
 double MIN_varY = GetMinVariable(varY_p);
 if(MAX_varX==0&& MIN_varX==0)
  MAX_varX=1;
 
 if(MAX_varY==0&&MIN_varY==0)
  MAX_varY=1;
 CreateHisto(nbinX,MIN_varX,MAX_varX,nbinY,MIN_varY,MAX_varY);
}

/** create the ABCD configuration with a fixed number of bins chosen in function of the number of events*/

void ABCD:: CreateHisto()
{
 int nbinX = GetBinXY();
 int nbinY = GetBinXY();
 SetBinX(nbinX);
 SetBinY(nbinY);
 CreateHisto(nbinX_p,nbinY_p);
 
}  


/**METHOD FOR THE BIN CORRECTION in order have A CORRECT POSITION OF A CUT
it is automatically used for the construction of the ABCD analysis*/  

void ABCD::BinCorrectionX(int& nbin,double & MIN,double & MAX)
{ 
 double dX = (MAX-MIN) / nbin;
 
 int bX = (cutX_p-MIN) / dX;
 
 double DeltaX = (cutX_p-MIN) - bX*dX;
 
 MIN = MIN + (DeltaX-dX);
 MAX = MAX + DeltaX;
 nbin = nbin + 1;
}  

void ABCD::BinCorrectionY(int& nbin,double & MIN,double & MAX)
{ 
 double dY = (MAX-MIN) / nbin;
 
 int bY = (cutY_p-MIN) / dY;
 
 double DeltaY = (cutY_p-MIN) - bY*dY;
 
 MIN = MIN + DeltaY - dY;
 MAX = MAX + DeltaY;
 
 nbin = nbin + 1;
} 

/** RETURN THE NUMBER OF BINS FOR A CORRECT representation OF THE 2 DIM DISTRIBUTION, minEntriesPerBin_ ENTRIES FOR BIN IS THE DEFAULT STANDARD*/

int ABCD::GetBinXY()
{
 int Entries = tree_in_p->GetEntries(gen_cut_p.c_str());
 if (Entries==0) return(3);
 
 if (Entries >= minEntriesPerBin_) return(sqrt(Entries/minEntriesPerBin_));
 else{
  return(3);
 }
}

/** correlation coefficient for the pair chosen*/ 

double ABCD::GetCorrelation()
{ 
 if(Htot_p!=NULL)
  return(Htot_p->GetCorrelationFactor());
 else
  return(-100);  
}  

/** correlation probability P(|r|<|r_{0}|) on a t-student distribution*/

double ABCD::GetCorrelationProbability()
{ 
 if(Htot_p!=NULL)
 { 
  double r=fabs(Htot_p->GetCorrelationFactor());
  double N=Htot_p->GetEntries();
  double t=(r*sqrt(N-2))/(sqrt(1-pow(r,2)));
  return(2*(1-TMath::StudentI(t,N-2)));
 }
 else 
  return(-100);
}  

/** return a pointer to the total ABCD 2-D space*/

TH2D* ABCD::GetTotalHisto()
{
 if(Htot_p!=NULL)
  return(Htot_p);
}   

/** RETURN A POINTER to THE SINGLE ABCD REGION */

TH2D* ABCD::GetHisto(char letter)
{
 if(letter=='A')
  return(VetHisto_p[0]);
 
 if(letter=='B')
  return(VetHisto_p[1]);
 
 
 if(letter=='C')
  return(VetHisto_p[2]);
 
 if(letter=='D')
  return(VetHisto_p[3]);
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(NULL);
}  

TH2D* ABCD::GetHisto(int number)
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

double ABCD::GetEntries(char region)
{
 if(region=='A')
  return(VetHisto_p[0]->GetEntries());
 if(region=='B')
  return(VetHisto_p[1]->GetEntries());
 if(region=='C')
  return(VetHisto_p[2]->GetEntries());
 if(region=='D')
  return(VetHisto_p[3]->GetEntries());
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
} 

double ABCD::GetEntries(int number)
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

double ABCD::GetEntriesError(char region)
{
 if(region=='A')
  return(sqrt(VetHisto_p[0]->GetEntries()));
 if(region=='B')
  return(sqrt(VetHisto_p[1]->GetEntries()));
 if(region=='C')
  return(sqrt(VetHisto_p[2]->GetEntries()));
 if(region=='D')
  return(sqrt(VetHisto_p[3]->GetEntries()));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
}

double ABCD::GetEntriesError(int number)
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

double ABCD::GetEffectiveEntries(char region)
{
 if(region=='A')
  return(VetHisto_p[0]->GetEffectiveEntries());
 if(region=='B')
  return(VetHisto_p[1]->GetEffectiveEntries());
 if(region=='C')
  return(VetHisto_p[2]->GetEffectiveEntries());
 if(region=='D')
  return(VetHisto_p[3]->GetEffectiveEntries());
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
 
}  

double ABCD::GetEffectiveEntries(int number)
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

double ABCD::GetEffectiveEntriesError(char region)
{
 if(region=='A')
  return(sqrt(VetHisto_p[0]->GetEffectiveEntries()));
 if(region=='B')
  return(sqrt(VetHisto_p[1]->GetEffectiveEntries()));
 if(region=='C')
  return(sqrt(VetHisto_p[2]->GetEffectiveEntries()));
 if(region=='D')
  return(sqrt(VetHisto_p[3]->GetEffectiveEntries()));
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(-1);
}

double ABCD::GetEffectiveEntriesError(int number)
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

double ABCD::GetRatio(char region1,char region2)
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

double ABCD::GetRatioError(char region1, char region2)
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

double ABCD::GetEffectiveRatio(char region1,char region2)
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


double ABCD::GetEffectiveRatioError(char region1, char region2)
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

double ABCD::GetDoubleRatio(char region1, char region2, char region3, char region4)
{
 if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
  return(-1);
 
 if(GetRatio(region1,region2)!=-2&& GetRatio(region3,region4)!=-2&&GetEntries(region3)!=0)
  return(GetRatio(region1,region2)/GetRatio(region3,region4));
 else
  return(-2);  
}  

double ABCD::GetDoubleRatioError(char region1, char region2, char region3, char region4)
{   
 if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
  return(-1);
 if(GetRatio(region1,region2)!=-2&& GetRatio(region3,region4)!=-2&&GetEntries(region3)!=0)
  return(GetDoubleRatio(region1,region2,region3,region4)*((GetRatioError(region1,region2)/GetRatio(region1,region2))+(GetRatioError(region3,region4)/(GetRatio(region3,region4)))));
 else
  return(-2);
} 


double ABCD::GetEffectiveDoubleRatio(char region1, char region2, char region3, char region4)
{    if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
return(-1);

if(GetRatio(region1,region2)!=-2&& GetRatio(region3,region4)!=-2&&GetEntries(region3)!=0)   
 return(GetEffectiveRatio(region1,region2)/GetEffectiveRatio(region3,region4));
else
 return(-2);
}

double ABCD::GetEffectiveDoubleRatioError(char region1, char region2, char region3, char region4)
{
 if(GetRatio(region1,region2)==-1|| GetRatio(region3,region4)==-1)
  return(-1);
 if(GetRatio(region1,region2)!=-2 && GetRatio(region3,region4)!=-2 && GetEntries(region3)!=0)
  return(GetEffectiveDoubleRatio(region1,region2,region3,region4) * ((GetEffectiveRatioError(region1,region2)/GetEffectiveRatio(region1,region2)) + (GetEffectiveRatioError(region3,region4)/(GetEffectiveRatio(region3,region4)))));
 else
  return(-2);
 
}  

/** Function which return the maximum value of a fixed variable*/

double ABCD::GetMaxVariable(std::string& Variable)
{  
 tree_in_p->Draw(Variable.c_str(),gen_cut_p.c_str());
 TH1F*htemp=(TH1F*)gPad->GetPrimitive("htemp");
 if(htemp!=0)
 {  
  double MAX_Var = htemp->GetXaxis()->GetXmax();
  return (MAX_Var);
 }
 else return(0);
}  
/** Function which return the minimum value of a fixed variable*/

double ABCD::GetMinVariable(std::string& Variable)
{
 tree_in_p->Draw(Variable.c_str(),gen_cut_p.c_str());
 TH1F* htemp = (TH1F*)gPad->GetPrimitive("htemp");
 if(htemp!=0)
 {  
  double MIN_Var = htemp->GetXaxis()->GetXmin();
  
  return(MIN_Var);
 }
 else return(0);
}  

/** Save the X distribution without any cuts applied*/

void ABCD::GetDistributionX()
{
 tree_in_p->Draw(varX_p.c_str());
 TH1D*htemp = (TH1D*) gPad->GetPrimitive("htemp");
 if(htemp!=0)
 {
  histoX_p =(TH1D*) htemp->Clone("HistoX_p");
 }
}

/** Save the X distribution with general cut applied*/

void ABCD::GetDistributionXCut()
{
 tree_in_p->Draw(varX_p.c_str(),gen_cut_p.c_str());
 TH1D*htemp = (TH1D*) gPad->GetPrimitive("htemp");
 if(htemp!=0)
 {
  
  histoXCut_p =(TH1D*) htemp->Clone("HistoXCut_p");
  
 }
 
}

/** Save the Y distribution without any cuts applied*/

void ABCD::GetDistributionY()
{
 tree_in_p->Draw(varY_p.c_str());
 TH1D*htemp = (TH1D*) gPad->GetPrimitive("htemp");
 if(htemp!=0)
 {
  histoY_p = (TH1D*) htemp->Clone("HistoY_p");
 }
}

/** Save the Y distribution with general cut applied*/

void ABCD::GetDistributionYCut()
{
 tree_in_p->Draw(varY_p.c_str(),gen_cut_p.c_str());
 TH1D*htemp = (TH1D*) gPad->GetPrimitive("htemp");
 if(htemp!=0)
 {
  histoYCut_p = (TH1D*) htemp->Clone("HistoYCut_p");
 }
}


/** return the one dimensional distribution in different regions */

TH1D* ABCD::GetDistribution(const char& variable,const std::string& region){
 TString nameHisto = Form ("%c_%s",variable,region.c_str());
 TH1D* returnDistribution = NULL;
 
 for (int ic = 0; ic< region.size(); ic++){
  //   std::cerr << " region[" << ic << "] = " << region.at(ic) << std::endl;
  //   TH2D* tempH2 = GetHisto(region.at(ic));
  //   TH1D *hproj = tempH2->ProjectionX ("_temp",bin,bin+ngroup-1,"e") ;
  
  TH1D *hproj;
  TString nameH = Form ("_%s_myproj_%c_%c",(GetHisto(region.at(ic)))->GetName(),variable,region.at(ic));
  if (variable=='X' || variable=='x') hproj = (GetHisto(region.at(ic)))->ProjectionX(nameH, 0, -1 , "");
  else  hproj = (GetHisto(region.at(ic)))->ProjectionY(nameH, 0, -1 , "");
  //   hproj = (GetHisto(region.at(ic)))->DoProjection(variable=='X' || variable=='x', "_proj", 0, -1 ,
  if (ic == 0) {
   if (variable=='X' || variable=='x') returnDistribution = new TH1D (nameHisto,nameHisto,nbinX_p,X_MIN_,X_MAX_);
   else returnDistribution = new TH1D (nameHisto,nameHisto,nbinY_p,Y_MIN_,Y_MAX_);
  }
  returnDistribution->Add(hproj);
  delete hproj;
 }
 
 return returnDistribution;
}









/** Method for Kolmogorov-Smirnov test**/

double ABCD::KolmogorovTest(std::string& Var)
{
 TString Variable;
 TString Cut;
 
 double MAX_varX = Htot_p->GetXaxis()->GetXmax(); 
 double MIN_varX = Htot_p->GetXaxis()->GetXmin();
 
 double MAX_varY = Htot_p->GetYaxis()->GetXmax(); 
 double MIN_varY = Htot_p->GetYaxis()->GetXmin();
 
 if(Kolmogorov_p1!=NULL)
  delete Kolmogorov_p1;
 
 if(Kolmogorov_p2!=NULL)
  delete Kolmogorov_p2;
 
 if(Kolmogorov_pA!=NULL)
  delete Kolmogorov_pA;
 
 if(Kolmogorov_pB!=NULL)
  delete Kolmogorov_pB;
 
 if(Kolmogorov_pC!=NULL)
  delete Kolmogorov_pC;
 
 if(Kolmogorov_pD!=NULL)
  delete Kolmogorov_pD;
 
 if( Var==varX_p )
 { 
  Kolmogorov_pA = new TH1D("Kolmogorov_pA","Test Kolgomorov_A",nbinX_p,MIN_varX,MAX_varX);
  Kolmogorov_pB = new TH1D("Kolmogorov_pB","Test Kolgomorov_B",nbinX_p,MIN_varX,MAX_varX);
  Kolmogorov_pC = new TH1D("Kolmogorov_pC","Test Kolgomorov_C",nbinX_p,MIN_varX,MAX_varX);
  Kolmogorov_pD = new TH1D("Kolmogorov_pD","Test Kolgomorov_D",nbinX_p,MIN_varX,MAX_varX);
  
  double AreaA = 0;
  double AreaB = 0;
  double AreaC = 0;
  double AreaD = 0;
  
  
  for(int i=0; i<nbinX_p; i++)
  {
   for(int j=0; j<nbinY_p; j++)
   {  
    AreaA = AreaA + VetHisto_p.at(0)->GetBinContent(i+1,j+1);
    AreaB = AreaB + VetHisto_p.at(1)->GetBinContent(i+1,j+1);
    AreaC = AreaC + VetHisto_p.at(2)->GetBinContent(i+1,j+1);
    AreaD = AreaD + VetHisto_p.at(3)->GetBinContent(i+1,j+1);
   }
   
   Kolmogorov_pA->SetBinContent(i,AreaA);
   Kolmogorov_pB->SetBinContent(i,AreaB);
   Kolmogorov_pC->SetBinContent(i,AreaC);
   Kolmogorov_pD->SetBinContent(i,AreaD);
   AreaA = 0;
   AreaB = 0;
   AreaC = 0;
   AreaD = 0;
  }  
  
  double A=Kolmogorov_pA->GetEffectiveEntries();
  double B=Kolmogorov_pB->GetEffectiveEntries();
  double C=Kolmogorov_pC->GetEffectiveEntries();
  double D=Kolmogorov_pD->GetEffectiveEntries();
  
  //Kolmogorov_pA->Scale(1/(Kolmogorov_pA->GetEffectiveEntries()+Kolmogorov_pC->GetEffectiveEntries()));
  //Kolmogorov_pB->Scale(1/(Kolmogorov_pB->GetEffectiveEntries()+Kolmogorov_pD->GetEffectiveEntries()));
  //Kolmogorov_pC->Scale(1/(Kolmogorov_pC->GetEffectiveEntries()+Kolmogorov_pA->GetEffectiveEntries()));
  //Kolmogorov_pD->Scale(1/(Kolmogorov_pD->GetEffectiveEntries()+Kolmogorov_pB->GetEffectiveEntries()));
  
  Kolmogorov_p1 = new TH1D("Kolmogorov_p1","Test Kolgomorov_1",nbinX_p,MIN_varX,MAX_varX);
  Kolmogorov_p2 = new TH1D("Kolmogorov_p2","Test Kolgomorov_2",nbinX_p,MIN_varX,MAX_varX);
  Kolmogorov_p1->Add(Kolmogorov_pA);
  Kolmogorov_p1->Add(Kolmogorov_pC);
  Kolmogorov_p2->Add(Kolmogorov_pB);
  Kolmogorov_p2->Add(Kolmogorov_pD);
  Kolmogorov_p1->Sumw2();
  Kolmogorov_p2->Sumw2();
  Kolmogorov_p1->Scale(1/(A+C));
  Kolmogorov_p2->Scale(1/(B+D));
  
  Kolmogorov_pA->Sumw2();
  Kolmogorov_pB->Sumw2();
  Kolmogorov_pC->Sumw2();
  Kolmogorov_pD->Sumw2();
  
  Kolmogorov_pA->Scale(1/(A+C));
  Kolmogorov_pB->Scale(1/(B+D));
  Kolmogorov_pC->Scale(1/(A+C));
  Kolmogorov_pD->Scale(1/(B+D));
  
  
  return(Kolmogorov_p1->KolmogorovTest(Kolmogorov_p2));
  
 }
 
 if(Var==varY_p)
 {
  
  Kolmogorov_pA=new TH1D("Kolmogorov_pA","Test Kolgomorov_A",nbinY_p,MIN_varY,MAX_varY);
  Kolmogorov_pB=new TH1D("Kolmogorov_pB","Test Kolgomorov_B",nbinY_p,MIN_varY,MAX_varY);
  Kolmogorov_pC=new TH1D("Kolmogorov_pC","Test Kolgomorov_C",nbinY_p,MIN_varY,MAX_varY);
  Kolmogorov_pD=new TH1D("Kolmogorov_pD","Test Kolgomorov_D",nbinY_p,MIN_varY,MAX_varY);
  
  double AreaA=0;
  double AreaB=0;
  double AreaC=0;
  double AreaD=0;
  
  for(int i=0; i<nbinY_p;i++)
  {
   for(int j=0;j<nbinX_p;j++)
   { 
    AreaA = AreaA + VetHisto_p.at(0)->GetBinContent(j+1,i+1);
    AreaB = AreaB + VetHisto_p.at(1)->GetBinContent(j+1,i+1);
    AreaC = AreaC + VetHisto_p.at(2)->GetBinContent(j+1,i+1);
    AreaD = AreaD + VetHisto_p.at(3)->GetBinContent(j+1,i+1);
   }
   Kolmogorov_pA->SetBinContent(i,AreaA);
   Kolmogorov_pB->SetBinContent(i,AreaB);
   Kolmogorov_pC->SetBinContent(i,AreaC);
   Kolmogorov_pD->SetBinContent(i,AreaD);
   
   AreaA=0;
   AreaB=0;
   AreaC=0;
   AreaD=0;
  }  
  
  double A=Kolmogorov_pA->GetEffectiveEntries();
  double B=Kolmogorov_pB->GetEffectiveEntries();
  double C=Kolmogorov_pC->GetEffectiveEntries();
  double D=Kolmogorov_pD->GetEffectiveEntries();
  
  // Kolmogorov_pA->Scale(1/(Kolmogorov_pA->GetEffectiveEntries()+Kolmogorov_pB->GetEffectiveEntries()));
  // Kolmogorov_pB->Scale(1/(Kolmogorov_pB->GetEffectiveEntries()+Kolmogorov_pA->GetEffectiveEntries()));
  // Kolmogorov_pC->Scale(1/(Kolmogorov_pC->GetEffectiveEntries()+Kolmogorov_pD->GetEffectiveEntries()));
  // Kolmogorov_pD->Scale(1/(Kolmogorov_pD->GetEffectiveEntries()+Kolmogorov_pC->GetEffectiveEntries()));
  
  
  Kolmogorov_p1 = new TH1D("Kolmogorov_p1","Test Kolgomorov_1",nbinY_p,MIN_varY,MAX_varY);
  Kolmogorov_p2 = new TH1D("Kolmogorov_p2","Test Kolgomorov_2",nbinY_p,MIN_varY,MAX_varY);
  Kolmogorov_p1->Add(Kolmogorov_pA);
  Kolmogorov_p1->Add(Kolmogorov_pB);
  Kolmogorov_p2->Add(Kolmogorov_pC);
  Kolmogorov_p2->Add(Kolmogorov_pD);
  Kolmogorov_p1->Sumw2();
  Kolmogorov_p2->Sumw2();
  Kolmogorov_p1->Scale(1/(A+B));
  Kolmogorov_p2->Scale(1/(C+D));
  
  Kolmogorov_pA->Sumw2();
  Kolmogorov_pB->Sumw2();
  Kolmogorov_pC->Sumw2();
  Kolmogorov_pD->Sumw2();
  
  Kolmogorov_pA->Scale(1/(A+B));
  Kolmogorov_pB->Scale(1/(C+D));
  Kolmogorov_pC->Scale(1/(A+B));
  Kolmogorov_pD->Scale(1/(C+D));
  
  return(Kolmogorov_p1->KolmogorovTest(Kolmogorov_p2));
  
  
  return(0);
  
 } 
 
 std::cerr<<"non esiste l'istogramma"<<std::endl;
 return(0);
 
}

/** information about the ABCD subdivision */

void ABCD::Print() 
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

/** Cancel the previous histograms related to the ABCD analysis*/

void ABCD::ResetHisto() 
{ 
 for(int i=0;i<VetHisto_p.size();i++)
 {
  if(VetHisto_p[i]!=NULL)
  {
   delete VetHisto_p[i];
   VetHisto_p[i]!=NULL;
  }
 }
 if(Htot_p!=NULL)
 {
  delete Htot_p;
  Htot_p=NULL; 
 }
 if(Kolmogorov_p1!=NULL)
 {delete Kolmogorov_p1;
 Kolmogorov_p1=NULL;}
 if(Kolmogorov_p2!=NULL)
 {delete Kolmogorov_p2;
 Kolmogorov_p2=NULL;}
 if(Kolmogorov_pA!=NULL)
 {delete Kolmogorov_pA;
 Kolmogorov_pA=NULL;}
 if(Kolmogorov_pB!=NULL)
 {delete Kolmogorov_pB;
 Kolmogorov_pB=NULL;}
 if(Kolmogorov_pC!=NULL)
 {delete Kolmogorov_pC;
 Kolmogorov_pC=NULL;}
 if(Kolmogorov_pD!=NULL)
 {delete Kolmogorov_pD;
 Kolmogorov_pD=NULL;} 
 if(histoX_p!=NULL)
 {delete histoX_p;
 histoX_p=NULL;}
 if(histoY_p!=NULL)
 {delete histoY_p ;
 histoY_p=NULL;} 
 if(histoXCut_p!=NULL)
 {delete histoXCut_p;
 histoXCut_p=NULL;} 
 if(histoYCut_p!=NULL)
 {delete histoYCut_p ;
 histoYCut_p=NULL;} 
 
 
 
}

/** Save results of the ABCD analysis in the TFILE passed as output in the constructor*/

void ABCD::Save() 
{  
 if(outFile_p==NULL)
 { outFile_p= new TFile(output_p.c_str(),"RECREATE");}
 
 outFile_p->cd();
 
 for(int i=0;i<4;i++)
 {
  VetHisto_p[i]->Write();
 }
 
 if(Htot_p!=NULL) {
  Htot_p->Write();
 }
 
 if(Kolmogorov_pA!=NULL) {  
  Kolmogorov_pA->Write();
 }
 
 if(Kolmogorov_pB!=NULL) {
  Kolmogorov_pB->Write();
 }
 
 if(Kolmogorov_pC!=NULL){ 
  Kolmogorov_pC->Write();
 }
 
 if(Kolmogorov_pD!=NULL){
  Kolmogorov_pD->Write();
 } 
 
 outFile_p->Write();
 outFile_p->Print();
 
}

/** Save results in a chosen outfile*/

void ABCD::Save(std::string& outFile)
{
 TFile* out= new TFile(outFile.c_str(),"RECREATE");
 out->cd();
 
 for(int i=0;i<4;i++) {
  VetHisto_p[i]->Write();
 }
 
 if(Htot_p!=NULL)
  Htot_p->Write();
 if(Kolmogorov_pA!=NULL)
  Kolmogorov_pA->Write();
 if(Kolmogorov_pB!=NULL)
  Kolmogorov_pB->Write();
 if(Kolmogorov_pC!=NULL)
  Kolmogorov_pC->Write();
 if(Kolmogorov_pD!=NULL)
  Kolmogorov_pD->Write();
 
 out->Write();
 out->Print();
}

/** this method save only the distribution on X and Y variable before and after the application of the general cut*/

void ABCD::SaveDistribution(std::string outFile)
{
 TFile* out= new TFile(outFile.c_str(),"RECREATE");
 out->cd();
 
 if(histoX_p!=NULL)
  histoX_p->Write();
 if(histoY_p!=NULL)
  histoY_p->Write();
 if(histoXCut_p!=NULL)
  histoXCut_p->Write();
 if(histoYCut_p!=NULL)
  histoYCut_p->Write();
 
 out->Write();
 out->Print();
 
}




/** List of Set method*/

void ABCD::SetVarX(std::string &VarX)
{
 varX_p=VarX;
}

///-----------------------------
void ABCD::SetVarX(TString VarX)
{
 varX_p=VarX;
}

///-----------------------------
void ABCD::SetVarY(std::string &VarY)
{
 varY_p=VarY;
}  

///-----------------------------
void ABCD::SetVarY(TString VarY)
{
 varY_p=VarY;
}  

///-----------------------------
void ABCD::SetGeneralCut(std::string & GenCut)
{
 gen_cut_p=GenCut;
}

///-----------------------------
void ABCD::SetGeneralCut(TString GenCut)
{
 
 gen_cut_p=GenCut;
 
}

///-----------------------------
void ABCD::SetCutX(double CutX)
{
 cutX_p=CutX;
}

///-----------------------------
void ABCD::SetCutY(double CutY)
{
 cutY_p=CutY;
}  

///-----------------------------
void ABCD::SetTree(TTree* tree_in)
{
 
 if(tree_in_p!=NULL)
 {  tree_in_p->Reset(); 
 }
 else{
  
  tree_in_p=tree_in->CloneTree(0);
 } 
 tree_in_p->CopyEntries(tree_in);
}

///-----------------------------
void ABCD::SetBinX(int nbinX)
{
 nbinX_p = nbinX;
}

///-----------------------------
void ABCD::SetBinY(int nbinY)
{
 nbinY_p = nbinY;
}

///-----------------------------
void ABCD::SetMinEntriesPerBin(int minEntriesPerBin){
 minEntriesPerBin_ = minEntriesPerBin;
}






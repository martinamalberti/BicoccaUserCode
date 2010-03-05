//To train with the selected method type for eg.
//root -l  MVA_Training.cxx\(\"RuleFit\"\)

///By Andrea Massironi

#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

Bool_t ReadDataFromAsciiIFormat = kFALSE;


void MVA_Training( TString myMethodList = "" ) {

 TMVA::Tools::Instance();
 
 std::map<std::string,int> Use;
 
 Use["Cuts"]            = 0;//1
 Use["CutsD"]           = 0;//1
 Use["CutsPCA"]         = 0;//1
 Use["CutsGA"]          = 0;//1
 Use["CutsSA"]          = 0;//1
 // ---
 Use["Likelihood"]      = 1;
 Use["LikelihoodD"]     = 1; // the "D" extension indicates decorrelated input variables (see option strings)
 Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
 Use["LikelihoodKDE"]   = 1;
 Use["LikelihoodMIX"]   = 1;
 // ---
 Use["PDERS"]           = 0;//1
 Use["PDERSD"]          = 0;//1
 Use["PDERSPCA"]        = 0;//1
 Use["PDERSkNN"]        = 0;//1 // depreciated until further notice
 Use["PDEFoam"]         = 0;//1
 // --
 Use["KNN"]             = 1;
 // ---
 Use["HMatrix"]         = 1;
 Use["Fisher"]          = 1;
 Use["FisherG"]         = 1;
 Use["BoostedFisher"]   = 1;
 Use["LD"]              = 1;
 // ---
 Use["FDA_GA"]          = 1;
 Use["FDA_SA"]          = 1;
 Use["FDA_MC"]          = 1;
 Use["FDA_MT"]          = 1;
 Use["FDA_GAMT"]        = 1;
 Use["FDA_MCMT"]        = 1;
 // ---
 Use["MLP"]             = 1; // this is the recommended ANN
 Use["MLPBFGS"]         = 1; // recommended ANN with optional training method
 Use["CFMlpANN"]        = 1; // *** missing
 Use["TMlpANN"]         = 1; 
 // ---
 Use["SVM"]             = 1;
 // ---
 Use["BDT"]             = 1;
 Use["BDTD"]            = 0;
 Use["BDTG"]            = 1;
 Use["BDTB"]            = 0;
 // ---
 Use["RuleFit"]         = 1;
 // ---
 Use["Plugin"]          = 0;
 // ---------------------------------------------------------------
 
 
 if (myMethodList != "") {
  for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
  
  std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
  for (UInt_t i=0; i<mlist.size(); i++) {
   std::string regMethod(mlist[i]);
   
   if (Use.find(regMethod) == Use.end()) {
    std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
    std::cout << std::endl;
    return;
   }
   Use[regMethod] = 1;
  }
 }
 

 
 std::cout << "Start TMVAnalysis" << std::endl << "=================" << std::endl << std::endl;
 
 //Input files
 TString fname_sig = "../ReducedTrees/Upsilon1StoEE_TMVAtree.root";
 TString fname_bkg = "../ReducedTrees/ppEleX_TMVAtree.root";
 
 // Create a new root output file.-> useful when using GUI to inspect the TMVA results
 TString outfileName( "../TMVA/TMVA_trained.root" );
 TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
 
 //==== Create the factory ====
 TMVA::Factory *factory = new TMVA::Factory( "TMVAnalysis", outputFile, Form("!V:!Silent:%sColor", gROOT->IsBatch()?"!":"") );
 
 
 //==== Add the input variable names to be used to train the MVAs to the factory ====
 float m_QQ_DeltaR;
 float m_QQ_probChi2;
 float m_QQ_STip;
 float m_QQ_SLip;
 float m_QQ_S3Dip;

 float m_electronHpT_tkIso;
 float m_electronHpT_emIso;
 float m_electronHpT_hadIso;
 float m_electronHpT_IdBDT;
  float m_electronHpT_EoP;
  
  float m_electronHpT_HoE;
  float m_electronHpT_dEtaSc;
  float m_electronHpT_dPhiSc;
  float m_electronHpT_SigiEtaiEtaSc;
  float m_electronHpT_Et;
  float m_electronHpT_pAtVtx;
  float m_electronHpT_ptAtVtx;

float m_electronLpT_tkIso;
float m_electronLpT_emIso;
float m_electronLpT_hadIso;
float m_electronLpT_IdBDT;
  float m_electronLpT_EoP;
  
    float m_electronLpT_HoE;
  float m_electronLpT_dEtaSc;
  float m_electronLpT_dPhiSc;
  float m_electronLpT_SigiEtaiEtaSc;
  float m_electronLpT_Et;
  float m_electronLpT_pAtVtx;
  float m_electronLpT_ptAtVtx;


 
 factory->AddVariable( "m_QQ_DeltaR" , 'F');
 factory->AddVariable( "m_QQ_probChi2" , 'F');
 factory->AddVariable( "m_QQ_STip" , 'F');
 factory->AddVariable( "m_QQ_SLip" , 'F');
 factory->AddVariable( "m_QQ_S3Dip" , 'F');

 factory->AddVariable( "m_electronHpT_tkIso" , 'F');
 factory->AddVariable( "m_electronHpT_emIso" , 'F');
 factory->AddVariable( "m_electronHpT_hadIso" , 'F');
 factory->AddVariable( "m_electronHpT_IdBDT" , 'F');
 factory->AddVariable( "m_electronHpT_EoP" , 'F');

 factory->AddVariable( "m_electronHpT_HoE" , 'F');
 factory->AddVariable( "m_electronHpT_dEtaSc" , 'F');
 factory->AddVariable( "m_electronHpT_dPhiSc" , 'F');
 factory->AddVariable( "m_electronHpT_SigiEtaiEtaSc" , 'F');
 factory->AddVariable( "m_electronHpT_Et" , 'F');
 factory->AddVariable( "m_electronHpT_pAtVtx" , 'F');
 factory->AddVariable( "m_electronHpT_ptAtVtx" , 'F');

 factory->AddVariable( "m_electronLpT_tkIso" , 'F');
 factory->AddVariable( "m_electronLpT_emIso" , 'F');
 factory->AddVariable( "m_electronLpT_hadIso" , 'F');
 factory->AddVariable( "m_electronLpT_IdBDT" , 'F');
 factory->AddVariable( "m_electronLpT_EoP" , 'F');
 
 factory->AddVariable( "m_electronLpT_HoE" , 'F');
 factory->AddVariable( "m_electronLpT_dEtaSc" , 'F');
 factory->AddVariable( "m_electronLpT_dPhiSc" , 'F');
 factory->AddVariable( "m_electronLpT_SigiEtaiEtaSc" , 'F');
 factory->AddVariable( "m_electronLpT_Et" , 'F');
 factory->AddVariable( "m_electronLpT_pAtVtx" , 'F');
 factory->AddVariable( "m_electronLpT_ptAtVtx" , 'F');

 //==== Define the input samples ====
 
 TFile *input_sig(0);
 std::cout << "--- TMVAnalysis    : Accessing Signal " << fname_sig << std::endl;
 input_sig = TFile::Open( fname_sig );
 TTree *signal;
 signal  = (TTree*) input_sig-> Get("TMVA_tree");

 TFile *input_bkg(0);
 std::cout << "--- TMVAnalysis    : Accessing Bkg " << fname_bkg << std::endl;
 input_bkg = TFile::Open( fname_bkg );
 TTree *background;
 background  = (TTree*) input_bkg-> Get("TMVA_tree");
 
 Double_t signalWeight = 1;
 Double_t backgroundWeight = 1;
 
 factory->AddSignalTree    ( signal,     signalWeight         );
 factory->AddBackgroundTree( background, backgroundWeight     );
 
//  TString mycuts_s = Form("Match4 == 4");
//  TString mycuts_b = Form("Match4 != 4");
//  
//  TCut mycuts = mycuts_s;
//  TCut mycutb = mycuts_b;
 
 // tell the factory to use all remaining events in the trees after training for testing:
//  factory->PrepareTrainingAndTestTree( mycuts, mycutb,"SplitMode=Random:NormMode=NumEvents:!V");//:nTest_Background=1000:nTest_Signal=1000" );
 factory->PrepareTrainingAndTestTree( "", "","SplitMode=Random:NormMode=NumEvents:!V");//:nTest_Background=1000:nTest_Signal=1000" );
 
 
   //==== Book the MVA methods ====

   // Cut optimisation
   if (Use["Cuts"])
      factory->BookMethod( TMVA::Types::kCuts, "Cuts", 
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

   if (Use["CutsD"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsD", 
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

   if (Use["CutsPCA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsPCA", 
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

   if (Use["CutsGA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
                           "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );
   
   if (Use["CutsSA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
                           "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
   
   // Likelihood
   if (Use["Likelihood"])
      factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", 
                           "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" ); 

   // test the decorrelated likelihood
   if (Use["LikelihoodD"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD", 
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" ); 

   if (Use["LikelihoodPCA"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA", 
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 
 
   // test the new kernel density estimator
   if (Use["LikelihoodKDE"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE", 
                           "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 

   // test the mixed splines and kernel density estimator (depending on which variable)
   if (Use["LikelihoodMIX"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX", 
                           "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 

   // test the multi-dimensional probability density estimator
   // here are the options strings for the MinMax and RMS methods, respectively:
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );   
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );   
   if (Use["PDERS"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERS", 
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSkNN"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSkNN", 
                           "!H:!V:VolumeRangeMode=kNN:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSD"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSD", 
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

   if (Use["PDERSPCA"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA", 
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
   if (Use["PDEFoam"])
      factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", 
                           "H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0333:nActiveCells=500:nSampl=2000:nBin=5:CutNmin=T:Nmin=100:Kernel=None:Compress=T" );

   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod( TMVA::Types::kKNN, "KNN", 
                           "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );
   // H-Matrix (chi2-squared) method
   if (Use["HMatrix"])
      factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V" ); 

   // Fisher discriminant   
   if (Use["Fisher"])
      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=60:NsmoothMVAPdf=10" );

   // Fisher with Gauss-transformed input variables
   if (Use["FisherG"])
      factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
   if (Use["BoostedFisher"])
      factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2");

   // Linear discriminant (same as Fisher)
   if (Use["LD"])
      factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None" );

	// Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );
   
   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );

   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_SA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   if (Use["FDA_MT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   if (Use["FDA_MCMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
   if (Use["MLP"])
      factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );

   if (Use["MLPBFGS"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS" );


   // CF(Clermont-Ferrand)ANN
   if (Use["CFMlpANN"])
      factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  
  
   // Tmlp(Root)ANN
   if (Use["TMlpANN"])
      factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...
  
   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
   
   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTG", 
                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDT", 
			   "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
   
   if (Use["BDTB"]) // Bagging
      factory->BookMethod( TMVA::Types::kBDT, "BDTB", 
                           "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTD", 
                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );
   
   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

   // --------------------------------------------------------------------------------------------------
   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

  
   //==== Train MVAs using the set of training events ====
   factory->TrainAllMethods();

   //==== Evaluate all MVAs using the set of test events ====
   factory->TestAllMethods();

   //==== Evaluate and compare performance of all configured MVAs ====
   factory->EvaluateAllMethods();    

   // --------------------------------------------------------------
   
   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAnalysis is done!" << std::endl;      

   delete factory;

   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVAGui( outfileName );
}


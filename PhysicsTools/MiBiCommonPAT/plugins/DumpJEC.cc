#include "PhysicsTools/MiBiCommonPAT/plugins/DumpJEC.h"



///--------------
///---- ctor ----

DumpJEC::DumpJEC(const edm::ParameterSet& iConfig)
{
  etaMin_   = iConfig.getUntrackedParameter<double>("etaMin",   -5.);
  etaMax_   = iConfig.getUntrackedParameter<double>("etaMax",   +5.);
  nBinsEta_ = iConfig.getUntrackedParameter<int>   ("nBinsEta",100);
  
  ptMin_   = iConfig.getUntrackedParameter<double>("ptMin",    0.);
  ptMax_   = iConfig.getUntrackedParameter<double>("ptMax",  100.);
  nBinsPt_ = iConfig.getUntrackedParameter<int>   ("nBinsPt",100);
  
  outFileName_ = iConfig.getUntrackedParameter<std::string>("outFileName","dumpJEC.txt");
}



///--------------
///---- dtor ----

DumpJEC::~DumpJEC()
{
}



///-------------------------------------------
/// ---- method called to for each event  ----

void DumpJEC::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // define outfile
  std::ofstream outFile(outFileName_.c_str(),std::ios::out);
  
  
  // define the JEC dumper
  edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  iSetup.get<JetCorrectionsRecord>().get("AK5PF",JetCorParColl); 
  JetCorrectorParameters const& JetCorPar = (*JetCorParColl)["Uncertainty"];
  JetCorrectionUncertainty* jecUnc = new JetCorrectionUncertainty(JetCorPar);
  
  
  // write JEC uncertainties in the file
  double etaWidth = (etaMax_ - etaMin_) / nBinsEta_;
  double ptWidth = (ptMax_ - ptMin_) / nBinsPt_;
  
  for(double eta = etaMin_; eta <= etaMax_; eta+=etaWidth)
  {
    for(double pt = ptMin_; pt <= ptMax_; pt+=ptWidth)    
    {
      jecUnc->setJetEta(eta);
      jecUnc->setJetPt(pt);
      double unc = jecUnc->getUncertainty(true);    
      
      outFile << std::fixed << std::scientific << std::setprecision(3) << eta << " " << pt << " " << unc << std::endl;
    }
  }
  
}



///===================================
DEFINE_FWK_MODULE(DumpJEC) ;



{


 TChain *tree = new TChain ("ntpla/VBFSimpleTree") ;
//  tree->Add ("/media/amassiro/Data/SimpleTree_skipBadFiles_JetCorrector_JetCleaning_090328_Everything_Skimmed_4Cluster_AllJets/VBF_SimpleTree_H160_WW_2l_redigi.root");
  tree->Add ("VBF_SimpleTree_H160_WW_2l_redigi.root");
 
 
 TClonesArray * otherJets_temp = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("otherJets_SisCone5CaloJets", &otherJets_temp) ;
 TClonesArray * electrons = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("electrons", &electrons) ;
 TClonesArray * muons = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("muons", &muons) ;
 TClonesArray * MET = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("MET", &MET) ;
 TClonesArray * tracks = new TClonesArray ("TLorentzVector") ;
 tree->SetBranchAddress ("tracks", &tracks) ;
 
 int IdEvent;
 tree->SetBranchAddress ("IdEvent", &IdEvent) ;
 
 
 int nentries = (int) tree->GetEntries () ;

 TH1F hDR("hDR","hDR",1000,0,10);
 
 nentries = 500;
 
 for (int evt = 0 ; evt < nentries ; ++evt)
 {
  if (! (evt%1000)) std::cerr << evt << std::endl;
  tree->GetEntry (evt) ;
  if (IdEvent!=123 && IdEvent!=124) continue;
    
  for (int iele = 0; iele < electrons->GetEntries() ; ++iele)
  {
   TLorentzVector * theEle = (TLorentzVector*) (electrons->At (iele)) ;
   double minDR = 100000;
   for (int l=0; l<otherJets_temp->GetEntries (); l++ ){
    TLorentzVector * theJet = (TLorentzVector*) (otherJets_temp->At (l)) ;
    double DR = theEle->DeltaR(*theJet);
    if (minDR>DR) minDR = DR;
   }
   hDR.Fill(minDR);
  }
 }
}
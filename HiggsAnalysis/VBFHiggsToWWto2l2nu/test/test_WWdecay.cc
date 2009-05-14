{

#include "TParticle.h"

 TChain *tree = new TChain ("ntpla/VBFSimpleTree") ;
//  tree->Add ("/media/amassiro/Data/SimpleTree_skipBadFiles_JetCorrector_JetCleaning_090328_Everything_Skimmed_4Cluster_AllJets/VBF_SimpleTree_H160_WW_2l_redigi.root");
  tree->Add ("VBF_SimpleTree_H160_WW_2l_redigi.root");
//   tree->Add ("VBF_SimpleTree_NAME.root");
  
  TClonesArray * genParticles = new TClonesArray ("TParticle") ;
  tree->SetBranchAddress ("genParticles", &genParticles) ;
 
 
      
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

  TH1F hDecay("hDecay","hDecay",10,0,10);
  TTree mytree("mytree","mytree");
  int num;
  mytree->Branch("num",&num,"num/I");
  nentries = 2000;
 
  for (int evt = 0 ; evt < nentries ; ++evt)
  {
   if (! (evt%1000)) std::cerr << evt << std::endl;
   tree->GetEntry (evt) ;
   if (IdEvent!=123 && IdEvent!=124) continue;
    
   std::cerr << "*** New Event ***"<< std::endl;
   for (int iGen = 0; iGen < genParticles->GetEntries() ; ++iGen){
    TParticle* myparticle = (TParticle*) genParticles->At(iGen);
    if (abs(myparticle->GetPdgCode()) == 24) {
     std::cerr << "found a W"<< std::endl;
     for (int iDaughter = 0; iDaughter<2; iDaughter++){
      if (abs(myparticle->GetDaughter(iDaughter)) == 11) {    
       std::cerr << "  W -> e"<< std::endl;     
       hDecay.Fill(0);
       num = 0;
      }
      if (abs(myparticle->GetDaughter(iDaughter)) == 13) {    
       std::cerr << "  W -> mu"<< std::endl;       
       hDecay.Fill(1);
       num = 1;
      }
      if (abs(myparticle->GetDaughter(iDaughter)) == 15) {    
       std::cerr << "  W -> tau"<< std::endl;       
       hDecay.Fill(2);
       num = 2;
      }
     
      if (abs(myparticle->GetDaughter(iDaughter)) == 0) {    
       std::cerr << "  W final state"<< std::endl;       
       num = -1;
      }

      mytree.Fill();
     }
    }
   }
 
  }
 
} 
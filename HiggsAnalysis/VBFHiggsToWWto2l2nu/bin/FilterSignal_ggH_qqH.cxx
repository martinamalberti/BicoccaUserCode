{
 
 TFile in("/tmp/amassiro/VBF_SimpleTree_H160_WW_2l_redigi.root");
 
  
 TTree *oldtree = (TTree*)in->Get("ntpla/VBFSimpleTree");
 
 TFile qq("/tmp/amassiro/VBF_SimpleTree_H160_WW_2l_redigi_qqH.root", "RECREATE");
 qq.mkdir("ntpla");
 qq.cd("ntpla");
 TTree *newtree_qq = oldtree->CloneTree(0);
 newtree_qq->SetName("VBFSimpleTree");
 
 TFile gg("/tmp/amassiro/VBF_SimpleTree_H160_WW_2l_redigi_ggH.root", "RECREATE"); 
 gg.mkdir("ntpla");
 gg.cd("ntpla");
 TTree *newtree_gg = oldtree->CloneTree(0);
 newtree_gg->SetName("VBFSimpleTree");
 
 Int_t nentries = (Int_t)oldtree->GetEntries();
 int IdEvent;
 oldtree->SetBranchAddress ("IdEvent", &IdEvent) ;
 
 std::cerr << "nentries = " << nentries << std::endl;
 nentries = 2000;
 for (Int_t i=0;i<nentries; i++) {
  if (!(i%1000)) std::cerr << "event = " << i << std::endl;
  oldtree->GetEntry(i);
  if (IdEvent==123 || IdEvent==124) newtree_qq->Fill();
  else newtree_gg->Fill();
 }
 
 
 
//  qq.cd(0);
//  newtree_qq->AutoSave();
//  
//  gg.cd(0);
//  newtree_gg->AutoSave();

 
}

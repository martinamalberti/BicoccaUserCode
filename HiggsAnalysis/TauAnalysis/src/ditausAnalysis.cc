#include <HiggsAnalysis/TauAnalysis/interface/ditausAnalysis.h>


ditausAnalysis::ditausAnalysis(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
 
  twopi  = 2*acos(-1.);

  MZ = 91.187;

  nEvt=0;
  MCPARMAX=20;
  ELEMAX=10;
  MUONMAX=10;
  JETMAX=30;
  
  thefile = new TFile("testNicolaII.root","recreate");
  thefile->cd();

//   zmass = new TH1F("zmass","invariant mass of e+mu",200,0,200);
//   hEoP  = new TH1F("hEoP","E/P for electron candidates",200,0,10);
//   hdEta = new TH1F("hdEta","#eta_{SC} - #eta_{trk} for electron candidates",200,-1,1);
//   hdPhi = new TH1F("hdPhi","#phi_{SC} - #phi_{trk}for electron candidates",200,-6,6);
//   //  hHoE = new TH1F("hHoE","H/E for electron candidates",200,0,10);

//   hETeW = new TH1F("hETeW","Electron E^{T} from W events",200,0,100); 
//   hETeZ = new TH1F("hETeZ","Electron E^{T} from Z events",200,0,100);
//   hETeW_cut = new TH1F("hETeW_cut","Electron E^{T} from W events (with selections)",200,0,100) ;
//   hETeZ_cut = new TH1F("hETeZ_cut","Electron E^{T} from Z events (with selections)",200,0,100) ;
//   hETeBkg = new TH1F("hETeBkg","Electron E^{T} (Z bkg)",200,0,100); 
//   hETeBkg_cut = new TH1F("hETeBkg_cut","Electron E^{T} (Z bkg, with sel.)",200,0,100);
 
  smalltree= new TTree("tree","tree");

  smalltree->Branch("nMCPar",&nMCPar,"nMCPar/I");
  smalltree->Branch("MCp",MCp,"MCp[nMCPar][3]/D");
  smalltree->Branch("MCeta",MCeta,"MCeta[nMCPar]/D");
  smalltree->Branch("MCphi",MCphi,"MCphi[nMCPar]/D");
  smalltree->Branch("MCpid",MCpid,"MCpid[nMCPar]/I");
 
  smalltree->Branch("nEleCand",&nEleCand,"nEleCand/I");
  smalltree->Branch("ele_p",ele_p,"Ele_p[nEleCand][3]/D");
  smalltree->Branch("ele_eSC",ele_eSC,"ele_eSC[nEleCand]/D");
  smalltree->Branch("ele_etSC",ele_etSC,"ele_etSC[nEleCand]/D");
  smalltree->Branch("ele_etaSC",ele_etaSC,"ele_etaSC[nEleCand]/D");
  smalltree->Branch("ele_UncorrEtaSC",ele_UncorrEtaSC,"ele_UncorrEtaSC[nEleCand]/D");
  smalltree->Branch("ele_phiSC",ele_phiSC,"ele_phiSC[nEleCand]/D");
  smalltree->Branch("ele_etaTrk",ele_etaTrk,"ele_etaTrk[nEleCand]/D");
  smalltree->Branch("ele_phiTrk",ele_phiTrk,"ele_phiTrk[nEleCand]/D");
  smalltree->Branch("ele_charge",ele_charge,"ele_charge[nEleCand]/I");
  smalltree->Branch("ele_TrkIso02",ele_TrkIso02,"ele_TrkIso02[nEleCand]/D");
  smalltree->Branch("ele_TrkIso035",ele_TrkIso035,"ele_TrkIso035[nEleCand]/D");
  smalltree->Branch("ele_TrkIso05",ele_TrkIso05,"ele_TrkIso05[nEleCand]/D");

  smalltree->Branch("nMuonCand",&nMuonCand,"nMuonCand/I");
  smalltree->Branch("muon_p",muon_p,"muon_p[nMuonCand][3]/D");
  smalltree->Branch("muon_eta",muon_eta,"muon_eta[nMuonCand]/D");
  smalltree->Branch("muon_phi",muon_phi,"muon_phi[nMuonCand]/D");
  smalltree->Branch("muon_charge",muon_charge,"muon_charge[nMuonCand]/I");

  smalltree->Branch("nJetCand",&nJetCand,"nJetCand/I");
  smalltree->Branch("JetCand_p",JetCand_p,"JetCand_px[nJetCand][3]/D");
  smalltree->Branch("JetCand_pt",JetCand_pt,"JetCand_pt[nJetCand]/D");
  smalltree->Branch("JetCand_eta",JetCand_eta,"JetCand_eta[nJetCand]/D");
  smalltree->Branch("JetCand_phi",JetCand_phi,"JetCand_phi[nJetCand]/D");

  smalltree->Branch("met",&met,"met/D");
  smalltree->Branch("mex",&mex,"mex/D");
  smalltree->Branch("mey",&mey,"mey/D");
  smalltree->Branch("met_phi",&met_phi,"met_phi/D");
  smalltree->Branch("Set",&Set,"Set/D");
  smalltree->Branch("GenMet",&GenMet,"GenMet/D");
  smalltree->Branch("GenMex",&GenMex,"GenMex/D");
  smalltree->Branch("GenMey",&GenMey,"GenMey/D");
  smalltree->Branch("GenMet_phi",&GenMet_phi,"GenMet_phi/D");
  smalltree->Branch("GenSet",&GenSet,"GenSet/D");

}


ditausAnalysis::~ditausAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
ditausAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   // this analyzer produces a small root file with basic candidates and some MC information
   // some additional print statements
   nEvt++;
   if((nEvt%10==0 && nEvt<=100)||(nEvt%100==0 && nEvt>100))
     std::cout << "reading event " << nEvt << std::endl;
   


   for(int rr=0; rr<20; rr++)
     MCpid[rr]=0;

   // MC information 
   Handle<HepMCProduct> evt;
   iEvent.getByLabel("VtxSmeared", evt);
   nMCPar=0;
   HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
   for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
	 p != myGenEvent->particles_end(); ++p ) 
     {  
       if(abs((*p)->pdg_id()) == 15 && (*p)->status() == 2)
	 {
	   std::vector<HepMC::GenParticle*> children = (*p)->listChildren();
	   std::vector<HepMC::GenParticle*>::const_iterator daughter;
	   for (daughter = children.begin();daughter != children.end();daughter++)
	     {
	       MCp[nMCPar][0]=(*daughter)->momentum().x();
	       MCp[nMCPar][1]=(*daughter)->momentum().y();
	       MCp[nMCPar][2]=(*daughter)->momentum().z();
	       MCeta[nMCPar]=(*daughter)->momentum().pseudoRapidity();
	       MCphi[nMCPar]=(*daughter)->momentum().phi();
	       MCpid[nMCPar]=(*daughter)->pdg_id();	 
	       nMCPar++;

	       //std::cout << "pdg_id (MCpid) = " << MCpid[nMCPar] << "; (*p)->pdg_id() = "<< (*p)->pdg_id() << "; (*p)->momentum().x() ="<< (*p)->momentum().x() << std::endl;
	     }
	 }
     }
   
   
   //Electrons
   Handle<reco::PixelMatchGsfElectronCollection> electrons;
   iEvent.getByLabel("pixelMatchGsfElectrons",electrons);
   reco::PixelMatchGsfElectronCollection::const_iterator ele;
   nEleCand=0;
   // TRACKS: needed to check electron isolation  
   Handle<reco::TrackCollection> tracks;
   iEvent.getByLabel("pixelTracks",tracks);
   reco::TrackCollection::const_iterator trk; 
   
   float newtheta;
   float CorrEtaSC;
   float R_ecal;
   //   float EtaBarrelEndcap = 1.479;

   for( ele = electrons->begin(); ele != electrons->end() && nEleCand<ELEMAX; ++ ele ) {
     ele_eSC[nEleCand]  = (ele->superCluster())->energy();// SC energy 
     ele_etSC[nEleCand] = (ele->superCluster())->energy() * sin(ele->theta());// SC ET  
     ele_p[nEleCand][0] = ele->px();// associated track momentum 
     ele_p[nEleCand][1] = ele->py();// associated track momentum 
     ele_p[nEleCand][2] = ele->pz();// associated track momentum 
     ele_charge[nEleCand] = ele->charge();


     //std::cout << "ele_p[" << nEleCand << "][0] = " << ele_p[nEleCand][0] << std::endl;
   
     // SC eta vertex correction 
     //std::cout<<"eta = "<<ele->superCluster()->eta()<<"    z_SC = "<<ele->superCluster()->z()<< "     RR = "<< sqrt(pow(ele->superCluster()->x(),2)+pow(ele->superCluster()->y(),2)) <<std::endl;

     R_ecal = sqrt(pow(ele->superCluster()->x(),2)+pow(ele->superCluster()->y(),2));
     newtheta = atan(R_ecal/(ele->superCluster()->z()-ele->gsfTrack()->vz()));
     if (newtheta<0.) newtheta = newtheta + twopi/2;
     
     CorrEtaSC = -log(tan(newtheta/2.));
     ele_etaSC[nEleCand]  = CorrEtaSC;
     ele_UncorrEtaSC[nEleCand]  = ele->superCluster()->eta();
     ele_etaSC[nEleCand]  = CorrEtaSC;
     ele_phiSC[nEleCand]  = ele->superCluster()->phi();
     ele_etaTrk[nEleCand] = ele->eta(); 
     ele_phiTrk[nEleCand] = ele->phi();
     EoP = ele_eSC[nEleCand]/ele->p();
     dEtaTrkClus = ele->superCluster()->eta() - ele->eta();
     dPhiTrkClus = fabs((ele->superCluster())->phi() - ele->phi());
     if (dPhiTrkClus>(twopi-dPhiTrkClus)) {dPhiTrkClus = twopi-dPhiTrkClus;}

     //   Some quality cuts to select electrons among "electron candidates"...   LI FACCIO DOPO!!
     //if (EoP < 0.8) {continue;}
     //if (fabs(1/ele_eSC[nEleCand]-1/ele->p())>0.02) {continue;}
     //if (fabs(dEtaTrkClus)>0.005) {continue;}??????????????????????????? boh?
     //if (fabs(dEtaTrkClus)>0.1) {continue;}
     //if (fabs(dPhiTrkClus)>0.1) {continue;}???????????????????
     // H_over_E ???????????????????????????????????????????????  
    
     // electron isolation 
     PTr02  = 0.;
     PTr035 = 0.;
     PTr05  = 0.;
     for(trk = tracks->begin(); trk!=tracks->end(); ++trk)
       {
	 deta = fabs( ele->eta() - trk->eta());
	 dphi = fabs( ele->phi() - trk->phi());
	 if (dphi>(twopi-dphi)) {dphi = twopi-dphi;}
	 R = sqrt(deta*deta + dphi*dphi);
	 if ((0.02 < R) && (R < 0.2)) { PTr02+=trk->pt() ;}
	 if ((0.02 < R) && (R < 0.35)) { PTr035+=trk->pt();}
	 if ((0.02 < R) && (R < 0.5)) { PTr05+=trk->pt() ;}
       }
      if (((ele->superCluster())->energy()*sin(ele->theta()))!=0){
       TrkIsolation02  = (PTr02 - ele->pt()) /(ele->superCluster()->energy()*sin(ele->theta()));
       TrkIsolation035 = (PTr035 - ele->pt())/(ele->superCluster()->energy()*sin(ele->theta()));
       TrkIsolation05  = (PTr05 - ele->pt()) /(ele->superCluster()->energy()*sin(ele->theta()));
     }
     else{
       TrkIsolation02  = 1 ;
       TrkIsolation035 = 1 ;
       TrkIsolation05  = 1 ;
     } 
     ele_TrkIso02[nEleCand] = TrkIsolation02;
     ele_TrkIso035[nEleCand]= TrkIsolation035;
     ele_TrkIso05[nEleCand] = TrkIsolation05;
     // if (TrkIsolation02>0.2) {continue;}     ????????????????????boh????????????????????????
     nEleCand++;
   }
   
   //Muons  POI CONTROLLO SULLA CARICA OPPOSTA CON L'e!!!!
   Handle<reco::TrackCollection> muons;
   iEvent.getByLabel("globalMuons",muons);
   reco::TrackCollection::const_iterator muon;
   nMuonCand=0;
   for( muon = muons->begin(); muon != muons->end() && nMuonCand<MUONMAX; ++ muon ) {
     muon_p[nMuonCand][0]=muon->px();
     muon_p[nMuonCand][1]=muon->py();
     muon_p[nMuonCand][2]=muon->pz();
     muon_phi[nMuonCand]=muon->phi();     
     muon_eta[nMuonCand]=muon->eta();
     muon_charge[nMuonCand]=muon->charge();
     nMuonCand++;
   }

   

   //Jets     (CON SELEZIONE DI ELETTRONI)
   bool is_ele=false;
   Handle<reco::CaloJetCollection> jets;
   iEvent.getByLabel("iterativeCone5CaloJets",jets);
   reco::CaloJetCollection::const_iterator jet;
   nJetCand=0;
   for( jet = jets->begin(); jet != jets->end() && nJetCand<JETMAX; ++ jet ) {
     for( int ie = 0; ie < nEleCand; ie++ ) {
       deta = fabs( ele_etaTrk[ie] - jet->eta());
       dphi = fabs( ele_phiTrk[ie] - jet->phi());
       if (dphi>(twopi-dphi)) {dphi = twopi-dphi;}
       R = sqrt(deta*deta + dphi*dphi);
       if (R<0.15) {is_ele = true; break;}
     }
     if (is_ele) {continue;}
     JetCand_pt[nJetCand]   = jet->et();
     JetCand_p[nJetCand][0] = jet->px();
     JetCand_p[nJetCand][1] = jet->py();
     JetCand_p[nJetCand][2] = jet->pz();
     JetCand_eta[nJetCand]  = jet->eta();
     JetCand_phi[nJetCand]  = jet->phi();
     nJetCand++;
   }
   
   //MET MC
   Handle<reco::GenMETCollection> GenMets;
   iEvent.getByLabel("genMet",GenMets);
   reco::GenMETCollection::const_iterator GenMissingET;
   GenMissingET = GenMets->begin();
   GenMet_phi = GenMissingET->phi();
   GenMet = GenMissingET->pt();
   GenMex = GenMissingET->px();
   GenMey = GenMissingET->py();
   GenSet = GenMissingET->sumEt();
 
   //RECO MET  from Calo Towers
   Handle<reco::CaloMETCollection> mets;
   iEvent.getByLabel("met",mets);
   reco::CaloMETCollection::const_iterator missingET;
   missingET = mets->begin();
   met_phi = missingET->phi();
   met = missingET->pt();
   mex = missingET->px();
   mey = missingET->py();
   Set = missingET->sumEt();
   

//    // select events and fill  histograms:
//    if (nEleCand ==1 && fabs(ele_etaSC[0])<2.4) {
//      if (isW) {hETeW->Fill(ele_etSC[0]);}  
//      if (isZ) {hETeBkg->Fill(ele_etSC[0]);}  
//      u = sqrt(pow(mex+ele_p[0][0],2)+pow(mey+ele_p[0][1],2));
//      if (isW && met>25 && u<20 && ele_etSC[0]>26) {hETeW_cut->Fill(ele_etSC[0]);} 
//      if (isZ && met>25 && u<20 && ele_etSC[0]>26) {hETeBkg_cut->Fill(ele_etSC[0]);} 
//    }

//    float diff = 10000;
//    float mee;
//    int chosenEle1 = -1;
//    int chosenEle2 = -1;
//    if (isZ && nEleCand>1 ) {
//      for(int ie=0; ie<nEleCand; ie++){
//        for(int je=ie+1; je<nEleCand; je++){
// 	 if(ele_charge[ie]*ele_charge[je]<0){//opposite charge electrons
// 	   p1 = sqrt(pow(ele_p[ie][0],2)+pow(ele_p[ie][1],2)+pow(ele_p[ie][2],2));
// 	   p2 = sqrt(pow(ele_p[je][0],2)+pow(ele_p[je][1],2)+pow(ele_p[je][2],2));
// 	   cphi = (ele_p[ie][0]*ele_p[je][0]+ele_p[ie][1]*ele_p[je][1]+ele_p[ie][2]*ele_p[je][2])/(p1*p2);
// 	   mass = sqrt(2*ele_eSC[ie]*ele_eSC[je]*(1-cphi));
// 	   if (fabs(mass - MZ) < diff) {
// 	     diff = (mass-MZ);
// 	     mee = mass;
// 	     chosenEle1 = ie;
// 	     chosenEle2 = je;
// 	   }
// 	 }
// 	 zmass->Fill(mee);
// 	 if (mee>50 && fabs(ele_etaSC[chosenEle1])<2.4 && fabs(ele_etaSC[chosenEle2])<2.4 ) {
// 	   hETeZ->Fill(ele_etSC[chosenEle1]);
// 	   metz = sqrt(pow(mex+ele_p[chosenEle2][0],2)+ pow(mey+ele_p[chosenEle2][1],2));
// 	   u=sqrt(pow(mex+ele_p[chosenEle1][0]+ele_p[chosenEle2][0],2)+pow(mey+ele_p[chosenEle1][1]+ele_p[chosenEle2][1],2));
// 	   if (metz>25 && u <20 && ele_etSC[chosenEle1]>26) hETeZ_cut->Fill(ele_etSC[chosenEle1]);
// 	 }  
//        }
//      }
//    }

// save the event in a smaller tree:

   smalltree->Fill();
   //std::cout << std::endl;
}


// ------------ method called once each job just before starting event loop  ------------
void 
ditausAnalysis::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ditausAnalysis::endJob() {
  std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "analyzed " << nEvt << " events " << std::endl;
  std::cout << "writing information into file: " << thefile->GetName() << std::endl;
  thefile->Write();
  thefile->Close();
}

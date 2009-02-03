// $Id: VBFDisplay.cc,v 1.5 2009/01/15 13:24:17 govoni Exp $
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFDisplay.h"
//#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include <TMarker.h>


VBFDisplay::VBFDisplay (const edm::ParameterSet& iConfig) :
  m_jetTagsInputTag (iConfig.getParameter<edm::InputTag> ("jetTagsInputTag")) ,
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_tracksInputTag (iConfig.getParameter<edm::InputTag> ("tracksInputTag")) ,
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_electronIDInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_metInputTag (iConfig.getParameter<edm::InputTag> ("metInputTag")) ,
  m_MCtruthInputTag (iConfig.getParameter<edm::InputTag> ("MCtruthInputTag")) ,
  m_drawMC (iConfig.getParameter<bool> ("drawMC"))
{}


// --------------------------------------------------------------------


VBFDisplay::~VBFDisplay ()
{
 
   delete m_c1 ;
   delete m_bkg ;
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// --------------------------------------------------------------------


void
VBFDisplay::analyze (const edm::Event& iEvent, 
                             const edm::EventSetup& iSetup)
{
  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_jetTagsInputTag, jetTagsHandle) ;
//  fillgraph (m_tagjet_eta, m_tagjet_phi, m_tagjet_graph, jetTagsHandle, 24, 1) ;

  m_tagjet_eta = new double [jetTagsHandle->size ()] ;
  m_tagjet_phi = new double [jetTagsHandle->size ()] ;

  for (int it = 0 ; it < jetTagsHandle->size () ; ++it)
    {
      m_tagjet_eta[it] = (*jetTagsHandle)[it].eta () ;
      m_tagjet_phi[it] = (*jetTagsHandle)[it].phi () ;
    }
  m_tagjet_graph = new TGraph (jetTagsHandle->size (), m_tagjet_eta, m_tagjet_phi) ;
  m_tagjet_graph->SetMarkerStyle (24) ;
  m_tagjet_graph->SetMarkerColor (4) ;

  //PG get the other jets
  edm::Handle<reco::CaloJetCollection> jetsHandle ;
  iEvent.getByLabel (m_jetInputTag, jetsHandle) ;

  m_jet_eta = new double [jetsHandle->size ()] ;
  m_jet_phi = new double [jetsHandle->size ()] ;

  for (int it = 0 ; it < jetsHandle->size () ; ++it)
    {
      m_jet_eta[it] = (*jetsHandle)[it].eta () ;
      m_jet_phi[it] = (*jetsHandle)[it].phi () ;
    }
  m_jet_graph = new TGraph (jetsHandle->size (), m_jet_eta, m_jet_phi) ;
  m_jet_graph->SetMarkerStyle (25) ;
  m_jet_graph->SetMarkerColor (1) ;

  //PG get the tracks collection
  edm::Handle<trackCollection> tracksHandle ;
  iEvent.getByLabel (m_tracksInputTag, tracksHandle) ;

  m_tracks_eta = new double [tracksHandle->size ()] ;
  m_tracks_phi = new double [tracksHandle->size ()] ;

  for (int it = 0 ; it < tracksHandle->size () ; ++it)
    {
      m_tracks_eta[it] = (*tracksHandle)[it].eta () ;
      m_tracks_phi[it] = (*tracksHandle)[it].phi () ;
    }
  m_tracks_graph = new TGraph (tracksHandle->size (), m_tracks_eta, m_tracks_phi) ;
  m_tracks_graph->SetMarkerStyle (5) ;
  m_tracks_graph->SetMarkerColor (67) ;

  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  m_elecs_eta = new double [GSFHandle->size ()] ;
  m_elecs_phi = new double [GSFHandle->size ()] ;

  for (int it = 0 ; it < GSFHandle->size () ; ++it)
    {
      m_elecs_eta[it] = (*GSFHandle)[it].eta () ;
      m_elecs_phi[it] = (*GSFHandle)[it].phi () ;
    }
  m_elecs_graph = new TGraph (GSFHandle->size (), m_elecs_eta, m_elecs_phi) ;
  m_elecs_graph->SetMarkerStyle (30) ;
  m_elecs_graph->SetMarkerColor (2) ;

  //VT get the Global muons collection
  edm::Handle<muonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 

  m_muons_eta = new double [MuonHandle->size ()] ;
  m_muons_phi = new double [MuonHandle->size ()] ;

  for (int it = 0 ; it < MuonHandle->size () ; ++it)
    {
      m_muons_eta[it] = (*MuonHandle)[it].eta () ;
      m_muons_phi[it] = (*MuonHandle)[it].phi () ;
    }
  m_muons_graph = new TGraph (MuonHandle->size (), m_muons_eta, m_muons_phi) ;
  m_muons_graph->SetMarkerStyle (26) ;
  m_muons_graph->SetMarkerColor (2) ;

  //PG get the calo MET
  edm::Handle<reco::CaloMETCollection> metCollectionHandle;
  iEvent.getByLabel (m_metInputTag, metCollectionHandle);
  const CaloMETCollection *calometcol = metCollectionHandle.product () ;
  const CaloMET *calomet = &(calometcol->front ()) ;  
 
  m_MET_eta = new double [1] ;
  m_MET_phi = new double [1] ;

  m_MET_eta[0] = calomet->eta () ;
  m_MET_phi[0] = calomet->phi () ;

  m_MET_graph = new TGraph (1, m_MET_eta, m_MET_phi) ;
  m_MET_graph->SetMarkerStyle (12) ;
  m_MET_graph->SetMarkerColor (8) ;

  //PG MCtruth
  TMarker * mcpoints[4] ;
  if (m_drawMC)
    {
      edm::Handle<reco::CandidateCollection> genParticles ; 
      iEvent.getByLabel (m_MCtruthInputTag, genParticles) ;
    
      //PG loop over generated particles  
      int num = 0 ;
      for (reco::CandidateCollection::const_iterator particle = genParticles->begin () ; 
           particle != genParticles->end () ; 
           ++particle) 
        {
          int mumPDG = particle->pdgId () ;
          int mumSTATUS = particle->status () ;
          if (abs (mumPDG) == 24 && mumSTATUS == 3) //W+-
            {
              for (int i = 0; i < particle->numberOfDaughters () ; ++i) 
                {
                  int PDG = particle->daughter (i)->pdgId () ;    
                  if (abs (PDG) == 11)
                    {
                      mcpoints[num] = new TMarker (particle->daughter (i)->eta (),
                                                   particle->daughter (i)->phi (), 21) ;
                      mcpoints[num]->SetMarkerSize (2) ;
                      mcpoints[num]->SetMarkerColor (196) ;
                      ++num ;
                    }
                  else if (abs (PDG) == 13)
                    {
                      mcpoints[num] = new TMarker (particle->daughter (i)->eta (),
                                                   particle->daughter (i)->phi (), 22) ;
                      mcpoints[num]->SetMarkerSize (2) ;
                      mcpoints[num]->SetMarkerColor (196) ;
                      ++num ;
                    }
                }
            } //PG W+-
            
          if ((abs (mumPDG) == 25) && (mumSTATUS == 3)) //PG look for jet tags
            {
              if (particle->numberOfMothers () < 1) continue ;
              const Candidate * interact0 = particle->mother (0) ;
              if (interact0->numberOfDaughters () < 2) continue ;
    
              mcpoints[num] = new TMarker (interact0->daughter (0)->eta (),
                                           interact0->daughter (0)->phi (), 20) ;
              mcpoints[num]->SetMarkerSize (3) ;
              mcpoints[num]->SetMarkerColor (21) ;
              ++num ;
              mcpoints[num] = new TMarker (interact0->daughter (1)->eta (),
                                           interact0->daughter (1)->phi (), 20) ;
              mcpoints[num]->SetMarkerSize (3) ;
              mcpoints[num]->SetMarkerColor (21) ;
              ++num ;
            } //PG look for jet tags
         } //PG loop over generated particles
     } //PG MC truth

// -------------------------------------------------

  m_c1->cd () ;
  m_bkg->Draw () ;
  if (m_drawMC) for (int i = 0 ; i < 4 ; ++i) mcpoints[i]->Draw () ;
  m_tracks_graph->Draw ("P") ;
  m_tagjet_graph->Draw ("P") ;
  m_jet_graph->Draw ("P") ;
  m_elecs_graph->Draw ("P") ;
  m_muons_graph->Draw ("P") ;
  m_MET_graph->Draw ("P") ;
  m_legend = new TLegend (0.9, 0.85, 1., 1.) ;
  char labelName[10] ;

  sprintf (labelName,"TAG %d",jetTagsHandle->size ()) ;
  m_legend->AddEntry (m_tagjet_graph,labelName,"P") ;
  sprintf (labelName,"JET %d",jetsHandle->size ()) ;   
  m_legend->AddEntry (m_jet_graph,labelName,"P") ;
  sprintf (labelName,"ELE %d",GSFHandle->size ()) ;    
  m_legend->AddEntry (m_elecs_graph,labelName,"P") ;
  sprintf (labelName,"TK %d",tracksHandle->size ()) ; 
  m_legend->AddEntry (m_tracks_graph,labelName,"P") ;
  sprintf (labelName,"MU %d",MuonHandle->size ()) ;   
  m_legend->AddEntry (m_muons_graph,labelName,"P") ;
  m_legend->AddEntry (m_MET_graph,"MET","P") ;
  m_legend->Draw () ;
  char nome[30] ;
  sprintf (nome,"event_%d.gif",iEvent.id ().run ()) ;
  m_c1->Print (nome,"gif") ;
  
  //PG FIXME add the event number

  delete m_legend ;
  if (m_drawMC) for (int i = 0 ; i < 4 ; ++i) delete mcpoints[i] ;
  delete [] m_tagjet_eta ;
  delete [] m_tagjet_phi ;
  delete m_tagjet_graph ;

  delete [] m_jet_eta ;
  delete [] m_jet_phi ;
  delete m_jet_graph ;

  delete [] m_elecs_eta ;
  delete [] m_elecs_phi ;
  delete m_elecs_graph ;

  delete [] m_tracks_eta ;
  delete [] m_tracks_phi ;
  delete m_tracks_graph ;

  delete [] m_muons_eta ;
  delete [] m_muons_phi ;
  delete m_muons_graph ;

  delete [] m_MET_eta ;
  delete [] m_MET_phi ;
  delete m_MET_graph ;
}


// --------------------------------------------------------------------


void 
VBFDisplay::beginJob (const edm::EventSetup&)
{
//  gROOT->SetStyle ("Plain") ;
  m_c1 = new TCanvas () ;
  m_c1->SetGrid () ;
  m_bkg = new TH2F ("bkg", "",10,-5,5,10,-3.14,3.14) ;
  m_bkg->SetStats (0) ;
  m_bkg->GetXaxis ()->SetTitle ("#eta") ;
  m_bkg->GetYaxis ()->SetTitle ("#phi") ;

}


// --------------------------------------------------------------------


void 
VBFDisplay::endJob () 
{
}


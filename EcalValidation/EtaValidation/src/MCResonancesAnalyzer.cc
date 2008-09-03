
#include "EcalValidation/EtaValidation/interface/MCResonancesAnalyzer.h"

#include "DataFormats/JetReco/interface/GenJet.h"

#include <iostream>

//MCResonancesAnalyzer::MCResonancesAnalyzer(const edm::ParameterSet& iConfig) : label_(iConfig.getUntrackedParameter("moduleLabel",std::string("source"))) 
MCResonancesAnalyzer::MCResonancesAnalyzer(const edm::ParameterSet& iConfig)
{
  
  std::string outputFilename = iConfig.getUntrackedParameter<std::string>("OutputFilename","dummy.root");
  thefile = new TFile(outputFilename.c_str(),"recreate");
  thefile->cd();
  
  Counter    = 0;
  Pi0Counter = 0;
  EtaCounter = 0;

  // initialize histograms
  
  hNeta = new TH1F("hNeta","Number of eta->GammaGamma",200,0,200);
  hNpi0 = new TH1F("hNpi0","Number of pi0->GammaGamma",200,0,200);
  hNGammaFromPi0  = new TH1F("hNGammaFromPi0 ","Number of photons from pi0",10,0,10);
  hNGammaFromEta  = new TH1F("hNGammaFromEta","Number of photons from eta",10,0,10);
  hPi0Pt = new TH1F("hPi0Pt","Pi0 transverse momentum",500,0,50);
  hEtaPt = new TH1F("hEtaPt","Eta transverse momentum",500,0,50);

}

MCResonancesAnalyzer::~MCResonancesAnalyzer() {}

void MCResonancesAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
   edm::Handle<edm::HepMCProduct> evt;
   iEvent.getByLabel("source",evt);
   const HepMC::GenEvent * myEvt = evt->GetEvent();

   Counter++;

   int nPi0 = 0;
   int nEta = 0;

   for (HepMC::GenEvent::particle_const_iterator p = myEvt->particles_begin(); p!=myEvt->particles_end(); ++p){
         
     
     int nGammaFromPi0 = -1;
     int nGammaFromEta = -1;
     
     //std::cout << "evt.: "<<Counter<<"  pid = "<< (*p)->pdg_id() << "   status = "<< (*p)->status()<<std::endl;
	  
     if ( ( abs((*p)->pdg_id()) == 111 ||  abs((*p)->pdg_id()) == 221 ) && ((*p)->status()==2)) {

       nGammaFromPi0 = 0;
       nGammaFromEta = 0;

       // ---- look for Eta and Pi0 ----
       if ( (*p)->end_vertex() ) {
	 for (HepMC::GenVertex::particle_iterator d = (*p)->end_vertex()->particles_begin(HepMC::children); d != (*p)->end_vertex()->particles_end(HepMC::children); ++d){ 
	   
	   //std::cout << "-->daughter pid  = "<<(*d)->pdg_id()<<"    status = "<< (*d)->status()<<std::endl;
	   
	   if ((abs((*d)->pdg_id()) == 22) && (*d)->status()==1){
	     
	     if (abs((*p)->pdg_id()) == 111 && (*p)->status()==2)  { nGammaFromPi0++; }
	     if (abs((*p)->pdg_id()) == 221 && (*p)->status()==2)  { nGammaFromEta++; }
	   }
	 }// end loop over daughters
	 
	 
       }
       
       if ( abs((*p)->pdg_id()) == 111 ) hNGammaFromPi0 ->Fill( nGammaFromPi0 );
       if ( abs((*p)->pdg_id()) == 221 ) hNGammaFromEta ->Fill( nGammaFromEta );
       
       if (abs((*p)->pdg_id()) == 111 && nGammaFromPi0 == 2)  { 
	 Pi0Counter++; 
	 nPi0++;
	 hPi0Pt->Fill((*p)->momentum().perp());
       }
       if ( abs((*p)->pdg_id()) == 221 && nGammaFromEta == 2)  { 
	 EtaCounter++; 
	 nEta++;
	 hEtaPt->Fill((*p)->momentum().perp());
       }       
     } //end loop over eta and pi0
     
   }//end loop over particles
   
   hNpi0->Fill(nPi0);
   hNeta->Fill(nEta);
}


// ------------ method called once each job just before starting event loop  ------------
void 
MCResonancesAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MCResonancesAnalyzer::endJob() {
  std::cout << "Writing information into file: " << thefile->GetName() << std::endl;
  
  std::cout << "# of events : " << Counter << std::endl;
  std::cout << "# Pi0 : " << Pi0Counter << std::endl;
  std::cout << "# Eta : " << EtaCounter << std::endl;


  hNGammaFromPi0->Write();
  hNGammaFromEta->Write();

  hNpi0->Write();
  hNeta->Write();

  hPi0Pt->Write();
  hEtaPt->Write();

  thefile->Write();
  thefile->Close();
  
}



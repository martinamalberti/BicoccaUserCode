#include "WAnalysis/WenuAnalysis/interface/WenuTreeContent.h"

bool WenuTreeContent::electronVariables      = true;
bool WenuTreeContent::metVariables           = true;
bool WenuTreeContent::jetVariables           = true;
bool WenuTreeContent::muonVariables          = true;
bool WenuTreeContent::HLTrigVariables        = true;
bool WenuTreeContent::L1TrigVariables        = true;
  
void setBranchAddresses(TTree* chain, WenuTreeContent& treeVars)
{
  chain -> SetBranchAddress("BX",            &treeVars.BX);
  chain -> SetBranchAddress("lumiId",        &treeVars.lumiId);
  chain -> SetBranchAddress("runId",         &treeVars.runId);
  chain -> SetBranchAddress("eventId",       &treeVars.eventId);
  chain -> SetBranchAddress("eventNaiveId",  &treeVars.eventNaiveId);

  // ELECTRON VARIABLES  
  if(WenuTreeContent::electronVariables)
    {  
      chain -> SetBranchAddress("nElectrons",             &treeVars.nElectrons);
      chain -> SetBranchAddress("elePx",                   treeVars.elePx);
      chain -> SetBranchAddress("elePy",                   treeVars.elePy);
      chain -> SetBranchAddress("elePz",                   treeVars.elePz);
      chain -> SetBranchAddress("eleE",                    treeVars.eleE);
      chain -> SetBranchAddress("eleEt",                   treeVars.eleEt);
      chain -> SetBranchAddress("eleEta",                  treeVars.eleEta);
      chain -> SetBranchAddress("elePhi",                  treeVars.elePhi);

      chain -> SetBranchAddress("eleId",                   treeVars.eleId);
      chain -> SetBranchAddress("eleSigmaIEtaIEta",        treeVars.eleSigmaIEtaIEta);
      chain -> SetBranchAddress("eleE1x5",                 treeVars.eleE1x5);
      chain -> SetBranchAddress("eleE2x5",                 treeVars.eleE2x5);
      chain -> SetBranchAddress("eleE5x5",                 treeVars.eleE5x5);
      chain -> SetBranchAddress("eleSeedSwissCross",       treeVars.eleSeedSwissCross);

      chain -> SetBranchAddress("eleCharge",               treeVars.eleCharge);
      chain -> SetBranchAddress("eleTrkIso",               treeVars.eleTrkIso);
      chain -> SetBranchAddress("eleEcalIso",              treeVars.eleEcalIso);
      chain -> SetBranchAddress("eleHcalIsoD1",            treeVars.eleHcalIsoD1);
      chain -> SetBranchAddress("eleHcalIsoD2",            treeVars.eleHcalIsoD2);

      chain -> SetBranchAddress("genelePt",                treeVars.genelePt); 
      chain -> SetBranchAddress("geneleEta",               treeVars.geneleEta); 
      chain -> SetBranchAddress("genelePhi",               treeVars.genelePhi); 


    } // ELECTRON VARIABLES
  
 
  // MET VARIABLES  
  if(WenuTreeContent::metVariables)
    {  

      chain -> SetBranchAddress("caloMet",                   &treeVars.caloMet);
      chain -> SetBranchAddress("caloMex",                   &treeVars.caloMex);
      chain -> SetBranchAddress("caloMey",                   &treeVars.caloMey);
      chain -> SetBranchAddress("caloMetPhi",                &treeVars.caloMetPhi);
      chain -> SetBranchAddress("caloSumEt",                 &treeVars.caloSumEt);

      chain -> SetBranchAddress("tcMet",                   &treeVars.tcMet);
      chain -> SetBranchAddress("tcMex",                   &treeVars.tcMex);
      chain -> SetBranchAddress("tcMey",                   &treeVars.tcMey);
      chain -> SetBranchAddress("tcMetPhi",                &treeVars.tcMetPhi);
      chain -> SetBranchAddress("tcSumEt",                 &treeVars.tcSumEt);
  

      chain -> SetBranchAddress("pfMet",                   &treeVars.pfMet);
      chain -> SetBranchAddress("pfMex",                   &treeVars.pfMex);
      chain -> SetBranchAddress("pfMey",                   &treeVars.pfMey);
      chain -> SetBranchAddress("pfMetPhi",                &treeVars.pfMetPhi);
      chain -> SetBranchAddress("pfSumEt",                 &treeVars.pfSumEt);
     

      
    } // MET VARIABLES
  

  // JET VARIABLES  
  if(WenuTreeContent::jetVariables)
    {  
      chain -> SetBranchAddress("nJets",             &treeVars.nJets);
      chain -> SetBranchAddress("jetPx",              treeVars.jetPx);
      chain -> SetBranchAddress("jetPy",              treeVars.jetPy);
      chain -> SetBranchAddress("jetPz",              treeVars.jetPz);
      chain -> SetBranchAddress("jetPt",              treeVars.jetPt);
      chain -> SetBranchAddress("jetEta",             treeVars.jetEta);
      chain -> SetBranchAddress("jetPhi",             treeVars.jetPhi);
      chain -> SetBranchAddress("jetBdisc",           treeVars.jetBdisc);
      
      chain -> SetBranchAddress("genjetPt",           treeVars.genjetPt);
      chain -> SetBranchAddress("genjetEta",          treeVars.genjetEta);
      chain -> SetBranchAddress("genjetPhi",          treeVars.genjetPhi);


    } // JET VARIABLES
  

  
  // MUON VARIABLES  
  if(WenuTreeContent::muonVariables)
    {  
      chain -> SetBranchAddress("nMuons",             &treeVars.nMuons);
      chain -> SetBranchAddress("muonPx",              treeVars.muonPx);
      chain -> SetBranchAddress("muonPy",              treeVars.muonPy);
      chain -> SetBranchAddress("muonPz",              treeVars.muonPz);
      chain -> SetBranchAddress("muonPt",              treeVars.muonPt);
      chain -> SetBranchAddress("muonEta",             treeVars.muonEta);
      chain -> SetBranchAddress("muonPhi",             treeVars.muonPhi);
            
    } // MUON VARIABLES
  


  //L1 VARIABLES
  if(WenuTreeContent::L1TrigVariables)
    {
      chain -> SetBranchAddress("techL1Bit",     treeVars.techL1Bit);
      chain -> SetBranchAddress("algoL1Bit",     treeVars.algoL1Bit);
    }  //L1 VARIABLES
  
  
  //HLT VARIABLES
  if(WenuTreeContent::HLTrigVariables)
    {
      chain -> SetBranchAddress("HLT_Ele15_LW_L1R",           &treeVars.HLT_Ele15_LW_L1R);
      chain -> SetBranchAddress("HLT_Photon10_L1R",           &treeVars.HLT_Photon10_L1R);
      chain -> SetBranchAddress("HLT_Photon15_L1R",           &treeVars.HLT_Photon15_L1R);
      chain -> SetBranchAddress("HLT_Photon20_L1R",           &treeVars.HLT_Photon20_L1R);
      
    }//HLT VARIABLES
  
}


 



void setBranches(TTree* chain, WenuTreeContent& treeVars)
{
  chain -> Branch("BX",            &treeVars.BX,                       "BX/i");
  chain -> Branch("lumiId",        &treeVars.lumiId,               "lumiId/i");
  chain -> Branch("runId",         &treeVars.runId,                 "runId/i");
  chain -> Branch("eventId",       &treeVars.eventId,             "eventId/i");
  chain -> Branch("eventNaiveId",  &treeVars.eventNaiveId,   "eventNaiveId/i");
  
  
  // ELECTRON  VARIABLES  
  if(WenuTreeContent::electronVariables)
    {
      chain -> Branch("nElectrons",        &treeVars.nElectrons,       "nElectrons/I");
      chain -> Branch("elePx",              treeVars.elePx,            "elePx[nElectrons]/F");
      chain -> Branch("elePy",              treeVars.elePy,            "elePy[nElectrons]/F");
      chain -> Branch("elePz",              treeVars.elePz,            "elePz[nElectrons]/F");
      chain -> Branch("eleE",               treeVars.eleE,             "eleE[nElectrons]/F");
      chain -> Branch("eleEt",              treeVars.eleEt,            "eleEt[nElectrons]/F");
      chain -> Branch("eleEta",             treeVars.eleEta,           "eleEta[nElectrons]/F");
      chain -> Branch("elePhi",             treeVars.elePhi,           "elePhi[nElectrons]/F");
      
      chain -> Branch("eleId",              treeVars.eleId,            "eleId[nElectrons]/F");
      chain -> Branch("eleSigmaIEtaIEta",   treeVars.eleSigmaIEtaIEta, "eleSigmaIEtaIEta[nElectrons]/F");
      chain -> Branch("eleE1x5",            treeVars.eleE1x5,          "eleE1xE5[nElectrons]/F");
      chain -> Branch("eleE2x5",            treeVars.eleE2x5,          "eleE2xE5[nElectrons]/F");
      chain -> Branch("eleE5x5",            treeVars.eleE5x5,          "eleE5xE5[nElectrons]/F");
      chain -> Branch("eleSeedSwissCross",  treeVars.eleSeedSwissCross,"eleSeedSwisscross[nElectrons]/F");
      
      chain -> Branch("eleCharge",          treeVars.eleCharge,       "eleCharge[nElectrons]/I");
      chain -> Branch("eleTrkIso",          treeVars.eleTrkIso,       "eleTrkIso[nElectrons]/F");
      chain -> Branch("eleEcalIso",         treeVars.eleEcalIso,      "eleEcalIso[nElectrons]/F");
      chain -> Branch("eleHcalIsoD1",       treeVars.eleHcalIsoD1,    "eleHcalIsoD1[nElectrons]/F");
      chain -> Branch("eleHcalIsoD2",       treeVars.eleHcalIsoD2,    "eleHcalIsoD2[nElectrons]/F");
  
      chain -> Branch("genelePt",           treeVars.genelePt,        "genelePt[nElectrons]/F");
      chain -> Branch("geneleEta",          treeVars.geneleEta,       "geneleEta[nElectrons]/F");
      chain -> Branch("genelePhi",          treeVars.genelePhi,       "genelePhi[nElectrons]/F");

    }
  
 

  // MET VARIABLES  
  if(WenuTreeContent::metVariables)
    {  
      chain -> Branch("caloMet",    &treeVars.caloMet,   "caloMet/F");
      chain -> Branch("caloMex",    &treeVars.caloMex,   "caloMex/F"); 
      chain -> Branch("caloMey",    &treeVars.caloMey,   "caloMey/F");
      chain -> Branch("caloMetPhi", &treeVars.caloMetPhi,"caloMetPhi/F");
      chain -> Branch("caloSumEt",  &treeVars.caloSumEt, "caloSumEt/F");

      chain -> Branch("tcMet",      &treeVars.tcMet,     "tcMet/F");
      chain -> Branch("tcMex",      &treeVars.tcMex,     "tcMex/F"); 
      chain -> Branch("tcMey",      &treeVars.tcMey,     "tcMey/F");
      chain -> Branch("tcMetPhi",   &treeVars.tcMetPhi,  "tcMetPhi/F");
      chain -> Branch("tcSumEt",    &treeVars.tcSumEt,   "tcSumEt/F");

      chain -> Branch("pfMet",      &treeVars.pfMet,     "pfMet/F");
      chain -> Branch("pfMex",      &treeVars.pfMex,     "pfMex/F"); 
      chain -> Branch("pfMey",      &treeVars.pfMey,     "pfMey/F");
      chain -> Branch("pfMetPhi",   &treeVars.pfMetPhi,  "pfMetPhi/F");
      chain -> Branch("pfSumEt",    &treeVars.pfSumEt,   "pfSumEt/F");

    } // MET VARIABLES
  

  // JET VARIABLES  
  if(WenuTreeContent::jetVariables)
    {  
      
      chain -> Branch("nJets",        &treeVars.nJets,       "nJets/I");
      chain -> Branch("jetPx",        treeVars.jetPx,        "jetPx[nJets]/F");
      chain -> Branch("jetPy",        treeVars.jetPy,        "jetPy[nJets]/F");
      chain -> Branch("jetPz",        treeVars.jetPz,        "jetPz[nJets]/F");
      chain -> Branch("jetPt",        treeVars.jetPt,        "jetPt[nJets]/F");
      chain -> Branch("jetEta",       treeVars.jetEta,       "jetEta[nJets]/F");
      chain -> Branch("jetPhi",       treeVars.jetPhi,       "jetPhi[nJets]/F");
      chain -> Branch("jetBdisc",     treeVars.jetBdisc,     "jetBdisc[nJets]/F");
     
      chain -> Branch("genjetPt",     treeVars.genjetPt,     "genjetPt[nJets]/F");
      chain -> Branch("genjetEta",    treeVars.genjetEta,    "genjetEta[nJets]/F");
      chain -> Branch("genjetPhi",    treeVars.genjetPhi,    "genjetPhi[nJets]/F");
 
    } // JET VARIABLES
  


  // MUON VARIABLES  
  if(WenuTreeContent::muonVariables)
    {  
      
      chain -> Branch("nMuons",     &treeVars.nMuons,         "nMuons/I");
      chain -> Branch("muonPx",      treeVars.muonPx,         "muonPx[nMuons]/F");
      chain -> Branch("muonPy",      treeVars.muonPy,         "muonPy[nMuons]/F");
      chain -> Branch("muonPz",      treeVars.muonPz,         "muonPz[nMuons]/F");
      chain -> Branch("muonPt",      treeVars.muonPt,         "muonPt[nMuons]/F");
      chain -> Branch("muonEta",     treeVars.muonEta,        "muonEta[nMuons]/F");
      chain -> Branch("muonPhi",     treeVars.muonPhi,        "muonPhi[nMuons]/F");
            
    } // MUON VARIABLES



 

  //L1 VARIABLES
  if(WenuTreeContent::L1TrigVariables)
    {
      chain -> Branch("techL1Bit",     treeVars.techL1Bit,    "techL1Bit[64]/I");
      chain -> Branch("algoL1Bit",     treeVars.algoL1Bit,    "algoL1Bit[128]/I");
    }  //L1 VARIABLES
  



  //HLT VARIABLES
  if(WenuTreeContent::HLTrigVariables)
    {
      chain -> Branch("HLT_Ele15_LW_L1R", &treeVars.HLT_Ele15_LW_L1R, "HLT_Ele15_LW_L1R/I");
      chain -> Branch("HLT_Photon10_L1R", &treeVars.HLT_Photon10_L1R, "HLT_Photon10_L1R/I");
      chain -> Branch("HLT_Photon15_L1R", &treeVars.HLT_Photon15_L1R, "HLT_Photon15_L1R/I");
      chain -> Branch("HLT_Photon20_L1R", &treeVars.HLT_Photon20_L1R, "HLT_Photon20_L1R/I");

    }//HLT VARIABLES
  
}



void initializeBranches(TTree* chain, WenuTreeContent& treeVars)
{
  treeVars.BX = 0;
  treeVars.lumiId = 0;
  treeVars.runId = 0;
  treeVars.eventId = 0; 
  treeVars.eventNaiveId = 0; 
  
  
  // ELECTRONS VARIABLES  
  if(WenuTreeContent::electronVariables)
    {    
      for(int i = 0; i < MAXELECTRONS; ++i)
	{
	  treeVars.elePx[i] = -9999;
	  treeVars.elePy[i] = -9999;
	  treeVars.elePz[i] = -9999;
	  treeVars.eleE[i] = -9999;
	  treeVars.eleEt[i] = -9999;
	  treeVars.eleEta[i] = -9999;
	  treeVars.elePhi[i] = -9999;
	  treeVars.eleId[i] = -9999;
	  treeVars.eleSigmaIEtaIEta[i] = -9999;
	  treeVars.eleE1x5[i] = -9999;
	  treeVars.eleE2x5[i] = -9999;
	  treeVars.eleE5x5[i] = -9999;
	  treeVars.eleSeedSwissCross[i] = -9999;
 
	  treeVars.eleCharge[i] = -9999;
	  treeVars.eleTrkIso[i] = -9999;
	  treeVars.eleEcalIso[i] = -9999;
	  treeVars.eleHcalIsoD1[i] = -9999;
	  treeVars.eleHcalIsoD2[i] = -9999;


	  treeVars.genelePt[i] = -9999;
	  treeVars.geneleEta[i] = -9999;
	  treeVars.genelePhi[i] = -9999;
	}
      
      treeVars.nElectrons = 0;
    } // ELECTRONS VARIABLES
  



 // MET VARIABLES  
  if(WenuTreeContent::metVariables)
    {  
      treeVars.caloMet = -9999;
      treeVars.caloMex = -9999;
      treeVars.caloMey = -9999;
      treeVars.caloMetPhi = -9999;
      treeVars.caloSumEt  = -9999;

      treeVars.tcMet = -9999;
      treeVars.tcMex = -9999;
      treeVars.tcMey = -9999;
      treeVars.tcMetPhi = -9999;
      treeVars.tcSumEt  = -9999;

      treeVars.pfMet = -9999;
      treeVars.pfMex = -9999;
      treeVars.pfMey = -9999;
      treeVars.pfMetPhi = -9999;
      treeVars.pfSumEt  = -9999;

    } // MET VARIABLES
  

  // JET VARIABLES  
  if(WenuTreeContent::jetVariables)
    {  
      for(int i = 0; i < MAXJETS; ++i) {
	treeVars.jetPx[i] = -9999;
	treeVars.jetPy[i] = -9999;
	treeVars.jetPz[i] = -9999;
	treeVars.jetPt[i] = -9999;
	treeVars.jetEta[i] = -9999;
	treeVars.jetPhi[i] = -9999;
	treeVars.jetBdisc[i] = -9999;

	treeVars.genjetPt[i]  = -9999;
	treeVars.genjetEta[i] = -9999;
	treeVars.genjetPhi[i] = -9999;
      }
 
      treeVars.nJets = 0;
      
    } // JET VARIABLES
  


  // MUON VARIABLES  
  if(WenuTreeContent::muonVariables)
    {  
      for(int i = 0; i < MAXMUONS; ++i) {
	treeVars.muonPx[i] = -9999;
	treeVars.muonPy[i] = -9999;
	treeVars.muonPz[i] = -9999;
	treeVars.muonPt[i] = -9999;
	treeVars.muonEta[i] = -9999;
	treeVars.muonPhi[i] = -9999;
      }
 
      treeVars.nMuons = 0;
     
            
    } // MUON VARIABLES





  
  
  //L1 VARIABLES
  if(WenuTreeContent::L1TrigVariables)
    {
      for (int i = 0; i < 64 ; i++){
	treeVars.techL1Bit[i] = -9999;
      }
      
      for (int i = 0; i < 128 ; i++){
	treeVars.algoL1Bit[i] = -9999;
      }
    }  //L1 VARIABLES
  
  
  //HLT VARIABLES
  if(WenuTreeContent::HLTrigVariables)
    {
      treeVars.HLT_Ele15_LW_L1R = -9999;
      treeVars.HLT_Photon10_L1R = -9999; 
      treeVars.HLT_Photon15_L1R = -9999; 
      treeVars.HLT_Photon20_L1R = -9999; 
    }//HLT VARIABLES
  

}

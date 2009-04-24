#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFSelectVqq.h"

#define PI 3.141592654

using namespace vbfhww2l;



std::pair<bool, bool> IsMatching(reco::CaloJetCollection::const_iterator& caloJet1,
                                 reco::CaloJetCollection::const_iterator& caloJet2,
                                 reco::GenJetCollection::const_iterator& genJet1,
                                 reco::GenJetCollection::const_iterator& genJet2);






VBFSelectVqq::VBFSelectVqq(const edm::ParameterSet& iConfig):
 srcGenParticles_p          (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 srcIC5GenJets_p            (iConfig.getParameter<edm::InputTag>("srcIC5GenJets")),
 srcIC5CaloJets_p           (iConfig.getParameter<edm::InputTag>("srcIC5CaloJets")),
 fileName_p                 (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFSelectVqq"))),
 mass_p                     (iConfig.getParameter<std::string>("mass")),
 verbosity_p                (iConfig.getUntrackedParameter<bool>("verbosity", false)),
 eventsToPrint_p            (iConfig.getUntrackedParameter<int>("eventsToPrint", 0)),
 eventId_p(0),
 naiveId_p(0),
 nMode_p(5)
{}



VBFSelectVqq::~VBFSelectVqq()
{}






void VBFSelectVqq::beginJob(const edm::EventSetup& iSetup)
{
  if(verbosity_p)
    std::cerr << "*** VBFSelectVqq::beginJob ***" << std::endl;


  // Create File
  std::string path = "/afs/cern.ch/user/a/abenagli/scratch0/VBF/CMSSW_2_2_7/src/HiggsAnalysis/VBFHiggsToWWto2l2nu/test/";
  std::string outFileName = path+fileName_p+"_H"+mass_p+".root";
  outFile_p = new TFile(outFileName.c_str(), "RECREATE");



  // -----------------------------------------------------------------
  // define histograms
  // -----------------------------------------------------------------

  for(int i = 0; i < nMode_p; ++i)
  { 
    char histoName[50];

    sprintf(histoName, "selectionEfficiency_mode%d", i);
    selectionEfficiency.push_back(new TProfile(histoName, histoName, 2, 0., 2.));

    sprintf(histoName, "matchingPurity_mode%d", i);
    matchingPurity.push_back(new TProfile(histoName, histoName, 3, 0., 3.));
  }
}



void VBFSelectVqq::endJob()
{
  if(verbosity_p)
    std::cerr << "*** VBFSelectVqq::endJob ***" << std::endl;
  
  
  //Save histograms
  outFile_p -> cd();  

  for(int i = 0; i < nMode_p; ++i)
  {
    char dirName[50];
    sprintf(dirName, "mode%d", i); 
    outFile_p -> mkdir(dirName);
    outFile_p -> cd(dirName);  

    selectionEfficiency.at(i) -> Write();
    matchingPurity.at(i)      -> Write();

    outFile_p -> cd();  
  }
  
     
  outFile_p -> Close();
}






void VBFSelectVqq::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if(verbosity_p)
    std::cerr << "*** VBFSelectVqq::analyze ***" << std::endl;
  
  
  // set event id
  eventId_p = (iEvent.id()).event();
  ++naiveId_p;
  
  if( (naiveId_p%100 == 0) && (verbosity_p == true) )
    std::cout << ">>>>>> VBFSelectVqq   Analyze event number: " << naiveId_p << std::endl;






  // Get the collections
  edm::Handle<reco::GenParticleCollection> hGenParticles;
  iEvent.getByLabel(srcGenParticles_p, hGenParticles);
  if( !(hGenParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFSelectVqq::Warning: " << srcGenParticles_p << " not available" << std::endl;
    return;
  }

  edm::ESHandle<ParticleDataTable> hPdt;
  iSetup.getData(hPdt);

  edm::Handle<reco::GenJetCollection> hIC5GenJets;
  iEvent.getByLabel(srcIC5GenJets_p, hIC5GenJets);
  if( !(hIC5GenJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFSelectVqq::Warning: " << srcIC5GenJets_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::CaloJetCollection> hIC5CaloJets;
  iEvent.getByLabel(srcIC5CaloJets_p, hIC5CaloJets);
  if( !(hIC5CaloJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFSelectVqq::Warning: " << srcIC5CaloJets_p << " not available" << std::endl;
    return;
  }


  
    
  
  
  // -----------------------------------------------------------------
  // mc analysis
  // -----------------------------------------------------------------  

  MCAnalysis mcAnalysis(hGenParticles, hPdt, false);
  bool isMCAnalysisValid = mcAnalysis.isValid();
  if(!isMCAnalysisValid) return;
  //mcAnalysis.PrintHInfo(hPdt);


  // match genJets with quarks
  reco::GenJetCollection::const_iterator iC5GenJet_mcF_fromV2_matchIt    = hIC5GenJets -> end();
  reco::GenJetCollection::const_iterator iC5GenJet_mcFbar_fromV2_matchIt = hIC5GenJets -> end();

  float DRmax_mcF    = 0.3;
  float DRmax_mcFbar = 0.3;
 
  std::pair<bool, bool> isMatchGenJets_fromV2 = 
    mcAnalysis.GetMatchJets_fromV2(iC5GenJet_mcF_fromV2_matchIt,
                                   iC5GenJet_mcFbar_fromV2_matchIt,
                                   DRmax_mcF,
                                   DRmax_mcFbar,
                                   hIC5GenJets);
  
  if( (isMatchGenJets_fromV2.first == false) || (isMatchGenJets_fromV2.second == false) ) return;
  //std::cout << "iC5GenJet_mcF_fromV2: ";
  //PrintParticleInfo(&(*iC5GenJet_mcF_fromV2_matchIt), hPdt);  
  //std::cout << "iC5GenJet_mcFbar_fromV2: ";
  //PrintParticleInfo(&(*iC5GenJet_mcFbar_fromV2_matchIt), hPdt);  
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // select V -> qq   1) two highest pT jets
  // -----------------------------------------------------------------  
  
  int mode = 0;

  std::vector<reco::CaloJetCollection::const_iterator> iC5CaloJet_selectIt;
  
  // loop on jets
  for(reco::CaloJetCollection::const_iterator it = hIC5CaloJets -> begin();
      it != hIC5CaloJets -> end(); ++it)
  {
    iC5CaloJet_selectIt.push_back(it);
  }



  if(iC5CaloJet_selectIt.size() >= 2)
  {
    selectionEfficiency.at(mode) -> Fill(0., 0.);
    selectionEfficiency.at(mode) -> Fill(1., 1.);
    

    std::pair<bool, bool> matching = IsMatching(iC5CaloJet_selectIt.at(0), iC5CaloJet_selectIt.at(1),
                                                iC5GenJet_mcF_fromV2_matchIt, iC5GenJet_mcFbar_fromV2_matchIt);

    if( (matching.first == true) && (matching.second == true) )
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 1.);
    }
    else if( (matching.first == false) && (matching.second == false) )
    {
      matchingPurity.at(mode) -> Fill(0., 1.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
    else
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 1.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
  }
   
  else
  {
    selectionEfficiency.at(mode) -> Fill(0., 1.);
    selectionEfficiency.at(mode) -> Fill(1., 0.);
  }






  // -----------------------------------------------------------------
  // select V -> qq   2) two highest pT jets with pT > 20  
  // -----------------------------------------------------------------  
  
  mode = 1;

  iC5CaloJet_selectIt.clear();

  // loop on jets
  for(reco::CaloJetCollection::const_iterator it = hIC5CaloJets -> begin();
      it != hIC5CaloJets -> end(); ++it)
  {
    if(it -> pt() > 30.)
      iC5CaloJet_selectIt.push_back(it);
  }



  if(iC5CaloJet_selectIt.size() >= 2)
  {
    selectionEfficiency.at(mode) -> Fill(0., 0.);
    selectionEfficiency.at(mode) -> Fill(1., 1.);
    

    std::pair<bool, bool> matching = IsMatching(iC5CaloJet_selectIt.at(0), iC5CaloJet_selectIt.at(1),
                                                iC5GenJet_mcF_fromV2_matchIt, iC5GenJet_mcFbar_fromV2_matchIt);

    if( (matching.first == true) && (matching.second == true) )
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 1.);
    }
    else if( (matching.first == false) && (matching.second == false) )
    {
      matchingPurity.at(mode) -> Fill(0., 1.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
    else
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 1.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
  }
   
  else
  {
    selectionEfficiency.at(mode) -> Fill(0., 1.);
    selectionEfficiency.at(mode) -> Fill(1., 0.);
  }






  // -----------------------------------------------------------------
  // select V -> qq   3) two highest pT jets with pT > 20   |eta| < 3   mass > 5
  // -----------------------------------------------------------------  
  
  mode = 2;

  iC5CaloJet_selectIt.clear();

  // loop on jets
  for(reco::CaloJetCollection::const_iterator it = hIC5CaloJets -> begin();
      it != hIC5CaloJets -> end(); ++it)
  {
    if( (it -> pt() > 30.) && (fabs(it -> eta()) < 2.5) && (it -> mass() > 5.) )
      iC5CaloJet_selectIt.push_back(it);
  }



  if(iC5CaloJet_selectIt.size() >= 2)
  {
    selectionEfficiency.at(mode) -> Fill(0., 0.);
    selectionEfficiency.at(mode) -> Fill(1., 1.);
    

    std::pair<bool, bool> matching = IsMatching(iC5CaloJet_selectIt.at(0), iC5CaloJet_selectIt.at(1),
                                                iC5GenJet_mcF_fromV2_matchIt, iC5GenJet_mcFbar_fromV2_matchIt);

    if( (matching.first == true) && (matching.second == true) )
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 1.);
    }
    else if( (matching.first == false) && (matching.second == false) )
    {
      matchingPurity.at(mode) -> Fill(0., 1.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
    else
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 1.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
  }
   
  else
  {
    selectionEfficiency.at(mode) -> Fill(0., 1.);
    selectionEfficiency.at(mode) -> Fill(1., 0.);
  }






  
  // -----------------------------------------------------------------
  // select V -> qq   4) two smallest deta jets
  // -----------------------------------------------------------------  
  
  mode = 3;

  std::map<float, std::pair<reco::CaloJetCollection::const_iterator,
                            reco::CaloJetCollection::const_iterator> > iC5CaloJetCouple_selectIt;
  iC5CaloJetCouple_selectIt.clear();

  // loop on jets
  for(reco::CaloJetCollection::const_iterator it = hIC5CaloJets -> begin();
      it != hIC5CaloJets -> end(); ++it)
    for(reco::CaloJetCollection::const_iterator it2 = it+1;
        it2 != hIC5CaloJets -> end(); ++it2)
    {
      if( (it  -> pt() > 30.) && (fabs(it  -> eta()) < 2.5) && (it  -> mass() > 5.) &&
          (it2 -> pt() > 30.) && (fabs(it2 -> eta()) < 2.5) && (it2 -> mass() > 5.) )
      {
        float deta = fabs(it -> eta() - it2 -> eta());
        std::pair<reco::CaloJetCollection::const_iterator, reco::CaloJetCollection::const_iterator> dummy(it, it2);
        iC5CaloJetCouple_selectIt[deta] = dummy;
      }
    }
  

  
  if(iC5CaloJetCouple_selectIt.size() >= 1)
  {
    selectionEfficiency.at(mode) -> Fill(0., 0.);
    selectionEfficiency.at(mode) -> Fill(1., 1.);
    
    std::map<float, std::pair<reco::CaloJetCollection::const_iterator,
                              reco::CaloJetCollection::const_iterator> >::iterator mapIt; 
    mapIt = iC5CaloJetCouple_selectIt.begin(); 

    std::pair<bool, bool> matching = IsMatching((mapIt -> second).first, (mapIt -> second).second,
                                                iC5GenJet_mcF_fromV2_matchIt, iC5GenJet_mcFbar_fromV2_matchIt);

    if( (matching.first == true) && (matching.second == true) )
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 1.);
    }
    else if( (matching.first == false) && (matching.second == false) )
    {
      matchingPurity.at(mode) -> Fill(0., 1.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
    else
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 1.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
  }
   
  else
  {
    selectionEfficiency.at(mode) -> Fill(0., 1.);
    selectionEfficiency.at(mode) -> Fill(1., 0.);
  }






  // -----------------------------------------------------------------
  // select V -> qq   5) two closest-to-Z-inv-mass  jets
  // -----------------------------------------------------------------  
  
  mode = 4;

  iC5CaloJetCouple_selectIt.clear();

  // loop on jets
  for(reco::CaloJetCollection::const_iterator it = hIC5CaloJets -> begin();
      it != hIC5CaloJets -> end(); ++it)
    for(reco::CaloJetCollection::const_iterator it2 = it+1;
        it2 != hIC5CaloJets -> end(); ++it2)
    {
      if( (it  -> pt() > 30.) && (fabs(it  -> eta()) < 2.5) && (it  -> mass() > 5.) &&
          (it2 -> pt() > 30.) && (fabs(it2 -> eta()) < 2.5) && (it2 -> mass() > 5.) )
      {
        float invMass = fabs((it -> p4() + it2 -> p4()).mass() - 91.1876);
        std::pair<reco::CaloJetCollection::const_iterator, reco::CaloJetCollection::const_iterator> dummy(it, it2);
        iC5CaloJetCouple_selectIt[invMass] = dummy;
      }
    }
  

  
  if(iC5CaloJetCouple_selectIt.size() >= 1)
  {
    selectionEfficiency.at(mode) -> Fill(0., 0.);
    selectionEfficiency.at(mode) -> Fill(1., 1.);
    
    std::map<float, std::pair<reco::CaloJetCollection::const_iterator,
                              reco::CaloJetCollection::const_iterator> >::iterator mapIt; 
    mapIt = iC5CaloJetCouple_selectIt.begin(); 

    std::pair<bool, bool> matching = IsMatching((mapIt -> second).first, (mapIt -> second).second,
                                                iC5GenJet_mcF_fromV2_matchIt, iC5GenJet_mcFbar_fromV2_matchIt);

    if( (matching.first == true) && (matching.second == true) )
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 1.);
    }
    else if( (matching.first == false) && (matching.second == false) )
    {
      matchingPurity.at(mode) -> Fill(0., 1.);
      matchingPurity.at(mode) -> Fill(1., 0.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
    else
    {
      matchingPurity.at(mode) -> Fill(0., 0.);
      matchingPurity.at(mode) -> Fill(1., 1.);
      matchingPurity.at(mode) -> Fill(2., 0.);
    } 
  }
   
  else
  {
    selectionEfficiency.at(mode) -> Fill(0., 1.);
    selectionEfficiency.at(mode) -> Fill(1., 0.);
  }


  


  
}







std::pair<bool, bool> IsMatching(reco::CaloJetCollection::const_iterator& caloJet1,
                                 reco::CaloJetCollection::const_iterator& caloJet2,
                                 reco::GenJetCollection::const_iterator& genJet1,
                                 reco::GenJetCollection::const_iterator& genJet2)
{
  float dphi11 = deltaPhi(caloJet1 -> phi(), genJet1 -> phi());
  float deta11 = fabs(caloJet1 -> eta() - genJet1 -> eta());
  float DR11 = sqrt(dphi11*dphi11 + deta11*deta11);

  float dphi22 = deltaPhi(caloJet2 -> phi(), genJet2 -> phi());
  float deta22 = fabs(caloJet2 -> eta() - genJet2 -> eta());
  float DR22 = sqrt(dphi22*dphi22 + deta22*deta22);


  float dphi12 = deltaPhi(caloJet1 -> phi(), genJet2 -> phi());
  float deta12 = fabs(caloJet1 -> eta() - genJet2 -> eta());
  float DR12 = sqrt(dphi12*dphi12 + deta12*deta12);

  float dphi21 = deltaPhi(caloJet2 -> phi(), genJet1 -> phi());
  float deta21 = fabs(caloJet2 -> eta() - genJet1 -> eta());
  float DR21 = sqrt(dphi21*dphi21 + deta21*deta21);


  if( (DR12 + DR21) < (DR11 + DR22) )
  {
    reco::CaloJetCollection::const_iterator buffer = caloJet1;
    caloJet1 = caloJet2;
    caloJet2 = buffer;

    dphi11 = dphi12;
    deta11 = deta12;
    DR11 = DR12;

    dphi22 = dphi21;
    deta22 = deta21;
    DR22 = DR21;
  } 
  
  
  if( (DR11 < 0.3) && (DR22 < 0.3) )
  {
    std::pair<bool, bool> result(true, true);
    return result;
  }

  else if( (DR11 < 0.3) && (DR22 > 0.3) )
  {
    std::pair<bool, bool> result(true, false);
    return result;
  }

  else if( (DR11 > 0.3) && (DR22 < 0.3) )
  {
    std::pair<bool, bool> result(false, true);
    return result;
  }
  
  else
  {
    std::pair<bool, bool> result(false, false);
    return result;
  }
}



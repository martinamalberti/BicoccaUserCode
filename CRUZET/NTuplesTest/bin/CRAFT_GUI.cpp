#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <string>
#include <boost/foreach.hpp>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TCanvas.h"

#define PHI_MIN -3.1416
#define PHI_MAX +3.1416
#define PHI_BIN 360

#define ETA_MIN -1.5
#define ETA_MAX +1.5
#define ETA_BIN 170

#define X_BIN 1600
#define X_MIN -800
#define X_MAX +800

#define Y_BIN 1600
#define Y_MIN -800
#define Y_MAX +800

#define P_BIN 500
#define P_MIN 0
#define P_MAX 1000



int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  // edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("Selections") ;
  edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;

  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;


  // input files
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ; ++listIt)
  {
    std::cout << *listIt << " " << std::endl ;
    chain->Add (listIt->c_str ()) ;
  }

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    
  // input files



  // output distributions
  TGraph XYaxis ;
  TGraph muXY_inner ;
  TGraph muXY_outer ;
  TGraph muXY_entry ;
  TGraph muXY_exit ;
  TGraph muXY_closest ;
  TGraph muXY_neckLace ;
  TGraph scXY ;

  TGraph ZRaxis ;
  TGraph muZR_inner ;
  TGraph muZR_outer ;
  TGraph muZR_entry ;
  TGraph muZR_exit ;
  TGraph muZR_closest ;
  TGraph muZR_neckLace ;
  TGraph scZR ;

  TApplication *theApp = new TApplication ("app", &argc, argv);
  // output distributions

 loop:
  int entry;
  std::cout << "Insert event number: ";
  std::cin >> entry;
  {
    chain->GetEntry (entry) ;

    int nPoints = 0;
    for (int MUindex = 0; MUindex < treeVars.nRecoMuons; ++MUindex)
    {
      std::cout << "**************** MU ****************" << std::endl ;
      muXY_inner.SetPoint (MUindex, treeVars.muonOutTkInnerHitX[MUindex], treeVars.muonOutTkInnerHitY[MUindex]) ;
      muXY_outer.SetPoint (MUindex, treeVars.muonOutTkOuterHitX[MUindex], treeVars.muonOutTkOuterHitY[MUindex]) ;

      if (treeVars.muonOutTkInnerHitY[MUindex] >= 0.)
      muZR_inner.SetPoint (MUindex, treeVars.muonOutTkInnerHitZ[MUindex], 
                                    sqrt( treeVars.muonOutTkInnerHitX[MUindex]*treeVars.muonOutTkInnerHitX[MUindex] + 
                                          treeVars.muonOutTkInnerHitY[MUindex]*treeVars.muonOutTkInnerHitY[MUindex] ) );
      else
      muZR_inner.SetPoint (MUindex, treeVars.muonOutTkInnerHitZ[MUindex], 
                                    -sqrt( treeVars.muonOutTkInnerHitX[MUindex]*treeVars.muonOutTkInnerHitX[MUindex] + 
                                           treeVars.muonOutTkInnerHitY[MUindex]*treeVars.muonOutTkInnerHitY[MUindex] ) );

      if (treeVars.muonOutTkOuterHitY[MUindex] >= 0.) 
      muZR_outer.SetPoint (MUindex, treeVars.muonOutTkOuterHitZ[MUindex],
                                    sqrt( treeVars.muonOutTkOuterHitX[MUindex]*treeVars.muonOutTkOuterHitX[MUindex] + 
                                          treeVars.muonOutTkOuterHitY[MUindex]*treeVars.muonOutTkOuterHitY[MUindex] ) );
      else
      muZR_outer.SetPoint (MUindex, treeVars.muonOutTkOuterHitZ[MUindex],
                                    -sqrt( treeVars.muonOutTkOuterHitX[MUindex]*treeVars.muonOutTkOuterHitX[MUindex] + 
                                           treeVars.muonOutTkOuterHitY[MUindex]*treeVars.muonOutTkOuterHitY[MUindex] ) ); 


      std::cout << "OutTk::InnerHit   " << treeVars.muonOutTkInnerHitX[MUindex] << "   "
                                        << treeVars.muonOutTkInnerHitY[MUindex] << std::endl ;
      std::cout << "OutTk::OuterHit   " << treeVars.muonOutTkOuterHitX[MUindex] << "   "
                                        << treeVars.muonOutTkOuterHitY[MUindex] << std::endl ;
      
      std::cout << "\n" << std::endl ;






      muXY_inner.SetPoint (MUindex+1, treeVars.muonInnTkInnerHitX[MUindex], treeVars.muonInnTkInnerHitY[MUindex]) ;
      muXY_outer.SetPoint (MUindex+1, treeVars.muonInnTkOuterHitX[MUindex], treeVars.muonInnTkOuterHitY[MUindex]) ;
      
      if (treeVars.muonInnTkInnerHitY[MUindex] >= 0.)
      muZR_inner.SetPoint (MUindex+1, treeVars.muonInnTkInnerHitZ[MUindex],
                                      sqrt( treeVars.muonInnTkInnerHitX[MUindex]*treeVars.muonInnTkInnerHitX[MUindex] + 
					    treeVars.muonInnTkInnerHitY[MUindex]*treeVars.muonInnTkInnerHitY[MUindex] ) ) ;
      else
      muZR_inner.SetPoint (MUindex+1, treeVars.muonInnTkInnerHitZ[MUindex],
                                      -sqrt( treeVars.muonInnTkInnerHitX[MUindex]*treeVars.muonInnTkInnerHitX[MUindex] + 
					     treeVars.muonInnTkInnerHitY[MUindex]*treeVars.muonInnTkInnerHitY[MUindex] ) ) ;
      if (treeVars.muonInnTkOuterHitY[MUindex] >= 0.)
      muZR_outer.SetPoint (MUindex+1, treeVars.muonInnTkOuterHitZ[MUindex], 
                                      sqrt( treeVars.muonInnTkOuterHitX[MUindex]*treeVars.muonInnTkOuterHitX[MUindex] + 
					    treeVars.muonInnTkOuterHitY[MUindex]*treeVars.muonInnTkOuterHitY[MUindex] ) ) ;
      else
      muZR_outer.SetPoint (MUindex+1, treeVars.muonInnTkOuterHitZ[MUindex], 
                                      -sqrt( treeVars.muonInnTkOuterHitX[MUindex]*treeVars.muonInnTkOuterHitX[MUindex] + 
					     treeVars.muonInnTkOuterHitY[MUindex]*treeVars.muonInnTkOuterHitY[MUindex] ) ) ;

      std::cout << "InnTk::InnerHit   " << treeVars.muonInnTkInnerHitX[MUindex] << "   "
                                        << treeVars.muonInnTkInnerHitY[MUindex] << std::endl ;
      std::cout << "InnTk::OuterHit   " << treeVars.muonInnTkOuterHitX[MUindex] << "   "
                                        << treeVars.muonInnTkOuterHitY[MUindex] << std::endl ;
      
      std::cout << "\n" << std::endl ;





      muXY_entry.SetPoint (MUindex, treeVars.muonEntryPointInEcalX[MUindex], treeVars.muonEntryPointInEcalY[MUindex]) ;
      muXY_exit.SetPoint (MUindex, treeVars.muonExitPointInEcalX[MUindex], treeVars.muonExitPointInEcalY[MUindex]) ;
      






      muXY_closest.SetPoint (MUindex, treeVars.muonX[MUindex], treeVars.muonY[MUindex]) ;

      if (treeVars.muonY[MUindex] >= 0.)
      muZR_closest.SetPoint (MUindex, treeVars.muonZ[MUindex], 
			              sqrt( treeVars.muonX[MUindex]*treeVars.muonX[MUindex] + 
					    treeVars.muonY[MUindex]*treeVars.muonY[MUindex] ) ) ;
      else
      muZR_closest.SetPoint (MUindex, treeVars.muonZ[MUindex], 
			              -sqrt( treeVars.muonX[MUindex]*treeVars.muonX[MUindex] + 
					     treeVars.muonY[MUindex]*treeVars.muonY[MUindex] ) ) ;

      std::cout << "ClosestApproax   " << treeVars.muonX[MUindex] << "   "
                                       << treeVars.muonY[MUindex] << std::endl ;
      



      

      std::cout << "NeckLace size = " << treeVars.muonNeckLace[MUindex] << std::endl;
      for (int i = 0; i < treeVars.muonNeckLace[MUindex]; ++i)
      {
        //if (treeVars.muonNeckLaceInsideXtal[MUindex][i] == 1)
        {
          muXY_neckLace.SetPoint (nPoints, treeVars.muonNeckLaceX[MUindex][i], treeVars.muonNeckLaceY[MUindex][i]) ;

          if (treeVars.muonNeckLaceY[MUindex][i] >= 0.)
          muZR_neckLace.SetPoint (nPoints, treeVars.muonNeckLaceZ[MUindex][i],
                                  sqrt( treeVars.muonNeckLaceX[MUindex][i]*treeVars.muonNeckLaceX[MUindex][i] + 
	  			        treeVars.muonNeckLaceY[MUindex][i]*treeVars.muonNeckLaceY[MUindex][i] ) ) ;
	  else 
          muZR_neckLace.SetPoint (nPoints, treeVars.muonNeckLaceZ[MUindex][i],
                                  -sqrt( treeVars.muonNeckLaceX[MUindex][i]*treeVars.muonNeckLaceX[MUindex][i] + 
	  			         treeVars.muonNeckLaceY[MUindex][i]*treeVars.muonNeckLaceY[MUindex][i] ) ) ;

//          std::cout << "NeckLace::   " << treeVars.muonNeckLaceX[MUindex][i] << "   "
//                                       << treeVars.muonNeckLaceInsideXtal[MUindex][i] << std::endl ;

          ++nPoints;
        } 
      }
        

  }
    
     
    
    for (int SCindex = 0; SCindex < treeVars.nSuperClusters; ++SCindex)
    {
      scXY.SetPoint (SCindex, treeVars.superClusterX[SCindex], treeVars.superClusterY[SCindex]) ;
    
      if (treeVars.superClusterY[SCindex] >= 0.)
      scZR.SetPoint (SCindex, treeVars.superClusterZ[SCindex], 
                              sqrt( treeVars.superClusterX[SCindex]*treeVars.superClusterX[SCindex] + 
				    treeVars.superClusterY[SCindex]*treeVars.superClusterY[SCindex] ) ) ;
      else
      scZR.SetPoint (SCindex, treeVars.superClusterZ[SCindex], 
                              -sqrt( treeVars.superClusterX[SCindex]*treeVars.superClusterX[SCindex] + 
				     treeVars.superClusterY[SCindex]*treeVars.superClusterY[SCindex] ) ) ;
    }

  }
  
  
  TF1 barrelUp ("barrelUp", "sqrt(129*129-x*x)", -129.999, 129.999) ;
  TF1 barrelDown ("barrelDown", "-sqrt(129*129-x*x)", -129.999, 129.999) ;
  
  TF1 backUp ("barrelUp", "sqrt(152*152-x*x)", -151.999, 151.999) ;
  TF1 backDown ("barrelDown", "-sqrt(152*152-x*x)", -151.999, 151.999) ;
  
  TF1 barrelInnerUp ("barrelInnerUp", "129", -350., 350.) ;
  TF1 barrelInnerDown ("barrelInnerDown", "-129", -350., 350.) ;

  TF1 barrelOuterUp ("barrelInnerUp", "129 + 23 / sqrt(1+x*x/129/129)", -350., 350.) ;
  TF1 barrelOuterDown ("barrelInnerDown", "-129 - 23 / sqrt(1+x*x/129/129)", -350., 350.) ;



  TCanvas* c1 = new TCanvas ("XY") ;

  XYaxis.SetPoint (0, -800, -800); 
  XYaxis.SetPoint (1, 800, 800); 
  XYaxis.Draw ("AP"); 

  muXY_inner.SetMarkerStyle (8) ;
  muXY_inner.SetMarkerColor (kGreen) ;
  muXY_inner.Draw ("P,same") ;
  muXY_outer.SetMarkerStyle (8) ;
  muXY_outer.SetMarkerColor (kBlue) ;
  muXY_outer.Draw ("P,same") ;
  muXY_entry.SetMarkerStyle (7) ;
  muXY_entry.SetMarkerColor (kGreen) ;
  muXY_entry.Draw ("P,same") ;
  muXY_exit.SetMarkerStyle (7) ;
  muXY_exit.SetMarkerColor (kBlue) ;
  muXY_exit.Draw ("P,same") ;
  muXY_closest.SetMarkerStyle (8) ;
  muXY_closest.SetMarkerColor (kBlack) ;
  muXY_closest.Draw ("P,same") ;
  muXY_neckLace.SetMarkerStyle (6) ;
  muXY_neckLace.SetMarkerColor (kBlack) ;
  muXY_neckLace.Draw ("P,same") ;
  
  scXY.SetMarkerStyle (8) ;
  scXY.SetMarkerColor (kMagenta) ;
  scXY.Draw ("P,same") ;
  
  barrelUp.SetLineWidth (1) ;
  barrelUp.Draw ("same") ;
  barrelDown.SetLineWidth (1) ;
  barrelDown.Draw ("same") ;
  
  backUp.SetLineWidth (1) ;
  backUp.Draw ("same") ;
  backDown.SetLineWidth (1) ;
  backDown.Draw ("same") ;



  TCanvas* c2 = new TCanvas ("ZR") ;

  ZRaxis.SetPoint (0, -800, -800); 
  ZRaxis.SetPoint (1, 800, 800); 
  ZRaxis.Draw ("AP"); 

  muZR_inner.SetMarkerStyle (8) ;
  muZR_inner.SetMarkerColor (kGreen) ;
  muZR_inner.Draw ("P,same") ;
  muZR_outer.SetMarkerStyle (8) ;
  muZR_outer.SetMarkerColor (kBlue) ;
  muZR_outer.Draw ("P,same") ;
  muZR_closest.SetMarkerStyle (8) ;
  muZR_closest.SetMarkerColor (kBlack) ;
  muZR_closest.Draw ("P,same") ;
  muZR_neckLace.SetMarkerStyle (6) ;
  muZR_neckLace.SetMarkerColor (kBlack) ;
  muZR_neckLace.Draw ("P,same") ;
  
  scZR.SetMarkerStyle (8) ;
  scZR.SetMarkerColor (kMagenta) ;
  scZR.Draw ("P,same") ;
  
  barrelInnerUp.SetLineWidth (1) ;
  barrelInnerUp.Draw ("same") ;
  barrelInnerDown.SetLineWidth (1) ;
  barrelInnerDown.Draw ("same") ;

  barrelOuterUp.SetLineWidth (1) ;
  barrelOuterUp.Draw ("same") ;
  barrelOuterDown.SetLineWidth (1) ;
  barrelOuterDown.Draw ("same") ;
  


  theApp -> Run ();
  
  goto loop;
  
  return 0 ;
}



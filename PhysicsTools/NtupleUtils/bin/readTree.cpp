#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TMath.h"
//#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include <vector>

#include "TRandom.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TBranch.h"
#include "TBranchElement.h"

#include <map>
#include <vector>
#include <string>

/*
source ~/Desktop/setupRoot5.25.sh
rootcint -f dict.cpp -c -p Math/LorentzVector.h LinkDef.h
c++ -o readTree `root-config --cflags --glibs` -lGenVector readTree.cpp dict.cpp 

*/

/*

mancano i vector 3D, manca altro?
*/

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main ()
{
  //PG open the test file
  TFile f ("./tree.root") ;
  TTree * tr = (TTree*) f.Get ("al") ;

  //PG read the tree structure and setup variables and branches
  std::map <std::string, std::vector<double> * > Dvectors ;
  std::map <std::string, std::vector<ROOT::Math::XYZTVector> * > LVvectors ;

  TObjArray * br_list = tr->GetListOfBranches () ;
  TIter br_it (br_list) ;
  TBranch * iBranch ; 
  
  //PG loop over branches
  while ((iBranch = (TBranch *) br_it.Next ())) 
    {
      TBranchElement* bre = (TBranchElement*) iBranch ;
      std::string bname = bre->GetClassName () ;      
      if (bname.find ("Lorentz") != std::string::npos)
        {
          std::cout << "LV | setting " << bre->GetName () 
                    << " for type : " << bre->GetClassName () << "\n" ;
          std::vector<ROOT::Math::XYZTVector> * dummy = new std::vector<ROOT::Math::XYZTVector> ;
          LVvectors[bre->GetName ()] = dummy ;
          tr->SetBranchAddress (bre->GetName (), &LVvectors[bre->GetName ()]) ;
        }
      else
        {
          std::cout << "DV | setting " << bre->GetName () 
                    << " for type : " << bre->GetClassName () << "\n" ;
          std::vector<double> * dummy = new std::vector<double> ;
          Dvectors[bre->GetName ()] = dummy ;
          tr->SetBranchAddress (bre->GetName (), &Dvectors[bre->GetName ()]) ;
        }
    } //PG loop over branches

  std::cout << " --> " << (LVvectors.size () + Dvectors.size ()) << " branches read\n" ;

  //PG loop over the events
  for (int iEvent = 0 ; iEvent < tr->GetEntries () ; ++iEvent)
    {
      tr->GetEntry (iEvent) ;
      
      for (std::map <std::string, std::vector<double> * >::const_iterator iMap = Dvectors.begin () ;
           iMap != Dvectors.end () ;
           ++iMap)
        {
          std::cout << "DV | " << iMap->first << " " << iMap->second->size () << " : " ;
          for (int i = 0 ; i < iMap->second->size () ; ++i)
            std::cout << iMap->second->at (i) << " " ;
          std::cout << "\n" ;
        } 

      for (std::map <std::string, std::vector<ROOT::Math::XYZTVector> * >::const_iterator iMap = LVvectors.begin () ;
           iMap != LVvectors.end () ;
           ++iMap)
        {
          std::cout << "LV | " << iMap->first << " " << iMap->second->size () << " : " ;
          for (int i = 0 ; i < iMap->second->size () ; ++i)
            std::cout << iMap->second->at (i) << " " ;
          std::cout << "\n" ;
        } 
    } //PG loop over the events
  


  return 0 ;

}
  

// $Id: VBFUtils.cc,v 1.2 2009/03/03 16:18:58 amassiro Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_OneShot.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_OneShot.cxx"

//PG NAMESPACE DEFINITION
namespace vbfhww2l
{


 void 
   setMomentum (TLorentzVector & myvector, 
                const reco::Candidate & gen)
 {
  myvector.SetPx (gen.px ()) ;
  myvector.SetPy (gen.py ()) ;
  myvector.SetPz (gen.pz ()) ;
  myvector.SetE (gen.energy ()) ;
 }


// --------------------------------------------------------------------


 std::pair<VBFjetIt,VBFjetIt>    
   findTagJets (VBFjetIt begin, VBFjetIt end,
                double jetPtMin, double jetEtaMax,
                double m_jetDEtaMin, double m_jetMjjMin) 
   {

    std::pair<VBFjetIt,VBFjetIt> tagJets (begin,begin) ;
    double maxInvMass = 0. ;

  //PG find the tagging jets

  //PG first loop over jets
    for (VBFjetIt firstJet = begin ; 
         firstJet != end ; 
         ++firstJet ) 
    {
     if (firstJet->pt () < jetPtMin || 
         fabs (firstJet->eta ()) > jetEtaMax) continue ;
     math::XYZTLorentzVector firstLV = firstJet->p4 () ;
      //PG second loop over jets
     for (VBFjetIt secondJet = firstJet + 1 ; 
          secondJet != end ; 
          ++secondJet ) 
     {
      if (secondJet->pt () < jetPtMin || 
          fabs (secondJet->eta ()) > jetEtaMax) continue ;
      //if (firstJet->eta ()*secondJet->eta () > 0) continue ; // only tags in opposite emispheres are considered
      math::XYZTLorentzVector sumLV = secondJet->p4 () + firstLV ;
      if (sumLV.M () > maxInvMass)
      {
       
             //---- Check if DEta and Mjj selections are fullfilled
       float etaA = firstJet->eta();
       float etaB= secondJet->eta();
       float DEta = fabs(etaA - etaB);
       float Mjj = (firstJet->p4() + secondJet->p4()).mass();
       if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin) continue;
       
       maxInvMass = sumLV.M () ;
       tagJets.first = firstJet ;
       tagJets.second = secondJet ;
      }
     } //PG second loop over jets
    } //PG first loop over jets

    return tagJets ;
   }


// --------------------------------------------------------------------


   std::pair<VBFjetIt,VBFjetIt>
     findMaxPtJetsPair (VBFjetIt begin, VBFjetIt end,
                        double jetPtMin, double jetEtaMax,
                        double m_jetDEtaMin, double m_jetMjjMin)
     {
      std::pair<VBFjetIt,VBFjetIt> tagJets (begin, begin) ; 

// // //   double ptMax1 = 0;
// // //   double ptMax2 = 0;
      // // // 
// // //   VBFjetIt myJet1;
// // //   VBFjetIt myJet2;
// // //   for (VBFjetIt Jet = begin ;
// // //        Jet != end ;
// // //        ++Jet)
// // //     {
// // //       if (Jet->p4().Pt() > ptMax1)
// // //    {
// // //      myJet1 = Jet;
// // //      myJet2 = myJet1;
// // //      ptMax2 = ptMax1;
// // //      ptMax1 = Jet->p4().Pt() ;
      // // // 
// // //         } 
// // //       // else if ((Jet->p4().Pt() > ptMax2) && (myJet1->p4().Eta() *Jet->p4().Eta() < 0))
// // //       else if (Jet->p4().Pt() > ptMax2) 
// // //    {
// // //          myJet2 = Jet;
// // //          ptMax2 = Jet->p4().Pt() ;
// // //    }
// // //     }
      // // // 
// // //   tagJets.first = myJet1;
// // //   tagJets.second = myJet2 ;
      // // // 
// // //   return tagJets ;

  
  

  
      double ptMax1 = 0;
      double ptMax2 = 0;
  
  //---- initialization at "begin" ----
      VBFjetIt myJet1 = begin;
      VBFjetIt myJet2 = begin;
  
      for (VBFjetIt Jet = begin; Jet != end; ++Jet){
       //---- pt min threshold ----
       if (Jet->p4().Pt()<jetPtMin) continue;
       //---- Eta max threshold ----
       if (Jet->p4().Eta()>jetEtaMax) continue;
       //---- if it's not the first jet, I may check if DEta and Mjj selections are fullfilled
       //---- Check between current (1) and the new jet
       if (Jet!=myJet1){
        float etaA = myJet1->p4().Eta();
        float etaB= Jet->p4().Eta();
        float DEta = fabs(etaA - etaB);
        float Mjj = (myJet1->p4() + Jet->p4()).mass();
        if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin) continue;
       }
       //---- first threshold: (2) ----
       if (Jet->p4().Pt() > ptMax2)
       {
    //---- second threshold: (1) ----
        if (Jet->p4().Pt() > ptMax1){
     //---- if both (1) and (2) threshold succeded than (1) becomes (2)
         myJet2 = myJet1;
         ptMax2 = ptMax1;
     //----  and the new one is (1)
         myJet1 = Jet;
         ptMax1 = myJet1->p4().Pt();
        }
        else {
         myJet2 = Jet;
         ptMax2 = Jet->p4().Pt();    
        }
       }
      }
  
      tagJets.first = myJet1;
      tagJets.second = myJet2 ;
  
      return tagJets ;
   
     }


// --------------------------------------------------------------------


     std::pair<VBFjetIt,VBFjetIt>
       findMaxNNJetsPair_PtOrdered (VBFjetIt begin, VBFjetIt end,
                          double jetPtMin, double jetEtaMax,
                          double m_jetDEtaMin, double m_jetMjjMin)
       {
        std::pair<VBFjetIt,VBFjetIt> tagJets (begin,begin) ;
        double maxNN = -10. ;
        
        double pair_DEta;
        double pair_Mjj;
        double pair_pt1;
        double pair_pt2;
        int pair_numJet;
        double pair_SumPtOthers;
        double pair_MjjOthers;
        
        NNfunction myNN;
        
  //PG first loop over jets
        for (VBFjetIt firstJet = begin ; 
             firstJet != end ; 
             ++firstJet ) 
        {
         if (firstJet->pt () < jetPtMin || 
             fabs (firstJet->eta ()) > jetEtaMax) continue ;
         math::XYZTLorentzVector firstLV = firstJet->p4 () ;
      //PG second loop over jets
         for (VBFjetIt secondJet = firstJet + 1 ; 
              secondJet != end ; 
              ++secondJet ) 
         {
          if (secondJet->pt () < jetPtMin || 
              fabs (secondJet->eta ()) > jetEtaMax) continue ;

          math::XYZTLorentzVector secondLV = secondJet->p4 () ;
                    
          pair_DEta = fabs(firstLV.Eta() - secondLV.Eta());
          math::XYZTLorentzVector sumPair = firstLV + secondLV;
          pair_Mjj = sumPair.mass();
          pair_pt1 = firstLV.Pt();
          pair_pt2 = secondLV.Pt();
          math::XYZTLorentzVector sumOthers(0,0,0,0);
          for (VBFjetIt otherJet = begin ; 
               otherJet != end ; 
               ++otherJet ) //AM loop over other jets
          {
           if (otherJet!=firstJet && otherJet!=secondJet){
            if (otherJet->pt () < jetPtMin || 
                fabs (otherJet->eta ()) > jetEtaMax) continue ;
            math::XYZTLorentzVector addOther = otherJet->p4 () ;
            sumOthers = sumOthers + addOther;
           }
          }//AM end loop over other jets
          
          pair_SumPtOthers = sumOthers.Pt();
          pair_MjjOthers = sumOthers.mass();     
     
          double flagNN = myNN.Value(0,pair_DEta,pair_Mjj,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);

          if (flagNN > maxNN)
          {
       //---- Check if DEta and Mjj selections are fullfilled
           float etaA = firstJet->eta();
           float etaB= secondJet->eta();
           float DEta = fabs(etaA - etaB);
           float Mjj = (firstJet->p4() + secondJet->p4()).mass();
           if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin) continue;
       //---- pt ordering of tagged jets
           if (pair_pt1 > pair_pt2) {
            tagJets.first = firstJet ;
            tagJets.second = secondJet ;
           }
           else {
            tagJets.first = secondJet ;
            tagJets.second = firstJet ;            
           }
           maxNN = flagNN ;
          } 
         } //PG second loop over jets
        } //PG first loop over jets

        return tagJets ;
   
       }


// --------------------------------------------------------------------------------



     double 
        deltaPhi (double phi1, double phi2)
     {

      double deltaphi=fabs(phi1-phi2);
      if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
      if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
      return deltaphi;
     }


} //PG namespace vbfhww2l


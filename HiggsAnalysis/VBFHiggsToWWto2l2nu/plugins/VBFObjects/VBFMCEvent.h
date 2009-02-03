#ifndef __VBFMCEVENT_H__
#define __VBFMCEVENT_H__


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFDefs.h"


namespace vbfhww2l{

  class VBFMCEvent{

  public:
    VBFMCEvent();
    ~VBFMCEvent();
    
    void Reset();
    
    LorentzVector p4Parton1_gen; 
    LorentzVector p4Parton2_gen; 
    LorentzVector p4Higgs_gen;
    LorentzVector p4W1_gen;
    LorentzVector p4W2_gen;      
    LorentzVector p4Lep1_gen;
    LorentzVector p4Lep2_gen;
    LorentzVector p4Nu1_gen; 
    LorentzVector p4Nu2_gen; 
       
    int QLep1_gen;                 // charge of the lepton
    int QLep2_gen;
    int pdgId_Lep1_gen;
    int pdgId_Lep2_gen;

    int procId;                    // MC process id

    Point mcPrimV;                 // MC primary vertex

    int DecChan;
    // =0 for 2e
    // =1 for 2mu
    // =2 for e + mu
    // =3 for mu + e
    // =4 tau(s)

    double weight;
    
//    double partx[2];                       
//    // momentum fraction of initial partons 
//    int    ptype[2];                        
//    // parton type (u,d,s,...,g) of initial
//    // partons
//    double Qscale;                          
//    // factorisation scale mu=Q (GeV)  
//    double PDFval[60];                      
//    // PDF values: f_p(x_p)*f_pbar(x_pbar) 
//    // for different PDF's
//    
      }; 
}
#endif

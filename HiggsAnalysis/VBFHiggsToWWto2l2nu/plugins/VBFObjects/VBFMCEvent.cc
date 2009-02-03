#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFMCEvent.h"


using namespace vbfhww2l;


VBFMCEvent::VBFMCEvent() :
  QLep1_gen(0), QLep2_gen(0),
  pdgId_Lep1_gen(0), pdgId_Lep2_gen(0),
  procId(0), 
  DecChan(-1),
  weight(0)
{
  p4Parton1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 
  p4Parton2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 
  p4Higgs_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4W1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4W2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);      
  p4Lep1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4Lep2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4Nu1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 
  p4Nu2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 

  mcPrimV.SetCoordinates(0.0, 0.0, 0.0);
}


VBFMCEvent::~VBFMCEvent(){}


void VBFMCEvent::Reset(){

  p4Parton1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 
  p4Parton2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 
  p4Higgs_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4W1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4W2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);      
  p4Lep1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4Lep2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  p4Nu1_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 
  p4Nu2_gen.SetCoordinates(0.0, 0.0, 0.0, 0.0); 

  mcPrimV.SetCoordinates(0.0, 0.0, 0.0);

  QLep1_gen =0;
  QLep2_gen =0;
  pdgId_Lep1_gen =0;
  pdgId_Lep2_gen =0;
  procId  =0; 
  DecChan =-1;
  weight  =0;
}

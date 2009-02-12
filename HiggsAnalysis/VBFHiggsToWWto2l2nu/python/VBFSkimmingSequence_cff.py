import FWCore.ParameterSet.Config as cms

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFLeptonsMinNumFilter_cfi import *

VBFSkimmingSequence = cms.Sequence(
     leptonsMinNumber 
    )
    

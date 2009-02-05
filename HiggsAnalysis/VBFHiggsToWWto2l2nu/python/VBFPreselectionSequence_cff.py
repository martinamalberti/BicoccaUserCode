import FWCore.ParameterSet.Config as cms

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonIsolator_cfi import *

higgsToWW2LeptonsPreselectionSequence = cms.Sequence(
    isolatedMuons
    )
    

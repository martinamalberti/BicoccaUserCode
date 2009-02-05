import FWCore.ParameterSet.Config as cms

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMetCorrector_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonSelector_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonSelectorRef_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonIsolator_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonIsolatorRef_cfi import *

VBFPreselectionSequence = cms.Sequence(
    correctedMet *
    selectedMuons *
    selectedMuonsRef *
    isolatedMuons *
    isolatedMuonsRef 
    )
    

import FWCore.ParameterSet.Config as cms

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMetCorrector_cfi import *

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonSelector_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonSelectorRef_cfi import *

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonIsolator_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMuonIsolatorRef_cfi import *

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIDSequence_cff import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIDSelector_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIDSelectorRef_cfi import *

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIsolationSequence_cff import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleHETIsolator_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleHETIsolatorRef_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleHETIsolator_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleHETIsolatorRef_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleHIsolator_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleHIsolatorRef_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleEIsolator_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleEIsolatorRef_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleTIsolator_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEleTIsolatorRef_cfi import *

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleaner_cfi import *
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerRef_cfi import *

VBFPreselectionSequence = cms.Sequence(
    correctedMet *
   
    cleanedJets * 
    cleanedJetsRef * 
     
    selectedMuons *
    selectedMuonsRef *
    isolatedMuons *
    isolatedMuonsRef *
    VBFElectronIdSequence *
    selectedIDElectrons *
    selectedIDElectronsRef *
    
    VBFElectronIsolationSequence *
    isolatedHETElectrons * 
    isolatedHETElectronsRef *
    isolatedHElectrons * 
    isolatedHElectronsRef *
    isolatedEElectrons * 
    isolatedEElectronsRef *
    isolatedTElectrons * 
    isolatedTElectronsRef 
    
    )
    

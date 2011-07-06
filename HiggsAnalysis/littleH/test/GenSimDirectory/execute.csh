#!/bin/tcsh

source /afs/cern.ch/cms/sw/cmsset_default.csh

cd /afs/cern.ch/user/m/maborgia/scratch0/lighta_LHCEra/CMSSW_4_2_2_patch1/src/HiggsAnalysis/littleH/test
project CMSSW
cmsenv
cmsRun $argv


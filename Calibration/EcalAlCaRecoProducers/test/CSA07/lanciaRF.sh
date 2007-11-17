#! /bin/sh
cd /afs/cern.ch/user/p/presotto/CMSSW_1_6_0/src
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/p/presotto/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/TestCsaRecalFilter.cfg 
cp *.root /afs/cern.ch/user/p/presotto/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/results2/


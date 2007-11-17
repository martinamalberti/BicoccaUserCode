#! /bin/sh 
cd /afs/cern.ch/user/p/presotto/CMSSW_1_6_0/src/ 
eval `scramv1 run -sh` 
cd -
export STAGE_SVCCLASS=cmscaf
cmsRun /afs/cern.ch/user/p/presotto/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/IMAcalib_CSA07_ALL_miscalib.cfg >& log_ALL_miscalib.txt  
ls -latr   
mkdir /afs/cern.ch/user/p/presotto/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL_miscalib 
mv *root /afs/cern.ch/user/p/presotto/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL_miscalib 
cp *xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL_miscalib 

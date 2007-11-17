#! /bin/sh 
cd /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/ 
eval `scramv1 run -sh` 
cd -
export STAGE_SVCCLASS=cmscaf
cmsRun /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/IMAcalib_CSA07_ALL.cfg >& log_ALL.txt  
ls -latr   
mkdir /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL 
mv *root /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL 
mv log_ALL.txt /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL 
cp *xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL 
mv EBrecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EBrecalib_ALL.xml   
mv EErecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EErecalib_ALL.xml   

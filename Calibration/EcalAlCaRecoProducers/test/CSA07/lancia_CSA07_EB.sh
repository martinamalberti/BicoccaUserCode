#! /bin/sh 
cd /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/ 
eval `scramv1 run -sh` 
cd -
export STAGE_SVCCLASS=cmscaf
cmsRun /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/IMAcalib_CSA07_EB.cfg >& log_EB.txt  
ls -latr   
mkdir /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EB 
mv *root /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EB 
mv log_EB.txt /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EB 
cp *xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EB 
mv EBrecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EBrecalib_EB.xml   
mv EErecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EErecalib_EB.xml   

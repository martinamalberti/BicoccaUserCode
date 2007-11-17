#! /bin/sh 
cd /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/ 
eval `scramv1 run -sh` 
cd -
export STAGE_SVCCLASS=cmscaf
cmsRun /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/IMAcalib_CSA07_EE.cfg >& log_EE.txt  
ls -latr   
mkdir /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EE 
mv *root /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EE 
mv log_EE.txt /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EE 
cp *xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_EE 
mv EBrecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EBrecalib_EE.xml   
mv EErecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EErecalib_EE.xml   

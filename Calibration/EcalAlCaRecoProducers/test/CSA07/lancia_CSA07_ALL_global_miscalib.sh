#! /bin/sh 
cd /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/ 
eval `scramv1 run -sh` 
cd -
export STAGE_SVCCLASS=cmscaf
cmsRun /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/IMAcalib_CSA07_ALL_global_miscalib.cfg >& log.txt  
head -n 200 log.txt > begin.txt
tail -n 200 log.txt > end.txt
cat begin.txt end.txt > logCompact.txt
ls -latr   
mkdir /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL_global_miscalib 
mv *root /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL_global_miscalib 
mv logCompact.txt /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL_global_miscalib 
cp *xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/results_ALL_global_miscalib 
cp EBrecalib.xml /afs/cern.ch/user/g/govoni/scratch0/EBrecalib_ALL_global_miscalib.xml   
cp EErecalib.xml /afs/cern.ch/user/g/govoni/scratch0/EErecalib_ALL_global_miscalib.xml   
mv EBrecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EBrecalib_ALL_global_miscalib.xml   
mv EErecalib.xml /afs/cern.ch/user/g/govoni/scratch1/CMSSW/CALIB/CMSSW_1_6_0/src/Calibration/EcalAlCaRecoProducers/test/CSA07/coeff/EErecalib_ALL_global_miscalib.xml   

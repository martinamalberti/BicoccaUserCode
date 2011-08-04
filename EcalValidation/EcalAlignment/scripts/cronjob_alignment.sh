#! /bin/sh

# set the cmssw environment
export SCRAM_ARCH=slc5_amd64_gcc434
source /afs/cern.ch/cms/cmsset_default.sh
cd /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/
eval `scramv1 runtime -sh`
cd scripts


# look for new runs
if [ -f /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list.txt ]
then
rm /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list.txt
fi

dbs search --query='find run where dataset=/SingleElectron/Run2011A-PromptReco-v4/RECO' | grep "1" > /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list.txt
chmod a+rw /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list.txt

# create the new run list
rm runList.txt

diff /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list.txt /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list_old.txt | grep "<" | awk '{print $2}' > runList.txt


# launch the jobs
perl launchJobs_lxbatch.pl params_lxbatch.CFG


# update the run list
mv /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list.txt /data2/amassiro/ECALAlignment/Releases/CMSSW_4_2_3/src/EcalValidation/EcalAlignment/scripts/list_old.txt





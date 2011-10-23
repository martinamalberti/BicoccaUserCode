#! /bin/sh


ls -lrh /data2/calibrator/NTUPLES/Run2011B/SingleElectron_Run2011B-PromptReco-v1/ | awk '{print $9}' | grep "run1" | tr "_" " " | tr "." " " | tr "n" " " | awk '{print $3}' > ./list.txt
chmod a+rw ./list.txt


# create the new run list
rm runList.txt
diff ./list.txt ./list_old.txt | grep "<" | awk '{print $2}' > runList.txt


# launch the jobs
perl launchJobs_local.pl params_local.CFG


# update the run list
mv ./list.txt ./list_old.txt


# make the history plots
cd /data2/calibrator/PLOTS/testAndrea/new/
source ./lancia.sh

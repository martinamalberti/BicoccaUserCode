#! /bin/sh


ls -ltrh /data2/calibrator/NTUPLES/Run2011A/SingleElectron_Run2011A-PromptReco-v4/ | awk '{print $9}' | grep "run1" | tr "_" " " | tr "." " " | tr "n" " " | awk '{print $3}' > ./list.txt
chmod a+rw ./list.txt


# create the new run list
rm runList.txt
diff ./list.txt ./list_old.txt | grep "<" | awk '{print $2}' > runList.txt


# launch the jobs
perl launchJobs_local.pl params_local.CFG


# update the run list
mv ./list.txt ./list_old.txt





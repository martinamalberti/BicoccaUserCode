#! /bin/sh



# set the cmssw environment
echo ""
echo "****** SET THE CMSSW ENVIRONMENT ******"

export SCRAM_ARCH=slc5_amd64_gcc462
source /afs/cern.ch/cms/cmsset_default.sh
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh
cd BASEDIR/test/cron_ntuples/
eval `scramv1 runtime -sh`



# look for new runs
echo ""
echo "****** LOOK FOR NEW RUNS ******"

if [ -f BASEDIR/test/cron_ntuples/tempList_WORZ.txt ]
then
rm BASEDIR/test/cron_ntuples/tempList_WORZ.txt
fi

dbs search --query='find run where dataset=DATASETNAME' | egrep '1|2|3|4|5|6|7|8|9|0' > BASEDIR/test/cron_ntuples/tempList_WORZ.txt
chmod a+rw BASEDIR/test/cron_ntuples/tempList_WORZ.txt



# create the new run list
echo ""
echo "****** CREATE THE NEW RUN LIST ******"

rm runList_WORZ.txt

touch BASEDIR/test/cron_ntuples/tempList_WORZ_old.txt

diff BASEDIR/test/cron_ntuples/tempList_WORZ.txt BASEDIR/test/cron_ntuples/tempList_WORZ_old.txt | grep "<" | awk '{print $2}' > BASEDIR/test/cron_ntuples/runList_WORZ.txt



# launch the jobs
echo ""
echo "****** LAUNCH THE JOBS ******"

perl launchNtuples.pl paramsCronjob_ntuples.cfg WORZ
sh BASEDIR/test/cron_ntuples/lancia_WORZ.sh



# update the run list
mv BASEDIR/test/cron_ntuples/tempList_WORZ.txt BASEDIR/test/cron_ntuples/tempList_WORZ_old.txt

#! /bin/sh                                                                                                                                                                         



## set the cmssw environment
#echo ""
#echo "****** SET THE CMSSW ENVIRONMENT ******"
#
#export SCRAM_ARCH=slc5_amd64_gcc462
#source /afs/cern.ch/cms/cmsset_default.sh
#source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
#source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh
#cd BASEDIR/test/cron_ntuples/
#eval `scramv1 runtime -sh`



# mount eos
echo ""
echo "****** MOUNT EOS ******"

/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select -b fuse mount ~/eos eos

ls -ltrh /afs/cern.ch/user/a/abenagli/eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/ > eosFileList.txt

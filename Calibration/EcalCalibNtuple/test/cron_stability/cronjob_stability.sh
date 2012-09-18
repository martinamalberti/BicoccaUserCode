#! /bin/sh



 set the cmssw environment
echo ""
echo "****** SET THE CMSSW ENVIRONMENT ******"

export SCRAM_ARCH=slc5_amd64_gcc462
source /afs/cern.ch/cms/cmsset_default.sh
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh
cd /afs/cern.ch/user/e/ecalmon/EoPStability/2012/cron_stability/CMSSW_5_3_3_patch1/src/
eval `scramv1 runtime -sh`
cd -



# mount eos
echo ""
echo "****** MOUNT EOS ******"

/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select -b fuse mount ~/eos eos



# launch the plots
echo ""
echo "****** LAUNCH THE PLOTS ******"

perl launchStability.pl



# copy the plots
echo ""
echo "****** COPY THE PLOTS ******"

rm -rf /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/EB_1_1_2012_31_12_2012/
rm -rf /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/EE_1_1_2012_31_12_2012/
rm -rf /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/EB_1_1_2012_31_12_2012_0.00-1.14/
rm -rf /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/EE_1_1_2012_31_12_2012_1.50-2.00/
rm -rf /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/EB_1_1_2012_31_12_2012_1.14-1.50/
rm -rf /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/EE_1_1_2012_31_12_2012_2.00-2.50/

cp -r EB_1_1_2012_31_12_2012 /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/
cp -r EE_1_1_2012_31_12_2012 /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/
cp -r EB_1_1_2012_31_12_2012_0.00-1.14 /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/
cp -r EE_1_1_2012_31_12_2012_1.50-2.00 /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/
cp -r EB_1_1_2012_31_12_2012_1.14-1.50 /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/
cp -r EE_1_1_2012_31_12_2012_2.00-2.50 /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/

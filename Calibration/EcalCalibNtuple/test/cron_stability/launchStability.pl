#!/usr/bin/perl

use POSIX;


# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------



#------------------
# get the user name

$user = `whoami`;
chomp($user);
print("user: ".$user."\n");



#--------------------------------
# get the files used for template

$command = "cp -r /afs/cern.ch/user/a/abenagli/eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/ReReco/13Jul2012_v2/SingleElectron_Run2012A-WElectron-13Jul2012-v1_USER/ /tmp/".$user;
system($command);
$command = "cp -r /afs/cern.ch/user/a/abenagli/eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/ReReco/13Jul2012_v2/SingleElectron_Run2012B-WElectron-13Jul2012-v1_USER/ /tmp/".$user;
system($command);

open INPUTMC, ">", "inputMC.txt";

print INPUTMC "/tmp/abenagli/SingleElectron_Run2012A-WElectron-13Jul2012-v1_USER/*.root\n";
print INPUTMC "/tmp/abenagli/SingleElectron_Run2012B-WElectron-13Jul2012-v1_USER/*.root\n";



#---------------------------------
# get the files used for stability

open INPUTDATA, ">", "inputDATA.txt";

$command = "ls -lh /afs/cern.ch/user/a/abenagli/eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".
           " | grep Run2012".
           " | awk \'{print \$9}\'".
           " > list_periods.txt";
system($command);

open(LISTPERIODS,"list_periods.txt");
while(<LISTPERIODS>)
{
  chomp;
  $period = $_;
  print(">>> ".$period."\n");
  
  $command = "ls -lh /afs/cern.ch/user/a/abenagli/eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period.
             " | grep Single".
             " | awk \'{print \$9}\'".
             " > list_".$period."_datasets.txt";
  system($command);
  
  $command = "cp -r /afs/cern.ch/user/a/abenagli/eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period."/cron*Single* /tmp/".$user;
  system($command);
  
  
  
  open(LISTDATASETS,"list_".$period."_datasets.txt");
  while(<LISTDATASETS>)
  {
    chomp;
    $dataset = $_;
    print(">>>>>> ".$dataset."\n");
    
    $command = "ls -lh /afs/cern.ch/user/a/abenagli/eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period."/".$dataset.
               " | grep :".
               " | awk \'{print \$9}\'".
               " > list_".$period."_".$dataset."_jobs.txt";
    system($command);
    
    
    
    open(LISTJOBS,"list_".$period."_".$dataset."_jobs.txt");
    while(<LISTJOBS>)
    {
      chomp;
      $job = $_;
      print(">>>>>>>>> ".$job."\n");
      
      print INPUTDATA "/tmp/".$user."/".$dataset."/".$job."/*.root\n";
     }
  }
}



#---------------------------
# launch the plot production

$command = "sh lancia.sh";
system($command);

#!/usr/bin/perl

use POSIX;


# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------



#-------------
# get the date

@time = localtime(time);

$year    = 1900+$time[5];
$month   = 1+$time[4];
$day     = $time[3];
$hours   = $time[2];
$minutes = $time[1];
if( $minutes < 9 )
{
  $minutes = "0".$minutes;
}

$date = $month."/".$day."/".$year." ".$hours.":".$minutes;

$command = "rm ./lastStabilityPlot.dat";
system($command);

open LASTSTABILITY, ">", "./lastStabilityPlot.dat";
print LASTSTABILITY "Latest plot creation time: ".$date."\n";
close LASTSTABILITY;



#------------------
# get the user name

$user = `whoami`;
chomp($user);



#--------------------------------
# get the files used for template

$command = "cp -r /afs/cern.ch/user/e/ecalmon//eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/ReReco/13Jul2012_v2/SingleElectron_Run2012A-WElectron-13Jul2012-v1_USER/ /tmp/".$user;
system($command);
$command = "cp -r /afs/cern.ch/user/e/ecalmon//eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/ReReco/13Jul2012_v2/SingleElectron_Run2012B-WElectron-13Jul2012-v1_USER/ /tmp/".$user;
system($command);

open INPUTMC, ">", "inputMC.txt";

print INPUTMC "/tmp/ecalmon/SingleElectron_Run2012A-WElectron-13Jul2012-v1_USER/*.root\n";
print INPUTMC "/tmp/ecalmon/SingleElectron_Run2012B-WElectron-13Jul2012-v1_USER/*.root\n";



#---------------------------------
# get the files used for stability

open INPUTDATA, ">", "inputDATA.txt";

print INPUTDATA "/tmp/ecalmon/SingleElectron_Run2012A-WElectron-13Jul2012-v1_USER/*.root\n";
print INPUTDATA "/tmp/ecalmon/SingleElectron_Run2012B-WElectron-13Jul2012-v1_USER/*.root\n";

$command = "ls -lh /afs/cern.ch/user/e/ecalmon//eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".
           " | egrep 'Run2012C|Run2012D|Run2012E|Run2012F'".
           " | awk \'{print \$9}\'".
           " > list_periods.txt";
system($command);

open(LISTPERIODS,"list_periods.txt");
while(<LISTPERIODS>)
{
  chomp;
  $period = $_;
  print(">>> ".$period."\n");
  
  $command = "ls -lh /afs/cern.ch/user/e/ecalmon//eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period.
             " | grep Single".
             " | awk \'{print \$9}\'".
             " > list_".$period."_datasets.txt";
  system($command);
  
  $command = "cp -r /afs/cern.ch/user/e/ecalmon//eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period."/cron*Single* /tmp/".$user;
  system($command);
  
  
  
  open(LISTDATASETS,"list_".$period."_datasets.txt");
  while(<LISTDATASETS>)
  {
    chomp;
    $dataset = $_;
    print(">>>>>> ".$dataset."\n");
    
    $command = "ls -lh /afs/cern.ch/user/e/ecalmon//eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period."/".$dataset.
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



#---------------
# clean up a bit

$command = "mv ./lastStabilityPlot.dat /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/lastStabilityPlot.dat";
system($command);

$command = "rm list_*.txt";
system($command);

$command = "rm inputDATA.txt";
system($command);

$command = "rm inputMC.txt";
system($command);

$command = "rm -rf /tmp/ecalmon/*";
system($command);

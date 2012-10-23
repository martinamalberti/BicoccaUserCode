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
if( $minutes < 10 )
{
  $minutes = "0".$minutes;
}

$date = $month."/".$day."/".$year." ".$hours.":".$minutes;

$command = "rm ./out.txt";
system($command);

$command = "rm ./lastStabilityPlot.dat";
system($command);

$command = "rm ./lastStabilityNtuple.dat";
system($command);

$command = "rm ./lastStabilityRun.dat";
system($command);

$command = "rm ./lastStabilityLumi.dat";
system($command);
$command = "cp /afs/cern.ch/user/a/abenagli/public/lastStabilityLumi.dat ./";
system($command);

open LASTSTABILITY, ">", "./lastStabilityPlot.dat";
print LASTSTABILITY "Latest plot creation time: ".$date."\n";
close LASTSTABILITY;

open LASTNTUPLE, ">", "./lastStabilityNtuple.dat";
$lastStabilityNtuple = "";

open LASTRUN, ">", "./lastStabilityRun.dat";
$lastRun = `tail -1 /afs/cern.ch/user/a/abenagli/public/lastStabilityJson.dat`;
@vals = split('"', $lastRun);
$lastRun = $vals[1];
print LASTRUN "Latest run: ".$lastRun."\n";

close LASTRUN;







#------------------------
# clean up the list files

$command = "rm inputDATA.txt";
system($command);

$command = "rm inputMC.txt";
system($command);



#--------------------------------
# get the files used for template
$eos = "/afs/cern.ch/project/eos/installation/0.2.5/bin/eos.select";

$command = "mkdir /tmp/ecalmon/MC";
system($command);

$command = $eos." cp -r /eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/ReReco/13Jul2012_v2/SingleElectron_Run2012A-WElectron-13Jul2012-v1_USER/ /tmp/ecalmon/MC/";
system($command);
$command = $eos." cp -r /eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/ReReco/13Jul2012_v2/SingleElectron_Run2012B-WElectron-13Jul2012-v1_USER/ /tmp/ecalmon/MC/";
system($command);

open INPUTMC, ">", "inputMC.txt";

print INPUTMC "/tmp/ecalmon/MC/*.root\n";



#---------------------------------
# get the files used for stability

$command = "mkdir /tmp/ecalmon/MC";
system($command);

open INPUTDATA, ">", "inputDATA.txt";

print INPUTDATA "/tmp/ecalmon/MC/*.root\n";
print INPUTDATA "/tmp/ecalmon/DATA/*.root\n";

$command = "cmsLs /store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".
           " | egrep 'Run2012C|Run2012D|Run2012E|Run2012F'".
           " | awk \'{print \$5}\'".
           " > list_periods.txt";
system($command);

open(LISTPERIODS,"list_periods.txt");
while(<LISTPERIODS>)
{
  chomp;
  $period = $_;
  @values = split('/', $period);
  $period = $values[@values-1];
  print(">>> ".$period."\n");
  
  $command = "cmsLs /store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period.
             " | grep Single".
             " | awk \'{print \$5}\'".
             " > list_".$period."_datasets.txt";
  system($command);
  
  
  
  open(LISTDATASETS,"list_".$period."_datasets.txt");
  while(<LISTDATASETS>)
  {
    chomp;
    $dataset = $_;
    @values = split('/', $dataset);
    $dataset = $values[@values-1];
    print(">>>>>> ".$dataset."\n");
    
    $command = "cmsLs /store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period."/".$dataset.
               " | grep :".
               " | awk \'{print \$5}\'".
               " > list_".$period."_".$dataset."_jobs.txt";
    system($command);
    
    
    
    $isFirstJob = 1;
    open(LISTJOBS,"list_".$period."_".$dataset."_jobs.txt");
    while(<LISTJOBS>)
    {
      chomp;
      $job = $_;
      @values = split('/', $job);
      $job = $values[@values-1];
      print(">>>>>>>>> ".$job."\n");
      
      $command = $eos." cp -r /eos/cms/store/group/alca_ecalcalib/ecalMIBI/NTUPLES_EOverP/PromptReco/".$period."/".$dataset."/".$job."/ /tmp/ecalmon/DATA/";
      print($command."\n");
      system($command);
      
      if( $isFirstJob == 1 )
      {
        $lastStabilityNtuple = $job;
        $isFirstJob = 0;
      }
    }
  }
}

@values2 = split('_', $lastStabilityNtuple);
$date = $values2[0];
$time = $values2[1];
@values3 = split('-', $date);
$year  = $values3[0];
$month = $values3[1];
$day   = $values3[2];
@values4 = split(':', $time);
$hours   = $values4[0];
$minutes = $values4[1];
if( $minutes <= 9 )
{
  $minutes = "0".$minutes;
}
$date = $month."/".$day."/".$year." ".$hours.":".$minutes;
print LASTNTUPLE "Latest ntuple creation time: ".$date."\n";
close LASTNTUPLE;



#---------------------------
# launch the plot production

$command = "unbuffer sh lancia.sh > /afs/cern.ch/user/e/ecalmon/EoPStability/2012/cron_stability/out.txt";
system($command);



#---------------
# clean up a bit

$command = "cp ./lastStabilityPlot.dat /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/lastStabilityPlot.dat";
system($command);

$command = "cp ./lastStabilityNtuple.dat /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/lastStabilityNtuple.dat";
system($command);

$command = "cp ./lastStabilityRun.dat /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/lastStabilityRun.dat";
system($command);

$command = "cp ./lastStabilityLumi.dat /afs/cern.ch/user/e/ecalmon/www/EoPStability2012/lastStabilityLumi.dat";
system($command);

$command = "rm list_*.txt";
system($command);

$command = "rm -rf /tmp/ecalmon/MC";
system($command);

$command = "rm -rf /tmp/ecalmon/DATA";
system($command);

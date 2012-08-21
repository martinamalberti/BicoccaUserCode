#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

#----------------------------------
# lettura dei parametri da cfg file

print "reading ".$ARGV[0]."\n";

open(USERCONFIG,$ARGV[0]);
while(<USERCONFIG>)
{
  chomp; 
  s/#.*//;                # no comments
  s/^\s+//;               # no leading white
  s/\s+$//;               # no trailing white
  
  ($var,$value) = split(/\s*=\s*/, $_, 2);
  $User_Preferences{$var} = $value;
}

$BASEDir     = $User_Preferences{"BASEDir"};
$DATASETName = $User_Preferences{"DATASETName"};
$FOLDERName  = $User_Preferences{"FOLDERName"};
$PERIODName  = $User_Preferences{"PERIODName"};

print "BASEDir     = ".$BASEDir.     "\n";
print "DATASETName = ".$DATASETName. "\n";
print "FOLDERName  = ".$FOLDERName."\n";
print "PERIODName  = ".$PERIODName.  "\n";






#--------------------
# create crab job dir

@time = localtime(time);

$year    = 1900+$time[5];
$month   = 1+$time[4];
$day     = $time[3];
$hours   = $time[2];
$minutes = $time[3];

$date = $year."-".$month."-".$day."_".$hours.":".$minutes;
print("date: ".$date."\n");

system("mkdir ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date);
system("cat crab_simpleNtupleEoverP_data_template.cfg   | sed -e s%BASEDIR%".$BASEDir.
                                                    "%g | sed -e s%DATASETNAME%".$DATASETName.
                                                    "%g | sed -e s%FOLDERNAME%".$FOLDERName.
                                                    "%g | sed -e s%PERIODNAME%".$PERIODName.
                                                    "%g | sed -e s%DATE%".$date.
                                                    "%g > ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date."/crab_simpleNtupleEoverP_data.cfg");



#-----------------
# create json file
# e.g.: {"190645": [[10, 110]], ... }

open(RUNLIST,"runList.txt");

open JSON, ">", "tempJson.txt";
print JSON "{";

$isFirstRun = 1;

while(<RUNLIST>)
{
  chomp;
  $run = $_;
  
  if( $isFirstRun == 1 )
  {
    print JSON "\"".$run."\": [[1,999999]]";
    $isFirstRun = 0;
  }
  else
  {
    print JSON ", \"".$run."\": [[1,999999]]";
  }
}

print JSON "}\n";

$CMSSW_RELEASE_BASE = $ENV{'CMSSW_RELEASE_BASE'};
system("python ".$CMSSW_RELEASE_BASE."/src/FWCore/PythonUtilities/scripts/compareJSON.py --and tempJson.txt /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt json.txt");



#-------------------
# create script file

system("rm lancia.sh");
open LANCIA, ">", "lancia.sh";
system("chmode a+x lancia.sh");

print LANCIA "echo \"\"\n";
print LANCIA "echo \"\"\n";
print LANCIA "echo \"\"\n";
print LANCIA "echo \"****** DATE: ".$date." ******\"\n";
print LANCIA "cd ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date."\n";
print LANCIA "crab -create -submit -cfg ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date."/crab_simpleNtupleEoverP_data.cfg\n";
print LANCIA "\n";

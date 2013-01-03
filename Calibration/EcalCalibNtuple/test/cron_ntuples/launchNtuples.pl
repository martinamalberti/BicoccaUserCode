#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

#----------------------------------
# lettura dei parametri da cfg file

print "reading ".$ARGV[0]."\n";

$WORZ = $ARGV[1];

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

if( $WORZ eq "W" )
{
  $BASEDir     = $User_Preferences{"BASEDir"};
  $DATASETName = $User_Preferences{"WDATASETName"};
  $FOLDERName  = $User_Preferences{"WFOLDERName"};
  $PERIODName  = $User_Preferences{"PERIODName"};
}
if( $WORZ eq "Z" )
{
  $BASEDir     = $User_Preferences{"BASEDir"};
  $DATASETName = $User_Preferences{"ZDATASETName"};
  $FOLDERName  = $User_Preferences{"ZFOLDERName"};
  $PERIODName  = $User_Preferences{"PERIODName"};
}

print "BASEDir     = ".$BASEDir.    "\n";
print "DATASETName = ".$DATASETName."\n";
print "FOLDERName  = ".$FOLDERName. "\n";
print "PERIODName  = ".$PERIODName. "\n";






#--------------------
# create crab job dir

@time = localtime(time);

$year    = 1900+$time[5];
$month   = 1+$time[4];
$day     = $time[3];
$hours   = $time[2];
$minutes = $time[1];

$date = $year."-".$month."-".$day."_".$hours.":".$minutes;
print("date: ".$date."\n");

system("mkdir ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date);
system("cat crab_simpleNtupleEoverP_data_template.cfg   | sed -e s%BASEDIR%".$BASEDir.
                                                    "%g | sed -e s%DATASETNAME%".$DATASETName.
                                                    "%g | sed -e s%FOLDERNAME%".$FOLDERName.
                                                    "%g | sed -e s%WORZ%".$WORZ.
                                                    "%g | sed -e s%PERIODNAME%".$PERIODName.
                                                    "%g | sed -e s%DATE%".$date.
                                                    "%g > ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date."/crab_simpleNtupleEoverP_data.cfg");



#-----------------
# create json file
# e.g.: {"190645": [[10, 110]], ... }

open(RUNLIST,"runList_".$WORZ.".txt");

open JSON, ">", "tempJson_".$WORZ.".txt";
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

system("python ".$CMSSW_RELEASE_BASE."/src/FWCore/PythonUtilities/scripts/compareJSON.py --and tempJson_".$WORZ.".txt /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt json_".$WORZ.".txt");

system("python ".$CMSSW_RELEASE_BASE."/src/FWCore/PythonUtilities/scripts/compareJSON.py --or json_".$WORZ.".txt ~/public/lastStabilityJson.dat ~/public/temp_lastStabilityJson.dat");

system("rm tempJson_".$WORZ.".txt");
system("mv ~/public/temp_lastStabilityJson.dat ~/public/lastStabilityJson.dat");



#-------------------
# create script file

system("rm lancia_".$WORZ.".sh");
open LANCIA, ">", "lancia_".$WORZ.".sh";
system("chmod a+x lancia_".$WORZ.".sh");

print LANCIA "echo \"\"\n";
print LANCIA "echo \"\"\n";
print LANCIA "echo \"\"\n";
print LANCIA "echo \"****** DATE: ".$date." ******\"\n";
print LANCIA "cd ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date."\n";
print LANCIA "crab -create -submit -cfg ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/".$date."/crab_simpleNtupleEoverP_data.cfg\n";
print LANCIA "\n";

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

$BASEDir      = $User_Preferences{"BASEDir"};
$WDATASETName = $User_Preferences{"WDATASETName"};
$WFOLDERName  = $User_Preferences{"WFOLDERName"};
$ZDATASETName = $User_Preferences{"ZDATASETName"};
$ZFOLDERName  = $User_Preferences{"ZFOLDERName"};
$PERIODName   = $User_Preferences{"PERIODName"};

print "BASEDir      = ".$BASEDir.     "\n";
print "WDATASETName = ".$WDATASETName."\n";
print "WFOLDERName  = ".$WDATASETName."\n";
print "ZDATASETName = ".$ZDATASETName."\n";
print "ZFOLDERName  = ".$ZDATASETName."\n";
print "PERIODName   = ".$PERIODName.  "\n";






#----------------------
# create W cronjob file

system("cat cronjob_ntuples_template.sh   | sed -e s%BASEDIR%".$BASEDir.
                                      "%g | sed -e s%DATASETNAME%".$WDATASETName.
                                      "%g | sed -e s%WORZ%W".
                                      "%g > cronjob_ntuples_W.sh");

#-------------------------
# create W crab job folder

system("mkdir ".$BASEDir."/test/cron_ntuples/".$WFOLDERName);
system("rm ".$BASEDir."/test/cron_ntuples/runList_W.txt");
system("rm ".$BASEDir."/test/cron_ntuples/tempList_W_old.txt");




#----------------------
# create Z cronjob file

system("cat cronjob_ntuples_template.sh   | sed -e s%BASEDIR%".$BASEDir.
                                      "%g | sed -e s%DATASETNAME%".$ZDATASETName. 
                                      "%g | sed -e s%WORZ%Z".
                                      "%g > cronjob_ntuples_Z.sh");

#-------------------------
# create Z crab job folder

system("mkdir ".$BASEDir."/test/cron_ntuples/".$ZFOLDERName);
system("rm ".$BASEDir."/test/cron_ntuples/runList_Z.txt");
system("rm ".$BASEDir."/test/cron_ntuples/tempList_Z_old.txt");

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

print "BASEDir     = ".$BASEDir.    "\n";
print "DATASETName = ".$DATASETName."\n";
print "FOLDERName  = ".$DATASETName."\n";
print "PERIODName  = ".$PERIODName. "\n";






#--------------------
# create cronjob file

system("cat cronjob_ntuples_template.sh   | sed -e s%BASEDIR%".$BASEDir.
                                      "%g | sed -e s%DATASETNAME%".$DATASETName. 
                                      "%g > cronjob_ntuples.sh");


#-----------------------
# create crab job folder

system("mkdir ".$BASEDir."/test/cron_ntuples/".$FOLDERName);
system("rm ".$BASEDir."/test/cron_ntuples/runList.txt");
system("rm ".$BASEDir."/test/cron_ntuples/tempList_old.txt");

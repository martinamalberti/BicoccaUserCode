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
$JSONFile    = $User_Preferences{"JSONFile"};
$PERIODName  = $User_Preferences{"PERIODName"};

print "BASEDir     = ".$BASEDir.     "\n";
print "DATASETName = ".$DATASETName. "\n";
print "FOLDERName  = ".$FOLDERName."\n";
print "JSONFile    = ".$JSONFile.  "\n";
print "PERIODName  = ".$PERIODName.  "\n";






#--------------------
# create cronjob file

system("rm lancia.sh");
open LANCIA, ">", "lancia.sh";
system("chmode a+x lancia.sh");

open(RUNLIST,"runList.txt");
while(<RUNLIST>)
{
  chomp;
  $run = $_;
  
  print("run: ".$run."\n");
  system("mkdir ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/run".$run);
  
  system("cat crab_simpleNtupleEoverP_data_template.cfg   | sed -e s%BASEDIR%".$BASEDir.
                                                      "%g | sed -e s%DATASETNAME%".$DATASETName.
                                                      "%g | sed -e s%FOLDERNAME%".$FOLDERName.
                                                      "%g | sed -e s%PERIODNAME%".$PERIODName.
                                                      "%g | sed -e s%JSONFILE%".$JSONFile.
                                                      "%g | sed -e s%RUN%".$run.
                                                      "%g > ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/run".$run."/crab_simpleNtupleEoverP_data.cfg");
  
  print LANCIA "echo \"\"\n";
  print LANCIA "echo \"\"\n";
  print LANCIA "echo \"\"\n";
  print LANCIA "echo \">>>>>> RUN ".$run." <<<<<<\"\n";
  print LANCIA "cd ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/run".$run."\n";
  print LANCIA "crab -create -submit -cfg ".$BASEDir."/test/cron_ntuples/".$FOLDERName."/run".$run."/crab_simpleNtupleEoverP_data.cfg\n";
  print LANCIA "\n";
}

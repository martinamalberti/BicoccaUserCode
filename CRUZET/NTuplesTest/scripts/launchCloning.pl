#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

#PG lettura dei parametri da cfg file
#PG --------------------------------
print "reading ".$ARGV[0]."\n" ;

open (USERCONFIG,$ARGV[0]) ;

while (<USERCONFIG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);
    $User_Preferences{$var} = $value;
  }


$RUNLISTFile = $User_Preferences{"RUNLISTFile"} ;
$CLONINGCfgFile = $User_Preferences{"CLONINGCfgFile"} ;
$CASTORSAVEFolder = $User_Preferences{"CASTORSAVEFolder"} ;

print "RUNLISTFile = ".$RUNLISTFile."\n" ;
print "CLONINGCfgFile = ".$CLONINGCfgFile."\n" ;
print "CASTORSAVEFolder = ".$CASTORSAVEFolder."\n" ;



# PG prepare the array containing the root files list

$runNumber ;
open (RUNLIST,$RUNLISTFile) ;
#PG loop on the original SourceFile
while (<RUNLIST>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    print "\nRUNNUMBER = ".$_."\n" ;
    $runNumber = $_ ;

    $tempo = "./tempo" ;
    system ("cat ".$CLONINGCfgFile." | sed -e s%RUNNUMBER%".$runNumber."% > ".$tempo) ;

    $tempo4 = "./tempo4" ;
    system ("rfdir ".$CASTORSAVEFolder.$runNumber." | grep root | awk '{print \$9}' > ".$tempo4."\n") ;

    $listOfFiles;   
    open (FILELIST,$tempo4) ;
    while (<FILELIST>)
    {
      chomp; 
      s/#.*//;                # no comments
      s/^\s+//;               # no leading white
      s/\s+$//;               # no trailing white
      $file = $_ ;

      $listOfFiles = $listOfFiles."APICErfio:".$CASTORSAVEFolder."/".$runNumber."/".$file."APICE," ;
    }

    chop($listOfFiles) ;
    #print $listOfFiles."\n" ;

    $tempo5 = "./tempo5" ;    
    system ("cat ".$tempo." | sed -e s%LISTOFFILES%".$listOfFiles."% > ".$tempo5) ;
    $listOfFiles = "" ;

    $tempo6 = "./tempo6" ;
    system ("cat ".$tempo5." | sed -e s%APICE%\\'%g > ".$tempo6) ;




    $currDir = `pwd` ;
    chomp ($currDir) ;

    $cfgFileName = $currDir."/cloning_".$runNumber.".cfg" ;
    system ("mv ".$tempo6." ".$cfgFileName) ;
    system ("rm ./tempo*") ;



    $tempBjob = $currDir."/bjobCloning_".$runNumber.".sh" ;
    $command = "touch ".$tempBjob ;
    system ($command) ;
    $command = "chmod 777 ".$tempBjob ;
    system ($command) ;


    $command = "cd ".$currDir ;
    system ("echo ".$command." > ".$tempBjob) ;

    $command = "eval `scramv1 runt -sh`" ;
    system ("echo ".$command." >> ".$tempBjob) ;

    $command = "cloning $cfgFileName" ;
    system ("echo ".$command." >> ".$tempBjob) ;

    $command = "rfcp /tmp/cloned_".$runNumber.".root ".$CASTORSAVEFolder.$runNumber ; 
    system ("echo ".$command." >> ".$tempBjob) ;

    print ("bsub -q cmscaf ".$tempBjob."\n") ;
    #system ("bsub -q cmscaf ".$tempBjob) ;
  
    print "\n" ;
    
  } #PG loop on the original SourceFile

#!/usr/bin/perl

use Math::Trig;

# ----------------------------------------------------------------------------

# qui ci sara' una sub che fa il cfg
sub makeConfig (@locvalues,$locindex) 
    {
       print $locindex." -> ".$locvalues[0]." ".$locvalues[1]." ".$locvalues[2]." ".$locvalues[3]." ".$locvalues[4]." ".$locvalues[5]."\n" ;    
    }

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------


open (USERCONFIG,$ARGV[0]) ;
print $ARGV[0]." opened\n" ;

while (<USERCONFIG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);    $User_Preferences{$var} = $value;  }

$templateConfig = $User_Preferences{"IMAtemplateConfig"} ;
$selectionsList = $User_Preferences{"selectionsList"} ;
$tagFolder = $User_Preferences{"tagFolder"} ; # abs path 
$queue = $User_Preferences{"queue"} ;

$configFolder = $tagFolder."_config" ;
if (! (-e $configFolder))
{
  mkdir $configFolder ;    
}

$scriptFolder = $tagFolder."_script" ;
if (! (-e $scriptFolder))
{
  mkdir $scriptFolder ;    
}


#PG read the selections file

open (SELECTIONS,$selectionsList) ;
print $selectionsList." opened\n" ;

$index = 0 ;
while (<SELECTIONS>)
  {
    chomp;                  # togli l'end
    next if s/#.*//;                # no comments

    @values = split(/ +/, $_) ;
    $size  = @values ;
    next if $size < 6 ;
#    print "line size ".$size."\n" ;
    print $index." -> ".$values[0]." ".$values[1]." ".$values[2]." ".$values[3]." ".$values[4]." ".$values[5]."\n" ;

    #PG prepare the CFG file from the template
    #PG ======================================
    
    $configName = "calibTest_".$index.".cfg" ;

    system("cat $templateConfig | sed -e s%ESCOPIN_MIN%".$values[0]."%g > tempo") ;
    system("mv tempo $configName");
    system("cat $configName | sed -e s%ESCOPIN_MAX%".$values[1]."%g > tempo") ;
    system("mv tempo $configName");
    system("cat $configName | sed -e s%ESEEDOPOUT_MIN%".$values[2]."%g > tempo") ;
    system("mv tempo $configName");
    system("cat $configName | sed -e s%ESEEDOPOUT_MAX%".$values[3]."%g > tempo") ;
    system("mv tempo $configName");
    system("cat $configName | sed -e s%PINMPOUTOPIN_MIN%".$values[4]."%g > tempo") ;
    system("mv tempo $configName");
    system("cat $configName | sed -e s%PINMPOUTOPIN_MAX%".$values[5]."%g > tempo") ;
    system("mv tempo $configName");
    system("mv $configName $configFolder");

    #PG prepare the script to be sent to the batch system
    #PG =================================================

    $resultsfolder = $tagFolder."_results_".$index ;
 #   if (! (-e $resultsfolder)) { mkdir $resultsfolder ; }
    $folder = `pwd` ;
    $folder =~s/\n//;
    
    $filename = "launch_".$index."_calib.sh" ;
    open JOBFILE, ">".$filename ;
    print JOBFILE "\#! /bin/sh\n" ;
    print JOBFILE "cd $folder\n" ;
    print JOBFILE "eval `scramv1 run -sh`\n" ;
    print JOBFILE "cd -\n" ;
    print JOBFILE "cmsRun ".$configFolder."/".$configName." >& log.txt \n" ;  
    print JOBFILE "head -n 200 log.txt > begin.txt \n" ;
    print JOBFILE "tail -n 200 log.txt > end.txt \n" ;
    print JOBFILE "cat begin.txt end.txt > logCompact.txt \n" ;
    print JOBFILE "ls -latr\n" ;  
    print JOBFILE "mkdir ".$resultsfolder."\n" ;
    print JOBFILE "mv *root ".$resultsfolder."\n" ;
    print JOBFILE "cp *xml ".$resultsfolder."\n" ;
    print JOBFILE "cp logCompact.txt ".$resultsfolder."\n" ;
    print JOBFILE "cp EBrecalib.xml /afs/cern.ch/user/g/govoni/scratch0/EBrecalib_EE_".$index."_recalib.xml\n" ;   
    print JOBFILE "cp EErecalib.xml /afs/cern.ch/user/g/govoni/scratch0/EErecalib_EE_".$index."_recalib.xml\n" ;
    close JOBFILE ;

    system ("chmod 755 ".$filename) ;
    system("mv $filename $scriptFolder");

    #PG run the script on the LSF
    #PG =========================

    $startTime = "" ;
    if ($index > 45) {$startTime = " -b 4:00 "} ;
    
    $command = "bsub -q ".$queue." ".$startTime." ".$scriptFolder."/".$filename ;
    print $command."\n" ;
    system ($command) ;

    ++$index ;
  }





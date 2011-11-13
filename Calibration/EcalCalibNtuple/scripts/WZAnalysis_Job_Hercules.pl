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


$CFGTemplate      = $User_Preferences{"CFGTemplate"} ;
$RUNFILEList      = $User_Preferences{"RUNFILEList"} ;
$INPUTSAVEFolder  = $User_Preferences{"INPUTSAVEFolder"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTSAVEFolder = $User_Preferences{"OUTPUTSAVEFolder"} ;
$OUTPUTFileName   = $User_Preferences{"OUTPUTFileName"} ;
$NUMBERGEDNtuple  = $User_Preferences{"NUMBERGEDNtuple"} ;
$Calibration_Flag = $User_Preferences{"ISCAlib"} ;

print "CFGTemplate = "      .$CFGTemplate."\n" ;
print "RUNFILEList = "      .$RUNFILEList."\n" ;
print "INPUTSAVEFolder = "  .$INPUTSAVEFolder."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTSAVEFolder = " .$OUTPUTSAVEFolder."\n" ;
print "OUTPUTFileName = "   .$OUTPUTFileName."\n" ;
print "ISCAlib = ".$Calibration_Flag."\n" ;


################
# loop over runs 
################
#$command = "ls ".$INPUTSAVEFolder." | awk 'END{print NR}'";



opendir(DIR, $INPUTSAVEFolder);
@count = readdir(DIR) ;
@count = sort(@count);

$count = @count ;  

$ifed=2; 
$jobNumber =0;

$command = "mkdir ".$OUTPUTSAVEPath."/".$OUTPUTSAVEFolder;
#print $command."\n" ;
system($command);


while($ifed< $count)
{
  
  $currDir = `pwd` ;
  chomp ($currDir) ;
  $jobDir = $currDir."/job".$jobNumber ;
  #print "JobDir= ".$jobDir ;
  system ("mkdir ".$jobDir) ;
  
  $LISTFiles = $jobDir."/listFiles.txt";
  

   
  $tempo1 = "./tempo1" ;
  
  system ("cat ".$CFGTemplate."   | sed -e s%OUTPUTSAVEPATH%".$OUTPUTSAVEPath. 
                              "%g | sed -e s%OUTPUTSAVEFOLDER%".$OUTPUTSAVEFolder. 
                              "%g | sed -e s%OUTPUTFILENAME%".$OUTPUTFileName. 
                              "%g | sed -e s%RUN%".$jobNumber.
                              "%g | sed -e s%LISTFILES%".$LISTFiles.
                              "%g | sed -e s%ISCAlib%".$Calibration_Flag.
                              "%g > ".$tempo1) ;
  
 
  
  for ($iFile = $ifed ; $iFile<$ifed + $NUMBERGEDNtuple ; ++$iFile)
  {
    if($iFile < $count)
    {
     $command = "echo ".$INPUTSAVEFolder."/".@count[$iFile]." >> ".$LISTFiles;
      #print "File list command ".$command."\n" ; 
     system($command);
     }
  }
  
  
  $CFGFile = $jobDir."/".$jobNumber."_cfg.py" ;
  print "CFG File generated ".$command."\n" ; 
  system ("mv ".$tempo1." ".$CFGFile) ;
  
  $tempBjob = $jobDir."/bjob_".$jobNumber.".sh" ;
  print ".sh file ".$tempBjob."\n" ;
  
  $command = "WZAnalysis.cpp ".$CFGFile ; system ("echo ".$command." >> ".$tempBjob);
  print($command."\n");
 
  ############
  # submit job."
  ############
  $command = "qsub -V -d ".$jobDir." -q shortcms ".$tempBjob."\n" ;      
  print ($command."\n");
  system ($command);
 
  $ifed = $ifed+$NUMBERGEDNtuple ;
  $jobNumber = $jobNumber +1 ; 

  print "\n" ;
} 
  
  





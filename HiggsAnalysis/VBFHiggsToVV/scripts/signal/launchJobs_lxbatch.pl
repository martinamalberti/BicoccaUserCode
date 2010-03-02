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


$LOCALFolder      = $User_Preferences{"LOCALFolder"} ;
$CMSSWCfgTemplate = $User_Preferences{"CMSSWCfgTemplate"} ;
$INPUTSAVEFolder  = $User_Preferences{"INPUTSAVEFolder"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTSAVEFolder = $User_Preferences{"OUTPUTSAVEFolder"} ;
$OUTPUTFileName   = $User_Preferences{"OUTPUTFileName"} ;
$JOBModulo        = $User_Preferences{"JOBModulo"} ;

print "LOCALFolder = "      .$LOCALFolder."\n" ;
print "CMSSWCfgTemplate = " .$CMSSWCfgTemplate."\n" ;
print "INPUTSAVEFolder = "  .$INPUTSAVEFolder."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTSAVEFolder = " .$OUTPUTSAVEFolder."\n" ;
print "OUTPUTFileName = "   .$OUTPUTFileName."\n" ;
print "JOBModulo = "        .$JOBModulo."\n" ;






#####################################################
# PG prepare the array containing the root files list
#####################################################

$LISTFile = "./list.txt" ;
system ("rfdir ".$INPUTSAVEFolder." | grep root | awk '{print \$9}' > ".$LISTFile."\n") ;



$totNumber = 0;
$jobNumber = 0;

open (LISTFile,$LISTFile) ;
while (<LISTFile>)
{
  #print "File = ".$_;
  ++$totNumber;
}

$jobNumber = int($totNumber/$JOBModulo);
if( $totNumber%$JOBModulo != 0)
{
  $jobNumber = $jobNumber+1;
}

print "NumberOfJobs = ".$jobNumber."\n";




  

 ##########################
 # create folders in castor
 ##########################    

 print ("rfmkdir ".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ;
 system ("rfmkdir ".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ;
 print ("rfchmod 777 ".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ;
 system ("rfchmod 777 ".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ;
  
  
  



################
# loop over jobs 
################

for($jobIt = 1; $jobIt <= $jobNumber; ++$jobIt)
{ 
  $currDir = `pwd` ;
  chomp ($currDir) ;
  system ("mkdir JOB_".$jobIt." \n") ;
  $currDir = $currDir."/JOB_".$jobIt ;
 
   
  $tempo1 = "./tempo1" ;
  system ("cat ".$CMSSWCfgTemplate."   | sed -e s%LOCALFOLDER%".$LOCALFolder.
                                    "%g | sed -e s%JOBIT%".$jobIt.
                                    "%g | sed -e s%OUTPUTFILENAME%".$OUTPUTFileName. 
                                    "%g > ".$tempo1) ;
  
  
  
  $listOfFiles;   
  $it = 0;
  open (LISTFile2,$LISTFile) ;
  while (<LISTFile2>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    $file = $_ ;

    if( ($it >= ($jobIt - 1)*$JOBModulo) && ($it < ($jobIt)*$JOBModulo) )
    { 
      $listOfFiles = $listOfFiles."APICErfio:".$INPUTSAVEFolder."/".$file."APICE," ;
    }
    ++$it;
  }
  
  chop($listOfFiles) ;
  #print $listOfFiles."\n" ;
  
  $tempo2 = "./tempo2" ;    
  system ("cat ".$tempo1." | sed -e s%LISTOFFILES%".$listOfFiles."%g > ".$tempo2) ;
  $listOfFiles = "" ;
  
  $tempo3 = "./tempo3" ;
  system ("cat ".$tempo2." | sed -e s%APICE%\\'%g > ".$tempo3) ;
  
  
  
  
  
  
  $CMSSWCfgFile = $currDir."/".$jobIt."_cfg.py" ;
  system ("mv ".$tempo3." ".$CMSSWCfgFile) ;
  system ("rm ./tempo*") ;
  
  
  
  $tempBjob = $currDir."/bjob_".$jobIt.".sh" ;
  $command = "touch ".$tempBjob ;
  system ($command) ;
  $command = "chmod 777 ".$tempBjob ;
  system ($command) ;
  
  
  $command = "cd ".$currDir ;
  system ("echo ".$command." > ".$tempBjob) ;
  
  $command = "eval `scramv1 runt -sh`" ;
  system ("echo ".$command." >> ".$tempBjob) ;

  $command = "cd -" ;
  system ("echo ".$command." >> ".$tempBjob) ;
  
  $command = "cmsRun ".$CMSSWCfgFile ;
  system ("echo ".$command." >> ".$tempBjob) ;
  
  
  
  ######################
  # copy files to castor
  ######################    
  
  $command = "rfcp ./".$OUTPUTFileName."_".$jobIt.".root ".$OUTPUTSAVEPath.$OUTPUTSAVEFolder; 
  system ("echo ".$command." >> ".$tempBjob) ;

  
  
  ############
  # submit job
  ############
  
  system ("cd ".$currDir." \n") ;
  print ("bsub -q 1nh -cwd ".$currDir." ".$tempBjob."\n") ;
  system ("bsub -q 1nh -cwd ".$currDir." ".$tempBjob) ;
  
  print "\n" ;
}
  

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

print "CFGTemplate = "      .$CFGTemplate."\n" ;
print "RUNFILEList = "      .$RUNFILEList."\n" ;
print "INPUTSAVEFolder = "  .$INPUTSAVEFolder."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTSAVEFolder = " .$OUTPUTSAVEFolder."\n" ;
print "OUTPUTFileName = "   .$OUTPUTFileName."\n" ;






################
# loop over runs 
################

open (RUNFILEList,$RUNFILEList) ;
while (<RUNFILEList>)
{
  $run = $_;
  chomp ($run);
  print("\n----- RUN ".$run." -----\n");
  
  $currDir = `pwd` ;
  chomp ($currDir) ;
  $jobDir = $currDir."/run".$run ;
  system ("mkdir ".$jobDir) ;
  
  
   
  $tempo1 = "./tempo1" ;
  system ("cat ".$CFGTemplate."   | sed -e s%OUTPUTSAVEPATH%".$OUTPUTSAVEPath. 
                              "%g | sed -e s%OUTPUTSAVEFOLDER%".$OUTPUTSAVEFolder. 
                              "%g | sed -e s%OUTPUTFILENAME%".$OUTPUTFileName. 
                              "%g | sed -e s%RUN%".$run.
                              "%g > ".$tempo1) ;
  
  $LISTFile = $currDir."/listFiles.txt";
  
  $command = "echo ".$INPUTSAVEFolder."/simpleNtuple_run".$run.".root  > ".$LISTFile;
  system($command);
  
  
  
  $CFGFile = $jobDir."/".$run."_cfg.py" ;
  system ("mv ".$tempo1." ".$CFGFile) ;
  system ("rm ./tempo*") ;
  
  $command = "WZAnalysis.exe ".$CFGFile ;
  print($command."\n");
  
  
  
  print "\n" ;
}
  

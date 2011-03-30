#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

use Env;

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

$BASEDir           = $User_Preferences{"BASEDir"} ; 
$LISTOFSamples     = $User_Preferences{"LISTOFSamples"} ;
$JOBCfgTemplate    = $User_Preferences{"JOBCfgTemplate"} ;
$INPUTSAVEPath     = $User_Preferences{"INPUTSAVEPath"} ;
$OUTPUTSAVEPath    = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName    = $User_Preferences{"OUTPUTFILEName"} ;
$JOBModulo         = $User_Preferences{"JOBModulo"} ;

$LISTOFSamples  = $BASEDir."/".$LISTOFSamples;
$JOBCfgTemplate = $BASEDir."/".$JOBCfgTemplate;

$TREENAME          = $User_Preferences{"TREENAME"} ;
$JSON              = $User_Preferences{"JSON"} ;

print "BASEDir = "          .$BASEDir."\n" ;
print "JETAlgorithm = "     .$JETAlgorithm."\n" ;
print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "JOBCfgTemplate = "   .$JOBCfgTemplate."\n" ;
print "INPUTSAVEPath = "    .$INPUTSAVEPath."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;
print "JOBModulo = "        .$JOBModulo."\n" ;

#####################################################
# PG prepare the array containing the root files list
#####################################################

# LISTOFSamples e' l'elenco delle cartelle
# LISTOFiles esiste per ogni elemento di LISTOFSamples ed e' l'elenco dei file, per la relativa cartella
# INPUTSAVEPath e' dove stanno tutte le cartelle elencate

open (LISTOFSamples,$LISTOFSamples) ;
while (<LISTOFSamples>)
{
  system("cd ".$BASEDir."\n");

  chomp($_);
  $sample = $_;
  print("Sample: ".$sample."\n") ;  
  # prepare the folder for the jobs settings
 
  system ("mkdir ".$sample."_".$JETAlgorithm."\n") ;

  $LISTOFFiles = "./list_".$sample.".txt" ;
  system ("rfdir ".$INPUTSAVEPath.$sample." | awk '{print \$9}' > ".$LISTOFFiles."\n") ;

#  system ("ls -1 ".$INPUTSAVEPath.$sample." | grep root > ".$LISTOFFiles."\n") ;
#  system ("ls -1 ".$INPUTSAVEPath.$sample." | grep root | awk '{print \"".$INPUTSAVEPath.$sample."\/\"\$1}' > ".$LISTOFFiles."\n") ;
  
  open (LISTOFFiles,$LISTOFFiles) ;
  $totNumber = 0 ;
#  $totNumber = @LISTOFFiles ;
  $jobNumber = 0;
  
#PG this should be replaced by the @LISTOFFiles
  while (<LISTOFFiles>)
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
  
  ################
  # loop over jobs 
  ################
  
  for($jobIt = 1; $jobIt <= $jobNumber; ++$jobIt)
  { 
    $currDir = `pwd` ;
    chomp ($currDir) ;
    
    $jobDir = $currDir."/".$sample."/JOB_".$jobIt ;
    system ("mkdir ".$jobDir." \n") ;
    
    $tempo1 = "./tempo1" ;
    print "cat ".$JOBCfgTemplate."     | sed -e s%TREENAME%".$TREENAME.
                                   "%g | sed -e s%JSON%".$JSON.
                                   "%g | sed -e s%JOBIT%".$jobIt.
                                   "%g | sed -e s%OUTPUTFILENAME%".$OUTPUTFILEName.
                                   "%g | sed -e s%OUTPUTFILEPATH%".$OUTPUTSAVEPath."/".$sample.
                                   "%g > ".$tempo1 ;

    $it = 0;
    #FIXME che cosa sono questi due, con e senza prefix?
    $JOBLISTOFFiles = $jobDir."/list_".$sample.".txt";
    $JOBLISTOFFiles_noPrefix = $jobDir."/list_".$sample."_noPrefix.txt";
    open (JOBLISTOFFiles, ">", $JOBLISTOFFiles) or die "Can't open file ".$JOBLISTOFFiles;
    open (JOBLISTOFFiles_noPrefix, ">", $JOBLISTOFFiles_noPrefix) or die "Can't open file ".$JOBLISTOFFiles_noPrefix;

    open (LISTOFFiles2,$LISTOFFiles) ;
    while (<LISTOFFiles2>)
      {
        chomp; 
        s/#.*//;                # no comments
        s/^\s+//;               # no leading white
        s/\s+$//;               # no trailing white
        $file = $_ ;
        
        if( ($it >= ($jobIt - 1)*$JOBModulo) && ($it < ($jobIt)*$JOBModulo) )
          { 
            print JOBLISTOFFiles $file."\n";
            print JOBLISTOFFiles_noPrefix $file."\n";
          }
        ++$it;
      }

    $tempo2 = "./tempo2" ;    
    system ("cat ".$tempo1." | sed -e s%INPUTFILELIST%".$JOBLISTOFFiles."%g > ".$tempo2) ;
    
    $JOBCfgFile = $jobDir."/selections_cfg.txt" ;
    system ("mv ".$tempo2." ".$JOBCfgFile) ;
    system ("rm ./tempo*") ;
    
    ######################
    # make job files
    ######################    
    
    $tempBjob = $jobDir."/bjob_".$jobIt.".sh" ;
    
    $command = "touch ".$tempBjob ;             system ($command) ;
    $command = "chmod 777 ".$tempBjob ;         system ($command) ;    
    $command = "cd ".$BASEDir ;                 system ("echo ".$command." > ".$tempBjob) ;
    $command = "source ./scripts/setup.sh" ;    system ("echo ".$command." >> ".$tempBjob) ;
    $command = "df -h";                         system ("echo ".$command." >> ".$tempBjob) ;
    $command = "cd -";                          system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rm -rf amassiro/" ;             system ("echo ".$command." >> ".$tempBjob) ;
    $command = "mkdir amassiro" ;               system ("echo ".$command." >> ".$tempBjob) ;
    $command = "mkdir amassiro/".$sample ;      system ("echo ".$command." >> ".$tempBjob) ;
    $command = "cd amassiro/".$sample ;         system ("echo ".$command." >> ".$tempBjob) ;

    open (JOBLISTOFFiles_noPrefix,$JOBLISTOFFiles_noPrefix) ;
    while (<JOBLISTOFFiles_noPrefix>)
      {
        chomp($_);
        $command = "echo rfcp ".$INPUTSAVEPath."/".$sample."/".$_." ./" ;
        system ("echo ".$command." >> ".$tempBjob) ;
#        $command = "echo cp ".$INPUTSAVEPath."/".$sample."/".$_." ./" ;
#        system ("echo ".$command." >> ".$tempBjob) ;
  
#      $command = "cp ".$INPUTSAVEPath."/".$sample."/".$_." ./" ;
#        system ("echo ".$command." >> ".$tempBjob) ;
      }

    system("rm ".$JOBLISTOFFiles_noPrefix);    

    # (there's instructions on the right)
    $command = "echo rfmkdir ".$OUTPUTSAVEPath."/".$sample;                       system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rfmkdir ".$OUTPUTSAVEPath."/".$sample;                            system ("echo ".$command." >> ".$tempBjob) ;
    $command = .$BASEDir"/bin/NtupleMC_test.exe ".$JOBCfgFile ;                   system ("echo ".$command." >> ".$tempBjob) ;    
    $command = "echo rfcp ./".$OUTPUTFILEName."_".$jobIt."_".$JETAlgorithm.".root ".$OUTPUTSAVEPath."/".$sample."/"; system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rfcp ./".$OUTPUTFILEName."_".$jobIt."_".$JETAlgorithm.".root ".$OUTPUTSAVEPath."/".$sample."/";        system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rm -rf /tmp/amassiro/" ;                                                                                  system ("echo ".$command." >> ".$tempBjob) ;
    
    ############
    # submit job
    ############
    
    #$command = "qsub -V -d ".$jobDir." -q longcms ".$tempBjob."\n" ;      
    $command = "bsub ".$jobDir." -q 1nh ".$tempBjob."\n" ;      
    print ($command);
########################################################################    system ($command);
    
    print "\n" ;
  }

  system ("rm ".$LISTOFFiles) ;
}  


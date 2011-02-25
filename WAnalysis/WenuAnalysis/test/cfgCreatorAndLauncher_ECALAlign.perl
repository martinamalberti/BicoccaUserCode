#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

use Env;

#AM lettura dei parametri da cfg file
#AM --------------------------------
print "reading ".$ARGV[0]."" ;

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

$BASEDir = `pwd` ;
chomp ($BASEDir) ;
$INFILESAMPLES    = $User_Preferences{"INFILESAMPLES"} ;
$INPUTSAVEPath    = $User_Preferences{"INPUTSAVEPath"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$WP               = $User_Preferences{"WP"} ;
$JOBModulo        = $User_Preferences{"JOBModulo"} ;

$INFILESAMPLES  = $BASEDir."/".$INFILESAMPLES;

system ("mkdir submit") ;
# $BASEDir = $BASEDir."/submit/" ;
print "BASEDir = "          .$BASEDir."\n" ;
print "INFILESAMPLES = "   .$INFILESAMPLES."\n" ;

#####################################################
# AM prepare the array containing the root files list
#####################################################

# $INFILESAMPLES e' l'elenco dei sample da analizzare
# LISTOFiles esiste per ogni elemento di LISTOFSamples ed e' l'elenco dei file, per il relativo sample
# INPUTSAVEPath e' dove stanno tutte le cartelle elencate

open (INFILESAMPLES,$INFILESAMPLES) ;
while (<INFILESAMPLES>)
{
  chomp($_);
  $sample = $_;
  
  $POSITION;
  $POSITION = index ($sample," ");

  $POSITIONTEST = index ($sample,"#");
#   print("POSITIONTEST: ".$POSITIONTEST."") ;  
  if ($POSITIONTEST != 0) {

  print("Sample: ".$sample."\n") ;  
  $sample = substr ($sample,0,$POSITION) ; 
#   print("Position: ".$POSITION."") ;  
  
  print("Sample: ".$sample."\n") ;  
  # prepare the folder for the jobs settings
  system ("mkdir ".$BASEDir."/submit/".$sample."") ;

  $LISTOFFiles = "./list_".$sample.".txt" ;
  system ("rfdir ".$INPUTSAVEPath.$sample." | awk '{print \"@\"\$9\"@\"}' | tr \"@\" \"\\\"\" | grep root > ".$LISTOFFiles."") ;
  
  open (LISTOFFiles,$LISTOFFiles) ;
  $totNumber = 0 ;
#  $totNumber = @LISTOFFiles ;
  $jobNumber = 0;
  
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
  
  print "NumberOfJobs = ".$jobNumber."";
  
  ################
  # loop over jobs 
  ################
  
  for($jobIt = 1; $jobIt <= $jobNumber; ++$jobIt)
  { 
    $currDir = `pwd` ;
    chomp ($currDir) ;

    $jobDir = $currDir."/submit/".$sample."/JOB_".$jobIt ;
    system ("mkdir ".$jobDir." ") ;


    ######################
    # make cfg files
    ######################    
    
    $tempBjob = $jobDir."/bjob_".$jobIt.".sh" ;
    
    $command = "touch ".$tempBjob ;             system ($command) ;
    $command = "chmod 777 ".$tempBjob ;         system ($command) ;    
    
    $command = "cd ".$BASEDir." ";                  system ("echo ".$command." > ".$tempBjob) ;
    $command = "eval \\\`scramv1 runtime -sh\\\`" ;       system ("echo ".$command." >> ".$tempBjob) ;
    $command = "df -h ";                            system ("echo ".$command." >> ".$tempBjob) ;
    $command = "cd - ";                             system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rm -rf amassiro/ " ;                system ("echo ".$command." >> ".$tempBjob) ;
    $command = "mkdir amassiro " ;                  system ("echo ".$command." >> ".$tempBjob) ;
    $command = "mkdir amassiro/".$sample." " ;      system ("echo ".$command." >> ".$tempBjob) ;
    $command = "cd amassiro/".$sample." " ;         system ("echo ".$command." >> ".$tempBjob) ;

    $tempCFG = $jobDir."/bjob_".$jobIt.".py" ;

    $command = "touch ".$tempCFG ;             system ($command) ;

    $command = "import FWCore.ParameterSet.Config as cms " ;  system ("echo ".$command." > ".$tempCFG) ;
    $command = "process = cms.Process\\\(\\\"TEST\\\"\\\) " ;         system ("echo ".$command." >> ".$tempCFG) ;
    $command = "process.Input = cms.PSet\\\(" ;                  system ("echo ".$command." >> ".$tempCFG) ;
    $command = "WP = cms.untracked.int32\\\(".$WP."\\\)," ;       system ("echo ".$command." >> ".$tempCFG) ;
    $command = "   inputFileList = cms.vstring\\\(" ;          system ("echo ".$command." >> ".$tempCFG) ;



    ######################
    # make job files too
    ######################    
    
    $tempBjob = $jobDir."/bjob_".$jobIt.".sh" ;
    
    $command = "touch ".$tempBjob ;             system ($command) ;
    $command = "chmod 777 ".$tempBjob ;         system ($command) ;    
    
    $command = "cd ".$BASEDir." ";                  system ("echo ".$command." > ".$tempBjob) ;
    $command = "eval \\\`scramv1 runtime -sh\\\`" ;       system ("echo ".$command." >> ".$tempBjob) ;
    $command = "df -h ";                            system ("echo ".$command." >> ".$tempBjob) ;
    $command = "cd - ";                             system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rm -rf amassiro/ " ;                system ("echo ".$command." >> ".$tempBjob) ;
    $command = "mkdir amassiro " ;                  system ("echo ".$command." >> ".$tempBjob) ;
    $command = "mkdir amassiro/".$sample." " ;      system ("echo ".$command." >> ".$tempBjob) ;
    $command = "cd amassiro/".$sample." " ;         system ("echo ".$command." >> ".$tempBjob) ;



    $it = 0;
    open (LISTOFFiles,$LISTOFFiles) ;
    while (<LISTOFFiles>)
      {
        chomp; 
        s/#.*//;                # no comments
        s/^\s+//;               # no leading white
        s/\s+$//;               # no trailing white
        $file = $_ ;
        if( ($it >= ($jobIt - 1)*$JOBModulo) && ($it < ($jobIt)*$JOBModulo) )
          {
            if ($it < (($jobIt)*$JOBModulo-1)) 
              {
              $command = "'".$file."'," ;          system ("echo ".$command." >> ".$tempCFG) ;
              }
              else {
                $command = "'".$file."'," ;          system ("echo ".$command." >> ".$tempCFG) ;
                }
          $command = "rfcp ".$INPUTSAVEPath."/".$sample."/".$_." ./ " ;          system ("echo ".$command." >> ".$tempBjob) ;
          }
        ++$it;
      }

    $command = "   \\\)," ;                                    system ("echo ".$command." >> ".$tempCFG) ;
    $command = "    outputRootFileName = cms.string\\\(\\\"ECALAlignMC_".$sample."_".$jobIt.".root\\\"\\\)" ;   system ("echo ".$command." >> ".$tempCFG) ;
    $command = "  \\\)" ;                                      system ("echo ".$command." >> ".$tempCFG) ;

    
    $command = "cp ".$tempCFG." ./ " ;                                     system ("echo ".$command." >> ".$tempBjob) ;
    $command = "ls " ;                                                     system ("echo ".$command." >> ".$tempBjob) ;
    $command = "WenuAnalysisNtple_Alignment bjob_".$jobIt.".py " ;         system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rfmkdir ".$OUTPUTSAVEPath."/".$sample." " ; system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rfcp ECALAlignMC_".$sample."_".$jobIt.".root ".$OUTPUTSAVEPath."/".$sample." " ; system ("echo ".$command." >> ".$tempBjob) ;
    $command = "cd ../../ " ;                                  system ("echo ".$command." >> ".$tempBjob) ;
    $command = "rm -rf amassiro/" ;                              system ("echo ".$command." >> ".$tempBjob) ;
    
    ############
    # submit job
    ############
    
    print ("\n submit \n");
    $command = "bsub -q '1nh' ".$tempBjob." \n" ;      
    print ($command);
    system ($command);
    
    print "" ;
  }
 system ("rm ".$LISTOFFiles) ;
 }
}  

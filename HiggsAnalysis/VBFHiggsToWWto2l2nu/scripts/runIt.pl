#!/usr/bin/perl

use Math::Trig ;


# to create cfg files from templates + informations
# $Id: runIt.pl,v 1.4 2008/04/11 15:32:42 govoni Exp $

sub makecfg  
{
  #PG @_ is the arguments vector
  #PG $_[i] are the arguments
  #PG $#_ may be the number of arguments 
  $localTemplateConfig = $_[0] ;
  $configName = $_[1] ;
  $localInputFileNames = $_[2] ;
  $localOutputFileName = $_[3] ;
  $localOutputReportName = $_[4] ;
  $localOutputLogName = $_[5] ;

  $localInputFileNames =~ s/"/\\"/g ;
  $localInputFileNames =~ s/ /\\ /g ;
  
  system ("cat $localTemplateConfig | sed -e s%FILELIST%".$localInputFileNames."%g > tempo") ;
  system ("mv tempo $configName") ;
  system ("cat $configName | sed -e s%OUTPUTFILE%".$localOutputFileName."%g > tempo") ;
  system ("mv tempo $configName") ;
  system ("cat $configName | sed -e s%OUTPUTREPORT%".$localOutputReportName."%g > tempo") ;
  system ("mv tempo $configName") ;
  system ("cat $configName | sed -e s%OUTPUTLOG%".$localOutputLogName."%g > tempo") ;
  system ("mv tempo $configName") ;

#  print "prepared ".$configName
#        ." from ".$localTemplateConfig
#        ." <> ".$localInputFileNames
#        ." <> ".$localOutputFileName
#        ." <> ".$localOutputReportName
#        ." <> ".$localOutputLogName."\n" ;
}


# ------------------------------------------------------------------------------------


sub makeMIBjob  
{
  #PG @_ is the arguments vector
  #PG $_[i] are the arguments
  #PG $#_ may be the number of arguments 
  $scriptName = $_[0] ;
  $configName = $_[1] ;
  $queue = $_[2] ;
  $results = $_[3] ;
  $outputFolder = $_[4] ;

  system ("rm -f ".$scriptName) ;
  open (SCRIPT,">".$scriptName) or die "Cannot open ".$scriptName." to write the config file" ;
  print SCRIPT "cmsRun ".$configName."\n" ;
  print SCRIPT "mv ".$results." ".$outputFolder."\n" ;

  system ("chmod 755 ".$scriptName."\n");
  print "JOB     | qsub -V -q ".$queue." ".$scriptName."...\n" ;
  system ("qsub -V -q ".$queue." ".$scriptName."\n");


}


# ------------------------------------------------------------------------------------


sub makeInputCfg
{
  #PG @_ is the arguments vector
  #PG $_[i] are the arguments
  #PG $#_ may be the number of arguments 
  $filePrefix = $_[0] ;
  $numberOfFiles = $_[1] ;
  $resultsFolder = $_[2] ;

  $inputCfgName = $resultsFolder."/".$filePrefix."_inputFile.cfi" ;  
  print "OUTPUT  | ".$inputCfgName."\n" ;  
  open (INPUTCFG,">".$inputCfgName) or die "Cannot open ".$inputCfgName." to write the config file" ;
  print INPUTCFG "source = PoolSource\n" ;
  print INPUTCFG "{\n" ;
  print INPUTCFG "    untracked vstring fileNames =\n" ;
  print INPUTCFG "    {\n" ;

  for ($index = 0 ; $index < $numberOfFiles ; $index++) 
    {
      print INPUTCFG "\"file:".$resultsFolder."/".$fileprefix."_output_".$index.".root\"\n" ;
    }  

  print INPUTCFG "  }}\n" ;
    
    
}


# ====================================================================================
#     M A I N      P R O G R A M
# ====================================================================================


# read a trivial config file
open (USERCONFIG,$ARGV[0]) ;
#print $ARGV[0]." opened\n" ;

while (<USERCONFIG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);
    $User_Preferences{$var} = $value;
    print "CONFIG  | ".$var." = ".$value."\n" ;
  }

#PG full paths
$templateConfig = $User_Preferences{"templateConfig"} ;
$datasetFile = $User_Preferences{"datasetFile"} ;
$inputFilesPerRun = $User_Preferences{"inputFilesPerRun"} ;
$queue = $User_Preferences{"queue"} ;
$workingFolder = $User_Preferences{"workingFolder"} ;
$resultsMainFolder = $User_Preferences{"resultsFolder"} ;
$fileprefix = $User_Preferences{"fileprefix"} ;

if (! (-e $resultsMainFolder))
{
  mkdir $resultsMainFolder ;    
}

if (! (-e $workingFolder))
{
  mkdir $workingFolder ;    
}

#PG read the input config file and prepare a vector of root files
@datasets = () ;
$dataset = "" ;
$counter = 0 ;
open (DATAFILE,$datasetFile) or die "cannot open ".$datasetFile."\n" ;
#PG loop over data cfg file
while (<DATAFILE>)
  {
    chomp ;
    s/#.*//;                # no comments
    #PG look for a single root file (one per line)
    if ($_ =~ m/.root/)
      {
        s/,// ;                #PG no commas
        s/ //g ;                #PG no spaces
        ++$counter ;
        $dataset .= $_." " ;
        if ($counter == $inputFilesPerRun)
          {
            push (@datasets, $dataset) ;
            $dataset = "" ;
            $counter = 0 ;
          }
      } #PG look for a single root file (one per line)
  } #PG loop over data cfg file
if ($counter < $inputFilesPerRun)
  {
    push (@datasets, $dataset) ;
  }

$dataset = "" ;
$counter = 0 ;
#PG loop on the vector of datasets
foreach $dataset (@datasets) 
  {
    $inputFiles = $dataset ;
    $inputFiles =~ s/ /, /g ;
    chop ($inputFiles) ;  
    chop ($inputFiles) ;  
    
    $cfgfilename = $workingFolder."/".$fileprefix."_".$counter.".cfg" ;
#    $outputfilename = $workingFolder."/".$fileprefix."_output_".$counter.".root" ;
#    $reportfilename = $workingFolder."/".$fileprefix."_report_".$counter ;
#    $logfilename = $workingFolder."/".$fileprefix."_log_".$counter ;
    $reportfilename = $fileprefix."_report_".$counter ;
    $logfilename = $fileprefix."_log_".$counter ;
    $outputfilename = $fileprefix."_output_".$counter.".root" ;
    
    makecfg ($templateConfig,$cfgfilename,$inputFiles,
             $outputfilename,$reportfilename,$logfilename) ;

    $scriptfilename = $workingFolder."/".$fileprefix."_".$counter.".sh" ;

    $results = $reportfilename.".log ".$logfilename.".log ".$outputfilename ;
    makeMIBjob ($scriptfilename, $cfgfilename, $queue, $results, $resultsMainFolder) ;
    ++$counter ;
  } #PG loop on the vector of datasets

makeInputCfg ($fileprefix, $counter, $resultsMainFolder) ;

print "SUMMARY | ".$counter." jobs launched\n".
      "SUMMARY | working in ".$workingFolder."\n".
      "SUMMARY | results in ".$resultsMainFolder."\n" ;

  
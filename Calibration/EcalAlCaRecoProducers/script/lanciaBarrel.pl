#!/usr/bin/perl

use Math::Trig;

# ----------------------------------------------------------------------------


sub makecfg  
{
  #PG @_ is the arguments vector
  #PG $_[i] are the arguments
  #PG $#_ may be the number of arguments 
  $localEtaIndexStart = $_[0] ;
  $localEtaIndexEnd = $_[1] ;
  $localEtaIndexWidth = $_[2] ;
  $localPhiIndexStart = $_[3] ;
  $localPhiIndexEnd = $_[4] ;
  $localPhiIndexWidth = $_[5] ;
  $localFilenames = $_[6] ;
  $localTemplateconfig = $_[7] ;
  $localMaxEventsPerCrystal = $_[8] ;

  print "running ".$localEtaIndexStart
        ." <> ".$localEtaIndexEnd
        ." <> ".$localEtaIndexWidth
        ." <> ".$localFilenames
        ." <> ".$localTemplateconfig."\n" ;

  $configName = "barrelCalib_".$localEtaIndexStart."-".$localEtaIndexEnd.".cfg" ; #FIXME cambiato qui

  system("cat $localTemplateconfig | sed -e s%FILELIST%".$localFilenames."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%ETAWIDTH%".$localEtaIndexWidth."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%ETASTART%".$localEtaIndexStart."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%ETAEND%".$localEtaIndexEnd."%g > tempo") ;

  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHISTARTEB%".$localPhiIndexStart."%g > tempo") ; #FIXME
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIENDEB%".$localPhiIndexEnd."%g > tempo") ; #FIXME
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIWIDTHEB%".$localPhiIndexWidth."%g > tempo") ; #FIXME
  system("mv tempo $configName");

  system("cat $configName | sed -e s%MAXEVENTSPERCRYSTAL%".$localMaxEventsPerCrystal."%g > tempo") ; #FIXME
  system("mv tempo $configName");

  #PG from here the endcap, such that no regions are created
  system("cat $configName | sed -e s%RADSTART%30%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%RADEND%30%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%RADWIDTH%5%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHISTARTEE%0%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIENDEE%0%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIWIDTHEE%5%g > tempo") ;
  system("mv tempo $configName");
    
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
$etaIndexStart = $User_Preferences{"IMAEBetaStart"} ;
$etaIndexEnd = $User_Preferences{"IMAEBetaEnd"} ;
$etaIndexWidth = $User_Preferences{"IMAEBetaWidth"} ;
$phiIndexStart = $User_Preferences{"IMAEBphiStart"} ;
$phiIndexEnd = $User_Preferences{"IMAEBphiEnd"} ;
$phiIndexWidth = $User_Preferences{"IMAEBphiWidth"} ;
$eventsNumPerCrystal = $User_Preferences{"IMAMaxNumPerCrystal"} ;
$queue = $User_Preferences{"queue"} ;
$dbfile = $User_Preferences{"IMAdbfile"} ;
$fileprefix = $User_Preferences{"IMAfileprefix"} ;
$maxEventsPerCrystal = $User_Preferences{"IMAMaxEventsPerCrystal"} ;
$coeffFolder = $User_Preferences{"coeffFolder"} ;
$resultsMainFolder = $User_Preferences{"resultsFolder"} ;

if (! (-e $resultsMainFolder))
{
  mkdir $resultsMainFolder ;    
}


#PG da fare
#PG -------
#PG - rimanda indietro i coefficienti
#PG - l'output salvato da qualche parte?
#PG - max num per evento


#PG loop over eta
#PG the calibration is lanched on a largr area, to then retrieve the coefficients 
#PG in its central part only
$alreadySwitched = 0 ;
for ($etaIndex=$etaIndexStart ; $etaIndex < $etaIndexEnd ; )
{
  #PG find min and max in the eta domain
  $etaMin = ($etaIndex - 2 * $etaWidth - 1) * 0.0175 ;
  print "STEP etaIndex : ".$etaIndex." eta value : ".$etaMin."\n" ;
  $oldOne = $etaIndex ; 
  $etaIndex += $etaIndexWidth ;
  #PG to cope with the missing 0
  if (($alreadySwitched == 0) && ($etaIndex * $etaIndexStart <= 0)) 
    {  
      $etaIndex += 1 ;
      $alreadySwitched = 1 ; 
    }  
  $etaMax = ($etaIndex + 2 * $etaWidth + 1) * 0.0175 ;
  print "     etaIndex : ".$etaIndex." eta value : ".$etaMax."\n" ;
  # look for the root files in the txt DB file
#  $inputfiles = "fileNames = ";
  $inputfiles = "" ;
  open (DBFILE,$dbfile) ;
  while (<DBFILE>) 
    {
      #print "Line $. is : $_";
      @linea = split (/ /,$_) ;  
      chomp ($linea[2]) ;
#      print $linea[0]."\n";
      if ($linea[0] < $etaMax && $linea[1] > $etaMin)
        { 
          #PG the number of \\\ is necessary for the shell command in the mackeConfig sub
          $inputfiles .= "\\\'".$fileprefix.$linea[2]."\\\'," ;
#          print "    ".$etaMin." ".$etaMax." <- ".$_." <- ".$linea[2]."\n" ;
        }
    }
  close (DBFILE) ;    
  chop ($inputfiles) ; #PG get rid of the last comma
  print "   insomma:\n   ".$inputfiles."\n" ; 
  $effEtaIndexStart = $oldOne - $etaIndexWidth ;
  if ($effEtaIndexStart * $oldOne <= 0) { $effEtaIndexStart-- } ;
  if ($effEtaIndexStart < -85) { $effEtaIndexStart = -85 ; }
  $effEtaIndexEnd = $etaIndex + $etaIndexWidth ;
  if ($effEtaIndexEnd * $etaIndex <= 0) { $effEtaIndexEnd++ } ;
  if ($effEtaIndexEnd > 86) { $effEtaIndexEnd = 86 ; }
  makecfg ($effEtaIndexStart, $effEtaIndexEnd, $etaIndexWidth, 
           $phiIndexStart ,$phiIndexEnd, $phiIndexWidth,
           $inputfiles, $templateConfig, $maxEventsPerCrystal) ;                                                                 

  #PG crea e lancia il job al batch system
  $folder = `pwd` ;
  $folder =~s/\n//;
  $resultsfolder = $resultsMainFolder."/calibResult_".$oldOne."_".$etaIndex ;
  $filename = "til_EBcalib_".$etaIndex."_calib.sh" ; # FIXME cambiato qui il nome
  $jobfoldername = "/tmp/IMAEBcalib_".$etaIndex."_".time () ;
#  while (-d $dir) { $dir .= "I" ; }
  open JOBFILE, ">".$filename ;
  print JOBFILE "\#! /bin/sh\n" ;
  print JOBFILE "cd $folder\n" ;
  print JOBFILE "eval `scramv1 run -sh`\n" ;
  print JOBFILE "cd -\n" ;
#  print JOBFILE "mkdir ".$jobfoldername."\n" ; #FIXME
#  print JOBFILE "cd ".$jobfoldername."\n" ; #FIXME
  print JOBFILE "cmsRun ".$folder."/".$configName."\n" ;  
  print JOBFILE "ls -latr\n" ;  
  print JOBFILE "mkdir ".$resultsfolder."\n" ;
  print JOBFILE "mv *root ".$resultsfolder."\n" ;
  print JOBFILE "cp *xml ".$resultsfolder."\n" ;
  #PG in realta di questi due ne serve uno solo
#  print JOBFILE "mv EErecalib.xml ".$coeffFolder."/EEcalibCoeff_".$oldOne."-".$etaIndex.".xml\n" ;  
  print JOBFILE "mv EBrecalib.xml ".$coeffFolder."/EBcalibCoeff_".$oldOne."-".$etaIndex.".xml\n" ;  
  close JOBFILE ;
  system ("chmod 755 ".$filename) ;
  print "bsub -q ".$queue." ".$filename."\n" ;
  system("bsub -q ".$queue." ".$filename);


} #PG loop over eta




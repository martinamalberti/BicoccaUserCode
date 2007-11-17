#!/usr/bin/perl

use Math::Trig;

# ----------------------------------------------------------------------------


sub makecfg  
{
  #PG @_ is the arguments vector
  #PG $_[i] are the arguments
  #PG $#_ may be the number of arguments 
  $localRadIndexStart = $_[0] ;
  $localRadIndexEnd = $_[1] ;
  $localRadIndexWidth = $_[2] ;
  $localPhiIndexStart = $_[3] ;
  $localPhiIndexEnd = $_[4] ;
  $localPhiIndexWidth = $_[5] ;
  $localFilenames = $_[6] ;
  $localTemplateconfig = $_[7] ;
  $localMaxEventsPerCrystal = $_[8] ;

  print "running ".$localRadIndexStart
        ." <> ".$localRadIndexEnd
        ." <> ".$localRadIndexWidth
        ." <> ".$localFilenames
        ." <> ".$localTemplateconfig."\n" ;

  $configName = "endcapCalib_".$localRadIndexStart."-".$localRadIndexEnd.".cfg" ; #FIXME modificato qui

  system("cat $localTemplateconfig | sed -e s%FILELIST%".$localFilenames."%g > tempo") ;
  system("mv tempo $configName");

  #PG from here the barrel, such that no regions are created
  system("cat $configName | sed -e s%ETAWIDTH%5%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%ETASTART%10%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%ETAEND%10%g > tempo") ;

  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHISTARTEB%0%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIENDEB%0%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIWIDTHEB%5%g > tempo") ; 
  system("mv tempo $configName");

  system("cat $configName | sed -e s%MAXEVENTSPERCRYSTAL%".$localMaxEventsPerCrystal."%g > tempo") ; 
  system("mv tempo $configName");

  system("cat $configName | sed -e s%RADSTART%".$localRadIndexStart."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%RADEND%".$localRadIndexEnd."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%RADWIDTH%".$localRadIndexWidth."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHISTARTEE%".$localPhiIndexStart."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIENDEE%".$localPhiIndexEnd."%g > tempo") ;
  system("mv tempo $configName");
  system("cat $configName | sed -e s%PHIWIDTHEE%".$localPhiIndexWidth."%g > tempo") ;
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
$radIndexStart = $User_Preferences{"IMAEEradStart"} ;
$radIndexEnd = $User_Preferences{"IMAEEradEnd"} ;
$radIndexWidth = $User_Preferences{"IMAEEradWidth"} ;
$phiIndexStart = $User_Preferences{"IMAEEphiStart"} ;
$phiIndexEnd = $User_Preferences{"IMAEEphiEnd"} ;
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


# --------------------------------

#$prima = $ARGV[0];

# $templateConfig = $ARGV[0] ;
# $castorfolder = $ARGV[1] ;
# $radIndexStart =  $ARGV[2] ;
# $radIndexEnd =  $ARGV[3] ;
# $radIndexWidth =  $ARGV[4] ;
# $phiStart = 0 ;
# $phiEnd = 0 ;
# $phiWidth = 0 ;
# $queue = "8nm" ;


#PG loop over eta
for ($radIndex=$radIndexStart ; $radIndex < $radIndexEnd ; )
{
  #PG find min and max in the eta domain
  $etaMax = -1 * log (tan (atan2 ( 24.7 * (10 + ($radIndex-$radIndexWidth)),3200.)/2)) ;
  print "STEP radIndex : ".$radIndex." eta value : ".$etaMax."\n" ;
  $oldOne = $radIndex ; 
  $radIndex += $radIndexWidth ;
  $etaMin = -1 * log (tan (atan2 ( 24.7 * (10 + ($radIndex+$radIndexWidth)),3200.)/2)) ;
  print "     radIndex : ".$radIndex." eta value : ".$etaMin."\n" ;
  #PG look for the root files in the txt DB file
  $inputfiles = "" ;
  open (DBFILE,$dbfile) ;
  while (<DBFILE>) 
    {
#      print "Line $. is : $_";
      @linea = split (/ /,$_) ;  
      chomp ($linea[2]) ;
#      print $linea[0]."\n";
      if ($linea[0] < $etaMax && $linea[1] > $etaMin)
        {
          $inputfiles .= "\\\'".$fileprefix.$linea[2]."\\\'," ;
#          print "    ".$etaMin." ".$etaMax." <- ".$_." <- ".$linea[2]."\n" ;
        }
    }
  close (DBFILE) ;    
  chop ($inputfiles) ; #PG get rid of the last comma
  print "   insomma:\n   ".$inputfiles."\n" ;     
  
# ---------------------------------
  $effradIndexStart = $oldOne - $radIndexWidth ;
  if ($effradIndexStart < 0) { $effradIndexStart = 0 ; }
  $effradIndexEnd = $radIndex + $radIndexWidth ;
  if ($effradIndexEnd > 50) { $effradIndexEnd = 50 ; }
  makecfg ($effradIndexStart, $effradIndexEnd, $radIndexWidth, 
           $phiIndexStart ,$phiIndexEnd, $phiIndexWidth,
           $inputfiles, $templateConfig, $maxEventsPerCrystal) ;                                                                 

  #PG crea e lancia il job al batch system
  $folder = `pwd` ;
  $folder =~s/\n//;
  $resultsfolder = $resultsMainFolder."/calibResult_".$oldOne."_".$radIndex ;
  $filename = "til_EEcalib_".$radIndex."_calib.sh" ;
  $jobfoldername = "/tmp/IMAEBcalib_".$radIndex."_".time () ;
  open JOBFILE, ">".$filename ;
  print JOBFILE "\#! /bin/sh\n" ;
  print JOBFILE "cd $folder\n" ;
  print JOBFILE "eval `scramv1 run -sh`\n" ;
  print JOBFILE "cd -\n" ;
#  print JOBFILE "mkdir ".$jobfoldername."\n" ;
#  print JOBFILE "cd ".$jobfoldername."\n" ;
  print JOBFILE "cmsRun ".$folder."/".$configName."\n" ;  
  print JOBFILE "ls -latr\n" ;  
  print JOBFILE "mkdir ".$resultsfolder."\n" ;
  print JOBFILE "mv *root ".$resultsfolder."\n" ;
  print JOBFILE "cp *xml ".$resultsfolder."\n" ;
  #PG in realta di questi due ne serve uno solo
  print JOBFILE "mv EErecalib.xml ".$coeffFolder."/EEcalibCoeff_".$oldOne."-".$radIndex.".xml\n" ;  
#  print JOBFILE "mv EBrecalib.xml ".$coeffFolder."/EBcalibCoeff_".$oldOne."-".$radIndex.".xml\n" ;  
  close JOBFILE ;
  system ("chmod 755 ".$filename) ;
  print "bsub -q ".$queue." -u pietro.govoni@gmail.com ".$filename."\n" ;
  system("bsub -q ".$queue." -u pietro.govoni@gmail.com ".$filename);
                                                               
} #PG loop over eta




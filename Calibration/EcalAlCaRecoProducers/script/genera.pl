#!/usr/bin/perl

#$prima = $ARGV[0];
#PG lettura dei parametri da cfg file
#PG --------------------------------

open (USERCONFIG,$ARGV[0]) ;

while (<USERCONFIG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);    $User_Preferences{$var} = $value;  }

$templateConfig = $User_Preferences{"templateConfig"} ;
$castorfolder = $User_Preferences{"castorfolder"} ;
$castorlogs = $User_Preferences{"castorlogs"} ;
$etaStart = $User_Preferences{"etaStart"} ;
$etaEnd = $User_Preferences{"etaEnd"} ;
$etaWidth = $User_Preferences{"etaWidth"} ;
$phiStart = $User_Preferences{"phiStart"} ;
$phiEnd = $User_Preferences{"phiEnd"} ;
$phiWidth = $User_Preferences{"phiWidth"} ;
$queue = $User_Preferences{"queue"} ;

$originalSourceFile = $User_Preferences{"originalSourceFile"} ;
$numberOfSouces = $User_Preferences{"numberOfSources"} ;
$sources = $User_Preferences{"sourcesFolder"} ;
$configFolder = $User_Preferences{"configFolder"} ;
$scriptFolder = $User_Preferences{"scriptFolder"} ;

#PG upgrade futuri
#PG ---------------
#PG - numero di eventi
#PG - collezione sulla quale girare
#PG - phimin, phimax, dphi
#PG - come si fa a stampare tutte le opzioni?
#PG - per controllare se un'opzione manca, si fa $User_Preferences{"opzione"} == ""
#PG - inserire anche i tagli energetici
#PG - far mettere script e cmssw-cfg files in sotto-cartelle per ordine

if (! (-e $configFolder))
{
  mkdir $configFolder ;    
}

if (! (-e $scriptFolder))
{
  mkdir $scriptFolder ;    
}

$myCMSSW = $ENV{CMSSW_BASE} ;
$myCMSSW .= "/src/" ;
#print "DIOBONO ".$myCMSSW."\n" ;

#PG loop over the sub-sections of the source
for ($section = 0 ; $section < $numberOfSouces ; $section++)
  {

    @path = split (/\//, $originalSourceFile) ;
  #  print $path[@path-1]."\n" ;
    #PG trovo la desinenza
    @pieces = split (/\./, $originalSourceFile) ;
    #PG prepare the output file name
    $nomeSource = $path[@path-1] ;
    $piece = "\.".$pieces[@pieces-1] ;
    $nomeSource =~ s/$piece/_$section$piece/ ;
    $nomeSource = $sources."/".$nomeSource ;
    $nomeSource =~ s/$myCMSSW// ;

    #PG loop over eta
    for ($eta=$etaStart ; $eta < $etaEnd ;)
    {
      print "etaStart = ".$eta ;
      #PG create the name of the new cfg file
      $configName = $configFolder."/genera_config_".$section."_".$eta.".cfg" ;
      #PG FIXME devo cambiare anche il sorgente <- che cosa vuol dire?
      #PG change che eta start and end
      system("cat $templateConfig | sed -e 's/ETASTART/$eta/g' > $configName");
      $eta += $etaWidth ;
      system("cat $configName | sed -e 's/ETAEND/$eta/g' > tempo");
      system("mv tempo $configName");
      print ", etaEnd = ".$eta."\n" ;
      system("cat $configName | sed -e 's%DATALISTFILE%$nomeSource%g' > tempo");
      system("mv tempo $configName");
      #PG change the ouptut file name
      $outfilename = "preSelected_".$section."_".($eta-$etaWidth)."_".$eta."_.root" ;  
      system("cat $configName | sed -e 's%OUTFILE%".$outfilename."%g' > tempo");
      system("mv tempo $configName");
      #PG crea e lancia il job al batch system
      $folder = `pwd` ;
      $folder =~s/\n//;
      $filename = $scriptFolder."/til_".$section."_".$eta."_select.sh" ;
      $logfilename = "log_".$section."_".$eta."_.txt" ;      
      open JOBFILE, ">".$filename ;
      print JOBFILE "\#! /bin/sh\n" ;
      print JOBFILE "cd $folder\n" ;
      print JOBFILE "eval `scramv1 run -sh`\n" ;
      print JOBFILE "export STAGE_SVCCLASS=cmscaf\n";
      print JOBFILE "cd -\n" ;
#      print JOBFILE "cmsRun ".$folder."/".$configName."\n" ;  
      print JOBFILE "cmsRun ".$configName." >& ".$logfilename."\n" ;  
      print JOBFILE "rfcp $outfilename $castorfolder\n" ;  
      print JOBFILE "rfcp $logfilename $castorlogs\n" ;  
      close JOBFILE ;
      system("chmod 755 ".$filename);
      print "bsub -q ".$queue." ".$filename."\n";
      system("bsub -q ".$queue." ".$filename);
    } #PG loop over eta
  } #PG loop over the sub-sections of the source


# --------------------------------------------------------------------
# $templateConfig = $ARGV[0] ;
# $castorfolder = $ARGV[1] ;
# $etaStart =  $ARGV[2] ;
# $etaEnd =  $ARGV[3] ;
# $etaWidth =  $ARGV[4] ;
# $phiStart = 0 ;
# $phiEnd = 0 ;
# $phiWidth = 0 ;
# $queue = "dedicated -R cmscaf" ;


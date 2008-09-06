#!/usr/bin/perl

#PG put here a function to create the new SourceFiles

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

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
    my ($var, $value) = split(/\s*=\s*/, $_, 2);
    $User_Preferences{$var} = $value;
  }


#$sourceCfgFile = $User_Preferences{"sourceCfgFile"} ;
#$numberOfSouces = $User_Preferences{"numberOfSources"} ;
$sources = $User_Preferences{"sources"} ;
$sourceCfgFile = $User_Preferences{"sourceCfgFile"} ;
$templateCfgFile = $User_Preferences{"templateCfgFile"} ;
$castorFolder = $User_Preferences{"castorFolder"} ;
$CMSSWFolder = $User_Preferences{"CMSSWFolder"} ;
$runNb = $User_Preferences{"runNb"} ;

#print $sourceCfgFile."\n" ;

# PG prepare the array containing the root files list

open (SOURCECFG,$sourceCfgFile) ;
$entry = 0 ;
@Input_Root_Files ;
#PG loop on the original SourceFile
while (<SOURCECFG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    $where = index( $_,"root");
    if ($where > 0) 
      {  
#        print $_."\n" ;
        push (@Input_Root_Files, $_) ;
        
         # print $entry." -> ".$Input_Root_Files[$entry]."\n" ;
        $entry++ ;
      }
  } #PG loop on the original SourceFile


#PG prepare the folders where to put the cfg files
if (! (-e $sources))
{
  mkdir $sources ;    
}


#PG loop over the array of sources
#PG prepare the cmssw config file 
#PG and the bsub job
for ($source = 0 ; $source < @Input_Root_Files ; $source++)
  {
    @path = split (/\//, $Input_Root_Files[$section]) ;
    print $path[@path-1]."\n" ;
    #PG trovo la desinenza
    @pieces = split (/\./, $sourceCfgFile) ;
    #PG prepare the output file name
    $nomeFile = $path[@path-1] ;

    $cfgFileName = $nomeFile ;
    $cfgFileName =~ s/root/cfg/ ;
    $cfgFileName =~ s/'// ;
    $cfgFileName = $sources."/".$cfgFileName ;

    $jobFileName = $nomeFile ;
    $jobFileName =~ s/root/job/ ;
    $jobFileName =~ s/'// ;
    $jobFileName = $sources."/".$jobFileName ;

    $ntupleFileName = $nomeFile ;
    $ntupleFileName =~ s/root/ntuple/ ;
    $ntupleFileName =~ s/'// ;

    print " | ".$nomeFile."\n" ;
    print " | ".$cfgFileName."\n" ;
    print " | ".$jobFileName."\n" ;
    print " | ".$ntupleFileName."\n" ;

    $tempo = $sources."/tempo" ;
    system ("cat ".$templateCfgFile." | sed -e s%FILENAME_PLACEHOLDER%".$Input_Root_Files[$section]."% > ".$tempo) ;
    $tempo2 = $sources."/tempo2" ;
    system ("cat ".$tempo." | sed -e s%NTUPLENAME_PLACEHOLDER%".$ntupleFileName."% > ".$tempo2) ;
    $tempo3 = $sources."/tempo3" ;
    $runNb = 9999 ;
    system ("cat ".$tempo2." | sed -e s%RUNNB_PLACEHOLDER%".$runNb."% > ".$tempo3) ;
    system ("mv ".$tempo3." ".$cfgFileName) ;
    system ("rm ".$sources."/tempo*") ;

    open (JOBFILE, ">".$jobFileName);
    print JOBFILE "#!/bin/bash\n" ;
    print JOBFILE "cd ".$CMSSWFolder."\n" ;
    print JOBFILE "cmsenv\n" ;
    print JOBFILE "cd -\n" ;
    print JOBFILE "cmsRun ".$cfgFileName."\n" ;
    print JOBFILE "ls\n" ;
    print JOBFILE "rfcp ".$ntupleFileName."-".$runNb.".tree.root ".$castorFolder."\n" ;
    close (JOBFILE);
    system ("chmod 755 ".$jobFileName."\n") ;

    print ("bsub -q cmsprs ".$jobFileName."\n") ;
    system ("bsub -q cmsprs ".$jobFileName) ;

  } #PG loop over the array of sources



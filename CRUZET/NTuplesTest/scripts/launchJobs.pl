#!/usr/bin/perl

#PG put here a function to create the new SourceFiles

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

$THISDir = `pwd`; chomp $THISDir ; $THISDir = $THISDir."/" ;
$RUNLISTFile = $User_Preferences{"RUNLISTFile"} ;
$CMSSWCfgTemplate = $User_Preferences{"CMSSWCfgTemplate"} ;
$CRABCfgTemplate = $User_Preferences{"CRABCfgTemplate"} ;
$CASTORSAVEFolder = $User_Preferences{"CASTORSAVEFolder"} ;
$CASTORFULLPath = "/castor/cern.ch".$CASTORSAVEFolder ;

print "THISDir = ".$THISDir."\n" ;
print "RUNLISTFile = ".$RUNLISTFile."\n" ;
print "CMSSWCfgTemplate = ".$CMSSWCfgTemplate."\n" ;
print "CRABCfgTemplate = ".$CRABCfgTemplate."\n" ;
print "CASTORSAVEFolder = ".$CASTORSAVEFolder."\n" ;




# PG prepare the array containing the root files list

open (RUNLIST,$RUNLISTFile) ;
#PG loop on the original SourceFile
while (<RUNLIST>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    $runNumber = $_ ;
    print "\n******************* runNumber = ".$runNumber." *******************\n" ;  


    $CRABWORKINGDir = "CRAB_".$runNumber."/" ;

    $CMSSWWORKINGDir = "CMSSW_".$runNumber."/" ;
    print ("mkdir ".$CMSSWWORKINGDir."\n") ;
    system("mkdir ".$CMSSWWORKINGDir."\n") ;








    $CMSSWCfgFile = $CMSSWWORKINGDir."ecalCosmicsTree_".$runNumber.".cfg.py" ; 
    system ("cat ".$CMSSWCfgTemplate." | sed -e s%RUNNUMBER%".$_."%g > ".$CMSSWCfgFile) ;






    $tempo = "./tempo" ;
    system ("cat ".$CRABCfgTemplate." | sed -e s%RUNNUMBER%".$_."%g > ".$tempo) ;

    $tempo2 = "./tempo2" ;
    system ("cat ".$tempo." | sed -e s%CMSSWPARAMETERSET%".$THISDir.$CMSSWCfgFile."% > ".$tempo2) ;

    $tempo3 = "./tempo3" ;
    system ("cat ".$tempo2." | sed -e s%SAVEFOLDER%".$CASTORSAVEFolder.$runNumber."% > ".$tempo3) ;

    $tempo4 = "./tempo4" ;
    system ("cat ".$tempo3." | sed -e s%UIWORKINGDIR%".$CRABWORKINGDir."% > ".$tempo4) ;

    $CRABCfgFile = "crab_".$runNumber.".cfg" ; 
    system ("mv ".$tempo4." ".$CRABCfgFile) ;
    system ("rm ./tempo*") ;
    
    print ("rfmkdir ".$CASTORFULLPath.$runNumber."\n") ; 
    system ("rfmkdir ".$CASTORFULLPath.$runNumber."\n") ;
    print ("rfchmod 777 ".$CASTORFULLPath.$runNumber."\n") ;    
    system ("rfchmod 777 ".$CASTORFULLPath.$runNumber."\n") ;    
    print ("crab -create -submit -cfg ".$CRABCfgFile."\n") ;
    system ("crab -create -submit -cfg ".$CRABCfgFile."\n") ;
    print ("mv ".$CRABCfgFile." ".$CMSSWWORKINGDir."\n") ;
    system ("mv ".$CRABCfgFile." ".$CMSSWWORKINGDir."\n") ;
 
    print "\n" ;
    
  } #PG loop on the original SourceFile

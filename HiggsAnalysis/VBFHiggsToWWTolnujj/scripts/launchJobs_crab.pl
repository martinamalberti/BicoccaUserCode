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


$DATASETPath         = $User_Preferences{"DATASETPath"} ;
$EVENTType           = $User_Preferences{"EVENTType"} ;
$TOTALNumberOfEvents = $User_Preferences{"TOTALNumberOfEvents"} ;
$EVENTSPerJob        = $User_Preferences{"EVENTSPerJob"} ;
$CMSSWCfgTemplate    = $User_Preferences{"CMSSWCfgTemplate"} ;
$CRABCfgTemplate     = $User_Preferences{"CRABCfgTemplate"} ;
$OUTPUTSAVEPath      = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTSAVEFolder    = $User_Preferences{"OUTPUTSAVEFolder"} ;
$OUTPUTFileName      = $User_Preferences{"OUTPUTFileName"} ;

print "DATASETPath = "        .$DATASETPath."\n" ;
print "EVENTType = "          .$EVENTType."\n" ;
print "TOTALNumberOfEvents = ".$TOTALNumberOfEvents."\n" ;
print "EVENTSPerJob = "       .$EVENTSPerJob."\n" ;
print "CMSSWCfgTemplate = "   .$CMSSWCfgTemplate."\n" ;
print "CRABCfgTemplate = "    .$CRABCfgTemplate."\n" ;
print "OUTPUTSAVEPath = "     .$OUTPUTSAVEPath."\n" ;
print "OUTPUTSAVEFolder = "   .$OUTPUTSAVEFolder."\n" ;
print "OUTPUTFileName = "     .$OUTPUTFileName."\n" ;


$currDir = `pwd` ;
chomp ($currDir) ;




  

$tempo1 = "./tempo1" ;
system ("cat ".$currDir."/".$CMSSWCfgTemplate."    | sed -e s%OUTPUTFILENAME%".$OUTPUTFileName.
                                               "%g  | sed -e s%OUTPUTFILETYPE%".$OUTPUTFileType.
                                               "%g  | sed -e s%EVENTTYPE%".$EVENTType.
                                               "%g > ".$tempo1) ;
  
$tempo2 = "./tempo2" ;    
system ("cat ".$tempo1." | sed -e s%LISTOFFILES%% > ".$tempo2) ;
  
$CMSSWCfgFile = $currDir."/CMSSW_cfg.py" ;
system ("mv ".$tempo2." ".$CMSSWCfgFile) ;
system ("rm ./tempo*") ;
  
  
  
$tempo1 = "./tempo1" ;
system ("cat ".$currDir."/".$CRABCfgTemplate." | sed -e s%CMSSWCFGFILE%"."/".$CMSSWCfgFile."% > ".$tempo1) ;

$tempo2 = "./tempo2" ;
system ("cat ".$tempo1."   | sed -e s%OUTPUTFILENAME%".$OUTPUTFileName.
                       "%g | sed -e s%OUTPUTFILETYPE%".$OUTPUTFileType.
                       "%g | sed -e s%OUTPUTSAVEFOLDER%".$OUTPUTSAVEPath.$OUTPUTSAVEFolder.
                       "%g | sed -e s%DATASETPATH%".$DATASETPath.
                       "%g | sed -e s%TOTALNUMBEROFEVENTS%".$TOTALNumberOfEvents.
                       "%g | sed -e s%EVENTSPERJOB%".$EVENTSPerJob.
                       "%g > ".$tempo2) ;

$CRABCfgFile = "crab.cfg" ; 
system ("mv ".$tempo2." ".$CRABCfgFile) ;
system ("rm ./tempo*") ;

print ("rfmkdir /castor/cern.ch/".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ; 
system ("rfmkdir /castor/cern.ch/".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ;
print ("rfchmod 777 /castor/cern.ch/".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ;    
system ("rfchmod 777 /castor/cern.ch/".$OUTPUTSAVEPath.$OUTPUTSAVEFolder."\n") ;    
print ("crab -create -submit -cfg ".$CRABCfgFile."\n") ;
system ("crab -create -submit -cfg ".$CRABCfgFile."\n") ;


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

$BASEDir          = $User_Preferences{"BASEDir"};
$LISTOFSamples    = $User_Preferences{"LISTOFSamples"} ;
$EXEName          = $User_Preferences{"EXEName"} ;
$JOBCfgTemplate   = $User_Preferences{"JOBCfgTemplate"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;
$JOBModulo        = $User_Preferences{"JOBModulo"} ;
$Storage          = $User_Preferences{"Storage"} ;
$isDATA           = $User_Preferences{"isDATA"} ;
$dataRun          = $User_Preferences{"dataRun"} ;


print "BASEDir = "          .$BASEDir."\n" ;
print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "EXEName = "          .$EXEName."\n" ;
print "JOBCfgTemplate = "   .$JOBCfgTemplate."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;
print "JOBModulo = "        .$JOBModulo."\n" ;
print "Storage = "          .$Storage."\n" ;
print "isDATA = "           .$isDATA."\n" ;
print "dataRun = "          .$dataRun."\n" ;





$sampleJobListFile = "./lancia.sh";
open(SAMPLEJOBLISTFILE, ">", $sampleJobListFile);


#####################################################
# PG prepare the array containing the root files list
#####################################################


open (LISTOFSamples,$LISTOFSamples) ;
while (<LISTOFSamples>)
{
    system("cd ".$BASEDir."\n");
    
    chomp($_);
    
    ($INPUTSAVEPath,$sample) = split(" ") ;
    $subsample = substr($sample,0,1);
    if($subsample eq "#")
    {
	next;
    }
    
    system ("rm -r ".$sample."\n") ;
    
    print("Sample: ".$sample."\n") ;  
    system ("mkdir ".$sample."\n") ;
    
    #########################
    ### Make List Of File ###
    #########################
 
    $LISTOFFiles = "./list_".$sample.".txt" ;

    if($Storage=~ /eos/) {system ("cmsLs ".$INPUTSAVEPath."/".$sample." | grep root | awk '{print \$5}' > ".$LISTOFFiles."\n") ;}
    if($Storage=~ /castor/)  {system ("rfdir ".$INPUTSAVEPath."/".$sample." | grep root | awk '{print \$9}' > ".$LISTOFFiles."\n") ;}
    
    $totNumber = 0;
    $jobNumber = 0;
  
    open (LISTOFFiles,$LISTOFFiles) ;
    while (<LISTOFFiles>)
    {
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
    
	$tempBjob = $jobDir."/bjob_".$jobIt.".sh" ;
	$command = "touch ".$tempBjob ;
	system ($command) ;
	$command = "chmod 777 ".$tempBjob ;
	system ($command) ;
    


        ###############################
        ##### Create Job cfg File #####
        ###############################  
    
	$tempo1 = "./tempo1" ;
	if($Storage =~ /castor/){ system ("cat ".$JOBCfgTemplate."   | sed -e s%OUTPUTFILENAME%".$OUTPUTFILEName."_".$jobIt.
		                       "%g > ".$tempo1) ;}
	if($Storage =~ /eos/){ system ("cat ".$JOBCfgTemplate."   | sed -e s%OUTPUTFILENAME%"."root://eoscms//eos/cms".$OUTPUTSAVEPath.$sample."/".$OUTPUTFILEName."_".$jobIt.
		                       "%g > ".$tempo1) ;}
    
        
	$it = 0;
	$JOBLISTOFFiles;
    
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
                if($Storage=~/eos/) {$JOBLISTOFFiles = $JOBLISTOFFiles."APICE".$file."APICE,";}
                if($Storage=~/castor/)  {$JOBLISTOFFiles = $JOBLISTOFFiles."APICE"."rfio:".$INPUTSAVEPath."/".$sample."/".$file."APICE,";}
                       
	    }
	    ++$it;
		
	}
	
	
	$tempo2 = "./tempo2" ;    
	system ("cat ".$tempo1." | sed -e s%LISTOFFILES%".$JOBLISTOFFiles."%g > ".$tempo2) ;
	$JOBLISTOFFiles = "" ;


	$tempo3 = "./tempo3" ;
	system ("cat ".$tempo2." | sed -e s%APICE%\\'".
                                "%g | sed -e s%isDATA%".$isDATA.
                                "%g | sed -e s%dataRUN%".$dataRun.
                                "%g > ".$tempo3) ;



    
	$JOBCfgFile = $jobDir."/simpleNtuple_data_cfg.py" ;
	system ("mv ".$tempo3." ".$JOBCfgFile) ;
	system ("rm ./tempo*") ;
    
      
    
    ######################
    # make job files sh
    ######################    
    open (SAMPLEJOBFILE, ">", $tempBjob) or die "Can't open file ".$tempBjob;


    
    $command = "cd ".$BASEDir ;
    print SAMPLEJOBFILE $command."\n";

    $command = "export SCRAM_ARCH=slc5_amd64_gcc462";
    print SAMPLEJOBFILE $command."\n";
  
    $command = "eval `scramv1 runtime -sh`" ;
    print SAMPLEJOBFILE $command."\n";
    
    $command = "cd -" ;
    print SAMPLEJOBFILE $command."\n";
 
    if($Storage=~/castor/){$command = "rfmkdir -p ".$OUTPUTSAVEPath.$sample;}
    if($Storage=~/eos/)   {$command = "cmsMkdir ".$OUTPUTSAVEPath.$sample;}

    print SAMPLEJOBFILE $command."\n";


    $command = "cmsRun ".$JOBCfgFile ;
    print SAMPLEJOBFILE $command."\n";
    
 
    
    if($Storage=~/castor/) {$command = "rfcp ".$OUTPUTFILEName."_".$jobIt.".root ".$OUTPUTSAVEPath.$sample;
                            print SAMPLEJOBFILE $command."\n";
    }

    
    
    ############
    # submit job
    ############
    
    $command = "bsub -cwd ".$jobDir." -q cmscaf1nh ".$tempBjob."\n" ;  
    print SAMPLEJOBLISTFILE $command."\n";
    
    #print "\n" ;
    }

    system ("rm ".$LISTOFFiles) ;
}  

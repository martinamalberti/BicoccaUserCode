#!/usr/bin/perl

# ---------------------------------------------------------------------------
#      MAIN PROGRAM
# ---------------------------------------------------------------------------

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
$RunString        = $User_Preferences{"RunString"} ;
$JOBCfgTemplate   = $User_Preferences{"JOBCfgTemplate"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;
$JOBModulo        = $User_Preferences{"JOBModulo"} ;
$Storage        = $User_Preferences{"Storage"} ;
$ISCAlib        = $User_Preferences{"ISCAlib"} ;


print "BASEDir = "          .$BASEDir."\n" ;
print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "JOBCfgTemplate = "   .$JOBCfgTemplate."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;
print "JOBModulo = "        .$JOBModulo."\n" ;
print "RunString = "        .$RunString."\n" ;





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
    
    ##########################
    ### Make list of file #### 
    ##########################

    $LISTOFFiles = "./list_".$sample.".txt" ;
    if($Storage=~ /eos/ ) {system ("cmsLs ".$INPUTSAVEPath."/".$sample." | grep root | awk '{print \"root://eoscms//eos/cms\" \$5}' > ".$LISTOFFiles."\n") ;}
    if($Storage=~ /castor/){system ("rfdir ".$INPUTSAVEPath."/".$sample." | grep root  | awk '{print \$9}' > ".$LISTOFFiles."\n") ;}
   
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
    

 	$it = 0;
 	$JOBLISTOFFiles;
        
        $LISTFiles = $jobDir."/listFiles.txt";
 	$command = "touch ".$LISTFiles ;
 	system ($command) ;

        open (LISTFiles, ">", $LISTFiles) ;
 	
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
		if($Storage=~/eos/) {print LISTFiles $file."\n";}
                if($Storage=~/castor/) {print LISTFiles $INPUTSAVEPath."/".$sample."/".$file."\n";}
            
 	    }
 	    ++$it;
 	}
	
        ################################
        #### Create Job cfg py File ####
        ################################

        $tempo1 = "./tempo1" ;

        if($Storage =~/castor/) {system ("cat ".$JOBCfgTemplate." | sed -e s%OUTPUTFILENAME%".$OUTPUTFILEName.
                                  "%g | sed -e s%jobIt%".$jobIt.
                                  "%g | sed -e s%OUTPUTPATH%"."/".
                                  "%g | sed -e s%LISTFILES%".$LISTFiles.
                                  "%g | sed -e s%RUN%".$RunString.
                                  "%g | sed -e s%ISCAlib%".$ISCAlib.
				  "%g > ".$tempo1) ;}
 
        if($Storage =~/eos/) {
                 system ("cat ".$JOBCfgTemplate." | sed -e s%OUTPUTFILENAME%".$OUTPUTFILEName.
                                  "%g | sed -e s%OUTPUTPATH%"."./".
                                  "%g | sed -e s%jobIt%".$jobIt.
                                  "%g | sed -e s%LISTFILES%".$LISTFiles.
                                  "%g | sed -e s%ISCAlib%".$ISCAlib.
                                  "%g | sed -e s%RUN%".$RunString.
                                  "%g > ".$tempo1) ;}
     
        $JOBCfgFile = $jobDir."/simpleNtuple_data_cfg.py" ;
        system("mv ".$tempo1." ".$JOBCfgFile);
    
    ######################
    # make job files
    ######################    
    
       open (SAMPLEJOBFILE, ">", $tempBjob) or die "Can't open file ".$tempBjob;
#
       $command = "cd ".$BASEDir ;
       print SAMPLEJOBFILE $command."\n";

       $command = "eval `scramv1 runtime -sh`" ;
       print SAMPLEJOBFILE $command."\n";
 
       $command = "cd -" ;
       print SAMPLEJOBFILE $command."\n";

       open (LISTFiles2,$LISTFiles) ;

       $LISTJobFile ;

       while (<LISTFiles2>)
       {
   	    chomp; 
   	    s/#.*//;                # no comments
   	    s/^\s+//;               # no leading white
   	    s/\s+$//;               # no trailing white
   	    $file = $_ ;

            if($STORAGE =~ /castor/) {$command = "rfcp ".$file." ./";
                                      print SAMPLEJOBFILE $command."\n"; }
             
       }


        if($Storage =~/castor/) {$command = "rfmkdir -p ".$OUTPUTSAVEPath.$sample;
                                 print SAMPLEJOBFILE $command."\n";}
        if($Storage =~/eos/)    {
                                 $command = "cmsMkdir ".$OUTPUTSAVEPath.$sample;
                                 print SAMPLEJOBFILE $command."\n";
        }
       
        $command = "WZAnalysis.cpp ".$JOBCfgFile ;
        print SAMPLEJOBFILE $command."\n";

        if($Storage =~ /castor/) {$command = "rfcp ".$OUTPUTFILEName." ".$OUTPUTSAVEPath.$sample;
                                  print SAMPLEJOBFILE $command."\n";}

	if($Storage =~ /eos/) {
                                 $command = "cmsStage ".$OUTPUTFILEName."_".$jobIt.".root ".$OUTPUTSAVEPath.$sample."/";
                                  print SAMPLEJOBFILE $command."\n";}


    
    ############
    # submit job
    ############
    
 	$command = "bsub -cwd ".$jobDir." -q cmscaf1nh ".$tempBjob."\n" ;  
 	print SAMPLEJOBLISTFILE $command."\n";
    
#     print "\n" ;
    }

     system ("rm ".$LISTOFFiles) ;
}  

# get the input

$SAMPLENAME = $ARGV[0] ;
$DATASETPATH = $ARGV[1] ;
$EVENTSNUMBER = $ARGV[2] ;

# prepare the crab cfg file

$CRABFILE = "crab-skim-".$SAMPLENAME.".cfg" ;
system ("cat crab-skim.cfg | sed -e s%DATASETPATH%".$DATASETPATH."% > tempo1\n") ;
system ("cat tempo1 | sed -e s%STORAGE_FOLDER%".$SAMPLENAME."% > tempo2\n") ;
system ("cat tempo2 | sed -e s%TOTEVENTS%".$EVENTSNUMBER."% > tempo3\n") ;
system ("mv tempo3 ".$CRABFILE."\n") ;
system ("rm tempo*\n") ;
#system ("crab -create -submit -cfg ".$CRABFILE."\n") ;

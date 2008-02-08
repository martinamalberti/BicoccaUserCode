# get the input

use Cwd;
my $DIR = getcwd () ;


$CFGFILE_TEMPLATE = $ARGV[0] ;
$SAMPLEFILE = $ARGV[1] ;
$OUTPUTTAG = $ARGV[2] ;

$SAMPLENAME = $ARGV[0] ;
$DATASETPATH = $ARGV[1] ;
$EVENTSNUMBER = $ARGV[2] ;

# prepare the cfg file
# --------------------

$ROOTFILE = $CFGFILE_TEMPLATE ;
$ROOTFILE =~ s/cfg/root/ ;
$ROOTFILE =~ s/template/$OUTPUTTAG/ ;
$CFGFILE = $CFGFILE_TEMPLATE ;
$CFGFILE =~ s/template/$OUTPUTTAG/ ;
system ("cat ".$CFGFILE_TEMPLATE." | sed -e s%DATASETFILE%".$SAMPLEFILE."% > tempo1\n") ;
system ("cat tempo1 | sed -e s%OUTPUT_ROOT%".$DIR."/".$ROOTFILE."% > tempo2\n") ;
#system ("cat tempo2 | sed -e s%TOTEVENTS%".$EVENTSNUMBER."% > tempo3\n") ;
system ("mv tempo2 ".$CFGFILE."\n") ;
system ("rm tempo*\n") ;

# prepare the shell script to be run
# ----------------------------------

$SCRIPT = "run_$OUTPUTTAG.sh";
system ("rm -f ".$SCRIPT) ;
open (SCRIPTNAME,">>".$SCRIPT) or die "Cannot open ".$SCRIPT." to write the config file" ;
print SCRIPTNAME "cmsRun ".$DIR."/".$CFGFILE."\n";

#system ("crab -create -submit -cfg ".$CRABFILE."\n") ;
#system ("eval `scramv1 runtime -sh`");
print ("qsub -V -q shortcms ".$SCRIPT."...\n");
system ("qsub -V -q shortcms ".$SCRIPT."\n");

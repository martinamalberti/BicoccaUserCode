$CFGFILE_TEMPLATE = $ARGV[0] ;
$SAMPLELIST = $ARGV[1] ; # lista di tutti i file .root del sample in questione senza path
$ORIGINALPATH = $ARGV[2] ;# path dei .root relativi al sample sul cluster
$OUTPUTTAG = $ARGV[3] ; # nome comune per il .cfi per il .cfg per il .csh per il .root
$FILEPERNODO = $ARGV[4] ; # numero file che si vogliono lanciare su un nodo

#creare prima la dir jobDataset sotto test per la copia in locale dei file

$JOBDATASET = "jobDataset_".$OUTPUTTAG;
if (!(-d $JOBDATASET)) {mkdir($JOBDATASET, 0755) || die "Cannot mkdir newdir: $!"; } 


use Cwd;
my $DIR = getcwd () ;

$COPIAPATH = $DIR."/".$JOBDATASET."/" ;
print ("Copiero' i .root in ".$COPIAPATH."\n") ;

########################################### #preparare i cfi

$NUMFILES = `cat $SAMPLELIST | wc -l`;
print ("Il numero di files nel sampel sample e' ".$NUMFILES."\n") ;

$GRUPPI = int ($NUMFILES / $FILEPERNODO) + 1;
print ("Volendo ".$FILEPERNODO." file per nodo si suddividera' in ".$GRUPPI." gruppi\n") ;

$MIN = 0;
$MAX = $FILEPERNODO;

for ($MYRUN = 0; $MYRUN < $NUMFILES; $MYRUN++ ) 
{
	if (($MYRUN >= $MIN) && ($MYRUN < $MAX)) 
	{
		$COPIA = $COPIAPATH."/".$OUTPUTTAG."_copia".$MIN.".csh"; # per la copia dei dati sul nodo
		system ("rm -f ".$COPIA ) ;
                print ($COPIA."\n");  
		open (COPIANAME,">>".$COPIA) or die "Cannot open ".$COPIA." to write the config file" ;
	
		#$CONFIG = "HiggsAnalysis/VBFHiggsToWW2e/data/datasets/listaFiles_From".$MIN.".cfi";
		$CONFIG = $OUTPUTTAG."_listaFiles_From".$MIN.".cfi";

		system ("rm -f ".$CONFIG ) ;  
		open (CONFIGNAME,">>".$CONFIG) or die "Cannot open ".$CONFIG." to write the config file" ;
		$MIN = $MIN + $FILEPERNODO;
		$MAX = $MAX + $FILEPERNODO;
		
		print CONFIGNAME "source = PoolSource\n"; 
                print CONFIGNAME "{\n"; 
		print CONFIGNAME "  untracked vstring fileNames =\n"; 
		print CONFIGNAME "   {\n";
	}

	$RIGA =`./goToRow.py $SAMPLELIST $MYRUN`; # sriptino di Danilo per leggere una riga di un file
	chop ($RIGA);

	print COPIANAME "cp ".$ORIGINALPATH."/".$RIGA." ".$COPIAPATH."\n"; 	# per la copia sul singolo nodo
	
    if ((($MYRUN%$FILEPERNODO) == ($FILEPERNODO-1)) || ($MYRUN == ($NUMFILES-1))) ### se e' l'ultima riga non serve la virgola e chiudo le graffe
		{
		 print CONFIGNAME "   \"file:".$COPIAPATH.$RIGA."\"  \n";
		 print CONFIGNAME "   }\n";
		 print CONFIGNAME "}\n";
		}
	else 
		{
		print CONFIGNAME "   \"file:".$COPIAPATH.$RIGA."\",  \n";
		}
}

########################################### 	#preparare i cfg e i csh


for ($INDEX = 0; $INDEX < $NUMFILES; $INDEX = $INDEX + $FILEPERNODO)
{
    $CONFIG = $OUTPUTTAG."_myCfg_From".$INDEX.".cfg";
    $ROOTFILE = $OUTPUTTAG."_from_".$INDEX.".root" ;
    $SAMPLEFILE = "HiggsAnalysis/VBFHiggsToWW2e/test/".$OUTPUTTAG."_listaFiles_From".$INDEX.".cfi";

    system ("cat ".$CFGFILE_TEMPLATE." | sed -e s%DATASETFILE%".$SAMPLEFILE."% > tempo1\n") ;
    system ("cat tempo1 | sed -e s%OUTPUT_ROOT%".$DIR."/".$ROOTFILE."% > tempo2\n") ;
    system ("mv tempo2 ".$CONFIG."\n") ;
    system ("rm tempo*\n") ;

    $JOB = $OUTPUTTAG."_lanciaFiles_From".$INDEX.".csh";
    $COPIA =  $COPIAPATH."/".$OUTPUTTAG."_copia".$INDEX.".csh";
	
    system ("rm -f ".$JOB ) ;  
    open (JOBNAME,">>".$JOB) or die "Cannot open ".$JOB." to write the config file" ;
    print JOBNAME  "source $COPIA\n";
    print JOBNAME  "cmsRun  ".$DIR."/".$CONFIG."\n"; 

    print ("qsub -V -q shortcms ".$JOB."...\n");
    system ("qsub -V -q shortcms ".$JOB."\n");
	
}
 

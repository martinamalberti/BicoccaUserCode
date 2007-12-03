#### filtra i dati H->WW via VBF a partire da sample H->WW
#### i dati di partenza sono in $DATASETPATH
#### i dati filtrati sono in $OUTROOT e vanno copiati in /castor

$SAMPLE = $ARGV[0] ;
$OUTROOT =  $SAMPLE.".root" ;
$DATASETPATH = $ARGV[1] ;
$CONFIG = "/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_0/src/HiggsAnalysis/VBFHiggsToWW2e/test/".$SAMPLE.".cfg";
#### per crab
$CONFIGCRAB = "/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_0/src/HiggsAnalysis/VBFHiggsToWW2e/test/".$SAMPLE."_crab.cfg";
$OUTDIR = "/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_0/src/HiggsAnalysis/VBFHiggsToWW2e/out_".$SAMPLE."/" ;

################################################################################

system("rm -f ".$CONFIG ) ;   
open (CONFIGNAME,">>".$CONFIG) or die "Cannot open ".$CONFIG." to write the config file" ;

#### writing the config file for the selection of WWF events test/selectVBF.cfg 
print CONFIGNAME "process VBFselect = {\n";

# ---- input files ---------------------------------------
print CONFIGNAME "source = PoolSource\n"; 
print CONFIGNAME "{\n"; 
print CONFIGNAME "untracked vstring fileNames = {\"myfile.root\"}\n";
print CONFIGNAME "}\n";
print CONFIGNAME "untracked PSet maxEvents = {untracked int32 input = 100}\n";

# ---- keep the logging output to a nice level ------------
print CONFIGNAME "service = MessageLogger {}\n";

print CONFIGNAME "module demo = VBFHiggsToWW2e { }\n";            

# ---- filtering the VBF part of the signal -------------------
print CONFIGNAME "module my_VBFMCProcessFilter = VBFMCProcessFilter{\n";
print CONFIGNAME "      untracked string moduleLabel = \"source\"}\n";

# ---- saving the ouptut --------------------------------------
print CONFIGNAME "  module saving = PoolOutputModule\n";
print CONFIGNAME "    {\n";
print CONFIGNAME "      untracked string fileName =\"$OUTROOT\"\n";
print CONFIGNAME "      untracked vstring outputCommands =\n";
print CONFIGNAME "        {\n";
print CONFIGNAME "          \"keep *\"\n";
print CONFIGNAME "        }\n";
# ---- select only the VBF events  
print CONFIGNAME "      untracked PSet SelectEvents =\n"; 
print CONFIGNAME "        {\n";
print CONFIGNAME "          vstring SelectEvents = { \"filtering\" }\n";
print CONFIGNAME "        }\n";
print CONFIGNAME "    }\n";

# ---- analysis paths --------------------------------------
print CONFIGNAME "  path filtering = {my_VBFMCProcessFilter}\n";
print CONFIGNAME "  endpath save = {saving}\n";

print CONFIGNAME "}\n"; # process VBFselect

################################################################################
################################################################################

system("rm -f ".$CONFIGCRAB ) ;   
open (CONFIGNAMECRAB,">>".$CONFIGCRAB) or die "Cannot open ".$CONFIGCRAB." to write the config crab file" ;

print CONFIGNAMECRAB "[CRAB]\n";
print CONFIGNAMECRAB "jobtype = cmssw\n";
print CONFIGNAMECRAB "scheduler = glitecoll\n"; 

print CONFIGNAMECRAB "[CMSSW]\n";
print CONFIGNAMECRAB "datasetpath = $DATASETPATH\n";
print CONFIGNAMECRAB "pset = $CONFIG\n";
#print CONFIGNAMECRAB "total_number_of_events = 10000\n"; 
print CONFIGNAMECRAB "events_per_job = 1000\n";
print CONFIGNAMECRAB "number_of_jobs = 10\n";
print CONFIGNAMECRAB "output_file = $OUTROOT\n";

print CONFIGNAMECRAB "[USER]\n";
print CONFIGNAMECRAB "publish_data = 0\n";
#print CONFIGNAMECRAB "return_data = 1\n";
print CONFIGNAMECRAB "use_central_bossDB = 0\n";
print CONFIGNAMECRAB "use_boss_rt = 1\n";
print CONFIGNAMECRAB "ui_working_dir = $OUTDIR\n";
print CONFIGNAMECRAB "copy_data = 1\n"; 
print CONFIGNAMECRAB "storage_element = srm.cern.ch\n"; 
print CONFIGNAMECRAB "storage_path = /castor/cern.ch/user/t/tancini/crab\n"; 

print CONFIGNAMECRAB "[EDG]\n";
print CONFIGNAMECRAB "lcg_version = 2\n";
print CONFIGNAMECRAB "proxy_server = myproxy.cern.ch\n"; 
print CONFIGNAMECRAB "virtual_organization = cms\n";
print CONFIGNAMECRAB "retry_count = 2\n";
print CONFIGNAMECRAB "lcg_catalog_type = lfc\n";
print CONFIGNAMECRAB "lfc_host = lfc-cms-test.cern.ch\n";
print CONFIGNAMECRAB "lfc_home = /grid/cms\n";


################################################################################

#system ("source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh\n") ; 
#system ("eval \`scramv1 runtime -csh\`\n") ;  
#system ("source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.csh\n") ; 
system("rm -rf ".$OUTDIR) ;
mkdir($OUTDIR, 0755) || die "Cannot mkdir newdir: $!";     
print ("command: crab -create -cfg ".$CONFIGCRAB."\n") ;
system ("crab -create -cfg ".$CONFIGCRAB) ;
print ("command: crab -submit -c ".$OUTDIR."\n") ;
system ("crab -submit -c ".$OUTDIR) ;

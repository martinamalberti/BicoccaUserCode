#### filtra i dati H->WW via VBF a partire da sample H->WW
#### i dati di partenza sono in $DATASETPATH
#### i dati filtrati sono in $OUTROOT e vanno copiati in /castor

$SAMPLE = $ARGV[0] ;
$OUTROOTREE =  $SAMPLE."_tree.root" ;
$DATASETPATH = $ARGV[1] ;
$CONFIG = "/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_0/src/HiggsAnalysis/VBFHiggsToWW2e/test/".$SAMPLE.".cfg";
#### per crab
$CONFIGCRAB = "/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_0/src/HiggsAnalysis/VBFHiggsToWW2e/test/".$SAMPLE."_crab.cfg";
$OUTDIR = "/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_0/src/HiggsAnalysis/VBFHiggsToWW2e/out_".$SAMPLE."/" ;

################################################################################
####################################################################################################################

system("rm -f ".$CONFIG ) ;   
open (CONFIGNAME,">>".$CONFIG) or die "Cannot open ".$CONFIG." to write the config file" ;

#### writing the config fil test/readVBF.cfg 
print CONFIGNAME "process VBFread = {\n";

print CONFIGNAME "include \"Geometry/CMSCommonData/data/cmsIdealGeometryXML.cfi\"\n";
print CONFIGNAME "include \"Geometry/CaloEventSetup/data/CaloGeometry.cfi\"\n";
print CONFIGNAME "include \"PhysicsTools/HepMCCandAlgos/data/genParticleCandidates.cfi\"\n";
print CONFIGNAME "include \"SimGeneral/HepPDTESSource/data/pythiapdt.cfi\"\n";

# per hcal isolation
print CONFIGNAME "include \"Geometry/CMSCommonData/data/cmsIdealGeometryXML.cfi\"\n";
print CONFIGNAME "include \"Geometry/CaloEventSetup/data/CaloGeometry.cfi\"\n";

# ----   keep the logging output to a nice level ----------------------
print CONFIGNAME "service = MessageLogger {}\n";

# ---- electron ID modules  --------------------------------------
print CONFIGNAME "include \"EgammaAnalysis/ElectronIDProducers/data/electronId.cfi\"\n";

# ---- muons
print CONFIGNAME "include \"Geometry/CMSCommonData/data/cmsIdealGeometryXML.cfi\"\n"; 
print CONFIGNAME "include \"Geometry/CommonDetUnit/data/globalTrackingGeometry.cfi\"\n";
print CONFIGNAME "include \"RecoMuon/DetLayers/data/muonDetLayerGeometry.cfi\"\n";
print CONFIGNAME "include \"Geometry/MuonNumbering/data/muonNumberingInitialization.cfi\"\n";
print CONFIGNAME "include \"RecoMuon/TrackingTools/data/MuonServiceProxy.cff\"\n";

print CONFIGNAME "module my_VBFReadEvent  = VBFReadEvent  {\n";

print CONFIGNAME "  InputTag metInputTag = met\n";
print CONFIGNAME "  InputTag genMetInputTag  = genMet\n";
print CONFIGNAME "  InputTag jetInputTag  = iterativeCone5CaloJets\n";
print CONFIGNAME "  InputTag genJetInputTag  = iterativeCone5GenJets\n";
print CONFIGNAME "  InputTag GSFInputTag  = pixelMatchGsfElectrons\n";
print CONFIGNAME "  InputTag eleIDInputTag  = electronId\n";
print CONFIGNAME "  InputTag MCtruthInputTag = genParticleCandidates\n";
print CONFIGNAME "  InputTag MC = source\n";
print CONFIGNAME "  InputTag muInputTag = muons\n";
print CONFIGNAME "  InputTag trackInputTag = ctfWithMaterialTracks\n";
print CONFIGNAME "  InputTag hcalRecHitProducer = hbhereco\n";
print CONFIGNAME "  InputTag emObjectProducer =  pixelMatchGsfElectrons\n";
  
# trk iso
print CONFIGNAME "  double ptMin = 1.5\n";
print CONFIGNAME "  double intRadius = 0.0\n";
print CONFIGNAME "  double extRadius = 1.0\n";
print CONFIGNAME "  double maxVtxDist = 30.\n";
  
# hcal iso
print CONFIGNAME "  double etMinHI = 0.0\n";
print CONFIGNAME "  double intRadiusHI = 0.15\n"; 
print CONFIGNAME "  double extRadiusHI = 0.3\n";

print CONFIGNAME "  untracked string rootfile = \"$OUTROOTREE\"\n";
  
print CONFIGNAME "PSet jetIdParameters = {\n";
print CONFIGNAME "        string mcSource = \"source\"\n";
print CONFIGNAME "        bool fillPartons = true\n";
print CONFIGNAME "        bool fillHeavyHadrons = false\n";
print CONFIGNAME "        bool fillLeptons =  false\n";
print CONFIGNAME "        double coneSizeToAssociate = 0.3\n";
print CONFIGNAME "        bool physicsDefinition = false\n";
print CONFIGNAME "        bool rejectBCSplitting = false\n";
print CONFIGNAME "        vstring vetoFlavour = {  }\n";
print CONFIGNAME "    }\n";

print CONFIGNAME "  }\n";
  
# ---- input files ---------------------------------------
print CONFIGNAME "source = PoolSource\n"; 
print CONFIGNAME "{\n";
print CONFIGNAME "      untracked vstring fileNames = {\"file:/tmp/tancini/VBFsignal.root\"}\n"; 
print CONFIGNAME "}\n";
  
# ---- saving the ouptut -----------------------------------------------------
#print CONFIGNAME "module saving = PoolOutputModule\n";
#print CONFIGNAME "{\n";
#print CONFIGNAME "      untracked string fileName = \"$OUTROOT\"\n";
#print CONFIGNAME "}\n";

# ---- analysis paths ---------------------------------------------
print CONFIGNAME "  path analysis = {electronId , my_VBFReadEvent}\n";
#print CONFIGNAME "  endpath save = {saving}\n";

print CONFIGNAME "}\n";


####################################################################################################################
####################################################################################################################

system("rm -f ".$CONFIGCRAB ) ;   
open (CONFIGNAMECRAB,">>".$CONFIGCRAB) or die "Cannot open ".$CONFIGCRAB." to write the config crab file" ;

print CONFIGNAMECRAB "[CRAB]\n";
print CONFIGNAMECRAB "jobtype = cmssw\n";
print CONFIGNAMECRAB "scheduler = glitecoll\n"; 

print CONFIGNAMECRAB "[CMSSW]\n";
print CONFIGNAMECRAB "datasetpath = $DATASETPATH\n";
print CONFIGNAMECRAB "pset = $CONFIG\n";
#print CONFIGNAMECRAB "total_number_of_events = 10000\n"; 
print CONFIGNAMECRAB "events_per_job = 2000\n"; #2000
print CONFIGNAMECRAB "number_of_jobs = 10\n"; #10
print CONFIGNAMECRAB "output_file = $OUTROOTREE\n";

print CONFIGNAMECRAB "[USER]\n";
print CONFIGNAMECRAB "publish_data = 0\n";
#print CONFIGNAMECRAB "return_data = 1\n";#comment if copy to castor
print CONFIGNAMECRAB "use_central_bossDB = 0\n";
print CONFIGNAMECRAB "use_boss_rt = 1\n";
print CONFIGNAMECRAB "ui_working_dir = $OUTDIR\n";
print CONFIGNAMECRAB "copy_data = 1\n"; #1 per copiare su castor  0 per non
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
################################################################################

#system ("source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh\n") ; 
#system ("eval \`scramv1 runtime -sh\`\n") ;  
#system ("source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh\n") ; 
system("rm -rf ".$OUTDIR) ;
mkdir($OUTDIR, 0755) || die "Cannot mkdir newdir: $!";     
print ("command: crab -create -cfg ".$CONFIGCRAB."\n") ;
system ("crab -create -cfg ".$CONFIGCRAB) ;
print ("command: crab -submit -c ".$OUTDIR."\n") ;
system ("crab -submit -c ".$OUTDIR) ;


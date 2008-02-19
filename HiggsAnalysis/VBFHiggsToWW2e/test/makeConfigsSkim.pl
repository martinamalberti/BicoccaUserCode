#### i dati di partenza sono in $DATASETPATH
#### i dati filtrati vanno copiati in /castor

$SAMPLE = $ARGV[0] ;
$DATASETPATH = $ARGV[1] ;
#### per crab
$CONFIGCRAB = "/afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_8/src/HiggsAnalysis/VBFHiggsToWW2e/test/".$SAMPLE."_crab.cfg";
$OUTDIR = "outSkim_".$SAMPLE ;

################################################################################
####################################################################################################################

print ("wrinting ".$CONFIGCRAB."\n") ;
system("rm -f ".$CONFIGCRAB ) ;   
open (CONFIGNAMECRAB,">>".$CONFIGCRAB) or die "Cannot open ".$CONFIGCRAB." to write the config crab file" ;

print CONFIGNAMECRAB "[CRAB]\n";
print CONFIGNAMECRAB "jobtype = cmssw\n";
print CONFIGNAMECRAB "scheduler = glitecoll\n";
print CONFIGNAMECRAB "server_mode = 1\n";
print CONFIGNAMECRAB "server_name = crabas.lnl.infn.it/data1/cms/\n";
print CONFIGNAMECRAB "[CMSSW]\n";
print CONFIGNAMECRAB "datasetpath = $DATASETPATH\n";
print CONFIGNAMECRAB "pset = /afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_8/src/HiggsAnalysis/VBFHiggsToWW2e/test/VBFLepTagSkim.cfg\n";
print CONFIGNAMECRAB "events_per_job = 2000\n";
print CONFIGNAMECRAB "total_number_of_events = -1\n";
#print CONFIGNAMECRAB "number_of_jobs = 10\n";
print CONFIGNAMECRAB "output_file = VBFskim.root, finalReport.log\n";
print CONFIGNAMECRAB "[USER]\n";
print CONFIGNAMECRAB "publish_data = 0\n";
print CONFIGNAMECRAB "use_central_bossDB = 0\n";
print CONFIGNAMECRAB "use_boss_rt = 1\n";
print CONFIGNAMECRAB "copy_data = 1\n";
print CONFIGNAMECRAB "ui_working_dir = /afs/cern.ch/user/t/tancini/scratch0/WWF/CMSSW_1_6_8/src/HiggsAnalysis/VBFHiggsToWW2e/$OUTDIR\n";
print CONFIGNAMECRAB "storage_element = srm.cern.ch\n";
print CONFIGNAMECRAB "storage_path = /castor/cern.ch/user/t/tancini/crabWWFSkim/$OUTDIR\n";
print CONFIGNAMECRAB "thresholdLevel = 80\n";
print CONFIGNAMECRAB "eMail = pietro.govoni\@gmail.com, valentina.tancini\@mib.infn.it, alessio.ghezzi\@mib.infn.it\n";
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
system ("rm -rf ".$OUTDIR) ;
mkdir ($OUTDIR, 0755) || die "Cannot mkdir newdir: $!";
$NEWDIR = "/castor/cern.ch/user/t/tancini/crabWWFSkim/".$OUTDIR;
print("removing ".$NEWDIR."\n");
system("rfrm -r ".$NEWDIR."\n");
print("creating ".$NEWDIR."\n");
system ("rfmkdir ".$NEWDIR);     
system ("rfchmod +775 ".$NEWDIR);
print ("command: crab -create -cfg ".$CONFIGCRAB."\n") ;
system ("crab -create -cfg ".$CONFIGCRAB) ;
print ("command: crab -submit all -c ".$OUTDIR."\n") ;
system ("crab -submit -c ".$OUTDIR) ;


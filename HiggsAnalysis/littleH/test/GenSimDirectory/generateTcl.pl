#!/usr/bin/perl
#

use Getopt::Std;
getopts('hd:');

$start = 1;
$stop = 251;

print "Starting...\n";

open(INFILE,"Reco_skel_cfg.py");
@infile=<INFILE>;
close(INFILE);

while($start < $stop) {

    $filename = ">recoTcl/config_$start"."_cfg.py";

    open(OUTFILE, $filename);

    for ($i=0;$i<($#infile+1);$i++){
	print OUTFILE "$infile[$i]";
    }

    print OUTFILE "process.source = cms.Source(\"PoolSource\",\n";
    print OUTFILE "        duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),\n";
    print OUTFILE "        fileNames = cms.untracked.vstring('castor:/castor/cern.ch/user/m/maborgia/SmallHiggs/422/10.5GEV/A0_GEN-SIM-RAW_$start.root'),\n";
    print OUTFILE ")\n\n";

    print OUTFILE "process.output = cms.OutputModule(\"PoolOutputModule\",\n";
    print OUTFILE "   splitLevel = cms.untracked.int32(0),\n";
    print OUTFILE "   outputCommands = process.RECOSIMEventContent.outputCommands,\n";
    print OUTFILE "   fileName = cms.untracked.string('castor:/castor/cern.ch/user/m/maborgia/SmallHiggs/422/10.5GEV/A0prod_$start.root'),\n";
    print OUTFILE "   dataset = cms.untracked.PSet(\n";
    print OUTFILE "   dataTier = cms.untracked.string('GEN-SIM-RECO'),\n";
    print OUTFILE "   filterName = cms.untracked.string('')\n";
    print OUTFILE "    ),\n";
    print OUTFILE ")\n\n";

    print OUTFILE "# Path and EndPath definitions\n";
    print OUTFILE "process.raw2digi_step = cms.Path(process.RawToDigi)\n";
    print OUTFILE "process.reconstruction_step = cms.Path(process.reconstruction)\n";
    print OUTFILE "process.endjob_step = cms.Path(process.endOfProcess)\n";
    print OUTFILE "process.out_step = cms.EndPath(process.output)\n\n";


    print OUTFILE "process.myschedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.out_step)\n";

    close(OUTFILE);

    $start = $start + 1;
}

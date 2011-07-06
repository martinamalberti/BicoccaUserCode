#!/usr/bin/perl
#

use Getopt::Std;
getopts('hd:');

$start = 1;
$stop = 251;

print "Starting...\n";

open(INFILE,"Sim_ske_cfg.py");
@infile=<INFILE>;
close(INFILE);

while($start < $stop) {

    $filename = ">gensimTcl/config_$start"."_cfg.py";

    open(OUTFILE, $filename);

    for ($i=0;$i<($#infile+1);$i++){
	print OUTFILE "$infile[$i]";
    }

    print OUTFILE "process.output = cms.OutputModule(\"PoolOutputModule\",\n";
    print OUTFILE "   splitLevel = cms.untracked.int32(0),\n";
    print OUTFILE "   outputCommands = process.RAWSIMEventContent.outputCommands,\n";
    print OUTFILE "   fileName = cms.untracked.string('castor:/castor/cern.ch/user/m/maborgia/SmallHiggs/422/10.5GEV/A0_GEN-SIM-RAW_$start.root'),\n";
    print OUTFILE "   dataset = cms.untracked.PSet(\n";
    print OUTFILE "   dataTier = cms.untracked.string('GEN-SIM-RAW'),\n";
    print OUTFILE "   filterName = cms.untracked.string('')\n";
    print OUTFILE "    ),\n";
    print OUTFILE "   SelectEvents = cms.untracked.PSet(\n";
    print OUTFILE "       SelectEvents = cms.vstring('generation_step')\n";
    print OUTFILE "   )\n";
    print OUTFILE ")\n\n";

    print OUTFILE "process.RandomNumberGeneratorService = cms.Service(\"RandomNumberGeneratorService\",\n";
    print OUTFILE "        moduleSeeds = cms.PSet(\n";
    print OUTFILE "            generator = cms.untracked.uint32($start),\n";
    print OUTFILE "            g4SimHits = cms.untracked.uint32($start),\n";
    print OUTFILE "            VtxSmeared = cms.untracked.uint32($start),\n";
    print OUTFILE "            LHCTransport = cms.untracked.uint32($start),\n";
    print OUTFILE "            simSiPixelDigis = cms.untracked.uint32($start),\n";
    print OUTFILE "            simSiStripDigis = cms.untracked.uint32($start),\n";
    print OUTFILE "            simEcalUnsuppressedDigis = cms.untracked.uint32($start),\n";
    print OUTFILE "            simHcalUnsuppressedDigis = cms.untracked.uint32($start),\n";
    print OUTFILE "            simMuonCSCDigis = cms.untracked.uint32($start),\n";
    print OUTFILE "            simMuonDTDigis = cms.untracked.uint32($start),\n";
    print OUTFILE "            simMuonRPCDigis = cms.untracked.uint32($start),\n";
    print OUTFILE "        ),\n";
    print OUTFILE "        sourceSeed = cms.untracked.uint32($start)\n";
    print OUTFILE ")\n\n";

    print OUTFILE "# Path and EndPath definitions\n";
    print OUTFILE "process.generation_step = cms.Path(process.pgen)\n";
    print OUTFILE "process.simulation_step = cms.Path(process.psim)\n";
    print OUTFILE "process.digitisation_step = cms.Path(process.pdigi)\n";
    print OUTFILE "process.L1simulation_step = cms.Path(process.SimL1Emulator)\n";
    print OUTFILE "process.digi2raw_step = cms.Path(process.DigiToRaw)\n";
    print OUTFILE "process.endjob_step = cms.Path(process.endOfProcess)\n";
    print OUTFILE "process.out_step = cms.EndPath(process.output)\n\n";


    print OUTFILE "process.schedule = cms.Schedule(process.generation_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)\n";
    print OUTFILE "process.schedule.extend(process.HLTSchedule)\n";
    print OUTFILE "process.schedule.extend([process.endjob_step,process.out_step])\n";

    print OUTFILE "for path in process.paths:\n";
    print OUTFILE "    getattr(process,path)._seq = process.ProductionFilterSequence*getattr(process,path)._seq\n";


    close(OUTFILE);

    $start = $start + 1;
}

#!/bin/csh

set start = 1
set stop  = 250
@ stop = $stop + 1

while ( $start < $stop )

set logfile = "/afs/cern.ch/user/m/maborgia/scratch0/lighta_LHCEra/CMSSW_4_2_2_patch1/src/HiggsAnalysis/littleH/test/logtcl/logfile_$start.log"

set config = "/afs/cern.ch/user/m/maborgia/scratch0/lighta_LHCEra/CMSSW_4_2_2_patch1/src/HiggsAnalysis/littleH/test/recoTcl/config_${start}_cfg.py"

bsub -q 1nw -o $logfile execute.csh $config

echo "$config"

@ start = $start + 1
end

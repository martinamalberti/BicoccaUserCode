if( @ARGV != 2 )
{
  print(">>> createJsonFromRunList::usage:   perl createJsonFromRunList.pl   runList.txt   out.txt\n");
  exit;
}

$runList = $ARGV[0];
$json = $ARGV[1];



open(RUNLIST,$runList);

open JSON, ">", "temp_".$json;
print JSON "{";

$isFirstRun = 1;

while(<RUNLIST>)
{
    chomp;
    $run = $_;

  if( $isFirstRun == 1 )
  {
      print JSON "\"".$run."\": [[1,999999]]";
      $isFirstRun = 0;
  }
  else
  {
      print JSON ", \"".$run."\": [[1,999999]]";
  }
}

print JSON "}\n";

$CMSSW_RELEASE_BASE = $ENV{'CMSSW_RELEASE_BASE'};
system("python ".$CMSSW_RELEASE_BASE."/src/FWCore/PythonUtilities/scripts/compareJSON.py --and temp_".$json." /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt ".$json);

system("rm temp_".$json);

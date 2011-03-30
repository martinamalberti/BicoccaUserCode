$baseDir = "/gwpool/users/mlucchin/Unfolding/CFNtuple";


$nJobs = 9;


$lanciaFile = "./lanciaDati.sh";
open(LANCIAFILE, ">", $lanciaFile);



for($job = 1; $job < $nJobs; ++$job)
{

    
      $jobDir = $baseDir."/test/DATA/";


# create configure file for each pdf set

    $inputFile = $jobDir."/data_nov_".$job.".cfg\n" ;
    system("cat ".$baseDir."/test/DATA/data_nov_template.cfg | sed -e s%NLISTA%".$job."%g > ".$inputFile);
    
    
    
    $jobFile = $jobDir."/lista_".$job.".sh";

    open(JOBFILE, ">", $jobFile);

    print JOBFILE "cd ".$baseDir."\n";
    print JOBFILE "source scripts/setup.sh \n";
    print JOBFILE "./bin/NtupleMC_test.exe ".$inputFile."\n";


    print LANCIAFILE "qsub -V -q shortcms -d ./ ".$jobFile."\n";

   
}

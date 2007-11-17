#!/usr/bin/perl

use File::Spec::Functions;

$folder = `pwd` ;
chomp ($folder) ;

opendir(FILES,$folder) || die "Cannot opendir $folder: $!";


while ($files = readdir(FILES)) 
{
  next if (rindex $files,"results") == -1 ;
#  @pieces = split ($files,"_") ;
  $commandEB = "coeffCompareEB " ;
  $commandEB .= "/data/govoni/TEMPSTORE/calibFiles/originals/ecal_barrel_startup.xml " ;
  $commandEB .= $files."/EBrecalib.xml " ;
  $commandEB .= "1 86 20 60 " ;
  $commandEB .= $files."/compareEB.root" ;
  print $commandEB."\n" ;
#  system ($commandEB) ;
  $commandEE = "coeffCompareEE " ;
  $commandEE .= "/data/govoni/TEMPSTORE/calibFiles/originals/ecal_endcap_startup.xml " ;
  $commandEE .= $files."/EErecalib.xml " ;
  $commandEE .= "20 45 15 45 " ;
  $commandEE .= $files."/compareEE.root" ;
  print $commandEE."\n" ;
  system ($commandEE) ;
}



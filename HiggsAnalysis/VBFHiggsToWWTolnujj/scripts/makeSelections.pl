#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

#PG lettura dei parametri da cfg file
#PG --------------------------------
print "reading ".$ARGV[0]."\n" ;

open (USERCONFIG,$ARGV[0]) ;

while (<USERCONFIG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);
    $User_Preferences{$var} = $value;
  }


$LOCALFolder         = $User_Preferences{"LOCALFolder"} ;
$SELECTIONSTemplate  = $User_Preferences{"SELECTIONSTemplate"} ;
$INPUTSAVEFolder     = $User_Preferences{"INPUTSAVEFolder"} ;
$JETAlgorithm        = $User_Preferences{"JETAlgorithm"} ;

print "LOCALFolder = "       .$LOCALFolder."\n" ;
print "SELECTIONSTemplate = ".$SELECTIONSTemplate."\n" ;  
print "INPUTSAVEFolder = "   .$INPUTSAVEFolder."\n" ;
print "JETAlgorithm = "      .$JETAlgorithm."\n" ;






#################################################
# PG prepare the array containing the sample list
#################################################

$LISTFile = "./list.txt" ;
system ("ls -lh ".$INPUTSAVEFolder." | grep -v total | awk '{print \$9}' > ".$LISTFile."\n") ;







open (LISTFile, $LISTFile) ;
while (<LISTFile>)
{
  chomp; 
  s/#.*//;                # no comments
  s/^\s+//;               # no leading white
  s/\s+$//;               # no trailing white
  
  $Path = $INPUTSAVEFolder.$_."/" ;
  print "Sample = ".$Path."\n" ;
  
  
  
  
  
  
  #################################################
  # prepare the array containing efficiencies
  #################################################
  
  $LISTFile_efficiencies = "./list_efficiencies.txt" ;
  system ("ls -lh ".$Path." | grep efficienc | awk '{print \$9}' > ".$LISTFile_efficiencies."\n") ;
  
  $listOfFiles_efficiencies = "";
  open (LISTFile_efficiencies, $LISTFile_efficiencies) ;
  while (<LISTFile_efficiencies>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    
    $listOfFiles_efficiencies = $listOfFiles_efficiencies."APICE".$Path.$_."APICE,ACAPO" ;
  }
  chop($listOfFiles_efficiencies) ;
  chop($listOfFiles_efficiencies) ;
  chop($listOfFiles_efficiencies) ;
  chop($listOfFiles_efficiencies) ;
  chop($listOfFiles_efficiencies) ;
  chop($listOfFiles_efficiencies) ;
  
  
  
  #################################################
  # prepare the array containing ntuples
  #################################################
  
  #sisCone5CaloJets
  
  $LISTFile_sisCone5CaloJets = "./list_sisCone5CaloJets.txt" ;
  system ("ls -lh ".$Path."/sisCone5CaloJets | grep root | awk '{print \$9}' > ".$LISTFile_sisCone5CaloJets."\n") ;
  
  $listOfFiles_sisCone5CaloJets = "" ;
  open (LISTFile_sisCone5CaloJets, $LISTFile_sisCone5CaloJets) ;
  while (<LISTFile_sisCone5CaloJets>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    
    $listOfFiles_sisCone5CaloJets = $listOfFiles_sisCone5CaloJets."APICE".$Path."sisCone5CaloJets/".$_."APICE,ACAPO" ;
  }
  chop($listOfFiles_sisCone5CaloJets) ;
  chop($listOfFiles_sisCone5CaloJets) ;
  chop($listOfFiles_sisCone5CaloJets) ;
  chop($listOfFiles_sisCone5CaloJets) ;
  chop($listOfFiles_sisCone5CaloJets) ;
  chop($listOfFiles_sisCone5CaloJets) ;
  
  
  
  #correctedSisCone5CaloJets
  
  $LISTFile_correctedSisCone5CaloJets = "./list_correctedSisCone5CaloJets.txt" ;
  system ("ls -lh ".$Path."/correctedSisCone5CaloJets | grep root | awk '{print \$9}' > ".$LISTFile_correctedSisCone5CaloJets."\n") ;
  
  $listOfFiles_correctedSisCone5CaloJets = "";
  open (LISTFile_correctedSisCone5CaloJets, $LISTFile_correctedSisCone5CaloJets) ;
  while (<LISTFile_correctedSisCone5CaloJets>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    
    $listOfFiles_correctedSisCone5CaloJets = $listOfFiles_correctedSisCone5CaloJets."APICE".$Path."correctedSisCone5CaloJets/".$_."APICE,ACAPO" ;
  }
  chop($listOfFiles_correctedSisCone5CaloJets) ;
  chop($listOfFiles_correctedSisCone5CaloJets) ;
  chop($listOfFiles_correctedSisCone5CaloJets) ;
  chop($listOfFiles_correctedSisCone5CaloJets) ;
  chop($listOfFiles_correctedSisCone5CaloJets) ;
  chop($listOfFiles_correctedSisCone5CaloJets) ;
  
  
  
  #sisCone5PFJets
  
  $LISTFile_sisCone5PFJets = "./list_sisCone5PFJets.txt" ;
  system ("ls -lh ".$Path."/sisCone5PFJets | grep root | awk '{print \$9}' > ".$LISTFile_sisCone5PFJets."\n") ;
  
  $listOfFiles_sisCone5PFJets = "";
  open (LISTFile_sisCone5PFJets, $LISTFile_sisCone5PFJets) ;
  while (<LISTFile_sisCone5PFJets>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    
    $listOfFiles_sisCone5PFJets = $listOfFiles_sisCone5PFJets."APICE".$Path."sisCone5PFJets/".$_."APICE,ACAPO" ;
  }
  chop($listOfFiles_sisCone5PFJets) ;
  chop($listOfFiles_sisCone5PFJets) ;
  chop($listOfFiles_sisCone5PFJets) ;
  chop($listOfFiles_sisCone5PFJets) ;
  chop($listOfFiles_sisCone5PFJets) ;
  chop($listOfFiles_sisCone5PFJets) ;
  
  
  
  
  
  
  
  
  
  
  
  
  #open the txt file with cross-section information
  $CROSSECTIONFile = $Path."/crossSection.txt" ;
  open (CROSSECTIONFile, $CROSSECTIONFile) ;
  
  @values;
  $#values = -1;
  while (<CROSSECTIONFile>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
    
    push(@values, $_) ;
  }
  
  
  
  
  
  
  $tempo1 = "./tempo1" ;
  system ("cat ".$LOCALFolder."/".$SELECTIONSTemplate."   | sed -e s%EVENTTYPE%".$values[0].
                                                      "%g | sed -e s%JETALGORITHM%".$JETAlgorithm.
                                                      "%g | sed -e s%CROSSECTION%".$values[1].
                                                      "%g | sed -e s%OUTPUTSAVEFOLDER%".$Path.
                                                      "%g | sed -e s%INPUTEFFICIENCIES%".$listOfFiles_efficiencies.
                                                      "%g | sed -e s%INPUTNTUPLES_SISCONE5CALOJETS%".$listOfFiles_sisCone5CaloJets.
                                                      "%g | sed -e s%INPUTNTUPLES_CORRECTEDSISCONE5CALOJETS%".$listOfFiles_correctedSisCone5CaloJets.
                                                      "%g | sed -e s%INPUTNTUPLES_SISCONE5PFJETS%".$listOfFiles_sisCone5PFJets.
                                                      "%g > ".$tempo1) ;
  $tempo2 = "./tempo2" ;
  system ("cat ".$tempo1." | sed -e s%APICE%\\'%g  > ".$tempo2) ;

  $tempo3 = "./tempo3" ;
  system ("cat ".$tempo2." | awk '{gsub(\"ACAPO\", \"\\n\", \$0); print \$0;}' > ".$tempo3) ;
  
  
  $SELECTIONSFile = $LOCALFolder."/selections_".$values[0]."_".$JETAlgorithm."_cfg.py" ;
  system ("mv ".$tempo3." ".$SELECTIONSFile) ;
  system ("rm ./tempo*") ;
  system ("rm ./list*.txt") ;
}

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


$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTSAVEFolder = $User_Preferences{"OUTPUTSAVEFolder"} ;
$OUTPUTFileName   = $User_Preferences{"OUTPUTFileName"} ;
$COPYSAVEFolder   = $User_Preferences{"COPYSAVEFolder"} ;

print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTSAVEFolder = " .$OUTPUTSAVEFolder."\n" ;
print "OUTPUTFileName = "   .$OUTPUTFileName."\n" ;
print "COPYSAVEFolder = "   .$COPYSAVEFolder."\n" ;






#####################################################
# PG prepare the array containing the root files list
#####################################################

$LISTFile = "./list.txt" ;
system ("rfdir /castor/cern.ch/".$OUTPUTSAVEPath.$OUTPUTSAVEFolder." | grep root | awk '{print \$9}' > ".$LISTFile."\n") ;






###############
# create folder
###############

$command = "mkdir ".$COPYSAVEFolder.$OUTPUTSAVEFolder ;
print  ($command."\n") ;
system ($command."\n") ;






#################
# loop over files 
#################

$listOfFiles;   
$it = 0;
open (LISTFile,$LISTFile) ;
while (<LISTFile>)
{
  chomp; 
  s/#.*//;                # no comments
  s/^\s+//;               # no leading white
  s/\s+$//;               # no trailing white
  $file = $_ ;

  $command = "rfcp /castor/cern.ch/".$OUTPUTSAVEPath.$OUTPUTSAVEFolder.$file." ".$COPYSAVEFolder.$OUTPUTSAVEFolder ;

  print  ($command."\n") ; 
  system ($command."\n") ;
}

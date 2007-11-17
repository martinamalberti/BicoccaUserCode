#!/usr/bin/perl

#PG put here a function to create the new SourceFiles

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

#PG lettura dei parametri da cfg file
#PG --------------------------------

open (USERCONFIG,$ARGV[0]) ;

while (<USERCONFIG>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);    $User_Preferences{$var} = $value;  }

$originalSourceFile = $User_Preferences{"originalSourceFile"} ;
$numberOfSouces = $User_Preferences{"numberOfSources"} ;
$sources = $User_Preferences{"sourcesFolder"} ;

#print $originalSourceFile."\n" ;

open (ORIGINALSOURCE,$originalSourceFile) ;
$entry = 0 ;
@Input_Root_Files ;
#PG loop on the original SourceFile
while (<ORIGINALSOURCE>)
  {
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    $where = index( $_,"root");
    if ($where > 0) 
      {  
#        print $_."\n" ;
        push (@Input_Root_Files, $_) ;
#        print $entry." -> ".$Input_Root_Files[$entry]."\n" ;
        $entry++ ;
      }  } #PG loop on the original SourceFile


$length = @Input_Root_Files / $numberOfSouces ;
#print $length." = ".@Input_Root_Files." / ".$numberOfSouces."\n" ;
$arrayIndex = 0 ;

#PG prepare the folders where to put the cfg files
if (! (-e $sources))
{
  mkdir $sources ;    
}

#PG loop over the sub-sections of the source
for ($section = 0 ; $section < $numberOfSouces ; $section++)
  {
    @path = split (/\//, $originalSourceFile) ;
  #  print $path[@path-1]."\n" ;
    #PG trovo la desinenza
    @pieces = split (/\./, $originalSourceFile) ;
    #PG prepare the output file name
    $nomeFile = $path[@path-1] ;
    $piece = "\.".$pieces[@pieces-1] ;
    $nomeFile =~ s/$piece/_$section$piece/ ;
    $nomeFile = $sources."/".$nomeFile ;
  #  print $nomeFile."\n" ;
    # fill the output file
    open THISNEWSOURCE,">".$nomeFile ;  
    print THISNEWSOURCE "replace PoolSource.fileNames = {\n" ;
  
    $arraylimit = $arrayIndex + $length ;   
    #PG loop over the root files vector
    for (; $arrayIndex < $arraylimit-1 ; $arrayIndex++)    
      {
        print THISNEWSOURCE $Input_Root_Files[$arrayIndex]."\n" ;
      } #PG loop over the root files vector
    $lastone = $Input_Root_Files[$arrayIndex] ;
    $arrayIndex++ ;
    $lastone =~ s/,// ;
    print THISNEWSOURCE $lastone."\n" ;
    # manca quello senza virgola
    print THISNEWSOURCE "}\n" ;
    close (THISNEWSOURCE) ;
  } #PG loop over the sub-sections of the source

print $numberOfSouces." config files created\n" ;
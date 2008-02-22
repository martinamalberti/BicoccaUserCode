#!/usr/local/bin/perl

use strict ;
use warnings ;

my $folder = $ARGV[0] ;
chomp ($folder) ;
my @path = split (/\//,$folder) ;
system ("mkdir ".$path[$#path]."\n") ;
system ("rfdir ".$folder 
       ." | awk '{print \"rfcp ".$folder 
       ."/\"\$9\" ./".$path[$#path] 
	."\"}' | /bin/sh ; touch ".$path[$#path]."_done\n") ;

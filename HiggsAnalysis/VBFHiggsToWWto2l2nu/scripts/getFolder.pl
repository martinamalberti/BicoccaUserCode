#!/usr/local/bin/perl

# script to dump a folder from CASTOR to a local lxplus machine
# $Id: getFolder.pl,v 1.2 2008/03/26 17:24:55 govoni Exp $

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

#! /usr/bin/python

import sys
import os
import stat

    
args=sys.argv

# check the command line and if wrong raise an error
if len(args)!=2:
    print 'Usage:\n'+\
          '%s CMSSW_dir rootfilelistfile basedir castor_dir queue\n' %args[0] +\
          'Example:\n'+\
          '%s ./CMSSW_1_2_3 mylist.txt /tmp 8nm\n' %args[0]
    raise "Wrong number of arguments"          

# relevant parameters
listofrootfiles_file_name=args[1]           

listofrootfiles_file=open(listofrootfiles_file_name,"r")
listofrootfiles=listofrootfiles_file.readlines()
listofrootfiles_file.close()

import re

#--------------------------------------------------------------    
    
def removechars(string):
    """
    removes 1st last but one and last character
    """
    return string[1:-3]
    
#--------------------------------------------------------------


# Loop on the content of the list of rootfiles.
# map applies element by element the function removechars to the list 
# listofrootfiles and returns the resulting list
for rootfile in map(removechars,listofrootfiles):
    basename = "_".join (re.match (".*/(.*)/(.*)[.]root",rootfile).group(1,2))

    print basename
    

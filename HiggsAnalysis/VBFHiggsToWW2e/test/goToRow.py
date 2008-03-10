#! /usr/bin/env python

import sys

#print open(sys.argv[1],"r").readlines()[int(sys.argv[2])]
print open(sys.argv[1],"r").readlines()[int(sys.argv[2])].split()[1]
#print open(sys.argv[1],"r").readlines()[int(sys.argv[2])].split()[1][:-1]

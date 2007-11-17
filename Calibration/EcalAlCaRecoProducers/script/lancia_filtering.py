#! /usr/bin/env python

'''
A program to produce filtering cfg files according to command line inputs and 
launch cmsRun accordingly on a selected queue on the pbs system.
'''

# A list which contain the name of all the parameters to be changed in order to
# autogenerate the code accordingly. One will have to edit the list and not the
# full code!
# In the config template function these strings will appear as keys of a
# dictionary.
# FOR THE IMPATIENT USER:
# 1) add your parameter names in the list.
# 2) use them in the lancia_filtering_template.py file as keys of a dictionary.
# And that is all!


param_names_list=['ptmin',
                  'etamin',
                  'etamax',
                  'phimin',
                  'phimax',
                  'escopinmin',
                  'escopinmax',
                  'eseedopoutmin',
                  'eseedopoutmax',
                  'pinmpoutpinmin',
                  'pinmpoutpinmax',
                  'outputfilename']

# a default value used for rising exceptions :)
def_value="__not_set__"
#---------------------------------
# The script

import lancia_filtering_template

import optparse
import sys
import os

def execute(command):
  print '[Executing] '+ command
  os.system(command)

# Push the user to use the helper:
if len(sys.argv)==1:
    print '\n\n','+'*20
    print 'Please type %s -h for help!' %sys.argv[0]
    print '+'*20,'\n\n'
    raise 'No arguments set!'

# Prepare a parser to read the options
current_params=""
for param in param_names_list:
   current_params+=' - %s\n' %param

usage='''
A program to produce filtering cfg files according to command line inputs and 
launch cmsRun accordingly on a selected queue on the pbs system.
# A list which contain the name of all the parameters to be changed in order to
# autogenerate the code accordingly. One will have to edit the list and not the
# full code!
# In the config template function these strings will appear as keys of a
# dictionary.
# FOR THE IMPATIENT USER:
# 1) add your parameter names in the list.
# 2) use them in the lancia_filtering_template.py file as keys of a dictionary.
# And that is all!

Example:
  ./%prog --energy 50 --events 20 --phimax 24 --phimin 34 -q 8nh -cfgfileid mycfg
This will launch a process with these 4 variable parameters (energy,events,
phimax,phimin) on yhe queue 8nh with the id for the cfg and the job sh file "mycfg"

Current parameter list is:
''' + current_params

parser = optparse.OptionParser(usage)

# add the queue option..
parser.add_option('-q','--queue',
                  help='The queue to use on the pbs system. Default is cmsprs.\n'+\
                       'If None is selected the job is not launched.',
                  default='cmsprs',
                  dest='queue')
                  
# add the cfg filename identifier:
parser.add_option('-i','--cfgfileid',
                  help='The id of the cfg filename that will follow the basename.',
                  default='filtering',
                  dest='cfgid')


# This is rather subtle: autogenerate the code for the commandline options
# according to the parameters list. The exec statement does the job..
for parameter_name in param_names_list:
    exec('parser.add_option("--%s",' %parameter_name+\
                    'help="%s parameter.",' %parameter_name+\
                    'default="%s",' %def_value+\
                    'dest="%s_value")' %parameter_name)

# Let's extract the info from the optparser object, the protagonist of the
# command line arguments parsing..See www.python.org for a lot of details..
(options,args) = parser.parse_args()

# Build a dictionary for the parameters. The names in the list will be the keys.
# Again we autogenerate!
param_dict={}
for parameter_name in param_names_list:
    exec('param_dict["%s"]=options.%s_value' %(parameter_name,parameter_name))

# Here we will prepare the cfg file, write it on disk, launch cmsRun with it on 
# the desired queue.

# fetch config content
cfg_filecontent=lancia_filtering_template.config_text(param_dict,def_value)

# write it on disk on a meaningful filename:
cfgfile_name='filtering_%s.cfg' %options.cfgid
cfgfile=open(cfgfile_name,'w')
cfgfile.write(cfg_filecontent)
cfgfile.close()

# Prepare the script and launch the job :). The directory will be the one from 
# where this script is launched.
queue=options.queue
if queue=='None':
    print 'The job is not launched!!'
    exit

# the sh script
cur_dir=os.environ['PWD']

scriptfile_name='job_%s.cfg' %options.cfgid
scriptfile=open(scriptfile_name,'w')
scriptfile.write('#! /bin/sh\n'+\
                 'cmsRun %s/%s\n' %(cur_dir,cfgfile_name))

scriptfile.close()

command='chmod +x %s/%s' %(cur_dir,scriptfile_name)
execute(command)

# launch it
command='bsub -q %s ./%s' %(queue,scriptfile_name)
execute(command)
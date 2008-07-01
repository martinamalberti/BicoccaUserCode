#! /usr/bin/python

import sys
import os
import stat

def cfgfile_text(in_rootfile_name,out_rootfile_name):
    """
    The whole cfg with 2 fileds that are filled: the input file
    and the outputone
    """
    return\
    '''
#PG INIZIO CFG FILE
process analysis = {

  source = PoolSource 
  {
    # Number of events program should run
    untracked vstring fileNames = {
      "'''+in_rootfile_name+'''"
    }
  }
 
  untracked PSet maxEvents = {untracked int32 input = -1}
  untracked PSet options = {untracked bool wantSummary = true}

 include "HiggsAnalysis/HiggsToWW2Leptons/data/HWWJetCleaner.cfi"

 module referenceTester = testReferences {
   bool runOnChowder = false
   InputTag genMetInputTag = genMet 
   InputTag metInputTag = met
   InputTag rawGSF = pixelMatchGsfElectrons
   InputTag ambiguity = refResolver
   InputTag tkIso = refTkisolation
   #InputTag hadIso = refHadisolation
   InputTag  hcalIso = egammaHcalIsolationRelative
   InputTag  ecalIso = egammaEcalIsolationRelative
   InputTag eleId = electronIdMedium
   InputTag eleIdLoose = electronId
   InputTag eleIdTight = electronIdTight
   InputTag eleIdRobust = electronIdRobust
   InputTag jet = cleanedJets
   InputTag truthMatchMap = electronMCMatch
   InputTag EBsuperClusters = correctedHybridSuperClusters
   InputTag EEsuperClusters = correctedIslandEndcapSuperClusters
   InputTag barrelClusterShapeAssoc = hybridSuperClusters:hybridShapeAssoc
   InputTag endcapClusterShapeAssoc = islandBasicClusters:islandEndcapShapeAssoc
   InputTag evt = source 
   untracked double deltaCone = 0.5
   untracked string rootfile = "'''+out_rootfile_name+'''"

    PSet jetIdParameters = {
        string mcSource = "source"
        bool fillPartons = true
        bool fillHeavyHadrons = false
        bool fillLeptons =  false
        double coneSizeToAssociate = 0.3
        bool physicsDefinition = false
        bool rejectBCSplitting = false
        vstring vetoFlavour = {  }
    }
   
}

  path p = {cleanedJets,referenceTester}
}
#PG FINE CFG FILE
    '''

#--------------------------------------------------------------    
    
def removechars(string):
    """
    removes 1st last but one and last character
    """
    return string[1:-3]
    
#--------------------------------------------------------------
    
args=sys.argv

# check the command line and if wrong raise an error
if len(args)!=6:
    print 'Usage:\n'+\
          '%s CMSSW_dir rootfilelistfile basedir castor_dir queue\n' %args[0] +\
          'Example:\n'+\
          '%s ./CMSSW_1_2_3 mylist.txt /tmp 8nm\n' %args[0]
    raise "Wrong number of arguments"          

# relevant parameters
CMSSW_dir=args[1]              
listofrootfiles_file_name=args[2]           
tempdir=args[3]                   
#castordir=os.environ['CASTOR_HOME']+"/"+args[4]
castordir=args[4]
queue=args[5]

print "-"*20+'\n'+\
      'CMSSW_dir: %s\n' %CMSSW_dir +\
      'listofrootfiles_file_name: %s\n' %listofrootfiles_file_name +\
      'castordir: %s\n' %castordir +\
      'queue: %s\n' %queue



listofrootfiles_file=open(listofrootfiles_file_name,"r")
listofrootfiles=listofrootfiles_file.readlines()
listofrootfiles_file.close()

import re

# Loop on the content of the list of rootfiles.
# map applies element by element the function removechars to the list 
# listofrootfiles and returns the resulting list
for rootfile in map(removechars,listofrootfiles):
    basename = "_".join (re.match (".*/(.*)/(.*)[.]root",rootfile).group(1,2))
#    basename=os.path.basename(rootfile)[:-5] # chop ".root" extension
    out_rootfile_name=tempdir+"/"+basename+'_ntuple.root' # the full path!

#    "_".join (re.match (".*/(.*)/(.*)[.]root",rootfile).group(1,2))
    
    # Write cfgfile
    cfg_name="alcaElectrons_%s.cfg" %basename
    cfg_file=open(cfg_name,"w")
    cfg_file.write(cfgfile_text("rfio:"+rootfile,out_rootfile_name)) # See the function up in the code.. 
    cfg_file.close()
    
    # Write the script
    script_name="job_alcaElectrons_%s.sh" %basename
    script_file=open(script_name,"w")
    script_file.write('mkdir %s \n' %tempdir +\
                      'pushd %s \n' %CMSSW_dir +\
                      'eval `scramv1 run -sh`\n' +\
                      'popd\n' +\
                      'cmsRun %s/%s\n' % (os.environ['PWD'],cfg_name) +\
                      'rfcp %s %s\n' %(out_rootfile_name, castordir))
    script_file.close()
    # Make it executable
    os.system('chmod +x %s' %script_name)
    
    print 'script_name: %s' %script_name
    print 'config name: %s' %cfg_name
    
    # Submit!
    os.system('bsub -q %s -o /dev/null -u luca.mucibello@cern.ch %s' %(queue, script_name))
    

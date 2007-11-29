#bring in the cms configuration classes
import FWCore.ParameterSet.Config as cms

process = cms.Process('Dump')
process.extend(cms.include("FWCore/MessageLogger/data/MessageLogger.cfi"))

process.analysis = cms.EDAnalyzer('ditausAnalysis')
#process.analysis.srcLabel = cms.string('source') 
# evenctual output file, now hard-coded
#process.analysis.rootfile = cms.untracked(cms.string('miniTree.root'))

#eventual bad runs: uncomment and fill as you want
#bad = [ 1, 2]
bad = []

input_files = cms.vstring()
# loop from i = 1 to i < 100
for i in range(1,100):
        if i not in bad:
                input_files.append( 'rfio:/castor/cern.ch/user/d/deguio/HLT/Z_DiElectron_HLT/Z_DiElectron_HLT_%d.root' % i )


process.source = cms.Source("PoolSource",
                fileNames = cms.untracked( input_files ),
                maxEvents = cms.untracked(cms.int32(-1))
                )

process.p = cms.Path( process.analysis )


ofile = open( 'last_config_dump.log', 'w' )
ofile.write( process.dumpConfig() )
ofile.close()

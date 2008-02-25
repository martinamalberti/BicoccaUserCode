$SAMPLE = $ARGV[0] ;
$DATASETPATH = $ARGV[1] ;
#$CHANNEL = $ARGV[2] ;

$OUTROOTHISTOS =  "/gwtera2/users/tancini/WWF/CMSSW_1_6_8/src/HiggsAnalysis/VBFHiggsToWW2e/test/".$SAMPLE."_histos_ptMax.root" ;
$CONFIG = "/gwtera2/users/tancini/WWF/CMSSW_1_6_8/src/HiggsAnalysis/VBFHiggsToWW2e/test/VBFplots_ptMax".$SAMPLE.".cfg";

system("rm -f ".$CONFIG ) ;   
open (CONFIGNAME,">>".$CONFIG) or die "Cannot open ".$CONFIG." to write the config file" ;

#### writing the config file 

print CONFIGNAME "process VBFplots = {\n";

print CONFIGNAME "include \"Geometry/CMSCommonData/data/cmsIdealGeometryXML.cfi\"\n";
print CONFIGNAME "include \"Geometry/CaloEventSetup/data/CaloGeometry.cfi\"\n";
print CONFIGNAME "include \"SimGeneral/HepPDTESSource/data/pythiapdt.cfi\"\n";
print CONFIGNAME "include \"PhysicsTools/HepMCCandAlgos/data/genCandidates.cfi\"\n";
print CONFIGNAME "include \"PhysicsTools/HepMCCandAlgos/data/genParticleCandidatesFast.cfi\"\n";
print CONFIGNAME "include \"PhysicsTools/RecoCandAlgos/data/allTracks.cfi\"\n";
print CONFIGNAME "include \"Configuration/EventContent/data/EventContent.cff\"\n";
print CONFIGNAME "include \"HiggsAnalysis/VBFHiggsToWW2e/data/allEleIds.cfi\"\n";

print CONFIGNAME "include \"$DATASETPATH\"\n";

print CONFIGNAME "untracked PSet maxEvents = {untracked int32 input = -1}\n";
print CONFIGNAME "untracked PSet options = { untracked bool wantSummary = true }\n";

print CONFIGNAME "  service = TFileService \n";
print CONFIGNAME "    { \n";
print CONFIGNAME "       string fileName = \"$OUTROOTHISTOS \"\n";
print CONFIGNAME "    }\n";
 
print CONFIGNAME "service = MessageLogger {}\n";


# ---- VBFJetCleaner --------------------------------------

print CONFIGNAME "module jetCleaner = VBFJetCleaning\n";
print CONFIGNAME "{\n";
print CONFIGNAME "InputTag src  = iterativeCone5CaloJets\n";
#print CONFIGNAME "InputTag GSFInputTag = pixelMatchGsfElectrons\n";
print CONFIGNAME "InputTag GSFInputTag = refResolver\n";
print CONFIGNAME "double maxDeltaR = 0.3\n";
print CONFIGNAME "double minEleOJetEratio = 0\n"; 
print CONFIGNAME "double maxHEoverEmE = 1000\n";
print CONFIGNAME "}\n";


print CONFIGNAME "module jetUEPU = VBFJetEtaPtSelecting\n";
print CONFIGNAME "{\n";
print CONFIGNAME "    InputTag src  = jetCleaner\n";
#print CONFIGNAME "InputTag src  = iterativeCone5CaloJets\n";
print CONFIGNAME "    double maxEta = 5\n";
print CONFIGNAME "    double minPt = 15 # GeV\n";
print CONFIGNAME "}\n";


print CONFIGNAME "module tagJets = VBFJetTagger\n";
print CONFIGNAME "  {\n";
print CONFIGNAME "    InputTag jetInputTag  =  jetUEPU\n";
print CONFIGNAME "    string tagJetsName = \"tagJets\"\n";
print CONFIGNAME "    string otherJetsName = \"otherJets\"\n";
print CONFIGNAME "    double jetEtaMax = 5\n";
print CONFIGNAME "    double jetPtMin = 15 # GeV\n";
print CONFIGNAME "    double gatherConeSize = 0.5\n";
print CONFIGNAME "    int algoType = 1\n";
print CONFIGNAME "  }\n";

print CONFIGNAME "module trivialReader = VBFplots\n";
print CONFIGNAME "{\n";
print CONFIGNAME "  InputTag jetTagsInputTag = tagJets:tagJets\n";
print CONFIGNAME "  InputTag jetOthersInputTag = tagJets:otherJets\n";
print CONFIGNAME "  InputTag GSFInputTag  = pixelMatchGsfElectrons\n";
print CONFIGNAME "  InputTag eleIDInputTag  = electronId\n";
print CONFIGNAME "  InputTag muInputTag = muons\n";
print CONFIGNAME "  InputTag metInputTag = met\n";
print CONFIGNAME "}\n";


if ($SAMPLE eq "WWF")
{
    #print ("sample WWF with channel $CHANNEL\n") ;

    #print CONFIGNAME "module channelSelector = VBFMCChannelFilter\n";
    #print CONFIGNAME "{\n";
    #print CONFIGNAME "  InputTag MCtruthInputTag = genParticleCandidates\n";
    #print CONFIGNAME "  int32 channel = $CHANNEL  # 1 = uu, 2 = ee, 3 = ue\n";
    #print CONFIGNAME "  bool filter = true\n";
    #print CONFIGNAME "}\n";

    print CONFIGNAME "module my_VBFMCProcessFilter = VBFMCProcessFilter\n";
    print CONFIGNAME "{\n";
    print CONFIGNAME "untracked string moduleLabel = \"source\"\n";
    print CONFIGNAME "}\n";
    #print CONFIGNAME "path reading = {my_VBFMCProcessFilter & channelSelector & jetUEPU & tagJets & trivialReader}\n";
    print CONFIGNAME "path reading = {my_VBFMCProcessFilter & jetCleaner & jetUEPU & tagJets & trivialReader}\n";
}

elsif ($SAMPLE eq "ggF")
{
    #print ("sample ggF with channel $CHANNEL\n") ;

    #print CONFIGNAME "module channelSelector = VBFMCChannelFilter\n";
    #print CONFIGNAME "{\n";
    #print CONFIGNAME "  InputTag MCtruthInputTag = genParticleCandidates\n";
    #print CONFIGNAME "  int32 channel = $CHANNEL  # 1 = uu, 2 = ee, 3 = ue\n";
    #print CONFIGNAME "  bool filter = true\n";
    #print CONFIGNAME "}\n";

    print CONFIGNAME "module my_VBFMCProcessFilter = VBFMCProcessFilter\n";
    print CONFIGNAME "{\n";
    print CONFIGNAME "untracked string moduleLabel = \"source\"\n";
    print CONFIGNAME "}\n";
    #print CONFIGNAME "path reading = {!my_VBFMCProcessFilter &  channelSelector & jetUEPU & tagJets & trivialReader}\n";
    print CONFIGNAME "path reading = {!my_VBFMCProcessFilter & jetCleaner & jetUEPU & tagJets & trivialReader}\n";
} 

else
{
    print ("sample other\n") ;
    print CONFIGNAME "path reading = {jetCleaner & jetUEPU & tagJets & trivialReader}\n";
}

print CONFIGNAME "}\n";


################################################################################################

################################################################################################
################################################################################################

$SCRIPT = "lancia_$SAMPLE.csh";
system("rm -f ".$SCRIPT) ;
open (SCRIPTNAME,">>".$SCRIPT) or die "Cannot open ".$SCRIPT." to write the config file" ;

#### writing the config file

system("eval `scramv1 runtime -sh`");
#print SCRIPTNAME "source /linux/newCms/slc4_ia32_gcc345/lcg/root/5.14.00f-CMS2/etc/profile.d/init.csh\n";
#print SCRIPTNAME "eval `scramv1 runtime -csh`\n";
print SCRIPTNAME "cmsRun $CONFIG\n";

system ("qsub -V -q fastcms ".$SCRIPT);

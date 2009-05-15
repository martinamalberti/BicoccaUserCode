
#!/bin/bash

cd /afs/cern.ch/user/a/amassiro/scratch0/VBF/HWW2l2nu/CMSSW_2_2_10/src/HiggsAnalysis/VBFHiggsToWWto2l2nu/python/Data
eval `scramv1 runtime -sh`


rm SimpleTreeInput_NAME_cfi.py
rm ciccia_NAME*
rfdir /castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/NAME | awk '{print $9}' | grep VBFHWW2l2nuTest | awk '{print "echo *rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/NAME/" $1 "*? >> ciccia_NAME.txt"}' | /bin/sh
cat ./ciccia_NAME.txt | tr "?" "," | tr "\n" "&" | tr "*" "?" | sed -e s%?,\&?%?,\\n?%g | sed -e s%,\&%\ %g | tr "?" "'" > ./ciccia_NAME.txt.new
cat SimpleTreeInput_begin_cfi.py ciccia_NAME.txt.new SimpleTreeInput_end_cfi.py >> SimpleTreeInput_NAME_cfi.py
rm ciccia_NAME*


cd -
cp /afs/cern.ch/user/a/amassiro/scratch0/VBF/HWW2l2nu/CMSSW_2_2_10/src/HiggsAnalysis/VBFHiggsToWWto2l2nu/test/VBFHWW2l2nu_SimpleTree_Template_NAME_cfg.py ./
cmsRun VBFHWW2l2nu_SimpleTree_Template_NAME_cfg.py

rfcp VBF_SimpleTree_NAME.root /castor/cern.ch/user/a/amassiro/VBF/SimpleTree_skipBadFiles_JetCorrector_JetCleaning_090430_Everything_Skimmed_4Cluster_AllJets_MC_Charge/VBF_SimpleTree_NAME.root

rfcp VBFHWW2l2nu_SimpleTree_Template_NAME_cfg.py /castor/cern.ch/user/a/amassiro/VBF/SimpleTree_skipBadFiles_JetCorrector_JetCleaning_090430_Everything_Skimmed_4Cluster_AllJets_MC_Charge/VBFHWW2l2nu_SimpleTree_Template_NAME_cfg.py


ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

CXX           = g++
CXXFLAGS      = -g -fPIC -Wno-deprecated -O -ansi -D_GNU_SOURCE -g -O2
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared

ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)

ifeq ($(ARCH),macosx)
# MacOS X with cc (GNU cc 2.95.2 and gcc 3.3)
MACOSX_MINOR := $(shell sw_vers | sed -n 's/ProductVersion://p' | cut -d . -f 2)
CXX           = c++ -lm
CXXFLAGS      = -O2 -pipe -Wall -W -Woverloaded-virtual -fPIC -Wno-deprecated -O -ansi -D_GNU_SOURCE  -j 4
LD           = c++
LDFLAGS       = -O2 -g
# The SOFLAGS will be used to create the .dylib,
# the .so will be created separately
DllSuf        = dylib
ifeq ($(MACOSX_MINOR),4)
UNDEFOPT      = dynamic_lookup
LD            = MACOSX_DEPLOYMENT_TARGET=10.4 c++
else
ifeq ($(MACOSX_MINOR),3)
UNDEFOPT      = dynamic_lookup
LD            = MACOSX_DEPLOYMENT_TARGET=10.3 c++
else
UNDEFOPT      = suppress
LD            = c++
endif
endif

endif

CXXFLAGS      += $(ROOTCFLAGS)

NGLIBS         = $(ROOTGLIBS) 
# Hack here, because RooFit is removed from ROOT:
# NGLIBS        += -L/afs/cern.ch/cms/sw/slc4_ia32_gcc345/cms/cmssw/CMSSW_3_3_4/external/slc4_ia32_gcc345/lib/ -lMinuit -lRooFit
GLIBS          = $(filter-out -lNew, $(NGLIBS))

INCLUDEDIR       = ./
CXX	         += -I$(INCLUDEDIR)
# Hack here, because RooFit is removed from ROOT:
# CXX	         += -I/afs/cern.ch/cms/sw/slc4_ia32_gcc345/lcg/roofit/5.25.02-cms/include/
OUTLIB	         = ./lib/

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ../../../../../lib/

$(OUTLIB)smallHBaseClass.o: $(INCLUDEDIR)/smallHBaseClass.C $(INCLUDEDIR)/MuMuLooper.cc
	$(CXX) $(CXXFLAGS) -c -o $(OUTLIB)JPsiTreeBase.o $<

$(OUTLIB)MuMuLooper.o: $(INCLUDEDIR)/MuMuLooper.cc
	$(CXX) $(CXXFLAGS) -c -o $(OUTLIB)MuMuLooper.o $<

$(OUTLIB)MuMuCutOptimizer.o: $(INCLUDEDIR)/MuMuCutOptimizer.cc
	$(CXX) $(CXXFLAGS) -c -o $(OUTLIB)MuMuCutOptimizer.o $<

$(OUTLIB)EleEleLooper.o: $(INCLUDEDIR)/EleEleLooper.cc 
	$(CXX) $(CXXFLAGS) -c -o $(OUTLIB)EleEleLooper.o $<

$(OUTLIB)MakeElePlots.o: $(INCLUDEDIR)/MakeElePlots.cc 	 
	         $(CXX) $(CXXFLAGS) -c -o $(OUTLIB)MakeElePlots.o $<
        
$(OUTLIB)hChain.o: $(INCLUDEDIR)/hChain.cc 
	$(CXX) $(CXXFLAGS) -c -o $(OUTLIB)hChain.o $<

$(OUTLIB)h2Chain.o: $(INCLUDEDIR)/h2Chain.cc 	 
	         $(CXX) $(CXXFLAGS) -c -o $(OUTLIB)h2Chain.o $< 	 
	         
RunMuMu:  $(INCLUDEDIR)run_mumulooper.cpp $(OUTLIB)smallHBaseClass.o $(OUTLIB)MuMuLooper.o
	$(CXX) $(CXXFLAGS) -o RunMuMu $(OUTLIB)/*.o $(GLIBS) $ $<

RunMuMuOpt:  $(INCLUDEDIR)run_mumucutoptimizer.cpp $(OUTLIB)smallHBaseClass.o $(OUTLIB)MuMuCutOptimizer.o
	$(CXX) $(CXXFLAGS) -o RunMuMuOpt $(OUTLIB)/*.o $(GLIBS) $ $<

RunEleEle:  $(INCLUDEDIR)run_eleelelooper.cpp $(OUTLIB)smallHBaseClass.o $(OUTLIB)EleEleLooper.o $(OUTLIB)hChain.o
	$(CXX) $(CXXFLAGS) -o RunEleEle $(OUTLIB)/*.o $(GLIBS) $ $<

MakeElePlots:  $(INCLUDEDIR)run_makeeleplots.cpp $(OUTLIB)smallHBaseClass.o $(OUTLIB)MakeElePlots.o $(OUTLIB)hChain.o $(OUTLIB)h2Chain.o 	 
	         $(CXX) $(CXXFLAGS) -o MakeElePlots $(OUTLIB)/*.o $(GLIBS) $ $< 	 
	 
clean:
	rm -f $(OUTLIB)*.o $(OUTLIBCOMMON)*.o
	rm -f $(OUTLIB)*.so
	rm -f JPsiApp

all:  RunMuMu RunMuMuOpt RunEleEle MakeElePlots

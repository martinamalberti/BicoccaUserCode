#####################
#### README FILE ####

1) interface/ABCDAnalysis.h && src/ABCDanalysis.cc

   This class can create the ABCD analysis taking a tree of data,
   2 variable selected for the ABCD space, 2 cuts on them and 
   general cut for the analysis on the variables that I can chose.
   
   Is also possible to develope 3 statistic test to check if the pair is 
   really suitable to the application of the ABCD method (independent pair):
   Correlation factor and probability, Kolmogorov-Smirnov Test and Double-Ratio.


2) ABCDanalysis.cpp and ABCDAnalysis.cfg

   This program applies the ABCD analysis to a fixed background chosen and search the true
   pair of independent variables.
   They are given with their selection trough the cfg file.


   Template input:
       variable >/< value 
   [it's possible to use <, >, <=, >=]
   Leave spaces between fields!
   
   It's possible to define control regions in a composite way, 
     #######   #######
     ## A ##   ## B ##
     #######   #######
   leaving a "hole" in the middle.
   Simply select "value" in the middle of the two regions and then write in the "[General_Cut]" zone
   the phase space between A and B to be rejected.

   Example used:

       [variable]
       pT_RECO_l1 < 30
       pT_RECO_l2 >= 30
       [General_Cut]
       Mll>=10&&eta_RECO_l1_eta_RECO_l2>=0&& charge_RECO_l1_charge_RECO_l2==-1

   the results are saved in a TFile of output

   Usage example: 
       ./bin/ABCDanalysis.exe test/ABCDanalysis.cfg
   

 3)  Print_Histo_Cut.cxx

    This macro allows to visualize the results of the ABCD analysis (made by ABCDanalysis.cpp) making 3 Canvas: one for the separated ABCD areas,
    one for all the ABCD Space, and one for the Projected Histograms on which is made the Kolmogorov test.
    
    In General:    
        root test/Print_Histo_Cut.cxx\(\"Var1\",\"Var2\"\)
    Example:
        root test/Print_Histo_Cut.cxx\(\"pT_RECO_q1\",\"Mjj\"\)
        root test/Print_Histo_Cut.cxx\(\"CJV_30\",\"Mll\"\)
        
 4)  Print_Variable.cxx

    This macro allows to visualize the results of the ABCD search for independent variables (Make by ABCDanalysis.cpp) making 4 Canvas:
    one for the correlation coefficient, one for the correlation probabilities P(|r|<|r_{0}|),
    one for the double-ratio values and the last one for Kolmogorov test results.

    Clicking on any element of matrix, the previous macros, Print_Histo_Cut.cxx, is called for the couple selected.

    e.g.
        r00t test/Print_Variable.cxx
        
        
5) Background_Contamination.cpp and Background_Contamination.cfg

   This program takes as input the background chosen and the dataset for the signal and the other backgrounds.
   For a fixed integrated luminosity, the ABCD analysis is performed to check if the couple chosen, for the ABCD method
   application, guarantees low signal and background contaminations.
   Output in output/background/name_sample.root
   Usage example: 
       ./bin/Background_Contamination.exe test/Background_Contamination.cfg

6) Print_BackgroundContamination.cxx

   Print results of test (6) 
   e.g. 
       r00t test/Print_BackgroundContamination.cxx\(\"CJV_30\",\"Dphi_RECO_l12\",\"output/background/HistoABCD_TTbarJets_\"\)

7) ToyMC3.cpp and TotMC3.cfg

   This program takes as input the dataset for the background chosen, then it applies on its the ABCD analysis.
   After that a Monte-Carlo toy experiment is performed to simulate the results of other possible analysis and 
   investigate the trend of the estimator of statistical and systematic uncertainties as function of the integrated luminosity.
   (Fixed the optimal cuts on each variables and the independent pair chosen for the analysis)

   The pseudo-generations are realized based on:
    -> The number of total events, expected for a fixed luminosity, in the ABCD space is considered a Poissonian parameter.
       So it is generated N times according to this distribution.
    -> These events are then divided in ABCD regions following a quadrinomial distribution based on the Monte-Carlo efficiencies
       related to them.
  
  
  
  
  
    
#include "TH1.h"
#include "TF1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"






Double_t langaufun(Double_t *x, Double_t *par);

TF1 *langaufit(TH1F *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi,
               Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF);



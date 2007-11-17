def config_text(param_dict,def_value):
    '''
    Simple: returns the config with the params set in the dictionary.
    '''
    for key in param_dict.keys():
      if param_dict[key]==def_value:
        raise 'Not all the parameters were set!'
    return '''
# stream selected electrons 
#
#
process AlCaElectronsProductionIMAFiltering = 
{

# --- general parameters ------------------------------------------


  untracked PSet maxEvents = {untracked int32 input = -1}
  untracked PSet options = {
      untracked bool wantSummary = true  # default is false
  }


# --- the message logger ------------------------------------------


#    include "Calibration/EcalAlCaRecoProducers/test/MessageLoggerPG.cfg"


# --- leggo il file ------------------------------------------------


  source = PoolSource {
    untracked vstring fileNames =
    {
'rfio:/castor/cern.ch/user/b/beaucero/AlCaReco/Batchalcarecowith2Prod.root' 
    }
    untracked uint32 debugVerbosity = 1
    untracked bool   debugFlag     = true
    }


# --- electrons selection ------------------------------------------


  module etaFilter = EtaPtMinPixelMatchGsfElectronFullCloneSelector
    {
      InputTag src = electronFilter
      bool filter = true
      double ptMin = '''+param_dict['ptmin']+'''
      double etaMin = '''+param_dict['etamin']+'''
      double etaMax = '''+param_dict['etamax']+'''
    }

  module phiFiltering = PhiRangeSelector
    {
      InputTag src = etaFilter
      bool filter = true
      double phiMin = '''+param_dict['phimin']+'''
      double phiMax = '''+param_dict['phimax']+'''
    }

  module IMAFiltering = IMASelector     
    {
      InputTag src = phiFiltering 
      bool filter = true
      double ESCOPinMin = '''+param_dict['escopinmin']+'''
      double ESCOPinMax = '''+param_dict['escopinmax']+'''
      double ESeedOPoutMin = '''+param_dict['eseedopoutmin']+'''
      double ESeedOPoutMax = '''+param_dict['eseedopoutmax']+'''
      double PinMPoutOPinMin = '''+param_dict['pinmpoutpinmin']+'''
      double PinMPoutOPinMax = '''+param_dict['pinmpoutpinmax']+'''
    }

# --- results testing ------------------------------------------------

path withFilter = {etaFilter,phiFiltering,IMAFiltering}

# --- results testing ------------------------------------------------

  module out = PoolOutputModule
  {
     untracked string fileName ="'''+param_dict['outputfilename']+'''"
     untracked vstring outputCommands = 
     {
       "keep *"
#       "drop  *",
#       "keep  *_IMAFiltering_*_*",
#       "keep  *_alCaIsolatedElectrons_*_*"
     }
  } 

}
'''

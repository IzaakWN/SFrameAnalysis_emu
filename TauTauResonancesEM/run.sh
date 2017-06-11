#! /bin/bash

if make distclean && make; then
  #sframe_main config/WJetsToLNu.xml
  sframe_main config/DYJets_M-10to50.xml
  #sframe_main config/DYJets_M-50.xml
  #sframe_main config/DYJets_M-50_tes.xml
  #sframe_main config/Signal.xml
  #sframe_main config/TT_TuneCUETP8M1.xml
  #sframe_main config/SingleMuon.xml      #| tee SingleMuon.log
  #sframe_main config/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1.xml
fi

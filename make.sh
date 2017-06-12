for dir in BTaggingTools GoodRunsLists LepEff2016 NtupleVariables PileupReweightingTool SFrame SVFitTools TauTauResonancesEM RecoilCorrections JetCorrectionTool
do
    echo
    echo $dir
    echo
    cd $dir
    make distclean
    make
    cd -
done


loadLibs=[
    #"libGenVector",
    "libNtupleVariables",
    "libGoodRunsLists",
    "libPileupReweightingTool",
    "libBTaggingTools",
    "libTauTauResonancesEM",
    "libSVFitTool",
    "libScaleFactorTool",
    "libRecoilCorrector",
    "libJetCorrectionTool",
            ]

loadPacks=[
    "SFrameCore.par",
    "NtupleVariables.par",
    "GoodRunsLists.par",
    "PileupReweightingTool.par",
    "BTaggingTools.par",
    "TauTauResonancesEM.par",
    "SVFitTool.par",
    "ScaleFactorTool.par",
    "RecoilCorrector.par",
    "JetCorrectionTool.par",
	        ]

compilePacks=[
    "../NtupleVariables",
    "../GoodRunsLists",
    "../PileupReweightingTool",
    "../BTaggingTools",
    "../SVFitTool",
    "../TauTauResonancesEM",
    "../LepEff2016",
    "../RecoilCorrections",
            ]

AddUserItems = [
    ["RecoTreeName","tree"], 
    ["doSVFit","true"],
    ["doJEC","true"],
    ["OutputTreeName_emu", "tree_emu" ],
            ]
  
inputTrees=["ntuplizer/tree"]
outputTrees=["tree_emu"]

#End

path2xml="$SFRAME_DIR/../BatchSubmission/xmls_postMoriond_T2"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
jobName="DY"
outDir="/scratch/$USER/SFrameAnalysis/AnalysisOutputEM/"+jobName #"$SFRAME_DIR/../AnalysisOutput/"+jobName
cycleName="TauTauAnalysis"
nEventsMax=-1
#nEventsMax=200
nProcesses=1
nFiles=10
hCPU="04:00:00"
hVMEM="5000M"
postFix="_Moriond"
dataSets = [
        
       [ "DYJetsToLL_M-10to50_TuneCUETP8M1", # M-10to50
        [   "DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000_0.xml",
            "DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000_1.xml", ]],
         
       [ "DY1JetsToLL_M-10to50_TuneCUETP8M1", # M-10to50
        [   "DY1JetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000_0.xml",
            "DY1JetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000_1.xml", ]],
        
       [ "DY2JetsToLL_M-10to50_TuneCUETP8M1", # M-10to50
        [   "DY2JetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000.xml", ]],
       
       [ "DY3JetsToLL_M-10to50_TuneCUETP8M1", # M-10to50
        [   "DY3JetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000.xml", ]],
         
       [ "DYJetsToLL_M-50_TuneCUETP8M1",
        [   "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_additional_0000_0.xml",
            "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_additional_0000_1.xml",
            "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext_0000_0.xml",
            "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext_0000_1.xml",
            "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext_0000_2.xml", ]],
        
       [ "DY1JetsToLL_M-50_TuneCUETP8M1",
        [   "DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000_0.xml",
            "DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000_1.xml",
            "DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000_2.xml", ]],
        
       [ "DY2JetsToLL_M-50_TuneCUETP8M1",
        [   "DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000.xml", ]],
        
       [ "DY3JetsToLL_M-50_TuneCUETP8M1",
        [   "DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000.xml", ]],
        
       [ "DY4JetsToLL_M-50_TuneCUETP8M1",
        [   "DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0000.xml", ]],
             
            ]

userItems = [ 
                ["IsData","false"],
                ["IsSignal","false"],
                ["doRecoilCorr","true"],
                ["doTES","false"],
                ["TESshift","0.00"],
                ["doEES","false"],
                ["EESshift","0.00"],
                ["doLTF","false"],
                ["LTFshift","0.00"],
                ["doZpt","true"], # only to LO DY!
             ]

jobOptionsFile2=open("AnalysisOptionsEM.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
    if ("#E" + "nd") in i : break
    command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

path2xml="$SFRAME_DIR/../BatchSubmission/xmls_postMoriond_T2"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
jobName="WZ"
outDir="/scratch/$USER/SFrameAnalysis/AnalysisOutputEM/"+jobName #"$SFRAME_DIR/../AnalysisOutput/"+jobName
cycleName="TauTauAnalysis"
nEventsMax=-1
#nEventsMax=200
nProcesses=1
nFiles=2
hCPU="04:00:00"
hVMEM="5000M"
postFix="_Moriond"
dataSets = [

       [ "WZTo3LNu_TuneCUETP8M1_13TeV_nlo",
        [   "WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_0000.xml", ]],
                          
       [ "WZTo1L1Nu2Q_13TeV_nlo",
        [   "WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_0000_0.xml",
            "WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_0000_1.xml", ]],
                          
       [ "WZTo2L2Q_13TeV_nlo",
        [   "WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_0000_0.xml",
            "WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_0000_1.xml", ]],
        
       [ "WZJToLLLNu_nlo",
        [   "WZJToLLLNu_TuneCUETP8M1_13TeV-amcnlo-pythia8_0000.xml", ]],
        
            ]

userItems = [ 
                ["IsData","false"],
                ["IsSignal","false"],
                ["doRecoilCorr","false"],
                ["doEES","false"],
                ["EESshift","0.00"],
             ]

jobOptionsFile2=open("AnalysisOptionsEM.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
    if ("#E" + "nd") in i : break
    command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

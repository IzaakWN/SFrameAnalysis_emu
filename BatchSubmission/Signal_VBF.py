path2xml="$SFRAME_DIR/../BatchSubmission/xmls_postMoriond_T2"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
jobName="VBF"
outDir="/scratch/$USER/SFrameAnalysis/AnalysisOutputEM/"+jobName #"$SFRAME_DIR/../AnalysisOutput/"+jobName
cycleName="TauTauAnalysis"
nEventsMax=-1
#nEventsMax=200
nProcesses=1
nFiles=2
hCPU="03:30:00"
hVMEM="5000M"
postFix="_Moriond"
dataSets = [
            
        ["VBFHToTauTau_M125_13TeV_powheg_pythia8",
         [  "VBFHToTauTau_M125_13TeV_powheg_pythia8_0000.xml", ]]
         
    ]

userItems = [
                ["IsData","false"],
                ["IsSignal","true"],
                ["doRecoilCorr","True"],
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

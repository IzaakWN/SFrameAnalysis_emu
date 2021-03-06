path2xml="$SFRAME_DIR/../BatchSubmission/xmls_postMoriond_T2"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
jobName="VV"
outDir="/scratch/$USER/SFrameAnalysis/AnalysisOutputEM/"+jobName
cycleName="TauTauAnalysis"
nEventsMax=-1
#nEventsMax=200
nProcesses=1
nFiles=1
hCPU="07:00:00"
hVMEM="5000M"
postFix="_Moriond"
dataSets = [
    
   [ "VVTo2L2Nu_13TeV_nlo",
    [   "VVTo2L2Nu_13TeV_amcatnloFXFX_madspin_pythia8_0000.xml", ]],
    
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

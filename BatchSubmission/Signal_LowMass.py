path2xml="$SFRAME_DIR/../BatchSubmission/xmls_postMoriond"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
jobName="LowMass"
outDir="/scratch/$USER/SFrameAnalysis/AnalysisOutputEM/"+jobName #"$SFRAME_DIR/../AnalysisOutput/"+jobName
cycleName="TauTauAnalysis"
nEventsMax=-1
#nEventsMax=200
nProcesses=1
nFiles=15
hCPU="03:30:00"
hVMEM="5000M"
postFix="_Moriond"
dataSets = [
            
        ["LowMass_30GeV_DiTauResonance",
         [  #"LowMass_30GeV_TauTau_13TeV_RunIISpring16MiniAOD_v2_0.xml",
            #"LowMass_30GeV_TauTau_13TeV_RunIISpring16MiniAOD_v2_1.xml",
            #"LowMass_30GeV_TauTau_13TeV_RunIISpring16MiniAOD_v2_2.xml",
            "LowMass_30GeV_TauTau_13TeV_RunIISpring16MiniAOD_v2.xml", ]],
            
            ]


userItems = [
                ["IsData","false"],
                ["IsSignal","true"],
                ["doRecoilCorr","True"],
                ["doEES","false"],
                ["EESshift","0.00"],
                ["doTES","false"],
                ["TESshift","0.00"],
             ]

jobOptionsFile2=open("AnalysisOptionsEM.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
    if ("#E" + "nd") in i : break
    command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

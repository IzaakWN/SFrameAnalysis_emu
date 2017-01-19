path2xml="$SFRAME_DIR/../BatchSubmission/xmls_Izaak"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
jobName="WW"
outDir="/scratch/ineuteli/SFrameAnalysis/AnalysisOutput/"+jobName #"$SFRAME_DIR/../AnalysisOutput/"+jobName
cycleName="TauTauAnalysis"
nEventsMax=-1
#nEventsMax=200
nProcesses=1
nFiles=4
hCPU="03:30:00"
hVMEM="5000M"
postFix="_ICHEP" #"_ICHEP" #"_Summer2016"
dataSets = [
       [ "WW_TuneCUETP8M1",
        [   "WW_TuneCUETP8M1_13TeV-pythia8_HLT1_0000.xml", ]],
            ]
                            
if "Summer2016" in postFix: dataSets = [        
       [ "WWTo1L1Nu2Q_13TeV_nlo",
        [   "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0000_0.xml",
            "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0000_1.xml",
            "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0000_2.xml",
            "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0001_0.xml",
            "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0001_1.xml",
            "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0001_2.xml",
            "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0002_0.xml",
            "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_V2_0002_1.xml", ]],
            ]

userItems = [ 
                ["IsData","false"],
                ["IsSignal","false"],
                #["doSVFit","true"],
                ["doRecoilCorr","false"],
             ]

jobOptionsFile2=open("AnalysisOptions.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
    if ("#E" + "nd") in i : break
    command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

inputTrees=["ntuplizer/tree"]
outputTrees=["tree_mutau","tree_eletau"]

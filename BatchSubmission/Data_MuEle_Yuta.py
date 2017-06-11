path2xml="$SFRAME_DIR/../BatchSubmission/xmls_postMoriond"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
jobName="MuonEG"
outDir="/scratch/$USER/SFrameAnalysis/AnalysisOutputEM/"+jobName #"$SFRAME_DIR/../AnalysisOutputEM/"+jobName
cycleName="TauTauAnalysis"
nEventsMax=-1
#nEventsMax=200
nProcesses=1
nFiles=12
hCPU="04:00:00"
hVMEM="5000M"
postFix="_Moriond" #"_ICHEP"
dataSets = [
    
    ["MuonEG_B_Run2016",
     [
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0000_0.xml",
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0000_1.xml",
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0000_2.xml",
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0001_0.xml",
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0001_1.xml",
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0001_2.xml",
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0002_0.xml",
            "MuonEG_Run2016B-03Feb2017_ver2-v2MuonEG_BCD_03Feb2017_0002_1.xml",
      ]],
    
    ["MuonEG_C_Run2016",
     [
            "MuonEG_Run2016C-03Feb2017-v1MuonEG_BCD_03Feb2017_0000_0.xml",
            "MuonEG_Run2016C-03Feb2017-v1MuonEG_BCD_03Feb2017_0000_1.xml",
            "MuonEG_Run2016C-03Feb2017-v1MuonEG_BCD_03Feb2017_0000_2.xml",
      ]],
    
    ["MuonEG_D_Run2016",
     [
            "MuonEG_Run2016D-03Feb2017-v1MuonEG_BCD_03Feb2017_0000_0.xml",
            "MuonEG_Run2016D-03Feb2017-v1MuonEG_BCD_03Feb2017_0000_1.xml",
            "MuonEG_Run2016D-03Feb2017-v1MuonEG_BCD_03Feb2017_0000_2.xml",
            "MuonEG_Run2016D-03Feb2017-v1MuonEG_BCD_03Feb2017_0001_0.xml",
            "MuonEG_Run2016D-03Feb2017-v1MuonEG_BCD_03Feb2017_0001_1.xml",
      ]],
    
    ["MuonEG_E_Run2016",
     [
            "MuonEG_Run2016E-03Feb2017-v1MuonEG_EF_03Feb2017_0000_0.xml",
            "MuonEG_Run2016E-03Feb2017-v1MuonEG_EF_03Feb2017_0000_1.xml",
            "MuonEG_Run2016E-03Feb2017-v1MuonEG_EF_03Feb2017_0000_2.xml",
            "MuonEG_Run2016E-03Feb2017-v1MuonEG_EF_03Feb2017_0001.xml",
      ]],

    ["MuonEG_F_Run2016",
     [
            "MuonEG_Run2016F-03Feb2017-v1MuonEG_EF_03Feb2017_0000_0.xml",
            "MuonEG_Run2016F-03Feb2017-v1MuonEG_EF_03Feb2017_0000_1.xml",
            "MuonEG_Run2016F-03Feb2017-v1MuonEG_EF_03Feb2017_0000_2.xml",
            "MuonEG_Run2016F-03Feb2017-v1MuonEG_G_03Feb2017_0000.xml",
      ]],
    
    ["MuonEG_G_Run2016",
     [
            "MuonEG_Run2016G-03Feb2017-v1MuonEG_G_03Feb2017_0000_0.xml",
            "MuonEG_Run2016G-03Feb2017-v1MuonEG_G_03Feb2017_0000_1.xml",
            "MuonEG_Run2016G-03Feb2017-v1MuonEG_G_03Feb2017_0000_2.xml",
            "MuonEG_Run2016G-03Feb2017-v1MuonEG_G_03Feb2017_0001_0.xml",
            "MuonEG_Run2016G-03Feb2017-v1MuonEG_G_03Feb2017_0001_1.xml",
            "MuonEG_Run2016G-03Feb2017-v1MuonEG_G_03Feb2017_0001_2.xml",
            "MuonEG_Run2016G-03Feb2017-v1MuonEG_G_03Feb2017_0002.xml",

      ]],
    
    ["MuonEG_H_Run2016",
     [  
            "MuonEG_Run2016H-03Feb2017_ver2-v1MuonEG_H_03Feb2017_0000_0.xml",
            "MuonEG_Run2016H-03Feb2017_ver2-v1MuonEG_H_03Feb2017_0000_1.xml",
            "MuonEG_Run2016H-03Feb2017_ver2-v1MuonEG_H_03Feb2017_0000_2.xml",
            "MuonEG_Run2016H-03Feb2017_ver2-v1MuonEG_H_03Feb2017_0001_0.xml",
            "MuonEG_Run2016H-03Feb2017_ver2-v1MuonEG_H_03Feb2017_0001_1.xml",
            "MuonEG_Run2016H-03Feb2017_ver2-v1MuonEG_H_03Feb2017_0001_2.xml",
            "MuonEG_Run2016H-03Feb2017_ver2-v1MuonEG_H_03Feb2017_0002.xml",
            "MuonEG_Run2016H-03Feb2017_ver3-v1MuonEG_H_03Feb2017_0000.xml",
        ]],
    
    ]

userItems = [ 
                ["IsData","true"],
                ["IsSignal","false"],
                ["doRecoilCorr","false"],
             ]

jobOptionsFile2=open("AnalysisOptionsEM.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
    if ("#E" + "nd") in i : break
    command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

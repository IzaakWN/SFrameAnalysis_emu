#! /bin/bash

if [[ $1 == 1 ]]; then
    echo ">>> DY, ST, TT"
    python -u submitSFrame.py -j Background_DY.py --nosandbox --useEnv | tee nohup/nohup_DY.log
    python -u submitSFrame.py -j Background_ST.py --nosandbox --useEnv | tee nohup/nohup_ST.log
    python -u submitSFrame.py -j Background_WW.py --nosandbox --useEnv | tee nohup/nohup_WW.log
    python -u submitSFrame.py -j Background_WZ.py --nosandbox --useEnv | tee nohup/nohup_WZ.log
    python -u submitSFrame.py -j Background_ZZ.py --nosandbox --useEnv | tee nohup/nohup_ZZ.log
    python -u submitSFrame.py -j Signal_LowMass.py --nosandbox --useEnv | tee nohup/nohup_signal.log
elif [[ $1 == 2 || $1 == 3 ]]; then
    echo ">>> WJ, WW, WZ, ZZ, Signal, HTT"
    python -u submitSFrame.py -j Background_TT.py --nosandbox --useEnv | tee nohup/nohup_TT.log
    python -u submitSFrame.py -j Background_WJ.py --nosandbox --useEnv | tee nohup/nohup_WJ.log
    if [[ $1 == 3 ]]; then
        echo ">>> Data single muon"
        python -u submitSFrame.py -j Data_MuonEG.py  --nosandbox --useEnv | tee nohup/nohup_MuonEG.log
        #/scratch/ineuteli/SFrameAnalysis/AnalysisOutput/combineData.sh
    fi
    # python -u submitSFrame.py -j Signal_HTT.py         --nosandbox --useEnv | tee nohup/nohup_HTT.log
    # python -u submitSFrame.py -j Signal_HTT_TES1p03.py --nosandbox --useEnv | tee nohup/nohup_HTT_TES1p03.log
    # python -u submitSFrame.py -j Signal_HTT_TES0p97.py --nosandbox --useEnv | tee nohup/nohup_HTT_TES0p97.log
elif [[ $1 == 4 ]]; then
    PROCS="DY TT ST"
    for proc in $PROCS; do
        python -u submitSFrame.py -j Background_${proc}_EES1p01.py --nosandbox --useEnv | tee nohup/nohup_${PROC}_EES1p01.log
        python -u submitSFrame.py -j Background_${proc}_EES0p99.py --nosandbox --useEnv | tee nohup/nohup_${PROC}_EES0p99.log
    done
    python -u submitSFrame.py -j Signal_LowMass_EES0p99.py --nosandbox --useEnv | tee nohup/nohup_signal_EES0p99.log
    python -u submitSFrame.py -j Signal_LowMass_EES1p01.py --nosandbox --useEnv | tee nohup/nohup_signal_EES1p01.log
elif [[ $1 == 5 ]]; then
    PROCS="WJ WW WZ ZZ"
    for proc in $PROCS; do
        python -u submitSFrame.py -j Background_${proc}_EES0p99.py --nosandbox --useEnv | tee nohup/nohup_${PROC}_EES0p99.log
        python -u submitSFrame.py -j Background_${proc}_EES1p01.py --nosandbox --useEnv | tee nohup/nohup_${PROC}_EES1p01.log
    done
else
    echo ">>> Wrong input! Use 1-5."
fi
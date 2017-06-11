#alias sS='source ~/setup_Scripts/setup_SFrame.sh'
#alias cdB='cd ~/analysis/SFrameAnalysis/BatchSubmission' 

tmux new session \;\
  split-window -v \;\
  send-keys 'sS; cdp; echo "python plot.py -c 0"' C-m \;\
  split-window -h \;\
  send-keys 'sS; cdp; echo "python plot.py -c 1"' C-m \;\
  split-window -h \;\
  send-keys 'sS; cdp; echo "python plot.py -c 2"' C-m \;\
  select-pane -t 1 \;\
  select-layout even-horizontal \;


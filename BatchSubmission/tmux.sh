#! /bin/bash
tmux new session \;\
  split-window -v \;\
  send-keys 'sS $SFRAME; cd BatchSubmission; echo "./submit_Izaak.sh 1"' C-m \;\
  split-window -h \;\
  send-keys 'sS $SFRAME; cd BatchSubmission; echo "./submit_Izaak.sh 2"; echo "./submit_Izaak.sh 3"' C-m \;\

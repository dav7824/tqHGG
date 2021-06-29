# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Set command prompt
# Set color with \[\033[COLORm\]
export PS1="\[\033[36m\]\u\[\033[m\]@\[\033[32m\]\h:\[\033[33;1m\]\W\[\033[m\]\\$ "

# User specific aliases and functions

source /cvmfs/cms.cern.ch/cmsset_default.sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/mc_cheng/mytools/libevent-2.1.8-stable/lib
export PATH=$PATH:/home/mc_cheng/bin

alias tmux='/home/mc_cheng/mytools/tmux-2.8/bin/tmux'

export DIR_TQHGG=/wk_cms2/mc_cheng/FCNH/CMSSW_9_4_11/src/tqHGG
export DIR_PROD=/wk_cms2/mc_cheng/public/tqHGG

alias cdAnal='cd $DIR_TQHGG'
alias cdProd='cd $DIR_PROD'
alias goAnal='cdAnal && cmsenv'
alias goProd='cdAnal && cmsenv && cdProd'

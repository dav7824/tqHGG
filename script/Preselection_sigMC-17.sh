# argument 1: selection executable
# argument 2: name of previous selection step
# argument 3: name of current selection step

#!/bin/bash
set -o noglob

exe=$DIR_TQHGG/bin/$1
dir_in=/wk_cms2/mc_cheng/public/tqHGG/2017/$2/ntuple
dir_out=/wk_cms2/mc_cheng/public/tqHGG/2017/$3/ntuple
mkdir -p $dir_out

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $exe \
	$dir_in/$1.root \
	flashggStdTree \
	$dir_out/$1.root
}

set -e

Run TT_FCNC-TtoHJ_aThad_hct
Run TT_FCNC-TtoHJ_aThad_hut
Run TT_FCNC-TtoHJ_aTlep_hct
Run TT_FCNC-TtoHJ_aTlep_hut
Run TT_FCNC-aTtoHJ_Thad_hct
Run TT_FCNC-aTtoHJ_Thad_hut
Run TT_FCNC-aTtoHJ_Tlep_hct
Run TT_FCNC-aTtoHJ_Tlep_hut
Run ST_FCNC-TH_Thad_hct
Run ST_FCNC-TH_Thad_hut
Run ST_FCNC-TH_Tlep_hct
Run ST_FCNC-TH_Tlep_hut

exit 0

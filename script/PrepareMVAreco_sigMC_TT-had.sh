# argument 1: preselection to run the code on

#!/bin/bash
set -o noglob

indir=/wk_cms2/mc_cheng/public/tqHGG/2017/$1/ntuple
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/signal
mkdir -p $outdir

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    mkdir $outdir/$1
    $DIR_TQHGG/bin/PrepareMVAreco_sigMC_TT-had \
	$indir/$1.root \
	flashggStdTree \
	$outdir/$1
}

set -e

Run TT_FCNC-TtoHJ_aThad_hct
Run TT_FCNC-TtoHJ_aThad_hut
Run TT_FCNC-aTtoHJ_Thad_hct
Run TT_FCNC-aTtoHJ_Thad_hut

exit 0

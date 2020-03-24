#!/bin/bash
set -o noglob

indir=$DIR_PROD/2017/Preselection_4-had_unblind/ntuple
outdir=$DIR_PROD/2017/MVAreco_ST-had/training

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $DIR_TQHGG/bin/PrepareMVAreco_sigMC_ST-had \
	$indir/$1.root \
	flashggStdTree \
	$outdir/$1
}

set -e

Run ST_FCNC-TH_Thad_hct
Run ST_FCNC-TH_Thad_hut

exit 0

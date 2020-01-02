#!/bin/bash
set -o noglob

dir_in=/wk_cms2/youying/public/thFCNC/flashgg_105X/2017
dir_out=/wk_cms2/mc_cheng/public/tqHGG/2017/Preselection_1/ntuple
mkdir -p $dir_out

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $DIR_TQHGG/bin/Preselection_data_1 \
	$2 \
	flashggNtuples/flashggStdTree \
	$dir_out/$1.root
}

set -e

Run DoubleEG \
    $dir_in/DoubleEG.root,$dir_in/DoubleEGB.root,$dir_in/DoubleEGC.root,$dir_in/DoubleEGD.root,$dir_in/DoubleEGE.root,$dir_in/DoubleEGF1.root,$dir_in/DoubleEGF2.root

exit 0

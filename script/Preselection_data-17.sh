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

Run DoubleEG

exit 0

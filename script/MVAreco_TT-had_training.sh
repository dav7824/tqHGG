# argument 1 : setting version

#!/bin/bash
set -o noglob

indir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/TT_FCNC-TtoHJ_aThad_hct
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/TT_FCNC-TtoHJ_aThad_hct/setting_$1
exe=$DIR_TQHGG/bin/MVAreco_TT-had_training_$1
mkdir $outdir

set -e

g++ -o $exe $DIR_TQHGG/src/MVAreco_TT-had_training_$1.cc `root-config --cflags --libs` -lTMVA

cd $outdir
$exe \
	$indir \
	flashggStdTree \
	$outdir \
	| tee $outdir/MVAreco_TT-had_training.log

exit 0

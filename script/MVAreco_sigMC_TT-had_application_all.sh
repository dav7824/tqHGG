# input arguments
# 1. name of MVA method

#!/bin/bash
set -o noglob

sample=TT_FCNC-TtoHJ_aThad_hct
fin=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/$sample/mix_test.root
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/output/All/$1
mkdir $outdir
method=$1
weightfile=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/All/dataset/weights/MVAreco_TT-had_$1.weights.xml

set -e

$DIR_TQHGG/bin/MVAreco_sigMC_TT-had_application_all \
	$fin flashggStdTree $method $weightfile $outdir/$sample.root \
	| tee $outdir/MVAreco_TT-had_application.log

exit 0

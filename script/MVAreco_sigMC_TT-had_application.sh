# input arguments
# 1. version of MVA config
# 2. name of MVA method

#!/bin/bash
set -o noglob

sample=TT_FCNC-TtoHJ_aThad_hct
input=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/$sample/mix_test.root
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/output/$sample/setting_$1_$2
mkdir $outdir
method=$2
weightfile=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/$sample/setting_$1/dataset/weights/MVAreco_TT-had_$2.weights.xml

set -e

echo -e "\n\n\n===Start processing: $sample==="
$DIR_TQHGG/bin/MVAreco_sigMC_TT-had_application \
	$input \
	flashggStdTree \
	$method \
	$weightfile \
	$outdir/$sample.root \
	| tee $outdir/MVAreco_sigMC_TT-had_application.log

exit 0

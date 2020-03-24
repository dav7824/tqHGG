# input arguments
# 1. name of MVA method

#!/bin/bash
set -o noglob

sample=ST_FCNC-TH_Thad_hct
fin=$DIR_PROD/2017/MVAreco_ST-had/training/$sample/mix_test.root
outdir=$DIR_PROD/2017/MVAreco_ST-had/output/All/$1
mkdir $outdir
method=$1
weightfile=$DIR_PROD/2017/MVAreco_ST-had/training/All/dataset/weights/MVAreco_ST-had_$1.weights.xml

set -e

$DIR_TQHGG/bin/MVAreco_sigMC_ST-had_application_all \
	$fin flashggStdTree $method $weightfile $outdir/$sample.root \
	| tee $outdir/MVAreco_sigMC_ST-had_application_all.log

exit 0

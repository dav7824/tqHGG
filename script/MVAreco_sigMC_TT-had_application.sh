#!/bin/bash
set -o noglob

indir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/TT_FCNC-TtoHJ_aThad_hct
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/output

set -e

echo -e "\n\n\n===Start processing: TT_FCNC-TtoHJ_aThad_hct==="
$DIR_TQHGG/bin/MVAreco_sigMC_TT-had_application \
	$indir/mix_test.root \
	flashggStdTree \
	$outdir/TT_FCNC-TtoHJ_aThad_hct.root

exit 0

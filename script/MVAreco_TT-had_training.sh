# argument 1 : name of signal MC used for training

#!/bin/bash
set -o noglob

indir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/signal/$1
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/signal/$1

set -e

echo -e "\n\n\n===Start processing: $1==="
$DIR_TQHGG/bin/MVAreco_TT-had_training \
	$indir \
	flashggStdTree \
	$outdir

exit 0

# argument 1: tag of the MVA code

#!/bin/bash
set -o noglob

outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_ST-had/training/All
bindir=$DIR_TQHGG/bin
srcdir=$DIR_TQHGG/src
exe=MVAreco_ST-had_training_all_$1

set -e

g++ -o $bindir/$exe $srcdir/MVAreco_ST-had_training_all.cc `root-config --cflags --libs` -lTMVA
echo "Compilation completed"

cd $outdir
$bindir/$exe $1 \
	| tee $outdir/MVAreco_ST-had_training_all_$1.log

exit 0

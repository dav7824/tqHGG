#!/usr/bin/env bash

indir=/wk_cms2/youying/public/Era2017_RR-31Mar2018/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/output
files=$(ls $indir)

for file in $files; do
    ./addWeight $indir/$file
done

echo "Complete!"

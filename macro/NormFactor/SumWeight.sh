#!/usr/bin/env bash

# Calculate sum of weight for MC sample

indir=$1
fout=$2

# File path
dir_macro=$DIR_TQHGG/macro/NormFactor
macro_create=$dir_macro/CreateWeight.cc
exe_add=$DIR_TQHGG/bin/AddWeight

# Create empty histogram
root -l -b -q $macro_create\(\"$fout\"\)

# Add weight from each input file
files=$(ls $indir)
for file in $files; do
    $exe_add $fout $indir/$file
done

echo "Complete!"

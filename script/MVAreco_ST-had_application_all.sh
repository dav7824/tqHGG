# input arguments
# 1. name of MVA method

#!/bin/bash
set -o noglob

indir=$DIR_PROD/2017/MVAreco_ST-had/input
outdir=$DIR_PROD/2017/MVAreco_ST-had/output/All/$1
mkdir $outdir
method=$1
weightfile=$DIR_PROD/2017/MVAreco_ST-had/training/All/dataset/weights/MVAreco_ST-had_$1.weights.xml

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $DIR_TQHGG/bin/MVAreco_ST-had_application_all \
	$indir/$1.root \
	flashggStdTree \
	$method \
	$weightfile \
	$outdir/$1.root
}

set -e

RunAll() {
    Run DoubleEG

    Run DiPhotonJetsBox
    Run GJet_Pt-20to40_MGG-80toInf
    Run GJet_Pt-20toInf_MGG-40to80
    Run GJet_Pt-40toInf_MGG-80toInf
    Run QCD_Pt-30to40_MGG-80toInf
    Run QCD_Pt-30toInf_MGG-40to80
    Run QCD_Pt-40toInf_MGG-80toInf
    Run TGJets
    Run tW_antitop
    Run tW_top
    Run TTGG
    Run TTGJets
    Run TTJets
    Run TTWJetsToLNu
    Run TTZToLLNuNu
    Run DYJetsToLL
    Run WGToLNuG
    Run ZGToLLG
    Run WW
    Run WZ
    Run ZZ
    Run GluGluHToGG
    Run VBFHToGG
    Run VHToGG
    Run ttHJetToGG
    Run THQ_HToGG
    Run THW_HToGG
}

RunAll | tee $outdir/MVAreco_ST-had_application_all.log

exit 0

# input arguments
# 1. version of MVA config
# 2. name of MVA method

#!/bin/bash
set -o noglob

sample=TT_FCNC-TtoHJ_aThad_hct
indir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/input
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/output/$sample/setting_$1_$2
mkdir $outdir
method=$2
weightfile=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/$sample/setting_$1/dataset/weights/MVAreco_TT-had_$2.weights.xml

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $DIR_TQHGG/bin/MVAreco_TT-had_application \
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

RunAll | tee $outdir/MVAreco_TT-had_application.log

exit 0

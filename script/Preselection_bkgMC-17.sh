# argument 1: selection executable
# argument 2: name of previous selection step
# argument 3: name of current selection step

#!/bin/bash
set -o noglob

exe=$DIR_TQHGG/bin/$1
dir_in=/wk_cms2/mc_cheng/public/tqHGG/2017/$2/ntuple
dir_out=/wk_cms2/mc_cheng/public/tqHGG/2017/$3/ntuple
mkdir -p $dir_out

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $exe \
	$dir_in/$1.root \
	flashggStdTree \
	$dir_out/$1.root
}

set -e

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

exit 0

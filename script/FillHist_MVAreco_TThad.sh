# argument 1: MVA method
# argument 2: MVA score cut value

#!/bin/bash
set -o noglob

sample=TT_FCNC-TtoHJ_aThad_hct
cutval=$2
exe=$DIR_TQHGG/bin/FillHist_MVAreco_TThad
exe_sig=$DIR_TQHGG/bin/FillHist_sigMC_MVAreco_TThad
indir=/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/output/All/$1
outdir=$indir/hist_cut$cutval
mkdir $outdir

set -e

g++ -o $exe $DIR_TQHGG/src/FillHist_MVAreco_TThad.cc $DIR_TQHGG/src/utility.cc -I$DIR_TQHGG/include `root-config --cflags --libs`
g++ -o $exe_sig $DIR_TQHGG/src/FillHist_sigMC_MVAreco_TThad.cc $DIR_TQHGG/src/utility.cc -I$DIR_TQHGG/include `root-config --cflags --libs`
echo "Compilation completed"

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $exe $indir/$1.root flashggStdTree $outdir/$1.root $cutval
}
RunSig() {
    echo -e "\n\n\n===Start processing: $1 (unblinded)==="
    $exe_sig $indir/$1.root flashggStdTree $outdir/$1_unblind.root $cutval 0
    echo -e "\n\n\n===Start processing: $1 (blinded)==="
    $exe_sig $indir/$1.root flashggStdTree $outdir/$1_blind.root $cutval 1
}

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

    RunSig $sample
}

RunAll | tee $outdir/FillHist_MVAreco_TThad.log

exit 0

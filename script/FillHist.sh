# argument 1: name of the selection step for filling histograms

#!/bin/bash
set -o noglob

exe=$DIR_TQHGG/bin/FillHist
indir=/wk_cms2/mc_cheng/public/tqHGG/2017/$1/ntuple
outdir=/wk_cms2/mc_cheng/public/tqHGG/2017/$1/hist
mkdir -p $outdir

set -e

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $exe $indir/$1.root flashggStdTree $outdir/$1.root
}

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

Run TT_FCNC-TtoHJ_aThad_hct
Run TT_FCNC-TtoHJ_aThad_hut
Run TT_FCNC-TtoHJ_aTlep_hct
Run TT_FCNC-TtoHJ_aTlep_hut
Run TT_FCNC-aTtoHJ_Thad_hct
Run TT_FCNC-aTtoHJ_Thad_hut
Run TT_FCNC-aTtoHJ_Tlep_hct
Run TT_FCNC-aTtoHJ_Tlep_hut
Run ST_FCNC-TH_Thad_hct
Run ST_FCNC-TH_Thad_hut
Run ST_FCNC-TH_Tlep_hct
Run ST_FCNC-TH_Tlep_hut

exit 0

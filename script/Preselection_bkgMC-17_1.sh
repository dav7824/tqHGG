#!/bin/bash
set -o noglob

dir_in=/wk_cms2/youying/public/thFCNC/flashgg_105X/2017
dir_out=/wk_cms2/mc_cheng/public/tqHGG/2017/Preselection_1/ntuple
mkdir -p $dir_out

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $DIR_TQHGG/bin/Preselection_bkgMC_1 \
	$2 \
	flashggNtuples/flashggStdTree \
	$dir_out/$1.root
}

set -e

Run GJet_Pt-20to40_MGG-80toInf \
    $dir_in/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root
Run GJet_Pt-20toInf_MGG-40to80 \
    $dir_in/GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root
Run GJet_Pt-40toInf_MGG-80toInf \
    $dir_in/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root
Run QCD_Pt-30to40_MGG-80toInf \
    $dir_in/QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root
Run QCD_Pt-30toInf_MGG-40to80 \
    $dir_in/QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root
Run QCD_Pt-40toInf_MGG-80toInf \
    $dir_in/QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root
Run TGJets \
    $dir_in/TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root
Run tW_antitop \
    $dir_in/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root
Run tW_top \
    $dir_in/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root
Run TTGG \
    $dir_in/TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root
Run TTGJets \
    $dir_in/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root
Run TTJets \
    $dir_in/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8.root
Run TTWJetsToLNu \
    $dir_in/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root
Run TTZToLLNuNu \
    $dir_in/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8.root
Run DYJetsToLL \
    $dir_in/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8.root
Run WGToLNuG \
    $dir_in/WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root
Run ZGToLLG \
    $dir_in/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root
Run WW \
    $dir_in/WW_TuneCP5_13TeV-pythia8.root
Run WZ \
    $dir_in/WZ_TuneCP5_13TeV-pythia8.root
Run ZZ \
    $dir_in/ZZ_TuneCP5_13TeV-pythia8.root
Run GluGluHToGG \
    $dir_in/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root
Run VBFHToGG \
    $dir_in/VBFHToGG_M125_13TeV_amcatnlo_pythia8.root
Run VHToGG \
    $dir_in/VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root
Run ttHJetToGG \
    $dir_in/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root
Run THQ_HToGG \
    $dir_in/THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root
Run THW_HToGG \
    $dir_in/THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root

exit 0

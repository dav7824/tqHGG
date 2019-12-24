#!/bin/bash
set -o noglob

dir_in=/wk_cms2/youying/public/tH_FCNC/Era2017_RR-31Mar2018_v2
dir_out=/wk_cms2/mc_cheng/public/tqHGG/2017/Preselection_1_unblind/ntuple
mkdir -p $dir_out

Run() {
    echo -e "\n\n\n===Start processing: $1==="
    $DIR_TQHGG/bin/Preselection_sigMC_1_unblind \
	$dir_in/$2 \
	flashggNtuples/flashggStdTree \
	$dir_out/$1.root
}

set -e

Run TT_FCNC-TtoHJ_aThad_hct \
    TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hct-MadGraph5-pythia8.root
Run TT_FCNC-TtoHJ_aThad_hut \
    TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hut-MadGraph5-pythia8.root
Run TT_FCNC-TtoHJ_aTlep_hct \
    TT_FCNC-T2HJ_aTleptonic_HToaa_eta_hct-MadGraph5-pythia8.root
Run TT_FCNC-TtoHJ_aTlep_hut \
    TT_FCNC-TtoHJ_aTleptonic_HToaa_eta_hut-MadGraph5-pythia8.root
Run TT_FCNC-aTtoHJ_Thad_hct \
    TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hct-MadGraph5-pythia8.root
Run TT_FCNC-aTtoHJ_Thad_hut \
    TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hut-MadGraph5-pythia8.root
Run TT_FCNC-aTtoHJ_Tlep_hct \
    TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8.root
Run TT_FCNC-aTtoHJ_Tlep_hut \
    TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8.root
Run ST_FCNC-TH_Thad_hct \
    ST_FCNC-TH_Thadronic_HToaa_eta_hct-MadGraph5-pythia8.root
Run ST_FCNC-TH_Thad_hut \
    ST_FCNC-TH_Thadronic_HToaa_eta_hut-MadGraph5-pythia8.root
Run ST_FCNC-TH_Tlep_hct \
    ST_FCNC-TH_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8.root
Run ST_FCNC-TH_Tlep_hut \
    ST_FCNC-TH_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8.root

exit 0

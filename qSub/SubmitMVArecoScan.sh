#!/usr/bin/env bash
#
# Send TMVA DNN hyperparameter scanning PBS job with specified recotype, channel and parameter setting.
# The calculated matching rate result will be appended to MatchRate_<recotype><ch>.log.
#
# Also see the job-launching script, SubmitMVArecoScan.py.

#PBS -q cms
#PBS -V

# [Attention]
# (1) Provide jobname with qsub option -N when submitting.
# (2) Variables from job-sending env are available to the job by enabling option -V. (done in this script so no need to
#     use -V when submitting)
#     (The variables need to be exported in original env or they cannot be known by the job!)

# [Arguments of this script]  (Export them before submitting job!)
# <recotype>:    reconstruction type (TT|ST)
# <ch>:          channel (had|lep)
# <tag>:         name for mva config
# <opt_err>:     (TMVA DNN option) loss function
# <opt_layout>:  (TMVA DNN option) NN layout
# <opt_strat>:   (TMVA DNN option) training strategy for each phase
#
# More details on TMVA options can be found in the manual.

# Note:
# Originally tried to send the arguments for the job by option -v <var1=value1,...>, but some of them contain commas
# and make qsub unable to get the correct arguments.  Using -V can avoid this issue.
# For more details, see:
# https://arc.liv.ac.uk/trac/SGE/ticket/653


#if [[ "$recotype" != "TT" && "$recotype" != "ST" ]]; then
#	echo "[ERROR] Invalid recotype!"
#	exit 1
#fi
#if [[ "$ch" != "had" && "$ch" != "lep" ]]; then
#	echo "[ERROR] Invalid channel!"
#	exit 1
#fi

echo "[JOB] Hostname = `hostname`"

# Set directories
dir_tqHGG=/wk_cms2/mc_cheng/FCNH/CMSSW_9_4_11/src/tqHGG
exe_train=$dir_tqHGG/bin/MVAreco_train_scan
exe_app=$dir_tqHGG/bin/MVAreco_application
dir_prod=/wk_cms2/mc_cheng/public/tqHGG/2017
dir_job=$dir_prod/MVAreco_scan/${PBS_JOBNAME}

# Set env and go to the dir for this job. Exit with 256 if failing.
env_set=true
cd $dir_tqHGG && eval `scram runtime -sh` && cd $dir_job || env_set=false
if ! $env_set; then
	echo "[ERROR] Fail to set the environment!"
	exit 1
fi

# Set input files
set -f    # Disable globbing
if [[ "$ch" == "had" ]]; then
	dir_in=$dir_prod/MVArecoV2_Perm_had
else
	dir_in=$dir_prod/MVArecoV3_Perm_lep
fi
if [[ "$recotype" == "TT" ]]; then
	fin_train=TT_FCNC*
	fin_app="TT_FCNC-TtoHJ_aT${ch}_hct TT_FCNC-TtoHJ_aT${ch}_hut TT_FCNC-aTtoHJ_T${ch}_hct TT_FCNC-aTtoHJ_T${ch}_hut"
else
	fin_train=ST_FCNC*
	fin_app="ST_FCNC-TH_T${ch}_hct ST_FCNC-TH_T${ch}_hut"
fi

# Run training
# Note: The double qoutes around options are mandatory since they can contain characters that would be interpreted by shell in normal condition, e.g., "|".
echo "[MSG] Environment set!  Start training...  (`date`)"
# ./MVAreco_train_scan <fin> <outdir> <recotype=TT|ST> <channel=had|lep> <method_name> <opt:err> <opt:layout> <apt:train_strat>
$exe_train $dir_in/$fin_train $dir_job $recotype $ch "$recotype${ch}_$tag" "$opt_err" "$opt_layout" "$opt_strat" > ${PBS_JOBNAME}_train.log

# Test training results
set +f    # Enable globbing
fweight=$dir_job/dataset/weights/*.weights.xml
# If weight file doesn't exist, exit job with 256
if ! [ -a $fweight ]; then
	echo "[ERROR] Training failed!"
	exit 1
fi

# Run applying
echo -e "\n[MSG] Complete training!  Start applying...  (`date`)"
for file in $fin_app; do
	# ./MVAreco_application <fin> <fweight> <fout> <nt_type=sig|bkg> <reco_type=TT|ST> <channel=had|lep>
	echo "[MSG] Processing: $file"
	$exe_app $dir_in/$file.root $fweight $dir_job/$file.root sig $recotype $ch > ${PBS_JOBNAME}_application_${file}.log
done

# Record matching rates
result="$PBS_JOBNAME    $recotype${ch}_$tag    $opt_err  $opt_layout  $opt_strat"
for file in $fin_app; do
	rate=$(cat ${PBS_JOBNAME}_application_${file}.log | tail -1 | awk '{print $4}')
	result=$result"    $rate"
done
summary=$dir_prod/MVAreco_scan/MatchRate_$recotype$ch.txt
echo $result >> $summary

echo -e "\n[MSG] Job complete!  (`date`)"

exit 0

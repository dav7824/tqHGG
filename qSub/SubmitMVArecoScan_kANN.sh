#!/usr/bin/env bash
#
# Send TMVA ANN hyperparameter scanning PBS job with specified recotype, channel and parameter setting.
# The calculated matching rate result will be appended to MatchRate_<recotype><ch>.log.
#
# Also see the job-launching script, SubmitMVArecoScan_kANN.py.

#PBS -q cms
#PBS -V

# [Attention]
# (1) Provide jobname with qsub option -N when submitting.
# (2) Variables from job-sending env are available to the job by enabling option -V. (done in this script so no need to
#     use -V when submitting)
#     (The variables need to be exported in original env or they won't be known by the job!)

# [Arguments of this script]  (Export them before submitting job!)
# <recotype>:    reconstruction type (TT|ST)
# <ch>:          channel (had|lep)
# <tag>:         name for mva config
# <opt_Ncycle>:     (TMVA ANN option) number of training epochs
# <opt_layers>:     (TMVA ANN option) hidden layer structure
# <opt_err>:        (TMVA ANN option) loss function
# <opt_learn>:      (TMVA ANN option) learning rate
# <opt_decay>:      (TMVA ANN option) decay rate
# <opt_batch>:      (TMVA ANN option) batch size
# <opt_teststep>:   (TMVA ANN option) frequency of overtraining/convergence test
# <opt_conv>:       (TMVA ANN option) threshold of convergence improvement
# <opt_convstep>:   (TMVA ANN option) number of tests for convergence
#
# More details on TMVA options can be found in the manual.

# Note:
# Originally tried to send the arguments for the job by option -v <var1=value1,...>, but some of them contain commas
# and make qsub unable to get the correct arguments.  Using -V can avoid this issue.
# For more details, see:
# https://arc.liv.ac.uk/trac/SGE/ticket/653


#if [[ "$recotype" != "TT" && "$recotype" != "ST" ]]; then
#    echo "[ERROR] Invalid recotype!"
#    exit 1
#fi
#if [[ "$ch" != "had" && "$ch" != "lep" ]]; then
#    echo "[ERROR] Invalid channel!"
#    exit 1
#fi

echo "[JOB] Hostname = `hostname`"

# Set directories
dir_tqHGG=/wk_cms2/mc_cheng/FCNH/CMSSW_9_4_11/src/tqHGG
exe_train=$dir_tqHGG/bin/MVAreco_train_scan_kANN
exe_app=$dir_tqHGG/bin/MVAreco_application
dir_prod=/wk_cms2/mc_cheng/public/tqHGG/2017
dir_job=$dir_prod/MVAreco_scan_kANN/${PBS_JOBNAME}

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
$exe_train $dir_in/$fin_train $dir_job $recotype $ch "$recotype${ch}_$tag" "$opt_Ncycle" "$opt_layers" "$opt_err" "$opt_learn" "$opt_decay" "$opt_batch" "$opt_teststep" "$opt_conv" "$opt_convstep" > ${PBS_JOBNAME}_train.log

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
    echo "[MSG] Processing: $file"
    $exe_app $dir_in/$file.root $fweight $dir_job/$file.root sig $recotype $ch > ${PBS_JOBNAME}_application_${file}.log
done

# Record matching rates
result="$PBS_JOBNAME    $recotype${ch}_$tag    "
for file in $fin_app; do
    rate=$(cat ${PBS_JOBNAME}_application_${file}.log | tail -1 | awk '{print $4}')
    result=$result"\t$rate"
done
summary=$dir_prod/MVAreco_scan_kANN/MatchRate_$recotype$ch.txt
echo -e $result >> $summary

echo -e "\n[MSG] Job complete!  (`date`)"

exit 0

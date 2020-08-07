#!/usr/bin/env python2
# Usage:
#   ./MVAreco_training.py <train_tag> <run_qsub=0|1>

import Path
import Util
import os
import sys

def RunTraining(indir, fin, nt_type, channel):
	cmd = cmd_.format( indir, fin, outdir, nt_type, channel, train_tag )
	print 'Running:', cmd
	cmd = 'set -o noglob; cd {} && '.format(outdir) + cmd
	if run_qsub:
		jobname = '_'.join( [train_tag, nt_type, channel] )
		cmd = '"'+cmd+'"'
		os.system( Path.dir_tqHGG + '/qSub/submitJOB.py -c {} -N {}'.format(cmd, jobname) )
	else:
		os.system( cmd )

# Command line arguments
train_tag = sys.argv[1]
run_qsub = sys.argv[2]
# Names of input dirs
had_name = 'had_btag-M_phID'
lep_name = 'lep_btag-L_phID'

# Executable
exe = Path.dir_bin + '/MVAreco_training'
# Input directories
indir_had = Path.dir_2017 + '/MVAreco_perm_' + had_name
indir_lep = Path.dir_2017 + '/MVAreco_perm_' + lep_name
# Input root files
fin_TT = 'TT_FCNC*'
fin_ST = 'ST_FCNC*'
# Output directory
outdir = Path.dir_2017 + '/MVAreco_training'
Util.CreateDir(outdir)

# Command template
cmd_ = exe + ' {}/{} {} {} {} {}'

# Run the executable
RunTraining( indir_had, fin_TT, 'TT', 'had' )
RunTraining( indir_had, fin_ST, 'ST', 'had' )
RunTraining( indir_lep, fin_TT, 'TT', 'lep' )
RunTraining( indir_lep, fin_ST, 'ST', 'lep' )

print 'Completed!'

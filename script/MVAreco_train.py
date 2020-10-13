#!/usr/bin/env python2
#
# Run MVA reconstruction training on signal MC permutation trees.
# Usage:
#   ./MVAreco_train.py <train_tag>

import Path, Util, Samples
import os, sys
from os.path import join

def RunTrain(cmd, jobname):
    print '---Start running job: {}'.format(jobname)
    fp = os.popen( cmd )
    out = open( join(outdir,'log',jobname+'.log'), 'w' )
    out.write(fp.read())
    fp.close()
    out.close()
    print '---Complete job: {}'.format(jobname)

# Name tag for TMVA factory root file and job names
train_tag = sys.argv[1]
# Set whether to run qsub
run_qsub = True

# Set executable name
exe_name = 'MVAreco_train_{}'.format(train_tag)
exe = join(Path.dir_bin, exe_name)

# Set I/O directories
indir_had = join(Path.dir_2017, 'MVAreco_Perm_had_v2')
indir_lep = join(Path.dir_2017, 'MVAreco_Perm_lep_v2')
outdir = join(Path.dir_2017, 'MVAreco_train_v2')
Util.CreateDir(outdir)

# Generate source code
Util.GenCode_MVAreco_train(train_tag)
# Compile source code
os.system('rm {}'.format(exe))
os.system('cd {}; make bin/{}'.format(Path.dir_tqHGG, exe_name))
if not os.path.exists(exe):
    print '[ERROR] Compilation failed!'
    sys.exit(1)

# Command template
cmd_train = 'set -o noglob; cd {outdir}; {bin} {{fin}} . {{recotype}} {{ch}} {tag}'.format(bin=exe, outdir=outdir, tag=train_tag)

cmd_TThad = cmd_train.format(fin=join(indir_had,'TT_FCNC*'), recotype='TT', ch='had')
cmd_SThad = cmd_train.format(fin=join(indir_had,'ST_FCNC*'), recotype='ST', ch='had')
cmd_TTlep = cmd_train.format(fin=join(indir_lep,'TT_FCNC*'), recotype='TT', ch='lep')
cmd_STlep = cmd_train.format(fin=join(indir_lep,'ST_FCNC*'), recotype='ST', ch='lep')

# Run training
if run_qsub:
    Util.submit_job( cmd_TThad, 'TThad_'+train_tag )
    #Util.submit_job( cmd_SThad, 'SThad_'+train_tag )
    #Util.submit_job( cmd_TTlep, 'TTlep_'+train_tag )
    #Util.submit_job( cmd_STlep, 'STlep_'+train_tag )
else:
    Util.CreateDir(join(outdir,'log'))
    RunTrain( cmd_TThad, 'TThad_'+train_tag )
    RunTrain( cmd_SThad, 'SThad_'+train_tag )
    RunTrain( cmd_TTlep, 'TTlep_'+train_tag )
    RunTrain( cmd_STlep, 'STlep_'+train_tag )

print 'End!'

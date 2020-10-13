#!/usr/bin/env python2
#
# Run MVA reconstruction training on signal MC permutation trees.
# This version of MVAreco_train script can be used when each recotype & channel needs different config for TMVA factory.
#
# Usage:
#   ./MVAreco_train_v2.py <recotype> <channel> <train_tag>

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

# Reconstruction type
recotype = sys.argv[1]
# Channel
ch = sys.argv[2]
# Name tag for TMVA factory root file and job names
train_tag = sys.argv[3]
# Set whether to run qsub
run_qsub = True

if recotype!='TT' and recotype!='ST':
    print '[ERROR] Invalid reconstruction type!'
    sys.exit(1)
if ch!='had' and ch!='lep':
    print '[ERROR] Invalid channel!'
    sys.exit(1)

# Set executable name
exe_name = 'MVAreco_train_{}{}_{}'.format(recotype, ch, train_tag)
exe = join(Path.dir_bin, exe_name)

# Set I/O directories
indir_had = join(Path.dir_2017, 'MVArecoV2_Perm_had')
indir_lep = join(Path.dir_2017, 'MVArecoV3_Perm_lep')
outdir = join(Path.dir_2017, 'MVArecoV3_train')
Util.CreateDir(outdir)

# Generate source code
fmethod = join(Path.dir_src, 'MVAreco_methods.cc')
src = join(Path.dir_tqHGG, 'tmp', exe_name+'.cc')
Util.GenCode_MVAreco_train(fmethod, src)
# Compile source code
os.system('rm {}'.format(exe))
os.system('cd {}; make bin/{}'.format(Path.dir_tqHGG, exe_name))
if not os.path.exists(exe):
    print '[ERROR] Compilation failed!'
    sys.exit(1)

# Command template
# ./MVAreco_train_xxx <fin> <outdir> <recotype=TT|ST> <channel=had|lep> <train_tag>
cmd_t = 'set -o noglob; cd {outdir}; {bin} {{fin}} . {{recotype}} {{ch}} {tag}'.format(bin=exe, outdir=outdir, tag=train_tag)
indir_run = ''
fin_run = ''

# Setting file name & dir name according to recotype & channel
if recotype=='TT':
    fin_run = 'TT_FCNC*'
elif recotype=='ST':
    fin_run = 'ST_FCNC*'
if ch=='had':
    indir_run = indir_had
elif ch=='lep':
    indir_run = indir_lep

# Complete command
cmd_run = cmd_t.format( fin=join(indir_run,fin_run), recotype=recotype, ch=ch )

# Run training
jobname = recotype+ch+'_'+train_tag
if run_qsub:
    Util.submit_job( cmd_run, jobname )
else:
    Util.CreateDir(join(outdir,'log'))
    RunTrain( cmd_run, jobname )

print 'End!'

#!/usr/bin/env python2
#
# Apply MVA methods to signal false permutations.
#
# Usage:
#   ./MVAreco_application_FalseSig.py <channel> <train_tag>

import Path, Util, Samples
import os, sys
from os.path import join, exists

# Debug
dbg_cmd = False

# Channel
ch = sys.argv[1]
# Train tag
train_tag = sys.argv[2]

def Process(cmd, log_):
    log = open(log_, 'w')
    fp = os.popen(cmd)
    log.write(fp.read())
    fp.close()
    log.close()

# Directory names
indir_name = ''
outdir_name = ''
weidir_name = ''
if ch=='had':
    indir_name = 'MVArecoV2_Perm_had'
    outdir_name = 'MVArecoV2_result_had'
    weidir_name = 'MVAreco_train'
elif ch=='lep':
    indir_name = 'MVArecoV3_Perm_lep'
    outdir_name = 'MVArecoV3_result_lep'
    weidir_name = 'MVAreco_train'
else:
    print '[ERROR] Invalid channel!'
    sys.exit(1)

# Set directories
indir = join(Path.dir_2017, indir_name)
outdir = join(Path.dir_2017, outdir_name, train_tag)
weidir = join(Path.dir_2017, weidir_name, 'dataset/weights')
logdir = join(outdir, 'log')
if not (exists(indir) and exists(outdir) and exists(weidir) and exists(logdir)):
    print '[ERROR] Incorrect path'
    sys.exit(1)

# Executable
exe = join(Path.dir_bin, 'MVAreco_application')

# Weight file
fweight = '{{reco}}{ch}_{tag}.weights.xml'.format(ch=ch, tag=train_tag)

# Command template
cmd_t = '{bin} {indir}/{{nt}}.root {fweight} {outdir}/{{nt}}.root bkg {{reco}} {ch}'.format(
        bin=exe, indir=indir, fweight=join(weidir,fweight), outdir=outdir, ch=ch)

reco_false = {'TT':'ST', 'ST':'TT'}

# Run
for recotype in reco_false:
    reco = reco_false[recotype]
    for nt in Samples.sig_MC_expr_v2[(recotype, ch)]:
        cmd = cmd_t.format(nt=nt, reco=reco)
        log = '{}/{}__{}.log'.format(logdir, nt, reco)
        if dbg_cmd:
            print '[Run]', cmd
            print '[Log]', log
            print ''
        else:
            print 'Processing:', nt
            Process( cmd, log )

print 'Complete!'



'''
[Note]

hadronic samples:
- Samples.sig_MC_expr_v2[('TT','had')]
  + inTree: TPerm_ST
  + outTree: Treco_ST
- Samples.sig_MC_expr_v2[('ST','had')]
  + inTree: TPerm_TT
  + outTree: Treco_TT

leptonic samples:
- Samples.sig_MC_expr_v2[('TT','lep')]
  + inTree: TPerm_ST
  + outTree: Treco_ST
- Samples.sig_MC_expr_v2[('ST','lep')]
  + inTree: TPerm_TT
  + outTree: Treco_TT

input dir:
- Had: MVArecoV2_Perm_had
- Lep: MVArecoV3_Perm_lep

output dir:
- Had: MVArecoV2_result_had/${train_tag}
- Lep: MVArecoV3_result_lep/${train_tag}

Weight file:
- Had: MVArecoV2_train/dataset/weights/
     + TThad_${train_tag}.weights.xml
     + SThad_${train_tag}.weights.xml
- Lep: MVArecoV3_train/dataset/weights/
     + TTlep_${train_tag}.weights.xml
     + STlep_${train_tag}.weights.xml

command:
- TThad sample ST reco:
    ./MVAreco_application ${indir_had}/${TThad_sample} ${weidir_had}/${SThad_fweight} ${outdir_had}/${TThad_sample} bkg ST had
- SThad sample TT reco:
    ./MVAreco_application ${indir_had}/${SThad_sample} ${weidir_had}/${TThad_fweight} ${outdir_had}/${SThad_sample} bkg TT had
- TTlep sample ST reco:
    ./MVAreco_application ${indir_lep}/${TTlep_sample} ${weidir_lep}/${STlep_fweight} ${outdir_lep}/${TTlep_sample} bkg ST lep
- STlep sample TT reco:
    ./MVAreco_application ${indir_lep}/${STlep_sample} ${weidir_lep}/${TTlep_fweight} ${outdir_lep}/${STlep_sample} bkg TT lep
'''

#!/usr/bin/env python2
#
# Apply trained MVA to event permutations.
# This version of MVAreco_application applies on single signal type (recotype x channel).
#
# Usage:
#   ./MVAreco_application_v2.py <recotype> <channel> <train_tag>
#
# <train_tag>: Name tag for MVA method

import Path, Util, Samples
import sys, os
from os.path import join, exists

def RunApp(cmd, log):
	out = open(log, 'w')
	fp = os.popen( cmd )
	out.write( fp.read() )
	out.close()
	fp.close()

# Reconstruction type
recotype = sys.argv[1]
# Channel
ch = sys.argv[2]
# Name tag for the MVA (used to specify the weight file and as part of the output directory names)
# Should be the same as the "method name" specified when booking MVA in training code, so this script
# can find the correct weight files.
train_tag = sys.argv[3]

if recotype!='TT' and recotype!='ST':
	print '[ERROR] Invalid reconstruction type!'
	sys.exit(1)
if ch!='had' and ch!='lep':
	print '[ERROR] Invalid channel!'
	sys.exit(1)

# Executable
exe = join(Path.dir_bin, 'MVAreco_application')

# Input dir
indir = ''
if ch=='had':
	indir = join(Path.dir_2017, 'MVArecoV2_Perm_had')
elif ch=='lep':
	indir = join(Path.dir_2017, 'MVArecoV2_Perm_lep')
# Output dir
outdir = ''
logdir = ''
if ch=='had':
	outdir = join(Path.dir_2017, 'MVArecoV2_result_had', train_tag)
elif ch=='lep':
	outdir = join(Path.dir_2017, 'MVArecoV2_result_lep', train_tag)
logdir = join(outdir, 'log')
Util.CreateDir(outdir)
Util.CreateDir(logdir)
# Weight file dir
weidir = join(Path.dir_2017, 'MVArecoV2_train/dataset/weights')

# Weight file
fweight = '{recotype}{ch}_{tag}.weights.xml'.format(recotype=recotype, ch=ch, tag=train_tag)

# Command template
#./MVAreco_application <fin> <fweight> <fout> <nt_type=sig|bkg> <reco_type=TT|ST> <channel=had|lep>
cmd_t = '{bin} {indir}/{{nt}}.root {fweight} {outdir}/{{nt}}.root {{nt_type}} {recotype} {ch}'.format(
		bin=exe, indir=indir, fweight=join(weidir,fweight), outdir=outdir, recotype=recotype, ch=ch)
# Log file template
log_t = '{dir}/{{nt}}__{recotype}.log'.format(dir=logdir, recotype=recotype)


# Run reconstruction
print '[INFO] Apply MVA %s%s_%s' % (recotype, ch, train_tag)
for nt in Samples.sig_MC_expr_v2[(recotype,ch)]:
	print 'Processing:', nt
	RunApp(cmd_t.format(nt=nt,nt_type='sig'), log_t.format(nt=nt))
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunApp(cmd_t.format(nt=nt,nt_type='bkg'), log_t.format(nt=nt))
print 'Processing: Data'
RunApp(cmd_t.format(nt='data',nt_type='bkg'), log_t.format(nt='data'))


print 'End!'

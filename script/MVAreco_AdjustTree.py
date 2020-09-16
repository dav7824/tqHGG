#!/usr/bin/env python2
#
# For further info, see src/MVAreco_AdjustTree.cc.
# Usage:
#   ./MVAreco_AdjustTree.py <train_tag>

import Path, Util, Samples
import sys, os
from os.path import join, exists

def Process(nt):
	print 'Processing:', nt
	fp = os.popen( cmd_t.format(nt=nt) )
	fp.close()

# Train tag
train_tag = sys.argv[1]

# I/O dirs
indir = join(Path.dir_2017, 'MVArecoV2_result_had', train_tag)
outdir = join(Path.dir_2017, 'MVArecoV3_result_had', train_tag)
if not exists(indir):
	print '[ERROR] Input dir does not exist!'
	sys.exit(1)
Util.CreateDir(outdir)

# Executable
exe = join(Path.dir_bin, 'MVAreco_AdjustTree')

# Command tmeplate
cmd_t = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root'.format(bin=exe, indir=indir, outdir=outdir)

# Process signal
for recotype in ['TT', 'ST']:
	for nt in Samples.sig_MC_expr_v2[(recotype, 'had')]:
		Process(nt)
# Process bkg
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		Process(nt)
# Process data
Process('data')

print 'Complete!'

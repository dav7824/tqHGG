#!/usr/bin/env python2
#
# For further info, see src/MVAreco_AdjustTree.cc.
# Usage:
#   ./MVAreco_AdjustTree.py <train_tag>

import Path, Util, Samples
import sys, os
from os.path import join, exists

def RunProcess(nt, recotype):
	fp = os.popen( cmd_t.format(nt=nt, recotype=recotype) )
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
cmd_t = '{bin} {indir}/{{nt}}.root {outdir}/{{nt}}.root {{recotype}}'.format(bin=exe, indir=indir, outdir=outdir)

# Process TT had signal
for nt in Samples.sig_MC_expr_v2[('TT', 'had')]:
	print 'Processing:', nt
	RunProcess(nt, 'TT')
# Process ST had signal
for nt in Samples.sig_MC_expr_v2[('ST', 'had')]:
	print 'Processing:', nt
	RunProcess(nt, 'ST')
# Process bkg
for cat in Samples.bkg_MC_s:
	for nt in Samples.bkg_MC_s[cat]:
		print 'Processing:', nt
		RunProcess(nt, 'TT')
		RunProcess(nt, 'ST')
# Process data
print 'Processing:', 'data'
RunProcess('data', 'TT')
RunProcess('data', 'ST')

print 'Complete!'
